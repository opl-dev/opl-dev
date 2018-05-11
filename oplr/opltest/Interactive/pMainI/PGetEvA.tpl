REM pGetEvA.tpl
REM EPOC OPL test code for Opler1 events.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

rem DECLARE EXTERNAL

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNAL Clearerr:
EXTERNAL tkeypress:
EXTERNAL tpointer:
EXTERNAL tpointerpos:
EXTERNAL ttimest:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pGetEvA", hThreadIdFromOplDoc&:, KhUserLoggingOnly%)
	REM After standalone completion, control returns here.
	GIPRINT "Tests complete" :PAUSE 10
	while 1 : dINIT "Tests complete" :DIALOG : endwh
ENDP


proc pGetEvA:
	global filter%,mask%
	global ev&(16)
	global info%(10)

	screeninfo info%()
	mask%=$7
	filter%=0

	print "Opler1 GetEvent32 Tests"
	print
	print "Some automated tests: please follow instructions!"

	rem hInitTestHarness:(KhInitTargetHandlesErrors%, KhInitNotUsed%)
	
	hCall%:("runPGetEvA")
	rem hCleanUp%:("Reset")

	print
	print "Opler1 GetEvent32 Tests Finished OK"
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
endp


proc reset:
	EXTERNAL info%()
	local ev&(16)
	while testevent<>0
		getevent32 ev&()	
	endwh
	clearerr:
	screen info%(3),info%(4)
	cls
	cursor off
	font KFontCourierNormal11&,0	
	rem defaultwin 1
	gcolor 0,0,0
endp


proc clearerr:
	rem to clear err
	onerr clear
	raise 0
	onerr off
clear::
	rem OK now
endp


PROC runPGetEvA:
	REM No point in attempting to skip to new test on error
	REM so run these as one large test under the harness.
	tkeypress:
	tpointer:
	tpointerpos:
	ttimest:
ENDP


proc tkeypress:
	external ev&()
	print
	print "Test key events"
	print "Press a key to clear previous events(Esc to skip this test)"
	if get=27		rem this clears all previous events
		return
	endif
	getevent32 ev&()		rem this clears "key up" from "get"
	if ev&(1)<>KEvKeyUp& : raise 1 : endif
	print
	print "Please press 'a' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 2 : endif
	getevent32 ev&()
	checkkeypress:(100,&61,&41,&0,&0)
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 7 : endif

	print "Please press 'h' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 9 : endif
	getevent32 ev&()
	checkkeypress:(110,&68,&48,&0,&0)
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 15 : endif

	print "Please press Shift + 'w' once"
	getevent32 ev&()		rem shift down
	if ev&(1)<>KEvKeyDown& : raise 16 : endif
	getevent32 ev&()		rem key down
	if ev&(1)<>KEvKeyDown& : raise 17 : endif
	getevent32 ev&()		rem keypress
	checkkeypress:(120,&57,&57,&2,&0)
	getevent32 ev&()		rem key up
	if ev&(1)<>KEvKeyUp& : raise 23 : endif
	getevent32 ev&()		rem shift up
	if ev&(1)<>KEvKeyUp& : raise 24 : endif

	print "Please press '6' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 25 : endif
	getevent32 ev&()
	checkkeypress:(130,&36,&36,&0,&0)
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 31 : endif

	print "Please press '!' once"
	getevent32 ev&()		rem shift down
	if ev&(1)<>KEvKeyDown& : raise 32 : endif
	getevent32 ev&()		rem key down
	if ev&(1)<>KEvKeyDown& : raise 33 : endif
	getevent32 ev&()		rem keypress
	checkkeypress:(140,&21,&31,&2,&0)
	getevent32 ev&()		rem key up
	if ev&(1)<>KEvKeyUp& : raise 39 : endif
	getevent32 ev&()		rem shift up
	if ev&(1)<>KEvKeyUp& : raise 40 : endif

	print "Please press Ctrl + 'k' once"
	getevent32 ev&()		rem ctrl down
	if ev&(1)<>KEvKeyDown& : raise 41 : endif
	getevent32 ev&()		rem key down
	if ev&(1)<>KEvKeyDown& : raise 42 : endif
	getevent32 ev&()		rem keypress
	checkkeypress:(150,&B,&4B,&4,&0)
	getevent32 ev&()		rem key up
	if ev&(1)<>KEvKeyUp& : raise 48 : endif
	getevent32 ev&()		rem ctrl up
	if ev&(1)<>KEvKeyUp& : raise 49 : endif
	
	print "Please press Alt/Fn once"
	getevent32 ev&()		rem alt down
	if ev&(1)<>KEvKeyDown& : raise 50 : endif
	if ev&(4)<>&20 : raise 51 : endif
	getevent32 ev&()		rem alt up
	if ev&(1)<>KEvKeyUp& : raise 57 : endif
	if ev&(4)<>&0 : raise 58 : endif	
	
	print "Please press 'Esc' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 59 : endif
	getevent32 ev&()
	checkkeypress:(160,&1b,&4,&0,&0)
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 65 : endif

	print "Please press 'Enter' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 66 : endif
	getevent32 ev&()
	checkkeypress:(170,&d,&3,&0,&0)
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 72 : endif
	
	print "Please press 'Tab' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 73 : endif
	getevent32 ev&()
	checkkeypress:(180,&9,&2,&0,&0)
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 79 : endif

	print "Please press 'Delete' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 80 : endif
	getevent32 ev&()
	checkkeypress:(190,&8,&1,&0,&0)
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 86 : endif
	
	print "Please press 'Shift' once"
	getevent32 ev&()
	if ev&(1)<>KEvKeyDown& : raise 87 : endif
	getevent32 ev&()
	if ev&(1)<>KEvKeyUp& : raise 88 : endif
	
	print "OK"
	rem pause 20
