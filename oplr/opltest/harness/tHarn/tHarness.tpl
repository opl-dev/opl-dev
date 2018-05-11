REM tHarness.tpl

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNALStandAlone:

PROC StandAlone:
	LOADM KhUtils$
	REM Callback proc must be same as filename.
	hLink:("tHarness", hThreadIdFromOplDoc&:)
	REM After completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


REM The 'real' Main proc has same name as .opo file

PROC tHarness:
	hRunTest%:("LogLevels")
ENDP

PROC LogLevels:
	hLog%:(KhLogAlways%, "This text is *always* logged.")
	hLog%:(KhLogHigh%, "At level High, this is probably logged a lot.")
	hLog%:(KhLogMed%, "This is medium level logging.")
	hLog%:(KhLogLow%, "A nice quiet low level log.")
ENDP

REM End of tHarness.tpl 
