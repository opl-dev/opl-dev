rem Symbian OS v6.0 OPL Demo Application
rem
rem Copyright © 2000-2002 Symbian Ltd.
rem
rem Last updated 21/10/2002

DECLARE EXTERNAL
INCLUDE "AppFrame.oxh"
INCLUDE "System.oxh"
INCLUDE "DBase.oxh"
INCLUDE "SendAs.oxh"
INCLUDE "Const.oph"
INCLUDE "DemoOPL.oph"
INCLUDE "DemoOPL.hlp.oph"

APP DemoOPL,KApplicationUID&
	ICON "DemoIcon.mbm"
ENDA

rem
rem Setup the AppFrame, complete our GLOBAL
rem definitions and other setup tasks before
rem calling the main event handler loop
rem
PROC MainSetup:
rem Required GLOBAL variables
GLOBAL gW%,gH%,ScreenWidth%,ScreenHeight%,Dia%,MenuPos%
GLOBAL InstallPath$(KdFILENameLen%),AfCBADefaultButton%
GLOBAL BitmapIDs&(KMaxBitmapsNeeded%),CloseNow%
GLOBAL SaCascs$(KMaxSendAsTypes%,KMaxSendAsCaptionLen%)
GLOBAL SaKeys&(KMaxSendAsTypes%),SaMaxTypes&,SaNextFreeKey&
rem These values are GLOBAL preferences
GLOBAL AfOn%,AfStatusType%,AfStatusOn%,AfTitleOn%
GLOBAL View%,MainScreenTextColor%,AppHotKey%,SampleINI%
GLOBAL SampleINI&,SampleINI,SampleINI$(KMaxStringLen%)
GLOBAL SampleINIDate&,SampleINITime&,SampleINIPw$(KdXINPUTMaxLen%)
LOCAL Off%(6),ScreenInfo%(10)

	rem Store these before any manipulation is done
	gW%=gWIDTH :gH%=gHEIGHT :CloseNow%=KFalse%
	SCREENINFO ScreenInfo%()
	ScreenWidth%=ScreenInfo%(KSInfoAScrW%)
	ScreenHeight%=ScreenInfo%(KSInfoAScrH%)

	rem Find where we're running from
	TRAP MKDIR "C:\System\Apps\"+KApplicationName$
	InstallPath$=PARSE$(CMD$(KCmdAppName%),"",Off%())
	InstallPath$=LEFT$(InstallPath$,Off%(KParseAOffFilename%)-1)
	rem Check for required files
	IF NOT EXIST(InstallPath$+KMBMFileName$)
		InfoDialog:("",KMBMFileName$+" not found.",KApplicationName$+" must close.")
		STOP
	ENDIF
	InitialiseDefaultSettings:
	
	rem Set the menu to start on our own first pane
	rem rather than the Task List or any other menu
	rem panes inserted by the System
	MenuPos%=AfMenuPaneToStartShowing%:

	rem Setup the AppFrame (but don't display it yet)
	AfSetCBAButton:(KCBAButtonFlipStatus%,"Flip"+CHR$(KLineFeed&)+"status",0,0,"HandleCBA")
	AfSetCBAButton:(KCBAButtonChangeView%,"Change"+CHR$(KLineFeed&)+"view",0,0,"HandleCBA")
	AfSetCBAButton:(KCBAButtonAbout%,"About",0,0,"HandleCBA")
	AfSetCBAButton:(KCBAButtonClose%,"Close",0,0,"HandleCBA")
	AfSetTitle:(KApplicationName$,KAfTitleTypeMainTitle%)
	AfSetTitle:("Brought to you by Symbian Ltd.",KAfTitleTypeTitleExtension%)

	IF NOT EXIST(KIniFile$)
		rem Write a default INI file if one doesn't exist
		WriteINI:(KTrue%)
	ENDIF
	ReadINI:

	rem Now we've picked up the user preferences
	rem (if any were set) from the INI, draw the
	rem AppFrame as appropriate
	AfSetStatus%:(AfStatusType%)
	AfSetCBAVisible%:(AfOn%)
	AfSetStatusVisible%:(AfStatusOn%)
	AfSetTitleVisible%:(AfTitleOn%)
	
	rem Draw the main display (this will take into
	rem account the AppFrame settings)
	DrawScreen:(KTrue%)

	rem Set the UID for Help to the one used in our
	rem Help file (happens to be the same as our app)
	SETHELPUID KApplicationUID&

	rem Set the default Help view
	SETHELP KHelpView%,KQuick_Start$
	SETHELP KHelpDialog%,KQuick_Start$
	SETHELP KHelpMenu%,KQuick_Start$

	rem Setup the available 'SendAs' requirements
	SetupSendAsKeysAndMenu:

	rem Pass control back out to the main event loop
	rem We should never have control returned to this
	rem point once we enter the loop.
	MainLoop:
ENDP

rem
rem Our callback to handle events from the CBA
rem
PROC HandleCBA:(aButtonId&)
EXTERNAL CloseNow%
	IF aButtonId&=KCBAButtonFlipStatus%
		SwitchStatusSize:
	ELSEIF aButtonId&=KCBAButtonChangeView%
		SwitchView:(KViewToggle%)
	ELSEIF aButtonId&=KCBAButtonAbout%
		AboutBox:
	ELSEIF aButtonId&=KCBAButtonClose%
		rem Can't call Exit: directly as OPL prevents
		rem you using STOP in a callback function
		CloseNow%=KTrue%
	ENDIF
ENDP

rem
rem This is the main control loop. It receives
rem events and then does some basic branching
rem to pass the real processing to other, dedicated
rem procedures. Main error handler is also here.
rem
PROC MainLoop:
EXTERNAL CloseNow%,View%
LOCAL Event&(16),E,AfOffered%,c$(KdFILENameLen%)
	Top::
	E=ERR :ONERR OFF
	ONERR Error::
	DO
		GETEVENT32 Event&()

		rem Check for messages from System and act
		rem on any we should be supporting.
		c$=GETCMD$
		IF LEFT$(c$,1)=KGetCmdLetterExit$
			Exit:
		ELSEIF LEFT$(c$,1)=KGetCmdLetterBackup$
			Exit:
		ELSEIF LEFT$(c$,1)=KGetCmdLetterBroughtToFGround$
			Gi:("Brought to foreground event registered by GETCMD$",KBusyTopRight%,0)
		ENDIF

		AfOffered%=AfOfferEvent%:(Event&(1),Event&(3),Event&(4),Event&(5),Event&(6),Event&(7))
		IF AfOffered%=KFalse%
			IF Event&(KEvAType%)=KEvFocusLost&
				HotKeyHandler:
			ELSEIF Event&(KEvAType%)=KEvDateChanged&
				Gi:("System date change event registered",KBusyTopRight%,0)
			ELSE
				IF (Event&(KEvAType%)<>KEvKeyDown& AND Event&(KEvAType%)<>KEvKeyUp&)
					IF (Event&(KEvAType%)<>KEvPtr&) AND (Event&(KEvAType%)<>KEvPtrEnter&) AND (Event&(KEvAType%)<>KEvPtrExit&)
						ProcessKeyEvent:(Event&(KEvAType%),Event&(KEvAKMod%),Event&(KEvAScan%))
					ELSE
						rem The Crystal Reference Design for Symbian OS v6.0 does not
						rem include pen support. If it did, this is where we could
						rem check pen events
					ENDIF
				ENDIF
			ENDIF
		ELSE
			IF View%=KViewKeyDebugScreen%
				PRINT "Key event consumed by AppFrame"
			ENDIF
		ENDIF
	UNTIL (ERR<>E) OR (CloseNow%)
	rem CloseNow% is for CBA 'Close' button
	rem functionality
	IF CloseNow%
		CloseNow%=KFalse% rem Just in case
		Exit:
	ENDIF
	GOTO Top::
	Error::
	InfoDialog:("E","","")
	GOTO Top::
ENDP

rem
rem Takes aKey&, aMod& and aScanCode& as passed
rem from 'MainLoop' procedure, decodes them and
rem calls appropriate procedures based on the results
rem
PROC ProcessKeyEvent:(aKey&,aMod&,aScanCode&)
EXTERNAL AfOn%,AfTitleOn%,AfStatusOn%,AfCBADefaultButton%
EXTERNAL View%,SaNextFreeKey&
LOCAL Key&,Mod&,CapsLock%,Shift%,Ctrl%,Fn%,AllMods&
LOCAL CtrlShiftMod&,CtrlShiftFnMod&,CtrlFnMod&
LOCAL ShiftFnMod&

	rem Values for all modifier combinations
	AllMods&=KKModShift% OR KKModControl% OR KKModFn%
	CtrlShiftMod&=KKModControl% OR KKModShift%
	CtrlShiftFnMod&=CtrlShiftMod& OR KKModFn%
	ShiftFnMod&=KKModShift% OR KKModFn%
	CtrlShiftMod&=KKModControl% OR KKModShift%
	
	CapsLock%=KFalse% :Shift%=KFalse% :Ctrl%=KFalse% :Fn%=KFalse%
	Key&=aKey& :Mod&=aMod&
	
	rem Check for and deal with CAPS lock
	IF (aMod&>=KKModCaps% AND aMod&<KKModFn%) OR (aMod&>=KKModCaps%+KKModFn% AND aMod&<AllMods&)
		CapsLock%=KTrue%
		Mod&=aMod&-KKModCaps%
	ELSEIF (aMod&>AllMods&)
		Mod&=0
	ENDIF
	
	rem Examine the modifiers
	IF (Mod&-KKModFn%)>=0			:Fn%=KTrue%			:Mod&=Mod&-KKModFn%			:ENDIF
	IF (Mod&-KKModControl%)>=0	:Ctrl%=KTrue%		:Mod&=Mod&-KKModControl%	:ENDIF
	IF (Mod&-KKModShift%)>=0		:Shift%=KTrue%	:Mod&=Mod&-KKModShift%		:ENDIF
	
	rem First handle any system keys
	IF (Key&=KKeyMenu32&) OR (Key&=KKeySidebarMenu32&)
		Key&=Menu&:
		rem Interpret any return from the menu
		IF (Key&>=ASC("A")) AND (Key&<=ASC("Z"))
			Key&=Key&-(ASC("A")-1) :Ctrl%=KTrue% :Shift%=KTrue%
		ELSE
			Key&=Key&-(ASC("a")-1) :Ctrl%=KTrue% :Shift%=KFalse%
		ENDIF
	ELSEIF Key&=KKeyHelp32&
		SHOWHELP
		RETURN
	ELSEIF Key&=KKeyZoomIn32&
		Gi:("Zoom in was pressed",KBusyTopRight%,0)
		RETURN
	ELSEIF Key&=KKeyZoomOut32&
		Gi:("Zoom out was pressed",KBusyTopRight%,0)
		RETURN
	ELSEIF Key&=KKeyIncBrightness32&
		Gi:("Screen brightness was toggled",KBusyTopRight%,0)
		RETURN
	ENDIF

	rem Now create a single Key& value in the %? form
	rem where %r = Ctrl+R or %R = Shift+Ctrl+R
	Mod&=0
	IF Shift%		:Mod&=Mod&+KKModShift%		:ENDIF
	IF Ctrl%		:Mod&=Mod&+KKModControl%	:ENDIF
	IF Fn%			:Mod&=Mod&+KKModFn%			:ENDIF
	Key&=Key&+ASC("a")-1
	IF Mod&=CtrlShiftMod&
		Key&=Key&-(ASC("a")-ASC("A"))
	ENDIF

	IF View%=KViewKeyDebugScreen%
		PRINT "Key event: Key =",Key&,", Mods =",Mod&,", ScanCode =",

		IF (aScanCode&>KMaxInt%) OR (aScanCode&<KMinInt%)
			GOTO NA::
		ENDIF
		VECTOR aScanCode&
			ScanDel,ScanTab,ScanEnter,ScanEsc,ScanSpace
		ENDV
		rem If we get to the ENDV, aScanCode& wasn't in
		rem one of the above values, so print that
		NA::
		PRINT "N/A ("+NUM$(aScanCode&,9)+")"
		GOTO EndOfScanCodePrints::
		
		ScanDel::
		PRINT "Del"
		GOTO EndOfScanCodePrints::

		ScanTab::
		PRINT "Tab"
		GOTO EndOfScanCodePrints::

		ScanEnter::
		PRINT "Enter"
		GOTO EndOfScanCodePrints::

		ScanEsc::
		PRINT "Esc"
		GOTO EndOfScanCodePrints::

		ScanSpace::
		PRINT "Space"
		GOTO EndOfScanCodePrints::
		
		EndOfScanCodePrints::
	ENDIF
	
	rem First handle default CBA button (Enter key)
	IF Mod&=0 AND (aScanCode&=KScanEnter%)
		HandleCBA:(AfCBADefaultButton%)
	ELSEIF Mod&=0 AND (aScanCode&=KScanTab% OR aScanCode&=KScanDel%) rem Prevent Tab/Del keys acting as Ctrl+I/H
		RETURN
	ELSEIF (Key&=KToggleIrDAKey% AND Fn%=KTrue% AND (Ctrl%=KFalse% AND Shift%=KFalse%)) OR (Key&=SaNextFreeKey&+KFreeKeyToggleIrDA%)
		AfToggleIrDA:
		RETURN	
	ELSEIF Fn% rem Any other Fn modified keys should not action anything
		RETURN
	ELSEIF (Key&>=KSendAsHotkeyStart&) AND (Key&<SaNextFreeKey&)
		HandleSendAs:(Key&)
	ELSEIF Key&=SaNextFreeKey&+KFreeKeyAddToDesk%
		AfAddToDesk:
	ELSEIF Key&=%A
		AboutBox:
	ELSEIF Key&=%c
		SimpleDBCreate:(KHardcodedDBFile$)
	ELSEIF Key&=%d
		GeneralDialogDemo:
	ELSEIF Key&=%k
		SettingsDialog:
	ELSEIF Key&=%K
		SwitchView:(KViewKeyDebugScreen%)
	ELSEIF Key&=%L
		LaunchLog:
	ELSEIF Key&=%q
		SwitchView:(KViewToggle%)
	ELSEIF Key&=%r
		RunOPXTest:
	ELSEIF Key&=%S
		SwitchView:(KViewMainScreen%)
	ELSEIF Key&=%t
		IF AfOn%
			AfOn%=KFalse%
		ELSE
			AfOn%=KTrue%
		ENDIF
		rem The CBA and Status view states are
		rem always the same in this app
		AfStatusOn%=AfOn%
		AfSetStatusVisible%:(AfStatusOn%)
		AfSetCBAVisible%:(AfOn%)
		IF View%=KViewKeyDebugScreen%
			DrawScreen:(KFalse%)
		ELSE
			DrawScreen:(KTrue%)
		ENDIF
	ELSEIF Key&=%T
		IF AfTitleOn%
			AfTitleOn%=KFalse%
		ELSE
			AfTitleOn%=KTrue%
		ENDIF
		AfSetTitleVisible%:(AfTitleOn%)
		DrawScreen:(KTrue%)
	ELSEIF Key&=%u
		SimpleDBQuery:(KHardcodedDBFile$)
	ENDIF
ENDP

rem
rem Display the main menu and return the result
rem
PROC Menu&:
EXTERNAL MenuPos%,AfTitleOn%,AfOn%,View%
EXTERNAL SaCascs$(),SaKeys&(),SaNextFreeKey&
LOCAL Ret&,AfTitleFlag%,AfFlag%,MainViewHKey%
LOCAL KeysViewHKey%,Ellipsis$(1)
	Ellipsis$=CHR$(KEllipsis&)
	IF AfTitleOn%		:AfTitleFlag%=KMenuSymbolOn%	:ENDIF
	IF AfOn% 				:AfFlag%=KMenuSymbolOn% 			:ENDIF
	IF View%=KViewMainScreen%
		MainViewHKey%=KMenuSymbolOn%
	ELSEIF View%=KViewKeyDebugScreen%
		KeysViewHKey%=KMenuSymbolOn%
	ENDIF
	MainViewHKey%=%S OR MainViewHKey% OR KMenuOptionStart%
	KeysViewHKey%=%K OR KeysViewHKey% OR KMenuOptionEnd%
	mINIT
	mCASC "Change view","Main screen",MainViewHKey%,"Key output",KeysViewHKey%
	mCASC "Databases","Create",%c,"Query"+Ellipsis$,%u
	mCASC "Send",SaCascs$(1),SaKeys&(1),SaCascs$(2),SaKeys&(2),SaCascs$(3),SaKeys&(3),SaCascs$(4),SaKeys&(4),SaCascs$(5),SaKeys&(5),SaCascs$(6),SaKeys&(6),SaCascs$(7),SaKeys&(7),SaCascs$(8),SaKeys&(8),SaCascs$(9),SaKeys&(9),SaCascs$(10),SaKeys&(10),SaCascs$(11),SaKeys&(11),SaCascs$(12),SaKeys&(12)
	mCARD "File","Send>",-(SaNextFreeKey&),"Add to Desk",SaNextFreeKey&+KFreeKeyAddToDesk%
	mCARD "Examples","General dialog"+Ellipsis$,%d,"Databases>",-(SaNextFreeKey&+KFreeKeyDatabaseCascade%),"Run OPX test"+Ellipsis$,%r
	mCARD "View","Show title",%T OR AfTitleFlag% OR KMenuCheckBox%,"Full screen",-(%t OR AfFlag% OR KMenuCheckBox%),"Change view>",%q
	mCARD "Tools","Settings"+Ellipsis$,-%k,"About "+KApplicationName$+Ellipsis$,-%A,"Log",%L,"Receive via infrared",SaNextFreeKey&+KFreeKeyToggleIrDA%
	LOCK ON :Ret&=MENU(MenuPos%) :LOCK OFF
	RETURN Ret&
ENDP

rem
rem Draw the screen border and some basic text.
rem The screen width will vary depending on the
rem AppFrame being on or off.
rem
PROC DrawScreen:(aClearScreen%)
EXTERNAL BitmapIDs&(),InstallPath$,View%,ScreenWidth%
EXTERNAL ScreenHeight%,gH%,MainScreenTextColor%
EXTERNAL AfCBADefaultButton%
LOCAL XOrigin%,YOrigin%,WinW%,WinH%,LogoW%,LogoH%
LOCAL Title$(KMaxStringLen%),ScreenInfo%(10),LostLines%
	AfScreenInfo:(XOrigin%,YOrigin%,WinW%,WinH%)
	rem First set the title bar's extension text width area to be 5/8
	rem of the total width (since in this example application it's
	rem longer than the Main text is)
	AfSetTitleAreaWidth:(KAfTitleTypeTitleExtension%,(WinW%/8)*5)
	gUSE 1
	IF aClearScreen%
		gCLS :CLS
	ENDIF
	rem Calculate the number of console lines 'lost' for
	rem the current window height vs. the default height
	SCREENINFO ScreenInfo%()
	LostLines%=((gH%-WinH%)/ScreenInfo%(KSInfoAPixH%))
	IF LostLines%<>0
		rem Lose 1 extra line in case of any rounding errors
		rem in the above calculation
		LostLines%=LostLines%+1
	ENDIF
	rem Set the screen size to take the lost lines into
	rem account. This allows the console to continue to scroll
	rem output correctly
	SCREEN ScreenWidth%,ScreenHeight%-LostLines%,1,1
	rem Continue with the rest of the screen drawing...
	gSETWIN XOrigin%,YOrigin%,WinW%,WinH%
	IF View%=KViewMainScreen%
		Title$=KApplicationName$+" Version "+PrintableVersionNo$:
		gBORDER KBordSglGap%
		IF BitmapIDs&(KBitmapSymbianLogo%)=0
			BitmapIDs&(KBitmapSymbianLogo%)=gLOADBIT(InstallPath$+KMBMFileName$,KBitmapSymbianLogo%-1,0)
		ENDIF
		gUSE BitmapIDs&(KBitmapSymbianLogo%)
		LogoW%=gWIDTH :LogoH%=gHEIGHT
		gUSE 1
		gAT (WinW%-LogoW%)/2,(WinH%-LogoH%)/2
		gCOPY BitmapIDs&(KBitmapSymbianLogo%),0,0,LogoW%,LogoH%,0
		
		gFONT KFontArialNormal13&
		SetColor:(MainScreenTextColor%)
		
		rem Print the title centered in the gap between
		rem the top of the image and top of the screen
		gAT 5,((WinH%-LogoH%)/2)/2
		gPRINTB Title$,WinW%-(5*2),KgPRINTBCenteredAligned%
		
		rem Print the description centered in the gap
		rem between the bottom of the image screen
		gAT 5,WinH%-((WinH%-(((WinH%-LogoH%)/2)+LogoH%))/2)
		gPRINTB KMainScreenDescription$,WinW%-(5*2),KgPRINTBCenteredAligned%
	ENDIF
	
	rem Set the default CBA button
	IF View%=KViewMainScreen%
		AfCBADefaultButton%=KCBAButtonChangeView%
	ELSE
		AfCBADefaultButton%=KCBAButtonFlipStatus%
	ENDIF
	AfSetCBAButtonDefault:(AfCBADefaultButton%)
ENDP

rem
rem This will switch the AppFrame status pane
rem between the available narrow/wide settings
rem
PROC SwitchStatusSize:
EXTERNAL View%
LOCAL Type%
	AfStatusVisible%:(Type%)
	IF (Type%)=KAfStatusPaneTypeNarrow%
		AfSetStatus%:(KAfStatusPaneTypeWide%)
	ELSE
		AfSetStatus%:(KAfStatusPaneTypeNarrow%)
	ENDIF
	IF View%=KViewKeyDebugScreen%
		DrawScreen:(KFalse%)
	ELSE
		DrawScreen:(KTrue%)
	ENDIF
ENDP

rem
rem This will switch the main application view
rem to either a specific view as passed in
rem aSwitchType% or (if KViewToggle% is specified)
rem toggle between the available views.
rem
PROC SwitchView:(aSwitchType%)
EXTERNAL View%
LOCAL OldView%
	OldView%=View%
	IF aSwitchType%<>KViewToggle%
		View%=aSwitchType%
	ELSE
		IF View%=KViewMainScreen%
			View%=KViewKeyDebugScreen%
		ELSE
			View%=KViewMainScreen%
		ENDIF
	ENDIF
	IF View%<>OldView%
		DrawScreen:(KTrue%)
	ENDIF
ENDP

rem
rem The procedure called when our app goes to the background.
rem It detects our application hotkey and brings our app
rem to the foreground if it is pressed. If the app comes to
rem the foreground in any other way the hotkey request is
rem cancelled and control is passed back to the main loop.
rem
PROC HotKeyHandler:
EXTERNAL AppHotKey%
LOCAL Event&(16),Key&,Mods&,Handle&,KeyDetected%
	rem Mods& should be set by a CONST really but we can't
	rem create a CONST by ORing two values, so we have to
	rem hard-code it here
	Key&=AppHotKey% :Mods&=KSyModifierCtrl& OR KSyModifierFunc&
	Handle&=SyCaptureKey&:(Key&,Mods&,Mods&)
	KeyDetected%=KFalse%
	DO
		GETEVENT32 Event&()
		IF LEFT$(GETCMD$,1)=KGetCmdLetterExit$
			SyCancelCaptureKey:(Handle&)
			Exit:
		ELSEIF Event&(KEvAType%)<&60
			KeyDetected%=KTrue%
		ENDIF
	UNTIL KeyDetected% OR Event&(KEvAType%)=KEvFocusGained&
	SyCancelCaptureKey:(Handle&)
	IF KeyDetected%
		SySetForeground:
	ENDIF
ENDP

rem
rem The procedure called when our app shuts down.
rem This PROC also does various tidying up.
rem
PROC Exit:
EXTERNAL BitmapIDs&()
LOCAL i%
	rem Use our own error handler because if any errors
	rem happen during shutdown we don't want to be thrown
	rem back into the main event handler where errors
	rem are normally dealt with as that will abort the
	rem shutdown procedure
	ONERR JustStop::
	WriteINI:(KFalse%)
	
	rem In the interest of good order, close all
	rem windows which we created.
	i%=1
	DO
		IF BitmapIDs&(i%)
			gCLOSE BitmapIDs&(i%)
		ENDIF
		i%=i%+1
	UNTIL i%=KMaxBitmapsNeeded%+1

	JustStop::
	ONERR OFF
	STOP
ENDP

rem
rem Our main settings/preferences dialog
rem
PROC SettingsDialog:
EXTERNAL Dia%,MainScreenTextColor%,View%,AppHotKey%
LOCAL OldColorChoice%
	SETHELP KHelpDialog%,KSettings$

	OldColorChoice%=MainScreenTextColor%
	dINIT "Settings"
	dCHOICE AppHotKey%,"Application hotkey (Ctrl+Chr+)","A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z"
	dCHOICE MainScreenTextColor%,"Main screen text colour","Black,Dark grey,Dark red,Dark green,Dark yellow,Dark blue,Dark magenta,Dark cyan,Red,Green,Yellow,Blue,Magenta,Cyan,Grey,Light grey,Lighter grey,White",KTrue%
	dBUTTONS "Close",KdBUTTONEnter%
	LOCK ON :Dia%=DIALOG :LOCK OFF
	SETHELP KHelpDialog%,KQuick_Start$
	IF Dia%<>KdBUTTONEnter%
		RETURN
	ELSEIF OldColorChoice%<>MainScreenTextColor% AND View%=KViewMainScreen%
		DrawScreen:(KTrue%)
	ENDIF
ENDP

rem
rem A dialog used for demonstration purposes to show
rem a variety of controls available to OPL.
rem
PROC GeneralDialogDemo:
EXTERNAL Dia%,SampleINI%,SampleINI&,SampleINI
EXTERNAL SampleINI$,SampleINIDate&,SampleINITime&
EXTERNAL SampleINIPw$
LOCAL ShortAsLong&,AllowNullText%,OldPw$(KdXINPUTMaxLen%)
	
	SETHELP KHelpDialog%,KGeneral_Dialog$
	ShortAsLong&=SampleINI%
	OldPw$=SampleINIPw$
	
	Top::
	dINIT "Example dialog"
	dTEXT "","These values should be saved in the INI file",KDTextCenter%
	dTEXT "","and get restored each time "+KApplicationName$+" is run.",KDTextCenter%
	dLONG ShortAsLong&,"Short value",KMinInt%,KMaxInt%
	dLONG SampleINI&,"Long value",KMinLong&,KMaxLong&
	dFLOAT SampleINI,"Floating point",KMinFloat,KMaxFloat
	dEDIT SampleINI$,"Text editor",10
	dCHOICE AllowNullText%,"Allow text to be null","No,Yes"
	dDATE SampleINIDate&,"Date editor",DAYS(01,01,1980),DAYS(31,12,2099)
	dTIME SampleINITime&,"Time editor",KdTIMEWithSeconds% OR KdTIME24Hour%,0,KMaxdTIMEValue&
	dXINPUT SampleINIPw$,"Secret editor",KTrue%
	dBUTTONS KdBUTTONBlank$,KdBUTTONBlank%,"Carefully"+CHR$(KLineFeed&)+"placed",%p,KdBUTTONBlank$,KdBUTTONBlank%,"Close",KdBUTTONEnter%
	LOCK ON :Dia%=DIALOG :LOCK OFF
	SETHELP KHelpDialog%,KQuick_Start$
	IF Dia%=%p
		Gi:("You selected the carefully placed second CBA button!",KBusyTopRight%,0)
		GOTO Top::
	ELSEIF Dia%<>KdBUTTONEnter%
		RETURN
	ELSEIF SampleINI$="" AND AllowNullText%<>KNoYesChoiceYes%
		SampleINI$="Ooops. The 'Allow text to be null' option wasn't set, but you still left the text null. So I filled it in for you!"
	ELSEIF OldPw$<>SampleINIPw$
		Gi:("This time you entered a different password to "+OldPw$,KBusyTopRight%,0)
	ELSEIF OldPw$=SampleINIPw$
		Gi:("You entered the same password as before ("+OldPw$+")",KBusyTopRight%,0)
	ENDIF
	
	SampleINI%=ShortAsLong&
	WriteINI:(KFalse%)
ENDP

rem
rem A procedure to create a database with 2 tables
rem and fill in some example data. This showcases
rem the power and ease of use of DBMS from OPL.
rem
PROC SimpleDBCreate:(aFileName$)
LOCAL Key&

	rem If the file exists, ask the user if they want to replace it
	IF EXIST(aFileName$)
		IF NOT (AfConfirmationDialog%:("Replace existing file?","The sample database already exists","Replace"+CHR$(KLineFeed&)+"file"))
			RETURN
		ELSE
			DELETE aFileName$
		ENDIF
	ENDIF
	
	rem Create the first table - "ArtistTable". Note
	rem that, for example, the (50) used here should
	rem match the CONST for the equivalent maximum
	rem name length as defined in our own OPH file.
	CREATE aFileName$+" FIELDS ArtistName(50) TO ArtistTable",A,ArtistName$
	USE A :CLOSE
	
	rem Create the second table - "Items"
	CREATE aFileName$+" FIELDS ItemName(100), ArtistName(50), Price, Got, Want TO ItemsTable",A,ItemName$,ArtistName$,Price,Got%,Want%
	USE A :CLOSE
	
	rem Create a key for an index
	Key&=DBNewKey&:
	DBAddField:(Key&,"ArtistName",KDBAscending&)
	DBAddFieldTrunc:(Key&,"ItemName",KDBAscending&,20)
	DBSetComparison:(Key&,KDBCompareNormal&)
	
	rem Create an index on the Items table using Key&
	DBCreateIndex:("MainIndex",Key&,aFileName$,"ItemsTable")
	
	DBDeleteKey:(Key&)
	
	rem Now open the file and add some test data
	OPEN aFileName$+" SELECT * FROM ArtistTable",A,ArtistName$
	BEGINTRANS
	_AddArtistToDB:("The Corrs")
	_AddArtistToDB:("Radiohead")
	_AddArtistToDB:("R.E.M.")
	_AddArtistToDB:("Coldplay")
	_AddArtistToDB:("Richard Ashcroft")
	_AddArtistToDB:("Sting")
	COMMITTRANS
	USE A :CLOSE
	
	OPEN aFileName$+" SELECT * FROM ItemsTable",A,ItemName$,ArtistName$,Price,Got%,Want%
	BEGINTRANS
	_AddItemToDB:("Forgiven Not Forgotten","The Corrs",6.99,KTrue%,KTrue%)
	_AddItemToDB:("Talk On Corners","The Corrs",9.99,KTrue%,KTrue%)
	_AddItemToDB:("MTV Unplugged","The Corrs",11.99,KTrue%,KTrue%)
	_AddItemToDB:("In Blue","The Corrs",12.99,KTrue%,KTrue%)
	_AddItemToDB:("The Best Of","The Corrs",12.99,KTrue%,KTrue%)
	_AddItemToDB:("Automatic For The People","R.E.M.",12.99,KTrue%,KTrue%)
	_AddItemToDB:("Murmur","R.E.M.",12.99,KFalse%,KTrue%)
	_AddItemToDB:("New Adventures In Hi-Fi","R.E.M.",13.99,KFalse%,KTrue%)
	_AddItemToDB:("Out Of Time","R.E.M.",12.99,KFalse%,KTrue%)
	_AddItemToDB:("Pablo Honey","Radiohead",14.99,KFalse%,KTrue%)
	_AddItemToDB:("The Bends","Radiohead",11.99,KTrue%,KFalse%)
	_AddItemToDB:("Yellow","Coldplay",12.99,KFalse%,KTrue%)
	COMMITTRANS
	USE A :CLOSE
	
	COMPACT aFileName$

	Gi:(aFileName$+" created",KBusyBottomRight%,0)
ENDP


rem
rem This procedure can be used to construct an SQL
rem query on our example database. Some very complex
rem work is then done to show the results in a
rem dEDITMULTI control of a dialog - this can be
rem ignored by all but advanced OPL programmers
rem

rem This should be a sufficient buffer size (1K)
rem in which to hold our query results.
CONST KMaxDBResultsBufferSize&=1024

PROC SimpleDBQuery:(aFileName$)
EXTERNAL Dia%
LOCAL ArtistMatch%,WantMatch%,i%,j%,OurThread&,Previous&
LOCAL ArtistNames$(KMaxArtistNames%,KMaxArtistNameLen%)
LOCAL _Buffer&,TotalBufferLen&,Output%,Temp$(255)
	IF NOT EXIST(aFileName$)
		InfoDialog:("","Database does not exist.","Use the Create option first.")
		RETURN
	ENDIF

	SETHELP KHelpDialog%,KDatabase_Query$
	
	OPEN aFileName$+" FOLDED SELECT * FROM ArtistTable",A,ArtistName$
	WHILE (NOT EOF) AND (i%<KMaxArtistNames%)		
		i%=i%+1
		ArtistNames$(i%)=A.ArtistName$
		NEXT
	ENDWH
	USE A :CLOSE

	dINIT "Query database"
	rem **** Dynamic dCHOICE building starts ****
	IF i%=1
		dCHOICE ArtistMatch%,"Artist name",ArtistNames$(1),KTrue%
	ELSE
		dCHOICE ArtistMatch%,"Artist name",ArtistNames$(1)+",...",KTrue% :j%=2
		WHILE (j%<>KMaxArtistNames%) AND (j%<i%)
			dCHOICE ArtistMatch%,"",ArtistNames$(j%)+",..."
			j%=j%+1
		ENDWH
		dCHOICE ArtistMatch%,"",ArtistNames$(j%)
	ENDIF
	rem **** Dynamic dCHOICE building ends ****
	dCHOICE WantMatch%,"Acqusition status","Ignore,Want,Don't want"
	dCHOICE Output%,"Query output file","None,Unicode text,ASCII text"
	dBUTTONS "Close",KdBUTTONEnter%
	LOCK ON :Dia%=DIALOG :LOCK OFF
	SETHELP KHelpDialog%,KQuick_Start$
	IF Dia%<>KdBUTTONEnter%
		RETURN
	ENDIF
	IF WantMatch%=1
		OPEN aFileName$+" FOLDED SELECT * FROM ItemsTable WHERE ArtistName LIKE '"+ArtistNames$(ArtistMatch%)+"' ORDER BY ItemName ASC",A,ItemName$,ArtistName$,Price,Got%,Want%
	ELSEIF WantMatch%=2
		rem Assumes KTrue%=-1
		OPEN aFileName$+" FOLDED SELECT * FROM ItemsTable WHERE ArtistName LIKE '"+ArtistNames$(ArtistMatch%)+"' AND Want=-1 ORDER BY ItemName ASC",A,ItemName$,ArtistName$,Price,Got%,Want%
	ELSEIF WantMatch%=3
		rem Assumes KFalse%=0
		OPEN aFileName$+" FOLDED SELECT * FROM ItemsTable WHERE ArtistName LIKE '"+ArtistNames$(ArtistMatch%)+"' AND Want=0 ORDER BY ItemName ASC",A,ItemName$,ArtistName$,Price,Got%,Want%
	ENDIF
	
	ONERR TidyUpAndReturn::
	TotalBufferLen&=0
	_Buffer&=ALLOC(KMaxDBResultsBufferSize&*KOplStringSizeFactor%)
	IF _Buffer&=0
		InfoDialog:("","Failed to allocate sufficient","memory for query results")
		GOTO TidyUpAndReturn::
	ENDIF
	rem Set the buffer to zero length initially
	POKEL _Buffer&,TotalBufferLen&

	IF Output%=2
		LOPEN "C:\DemoOPL Query (Unicode).txt"
	ELSEIF Output%=3
		SETFLAGS KAlwaysWriteAsciiTextFiles&
		LOPEN "C:\DemoOPL Query (ASCII).txt"
	ENDIF
	USE A
	WHILE NOT EOF
		Temp$=A.ItemName$
		_AddItemToMultiDisplayBuffer:(_Buffer&,ADDR(TotalBufferLen&),Temp$,KTrue%)
		_AddItemToFile:(Output%,Temp$)
		Temp$=A.ArtistName$
		_AddItemToMultiDisplayBuffer:(_Buffer&,ADDR(TotalBufferLen&),Temp$,KTrue%)
		_AddItemToFile:(Output%,Temp$)
		Temp$=FIX$(A.Price,2,9)
		_AddItemToMultiDisplayBuffer:(_Buffer&,ADDR(TotalBufferLen&),Temp$,KTrue%)
		_AddItemToFile:(Output%,Temp$)
		IF A.Got%=KTrue% :Temp$="Got" :ELSE :Temp$="Not got" :ENDIF
		_AddItemToMultiDisplayBuffer:(_Buffer&,ADDR(TotalBufferLen&),Temp$,KTrue%)
		_AddItemToFile:(Output%,Temp$)
		IF A.Want%=KTrue% :Temp$="Wanted" :ELSE :Temp$="Not wanted" :ENDIF
		_AddItemToMultiDisplayBuffer:(_Buffer&,ADDR(TotalBufferLen&),Temp$,KTrue%)
		_AddItemToFile:(Output%,Temp$)
		_AddItemToMultiDisplayBuffer:(_Buffer&,ADDR(TotalBufferLen&),CHR$(KLineBreak&),KFalse%)
		_AddItemToFile:(Output%,"")
		NEXT
	ENDWH

	rem Check if the buffer length is zero. If it is
	rem then no records matched the query so we'll
	rem add something to the buffer to indicate that.
	IF TotalBufferLen&=0
		_AddItemToMultiDisplayBuffer:(_Buffer&,ADDR(TotalBufferLen&),"No results to match that query",KFalse%)
		_AddItemToFile:(Output%,"")
	ENDIF
	
	rem Now showing the results in a dEDITMULTI. First
	rem of all we get the thread of this application so
	rem we can send a keypress to the dialog later on...
	OurThread&=SyThreadIdFromAppUid&:(KApplicationUid&,Previous&)
	Previous&=0
	dINIT "Query results"
	rem The new (optional) KTrue% at the end below
	rem means this editor will be read-only...
	dEDITMULTI _Buffer&,"Results",15,6,TotalBufferLen&,KTrue%
	dBUTTONS "Cancel",KdBUTTONEsc%
	rem Simulate the Ctrl+Fn+Left Arrow keypress to
	rem remove the highlight on the text and position
	rem the cursor in the top-left of the edit box
	SySendKeyEventToApp&:(OurThread&,Previous&,KKeyLeftArrow32&,KKModControl% OR KKModFn%,0,0)
	LOCK ON :Dia%=DIALOG :LOCK OFF
	
	TidyUpAndReturn::

	ONERR OFF
	FREEALLOC _Buffer&
	TRAP LCLOSE
	IF Output%=3
		CLEARFLAGS KAlwaysWriteAsciiTextFiles&
	ENDIF
	USE A :CLOSE
ENDP

rem
rem Write out aString$ to file, provided aOutput% is not
rem set to the "None" option above.
rem
PROC _AddItemToFile:(aOutput%,aString$)
	IF (aOutput%=2) OR (aOutput%=3)
		LPRINT aString$
	ENDIF
ENDP

rem
rem This function uses some advanced OPL functions
rem to manipulate memory and strings.
rem
rem WARNING:	Only advanced OPL programmers should
rem						be concerned with this function! It
rem						demonstrates the use of PEEK and POKE
rem						commands to manipulate memory.
rem
PROC _AddItemToMultiDisplayBuffer:(_aBuffer&,_aTotalBufferLen&,aString$,aNewLine%)
LOCAL CurrentOffset&,CurrentBufferLen&
LOCAL String$(KMaxStringLen%),Len%,i%
	String$=aString$
	Len%=LEN(String$)
	i%=1+KOplAlignment% rem Skip length and indeterminate bytes
	CurrentBufferLen&=PEEKL(_aTotalBufferLen&)
	CurrentOffset&=CurrentBufferLen&*KOplStringSizeFactor%
	DO
		POKEB _aBuffer&+KLongIntWidth&+CurrentOffset&,PEEKB(ADDR(String$)+i%)
		CurrentOffset&=CurrentOffset&+1
		i%=i%+1
	UNTIL i%=(Len%*KOplStringSizeFactor%)+1+KOplAlignment%
	CurrentBufferLen&=CurrentBufferLen&+Len%
	CurrentOffset&=CurrentBufferLen&*KOplStringSizeFactor%

	POKEL _aTotalBufferLen&,CurrentBufferLen&
	POKEL _aBuffer&,PEEKL(_aTotalBufferLen&)

	IF aNewLine%
		_AddItemToMultiDisplayBuffer:(_aBuffer&,_aTotalBufferLen&,CHR$(KLineBreak&),KFalse%)
	ENDIF
ENDP

rem
rem 'Wrapper' procedure to write a new Artist record
rem to our database table
rem
PROC _AddArtistToDB:(aArtist$)
	INSERT
	A.ArtistName$=aArtist$
	PUT
ENDP

rem
rem 'Wrapper' procedure to write a new Item record
rem to our database table
rem
PROC _AddItemToDB:(aItem$,aArtist$,aPrice,aGot%,aWant%)
	INSERT
	A.ItemName$=aItem$
	A.ArtistName$=aArtist$
	A.Price=aPrice
	A.Got%=aGot%
	A.Want%=aWant%
	PUT
ENDP

rem
rem This procedure demonstrates the use of LOADM
rem to load other pre-compiled OPL modules. It can
rem also be used to demonstrate some advanced features
rem in SYSTEM.OPX involving threads.
rem
PROC RunOPXTest:
EXTERNAL Dia%
LOCAL WithRunApp%,File$(KdFILENameLen%),Thread&
LOCAL ThreadStatus&,EventStatus%,Event&(16)
	WithRunApp%=KNoYesChoiceYes%
	File$=KDefaultOPXTestPath$
	dINIT "Run OPX test code"
	dFILE File$,"Name,Folder,Disk",(KdFILESelectorWithROM% OR KdFILESelectorWithSystem%),KUidDirectFileStore&,KUidOpo&,KUidOplInterpreter&
	dCHOICE WithRunApp%,"Run rather than LOADM","No,Yes"
	dBUTTONS "Close",KdBUTTONEnter%
	LOCK ON :Dia%=DIALOG :LOCK OFF
	IF Dia%<>KdBUTTONEnter%
		RETURN
	ENDIF
	
	ONERR TrappedError::
	IF WithRunApp%=KNoYesChoiceYes%
		Thread&=SyRunApp&:("OPL","","R"+File$,KSyRunAppRun%)
		BUSY "Waiting for OPX test to finish",KBusyBottomRight%
		LOCK ON
		SyLogonToThread:(Thread&,ThreadStatus&)
		WHILE ThreadStatus&=KStatusPending32&
			GETEVENTA32 EventStatus%,Event&()
			IOWAIT
		ENDWH
		LOCK OFF
		BUSY OFF
		rem Bring ourselves to the foreground just in case
		rem the user has switched to another app
		SySetForeground:
	ELSE
		LOADM File$
		BeginOPXTests:
		UNLOADM File$
	ENDIF
	ONERR OFF
	RETURN
	
	TrappedError::
	ONERR OFF
	TRAP UNLOADM File$
	InfoDialog:("E","","")
ENDP

rem
rem This procedure demonstrates the use of the SendAs
rem OPX to send a user defined string across the
rem chosen bearer type.
rem
PROC HandleSendAs:(aKey&)
EXTERNAL Dia%,SaCascs$()
LOCAL String$(KMaxStringLen%),_Body&,AddAttachment%
LOCAL File$(KdFILENameLen%),Recipient$(2,KMaxStringLen%)
	ONERR Cleanup::
	String$="Test send from "+KApplicationName$+" using the SendAs OPX!"
	dINIT "SendAs demonstration"
	dTEXT "Method type","Send "+SaCascs$(aKey&)
	dTEXT "Method UID","0x"+HEX$(SaUID&:(aKey&))
	IF SaCapabilitySupported%:(aKey&,KCapabilityAttachments&)
		dCHOICE AddAttachment%,"Add file attachment","No,Yes"
	ENDIF
	dEDIT String$,"Text to send",10
	dEDIT Recipient$(1),"Recipient 1",10
	dEDIT Recipient$(2),"Recipient 2",10
	dBUTTONS "Close",KdBUTTONEnter%
	LOCK ON :Dia%=DIALOG :LOCK OFF
	IF Dia%<>KdBUTTONEnter%
		Gi:("Nothing was sent",KBusyTopRight%,0)
		RETURN
	ELSEIF AddAttachment%=KNoYesChoiceYes%
		File$="C:\"
		dINIT "Select file to attach"
		dFILE File$,"Name,Folder,Disk",0
		dBUTTONS "Close",KdBUTTONEnter%
		LOCK ON :Dia%=DIALOG :LOCK OFF
		IF Dia%<>KdBUTTONEnter%
			Gi:("Nothing was sent",KBusyTopRight%,0)
			RETURN
		ENDIF
	ENDIF

	BUSY "Creating a SendAs message"+CHR$(KEllipsis&),KBusyBottomRight%
	_Body&=SaNewBody&:
	SaPrepareMessage:(aKey&)
	SaAppendToBody:(DATIM$+CHR$(KLineBreak&)) rem Just for reference
	SaAppendToBody:(String$)
	SaSetBody:(_Body&)
	IF AddAttachment%=KNoYesChoiceYes%
		rem Can add multiple attachments with repeated calls to
		rem SaAddFile:() with different file names
		SaAddFile:(File$)
	ENDIF
	
	rem Add recipients - can add multiple recipients by simply
	rem repeating calls to SaAddRecipient:() with new strings
	SaAddRecipient:(Recipient$(1))
	SaAddRecipient:(Recipient$(2))
	SaLaunchSend:
	SaDeleteBody:
	BUSY OFF
	ONERR OFF
	RETURN

	Cleanup::
	ONERR OFF
	BUSY OFF
	InfoDialog:("E","","")
	IF _Body&
		SaDeleteBody:
	ENDIF
ENDP

rem
rem This procedure uses the AppFrame OPX to
rem launch the System Log application. A menu
rem command to do this features in most
rem of the standard applications.
rem
PROC LaunchLog:
	AfLaunchSystemLog:
ENDP

rem
rem This procedure simply initialises all our
rem settings/preference variables to default values.
rem
PROC InitialiseDefaultSettings:
EXTERNAL AfOn%,AfStatusType%,AfStatusOn%,AfTitleOn%
EXTERNAL View%,MainScreenTextColor%,AppHotKey%
EXTERNAL SampleINI%,SampleINI&,SampleINI,SampleINI$
EXTERNAL SampleINIDate&,SampleINITime&,SampleINIPw$
	AfOn%=KTrue%
	AfStatusType%=KAfStatusPaneTypeWide%
	AfStatusOn%=KTrue%
	AfTitleOn%=KTrue%
	View%=KViewMainScreen%
	MainScreenTextColor%=KColorSettingBlack%	
	AppHotKey%=KDefaultAppHotKey%
	
	SampleINI%=44
	SampleINI&=44
	SampleINI=44.44
	SampleINI$="Forty Four"
	SampleINIDate&=DAYS(09,10,1980)
	SampleINITime&=(INT(HOUR)*60+MINUTE)*60+SECOND
	SampleINIPw$="Password"
ENDP

rem
rem Procedure to restore settings/preferences
rem from our INI file
rem
PROC ReadINI:
EXTERNAL AfOn%,AfStatusType%,AfStatusOn%,AfTitleOn%
EXTERNAL View%,MainScreenTextColor%,AppHotKey%
EXTERNAL SampleINI%,SampleINI&,SampleINI,SampleINI$
EXTERNAL SampleINIDate&,SampleINITime&,SampleINIPw$
LOCAL h%,UID$(16),AppVerInFile$(KMaxVersionStringLen%)
LOCAL AppBuildInFile$(KMaxBuildStringLen%)
	IF IOOPEN(h%,KIniFile$,0)
		rem Show error: 1 for read mode, KTrue% to stop program
		CannotAccessFile:(KIniFile$,1,KTrue%)
	ENDIF
	IOREAD(h%,ADDR(UID$)+1+KOplAlignment%,16*KOplStringSizeFactor%)
	POKEB ADDR(UID$),16
	IF UID$<>SyUIDCheckSum$:(0,0,KApplicationUID&)
		InfoDialog:("","Header of INI file is incorrect. A new","file will be created using default values.")
		IOCLOSE(h%)
		WriteINI:(KTrue%)
		RETURN
	ENDIF
	rem Although we read the version info stored in our
	rem INI file, we don't act upon it in this
	rem application - but one could, for example, use
	rem this when upgrading from one version of
	rem an application to another.
	IOReadString:(h%,ADDR(AppVerInFile$))
	IOReadString:(h%,ADDR(AppBuildInFile$))
	IOREAD(h%,ADDR(AfOn%),KShortIntWidth&)
	IOREAD(h%,ADDR(AfStatusType%),KShortIntWidth&)
	IOREAD(h%,ADDR(AfStatusOn%),KShortIntWidth&)
	IOREAD(h%,ADDR(AfTitleOn%),KShortIntWidth&)
	IOREAD(h%,ADDR(View%),KShortIntWidth&)
	IOREAD(h%,ADDR(MainScreenTextColor%),KShortIntWidth&)
	
	IOREAD(h%,ADDR(SampleINI%),KShortIntWidth&)
	IOREAD(h%,ADDR(SampleINI&),KLongIntWidth&)
	IOREAD(h%,ADDR(SampleINI),KFloatWidth&)
	IOReadString:(h%,ADDR(SampleINI$))
	IOREAD(h%,ADDR(SampleINIDate&),KLongIntWidth&)
	IOREAD(h%,ADDR(SampleINITime&),KLongIntWidth&)
	IOReadString:(h%,ADDR(SampleINIPw$))

	IOREAD(h%,ADDR(AppHotKey%),KShortIntWidth&)
	IOCLOSE(h%)
ENDP

rem
rem Procedure to save settings/preferences
rem to our INI file
rem
PROC WriteINI:(aDefault%)
EXTERNAL AfOn%,AfStatusType%,AfStatusOn%,AfTitleOn%
EXTERNAL View%,MainScreenTextColor%,AppHotKey%
EXTERNAL SampleINI%,SampleINI&,SampleINI,SampleINI$
EXTERNAL SampleINIDate&,SampleINITime&,SampleINIPw$
LOCAL h%,UID$(16),AppVer$(KMaxVersionStringLen%)
LOCAL AppBuild$(KMaxBuildStringLen%)
	AppVer$=KApplicationVersion$
	AppBuild$=KApplicationBuild$
	UID$=SyUIDCheckSum$:(0,0,KApplicationUID&)
	IF aDefault%
		InitialiseDefaultSettings:
	ENDIF
	IF IOOPEN(h%,KIniFile$,$0102)
		rem Show error: 0 for write mode, KTrue% to stop program
		CannotAccessFile:(KIniFile$,0,KTrue%)
	ENDIF
	IOWRITE(h%,ADDR(UID$)+1+KOplAlignment%,SIZE(UID$))
	IOWRITE(h%,ADDR(AppVer$),SIZE(AppVer$)+1+KOplAlignment%)
	IOWRITE(h%,ADDR(AppBuild$),SIZE(AppBuild$)+1+KOplAlignment%)
	IOWRITE(h%,ADDR(AfOn%),KShortIntWidth&)
	IOWRITE(h%,ADDR(AfStatusType%),KShortIntWidth&)
	IOWRITE(h%,ADDR(AfStatusOn%),KShortIntWidth&)
	IOWRITE(h%,ADDR(AfTitleOn%),KShortIntWidth&)
	IOWRITE(h%,ADDR(View%),KShortIntWidth&)
	IOWRITE(h%,ADDR(MainScreenTextColor%),KShortIntWidth&)

	IOWRITE(h%,ADDR(SampleINI%),KShortIntWidth&)
	IOWRITE(h%,ADDR(SampleINI&),KLongIntWidth&)
	IOWRITE(h%,ADDR(SampleINI),KFloatWidth&)
	IOWRITE(h%,ADDR(SampleINI$),SIZE(SampleINI$)+1+KOplAlignment%)
	IOWRITE(h%,ADDR(SampleINIDate&),KLongIntWidth&)
	IOWRITE(h%,ADDR(SampleINITime&),KLongIntWidth&)
	IOWRITE(h%,ADDR(SampleINIPw$),SIZE(SampleINIPw$)+1+KOplAlignment%)

	IOWRITE(h%,ADDR(AppHotKey%),KShortIntWidth&)
	IOCLOSE(h%)
ENDP

PROC SetupSendAsKeysAndMenu:
EXTERNAL SaCascs$(),SaKeys&(),SaMaxTypes&,SaNextFreeKey&
LOCAL i%
	rem Scan for SendAs types we can use
	SaScanSendAsTypes:
	
	rem Begin to process items for the Send menu cascade
	SaMaxTypes&=SaMaximumTypes&:
	SaNextFreeKey&=SaNextAvailableHotkey&:
	rem If no MTMs are found, set the next hotkey
	rem to zero so no 'Send' menu cascade appears
	IF SaNextFreeKey&=KSendAsHotkeyStart&
		SaNextFreekey&=0
	ENDIF
	i%=1
	WHILE i%<=SaMaxTypes&
		IF SaCapabilitySupported%:(i%,KCapabilityBodyText&)
			SaCascs$(i%)=SaCascName$:(i%)
			SaKeys&(i%)=SaHotkeyValue&:(i%)
		ELSE
			SaCascs$(i%)=""
			SaKeys&(i%)=0
		ENDIF
		i%=i%+1
	ENDWH
ENDP

rem *******************************************
rem * The procedures which follow are generally
rem * re-usable utility functions
rem *******************************************

rem
rem 'Wrapper' function to easily IOREAD a string
rem from our INI file.
rem
PROC IOReadString:(aFileHandle%,_aAddr&)
LOCAL Len%
	IOREAD(aFileHandle%,ADDR(Len%),KShortIntWidth&)
	POKEB _aAddr&,Len%
	IOREAD(aFileHandle%,_aAddr&+1+KOplAlignment%,Len%*KOplStringSizeFactor%)
ENDP

rem
rem Generic dialog to alert user to problem
rem reading or writing to a file
rem
PROC CannotAccessFile:(aFile$,aMode%,aStopAfterError%)
LOCAL Mode$(6)
	IF aMode%=0
		Mode$="write "
	ELSE
		Mode$="read "
	ENDIF
	IF aStopAfterError%
		InfoDialog:("","Cannot "+Mode$+aFile$,KApplicationName$+" must now close.")
		STOP
	ELSE
		InfoDialog:("","Cannot "+Mode$+aFile$,"")
	ENDIF
ENDP

rem
rem Procedure to set the pen colour using RGB values
rem
PROC SetColor:(aColourMapping%)
LOCAL RGB&,Red%,Green%,Blue%
	RGB&=KRgbBlack& rem Black is the default
	IF aColourMapping%=KColorSettingBlack%
		RGB&=KRgbBlack&
	ELSEIF aColourMapping%=KColorSettingDarkGrey%
		RGB&=KRgbDarkGray&
	ELSEIF aColourMapping%=KColorSettingDarkRed%
		RGB&=KRgbDarkRed&
	ELSEIF aColourMapping%=KColorSettingDarkGreen%
		RGB&=KRgbDarkGreen&
	ELSEIF aColourMapping%=KColorSettingDarkYellow%
		RGB&=KRgbDarkYellow&
	ELSEIF aColourMapping%=KColorSettingDarkBlue%
		RGB&=KRgbDarkBlue&
	ELSEIF aColourMapping%=KColorSettingDarkMagenta%
		RGB&=KRgbDarkMagenta&
	ELSEIF aColourMapping%=KColorSettingDarkCyan%
		RGB&=KRgbDarkCyan&
	ELSEIF aColourMapping%=KColorSettingRed%
		RGB&=KRgbRed&
	ELSEIF aColourMapping%=KColorSettingGreen%
		RGB&=KRgbGreen&
	ELSEIF aColourMapping%=KColorSettingYellow%
		RGB&=KRgbYellow&
	ELSEIF aColourMapping%=KColorSettingBlue%
		RGB&=KRgbBlue&
	ELSEIF aColourMapping%=KColorSettingMagenta%
		RGB&=KRgbMagenta&
	ELSEIF aColourMapping%=KColorSettingCyan%
		RGB&=KRgbCyan&
	ELSEIF aColourMapping%=KColorSettingGrey%
		RGB&=KRgbGray&
	ELSEIF aColourMapping%=KColorSettingLightGrey%
		RGB&=KRgbDitheredLightGray&
	ELSEIF aColourMapping%=KColorSettingLighterGrey%
		RGB&=KRgb1in4DitheredGray&
	ELSEIF aColourMapping%=KColorSettingWhite%
		RGB&=KRgbWhite&
	ENDIF
	
	Red%=RGB&/KRgbRedPosition& AND KRgbColorMask&
	Green%=RGB&/KRgbGreenPosition& AND KRgbColorMask&
	Blue%=RGB& AND KRgbColorMask&

	gCOLOR Red%,Green%,Blue%
ENDP

rem
rem Generic dialog to display prompts/information. If
rem aTitle$ is just "E" then we display an OPL-style
rem error dialog with the appropriate error diagnostics.
rem
rem For all other situations we make use of the Application
rem Framework standard information-style dialog box
rem
PROC InfoDialog:(aTitle$,aLine1$,aLine2$)
LOCAL Title$(255),Text$(255)
EXTERNAL Dia%
	BUSY OFF rem Just in case
	IF aTitle$="E"
		dINIT "Error"
		dTEXT "",ERRX$,KDTextCenter%
		dTEXT "",ERR$(ERR),KDTextCenter%
		dBUTTONS "OK",KdBUTTONEnter%
		LOCK ON :Dia%=DIALOG :LOCK OFF
		RETURN
	ELSEIF aTitle$=""
		Title$="Note"
	ELSE
		Title$=aTitle$
	ENDIF
	Text$=aLine1$
	IF aLine2$<>""
		Text$=Text$+CHR$(KLineFeed&)+aLine2$
	ENDIF
	AfInformationDialog%:(Title$,Text$)
ENDP

rem
rem A simple dialog to serve as an 'About' box
rem
PROC AboutBox:
EXTERNAL Dia%
	dINIT "About "+KApplicationName$
	dTEXT "",KAboutBoxDescription$,KDTextCenter%
rem dTEXT "","Copyright © "+KCopyrightDate$+" Symbian Ltd.",KDTextCenter%
rem	dTEXT "","All rights reserved.",KDTextCenter%
	dTEXT "",KApplicationName$+" program - Version "+PrintableVersionNo$:,KDTextCenter%
	dTEXT "","For more information, see",KDTextCenter%
	dTEXT "","http://opl-dev.sourceforge.net",KDTextCenter%
	dBUTTONS "Close",KdBUTTONEnter%
	LOCK ON :Dia%=DIALOG :LOCK OFF
ENDP

rem
rem Return a more user friendly version number for
rem this application
rem
PROC PrintableVersionNo$:
LOCAL Build$(KMaxBuildStringLen%)
	Build$=KApplicationBuild$
	IF Build$<"1000"
		Build$=RIGHT$(KApplicationBuild$,3)
	ENDIF
	RETURN LEFT$(KApplicationVersion$,4)+"("+Build$+")"
ENDP

rem
rem Infoprint with optional pause
rem
PROC Gi:(aMsg$,aPos%,aPause&)
	GIPRINT aMsg$,aPos%
	IF aPause&<>0 rem Disallow indefinite pauses
		PAUSE aPause&
	ENDIF
ENDP
