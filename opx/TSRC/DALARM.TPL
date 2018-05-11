REM DALARM.OPL
REM
REM Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
REM
REM Demo code for Alarm OPX

!!!!!!!!!!!! THIS PROBABLY WON'T TRANSLATE - well, not now with this line, anyway :-) - but we
!!!!!!!!!!!! need to check the logic is OK as I've just bolted on the 5.10 'extra' test code
!!!!!!!!!!!! at the start...

rem DECLARE EXTERNAL - only EXTERNALs for the 5.10 extra tests so far

INCLUDE "DATE.OXH"
INCLUDE "ALARM.OXH"

EXTERNAL ShowNumbersInDB:
EXTERNAL TestAddCity:
EXTERNAL TestAddCountry:
EXTERNAL TestEditCity:
EXTERNAL TestEditCountry:
EXTERNAL TestDeleteCity:
EXTERNAL TestDeleteCountry:
EXTERNAL TestDataFileSave:
EXTERNAL TestDataFileRevert:
EXTERNAL AddTempTestCity:
EXTERNAL TestSilentAlarm:
EXTERNAL Ooops:

CONST KTempTestCityName$="AAAAAA"

PROC Main:
	local k$(1)


	DBManipulationTests:

	print "This rest of this program allows you to set and test"
	print "clock alarms on your machine. There are eight such"
	print "alarms, numbered 0 to 7."
	do
		DisplayState:
		Print "What would you like to do?"
		print "Set an (A)larm, set (Q)uiet time, (E)nable or (D)isable Alarm?"
		print "Or maybe (C)ancel an alarm, or (c)ancel a quiet time?"
		print "Turn (o)n the sound, or turn it of(f)?"
		print "Or would you rather search the (W)orld database?"
		print "Perhaps you are interested in Sun(L)ight calculations?"
		print "Or maybe just e(x)it?"
		k$=get$
		if k$="A" or k$="a"
			SetAlarm:
		elseif k$="Q" or k$="q"
			SetQuietTime:
		elseif k$="E" or k$="e"
			EnableAlarm:(&1)
		elseif k$="D" or k$="d"
			EnableAlarm:(&0)
		elseif k$="C"
			DeleteAlarm:
		elseif k$="c"
			CancelQuiet:
		elseif k$="O" or k$="o"
			SetSound:(&1)
		elseif k$="F" or k$="f"
			SetSound:(&0)
		elseif k$="W" or k$="w"
			SearchWorld:
		elseif k$="L" or k$="l"
			Sunlight:
		endif
	until k$="X" or k$="x"			
ENDP

PROC SetAlarm:
	local alarmNo&, dtime&, msg$(KMaxAlarmMessage%), sound$(KMaxAlarmSoundName%), repeat&
	local Y&, M&, D&, Hour&, Min&
	print "Which alarm number (0 - 7) :"		
	input alarmNo&
	print "Year: ";
	input Y&
	print "Month: ";
	input M&
	print "Day (of Month): ";
	input D&
	print "Hour: ";
	input Hour&
	print "Minute: ";
	input Min&
	dtime&=DTNewDateTime&:(Y&,M&,D&,Hour&,Min&, 0, 0)
	print "Would you like to have this alarm:"
	print "  (1) Once only"
	print "  (2) In the next 24 hours"
	print "  (3) Daily"
	print "  (4) Weekly"
	print "  (5) On workdays?"
	input repeat&
	repeat&=repeat& - 1
	print "Message to be shown at alarm time: ";
	input msg$
	print "Name of sound to be played (e.g. ""Rings"", ""Fanfare""): ";
	input sound$
	AlmSetClockAlarm:(alarmNo&, dtime&, msg$, sound$, repeat&)
	DTDeleteDateTime:(dtime&)
	print "Alarm",alarmNo&,"has been set."
ENDP

