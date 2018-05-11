REM wDlgerr.tpl
REM EPOC OPL interactive test code for dialog error handling.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wDLgerr", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wdlgerr:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowdlgerr")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


proc dowDlgErr:
	local v%,raise%

	onerr carryOn1::
	dialog
	onerr off
	rem print "BUG: Dialog run before initialized or setup"
	RAISE 1

carryOn1::
	onerr off
	onerr carryOn2::
	
	dText "1","2"
	onerr off
	rem print "BUG: Dialog setup before initialized"
	RAISE 2

carryOn2::
	onerr off
	
	raise%=3
	onerr badNews::

	PRINT :PRINT "Hit Enter."
	dInit
	dialog
	onerr off
	rem print "Dialog initialized and run without setup : ALLOWED"
	return

BadNews::
	onerr off
	raise raise%
endp


REM End of wDlgerr.tpl

