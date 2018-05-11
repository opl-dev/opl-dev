REM swDlgerr.tpl
REM EPOC OPL script for interactive dialog errors.
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
	hLink:("swDlgerr", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wDlgerr.opo"


PROC swDlgerr:
	hRunTest%:("dowdlgerr")
	hCleanUp%:("CleanUp")
ENDP


PROC dowDlgerr:
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
	hEnter%:
ENDP


REM End of swDlgerr.tpl
