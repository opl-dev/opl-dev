REM pGetEvAs.tpl
REM EPOC OPL interactive test code for GETEVENT async.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL

INCLUDE "hUtils.oph"

EXTERNAL runPGetEvAs:
EXTERNAL	tkeypress:
EXTERNAL	tpointer:
EXTERNAL	tpointerpos:
EXTERNAL	ttimest:
EXTERNAL waitkeypress:(raise%,a$,l1&,l2&,l3&)

EXTERNAL checkpointerevent:(r%,wid&,type&,mod&)

EXTERNAL checkpos:(l1&,l2&,l3&)
EXTERNAL checkpointerenter:(r%,wid&)
EXTERNAL checkpointerexit:(r%,wid&)



PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pGetEvAs", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


const FCANCEL%=4
const FRELATIVE%=1

proc pGetEvAs:
	global filter%,mask%,status%,timeStat%
	global ev&(16)

	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)

	mask%=$7
	filter%=0
		
	print "Opler1 GetEventA32 Tests"
	rem pause 10
	print
	print "Some automated tests: please follow instructions!"
	hCall%:("runPGetEvAs")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
	print
	print "Opler1 GetEvent32 Tests Finished OK"
	rem pause 20
endp


proc runPGetEvAs:
	tkeypress:
	tpointer:
	tpointerpos:
	ttimest:
endp	


proc tkeypress:
	external ev&(),status%
	local a%
	print
	print "Test key events"
	print "Press a key to clear previous events (Esc to skip test)"
	if get=27		rem this clears all previous events
		return
	endif
	getevent32 ev&()		rem this clears "key up" from "get"
	if ev&(1)<>&407 : raise 1 : endif
	print
	waitkeypress:(100,"a",&61,&41,&0)
	waitkeypress:(110,"h",&68,&48,&0)
	waitkeypress:(120,"Shift + w",&57,&57,&2)
	waitkeypress:(130,"6",&36,&36,&0)
	waitkeypress:(140,"!",&21,&31,&2)
	waitkeypress:(150,"Ctrl + k",&B,&4B,&4)
	waitkeypress:(160,"Esc",&1b,&4,&0)
	waitkeypress:(170,"Enter",&d,&3,&0)
	waitkeypress:(180,"Tab",&9,&2,&0)
	waitkeypress:(190,"Delete",&8,&1,&0)
	
	print "Please press Alt/Fn once"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)<>&406 : raise 9 : endif
	if ev&(4)<>&20 : raise 10 : endif
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)<>&407 : raise 11 : endif
	if ev&(4)<>&0 : raise 12 : endif

	print "Please press Shift once"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)<>&406 : raise 13 : endif
	if ev&(4)<>&2 : print ev&(4) : raise 14 : endif
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)<>&407 : raise 15 : endif
	if ev&(4)<>&0 : raise 16 : endif

	print "OK"
	rem pause 20
endp


