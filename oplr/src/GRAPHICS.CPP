// GRAPHICS.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#include "graphics.h"
#include <oplr.h>
#include <s32file.h> 			 	 
#include <f32file.h> 
#if !defined(__SERIES60__)
#include <fontids.h>
#endif
#include "oplutil.h"			 	 
#include <eiklabel.h>
#include <clock.h>
#include <gulutil.h>
#include <eikon.hrh>
#include <eikon.rsg>
#include "clckupdt.h"

// const TInt KSinglePixelGap = 0x100;  not supported by wserv
const TInt KSinglePixelShadow = 1;
const TInt KGapForSinglePixelShadow = 2;
const TInt KDoublePixelShadow = 3;
const TInt KGapForDoublePixelShadow = 4;
const TInt KFontIdOffset = 0x1000;

const TInt KNormalCornerSize = 4;
const TInt KMoreRoundedCornerSize = 6;


/*****************************
class CDrawablesCollection
*****************************/

CDrawablesCollection::CDrawablesCollection(RWsSession& aWsSession) 
: iFlushState(ETrue) ,iWsSession(aWsSession)
	{
	}

CDrawablesCollection::~CDrawablesCollection()
	{
	COplDrawable** drawablePtr = iDrawablesArray;
	for (TInt index=0; index < KMaxDrawables; index++,drawablePtr++)
		{
		delete (*drawablePtr);
		}
	}

void CDrawablesCollection::SetConsoleWinL(COplRuntime& aRuntime, COplConsole* aConsole)
	{
	COplDrawable* window = new(ELeave) COplWindow;
	CleanupStack::PushL(window);
	TInt color=0;
	TInt gray=0;
	TDisplayMode dMode=aRuntime.ConEnv()->WsSession().GetDefModeMaxNumColors(color,gray);
	((COplWindow*)window)->ConstructL(aRuntime,aConsole->Window(),dMode);
//	((COplWindow*)window)->ConstructL(aRuntime,aConsole->Window(),EGray4);
	AddDrawableL(window,1);	// first drawable
	iDrawableWithCursor = 0; // the console gets the cursor
	CleanupStack::Pop();
	}

TInt CDrawablesCollection::NextAvailableId()
	{
	TInt found = -1;
	for (TInt index=0; index<KMaxDrawables; index++)
		{
		if (iDrawablesArray[index] == NULL)
			{
			found = index;
			break;
			}
		}
	if (found < 0)
		User::Leave(KOplErrMaxDraw);
	return (found+KDiffBtnIdAndPos);
	}

void CDrawablesCollection::AddDrawableL(COplDrawable* aDrawable, TInt aDrawableId)
  // returns id of drawable in array
	{
	if (iNumOfDrawables == KMaxDrawables)
		User::Leave(KOplErrMaxDraw);
	iDrawablesArray[aDrawableId-KDiffBtnIdAndPos] = aDrawable;
	iNumOfDrawables++;
	iCurrentDrawable = aDrawable;
	}

void CDrawablesCollection::CloseDrawableL(TInt aId)
	// note the aId will never be 1 as it is already handled before this function call
	{
	COplDrawable* drawable = DrawableL(aId);
	if (DrawableWithCursor()==aId)
		SetDrawableWithCursor(-1);
	if (iCurrentDrawable == drawable)	// if drawable to close = current drawable
		iCurrentDrawable = iDrawablesArray[0];	// this implies the console becomes the current drawable
	delete (drawable);
	iDrawablesArray[aId - KDiffBtnIdAndPos]=NULL;
	iNumOfDrawables--;
	}												    

TInt CDrawablesCollection::DrawableId(COplDrawable* aDrawable)
 // returns position of drawable in array. Note that aDrawable will ALWAYS be in the array
	{		 
	TInt index = 0;
	for (; index < KMaxDrawables; index++)
		{
		if (aDrawable == iDrawablesArray[index])
			break;
		}
	return index + KDiffBtnIdAndPos;
	}

void CDrawablesCollection::SetCurrentDrawableL(TInt aId)
 // sets iCurrentDrawable to the drawable at aId-1
	{
	iCurrentDrawable = DrawableL(aId);
	}

 COplDrawable* CDrawablesCollection::DrawableL(TInt aId) 
 	{
	if ((aId<KDiffBtnIdAndPos) || (aId>=KMaxDrawables+KDiffBtnIdAndPos))
		User::Leave(KErrArgument);
 	COplDrawable* drawable=(iDrawablesArray[aId - KDiffBtnIdAndPos]);
	if (drawable==NULL)
		User::Leave(KOplErrDrawNotOpen);
	return drawable;
 	}

CFbsFont* CDrawablesCollection::GetFontByIdL(TUid aFontId, const TAlgStyle& aAlgStyle)
	{
	CBitmapDevice* device = NULL;
	CFont* font = NULL;
	TInt err=KErrNone;
	if (iCurrentDrawable->DrawableFlag() != EIsOplBitmap)
		{
		device = ((COplWindow*)iCurrentDrawable)->iScreenDevice;
		err=((CWsScreenDevice*)device)->GetFontById(font,aFontId,aAlgStyle);
		}
	else
		{
		device = ((COplBitmap*)iCurrentDrawable)->BitmapDevice();
		err=((CFbsBitmapDevice*)device)->GetFontById(font,aFontId,aAlgStyle);
		}
	if (err)
		{
		if (err==KErrNotFound)
			err=KOplErrFontNotLoaded;
		User::Leave(err);
		}
	return ((CFbsFont*)font);
	}
 
CFbsFont* CDrawablesCollection::GetFontByNameL(const TFontSpec& aFontSpec)
	{
	CBitmapDevice* device = NULL;
//	CFont* font = NULL;
//	TInt err=KErrNone;
	if (iCurrentDrawable->DrawableFlag() != EIsOplBitmap)
		{
		//device = ((COplWindow*)iCurrentDrawable)->iScreenDevice;
		CFbsFont* font=TheRuntime()->ConEnv()->CreateScreenFontL(aFontSpec);
		//err=((CWsScreenDevice*)device)->GetFontById(font,aFontId,aAlgStyle);
		return ((CFbsFont*)font);
		}
	else
		{
		device = ((COplBitmap*)iCurrentDrawable)->BitmapDevice();
		CFbsFont* font=TheRuntime()->ConEnv()->CreateDeviceFontL(device,aFontSpec);
//		err=((CFbsBitmapDevice*)device)->GetFontById(font,aFontId,aAlgStyle);
		return ((CFbsFont*)font);
		}
//	if (err)
//		{
//		if (err==KErrNotFound)
//			err=KOplErrFontNotLoaded;
//		User::Leave(err);
//		}
	}


