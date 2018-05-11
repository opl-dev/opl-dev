// G_KEYWRD.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <e32std.h>
#include <eikmsg.h>
#include <eikimage.h>
#include <eiklabel.h>
#include "graphics.h"
#include <opcodes.h>
#include "oplutil.h"

#define KGInfo32ArraySize 36
#define KGColorInfoArraySize 8

//const TInt KCursorObloid = 1;
const TInt KCursorNotFlashing = 2;
const TInt KCursorGray = 4;
// Partial solution for bug [ 910981 ] gFONT doesn't work on UIQ,
// the gFONT problem in beta release of OPL for UIQ.
// 0x017B4B0D is a random value, but outside the valid uid range, so guaranteed
// not to be a valid font id.
const TInt KFontUiqSwissABeta=0x017B4B0D;


void OpCode::gAt(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	currentDrawable->SetCursorPosition(COplDrawable::PopPoint(aStack));
	} 
	
void OpCode::gMove(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	TPoint cursorPos = currentDrawable->CursorPosition();
	currentDrawable->SetCursorPosition(cursorPos + COplDrawable::PopPoint(aStack));
	}
		
void OpCode::gBox(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt height = aStack.PopInt16();
	TInt width = aStack.PopInt16();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	currentDrawable->SetGcMode(currentDrawable->CurrentDrawMode());
	TPoint cursorPos = currentDrawable->CursorPosition();
	TPoint heightDiff(0,height);
	TPoint widthDiff(width,0);
	TRect rect;
	if ((width < 0) && (height < 0))
		rect.SetRect(cursorPos+widthDiff+heightDiff,cursorPos);
	else if (width < 0)
		rect.SetRect(cursorPos+widthDiff,cursorPos+heightDiff);
	else if (height < 0)
		rect.SetRect(cursorPos+heightDiff,cursorPos+widthDiff);
	else 
		rect.SetRect(cursorPos,cursorPos+widthDiff+heightDiff);
	currentDrawable->DrawableGc().DrawRect(rect);
	coll.FlushIfOn();
	}
	
void OpCode::gLineBy(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPoint endPoint=COplDrawable::PopPoint(aStack);
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CurrentDrawable()->DrawLine(endPoint,ETrue);
	coll.FlushIfOn();
	}

void OpCode::gLineTo(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPoint endPoint=COplDrawable::PopPoint(aStack);
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CurrentDrawable()->DrawLine(endPoint,EFalse);
	coll.FlushIfOn();
	}
	
void OpCode::gBorder(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numOfArgs = aRuntime.IP8();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TSize size = currentDrawable->Size();
	TPoint origin(0,0);
	if (numOfArgs == 3)
		{
		size = COplDrawable::PopSize(aStack);
		origin=currentDrawable->CursorPosition();
		}
    TInt flag = aStack.PopInt16();
	if ((flag&0xff)>4)
		User::Leave(KErrArgument);
	currentDrawable->DrawS3Border(flag,TRect(origin,size));
	coll.FlushIfOn();
	}
	
void OpCode::gUse(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt drawableId = aStack.PopInt16();
	aRuntime.DrawablesCollection().SetCurrentDrawableL(drawableId);
    aRuntime.ClearTrap();
	}
	
void OpCode::gCls(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		((COplBitmap*)currentDrawable)->ClearBitmap();
	else
		((COplWindow*)currentDrawable)->ClearWindow();
	currentDrawable->SetCursorPosition(KScreenOrigin);
	coll.DrawAllClocks();
	coll.FlushIfOn();
	}		   

LOCAL_C void DoEllipse(COplRuntime& aRuntime,TInt aDx,TInt aDy,TBool aFill)
	{
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	currentDrawable->SetGcMode(currentDrawable->CurrentDrawMode());
	TPoint center = currentDrawable->CursorPosition();
	CBitmapContext& gc=currentDrawable->DrawableGc();
	if (aFill)
		{
		currentDrawable->SetGcModeInvalid();
		gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
		if (currentDrawable->CurrentDrawMode()!=EModeClear)
			gc.SetBrushColor(currentDrawable->ForegroundColor());
		}
	gc.DrawEllipse(TRect(TPoint(center.iX-aDx,center.iY-aDy),TSize(2*aDx,2*aDy)));
	coll.FlushIfOn();
	}
	
void OpCode::gEllipse(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBool fill=(aRuntime.IP8()==0)?EFalse:(aStack.PopInt16()!=0);
	TInt dY = aStack.PopInt16();
	TInt dX = aStack.PopInt16();
	DoEllipse(aRuntime,dX,dY,fill);
	}
	
void OpCode::gCircle(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBool fill=(aRuntime.IP8()==0)?EFalse:(aStack.PopInt16()!=0);
	TInt16 radius = aStack.PopInt16();
	DoEllipse(aRuntime,radius,radius,fill);
    }

void OpCode::gSetPenWidth(CStack& aStack,COplRuntime& aRuntime,CFrame* /*aFramePtr*/)
	{
	TInt size=aStack.PopInt16();
	aRuntime.DrawablesCollection().CurrentDrawable()->DrawableGc().SetPenSize(TSize(size,size));
	}
	
void OpCode::gVisible(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	TInt visibleArg = aRuntime.IP8();
	TBool visible = EFalse; // not visible
	if (visibleArg == 1)
		visible = ETrue;
	((COplWindow*)currentDrawable)->Window().SetVisible(visible);
	coll.FlushIfOn();
	}
	
void OpCode::gSetWin(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	TInt numOfArgs = aRuntime.IP8();
	if (numOfArgs == 4)
		currentDrawable->SetSizeL(COplDrawable::PopSize(aStack));
	((COplWindow*)currentDrawable)->SetWinPosition(COplDrawable::PopPoint(aStack));
	coll.FlushIfOn();
	}
	
void OpCode::gOrder(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt ordinalPos = aStack.PopInt16();
	TInt drawableId = aStack.PopInt16();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* drawable = coll.DrawableL(drawableId);
	if (drawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	((COplWindow*)drawable)->Window().SetOrdinalPosition((ordinalPos)?ordinalPos-1:0);
	coll.FlushIfOn();
	}
	
void OpCode::gClose(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt drawableId = aStack.PopInt16();
	if (drawableId==1) // can not close the console
		User::Leave(KErrArgument);
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CloseDrawableL(drawableId);
    aRuntime.ClearTrap();
	coll.FlushIfOn();
	}

_LIT(KFormatDecimal,"%d");
	
void OpCode::gPrintWord(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<16> buf;								    
	buf.Format(KFormatDecimal,aStack.PopInt16());

	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CurrentDrawable()->PrintAndMoveCursor(buf);
	coll.FlushIfOn();
	}
	
void OpCode::gPrintLong(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<16> buf;
	buf.Format(KFormatDecimal,aStack.PopInt32());
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CurrentDrawable()->PrintAndMoveCursor(buf);
	coll.FlushIfOn();
	}
	
void OpCode::gPrintDbl(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TReal value = aStack.PopReal64();
	TReal* pValue;
	pValue = &value;
	TBuf<16> buf;
	TInt flags=aRuntime.UserFlags();
	TOplRealFormat format(flags,16);
	buf.Num(*pValue,format);
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CurrentDrawable()->PrintAndMoveCursor(buf);
	coll.FlushIfOn();
	}
	
void OpCode::gPrintStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPtrC buf = aStack.PopString();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CurrentDrawable()->PrintAndMoveCursor(buf);
	coll.FlushIfOn();
	}
	