endp

proc tpointer:
	local id%,wid&,i%
	cls
	print "Test pointer events"
	print "Press a key to clear previous events (Esc to skip test)"
	if get=27		rem this clears all previous events
		return
	endif
	getevent32 ev&()		rem this clears "key up" from "get"
	if ev&(1)<>KEvKeyUp& : raise 201 : endif	
	print
	print "For the following tests, move events will be filtered out so "
	print "tests will work on Opler1"
	print "(Ensure Caps lock and Num lock are switched off)"
	pointerfilter 2,7
	print
	print "Please click the left mouse button/pen"
	getevent32 ev&()
	if ev&(1)=&409     rem allow pointer enter for Opler1
		getevent32 ev&()
	endif
	checkpointerevent:(202,&1,&0,&0)
	getevent32 ev&()
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		getevent32 ev&()
		i%=i%+1
		if i%>5 : raise 6 : endif
	endwh
	checkpointerevent:(207,&1,&1,&0)
	
	rem pause 10 
	print "Please drag the pen/mouse"
	getevent32 ev&()
	if ev&(1)=&409     rem allow pointer enter for Opler1
		getevent32 ev&()
	endif
	checkpointerevent:(210,&1,&0,&0)
	getevent32 ev&()
	while ev&(4)=6
		checkpointerevent:(214,&1,&6,&0)
		getevent32 ev&()
	endwh
	checkpointerevent:(218,&1,&1,&0)
	
	id%=gcreate (320,0,320,gheight,1,1)
	gborder 2
	pointerfilter 2,7
	wid&=id%
	pause 10
	print "Please click the left mouse button"
	print "/pen in the new window"
	getevent32 ev&()
	if ev&(1)=&40a
		checkpointerexit:(222,&1)
		getevent32 ev&()
	endif	
	checkpointerenter:(224,wid&)
	getevent32 ev&()
	checkpointerevent:(226,wid&,&0,&0)
	getevent32 ev&()
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		getevent32 ev&()
		i%=i%+1
		if i%>5 : raise 6 : endif
	endwh	
	checkpointerevent:(230,wid&,&1,&0)

	pause 10
	print "Please drag the mouse/pen"
	print "back to the main window"
	getevent32 ev&()
	if ev&(1)=&409     rem allow pointer enter for Opler1
		getevent32 ev&()
	endif
	checkpointerevent:(234,wid&,&0,&0)
	getevent32 ev&()
	while ev&(4)=6
		checkpointerevent:(238,wid&,&6,&0)
		getevent32 ev&()
	endwh
	checkpointerexit:(242,wid&)
	getevent32 ev&()
	checkpointerenter:(244,&1)
	getevent32 ev&()
	while ev&(4)=6
		checkpointerevent:(246,&1,&6,&0)
		getevent32 ev&()
	endwh
	checkpointerevent:(250,&1,&1,&0)
	gclose id%
	
	pause 10
	print "Please double click"
	getevent32 ev&()
	if ev&(1)=&409     rem allow pointer enter for Opler1
		getevent32 ev&()
	endif
	checkpointerevent:(254,&1,&0,&0)
	getevent32 ev&()
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		getevent32 ev&()
		i%=i%+1
		if i%>5 : raise 6 : endif
	endwh	
	checkpointerevent:(258,&1,&1,&0)
	getevent32 ev&()
	if ev&(1)=&409     rem allow pointer enter for Opler1
		getevent32 ev&()
	endif
	checkpointerevent:(262,&1,&0,&80000)
	getevent32 ev&()
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		getevent32 ev&()
		i%=i%+1
		if i%>5 : raise 6 : endif
	endwh	
	checkpointerevent:(266,&1,&1,&0)	
	
	pause 10
	print
	print "Please click on system and then return to OPL"
	getevent32 ev&()
	while ev&(1)<>$402
		getevent32 ev&()
	endwh
	if ev&(1)<>&402 : raise 276 : endif
	getevent32 ev&()
	if ev&(1)<>&401 : raise 282 : endif
		
	print "OK"
	rem pause 20