PROC DisplayState:
	local i&, state&, dtime&
	print "Your home city is",WldHome$:
	i&=0
	while i& < 8
		print "Alarm",i&,
		state&=AlmAlarmState&:(i&)
		if state&=KAlarmNotSet&
			print "is not set"
		ELSEIF state&=KAlarmSet&
			print "is set"
		ELSEIF state&=KAlarmDisabled&
			print "is set but disabled"
		ENDIF
		i&=i& + 1
	endwh
	state&=AlmAlarmSoundState&:
	if state&=KAlarmSoundOn&
		print "Alarm sound is on"
	elseif state&=KAlarmSoundOff&
		print "Alarm sound is off"
	elseif state&=KAlarmQuietPeriod&
		dtime&=DTNow&:
		AlmQuietPeriodUntil:(dtime&)
		print "A quiet period is in effect until",DTHour&:(dtime&);":";DTMinute&:(dtime&)
		DTDeleteDateTime:(dtime&)
	endif
ENDP

PROC SetQuietTime:
	local minutes&
	print "How long (in minutes): ";
	input minutes&
	AlmQuietPeriodSet:(minutes&)
ENDP

PROC EnableAlarm:(onoff&)
	local alarmNumber&
	print "Which Alarm (0-7): ";
	input alarmNumber&
	AlmAlarmEnable:(onoff&, alarmNumber&)
ENDP

PROC DeleteAlarm:
	local alarmNumber&
	print "Delete which Alarm (0-7): ";
	input alarmNumber&
	AlmAlarmDelete:(alarmNumber&)
ENDP

PROC CancelQuiet:
	AlmQuietPeriodCancel:
ENDP

PROC SetSound:(onoff&)
	AlmSetAlarmSound:(onoff&)
ENDP

rem Now this is somewhat tricky:
rem WldFindCity searches the World database for the first
rem city starting with the given prefix.
rem It then calls a function with
rem the given name, with the arguments as shown below.
rem The function must return a long integer (&-type)
rem If it returns zero, the next city will be considered,
rem until the end of the database
rem If it returns anything else, the search ends.

PROC SearchWorld:
	global city$(100), country$(100)
	print "Enter the start of the cityname : ";
	input city$
	WldFindCity:(city$, "CityCallback")
	print "Citysearch finished"
	city$ = WldNextCity$:(city$)
	print "Next city:", city$
	city$ = WldNextCity$:(city$)
	print "Next city:", city$
	city$ = WldPreviousCity$:(city$)
	print "Previous city:", city$
	print "Enter a countryname : ";
	input country$
	country$ = WldNextCountry$:(country$)
	print "Next country:", country$
	country$ = WldNextCountry$:(country$)
	print "Next country:", country$
	country$ = WldPreviousCountry$:(country$)
	print "Previous country:", country$
ENDP

rem NOTE: Name of procedure must end in &
rem But don't include the & in the string in the call
rem to WldFindCity
rem lat& itude and long& itude are in minutes (of an arc)
rem time difference is in minutes (of time)

PROC CityCallback&:(cityName$, country$, lat&, long&, area$, timediff&, dst&, natcode$, capital$, natpfx$, intpfx$)
	if left$(cityName$, len(city$))=city$
		print cityName$;" at (";lat&;",";long&;") time difference =",
		print timediff&,"Daylight savings time =",dst&
		print "  To dial there:", natcode$,area$, "to dial out:",natpfx$,"to dial international:",intpfx$
		print "  In",country$,"with capital",capital$
		return 0
	else
		return -1
	endif
ENDP

PROC Sunlight:
	local k$(1),sunrise&, sunset&, city$(100), theday&, month&, day&
	sunrise&=DTNow&: : sunset&=DTNow&: : theday&=DTNow&:
	print "Sunlight calculation for which city: ";
	input city$
	print "Today? (Y/N) ";
	k$=get$
	if k$="Y" or k$="y"
		WldSunlight:(city$, sunrise&, sunset&, &0)
	else
		print "Month: ";
		input month&
		print "Day: ";
		input day&
		theday&=DTNewDateTime&:(1998, month&, day&, 12, 0, 0, 0)
		WldSunlight:(city$, sunrise&, sunset&, theday&)
	endif
	print "Sunrise at",DTHour&:(sunrise&);":";DTMinute&:(sunrise&)
	print "Sunset at",DTHour&:(sunset&);":";DTMinute&:(sunset&)
	DTDeleteDateTime:(sunrise&)
	DTDeleteDateTime:(sunset&)
	DTDeleteDateTime:(theday&)
