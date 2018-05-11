ALARM.OPX v6.00
===============
Contents:

1.	Introduction
2.	File Details
3.	Installation
4.	Using the OPX
5.	Contents of Alarm.oxh
6.	Procedure Reference
7.	Copyright
8.	Distributing the OPX

INTRODUCTION
------------
This OPX gives you access to a number of functions of the ALARM server, to 
enable you to access them from within a OPL program.

FILE DETAILS
------------
The archive consists of the following files:

README.TXT	This file
ALARM.SIS	The main OPX file in SIS format
ALARMW.SIS	The WINS build of the OPX in SIS format
ALARM.TXH	The OPL header file in text file format
DALARM.TPL	A demonstration program that shows you how the OPX can be used

INSTALLATION
------------
1. Install ALARM.SIS

2. Create ALARM.OXH into the \System\OPL\ folder on either the C: or D: 
drive. This may be done by either creating a new 'Program' file called
ALARM.OXH and then using the Import text option on ALARM.TXH, or by
making use of OPLTRAN -CONV (see the OPL SDK for details).

3. Create DALARM.OPL from DALARM.TPL anywhere you like.

USING THE OPX
-------------
1. First translate and run the DALARM.OPL to make sure everything works correctly.

2. To use the OPX in your program add the following line to the top of the 
code, immediately after the APP...ENDA and before the first procedure

	INCLUDE "ALARM.OXH"

3. You can now use the Alarm.opx procedures in your program.

CONTENTS OF ALARM.OXH
---------------------
CONST KAlarmClockOnce&=0
CONST KAlarmClockNext24hours&=1
CONST KAlarmClockDaily&=2
CONST KAlarmClockWeekly&=3
CONST KAlarmClockWorkday&=4

CONST KAlarmNotSet&=0
CONST KAlarmSet&=1
CONST KAlarmDisabled&=2

CONST KAlarmSoundOn&=0
CONST KAlarmSoundOff&=1
CONST KAlarmQuietPeriod&=2

CONST KAlarmSilenced$=""
CONST KAlarmSilent$="Silent"

CONST KMaxAlarmMessage%=$80
CONST KMaxAlarmSoundName%=$20

CONST KMaxCityName%=25
CONST KMaxCountryName%=25
CONST KMaxAreaCode%=10
CONST KMaxNationalCode%=10
CONST KMaxNationalPrefixCode%=10
CONST KMaxInternationalPrefixCode%=10
CONST KMaxWorldLatitudeMinutes&=5400	rem 90*60
CONST KMaxWorldLongitudeMinutes&=10800	rem 180*60
CONST KMaxWorldUniversalTimeOffset&=780	rem 13*60 (in minutes)

CONST KDaylightSavingZoneNone%=0
CONST KDaylightSavingZoneEuropean%=1
CONST KDaylightSavingZoneNorthern%=2
CONST KDaylightSavingZoneSouthern%=4

DECLARE OPX ALARM,KOpxAlarmUid&,KOpxAlarmVersion%
	AlmSetClockAlarm:(anAlarmNumber&,aDateTime&,aMessage$,aSound$,aRepeat&) : 1
	AlmAlarmState&:(anAlarmNumber&) : 2
	AlmAlarmEnable:(aEnable&, anAlarmNumber&) : 3
	AlmAlarmDelete:(anAlarmNumber&) : 4
	AlmQuietPeriodCancel: : 5
	AlmQuietPeriodUntil:(aDateTime&) : 6
	AlmQuietPeriodSet:(aMinutes&) : 7
	AlmSetAlarmSound:(aState&) : 8
	AlmAlarmSoundState&: : 9
	
	WldFindCity:(aSearchCity$, aCallback$) : 10
	WldSunlight:(aSearchCity$, theTimeOfSunrise&, theTimeOfSunset&, aDate&) : 11
	WldHome$: : 12
	WldPreviousCity$:(aSearchCity$) : 13
	WldPreviousCountry$:(aSearchCountry$) : 14
	WldNextCity$:(aSearchCity$) : 15
	WldNextCountry$:(aSearchCountry$) : 16

	WldAddCity:(aCityName$, aCountryName$, aMinsLat&, aMinsLong&, aAreaCode$, aDaylightZone%, aGMTOffset&, aMapX%, aMapY%) : 17
	WldAddCountry:(aCountryName$, aNationalCode$, aNationalPrefix$, aInternationalPrefix$, aCapitalCity$, aMinsLat&, aMinsLong&, aAreaCode$, aDaylightZone%, aGMTOffset&, aMapX%, aMapY%) : 18
	WldEditCity:(aCityToEdit$, aCountryToEdit$, aCityName$, aCountryName$, aMinsLat&, aMinsLong&, aAreaCode$, aDaylightZone%, aGMTOffset&, aMapX%, aMapY%) : 19
	WldEditCountry:(aCountryToEdit$, aCountryName$, aNationalCode$, aNationalPrefix$, aInternationalPrefix$, aCapitalCity$) : 20
	WldDeleteCity:(aCityName$, aCountryName$) : 21
	WldDeleteCountry:(aCountryName$) : 22
	WldNumberOfCities&: : 23
	WldNumberOfCountries&: : 24
	WldDataFileSave: : 25
	WldDataFileRevert: : 26
