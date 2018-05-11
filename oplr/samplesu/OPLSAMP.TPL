REM Copyright (c) 1997-1998 Symbian Ltd. All rights reserved.

INCLUDE "const.oph"
INCLUDE "dbase.oxh"
INCLUDE "date.oxh"


app oplapp,123
	icon "tdemo.mbm"
	caption "Sample",1
enda

rem Array element indexes for pointer events GetEvent32 keyword
const KEvType% = 1
const KEvTime% = 2
const KEvPtrOplWindowId% = 3
const KEvPtrType% = 4
const KEvPtrModifiers% = 5
const KEvPtrPositionX% = 6
const KEvPtrPositionY% = 7
const KEvPtrPositionRelParentX% = 8
const KEvPtrPositionRelParentY% = 9

proc main:
	global OnOpl1993%
	global scrInfo%(10),scrW%,scrH%
	global wIdTb% rem toolbar window
	global orgDb$(128),orgCol1$(10),orgCol2$(10),orgCol3$(10)
	global db$(128),col1$(10),col2$(10),col3$(10),tab$(20)
	global sqlO$(200),sqlC$(200)
	local m%,m$(1)
	local pos%
	local cmdn$(5)
	local i%
	local lastMenu%
	
		
	init:
	doCmd:		rem show and handle the command C/O<doc>
	while 1
		mInit
			mcard "App","Exit",%e
			mcasc "Editors","Text editor",%E,"Date/time",%d ,"Filename",%f
			mcard "Events","Get events",%v
			mcard "Dialogs","Position",%p,"Editors>",16,"Buttons",%b,"Text",%T
			mcard "Graphics","Grey scales",%g,"Windows",%w
			mcard "Advanced","DBMS database usage",%D,"Stopwatch OPX demo",%s,"DBMS index OPX demo",%i
			mcard "Other","RND test",%r
		m%=menu(lastMenu%)
		cls
		if m%=0
			continue
		endif
		m$=chr$(m%)
		if m$=upper$(m$)
			cmdn$="cmds" rem shifted
		else
			cmdn$="cmd"	rem unshifted
		endif
		@%(cmdn$+m$):
rem		screen scrInfo%(3)-1,scrInfo%(4)-1,1,2

		gUse 1
		gCls
	endwh
endp


proc cmds0%:
	rem Dummy cascade item for S3a
	giprint "No cascades on S3a"
endp
	
proc cmde%:
	stop
endp

proc cmda%:
	giprint "Nothing in this one"
endp

proc cmdSe%:
	rem Text editor
	local s1$(255),s2$(5),d%

	s1$=rept$("1234567890",25)+"12345"
	do
		dInit "Text editor: Esc quits"
			dEdit s2$,"Edit 5 char string"
			dEdit s1$,"Edit 255 char string",20
		if dialog=0
			break
		endif
		print "You entered:"
		print "<";s2$;">"
		print "and"
		print "<";s1$;">"
		print "Press a key to continue"
		get
		cls
	until 0
endp

proc cmdd%:
	rem Date/time editors
	local d1&,d2&,t1&,t2&

	do
		dInit "Date and time editors: Esc quits"
			dDate d1&,"Date",0,&10000
			dTime t1&,"Time",1,0,&10000
			dTime t1&,"Time (no seconds)",0,0,&7fff
		if dialog=0
			break
		endif
	until 0
endp

proc cmdf%:
	rem Filename editor
	local f1$(255)
	local flags&,flags$(30),label$(8)

	flags$="$1 or $80"
	while 1
		dInit "Filename editor/selector(flags=$"+hex$(flags&)+"): Esc quits"
			if flags& and 1	rem bit 1 set for editor
				label$="Edit"
			else
				label$="Select"
			endif
			onerr e1

			dFile f1$,label$+" file:",flags&
			onerr off
			dEdit flags$,"Set next flags",20
			dText "Flag values:","$1->editor,$2->allow dirs"
			dText "and:","$4->dirs only,$8-> disallow existing (e)"
			dText "and:","$10->query existing (e),$20->allow null string"
			dText "and:","$40->no extension (s),$80->allow wildcards"
		if dialog=0
			break
		endif
		flags&=eval(flags$)
		giprint f1$
		continue
	e1::
		giprint "Bad flags:"+err$(err)
