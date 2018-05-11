// APPFRAME.CPP
//
#include <bldvariant.hrh>

#include <eikon.hrh>
#include <eikenv.h>
#include <w32std.h>

#include <eiktbar.h>
#if defined(__CRYSTAL__)
#include <indicato.rsg>
#include <ckntitle.h>
#if defined(USE_ADDTODESK)
#if defined(__S80_DP2_0__)
 #include <addtodesk.h>
#else
 #include <linkbase.h>
 #include <linkapplication.h>
 #include <linkdocument.h>
 #endif
#endif
#include <linkutils.h>
#include <cknconf.h> // for CCknConfirmationDialog
#include <ckninfo.h> // for CCknInfoDialog
#endif
#if defined(__UIQ__)
#include <oplr.rsg>
#include <qiktoolbar.h>
#endif
#include <eikspane.h>



#include "opxutil.h"
#include "appframe.h"

COpxAppFrameObserver* COpxAppFrameObserver::NewL(OplAPI& aOplAPI)
	{
	COpxAppFrameObserver* This=new(ELeave) COpxAppFrameObserver();
	CleanupStack::PushL(This);
	This->ConstructL(aOplAPI);
	CleanupStack::Pop(This);
	return This;
	}

void COpxAppFrameObserver::ConstructL(OplAPI& aOplAPI)
	{
	iCallback = new(ELeave) CArrayFixFlat<TProcedureName>(4);
	iOplAPI = &aOplAPI;
	}

COpxAppFrameObserver::COpxAppFrameObserver()
	:MEikCommandObserver()
	{
	}

COpxAppFrameObserver::~COpxAppFrameObserver()
	{
	delete iCallback;
	}

void COpxAppFrameObserver::ProcessCommandL(TInt aCommandId)
	{
	// where aCommandId ranges from 1..4
	TPtrC procedure=iCallback->At(OpxUtil::CppIndex(aCommandId)); //Index starts at 0.
	iOplAPI->InitCallbackL(procedure);
	iOplAPI->PushParamL(TInt32(aCommandId)); //Callback starts at 1.
	User::LeaveIfError(iOplAPI->CallProcedure(EReturnFloat));
	iOplAPI->PopReal64();
	}

void COpxAppFrameObserver::SetMaxCommandsL(TInt aMaxCommands)
	{
	iCallback->ResizeL(aMaxCommands);
	}

void COpxAppFrameObserver::SetCallback(TInt anIndex, TPtrC aCallback)
	{
	iCallback->At(anIndex)=aCallback;
	}

CAppFrameUser* CAppFrameUser::NewL(OplAPI& aOplAPI)
	{
	CAppFrameUser* This=new(ELeave) CAppFrameUser();
	CleanupStack::PushL(This);
	This->ConstructL(aOplAPI);
	CleanupStack::Pop(This);
	return This;
	}

_LIT(KDefaultLabel,"Default");

void CAppFrameUser::ConstructL(OplAPI& aOplAPI)
	{
	// CBA
	iAppFrameObserver = COpxAppFrameObserver::NewL(aOplAPI);
#if defined(__UIQ__)
	const TRect screenRect=CEikonEnv::Static()->EikAppUi()->ClientRect(); // for future usage
	iButtonGroupOwner = new (ELeave) CBaseControl;
	iButtonGroupOwner->ConstructL();
	iButtonGroupContainer = CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::EDialog,
														CEikButtonGroupContainer::EHorizontal,
														iAppFrameObserver,0,*iButtonGroupOwner,
														CEikButtonGroupContainer::EUseMaxSize|CEikButtonGroupContainer::EAddToStack);
	iButtonGroupOwner->iCtrl=iButtonGroupContainer;
	iButtonGroupContainer->SetObserver(iButtonGroupOwner);
#else
	iButtonGroupContainer = CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::ECba,
														CEikButtonGroupContainer::EVertical,
														iAppFrameObserver,NULL);
#endif

#if defined(__UIQ__)
	//!!TODOUIQ - Lars has this as true at construction.
	iButtonGroupContainer->MakeVisible(ETrue); //was false
#else
	iButtonGroupContainer->MakeVisible(EFalse); //was false
#endif

#if defined(__SERIES60__)
	TInt maxCommands=1;
#else
	TInt maxCommands=iButtonGroupContainer->MaxCommands();
#endif
	iAppFrameObserver->SetMaxCommandsL(maxCommands);
#if !defined(__UIQ__)
	for (TInt id=KAfOpxFirstButton; id<=maxCommands; id++)
		iButtonGroupContainer->SetCommandL(id-1,id,KDefaultLabel);
#else
	for (TInt id=KAfOpxFirstButton; id<=maxCommands; id++)
		iButtonGroupContainer->AddCommandL(id-1,id,_L(""));
#endif
	iButtonGroupContainer->DrawableWindow()->SetPointerGrab(EFalse);
	iButtonGroupContainer->ActivateL();

	CEikonEnv& eikonEnv=aOplAPI.EikonEnv(); // cache for speed
#if !defined(__UIQ__)
	const TRect screenRect=eikonEnv.ScreenDevice()->SizeInPixels(); // for future usage
#endif

	TRect boundingRect = screenRect;
	TSize minimumSize=iButtonGroupContainer->MinimumSize();
	boundingRect.iTl.iX=boundingRect.iBr.iX-minimumSize.iWidth;
	iButtonGroupContainer->SetBoundingRect(boundingRect);
#if defined(__UIQ__)
	iButtonGroupOwner->SetSize(TSize(eikonEnv.EikAppUi()->ClientRect().Width(),minimumSize.iHeight));
