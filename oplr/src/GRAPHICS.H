// GRAPHICS.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//



#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <bldvariant.hrh>
#include <coemain.h>
#include <coecntrl.h>
#include <bitstd.h>
#include <eikcmbut.h>
#include <eikclock.h>


const TInt KStyleNormal = 0;
const TInt KStyleBold = 1;
const TInt KStyleUnderlined = 2;
const TInt KStyleInverse = 4;
const TInt KStyleDblHeight = 8;
const TInt KStyleMonoSpaced = 16;
const TInt KStyleItalics = 32; 

const TInt KMaxFontFiles = 16;
const TInt KMaxDrawables = 64;
const TInt KDiffBtnIdAndPos = 1;
const TInt KMoreRoundedCorners = 0x200;
const TInt KOplWindowDisplayModeMask = 0x000f;
const TInt KOplWindowShadowOnMask = 0x00f0;
const TInt KOplWindowShadowMask = 0xff00;
const TInt KOplShadowHeight = 1;
#define KScreenOrigin TPoint(0,0)

class COplRuntime;	// forward decl
class CStack;
class CEikonEnv;

enum TDrawableFlag {EIsOplBitmap, EIsOplWindow, EIsOplConsole};
enum TOplDrawMode
	{
	EModeSet,
	EModeClear,
	EModeInvert,
	EModeReplace,
	EModeInverseStyle,
	EModeGXPrint,
	EOplGcModeInvalid
	};

class COplDrawable	: public CBase
	{
public:
	COplDrawable();
	~COplDrawable();
	virtual TSize Size() const {return iSize;}
	virtual void SetSizeL(const TSize& aSize)=0;
	virtual void SetCursorPosition(const TPoint& aPosition) {iCursorPosition = aPosition;}
	virtual void StoreBitmapL(COplRuntime& aRuntime, const TDesC& aFileName, TSize& aSize)=0;
	virtual void ReleaseFont()=0;
	virtual void Scroll(const TPoint& aOffset,const TRect& aArea)=0;
	virtual TInt BitmapHandle()=0;
	
	void SetDrawMode(TOplDrawMode aDrawMode);
	TOplDrawMode CurrentDrawMode() const {return iCurrentDrawMode;}
	void SetTextMode(TOplDrawMode aTextMode);
	TOplDrawMode CurrentTextMode() const {return iCurrentTextMode;}
	void SetStyle(TInt aStyle) {iStyle = aStyle;}
	TInt CurrentStyle() const {return iStyle;}
	TDrawableFlag DrawableFlag() const {return iDrawableFlag;}
	TPoint CursorPosition() const {return iCursorPosition;}
	TRgb BackgroundColor() const {return iBackgroundColor;}
	void SetBackgroundColor(TRgb& aColor) {iBackgroundColor = aColor; SetGcModeInvalid();}
	TRgb ForegroundColor() const {return iForegroundColor;}
	void SetForegroundColor(TRgb& aColor) {iForegroundColor = aColor; SetGcModeInvalid();}
#if defined(__SERIES60__)
	void SetFont(CFbsFont* aFont,const TBool aStandardFont);
#else
	void SetFont(CFbsFont* aFont);
#endif
	CFbsFont* Font() const {return iFont;}
	void SetFontId(TInt aFontId) {iFontId = aFontId;}
	TInt FontId() const {return iFontId;}
	void MoveCursorToEndOfBuffer(TDesC& aText);
	void SetGcMode(TOplDrawMode aMode);
	void SetGcModeInvalid()	{iCurrentGcMode=EOplGcModeInvalid;} // called when GC is changed directly
	void Print(TDesC& aText);
	void PrintAndMoveCursor(TDesC& aText);
	void SetPattern(COplDrawable* aSource, TSize& aSize);
	CBitmapContext& DrawableGc() {return (*iDrawableGc);}
	TRect FontRect(TDesC& aText, TInt aWidth=0);
	void SetInverseStyle(TBool aStyle);
	TBool InverseStyle() const {return iStyle&KStyleInverse;}
	void DrawLine(TPoint& aEndPoint,TBool aLineBy);
	void DrawGXBorderShadow(TSize& aSize, TInt aCorner, TBool aShadow);
	void DoScrollClear(const TPoint& aOffset,const TRect& aArea);
	void DrawS3Border(TInt aFlags,TRect aRect);
	void DrawS3ButtonL(TInt aState,TInt aWidth,TInt aHeight,const TDesC& aText);
	void DrawS3aButtonL(TInt aState,TInt aWidth,TInt aHeight,const TDesC& aText);
	TDisplayMode GetDisplayMode() {return iDisplayMode;};

//	static void UpdatePoints(TPoint& aStartPoint, TPoint& aEndPoint);
	static void CalcMsgPosition(TInt aCorner, TGulAlignment& aAlignment);
	static TOplDrawMode GetOplDrawMode(TInt aMode);
	static TPoint PopPoint(CStack& aStack);
	static TSize PopSize(CStack& aStack);
	static TRect PopRect(CStack& aStack);
	static TDisplayMode DisplayMode(TInt aValue);
	static TInt DisplayMode(TDisplayMode aMode);
protected:
	void SetDrawableFlag(const TDrawableFlag aFlag) {iDrawableFlag = aFlag;}
	TRect DrawCustomBorder(TSize& aSize, TInt aFlag, TInt aExtraFlags) const;
	CFbsFont* GetDefaultFontL(CBitmapDevice& aDevice);
	static void SwapPoints(TPoint& aPoint1, TPoint& aPoint2); // aPoint1=aPoint2 and aPoint2=aPoint1
	void DoStoreBitmapL(COplRuntime& aRuntime, const TDesC& aFileName, TSize& aSize,const CFbsBitmap& aBitmap) const;
protected:
	CBitmapContext* iDrawableGc;
	TSize iSize;
	TPoint iCursorPosition;
	TOplDrawMode iCurrentGcMode;	// Gc has one mode : we need to change it for text and lines
	TOplDrawMode iCurrentDrawMode;
	TOplDrawMode iCurrentTextMode;
	TDrawableFlag iDrawableFlag;
	TRgb iBackgroundColor;
	TRgb iForegroundColor;
	CFbsFont* iFont;
	TInt iFontId;
#if defined(__SERIES60__)||defined(__UIQ__)
	TBool iIsFontToBeReleased; // Don't release standard fonts
#endif
	TInt iStyle;
	TDisplayMode iDisplayMode;
	};		   