proc tpointer:
	external ev&(),status%
	local id%,wid&,i%
	cls
	print "Test pointer events"
	print "Press a key to clear previous events (Esc to skip test)"
	if get=27		rem this clears all previous events
		return
	endif
	getevent32 ev&()		rem this clears "key up" from "get"
	if ev&(1)<>&407 : raise 1 : endif	
	print
	print "For the following tests, move events will be filtered out so "
	print "tests will work on Opler1"
	pointerfilter 2,7
	print
	
	print "Please click the left mouse button/pen"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)=&409
		geteventa32 status%,ev&()
		iowaitstat status%
	endif
	checkpointerevent:(2,&1,&0,&0)
	geteventa32 status%,ev&()
	iowaitstat status%
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		geteventa32 status%,ev&()
		iowaitstat status%
		i%=i%+1
		if i%>5 : raise 100 : endif
	endwh
	checkpointerevent:(6,&1,&1,&0)
	
	rem pause 10
	print "Please drag the pen/mouse"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)=&409
		geteventa32 status%,ev&()
		iowaitstat status%
	endif
	checkpointerevent:(10,&1,&0,&0)
	geteventa32 status%,ev&()
	iowaitstat status%
	while ev&(4)=6
		checkpointerevent:(14,&1,&6,&0)
		geteventa32 status%,ev&()
		iowaitstat status%
	endwh
	checkpointerevent:(18,&1,&1,&0)
	
	id%=gcreate (320,0,320,gheight,1,1)
	gborder 2
	pointerfilter 2,7
	wid&=id%
	rem pause 10
	print "Please click the left mouse button"
	print "/pen in the new window"
	geteventa32 status%,ev&()
	iowaitstat status%
	checkpointerenter:(24,wid&)
	geteventa32 status%,ev&()
	iowaitstat status%
	checkpointerevent:(26,wid&,&0,&0)
	geteventa32 status%,ev&()
	iowaitstat status%
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		geteventa32 status%,ev&()
		iowaitstat status%
		i%=i%+1
		if i%>5 : raise 200 : endif
	endwh
	checkpointerevent:(30,wid&,&1,&0)

	rem pause 10
	print "Please drag the mouse/pen"
	print "back to the main window"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)=&409
		geteventa32 status%,ev&()
		iowaitstat status%
	endif
	checkpointerevent:(34,wid&,&0,&0)
	geteventa32 status%,ev&()
	iowaitstat status%
	while ev&(4)=6
		checkpointerevent:(38,wid&,&6,&0)
		geteventa32 status%,ev&()
		iowaitstat status%
	endwh
	checkpointerexit:(42,wid&)
	geteventa32 status%,ev&()
	iowaitstat status%
	checkpointerenter:(44,&1)
	geteventa32 status%,ev&()
	iowaitstat status%
	while ev&(4)=6
		checkpointerevent:(46,&1,&6,&0)
		geteventa32 status%,ev&()
		iowaitstat status%
	endwh
	checkpointerevent:(50,&1,&1,&0)
	gclose id%
	
	rem pause 10
	print "Please double click"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)=&409
		geteventa32 status%,ev&()
		iowaitstat status%
	endif
	checkpointerevent:(54,&1,&0,&0)
	geteventa32 status%,ev&()
	iowaitstat status%
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		geteventa32 status%,ev&()
		iowaitstat status%
		i%=i%+1
		if i%>5 : raise 300 : endif
	endwh
	checkpointerevent:(58,&1,&1,&0)
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)=&409
		geteventa32 status%,ev&()
		iowaitstat status%
	endif
	checkpointerevent:(62,&1,&0,&80000)
	geteventa32 status%,ev&()
	iowaitstat status%
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		geteventa32 status%,ev&()
		iowaitstat status%
		i%=i%+1
		if i%>5 : raise 400 : endif
	endwh
	checkpointerevent:(66,&1,&1,&0)	
	
	rem pause 10
	print
	print "Please click on system and then return to OPL"
	geteventa32 status%,ev&()
	iowaitstat status%
	while ev&(1)<>&402
		geteventa32 status%,ev&()
		iowaitstat status%
	endwh
	if ev&(1)<>&402 : raise 76 : endif
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)<>&401 : raise 77 : endif
		
	print "OK"
	rem pause 20
endp


