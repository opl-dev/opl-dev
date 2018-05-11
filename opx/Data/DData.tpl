REM DDATA.OPL
REM
REM Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.
REM
REM Demo for Data OPX

INCLUDE "DATA.OXH"
INCLUDE "DATE.OXH"
INCLUDE "DBASE.OXH"
INCLUDE "Const.oph"

PROC Main:
	print "Do you want to (l)ook into some OPL database?"
	print "Or rather (t)est the softcoded OPEN/CREATE functions?"
	if upper$(get$)="L"
		ViewDatabase:
	else
		CreateDb:
		ReadDb:
	endif
ENDP

PROC CreateDb:
	local i&, dnow&, buffer&, length&, key&
	length&=500
	buffer&=ALLOC(length&)
	i&=0
	while i&<length&
		POKEB buffer& + i&, (i& / 3)
		i&=i& + 1
	endwh
	dnow&=DTNow&:
	if exist("c:\testdb")
		delete "c:\testdb"
	endif
	ODbStartTable:
	ODbTableField:("name", KODbColText&, 25) : rem string of length 25
	ODbTableField:("age", KODbColUint8&, 1) : rem unsigned byte, not empty
	ODbTableField:("salary", KODbColInt32&, 0) : rem standard OPL long integer
	ODbTableField:("height", KODbColReal64&, 1) : rem OPL real, not empty
	ODbTableField:("birthday", KODbColDateTime&, 1) : rem datetime, not empty
	ODbTableField:("spousename", KODbColText&, 100) : rem string(100)
	ODbTableField:("married", KODbColBit&, 1) : rem Yes/No, not empty
	ODbTableField:("cv", KODbColLongText8&, 0) : rem Long Text8 field
	ODbCreateTable:("c:\testdb", "Employees")
	print "Created table (and file)"
	print "Now opening for writing"
	ODbOpen:(1, "C:\testdb SELECT name, age, salary, height, spousename, birthday, married, cv FROM Employees", "$?&.$???")
	USE B
	FIRST
	i&=0
	BEGINTRANS
	while i&<100
		INSERT
		ODbPutString:("abcd"+num$(i&,3)+"pipapo", 1)
		ODbPutWord:(2+i&, 2)
		ODbPutInt:(32000+ 19*i&, 3)
		rem OPL Syntax works for fields supported by OPL:
		B.F4=1.78 + 0.2 * sin(flt(i&))
		rem equivalent: ODbPutReal:(1.78 + 0.2 * sin(flt(i&)), 4)
		ODbPutString:("spouse"+num$(102-i&,3), 5)
		ODbPutDateTime:(dnow&, 6)
		if sin(flt(i&) * 1.7)>0
			ODbPutWord:(&1, 7)
		else
			ODbPutWord:(&0, 7)
		endif
		ODbPutLong:(buffer&, length&, 8)
		PUT
		i&=i& + 1
	endwh
	COMMITTRANS
	CLOSE
	print "Database has been created."
	print "Press a key"
	get
	print "Creating index"
	key&=DbNewKey&:
	DbAddField:(key&, "age", 1)
	DbCreateIndex:("AgeIndex", key&, "c:\testdb", "Employees")
	DbDeleteKey:(key&)
	print "Press a key"
	get
ENDP