class COplWindow : public COplDrawable, public MEikClockContainer
	{
public:
	COplWindow();
	~COplWindow();
	void ConstructL(COplRuntime& aRuntime, TInt aHandle, const TPoint& aPosition, const TSize& aSize, TInt aVisible, TInt aGray=1);	
	void ConstructL(COplRuntime& aRuntime, RBackedUpWindow& aWin,TDisplayMode aDisplayMode); // used to set the console
	void SetWinPosition(const TPoint& aPoint);
	RBackedUpWindow& Window() {return iWindow;}
	void ClearWindow();
	void ActivateGc();
	void DeactivateGc();
	void BackupBmpHandleL(CFbsBitmap& aBitmap);
	void SetCursorEnabled(TBool aEnabled);
	void DrawCursorIfOn(TTextCursor aCursor);
	TBool CursorState() {return iCursorEnabled;}
	TTextCursor Cursor() {return iCursor;}
	void ReleaseFont();

	void UseNewWindow(RBackedUpWindow& aNewWindow,TDisplayMode aDisplayMode); // really only for console DEFAULTWIN
   
	void CreateClockL(CEikonEnv& aEikonEnv,TInt aType,TInt aOffset=0);
	void CreateFormattedClockL(RAnimDll& aClockDll,TInt aOffset,TInt aFontId,TInt aStyle,const TDesC& aFormat);
	void DrawClock();
	void UpdateClock(const TLocale& aLocale);
public: // from MEikClockContainer
	TBool ClockIsVisible(const CEikLocaleConformantClock& aClock) const;
	void GetClockPositionAndSize(TPoint& aPosition, TSize& aSize, const CEikLocaleConformantClock& aClock, const TSize& aMinimumSize);
    void DeleteClock();
 
	// virtual functions	
	virtual void SetSizeL(const TSize& aSize);
	virtual void SetCursorPosition(const TPoint& aPosition);
	virtual void StoreBitmapL(COplRuntime& aRuntime, const TDesC& aFileName, TSize& aSize);
	virtual void Scroll(const TPoint& aOffset,const TRect& aArea);
	virtual TInt BitmapHandle();
	CWsScreenDevice* iScreenDevice;
protected:	
	void SetAttributesL(CCoeEnv& aConEnv);
	CBitmapContext* CreateWindowGcL();
protected:
	RBackedUpWindow iWindow;
	TBool iCursorEnabled;
	TTextCursor iCursor;
private: // for now
	CEikLocaleConformantClock* iClock;
	TBool iFormatMatchesLocale;
	TInt iTimeOffsetInSeconds;
	TPoint iClockPosition;
#if !defined(__SERIES60__)
	RDigitalClock* iFormattedClock;
#endif
	};

