REM wDFloat.tpl
REM EPOC OPL interactive test code for dFLOAT.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL
INCLUDE "hUtils.oph"
EXTERNAL dowDFloat1:
EXTERNAL dowDFloat2:
EXTERNAL dowDFloat3:
EXTERNAL dowDFloat4:
EXTERNAL dowDFloat10:
EXTERNAL dowDFloat12:
EXTERNAL dowDFloat13:


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wdFloat", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wdFloat:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("doWDFloat")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


PROC dowDFloat:
	dowDFloat1:
	dowDFloat2:
	dowDFloat3:
	dowDFloat4:

	dowDFloat10:
	dowDFloat12:
	dowDFloat13:
ENDP


proc dowDFloat1:
	local d1,d2,d3,d4,d5,d6
	local rep&
	dInit "dFloat test"
	dFloat d1,"]-1e100,1e100[ (9.999e99)",-9.99999999999999e99,9.99999999999999e99
	dFloat d2,"]-1e100,-1e-99] (-1e-99)",-9.99999999999999e99,-1e-99
	dFloat d3,"]-1e100,0] (-9.999e99)",-9.99999999999999e99,0
	dFloat d4,"[0,1e100[ (9.999e99)",0,9.99999999999999e99
	dFloat d5,"[1e-99,1e100[ (1e99)",1e-99,9.99999999999999e99
	dFloat d6,"0 only (0)",0,0
	IF dialog=0 :RAISE 1 :ENDIF
	IF d1<>9.999e99 :rep&=rep&+2 :ENDIF
	IF d2<>-1e-99 :rep&=rep&+4 :ENDIF
	IF d3<>-9.999e99 :rep&=rep&+8 :ENDIF
	IF d4<>9.999e99 :rep&=rep&+16 :ENDIF
	IF d5<>1e99 :rep&=rep&+32 :ENDIF
	IF d6<>0 :rep&=rep&+64 :ENDIF
	IF rep& :RAISE rep& :ENDIF
endp


proc dowDFloat2:
	local d11,d12,d13,d14,d15,d16
	local rep&
	rem New dialog added for Opl32 to check numbers >=1E100
	dInit "dFloat test full range"
	dFloat d11,"]-Max,Max[ (1.797e308)",-1.7976931348623156e308,1.7976931348623156e308
	dFloat d12,"]-Max,-Min] (-4.999e-324)",-1.7976931348623156e308,-5e-324
	dFloat d13,"]-Max,0] (-1.797e308)",-1.7976931348623156e308,0
	dFloat d14,"[0,Max[ (1.797e308)",0,1.7976931348623156e308
	dFloat d15,"[Min,Max[ (4.999e-324)",5e-324,1.7976931348623156e308
	dFloat d16,"0 only (0)",0,0
	IF dialog=0 :RAISE 1 :ENDIF
	IF d11<>1.797e308 : rep&=rep&+2 :ENDIF
	IF d12<>-4.999e-324 : rep&=rep&+4 :ENDIF
	IF d13<>-1.797e308 : rep&=rep&+8 :ENDIF
	IF d14<>1.797e308 : rep&=rep&+16 :ENDIF
	IF d15<>4.999e-324 : rep&=rep&+32 :ENDIF
	IF d16<>0 :rep&=rep&+64 :ENDIF
	IF rep& :RAISE rep& :ENDIF
endp


PROC dowDFloat3:
	local d1
	d1=0
	dInit
	dFloat d1,"[0,5.77] (3.14)",0,5.77
	IF dialog=0 :RAISE 1 :ENDIF
	IF d1<>3.14 :RAISE 2 :ENDIF
ENDP


PROC dowDFloat4:
	local d1,d2,d3,d4,d5,d6
	local rep&
	dInit
	dFloat d1,"-1e100,0 (-1e100)",-1e99*10,0
	dFloat d2,"-1e121,0 (-9.999e120)",-1e99*1e22,0
	dFloat d3,"0,1e100 (1e100)",0,1e99*10
	dFloat d4,"0,1e121 (9.999e120)",0,1e22*1e99
	dFloat d5,"-1e-100,0 (-1e-100)",-1e-99*10,0
	dFloat d6,"-1e-121,0 (-1e-121)",-1e-99*1e-22,0
	IF dialog=0 : RAISE 1 : ENDIF
	IF d1<>-1e100 : rep&=rep&+2 :ENDIF
	IF d2<>-9.999e-120 : rep&=rep&+4 :ENDIF
	IF d3<>1e100 : rep&=rep&+8 :ENDIF
	IF d4<>9.999e120 : rep&=rep&+16 :ENDIF
	IF d5<>-1e-100 : rep&=rep&+32 :ENDIF
	IF d6<>-1e-121 :rep&=rep&+64 :ENDIF
	IF rep& :RAISE rep& :ENDIF
ENDP


PROC dowDFloat10:
	local d1,d2
	local rep&
	dInit
	dFloat d1,"0,1e-100 (1e-100)",0,1e-99*10
	dFloat d2,"0,1e-121 (1e-121)",0,1e-22*1e-99
	IF dialog=0 : RAISE 1 : ENDIF
	IF d1<>1e-100 : rep&=rep&+2 :ENDIF
	IF d2<>1e-121 : rep&=rep&+4 :ENDIF
	IF rep& :RAISE rep& :ENDIF
ENDP


PROC dowDFloat12:
	local d1,rep&
	dInit
	d1=-1
	dFloat d1,"0,10 :init -1 (10)",0,10
	IF dialog=0 : RAISE 1 : ENDIF
	IF d1<>10 : rep&=rep&+2 :ENDIF
	IF rep& :RAISE rep& :ENDIF
ENDP


PROC dowDFloat13:
	local d1,rep&
	dInit
	d1=20
	dFloat d1,"0,10 :init 20 (10)",0,10
	IF dialog=0 : RAISE 1 : ENDIF
	IF d1<>10 : rep&=rep&+2 :ENDIF
	IF rep& :RAISE rep& :ENDIF
ENDP


REM End of WDFloat.tpl

