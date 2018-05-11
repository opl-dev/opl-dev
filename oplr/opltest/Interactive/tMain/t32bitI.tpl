REM t32bitI.tpl
REM EPOC OPL interactive test code for opler1 32-bit ops.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL
INCLUDE "const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("t32bitI", hThreadIdFromOplDoc&:, KhUserLoggingOnly%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete"
	DIALOG
ENDP


PROC t32bitI:
	hCall%:("dot32bitI")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
rem 	hCleanUp%:("Reset")
ENDP


PROC Reset:
	rem Any clean-up code here.
ENDP


PROC dot32bitI:
	local v$(255), arr$(2,5), ed$(30), xin$(8), file$(255)
	local v, arr(4)
	local v%, choice%, stat%, ev%(16)
	local time&, l&, ev&(16)

	hLog%:(KhLogAlways%,"Edit v$")
	rem edit/input values
	print "Edit v$: Please enter ""12345"""
	v$=""
	EDIT v$
	if v$<>"12345" : raise 28 : endif
	
	hLog%:(KhLogAlways%,"Edit arr$(2)")
	print "Edit arr$(2): Please enter ""12345"""
	arr$(2)=""
	EDIT arr$(2)
	if arr$(2)<>"12345" : raise 29 : endif
	
	hLog%:(KhLogAlways%,"Edit v")
	print "Input v: Please enter ""12345"""
	INPUT v
	if v<>12345 : raise 30 : endif
	
	hLog%:(KhLogAlways%,"Edit arr(2)")
	print "Input arr(2): Please enter ""12345"""
	INPUT arr(2)
	if arr(2)<>12345 : raise 31 : endif
	
	hLog%:(KhLogAlways%,"Menu")
	rem Args passed ByRef (var or #)
	print "In the next menu, please select 3rd item on 2nd pane ('Item e')"
	rem pause 10
	v%=256+$100
	minit
	mcard "Pane1","Item a",%a,"Item b",%b
	mcard "Pane2","Item c",%c,"Item d",%d,"Item e",%e
	if menu(v%)<>%e :raise 333 :endif
	if v%<>$0202 : raise 32 : endif
	
	hLog%:(KhLogAlways%,"Control live vars")
	do
		dinit "Check control live vars"
		dchoice choice%,"dChoice- Please select C","A,B,C,D"
		dTime time&,"dTime - Please enter 01:02:03",1,&0,&7fff
		dEdit ed$,"dEdit - Please enter ""hello""",10
		dXInput xin$,"dXInput - Please enter ""pass"""
		dFile file$,"dFile - Please enter ""abc"",for root drive C",1
		dLong l&,"dLong - Please enter -32769", -32770,10
		dBUTTONS "OK",13,"Cancel",-27
	until dialog
	if choice%<>3 : raise 33 : endif
	if time&<>60*60+60*2+3 : raise 34 : endif
	if ed$<>"hello" : raise 35 : endif
	if xin$<>"pass" : raise 36 : endif
	if upper$(file$)<>upper$("c:\abc") : raise 37 : endif
	if l&<>-32769 : raise 38 : endif
	
	hLog%:(KhLogAlways%,"Clear events")
	cls
	do
		getevent ev%()
	until testevent=0
	
	hLog%:(KhLogAlways%,"Getevent() for x")
	print "Test getEvent ev%() - please press 'x'"
	GETEVENT ev%()
	if ev%(1)<>$406
		hLog%:(KhLogAlways%,HEX$(ev%(1)))
		raise 39
	endif						rem key down
	GETEVENT ev%()
	if ev%(1)<>%x
		raise 40
	endif							rem keypress
	GETEVENT ev%()
	if ev%(1)<>$407
		raise 41
	endif						rem key up
	
	hLog%:(KhLogAlways%,"GetEventa32 for x")
	print "Test getEventa32 stat%,ev&() - please press 'x'"
	GETEVENTA32 stat%, ev&()
	IOWAITSTAT stat%
	if stat%<>0 : raise 42 : endif
	if ev&(1)<>&406 : raise 43 : endif
	GETEVENTA32 stat%, ev&()
	IOWAITSTAT stat%
	if stat%<>0 : raise 44 : endif
	if ev&(1)<>%x : raise 45 : endif
	GETEVENTA32 stat%, ev&()
	IOWAITSTAT stat%
	if stat%<>0 : raise 46 : endif
	if ev&(1)<>&407 : raise 47 : endif
	
	hLog%:(KhLogAlways%,"getEvent32 ev&() for x")
	print "Test getEvent32 ev&() - please press 'x'"
	ev&(1)=0
	GETEVENT32 ev&()
	if ev&(1)<>&406 : raise 48 : endif
	GETEVENT32 ev&()
	if ev&(1)<>%x : raise 49 : endif
	GETEVENT32 ev&()
	if ev&(1)<>&407 : raise 50 : endif
ENDP


REM End of t32bitI.tpl