TInt16 CDrawablesCollection::AddFontFileL(const TDesC& aFontFile)
	{
	TInt16 id=-1;
	TInt* fileIdPtr=iFontFileId;
    TInt ii;
	for (ii=0;ii<KMaxFontFiles;ii++,fileIdPtr++)
		{
		if (*fileIdPtr==0)
			{
			id=(TInt16)(ii+KFontIdOffset);
			break;
			}
		}
	if (ii==KMaxFontFiles)
		User::Leave(KErrGeneral);
	COplDrawable* drawable=DrawableL(1);
	// add to screen device via the console drawable
#if defined(_DEBUG)
	_LIT(KAddFontFile,"AddFontFile");
#endif
	__ASSERT_DEBUG(drawable->DrawableFlag()==EIsOplConsole,User::Panic(KAddFontFile,1));
	User::LeaveIfError(STATIC_CAST(COplWindow*,drawable)->iScreenDevice->AddFile(aFontFile,iFontFileId[ii]));
	return id;
	}


void CDrawablesCollection::RemoveFontFile(TInt aId)
	{
	if (aId==0)
		return;
	TInt index=aId-KFontIdOffset;
	if (index<0 || index >KMaxFontFiles)
		User::Leave(KErrArgument);
	COplDrawable* drawable=DrawableL(1);
	// remove from screen device via the console drawable
#if defined(_DEBUG)
	_LIT(KRemoveFontFile,"RemoveFontFile");
#endif
	__ASSERT_DEBUG(drawable->DrawableFlag()==EIsOplConsole,User::Panic(KRemoveFontFile,1));
	TInt& id=iFontFileId[index];
	if (id)
		STATIC_CAST(COplWindow*,drawable)->iScreenDevice->RemoveFile(id);
	else
		User::Leave(KErrArgument);
	id=0;
	}


TBool CDrawablesCollection::SetFlushState(TBool aState)
	{
	TBool prev=iFlushState;
	iFlushState=aState;
	return prev;
	}

void CDrawablesCollection::DrawCursorIfOn()
	{
	if (iDrawableWithCursor>0) // not console text cursor
		{
		COplWindow* win=STATIC_CAST(COplWindow*,DrawableL(iDrawableWithCursor));
#if defined(_DEBUG)
		_LIT(KOplrName,"OPLR");
#endif
		__ASSERT_DEBUG(win->DrawableFlag()!=EIsOplBitmap,User::Panic(KOplrName,1)); // check the cast was OK
		win->DrawCursorIfOn(win->Cursor());
		}
	}

void CDrawablesCollection::DrawAllClocks()
	{
	COplDrawable** drawablePtr = iDrawablesArray;
	for (TInt index=0; index < KMaxDrawables; index++,drawablePtr++)
		{
		if (*drawablePtr && (*drawablePtr)->DrawableFlag()!=EIsOplBitmap)
			STATIC_CAST(COplWindow*,*drawablePtr)->DrawClock();
		}
	}

void CDrawablesCollection::UpdateAllClocks()
	{
	TLocale locale;
	COplDrawable** drawablePtr = iDrawablesArray;
	for (TInt index=0; index < KMaxDrawables; index++,drawablePtr++)
		{
		if (*drawablePtr && (*drawablePtr)->DrawableFlag()!=EIsOplBitmap)
			STATIC_CAST(COplWindow*,*drawablePtr)->UpdateClock(locale);
		}
	}

/*******************************
class COplDrawable
*******************************/


COplDrawable::COplDrawable()
	{
	}
		  

COplDrawable::~COplDrawable()
	{
	delete iDrawableGc;
	}

void COplDrawable::MoveCursorToEndOfBuffer(TDesC& aText)
	{
	TPoint newPos(iCursorPosition.iX+iFont->TextWidthInPixels(aText),iCursorPosition.iY);
	SetCursorPosition(newPos);
	}

/*
void COplDrawable::UpdatePoints(TPoint& aStartPoint, TPoint& aEndPoint)
	// note that startPoint and endPoint will never be the same as that will be handled before
	// note also that whenever either the x or y coordinates of the end point is greater than its
	// corresponding start point coordinates then no change is required as the wserv does not draw the
	// the last point on a line!
	{
	if (aStartPoint.iX == aEndPoint.iX) 
			// ie a vertical line
		{
		if (aStartPoint.iY > aEndPoint.iY)	  // vertical line drawn upwards
			SwapPoints(aStartPoint,aEndPoint); // swap over so that bottom point is not included
		return;
		}
	if (aStartPoint.iY == aEndPoint.iY)
			// ie is a horizontal line
		{
		if (aStartPoint.iX > aEndPoint.iX)	  // horizontal line drawn right to left
			SwapPoints(aStartPoint,aEndPoint); // swap over so that right-most point	is not included
		return;
		}

	TInt yDiff = aStartPoint.iY - aEndPoint.iY;
	TInt xDiff = aStartPoint.iX - aEndPoint.iX;
	TBool slope = xDiff^yDiff;
	
	TInt singlePixelOffset = 1;
	if (!slope)
		{
		if (aStartPoint.iX > aEndPoint.iX)	// if start point is below and to the right of the end point  
			SwapPoints(aStartPoint,aEndPoint);  // swap over so that the bottom-right point is not included
		}
	else
		{
		if (aStartPoint.iY > aEndPoint.iY)
			aStartPoint.iY -= singlePixelOffset;
		else
			aEndPoint.iY -= singlePixelOffset;
		if (aStartPoint.iX > aEndPoint.iX)
			{
			aStartPoint.iX -= singlePixelOffset;
			aEndPoint -= TPoint(singlePixelOffset,-singlePixelOffset);	 // make sure the *new* 
			}														// endpoint is drawn by the wserv
		else
			{
			aEndPoint.iX -= singlePixelOffset;
			aEndPoint += TPoint(singlePixelOffset,-singlePixelOffset);	 // as above!
			}
		}
	}
*/

void COplDrawable::CalcMsgPosition(TInt aCorner, TGulAlignment& aAlignment)
	{
	switch(aCorner)
		{
	case 0 :
		aAlignment=EHLeftVTop;
		break;
	case 1 :
		aAlignment=EHLeftVBottom;
		break;
	case 2 :
		aAlignment=EHRightVTop;
		break;
	case 3 :
		aAlignment=EHRightVBottom;
		break;
	default:
		User::Leave(KErrArgument);
		break;
		}
	}

TOplDrawMode COplDrawable::GetOplDrawMode(TInt aMode)
	{
	if ((aMode<0) || (aMode>3))
		User::Leave(KOplErrInvalidArgs);
	switch(aMode)
		{
	case 1 :
		return EModeClear;	
	case 2 :
		return EModeInvert;
	case 3 :
		return EModeReplace;
	default :
		return EModeSet;
		}
	}

void COplDrawable::SetInverseStyle(TBool aStyle)
	{
	if (aStyle && (iStyle&KStyleInverse))
		return;
	if (aStyle)
		iStyle += KStyleInverse;
	else
		iStyle -= KStyleInverse;
	}

void COplDrawable::PrintAndMoveCursor(TDesC& aText)
	{
	Print(aText);
	MoveCursorToEndOfBuffer(aText);
	}

