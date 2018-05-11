// APPFRAME.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __APPFRAME_H__
#define __APPFRAME_H__

#include <opxapi.h>
#include <oplerr.h>
#include "eikbtgpc.h"
#include <eikdll.h>
#include <eikcmobs.h>
#if defined(__S80_DP2_0__)
#include <IrListenAppUi.h>
#elif defined(__CRYSTAL__)
#include <IrListenUi.h>
#endif

#if defined(__SERIES60__)
const TInt KOpxVersion=0x025; // Still alpha on Series60.
#else
const TInt KOpxVersion=0x130;
#endif
const TInt KUidAfOpx=0x10005235;

const TInt KOplMaxStringLength=255;
const TInt KOplMaxProcNameLength=32;

const TInt KAfOpxFirstButton=1;

const TInt KAfOpxTitleHeight=30;

const TInt16 KNoRedraw=EFalse;
const TInt16 KRedrawRequired=ETrue;

const TInt16 KMenuPaneToStartOn=256;	// Hack to get around the 'Task List' menu pane
										// being the first to show unless manually changed.
const TInt KMenuIrListener=0x9000;		// Also defined in EN_MAIN.CPP in OPLR for reference.
										// Dummy menu item ID for CIrListenAppUi::NewL()

class MEikCommandObserver;

/**
 * The class COpxAppFrameObserver watches the buttons, and passes on any button presses
 * to the OPL callbacks. 
 */
class COpxAppFrameObserver : public CBase, public MEikCommandObserver
	{
public:
	static COpxAppFrameObserver* NewL(OplAPI& aOplAPI);
	~COpxAppFrameObserver();
public:	// from MEikCommandObserver:
	void ProcessCommandL(TInt aCommandId);
	void SetMaxCommandsL(TInt aMaxCommands);
	void SetCallback(TInt anIndex, TPtrC aCallback);
private:
	void ConstructL(OplAPI& aOplAPI);
	COpxAppFrameObserver();
private:
	typedef TBuf<32> TProcedureName;
	CArrayFixFlat<TProcedureName>* iCallback;
	OplAPI* iOplAPI;
	};

#if defined(__UIQ__)
class CBaseControl:public CCoeControl,public MCoeControlObserver
	{
public:
	CBaseControl(){};
	~CBaseControl(){};
	TInt CountComponentControls() const
		{
		if(iCtrl)
			return 1;
		return 0;
		};
	CCoeControl* ComponentControl(TInt /*aIndex*/) const
		{
		return iCtrl;
		}

	void ConstructL()
		{
		CreateWindowL();
		SetComponentsToInheritVisibility(ETrue);
		ActivateL();
		}

	void SizeChanged()
		{
		if(iCtrl)
			iCtrl->SetSize(Size());
		TRect clientRect=iEikonEnv->EikAppUi()->ClientRect();
		SetPosition(TPoint(0,clientRect.iBr.iY));
		iCtrl->SetPosition(TPoint(0,0));
		}

	void HandleControlEventL(CCoeControl* /*aControl*/,TCoeEvent aEventType)
		{
		if(EEventRequestFocus==aEventType)
			{
			iCtrl->SetFocus(ETrue);
			}
		}

	CCoeControl* iCtrl;
	};
#endif

/**
 * The class CAppFrameUser is a singleton which provides the OPX procedure service routines. 
 */
