REM gvisible.tpl
REM EPOC OPL automatic test code for gVisible.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gvisible", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gvisible:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogvisible")
endp


proc dogVisible:
	local id%
	id%=gcreate(0,0,400,200,1,1)
	gAt 3,2
	gBox gWidth-3,gHeight-30
	gAt 10,gHeight-30 :gPrint "Window 1"
	gvisible off
	gvisible on
	gclose id%
endp


REM End of gvisible.tpl

