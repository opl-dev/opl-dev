REM qFrame.tpl
REM Copyright (c) 1999-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL

INCLUDE "Const.oph"
INCLUDE "BgOpx.oxh"
INCLUDE "System.oxh"

EXTERNAL HandleEvent:

EXTERNAL Init:
EXTERNAL InitButtons:
EXTERNAL InitStatus:
EXTERNAL InitTitle:

EXTERNAL ResizeScreen:
EXTERNAL DrawBorder:

EXTERNAL OfferCommand%:(aKey&,aModifier&)
EXTERNAL ShowType$:(aType&)
EXTERNAL hCba:(aButton&)
EXTERNAL hCba2:(aButton&)

EXTERNAL CmdA%:	rem toggle auto screen resize.
EXTERNAL CmdB%:	rem toggle auto border.
EXTERNAL CmdC%:	rem toggle CBA.
EXTERNAL CmdD%:	rem Show display info.
EXTERNAL CmdE%:	rem Exit.

EXTERNAL CmdG%: rem Grow the title bar.

EXTERNAL CmdS%:	rem toggle status pane on/off.
EXTERNAL CmdR%:	rem Resize screen.
EXTERNAL CmdT%:	rem toggle title.
EXTERNAL CmdU%: rem Update the title text.
EXTERNAL CmdV%:
EXTERNAL CmdW%:	rem toggle status pane width.
EXTERNAL CmdX%: rem Clear screen.


PROC qFrame:
	Init:
	HandleEvent:
	dinit "Hit CAS-K now!" :dialog
ENDP


PROC HandleEvent:
	GLOBAL ev&(16)
	GLOBAL menuPos%
	GLOBAL exit%
	GLOBAL resizeRequired%	REM Flag true if a screen resize is needed.
	GLOBAL autoBorderOn%,autoResizeOn%
	GLOBAL titleText$(255)

	LOCAL evType&,offer%
	LOCAL command$(255)

	menuPos%=256 rem skip first card!
	resizeRequired%=KTrue% rem Force a resize at the start.
	autoBorderOn%=KTrue%
	autoResizeOn%=KTrue%

	DO
		IF resizeRequired%
			resizeRequired%=KFalse%
			ResizeScreen:
		ENDIF

		GetEvent32 ev&()
		evType&=ev&(KEvaType%)
		PRINT "event=";ShowType$:(evType&)
		IF evType&=KEvCommand&
			command$=GETCMD$
			IF LEFT$(command$,1)="X"
				CmdE%: rem exit.
			ENDIF
		ELSEIF evType&=KEvPtr& OR evType&=KEvPtrEnter& OR evType&=KEvPtrExit&
			GIPRINT "Ignoring pointer events"
			CONTINUE
		ELSEIF (evType& AND $400)=0
			REM Key
			offer%=BGOfferEvent%:(ev&(1),ev&(3),ev&(4),ev&(5),ev&(6),ev&(7))
			IF offer%
				GIPRINT "CBA event consumed."
				CONTINUE
			ENDIF
			IF ((ev&(KEvAKMod%) and 4)=0) AND (evType&<>KKeyMenu32&)
				CONTINUE
			ENDIF
			IF OfferCommand%:(ev&(KEvaType%),ev&(KEvaKMod%)) 
				CONTINUE
			ENDIF

		ELSEIF evType&=KEvKeyUp& rem ignore
		ELSEIF evType&=KEvKeyDown&
		ELSE
			PRINT ShowType$:(evType&) +" This puppy ain't handled."
		ENDIF
	UNTIL exit%
ENDP


PROC OfferCommand%:(aKey&,aModifier&)
	EXTERNAL ev&(),menuPos%
	EXTERNAL autoBorderOn%,autoResizeOn%

	LOCAL mod&, m%,hotKey%
	LOCAL isMenuKey%
	LOCAL CmdRoot$(4)
	LOCAL borderOn%,resizeOn%


	PRINT "OfferCommand",HEX$(aKey&),HEX$(aModifier&)

	rem k%=aKey&
	mod&=aModifier&
	IF aKey&=KKeyMenu32&
		isMenuKey%=KTrue%
		IF autoBorderOn%
			borderOn%=KMenuSymbolOn%
		ENDIF
		IF autoResizeOn%
			resizeOn%=KMenuSymbolOn%
		ENDIF

		mINIT
		mCard "Local card for local people","Local",%Z
		mCard "App","Exit",%e
		mCARD "Toggle", "Toggle CBA",%c, "Toggle status pane",%s, "Toggle status width",%w, "Toggle title",%t
		mCARD "Tools", "Resize window",%r, "Show display info",%d, "Update title",%u, "Grow title text",%g, "Cls",%x
		mCARD "Locks", "Lock border redraw",%b OR KMenuCheckBox% OR borderOn%, "Lock auto screen resize",%a OR KMenuCheckBox% OR resizeOn%
		mCARD "Junk", "Verify things",%v
		m%=MENU(MenuPos%)
		IF m%
			hotKey%=m%
			mod&=4 rem convert to accelerator
			IF hotKey%<=%Z
				mod&=mod& or 2	rem Shift
			ENDIF
		ELSE
			RETURN -1
		ENDIF
	ELSE	rem not menu key
		hotKey%=aKey&-1+%a rem Control+a/A -> 1
	ENDIF

	IF mod& and 4	rem Hotkey modif, so maybe accelerator
		cmdRoot$="cmd"
		IF mod& and 2
			cmdRoot$="cmds"
		ENDIF
		rem print "call ";cmdRoot$+chr$(hotK%),hotK%
		ONERR errNotCmd

