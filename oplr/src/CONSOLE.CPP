// CONSOLE.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <bldvariant.hrh>
#include <w32std.h>
#include <e32hal.h>
#if !defined(__SERIES60__)
#include <fontids.h>
#endif
#include <gdi.h>
#include <e32math.h>
#include <eikenv.h>

#include "iomain.h"
#include "console.h"
#include <oplerr.h>
#include "oplr.h"

#if defined(__SERIES60__)||defined(__UIQ__)
#include "oplutil.h"
#endif

COplConsole::COplConsole(RWsSession& aSession)
	:iWindow(aSession),iCurrentPos(1,1)
#if defined(__SERIES60__)||defined(__UIQ__)
	,iIsFontToBeReleased(ETrue) // Assume font is to be released
#endif
	{
	}

COplConsole::~COplConsole()
	{
	// Drop the loaded font.
	if (iLoadedConsoleFontId)
		iDevice->RemoveFile(iLoadedConsoleFontId);
	iWindow.Close();
	delete iGc;
	}

void COplConsole::ConstructL(CWsScreenDevice* aDevice,CWsEventHandler* aEventHandler,RWindowGroup& aParent,TDisplayMode aDisplayMode)
	{
	iEventHandler=aEventHandler;
	iDevice=aDevice;
	iRootWin=&aParent;
#if defined(__SERIES60__)
	// Fix defect 817867 "reset the default window" by making the Series 60 status pane 
	// invisible at startup.
	CEikStatusPane* statusPane=CEikonEnv::Static()->AppUiFactory()->StatusPane();
	if (statusPane)
		statusPane->MakeVisible(EFalse);
#endif
	User::LeaveIfError(iWindow.Construct(aParent,aDisplayMode,1));
	User::LeaveIfError(iDevice->CreateContext(iGc));
	iGc->Activate(iWindow);
	iGc->SetDrawMode(CGraphicsContext::EDrawModePEN);
	iGc->SetPenStyle(CGraphicsContext::ESolidPen);
	iGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	SetInverseAndUnderline(EFalse,EFalse);
	TAlgStyle style;
	style.SetWidthFactor(1);
	style.SetHeightFactor(1);

#if defined(__SERIES60__)
	User::LeaveIfError(SetStandardFont((CFont*)CEikonEnv::Static()->DenseFont(),style,EFalse,EFalse));
#elif defined(__UIQ__)
	User::LeaveIfError(SetStandardFont((CFont*)CEikonEnv::Static()->LegendFont(),style,EFalse,EFalse));
#else
	// Attempt to load the font by UID. If that fails, install it into FBSERV and try again.
	// ArjenB: 1.55 - always load the default font to prevent "Does not exist" or "Font not loaded" error messages
	// ArjenB:        in running OPL applications when the first instance of the runtime is unloaded
// TInt error=SetFontByUid(TUid::Uid(KScreenFontUidCourierNormal11),style,EFalse,EFalse); //HAS FontByName pair
//	if (error)
//		{
		_LIT(KConsoleFontFileName,"Eon14.gdr");
		TFindFile fontFile(TheRuntime()->ConEnv()->FsSession());
		// First attempt to look for it in \System\Libs\. We now install it there by default
		// because on Crystal EON14.GDR causes problems with checkboxes used in other
		// applications. It also causes problems for backup/install because it is constantly
		// marked as "In use". If we can't find it there, fall back to \System\Fonts\ which
		// allows us to still run on WINS by default.
		TInt error=fontFile.FindByDir(KConsoleFontFileName,KSystemLibsPath);
		if (error)
			{
			_LIT(KSystemFontsPath,"\\System\\Fonts\\");
			User::LeaveIfError(fontFile.FindByDir(KConsoleFontFileName,KSystemFontsPath));
			}
		// If we get to here, should be OK to Add to FBSERV
		User::LeaveIfError(iDevice->AddFile(fontFile.File(),iLoadedConsoleFontId));
		// Now FBSERV is aware of it, try to load it once more...
		User::LeaveIfError(SetFontByUid(TUid::Uid(KScreenFontUidCourierNormal11),style,EFalse,EFalse));
//		}
#endif
	iWindow.Activate();
	}

