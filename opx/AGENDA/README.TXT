// README.TXT
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

AGENDA.OPX
==========

Contents:

1.	Introduction
2.	File Details
3.	Installation
4.	Using the OPX
5.  Contents of Alarm.oxh
6.  Procedure Reference
7.  Copyright
8.	Distributing the OPX


INTRODUCTION
------------

This OPX gives you access to a number of functions of the AGENDA engine, to 
enable you to access them from within a OPL program. 

Note that there is a separate AGNOPX.OPX supplied by Symbian in the SDK which 
allows day entries to be written to an Agenda. This newer OPX provides the 
same capabilities but in addition supports all other agenda entry types, i.e. 
day entries, events, to-dos, anniversaries and to-do lists.

WARNING: If you don't write your OPL code correctly to use the supplied OPX it 
is very likely that you will either crash your program or corrupt your Agenda 
file. So you should thoroughly test your code on a temporary Agenda file before 
using or distributing code based upon the AGENDA.OPX.

There are 4 basic entries that can be read/written to an Agenda:

	appointments/day entry's
	events
	to-do's
	anniversaries

There are also to-do lists within which to-do's reside.

Creation of entries involves the following steps:

1. Creating an entry in memory and obtaining a handle to it, e.g.

	appt& = AgnEnNewAppt&:

2. Setting properties on the entry, e.g. start and end times

	AgnEnSetText:(appt&,"Text")
	AgnApSetStartTime:(appt&,1998,12,25,13,0)

3. Adding the entry to the file, i.e.

	AgnAdd&:(appt&)

4. Freeing the entry in memory

	AgnEnFree:(appt&)

It is important that these steps are followed in order to avoid memory leaks, 
panics and errors in your code.

Most of the functions require a memory handle returned by one of the AgnEnNew* 
family of functions, the AgnFirstEntry& or AgnNextEntry& functions, or the 
AgnFetch& function. If any of these are called then it is important to call 
AgnEnFree at the end.

Some functions take an id parameter which identifies the object in the agenda 
file, e.g. AgnFetch&. This id can be obtained when the entry is added using 
AgnAdd& or for an existing entry which has been retrieved from the file use 
AgnEnGetId&.


FILE DETAILS
------------

The archive consists of the following files:

README.TXT	This file
AGENDAOPX.SIS	This is the main OPX file in SIS format
AGENDAOPXW.SIS	This is the WINS version in SIS format
AGENDA.TXH	This is the header file in text file format
DAGENDA.TPL	This is a demonstration program that shows you how the OPX can be used

INSTALLATION
------------

1.  Install AGENDAOPX.SIS

2.  Create AGENDA.OXH into the \System\OPL\ folder on either the C: or D: 
drive.  This may be done by either starting a new file called AGENDA.OXH and 
then using Import text... AGENDA.TXH, or by making use of OPLTRAN -CONV (see 
the OPL SDK for details).

3.  Create DAGENDA.OPL from DAGENDA.TPL any where you like.


USING THE OPX
-------------

1.  First translate and run the DAGENDA.OPL file to make sure everything correctly.

2.  To use the OPX in your program add the following line to the top of the 
code, immediately after the APP...ENDA and before the first procedure

    INCLUDE "AGENDA.OXH"

3.  You can now use the Agenda OPX procedures in your program.


CONTENTS OF ALARM.OXH
---------------------

CONST KUidOpxAgenda&=&10000547
CONST KOpxAgendaVersion%=$500

REM values returned by AgnEnGetType%:()
CONST KAgnApptEntry%=0
CONST KAgnTodoEntry%=1
CONST KAgnEventEntry%=2
CONST KAgnAnnivEntry%=3

REM values used with AgnAnSetShow:() and AgnAnGetShow%:()
CONST KAgnAnShowNone%=0
CONST KAgnAnShowBase%=1
CONST KAgnAnShowElapsed%=2
CONST KAgnAnShowBoth%=3

REM values used with AgnLiSetOrder:() and AgnLiGetOrder%:()
CONST KAgnLiOrderManual%=0
CONST KAgnLiOrderDate%=1
CONST KAgnLiOrderPriority%=2