ENDP

PROC DBManipulationTests:
	TestAddCity:
	TestAddCountry:
	TestEditCity:
	TestEditCountry:
	TestDeleteCity:
	TestDeleteCountry:
	TestDataFileSave:
	TestDataFileRevert:
	TestSilentAlarm:
	PRINT "Database Manipulation Tests Complete. Press a key."
	GET
ENDP

PROC ShowNumbersInDB:
	PRINT "Number of cities in database:",WldNumberOfCities&:
	PRINT "Number of countries in database:",WldNumberOfCountries&:
ENDP

PROC TestAddCity:
	ShowNumbersInDB:
	PRINT "Testing WldAddCity:"
	PRINT "Adding 'Test City' in Germany"
	WldAddCity:("Test City","Germany",46,-44,"69",KDaylightSavingZoneEuropean%,1,66,77)
	
	PRINT "Trying to add it again"
	ONERR A1::
	WldAddCity:("Test City","Germany",46,-44,"69",KDaylightSavingZoneEuropean%,1,66,77)
	Ooops:
	GOTO A1A::
	A1::
	PRINT "-->FAILED to add same city twice"
	A1A::
	ONERR OFF

	PRINT "Trying to add same name, other details"
	ONERR A2::
	WldAddCity:("Test City","France",41,-34,"12",KDaylightSavingZoneNorthern%,4,50,60)
	PRINT "-->PASSED adding same city, other details OK"
	GOTO A2A::
	A2::
	PRINT "-->FAILED to add same city name but with other details"
	A2A::
	ONERR OFF

	PRINT "Trying to add city with same name/country as a ROM city"
	ONERR A3::
	WldAddCity:("Oxford","United Kingdom",46,-44,"69",KDaylightSavingZoneEuropean%,1,66,77)
	Ooops:
	GOTO A3A::
	A3::
	PRINT "-->FAILED to add city with same name/country as a ROM one"
	A3A::
	ONERR OFF

	PRINT "Trying to add city with same name, different country as a ROM city"
	ONERR A4::
	WldAddCity:("Oxford","USA",46,-44,"69",KDaylightSavingZoneEuropean%,1,66,77)
	PRINT "-->PASSED adding same city name, different country as a ROM city OK"
	GOTO A4A::
	A4::
	PRINT "-->FAILED to add city with same name, different country as a ROM one"
	A4A::
	ONERR OFF

	ShowNumbersInDB:
	PRINT "Add city tests completed. Press a key"
	GET
	CLS
ENDP

PROC TestAddCountry:
	ShowNumbersInDB:
	PRINT "Testing WldAddCountry:"
	PRINT "Adding 'Symbian Land' with capital 'Sentinel House'"
	WldAddCountry:("Symbian Land","0","1","2","Sentinel House",44,44,"44",KDaylightSavingZoneEuropean%,0,144,81)
	
	PRINT "Trying to add it again"
	ONERR A1::
	WldAddCountry:("Symbian Land","0","1","2","Sentinel House",44,44,"44",KDaylightSavingZoneEuropean%,0,144,81)
	Ooops:
	GOTO A1A::
	A1::
	PRINT "-->FAILED to add same country twice"
	A1A::
	ONERR OFF

	PRINT "Trying to add same name, other details"
	ONERR A2::
	WldAddCountry:("Symbian Land","3","4","5","Harcourt Street",33,33,"33",KDaylightSavingZoneSouthern%,1,6,99)
	Ooops:
	GOTO A2A::
	A2::
	PRINT "-->FAILED to add same country name but with other details"
	A2A::
	ONERR OFF

	PRINT "Trying to add country with same name/capital as a ROM city"
	ONERR A3::
	WldAddCountry:("USA","0","1","2","Washington, DC",44,44,"44",KDaylightSavingZoneEuropean%,0,144,81)
	Ooops:
	GOTO A3A::
	A3::
	PRINT "-->FAILED to add country with same name/capital as a ROM one"
	A3A::
	ONERR OFF

	PRINT "Trying to add country with same name, different capital as a ROM city"
	ONERR A4::
	WldAddCountry:("USA","0","1","2","London Inner",44,44,"44",KDaylightSavingZoneEuropean%,0,144,81)
	Ooops:
	GOTO A4A::
	A4::
	PRINT "-->FAILED to add country with same name, different capital as a ROM one"
	A4A::
	ONERR OFF

	ShowNumbersInDB:
	PRINT "Add country tests completed. Press a key"
	GET
	CLS