void OpCode::gPrintSpace(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	_LIT(KCharSpace," ");
	TPtrC buf=KCharSpace();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.CurrentDrawable()->PrintAndMoveCursor(buf);
	coll.FlushIfOn();
	}
	
void OpCode::gSaveBit(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numOfArgs = aRuntime.IP8();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TSize size(0,0);
	if (numOfArgs == 1)
		size = COplDrawable::PopSize(aStack);
	TPtrC fileName = aStack.PopString();
	currentDrawable->StoreBitmapL(aRuntime,fileName,size);
    aRuntime.ClearTrap();
	coll.FlushIfOn();
	}
	
void OpCode::gGMode(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt mode = aStack.PopInt16();
	TOplDrawMode drawMode = COplDrawable::GetOplDrawMode(mode);
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	currentDrawable->SetDrawMode(drawMode);
	// flushed before next drawing
	}
	
void OpCode::gTMode(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt mode = aStack.PopInt16();
	TOplDrawMode textMode = COplDrawable::GetOplDrawMode(mode);
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	currentDrawable->SetTextMode(textMode);
	// flushed before drawing
	}
	
void OpCode::gFill(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{			  
	TInt mode = aStack.PopInt16();
	if ((mode < NULL) || (mode > 2))
		User::Leave(KOplErrInvalidArgs);
	CGraphicsContext::TDrawMode drawMode = CGraphicsContext::EDrawModePEN;  // if mode =0 or mode=1
	if (mode == 2)
		drawMode = CGraphicsContext::EDrawModeNOTSCREEN;		
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TRect rect(currentDrawable->CursorPosition(),COplDrawable::PopSize(aStack)); 
	TRgb color = currentDrawable->ForegroundColor();
	if (mode == 1)
		color = currentDrawable->BackgroundColor();
	currentDrawable->SetGcModeInvalid();	// setting drawable GC directly below
	CBitmapContext& gc = currentDrawable->DrawableGc();
	gc.SetBrushColor(color);	
	gc.SetPenColor(color); 
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);   
	gc.SetDrawMode(drawMode);						   
	gc.DrawRect(rect);
	coll.FlushIfOn();
	}
	
void OpCode::gPoly(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16* p = (TInt16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt)+sizeof(TPoint));
	// needed to restore cursor pos on completion
	TInt xStart=OplUtil::GetWord(p++);
	TInt yStart=OplUtil::GetWord(p++);
	TInt pairs=OplUtil::GetWord(p++);
	TPoint point1(xStart,yStart);
	const RHeap& heap=aRuntime.Heap64();
	if (pairs<0 || ((TUint8*)p)+(pairs*2*sizeof(TInt16))-heap.Base()>heap.Size())
		User::Leave(KOplErrInvalidArgs);  // accessing beyond the heap
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	currentDrawable->SetGcMode(currentDrawable->CurrentDrawMode());
	CBitmapContext& gc = currentDrawable->DrawableGc();
	while (pairs--)
		{
		// dy accessed next line
		TInt dx=OplUtil::GetWord(p++);
		TPoint point2=point1+TPoint(dx>>1,OplUtil::GetWord(p++));
		if (!(dx&1))	// bit clear to draw
			gc.DrawLine(point1,point2);
		point1=point2;
		}
	currentDrawable->SetCursorPosition(TPoint(xStart,yStart));
	coll.FlushIfOn();
	}

void OpCode::gStyle(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt style = aStack.PopInt16();
	CDrawablesCollection& collection = aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = collection.CurrentDrawable();
	if (style!=currentDrawable->CurrentStyle())
		{
		TAlgStyle algStyle;
		algStyle.SetWidthFactor(1);
		algStyle.SetHeightFactor(1);
		CBitmapContext& drawableGc = currentDrawable->DrawableGc();	
		if (style & KStyleBold)		// bold
			algStyle.SetIsBold(ETrue);
		
		if (style & KStyleUnderlined)		// underline
			drawableGc.SetUnderlineStyle(EUnderlineOn);
		else
			drawableGc.SetUnderlineStyle(EUnderlineOff);
		
/*		if (style & KStyleInverse)		// inverse
			currentDrawable->SetInverseStyle(ETrue);
		else
			currentDrawable->SetInverseStyle(EFalse);*/
		
		if (style & KStyleDblHeight)		// double height
			algStyle.SetHeightFactor(2);
		
		if (style & KStyleMonoSpaced)	 // mono-spaced characters
			algStyle.SetIsMono(ETrue);
		
		if (style & KStyleItalics)	  // italic
			algStyle.SetIsItalic(ETrue);

#if defined(__SERIES60__)
		TUid fontUid=OplUtil::MapFontId(currentDrawable->FontId(),algStyle);
		const CFont* font;
		TBool standardFont=OplUtil::IsStandardFontUid(fontUid);
		if (standardFont)
			font=OplUtil::MapFontUidToStandardFontL(fontUid);
		else
			font = collection.GetFontByIdL(fontUid,algStyle);
		currentDrawable->SetStyle(style);
		currentDrawable->SetFont((CFbsFont*)font,standardFont);
#else
		TUid fontId=OplUtil::MapFontId(currentDrawable->FontId(),algStyle);
		CFbsFont* font = collection.GetFontByIdL(fontId,algStyle);
		currentDrawable->SetStyle(style);
		currentDrawable->SetFont(font);
#endif
		}
	// flushed before next draw
	}
	
void OpCode::gInvert(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TPoint cursorPos = currentDrawable->CursorPosition();
	TRect rect(cursorPos,COplDrawable::PopSize(aStack));
	currentDrawable->SetGcModeInvalid();	// setting GC directly
	CBitmapContext& gc = currentDrawable->DrawableGc();
	gc.SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRoundRect(rect,TSize(3,3));
	coll.FlushIfOn();
	}
	
void OpCode::gScroll(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numOfArgs = aRuntime.IP8();
	TRect scrollRect;
	if (numOfArgs == 6)
		scrollRect = COplDrawable::PopRect(aStack);
	TInt dY = aStack.PopInt16();
	TInt dX = aStack.PopInt16();
/*	if (dY < NULL)
		scrollRect.iTl.iY += dY;
	else if (dY > NULL)
		scrollRect.iBr.iY += dY;
	if (dX < NULL)
		scrollRect.iTl.iX += dX;
	else if (dX > NULL)
		scrollRect.iBr.iX += dX;*/
	TPoint offset(dX,dY);
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	if (numOfArgs != 6)
		scrollRect=currentDrawable->Size();
	currentDrawable->Scroll(offset,scrollRect);
	coll.FlushIfOn();
	}
	