#if defined(__SERIES60__)||defined(__UIQ__)
TInt COplConsole::SetStyle(TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline)
	{
	TAlgStyle style=iFontStyle;
	style.SetIsItalic(aAlgStyle.IsItalic());
	if (!(iFontStyle==style))
		{
		const CFont* font;
		TBool standardFont=OplUtil::IsStandardFontUid(iFontUid);
		if (standardFont)
			font=OplUtil::MapFontUidToStandardFontL(iFontUid);
		else
			{
			TInt ret=iDevice->GetFontById((CFont*&)font,iFontUid,aAlgStyle); //NEEDS FontByName
			if (ret!=KErrNone)
				{
				if (ret==KErrNotFound || ret==KErrGeneral)
					ret=KOplErrFontNotLoaded;
				return ret;
				}
			}
		ReleaseCurrentFontIfNecessary();
		iGc->UseFont(font);
		iIsFontToBeReleased=(standardFont)?EFalse:ETrue;
		iFont=(CFont*)font;
		iFontStyle=style;
		// char size unchanged !?!
		}
	SetInverseAndUnderline(aInverse,aUnderline);
	return KErrNone;
	}


void COplConsole::ReleaseCurrentFontIfNecessary()
	{
	if (iFont)
		{
		if (iIsFontToBeReleased!=EFalse)
			{
			iDevice->ReleaseFont(iFont);
			iFont=NULL;
			iIsFontToBeReleased=EFalse;
			}
		}
	}
#else
TInt COplConsole::SetStyle(TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline)
	{
	TAlgStyle style=iFontStyle;
	style.SetIsItalic(aAlgStyle.IsItalic());
	if (!(iFontStyle==style))
		{
		CFont* font;
		TInt ret=iDevice->GetFontById(font,iFontUid,style);
		if (ret!=KErrNone)
			{
			if (ret==KErrGeneral)
				ret=KOplErrFontNotLoaded;
			return ret;
			}
		if (iFont)
			iDevice->ReleaseFont(iFont);
		iGc->UseFont(font);
		iFont=font;
		iFontStyle=style;
		// char size unchanged !?!
		}
	SetInverseAndUnderline(aInverse,aUnderline);
	return KErrNone;
	}
#endif

void COplConsole::SetInverseAndUnderline(TBool aInverse,TBool aUnderline)
	{
	if ((aInverse&&!(iFlags&EConsoleInverse))||((!aInverse)&&(iFlags&EConsoleInverse)))
		{ // need to set gc to aInverse
		if (aInverse)
			{
			iGc->SetBrushColor(KRgbBlack);
			iGc->SetPenColor(KRgbWhite);
			iFlags|=EConsoleInverse;
			}
		else
			{
			iGc->SetBrushColor(KRgbWhite);
			iGc->SetPenColor(KRgbBlack);
			iFlags&=~EConsoleInverse;
			}
		}
	if ((aUnderline&&!(iFlags&EConsoleUnderline))||((!aUnderline)&&(iFlags&EConsoleUnderline)))
		{ // need to set gc to use underline
		iGc->SetUnderlineStyle((aUnderline)?EUnderlineOn:EUnderlineOff);
		if (aUnderline)
			iFlags|=EConsoleUnderline;
		else
			iFlags&=~EConsoleUnderline;
		}
	}


#ifdef ___NOT_YET____
CFont* COplConsole::GetFontByNameL(TPtrC aFontName,TAlgStyle /*aAlgStyle*/,TBool aInverse,TBool aUnderline)
	{
	TFontSpec fontSpec(aFontName,200);
	CFont* font;
	TInt err=((CWsScreenDevice*)iDevice)->GetNearestFontInTwips(font,fontSpec);
	if (err!=KErrNone)
		{
		if (err==KErrNotFound)
			err=KOplErrFontNotLoaded;
		User::Leave(err);
		}
	return font;	
	}
#endif

