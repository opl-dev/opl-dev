REM spIO.tpl
REM EPOC OPL script for interactive pIO.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL 

REM For modifier consts.
INCLUDE "System.oxh" 	
INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNAL KeyScript%:
EXTERNAL SpoofSanityCheck%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("spIO", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\pMainI\pIO.opo"


PROC spIO:
	hRunTest%:("dospIO")
	hCleanUp%:("CleanUp")
ENDP


PROC dospIO:
	LOCAL thread&
	thread&=hSpoofRunTargetApp&:("OPL", "", KSyRunAppOpl$+hDiskName$:+KTarget$,KSyRunAppRun%)
	hSpoofSetFlagTargetApp%:(KTarget$,KFalse%)
	KeyScript%:
	PAUSE KhCloseAppDelay%
	IF hSpoofGetFlagTargetApp%:(KTarget$) :RAISE 31313 :ENDIF
ENDP


PROC CleanUp:
	hSpoofCloseApp%:
ENDP


PROC KeyScript%:
	PAUSE 40 REM Wait over a second.
	hEnter%: REM Send sync keypress.
	
	PAUSE 20 REM Exactly one second.
	hSpace%: REM Keypress to interupt the IO.
	PAUSE 60 REM Wait three seconds for second test to complete.
	
ENDP


REM End of spIO.tpl