void OpCode::gCopy(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt mode = aStack.PopInt16();
	TRect sourceRect = COplDrawable::PopRect(aStack);
	TInt drawableId = aStack.PopInt16();
	
	CDrawablesCollection& collection = aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = collection.CurrentDrawable();
	COplDrawable* sourceDrawable = collection.DrawableL(drawableId);

	if (!sourceDrawable)
		User::Leave(KOplErrDrawNotOpen);
	CGraphicsContext::TDrawMode drawMode = CGraphicsContext::EDrawModePEN;
	switch(mode)
		{
	case 0 :
		drawMode = CGraphicsContext::EDrawModeAND;
		break;
	case 1 :
		drawMode = CGraphicsContext::EDrawModeORNOT;
		break;
	case 2 :
		drawMode = CGraphicsContext::EDrawModeNOTXOR;
		}
	currentDrawable->SetGcModeInvalid();	// setting GC directly
	currentDrawable->DrawableGc().SetDrawMode(drawMode);
	
	if (sourceDrawable->DrawableFlag() == EIsOplBitmap)
		{
		CFbsBitmap& backup = ((COplBitmap*)sourceDrawable)->FbsBitmap();
		currentDrawable->DrawableGc().BitBlt(currentDrawable->CursorPosition(),&backup,sourceRect);
		}
	else
		{
		CFbsBitmap* backup = new(ELeave) CFbsBitmap();
		CleanupStack::PushL(backup);
		((COplWindow*)sourceDrawable)->BackupBmpHandleL(*backup);
		currentDrawable->DrawableGc().BitBlt(currentDrawable->CursorPosition(),backup,sourceRect);
		CleanupStack::PopAndDestroy();
		}
    aRuntime.ClearTrap();
	collection.FlushIfOn();
    }
	