#ifdef _NOT_YET_
// FontByName not currently used by OPLT.
TInt COplConsole::SetFontByName(TPtrC aFontName,TAlgStyle /*aAlgStyle*/,TBool aInverse,TBool aUnderline) //IS FontByName pair
	{
	TInt ret=ResizeWindow();
	if (ret!=KErrNone)
		return ret;
	TFontSpec fontSpec(aFontName,200);
	TFontStyle myStyle;
	fontSpec.iFontStyle=myStyle;
	//is it a standard font?
//	TBool standardFont=test if font is standard();
	CFont* font;
	TInt err=((CWsScreenDevice*)iDevice)->GetNearestFontInTwips(font,fontSpec);
	if (err!=KErrNone)
		{
		if (err==KErrNotFound)
			err=KOplErrFontNotLoaded;
		return err;
		}
	ReleaseCurrentFontIfNecessary();
	iGc->UseFont(font); //pukka because unused
	iFont=font;
//	iIsFontToBeReleased=(standardFont)?EFalse:ETrue;
//	iFontUid=aUid;
//	iFontStyle=aAlgStyle;
	TInt charWidth=iFont->MaxNormalCharWidthInPixels();
	if (charWidth==0)
		charWidth=iFont->MaxCharWidthInPixels();
	iCharSizeInPixels=TSize(charWidth,iFont->HeightInPixels());
	ResetScreenSize();
	SetCursorToFont();

	SetInverseAndUnderline(aInverse,aUnderline);
	iCurrentPos=TPoint(1,1);
	DrawCursorIfOn(iCurrentPos);
	return KErrNone;
	}
#endif


#if defined(__SERIES60__)||defined(__UIQ__)
TInt COplConsole::SetFontByUidL(TUid aUid,TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline) //NEEDS FontByName pair?
	{
	TInt ret=ResizeWindow();
	if (ret!=KErrNone)
		return ret;
	if (iFontUid!=aUid || !(aAlgStyle==iFontStyle))
		{ // need to change font
		const CFont* font;
		TBool standardFont=OplUtil::IsStandardFontUid(iFontUid);
		if (standardFont)
			font=OplUtil::MapFontUidToStandardFontL(iFontUid);
		else
			{
			ret=iDevice->GetFontById((CFont*&)font,aUid,aAlgStyle); //NEEDS FontByName
			if (ret!=KErrNone)
				{
				if (ret==KErrNotFound)
					ret=KOplErrFontNotLoaded;
				return ret;
				}
			}
		ReleaseCurrentFontIfNecessary();
		iGc->UseFont(font); //pukka
		iIsFontToBeReleased=(standardFont)?EFalse:ETrue;
		iFont=(CFont*)font;
		iFontUid=aUid;
		iFontStyle=aAlgStyle;
		TInt charWidth=iFont->MaxNormalCharWidthInPixels();
		if (charWidth==0)
			charWidth=iFont->MaxCharWidthInPixels();
		iCharSizeInPixels=TSize(charWidth,iFont->HeightInPixels());
		ResetScreenSize();
		SetCursorToFont();
		}
	SetInverseAndUnderline(aInverse,aUnderline);
	iCurrentPos=TPoint(1,1);
	DrawCursorIfOn(iCurrentPos);
	return KErrNone;
	}

/*
 * Use one of the CEikonEnv "built-in" fonts:
 */
TInt COplConsole::SetStandardFont(const CFont* aFont,TAlgStyle /*aAlgStyle*/,TBool aInverse,TBool aUnderline) //IS FontByName pair
	{
	TInt ret=ResizeWindow();
	if (ret!=KErrNone)
		return ret;
	ReleaseCurrentFontIfNecessary();
	iGc->UseFont(aFont); //pukka
	iIsFontToBeReleased=EFalse;//This is a standard font.
	iFont=(CFont*)aFont;
//	TInt charWidth=iFont->MaxNormalCharWidthInPixels();
	TInt charWidth=iFont->WidthZeroInPixels();
	if (charWidth==0)
		charWidth=iFont->MaxCharWidthInPixels();
	iCharSizeInPixels=TSize(charWidth,iFont->HeightInPixels());
	ResetScreenSize();
	SetCursorToFont();

	SetInverseAndUnderline(aInverse,aUnderline);
	iCurrentPos=TPoint(1,1);
	DrawCursorIfOn(iCurrentPos);
	return KErrNone;
	}