DECLARE OPX AGENDA,KUidOpxAgenda&,KOpxAgendaVersion%
	AgnOpen:(filename$) : 1
	AgnClose: : 2

	AgnAdd&:(entry&) : 3
	AgnModify:(entry&) : 4
	AgnDelete:(entry&) : 5
	AgnFetch&:(id&) : 6

	AgnFirstEntry&: : 7
	AgnNextEntry&: : 8

	AgnEnNewAppt&: : 9
	AgnEnNewTodo&: : 10
	AgnEnNewEvent&: : 11
	AgnEnNewAnniv&: : 12

	AgnEnSetText:(entry&,text$) : 13
	AgnEnSetSymbol:(entry&,symbol$) : 14
	AgnEnSetAlarm:(entry&,dayswarning&,hour&,minute&,alarm$) : 15
	AgnEnSetCrossOut:(entry&,flag%) : 16
	AgnEnSetTentative:(entry&,flag%) : 17

	AgnEnGetId&:(entry&) : 18
	AgnEnGetType%:(entry&) : 19
	AgnEnGetText$:(entry&) : 20
	AgnEnGetSymbol$:(entry&) : 21
	AgnEnGetAlarm$:(entry&,BYREF dayswarning&,BYREF hour&,BYREF minute&) : 22
	AgnEnGetCrossOut%:(entry&) : 23
	AgnEnGetTentative%:(entry&) : 24

	AgnEnFree:(entry&) : 25

	AgnApSetStartTime:(appt&,year&,month&,day&,hour&,minute&) : 26
	AgnApSetEndTime:(appt&,year&,month&,day&,hour&,minute&) : 27
	AgnApGetStartTime:(appt&,BYREF year&,BYREF month&,BYREF day&,BYREF hour&,BYREF minute&) : 28
	AgnApGetEndTime:(appt&,BYREF year&,BYREF month&,BYREF day&,BYREF hour&,BYREF minute&) : 29

	AgnTdAt&:(list&,index&) : 30
	AgnTdSetList:(todo&,list&) :31
	AgnTdSetPriority:(todo&,priority&) :32
	AgnTdSetDueDate:(todo&,year&,month&,day&) : 33
	AgnTdSetDuration:(todo&,days&) : 34
	AgnTdGetList&:(todo&) :35
	AgnTdGetPriority&:(todo&) :36
	AgnTdGetDueDate:(todo&,BYREF year&,BYREF month&,BYREF day&) : 37
	AgnTdGetDuration&:(todo&) : 38

	AgnEvSetStartDate:(event&,year&,month&,day&) : 39
	AgnEvSetEndDate:(event&,year&,month&,day&) : 40
	AgnEvGetStartDate:(event&,BYREF year&,BYREF month&,BYREF day&) : 41
	AgnEvGetEndDate:(event&,BYREF year&,BYREF month&,BYREF day&) : 42

	AgnAnSetDate:(anniv&,year&,month&,dayofmonth&) : 43
	AgnAnSetShow:(anniv&,flag%) : 44
	AgnAnGetDate:(anniv&,BYREF year&,BYREF month&,BYREF dayofmonth&) : 45
	AgnAnGetShow%:(anniv&) : 46

	AgnLiAdd&:(list&) : 47
	AgnLiModify:(list&) : 48
	AgnLiDelete:(list&) : 49
	AgnLiFetch&:(id&) : 50

	AgnLiNew&: : 51
	AgnLiAt&:(index&) : 52
	AgnLiSetTitle:(list&,name$) : 53
	AgnLiSetOrder:(list&,order%) : 54
	AgnLiSetViewDisplay:(list&,display%,hour&,minute&) : 55
	AgnLiGetId&:(list&) : 56
	AgnLiGetTitle$:(list&) : 57
	AgnLiGetOrder%:(list&) : 58
	AgnLiGetViewDisplay%:(list&,BYREF hour&,BYREF minute&) : 59
	AgnLiFree:(list&) : 60
	AgnLiChangePosition:(oldPos&, newPos&) : 61
END DECLARE


PROCEDURE REFERENCE
-------------------

AgnOpen:
--------
Usage: AgnOpen:(filename$)

Open the Agenda file.  You need to ensure that the file is not open in Agenda 
before using AgnOpen. See the demo OPL code for details of how to 
automatically close and reopen Agenda.


AgnClose:
---------
Usage: AgnClose:

Close the Agenda file.


AgnAdd&:
--------
Usage: id& = AgnAdd&:(entry&)

