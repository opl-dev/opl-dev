REM gPatt.tpl
REM EPOC OPL automatic test code for gpatt.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gpatt", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gpatt:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogpatt")
rem	hCleanUp%:("CleanUp")
endp


proc dogPatt:
	local idBit1%,wBit1%,hBit1%
	gAt 1,12
 	gPatt -1,gWidth/2-10,gHeight,0
	at 1, 1
	rem print "grey worked"
	idBit1%=gCreateBit(20,20)
	gCls
	gFill 16,16,0
	gUse 1
	gAt gWidth/2-6,12
	gPatt idBit1%,gWidth/2+4,gHeight,0
	rem at gWidth/12,1
	rem print "bitmap worked";
	gclose idbit1%
	gcls
endp

REM End of gpatt.tpl