#else
TInt COplConsole::SetFontByUid(TUid aUid,TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline) //NEEDS FontByName pair?
	{
	TInt ret=ResizeWindow();
	if (ret!=KErrNone)
		return ret;
	if (iFontUid!=aUid || !(aAlgStyle==iFontStyle))
		{ // need to change font
		CFont* font;
		ret=iDevice->GetFontById(font,aUid,aAlgStyle);
		if (ret!=KErrNone)
			{
			if (ret==KErrNotFound)
				ret=KOplErrFontNotLoaded;
			return ret;
			}
		if (iFont)
			iDevice->ReleaseFont(iFont);
		iGc->UseFont(font);
		iFont=font;
		iFontUid=aUid;
		iFontStyle=aAlgStyle;
		TInt charWidth=iFont->MaxNormalCharWidthInPixels();
		if (charWidth==0)
			charWidth=iFont->MaxCharWidthInPixels();
		iCharSizeInPixels=TSize(charWidth,iFont->HeightInPixels());
		ResetScreenSize();
		SetCursorToFont();
		}
	SetInverseAndUnderline(aInverse,aUnderline);
	iCurrentPos=TPoint(1,1);
	DrawCursorIfOn(iCurrentPos);
	return KErrNone;
	}
#endif

TInt COplConsole::ResizeWindow()
	{
	TMachineInfoV1Buf machInfo;
	UserHal::MachineInfo(machInfo);
	TRect rect(TPoint(0,0),machInfo().iDisplaySizeInPixels);

	TInt ret=iWindow.SetExtentErr(rect.iTl,machInfo().iDisplaySizeInPixels);
	if (ret==KErrNone)
		DoClearRect(rect);
	return ret;
	}

void COplConsole::ResetScreenSize()
	{
	TSize windowSize(iWindow.Size());
	iScreenSizeInChars=windowSize;
	iScreenSizeInChars.iWidth/=iCharSizeInPixels.iWidth;
	iScreenSizeInChars.iHeight/=iCharSizeInPixels.iHeight;
	iTopLeft.iX=(windowSize.iWidth-(iScreenSizeInChars.iWidth*iCharSizeInPixels.iWidth))/2;
	iTopLeft.iY=(windowSize.iHeight-(iScreenSizeInChars.iHeight*iCharSizeInPixels.iHeight))/2;
	SetScreen(iScreenSizeInChars,TPoint(1,1)); // will not fail in this case
	}

TInt COplConsole::SetScreen(TSize aSizeInChars,TPoint aPosInChars)
	{
	if (aPosInChars==TPoint(0,0))
		{
		TSize size=(iScreenSizeInChars-aSizeInChars);
		aPosInChars.iX=size.iWidth/2+1;
		aPosInChars.iY=size.iHeight/2+1;
		}
	TRect rect=TRect(aPosInChars,aSizeInChars);
	if (aPosInChars.iX<1 || aPosInChars.iY<1 || aSizeInChars.iWidth<1 || aSizeInChars.iHeight<1 ||
		iScreenSizeInChars.iWidth<rect.iBr.iX-1 ||
		iScreenSizeInChars.iHeight<rect.iBr.iY-1)
		return KErrArgument;
	iScreenRectInChars=rect;
	iGc->SetClippingRect(CharsToPixels(TRect(TPoint(1,1),aSizeInChars))); // only allow drawing to screen area
	return KErrNone;
	}

TRect COplConsole::CharsToPixels(const TRect& aCharRect)
	{
	TInt charWidth=iCharSizeInPixels.iWidth;
	TInt charHeight=iCharSizeInPixels.iHeight;
	TPoint topLeft(iTopLeft.iX+(iScreenRectInChars.iTl.iX-1)*charWidth,iTopLeft.iY+(iScreenRectInChars.iTl.iY-1)*charHeight);
	TPoint tl((topLeft.iX+(aCharRect.iTl.iX-1)*charWidth),(topLeft.iY+(aCharRect.iTl.iY-1)*charHeight));
	TPoint br((topLeft.iX+(aCharRect.iBr.iX-1)*charWidth),(topLeft.iY+(aCharRect.iBr.iY-1)*charHeight));
	return TRect(tl,br);
	}

void COplConsole::ScreenInfo(SScreenInfo& aScreenInfo)
	{
	aScreenInfo.iLeftInPixels=(TUint16)iTopLeft.iX;
	aScreenInfo.iTopInPixels=(TUint16)iTopLeft.iY;
	TSize size=iScreenRectInChars.Size();
	aScreenInfo.iTextWidth=(TUint16)size.iWidth;
	aScreenInfo.iTextHeight=(TUint16)size.iHeight;
	aScreenInfo.iWindowId=1;
	aScreenInfo.iNotUsed=0;
	aScreenInfo.iCharWidth=(TUint16)iCharSizeInPixels.iWidth;
	aScreenInfo.iCharHeight=(TUint16)iCharSizeInPixels.iHeight;
	aScreenInfo.iFontUid=iFontUid.iUid;
	}