Adds an entry to the agenda file.  The entry should first have been created 
using one of the AgnEnNew... procedures, and set up using the AgnEnSet 
procedures.  An id for the entry is returned, which may be used in subsequent 
calls to AgnFetch&:


AgnModify:
----------
Usage: AgnModify:(entry&)

Update an entry in the file.


AgnDelete:
----------
Usage: AgnDelete:(entry&)

Mark an entry in the file deleted.


AgnFetch&:
----------
Usage: entry& = AgnFetch&:(id&)

Fetch an entry from the file, given it's id.  A handle to the entry is 
returned.


AgnFirstEntry&:
---------------
Usage: entry& = AgnFirstEntry&:

Create an in-memory copy of the first entry in the agenda file, and return a 
handle to it.  This must subsequently be freed using AgnEnFree:. If there are 
no entries in the file, zero is returned.

This procedure is usually used in conjunction with AgnNextEntry&:
 

AgnNextEntry&:
--------------
Usage: AgnNextEntry&:

Create an in-memory copy of the next entry in the agenda file, and return a 
handle to it.  This must subsequently be freed using AgnEnFree:. If there are 
no more entries in the file, zero is returned.

This procedure is usually used in conjunction with AgnFirstEntry&:


AgnEnNewAppt&:
--------------
Usage: appt& = AgnEnNewAppt&:

Create a new empty appointment object in memory and return a handle to it.  
Once the object has been finished with, for example by adding it to the agenda 
file, you need to call AgnFree&: to release the memory allocated to the object.


AgnEnNewTodo&:
--------------
Usage: todo& = AgnEnNewTodo&:

Create a new empty to-do object in memory and return a handle to it. Once the 
object has been finished with, for example by adding it to the agenda file, you 
need to call AgnFree&: to release the memory allocated to the object.


AgnEnNewEvent&:
---------------
Usage: event& = AgnEnNewEvent&:

Create a new empty event object in memory and return a handle to it. Once the 
object has been finished with, for example by adding it to the agenda file, you 
need to call AgnFree&: to release the memory allocated to the object.


AgnEnNewAnniv&:
---------------
Usage: anniv& = AgnEnNewAnniv&:

Create a new empty anniversary object in memory and return a handle to it. 
Once the object has been finished with, for example by adding it to the agenda 
file, you need to call AgnFree&: to release the memory allocated to the object.


AgnEnSetText:
-------------
Usage: AgnEnSetText:(entry&,text$)

Set the text property of the object entry& to text$.


AgnEnSetSymbol:
---------------
Usage: AgnEnSetSymbol:(entry&,symbol$)

Set the symbol property of the object entry& to symbol$.


AgnEnSetAlarm:
--------------
Usage: AgnEnSetAlarm:(entry&,dayswarning&,hour&,minute&,alarm$)

Set the alarm property of the object entry&. The alarm will use a built in 
sound or a sound file of the name alarm$. It will be set for dayswarning& days 
before the entry at the time specified by hour&, minute&.


AgnEnSetCrossOut:
-----------------
Usage: AgnEnSetCrossOut:(entry&,flag%)

Set the crossed out property of the object entry&.  


AgnEnSetTentative:
------------------
Usage: AgnEnSetTentative:(entry&,flag%)

Set the symbol property of the object entry& to symbol$.


AgnEnGetId&:
------------
Usage: id& = AgnEnGetId&:(entry&)

Given an entry's handle, this call returns it's id.


AgnEnGetType%:
--------------
Usage: type% = AgnEnGetType%:(entry&)

Returns the type of an entry.  This corresponds to one of the following 
consts: KAgnApptEntry%, KAgnTodoEntry%, KAgnEventEntry%=2, KAgnAnnivEntry%.


AgnEnGetText$:
--------------
Usage: text$ = AgnEnGetText$:(entry&)

Returns the text associated with an entry.


AgnEnGetSymbol$:
----------------
Usage: symbol$ = AgnEnGetSymbol$:(entry&)

Returns the symbol associated with an entry.


AgnEnGetAlarm$:
---------------
Usage: sound$ = AgnEnGetAlarm$:(entry&,BYREF dayswarning&,BYREF hour&,BYREF minute&)

Retrieves information about an alarm attached to an entry.  The handle is 
passed to the procedure in entry&.  On return, sound$ contains the sound file 
or inbuilt sound name, dayswarning& contains the number of days before the 
event that the alarm is to sound, and hour& and minute& give the time of day 
at which the alarm will sound. 

