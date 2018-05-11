REM wDTime.tpl
REM EPOC OPL interactive test code for DTime.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wDTime", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wDTime:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowDTime")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


PROC dowDTime:
	dowDTime1:
	dowDTime2:
ENDP


proc dowDTime1:
	local t1&,t2&,t3&,t4&
	local rep&

	dInit "Test dTime - full range accepted"
	dTime t1&,"Absolute, no secs (0100a)",0,0,86399
	dTime t2&,"Absolute, with secs (020304a)",1,0,86399
	dTime t3&,"Duration, no secs (0506)",2,0,86399
	dTime t4&,"Duration, with secs (070809)",3,0,86399
	IF dialog=0 :RAISE 1 :ENDIF
	IF t1&<>3600 :rep&=rep&+2 :ENDIF
	IF t2&<>7384 :rep&=rep&+4 :ENDIF
	IF t3&<>18360 :rep&=rep&+8 :ENDIF
	IF t4&<>25689 :rep&=rep&+16 :ENDIF
	IF rep& :RAISE rep& :ENDIF
ENDP


PROC dowDTime2:
	local t1&,t2&,t3&,t4&
	local rep&

	dInit "Test dTime - until 12 noon accepted"
	dTime t1&,"Absolute, no secs (1011a)",0,0,43200
	dTime t2&,"Absolute, with secs (120000a)",1,0,43200
	dTime t3&,"Duration, no secs (1159)",2,0,43200
	dTime t4&,"Duration, with secs (115959)",3,0,43200
	IF dialog=0 :RAISE 1 :ENDIF
	IF t1&<>36660 :rep&=rep&+2 :ENDIF
	IF t2&<>0 :rep&=rep&+4 :ENDIF
	IF t3&<>43140 :rep&=rep&+8 :ENDIF
	IF t4&<>43199 :rep&=rep&+16 :ENDIF
	IF rep& :RAISE rep& :ENDIF

	rem print "Test: dTime t&,""Range 10000,0"""
	onerr e1
	dinit
	dTime t1&,"Range 10000,0",0,10000,0
	dialog
	onerr off
	rem print "Invalid range not detected" :get
	RAISE 100

e1::
	ONERR OFF

	REM	Range beyond times detection
	onerr e2
	dInit "Test dTime"
	dTime t1&,"Absolute, no secs",0,&21900,&43200
	dialog
	onerr off
	rem print "Range beyond dates not detected" :get
	RAISE 101
	
e2::
	ONERR OFF
	RETURN
endp


REM End of wDTime.tpl

