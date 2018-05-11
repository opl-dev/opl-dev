REM swAlerts.tpl
REM EPOC OPL script for interactive wAlert.
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
	hLink:("swAlert", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wAlert.opo"


PROC swAlert:
	hRunTest%:("doswAlert")
	hCleanUp%:("CleanUp")
ENDP


PROC doswAlert:
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
	hEsc%:
	hEsc%:
	hEsc%:

	hEnter%:
	hEsc%:
	
	hEnter%:
	hSpace%:
	hEsc%:

	hEsc%:
	hEsc%:

	hEnter%:
	hEsc%:

	hEnter%:
	hSpace%:
	hEsc%:

	hEsc%:
	hEsc%:
	hEsc%:
	hEsc%:
	hEsc%:
ENDP


REM End of swAlert.tpl