proc tpointerpos:
	external ev&(),status%,timestat%
	local i%,t1&,t2&,timeout%,arg2%,timechan%
	cls
	pointerfilter 2,7
	print "Test pointer positions"
	print "Press a key to clear previous events (Esc to skip test)"
	if get=27		rem this clears all previous events
		return
	endif
	getevent32 ev&()		rem this clears "key up" from "get"
	if ev&(1)<>&407 : raise 1 : endif	
	print
	
	gat 50,50
	gcircle 2
	print "Please click in the circle - top right"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)=&409
		geteventa32 status%,ev&()
		iowaitstat status%
	endif
	checkpointerevent:(1,&1,&0,&0)
	checkpos:(&32,&32,&2)
	geteventa32 status%,ev&()
	iowaitstat status%
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		geteventa32 status%,ev&()
		iowaitstat status%
		i%=i%+1
		if i%>5 : raise 100 : endif
	endwh
	checkpointerevent:(6,&1,&1,&0)
	checkpos:(&32,&32,&2)

	gat 500,200
	gcircle 2
	print "Please click in the circle - bottom left"
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)=&409
		geteventa32 status%,ev&()
		iowaitstat status%
	endif
	checkpointerevent:(11,&1,&0,&0)
	checkpos:(&1f4,&c8,&2)
	geteventa32 status%,ev&()
	iowaitstat status%
	i%=0
	while ev&(1)=&408 and ev&(4)=6
		geteventa32 status%,ev&()
		iowaitstat status%
		i%=i%+1
		if i%>5 : raise 200 : endif
	endwh
	checkpointerevent:(16,&1,&1,&0)
	checkpos:(&1f4,&c8,&2)
	
	REM Moved this from TTimest:
	print "Please hold down mouse button/pen for approximately 3 secs"	
	pointerfilter 7,7
	getevent32 ev&()    rem gets pen down
	if ev&(1)<>$408 : raise 13 : endif
	if ev&(4)<>0 : raise 14 : endif
	t1&=ev&(2)
	
	geteventa32 status%,ev&()    rem gets pen up
	timeOut%=33
	ioa(timeChan%,FRELATIVE%,timeStat%,timeOut%,arg2%)
	iowait
	if status%=-46
		print "Time out!"
		print "(Still waiting for keypress)"
	else
		if ev&(1)<>$408 : raise 15 : endif
		if ev&(4)<>1 : raise 16 : endif
		t2&=ev&(2)	
		t2&=(t2&-t1&)/1000000
		print t2&;" secs apart"	
		if t2&<3 : print "Too quick!" : endif
	endif
	iow(timeChan%,FCANCEL%,#0,#0)
	iowaitstat timeStat%
	ioclose(timeChan%)
	
	print "OK"
	rem pause 20
endp


proc ttimest:
	external ev&(),status%,timestat%
	local unusedSafety%
	local debug$(255)
	local t1&,t2&,timeOut&,timeChan%,mode%
	cls
	print "Test time stamps"
	print "Press a key to clear previous events (Esc to skip test)"
	if get=27
		return
	endif
	getevent32 ev&()
	if ev&(1)<>$407 : raise 100 : endif
	print	

	mode%=-1
	ioopen(timeChan%,"TIM:",mode%)
	
	print "Please press two keys, approximately 2 secs apart"	
	rem first key
	getevent32 ev&()    rem gets key down
	if ev&(1)<>$406 : raise 1 : endif
	t1&=ev&(2)
	getevent32 ev&()    rem gets key press
	if (ev&(1) and $400)<>0 : print hex$(ev&(1)) : raise 2 : endif
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 3 : endif
	
	rem wait for second key
	geteventa32 status%,ev&()    rem gets key down
	timeOut&=23		rem allow a little extra time
	ioc(timeChan%,FRELATIVE%,timeStat%,timeOut&,unusedSafety%)
	iowait

	if status%=-46
		print "Timestat% must have completed:",timeStat%
		debug$="Time out! (Still waiting for keypress)"
		print debug$
	else
		rem status<>KErrFilePending%
		if ev&(1)<>$406 : raise 4 : endif
		t2&=ev&(2)	
		t2&=(t2&-t1&)/1000000
		print t2&;" secs apart"
		if t2&<2 : print "Too quick!" : endif
	endif
	iow(timeChan%,FCANCEL%,#0,#0)
	REM Or IOCANCEL(timeChan%)
	iowaitstat timeStat%

	getevent32 ev&()    rem gets key press
	IF LEN(debug$)
		hLog%:(KhLogAlways%,debug$)
	ENDIF
	if (ev&(1) and $400)<>0
		hLog%:(KhLogAlways%,"tTimest: EV&(1)=&"+HEX$(ev&(1)))
		raise 5
	endif
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 6 : endif
		
	print "Please press two keys, approximately 5 secs apart"	
	rem first key
	getevent32 ev&()    rem gets key down
	if ev&(1)<>$406 : raise 7 : endif
	t1&=ev&(2)
	getevent32 ev&()    rem gets key press
	if (ev&(1) and $400)<>0 : print hex$(ev&(1)) : raise 8 : endif
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 9 : endif
	
	rem wait for second key
	geteventa32 status%,ev&()    rem gets key down
	timeOut&=53
	ioC(timeChan%,FRELATIVE%,timeStat%,timeOut&)
	iowait
	if status%=-46
		print "Time out!"
		print "(Still waiting for keypress)"
	else
		if ev&(1)<>$406 : raise 10 : endif
		t2&=ev&(2)	
		t2&=(t2&-t1&)/1000000
		print t2&;" secs apart"
		if t2&<5 : print "Too quick!" : endif
	endif
	iow(timeChan%,FCANCEL%,#0,#0)
	iowaitstat timeStat%
	getevent32 ev&()    rem gets key press
	if (ev&(1) and $400)<>0 : print hex$(ev&(1)) : raise 11 : endif
	getevent32 ev&()    rem gets key up
	if ev&(1)<>$407 : raise 12 : endif
	
	
	print "OK"
	rem pause 20
	IOCLOSE(timeChan%)
endp


proc waitkeypress:(r%,keyp$,key&,sc&,mod&)
	external ev&(),status%
	print "Please press ";keyp$;" once"
	
	if mod&<>0
		geteventa32 status%,ev&()
		iowaitstat status%
		if ev&(1)<>&406 : raise r%+1 : endif
	endif
	
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)<>&406 : print ev&(1) : raise r%+2 : endif
	
	geteventa32 status%,ev&()
	iowaitstat status%
	if (ev&(1) AND $400)<>0 : print "Incorrect event ";hex$(ev&(1)) : raise r%+3 : endif
	if ev&(1)<>key& : print "Incorrect key ";hex$(ev&(1)) : raise r%+4 : endif
	if ev&(3)<>sc& : print "Incorrect scan code ";hex$(ev&(3)) : raise r%+5 : endif
	if ev&(4)<>mod& : print "Incorrect modifier ";hex$(ev&(4)) : raise r%+6 : endif
	if ev&(5)<>&0 : print "Incorrect repeats ";hex$(ev&(5)) : raise r%+7 : endif
	geteventa32 status%,ev&()
	iowaitstat status%
	if ev&(1)<>&407 : raise r%+8: endif
	
	if mod&<>0
		geteventa32 status%,ev&()
		iowaitstat status%
		if ev&(1)<>&407 : raise r%+9 : endif
	endif
endp


proc checkpointerevent:(r%,wid&,type&,mod&)
	external ev&()
	if (ev&(1) AND $fff)<>$408 : print "Incorrect event ";hex$(ev&(1)) : raise r% : endif
	if ev&(3)<>wid& : print "Incorrect window id ";hex$(ev&(3)) : raise r%+1 : endif
	if ev&(4)<>type& : print "Incorrect type ";hex$(ev&(4)) : raise r%+2 : endif
	if ev&(5)<>mod& : print "Incorrect modifier ";hex$(ev&(5)) : raise r%+3  : endif
endp


proc checkpointerenter:(r%,wid&)
	external ev&()
	if (ev&(1) AND $fff)<>$409 : print "Incorrect event ";hex$(ev&(1)) : raise r% : endif
	if ev&(3)<>wid& : print "Incorrect window id ";hex$(ev&(3)) : raise r%+1 : endif	
endp


proc checkpointerexit:(r%,wid&)
	external ev&()
	if (ev&(1) AND $fff)<>$40A : print "Incorrect event ";hex$(ev&(1)) : raise r% : endif
	if ev&(3)<>wid& : print "Incorrect window id ";hex$(ev&(3)) : raise r%+1 : endif	
endp


proc checkpos:(x&,y&,r&)
	external ev&()
	local dis&
	dis&=(ev&(6)-x&)*(ev&(6)-x&)+(ev&(7)-y&)*(ev&(7)-y&)
	if dis&>=r&*r&
		print "Outside the circle!" : print "Square root of ";dis&;" from centre!"
	else
		print "Inside the circle - OK"
	endif
endp

REM End of pGetEvAs.tpl
