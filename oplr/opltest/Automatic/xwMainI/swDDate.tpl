REM swDDate.tpl
REM EPOC OPL script for interactive wDDate.
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
	hLink:("swDDate", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wDDate.opo"


PROC swDDate:
	hRunTest%:("doswDDate")
	hCleanUp%:("CleanUp")
ENDP


PROC doswDDate:
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
	hSendString&:("29022000") :hDown%:
	hSendString&:("21011987") :hDown%:
	hSendString&:("01011900") :hDown%:
	hSendString&:("31122155") :hDown%:
	hSendString&:("31122049") :hEnter%:

	REM Last.
	hSendString&:("01012555") :hEnter%:
ENDP


REM End of swDDate.tpl
