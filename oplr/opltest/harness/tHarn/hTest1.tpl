REM hTest.tpl
REM Utils for testing/logging EPOC OPL.
REM v0.14

DECLARE EXTERNAL

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNALStandAlone:
EXTERNAL hTest:
EXTERNAL myTestInit:
EXTERNAL	tER4:
EXTERNAL	tER5oneBad:
EXTERNAL	tER6:
EXTERNAL	EekDoesNotExist:
EXTERNAL	tER7oneMissing:
EXTERNAL	tLast:

REM Main procedure for standalone testing.
REM (Not called when executed from test harness.)

PROC StandAlone:
	LOADM KhUtils$
	hLink:("hTest1",hThreadIdFromOplDoc&:)

	REM After completion, control returns here.
	dINIT "Tests complete"
	DIALOG
ENDP


REM The 'real' Main proc has same name as .opo file

PROC hTest1:
	REM Any local testing initialisations...
	myTestInit:
	tER4:
	tER5oneBad:
	tER6:

	REM Calling a proc which doesn't exist will stop
	REM the harness when running standalone (so the
	REM test author can fix the bug), but will not
	REM prevent a suite of tests running -- rather, the
	REM error will be logged and the harness will skip
	REM to the next test.
	EekDoesNotExist:

	tER7oneMissing:
	tLast:
ENDP


PROC MyTestInit:
	rem hLogChangeThreshold%:(KhLogMed%):
ENDP


PROC tER4:
	hRunTest%:("Work01")
	hRunTest%:("Work02")
ENDP

PROC tER5onebad:
	hRunTest%:("Work03")
	hRunTest%:("Fail01")
	hRunTest%:("Work04")
ENDP

PROC tER6:
	hRunTest%:("Work05")
	hRunTest%:("Work06")
ENDP

PROC tER7oneMissing:
	hRunTest%:("ThisProcIsMissing")
	hRunTest%:("Work08")
ENDP

PROC tLast:
	hRunTest%:("Work09")
	hRunTest%:("Work10")
ENDP


CONST KFilename$="C:\testIO.bin"

CONST KTestString$="Hello world"
CONST KTestLong&=31415
CONST KTestInt%=151

CONST KWrite%=KTrue%
CONST KRead%=KFalse%

EXTERNAL tCTest01:
EXTERNAL tCTest02:
EXTERNAL tCTest03:
EXTERNAL tCTest04:

EXTERNAL doOpen:(aMode%)
EXTERNAL doWrite:
EXTERNAL doRead:
EXTERNAL doClose:
EXTERNAL errorHandler:



PROC Work01:
	GLOBAL gHandle%
	hLog%:(KhLogLow%,"TestAlignIO: started.")

	doOpen:(KWrite%)
	doWrite:
	doClose:

	doOpen:(KRead%)
	doRead:
	doClose:
	
	hLog%:(KhLogLow%,"Test procedure TestAlignIO: has passed.")
	TRAP DELETE KFilename$
ENDP


PROC doOpen:(aMode%)
	EXTERNAL gHandle%
	LOCAL ret%,mode%
	LOCAL name$(255)
	
	name$=KFilename$
	IF aMode%=KWrite%
		mode%=KIoOpenModeReplace% OR KIoOpenAccessUpdate%
	ELSE
		mode%=KIoOpenModeOpen% OR KIoOpenAccessShare%
	ENDIF
	mode%=mode% OR KIoOpenFormatBinary%
	ret%=IOOPEN(gHandle%,name$,mode%)
	IF ret%<0 :RAISE ret% :ENDIF
	hLog%:(KhLogLow%,name$+" opened.")
ENDP


PROC doWrite:
	EXTERNAL gHandle%
	LOCAL ret%,addr&,len%
	LOCAL buffer$(KMaxStringLen%),buffer&,buffer%

	buffer$=KTestString$ 
	addr&=ADDR(buffer$)+1+KAlignment% :len%=LEN(KTestString$)
	ret%=IOWRITE(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
	hLog%:(KhLogLow%, "Written "+buffer$+" to test file.")

	buffer&=KTestLong&
	addr&=ADDR(buffer&) :len%=4
	ret%=IOWRITE(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
	hLog%:(KhLogLow%, "Written "+GEN$(buffer&,8)+" to test file.")

	buffer%=KTestInt%
	addr&=ADDR(buffer%) :len%=2
	ret%=IOWRITE(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
	hLog%:(KhLogLow%, "Written "+GEN$(buffer%,8)+" to test file.")
ENDP


PROC doRead:
	EXTERNAL gHandle%
	LOCAL ret%,addr&,len%
	LOCAL buffer$(KMaxStringLen%),buffer&,buffer%

	addr&=ADDR(buffer$)+1+KAlignment% :len%=LEN(KTestString$)
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
	POKEB ADDR(buffer$),ret%
	hLog%:(KhLogLow%,"Read "+buffer$+" from test file.")

	addr&=ADDR(buffer&) :len%=4
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
	hLog%:(KhLogLow%,"Read "+GEN$(buffer&,8)+" from test file.")

	addr&=ADDR(buffer%) :len%=2
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
	hLog%:(KhLogLow%,"Read "+GEN$(buffer%,8)+" from test file.")

	IF buffer$<>KTestString$ :
		print "Read [";buffer$;"] Len=";LEN(buffer$)
		print "Expecting [";KTestString$;"] Len=";LEN(KTestString$) 
		RAISE -1000
	ENDIF
	IF buffer&<>KTestLong& :RAISE -1001 :ENDIF
	IF buffer%<>KTestInt% :RAISE -1002 :ENDIF
ENDP


PROC doClose:
	EXTERNAL gHandle%
	LOCAL ret%
	ret%=IOCLOSE(gHandle%)
	IF ret%<0 :RAISE ret% :ENDIF
	hLog%:(KhLogLow%,"Test file closed.")
ENDP


PROC Work02:
ENDP

PROC Work03:
ENDP

PROC Work04:
ENDP

PROC Work05:
ENDP

PROC Work06:
ENDP

PROC Fail01:
	LOCAL a
	hLog%:(KhLogLow%,"Fail01: started.")
	REM Nice 'divide by zero' coming right up...
	a=10/0
	hLog%:(KhLogLow%,"Fail01: completed.")
ENDP

PROC Work08:
ENDP

PROC Work09:
ENDP

PROC Work10:
	hLog%:(KhLogLow%,"Work10: started.")
	rem ...
	hLog%:(KhLogLow%,"Work10: completed.")
ENDP

REMEnd of hTest1.tpl