void OpCode::Cursor(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{									   
    TInt arg = aRuntime.IP8();
	CDrawablesCollection& collection = aRuntime.DrawablesCollection();
	TTextCursor cursor;
	TInt type = 0;	// default type - corresponds to ETypeRectangle
	TInt id = collection.DrawableWithCursor();
	switch (arg)
		{
	case 0 :
		aRuntime.Console().HideCursor();
		if (id>0)
			{
			((COplWindow*)collection.DrawableL(id))->SetCursorEnabled(EFalse);
			collection.SetDrawableWithCursor(-1);
			collection.FlushIfOn();
			}
		return;
	case 1 :
		collection.SetDrawableWithCursor(0);
		aRuntime.Console().CursorOn();
		collection.FlushIfOn();
		return;
	case 4 :
		type = aStack.PopInt16(); // fall through to case 3:
	case 3 :
		cursor.iHeight = aStack.PopInt16();
		cursor.iWidth = aStack.PopInt16();
		cursor.iAscent = aStack.PopInt16();
		break;
		}
	aRuntime.Console().HideCursor();
	TInt drawableId = aStack.PopInt16();
	COplDrawable* drawable = collection.DrawableL(drawableId);   
	// if the supplied drawable id is a drawable that is a bitmap - leave.	
	if (drawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	
	collection.SetDrawableWithCursor(drawableId);
	if (arg == 2)
		{
		cursor.iWidth = 2;	 // default width
		cursor.iHeight = drawable->Font()->HeightInPixels();	// default height
		cursor.iAscent = drawable->Font()->AscentInPixels();	// default ascent
		}

	cursor.iFlags = 0;
	cursor.iType = TTextCursor::ETypeRectangle;		// default cursor
	
//	if (type & KCursorObloid)
//		cursor.iType = TTextCursor::ETypeObloid;	// obloid shape

	if (type & KCursorNotFlashing)
		cursor.iFlags = TTextCursor::EFlagNoFlash;		// not flashing
	
	if (type & KCursorGray)														 
		cursor.iColor=KRgbGray; // grey color
	
	((COplWindow*)drawable)->SetCursorEnabled(ETrue);
	((COplWindow*)drawable)->DrawCursorIfOn(cursor);
	collection.FlushIfOn();
	}
	
void OpCode::gPatt(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt mode = aStack.PopInt16();
	TSize size = COplDrawable::PopSize(aStack);
	TInt drawableId = aStack.PopInt16();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	currentDrawable->SetGcModeInvalid();	// setting GC directly
	CBitmapContext& gc = currentDrawable->DrawableGc();
	CGraphicsContext::TDrawMode drawMode=CGraphicsContext::EDrawModeAND;
	switch(COplDrawable::GetOplDrawMode(mode))
		{
	case EModeSet:
		break;
	case EModeClear:
		drawMode=CGraphicsContext::EDrawModeORNOT;
		break;
	case EModeInvert:
		drawMode=CGraphicsContext::EDrawModeNOTXOR;
		break;
	case EModeReplace:
		drawMode=CGraphicsContext::EDrawModePEN;
		break;
	default:
		User::Leave(KErrArgument);
		}
	gc.SetDrawMode(drawMode);
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	if (drawableId == -1)
		{
		gc.SetBrushColor(currentDrawable->BackgroundColor());
		gc.SetPenColor(currentDrawable->ForegroundColor());
		gc.SetBrushStyle(CGraphicsContext::EDiamondCrossHatchBrush);
		gc.DrawRect(TRect(currentDrawable->CursorPosition(),size));
		}
	else
		{
		COplDrawable* sourceDrawable = aRuntime.DrawablesCollection().DrawableL(drawableId);
		if (!sourceDrawable)
			User::Leave(KOplErrDrawNotOpen);
		currentDrawable->SetPattern(sourceDrawable,size);
		}
    aRuntime.ClearTrap();
    coll.FlushIfOn();
	}
	
void OpCode::gUpdate(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt flag = aRuntime.IP8();
	RWsSession& wsSession = aRuntime.ConEnv()->WsSession();
	if (flag == 255)
		{
		wsSession.Flush();
		return;
		}
	TBool fl=ETrue;
	if (flag==NULL)
		fl=EFalse;
	aRuntime.DrawablesCollection().SetFlushState(flag);
    }

void FuncOpCode::gLoadFont(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPtrC fileName = aStack.PopString();
	TParse parse;
	User::LeaveIfError(aRuntime.ConEnv()->FsSession().Parse(fileName,parse));
	aStack.Push(aRuntime.DrawablesCollection().AddFontFileL(parse.FullName()));
    }

	
void OpCode::gUnloadFont(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.RemoveFontFile(aStack.PopInt16());	
    aRuntime.ClearTrap();
	// flushed before next draw
    }
	
void OpCode::gFont(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt fontId = aStack.PopInt32();
	CDrawablesCollection& collection = aRuntime.DrawablesCollection();
	TAlgStyle algStyle;
	algStyle.SetWidthFactor(1);
	algStyle.SetHeightFactor(1);
	COplDrawable* currentDrawable=collection.CurrentDrawable();
	TInt style=currentDrawable->CurrentStyle();
	
	if (style & KStyleBold)		// bold
		algStyle.SetIsBold(ETrue);

	if (style & KStyleDblHeight)		// double height
		algStyle.SetHeightFactor(2);
	
	if (style & KStyleMonoSpaced)	 // mono-spaced characters
		algStyle.SetIsMono(ETrue);
	
	if (style & KStyleItalics)	  // italic
		algStyle.SetIsItalic(ETrue);
	TUid fontUid=OplUtil::MapFontId(fontId,algStyle);

//#define __FONTBYNAME
#if defined(__FONTBYNAME)||defined(__UIQ__)
#pragma message("g_keywrd.cpp: !!TODO __FONTBYNAME is defined.")
#if defined(__UIQ__)	
// Partial solution for bug [ 910981 ] gFONT doesn't work on UIQ
// Rather than implementing the FontByName keyword, use the fontId to determine 
// whether to use the old Symbian Viking, or the more generic SwissA font.
// This is for the beta release only, and must be fixed before the final v1.00 release.
	TFontSpec spec(_L(""),0);
	if (fontId==KFontUiqSwissABeta)
		{
		spec=TFontSpec(_L("SwissA"),200); //height in twips
		}
	else
		{
		spec=TFontSpec(_L("Symbian Viking"),100);
		}
#else
	TFontSpec spec(_L("Courier"),300);
#endif
//	CFbsFont* font=aRuntime.ConEnv()->CreateScreenFontL(spec);
	CFbsFont* font = collection.GetFontByNameL(spec); // ,algStyle);
	currentDrawable->SetFont(font);
#else
#if defined(__SERIES60__)
	const CFont* font;
	TBool standardFont=OplUtil::IsStandardFontUid(fontUid);
	if (standardFont)
		font=OplUtil::MapFontUidToStandardFontL(fontUid);
	else
		font=collection.GetFontByIdL(fontUid,algStyle);
	currentDrawable->SetFont((CFbsFont*)font,standardFont);
#else
	CFbsFont* font=collection.GetFontByIdL(fontUid,algStyle);
	currentDrawable->SetFont(font);
#endif
#endif
	currentDrawable->SetFontId(fontId);
    aRuntime.ClearTrap();
	// flushed before next draw
    }

void OpCode::DefaultWin(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TDisplayMode mode = COplDrawable::DisplayMode(aStack.PopInt16());
	RBackedUpWindow win(aRuntime.ConEnv()->WsSession());
	CleanupClosePushL(win);
	User::LeaveIfError(win.Construct(aRuntime.ConEnv()->RootWin(),mode,1));
	TSize size=aRuntime.ConEnv()->ScreenDevice()->SizeInPixels();
	User::LeaveIfError(win.SetExtentErr(TPoint(0,0),size));
	win.SetOrdinalPosition(0,-1); // just less than default
	User::LeaveIfError(win.SetCornerType(EWindowCornerSquare));	// by default
	win.PointerFilter(EPointerFilterDrag|EPointerFilterMove|EPointerFilterEnterExit,0);
	win.Activate();
	STATIC_CAST(COplWindow*,aRuntime.DrawablesCollection().DrawableL(1))->UseNewWindow(win,mode); // console must exist
	CleanupStack::Pop();
	aRuntime.Console().UseNewWindow(win); // this one closes the old window
	}
	
void OpCode::gXBorder(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TInt numOfArgs = aRuntime.IP8();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TPoint cursorPos=currentDrawable->CursorPosition();
	currentDrawable->SetGcMode(currentDrawable->CurrentDrawMode()); //hp: otherwise gGMode 2: gXBorder has no effect
	currentDrawable->SetGcModeInvalid();	// setting GC directly below
	TPoint origin;
	TSize outerSize = currentDrawable->Size();
	if (numOfArgs == 4)
		{
		outerSize = COplDrawable::PopSize(aStack);
		origin =cursorPos;
		}
	TRect rect(origin,outerSize);
    TInt flag = aStack.PopInt16();
	TInt type = aStack.PopInt16();
	if ((type==0||type==1)&&(flag&0xff)>4)
		User::Leave(KErrArgument);
	switch (type)
		{
		case 0:
			{
			currentDrawable->DrawS3Border(flag,rect);
			break;
			}
		case 1:
			{
			TInt corner = 0;
			TSize cornerSize(4,4);
			if (flag&KMoreRoundedCorners)
				{
		//		flag -= KMoreRoundedCorners;
				corner = KMoreRoundedCorners;
				cornerSize.SetSize(6,6);
				}

			CBitmapContext& gc = currentDrawable->DrawableGc();
			if (flag&0x100)
				rect.Shrink(1,1);
		//	gc.DrawRect(rect);
  
			gc.SetPenColor(TRgb::Gray16(0));
			gc.DrawRoundRect(TRect(origin,outerSize),cornerSize);
			TSize innerSize = outerSize - TSize(8,8);
			currentDrawable->SetCursorPosition(origin+TPoint(4,4));
			flag&=0x7; // remove extra flags
			if ((flag==3) || (flag==4))
				{
				innerSize -= TSize(4,4);
				currentDrawable->SetCursorPosition(origin+TPoint(6,6));
				}
			gc.DrawRoundRect(TRect(currentDrawable->CursorPosition(),innerSize),cornerSize);
			currentDrawable->SetCursorPosition(origin);

			if ((flag==2) || (flag==4))
				currentDrawable->DrawGXBorderShadow(outerSize,corner,EFalse);
			else
				currentDrawable->DrawGXBorderShadow(outerSize,corner,ETrue);
			if ((flag==3) || (flag==4))
				{
				outerSize -= TSize(4,4);
				currentDrawable->SetCursorPosition(origin+TPoint(2,2));
				if (flag == 3)
					currentDrawable->DrawGXBorderShadow(outerSize,corner,ETrue);
				else
					currentDrawable->DrawGXBorderShadow(outerSize,corner,EFalse);
				}
			currentDrawable->SetCursorPosition(cursorPos);
			break;
			}
		case 2:
			{
			TGulBorder border((TGulBorder::TBorderType)flag);
			border.Draw(currentDrawable->DrawableGc(),rect);
			break;
			}
		default:
			User::Leave(KErrArgument);
		}
	coll.FlushIfOn();
    }

void OpCode::gXBorder32(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TInt numOfArgs = aRuntime.IP8();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TPoint cursorPos=currentDrawable->CursorPosition();
	currentDrawable->SetGcMode(currentDrawable->CurrentDrawMode()); //hp: otherwise gGMode 2: gXBorder has no effect
	currentDrawable->SetGcModeInvalid();	// setting GC directly below
	TPoint origin;
	TSize outerSize = currentDrawable->Size();
	if (numOfArgs == 3)
		{
		outerSize = COplDrawable::PopSize(aStack);
		origin =cursorPos;
		}
	TRect rect(origin,outerSize);
	TGulBorder::TBorderType flag = (TGulBorder::TBorderType)aStack.PopInt32();

	TGulBorder border(flag);
	border.Draw(currentDrawable->DrawableGc(),rect);

	coll.FlushIfOn();
    }

void OpCode::gXPrint(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TInt flag = aStack.PopInt16();
	if (flag<0||flag>6)
		User::Leave(KErrGeneral);
	TPtrC buf = aStack.PopString();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TOplDrawMode textMode=currentDrawable->CurrentTextMode();
	currentDrawable->SetTextMode(EModeReplace);
	TBool style = currentDrawable->CurrentStyle();
	TRect rect = currentDrawable->FontRect(buf);
	currentDrawable->SetGcModeInvalid();	// setting GC directly below
	CBitmapContext& gc = currentDrawable->DrawableGc();
	if (flag == 0)
		currentDrawable->Print(buf);
	else
		{
		TOplDrawMode currentTextMode = currentDrawable->CurrentTextMode();
		TOplDrawMode currentDrawMode = currentDrawable->CurrentDrawMode();
		if (style&KStyleInverse)
			{
			if ((flag==5) || (flag==6))
				{
				currentDrawable->SetInverseStyle(ETrue);
				currentDrawable->Print(buf);				// temporarily sets up inverse style
				currentDrawable->SetInverseStyle(EFalse);
				}
			else
				{
				currentDrawable->SetDrawMode(EModeGXPrint);
				currentDrawable->SetTextMode(EModeReplace);
				currentDrawable->Print(buf);
				currentDrawable->SetTextMode(currentTextMode);
				currentDrawable->SetDrawMode(currentDrawMode);
				}
			}
		else
			{
			if ((flag==5) || (flag==6))
				{
				currentDrawable->SetDrawMode(EModeGXPrint);
				currentDrawable->SetTextMode(EModeReplace);
				currentDrawable->Print(buf);
				currentDrawable->SetTextMode(currentTextMode);
				currentDrawable->SetDrawMode(currentDrawMode);
				}
			else
				{
				currentDrawable->SetInverseStyle(ETrue);  // temporarily sets up inverse style
				currentDrawable->Print(buf);
				currentDrawable->SetInverseStyle(EFalse);
				}
			}
		}
	if (((style&KStyleInverse)!=0)^(flag==0))
		gc.SetPenColor(currentDrawable->BackgroundColor());
	else
		gc.SetPenColor(currentDrawable->ForegroundColor());
	if ((flag==1) || (flag==2) || (flag==0))
		{
		rect.Grow(1,1);
		gc.SetBrushStyle(CGraphicsContext::ENullBrush);
		if (flag == 1)
			gc.DrawRect(rect);
		else
			gc.DrawRoundRect(rect,TSize(2,2));
		}
	else if ((flag==5) || (flag==6))
		{
		TInt fontDescent = currentDrawable->Font()->HeightInPixels()-currentDrawable->Font()->AscentInPixels();
		TPoint offset(0,fontDescent);
		if (flag==5)
			gc.DrawLine(currentDrawable->CursorPosition()+offset,rect.iBr);
		else
			{
			offset.SetXY(0,fontDescent-1);
			gc.DrawLine(currentDrawable->CursorPosition()+offset,TPoint(rect.iBr.iX,rect.iBr.iY-1));
			}
		}
	else if (flag==4)
		{
		TPoint topLeft = rect.iTl;
		TPoint offset(1,1);
		TPoint widthOffset(rect.Size().iWidth-1,0);
		TPoint heightOffset(0,rect.Size().iHeight-1);
		gc.SetPenColor(currentDrawable->BackgroundColor());
		gc.DrawLine(topLeft,topLeft-offset);
		gc.DrawLine(topLeft+widthOffset,topLeft+widthOffset-offset);
		gc.DrawLine(topLeft+heightOffset,topLeft+heightOffset+offset);
		gc.DrawLine(topLeft+widthOffset+heightOffset,topLeft+widthOffset+heightOffset-offset);
		}
	currentDrawable->SetTextMode(textMode);
	coll.FlushIfOn();
    }




/////////////////////////////////
////  FuncOpCode  functions  ////
/////////////////////////////////
LOCAL_C  void gCreateShare(TInt aGray,CStack& aStack, COplRuntime& aRuntime)
    {
	TInt visible = aStack.PopInt16();
	TRect winRect = COplDrawable::PopRect(aStack);
	TInt id = aRuntime.DrawablesCollection().NextAvailableId();
	COplWindow* newWindow = new(ELeave) COplWindow;
	CleanupStack::PushL(newWindow);
	newWindow->ConstructL(aRuntime,id,winRect.iTl,winRect.Size(),visible,aGray);
	aRuntime.DrawablesCollection().AddDrawableL(newWindow,id);
	CleanupStack::Pop();
	aStack.Push((TInt16)id);
    }

void FuncOpCode::gCreate(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{ // gCREATE(x%,y%,w%,h%,v%) 
    gCreateShare(0,aStack,aRuntime);
    }

void FuncOpCode::gCreateEnhanced(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{ // gCREATE(x%,y%,w%,h%,v%,m%)
	gCreateShare(aStack.PopInt16(),aStack,aRuntime);
    }
    

void FuncOpCode::gCreateBit(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	// gCreateBit(width%,height%[,mode%])    where mode% is the TDisplayMode gray setting
	//
	{
	TInt mode=0;		// 2-colour by default
	if (aRuntime.IP8()==3)
		mode=aStack.PopInt16();			// pop grey mode
	TSize size = COplDrawable::PopSize(aStack);
	TInt id = aRuntime.DrawablesCollection().NextAvailableId();
	COplBitmap* newBitmap = new(ELeave) COplBitmap;
	CleanupStack::PushL(newBitmap);
	newBitmap->ConstructL(size,COplDrawable::DisplayMode(mode));
	aRuntime.DrawablesCollection().AddDrawableL(newBitmap,id);
	CleanupStack::Pop();
	aStack.Push((TInt16)id);
	}

void FuncOpCode::gLoadBit(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numOfArgs = aRuntime.IP8();
	TInt write = 1; // default - file opened with read/write attribute
	TInt num = 0;  // only one bitmap - default
	if (numOfArgs > 2)
		num = aStack.PopInt16();
	if (numOfArgs > 1)
		write = aStack.PopInt16();
	TPtrC fileName = aStack.PopString();
	TParse parse;
	User::LeaveIfError(aRuntime.ConEnv()->FsSession().Parse(fileName,parse));
	TInt id = aRuntime.DrawablesCollection().NextAvailableId();
	COplBitmap* newBitmap = new(ELeave) COplBitmap;
	CleanupStack::PushL(newBitmap);
	newBitmap->ConstructL(&parse.FullName(),write,num);
	aRuntime.DrawablesCollection().AddDrawableL(newBitmap,id);
	CleanupStack::Pop();
	aStack.Push((TInt16)id);
	}

void FuncOpCode::gIdentity(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	aStack.Push((TInt16)aRuntime.DrawablesCollection().DrawableId(currentDrawable));
	}

void FuncOpCode::gX(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TInt16)aRuntime.DrawablesCollection().CurrentDrawable()->CursorPosition().iX);
	}

void FuncOpCode::gY(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TInt16)aRuntime.DrawablesCollection().CurrentDrawable()->CursorPosition().iY);
	}

void FuncOpCode::gWidth(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TInt16)aRuntime.DrawablesCollection().CurrentDrawable()->Size().iWidth);
	}

