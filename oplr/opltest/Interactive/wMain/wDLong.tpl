REM wDLong.tpl
REM EPOC OPL interactive test code for wDLong.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wDLong", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wDLong:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowDLong")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


proc dowDLong:
	local l1&,l2&,l3&,l4&,l5&
	local rep&

	rem do
	dInit "dLong test"
	dLong l1&,num$(&80000000,13)+" to -1 (-2147483646)",&80000000,-1
	dLong l2&,num$(&80000000,13)+" to 0 (-2147483646)", &80000000, 0
	dLong l3&,"1 to "+num$(&7fffffff,12)+" (2147483647)",1,&7fffffff
	dLong l4&,"0 to "+num$(&7fffffff,12)+" (2147483647)",0,&7fffffff
	dLong l5&,"0 only (0)",0,0
	rem until dialog=0
	
	IF dialog=0 : RAISE 1 :ENDIF
	IF l1&<>-2147483646 :rep&=rep&+2 :ENDIF
	IF l2&<>-2147483646 :rep&=rep&+4 :ENDIF
	IF l3&<>2147483647 :rep&=rep&+8 :ENDIF
	IF l4&<>2147483647 :rep&=rep&+16 :ENDIF
	IF l5&<>0 :rep&=rep&+64 :ENDIF
	IF rep& :RAISE rep& :ENDIF
endp

REM End of wDLong.tpl

