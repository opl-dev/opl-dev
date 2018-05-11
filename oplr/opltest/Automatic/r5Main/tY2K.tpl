REM ty2k.tpl
REM EPOC OPL automatic test code for Y2K and later date checks.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "date.oxh"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "ty2k", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc ty2k:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("doY2K")
	rem hCleanUp%:("CleanUp")
endp



PROC doY2k:
	GLOBAL gyear%,gmonth%,gday%,ghour%,gminute%,gsecond%
	GLOBAL gres1&,gres3&,gres5&
	GLOBAL gDateId&

	LOCAL year%(6),month%(6),day%(6), hour%(6), minute%(6), second%(6)
	LOCAL res1&(6),res3&(6),res5&(6)
	LOCAL count%

	REM Set up the test date array.
	year%(1)=1999 :year%(2)=2000 :year%(3)=2000 :year%(4)=2000 :year%(5)=2004 :year%(6)=2004
	month%(1)=12  :month%(2)=1   :month%(3)=2   :month%(4)=2   :month%(5)=2  :month%(6)=2
	day%(1)=31    :day%(2)=1     :day%(3)=28    :day%(4)=29    :day%(5)=28    :day%(6)=29
	hour%(1)=23   :hour%(2)=0    :hour%(3)=23   :hour%(4)=0    :hour%(5)=23   :hour%(6)=0
	minute%(1)=59 :minute%(2)=0  :minute%(3)=59 :minute%(4)=0  :minute%(5)=59 :minute%(6)=0
	second%(1)=59 :second%(2)=0  :second%(3)=59 :second%(4)=0  :second%(5)=59 :second%(6)=0

	REM Expected results
	REM Seconds since 00:00 1/1/1970
	res1&(1)=946684799 :res1&(2)=res1&(1)+1 :res1&(3)=951782399 :res1&(4)=res1&(3)+1 :res1&(5)=1078012799 :res1&(6)=res1&(5)+1

	REM days since 1/1/1900
	res3&(1)=36523 :res3&(2)=res3&(1)+1 :res3&(3)=36582 :res3&(4)=res3&(3)+1 :res3&(5)=38043 :res3&(6)=res3&(5)+1

	REM Day of week (1=monday)
	res5&(1)=5 :res5&(2)=6 :res5&(3)=1 :res5&(4)=2 :res5&(5)=6 :res5&(6)=7
	
	FONT 12,0
	PRINT "Year 2000 testing for OPL"
	
	REM Run through the tests twice; once with the system
	REM clock set before 1/1/2000, and once after.

	count%=1	
	DO
		gyear%=year%(count%)
		gmonth%=month%(count%)
		gday%=day%(count%)
		ghour%=hour%(count%)
		gminute%=minute%(count%)
		gsecond%=second%(count%)
		gres1&=res1&(count%)
		gres3&=res3&(count%)
		gres5&=res5&(count%)

		PRINT
		PRINT "Test date is " + TestDate$:

		Test1:
		Test3:
		Test5:
		Test6:

		Test8:

		REM Create id.
		gDateId&=DtNewDateTime&:(gyear%,gmonth%,gday%,ghour%,gminute%,gsecond%,0)

		Test9:
		Test10:
		REM Kill it.
		DtDeleteDateTime:(gDateId&)

		count%=count%+1
	UNTIL count%>6
ENDP



PROC ASSERT:(e%)
	IF e%
		RETURN 0
	ENDIF
	RETURN -7 REM "Out of range"
rem 	dINIT "Test failed!"
ENDP


PROC TestDate$:
	RETURN f2$:(gHour%)+":"+f2$:(gMinute%)+":"+f2$:(gSecond%)+" "+f2$:(gDay%)+"/"+f2$:(gMonth%)+"/"+GEN$(gYear%,4)
ENDP


PROC f2$:(val%)
	RETURN RIGHT$("0"+GEN$(val%,2),2)
ENDP


PROC Test1:	REM DATETOSECS
	LOCAL s&, err%
	PRINT "DATETOSECS()"
	s&=DATETOSECS(gyear%,gmonth%,gday%,ghour%,gminute%,gsecond%)
	err%=Assert:(s&=gres1&)
	IF err%
		RAISE err%
	ENDIF
ENDP



PROC Test3:	REM DAYS
	LOCAL s&,err%
	PRINT "DAYS()"
	s&=DAYS(gday%,gmonth%,gyear%)
	err%=Assert:(s&=gres3&)
	IF err%
		RAISE err%
	ENDIF
ENDP



PROC Test5:	REM DOW
	LOCAL s&,err%
	PRINT "DOW()"
	s&=DOW(gday%,gmonth%,gyear%)
	err%=Assert:(s&=gres5&)
	IF err%
		RAISE err%
	ENDIF
ENDP


PROC Test6:	REM SECSTODATE
	LOCAL s&, err%, res%
	LOCAL ryear%, rmonth%, rday%, rhour%, rminute%, rsecond%
	LOCAL ignore%
	s&=gres1&
	PRINT "SECSTODATE()"
	SECSTODATE s&, ryear%,rmonth%,rday%,rhour%,rminute%,rsecond%,ignore%
	res%=(ryear%=gyear%)+(rmonth%=gmonth%)+(rday%=gday%)+(rhour%=ghour%)+(rminute%=gminute%)+(rsecond%=gsecond%)
	err%=Assert:(res%=-6)
	IF err%
		RAISE err%
	ENDIF
ENDP


PROC Test8:	REM DAYSTODATE
	LOCAL s&, err%, res%
	LOCAL ryear%, rmonth%, rday%
	s&=gres3&
	PRINT "DAYSTODATE()"
	DAYSTODATE s&, ryear%,rmonth%,rday%
	res%=(ryear%=gyear%)+(rmonth%=gmonth%)+(rday%=gday%)
	err%=Assert:(res%=-3)
	IF err%
		RAISE err%
	ENDIF
ENDP


PROC Test9:	REM DtYear&:
	LOCAL s&,err%
	PRINT "DtYear&:()"
	s&=DtYear&:(gDateId&)
	err%=Assert:(s&=gyear%)
	IF err%
		RAISE err%
	ENDIF
ENDP


PROC Test10:	REM DtSetYear
	LOCAL s&,err%
	PRINT "DtSetYear:()"
	DtSetYear:(gDateId&,gYear%)
	REM Now read it back.
	s&=DtYear&:(gDateId&)
	err%=Assert:(s&=gyear%)
	IF err%
		RAISE err%
	ENDIF
ENDP

REM End.