void COplDrawable::Print(TDesC& aText)
	{
	if ((InverseStyle()) && (iCurrentDrawMode!=EModeGXPrint))
		{
		TRect rect = FontRect(aText);
		SetGcMode(EModeInverseStyle);
		iDrawableGc->DrawText(aText,rect,iFont->AscentInPixels());
		}
	else
		{
		SetGcMode(iCurrentTextMode);
		if (iCurrentTextMode == EModeReplace)
			{
			TRect rect = FontRect(aText);
			iDrawableGc->DrawText(aText,rect,iFont->AscentInPixels());
			}
		else
			iDrawableGc->DrawText(aText,iCursorPosition);
		}
	}
	
TRect COplDrawable::FontRect(TDesC& aText, TInt aWidth)
	{
	TPoint topLeft(iCursorPosition.iX,iCursorPosition.iY-iFont->AscentInPixels());
	TSize size(0,0);
	if (aWidth == 0)
		aWidth = iFont->TextWidthInPixels(aText);
	size.SetSize(aWidth,iFont->HeightInPixels());
	return (TRect(topLeft,size));
	}

TPoint COplDrawable::PopPoint(CStack& aStack)
	{
	TInt yPos = aStack.PopInt16();
	return (TPoint(aStack.PopInt16(),yPos));
	}

TSize COplDrawable::PopSize(CStack& aStack)
	{
	TInt height = aStack.PopInt16();
	return (TSize(aStack.PopInt16(),height));
	}

TRect COplDrawable::PopRect(CStack& aStack)
	{
	TSize size = PopSize(aStack);
	return (TRect(PopPoint(aStack),size));
	}

#if defined(__SERIES60__)
void COplDrawable::SetFont(CFbsFont* aFont,const TBool aStandardFont)
	{
	if (iFont)
		ReleaseFont();
	iFont = aFont;
	iDrawableGc->UseFont(aFont); //pukka
	iIsFontToBeReleased=(aStandardFont)?EFalse:ETrue;
	}
#else
void COplDrawable::SetFont(CFbsFont* aFont)
	{
	if (iFont)
		ReleaseFont();
	iFont = aFont;
	iDrawableGc->UseFont(aFont);
	}
#endif

void COplDrawable::SetDrawMode(TOplDrawMode aDrawMode)
	{
//	SetGcMode(aDrawMode);
	iCurrentDrawMode = aDrawMode;
	}

void COplDrawable::SetTextMode(TOplDrawMode aTextMode)
	{
//	SetGcMode(aTextMode);
	iCurrentTextMode = aTextMode;
	}

void COplDrawable::SetGcMode(TOplDrawMode aMode)
// Functions relying on the gGMode, gTMode settings call this before drawing in case GC
// has been changed directly.
// Just returns if still the same valid setting.
	{
#if defined(_DEBUG)
	_LIT(KInvalidGCMode,"Invalid Opl GC mode");
#endif
	__ASSERT_DEBUG(aMode!=EOplGcModeInvalid,User::Panic(KInvalidGCMode,1)); // user should have call equivalent SetGcModeInvalid() instead
	if (iCurrentGcMode==aMode)
		return;
	iCurrentGcMode=aMode;
	TRgb foreGround = iForegroundColor;
	TRgb backGround = iBackgroundColor;
	iDrawableGc->SetPenStyle(CGraphicsContext::ESolidPen);
	CGraphicsContext::TBrushStyle brushStyle = CGraphicsContext::ENullBrush;
	CGraphicsContext::TDrawMode drawMode = CGraphicsContext::EDrawModePEN;
	switch(aMode)
		{
	case EModeClear :
		foreGround = iBackgroundColor;
		break;
	case EModeInvert :
		drawMode = CGraphicsContext::EDrawModeNOTSCREEN;
		break;
	case EModeReplace :
		brushStyle = CGraphicsContext::ESolidBrush;
		break;
	case EModeInverseStyle :
		backGround = iForegroundColor;
		foreGround = iBackgroundColor;
		brushStyle = CGraphicsContext::ESolidBrush;
		break;
	case EModeSet :
    default:
		break;
		}
	iDrawableGc->SetPenColor(foreGround);
	iDrawableGc->SetBrushColor(backGround);
	iDrawableGc->SetBrushStyle(brushStyle);
	iDrawableGc->SetDrawMode(drawMode);
	}

void COplDrawable::SetPattern(COplDrawable* aSource, TSize& aSize)
	{
	TRect rect(iCursorPosition,aSize);
	CFbsBitmap* backup=NULL;
	SetGcModeInvalid();	// GC set below directly
	if (aSource->DrawableFlag() == EIsOplBitmap)
		iDrawableGc->UseBrushPattern(&((COplBitmap*)aSource)->FbsBitmap());
	else
		{
        backup = new(ELeave) CFbsBitmap();
		CleanupStack::PushL(backup);
		((COplWindow*)aSource)->BackupBmpHandleL(*backup);
		iDrawableGc->UseBrushPattern(backup);
		}
	iDrawableGc->SetBrushStyle(CGraphicsContext::EPatternedBrush);		
	iDrawableGc->DrawRect(rect);
	if (backup)
		CleanupStack::PopAndDestroy();
	}

void COplDrawable::DoScrollClear(const TPoint& aOffset,const TRect& aArea)
	{
	SetGcMode(EModeReplace); // does not affect current draw and text modes
	TInt absDX=Abs(aOffset.iX);
	TInt absDY=Abs(aOffset.iY);
	if (absDX>aArea.Size().iWidth||absDY>aArea.Size().iHeight)
		iDrawableGc->Clear(aArea); // no overlap
	else
		{
		TSize size(absDX,aArea.Size().iHeight);
		if (aOffset.iX>0)
			iDrawableGc->Clear(TRect(aArea.iTl,size));
		else if (aOffset.iX<0)
			iDrawableGc->Clear(TRect(TPoint(aArea.iBr.iX+aOffset.iX,aArea.iTl.iY),size));
		size.SetSize(aArea.Size().iWidth,absDY);
		if (aOffset.iY>0)
			iDrawableGc->Clear(TRect(aArea.iTl,size));
		else if (aOffset.iY<0)
			iDrawableGc->Clear(TRect(TPoint(aArea.iTl.iX,aArea.iBr.iY+aOffset.iY),size));
		}
	}

void COplDrawable::DrawLine(TPoint& aEndPoint,TBool aLineBy)
	{
	SetGcMode(CurrentDrawMode());
	TPoint startPoint = CursorPosition();
	if (aLineBy)
		aEndPoint+=startPoint;
	if (startPoint == aEndPoint)
		iDrawableGc->Plot(aEndPoint);
	else
		{
		SetCursorPosition(aEndPoint);
		iDrawableGc->DrawLine(startPoint,aEndPoint);
		}
	}