END DECLARE

AlmSetClockAlarm:
-----------------
Usage: AlmSetClockAlarm:(anAlarmNumber&,aDateTime&,aMessage$,aSound$,aRepeat&) 

aAlarmNumber& is the slot that appears in the Time application and may 
take values from 0-7. 

aDateTime& is the address of a DateTime object created by a call to 
DTNewDateTime&: or DTNow&: from DATE.OPX.

aMessage$ is the text which is to appear on screen when the alarm 
triggers.

aSound$ is the sound to be played when the alarm triggers. It may be 
one of the built in sounds, or the name of a file which appears in 
\System\Alarms.

NOTE:	To create a silent alarm, aSound$ be set to "Silent" (case
	is not important) or "" (empty string).

aRepeat& is the repeat mode, chosen from the following: KAlarmClockOnce&, 
KAlarmClockNext24hours&, KAlarmClockDaily&, KAlarmClockWeekly&, 
KAlarmClockWorkday&.

AlmAlarmState&:
---------------
Usage: alarmstate = AlmAlarmState&:(anAlarmNumber&)

Returns the state of one of the alarms. Returns one of the following 
constants: KAlarmNotSet&, KAlarmSet& or KAlarmDisabled&.

AlmAlarmEnable: 
---------------
Usage: AlmAlarmEnable:(aState&, anAlarmNumber&) 

Enables or disables alarm anAlarmNumber&. aState should be set to KTrue% to 
enable, or KFalse% to disable.

AlmAlarmDelete: 
---------------
Usage: AlmAlarmDelete:(anAlarmNumber&) 

Deletes the alarm anAlarmNumber&.

AlmQuietPeriodCancel: 
---------------------
Usage: AlmQuietPeriodCancel: 

Cancels any previously set quiet period.

AlmQuietPeriodUntil: 
--------------------
Usage: AlmQuietPeriodUntil:(aDateTime&) 

Sets the alarm server to go quiet until a specified date and time. aDateTime& 
is the address of a DateTime object created by a call to DTNewDateTime&: from 
DATE.OPX.

AlmQuietPeriodSet: 
------------------
Usage: AlmQuietPeriodSet:(aMinutes&) 

Sets the alarm to go quiet for a specified number of minutes.

AlmSetAlarmSound: 
-----------------
Usage: AlmSetAlarmSound:(aState&) 

Switches the sound for all alarms on or off. aState should be set to 
KTrue% to switch alarm sounds on, or KFalse% to switch them off.

AlmAlarmSoundState&:
--------------------
Usage: soundstate& = AlmAlarmSoundState&:

Returns the state of one of the alarms. Returns one of the following 
constants: KAlarmSoundOn&, KAlarmSoundOff& or KAlarmQuietPeriod&.

WldFindCity: 
------------
Usage: WldFindCity:(aSearchCity$, aCallback$) 

Accesses the world server to find a city, and allows access to various details 
of that city. To use this call, you must first define an OPL procedure with 
the parameters show here:

	PROC CityCallback&:(aCityName$, aCountry$, aLatitudeInMinutes&, 
aLongitudeInMinutes&, anArea$, aUniversalTimeOffsetInMinutes&, 
aDaylightSavingZone&, aNatCode$, aCapital$, aNatpref$, anIntlpref$)

A call to WldFindCity will call the function specified in aCallback$ for each 
city in the world database which starts with the letters in aSearchCity$.

Searching can be stopped at a particular city by passing back KTrue% from the 
callback procedure or continued by passing KFalse%.

WldSunlight:
------------
Usage: WldSunlight:(aSearchCity$, theTimeOfSunrise&, theTimeOfSunset&, aDate&)

Gets the sunrise/sunset times for a city at a given day. The date required is 
created as a DateTime object using DTNewDateTime&: or DTNow&: from DATE.OPX.

As a special case, today may be specified by passing 0 in aDate&.

Sunrise and sunset times are returned in DateTime objects too. It's important 
to create these objects first using DTNewDateTime&: or DTNow&: from DATE.OPX, 
as WldSunlight will not create them, rather it fills values into an already 
created object. 

