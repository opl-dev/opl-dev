REM swDTime.tpl
REM EPOC OPL script for interactive wDTime.
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
	hLink:("swDTime", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wDTime.opo"


PROC swDTime:
	hRunTest%:("doswDTime")
	hCleanUp%:("CleanUp")
ENDP


PROC doswDTime:
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
	REM First dialog.
	hSendString&:("0100a") :hDown%:
	hSendString&:("020304a") :hDown%:
	hSendString&:("0506") :hDown%:
	hSendString&:("070809") :hEnter%:

	REM Second dialog.
	hSendString&:("1011a") :hDown%:
	hSendString&:("120000a") :hDown%:
	hSendString&:("1159") :hDown%:
	hSendString&:("115959") :hEnter%:
ENDP


REM End of swDTime.tpl
