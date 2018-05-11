REMxxxx.tpl
REMEPOC OPL interactive test code for xxxx
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE"hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "xxxx", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC xxxx:
	global xyz
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("myFirstProc")
	hCall%:("mySecondProc")
	hCleanUp%:("Reset")
ENDP

PROC Reset:
	rem Any clean-up code here.
ENDP

