REM tLog.tpl

REM Demonstrates the harness utilities can be used 
REM to log text while running normally -- that is, 
REM not running as a test under the harness.

REM Note that hRunTest is NOT used.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNALStandAlone:

PROC StandAlone:
	LOADM KhUtils$
	REM Callback proc must be same as filename.
	hLink:("tLog", hThreadIdFromOplDoc&:)
	REM After completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


REM The 'real' Main proc has same name as .opo file

PROC tLog:
	rem hCall0%:("tLog01")
	tLog01:
ENDP

PROC tLog01:
	hLog%:(KhLogAlways%, "This text is *always* logged.")
	hLog%:(KhLogHigh%, "At level High, this is probably logged a lot.")
	hLog%:(KhLogMed%, "This is medium level logging.")
	hLog%:(KhLogLow%, "A nice quiet low level log.")

	REM This should stop this program running.
	RAISE0548
ENDP

REM End of tLog.tpl 
