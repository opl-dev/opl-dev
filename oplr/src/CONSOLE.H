// CONSOLE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include <e32base.h>
#include <fntstore.h>

class RBackedUpWindow;
class CWsScreenDevice;
class CWsEventHandler;

struct SScreenInfo
	{
public:
	TUint16 iLeftInPixels;		// info(1) 
	TUint16 iTopInPixels;		// info(2)
	TUint16 iTextWidth;  		// info(3)   in character units
	TUint16 iTextHeight;  		// info(4)  in character units
	TUint16 iWindowId;			// info(5)
	TUint16 iNotUsed;			// info(6)
	TUint16	iCharWidth;	    // info(7)   in pixels
	TUint16	iCharHeight;	    // info(8)   in pixels
	TInt32	iFontUid;		// info(9) and info(10)
	};


const TInt KOplIntLen=6;
const TInt KOplLongLen=11;
const TInt KOplFloatLen=18;
const TInt KOplMaxStrLen=255;
const TInt KOplEvent16ArrayElems=6;
const TInt KOplEvent32ArrayElems=16;
const TInt KOplEditStartToScroll=11;

// these really belong in iomain.h from here
enum TOplModifiers
	{
	EOplModShift=2,
	EOplModCtrl=4,
	EOplModAlt=8,
	EOplModCapsLock=16,
	EOplModFunc=32
	};

class TOplEventBuf 
	{
public:
	TInt iCode;
	TInt iTimeStamp;
	TInt iScanOrWinId;
	TInt iModifiers;
	TInt iRepeat;
	};

#include <w32std.h>
class TOplPointerEventBuf 
	{
public:
	TInt iCode;
	TInt iTimeStamp;
	TInt iWinId;
	struct TPointerEvent iPointer;
	};
// to here

const TInt KTabStop=8;

class CCoeEnv;

class COplConsole : public CBase
	{
public:
	COplConsole(RWsSession& aSession);
	~COplConsole();
	void ConstructL(CWsScreenDevice* aDevice,CWsEventHandler* aEventHandler,RWindowGroup& aParent,TDisplayMode aDisplayMode=EGray4);
#if defined(__SERIES60__)||defined(__UIQ__)
	TInt SetFontByUidL(TUid aUid,TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline); //HAS FontByName pair
	TInt SetFontByName(TPtrC aFontname,TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline); //IS FontByName pair
	TInt SetStandardFont(const CFont *aFont,TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline); //IS FontByName pair
	void ReleaseCurrentFontIfNecessary();
#else
	TInt SetFontByUid(TUid aUid,TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline); //HAS FontByName pair
#endif
	TInt SetStyle(TAlgStyle aAlgStyle,TBool aInverse,TBool aUnderline);
	TInt SetScreen(TSize aSizeInChars,TPoint aPosInChars); // TPoint(0,0) centers the text screen on the display
	void ScreenInfo(SScreenInfo& aScreenInfo);
	void Cls();
	void ClearRect(const TRect& aRectinChars);
	TPoint CurrentPos() const;
	TInt At(const TPoint& aPos);
	void Print(const TDesC& aText);
	TInt Edit(CCoeEnv& aCoeEnv,TDes& aString,TBool aTrap);
	void InputL(CCoeEnv& aCoeEnv,TInt16& aInt,TBool aTrap);
	void InputL(CCoeEnv& aCoeEnv,TInt32& aLong,TBool aTrap);
	void InputL(CCoeEnv& aCoeEnv,TReal64& aReal,TBool aTrap);
	void Beep();
	void BackSpace();
	void Tab();
	void Lf();
	void Ff() {Cls();};
	void Cr();
	void Scroll();
	void HideCursor();
	void CursorOn();
	TBool IsCursorOn() { return iFlags&EConsoleCursorOn; };
	void DrawCursorIfOn(const TPoint& aPos);
	void ScrollRect(const TPoint& aOffsetInChars,const TRect& aRectInChars);
	RBackedUpWindow& Window() {return iWindow;};
	void ScreenRectInChars(TRect& aRect) {aRect=iScreenRectInChars;};
	TSize CharSize() {return iCharSizeInPixels;};
	void SetScrollLock(TBool aLock);
	void SetWrapLock(TBool aLock);
	void SetLastLineWrap(TBool aWrap);
	void UseNewWindow(RBackedUpWindow& aNewWindow); // closes old window
private:
	// will need some internal helper functions
	void SetInverseAndUnderline(TBool aInverse,TBool aUnderline);
	TInt ResizeWindow();
	void ResetScreenSize();
	TRect CharsToPixels(const TRect& aRectInChars); // takes account of margins and SCREEN setting
	void ScrollIfNeeded();
	void WrapIfNeeded();
	void DoPrint(const TDesC& aText); // updates iCurrentPos
	void DoClearRect(const TRect& aRectInPixels);
	void SetCursorToFont();
	void DrawText(const TDesC& aText,const TPoint& aPosInChars);
	void DoDrawCursor(const TPoint& aPos);
	void DoScrollClear(const TPoint& aOffset,const TRect& aRect);
	void DrawString(TPoint& aPos,TInt& aPosInString,const TDesC& aBuf);
	void SetFlag(TInt aFlag,TBool aSet);
private:
	enum
		{
		EConsoleCursorOn=0x01,
		EConsoleInverse=0x02,
		EConsoleUnderline=0x04,
		EConsoleScrollLock=0x08,
		EConsoleWrapLock=0x10,
		EConsoleLastLineWrap=0x20
		};
	TInt iFlags;
	RBackedUpWindow iWindow;
	RWindowGroup* iRootWin;
	TSize iScreenSizeInChars;
	TPoint iTopLeft;
	TSize iCharSizeInPixels;
	TRect iScreenRectInChars;
	TPoint iCurrentPos;
	CWsScreenDevice* iDevice;
	CWindowGc* iGc;
	CWsEventHandler* iEventHandler;
	TTextCursor iCursor;
	CFont* iFont;
#if defined(__SERIES60__)||defined(__UIQ__)
	TBool iIsFontToBeReleased; // Don't try and unload these 'built-in' fonts...
#endif
	TUid iFontUid;
	TAlgStyle iFontStyle;
	TInt iLoadedConsoleFontId;	// Font id of installed font file, if any.
	};

#endif