endp


proc tpointerpos:
	local i%
	local t1&,t2&
	cls
	pointerfilter 2,7
	print "Test pointer positions"
	print "Press a key to clear previous events(Esc to skip test)"
	if get=27		rem this clears all previous events
		return
	endif
	getevent32 ev&()		rem this clears "key up" from "get"
	if ev&(1)<>KEvKeyUp& : raise 1 : endif	
	print
	gat 50,50
	gcircle 2
	print "Please click in the circle - top right"
	getevent32 ev&()
	checkpointerenter:(302,&1)
	getevent32 ev&()
	checkpointerevent:(304,&1,&0,&0)
	checkpos:(&32,&32,&2)
	getevent32 ev&()
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		getevent32 ev&()
		i%=i%+1
		if i%>5 : raise 6 : endif
	endwh
	checkpointerevent:(310,&1,&1,&0)
	checkpos:(&32,&32,&2)

	gat 500,200
	gcircle 2
	print "Please click in the circle - bottom left"
	getevent32 ev&()
	checkpointerenter:(315,&1)
	getevent32 ev&()
	checkpointerevent:(316,&1,&0,&0)
	checkpos:(&1f4,&c8,&2)
	getevent32 ev&()
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		getevent32 ev&()
		i%=i%+1
		if i%>5 : raise 6 : endif
	endwh
	checkpointerevent:(321,&1,&1,&0)
	checkpos:(&1f4,&c8,&2)

	REM Moved this from TTimest:
	print "Please hold down mouse button/pen for approximately 3 secs"	
	pointerfilter 7,7
	getevent32 ev&()    rem gets pen down
	if ev&(1)<>$408 : raise 415 : endif
	if ev&(4)<>0 : raise 416 : endif
	t1&=ev&(2)
	getevent32 ev&()    rem gets pen up
	if ev&(1)<>$408 : raise 417 : endif
	if ev&(4)<>1 : raise 1000+ev&(4) : raise 418 : endif
	t2&=ev&(2)
	t2&=(t2&-t1&)/1000000
	print t2&;" secs apart"
	if t2&<3 : print "Too quick!" : endif
	if t2&>3 : print "Too slow!" : endif
	
	print "OK"
	rem pause 20
endp


