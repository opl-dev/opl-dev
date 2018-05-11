REM spdEdMulti.tpl
REM EPOC OPL script for interactive testing of pdEdMulti.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL 

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "System.oxh"

EXTERNAL KeyScript%:
EXTERNAL Keys01%:
EXTERNAL Keys02%:
EXTERNAL KeysNegative%:
EXTERNAL SpoofSanityCheck%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("spdEdMulti", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem	dINIT "Tests complete" :DIALOG
ENDP

CONST kTarget$="\Opltest\Interactive\pMainI\pdEdMulti.opo"

PROC spdEdMulti:
	hRunTest%:("spdEdMulti01")
	hCleanUp%:("CleanUp")
ENDP


PROC spdEdMulti01:
	LOCAL thread&
	thread&=hSpoofRunTargetApp&:("OPL", "", KSyRunAppOpl$+hDiskName$:+KTarget$,KSyRunAppRun%)
	hSpoofSetFlagTargetApp%:(KTarget$,KFalse%)
	rem print thread&
	KeyScript%:
	PAUSE KhCloseAppDelay%
	IF hSpoofGetFlagTargetApp%:(KTarget$) :RAISE 31313 :ENDIF
ENDP


PROC CleanUp:
	hSpoofCloseApp%:
ENDP


PROC KeyScript%:
	REM Alloced dialogs.
	Keys01%:
	Keys02%:

	REM Array dialogs.
	Keys02%:
	
	REM Negative tests.
	KeysNegative%:
ENDP


PROC Keys01%:
	REM 100 char multi-line edit.
	hSendString&:("The quick brown fox jumped over the lazy dog. ")
	hSendString&:("The quick brown fox jumped over the lazy dog. ")
	hSendString&:("The quic") :hEnter%:
ENDP


PROC Keys02%:
	REM Alphabet in lower-case.
	hSendString&:("abcdefghijklmnopqrstuvwxyz") :hEnter%:

	REM Alphabet in upper-case.
	hSendString&:("ABCDEFGHIJKLMNOPQRSTUVWXYZ") :hEnter%:

	REM Same buffer as before.
	hEnter%:

	REM Set buffer.
	hEnter%:

	REM Cancel.
	hEsc%:

	REM Small.
	hSendString&:("Boat") : hEnter%:
ENDP



PROC KeysNegative%:
	REM 36
	REM            12345678901234567890123
	hSendString&:("Mary had a little lamb.")
	hSendString&:("Mary had a li.")
	hEnter%:

	REM 4
	hSendString&:("Crow") : hEnter%:

	REM 
	hEnter%:

	hEnter%:

	REM
	hSendString&:("Alphabet soup") :	hEnter%:

	REM 36
	REM            12345678901234567890123
	hSendString&:("Mary had a little lamb.")
	hSendString&:("Mary had a li.")
	hEnter%:

	REM 4
	hSendString&:("Crow") : hEnter%:

	REM 
	hEnter%:

	hEnter%:
ENDP



PROC SpoofSanityCheck%:
	dinit "Some sanity check"
	dialog
ENDP


REM End of spdEdMulti.tpl