void COplDrawable::DrawGXBorderShadow(TSize& aSize, TInt aCorner, TBool aShadow)
	{
	TInt w = aSize.iWidth;
	TInt h = aSize.iHeight;
	SetGcModeInvalid();	// setting GC directly
	if (aShadow)
		iDrawableGc->SetPenColor(TRgb::Gray16(0));
	else
		iDrawableGc->SetPenColor(TRgb::Gray16(15));
	
	iDrawableGc->DrawLine(iCursorPosition+TPoint(w-3,2),iCursorPosition+TPoint(w-3,h-3)); // right
	iDrawableGc->DrawLine(iCursorPosition+TPoint(w-5,h-3),iCursorPosition+TPoint(w-3,h-5));	 // bottom
	iDrawableGc->DrawLine(iCursorPosition+TPoint(w-6,h-3),iCursorPosition+TPoint(w-3,h-6));	 // right corner
	iDrawableGc->DrawLine(iCursorPosition+TPoint(w-3,h-3),iCursorPosition+TPoint(1,h-3)); // bottom
	
	if (aCorner)
		{
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-7,h-3),iCursorPosition+TPoint(w-3,h-7));	// bottom right corner
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-2,3),iCursorPosition+TPoint(w-2,h-3)); // right
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-4,3),iCursorPosition+TPoint(w-4,6));	 // right
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-5,4),iCursorPosition+TPoint(w-4,4));	 // right
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-4,h-2),iCursorPosition+TPoint(2,h-2)); // bottom
		iDrawableGc->DrawLine(iCursorPosition+TPoint(3,h-4),iCursorPosition+TPoint(6,h-4));	// bottom left corner
		iDrawableGc->DrawLine(iCursorPosition+TPoint(4,h-5),iCursorPosition+TPoint(4,h-4));	// bottom left corner
		}
	else 
		{
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-2,2),iCursorPosition+TPoint(w-2,h-2));  // right
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-4,3),iCursorPosition+TPoint(w-4,5));	  // top right corner
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-3,h-2),iCursorPosition+TPoint(1,h-2)); // bottom   
		iDrawableGc->DrawLine(iCursorPosition+TPoint(3,h-4),iCursorPosition+TPoint(5,h-4));	 // bottom left corner
		}

	if (aShadow)
		iDrawableGc->SetPenColor(TRgb::Gray16(10));

	iDrawableGc->DrawLine(iCursorPosition+TPoint(4,2),iCursorPosition+TPoint(2,4));	 // top left corner
	iDrawableGc->DrawLine(iCursorPosition+TPoint(5,2),iCursorPosition+TPoint(2,5));	  // top left corner
	iDrawableGc->DrawLine(iCursorPosition+TPoint(2,2),iCursorPosition+TPoint(2,h-3)); // left
	if (aCorner)
		{
		iDrawableGc->DrawLine(iCursorPosition+TPoint(3,1),iCursorPosition+TPoint(w-3,1)); // top
		iDrawableGc->DrawLine(iCursorPosition+TPoint(3,2),iCursorPosition+TPoint(w-3,2));  // top
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-5,3),iCursorPosition+TPoint(w-7,3));	// top right corner
		iDrawableGc->DrawLine(iCursorPosition+TPoint(6,2),iCursorPosition+TPoint(2,6));	 // top left corner
		iDrawableGc->DrawLine(iCursorPosition+TPoint(1,3),iCursorPosition+TPoint(1,h-3));	// left
		iDrawableGc->DrawLine(iCursorPosition+TPoint(3,h-5),iCursorPosition+TPoint(3,h-7));	// left
		}
	else
		{
		iDrawableGc->DrawLine(iCursorPosition+TPoint(2,1),iCursorPosition+TPoint(w-2,1));  // top
		iDrawableGc->DrawLine(iCursorPosition+TPoint(2,2),iCursorPosition+TPoint(w-3,2));  // top
		iDrawableGc->DrawLine(iCursorPosition+TPoint(w-5,3),iCursorPosition+TPoint(w-6,3));	// top right corner
		iDrawableGc->DrawLine(iCursorPosition+TPoint(1,2),iCursorPosition+TPoint(1,h-2));	// left
		iDrawableGc->DrawLine(iCursorPosition+TPoint(3,h-5),iCursorPosition+TPoint(2,h-5));	// left
		}
	}

void COplDrawable::DrawS3Border(TInt aFlags,TRect aRect)
	{
	if (aFlags&0x100) // one pixel gap
		aRect.Shrink(1,1);
	TSize cornerSize(KNormalCornerSize,KNormalCornerSize);
	if (aFlags&0x200)
			cornerSize.SetSize(KMoreRoundedCornerSize,KMoreRoundedCornerSize);
	TInt pixelCount=((aFlags&0x7)+1)/2;
	aRect.iBr-=TSize(pixelCount,pixelCount);
	SetGcMode(CurrentDrawMode());           //hp: otherwise gGMode 2: gBorder has no effect
	SetGcModeInvalid();	// setting GC directly
	iDrawableGc->SetBrushStyle(CGraphicsContext::ENullBrush);
	iDrawableGc->SetPenColor(TRgb::Gray16(0));
	iDrawableGc->DrawRoundRect(aRect,cornerSize);
	if (!(aFlags&0x1)) // must clear shadow area if not wanted
		iDrawableGc->SetPenColor(TRgb::Gray16(15));
	// draw shadow here
	for (;pixelCount--;)
		{
		aRect.Move(1,1);
		TRect cornerRect(aRect.iBr-cornerSize-cornerSize,cornerSize+cornerSize);
		TPoint start(aRect.iBr-TSize(cornerSize.iWidth,0));
		TPoint end(aRect.iBr-TSize(0,cornerSize.iHeight));
		iDrawableGc->DrawArc(cornerRect,start,end);
		if (cornerSize.iHeight==KMoreRoundedCornerSize)
			{
			iDrawableGc->DrawLine(aRect.iBr-TSize(4,2),aRect.iBr-TSize(1,5));
			//iDrawableGc->Plot(aRect.iBr-TSize(2,4));
			//iDrawableGc->Plot(aRect.iBr-TSize(3,3));
			}
		else
			{
			iDrawableGc->Plot(aRect.iBr-TSize(3,2));
			iDrawableGc->Plot(aRect.iBr-TSize(2,3));
			}
		start-=TSize(1,1);
		end-=TSize(1,1);
		iDrawableGc->DrawLine(start,start-TSize(aRect.Size().iWidth-3*(cornerSize.iWidth/2),0));
		// cornerSize.iXxx/2 ok as cornerSize is always divisible by 2
		iDrawableGc->DrawLine(end,end-TSize(0,aRect.Size().iHeight-3*(cornerSize.iHeight/2)));
		}
	}


