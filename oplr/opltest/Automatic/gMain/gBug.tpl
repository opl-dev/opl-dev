REM gBug.tpl
REM EPOC OPL automatic test code for graphics bug-fixes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gbug", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gBug:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogbug")
rem	hCleanUp%:("CleanUp")
endp


proc dogbug:
	local w1%,w2%,orgH%
 	local chrW%  rem character width in console
 	local info%(10)

 	screeninfo info%()
 	chrW%=info%(7)

	orgH%=gHeight
rem	screen gWidth/chrW%-2,3,2,2
	gBox gWidth,60
	w1%=makew%:(0,gWidth/2,orgH%)
	w2%=makew%:(gWidth,gWidth,orgH%)
	gUse w1%
	rem print "Pattern in",w1%
	gPatt -1,gWidth,gHeight,0
	rem pause 30
	gUse w2%
	rem print "Pattern in",w2%
	gPatt -1,gWidth,gHeight,0
	rem pause 30
	rem print "Finished"
	rem pause 20
	gclose w1%
	gclose w2%
	gcls
endp


proc makew%:(x%,width%,height%)
	local w%

	w%=gcreate(x%+1,70,width%-2,height%-70,1)
	gFont 10
	gstyle 1
	gBorder 0
	gAt 10, gheight/2
	gPrint "Window",w%
	gAt 0,0
	return w%
endp

REM End of gbug.tpl
