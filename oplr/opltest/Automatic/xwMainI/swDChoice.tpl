REM swDChoice.tpl
REM EPOC OPL script for interactive wDChoice.
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
	hLink:("swDChoice", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wDChoice.opo"


PROC swDChoice:
	hRunTest%:("doswDChoice")
	hCleanUp%:("CleanUp")
ENDP


PROC doswDChoice:
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
	hRight%: :hDown%:
	hLeft%: :hDown%:
	hLeft%: :hDown%:
	hLeft%: :hDown%:
	hLeft%: :hLeft%: :hDown%:
	hEnter%:

	REM Next.
	hEsc%:
		
	hEnter%:
	
	hEsc%:
ENDP


REM End of swDChoice.tpl