void COplDrawable::DrawS3ButtonL(TInt aState,TInt aWidth,TInt aHeight,const TDesC& aText)
	{
	SetGcModeInvalid();	// setting GC below directly
	iDrawableGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	iDrawableGc->SetDrawMode(CGraphicsContext::EDrawModePEN);
	TPoint tl=iCursorPosition;
	TPoint br=iCursorPosition+TSize(aWidth-1,aHeight-1);
	iDrawableGc->SetPenColor(TRgb::Gray16(15)); // white
	iDrawableGc->SetBrushColor(TRgb::Gray16(15)); // white
	// clear the corners
	iDrawableGc->Plot(tl+TSize(aWidth-1,0));
	iDrawableGc->Plot(tl+TSize(aWidth-1,1));
	iDrawableGc->Plot(tl+TSize(0,aHeight-1));
	iDrawableGc->Plot(tl+TSize(1,aHeight-1));
	if (aState!=0)
		{
		iDrawableGc->DrawLine(tl,tl+TSize(aWidth,0));
		iDrawableGc->DrawLine(tl,tl+TSize(0,aHeight));
		tl+=(TSize(1,1));
		}
	else
		{
		TPoint p=br-TSize(1,1);
		iDrawableGc->DrawLine(p,p-TSize(aWidth-1,0));
		iDrawableGc->DrawLine(p,p-TSize(0,aHeight-1));
		}
	iDrawableGc->SetPenColor(TRgb::Gray16(0)); // black
	TInt descent=((aHeight-4)-iFont->HeightInPixels())/2+iFont->AscentInPixels();
	iDrawableGc->DrawText(aText,TRect(tl+TSize(1,1),TSize(aWidth-4,aHeight-4)),descent,CGraphicsContext::ECenter);
	iDrawableGc->SetBrushStyle(CGraphicsContext::ENullBrush);
	iDrawableGc->DrawRect(TRect(tl,	TSize(aWidth-2,aHeight-2)));
	iDrawableGc->Plot(br-TSize(aWidth-2,1));
	iDrawableGc->Plot(br-TSize(1,aHeight-2));
	iDrawableGc->DrawLine(br-TSize(aWidth-3,0),br);
	iDrawableGc->DrawLine(br,br-TSize(0,aHeight-2));
	}

//////////////////////////////////////
//////// PROTECTED FUNCTIONS /////////
//////////////////////////////////////


void COplDrawable::SwapPoints(TPoint& aPoint1, TPoint& aPoint2)
	{
	TPoint temp;
	temp = aPoint1;
	aPoint1 = aPoint2;	
	aPoint2 = temp;			
	}

#if defined(__SERIES60__)||defined(__UIQ__)
CFbsFont* COplDrawable::GetDefaultFontL(CBitmapDevice& aDevice)
	{
	CFont* font;
	TAlgStyle style;
	style.SetWidthFactor(1);
	style.SetHeightFactor(1);
	// Load a decent system font...
// NB. If this isn't a standard font, set the iIsFontToBeReleased flag True.
	_LIT(KMonoFont,"LatinPlain12"); //CourierNew
	TPtrC fontName(KMonoFont);
	TFontSpec fontSpec(fontName,200); // Ignoring any style info for now.... !!TODO 
	if (iDrawableFlag == EIsOplBitmap)
		{
//		User::LeaveIfError(((CFbsBitmapDevice&)aDevice).GetFontById(font,TUid::Uid(KScreenFontUidArialNormal15),style));
		User::LeaveIfError(((CFbsBitmapDevice&)aDevice).GetNearestFontInTwips(font,fontSpec));
		}
	else
		{
		User::LeaveIfError(((CWsScreenDevice&)aDevice).GetNearestFontInTwips(font,fontSpec));
		}
	iFontId = 0x9a; // default font id
	iIsFontToBeReleased=EFalse; // Don't release standard fonts.
	return((CFbsFont*)font);
	}
#else
CFbsFont* COplDrawable::GetDefaultFontL(CBitmapDevice& aDevice)
	{
	CFont* font;
	TAlgStyle style;
	style.SetWidthFactor(1);
	style.SetHeightFactor(1);
	if (iDrawableFlag == EIsOplBitmap)
		User::LeaveIfError(((CFbsBitmapDevice&)aDevice).GetFontById(font,TUid::Uid(KScreenFontUidArialNormal15),style));
	else
		User::LeaveIfError(((CWsScreenDevice&)aDevice).GetFontById(font,TUid::Uid(KScreenFontUidArialNormal15),style));
	iFontId = 0x9a; // default font id
	return((CFbsFont*)font);
	}
#endif

TDisplayMode COplDrawable::DisplayMode(TInt aValue)
	{
	switch (aValue)
		{
	case 0 :
		return EGray2;
	case 1 :
		return EGray4;
	case 2 :
		return EGray16;
	case 3 :
		return EGray256;
	case 4 :
		return EColor16;
	case 5 :
		return EColor256;
	case 6 :
		return EColor64K;
	case 7 :
		return EColor16M;
	case 8 :
		return ERgb;
	case 9 :
		return EColor4K;
	default :
		return EGray16;
		}
	}

TInt COplDrawable::DisplayMode(TDisplayMode aMode)
	{
	switch (aMode)
		{
	case EGray2:
		return 0;
	case EGray4:
		return 1;
	case EGray256:
		return 3;
	case EColor16:
		return 4;
	case EColor256:
		return 5;
	case EColor64K:
		return 6;
	case EColor16M:
		return 7;
	case ERgb:
		return 8;
	case EColor4K:
		return 9;
	case EGray16:
	default :
		return 2;
		}
	}

void COplDrawable::DoStoreBitmapL(COplRuntime& aRuntime, const TDesC& aFileName, TSize& aSize,const CFbsBitmap& aBitmap) const
	{
	
	RFile file;
    User::LeaveIfError(file.Replace(aRuntime.ConEnv()->FsSession(),aFileName,EFileRead|EFileWrite));
	CDirectFileStore* filestore=CDirectFileStore::NewLC(file);
	TUidType uids(KDirectFileStoreLayoutUid,KUidExternalOplFile); //default to no doc
	if (aRuntime.CurrentDocumentName().CompareF(aFileName)==0)
		uids=TUidType(KDirectFileStoreLayoutUid,KUidOplDoc,aRuntime.CommandLine().AppUid());
	filestore->SetTypeL(uids);
	RStoreWriteStream bmpstream;
	TStreamId bmpstreamid=bmpstream.CreateLC(*filestore);
	if ((aSize.iWidth != NULL) || (aSize.iHeight != NULL))
		aBitmap.ExternalizeRectangleL(bmpstream, TRect(CursorPosition(),aSize));
	else
		aBitmap.ExternalizeL(bmpstream);	
	bmpstream.Close();
	RStoreWriteStream headstream;
	TStreamId headstreamid=headstream.CreateLC(*filestore);
	headstream.WriteInt32L(1); // number of bitmaps
	bmpstreamid.ExternalizeL(headstream); // stream id of bitmap
	headstream.Close();
	filestore->SetRootL(headstreamid);
	CleanupStack::PopAndDestroy(3);
	}

	

	
/*******************************
class COplWindow
*******************************/


COplWindow::COplWindow() 
	{
	}