#endif
#if defined(__CRYSTAL__)
	// Title
	iTitleBar=CCknAppTitle::NewL(); // EEditor is default type
	TRect titleRect;
	titleRect.iTl=TPoint(0,0);
	titleRect.iBr=TPoint(screenRect.Width(),KAfOpxTitleHeight);
	iTitleBar->SetRect(titleRect);
	iTitleBar->SetFocus(ETrue);

	TInt color=0;
	TInt gray=0;
	iTitleWindow=RBackedUpWindow(aOplAPI.WsSession());
	User::LeaveIfError(iTitleWindow.Construct(aOplAPI.RootWindow(),
										aOplAPI.WsSession().GetDefModeMaxNumColors(color,gray),
																		(TUint32)&iTitleWindow));
	iTitleWindow.SetVisible(EFalse);
// Fix bug [1158074] Application title bar casts shadow
	iTitleWindow.SetShadowDisabled(ETrue);  // disable shadow
	TSize size(screenRect.Width(),KAfOpxTitleHeight);
	User::LeaveIfError(iTitleWindow.SetSizeErr(size));
	iTitleWindow.SetPosition(titleRect.iTl);
	User::LeaveIfError(aOplAPI.ScreenDevice()->CreateContext(iTitleContext));
	iTitleContext->Activate(iTitleWindow);
	iTitleWindow.SetOrdinalPosition(0,0);
	iTitleWindow.Activate();

	iTitleBar->CCoeControl::SetContainerWindowL(iTitleWindow);
	iTitleBar->ActivateL();
	ResizeTitleL(eikonEnv);
	iIsTitleBarVisible=EFalse;
	
#if defined(USE_IRLISTEN)
	// Finally, construct our IrListener object (passing a NULL hotkey table)
#if defined(__S80_DP2_0__)
	iIrListenAppUi=CIrListenAppUi::NewL();
#else
	iIrListenAppUi=CIrListenAppUi::NewL(KMenuIrListener, NULL);
#endif
#endif
#endif
	}

#if defined(__CRYSTAL__)
void CAppFrameUser::InitStatusPane(OplAPI& aOplAPI)
	{
	// Status
	if (iStatusPane)
		return; // already present.
#pragma message("Need to sort out the status pane - default is visible")
	CEikonEnv& eikonEnv=aOplAPI.EikonEnv(); //cache for speed.
	if (eikonEnv.StatusPaneCoreResId() != 0)
		iStatusPane=CEikStatusPane::NewL(eikonEnv,&aOplAPI.RootWindow(),
													eikonEnv.StatusPaneCoreResId(),
													EEikStatusPaneUseDefaults);
#if !defined(__S80_DP2_0__)
	else
		iStatusPane=CEikStatusPane::NewL(eikonEnv,&aOplAPI.RootWindow(),R_INDICATOR);
#endif
	// Make OPLR's status pane sit behind ours, this helps solve some
	// drawing problems
	CEikStatusPane* statusPane = eikonEnv.AppUiFactory()->StatusPane();
	if (statusPane)
		{
		statusPane->SwitchLayoutL(R_INDICATOR_LAYOUT_WIDE);
		statusPane->ApplyCurrentSettingsL();
		statusPane->MakeVisible(EFalse);
#if defined(__S80_DP2_0__)
		statusPane->HandleStatusPaneUpdateMessageL();
#endif
		}
	iStatusPane->SwitchLayoutL(R_INDICATOR_LAYOUT_WIDE);
	iStatusPane->ApplyCurrentSettingsL();
	iStatusPane->MakeVisible(EFalse);
#if defined(__S80_DP2_0__)
	iStatusPane->HandleStatusPaneUpdateMessageL();
#endif
	iStatusPaneType=EAfStatusPaneTypeWide;
	}
#else
void CAppFrameUser::InitStatusPane(OplAPI& /*aOplAPI*/)
	{
	// Status
	if (iStatusPane)
		return; // already present.
	}
#endif

TUint CAppFrameUser::MapFromOplMod(TUint aOplMod)
	{
	TUint mod=0;
	if (aOplMod==0)
		return mod;
	if (aOplMod&EOplModAlt)
		mod |= EModifierLeftAlt|EModifierAlt;
	if (aOplMod&EOplModCtrl)
		mod |= EModifierLeftCtrl|EModifierCtrl;
	if (aOplMod&EOplModShift)
		mod |= EModifierLeftShift|EModifierShift;
	if (aOplMod&EOplModCapsLock)
		mod |= EModifierCapsLock;
	if (aOplMod&EOplModFunc)
		mod |= EModifierLeftFunc|EModifierFunc;
	return mod;
	}

/**
 * Processes the OPL procedure
 * AfOfferEvent%:
 * Allows the OPX to handle an event received by the app.
 * Returns KTrue% if event consumed.
 */
