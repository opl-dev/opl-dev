REM gStyle.tpl
REM EPOC OPL automatic test code for gStyle.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gStyle", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gStyle:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogstyle")
	rem hCleanUp%:("CleanUp")
endp


proc dogStyle:
	local style%,gap%

	gap%=40
	gAt 1,0
	while style%<=63
		gAt 1,gY+gap%
		gStyle style%
		if gY>gHeight-2
rem			pause -30 :key
 		  gScroll 0,-gap%,0,gap%,gWidth,gHeight-gap% :gAT gX,gY-gap%
		endif
		if style%=0
			gPrint	"Normal"
		else
			if style% and 1 :gPrint "Bold " :endif
			if style% and 2 :gPrint "Under " :endif
			if style% and 4 :gPrint "Inverse " :endif
			if style% and 8 :gPrint "Double " :endif
			if style% and 16 :gPrint "Mono " :endif
			if style% and 32 :gPrint "Italic" :endif
		endif

		style%=style%+1
	endwh
	rem pause -20 :key
	gCLS
endp

REM End of gStyle.tpl