void COplWindow::ConstructL(COplRuntime& aRuntime, TInt aHandle, const TPoint& aPosition, const TSize& aWinSize, TInt aVisible, TInt aGray)	
	{ 
    CEikonEnv* eikonEnv = (CEikonEnv*)aRuntime.ConEnv();
	iWindow = RBackedUpWindow(eikonEnv->WsSession());
	iDisplayMode=DisplayMode(aGray & KOplWindowDisplayModeMask);
	User::LeaveIfError(iWindow.Construct((RWindowTreeNode&)eikonEnv->RootWin(),iDisplayMode,(TUint32)aHandle));
//	User::LeaveIfError(iWindow.SetRequiredDisplayMode(DisplayMode(aGray)));
	User::LeaveIfError(iWindow.SetCornerType(EWindowCornerSquare));	// by default
	iWindow.SetPosition(aPosition);
    if (aGray & KOplWindowShadowOnMask)
        {
        iWindow.SetShadowDisabled(EFalse);  // enable shadow
        if	(aGray & KOplWindowShadowMask)
	        iWindow.SetShadowHeight(((aGray&KOplWindowShadowMask)>>8) * KOplShadowHeight); 
        }
    else
        iWindow.SetShadowDisabled(ETrue);  // disable shadow
    iWindow.PointerFilter(EPointerFilterDrag|EPointerFilterMove|EPointerFilterEnterExit,0);
	if (!aVisible)
		iWindow.SetVisible(EFalse);
	TRgb color=TRgb::Gray16(15);
	SetBackgroundColor(color); // white
	SetSizeL(aWinSize); 
	SetDrawableFlag(EIsOplWindow);
	SetAttributesL(*eikonEnv);
	iWindow.Activate();
	}

void COplWindow::ConstructL(COplRuntime& aRuntime, RBackedUpWindow& aWin,TDisplayMode aDisplayMode)
	{
	iWindow = aWin;
	iDisplayMode=aDisplayMode;
	User::LeaveIfError(iWindow.SetCornerType(EWindowCornerSquare));	// by default
	SetSizeL(aWin.Size());
	SetDrawableFlag(EIsOplConsole);
	SetAttributesL(*aRuntime.ConEnv());
	iWindow.PointerFilter(EPointerFilterDrag|EPointerFilterMove|EPointerFilterEnterExit,0);
	}

COplWindow::~COplWindow()		
	{
	DeleteClock();
	ReleaseFont();
	if (iDrawableGc)
		DeactivateGc();
	if (iDrawableFlag != EIsOplConsole)
		iWindow.Close();
	}

#if defined(__SERIES60__)
void COplWindow::ReleaseFont()
	{
	if (iScreenDevice&&iFont)
		if (iIsFontToBeReleased!=EFalse)
			iScreenDevice->ReleaseFont(iFont);
	}
#else
void COplWindow::ReleaseFont()
	{
	if (iScreenDevice&&iFont)
		iScreenDevice->ReleaseFont(iFont);
	}
#endif

void COplWindow::StoreBitmapL(COplRuntime& aRuntime, const TDesC& aFileName, TSize& aSize)
	{
	CFbsBitmap* bitmap=new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	BackupBmpHandleL(*bitmap);
	DoStoreBitmapL(aRuntime,aFileName,aSize,*bitmap);
	CleanupStack::PopAndDestroy();
	}

void COplWindow::SetSizeL(const TSize& aSize)
	{
	iSize = aSize;
	User::LeaveIfError(iWindow.SetSizeErr(aSize));
	}

void COplWindow::ClearWindow() 
 	{
	SetGcMode(EModeReplace); // does not affect current draw and text modes
	iDrawableGc->Clear(TRect(KScreenOrigin,Size()));
	}
 
void COplWindow::SetWinPosition(const TPoint& aPoint)
	{
	iWindow.SetPosition(aPoint);
	}

void COplWindow::ActivateGc()
	{
	((CWindowGc*)iDrawableGc)->Activate(iWindow);
	}

void COplWindow::DeactivateGc()
	{
	((CWindowGc*)iDrawableGc)->Deactivate();
	}

void COplWindow::BackupBmpHandleL(CFbsBitmap& aBitmap)
	{
	User::LeaveIfError(aBitmap.Duplicate(BitmapHandle()));
	}

TInt COplWindow::BitmapHandle()
	{
	iWindow.UpdateBackupBitmap();
	return iWindow.BitmapHandle();
	}


void COplWindow::SetCursorPosition(const TPoint& aPosition)
	{
	iCursorPosition = aPosition;
	DrawCursorIfOn(iCursor);
	}

void COplWindow::SetCursorEnabled(TBool aEnabled)
	{
	iCursorEnabled = aEnabled;
	}

void COplWindow::DrawCursorIfOn(TTextCursor aCursor)
	{
	if (iCursorEnabled)
		{
		iCursor = aCursor;
		((CEikonEnv*)(TheRuntime()->ConEnv()))->RootWin().SetTextCursor(iWindow,iCursorPosition,aCursor);
		}
	}


void COplWindow::Scroll(const TPoint& aOffset,const TRect& aArea)
	{
	iWindow.Scroll(aOffset,aArea);
	DoScrollClear(aOffset,aArea);
	}


void COplWindow::UseNewWindow(RBackedUpWindow& aNewWindow,TDisplayMode aDisplayMode)
	{
	DeleteClock();
	DeactivateGc();
	iWindow=aNewWindow;
	iDisplayMode=aDisplayMode;
	ActivateGc();
	iDrawableGc->UseFont(iFont);
//	iIsFontToBeReleased is already set because the font is aleady set...
	SetCursorPosition(TPoint(0,0));
	SetGcModeInvalid(); // ensure gc set correctly before it is used
	}

TBool COplWindow::ClockIsVisible(const CEikLocaleConformantClock& /*aClock*/) const
	{
	return ETrue; // what if window is not visible??????
	}

void COplWindow::GetClockPositionAndSize(TPoint& aPosition, TSize& aSize, const CEikLocaleConformantClock& /*aClock*/, const TSize& aMinimumSize)
	{
	aPosition=iClockPosition;
	if (aSize.iWidth==0)
		aSize=aMinimumSize;
	}