rem		if ((mod% and 2)=0) and ((hotKey%=%c) or (hotKey%=%a))
rem			gOrder 1,255				rem Text window to back
rem		else
rem			gOrder TbWinId%,1		rem Toolbar to front
rem			gOrder 1,2					rem Text window behind toolbar
rem		ENDIF

		@%(cmdRoot$+chr$(hotKey%)):
		RETURN -1
	ENDIF

errNotCmd::
	ONERR OFF
	PRINT ERR
	IF isMenuKey% OR ERR<>-99
		GIPRINT "Bug: Proc "+cmdRoot$+chr$(hotKey%)+"%:,"+err$(err)
		IF err=-98	rem missing externals
			alert("ERRX$: "+ErrX$,err$(err))
		endif
		return -1
	endif
	return 0
ENDP


PROC ShowType$:(aType&)
rem const KEvNotKeyMask&=&400
	IF aType&=KEvFocusGained& :RETURN "FocusGained"
	ELSEIF aType&=KEvFocusLost& :RETURN "FocusLost"
	ELSEIF aType&=KEvSwitchOn& :RETURN "SwitchOn"
	ELSEIF aType&=KEvCommand& :RETURN "Command"
	ELSEIF aType&=KEvDateChanged& :RETURN "DateChanged"
	ELSEIF aType&=KEvKeyDown& :RETURN "KeyDown"
	ELSEIF aType&=KEvKeyUp& :RETURN "KeyUp"
	ELSEIF aType&=KEvPtr& :RETURN "Ptr"
	ELSEIF aType&=KEvPtrEnter& :RETURN "PtrEnter"
	ELSEIF aType&=KEvPtrExit& :RETURN "PtrExit"
	ELSE
		RETURN "Type "+HEX$(aType&)+" unknown."
	ENDIF
ENDP


PROC ResizeScreen:
	REM Following a change to the status pane, title bar or CBA,
	REM the screen size and postition should be changed.
	EXTERNAL autoBorderOn%
	LOCAL topLeftX%,topLeftY%, width%,height%
	LOCAL ScrWidth&,ScrHeight&,InputWidth&,InputHeight&,PWidth&,PHeight&

	
	giprint "ResizeScreen:"
	gUSE 1
	
	REM Calc new origin.
	topLeftX%=BgStatusWidth%:
	topLeftY%=BgTitleHeight%:

	SyDisplaySize:(ScrWidth&,ScrHeight&,InputWidth&,InputHeight&,PWidth&,PHeight&)
	width%=ScrWidth&-BgCBAWidth%:-topLeftX%
	height%=ScrHeight&-topLeftY%

	gSETWIN topLeftX%,topLeftY%, width%,height%

	IF autoBorderOn%
		DrawBorder:
	ENDIF
ENDP


PROC DrawBorder:
	gXBorder 2,$194
ENDP


PROC Init:
	PRINT "Starting initialization."
	InitButtons:
	InitStatus:
	InitTitle:
ENDP


PROC InitButtons:
	IF BgCBAButtonCount%:<>4
		ALERT("Expecting 4 buttons in CBA.")
	ENDIF
	BGSetCBAButton:(1,"One",0,0,"hCba")
	BGSetCBAButton:(2,"This is"+CHR$(10)+"Button2",0,0,"hCba")
	BGSetCBAButton:(3,"Three3",0,0,"hCba")
	BGSetCBAButton:(4,"Four",0,0,"hCba")
	BgShowCBA%:
ENDP


PROC InitStatus:
	BgSetStatus%:(KBgStatusPaneWide%)
	BgShowStatus%:
ENDP


PROC InitTitle:
	BgSetTitle:("qFrame")
	BgShowTitle%:
ENDP


PROC hCba:(aButton&)
	PRINT "CBA button",aButton&,"pressed."
	IF aButton&=1
		InitButtons:
	ELSEIF aButton&=2
		BGSetCBAButton:(3,"Press Me",0,0,"hCba2")
		BGSetCBAButton:(4,"Me too!",0,0,"hCba2")
	ENDIF
ENDP


