REM tAppFrame.tpl
REM Copyright (c) 1999-2001 Symbian Ltd. All rights reserved.
REM v0.7

APP tAppFrame,065617
ENDA

INCLUDE "Const.oph"
INCLUDE "AppFrame.oxh"

DECLARE EXTERNAL

EXTERNAL UseMain:
EXTERNAL hCBAMain:(aButton&)
EXTERNAL Welcome:
EXTERNAL UseAll:
EXTERNAL hCBAAll:(aButton&)
EXTERNAL UseCBA:
EXTERNAL hCBACBA:(aButton&)
EXTERNAL UseTitle:
EXTERNAL hCBATitle:(aButton&)
EXTERNAL hCloseButton:(aButton&)
EXTERNAL Redraw%:
EXTERNAL Init:
EXTERNAL UseVisible:
EXTERNAL hCBAVisible:(aButton&)
EXTERNAL UseDimmed:
EXTERNAL hCBADimmed:(aButton&)
EXTERNAL UseDefault:
EXTERNAL hCBADefault:(aButton&)
EXTERNAL UseStatus:
EXTERNAL hCBAStatus:(aButton&)



PROC tAppFrame:
	GLOBAL done%
	GLOBAL redraw%
	GLOBAL title$(255)
	LOCAL ev&(16)

	Welcome:
	Init:
	UseMain:
	IF AfCBAMaxButtons%:<>4
		GIPRINT "Unexpected number of buttons"
	ENDIF

	DO
		GETEVENT32 ev&()
		IF AfOfferEvent%:(ev&(1),ev&(3),ev&(4),ev&(5),ev&(6),ev&(7))
			GIPRINT "Event consumed"
		ENDIF
		IF redraw%
			redraw%=0
			Redraw%:
		ENDIF
	UNTIL done%
ENDP




PROC UseMain:
	REM Main CBA menu.
	AfSetCBAButton:(1,"Test all",0,0,"hCBAMain")
	AfSetCBAButton:(2,"Test CBA",0,0,"hCBAMain")
	AfSetCBAButton:(3,"Test title",0,0,"hCBAMain")
	AfSetCBAButton:(4,"Close",0,0,"hCloseButton")
ENDP


PROC hCBAMain:(aButton&)
	IF aButton&=1
		UseALL:
	ELSEIF aButton&=2
		UseCBA:
	ELSEIF aButton&=3
		UseTitle:
	ELSE
		REM Unhandled button!
		GIPRINT "hCBAMain: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP


PROC hCloseButton:(aButton&)
	EXTERNAL done%
	IF aButton&=4
		done%=KTrue%
	ELSE
		GIPRINT "hCloseButton: Unhandled button "+GEN$(aButton&,2)
	ENDIF
ENDP


PROC Init:
	EXTERNAL title$
	AfSetStatus%:(KAfStatusPaneTypeWide%)
	title$="tAppFrame - ApplicationFrame OPX test code"
	AfSetTitle:(title$)
	AfSetStatusVisible%:(KTrue%)
	AfSetTitleVisible%:(KTrue%)
	AfSetCBAVisible%:(KTrue%)
	Redraw%:
ENDP


PROC UseAll:
	REM All CBA menu.
	Init:
	AfSetCBAButton:(1,"CBA off",0,0,"hCBAAll")
	AfSetCBAButton:(2,"Test"+CHR$(10)+"status",0,0,"hCBAAll")
	AfSetCBAButton:(3,"Title off",0,0,"hCBAAll")
	AfSetCBAButton:(4,"Close",0,0,"hCBAAll")
ENDP


PROC hCBAAll:(aButton&)
	EXTERNAL done%
	EXTERNAL redraw%

	IF aButton&=1 					REM Toggle CBA
		IF AfCBAVisible%:
			redraw%=AfSetCBAVisible%:(KFalse%)
			AfSetCBAButton:(1,"CBA on",0,0,"hCBAAll")
		ELSE
			rem Change text before showing CBA.
			AfSetCBAButton:(1,"CBA off",0,0,"hCBAAll")
			redraw%=AfSetCBAVisible%:(KTrue%)
		ENDIF

	ELSEIF aButton&=2				REM Test status state...
		UseStatus:

	ELSEIF aButton&=3				REM Toggle title.
		IF AfTitleVisible%:
			redraw%=AfSetTitleVisible%:(KFalse%)
			AfSetCBAButton:(3,"Title on",0,0,"hCBAAll")
		ELSE
			redraw%=AfSetTitleVisible%:(KTrue%)
			AfSetCBAButton:(3,"Title off",0,0,"hCBAAll")
		ENDIF

	ELSEIF aButton&=4:				REM Back
		UseMain:
	ELSE
		REM Unhandled button!
		GIPRINT "hCBAAll: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP



