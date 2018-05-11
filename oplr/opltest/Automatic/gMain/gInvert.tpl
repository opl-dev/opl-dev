REM gInvert.tpl
REM EPOC OPL automatic test code for gInvert.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gInvert", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gInvert:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogInvert")
	rem hCleanUp%:("CleanUp")
endp


proc doginvert:
	local a$(30),x%,y%

	rem print "gInvert test"
	rem print "============"
	rem print "Invert all"
	gInvert gWidth,gHeight
	rem pause -30 :key
	rem print "and again..."
	gInvert gWidth,gHeight
	rem pause -30 :key
	cls
	gFont 11
	a$="Invert block around this text"
	x%=(gWidth-gTWidth(a$))/2 :y%=(gHeight+5)/2
	gAt x%,y%
	gPrint a$
	gAt x%,y%-13
	gInvert gTWidth(a$),17
	rem pause -30 :key
	cls
	rem print "Invert largest possible"
	gAt 0,0
	gInvert 32767,32767
	rem pause -30 :key
	rem print "Finished gInvert test"
	rem pause -30 :key
	gcls
endp

REM End of gInvert.tpl
