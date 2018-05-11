REM gupdate.tpl
REM EPOC OPL automatic test code for gupdate.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gupdate", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gupdate:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogupdate")
rem	hCleanUp%:("CleanUp")
endp


proc dogUpdate:
 local i%

	print "gUpdate test"
	print "Doing gUpdate OFF"
	gUpdate off
	print "This should have taken long!!!"
rem	pause 100
	print "gUpdate"
	gUpdate
	gUpdate on
	print "gUpdate ON"
	gcls
endp

REM End of gupdate.tpl