class COplBitmap : public COplDrawable
	{
public:
	COplBitmap();
	~COplBitmap();
	void ConstructL(TSize& aSize,TDisplayMode aMode);
	void ConstructL(const TDesC* aFilename, TInt aWrite, TInt aNum); 
	// overloaded constructor used by gLoadBit
	CFbsBitmap& FbsBitmap() {return (*iBitmap);}
	void ClearBitmap();
	CBitmapDevice* BitmapDevice() const {return iBitmapDevice;}
	void ReleaseFont();
  
	// all virtual functions
	virtual void SetSizeL(const TSize& aSize);
	virtual void StoreBitmapL(COplRuntime& aRuntime, const TDesC& aFileName, TSize& aSize);
	virtual void Scroll(const TPoint& aOffset,const TRect& aArea);
	virtual TInt BitmapHandle();
protected:	
	void SetAttributesL(CFbsBitmap* aBitmap);
	CBitmapContext* CreateBitmapGcL(CFbsBitmap* aBitmap);
	void RestoreBitmapL(const TDesC* aFileName, TInt aWrite, TInt aNum);
protected:
	CFbsBitmap* iBitmap;
	TFileName iBitmapFilename;
	CBitmapDevice* iBitmapDevice;
	CFbsBitmap* iNullBitmap; // used for GC if read-only
	};


class COplConsole;


class CDrawablesCollection : public CBase
	{
public:
	CDrawablesCollection(RWsSession& aWsSession);
	~CDrawablesCollection();
	void SetConsoleWinL(COplRuntime& aRuntime, COplConsole* aConsole);
	TInt NextAvailableId();			 // determined the next available id before adding a drawable
	void AddDrawableL(COplDrawable* aDrawable, TInt aId);  // adds drawable at position aDrawableId - 1
	TInt DrawableId(COplDrawable* aDrawable); // returns Id of aDrawable. Note id = arrayPos + 1
	COplDrawable* CurrentDrawable() {return iCurrentDrawable;}
	COplDrawable* DrawableL(TInt aId);	// returns the drawable at aId-2 in drawablesarray
	void SetCurrentDrawableL(TInt aId); // sets iCurrentDrawable to CDrawable to aId-1
	void CloseDrawableL(TInt aId);	// closes and destroys the specified drawable
	TInt16 AddFontFileL(const TDesC& aFontFile);
	void RemoveFontFile(TInt aId);
	void AddLoadedFontFilesL(COplBitmap* aOplBitmap); // used when creating a bitmap
	CFbsFont* GetFontByIdL(TUid aFontId, const TAlgStyle& aAlgStyle);
	CFbsFont* GetFontByNameL(const TFontSpec& aFontSpec);
	void DrawCursorIfOn();
	void SetDrawableWithCursor(TInt aId) {iDrawableWithCursor = aId;}
	TInt DrawableWithCursor() const {return iDrawableWithCursor;}
	TBool SetFlushState(TBool aState);
	TBool FlushState() const {return iFlushState;}
	void FlushIfOn() const {if (iFlushState) iWsSession.Flush();}
	void DrawAllClocks();
	void UpdateAllClocks();
protected:
	COplDrawable* iCurrentDrawable;
	COplDrawable* iDrawablesArray[KMaxDrawables];
	TInt iNumOfDrawables;
	TInt iDrawableWithCursor;
	TInt iFontFileId[KMaxFontFiles];
private:
	TBool iFlushState;
	RWsSession& iWsSession;  // cached copy

	};

#endif

/* End of $Workfile GRAPHICS.H$ */
  
  