ENDP

PROC TestEditCity:
	ShowNumbersInDB:
	PRINT "Testing WldEditCity:"
	PRINT "Trying to edit the city we added earlier ('Test City'/'Germany')"
	WldEditCity:("Test City","Germany","Test-city","Germany",8,8,"13",KDaylightSavingZoneEuropean%,2,15,15)

	PRINT "Trying to edit a ROM city"
	ONERR A1::
	WldEditCity:("Aachen","Germany","AachenAachen","Germany",1,-1,"11",KDaylightSavingZoneEuropean%,1,66,77)
	PRINT "-->PASSED editing a ROM city OK"
	GOTO A1A::
	A1::
	PRINT "-->FAILED to edit a ROM city"
	A1A::
	ONERR OFF

	PRINT "Trying to edit a city which doesn't exist"
	ONERR A2::
	WldEditCity:("IDoNotExist","NeitherDoI","NowIExist?","SoDoI?",1,-1,"11",KDaylightSavingZoneEuropean%,1,66,77)
	Ooops:
	GOTO A2A::
	A2::
	PRINT "-->FAILED to edit a non-existant city"
	A2A::
	ONERR OFF

	ShowNumbersInDB:
	PRINT "Edit city tests completed. Press a key"
	GET
	CLS
ENDP

PROC TestEditCountry:
	ShowNumbersInDB:
	PRINT "Testing WldEditCountry:"
	PRINT "Trying to edit the country we added earlier ('Symbian Land'-->'Symbian Country')"
	WldEditCountry:("Symbian Land","Symbian Country","0","1","2","Sentinel House")

	PRINT "Trying to edit a ROM country"
	ONERR A1::
	WldEditCountry:("Germany","Deutschland","8","7","13","Berlin")
	PRINT "-->PASSED editing a ROM country OK"
	GOTO A1A::
	A1::
	PRINT "-->FAILED to edit a ROM country"
	A1A::
	ONERR OFF

	PRINT "Trying to edit a country which doesn't exist"
	ONERR A2::
	WldEditCountry:("IDoNotExist","ButNowIDo?","8","8","13","AnywhereButHome")
	Ooops:
	GOTO A2A::
	A2::
	PRINT "-->FAILED to edit a non-existant country"
	A2A::
	ONERR OFF

	ShowNumbersInDB:
	PRINT "Edit country tests completed. Press a key"
	GET
	CLS
ENDP

PROC TestDeleteCity:
	ShowNumbersInDB:
	PRINT "Testing WldDeleteCity:"
	PRINT "Trying to delete the city we added earlier ('Test-City'/'Deutschland')"
	WldDeleteCity:("Test-City","Deutschland")
	PRINT "Trying to delete the city we added earlier ('Test City'/'France')"
	WldDeleteCity:("Test City","France")
	PRINT "Trying to delete the city we added earlier ('Oxford'/'USA')"
	WldDeleteCity:("Oxford","USA")

	PRINT "Trying to delete a city which doesn't exist"
	ONERR A2::
	WldDeleteCity:("NonExistantCity","Germany")
	Ooops:
	GOTO A2A::
	A2::
	PRINT "-->FAILED to delete the non-existant city"
	A2A::
	ONERR OFF

	PRINT "Trying to delete a ROM city"
	ONERR A3::
	WldDeleteCity:("Aachen","Germany") rem Sorry, Aachen, nothing personal!
	Ooops:
	GOTO A3A::
	A3::
	PRINT "-->FAILED to delete the ROM city"
	A3A::
	ONERR OFF

	ShowNumbersInDB:
	PRINT "Delete city tests completed. Press a key"
	GET
	CLS	
