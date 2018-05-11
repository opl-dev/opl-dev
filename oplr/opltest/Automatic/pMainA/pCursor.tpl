REM pCursor.tpl
REM EPOC OPL automatic test code for cursor.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL
INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNAL	ttextcur:
EXTERNAL	tgraphcur:(mode%)


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pCursor", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pCursor:
	global info%(10)
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tcursor")
	rem hCleanUp%:("Reset")
endp


proc tCursor:
	external info%()
	screeninfo info%()
	rem print "Opler1 Cursor Tests"
	rem print
	ttextcur:
	tgraphcur:(1)
	tgraphcur:(2)
	rem print
	rem print "Opler1 Cursor Tests Finished OK"
	rem pause 30
endp


proc ttextcur:
	external info%()

	local x%,y%
	
	rem print "Testing text cursor"
	rem print "Initially cursor is not displayed"
	rem pause 20
	cursor on
	print "Cursor now ON at the end of this text"
	rem pause 20
	print "Cursor should be at approx centre of screen"
	x%=info%(3)/2  : y%=info%(4)/2
	at x%,y%
	rem pause 30
	cls
endp


proc tgraphcur:(mode%)
	external info%()

	local id%
	local chrw%,chrh%
	local m$(20)
	
	chrw%=info%(7)
	chrh%=info%(8)
	screen gwidth/chrw%,30/chrh%,1,1
	
	if mode%=1 : m$=" 4-colour " : endif
	if mode%=2 : m$=" 16-colour " : endif
	print "Testing graphics cursor types in"+m$+"window"
 	id%=gcreate(0,30,gwidth,gheight-30,1,mode%)
	gborder 1
	gfont 10 
	gat 10,20
	cursor id%,2,11,2
	rem pause -30
	gprint "The cursor should move to the end of this text"
	rem pause -30
	gat 10,40
	gprint "The cursor should remain unchanged"
	cursor id%,2,11,2,1
	rem pause -30
	gat 10,60
	gprint "The cursor should stop flashing"
	cursor id%,2,11,2,2
	rem pause -30
	gat 10,80
	gprint "The cursor should start flashing"
	cursor id%,2,11,2,1
	rem pause -30
	gat 10,100
	gprint "The cursor should be grey"
	cursor id%,2,11,2,4
	rem pause -30
	gat 10,120
	gprint "Large cursor for colour checking"
	cursor id%,20,20,20,4
	rem pause -60	
	gcls
	gclose id%
endp

REM End of pCursor.tpl

