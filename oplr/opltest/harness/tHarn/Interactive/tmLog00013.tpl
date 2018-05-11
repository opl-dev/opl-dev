REM tmLog00013.tpl
REM test Multi log.

REM Demonstrates the harness utilities can be used 
REM to log text while running normally
REM -- that is, not running as a test under the 
REM harness.
REM Note that hCall is NOT used.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNALStandAlone:

PROC StandAlone:
	LOADM KhUtils$
	REM Callback proc must be same as filename.
	hLink:("tmLog00013",	hThreadIdFromOplDoc&:)
	REM After completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP

REM The 'real' Main proc has same name as .opo file

PROC tmLog00013:
	rem hCall0%:("tLog01")
	tLog01:
ENDP

PROC tLog01:
	LOCAL seq%
	hLog%:(KhLogAlways%, "This text is *always* logged.")
	hLog%:(KhLogHigh%, "At level High, this is probably logged a lot.")
	hLog%:(KhLogMed%, "This is medium level logging.")
	hLog%:(KhLogLow%, "A nice quiet low level log.")
	BUSY"Running..."
	seq%=10000
	RANDOMIZE 00013

	rem Breather to start second logger too.
	PAUSE20
	WHILE seq%<10999
		hLog%:(KhLogAlways%,DATIM$+" "+GEN$(seq%,5)+"        ")
rem		PRINTDATIM$,seq%
rem		PAUSE 1+rnd*2
		seq%=seq%+1
	ENDWH
	BUSYOFF
ENDP

REM End of tmLog00013.tpl 