void FuncOpCode::gHeight(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TInt16)aRuntime.DrawablesCollection().CurrentDrawable()->Size().iHeight);
	}

void FuncOpCode::gOriginX(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	// Produce error if drawable is a bitmap...
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	aStack.Push((TInt16)((COplWindow*)currentDrawable)->Window().Position().iX);
	}

void FuncOpCode::gOriginY(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	// Produce error if drawable is a bitmap...
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	aStack.Push((TInt16)((COplWindow*)currentDrawable)->Window().Position().iY);
	}

void FuncOpCode::gRank(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	aStack.Push((TInt16)(((COplWindow*)currentDrawable)->Window().OrdinalPosition()+1));
	}

void FuncOpCode::gPixel(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPoint pixel = COplDrawable::PopPoint(aStack);

	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();

	CFbsBitmap* bitmap;
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		bitmap=&(((COplBitmap*)currentDrawable)->FbsBitmap());
	else
		{
		bitmap=new(ELeave) CFbsBitmap;
		CleanupStack::PushL(bitmap);
		((COplWindow*)currentDrawable)->BackupBmpHandleL(*bitmap);
		}
	TRgb color;
	bitmap->GetPixel(color,pixel);

	if (currentDrawable->DrawableFlag() != EIsOplBitmap) // must have created a bitmap
		CleanupStack::PopAndDestroy();

	aStack.Push((TInt32)((color.Red()*0x10000) + (color.Green()*0x100) + color.Blue()));
	}

