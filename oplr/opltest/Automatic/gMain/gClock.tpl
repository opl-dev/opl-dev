REM gClock.tpl
REM EPOC OPL automatic test code for gCLOCK.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("gClock", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gClock:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("gClockI")
rem	hCleanUp%:("CleanUp")
endp


proc gClockI:
	local type%,date%,secs%,amPm%,offset&
	local flags%
	
	rem print "This tests all Opl1993 modes only"
	rem print "For ER1 many of these modes are invalid"
	rem print "See also PCLOCK and PCLOCK11 tests"
	rem get
	
	flags%=-1
	onerr e1
	raise 0
e1::
	onerr off
	if err
		rem print err$(err)
		if (flags%<6 or flags%>9) and err<>-2
			raise flags%-1
		endif
		rem pause 20
	endif
	flags%=flags%+1
	if flags%>$8A : gclock off : return : endif
	gAt gWidth/2-15,10
	do
		if (flags% and $000f)=$B
			flags%=flags% and $fff0
			if (flags%=0) :flags%=$10
			elseif flags%=$10 :flags%=$20
			elseif flags%=$20 :flags%=$40
			elseif flags%=$40 :flags%=$80
			endif
		endif
		cls
		gClock off
		if offset&=0 :offset&=60		:rem cycle between 0,60,-60 offset
		elseif offset&=60 :offset&=-60
		elseif offset&=-60 :offset&=0
		endif
		rem print "gClock flags=&";hex$(flags%)
		rem print "Offset=";offset&
		onerr e1
		gClock on,flags%,offset&
		onerr off
		flags%=flags%+1
		pause 10
		rem get
	until flags%>$8A
	gClock off
endp

REM End of gClock.tpl 
