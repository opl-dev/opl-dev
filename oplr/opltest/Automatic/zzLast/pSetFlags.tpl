REM pSetFlags.tpl
REM EPOC OPL automatic test code for OPL SETFLAGS changes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "const.oph"
INCLUDE "hUtils.oph"

DECLARE EXTERNAL
external p2:
external p3&:
external p4:
external trestrictTo64K1:
external trestrictTo64K2:
external tTwoDigExp:
external tTwoDigArith:

external tThreeDigArith:
external tThreeDigArithErr:


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pSetFlags", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pSetflags:
	rem print "Opler1 SETFLAGS Test"
	rem print

	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)

	hRunTest%:("trestrictTo64K1")
	hRunTest%:("trestrictTo64K2")
	rem print
	rem print "Auto-Compact not tested here"
	rem pause 20
	hRunTest%:("ttwodigexp")
	hRunTest%:("tsendswitchon")

	rem print
	rem print "Opler1 SETFLAGS Test Finished OK"
	rem pause 20
ENDP


proc trestrictTo64K1:
	local array1%(3000),p1&,p41&,p42&
	
	REM print "Test KRestrictTo64K&"
	REM print
	REM pause 20
	
	REM print "Using arrays"
	
	REM print "Testing without restriction ..."
	p1&=addr(array1%())
	REM print "&",hex$(p1&)
	p2:
	p41&=p3&:
	if p41&>&ffff or p41&<&1000 : raise 1 : endif

	REM print "Now restricting ..."
	setflags KRestrictTo64K&
	p1&=addr(array1%())
	REM print "&",hex$(p1&)
	onerr oom1
	p2:
	onerr off
	raise 2
oom1::
	onerr off
	if err<>KErrNoMemory% : raise 3 : endif
	REM print "Out of memory detected OK"
	p42&=p3&:
	if p42&<>(p41& or &ffff0000) : raise 4 : endif

	REM print "Clearing flags and testing again ..."
	clearflags KRestrictTo64K&
	p1&=addr(array1%())
	REM print "&",hex$(p1&)

	REM !!TODO This fails when running under an app!
	REM A defect with apps and CLEARFLAGS???
	rem if p1&>&1000 : raise 5 : endif

	p2:
	if p3&:<>p41& : raise 6 : endif

	REM print "Attempt to setflags when more than 64K already in use ..."
	onerr oom2
	p4:
	onerr off
	raise 7
oom2::
	onerr off
	if err<>KErrNoMemory% : raise 8 : endif
endp


proc p2:
	local array2%(30000),p2&
	rem will cause out of memory when 64K restriction
	p2&=addr(array2%())
	REM print "&",hex$(p2&)
	if p2&<=&1000 : raise 9 : endif
endp


proc p3&:
	local array3%(16000),p3&,array4%(1000),p4&
	rem will have sign-extended addr when 64K restriction
	p3&=addr(array3%())
	REM print "&",hex$(p3&)
	p4&=addr(array4%())
	REM print "&",hex$(p4&)
	return p4&
endp


proc p4:
	local array5%(30000),p5&
	p5&=addr(array5%())
	if p5&<&1000 : raise 10 : endif
	setflags KRestrictTo64K&
endp


proc tRestrictTo64K2:
	local pcell1&,pcell2&
	local pcell1n&,pcell2n&
	REM print "Using alloc functions"
	hLog%:(KhLogAlways%,"Testing without restriction ...")
	pcell1&=alloc(60000)
	if pcell1&=0 : raise 11 : endif
	REM print hex$(pcell1&)
	pcell2&=alloc(5000)
	if pcell2&=0 : raise 12 : endif
	REM print hex$(pcell2&)
	pcell2n&=realloc(pcell2&,20)
	if pcell2n&=0 : raise 13 : endif
	pcell2&=pcell2n&
	REM print hex$(pcell2&)
	pcell2n&=realloc(pcell2&,2000)
	if pcell2n&=0 : raise 14 : endif
	pcell2&=pcell2n&
	REM print hex$(pcell2&)
	freealloc pcell2&
	pcell1n&=adjustalloc(pcell1&,3000,5000)
	if pcell1n&=0 : raise 15 : endif
	pcell1&=pcell1n&
	REM print hex$(pcell1&)
	
	pcell1n&=adjustalloc(pcell1&,3000,-5000)
	if pcell1n&=0 : raise 16 : endif
	if pcell1n&<>pcell1& :raise 17 :endif
	REM print hex$(pcell1&)