get
	endwh
endp

proc cmdv%:
	rem Get events
	global ev&(16)
	global wId1%,wId2%,wTitle%

	local sInfo%(10)
	
	screenInfo sInfo%()
	
	screen sInfo%(3)-10,sInfo%(4)-2,1,3
	evWinCr:	rem Create demo windows
	while 1
		clrEv:	rem clear the array
		getevent32 ev&()
		if (ev&(1)=27) and (ev&(4)=0)
			rem Esc with no modifier
			break
		endif
		showEv:
	endwh
	evWinCl:
endp

proc showEv:
	rem uses globals ev&(16),wId1%,wId2%,wTitle%
	local ev1&,mod%,rep%
	local i%,cnt%

	ev1&=ev&(1)
	cnt%=10	
	if (ev1& and $400)=0
		mod%=ev&(4)
		rep%=ev&(5)
		print "Key event: Ascii ";
		print num$(ev1&,-3),
		if ev1&<$100
			print " ";
			if ev1&<$10
				print " ";
			endif
		endif
		print "(0x";hex$(ev1&);")",
		if ev1&>=32
			print "='";chr$(ev1&);"'",
		else
			print "    ",
		endif
		print "  Modif=0x";hex$(mod%),
	      print "Repeats=";rep%;"  "
	else
		print "Event: ",hex$(ev1&),
		i%=2
		print "  ";
		vector ev1&-&400
			l401,l402,l403,l404,l405,l406,l407,l408,l409,l410
		endv
    if ev1&<>&400   rem not null event
      print "Unknown",hex$(ev1&)
			return
		endif
		while 0
l401::
			print "Focus gained"
			break
l402::
			print "Focus lost"
			break			
l403::
			print "Switched on"
			break
l404::
			print "Exit command received"  rem Not yet implemented
			break
l405::
			print "Date changed"
			break
l406::
			print "Key down, scan code=";ev&(3);"                    "
			break
l407::
			print "Key up, scan code=";ev&(3);"                      "
			break
l408::
			print "Pointer event ";ev&(KEvPtrType%),
			print "At (";ev&(KEvPtrPositionX%);",";ev&(KEvPtrPositionY%);")",
			print "WindowId=";ev&(KEvPtrOplWindowId%)
			break
l409::
			print "Pointer entered window",ev&(KEvPtrOplWindowId%)
			break
l410::
			print "Pointer exited window",ev&(KEvPtrOplWindowId%)
			break
		endwh
	endif
endp

proc clrEv:
	rem uses global ev&(16)
	local i%

	while i%<16
		i%=i%+1
		ev&(i%)=0
	endwh
endp

proc evWinCr:
	rem Create event demo windows
	rem sets globals wId1%,wId2%,wTitle%

	local w%,h%
	local posX%
	local title$(40)
	
	title$="Read pen and other events: Esc quits"
	w%=gWidth
	h%=gHeight
	posX%=w%-85 rem X position of windows
	wTitle%=gCreate(0,0,w%,20,1)
	gborder 0
	gAt (w%-gTWidth(title$))/2,16
	gPrint title$
	wId1%=gCreate(posX%,40,80,30,1)
	gborder 0
	gAt (gWidth-gTWidth("Window0"))/2,gHeight-10
	gprint "Window";gIdentity
	wId2%=gCreate(posX%,80,80,30,1)	
	gborder 0
	gAt (gWidth-gTWidth("Window0"))/2,gHeight-10
	gprint "Window";gIdentity
endp

proc evWinCl:
	rem Close event demo windows
	gclose wId1%
	gclose wId2%
	gclose wTitle%
