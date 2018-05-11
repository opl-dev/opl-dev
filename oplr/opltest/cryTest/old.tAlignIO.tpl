REM tAlignIO.tpl
REM Testing aligned IO buffers in EPOC OPL.
REM v0.04

INCLUDE "const.oph"
rem INCLUDE "cUtils.tph"

rem cUtils.tph looks like
this****************************************************************************
*******************
rem cUtils.tph looks like
this****************************************************************************
*******************
rem cUtils.tph looks like
this****************************************************************************
*******************
REM cUtils.tph
REM Include file for Crystal OPL test operations.
REM Copyright (C) 1999 Symbian Ltd. All rights reserved.

EXTERNAL cLog:(aThreshold%,aData$)
EXTERNAL cLogOpen%:
EXTERNAL cLogClose:
EXTERNAL cLogChangeThreshold%:

CONST KMaxLenFilename%=255

CONST KcTestLib$="\Opltest\Lib"	REM Location of test libraries.
CONST KcTestUtil$="\Opltest\Lib\cUtils.opo" REM This library!

REM fileLog constants:
CONST KcLogName$="C:\Opltest.Log" REM Name of Unicode text log.
CONST KcLogPriLow%=200
CONST KcLogPriMed%=100
CONST KcLogPriHigh%=50
CONST KcLogPriAlways%=0 REM Always log this

REM Should really be in Const.tph
CONST KOplAlignment%=1 REM 1 for Unicode, 0 for ASCII.

REM Some usage notes:

REM cLog:(data$) will log the Unicode text 'data$' to the KfLogName$
REM file, opening the file if it isn't already open.

REM End of cUtils.tph
rem
********************************************************************************
*****************************************
rem
********************************************************************************
*****************************************
rem
********************************************************************************
*****************************************




DECLARE EXTERNAL

EXTERNAL testAlignIO:
EXTERNAL doOpen:(aMode%)
EXTERNAL doWrite:
EXTERNAL doRead:
EXTERNAL doClose:
EXTERNAL errorHandler:

CONST KFilename$="C:\testIO.bin"

CONST KTestString$="Hello world"
CONST KTestStringLen%=20 REM Should be at least length of TestString$
CONST KTestLong&=31415
CONST KTestInt%=151

CONST KWrite%=KTrue%
CONST KRead%=KFalse%


PROC Main:
	GLOBAL cLogHandle%,cLogThreshold%
	rem LOADM KCTestUtil$
	cLogOpen%:
	TestAlignIO:
	cLogClose: REM xxxx Lose this?
ENDP


PROC TestAlignIO:
	GLOBAL gHandle%

REM ONERR ErrHandler::
	cLog:(KcLogPriLow%,"TestAlignIO: started.\n")

	doOpen:(KWrite%)
	doWrite:
	doClose:

	doOpen:(KRead%)
	doRead:
	doClose:
	cLog:(KcLogPriLow%,"TestAlignIO passed.")
	TRAP DELETE KFilename$
	RETURN
ErrHandler::
	ONERR OFF
	ErrorHandler:
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
ENDP


PROC doWrite:
	EXTERNAL gHandle%
	LOCAL ret%,addr&,len%
	LOCAL buffer$(KTestStringLen%),buffer&,buffer%

	buffer$=KTestString$ 
	addr&=ADDR(buffer$) :len%=KTestStringLen%
	ret%=IOWRITE(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	buffer&=KTestLong&
	addr&=ADDR(buffer&) :len%=4
	ret%=IOWRITE(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	buffer%=KTestInt%
	addr&=ADDR(buffer%) :len%=2
	ret%=IOWRITE(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
ENDP


PROC doRead:
	EXTERNAL gHandle%
	LOCAL ret%,addr&,len%
	LOCAL buffer$(KTestStringLen%),buffer&,buffer%

	addr&=ADDR(buffer$) :len%=KTestStringLen%
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	addr&=ADDR(buffer&) :len%=4
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	addr&=ADDR(buffer%) :len%=2
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	IF buffer$<>KTestString$ :
		print "[";buffer$;"]" :
		print "[";KTestString$;"]" :
		RAISE -1000 :ENDIF
	IF buffer&<>KTestLong& :RAISE -1001 :ENDIF
	IF buffer%<>KTestInt% :RAISE -1002 :ENDIF
ENDP


PROC doClose:
	EXTERNAL gHandle%
	LOCAL ret%
	ret%=IOCLOSE(gHandle%)
	IF ret%<0 :RAISE ret% :ENDIF
ENDP



PROC ErrorHandler:
	cLog:(KcLogPriAlways%,"Error: TestAlignIO:"+ERR$(ERR)+" In more detail:"+ERRX$)
ENDP


rem cUtils.tpl looks like
this****************************************************************************
*******************
rem cUtils.tpl looks like
this****************************************************************************
*******************
rem cUtils.tpl looks like
this****************************************************************************
*******************

REM File logging utilities for OPL testing.
REM (c) 1999 Symbian Ltd. All rights reserved.

rem INCLUDE "Const.oph"
REM INCLUDE "cUtils.tph"
DECLARE EXTERNAL

REM Private functions.
EXTERNAL cLogConvert$:(aRaw$)

PROC cLog:(aLevel%,aData$)
	EXTERNAL cLogHandle%,cLogThreshold%
	LOCAL ret%
	LOCAL data$(KMaxStringLen%)

	REM Lose it if its too low...
	IF aLevel%<cLogThreshold% :RETURN :ENDIF

	data$=cLogConvert$:(aData$)
	IF cLogHandle%=0
		PRINT data$
	ELSE
		ret%=IOWRITE(cLogHandle%,ADDR(data$)+1+KOplAlignment%,LEN(data$))
		IF ret%<0
			cLogClose:
			cLog:(aLevel%,"Warning: Unable to write to file log, using screen.")
			cLog:(aLevel%,"Log write error='"+ERR$(ret%)+"'")
			cLog:(aLevel%,data$)
		ENDIF
	ENDIF
ENDP


PROC cLogOpen%:
	EXTERNAL cLogHandle%
	LOCAL filename$(KMaxLenFilename%),mode%,ret%
	mode%=KIoOpenModeReplace% OR KIoOpenFormatText% OR KIoOpenAccessUpdate%
	ret%=IOOPEN(cLogHandle%,KcLogName$,mode%)
	IF ret%<0
		cLogHandle%=0
		cLog:(KcLogPriAlways%,"Warning: Unable to open filelog, using screen
instead.")
	ENDIF
	RETURN cLogHandle%
ENDP


PROC cLogClose:
	EXTERNAL cLogHandle%
	LOCAL ret%
	ret%=IOCLOSE(cLogHandle%)
	cLogHandle%=0
ENDP


PROC cLogThreshold%:(aNewLevel%)
	EXTERNAL cLogThresholdLevel%
	LOCAL oldLevel%
	oldLevel%=cLogThresholdLevel%
	cLogThresholdLevel%=aNewLevel%
	RETURN oldLevel%
ENDP

REM Private functions

PROC cLogConvert$:(aRaw$)
	LOCAL convert$(KMaxStringLen%)
	LOCAL esc%

	RETURN aRaw$


	esc%=LOC(aRaw$,"\")
	IF esc%=0
		RETURN aRaw$
	ENDIF

	REM p1=start of string.
	DO
		REM copy pre block
		REM write convert \+next
		REM locate next
	UNTIL 0
	RETURN convert$
ENDP

REM End of cUtils.tpl

rem
********************************************************************************
*****************************************
rem
********************************************************************************
*****************************************
rem
********************************************************************************
*****************************************
