REM wDDate.tpl
REM EPOC OPL interactive test code for dDATE.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wDDate", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wDDate:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowDDate")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


proc dowDDate:
	local d1&,d2&,d3&,d4&,d5&,d6&
	local rep&

	dInit "Test dDate"
	dDate d1&,"1/1/1900 to 31/12/2155 (29/2/2000)",0,days(31,12,2155)
	dDate d2&,"1/1/1970 to 31/12/1999 (21/1/1987)",days(1,1,1970),days(31,12,1999)
	dDate d3&,"1/1/1900 to 1/1/1900 (1/1/1900)",days(1,1,1900),days(1,1,1900)
	dDate d4&,"31/12/2155 to 31/12/2155 (31/12/2155)",days(31,12,2155),days(31,12,2155)
	dDate d5&,"Current to 31/12/2099 (31/12/2049)",days(day,month,year),days(31,12,2099)
	IF dialog=0 :RAISE 1 :ENDIF
	IF d1&<>36583 :rep&=rep&+2 :ENDIF
	IF d2&<>31796 :rep&=rep&+4 :ENDIF
	IF d3&<>0 :rep&=rep&+8 :ENDIF
	IF d4&<>93501 :rep&=rep&+16 :ENDIF
	IF d5&<>54786 :rep&=rep&+32 :ENDIF
	IF rep& :RAISE rep& :ENDIF

	rem print "dDate d1&,""Range 10000,0"""
	rem print "Min>Max - expect Invalid arguments"
  onerr e2
	dinit
	dDate d1&,"Range 10000,0",10000,0
	dialog
	ONERR OFF
  RAISE 100

e2::
	onerr off
	IF ERR<>-2 :RAISE 101 :ENDIF

	rem print "dDate d1&,""Absolute, no secs"",";
	rem &21900;",";&43200,""

  onerr e3
	dInit "Test dDate"
	dDate d1&,"Absolute, no secs (01/01/2555)",&21900,&43200
	IF dialog=0: ONERR OFF :RAISE 102: ENDIF
	IF d1&<>239234 :rep&=rep&+200 :ENDIF
	ONERR OFF
	IF rep& :RAISE rep& :ENDIF
	RETURN

e3::
	ONERR OFF
	RAISE 102
endp

REM End of wDDate.tpl

