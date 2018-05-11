REM spMPopup.tpl
REM EPOC OPL script for interactive testing of pMPopup.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL 

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "System.oxh"

EXTERNAL PopupScript%:
EXTERNAL SpoofSanityCheck%:
EXTERNAL EasyEnter%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("spMPopup", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem	dINIT "Tests complete" :DIALOG
ENDP

CONST kTarget$="\Opltest\Interactive\pMainI\pMPopup.opo"

PROC spMPopup:
	hRunTest%:("spMPopup01")
	hCleanUp%:("CleanUp")
ENDP


PROC spMPopup01:
	LOCAL thread&
	thread&=hSpoofRunTargetApp&:("OPL", "", KSyRunAppOpl$+hDiskName$:+KTarget$,KSyRunAppRun%)
	hSpoofSetFlagTargetApp%:(KTarget$,KFalse%)
	PopupScript%:
	PAUSE KhCloseAppDelay%
	IF hSpoofGetFlagTargetApp%:(KTarget$) :RAISE 31313 :ENDIF
ENDP


PROC CleanUp:
	hSpoofCloseApp%:
ENDP


PROC PopupScript%:
	local i%
	i%=15
	DO
		EasyEnter%:
		i%=i%-1
	UNTIL i%=0

	hDown%:
	EasyEnter%:
ENDP


PROC EasyEnter%:
	hEnter%:
	PAUSE KhUIDelay%
	RETURN
ENDP


PROC SpoofSanityCheck%:
	dinit "Some sanity check"
	dialog
ENDP


REM End of spMPopup.tpl