endp

proc cmdsT%:
	rem Text labels

	dInit "Text in dialogs"
		dText "Left aligned","xxxxxx",0
		dText "Right aligned","xxxxxx",1
		dText "","Centred",2
		dText "Bold","xxxxxx",$100
		dText "Line below","xxxxxx",$200
		dText "Allow select","xxxxxx",$400
	dialog
endp

proc cmdp%:
	rem Dialog position
	local h&,v&
	while 1
		dInit "Select position for next (-1,0 or 1)"
			dPosition h&,v&
			dLong h&,"Horizontal",-1,1
			dLong v&,"Vertical",-1,1
		if dialog=0
			return
		endif
	endwh
endp

proc toolbar:(val%)
	local orgTb%

	orgTb%=tb%
	tb%=val%
	vector tb%
		large,small
	endv
none::
	if wIdTb%
		gClose wIdTb%
		wIdTb%=0
	endif
	return
large::
	if wIdTb%
		gUse wIdTb%
		gCls
	else
		wIdTb%=gCreate(scrW%-80,0,80,scrH%,1,1)
	endif
	gBorder 0
	gAt 2,20
	drawBut:("But1")
	drawBut:("But2")
	drawBut:("But3")
	drawBut:("But4")
	return
small::
	nyi:

endp

proc drawBut:(label$)
	gButton label$,1,gWidth-4,(gHeight-20-80)/4,0
	gMove 0,(gHeight-20-80)/4+2
endp

proc cmdb%:
	rem Dialog and toolbar buttons
	global wIdTb%(5),tb%
	local b%

	nyi:
	while 1
		dInit "Set toolbar : Esc quits"
			dButtons "&Large",%l,"&Small",%s,"&None",%n
		b%=dialog
		if b%=0 or b%=%n
			toolBar:(0)
			if b%=0
				break
			endif
		endif
		if b%=%l
			toolbar:(1)
		elseif b%=%s
			toolbar:(2)
		endif
	endwh
	toolbar:(0)
endp

proc cmdg%:
	rem Grey scales
	local win1%,win2%
	local width&,rgb&

	width&=2
	rgb&=255
	dInit " Enter width of each band"
		dLong width&,"Width",1,scrW%
		dLong rgb&,"Initial colours",0,255
	if dialog=0
		return
	endif
	win1%=gcreate(0,0,scrW%,scrH%/2,1,1)
	guse 1
	win2%=gcreate(0,scrH%/2,scrW%,scrH%/2,1,2)
	guse win1%
	bands:(width&,rgb&)
	guse win2%
	bands:(width&,rgb&)
	get
	gClose win2%
	gClose win1%
	
endp

proc bands:(width&,initColor&)
	local col%,ii%,dif%
	dif%=16
	col%=initColor&
	ii%=(gwidth+width&-1)/width&
	gat 0,0
	while ii%>0
		gcolor col%,col%,col%
		gfill width&,gheight,0
		gmove width&,0
		col%=col%+dif%
		if col%>255
			col%=col%-32
			dif%=-16
		elseif col%<0
			col%=col%+32
			dif%=16
		endif
		ii%=ii%-1
	endwh
endp

proc doPause%:(orgK%,inPause%)
	local k%,pause%
	
	pause%=inPause%
	k%=orgK%
	while k%
		if k%=%-
			pause%=pause%+1
		elseif k%=%+
			pause%=pause%-1
		endif
		if pause%<0
			pause%=0
			break
		endif
		k%=key
	endwh
	return pause%
endp