void CAppFrameUser::OfferEvent(OplAPI& aOplAPI)
	{
	/*TUint ev7 =*/ aOplAPI.PopInt32(); // y coord -- not used yet, but ready for pen-based Crystal device.
	/*TUint ev6 =*/ aOplAPI.PopInt32(); // x coord -- "
	TUint ev5 = aOplAPI.PopInt32(); //repeat.
	TUint ev4 = aOplAPI.PopInt32(); //modifier
	TUint ev3 = aOplAPI.PopInt32(); //scan
	TUint ev1 = aOplAPI.PopInt32(); //type
	TBool wasConsumed = EFalse;
	if (iButtonGroupContainer)
		{
		if (!(ev1 & ENotKeyMask))
			{
			TKeyEvent keyEvent;
			keyEvent.iCode		= ev1;
			keyEvent.iScanCode	= ev3;
			keyEvent.iModifiers	= MapFromOplMod(ev4);
			keyEvent.iRepeats	= ev5;
			// First check that if iCode was EKeyEnter then iScanCode was EStdKeyEnter.
			// This prevents Ctrl+(Shift)+(Alt)+M being processed as <Enter> which would
			// happen otherwise (thus preventing OPL authors using them as hotkeys)
			if ((keyEvent.iCode==EKeyEnter) && (keyEvent.iScanCode!=EStdKeyEnter))
				wasConsumed=EFalse;
			else
				wasConsumed=iButtonGroupContainer->OfferKeyEventL(keyEvent,EEventKey);
			}
		}
	aOplAPI.Push(OpxUtil::OplBool16(wasConsumed));
	}

/**
 * Processes the OPL procedure
 * AfSetCBAButton:(aButtonPosition%,aText$,aBitmap&,aMask&,aCallback$)
 */
void CAppFrameUser::SetCBAButton(OplAPI& aOplAPI)
	{
	TPtrC callback	= aOplAPI.PopString();
	TInt16 maskId	= aOplAPI.PopInt16();
	TInt16 bitmapId	= aOplAPI.PopInt16();
	TPtrC text	= aOplAPI.PopString();
	TInt buttonPosition = aOplAPI.PopInt16();
	if (callback.Length()==0)
		User::Leave(KOplErrInvalidArgs);
	if (iButtonGroupContainer)
		{
		if (buttonPosition<KAfOpxFirstButton || buttonPosition>iButtonGroupContainer->MaxCommands())
			User::Leave(KOplErrInvalidArgs);
		const TInt containerLocation=buttonPosition-1;
		if (bitmapId>0)
			{
			TInt bitmapHandle = aOplAPI.BitmapHandleFromIdL(bitmapId);
			TInt maskHandle = aOplAPI.BitmapHandleFromIdL(maskId);

#if defined(__UIQ__)
			if(buttonPosition<iButtonGroupContainer->ButtonCount())
				iButtonGroupContainer->SetCommandL(containerLocation,buttonPosition,text,bitmapHandle,maskHandle);
			else
				iButtonGroupContainer->AddCommandL(containerLocation,buttonPosition,text,bitmapHandle,maskHandle);
			}
		else
			{
			if(buttonPosition<iButtonGroupContainer->ButtonCount())
				iButtonGroupContainer->SetCommandL(containerLocation,buttonPosition,text);
			else
				iButtonGroupContainer->AddCommandL(containerLocation,buttonPosition,text);
			}
#else
			iButtonGroupContainer->SetCommandL(containerLocation,buttonPosition,text,bitmapHandle,maskHandle);
			}
		else
			{
			iButtonGroupContainer->SetCommandL(containerLocation,buttonPosition,text);
			}
#endif
#if defined(__UIQ__)
		iButtonGroupContainer->SetSize(iButtonGroupContainer->Size());
#endif
		iButtonGroupContainer->DrawNow();
		iAppFrameObserver->SetCallback(OpxUtil::CppIndex(buttonPosition),callback); //Index from 0..3
		}
	aOplAPI.Push(0.0);
	}

/**
 * Processes the OPL procedure:
 * AfSetCBAButtonDefault:(aButtonPosition%)
 * Sets the CBA button to be default. Only one button may be default at a time.
 */
void CAppFrameUser::SetCBAButtonDefault(OplAPI& aOplAPI)
	{
	TInt buttonPosition=aOplAPI.PopInt16();//aButtonPosition%
	if (iButtonGroupContainer)
		{
		if (buttonPosition<KAfOpxFirstButton || buttonPosition>iButtonGroupContainer->ButtonCount())
			User::Leave(KOplErrInvalidArgs);
		iButtonGroupContainer->SetDefaultCommand(buttonPosition); //CommandId
#if defined(__UIQ__)
		iButtonGroupContainer->SetSize(iButtonGroupContainer->Size());
#endif
		iButtonGroupContainer->DrawNow();
		}
	aOplAPI.Push(0.0);
	}

/**
 * Processes the OPL procedure
 * AfSetCBAButtonDimmed:(aButtonPosition%,aDimmed%)
 * Makes the CBA button dimmed/undimmed, depending on the flag passed in.
 */
void CAppFrameUser::SetCBAButtonDimmed(OplAPI& aOplAPI)
	{
	TBool dimmed=OpxUtil::CppBool(aOplAPI.PopInt16()); //aDimmed%
	TInt buttonPosition=aOplAPI.PopInt16(); //aButtonPosition%
	if (iButtonGroupContainer)
		{
		if (buttonPosition<KAfOpxFirstButton || buttonPosition>iButtonGroupContainer->ButtonCount())
			User::Leave(KOplErrInvalidArgs);
		iButtonGroupContainer->DimCommand(buttonPosition,dimmed); //CommandId
		iButtonGroupContainer->DrawNow();
		}
	aOplAPI.Push(0.0);
	}

/**
 * Processes the OPL procedure
 * AfCBAButtonDimmed%:(aButtonPosition%)
 * Returns 1 if the CBA button is dimmed, otherwise 0.
 */