PROC hCba2:(aButton&)
	PRINT "CBA button2",aButton&,"pressed."
ENDP


PROC HighlitePoint:(ax%,ay%)
	gAT 0,0
	gLINETO ax%,ay%
	gLINETO 0,gHEIGHT
ENDP


REM Menu/hotkey commands...

PROC CmdA%:
	REM Lock auto screen resize.
	EXTERNAL autoResizeOn%

	giprint "Lock auto screen resize"
	autoResizeOn%=NOT autoResizeOn%
ENDP


PROC CmdB%:
	REM Lock auto border redrawing.
	EXTERNAL autoBorderOn%
	giprint "Lock auto border redrawing"
	autoBorderOn%=NOT autoBorderOn%
ENDP


PROC CmdC%:
	rem ToggleCbaVisibility:
	EXTERNAL resizeRequired%
	IF BgCBAVisible%:
		resizeRequired%=BgHideCBA%:
		GIPRINT "Button group is now hidden."
	ELSE
		resizeRequired%=BgShowCBA%:
		GIPRINT "Button group is now visible."
	ENDIF
ENDP


PROC CmdD%:
	REM Show display information.
	LOCAL state%,width%
	CLS
	width%=gWIDTH
	PRINT "Width of screen=";width%
	PRINT "Width of CBA=";BgCBAWidth%:
	PRINT "Width of status=";BgStatusWidth%:
	PRINT "Height of title=";BgTitleHeight%:
	IF BgCBAVisible%:
		width%=width%-BgCBAWidth%:
	ENDIF
	IF BgStatusVisible%:(state%)
		width%=width%=BgStatusWidth%:
	ENDIF
	HighlitePoint:(width%,(gHEIGHT-BgTitleHeight%:)/2)
ENDP


PROC CmdE%:
	EXTERNAL exit%
	exit%=KTrue%
ENDP


PROC CmdG%:
	REM Grow the title bar.
	EXTERNAL titleText$
	LOCAL lTitleText$(255)
	
	lTitleText$=titleText$
	dinit "Edit the title text"
	dEDIT lTitleText$,"Text",20
	IF dialog=0
		RETURN
	ENDIF
	titleText$=lTitleText$
	giprint "Setting title text to "+lTitleText$
	BgSetTitle:(lTitleText$)
ENDP


PROC CmdR%:
	rem Resize screen.
	gSETWIN rnd*600,rnd*200
ENDP


PROC CmdS%:
	REM Toggle status pane on/off.
	EXTERNAL resizeRequired%
	LOCAL vis%,state%
	vis%=BgStatusVisible%:(state%)
	IF vis%
		resizeRequired%=BgHideStatus%:
	ELSE
		resizeRequired%=BgShowStatus%:
	ENDIF
ENDP


PROC CmdT%:
	EXTERNAL resizeRequired%
	REM toggle Title on/off.
	IF BgTitleVisible%:
		resizeRequired%=BgHideTitle%:
	ELSE
		resizeRequired%=BgShowTitle%:
	ENDIF
ENDP


PROC CmdU%:
	IF RND>.9
		BgSetTitle:(CMD$(1))
	ELSE
		BgSetTitle:(DATIM$)
	ENDIF
ENDP


PROC CmdV%:
	REM Test that this app works.
	GIPRINT "Things verified"
ENDP


PROC CmdW%:
	REM Toggle status pane width.
	EXTERNAL resizeRequired%
	LOCAL state%
	BgStatusVisible%:(state%)
	IF state%=KBgStatusPaneNarrow%
		resizeRequired%=BgSetStatus%:(KBgStatusPaneWide%)
	ELSE
		resizeRequired%=BgSetStatus%:(KBgStatusPaneNarrow%)
	ENDIF
ENDP


PROC CmdX%:
	CLS
ENDP

rem const KBgStatusPaneNarrow%=1
rem const KBgStatusPaneWide%=2

rem x	BgSetCBAButton:(aButtonIndex%,aText$,aBitmapId%,aMaskId%,aCallback$) : 1
rem	x	BgOfferEvent%:(aEv1&,aEv3&,aEv4&,aEv5&) : 2
rem	x	BgShowCBA: : 3
rem	x	BgHideCBA: : 4
rem	x	BgCBAVisible%: : 5
rem	x	BgCBAButtonCount%: :6
rem	x	BgCBAWidth%: :7
rem	x	BgSetStatus:(aState%) : 8
rem	x	BgShowStatus: : 9
rem	x	BgHideStatus: : 10
rem	x	BgStatusVisible%:(BYREF aState%) : 11
rem	x	BgStatusWidth%: : 12
rem	x	BgSetTitle:(aTitle$) : 13
rem	x	BgShowTitle: : 14
rem	x	BgHideTitle: : 15
rem	x	BgTitleVisible%: : 16
rem	x	BgTitleHeight%: : 17



REM End of qFrame.tpl