proc cmdw%:
	rem Windows demo
	local i%
	local wId%(8),curWid%
	local w%,h%,gap%,orgY%,speed%
	local orgK%,k%,pause%,seed&
	local cnt%
	cnt%=7

	gUpdate off
	seed&=day+month+year+hour+minute+second
	randomize seed&	rem for random window
	pause%=0
	w%=40
	h%=40
	orgY%=30
	gap%=-10

	gcls
	gpatt -1,gwidth,gHeight,0
	print "Random seed=";seed&,"+ speeds up, - slows down: Esc quits"
	busy "Pause count="+num$(pause%,5)
	while i%<cnt%
		i%=i%+1
		wId%(i%)=gcreate(i%*(w%+gap%),orgY%,w%,h%,1)
		gAt (gWidth-gTWidth("W0"))/2,h%-14
		gPrint "W";i%+1 rem console is w1
		gBorder 0	
		gAt 0,0
	endwh
	while 1
		curWId%=rnd*(cnt%)+2
		gUse curWId%
		gsetwin gOriginX+20,gOriginY+10
		gat 3,3
		gInvert gWidth-6,gHeight-6
		gOrder gIdentity,1 rem random to front
rem		orgK%=key
rem		if orgK%
rem			if orgK%=27
rem				break
rem			endif
rem			pause%=doPause%:(orgK%,pause%)
rem			if pause%
rem				busy "Pause count="+num$(pause%,5)
rem			else
rem				busy "No pause"
rem			endif
rem		endif
rem		if pause%
rem			pause pause%
rem		endif
		if ((gOriginX+gWidth)>scrW%) or ((gOriginY+gHeight)>scrH%)
			gUse 1
			gCls
			print "Window",curWid%,"won"
			print "Press a key to continue..."
			busy off
			get		
			break
		endif
		gInvert gWidth-6,gHeight-6
		gat 0,0
	endwh
	i%=0
	while i%<cnt%
		i%=i%+1
		gclose wId%(i%)
	endwh	
	gcls
	busy off
	gUpdate on
endp


proc nyi:
	giprint "Not yet implemented"
endp

proc cmdr%:
	rem random number generation test
	local seed&
	local margX%,margY%
	local loops&,i&
	local min%,max%,minBox%,maxBox%
	local boxes&,box(1000),ix%,avgCnt&,boxW%
	boxes&=460
	avgCnt&=10 rem average in each box

	gUpdate off
	margX%=10
	margY%=10
	dInit "Enter seed for random numbers"
		dLong seed&,"Seed",0,&7fffffff
		dLong boxes&,"Number of boxes",2,gWidth-2*margX%
		dLong avgCnt&,"Average number per box",1,110
	if dialog=0
		return
	endif
	loops&=boxes&*avgCnt&
	boxW%=(gWidth-2*margX%)/boxes&
	if boxW%<=0
		boxW%=1
	endif
	gAt margX%,gHeight-margY%
	gLineBy gWidth,0
	gMove -gWidth,0
	gLineBy 0,-gHeight
	while i&<loops&
		i&=i&+1
		ix%=rnd*boxes&+1
		box(ix%)=box(ix%)+1
		gAt (ix%-1)*boxW%+margX%,gHeight-margY%-box(ix%)
		gLineBy boxW%,0
		if boxW%>2
			if box(ix%)<>1
				gMove -1,1
				gGMode 1
				gLineBy -boxW%+2,0
				gGMode 0
			endif
		endif
		if key=27
			break
		endif
	endwh
	gUpdate on
	rem beep 3,500
	giprint "Finished"
	gAt margX%+5,15
	min%=min(box(),boxes&)
	max%=max(box(),boxes&)
	ix%=0
	minBox%=0
	maxBox%=0
	while ix%<boxes& and (minBox%=0 or maxBox%=0)
		ix%=ix%+1
		if box(ix%)=min%
			minBox%=ix%
		elseif box(ix%)=max%
			maxBox%=ix%
		endif		
	endwh
	gPrint "Max=";max%;" (box";maxBox%;"), "
	gPrint "Min=";min%;" (box";minBox%;"), "
	gPrint "Std=";fix$(std(box(),boxes&),2,10)
	gat margX%+boxW%*(maxBox%-1),gHeight-margY%+2
	glineby boxW%,0
	gat margX%+boxW%*(minBox%-1),gHeight-margY%+2
	glineby boxW%,0
	get