void FuncOpCode::gPrintClip(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt width = aStack.PopInt16();
	TPtrC buf = aStack.PopString();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	CFbsFont* currentFont = currentDrawable->Font();
	TInt numOfChars = currentFont->TextCount(buf,width); // the number of characters 
	buf.Set(buf.Ptr(),numOfChars);
//	currentDrawable->PrintAndMoveCursor(buf);
	currentDrawable->Print(buf);
	TPoint curPos=currentDrawable->CursorPosition();
	curPos.iX+=width;
	currentDrawable->SetCursorPosition(curPos);
	aStack.Push((TInt16)numOfChars);
	coll.FlushIfOn();
	}

void FuncOpCode::gTWidth(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPtrC text = aStack.PopString();
	TInt width = aRuntime.DrawablesCollection().CurrentDrawable()->Font()->TextWidthInPixels(text);
	aStack.Push((TInt16)width);
	}

void OpCode::gPrintBoxText(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numOfArgs = aRuntime.IP8();
	TInt margin = 0;
	TInt bottom = 0;
	TInt top = 0;
	CGraphicsContext::TTextAlign alignment = CGraphicsContext::ELeft;
	if (numOfArgs > 4)
		margin = aStack.PopInt16();
	if (numOfArgs > 3)
		bottom = aStack.PopInt16();
	if (numOfArgs > 2)
		top = aStack.PopInt16();
	if (numOfArgs > 1)
		{
		TInt al = aStack.PopInt16();
		if (al < 1)
			User::Leave(KOplErrInvalidArgs);
		switch(al)
			{
		case 1 :
			alignment = CGraphicsContext::ERight;
			break;
		case 3 :
			alignment = CGraphicsContext::ECenter;
			break;
			}
		}
	TInt width = aStack.PopInt16();
	TPtrC buf = aStack.PopString();
	TRect textRect, overallRect;
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = coll.CurrentDrawable();
	TPoint cursorPos = currentDrawable->CursorPosition();
	CFbsFont* currentFont = currentDrawable->Font();
	TPoint topLeft(cursorPos.iX,cursorPos.iY-currentFont->AscentInPixels()-top);
	TSize size(width,currentFont->HeightInPixels()+top+bottom);
	overallRect.SetRect(topLeft,size);
	topLeft += TPoint(0,top);
	size -= TSize(0,(top+bottom));
	if (margin != NULL) 
		{
		if ((alignment == CGraphicsContext::ELeft) || 
		   ((alignment == CGraphicsContext::ECenter) && (margin > NULL)))
			topLeft.iX += margin;
		else if ((alignment == CGraphicsContext::ECenter) && (margin < NULL))
			margin=(-margin);
		size.iWidth -= margin;
		}
	textRect.SetRect(topLeft,size);
	CBitmapContext& gc = currentDrawable->DrawableGc();
	currentDrawable->SetGcModeInvalid();	// setting GC directly below
	TRgb foreground=currentDrawable->ForegroundColor();
	TRgb background=currentDrawable->BackgroundColor();
	if (currentDrawable->InverseStyle())
		{
		foreground=background;
		background=currentDrawable->ForegroundColor();
		}
	gc.SetPenColor(foreground);
	gc.SetBrushColor(background);
	gc.SetDrawMode(CGraphicsContext::EDrawModePEN);
	gc.SetPenStyle(CGraphicsContext::ENullPen);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(overallRect);
	gc.SetPenStyle(CGraphicsContext::ESolidPen);
	gc.SetBrushStyle(CGraphicsContext::ENullBrush);
	gc.DrawText(buf,textRect,currentDrawable->Font()->AscentInPixels(),alignment);
	coll.FlushIfOn();
	}
	
LOCAL_C CFbsBitmap* getBitmapIfOplDrawable(COplRuntime& aRuntime,CFbsBitmap* bitmap)
	{
	if (bitmap!=NULL && TInt(bitmap)<=KMaxDrawables)
		{
		COplDrawable* drawable=aRuntime.DrawablesCollection().DrawableL(TInt(bitmap));
		if (!drawable)
			User::Leave(KOplErrDrawNotOpen);
		if (drawable->DrawableFlag() == EIsOplBitmap)
			bitmap = &((COplBitmap*)drawable)->FbsBitmap();
		else
			User::Leave(KOplErrInvalidArgs);	// only bitmaps allowed
		}
	return bitmap;
	}