void CAppFrameUser::CBAButtonDimmed(OplAPI& aOplAPI)
	{
	TInt buttonPosition=aOplAPI.PopInt16();//aButtonPosition%
	if (iButtonGroupContainer)
		{
		if (buttonPosition<KAfOpxFirstButton || buttonPosition>iButtonGroupContainer->ButtonCount())
			User::Leave(KOplErrInvalidArgs);
		aOplAPI.Push(OpxUtil::OplBool16(iButtonGroupContainer->IsCommandDimmed(buttonPosition)));
		return;
		}
	aOplAPI.Push(OpxUtil::OplBool16(EFalse));
	}

/**
 * Processes the OPL procedure
 * AfSetCBAButtonVisible:(aButtonPosition%,aVisible%)
 * Makes the CBA button visible/invisible, depending on the flag passed in.
 */
void CAppFrameUser::SetCBAButtonVisible(OplAPI& aOplAPI)
	{
	TBool visible=OpxUtil::CppBool(aOplAPI.PopInt16()); //aVisible%
	TInt buttonPosition=aOplAPI.PopInt16(); //aButtonPosition%
	if (iButtonGroupContainer)
		{
		if (buttonPosition<KAfOpxFirstButton || buttonPosition>iButtonGroupContainer->ButtonCount())
			User::Leave(KOplErrInvalidArgs);
		iButtonGroupContainer->MakeCommandVisible(buttonPosition,visible);
#if defined(__UIQ__)
		iButtonGroupContainer->SetSize(iButtonGroupContainer->Size());
#endif
		iButtonGroupContainer->DrawNow();
		}
	aOplAPI.Push(0.0);
	}

/**
 * Processes the OPL procedure
 * AfCBAButtonVisible%:(aButtonPosition%)
 * Returns KTrue% if the CBA button is visible, otherwise KFalse%
 */
void CAppFrameUser::CBAButtonVisible(OplAPI& aOplAPI)
	{
	TInt buttonPosition=aOplAPI.PopInt16();//aButtonPosition%
	if (iButtonGroupContainer)
		{
		if (buttonPosition<KAfOpxFirstButton || buttonPosition>iButtonGroupContainer->ButtonCount())
			User::Leave(KOplErrInvalidArgs);
		aOplAPI.Push(OpxUtil::OplBool16(iButtonGroupContainer->IsCommandVisible(buttonPosition)));
		return;
		}
	aOplAPI.Push(OpxUtil::OplBool16(EFalse));
	}

/**
 * Processes the OPL procedure:
 * AfCBAMaxButtons%:
 * Returns the maximum number of buttons available in the ButtonGroupContainer.
 */
void CAppFrameUser::CBAMaxButtons(OplAPI& aOplAPI)
	{
	if (iButtonGroupContainer)
		{
		aOplAPI.Push(TInt16(iButtonGroupContainer->MaxCommands()));
		return;
		}
	aOplAPI.Push(TInt16(0));
	}

/**
 * Processes the OPL procedure
 * AfSetCBAVisible%:(aVisibility%)
 * Makes the CBA visible/invisible, depending on the visibility flag passed in.
 * Returns KTrue% if screen size needs changing, otherwise KFalse%
 */
void CAppFrameUser::SetCBAVisible(OplAPI& aOplAPI)
	{
	TBool newVisibility=OpxUtil::CppBool(aOplAPI.PopInt16());//aVisibility%
	if (iButtonGroupContainer)
		{
		TBool currentVisibility=iButtonGroupContainer->IsVisible();
		if (currentVisibility==newVisibility)
			{
			aOplAPI.Push(OpxUtil::OplBool16(KNoRedraw));
			return;
			}
#if defined(__UIQ__)
		iButtonGroupOwner->MakeVisible(newVisibility);
#else
		iButtonGroupContainer->MakeVisible(newVisibility);
#endif
		TBool redraw=ResizeTitleL(aOplAPI.EikonEnv());
		iButtonGroupContainer->DrawNow();
		aOplAPI.Push(OpxUtil::OplBool16(redraw));
		return;
		}
	aOplAPI.Push(OpxUtil::OplBool16(KNoRedraw));
	}

/**
 * Processes the OPL procedure
 * AfCBAVisible%:
 * Returns KTrue% if the whole CBA is visible, otherwise KFalse%
 */
void CAppFrameUser::CBAVisible(OplAPI& aOplAPI)
	{
	if (iButtonGroupContainer)
		{
		aOplAPI.Push(OpxUtil::OplBool16(iButtonGroupContainer->IsVisible()));
		return;
		}
	aOplAPI.Push(OpxUtil::OplBool16(EFalse));
	}

/**
 * Processes the OPL procedure
 * AfSetStatus%:(aType%)
 * Sets the status pane type to one of ...Narrow% or ...Wide%.
 * Returns KTrue% if screen size needs changing, otherwise KFalse%
 */
