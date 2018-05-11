REM hTest3.tpl

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNALStandAlone:
EXTERNAL Badness:

PROC StandAlone:
	LOADM KhUtils$
	hLink:("hTest3", hThreadIdFromOplDoc&:)
	REM After completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


REM The 'real' Main proc has same name as .opo file

PROC HTest3:
	hRunTest%:("Jim")
ENDP

PROC Jim:
	hLog%:(KhLogHigh%, "Demonstrates that hTest2 did not stop further execution in this folder.")
ENDP

REM End of hTest3.tpl 
