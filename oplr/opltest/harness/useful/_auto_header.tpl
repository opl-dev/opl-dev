REM xxxx.tpl
REM EPOC OPL automatic test code for xxxx
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "xxxx", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc xxxx:
	global xyz
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("myFirstProc")
	hRunTest%:("mySecondProc")
	hCleanUp%:("CleanUp")
	KLog%:(KhLogHigh%,"Some sample text")
endp


