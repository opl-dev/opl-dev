REM spDTime.tpl
REM EPOC OPL script for interactive pDTime.
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
	hLink:("spDTime", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\pMainI\pDTime.opo"


PROC spDTime:
	hRunTest%:("dospDTime")
	hCleanUp%:("CleanUp")
ENDP


PROC dospDTime:
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
	hSendString&:("59") :hDown%:
	hSendString&:("125959p") :hDown%:
	hSendString&:("2359") :hDown%:
	hSendString&:("0102") :hEnter%:

	REM Second dialog.
	hSendString&:("5959p") :hDown%:
	hSendString&:("235959") :hDown%:
	hSendString&:("59") :hDown%:
	hSendString&:("5959") :hEnter%:
	
	REM Third dialog.
	hSendString&:("59") :hDown%:
	hSendString&:("2359") :hEnter%:
ENDP


REM End of spDTime.tpl