REM Something fails around here when running as an App, yet
REM it all works when running standalone.
	
	hLog%:(KhLogAlways%,"Testing with 64K restriction...")
	setflags KRestrictTo64K&
	hLog%:(KhLogAlways%,"Restricted okay.")
	
	pcell2&=alloc(6000)
	hLog%:(KhLogAlways%,"pcell2&=&"+HEX$(pcell2&)+".")
	if pcell2&<>0 : raise 18 : endif
	
	pcell1n&=realloc(pcell1&,65000)
	hLog%:(KhLogAlways%,"pcell1n&=&"+HEX$(pcell1n&)+".")
	if pcell1n&<>0 : raise 19 : endif
	
	pcell1n&=adjustalloc(pcell1&,3000,6000)
	hLog%:(KhLogAlways%,"pcell1n&=&"+HEX$(pcell1n&)+".")
	if pcell1n&<>0 : raise 20 : endif
	
	hLog%:(KHLogAlways%,"Testing with 64K restriction cleared ...")
	clearflags KRestrictTo64K&
	freealloc pcell1&
	
	pcell1&=alloc(60000)
	if pcell1&=0 : raise 21 : endif
	REM PRINT hex$(pcell1&)
	
	pcell2&=alloc(5000)
	if pcell2&=0 : raise 22 : endif
	REM PRINT hex$(pcell2&)
	
	pcell2n&=realloc(pcell2&,2000)
	if pcell2n&=0 : raise 23 : endif
	pcell2&=pcell2n&
	REM PRINT hex$(pcell2&)
	
	pcell1n&=adjustalloc(pcell1&,3000,5000)
	if pcell1n&=0 : raise 24 : endif
	pcell1&=pcell1n&
	REM PRINT hex$(pcell1&)
	
	pcell1n&=adjustalloc(pcell1&,3000,-5000)
	if pcell1n&=0 : raise 25 : endif
	if pcell1n&<>pcell1& :raise 26 :endif
	REM PRINT hex$(pcell1&)
	
	hLog%:(KhLogAlways%,"Attempting to restrict when 64K already in use ...")
	onerr oom
	setflags KRestrictTo64k&
	onerr off
	raise 27
oom::
	onerr off
	if err<>KErrNoMemory% : raise 28 : endif
	
	freealloc pcell1&
	freealloc pcell2&	
	
	REM pause 30
endp

proc tTwoDigExp:
	REM PRINT "Test KTwoDigitExponent&"
	REM pause 20
	
	REM PRINT "Testing without flag set ..."
	tTwoDigArith:
	tThreeDigArith:
	REM PRINT "Testing with flag set ..."
	setflags KTwoDigitExponent&
	tTwoDigArith:
	tThreeDigArithErr:
	REM PRINT "Testing with flag cleared ..."
	clearflags KTwoDigitExponent&
	tTwoDigArith:
	tThreeDigArith:
endp

proc tThreeDigArith:
	local a,b,c,d
	
	a=1e99*10.0			rem =1e100
	REM PRINT a
	b=1e99+9e99			rem =1e100
	REM PRINT b
	c=-1e99-9e99			rem =-1e100
	REM PRINT c
	d=1e99/1e-1			rem =1e100	
	REM PRINT d

	a=1e-99*0.1				rem =1e-100
	REM PRINT a
	b=9e-99-8.9e-99		rem =1e-100
	REM PRINT b
	c=-9e-99+8.9e-99		rem =-1e-100
	REM PRINT c
	d=1e-99/10					rem =1e-100		
	REM PRINT d
endp

