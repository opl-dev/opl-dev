REM tAlignIO.tpl
REM Testing aligned IO buffers in EPOC OPL.
REM v0.06

INCLUDE "Const.oph"
rem INCLUDE "LogUtils.tph"

rem LogUtils.tph looks like this***********************************************************************************************
rem LogUtils.tph looks like this***********************************************************************************************
rem LogUtils.tph looks like this***********************************************************************************************
REM LogUtils.tph
REM Include file for Crystal OPL test and log operations.
REM Copyright (C) 1999 Symbian Ltd. All rights reserved.

EXTERNAL Log%:(aThreshold%,aData$)
EXTERNAL LogOpen%:
EXTERNAL LogClose:
EXTERNAL LogChangeThreshold%:(aNewLevel%)

CONST KMaxLenFilename%=255

CONST KLogTestLib$="\Opltest\Lib"	REM Location of test libraries.
CONST KLogTestUtil$="\Opltest\Lib\LogUtils.opo" REM This library!

REM fileLog constants:
CONST KLogName$="C:\Opltest.Log" REM Name of Unicode text log.
CONST KLogLow%=200
CONST KLogMed%=100
CONST KLogHigh%=50
CONST KLogAlways%=0 REM Always log this

REM Should really be in Const.tph
CONST KOplAlignment%=1 REM 1 for Unicode, 0 for ASCII.

REM Some usage notes:

REM Log%:(KLogAlways%,data$) will log the Unicode text 
REM 'data$' to the KLogName$ file, opening the file if it 
REM isn't already open.

REM End of LogUtils.tph
rem *************************************************************************************************************************
rem *************************************************************************************************************************
rem *************************************************************************************************************************




DECLARE EXTERNAL

EXTERNAL testAlignIO:
EXTERNAL doOpen:(aMode%)
EXTERNAL doWrite:
EXTERNAL doRead:
EXTERNAL doClose:
EXTERNAL errorHandler:

CONST KFilename$="C:\testIO.bin"

CONST KTestString$="Hello world"
rem !!TODO this don't work. CONST KTestString$="Hello world"


CONST KTestLong&=31415
CONST KTestInt%=151

CONST KWrite%=KTrue%
CONST KRead%=KFalse%


PROC Main:
	GLOBAL LogHandle%,LogThreshold%
	rem LOADM KLogTestUtil$
	LogOpen%:
	TestAlignIO:
	LogClose: REM xxxx Lose this?
ENDP


PROC TestAlignIO:
	GLOBAL gHandle%

REM ONERR ErrHandler::
	Log%:(KLogLow%,"TestAlignIO: started.")

	doOpen:(KWrite%)
	doWrite:
	doClose:

	doOpen:(KRead%)
	doRead:
	doClose:
	Log%:(KLogLow%,"TestAlignIO: has passed.")
	TRAP DELETE KFilename$
	print "All tests complete"
	print "Press any key" :GET
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
	LOCAL buffer$(KMaxStringLen%),buffer&,buffer%

	buffer$=KTestString$ 
	addr&=ADDR(buffer$)+1+KOplAlignment% :len%=LEN(KTestString$)
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
	LOCAL buffer$(KMaxStringLen%),buffer&,buffer%

	addr&=ADDR(buffer$)+1+KOplAlignment% :len%=LEN(KTestString$)
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	POKEB ADDR(buffer$),ret%

	addr&=ADDR(buffer&) :len%=4
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	addr&=ADDR(buffer%) :len%=2
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF

	IF buffer$<>KTestString$ :
		print "Read [";buffer$;"] Len=";LEN(buffer$)
		print "Expecting [";KTestString$;"] Len=";LEN(KTestString$) 
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
	Log%:(KLogAlways%,"Error: TestAlignIO:"+ERR$(ERR)+" In more detail:"+ERRX$)
ENDP


rem LogUtils.tpl looks like this***********************************************************************************************
rem LogUtils.tpl looks like this***********************************************************************************************
rem LogUtils.tpl looks like this***********************************************************************************************

REM File logging utilities for OPL testing.
REM (c) 1999 Symbian Ltd. All rights reserved.

rem INCLUDE "Const.oph"
REM INCLUDE "LogUtils.tph"
DECLARE EXTERNAL

REM Private functions.
rem EXTERNAL LogConvert$:(aRaw$)

PROC Log%:(aLevel%,aData$)
	EXTERNAL LogHandle%,LogThreshold%
	LOCAL ret%
	LOCAL data$(KMaxStringLen%)

	REM Lose it if its too low...
	IF aLevel%>LogThreshold% :RETURN :ENDIF

rem	data$=LogConvert$:(aData$)
	data$=aData$
	IF LogHandle%=0
		PRINT data$
	ELSE
		ret%=IOWRITE(LogHandle%,ADDR(data$)+1+KOplAlignment%,LEN(data$))
		IF ret%<0
			LogClose:
			Log%:(aLevel%,"Warning: Unable to write to file log, using screen.")
			Log%:(aLevel%,"Log write error='"+ERR$(ret%)+"'")
			Log%:(aLevel%,data$)
		ENDIF
	ENDIF
ENDP


PROC LogOpen%:
	EXTERNAL LogHandle%,LogThreshold%
	LOCAL filename$(KMaxLenFilename%),mode%,ret%
	LogThreshold%=KLogHigh%
	IF EXIST(KLogName$)
		REM Append to any existing log, to keep the previous log in case
		REM a defect isn't easily repeatable.
		mode%=KIoOpenModeAppend% OR KIoOpenFormatText% OR KIoOpenAccessUpdate%
	ELSE
		mode%=KIoOpenModeCreate% OR KIoOpenFormatText% OR KIoOpenAccessUpdate%
	ENDIF	
	ret%=IOOPEN(LogHandle%,KLogName$,mode%)
	IF ret%<0
		LogHandle%=0
		Log%:(KLogAlways%,"Warning: Unable to open filelog '"+KLogName$+"', using screen instead.")
	ENDIF
	Log%:(KLogAlways%,"OPL test log opened at "+DATIM$)
	RETURN LogHandle%
ENDP


PROC LogClose:
	EXTERNAL LogHandle%
	LOCAL ret%
	Log%:(KLogAlways%,"OPL test log closed at "+DATIM$)
	Log%:(KLogAlways%,"---")
	ret%=IOCLOSE(LogHandle%)
	LogHandle%=0
ENDP


PROC LogChangeThreshold%:(aNewLevel%)
	REM Change the threshold level at which log notes are
	REM written to the log file. Returns the old level.
	EXTERNAL LogThresholdLevel%
	LOCAL oldLevel%
	oldLevel%=LogThresholdLevel%
	LogThresholdLevel%=aNewLevel%
	RETURN oldLevel%
ENDP


REM Private functions


PROC LogConvert$:(aRaw$)
	IF MID$(aRaw$,LEN(aRaw$),1)=","
		RETURN aRaw$
	ELSE
		RETURN aRaw$+CHR$(13)
	ENDIF
ENDP

REM End of LogUtils.tpl

rem *************************************************************************************************************************
rem *************************************************************************************************************************
rem *************************************************************************************************************************
