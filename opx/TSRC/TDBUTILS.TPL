REM tdbUtils.opl - test code for DBRefresh: function of DBase OPX
REM Copyright (c) 1998-2001 Symbian Ltd. All rights reserved.

INCLUDE "dbase.oxh"
INCLUDE "Sysram1.oxh"
INCLUDE "Const.oph"

DECLARE EXTERNAL
EXTERNAL CreateDB:
EXTERNAL TestUtils:
EXTERNAL AddRow:(name$)
EXTERNAL MakeIndex:
EXTERNAL SortDB:
EXTERNAL RandomName$:

CONST kMaxRecord&=17
CONST kFileName$="C:\Documents\dbUtils.db"

PROC Main:
	CreateDB:
	rem MakeIndex:
	TestUtils:
	PRINT "Done. Press any key." :GET
ENDP

CONST kSearch$="Alice"

PROC TestUtils:
	LOCAL check&
  OPEN kfilename$+" SELECT * FROM Book WHERE name='"+kSearch$+"' ORDER BY number ASC", A,name$,details$,number$,misc&
	PRINT "Opened",kFilename$;": searching for",kSearch$,"(COUNT=";COUNT,"in this view)" :PRINT
	
	WHILE NOT EOF
		PRINT A.name$,a.number$
		NEXT
	ENDWH
	
	AddRow:(kSearch$)

	PRINT "COUNT before refresh=";COUNT
	DbRefresh:
	PRINT "COUNT after refresh=";COUNT
	
	check&=DbFindField&:(kSearch$,1,1,KFindForwardsFromStart%)
	IF check&=0
		PRINT "Test failed! This should be 1 (it was for 1.01)"
	ELSE
		PRINT "Test passed -- the refresh OPX works."
	ENDIF

	CLOSE
	PRINT "Closed" :PRINT
ENDP

PROC AddRow:(name$)
	PRINT "Adding a row for",name$
	OPEN kFileName$+" SELECT * FROM Book",b,name$,details$,number$,misc&
	BEGINTRANS
	B.name$=name$
	B.details$="Some new entry"
	B.misc&=100000
	B.number$="123"
	APPEND
	COMMITTRANS
	PRINT "Row appended." :PRINT
	CLOSE
ENDP

PROC CreateDB:
	LOCAL name$(80),details$(80),number$(20),misc&
	LOCAL recordCount&
	LOCAL key&
	
	RANDOMIZE 31415926
	
	REM Create it.
	TRAP DELETE kFileName$
	CREATE kFileName$+" FIELDS name(80),details(80),number(20),misc TO Book",A,name$,details$,number$,misc&
	
	REM Populate it.
	BUSY "Creating..."
	BEGINTRANS
	WHILE recordCount&<kMaxRecord&
		INSERT
		A.name$=RandomName$:
		A.details$=LOWER$(DATIM$)
		A.misc&=100000+recordCount&		
		A.number$="+44 118 9"+GEN$(A.misc&*rnd,6)
		PUT
		recordCount&=recordCount&+1
	ENDWH
	COMMITTRANS
	CLOSE
	BUSY OFF
	PRINT "Created",kMaxRecord&,"records in '";kFileName$;"'."
ENDP

PROC MakeIndex:
	LOCAL key&
	key&=DbNewKey&:
	DbAddField:(key&,"number",KDbAscending&)
	DbCreateIndex:("index",key&,kFileName$,"Book")
	DbDeleteKey:(key&)
	PRINT "Index created."

	return
	OPEN kfilename$+" SELECT * FROM Book ORDER BY number ASC",A,name$,details$,number$,misc&
	PRINT "Sorted by number ascending"
	WHILE NOT EOF
rem		PRINT A.number$,A.name$
		NEXT
	ENDWH
	CLOSE
rem	DbDropIndex:("index","Sorter.dat","table1")
ENDP

CONST kMaxName%=6
PROC RandomName$:
	LOCAL name$(kMaxName%,20)
	name$(1)="Alice" :name$(2)="Bob": name$(3)="Charlie"
	name$(4)="Dave" :name$(5)="Eric": name$(6)="Fred"
	return name$(kMaxName%*RND+1)
ENDP