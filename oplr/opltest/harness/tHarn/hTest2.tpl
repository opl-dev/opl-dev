REM hTest2.tpl

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNALStandAlone:
EXTERNAL hTest2:

PROC StandAlone:
	LOADM KhUtils$
	hLink:("hTest2", hThreadIdFromOplDoc&:)
	REM After completion, control returns here.
	dINIT"Tests complete" :DIALOG
ENDP


REM The 'real' Main proc has same name as .opo file

PROC HTest2:
	hRunTest%:("fred")
	REM Demonstrate bad main.
	hLog%:(KhLogHigh%,"hTest2 is going to RAISE -666 now.")
	rem xxxxx
	RAISE -666
ENDP

PROC Fred:
	RETURN
ENDP

REM End of hTest2.tpl