proc ttimest:
	local t1&,t2&
	cls
	print "Test time stamps"
	print "Press a key to clear previous events (Esc to skip test)"
	if get=27
		return
	endif
	getevent32 ev&()
	if ev&(1)<>$407 : raise 400 : endif
	print	
	print "Please press two keys, approximately 2 secs apart"	
	getevent32 ev&()    rem gets key down
	if ev&(1)<>$406 : raise 401 : endif
	getevent32 ev&()    rem gets key press
	if (ev&(1) and $400)<>0 : print hex$(ev&(1)) : raise 402 : endif
	t1&=ev&(2)
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 403 : endif
	getevent32 ev&()    rem gets key down
	if ev&(1)<>$406 : raise 404 : endif
	getevent32 ev&()    rem gets key press
	if (ev&(1) and $400)<>0 : print hex$(ev&(1)) : raise 405 : endif
	t2&=ev&(2)
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 406 : endif
	t2&=(t2&-t1&)/1000000
	print t2&;" secs apart"
	if t2&<2 : print "Too quick!" : endif
	if t2&>2 : print "Too slow!" : endif
	
	print "Please press two keys, approximately 5 secs apart"	
	getevent32 ev&()    rem gets key down
	if ev&(1)<>$406 : raise 408 : endif
	getevent32 ev&()    rem gets key press
	if (ev&(1) and $400)<>0 : print hex$(ev&(1)) : raise 409 : endif
	t1&=ev&(2)
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 410 : endif
	getevent32 ev&()    rem gets key down
	if ev&(1)<>$406 : raise 411 : endif
	getevent32 ev&()    rem gets key press
	if (ev&(1) and $400)<>0 : print hex$(ev&(1)) : raise 412 : endif
	t2&=ev&(2)
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 413 : endif
	t2&=(t2&-t1&)/1000000
	print t2&;" secs apart"
	if t2&<5 : print "Too quick!" : endif
	if t2&>5 : print "Too slow!" : endif

	print "OK"
	rem pause 20
endp

proc checkkeypress:(r%,key&,sc&,mod&,rep&)
	if (ev&(1) AND $400)<>0 
		print "Incorrect event ";hex$(ev&(1))
		raise r%
	endif
	if ev&(1)<>key&
		print "Incorrect key ";hex$(ev&(1)) ;"(Expecting ";hex$(key&);")"
		raise r%+1
	endif
	if ev&(3)<>sc&
		print "Incorrect scan code ";hex$(ev&(3))
		raise r%+2
	endif
	if ev&(4)<>mod&
		print "Incorrect modifier ";hex$(ev&(4));"(Expecting ";hex$(mod&);")"
		raise r%+3
	endif
	if ev&(5)<>rep&
		print "Incorrect repeats ";hex$(ev&(5))
		raise r%+4
	endif
endp


proc checkpointerevent:(r%,wid&,type&,mod&)
	if (ev&(1) AND $fff)<>$408
		print "Incorrect event ";hex$(ev&(1))
		raise r%
	endif
	if ev&(3)<>wid&
		print "Incorrect window id ";hex$(ev&(3))
		raise r%+1
	endif
	if ev&(4)<>type&
		print "Incorrect type ";hex$(ev&(4))
		raise r%+2
	endif
	if ev&(5)<>mod&
		print "Incorrect modifier ";hex$(ev&(5));" (expecting ";hex$(mod&);")"
		raise r%+3
	endif
endp

proc checkpointerenter:(r%,wid&)
	if (ev&(1) AND $fff)<>$409 : print "Incorrect event ";hex$(ev&(1)) : raise r% : endif
	if ev&(3)<>wid& : print "Incorrect window id ";hex$(ev&(3)) : raise r%+1 : endif	
endp

proc checkpointerexit:(r%,wid&)
	if (ev&(1) AND $fff)<>$40A : print "Incorrect event ";hex$(ev&(1)) : raise r% : endif
	if ev&(3)<>wid& : print "Incorrect window id ";hex$(ev&(3)) : raise r%+1 : endif	
endp

proc checkpos:(x&,y&,r&)
	local dis&
	dis&=(ev&(6)-x&)*(ev&(6)-x&)+(ev&(7)-y&)*(ev&(7)-y&)
	if dis&>=r&*r&
		print "Outside the circle!" : print "Square root of ";dis&;" from centre!"
	else
		print "Inside the circle - OK"
	endif
endp

rem End of pGetEvA.tpl