void CAppFrameUser::SetStatus(OplAPI& aOplAPI)
	{
	TUint16 newType=aOplAPI.PopInt16();//aType%
	InitStatusPane(aOplAPI);
	if (iStatusPane)
		{
		if (newType!=iStatusPaneType) //Requesting a type different from current type.
			{
#pragma message("Need to sort out the status pane here too")
#if defined(__CRYSTAL__)
			CEikStatusPane* statusPane = aOplAPI.EikonEnv().AppUiFactory()->StatusPane();
			if (newType==EAfStatusPaneTypeNarrow)
				{
				// Make OPLR's status pane sit behind ours, this helps solve some
				// drawing problems
				if (statusPane)
				{
					statusPane->SwitchLayoutL(R_INDICATOR_LAYOUT_NARROW);
					statusPane->ApplyCurrentSettingsL();
#if defined(__S80_DP2_0__)
					statusPane->HandleStatusPaneUpdateMessageL();
#endif
				}
				iStatusPane->SwitchLayoutL(R_INDICATOR_LAYOUT_NARROW);
				iStatusPane->ApplyCurrentSettingsL();
#if defined(__S80_DP2_0__)
				iStatusPane->HandleStatusPaneUpdateMessageL();
#endif
				}
			else
				{
				// Make OPLR's status pane sit behind ours, this helps solve some
				// drawing problems
				if (statusPane)
				{
					statusPane->SwitchLayoutL(R_INDICATOR_LAYOUT_WIDE);
					statusPane->ApplyCurrentSettingsL();
#if defined(__S80_DP2_0__)
					statusPane->HandleStatusPaneUpdateMessageL();
#endif
				}
				iStatusPane->SwitchLayoutL(R_INDICATOR_LAYOUT_WIDE);
				iStatusPane->ApplyCurrentSettingsL();
#if defined(__S80_DP2_0__)
				iStatusPane->HandleStatusPaneUpdateMessageL();
#endif
				}
#endif
			iStatusPaneType=(TAfStatusPaneType)newType;
			if (iStatusPane->IsVisible())
				{
				aOplAPI.Push(OpxUtil::OplBool16(ResizeTitleL(aOplAPI.EikonEnv())));
				return;
				}
			}
		}
	aOplAPI.Push(OpxUtil::OplBool16(KNoRedraw));
	}

/**
 * Processes the OPL procedure
 * AfSetStatusVisible%:(aVisibility%)
 * Makes the Status pane visible/invisible, depending on the visibility flag passed in.
 * Returns KTrue% if screen size needs changing, otherwise KFalse%
 */
void CAppFrameUser::SetStatusVisible(OplAPI& aOplAPI)
	{
	TBool newVisibility=OpxUtil::CppBool(aOplAPI.PopInt16()); //aVisibility%
	InitStatusPane(aOplAPI);
	CEikStatusPane* statusPane = aOplAPI.EikonEnv().AppUiFactory()->StatusPane();
	if (statusPane)
	{
		statusPane->MakeVisible(newVisibility);
		statusPane->ApplyCurrentSettingsL();
#if defined(__S80_DP2_0__)
		statusPane->HandleStatusPaneUpdateMessageL();
#endif
	}
	if (iStatusPane)
		{
		TBool currentVisibility=iStatusPane->IsVisible();
		if (currentVisibility!=newVisibility) //change required.
			{
			// Make OPLR's status pane sit behind ours, this helps solve some
			// drawing problems
			if (statusPane)
			{
				statusPane->MakeVisible(newVisibility);
				statusPane->ApplyCurrentSettingsL();
#if defined(__S80_DP2_0__)
				statusPane->HandleStatusPaneUpdateMessageL();
#endif
			}
			iStatusPane->MakeVisible(newVisibility);
			iStatusPane->ApplyCurrentSettingsL();
#if defined(__S80_DP2_0__)
			iStatusPane->HandleStatusPaneUpdateMessageL();
#endif
			aOplAPI.Push(OpxUtil::OplBool16(ResizeTitleL(aOplAPI.EikonEnv())));
			return;
			}
		}
	aOplAPI.Push(OpxUtil::OplBool16(KNoRedraw));
	}

/**
 * Processes the OPL procedure
 * AfStatusVisible%:(BYREF aType%)
 * Sets the referenced var to the type of the status pane.
 * Returns KTrue% if the status pane is visible, KFalse% otherwise.
 */
void CAppFrameUser::StatusVisible(OplAPI& aOplAPI)
	{
	TInt16 *type=aOplAPI.PopPtrInt16();
	InitStatusPane(aOplAPI);
	aOplAPI.PutWord(type,(TInt16)iStatusPaneType);
	if (iStatusPane)
		aOplAPI.Push(OpxUtil::OplBool16(iStatusPane->IsVisible()));
	else
		aOplAPI.Push(OpxUtil::OplBool16(KNoRedraw));
	}

/**
 * Processes the OPL procedure
 * AfSetTitle:(aTitle$,aTitleType%)
 * Sets the title in the titlebar.
 */
void CAppFrameUser::SetTitle(OplAPI& aOplAPI)
	{ // AfSetTitle:(aTitle$,aTitleType%)
#if defined(__CRYSTAL__)
	TInt16 type=aOplAPI.PopInt16();
	TPtrC titleString=aOplAPI.PopString();
	CCknAppTitle::TAppTitleElement titleType=(CCknAppTitle::TAppTitleElement)(OpxUtil::CppIndex(type));
	if (titleType!=CCknAppTitle::EMainTitle && titleType!=CCknAppTitle::ETitleExtension)
		User::Leave(KOplErrInvalidArgs); // disallow ESubTitle, EAdditionalInfo and invalid values
	if (iTitleBar)
		{
		iTitleBar->SetTextL(titleString,titleType);
		iTitleBar->DrawNow();
		}
#else // not crystal
	aOplAPI.PopInt16();
	aOplAPI.PopString();
	User::Leave(KOplErrNotSupported);
#endif
	aOplAPI.Push(0.0);
	}

/**
 * Processes the OPL procedure
 * AfSetTitleAreaWidth:(aTitleType%,aWidth%)
 * Sets the area width of the given title
 */
