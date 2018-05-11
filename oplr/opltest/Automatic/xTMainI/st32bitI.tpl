REM st32bitI.tpl
REM EPOC OPL script for interactive t32bitI.
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
	hLink:("st32bitI", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


CONST kTarget$="\Opltest\Interactive\tMain\t32bitI.opo"


PROC st32bitI:
rem	hLogChangeThreshold%:(KhLogHigh%)
	hRunTest%:("dost32bitI")
	hCleanUp%:("CleanUp")
ENDP


PROC dost32bitI:
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
	hSendString&:("12345") :hEnter%:
	hSendString&:("12345") :hEnter%:
	hSendString&:("12345") :hEnter%:
	hSendString&:("12345") :hEnter%:

	REM Menu selection.
	hDown%: :hDown%: :hEnter%:
	
	REM Dialog.
	REM choice=c
	hSendKey&:(%c) :hDown%: 

	REM time=01:02:03
	hSendString&:("01:02:03") :hDown%: 
	
	REM edit=hello
	hSendString&:("hello") :hDown%: 

	REM secret edit=pass
	hSendString&:("pass") :hDown%: 
	
	REM dfile=abc
	hSendString&:("abc") :hDown%: :hDown%: :hDown%: 
	
	REM dlong=-32769, with a nice rich Enter to close the dialog.
	hSendString&:("-32769")
	hSendKey&:(KEvKeyDown&) :hEnter%: :hSendKey&:(KEvKeyUp&)

	REM Getevent keys.
	REM x
	hSendKey&:(KEvKeyDown&) :hSendKey&:(%x) :hSendKey&:(KEvKeyUp&)

	REM x 
	hSendKey&:(KEvKeyDown&) :hSendKey&:(%x) :hSendKey&:(KEvKeyUp&)

	REM and x again.
	hSendKey&:(KEvKeyDown&) :hSendKey&:(%x) :hSendKey&:(KEvKeyUp&)
ENDP


REM End of st32bitI.tpl