PROC UseTitle:
	REM Title CBA menu.
	AfSetCBAButton:(1,"Edit title",0,0,"hCBATitle")
	AfSetCBAButton:(2,"Grow title",0,0,"hCBATitle")
	AfSetCBAButton:(3,"Toggle title",0,0,"hCBATitle")
	AfSetCBAButton:(4,"Close",0,0,"hCBATitle")
ENDP


PROC hCBATitle:(aButton&)
	EXTERNAL redraw%
	EXTERNAL title$
	IF aButton&=1
		dINIT
		dEDIT title$,"Title",20
		dBUTTONS "Ok",13
		IF DIALOG
			AfSetTitle:(title$)
		ENDIF
	ELSEIF aButton&=2
		title$=title$+CHR$(%a+RND*26)
		AfSetTitle:(title$)
	ELSEIF aButton&=3
		IF AfTitleVisible%:
			redraw%=AfSetTitleVisible%:(KFalse%)
		ELSE
			redraw%=AfSetTitleVisible%:(KTrue%)
		ENDIF
	ELSEIF aButton&=4
		UseMain:
	ELSE
		REM Unhandled button!
		GIPRINT "hCBAMain: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP


PROC UseCBA:
	REM CBA-specific CBA menu.
	AfSetCBAButton:(1,"Test visible",0,0,"hCBACBA")
	AfSetCBAButton:(2,"Test default",0,0,"hCBACBA")
	AfSetCBAButton:(3,"Test dimmed",0,0,"hCBACBA")
	AfSetCBAButton:(4,"Close",0,0,"hCBACBA")
ENDP


PROC hCBACBA:(aButton&)
	IF aButton&=1
		UseVisible:
	ELSEIF aButton&=2
		UseDefault:
	ELSEIF aButton&=3
		UseDimmed:
	ELSEIF aButton&=4
		UseMain:
	ELSE
		GIPRINT "hCBACBA: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP


PROC UseVisible:
	REM Test Visible settings of CBA.
	AfSetCBAButton:(1,"Toggle CBA1"+CHR$(10)+"visibility",0,0,"hCBAVisible")
	AfSetCBAButton:(2,"Toggle CBA2"+CHR$(10)+"visibility",0,0,"hCBAVisible")
	AfSetCBAButton:(3,"Toggle CBA3"+CHR$(10)+"visibility",0,0,"hCBAVisible")
	AfSetCBAButton:(4,"Close",0,0,"hCBAVisible")
ENDP


PROC hCBAVisible:(aButton&)
	LOCAL button&
	IF aButton&>0 AND aButton&<4
		AfSetCBAButtonVisible:(aButton&,NOT AfCBAButtonVisible%:(aButton&))
	ELSEIF aButton&=4
		GIPRINT "Setting all buttons visible"
		DO
			button&=button&+1
			AfSetCBAButtonVisible:(button&,KTrue%)
		UNTIL button&=AfCBAMaxButtons%:
		UseCBA: REM Back to CBA tests.
	ELSE
		GIPRINT "hCBAVisible: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP


PROC UseDimmed:
	REM Test dimmed settings of CBA.
	AfSetCBAButton:(1,"Toggle CBA1"+CHR$(10)+"dimmed",0,0,"hCBADimmed")
	AfSetCBAButton:(2,"Toggle CBA2"+CHR$(10)+"dimmed",0,0,"hCBADimmed")
	AfSetCBAButton:(3,"Toggle CBA3"+CHR$(10)+"dimmed",0,0,"hCBADimmed")
	AfSetCBAButton:(4,"Close",0,0,"hCBADimmed")
ENDP


PROC hCBADimmed:(aButton&)
	LOCAL button&
	IF aButton&>0 AND aButton&<4
		AfSetCBAButtonDimmed:(aButton&,NOT AfCBAButtonDimmed%:(aButton&))
	ELSEIF aButton&=4
		GIPRINT "Setting all buttons undimmed"
		DO
			button&=button&+1
			AfSetCBAButtonDimmed:(button&,KFalse%)
		UNTIL button&=AfCBAMaxButtons%:
		UseCBA: REM Back to CBA tests.
	ELSE
		GIPRINT "hCBADimmed: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP


PROC UseDefault:
	REM Test default settings of CBA.
	AfSetCBAButton:(1,"Set CBA1"+CHR$(10)+"default",0,0,"hCBADefault")
	AfSetCBAButton:(2,"Set CBA2"+CHR$(10)+"default",0,0,"hCBADefault")
	AfSetCBAButton:(3,"Set CBA3"+CHR$(10)+"default",0,0,"hCBADefault")
	AfSetCBAButton:(4,"Close",0,0,"hCBADefault")
ENDP


PROC hCBADefault:(aButton&)
	IF aButton&>0 AND aButton&<4
		AfSetCBAButtonDefault:(aButton&)
		GIPRINT "Setting button "+GEN$(aButton&,1)+" default.",2
		REM Keep this on screen for a second...
		PAUSE 20
	ELSEIF aButton&=4
		UseCBA: REM Back to CBA tests.
	ELSE
		GIPRINT "hCBADefault: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP


PROC Redraw%:
	LOCAL statusPaneState%
	LOCAL xo%,yo%,w%,h%

	AfScreenInfo:(xo%,yo%,w%,h%)
	rem PAUSE 50
	gSetWin xo%,yo%,w%,h%

	GIPRINT "Redraw..."
	gSTYLE KgStyleBold%
	gAT 100,20 :gPRINT "Current dimensions"
	gSTYLE KgStyleNormal%
	
	gAT 100, 60 :gPRINT "X origin"
	gAT 100, 90 :gPRINT "Y origin"
	gAT 100,120 :gPRINT "Width"
	gAT 100,150 :gPRINT "Height"


	gAT 200, 60 :gPRINTB ": "+GEN$(xo%,3),100
	gAT 200, 90 :gPRINTB ": "+GEN$(yo%,3),100
	gAT 200,120 :gPRINTB ": "+GEN$(w%,3),100
	gAT 200,150 :gPRINTB ": "+GEN$(h%,3),100
ENDP


PROC Welcome:
	dINIT "tAppFrame -- testing Button Group OPX"
	dTEXT "","No status pane, title bar or CBA at startup"
	dTEXT "","Hit ESC to continue"
	DIALOG
ENDP


PROC UseStatus:
	REM Status-specific CBA menu.
	AfSetCBAButton:(1,"Status"+CHR$(10)+"off",0,0,"hCBAStatus")
	AfSetCBAButton:(2,"Status"+CHR$(10)+"narrow",0,0,"hCBAStatus")
	AfSetCBAButton:(3,"",0,0,"hCBAStatus")
	AfSetCBAButton:(4,"Close",0,0,"hCBAStatus")
ENDP


PROC hCBAStatus:(aButton&)
	EXTERNAL redraw%
	LOCAL visible%, type%
	REM Get current state...
	visible%=AfStatusVisible%:(type%)

	IF aButton&=1				REM Toggle visibility.
		IF visible%
			visible%=KFalse%
			AfSetCBAButton:(1,"Status"+CHR$(10)+"on",0,0,"hCBAStatus")
		ELSE
			visible%=KTrue%
			AfSetCBAButton:(1,"Status"+CHR$(10)+"off",0,0,"hCBAStatus")
		ENDIF
		redraw%=AfSetStatusVisible%:(visible%)

	ELSEIF aButton&=2			REM Toggle width
		IF type%=KAfStatusPaneTypeNarrow%
			type%=KAfStatusPaneTypeWide%
			AfSetCBAButton:(2,"Status"+CHR$(10)+"narrow",0,0,"hCBAStatus")

		ELSE
			type%=KAfStatusPaneTypeNarrow%
			AfSetCBAButton:(2,"Status"+CHR$(10)+"wide",0,0,"hCBAStatus")
		ENDIF
		redraw%=AfSetStatus%:(type%)

		
	ELSEIF aButton&=3			REM Not used.

	ELSEIF aButton&=4:				REM Back
		UseAll:

	ELSE
		REM Unhandled button!
		GIPRINT "hCBAStatus: Unhandled button "+GEN$(aButton&,3)
	ENDIF
ENDP


rem const KAfStatusPaneTypeNarrow%=1
rem const KAfStatusPaneTypeWide%=2

rem x	AfOfferEvent%:(aEv1&,aEv3&,aEv4&,aEv5&,aEv6&,aEv7&) : 1
rem x	AfSetCBAButton:(aButtonIndex%,aText$,aBitmapId%,aMaskId%,aCallback$) : 2
rem .	AfSetCBAButtonDefault:(aButtonIndex%) : 3
rem .	AfSetCBAButtonDimmed:(aButtonIndex%,aVisibility%) : 4
rem .	AfCBAButtomDimmed%:(aButtonIndex%) : 5
rem .	AfSetCBAButtonVisible:(aButtonIndex%,aVisibility%) : 6
rem .	AfCBAButtonVisible%:(aButtonIndex%) : 7
rem x	AfSetCBAVisible%:(aVisibility%) : 8
rem x	AfCBAVisible%: : 9
rem x	AfCBAMaxButtons%: :10
rem x	AfSetStatus%:(aType%) : 11
rem x	AfSetStatusVisible%:(aVisibility%) : 12
rem x	AfStatusVisible%:(BYREF aType%) : 13
rem x	AfSetTitle:(aTitle$) : 14
rem x	AfSetTitleVisibile%:(aVisibility%) : 15
rem x	AfTitleVisible%: : 16
rem x	AfScreenInfo:(BYREF aXOrigin%, BYREF aYOrigin%, BYREF aWidth%, BYREF aHeight%) : 17

REM End of tAppFrame.tpl