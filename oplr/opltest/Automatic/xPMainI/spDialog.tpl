REM spdialog.tpl
REM EPOC OPL script for interactive testing of pdialog.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL 

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "System.oxh"

EXTERNAL DialogScript%:
EXTERNAL SpoofSanityCheck%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("spDialog", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem	dINIT "Tests complete" :DIALOG
ENDP

CONST kTarget$="\Opltest\Interactive\pMainI\pDialog.opo"

PROC spDialog:
	rem hLogChangeThreshold%:(KhLogHigh%)
	hRunTest%:("spDialog01")
	hCleanUp%:("CleanUp")
ENDP


PROC spdialog01:
	LOCAL thread&

	thread&=hSpoofRunTargetApp&:("OPL", "", KSyRunAppOpl$+hDiskName$:+KTarget$,KSyRunAppRun%)
	rem print thread&
	rem dinit "The target app will be dead":dialog

	hSpoofSetFlagTargetApp%:(KTarget$,KFalse%)
	DialogScript%:
	PAUSE KhCloseAppDelay%
	IF hSpoofGetFlagTargetApp%:(KTarget$) :RAISE 31313 :ENDIF
ENDP


PROC CleanUp:
rem	dinit "waiting to cleanup" : dialog
	hSpoofCloseApp%:
ENDP


PROC DialogScript%:
	REM Cover the screen.
	hEnter%:
	
	REM Buttons.
	hEnter%:

	REM Buttons at right.
	hEnter%:

	REM Too wide.
	hEnter%:

	REM No labels.
	hEsc%:

	REM Buttons at right,label
	hEnter%:

	REM Incorrect labelling.
	hEnter%:
		
	REM Plain.
	hEnter%:

	REM Right plain.
	hEnter%:

	REM No labels,plain.
	hEsc%:
	
	REM dFlags1
	hEnter%:
	
	REM dFlags, empty body.
	hEnter%:

	REM dFlags, prompt, body.
	hEnter%:

	REM untitled.
	hEnter%:

	REM 
	hSendString&:("Sunday") : hEnter%:

	PAUSE KhUIDelay%

	REM Not.
	hEnter%:

	REM Empty.
	hEnter%:
	
	REM Full-screen.
	hSendString&:("Fullscreen") : hDown%:
	hEnter%: REM Pick up the default Change CBA button
	hDown%: REM Down to choice 2
	hEnter%: REM And select it.
	hUp%: REM Back to the edit line.
	hEsc%: REM And out.
	PAUSE KhUIDelay%

	REM bottom right.
	hEnter%:
 	
	REM top left.
	hEnter%:
	PAUSE KhUIDelay%

	REM Too long.
	hEnter%:

	REM Buttons at bottom.
	hEnter%:

	REM Dragging.
	hEnter%:
	PAUSE KhUIDelay%

	REM No dragging.
	hEnter%:

	REM @tDense
	REM Normal.
	hEnter%:
	
	REM Dense.
	hEnter%:
	PAUSE KhUIDelay%

	REM Not dense.
	hEnter%:
	
	REM Dense.
	hEnter%:
	PAUSE KhUIDelay%

	REM @tCombine.
	REM Full screen, buttons at right.
	hEnter%:

	REM No title, buttons at right.
	hEnter%:
	PAUSE KhUIDelay%

	REM No title, no drag.
	hEnter%:

	REM @tMultiChoice.
	REM dChoice.
	hEsc%:
	PAUSE KhUIDelay%
	
	REM Different prompts.
	hEsc%:

	REM Missing...
	hEsc%:

	REM @tCheckBox.
	REM Checkbox.
	hEsc%:
	PAUSE KhUIDelay%

	REM Very wide - Checkbox.
	hEsc%:
ENDP


PROC SpoofSanityCheck%:
	dinit "Some sanity check"
	dialog
ENDP


REM End of spDialog.tpl
