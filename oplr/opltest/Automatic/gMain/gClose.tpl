REM gClose.tpl
REM EPOC OPL automatic test code for gCLOSE.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gClose", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gClose:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogClose")
	rem hCleanUp%:("CleanUp")
endp


proc dogClose:
	local idW2%
	local chrW%  rem character width in console
	local info%(10)

	screeninfo info%()
	chrW%=info%(7)
	
rem	screen gWidth/chrW%,4
	idW2%=gCreate(0,45,gWidth,gHeight-45,1)
	gBox gWidth,gHeight
	gAt 0,15
	rem gPrint "Window 2 created"
	rem pause -30 :key
	gClose idW2%
	if gIdentity<>1 :raise 1 :endif
	rem print "Window 2 closed ok"
	rem pause -30 :key
	rem print "Attempting to close default window"
	rem pause -20
	onerr e
	gClose 1
	onerr off
	raise 2
e::
	onerr off
	if err<>-2 :raise 3 : cls :print err,err$(err) :get :raise 3 :endif
	rem print "Failed to close OK"
	rem print "Done"
	rem pause -30 :key
endp

REM End of gClose.tpl