#if !defined(__SERIES60__)
void COplWindow::CreateClockL(CEikonEnv& aEikonEnv,TInt aType,TInt aOffset)
	{
	TInt type=EEikLocaleConformantClockTypeAnalog;
	TInt analogClock=R_EIK_ANALOG_CLOCK;
	iFormatMatchesLocale=EFalse;
	TLocale locale;
	switch (aType)
		{
	case 6:
		if (locale.ClockFormat()!=EClockAnalog)
			type=EEikLocaleConformantClockTypeDigital;
		iFormatMatchesLocale=ETrue;
		break;
	case 7:
		break;
	case 8:
		type=EEikLocaleConformantClockTypeDigital;
		break;
	case 9:
		analogClock=R_EIK_BIG_ANALOG_CLOCK;
		type=EEikLocaleConformantClockTypeAnalog;
		break;
	default:
		User::Leave(KErrArgument);
		}
	iClockPosition=iCursorPosition;
	CEikLocaleConformantClock* clock=new(ELeave)CEikResourceConstructedClock(*this,aEikonEnv.ClockDllL(),iWindow,
		type,EFalse,aEikonEnv,R_EIK_DIGITAL_CLOCK,analogClock);
	CleanupStack::PushL(clock);
	iTimeOffsetInSeconds=aOffset;
	clock->ConstructL(iTimeOffsetInSeconds+locale.UniversalTimeOffset().Int(),EDstHome);
	iClock=clock;
	iClock->SetVisible(ETrue);
	//iClock->Draw(*(CWindowGc*)iDrawableGc);
	CleanupStack::Pop();
	}
#else
void COplWindow::CreateClockL(CEikonEnv&,TInt,TInt)
	{
	}
#endif



void COplWindow::DrawClock()
	{
#if !defined(__SERIES60__)
	if (iFormattedClock)
		iFormattedClock->Draw();
	if (iClock)
		iClock->RedrawAll(*(CWindowGc*)iDrawableGc);
#endif
	}

#if !defined(__SERIES60__)
void COplWindow::UpdateClock(const TLocale& aLocale)
	{
	TInt universalOffset=aLocale.UniversalTimeOffset().Int();
	if (iClock)
		{
		if (iFormatMatchesLocale) // only iClock can change format
			{
			TRAPD(ignore, iClock->ChangeFormatL(aLocale.ClockFormat()));
			}
//#ifdef BADCLOCK
		iClock->ChangeUniversalTimeOffsetAndDaylightSavingZone(iTimeOffsetInSeconds+universalOffset,EDstHome);
//#endif
		iClock->Draw(*(CWindowGc*)iDrawableGc);
		}
	if (iFormattedClock)
		{
		iFormattedClock->SetUniversalTimeOffset(iTimeOffsetInSeconds+universalOffset+((aLocale.QueryHomeHasDaylightSavingOn())?3600:0));
		iFormattedClock->Draw();
		}
	}
#else
void COplWindow::UpdateClock(const TLocale&)
	{
	}
#endif		

#if !defined(__SERIES60__)
LOCAL_C void ClockDestroy(TAny* aClock)
	{
	((RClock*)aClock)->Destroy();
	}
#endif

#if !defined(__SERIES60__)
void COplWindow::CreateFormattedClockL(RAnimDll& aClockDll,TInt aOffset,TInt aFontId,TInt aStyle,const TDesC& aFormat)
	{
	iFormatMatchesLocale=EFalse;
	TLocale locale;
	RDigitalClock* clock=new(ELeave) RDigitalClock(aClockDll,iWindow);
	STimeDeviceShadow shadow;
	shadow.iIsOn=EFalse;
	TRgb foregroundColor=(aStyle&0x04)?iBackgroundColor:iForegroundColor;
	TRgb backgroundColor=(aStyle&0x04)?iForegroundColor:iBackgroundColor;
	TMargins margins;
	margins.iLeft=0;
	margins.iRight=0;
	margins.iTop=0;
	margins.iBottom=0;
	CleanupStack::PushL(TCleanupItem(ClockDestroy,clock));
	iTimeOffsetInSeconds=aOffset;
	clock->ConstructL(iTimeOffsetInSeconds+locale.UniversalTimeOffset().Int()+((locale.DaylightSaving()&EDstHome)?3600:0),iCursorPosition,TSize(),margins,shadow,backgroundColor,1);
	CFont* font=NULL;
	TAlgStyle style;
	style.SetHeightFactor(1);
	style.SetWidthFactor(1);
	if (aStyle&0x01)
		style.SetIsBold(ETrue);
	if (aStyle&0x08)
		style.SetHeightFactor(2);
	if (aStyle&0x10)
		style.SetIsMono(ETrue);
	if (aStyle&0x20)
		style.SetIsItalic(ETrue);
	TUid fontUid=OplUtil::MapFontId(aFontId,style);
	User::LeaveIfError(iScreenDevice->GetFontById(font,fontUid,style));
	SEikLocaleAndFontData localeAndFontData;
	localeAndFontData.iLocale=&locale;
	localeAndFontData.iFont=font;
	_LIT(KSpace," ");
	localeAndFontData.iWidthOfSpace=font->TextWidthInPixels(KSpace);
	localeAndFontData.iWidthOfWidestCharacter=font->MaxNormalCharWidthInPixels();
	localeAndFontData.iWidthOfWidestDigit=TFindWidthOfWidestDigit().MaximumWidthInPixels(*font);
	localeAndFontData.iWidthOfWidestAmPmName=TFindWidthOfWidestAmPmName().MaximumWidthInPixels(*font);
	localeAndFontData.iWidthOfWidestAbbreviatedDayName=TFindWidthOfWidestAbbreviatedDayName().MaximumWidthInPixels(*font);
	localeAndFontData.iWidthOfWidestDayName=TFindWidthOfWidestDayName().MaximumWidthInPixels(*font);
	localeAndFontData.iWidthOfWidestAbbreviatedMonthName=TFindWidthOfWidestAbbreviatedMonthName().MaximumWidthInPixels(*font);
	localeAndFontData.iWidthOfWidestMonthName=TFindWidthOfWidestMonthName().MaximumWidthInPixels(*font);
	localeAndFontData.iWidthOfWidestDateSuffix=TFindWidthOfWidestDateSuffix().MaximumWidthInPixels(*font);
	TInt width=CEikLocaleConformantClock::PotentialWidthOfTextBlock(aFormat,localeAndFontData);
	TInt fontHandle=STATIC_CAST(CFbsFont*,font)->Handle();
	TDigitalDisplayTextSection textSection(fontHandle,foregroundColor,EDigitalDisplayHorizontalTextAlignmentLeft,
		EDigitalDisplayVerticalTextAlignmentTop,0,0,aFormat);
	TRAPD(err,clock->AddTextSectionL(textSection));
	if (!err)
		clock->SetSize(TSize(width,font->HeightInPixels()));
	iScreenDevice->ReleaseFont(font);
	User::LeaveIfError(err);
	iFormattedClock=clock;
	CleanupStack::Pop(); // clock
	iFormattedClock->SetVisible(ETrue);
//	iFormattedClock->Draw();
	}
#else
void COplWindow::CreateFormattedClockL(RAnimDll&,TInt,TInt,TInt,const TDesC&)
	{
	}
#endif

void COplWindow::DeleteClock()
	{
#if !defined(__SERIES60__)
	if (iFormattedClock)
		{
		iFormattedClock->Destroy();
		iFormattedClock=NULL;
		}
	delete iClock;
	iClock=NULL;
#endif
	}

///////////////////////////
//// protected functions ////
///////////////////////////

