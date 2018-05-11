REM spMenu.tpl
REM EPOC OPL script for interactive testing of pMenu.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL 

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "System.oxh"

EXTERNAL CascScript%:
EXTERNAL DimScript%:
EXTERNAL CheckboxScript%:
EXTERNAL OptionScript%:
EXTERNAL ComboScript%:
EXTERNAL SpoofSanityCheck%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("spMenu", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem	dINIT "Tests complete" :DIALOG
ENDP

CONST kTarget$="\Opltest\Interactive\pMainI\pMenu.opo"


PROC spMenu:
	hRunTest%:("spMenu01")
	hCleanUp%:("CleanUp")
ENDP


PROC spMenu01:
	LOCAL thread&
	thread&=hSpoofRunTargetApp&:("OPL", "", KSyRunAppOpl$+hDiskName$:+KTarget$,KSyRunAppRun%)
	hSpoofSetFlagTargetApp%:(KTarget$,KFalse%)
	CascScript%:
	DimScript%:
	CheckboxScript%:
	OptionScript%:
	ComboScript%:		
	PAUSE KhCloseAppDelay%
	IF hSpoofGetFlagTargetApp%:(KTarget$) :RAISE 31313 :ENDIF
ENDP


PROC CleanUp:
	hSpoofCloseApp%:
ENDP


PROC CascScript%:
	REM Send the keys.
	LOCAL count&

	REM CASCADE
	REM File new.
	hEnter%:
	PAUSE KhUIDelay%

	REM Open.
	hDown%:		:hEnter%:
	PAUSE KhUIDelay%

	REM Casc.
	hRight%: :hDown%: :hDown%: :hDown%: :hRight%: :hDown%: :hEnter%:
	PAUSE KhUIDelay%
	
	REM Extra>
	hRight%: :hRight%: :hRight%: :hDown%: :hEnter%:
	PAUSE KhUIDelay%
	
	REM No shortcut
	hRight%: :hRight%: :hDown%: :hEnter%: 
	PAUSE KhUIDelay%
	
	REM Missing>
	hDown%: :hDown%: :hRight%: :hEnter%: 
	PAUSE KhUIDelay%
ENDP


PROC DimScript%:
	REM Dim entry.
	hEnter%: REM No response here.
	PAUSE KhUIDelay%
	
	REM Dim casc.
	hDown%: :hDown%: :hRight%: :hEnter%: REM No response.
	PAUSE KhUIDelay%
	
	REM Exit!
	hDown%: :hDown%: :hEnter%: 
	PAUSE KhUIDelay%
ENDP


PROC CheckboxScript%:
	REM Select Carriage Return.
	hDown%: :hEnter%:
	PAUSE KhUIDelay%
ENDP


PROC OptionScript%:
	REM Select Roman.
	hDown%: :hEnter%:
	PAUSE KhUIDelay%
ENDP


PROC ComboScript%:
	REM Select Helvetica.
	hDown%: :hDown%:
	hRight%: :hRight%: :hRight%: :hRight%: 
	hDown%: :hDown%:
	hEnter%:
	PAUSE KhUIDelay%
ENDP


PROC SpoofSanityCheck%:
	dinit "Some sanity check"
	dialog
ENDP


REM End of spGetEvA.tpl