void COplConsole::DoClearRect(const TRect& aRectInPixels)
	{
	if (iFlags&EConsoleInverse)
		iGc->SetBrushColor(KRgbWhite);
	iGc->Clear(aRectInPixels);
	if (iFlags&EConsoleInverse)
		iGc->SetBrushColor(KRgbBlack);
	}

void COplConsole::Cls()
	{
	DoClearRect(CharsToPixels(TRect(TPoint(1,1),iScreenRectInChars.Size())));
	iCurrentPos=TPoint(1,1);
	DrawCursorIfOn(iCurrentPos);
	}

void COplConsole::ClearRect(const TRect& aRectInChars)
	{
	DoClearRect(CharsToPixels(aRectInChars));
	}

TPoint COplConsole::CurrentPos() const
	{
	return iCurrentPos;
	}

TInt COplConsole::At(const TPoint& aPos)
	{
	if (aPos.iX<1 || aPos.iX>iScreenRectInChars.Size().iWidth || aPos.iY<1 || aPos.iY>iScreenRectInChars.Size().iHeight)
		return KErrArgument;
	iCurrentPos=aPos;
	DrawCursorIfOn(iCurrentPos);
	return KErrNone;
	}

void COplConsole::BackSpace()
	{
	if (iCurrentPos.iX>1)
		iCurrentPos.iX--;
	}

void COplConsole::Cr()
	{
	iCurrentPos.iX=1;
	}

void COplConsole::Lf()
	{
	ScrollIfNeeded();
	iCurrentPos.iY++;
	}

void COplConsole::Tab()
	{
	ScrollIfNeeded();
	iCurrentPos.iX=((iCurrentPos.iX+(KTabStop-1))&~(KTabStop-1))+1;
	WrapIfNeeded();
	}

void COplConsole::ScrollIfNeeded()
	{
	if (iCurrentPos.iX>iScreenRectInChars.Size().iWidth) // should only be possible if last line wrap
		{
		Cr();
		Lf();
		}
	if (iCurrentPos.iY>iScreenRectInChars.Size().iHeight)
		{
		if (iFlags&EConsoleScrollLock)
			{
			iCurrentPos.iY=iScreenRectInChars.Size().iHeight; // set to start of last row
			Cr();
			}
		else
			Scroll();
		}
	}

void COplConsole::WrapIfNeeded()
	{
	if (iCurrentPos.iX>iScreenRectInChars.Size().iWidth)
		{
		if (iFlags&EConsoleWrapLock)
			iCurrentPos.iX=iScreenRectInChars.Size().iWidth; // set to end of row
		else if (!(iFlags&EConsoleLastLineWrap) && iCurrentPos.iY==iScreenRectInChars.Size().iHeight)
			iCurrentPos.iX=iScreenRectInChars.Size().iWidth+1; // just off the end of the line
		else
			{ // set to start of next row
			Cr();
			Lf();
			}
		}
	}

void COplConsole::Beep()
	{
	CEikonEnv::Beep();
	}

void COplConsole::Scroll()
	{
	TInt scrollHeight=iCharSizeInPixels.iHeight;
	TRect area=CharsToPixels(TRect(TPoint(1,1),iScreenRectInChars.Size()));
	area.iTl.iY+=scrollHeight;
	iWindow.Scroll(TPoint(0,-scrollHeight),area);
	area.iTl.iY=area.iBr.iY-scrollHeight;
	DoClearRect(area);
	iCurrentPos.iY-=1; // done here or in ScrollIfNeeded !?!
	}

void COplConsole::DoScrollClear(const TPoint& aOffset,const TRect& aArea)
	{
	if (iFlags&EConsoleInverse)
		iGc->SetBrushColor(KRgbWhite);
	TInt absDX=Abs(aOffset.iX);
	TInt absDY=Abs(aOffset.iY);
	if (absDX>aArea.Size().iWidth||absDY>aArea.Size().iHeight)
		iGc->Clear(aArea); // no overlap
	else
		{
		TSize size(absDX,aArea.Size().iHeight);
		if (aOffset.iX>0)
			iGc->Clear(TRect(aArea.iTl,size));
		else if (aOffset.iX<0)
			iGc->Clear(TRect(TPoint(aArea.iBr.iX+aOffset.iX,aArea.iTl.iY),size));
		size.SetSize(aArea.Size().iWidth,absDY);
		if (aOffset.iY>0)
			iGc->Clear(TRect(aArea.iTl,size));
		else if (aOffset.iY<0)
			iGc->Clear(TRect(TPoint(aArea.iTl.iX,aArea.iBr.iY+aOffset.iY),size));
		}
	if (iFlags&EConsoleInverse)
		iGc->SetBrushColor(KRgbBlack);
	}

