rem ============================================
rem Sample OPL code which demonstrates how to
rem use the Agenda.opx
rem ============================================

rem This code uses c:\documents\Calendar.test
rem Create this file using Agenda app before 
rem running demo.


INCLUDE "Agenda.oxh"
INCLUDE "System.oxh"
INCLUDE "Const.oph"

PROC test:
	LOCAL i&,j&,type%,entry&
	LOCAL list&,lid&,todo&,id&(5),name$(50)
	LOCAL alarm$(50),dayswarning&, hour&, minute&,x%,t%
	LOCAL file$(255),Open%

	file$="c:\Documents\Calendar.test"

	font 12,0
	print "Agenda.opx demonstration program"
	print

	busy "Busy",1

	rem the following file should already exist
	if not exist(file$)
		alert(file$,"does not exist","please create it")
		stop
	endif

	print "Checking if file open...";
	Open%=DocOpenCheck%:(file$)
	if Open%
		print "...yes it is!"
	else
		print "...no it's not!"
	endif

	AgnOpen:(file$)

	rem loop over todo lists looking for "My List"

	print "checking for old todo list..."

	do
		if list&<>0
			AgnLiFree:(list&)
		endif
		list&=AgnLiAt&:(i&)
		if list&<>0
			name$=AgnLiGetTitle$:(list&)
		endif
		i&=i&+1
	until name$="Agenda.opx test" or list&=0

	rem if found delete whole list

	if list&<>0
		AgnLiDelete:(list&)
		AgnLiFree:(list&)
		print "found & deleted old todo list"
	endif

	rem create a new todo list
	
	print "creating new todo list..."

	list&=AgnLiNew&:
	AgnLiSetTitle:(list&,"Agenda.opx test")
	AgnLiSetOrder:(list&,KAgnLiOrderDate%)
	AgnLiSetViewDisplay:(list&,1,9,0)
	AgnLiAdd&:(list&)

	rem create a new event entry

	print "creating new event..."

	entry&=AgnEnNewEvent&:
	AgnEnSetText:(entry&, "An event")
	AgnEnSetSymbol:(entry&,"E")
	AgnEnSetAlarm:(entry&,0,1,0,"Chimes")
	AgnEvSetStartDate:(entry&,year,month,day)
	AgnEvSetEndDate:(entry&,year,month,day+1)
	AgnAdd&:(entry&)
	AgnEnFree:(entry&)

	print "creating new appointment..."

	entry&=AgnEnNewAppt&:
	AgnEnSetText:(entry&, "An appointment")
	AgnEnSetSymbol:(entry&,"A")
	AgnApSetStartTime:(entry&, year,month,day,9,0)
	AgnApSetEndTime:(entry&,year,month,day,10,0)
	AgnEnSetTentative:(entry&,1)
	AgnEnSetCrossOut:(entry&,0)
	AgnAdd&:(entry&)
	AgnEnFree:(entry&)

	print "creating new anniversary..."

	entry&=AgnEnNewAnniv&:
	AgnEnSetText:(entry&, "An anniversary")
	AgnEnSetSymbol:(entry&,"B")
	AgnAnSetDate:(entry&, 1980,month,day)
	AgnAnSetShow:(entry&, KAgnAnShowElapsed%)
	AgnAdd&:(entry&)
	AgnEnFree:(entry&)

	print "creating new todos..."

	lid&=AgnLiGetId&:(list&)
	todo&=AgnEnNewTodo&:
	AgnTdSetList:(todo&, lid&)
	AgnTdSetPriority:(todo&, 1)
	AgnTdSetDueDate:(todo&,year,month,day)
	AgnTdSetDuration:(todo&,3)
	AgnEnSetAlarm:(todo&,1,hour,minute,"Rings")
	AgnEnSetSymbol:(todo&,"T")

	i&=0
	while i&<5
		AgnEnSetText:(todo&, "Todo "+num$(i&,10))
	 	i&=i&+1
	 	id&(i&)=AgnAdd&:(todo&)
	endwh
	AgnEnFree:(todo&)
	AgnLiFree:(list&)

	AgnClose:

	rem open file
	
	AgnOpen:(file$)

	rem modify last todo entry

	print "deleting last todo..."

	todo&=AgnFetch&:(id&(5))
 	AgnDelete:(todo&)
	AgnEnFree:(todo&)

	print "modifying todo..."

	todo&=AgnFetch&:(id&(4))
	AgnEnSetText:(todo&, "Modified")

 	AgnModify:(todo&)
	AgnEnFree:(todo&)

	busy off

	rem list todo lists
	
	print "scanning todo lists"
	
	i&=0
	while 1
		list&=AgnLiAt&:(i&)
		if list&=0 :break :endif
		j&=0
		while 1
			todo&=AgnTdAt&:(list&,j&)
			if todo&=0 :break :endif
			AgnEnFree:(todo&)
			j&=j&+1
		endwh
		print "List=";AgnLiGetTitle$:(list&)
		print "  Number of todos=",j&
		AgnLiFree:(list&)
		i&=i&+1
	endwh

	rem scan through all entries

	print "scanning entries..."

	entry&=AgnFirstEntry&:
	while entry&<>0
		type%=AgnEnGetType%:(entry&)
		dinit "Agenda entry"
		if type%=KAgnApptEntry%
			dtext "Entry type:","Appointment"
		elseif type%=KAgnTodoEntry%
			dtext "Entry type:","Todo"
		elseif type%=KAgnEventEntry%
			dtext "Entry type:","Event"
		elseif type%=KAgnAnnivEntry%
			dtext "Entry type:","Anniversary"
		else
			dtext "Entry type:","Unknown type"
		endif
		dtext "Text:",AgnEnGetText$:(entry&)
		dtext "Symbol:",AgnEnGetSymbol$:(entry&)
		alarm$=AgnEnGetAlarm$:(entry&, dayswarning&, hour&, minute&)
		if dayswarning&>=0
			dtext "Alarm:",num$(dayswarning&,10)+"d "+num$(hour&,2)+":"+num$(minute&,2)+"("+alarm$+")"
		else
			dtext "Alarm:","Not set"
		endif
		x%=AgnEnGetCrossOut%:(entry&)
		if x%=KTrue%
			x%=2
		else
			x%=1
		endif
		dCHOICE x%,"Crossed out:","No,Yes"
		t%=AgnEnGetTentative%:(entry&)
		if t%=KTrue%
			t%=2
		else
			t%=1
		endif
		dCHOICE t%,"Tentative:","No,Yes"
		AgnEnFree:(entry&)
		dbuttons "Cancel",-$11B,"Next",$10D
		if dialog=0 :break :endif
		entry&=AgnNextEntry&:
	endwh
	
	AgnClose:
ENDP

rem =================================
rem Check if file is open with Agenda
rem =================================
PROC DocOpenCheck%:(file$)
local id&,prev&,g$(255),s%
	s%=KFalse%
	while 1
		onerr e1
		id&=GetThreadIdFromOpenDoc&:(file$, prev&)
		if id&
			s%=KTrue%
		endif
	endwh
e1::
	ONERR OFF
	return s%
ENDP