endp

proc init:
	onOpl1993%=exist("rom::sys$prgo.img")

	rem Initialise globals for shell command
	rem to create new cmd(2) - defaults

	if not onOpl1993%
		Col1$="Name"
		Col2$="Address"
		Col3$="ID"
		Tab$="Table1"
	endif
	screenInfo scrInfo%() rem save for restoring
	scrW%=gWidth
	scrH%=gHeight
endp

proc doCmd:
	rem Show  and handle the command line
	local cnt%,i%

	cnt%=5	
	if not OnOpl1993%
		rem only 3 command components on S5
		cnt%=3
	endif
	
	while i%<cnt%
		at 10,7+i%
		i%=i%+1
		print "cmd$(";i%;")=";cmd$(i%)
	endwh
	Db$=cmd$(2)
	if cmd$(3)="O"
		sqlO$=Db$
		if not OnOpl1993%
			rem full DBMS SQL query of any kind allowed on ER1
			sqlO$=sqlO$+" SELECT "+Col1$+","+Col2$+","+Col3$+" FROM "+Tab$
		endif
		trap open sqlO$,a,name$,address$,id%
	elseif cmd$(3)="C"
		sqlC$=Db$
		Tab$="Table1"
		doCreate:(-1)	rem Create from cmdLine
	endif
endp

proc doCreate:(cmdLine%)
	do
		dInit "Create Database table"
			dPosition 0,1
			if cmdLine%
				dText "Database",Db$
			else
				dEdit Db$,"Database",20
			endif
			if not OnOpl1993%
				dEdit Tab$,"Table name",20
			endif
		if (dialog=0) and (tab$="")
			if cmdLine%
				giprint "Table name required"
				pause 20
				stop
			endif
		endif
		sqlC$=Db$
		if not OnOpl1993%
			sqlC$=sqlC$+" FIELDS Name,Address,ID TO "+Tab$
		endif
		trap create sqlC$,a,name$,add$,id%
		if err
			dInit "Error creating table"
				dText "SQL query",SqlC$
				dText "",err$(err)
				dButtons "&Exit app",%e,"&Retry",%r
			if dialog=%e
				stop
			endif
		endif
	until err=0
endp

rem ====================================