void COplConsole::ScrollRect(const TPoint& aOffsetInChars,const TRect& aRectInChars)
	{
	TInt charWidth=iCharSizeInPixels.iWidth;
	TInt charHeight=iCharSizeInPixels.iHeight;
	TPoint offset(aOffsetInChars.iX*charWidth,aOffsetInChars.iY*charHeight);
	TRect rect=CharsToPixels(aRectInChars);
	iWindow.Scroll(offset,rect);
	DoScrollClear(offset,rect);
	}

void COplConsole::Print(const TDesC& aText)
	{
	TInt remaining=aText.Length();
	const TText* text=aText.Ptr();
	TInt lineLength=iScreenRectInChars.Size().iWidth;
	while (remaining>0)
		{
		TInt len=0;
		TPtrC ptr(text,len);
		ScrollIfNeeded();
		while (remaining>0 && len<lineLength-iCurrentPos.iX+1) // chars left on line including current pos
			{
			if (TChar(*text).IsControl())
				{
				if (len!=0)
					{
					ptr.Set(ptr.Ptr(),len);
					DoPrint(ptr);
					len=0;
					}
				switch (*text)
					{
					case 0x7:
						Beep();
						break;
					case 0x8:
						BackSpace();
						break;
					case 0x9:
						Tab();
						break;
					case 0xa:
						Lf();
						break;
					case 0xc:
						Ff();
						break;
					case 0xd:
						Cr();
						break;
					}
				remaining--;
				text++;
				ptr.Set(text,len);
				continue;
				}
			len++;
			remaining--;
			text++;
			}
		if (len!=0)
			{
			ptr.Set(ptr.Ptr(),len);
			DoPrint(ptr);
			}
		}
	DrawCursorIfOn(iCurrentPos);
	}

void COplConsole::DoPrint(const TDesC& aText)
	{
	ScrollIfNeeded();
	DrawText(aText,iCurrentPos);
	iCurrentPos.iX+=aText.Length();
	WrapIfNeeded();
	}

void COplConsole::DrawText(const TDesC& aText,const TPoint& aPosInChars)
	{
	TRect rect(TPoint(aPosInChars.iX,aPosInChars.iY),TSize(aText.Length(),1));
	iGc->DrawText(aText,CharsToPixels(rect),iFont->AscentInPixels());
	}

void COplConsole::SetCursorToFont()
	{
	iCursor.iType=TTextCursor::ETypeRectangle;
	iCursor.iHeight=iCharSizeInPixels.iHeight;
	iCursor.iWidth=iCharSizeInPixels.iWidth;
	iCursor.iAscent=iFont->AscentInPixels();
	iCursor.iFlags=0;
	iCursor.iColor=KRgbWhite;
	}

void COplConsole::CursorOn()
	{
	iFlags|=EConsoleCursorOn;
	DrawCursorIfOn(iCurrentPos);
	}

void COplConsole::DrawCursorIfOn(const TPoint& aPos)
	{
	if (iFlags&EConsoleCursorOn)
		{
		DoDrawCursor(aPos);
		}
	}

void COplConsole::DoDrawCursor(const TPoint& aPos)
	{
	TInt charWidth=iCharSizeInPixels.iWidth;
	TInt charHeight=iCharSizeInPixels.iHeight;
	TPoint topLeft(iTopLeft.iX+(iScreenRectInChars.iTl.iX-1)*charWidth,iTopLeft.iY+(iScreenRectInChars.iTl.iY-1)*charHeight);
	TPoint pos((topLeft.iX+(aPos.iX-1)*charWidth),(topLeft.iY+(aPos.iY-1)*charHeight+iCursor.iAscent));
	iRootWin->SetTextCursor(iWindow,pos,iCursor);
	}

void COplConsole::HideCursor()
	{
	iRootWin->CancelTextCursor();
	iFlags&=~EConsoleCursorOn;
	}