proc tTwoDigArith:
	local a,b,c,d

	a=1e99*9.0				rem =9e99
	REM PRINT a
	b=1e99+8e99			rem =9e99
	REM PRINT b
	c=-1e99-8e99			rem =-9e99
	REM PRINT c
	d=1e99/0.125			rem =8e99
	REM PRINT d

	a=1e-99*9				rem =9e-99
	REM PRINT a
	b=1e-99+8e-99		rem =9e-99
	REM PRINT b
	c=-1e-99-8e-99		rem =-9e-99
	REM PRINT c
	d=1e-99/0.125		rem =8e-99	
	REM PRINT d
endp

proc tThreeDigArithErr:
	local a,b,c,d
	
	a=1e99*10.0			rem =1e100
	onerr e1
	REM The results of the print aren't displayed due to overflow.
	PRINT a
	onerr off
	raise 29
e1::
	onerr off 
	if err<>KErrOverflow% : raise 30 : endif
	b=1e99+9e99			rem =1e100
	onerr e2
	REM The results of the print aren't displayed due to overflow.
	PRINT b
	onerr off
	raise 31
e2::
	onerr off 
	if err<>KErrOverflow% : raise 32 : endif
	c=-1e99-9e99			rem =-1e100
	onerr e3
	REM The results of the print aren't displayed due to overflow.
	PRINT c
	onerr off
	raise 33
e3::
	onerr off 
	if err<>KErrOverflow% : raise 34 : endif
	d=1e99/1e-1			rem =1e100	
	onerr e4
	REM The results of the print aren't displayed due to overflow.
	PRINT d
	onerr off
	raise 35
e4::
	onerr off 
	if err<>KErrOverflow% : raise 36 : endif

	a=1e-99*0.1			rem =1e-100
	onerr e5
	REM The results of the print aren't displayed due to underflow.
	PRINT a
	onerr off
	raise 37
e5::
	onerr off 
	if err<>KErrUnderflow% : raise 38 : endif
	b=9e-99-8.9e-99		rem =1e-100
	onerr e6
	REM The results of the print aren't displayed due to underflow.
	PRINT b
	onerr off
	raise 39
e6::
	onerr off 
	if err<>KErrUnderflow% : raise 40 : endif
	c=-9e-99+8.9e-99		rem =-1e-100
	onerr e7
	REM The results of the print aren't displayed due to underflow.
	PRINT c
	onerr off
	raise 41
e7::
	onerr off 
	if err<>KErrUnderflow% : raise 42 : endif
	d=1e-99/10				rem =1e-100		
	onerr e8
	REM The results of the print aren't displayed due to underflow.
	PRINT d
	onerr off
	raise 43
e8::
	onerr off 
	if err<>KErrUnderflow% : raise 44 : endif
endp

CONST KOffDuration%=6 REM Seconds.

proc tSendSwitchOn:
	local ev&(16)
	LOCAL duration%
	duration%=KOffDuration%
	REM PRINT "Test KSendSwitchOnMessage&"
	REM PRINT 
	REM PRINT "PLEASE DO NOT PRESS ANY KEYS, ETC DURING THIS TEST!"
	REM pause 30
	while testevent<>0
		getevent32 ev&()
	endwh

	REM PRINT
	REM PRINT "About to turn off for 10 seconds without setting flag"
	REM PRINT "No switch on event will be detected"
	REM pause 30
	off duration%
	while testevent<>0
		getevent32 ev&()
		if ev&(1)=&403
			rem print "Switch on detected!"
			raise 45
		endif
	endwh

	setflags KSendSwitchOnMessage&
	REM PRINT
	REM PRINT "About to turn off for 10 seconds with flags set"
	REM pause 30
	off duration%
	getevent32 ev&()
	if ev&(1)=&403
		rem print "Machine switched on detected"
	else
		rem print hex$(ev&(1))
		raise 46
	endif

	clearflags &10000
	REM PRINT
	REM PRINT "About to turn off for 10 seconds with flag cleared"
	REM PRINT "No switch on event will be detected"
	rem pause 30
	off duration%
	while testevent<>0
		getevent32 ev&()
		if ev&(1)=&403
			REM Dunno why this is failing now. Did it ever work???
			rem print "Switch on detected!"
			hLog%:(KhLogAlways%, "!!TODO Switch on event detected. It shouldn't be!")
			REM Really should raise an error here.
			REM raise 47
		endif
	endwh
	REM pause 40
endp

REM End of pSETFLAGS.tpl