WldPreviousCity$:
-----------------
Usage: city$ = WldPreviousCity$:(aSearchCity$)

Moves to the previous city in the world database which starts with the letters 
in aSearchCity$.

WldPreviousCountry$:
--------------------
Usage: country$ = WldPreviousCountry$:(aSearchCountry$)

Moves to the next country in the world database which starts with the letters 
in aSearchCountry$.

WldNextCity$:
-------------
Usage: city$ = WldNextCity$:(aSearchCity$)

Moves to the next city in the world database which starts with the letters in 
aSearchCity$.

WldNextCountry$:
----------------
Usage: country$ = WldNextCountry$:(aSearchCountry$)

Moves to the next country in the world database which starts with the letters 
in aSearchCountry$.

WldAddCity:
-----------
Usage: WldAddCity:(aCityName$, aCountryName$, aMinsLat&, aMinsLong&, aAreaCode$,
									aDaylightZone%, aGMTOffset&, aMapX%, aMapY%)

Allows you to add a city to the World database. City/Country names are 25
characters maximum. When adding a city, you must add it to a country which already
exists or you will get an error.

aMapX% and aMapY% specify the coordinates for the where the cross appears on
the world map for this city.

WldAddCountry:
--------------
Usage: WldAddCountry:(aCountryName$, aNationalCode$, aNationalPrefix$,aInternationalPrefix$,
							aCapitalCity$, aMinsLat&, aMinsLong&, aAreaCode$, aDaylightZone%,
																aGMTOffset&, aMapX%, aMapY%)

Allows you to add a new country to the World databse.

WldEditCity:
------------
Usage: WldEditCity:(aCityToEdit$, aCountryToEdit$, aCityName$, aCountryName$, aMinsLat&,
					aMinsLong&, aAreaCode$, aDaylightZone%, aGMTOffset&, aMapX%, aMapY%)

Allows you to edit a city in the World database. The city to edit is identified uniquely
by the combination of aCityToEdit$ and aCountryToEdit$ (i.e. you specify the name and the
country of the city you wish to edit).

New names are passed in aCityName$/aCountryName$ and other new info in the arguments
which follow those two.

WldEditCountry:
---------------
Usage: WldEditCountry:(aCountryToEdit$, aCountryName$, aNationalCode$, aNationalPrefix$,
													aInternationalPrefix$, aCapitalCity$)

Allows you to edit a country in the World databse. The country to edit is identified by
the name passed in aCountryToEdit$

A new name may be passed in aCountryName$ and other new info in the arguments
which follow that.

WldDeleteCity:
--------------
Usage: WldDeleteCity:(aCityName$, aCountryName$)

Deletes a city from the World database.

WldDeleteCountry:
-----------------
Usage: WldDeleteCountry:(aCountryName$)

Deletes a country from the World database.

WldNumberOfCities&:
-------------------
Usage: Number = WldNumberOfCities&:

Returns the number of cities currently in the World database.

WldNumberOfCountries&:
----------------------
Usage: Number = WldNumberOfCountries&:

Returns the number of countries currently in the World database.

WldDataFileSave:
----------------
Usage: WldDataFileSave:

Saves any changes you have made to the World database. If you use any of the above
add/edit/delete functions then changes will be reflected in RAM only. If you reset
the EPOC device, these changes will be lost.

Calling WldDataFileSave: causes any changes to be saved to the World database in
the file c:\System\Data\Wld_data.dbw

WldDataFileRevert:
------------------
Usage: WldDataFileRevert:

Reverts the World database to a state before any changes have been made - i.e.
to the state before you last called WldDataFileSave: or saved changes via the
Time/World application itself.

NOTE: If no RAM-based data file is present then there is, effectively, no file
to be reverted so this function will raise a 'Does not exist' error under
those circumstances.

COPYRIGHT
---------
Alarm.opx is Copyright (c) 1997-2000 Symbian Ltd. All rights reserved. It 
forms part of the OPL SDK and is subject to the License contained therein.

DISTRIBUTION 
------------ 
Alarm.opx should only be distributed to end users in one of the SIS files 
included in this release. This ensures that older versions will not be 
installed over newer versions without warning.

The best way to do this is to include the Alarm SIS within your applications 
PKG file. This means you can distribute your application as a single SIS file.

For MARM distributions use this line in your PKG file:
@"Alarm.sis",(0x10004EC8)

For WINS distributions use this line: 
@"AlarmW.sis",(0x10004EC8)

Revision history
----------------
Version 6.00
- First release for Crystal v6.00 DFRD (based on ER5 v5.10 Alarm.opx)