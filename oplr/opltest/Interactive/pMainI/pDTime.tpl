REM pDTime.tpl
REM EPOC OPL interactive test code for dTIME
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

include "const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pDTime", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pDTime:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("timeDialog")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
	rem	hCleanUp%:("Reset")
ENDP


proc timeDialog:
	time01:
	time02:
	time03:
endp


proc time01:
	local t1&,t2&,t3&,t4&
	local rep&
	dInit "One flag only - all items allow 24 hours"
	dTime t1&,"No hours (59)",KDTimeNoHours%,0,86399
	dTime t2&,"With Seconds (12:59:59p)",KDTimeWithSeconds%,0,86399
	dTime t3&,"Duration (23:59)",KDTimeDuration%,0,86399
	dTime t4&,"24 Hour (01:02)",KDTime24Hour%,0,86399
	dBUTTONS "OK",13,"Cancel",-27

	IF dialog=0 :RAISE 1 :ENDIF
	IF t1&<>3540 :rep&=rep&+2 :ENDIF
	IF t2&<>46799 :rep&=rep&+4 :ENDIF
	IF t3&<>86340 :rep&=rep&+8 :ENDIF
	IF t4&<>3720 :rep&=rep&+16 :ENDIF
	IF rep& :RAISE rep& :ENDIF	
endp


proc time02:
	local t1&,t2&,t3&,t4&
	local rep&
	dInit "More than one flag - all items allow 24 hours"
	dTime t1&,"No hours with seconds (5959p)",KDTimeNoHours% or KDTimeWithSeconds%,0,86399
	dTime t2&,"Duration with seconds (23:59:59)",KDTimeWithSeconds% or KDTimeDuration%,0,86399
	dTime t3&,"Duration with no hours (59)",KDTimeDuration% or KDTimeNoHours%,0,86399
	dTime t4&,"Duration with no hours but seconds (59:59)",KDTimeDuration% or KDTimeNoHours% or KDTimeWithSeconds%,0,86399
	dBUTTONS "OK",13,"Cancel",-27
	IF dialog=0 :RAISE 1 :ENDIF
	IF t1&<>46799 :rep&=rep&+2 :ENDIF
	IF t2&<>86399 :rep&=rep&+4 :ENDIF
	IF t3&<>3540 :rep&=rep&+8 :ENDIF
	IF t4&<>3599 :rep&=rep&+16 :ENDIF
	IF rep& :RAISE rep& :ENDIF	
endp


proc time03:
	local t1&,t2&,t3&,t4&
	local rep&

	dInit "Meaningless flags - all items allow 24 hours"
	dTime t1&,"24 Hour with no hours (59)",KDTime24Hour% or KDTimeNoHours%,0,86399
	dTime t2&,"Duration with 24 Hour (2359)",KDTime24Hour% or KDTimeDuration%,0,86399		
	dBUTTONS "OK",13,"Cancel",-27

	IF dialog=0 :RAISE 1 :ENDIF
	IF t1&<>3540 :rep&=rep&+2 :ENDIF
	IF t2&<>86340 :rep&=rep&+4 :ENDIF
	IF rep& :RAISE rep& :ENDIF	
endp


REM End of pDTime.tpl

