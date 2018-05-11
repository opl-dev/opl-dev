REM giprint.tpl
REM EPOC OPL automatic test code for GIPRINT.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "giprint", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc giprint:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogiprint")
rem	hCleanUp%:("CleanUp")
rem	KLog%:(KhLogHigh%,"Some sample text")
endp


proc dogIPrint:
	rem print "Info message for 1 second"
	rem print "  or until keypress..."
	gIPrint "Info message"
	rem pause -20 :key
	gIPrint ""
	gIPrint "topLeft",0
	rem pause -20 :key
	gIPrint "bottomLeft",1
	rem pause -20 :key
	gIPrint "topRight",2
	rem pause -20 :key
	gIPrint "bottomRight",3
	rem pause -20 :key

	rem print "Testing bad arguments..."
	onerr e1
	gIPrint "Invalid corner 4",4
	onerr off
	raise 1
e1::
	onerr e2
	gIPrint "Invalid corner -1",-1
	onerr off
	raise 2
e2::

	rem print "Finished ok"
	rem pause -50 :key
endp

REM End of giprint.tpl