If the entry does not have an alarm set, the values "",-1,-1,-1 are returned.


AgnEnGetCrossOut%:
------------------
Usage: crossOut% = AgnEnGetCrossOut%:(entry&)

Returns a non-zero value if the entry is crossed out.


AgnEnGetTentative%:
-------------------
Usage: tentative% = AgnEnGetTentative%:(entry&)

Returns a non-zero value if the entry is marked as tentative.


AgnEnFree:
----------
Usage: AgnEnFree:(entry&)

To free an entry in memory following a call to AgnEnNew&, AgnFetch&,
AgnFirstEntry& or AgnNextEntry&:


AgnApSetStartTime:
------------------
Usage: AgnApSetStartTime:(appt&,year&,month&,day&,hour&,minute&)

Sets the start time of an appointment.  Pass the entry handle in appt&.


AgnApSetEndTime:
----------------
Usage: AgnApSetEndTime:(appt&,year&,month&,day&,hour&,minute&)

Sets the end time of an appointment.  Pass the entry handle in appt&.


AgnApGetStartTime:
------------------
Usage: AgnApGetStartTime:(appt&,BYREF year&,BYREF month&,BYREF day&,BYREF hour&,BYREF minute&)

Gets the start time of an appointment.  Pass the entry handle in appt&.


AgnApGetEndTime:
----------------
Usage: AgnApGetEndTime:(appt&,BYREF year&,BYREF month&,BYREF day&,BYREF hour&,BYREF minute&)

Gets the end time of an appointment.  Pass the entry handle in appt&.


AgnTdAt&:
---------
Usage: todo& = AgnTdAt&:(list&,index&)

Moves to a to-do entry within a to-do list. list& is the handle of the list, 
index& is the number of the to-do entry (starting from 0).


AgnTdSetList:
-------------
Usage: AgnTdSetList:(todo&,list&)

Sets the list that the to-do entry belongs to. todo& is the handle of the to-do 
entry. list& is the handle of the to-do list.


AgnTdSetPriority:
-----------------
Usage: AgnTdSetPriority:(todo&,priority&)

Sets the priority of a to-do item.  priority& should be a value in the range 
1-9.


AgnTdSetDueDate:
----------------
Usage: AgnTdSetDueDate:(todo&,year&,month&,day&)

Sets the due date of a to-do item.


AgnTdSetDuration:
-----------------
Usage: AgnTdSetDuration:(todo&,days&)

Sets the duration of a to-do entry in days.


AgnTdGetList&:
--------------
Usage: list& = AgnTdGetList&:(todo&)

Gets the id of the list to which the to-do entry belongs.


AgnTdGetPriority&:
------------------
Usage: priority = AgnTdGetPriority&:(todo&)

Gets the priority of the to-do entry.


AgnTdGetDueDate:
----------------
Usage: AgnTdGetDueDate:(todo&,BYREF year&,BYREF month&,BYREF day&)

Gets the due date of a to-do entry.


AgnTdGetDuration&:
------------------
Usage: days = AgnTdGetDuration&:(todo&)

Gets the duration of a to-do entry in days.


AgnEvSetStartDate:
------------------
Usage: AgnEvSetStartDate:(event&,year&,month&,day&)

Set the start date of an event. The handle of the event is passed in event&.


AgnEvSetEndDate:
----------------
Usage: AgnEvSetEndDate:(event&,year&,month&,day&)

Set the end date of an event. The handle of the event is passed in event&.


AgnEvGetStartDate:
------------------
Usage: AgnEvGetStartDate:(event&,BYREF year&,BYREF month&,BYREF day&)

Gets the start date of an event. The handle of the event is passed in event&.


AgnEvGetEndDate:
----------------
Usage: AgnEvGetEndDate:(event&,BYREF year&,BYREF month&,BYREF day&)

Gets the end date of an event. The handle of the event is passed in event&.


AgnAnSetDate:
-------------
Usage: AgnAnSetDate:(anniv&,year&,month&,dayofmonth&)

Sets the date of an anniversary.  The handle of the anniversary is passed in 
anniv&.


AgnAnSetShow:
-------------
Usage: AgnAnSetShow:(anniv&,flag%)