void OpCode::gButton(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
// gBUTTON text$,type%,width%,height%,state% [,bitmap&,mask&,layout%]
// Optionals pushed as last 3 args, IP8() is 3, 2, 1 if args passed or else 0
	{
	CFbsBitmap* bitmap=NULL;
	CFbsBitmap* mask=NULL;
	CEikCommandButton::TLayout layout = CEikCommandButton::ETextRightPictureLeft;
	CEikCommandButton::TExcess excess = CEikCommandButton::EShare;

	TInt optArgs=aRuntime.IP8();
	switch (optArgs)
		{
	case 3:
		{
		TInt16 oplLayout=aStack.PopInt16();
		layout=(CEikCommandButton::TLayout)(oplLayout & 0x0f);
		excess=(CEikCommandButton::TExcess)(oplLayout & 0xf0);
		}
	case 2:
		mask=(CFbsBitmap*)aStack.PopInt32();
		mask=getBitmapIfOplDrawable(aRuntime,mask);
	case 1:
		if ((bitmap=(CFbsBitmap*)aStack.PopInt32())==NULL)
			User::Leave(KOplErrInvalidArgs);
		bitmap=getBitmapIfOplDrawable(aRuntime,bitmap);
		break;
	default:
		break;
		}
	TInt state=aStack.PopInt16();
	TInt height=aStack.PopInt16();
	TInt width=aStack.PopInt16();
	TInt type=aStack.PopInt16();
	TPtrC text=aStack.PopString();
	COplDrawable* drawable=aRuntime.DrawablesCollection().CurrentDrawable();
	switch(type)
		{
	case 0:
		// just any ignore bitmaps
		drawable->DrawS3ButtonL(state,width,height,text);
		break;
	case 1:
		switch(state)
			{
		case 0:
		case 1:
			break;
// 2 is EIndeterminate.
//		case 2:
//			state=17;
//			break;
		default:
			User::Leave(KErrArgument);
			}
		// fall through to case 3:
	case 2:
		{
		if (drawable->DrawableFlag()==EIsOplBitmap)
			User::Leave(KOplErrInvalidWindow);
		CEikCommandButton* button=new(ELeave) CEikCommandButton;
		CleanupStack::PushL(button);
		button->CCoeControl::SetContainerWindowL(((COplWindow*)drawable)->Window()); 
		if (optArgs)
			{
			button->SetPictureL(bitmap,mask);
			button->Picture()->SetPictureOwnedExternally(ETrue);
			if (optArgs==3)
				{
				button->SetButtonLayout(layout);
				button->SetExcessSpace(excess);
				}
			if (text.Length()==0)
				button->SetDisplayContent(CEikCommandButton::EPictureOnly);
			}
		switch(state)
			{
		case 0:
		case 1:
			break;
		default:
			User::Leave(KErrArgument);
			}
		button->SetTextL(text);
		button->Label()->SetFont(drawable->Font());
		button->SetExtent(drawable->CursorPosition(),TSize(width,height));
		button->ActivateL();		
		button->SetState((CEikButtonBase::TState)state);
		button->DrawNow();
		CleanupStack::PopAndDestroy(); // button
		break;
		}
	default:
		User::Leave(KErrArgument);
		}
	// flushing needed?
	}




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  OTHER UI KEYWORDS  ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


void OpCode::Busy(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numOfArgs = aRuntime.IP8();
	TInt corner = 1;	// by default - will display message at bottom right of screen
	TInt delay = 0;
	TPtrC busyMsgString;
	if (numOfArgs > 2)		// ie numOfArgs == 3
		delay = aStack.PopInt16();
	if (numOfArgs > 1)		// ie numOfArgs == 2 or 3
		corner = aStack.PopInt16();
	if (numOfArgs>0)
		busyMsgString.Set(aStack.PopString());
	TInt len=busyMsgString.Length();
	if (len==0)
		{
		((CEikonEnv*)aRuntime.ConEnv())->BusyMsgCancel();
		return;
		}
	if (len>KEikBusyMsgMaxLen)
		User::Leave(KOplErrInvalidArgs);
	TGulAlignment alignment;
	COplDrawable::CalcMsgPosition(corner,alignment);
	((CEikonEnv*)aRuntime.ConEnv())->BusyMsgL(busyMsgString,alignment,TTimeIntervalMicroSeconds32(500000*delay));
	}
	
void OpCode::gIPrint(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numOfArgs = aRuntime.IP8();
	TInt corner = 3;
	if (numOfArgs == 1)
		corner = aStack.PopInt16();
	TPtrC infoMsgString = aStack.PopString();
	TGulAlignment alignment;
	COplDrawable::CalcMsgPosition(corner,alignment);
	((CEikonEnv*)aRuntime.ConEnv())->InfoMsgWithAlignment(alignment,infoMsgString);
    }

void OpCode::gInfo32(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TUint32* array=(TUint32*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TUint32)*KGInfo32ArraySize);
	array+=2; // first two positions in the array are unused

    CDrawablesCollection& collection = aRuntime.DrawablesCollection();
	COplDrawable* currentDrawable = collection.CurrentDrawable();
	CFbsFont* currentFont = currentDrawable->Font();
	TPoint cursorPos(0,0);
	TTextCursor cursor;
	TFontSpec fontSpec=currentFont->FontSpecInTwips();

	OplUtil::PutLong(array,currentFont->HeightInPixels());	// (3)
	OplUtil::PutLong(++array,(currentFont->HeightInPixels() - currentFont->AscentInPixels())); // (4)
	OplUtil::PutLong(++array,currentFont->AscentInPixels());	// (5)
	OplUtil::PutLong(++array,currentFont->MaxNormalCharWidthInPixels());	// (6)
	OplUtil::PutLong(++array,currentFont->MaxCharWidthInPixels());	// (7)

    TInt style = currentDrawable->CurrentStyle();
	TInt32 flags = (fontSpec.iTypeface.IsSymbol()?0:1);	// standard ASCII font
	if (style&KStyleBold)
		flags += 4;
	if (style&KStyleItalics)
		flags += 8;
	if (style&KStyleMonoSpaced)
		flags += 32;
	else
		flags += 16;

    OplUtil::PutLong(++array,flags);		// (8)
	
	OplUtil::PutLong(++array,currentDrawable->FontId());
	array++; // move to start next element
	Mem::FillZ(array,sizeof(TUint32)*8); // zero fill
	array+=7; // move pointer to next element

	OplUtil::PutLong(++array,currentDrawable->CurrentDrawMode());	// (18)
	OplUtil::PutLong(++array,currentDrawable->CurrentTextMode());	// (19)
	OplUtil::PutLong(++array,currentDrawable->CurrentStyle());		// (20)
	
	TBool cursorOn=EFalse;
	TInt32 drawableWithCursor=0;
	if (aRuntime.Console().IsCursorOn())
		{
		cursorOn=ETrue;
		drawableWithCursor=-1;
		}
	else
		{
		drawableWithCursor=collection.DrawableWithCursor(); // must be a window
		if (drawableWithCursor>0)
			{
			COplWindow* cursorWindow=STATIC_CAST(COplWindow*,collection.DrawableL(drawableWithCursor));
			cursorOn=cursorWindow->CursorState();
			if (cursorOn)
				{
				cursorPos=cursorWindow->CursorPosition();
				cursor=cursorWindow->Cursor();
				}
			}
		}
	OplUtil::PutLong(++array,cursorOn);
	OplUtil::PutLong(++array,drawableWithCursor);	// (22)
	OplUtil::PutLong(++array,cursor.iWidth);	// (23)
	OplUtil::PutLong(++array,cursor.iHeight);	// (24)
	OplUtil::PutLong(++array,cursor.iAscent);	// (25)		
	OplUtil::PutLong(++array,cursorPos.iX);		// (26)
	OplUtil::PutLong(++array,cursorPos.iY);		// (27)
	
    if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		OplUtil::PutLong(++array,1);					
	else						 // (28)
		OplUtil::PutLong(++array,0);
	TInt16 effects = 0;
// ETypeObloid dropped by WSERV
//	if (cursor.iType == TTextCursor::ETypeObloid)
//		effects = 1;
	if (cursor.iFlags&TTextCursor::EFlagNoFlash)
		effects += 2;
	TRgb black(0,0,0);
	if (cursor.iColor != black)
		effects += 4;
	OplUtil::PutLong(++array,effects);	// (29) 
	OplUtil::PutLong(++array,COplDrawable::DisplayMode(currentDrawable->GetDisplayMode())); // (30)
    OplUtil::PutLong(++array,currentDrawable->ForegroundColor().Red());  // (31)
	OplUtil::PutLong(++array,currentDrawable->ForegroundColor().Green());  // (32)
    OplUtil::PutLong(++array,currentDrawable->ForegroundColor().Blue());  // (33)
    OplUtil::PutLong(++array,currentDrawable->BackgroundColor().Red());  // (34)
    OplUtil::PutLong(++array,currentDrawable->BackgroundColor().Green());  // (35)
    OplUtil::PutLong(++array,currentDrawable->BackgroundColor().Blue());  // (36)
    }