ENDP

PROC TestDeleteCountry:
	ShowNumbersInDB:
	PRINT "Testing WldDeleteCountry:"
	PRINT "Trying to delete the country we added earlier ('Symbian Country')"
	WldDeleteCountry:("Symbian Country")

	PRINT "Trying to delete a country which doesn't exist"
	ONERR A2::
	WldDeleteCountry:("NonExistantCountry")
	Ooops:
	GOTO A2A::
	A2::
	PRINT "-->FAILED to delete the non-existant country"
	A2A::
	ONERR OFF

	PRINT "Trying to delete a ROM country"
	ONERR A3::
	WldDeleteCountry:("France") rem Sorry, France, nothing personal!
	Ooops:
	GOTO A3A::
	A3::
	PRINT "-->FAILED to delete the ROM country"
	A3A::
	ONERR OFF

	ShowNumbersInDB:
	PRINT "Delete country tests completed. Press a key"
	GET
	CLS	
ENDP

PROC TestDataFileSave:
	ONERR NoFilePresentYet::
	WldDataFileRevert: rem Remove any stuff added already in prior PROCs
	NoFilePresentYet::
	ONERR OFF
	
	ShowNumbersInDB:
	PRINT "Testing WldDataFileSave:"
	PRINT "Adding a temp test city"
	AddTempTestCity:
	ShowNumbersInDB:

	PRINT "Now calling the save function:"
	WldDataFileSave:
	ShowNumbersInDB:
	PRINT "Now calling the revert function (ensure numbers are the same):"
	WldDataFileRevert:
	ShowNumbersInDB:

	PRINT "Now deleting the test city (and re-saving):"
	WldDeleteCity:(KTempTestCityName$,"USA")
	WldDataFileSave:
	ShowNumbersInDB:

	PRINT "Data File Save test completed. Press a key"
	GET
	CLS
ENDP

PROC TestDataFileRevert:
	ShowNumbersInDB:
	PRINT "Testing WldDataFileRevert:"
	PRINT "Adding a temp test city"
	AddTempTestCity:
	ShowNumbersInDB:

	PRINT "Now calling the revert function:"
	WldDataFileRevert:
	ShowNumbersInDB:
	PRINT "Data File Revert test completed. Press a key"
	GET
	CLS
ENDP

PROC AddTempTestCity:
	WldAddCity:(KTempTestCityName$,"USA",55,-55,"04",KDaylightSavingZoneEuropean%,1,90,80)
ENDP

CONST KTestAlarms%=6

PROC TestSilentAlarm:
LOCAL DTime&,H&(KTestAlarms%),M&(KTestAlarms%),i%,Snd$(KTestAlarms%,KMaxAlarmSoundName%)
	PRINT "Testing some silent and 'normal' alarms (";KTestAlarms%,"in total)"
	Snd$(1)=KAlarmSilenced$
	Snd$(2)="Silent"
	Snd$(3)="Chimes"
	Snd$(4)="Rings"
	Snd$(5)="D:\System\Alarms\Big Ben"
	Snd$(6)="Fanfare"
	i%=1
	DO
		H&(i%)=HOUR :M&(i%)=MINUTE+i%
		IF M&(i%)>59
			M&(i%)=0
			H&(i%)=H&(i%)+1
			IF H&(i%)>23
				H&(i%)=0
			ENDIF
		ENDIF
		DTime&=DTNewDateTime&:(YEAR,MONTH,DAY,H&(i%),M&(i%),0,0)
		AlmSetClockAlarm:(i%-1,DTime&,"Test alarm "+NUM$(i%,9),Snd$(i%),KAlarmClockOnce&)
		DTDeleteDateTime:(DTime&)
		i%=i%+1
	UNTIL i%=KTestAlarms%+1
	PRINT "Alarm tests completed. Press a key"
	GET
	CLS
ENDP

PROC Ooops:
	PRINT "*********************************************"
	PRINT "*Errm, that seems to have worked. Oh dear...*"
	PRINT "*********************************************"
	GET
ENDP