void CAppFrameUser::SetTitleAreaWidth(OplAPI& aOplAPI)
	{
#if defined(__CRYSTAL__)
	TInt width=aOplAPI.PopInt16();
	TInt type=aOplAPI.PopInt16();
	CCknAppTitle::TAppTitleElement titleType=(CCknAppTitle::TAppTitleElement)(OpxUtil::CppIndex(type));
	if (titleType!=CCknAppTitle::EMainTitle && titleType!=CCknAppTitle::ETitleExtension)
		User::Leave(KOplErrInvalidArgs); // disallow ESubTitle, EAdditionalInfo and invalid values
	if (iTitleBar)
		{
		iTitleBar->SetAreaWidth(width,titleType);
		iTitleBar->DrawNow();
		}
#else
	aOplAPI.PopInt16();
	aOplAPI.PopInt16();
	User::Leave(KOplErrNotSupported);
#endif
	aOplAPI.Push(0.0);
	}

/**
 * Processes the OPL procedure
 * AfSetTitleVisible%:(aVisibility%)
 * Makes the title visible/invisible, depending on the visibility flag passed in.
 * Returns KTrue% if screen size needs changing, otherwise KFalse%.
 */
void CAppFrameUser::SetTitleVisible(OplAPI& aOplAPI)
	{
#if defined(__CRYSTAL__)
	TBool newVisibility=OpxUtil::CppBool(aOplAPI.PopInt16()); //aVisibility%
	TBool currentVisibility=iIsTitleBarVisible;
	if (currentVisibility!=newVisibility) // change required.
		{
		iIsTitleBarVisible=newVisibility;
		iTitleWindow.SetVisible(newVisibility);
		// If other controls have changed state while title invisible.
		iTitleBar->DrawNow();
		aOplAPI.Push(OpxUtil::OplBool16(ResizeTitleL(aOplAPI.EikonEnv())));
		return;
		}
#else // not crystal
	aOplAPI.PopInt16();
	User::Leave(KOplErrNotSupported); // Not available on Series60
#endif
	aOplAPI.Push(OpxUtil::OplBool16(KNoRedraw));
	}

/**
 * Processes the OPL procedure
 * AfTitleVisible%:
 * Returns KTrue% if the title is visible, else KFalse%
 */
void CAppFrameUser::TitleVisible(OplAPI& aOplAPI)
	{
	aOplAPI.Push(OpxUtil::OplBool16(iIsTitleBarVisible));
	}

/**
 * Processes the OPL procedure
 * AfScreenInfo:(BYREF aXOrigin%, BYREF aYOrigin%, BYREF aWidth%, BYREF aHeight%)
 * Populates the vars with the following info:
 *  x position of origin
 *  y position of origin
 *  width of visible screen in pixels
 *  height of visible screen in pixels
 */
void CAppFrameUser::ScreenInfo(OplAPI& aOplAPI)
	{
	TInt16 *visibleHeight=aOplAPI.PopPtrInt16();//aHeight%
	TInt16 *visibleWidth=aOplAPI.PopPtrInt16();//aWidth%
	TInt16 *yOrigin=aOplAPI.PopPtrInt16();//aYOrigin%
	TInt16 *xOrigin=aOplAPI.PopPtrInt16();//aXOrigin%

	TRect visibleRect=aOplAPI.EikonEnv().ScreenDevice()->SizeInPixels();
#if defined(__UIQ__)
	visibleRect=CEikonEnv::Static()->EikAppUi()->ClientRect();
#else
	if (iButtonGroupContainer)
		if (iButtonGroupContainer->IsVisible())
			iButtonGroupContainer->ReduceRect(visibleRect);
#endif
	if (iStatusPane)
		if (iStatusPane->IsVisible())
			iStatusPane->ReduceRect(visibleRect);
	if (iIsTitleBarVisible)
		{
		TSize titleSize=iTitleWindow.Size();
		visibleRect.iTl.iY+=titleSize.iHeight;
		}

	aOplAPI.PutWord(xOrigin,(TInt16)visibleRect.iTl.iX);
	aOplAPI.PutWord(yOrigin,(TInt16)visibleRect.iTl.iY);
	aOplAPI.PutWord(visibleWidth,(TInt16)(visibleRect.iBr.iX-visibleRect.iTl.iX));
	aOplAPI.PutWord(visibleHeight,(TInt16)(visibleRect.iBr.iY-visibleRect.iTl.iY));
	aOplAPI.Push(0.0);
	}

// Fix bug [1167022] AppFrame OPX doesn't support AddToDesk on 9500

void CAppFrameUser::AddToDesk(OplAPI& aOplAPI)
	{
#if defined(USE_ADDTODESK)
	TPtrC linkDocName=aOplAPI.AppCurrentDocument();
	if (linkDocName.CompareF(KNullDesC)==0)	// No current document so add Application link
		{
 #if defined(__S80_DP2_0__)
		AddToDesk::AddAppToDeskL(aOplAPI.EikonEnv().FsSession(),aOplAPI.AppUid());
 #else
		CLinkApplication* linkApp=CLinkApplication::NewL();
		CleanupStack::PushL(linkApp);
		linkApp->SetApplicationUidL(aOplAPI.AppUid());
		LinkUtils::CreateLinkDocumentL(*linkApp);
		CleanupStack::PopAndDestroy(linkApp);
 #endif
		}
	else									// Add a link to the current document
		{
 #if defined(__S80_DP2_0__)
		AddToDesk::AddDocToDeskL(aOplAPI.EikonEnv().FsSession(),linkDocName);
 #else
		CLinkDocument* linkDoc=CLinkDocument::NewL();
		CleanupStack::PushL(linkDoc);
		linkDoc->SetDocumentL(linkDocName);
		LinkUtils::CreateLinkDocumentL(*linkDoc);
		CleanupStack::PopAndDestroy(linkDoc);
 #endif
		}
#else
	User::Leave(KOplErrNotSupported); // Not available on Series60
#endif
	aOplAPI.Push(0.0);
	}

