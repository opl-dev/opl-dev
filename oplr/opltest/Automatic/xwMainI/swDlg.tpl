REM swDlg.tpl
REM EPOC OPL script for interactive wDlg.
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
	hLink:("swDlg", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wDlg.opo"


PROC swDlg:
rem	hLogChangeThreshold%:(KhLogHigh%)
	hRunTest%:("doswDlg")
	hCleanUp%:("CleanUp")
ENDP


PROC doswDlg:
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
	REM Text dialog.
	REM First entry.
	hEnter%:
	
	REM Last entry.
	hUp%: :hEnter%:
	
	REM Second entry.
	hDown%: :hEnter%:
	
	REM Mixed dialog.
	hDown%: 
	hSendString&:("03:04a") :hDown%: :REM 03:04am
	hSendString&:("27111993") :hDown%: :REM 27/11/1993
	hSendString&:("Mary had a little lamb") :hDown%:
	hSendString&:("Secret") :hEnter%:

	REMMix2
	hSendString&:("314159265") :hDown%:
	hSendString&:("3.14159265") :hDown%:
	hDown%:
	hSendKey&:(%4) :hEnter%:
	
	REM Button dialog.
	hSendKey&:(4) REM Ctrl+d

	REMSecond button dialog.
	hSendKey&:(%2) :hEsc%:
	
	REM Width test1
	hEnter%:
	
	REMWidth test2
	hEnter%:
ENDP


REM End of swDlg.tpl
