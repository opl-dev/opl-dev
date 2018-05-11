rem	tdate.tpl
rem 	This code tests date.opx 

INCLUDE "const.oph"
INCLUDE "date.oxh"

proc tdate:
	local start&,end&
	local y&,m&,d&,h&,mi&,s&,micro&,i&
	local gfiletime&
	local sfiletime&

testDateOPX101:

	DTSetHomeTime:(DTNow&:)
	start&=DTNow&:

	print"Year: ";DTYear&:(start&);"Month: ";DTMonth&:(start&);"Day: ";DTDay&:(start&)
	print DTHour&:(start&);":";DTMinute&:(start&);":";DTSecond&:(start&);":";DTMicro&:(start&)

	print "double click the space bar"
	get
	start&=DTNow&:
	get
	end&=DTNow&:
	print"start "; DTHour&:(start&);":";DTMinute&:(start&);":";DTSecond&:(start&);":";DTMicro&:(start&)
	print "end ";DTHour&:(end&);":";DTMinute&:(end&);":";DTSecond&:(end&);":";DTMicro&:(end&)

	DTDateTimeDiff:(start&,end&,y&,m&,d&,h&,mi&,s&,micro&)
	print "Diff=","y=";y&,"m=";m&,"d=";d&,h&;":";mi&;":";s&;":";micro&
	print

	print "Diffences in,"
	print " Years: ";DTYearsDiff&:(start&,end&);" Months: ";DTMonthsDiff&:(start&,end&);" Days: ";DTDaysDiff&:(start&,end&)
	print " Hours: ";DTHoursDiff&:(start&,end&);" Minutes: ";DTMinutesDiff&:(start&,end&);" Seconds: ";DTSecsDiff&:(start&,end&)
	print " Microseconds: ";DTMicrosDiff&:(start&,end&)
	print "press a key"
	get
	cls

	print "days in this month: "; DTDaysInMonth&:(DTNow&:)
	print "day no in week (1-7): "; DTDayNoInWeek&:(DTNow&:)
	print "day no in year test (should say 1): "; DTDayNoInYear&:(DTNow&:,DTNow&:)

	print "press a key"
	get
	cls
	print "further datatime object testing"

	DTDeleteDatetime:(start&)
	DTDeleteDatetime:(end&)

	start&=DTnewdatetime&:(1974,4,27,8,45,23,567567)
	end&=DTnewdatetime&:(2000,6,7,3,4,3,348974)
	DTSetyear:(end&,1974)
	DTSetmonth:(end&,4)
	DTSetday:(end&,27)
	DTSethour:(end&,8)
	DTSetminute:(end&,45)
	DTSetsecond:(end&,23)
	DTSetmicro:(end&,567567)
	DTDateTimeDiff:(start&,end&,y&,m&,d&,h&,mi&,s&,micro&)
	if y&<>0
		raise 1
	endif
	if m&<>0
		raise 2
	endif
	if d&<>0
		raise 3
	endif
	if h&<>0
		raise 4
	endif
	if mi&<>0
		raise 5
	endif
	if s&<>0
		raise 6
	endif
	if micro&<>0
		raise 7
	endif

	DTDeleteDatetime:(start&)
	DTDeleteDatetime:(end&)

	print"COMPLETE"

	print "testing get/set file time"
	sfiletime&=DTNewdatetime&:(1989,1,1,1,1,1,0)
	gfiletime&=DTNewdatetime&:(1990,2,2,2,2,2,0)
	trap delete"ppppppp"
	create"ppppppp",a,h&
	append
	close
	DTsetfiletime:("ppppppp",sfiletime&)
	DTfiletime:("ppppppp",gfiletime&)

	if  dtyear&:(gfiletime&) <> 1989
		raise 7
	endif
	if  dtday&:(gfiletime&) <> 1
		raise 8
	endif
	if  dtmonth&:(gfiletime&) <> 1
		raise 9
	endif
	if  dthour&:(gfiletime&) <> 1
		raise 10
	endif
	if  dtminute&:(gfiletime&) <> 1
		raise 11
	endif
	if   dtsecond&:(gfiletime&) <> 1
	print"get set file time secs bug"	rem raise 12
	endif
	if  dtmicro&:(gfiletime&) <> 0
		raise 13
	endif
	print "All Clear - press a key to continue"
get
cls


	print "finish"
	get
endp

proc testDateOPX101:
	PRINT "Date OPX v1.01 testing"
rem	ONERR DateError::
	Month101:
	Micro101:
	Sec101:
	Min101:
	Hour101:
	SetDay101:
	SetHour101:
	SetMin101:
	SetSec101:
	SetMicro101:
	print "Passed"
	print
	RETURN

DateError::
	ONERR OFF
	PRINT CHR$(7)
	PRINT "Date 1.01 OPX tests FAILED!"
	PRINT
	GET
ENDP


PROC Month101:
	LOCAL dateh&
	PRINT "Testing Month bug:",
	ONERR EHandler::
	REM Will fail "out of range" at runtime with Date 1.00
	dateh&=DtNewDateTime&:(1998,5,31,0,0,0,0) REM A valid date.
	ONERR OFF
	PRINT "Passed"
	RETURN

EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Failed with Out Of Range bug"
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
	ENDIF
	RAISE ERR
ENDP


PROC Micro101:
	LOCAL dateh&
	PRINT "Testing Microsecond bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,0,0,0,1000000) REM invalid microsecond.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 500
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC Sec101:
	LOCAL dateh&
	PRINT "Testing Second bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,0,0,60,0) REM invalid second.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 501
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC Min101:
	LOCAL dateh&
	PRINT "Testing Minute bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,0,60,0,0) REM invalid minute.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 502
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC Hour101:
	LOCAL dateh&
	PRINT "Testing Hour bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,24,0,0,0) REM invalid hour.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 503
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP


PROC SetDay101:
	LOCAL dateh&
	PRINT "Testing SetDay bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetDay:(dateh&,31) REM 31st May is a valid date.
	ONERR OFF
	PRINT "Passed"
	RETURN
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Failed with Out Of Range error"
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
	ENDIF
	RAISE ERR
ENDP

PROC SetHour101:
	LOCAL dateh&
	PRINT "Testing SetHour bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetHour:(dateh&,24) REM 24th hour should be invalid.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 504
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC SetMin101:
	LOCAL dateh&
	PRINT "Testing SetMinute bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetMinute:(dateh&,60) REM 60th minute is invalid.
	ONERR OFF
	PRINT "Failed - an error should have been reported"
	RAISE 505
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC SetSec101:
	LOCAL dateh&
	PRINT "Testing SetSecond bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetSecond:(dateh&,60) REM 60th second is invalid.
	ONERR OFF
	PRINT "Failed - an error should have been reported"
	RAISE 506
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC SetMicro101:
	LOCAL dateh&
	PRINT "Testing SetMicro bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetMicro:(dateh&,1000000) REM 1 million microseconds is invalid.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 507
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP


