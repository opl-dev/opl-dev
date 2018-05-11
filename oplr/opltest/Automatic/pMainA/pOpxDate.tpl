REM pOpxDate.tpl
REM EPOC OPL automatic test code for Date OPX.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "date.oxh"

declare external

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pOpxDate", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pOpxDate:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tOpxDate")
rem	hCleanUp%:("CleanUp")
endp


proc tOpxDate:
	local start&,end&
	local y&,m&,d&,h&,mi&,s&,micro&,i&

	DTSetHomeTime:(DTNow&:)
	start&=DTNow&:

	print"Year: ";DTYear&:(start&);"Month: ";DTMonth&:(start&);"Day: ";DTDay&:(start&)
	print DTHour&:(start&);":";DTMinute&:(start&);":";DTSecond&:(start&);":";DTMicro&:(start&)

	rem print "double click the space bar"
	start&=DTNow&:
	pause rnd*40
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
	rem get
	cls

	print "days in this month: "; DTDaysInMonth&:(DTNow&:)
	print "day no in week (1-7): "; DTDayNoInWeek&:(DTNow&:)
	print "day no in year test (should say 1): "; DTDayNoInYear&:(DTNow&:,DTNow&:)

	print "press a key"
	rem get
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

	rem print"COMPLETE"
	rem print "finish"
	rem get
endp

REM End of pOpxDate.tpl

