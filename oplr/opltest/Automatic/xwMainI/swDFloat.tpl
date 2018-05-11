REM swDFloat.tpl
REM EPOC OPL script for interactive wDFloat.
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
	hLink:("swDFloat", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\wMain\wDFloat.opo"


PROC swDFloat:
	hRunTest%:("doswDFloat")
	hCleanUp%:("CleanUp")
ENDP


PROC doswDFloat:
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
	REM Old float limits.
	hSendString&:("9.999e99") :hDown%:
	hSendString&:("-1e-99") :hDown%:
	hSendString&:("-9.999e99") :hDown%:
	hSendString&:("9.999e99") :hDown%:
	hSendString&:("1e99") :hDown%:
	hSendString&:("0") :hEnter%:

	REM More float limits.
	hSendString&:("1.797e308") :hDown%:
	hSendString&:("-4.999e-324") :hDown%:
	hSendString&:("-1.797e308") :hDown%:
	hSendString&:("1.797e308") :hDown%:
	hSendString&:("4.999e-324") :hDown%:
	hSendString&:("0") :hEnter%:

	REM float3
	hSendString&:("3.14") :hEnter%:

	REM And again.
	hSendString&:("-1e100") :hDown%:
	hSendString&:("-9.999e-120") :hDown%:
	hSendString&:("1e100") :hDown%:
	hSendString&:("9.999e120") :hDown%:
	hSendString&:("-1e-100") :hDown%:
	hSendString&:("-1e-121") :hEnter%:

	hSendString&:("1e-100") :hDown%:
	hSendString&:("1e-121") :hEnter%:

	hSendString&:("10") :hEnter%:
	hSendString&:("10") :hEnter%:
ENDP


REM End of swDFloat.tpl
