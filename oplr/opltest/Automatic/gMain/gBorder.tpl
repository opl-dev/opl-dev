REM gBorder.tpl
REM EPOC OPL automatic test code for gBorder.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gBorder", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gBorder:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogborder")
rem	hCleanUp%:("CleanUp")
endp


proc dogborder:
	gAt 20,20
	rem print "gAt 20,20"
	rem print "gborder 0,100,50"
	gborder 0,100,50
	rem pause pause% :key
	cls
	rem print "gborder 0,50,50"
	gborder 0,50,50
	rem pause pause% :key
	cls
	rem print "gborder 0,100,50"
	gborder 0,100,50
	rem pause pause% :key
	cls
	rem print "gborder 0,50,50"
	gborder 0,50,50
	rem pause pause% :key
	gAt 50,30
	cls
	rem print "gAt 50,30"
	rem print "gborder 1, 100,50"
	gborder 1, 100,50
	rem pause pause% :key
	cls
	rem print "gborder 1, 50,50"
	gborder 1, 50,50
	rem pause pause% :key
	cls
	rem print "gborder $104, 100,50"
	gborder $104, 100,50
	rem pause pause% :key
	cls
	rem print "gborder $204, 50,50"
	gborder $204, 50,50
	rem pause pause% :key
	gCLS
endp

REM End of gBorder.tpl