proc cmdsD%:
  rem DBMS demo

  local choice%
  local c%,d%,str$(16),n$(3)
  global type$(4,16),typ$(4,1),ty$(4,1)
  global file$(24),table$(24),field1$(16),field1%,field2$(16),field2%
  setdbvar:

  dinit "SQL in OPL32 databases"
  dtext "","A database in OPL32 can contain one or more 'TABLES'. ",2
  dtext"","A table contains records (a collection of fields).",2
  dtext"","This demo will show you how to create a database with 1 table and 2 fields",2
  dtext"","Edit the following boxes as desired.",2
  dedit file$,"File name: "
  dedit table$,"Table name: "
  dButtons "quit",27,"continue",13

  if dialog=0
    return
  endif

  dinit  "Demo of creating a database with 1 table and 2 fields"
  dtext"","A record is a collection of different fields: ",2
  dedit field1$,"Name of 1st  field: "
  dchoice field1%,"Type  of 1st  field: ",type$(1)+","+type$(2)+","+type$(3)+","+type$(4)
  dedit field2$,"Name of 2nd  field: "
  dchoice field2%,"Type of 2nd field: ",type$(1)+","+type$(2)+","+type$(3)+","+type$(4)
  dButtons "quit",27,"continue",13

  if dialog=0
    return
  endif

  dinit  "Demo of creating a database with 1 table and 2 fields"
  dtext"","You have chosen to create a table called '"+table$+"'",2
  dtext"","in a data file called '"+file$+"'.",2
  dtext"","Records in this table will contain 2 fields;",2
  dtext"","'"+field1$+"' of type "+type$(field1%)+" and '"+field2$+"' of type "+type$(field2%)+".",2
  dtext"","To create this in OPL32 the command is:",2
  dtext""," ",2
  dtext"","CREATE """+file$+" FIELDS "+field1$+","+field2$+" TO " +table$+""",A,F1"+typ$(field1%)+",F2"+typ$(field2%),$102

  dButtons "quit",27,"continue",13

  if dialog=0
    return
  endif

  dinit "CREATE """+file$+" FIELDS "+field1$+","+field2$+" TO " +table$+""",A,F1"+typ$(field1%)+",F2"+typ$(field2%)
  dtext"","The string in quotes is the 'SQL query', the letter A is the 'logical view name'",2
  dtext"","F1"+typ$(field1%)+" and F2"+typ$(field2%)+" are the field handles,",2
  dtext"","they refer to '"+field1$+"' and '"+field2$+"' respecively.",2
  dtext"","The symbols "+typ$(field1%)+" and "+typ$(field2%)+" indicate the types of these fields",2
  dtext""," ",2
  dtext"","This will create "+file$+" if does not already exist and add "+table$+" to it",2
  dtext"","The created 'view' will be left opened and the logical view name refers to it.",2
  dButtons "quit",27,"continue",13

  if dialog=0
    return
  endif

  dinit "Compatability with Opl1993 OPL syntax"
  dtext"","If the command given was CREATE """+file$+""",A,F1"+typ$(field1%)+",F2"+typ$(field2%),2
  dtext"","file '"+file$+"' is created if necessary and a table called 'TABLE1' added.",2
  dtext"","The field names are derived from the field handles.",2
  dtext""," The type indicators %, $ and amphresand are changed to i, s and a respectively,",2
  dtext""," in this example: 'TABLE1' will have fields called 'F1"+ty$(field1%)+"' and 'F2"+ty$(field2%)+"'.",2
  dButtons "quit",27,"continue",13

  if dialog=0
    return
  endif

  dinit"Examples of other uses of SQL in OPL32"
    dtext"", "OPEN """+file$+" SELECT "+field1$+","+field2$+" FROM " +table$+""",A,fld1"+typ$(field1%)+",fld"+typ$(field2%),2
    dtext"","opens a view on the '"+table$+"' table with access ",2
    dtext"","to just the fields specified between the words SELECT and FROM.",2
    dtext"","The field handles aren't significant but their type indicators are checked.",2
    dtext""," ",2
    dtext"... SELECT * FROM ...","selects all the fields from the table.",2
    dButtons "quit",27,"last page",13

  if dialog=0
    return
  endif

  dinit "SQL   continued"
  dtext"","SELECT * FROM "+table$+" ORDER BY "+field1$+" ASC"",A,F1"+typ$(field1%),2
  dtext"","Opens an ordered view on the table with access to all the fields",2
  dtext""," in each record. The view is ordered using the '"+field1$+"' field values.",2
  dtext"","The order can be alphanmuerically ASCending or DESCending",2
  dtext""," ",2
  dtext"","To do this an index will need to be created first (on the '"+field1$+"' field first).",2
  dtext"","This is done with the relevant OPX function - see Dbase OPX demo.",2
  dbuttons "FINISH",%F
  dialog

endp

proc cmdi%:
  rem DBASE OPX DBMS index file demo
  local path$(32),choice%,app%,key&(3)
  local c%,d%,str$(16),n$(3)

  path$="c:\opl\"
	trap mkdir path$
  trap delete path$+"dbsmp.dbf"
  create path$+"dbsmp.dbf",a, string$

  close
Again::
  open path$+"dbsmp.dbf",a, string$


  while 1

    dinit "DBMS demonstation"
    if c%=0
      dtext"","An empty database has been created for you.",2
      dtext" "," ",2
      dtext"","Add some records to it ",2
    else
      n$=Num$(count,3)
      dtext "You have "+n$+" records(S)","Add another record ?.",2
    endif
    str$=""
    dedit str$,"Record: "
    dText""," ",2
    dText "","The 'Append' button will add this string field to the database",$202

    dButtons  "Append",9,"Finished",13

    if dialog=13
      break
    else
      a.string$=str$
      append
    endif
    c%=c%+1
  endwh
  if count=0
    close
    return
  endif

rem  close
  if app%=0

    key&(1)=DbNewKey&:
    DbAddFieldTrunc:(key&(1),"strings",0,8)
    DbCreateIndex:("indexa",key&(1),path$+"Dbsmp.dbf","table1")
    key&(2)=DbNewKey&:
    DbAddFieldTrunc:(key&(2),"strings",1,8)
    DbCreateIndex:("indexb",key&(2),path$+"Dbsmp.dbf","table1")

    app%=app%+1
  endif
  while 1
    choice%=1
    dinit "Display Records"
      dtext"","Choose how to order the records that you've just entered",2
      dchoice choice%,"Sort order:","Alphabetically, reverse alphabetically, as entered, reverse as entered"
      dtext""," ",2
      dtext"","An index will be created using 'CreateIndex' OPX function",2
      dtext""," ",2
      dButtons "quit",-27,"append more",9,"display",13
    d%=dialog
    if d%=0
      break
    endif
    if d%=9
      goto Again
    endif
    vector choice%
    	c1,c2,c3,c4
    endv
    while 0
c1::
  	  open path$+"Dbsmp.dbf SELECT strings FROM table1 ORDER BY strings ASC",A,string$
    	break
c2::
    	open path$+"Dbsmp.dbf SELECT strings FROM table1 ORDER BY strings DESC",A,string$
    	break
c3::
c4::
    	open path$+"Dbsmp.dbf",a,string$
    	if choice%=4
	      last
    	endif
    endwh
    cls
    c%=0

    while c%<count
      if c%/17*17=c% and c%<>0
        print"                                     More  :  press any key"
        get
      endif
      print " ", a.string$
      if choice%=4
        back
      else
        next
      endif
      c%=c%+1
    endwh
    print
    print "That was ";count;" records  : Press a key to continue"
    get
    close
    cls
  endwh
endp

proc cmds%:
  rem Stopwatch demo using microsecond timing in DATE.OPX
  local s&,e&,d&,a%(2),micro$(6),k%
  local y&,mo&,day&,h&,mi&,se&,m&

  cls
  print
  print " The Opl32 DateTime OPX gives extended access to E32's time functions"
  print
  print " One of abilities of this OPX is to allow OPL programmers easily to"
  print " achieve microsecond accuracy in their programs. This is"
  print " demonstrated below. "
  print
  print
  print " Press any key to start / 'lap time' the clock  :  Esc to quit"
  get
  s&=DTNOW&:

  while 1

    e&=DTNOW&:

    DTDateTimeDiff:(s&,e&,y&,mo&,day&,h&,mi&,se&,m&)

    at 30,15
    micro$=num$(m&/1000,-3)
    print h&;":";mi&;":";se&;":"; micro$;
    print "                     "

    k%=key
    if k%=27
      break
    endif

    if k%
      at 1,17
      print"LAP TIME      ";y& ;" Year(s), "; mo&;" Month(s) and ";day&;" Day(s)."
      print
      print "  ";h&;" Hour(s), ";mi&;" Minute(s), ";se&;" Seconds and ";m&;" Microseconds."
    endif
  endwh
  cls
endp

proc setdbvar:

  file$="dbase.odb"
  table$="wages"
  field1$="employee"
  field2$="salary"
  field1%=4
  field2%=1

  type$(1)="integer"
  type$(2)="long"
  type$(3)="floating point"
  type$(4)="string"
  typ$(1)="%"
  typ$(2)="@" rem should be & but nmonic probs
  typ$(3)=""
  typ$(4)="$"
  ty$(1)="i"
  ty$(2)="a"
  ty$(3)=""
  ty$(4)="s"
endp