TInt COplConsole::Edit(CCoeEnv& aCoeEnv,TDes& aDes,TBool aTrap)
	{
	ScrollIfNeeded();
	TBuf<KOplMaxStrLen> buf(aDes);
	TInt maxLength=aDes.MaxLength();
	TInt width=iScreenRectInChars.Size().iWidth;

	TInt end=iCurrentPos.iX+maxLength;
	if (end>width+1)
		end=width+1;
	TPoint endPos(end,iCurrentPos.iY+1);
	ClearRect(TRect(iCurrentPos,endPos));

DrawAtEnd:	
	TPoint pos(iCurrentPos);
	TInt posInString=0;
	TInt availableWidth=width-iCurrentPos.iX+1;
	TInt bufLen=buf.Length();
	if (bufLen>=availableWidth)
		posInString=bufLen-availableWidth+1;
	DrawString(pos,posInString,buf);
	TPoint eol(width+1,pos.iY+1);
	ClearRect(TRect(pos,TSize(1,1)));
	FOREVER
		{
		if (pos.iX>width)
			{
			pos.iX=width;
			ScrollRect(TPoint(-1,0),TRect(TPoint(iCurrentPos.iX+1,iCurrentPos.iY),eol));
			}
		DoDrawCursor(pos);
		TUint key=iEventHandler->DoGetUnmapped(aCoeEnv);
		switch (key)
			{
		case EKeyEnter:
			aDes.Copy(buf);
			if (!iFlags&EConsoleCursorOn)
				HideCursor();
			Cr();
			Lf();
			return KErrNone;
		case EKeyEscape:
			if (buf.Length()>0)
				{
				buf.SetLength(0);
				pos.iX=iCurrentPos.iX;
				posInString=0;
				ClearRect(TRect(pos,eol));
				}
			else if (aTrap)
				{
				if (!iFlags&EConsoleCursorOn)
					HideCursor();
				return KErrCancel;
				}
			break;
		case EKeyLeftArrow:
			if (posInString>0)
				{
				posInString--;
				if (pos.iX==iCurrentPos.iX)
					{
					ScrollRect(TPoint(1,0),TRect(pos,TPoint(eol.iX-1,eol.iY)));
					DrawText(buf.Mid(posInString,1),pos);
					}
				else
					pos.iX--;
				}
			break;
		case EKeyRightArrow:
			if (posInString<buf.Length())
				{
				posInString++;
				if (pos.iX==width)
					{
					ScrollRect(TPoint(-1,0),TRect(TPoint(iCurrentPos.iX+1,iCurrentPos.iY),eol));
					if (posInString<buf.Length())
						DrawText(buf.Mid(posInString,1),pos);
					}
				else
					pos.iX++;
				}
			break;
		case EKeyDelete:
			if (posInString==buf.Length())
				break;
			pos.iX++;
			posInString++;
			// fall through to EKeyBackspace
		case EKeyBackspace:
			if (posInString!=0)
				{
				buf.Delete(--posInString,1);
				ScrollRect(TPoint(-1,0),TRect(pos,eol));
				TInt eolCharPos=eol.iX-pos.iX+posInString;
				if (eolCharPos<buf.Length())
					DrawText(buf.Mid(eolCharPos,1),TPoint(eol.iX-1,eol.iY-1));
				pos.iX--;
				}
			break;
		case EKeyEnd:
			goto DrawAtEnd;
		case EKeyHome:
			{
			pos=iCurrentPos;
			posInString=0;
			TPoint tempPos(iCurrentPos);
			TInt temp=0;
			DrawString(tempPos,temp,buf);
			}
			break;
		default:
			{
			TChar c(key);
			if (c.IsPrint())
				{
				if (posInString==maxLength)
					{
					Beep();
					break;
					}
				TBuf<1> b;
				b.Append(c);
				buf.Insert(posInString++,b);
				ScrollRect(TPoint(1,0),TRect(pos,TPoint(eol.iX-1,eol.iY)));
				DrawText(b,pos);
				pos.iX++;
				}
			}
			}
		}
	}

