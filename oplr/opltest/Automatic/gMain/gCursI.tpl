REM gCursi.tpl
REM EPOC OPL automatic test code for cursor.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gcursi", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gcursi:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogcursi")
rem	hCleanUp%:("CleanUp")
endp


proc dogCursI:
	t0:
	t1:
endp


proc t0:
	local id%
	cls
rem	print "Test cursor can be opened non-current window"
	id%=gCreate(100,0,gWidth-102,10,1)
	gBox gWidth,gHeight
	gAt gWidth/2-10,0
	gUse 1
	gAt 30,30
	cursor id%,0,20,10
	rem print "Cursor should be on in centre of boxed window"
	rem pauseKey:
	gClose id%
	gAt 0,0
endp


proc t1:
	local cursy%
	cls
	rem print "Test Cursor state remembered over dialog"
	cursor on
	dCurs:
	rem print "Console cursor should be on"
	rem pause -50
	rem if key :get :endif
	cursy%=gheight-1
	gAt 1,cursy%
	cursor 1,10,20,10
	rem print "20x10 Cursor should be on in window 1"
	rem print "at x=1,y=",cursy%
	rem pause -50
	rem if key :get :endif
	dCurs:
	rem print "20x10 Cursor should STILL be on in window 1"
	rem print "at x=1,y=",cursy%
	rem pauseKey:
	rem print "Creating and using new window"
	gCreate(gWidth-100,gHeight-12,100,10,1)	:rem check cursor restored to window 1 when in new window
	gBox gWidth,gHeight
	gAt 2,9 :gPrint "New window"
	rem print "20x10 Cursor should STILL be on in window 1"
	rem print "at x=1,y=",cursy%
	rem pauseKey:
	rem dCurs:
	rem print "20x10 Cursor should STILL be on in window 1"
	rem print "at x=1,y=",cursy%
	rem pauseKey:
	gClose gIdentity
	cursor off
endp


proc pauseKey:
	rem pause -50
	rem if key :get :endif
endp


proc dCurs:
	local ed$(10)

	rem dInit "Dialog with cursor"
	rem dEdit ed$,"dEdit"
	rem dialog
endp


REM End of gcursi.tpl