PROC ReadDb:
	local f%, length&, buffer&, dtime&, i&
	dtime&=DTNow&:
	ODbOpenR:(2, "c:\testdb SELECT name, salary, spousename, height, age, married FROM Employees", "$&$.??")
	ODbUse:(2) : rem same as use C
	print "Opl COUNT returns", COUNT
	print "OdbCount& returns", ODbCount&:
	print "Press a key"
	get
	first
	do
		rem Normal OPL Syntax:
		print C.f1$, C.f2&, C.f3$, C.f4,
		rem OPX calls
		print ODbGetWord&:(5),ODbGetWord&:(6)
		next
	until EOF
	close
	print "Press a key"
	get
	ODbOpen:(3, "c:\testdb SELECT name, age, height, cv, birthday FROM Employees", "$?.??")
	POSITION 50
	print "The entry at position 50:"
	print ODbGetString$:(1),ODbGetWord&:(2), D.F3
	print "Length of CV is", ODbGetLength&:(4)
	ODbGetDateTime:(dtime&, 5)
	print "Birthday is", DTYear&:(dtime&),DTMonth&:(dtime&),DTDay&:(dtime&)
	MODIFY
	ODbPutWord:(235,2)
	Print "I've modified the age in this entry"
	PUT
	NEXT
	NEXT
	print "Entry at position 52:"
	print ODbGetString$:(1),ODbGetWord&:(2), D.f3
	BACK : BACK
	print "Back to entry at position 50:"
	print ODbGetString$:(1),ODbGetWord&:(2), D.f3
	FIRST
	print "Doing a Find(""*40*""):"
	FIND("*40*")
	print ODbGetString$:(1),ODbGetWord&:(2), D.f3
	buffer&=ALLOC(20)
	ODbGetLong:(buffer&, 20, 4)
	print "The first 20 bytes of the CV are:"
	i&=0
	while i&<20
		print PEEKB(buffer& + i&),
		i&=i& + 1
	endwh
	print
	print "Press a key"
	get
	rem
	rem SQL Find function
	FIRST
	print "And here are all entries that fit the SQL expression:"
	print "name LIKE '*5*' AND height > 1.9"
	do
		f%=ODbFindSql&:("name LIKE '*5*' AND height > 1.9", 1)
		if f%<>0
			print D.f1$, ODbGetWord&:(2), D.f3
			NEXT
		endif
	until f%=0
	print "Press a key"
	get
	FIRST
	print "Seek to: age equal to 50 returns", ODbSeekWord&:(50, "Employees", "AgeIndex", KODbEqualTo&)
 	print D.f1$, ODbGetWord&:(2), D.f3
	print "Seek to: age greater than 150 returns", ODbSeekWord&:(150, "Employees", "AgeIndex", KODbGreaterThan&)
 	print D.f1$, ODbGetWord&:(2), D.f3
	print "Seek to: age less than 1 returns", ODbSeekWord&:(1, "Employees", "AgeIndex", KODbLessThan&)
 	print D.f1$, ODbGetWord&:(2), D.f3
	print "Press a key"
	CLOSE
	get
ENDP

PROC ViewDatabase:
	local dbase$(255), table$(100), index$(100), tabno&, idxno&, i&, j&, colno&
	local dialog%
	dbase$="c:\testdb"

	dINIT "Select database to view"
	dFILE dbase$,"Name,Folder,Disk",0
	dBUTTONS "Close",KdBUTTONEnter%
	LOCK ON :dialog%=DIALOG :LOCK OFF
	IF dialog%<>KdBUTTONEnter%
		RETURN
	ENDIF
	tabno&=1
	tabno&=ODbGetTableCount&:(dbase$)
	print "There are",tabno&,"tables:"
	i&=1
	while i&<=tabno&
		table$="Persons"
		table$=ODbGetTableName$:(dbase$, i&)
		print table$,
		i&=i& + 1
		idxno&=0
		idxno&=ODbGetIndexCount&:(dbase$, table$)
		if idxno&=0
			print "(no indices)"
		else
			print "(Indices: ";
			j&=1
			while j&<=idxno&
				Index$=ODbGetIndexName$:(dbase$, table$, j&)
				print Index$,"[";ODbGetIndexDescription$:(dbase$, table$, Index$);"],",
				j&=j& + 1
			endwh
			print ")"
		endif
		colno&=ODbGetFieldCount&:(dbase$, table$)
		print "  ";
		j&=1
		while j&<=colno&
			print ODbGetFieldName$:(dbase$, table$, j&),"(Type:",
			print ODbGetFieldType&:(dbase$, table$, j&);", Size:",
			print ODbGetFieldSize&:(dbase$, table$, j&);", CanBeEmpty:",
			print ODbGetCanBeEmpty%:(dbase$, table$, j&);")",
			j&=j& + 1
		endwh
		print
	endwh
	print "Press a key"
	get
ENDP