void CAppFrameUser::MenuPaneToStartOn(OplAPI& aOplAPI)
	{
	aOplAPI.Push(KMenuPaneToStartOn);
	}

void CAppFrameUser::ViewSystemLog(OplAPI& aOplAPI)
	{
	_LIT(KLogAppFileName, "\\System\\Apps\\LogView\\LogView.app");
	CApaCommandLine* cmdLine=CApaCommandLine::NewLC();
	cmdLine->SetCommandL(EApaCommandRun);
	cmdLine->SetLibraryNameL(KLogAppFileName);
	EikDll::StartAppL(*cmdLine);
	CleanupStack::PopAndDestroy(); // cmdLine
	aOplAPI.Push(0.0);
	}

void CAppFrameUser::ToggleIrDA(OplAPI& aOplAPI)
	{ //AfToggleIrDA:
#if defined(USE_IRLISTEN)
	if (iIrListenAppUi)
		iIrListenAppUi->ToggleListeningL();
	else
		User::Leave(KErrNotSupported);
#else
	User::Leave(KOplErrNotSupported); // Not available on Series60
#endif
	aOplAPI.Push(0.0);
	}

/*void CAppFrameUser::IrDAIsActive(OplAPI& aOplAPI)
	{ //AfIrDAIsActive%:
#if defined(USE_IRLISTEN)
	if (iIrListenAppUi)
		aOplAPI.Push(TInt16(OpxUtil::OplBool16(iIrListenAppUi->ListeningL())));
	else
		User::Leave(KErrNotSupported);
#else
	User::Leave(KOplErrNotSupported); // Not available on Series60
#endif
	aOplAPI.Push(TInt16(0));
	}

void CAppFrameUser::ToggleBluetooth(OplAPI& aOplAPI)
	{ //AfToggleBluetooth:
#if defined(USE_IRLISTEN) && defined(__S80_DP2_0__)
	if (iIrListenAppUi)
		iIrListenAppUi->ToggleBtListeningL();
	else
		User::Leave(KErrNotSupported);
#else
	User::Leave(KOplErrNotSupported); // Not available on Series60 and 92xx
#endif
	aOplAPI.Push(0.0);
	}

void CAppFrameUser::BluetoothIsActive(OplAPI& aOplAPI)
	{ //AfBluetoothIsActive%:
#if defined(USE_IRLISTEN) && defined(__S80_DP2_0__)
	if (iIrListenAppUi)
		aOplAPI.Push(TInt16(OpxUtil::OplBool16(iIrListenAppUi->BtListeningL())));
	else
		User::Leave(KErrNotSupported);
#else
	User::Leave(KOplErrNotSupported); // Not available on Series60 and 92xx
#endif
	aOplAPI.Push(TInt16(0));
	}
*/
void CAppFrameUser::ConfirmationDialog(OplAPI& aOplAPI)
	{ //AfConfirmationDialog%:(aTitle$,aMessage$,aConfirmButtonTitle$)
#if defined(__SERIES60__)
	aOplAPI.PopString();
	aOplAPI.PopString();
	aOplAPI.PopString();
	User::Leave(KOplErrNotSupported); // Not available on Series60
	aOplAPI.Push(OpxUtil::OplBool16(0));
#elif defined(__UIQ__)
	aOplAPI.PopString();
	TPtrC message=aOplAPI.PopString();
	TPtrC title=aOplAPI.PopString();
	aOplAPI.Push(OpxUtil::OplBool16( CEikonEnv::Static()->QueryWinL(title,message)));
#else
	TPtrC confirmButtonTitle=aOplAPI.PopString();
	TPtrC message=aOplAPI.PopString();
	TPtrC title=aOplAPI.PopString();
	aOplAPI.Push(OpxUtil::OplBool16( CCknConfirmationDialog::RunDlgLD(title,message,NULL,&confirmButtonTitle) ));
#endif
	}

void CAppFrameUser::InfoDialog(OplAPI& aOplAPI)
	{ //AfInformationDialog%:(aTitle$,aMessage$)
	TPtrC aMessage=aOplAPI.PopString();
	TPtrC aTitle=aOplAPI.PopString();
#if defined(__SERIES60__)
	User::Leave(KOplErrNotSupported); // Not available on Series60
	aOplAPI.Push(OpxUtil::OplBool16(0));
#elif defined(__UIQ__)
	CEikonEnv::Static()->InfoWinL(aTitle,aMessage);
	aOplAPI.Push(OpxUtil::OplBool16( ETrue ));
#else
	aOplAPI.Push(OpxUtil::OplBool16( CCknInfoDialog::RunDlgLD(aTitle,aMessage,NULL) ));
#endif
	}

//
// Private
//

/**
 * Resize the title bar.
 * Returns ETrue if an OPL redraw is required, EFalse otherwise.
 */
