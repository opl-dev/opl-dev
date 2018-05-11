REM tColor.tpl
REM EPOC OPL automatic test code for ER5 color demo.
REM Copyright (c) 1998-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

REM Bug fix coming soon...
rem CONST gColorInfoANumGrays%=gColorInfoANumGreys%

DECLARE EXTERNAL

EXTERNAL Spots:
EXTERNAL ColorBg:
EXTERNAL TryDMode:(adw%,ar%)
EXTERNAL DefWinMode$:(amode%)
EXTERNAL DisplayMode$:(amode%)

REM const KUidOPLColorDemo&=&1000484F


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "tColor", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC tColor:
	GLOBAL currentDisplayMode&
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("gColorInfo")
	hRunTest%:("Color")
	hCleanUp%:("Reset")
ENDP


PROC Reset:
	EXTERNAL currentDisplayMode&
	DEFAULTWIN currentDisplayMode&
	gUSE 1
	gCOLORBACKGROUND 255,255,255
ENDP


PROC gColorInfo:
	EXTERNAL currentDisplayMode&

	LOCAL defaultDisplayMode&,nColor&,nGray&
	LOCAL gInfo&(48),cInfo&(7)

	REM Current display mode.
	gINFO32 gInfo&()
	currentDisplayMode&=gInfo&(30)

	REM Get default values.
	gColorInfo cinfo&()
	defaultDisplayMode&=cInfo&(gColorInfoADisplayMode%)
	nColor&=cInfo&(gColorInfoANumColors%)
	nGray&=cInfo&(gColorInfoANumGrays%)

	REM Sanity checks -- default display mode.
	IF defaultDisplayMode&<KDisplayModeGray2%
		PRINT defaultDisplayMode&
		RAISE 1
	ENDIF

	IF defaultDisplayMode&>KDisplayModeColor4K%
		PRINT defaultDisplayMode&
		RAISE 2
	ENDIF

	REM Number colors.
	IF nColor&>16777216 rem 16 meg.
		PRINT nColor&
		RAISE 3
	ENDIF

	REM Number grays.
	IF nGray&>256
		PRINT nGray&
		RAISE 4
	ENDIF

	REM Switch display mode.
	TryDMode:(KDefaultWin2GrayMode%, 5)
	TryDMode:(KDefaultWin4GrayMode%, 10)
	TryDMode:(KDefaultWin16GrayMode%, 20)

	IF defaultDisplayMode&>KDisplayModeColor16%
		TryDMode:(KDefaultWin256ColorMode%, 30)
	ENDIF

	REM Put it back to previous setting.
	DEFAULTWIN currentDisplayMode&
	gINFO32 gInfo&()
	IF gInfo&(30)<>currentDisplayMode&
		RAISE 40
	ENDIF
ENDP


PROC TryDMode:(aDefWin%, aRaise%)
	EXTERNAL cInfo&()
	LOCAL gInfo&(48)

	DEFAULTWIN aDefWin%
	PRINT :PRINT "Switched DEFAULTWIN to ";DefWinMode$:(aDefWin%)

	gINFO32 gInfo&()
	PRINT "Expecting ";DisplayMode$:(aDefWin%)
	IF gInfo&(30)<>aDefWin%
		PRINT "Received ";DisplayMode$:(gInfo&(30))
		RAISE aRaise%
	ENDIF
ENDP


PROC Color:
	LOCAL id%
	reset:
	print "The following tests are repeated twice,"
	print "first for the DEFAULTWINdow, then a gCREATEd window."
	id%=1
	do
		if id%=1
			print "Testing DEFAULTWINdow"
			gUSE 1
		else
			print "Testing gCREATEd window"
		endif
		print
		
		rem Put the others here...
		colorBg:
		Spots:

		if id%=1
			rem create window on right half of screen
			rem to compare the two results.
			gUSE 1
			gcolorbackground 255,255,255
			gCLS
			id%=gCREATE(gWidth/2+50,30, gWidth/2-100, gHeight-60, 1, KgCreate16GrayMode%)
			gxborder 2,1
		else
			gClose id%
			gUSE 1
			break
		endif
	until 0
ENDP


PROC Spots:
	LOCAL w%,h%,iter%
	w%=gWIDTH :h%=gHEIGHT
	rem	gsetwin rnd*600,rnd*200,w%,h%

	rem giprint "Hit Esc to cancel"
	do
		iter%=iter%+1
		gcolor rnd*256,rnd*256,rnd*256
		gat rnd*w%,rnd*h%
		gcircle rnd*50,1
	until iter%=400
ENDP


PROC colorBg:
	gcolorbackground $ff,$ff,$00
	gat 40,20
	gprintB "This should have a yellow background",310
	pause 10
endp


PROC DefWinMode$:(aMode%)
	VECTOR aMode%+1
	KDefaultWin2GrayMode,KDefaultWin4GrayMode,KDefaultWin16GrayMode,KDefaultWin256GrayMode,KDefaultWin16ColorMode,KDefaultWin256ColorMode
	ENDV
	REM Not found.
	RAISE 1000
	RETURN "Not found"
KDefaultWin2GrayMode::
	RETURN "2GrayMode"
KDefaultWin4GrayMode::
	RETURN "2GrayMode"
KDefaultWin16GrayMode::
	RETURN "16GrayMode"
KDefaultWin256GrayMode::
	RETURN "256GrayMode"
KDefaultWin16ColorMode::
	RETURN "16ColorMode"
KDefaultWin256ColorMode::
	RETURN "256ColorMode"
ENDP


PROC DisplayMode$:(aMode%)
	VECTOR aMode%+1
	KDisplayModeNone, KDisplayModeGray2, KDisplayModeGray4, KDisplayModeGray16, KDisplayModeGray256, KDisplayModeColor16, KDisplayModeColor256, KDisplayModeColor64K, KDisplayModeColor16M, KDisplayModeRGB, KDisplayModeColor4K
	ENDV
	REM Not found
	RAISE 1001
	RETURN "Not found"
KDisplayModeNone::
	RETURN "None"
KDisplayModeGray2::
	RETURN "2Gray"
KDisplayModeGray4::
	RETURN "4Gray"
KDisplayModeGray16::
	RETURN "16Gray"
KDisplayModeGray256::
	RETURN "256Gray"
KDisplayModeColor16::
	RETURN "16Color"
KDisplayModeColor256::
	RETURN "256Color"
KDisplayModeColor64K::
	RETURN "64KColor"
KDisplayModeColor16M::
	RETURN "16MColor"
KDisplayModeRGB::
	RETURN "RGBColor"
KDisplayModeColor4K::
	RETURN "4KColor"
ENDP


REM End of tColor.tpl
