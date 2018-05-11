REM swDLong.tpl
REM EPOC OPL script for interactive wDLong.
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
	hLink:("swDLong", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wDLong.opo"


PROC swDLong:
	hRunTest%:("doswDLong")
	hCleanUp%:("CleanUp")
ENDP


PROC doswDLong:
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
	hSendString&:("-2147483646") :hDown%:
	hSendString&:("-2147483646") :hDown%:
	hSendString&:("2147483647") :hDown%:
	hSendString&:("2147483647") :hDown%:
	hSendString&:("0") :hEnter%:
ENDP


REM End of swDLong.tpl