TBool CAppFrameUser::ResizeTitleL(const CEikonEnv& aEikonEnv)
	{
	if (!iIsTitleBarVisible)
		{
		if (iStatusPane || iButtonGroupContainer)
			return ETrue;
		else
			return EFalse; //Invisible title and no other controls, so no redraw required.
		}

	// Recalc title bar size after status pane and/or CBA have changed.
	TRect titleRect=aEikonEnv.ScreenDevice()->SizeInPixels();
	if (iStatusPane)
		if (iStatusPane->IsVisible())
			iStatusPane->ReduceRect(titleRect);
	if (iButtonGroupContainer)
		if (iButtonGroupContainer->IsVisible())
			iButtonGroupContainer->ReduceRect(titleRect);
#if defined(__SERIES60__)||defined(__UIQ__)
	User::Leave(KOplErrNotSupported); // Not available on Series60
#else
	TPoint topLeft(titleRect.iTl.iX,0);
	TSize newSize(titleRect.iBr.iX-titleRect.iTl.iX,iTitleBar->Size().iHeight);
	iTitleWindow.SetPosition(topLeft);
	User::LeaveIfError(iTitleWindow.SetSizeErr(newSize));
	iTitleBar->SetPosition(TPoint(0,0)); //top left of container window
	iTitleBar->SetSize(iTitleWindow.Size());
	iTitleContext->Clear();
	iTitleBar->DrawNow();
#endif
	return ETrue;
	}

CAppFrameUser::~CAppFrameUser()
	{
#if !defined(__SERIES60__)&&!defined(__UIQ__)
	delete iTitleBar;
#endif
	delete iTitleContext;
	iTitleWindow.Close();
	if (iStatusPane)
		delete iStatusPane;
#if defined(__UIQ__)
	delete iButtonGroupOwner;
#endif
	delete iButtonGroupContainer;
	delete iAppFrameObserver;
#if defined(USE_IRLISTEN)
	delete iIrListenAppUi;
#endif
	};

COpxAppFrame::COpxAppFrame(OplAPI& aOplAPI) 
	:COpxBase(aOplAPI)
	{
	}

COpxAppFrame* COpxAppFrame::NewLC(OplAPI& aOplAPI)
	{
	COpxAppFrame* This=new(ELeave) COpxAppFrame(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL(aOplAPI);
	return This;
	}

void COpxAppFrame::ConstructL(OplAPI& aOplAPI)
	{
	iAppFrameUser=CAppFrameUser::NewL(aOplAPI);
	}

COpxAppFrame::~COpxAppFrame()
	{
	delete iAppFrameUser;
	Dll::FreeTls();
	}

void COpxAppFrame::RunL(TInt aProcNum)
	{
	switch (aProcNum)
		{
	case EOfferEvent:
		iAppFrameUser->OfferEvent(iOplAPI);
		break;
	case ESetCBAButton:
		iAppFrameUser->SetCBAButton(iOplAPI);
		break;
	case ESetCBAButtonDefault:
		iAppFrameUser->SetCBAButtonDefault(iOplAPI);
		break;
	case ESetCBAButtonDimmed:
		iAppFrameUser->SetCBAButtonDimmed(iOplAPI);
		break;
	case ECBAButtonDimmed:
		iAppFrameUser->CBAButtonDimmed(iOplAPI);
		break;
	case ESetCBAButtonVisible:
		iAppFrameUser->SetCBAButtonVisible(iOplAPI);
		break;
	case ECBAButtonVisible:
		iAppFrameUser->CBAButtonVisible(iOplAPI);
		break;
	case ESetCBAVisible:
		iAppFrameUser->SetCBAVisible(iOplAPI);
		break;
	case ECBAVisible:
		iAppFrameUser->CBAVisible(iOplAPI);
		break;
	case ECBAMaxButtons:
		iAppFrameUser->CBAMaxButtons(iOplAPI);
		break;
	case ESetStatus:
		iAppFrameUser->SetStatus(iOplAPI);
		break;
	case ESetStatusVisible:
		iAppFrameUser->SetStatusVisible(iOplAPI);
		break;
	case EStatusVisible:
		iAppFrameUser->StatusVisible(iOplAPI);
		break;
	case ESetTitle:
		iAppFrameUser->SetTitle(iOplAPI);
		break;
	case ESetTitleAreaWidth:
		iAppFrameUser->SetTitleAreaWidth(iOplAPI);
		break;
	case ESetTitleVisible:
		iAppFrameUser->SetTitleVisible(iOplAPI);
		break;
	case ETitleVisible:
		iAppFrameUser->TitleVisible(iOplAPI);
		break;
	case EScreenInfo:
		iAppFrameUser->ScreenInfo(iOplAPI);
		break;
	case EAddToDesk:
		iAppFrameUser->AddToDesk(iOplAPI);
		break;
	case EMenuPaneToStartOn:
		iAppFrameUser->MenuPaneToStartOn(iOplAPI);
		break;
	case EViewSystemLog:
		iAppFrameUser->ViewSystemLog(iOplAPI);
		break;
	case EToggleIrDA:
		iAppFrameUser->ToggleIrDA(iOplAPI);
		break;
	case EConfirmationDialog:
		iAppFrameUser->ConfirmationDialog(iOplAPI);
		break;
	case EInfoDialog:
		iAppFrameUser->InfoDialog(iOplAPI);
		break;

/*	case EIrDAIsActive:
		iAppFrameUser->IrDAIsActive(iOplAPI);
		break;
	case EToggleBluetooth:
		iAppFrameUser->ToggleBluetooth(iOplAPI);
		break;
	case EBluetoothIsActive:
		iAppFrameUser->BluetoothIsActive(iOplAPI);
		break;
*/
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxAppFrame::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& iOplAPI)
	{
	COpxAppFrame* tls=((COpxAppFrame*)Dll::Tls());
	if (tls==NULL) // tls is NULL on loading an OPX DLL (also after unloading it)
		{
		tls=COpxAppFrame::NewLC(iOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop();    // tls
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