CBitmapContext* COplWindow::CreateWindowGcL()
	{
	CWindowGc* gc;
    User::LeaveIfError(iScreenDevice->CreateContext((CGraphicsContext*&)gc));
    return gc;
	}

void COplWindow::SetAttributesL(CCoeEnv& aConEnv)
	{
	TRgb color=TRgb::Gray16(0);
	SetForegroundColor(color);
	iScreenDevice=aConEnv.ScreenDevice();
	iDrawableGc = CreateWindowGcL();
	ActivateGc();  
	SetDrawMode(EModeSet);
	SetTextMode(EModeSet);
	iFont = GetDefaultFontL(*iScreenDevice); //This sets iIsFontToBeReleased flag if necessary.
	iDrawableGc->UseFont(iFont); 
	iWindow.SetOrdinalPosition(0,-1); // just less than default
	}



/*******************************
class COplBitmap
*******************************/


COplBitmap::COplBitmap()
	{
	}

 
void COplBitmap::ConstructL(TSize& aSize,TDisplayMode aMode)
	{
	iBitmap = new(ELeave) CFbsBitmap();
	iDisplayMode=aMode;
	User::LeaveIfError(iBitmap->Create(aSize,iDisplayMode));
	SetSizeL(aSize);
	SetAttributesL(iBitmap);
	}


void COplBitmap::ConstructL(const TDesC* aFileName, TInt aWrite, TInt aNum) 
	// overloaded constructor used by gLoadBit
	{
    iBitmap = new(ELeave) CFbsBitmap();
    if (aFileName)
		RestoreBitmapL(aFileName,aWrite,aNum);
	iSize=iBitmap->SizeInPixels();
	iDisplayMode=iBitmap->DisplayMode();

	if (aWrite==0)
		{
		iNullBitmap=new(ELeave) CFbsBitmap;
		User::LeaveIfError(iNullBitmap->Create(TSize(0,0),DisplayMode(0)));
		SetAttributesL(iNullBitmap);
		}
	else
		SetAttributesL(iBitmap);
	}

void COplBitmap::StoreBitmapL(COplRuntime& aRuntime, const TDesC& aFileName, TSize& aSize)
	{
	DoStoreBitmapL(aRuntime,aFileName,aSize,*iBitmap);
	}

COplBitmap::~COplBitmap()
	{
	ReleaseFont();
	delete iBitmapDevice;
	delete iBitmap;
	delete iNullBitmap;
	}

#if defined(__SERIES60__)
void COplBitmap::ReleaseFont()
	{
	if (iBitmapDevice&&iFont)
		if (iIsFontToBeReleased!=EFalse)
			iBitmapDevice->ReleaseFont(iFont);
	}
#else
void COplBitmap::ReleaseFont()
	{
	if (iBitmapDevice&&iFont)
		iBitmapDevice->ReleaseFont(iFont);
	}
#endif

void COplBitmap::SetSizeL(const TSize& aSize)
	{
	if (aSize != iBitmap->SizeInPixels())
		User::LeaveIfError(iBitmap->Resize(aSize));
	iSize = aSize;
	}

void COplBitmap::ClearBitmap()
	{
	SetGcMode(EModeReplace); // does not affect current draw and text modes
	iDrawableGc->Clear(TRect(KScreenOrigin,Size()));
	((CFbsBitGc*)iDrawableGc)->SetShadowMode(EFalse); // disable shadow
	}


void COplBitmap::Scroll(const TPoint& aOffset,const TRect& aArea)
	{
	TPoint point=aArea.iTl+aOffset;
	iDrawableGc->BitBlt(point,iBitmap,aArea);
	DoScrollClear(aOffset,aArea);
	}

TInt COplBitmap::BitmapHandle()
	{
	return iBitmap->Handle();
	}


///////////////////////////
//// protected functions ////
///////////////////////////

CBitmapContext* COplBitmap::CreateBitmapGcL(CFbsBitmap* aBitmap)
	{
	iBitmapDevice=CFbsBitmapDevice::NewL(aBitmap);
	CFbsBitGc* gc;
	User::LeaveIfError(iBitmapDevice->CreateContext((CGraphicsContext*&)gc));
    // this also activates the gc
    return gc;
	}

void COplBitmap::RestoreBitmapL(const TDesC* aFileName, TInt aWrite, TInt aNum)
	{
	iBitmapFilename = *aFileName;
	User::LeaveIfError(iBitmap->Load(iBitmapFilename,aNum,(aWrite==0)));
	}			 

void COplBitmap::SetAttributesL(CFbsBitmap* aBitmap)
	{
	SetDrawableFlag(EIsOplBitmap);
	TRgb color=TRgb::Gray16(0);
	SetForegroundColor(color); // black
	color=TRgb::Gray16(15);
	SetBackgroundColor(color); // white
	SetCursorPosition(KScreenOrigin);
    iDrawableGc = CreateBitmapGcL(aBitmap);
	SetDrawMode(EModeSet);
	SetTextMode(EModeSet);
	iFont = GetDefaultFontL(*iBitmapDevice); // This sets iIsFontToBeReleased if necessary.
	iDrawableGc->UseFont(iFont);
	}										    


//
// class COplClockUpdater
//

#if defined(_DEBUG)
_LIT(KClockUpdater, "ClockUpdater");
#endif

COplClockUpdater::COplClockUpdater(CDrawablesCollection& aDrawablesCollection)
	: CActive(EPriorityStandard),
	  iDrawablesCollection(aDrawablesCollection)
	{
	}

COplClockUpdater::~COplClockUpdater()
	{
	Cancel();
	iChangeNotifier.Close();
	}

void COplClockUpdater::ConstructL()
	{
	User::LeaveIfError(iChangeNotifier.Create());
#if defined(_DEBUG)
	const TInt error=
#endif
	iChangeNotifier.Logon(iStatus);
	__ASSERT_DEBUG(error==KErrNone, User::Panic(KClockUpdater, 1));
	CActiveScheduler::Add(this);
	SetActive();
	}

void COplClockUpdater::RunL()
	{
	if (iStatus.Int()&EChangesLocale)
		iDrawablesCollection.UpdateAllClocks();
#if defined(_DEBUG)
	const TInt error=
#endif
	iChangeNotifier.Logon(iStatus);
	__ASSERT_DEBUG(error==KErrNone, User::Panic(KClockUpdater, 2));
	if (!IsActive()) // COplClockUpdater::RunL may be called from the sequencer which will not set the CActive::iActive field to EFalse
		SetActive();
	}

void COplClockUpdater::DoCancel()
	{
#if defined(_DEBUG)
	const TInt error=
#endif
	iChangeNotifier.LogonCancel();
	__ASSERT_DEBUG((error==KErrNone) || (error==KErrGeneral), User::Panic(KClockUpdater, 3)); // RChangeNotifier::LogonCancel returns KErrGeneral if the preceding RChangeNotifier::Logon call completed straightaway
	}

/* End of $Workfile GRAPHICS.CPP$ */