Sets the "Show start year as" property of an anniversary. flag% may be set to 
one of the following constants: KAgnAnShowNone%, KAgnAnShowBase%, 
KAgnAnShowElapsed%, KAgnAnShowBoth%

AgnAnGetDate:
-------------
Usage: AgnAnGetDate:(anniv&,BYREF year&,BYREF month&,BYREF dayofmonth&)

Gets the date of an anniversary.  The handle of the anniversary is passed in 
anniv&.


AgnAnGetShow%:
--------------
Usage: show% = AgnAnGetShow%:(anniv&)

Gets the "Show start year as" property of an anniversary. The call will return 
one of the following constants: KAgnAnShowNone%, KAgnAnShowBase%, 
KAgnAnShowElapsed%, KAgnAnShowBoth%


AgnLiAdd&:
----------
Usage: id = AgnLiAdd&:(list&)

Adds a new to-do list to the agenda file.  This should first have been created 
using AgnLiNew&:


AgnLiModify:
------------
Usage: AgnLiModify:(list&)

Modify a to-do list in the agenda file.


AgnLiDelete:
------------
Usage: AgnLiDelete:(list&)

Mark a to-do list in the agenda file deleted.


AgnLiFetch&:
------------
Usage: list& = AgnLiFetch&:(id&)

Return a handle to a to do list specified by id.


AgnLiNew&:	
----------
Usage: list& = AgnLiNew&:

Create a new to-do list in memory and return a handle to it.  Once the list 
has been finished with, for example after adding it to the agenda file, you 
need to call AgnFree&: to release the memory allocated to the entry.


AgnLiAt&:
---------
Usage: list& = AgnLiAt&:(index&)

Allows iteration through the to-do lists.


AgnLiSetTitle:
--------------
Usage: AgnLiSetTitle:(list&,name$)

Sets the title of a to-do list.


AgnLiSetOrder:
--------------
Usage: AgnLiSetOrder:(list&,order%)

Sets the method by which the items in the to-do list are ordered. order% can 
take any of the following consts: KAgnLiOrderManual%, KAgnLiOrderDate%, 
KAgnLiOrderPriority%.


AgnLiSetViewDisplay:
--------------------
Usage: AgnLiSetViewDisplay:(list&,display%,hour&,minute&)

Controls whether to-do entries are displayed in the other views. If display%=1 
then they are shown, 0 means they aren't. If to-do's are displayed then the 
hour and minute parameter controls the time slot they are displayed in.


AgnLiGetId&:
------------
Usage: id& = AgnLiGetId&:(list&)

Given a to-do list handle, returns it's id.


AgnLiGetTitle$:
---------------
Usage: title$ = AgnLiGetTitle$:(list&)

Returns the title of a to-do list.


AgnLiGetOrder%:
---------------
Usage: order% = AgnLiGetOrder%:(list&)

Queries the method by which the items in the to-do list are ordered. Returns 
one of the following constants: KAgnLiOrderManual%, KAgnLiOrderDate%, 
KAgnLiOrderPriority%.


AgnLiGetViewDisplay%:
---------------------
Usage: viewDisplay = AgnLiGetViewDisplay%:(list&,BYREF hour&,BYREF minute&)

Queries whether to-do entries are displayed in the other views. If 1 is 
returned then they are shown, 0 means they aren't. If to-do's are displayed 
then the hour and minute parameter indicate the time slot they are displayed 
in.


AgnLiFree:
----------
Usage: AgnLiFree:(list&)

Frees a to-do list object previously created in memory by AgnLiNew&, AgnLiFetch&
or AgnLiAt&:


AgnLiChangePosition:
--------------------
Usage: AgnLiChangePosition:(oldPos&, newPos&)

Changes the position of a to-do list.


COPYRIGHT
---------

Agenda.opx is Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.  It 
forms part of the OPL SDK and is subject to the License contained therein.

DISTRIBUTION 
------------ 

Agenda.opx should only be distributed to end users in one of the SIS files 
included in this release.  This ensures that older versions will not be 
installed over newer versions without warning.

The best way to do this is to include the Alarm SIS within your applications 
PKG file.  This means you can distribute your application as a single SIS file.

For MARM distributions use this line in your PKG file:  
@"AgendaOPX.sis",(0x10000547)

For WINS distributions use this line: 
@"AgendaOPXW.sis",(0x10000547)