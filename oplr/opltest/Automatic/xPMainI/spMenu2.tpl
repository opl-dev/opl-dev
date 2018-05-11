REM spMenu2.tpl
REM EPOC OPL script for Automatic testing of pMenu2.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL 

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "System.oxh"

EXTERNAL KeyScript%:
EXTERNAL SpoofSanityCheck%:
EXTERNAL EasyEnter%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("spMenu2", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem	dINIT "Tests complete" :DIALOG
ENDP

CONST kTarget$="\Opltest\Interactive\pMainI\pMenu2.opo"


PROC spMenu2:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("spMenu02")
	hCleanUp%:("CleanUp")
ENDP


PROC spMenu02:
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
	REM Send the keys.
	
REM First menu card.
	REM File | New
	EasyEnter%:

	REM File | Open
	EasyEnter%:

	REM File | More | Save
	EasyEnter%:
	EasyEnter%:

	REM File | Printing | Page setup
	EasyEnter%:
	EasyEnter%:

	REM File | Close
	EasyEnter%:

	REM File | New
	EasyEnter%:

	REM File | New (again)
	EasyEnter%:
	
REM Second card.
	REM Edit | Cut
	EasyEnter%:

	REM Edit | Copy
	EasyEnter%:

	REM Edit | Paste
	EasyEnter%:

	REM Edit | Find | Find next
	EasyEnter%:
	EasyEnter%:

	REM Edit | Cut
	EasyEnter%:

	REM Let those keypresses trickle through...
	PAUSE 30
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


REM End of spGetEvA.tpl