class CAppFrameUser : public CBase
	{
public:
	static CAppFrameUser* NewL(OplAPI& aOplAPI);
	~CAppFrameUser();
public:
	void OfferEvent(OplAPI& aOplAPI);
// CBA
	void SetCBAButton(OplAPI& aOplAPI);
	void SetCBAButtonDefault(OplAPI& aOplAPI);
	void SetCBAButtonDimmed(OplAPI& aOplAPI);
	void CBAButtonDimmed(OplAPI& aOplAPI);
	void SetCBAButtonVisible(OplAPI& aOplAPI);
	void CBAButtonVisible(OplAPI& aOplAPI);
	void SetCBAVisible(OplAPI& aOplAPI);
	void CBAVisible(OplAPI& aOplAPI);
	void CBAMaxButtons(OplAPI& aOplAPI);
// Status pane
	void SetStatus(OplAPI& aOplAPI);
	void SetStatusVisible(OplAPI& aOplAPI);
	void StatusVisible(OplAPI& aOplAPI);
// Title bar
	void SetTitle(OplAPI& aOplAPI);
	void SetTitleAreaWidth(OplAPI& aOplAPI);
	void SetTitleVisible(OplAPI& aOplAPI);
	void TitleVisible(OplAPI& aOplAPI);
// Screen info
	void ScreenInfo(OplAPI& aOplAPI);
// Desk Access
	void AddToDesk(OplAPI& aOplAPI);
// First-menu-pane-to-show-hackery
	void MenuPaneToStartOn(OplAPI& aOplAPI);
// Launch the Log application
	void ViewSystemLog(OplAPI& aOplAPI);
// Toggle the IrDA listener
	void ToggleIrDA(OplAPI& aOplAPI);
/*	void IrDAIsActive(OplAPI& aOplAPI);
// Toggle the Bluetooth listener
	void ToggleBluetooth(OplAPI& aOplAPI);
	void BluetoothIsActive(OplAPI& aOplAPI);
*/
// CKON Dialogs
	void ConfirmationDialog(OplAPI& aOplAPI);
	void InfoDialog(OplAPI& aOplAPI);
private:
	void ConstructL(OplAPI& aOplAPI);
	TUint MapFromOplMod(TUint aOplMod);
	TBool ResizeTitleL(const CEikonEnv& aEikonEnv);
	void InitStatusPane(OplAPI& aOplAPI);
private:
	// Has to match status pane type constants in .txh file.
	enum TAfStatusPaneType
		{
		EAfStatusPaneTypeNarrow=1,
		EAfStatusPaneTypeWide=2
		};
private:
#if defined(__UIQ__)
	CBaseControl* iButtonGroupOwner;
#endif
	COpxAppFrameObserver* iAppFrameObserver;
	CEikButtonGroupContainer* iButtonGroupContainer;
	CEikStatusPane* iStatusPane;
	TAfStatusPaneType iStatusPaneType;
	RBackedUpWindow iTitleWindow;
#if defined(__CRYSTAL__)
	CCknAppTitle* iTitleBar;
#endif
#if defined(__UIQ__)
	//RBackedUpWindow iToolBarWindow; // only used for the uiq toolbar
	CWindowGc* iToolContext;
#endif
	CWindowGc* iTitleContext;
	TBool iIsTitleBarVisible;
private:
	enum TAfOpxEvent
		{
		ENotKeyMask=0x0400,
		EFocusGained,
		EFocusLost,
		ESwitchOn,
		ECommand,
		EKeyDown=0x0406,
		EKeyUp,
		EPtr,
		EPtrEnter,
		EPtrExit
		}; 
	enum TOplModifiers
		{
		EOplModShift=2,
		EOplModCtrl=4,
		EOplModAlt=8,
		EOplModCapsLock=16,
		EOplModFunc=32
		};
private:
#if defined(USE_IRLISTEN)
	CIrListenAppUi* iIrListenAppUi;
#endif
	};

/**
 * The class COpxAppFrame dispatches OPX procedures called by ordinal to the 
 * service routines
 */
class COpxAppFrame: public COpxBase 
	{
public:
	static COpxAppFrame* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
	COpxAppFrame(OplAPI& aOplAPI);
	void ConstructL(OplAPI& aOplAPI);
	~COpxAppFrame();
private:
	enum TExtensions
		{
		EOfferEvent=1,			//	AfOfferEvent%:(aEv1&,aEv3&,aEv4&,aEv5&,aEv6&,aEv7&)
		ESetCBAButton,			//	AfSetCBAButton:(aButtonIndex%,aText$,aBitmapId%,aMaskId%,aCallback$)
		ESetCBAButtonDefault,	//	AfSetCBAButtonDefault:(aButtonIndex%,aDefault%)
		ESetCBAButtonDimmed,	//	AfSetCBAButtonDimmed:(aButtonIndex%,aDimmed%)
		ECBAButtonDimmed,		//	AfCBAButtomDimmed%:(aButtonIndex%)
		ESetCBAButtonVisible,	//	AfSetCBAButtonVisible:(aButtonIndex%,aVisibility%)
		ECBAButtonVisible,		//	AfCBAButtonVisible%:(aButtonIndex%)
		ESetCBAVisible,			//	AfSetCBAVisible%:(aVisibility%)
		ECBAVisible,			//	AfCBAVisible%:
		ECBAMaxButtons,			//	AfCBAMaxButtons%:
		ESetStatus,				//	AfSetStatus%:(aType%)
		ESetStatusVisible,		//	AfSetStatusVisible%:(aVisibility%)
		EStatusVisible,			//	AfStatusVisible%:(BYREF aType%)
		ESetTitle,				//	AfSetTitle:(aTitle$,aTitleType%)
		ESetTitleAreaWidth,		// 	AfSetTitleAreaWidth:(aTitleType%,aWidth&)
		ESetTitleVisible,		//	AfSetTitleVisibile%:(aVisibility%)
		ETitleVisible,			//	AfTitleVisible%:
		EScreenInfo,			//	AfScreenInfo:(BYREF aXOrigin%, BYREF aYOrigin%, BYREF aWidth%, BYREF aHeight%)
		EAddToDesk,				//	AfAddToDesk:
		EMenuPaneToStartOn,		//	AfMenuPaneToStartShowing%:
		EViewSystemLog,			//	AfViewSystemLog:
		EToggleIrDA,			//	AfToggleIrDA:
		EConfirmationDialog,	//	AfConfirmationDialog%:(aTitle$,aMessage$,aConfirmButtonTitle$)
		EInfoDialog				//	AfInformationDialog%:(aTitle$,aMessage$)
/*		EIrDAIsActive,			//  AFIrDAIsActive%:
		EToggleBluetooth,		//  AFToggleBlueTooth:
		EBluetoothIsActive		//  AFBlueToothIsActive%:
*/
		};
	CAppFrameUser* iAppFrameUser;
	};

#endif