void COplConsole::DrawString(TPoint& aPos,TInt& aPosInString,const TDesC& aBuf)
	{
	TInt bufLen=aBuf.Length();
	while (aPosInString<bufLen)
		{
		TChar c(aBuf[aPosInString]);
		if (!c.IsPrint())
			{
			switch(c)
				{
				case 0x9:
					break; // 9 gives right arrow
				case 0xa:
				case 0xd:
					c=0x7; // 7 is down arrow
					break;
				default:
					c=0xf; // centered dot
				}
			}
		TBuf<1> b;
		b.Append(c);
		DrawText(b,aPos);
		aPosInString++;
		aPos.iX++;
		}
	}

_LIT(KQuestionMark,"?");

void COplConsole::InputL(CCoeEnv& aCoeEnv,TInt16& aInt,TBool aTrap)
	{
	TBuf<KOplLongLen> buf;
	TInt16 temp;
	TReal64 val;
	FOREVER
		{
		TInt ret=Edit(aCoeEnv,buf,aTrap);
		if (ret!=KErrNone)
			User::Leave(KOplErrEsc);
		// check input
		TLex lex(buf);
		if (lex.Val(val)==KErrNone && lex.Remainder().Length()==0 && Math::Int(temp,val)==KErrNone)
			{
			aInt=temp;
			return;
			}
		else if (aTrap)
			User::Leave(KOplErrGenFail);
		Print(KQuestionMark);
		buf.SetLength(0);
		}
	}

void COplConsole::InputL(CCoeEnv& aCoeEnv,TInt32& aLong,TBool aTrap)
	{
	TBuf<KOplLongLen> buf;
	TInt32 temp;
	TReal64 val;
	FOREVER
		{
		TInt ret=Edit(aCoeEnv,buf,aTrap);
		if (ret!=KErrNone)
			User::Leave(KOplErrEsc);
		// check input
		TLex lex(buf);
		if (lex.Val(val)==KErrNone && lex.Remainder().Length()==0 && Math::Int(temp,val)==KErrNone)
			{
			aLong=temp;
			return;
			}
		else if (aTrap)
			User::Leave(KOplErrGenFail);
		Print(KQuestionMark);
		buf.SetLength(0);
		}
	}

void COplConsole::InputL(CCoeEnv& aCoeEnv,TReal64& aInt,TBool aTrap)
	{
	TBuf<KOplFloatLen> buf;
	TReal64 val;
	FOREVER
		{
		TInt ret=Edit(aCoeEnv,buf,aTrap);
		if (ret!=KErrNone)
			User::Leave(KOplErrEsc);
		// check input
		TLex lex(buf);
		if (lex.Val(val)==KErrNone && lex.Remainder().Length()==0)
			{
			aInt=val;
			return;
			}
		else if (aTrap)
			User::Leave(KOplErrGenFail);
		Print(KQuestionMark);
		buf.SetLength(0);
		}
	}

void COplConsole::SetFlag(TInt aFlag,TBool aSet)
	{
	if (aSet)
		iFlags|=aFlag;
	else
		iFlags&=~aFlag;
	}

void COplConsole::SetScrollLock(TBool aLock)
	{
	SetFlag(EConsoleScrollLock,aLock);
	}

void COplConsole::SetWrapLock(TBool aLock)
	{
	SetFlag(EConsoleWrapLock,aLock);
	}

void COplConsole::SetLastLineWrap(TBool aWrap)
	{
	SetFlag(EConsoleLastLineWrap,aWrap);
	}

void COplConsole::UseNewWindow(RBackedUpWindow& aNewWindow)
	{
	iGc->Deactivate();
	iWindow.Close();
	iWindow=aNewWindow;
	iGc->Activate(iWindow);
#if defined(__SERIES60__)
	TBool standardFont=OplUtil::IsStandardFontUid(iFontUid);//
	iGc->UseFont(iFont);
	iIsFontToBeReleased=(standardFont)?EFalse:ETrue;
#else
	iGc->UseFont(iFont);
#endif
	iGc->SetDrawMode(CGraphicsContext::EDrawModePEN);
	iGc->SetPenStyle(CGraphicsContext::ESolidPen);
	iGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	TBool inverse=iFlags&EConsoleInverse;
	TBool underline=iFlags&EConsoleUnderline;
	iFlags&=~(EConsoleInverse|EConsoleUnderline); // remove these as the gc has been reset
	SetInverseAndUnderline(inverse,underline);
	ResetScreenSize();
	iCurrentPos=TPoint(1,1);
	DrawCursorIfOn(iCurrentPos);
	}
