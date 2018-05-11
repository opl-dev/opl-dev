REM ggmode.tpl
REM EPOC OPL automatic test code for gGMODE keyword.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "ggmode", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc ggmode:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("doggmode")
	rem hCleanUp%:("CleanUp")
	rem KLog%:(KhLogHigh%,"Some sample text")
endp


proc dogGMode:
	local gMode%,ind%
	local val%(8),bits%,ix%

	val%(1)=255
	val%(2)=255
	val%(3)=255
	val%(4)=255
	val%(5)=0
	val%(6)=0
	val%(7)=255
	val%(8)=0

	gAt 0,15
	gLineBy 8,0

	ix%=ix%+1
	pkCheck%:(val%(ix%),ix%)

	gPrint "GMode=default"

	gTMode 2				:rem invert
	ind%=gWidth/2
	gAt ind%,0
	gFill ind%,gHeight,0
	gAt ind%,15
	gLineBy 8,0

	ix%=ix%+1
	pkCheck%:(val%(ix%),ix%)

	gPrint "GMode=default"

	gMode%=0
	while gMode%<=2
		gGMode gMode%
		gAt 0,gY+15
		gLineBy 8,0
		ix%=ix%+1
		pkCheck%:(val%(ix%),ix%)
		gPrint "GMode=";gMode%
		gAt ind%,gY
		gLineBy 8,0

		ix%=ix%+1
		pkCheck%:(val%(ix%),ix%)

		gPrint "GMode=";gMode%
		gMode%=gMode%+1
	endwh

	rem at 12,9 :print "Done";
	rem 	pause -50 :key
	gCLS
endp


proc pkCheck%:(val%,ix%)
		local bits%
		gPeekLine gIdentity,gx-8,gY,#addr(bits%),8
		if val%<>(bits% and $ff)
				raise 1
				at 1,8
				print "Peeked",bits%,"and expect",val%
				print "val() index=";ix%
				get
		endif
endp

REM End of ggmode.tpl