// Changes by Arjen to allow gPeekLine to support ERgb color.
// See feature [734702] Graphics commands

// Changes removed since GetScanLine messes up the passed memory if the display mode 
// of the window!ERgb (only on S80). As an alternative, a new command gPIXEL was introduced.

void OpCode::gPeekLine(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt mode = aStack.PopInt16();
	if (mode<-1||mode>2)
		User::Leave(KErrArgument);
	TInt pixels = aStack.PopInt16();
	TInt bits=(pixels<<((mode==-1)?0:mode)); // bit count
	TInt words=(bits+15)/16; // word count (rounded up)
	TUint16* p=(TUint16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),words*sizeof(TUint16));
	TPtr8 ptrToBuf((TUint8*)p,words*2);
	TPoint start = COplDrawable::PopPoint(aStack);
	TInt drawableId = aStack.PopInt16();
	COplDrawable* drawable = aRuntime.DrawablesCollection().DrawableL(drawableId);
	TDisplayMode dispMode=EGray2;
	// set last word to all set so inversion sets to zero
	if (mode==-1)
		OplUtil::PutWord(&p[words-1],(TUint16)0xffff);
	else
		dispMode=COplDrawable::DisplayMode(mode);

	CFbsBitmap* bitmap;
	if (drawable->DrawableFlag() == EIsOplBitmap)
		bitmap=&(((COplBitmap*)drawable)->FbsBitmap());
	else
		{
		bitmap=new(ELeave) CFbsBitmap;
		CleanupStack::PushL(bitmap);
		((COplWindow*)drawable)->BackupBmpHandleL(*bitmap);
		}
	bitmap->GetScanLine(ptrToBuf,start,pixels,dispMode);
	if (mode==-1)
		{
		while (words--)
			{
			OplUtil::PutWord(p,(TUint16)~OplUtil::GetWord(p));
			++p;
			}
		}
	if (drawable->DrawableFlag() != EIsOplBitmap) // must have created a bitmap
		CleanupStack::PopAndDestroy();
    }

void OpCode::gColor(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TUint8 blue = (TUint8)aStack.PopInt16();
	TUint8 green = (TUint8)aStack.PopInt16();
	TUint8 red = (TUint8)aStack.PopInt16();
	TRgb color = TRgb(red,green,blue);
	//					red value				  
	aRuntime.DrawablesCollection().CurrentDrawable()->SetForegroundColor(color);
    }

void OpCode::gColorBackground(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TUint8 blue = (TUint8)aStack.PopInt16();
	TUint8 green = (TUint8)aStack.PopInt16();
	TUint8 red = (TUint8)aStack.PopInt16();
	TRgb color = TRgb(red,green,blue);
	//					red value				  
	aRuntime.DrawablesCollection().CurrentDrawable()->SetBackgroundColor(color);
    }

	
void OpCode::gGrey(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt mode = aStack.PopInt16();
	TRgb color=KRgbBlack;
	if (mode==1)
		color=KRgbGray;
	aRuntime.DrawablesCollection().CurrentDrawable()->SetForegroundColor(color);
    }

void OpCode::gClock(CStack& aStack , COplRuntime& aRuntime, CFrame* /*  */)
	{
    //((CEikonEnv*)aRuntime.ConEnv())->AlertWin(_L("GCLOCK not yet implemented"));
	// just fix stack for now
	TInt noOfParams=aRuntime.IP8();
	COplDrawable* drawable=aRuntime.DrawablesCollection().CurrentDrawable();
	if (drawable->DrawableFlag()==EIsOplBitmap)
		User::Leave(KOplErrInvalidWindow);
	TInt offset=0;
	TInt type=6; // default to this for now !!!
	TInt style=0;
	TInt fontId=0x9a; // default font
	TPtrC format;

	switch (noOfParams)
		{
    case 6:
		style=aStack.PopInt16();
		// fall through
    case 5:
		fontId=aStack.PopInt32();
		// fall through
    case 4:
		format.Set(aStack.PopString());
		// fall through
	case 3:
		offset=aStack.PopInt32();
		// fall through
	case 2:
		type=aStack.PopInt16();
		// fall through
	case 1:
		break;
    case 0: // gClock OFF
        ((COplWindow*)drawable)->DeleteClock();
        return;
    default:
		break;	// never happens
		}
	((COplWindow*)drawable)->DeleteClock(); // delete if exists
	if (!(type&0x100))
		offset*=60;
	type&=0xff;
	if (type==11)  // formatted clock used to be 10 but string format has changed
		{
		((COplWindow*)drawable)->CreateFormattedClockL(((CEikonEnv*)aRuntime.ConEnv())->ClockDllL(),offset,fontId,style,format);
		}
	else
		{
		((COplWindow*)drawable)->CreateClockL(*(CEikonEnv*)aRuntime.ConEnv(),type,offset);
		}
    }

void OpCode::gColorInfo(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	// gColorInfo cInfo&(3) where 1=TDisplayMode, 2=number of colors, 3=number of grays.
	TUint32* array=(TUint32*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TUint32)*KGColorInfoArraySize);
	TInt color=0;
	TInt gray=0;
	TDisplayMode dMode=aRuntime.ConEnv()->WsSession().GetDefModeMaxNumColors(color,gray);
	OplUtil::PutLong(array,dMode);//(1)
	OplUtil::PutLong(++array,color);//(2)
	OplUtil::PutLong(++array,gray);//(3)
    }



/* End of $Workfile G_KEYWRD.CPP$ */
