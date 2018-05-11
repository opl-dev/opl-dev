REMWriter.tpl
REMEPOC OPL automatic test code for xxxx
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE"hUtils.oph"
declare external
external script%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("Writer", hThreadIdFromOplDoc&:) rem same name as file.
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc Writer:
	hLogChangeThreshold%:(KhLogHigh%)

	hCall%:("doSend")
	hCleanUp%:("Cleanup")
endp

CONST kTarget$="\Opltest\Harness\Event\Reader.opo"


PROC doSend:
	LOCAL thread&

	thread&=hSpoofRunTargetApp&:("OPL", "", "R"+hDiskName$:+KTarget$,khRunAppRun%)
	rem print thread&
	rem dinit "The target app will be dead":dialog

	rem !!TODO Adjust these figures.
	pause 20


	Script%:
ENDP


PROC CleanUp:
	REM !!todo Ensure the target is closed.
	dinit "About to cleanup" :dialog
	hSpoofCloseApp%:
ENDP


PROC Script%:
	REM Send the keys.
	LOCAL count&
	
	hSendKey&:(&406)
	hSendKey&:(%m)
	hSendKey&:(&407)
	
	hEsc%:

ENDP




REM End
