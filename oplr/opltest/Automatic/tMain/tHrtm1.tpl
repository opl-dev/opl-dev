REM tHrtm1.tpl
REM EPOC OPL automatic test code for early OPL bugs.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tHrtm1", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem dINIT "Tests complete" :DIALOG
	gIPRINT "Tests complete" :PAUSE 10
ENDP


proc tHrtm1:
  global path$(9),drv$(2),patha$(9)
  drv$="c:"
  path$=drv$
  patha$=drv$+"\Opl1993"
  trap mkdir patha$
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	rem Strtest:("TESTS for BUGS SINCE ORIGINAL BETA RELEASE V1.?")
	hRunTest%:("dirStr")
	hRunTest%:("numStr")
	hRunTest%:("sciStr")
	hRunTest%:("midStr")
	hRunTest%:("len")
	hRunTest%:("open")
	hRunTest%:("onermiss")
	hRunTest%:("delete")
	hRunTest%:("intovr")
	hRunTest%:("filcmd")
	hRunTest%:("lnexp")
	hRunTest%:("year")
	hRunTest%:("posbugs")
	hRunTest%:("dtob")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	trap delete "c:\Opl1993\*.*"
	trap rmdir "c:\Opl1993"
	trap delete "filcmd.odb"
	trap delete "pos.odb"
	trap delete "monday.odb"
	trap delete "empt.odb"
ENDP


proc testerr:(conv$,expect$,err%)
	if conv$<>expect$
		rem print err$(err%),"in",tstName$
		rem get
		raise 1	
	endif
endp


proc dirStr:
	local dn$(255),a,i%
	rem SubTest:("DIRStr")
	rem print "dir$('')"
	dn$=dir$("")
	rem print "dir$('"+path$+"\*.opo')"
	dn$=dir$(path$+"\*.opo")
	i%=1
	do
		rem print dn$
		dn$= dir$("")
		i%=i%+1
	until dn$="" or i%=100
	dn$=dir$("") :rem this used to crash

	rem a=1.0
	rem print "print a,dir$('*.opo') where a==1.0"
	rem dn$=dir$(path$+"\*.opo")
	rem print a,dn$
	rem do
	rem 	rem 	print a,dir$("")
	rem 	i%=i%-1
	rem until i%=1
endp


proc numStr:
	local a$(255)
	rem SubTest:("NUMStr")
	a$=num$(1.1,6)
	rem print "num$(1.1,6) ->",a$
	testerr:(a$,"1",1)
	
	a$=num$(-1.1,6)
	rem print "num$(-1.1,6) ->",a$
	testerr:(a$,"-1",2)
	
	a$=num$(1.1e99,6)
	rem print "num$(1.1e99,6) ->",a$
	testerr:(a$,"******",3)
	
	a$=num$(-1.1e-99,-6)
	rem print "num$(-1.1e-99,-6) ->",a$
	testerr:(a$,"     0",4)
endp


proc sciStr:
	local a$(255)
	onerr cont::
	rem SubTest:("SCIStr")
	a$=sci$(123,2,8)
	rem print "sci$(123,2,8)->";a$
	testerr:(a$,"1.23E+02",1)
	
	a$=sci$(123,2,-8)
	rem print "sci$(123,2,-8)->";a$
	testerr:(a$,"1.23E+02",2)
	
	a$=sci$(123,2,-20)
	rem print "sci$(123,2,-20)->";a$
	testerr:(a$,"            1.23E+02",3)
	
	a$=sci$(123,2,7)
	rem print "sci$(123,2,7)->";a$
	testerr:(a$,"*******",4)
	
	a$=sci$(123,0,5)
	rem print "sci$(123,0,5)->";a$
	testerr:(a$,"1E+02",5)
	
	a$=sci$(123,0,4)
	rem print "sci$(123,0,4)->";a$
	testerr:(a$,"****",6)
	
	a$=sci$(123,128,3)
	rem print "sci$(123,128,3)->";a$
	testerr:(a$,"***",7)
	
	a$=sci$(-123.456,2,9)
	rem print "sci$(-123.456,2,9)->";a$
	testerr:(a$,"-1.23E+02",8)
	
	a$=sci$(-123.456,2,8)
	rem print "sci$(-123.456,2,8)->";a$
	testerr:(a$,"********",9)
	
	a$=sci$(-123.456,0,6)
	rem print "sci$(-123.456,0,6)->";a$
	testerr:(a$,"-1E+02",10)
	
	a$=sci$(-123.456,0,5)
	rem print "sci$(-123.456,0,5)->";a$
	testerr:(a$,"*****",11)
	
	a$=sci$(-123.456,128,3)
	rem print "sci$(-123.456,128,3)->";a$
	testerr:(a$,"***",12)
	
	a$=sci$(0.000123456,2,-8)
	rem print "sci$(0.000123456,2,-8)->";a$
	testerr:(a$,"1.23E-04",13)
	
	a$=sci$(0.000123456,2,-10)
	rem print "sci$(0.000123456,2,-10)->";a$
	testerr:(a$,"  1.23E-04",14)
	
	a$=sci$(-0.000123456,2,-9)
	rem print "sci$(-0.000123456,2,-9)->";a$
	testerr:(a$,"-1.23E-04",15)
	
	a$=sci$(-0.000123456,2,-8)
	rem print "sci$(-0.000123456,2,-8)->";a$
	testerr:(a$,"********",16)
	
	return
cont::
	onerr off
	if err
		raise 1000
		rem print err$(err)
	endif
endp


proc len:
	local len%,a$(255)
	rem SubTest:("LEN")
	a$=rept$("R",255)
	len%=len(a$)
	rem print "len(rept$('R',255))=";len%
	if len%<>255
		raise 400
		rem print err$(1)
	endif
	len%=len("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")
	rem print "expecting 128, slen returned",len%
	if len%<>128
		raise 401
		rem print err$(2)
	endif
	len%=len("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")
	rem print "expecting 220, slen returned",len%
	if len%<>220
		raise 402
		rem print err$(3)
	endif
endp


proc midStr:
	local a$(255)
	
	rem SubTest:("MID$")
	a$="ABCDEFGH"

	CompStr:(mid$(a$,2,1), "B", 100)
	CompStr:(mid$(a$,1,1), "A", 101)
	CompStr:(mid$(a$,8,1), "H", 102)
	CompStr:(mid$(a$,8,2), "H", 103)

	CompStr:(mid$(a$,9,1), "", 104)
	CompStr:(mid$(a$,5,0), "", 105)
	CompStr:(mid$(a$,1,8), "ABCDEFGH", 106)
	CompStr:(mid$(a$,1,20), "ABCDEFGH", 107)

	onerr cont::
	REM Print doesn't actually print.
	print mid$(a$,0,1)
	onerr off
	raise 1
	rem beep 5, 1000 :get :raise 1
cont::
endp


proc open:
	rem SubTest:("OPEN")
	trap delete "monday.odb"
	create "monday.odb",a,s1$
	close
	open "monday.odb",a,s1$
  trap open "monday.odb",a,s1$
	close
	if err<>KErrOpen%
		raise 1
		print err$(err)
	endif
endp


proc dtob:
	local x,y
	rem SubTest:("DTOB")
	x=1.0e-98
	IF x<>1e-98 AND x/10<>1e-99
		RAISE 100
	ENDIF
	y=-x-x-x-x-x-x-x-x-x-x+x+x+x+x+x+x+x+x+x
	rem print gen$(y,20)
	IF GEN$(y,20)<>"-1E-98"
		RAISE 101
	ENDIF
endp


proc onermiss:
	rem missing external etc. added beyond buffer.
	local x(2),i%(2),l&(2),s$(2,10)
	
	rem SubTest:("ONERMISS - error buffer not initialized ONERR trapping")
	onerr dblok::
	x(3)=0
dblok::
	onerr intok::
	chksubs:(1)
	i%(3)=0
intok::
	onerr longok::
	chksubs:(2)
	l&(3)=0
longok::
	onerr stringok::
	chksubs:(3)
	s$(3)="hello"
stringok::
	onerr allok::
	chksubs:(4)
allok::
	onerr off
endp


proc chksubs:(raisno%)
	if err<>KErrUndef% : rem Missing external in added to buffer
		raise raisno%
	endif
endp


proc delete:
	local nameErr%,dirErr%
	nameErr%=-38
	dirErr%=-42
	rem	SubTest:("DELETE NULL STRING and NAME WITHOUT EXTENSION")
	rem print
	trap delete ""
	rem DOS filing system returns dirErr% and EPOC nameErr%
	if (err <> dirErr%) and (err <> nameErr%)
		raise 1
	endif
	trap delete "tdele.*"
	create "tdele.odb",a,a$ :close
	create "tdele.x",a,a$ :close
	create "tdele.y",a,a$ :close
	create "tdele.z",a,a$ :close
	delete "tdele.odb"
	rem shouldn't delete any except .odb
	if dir$("tdele.*")="" :raise 2 :endif
	if dir$("tdele.odb")<>"" :raise 3 :endif
	delete "tdele.*"
endp


proc intovr:
	local i%
	rem SubTest:("INTEGER OVERFLOW")
	
	i%=32767/2*2+1
	if i%<>32767 :raise 101 :endif
	i%=-32766/2*2-2
	if i%<>-32768 :raise 102 :endif
	i%=-32767/2*2
	if i%<>-32766 :raise 103 :endif
	onerr a::
	i%=32700*2
	a::
	onerr off :if err<>-6 :raise 1 :endif
	onerr b::
	i%=-32700*2
	b::
	onerr off :if err<>-6 :raise 2 :endif
	onerr c::
	i%=32760**2
	c::
	onerr off :if err<>-6 :raise 3 :endif
	onerr d::
	i%=-32760**3
	d::
	onerr off :if err<>-6 :raise 4 :endif
	onerr e::
	i%=32767+1
	e::
	onerr off :if err<>-6 :raise 5 :endif
	onerr f::
	i%=-32767-2
	f::
	onerr off :if err<>-6 :raise 6 :endif
endp


proc filcmd:
	local cnt%
	
	rem SubTest:("FILE COMMANDS")
	trap delete "filcmd.odb"
	create "filcmd.odb",a,a$
	a.a$="record 1"
	append
	cnt%=count
	if cnt%<>1 :raise 102 :endif
	close
	open "filcmd.odb",a,a$
	first
	if a.a$<>"record 1" :raise 1 :endif
	next
	if a.a$<>"" :raise 2 :endif
	back
	if a.a$<>"record 1" :raise 3 :endif
	last
	if a.a$<>"record 1" :raise 4 :endif
	a.a$="record 2"
	append
	if a.a$<>"record 2" :raise 5 :endif
	first
	if a.a$<>"record 1" :raise 6 :endif
	next
	if a.a$<>"record 2" :raise 7 :endif
	next
	if a.a$<>"" :raise 8 :endif
	first
	find("*record 1*")
	if a.a$<>"record 1" :raise 9 :endif
	first
	find("*record 2*")
	if a.a$<>"record 2" :raise 10 :endif
	position 2
	if a.a$<>"record 2" :raise 11 :endif
	erase
	if a.a$<>"" :raise 12 :endif
	find("*record 1*")
	if a.a$<>"" :raise 13 :endif
	back
	if a.a$<>"record 1" :raise 14 :endif
	a.a$="record 2 again"
	append
	a.a$="record 3"
	append
	first
	while not eof
		find("*")
		if pos=1 and a.a$<>"record 1" :raise 15 :endif
		if pos=2 and a.a$<>"record 2 again" :raise 16 :endif
		if pos=3 and a.a$<>"record 3" :raise 17 :endif
		next
	endwh
	close
endp


proc lnexp:
	rem SubTest:("LNEXP")
	onerr e1::
	print LN(EXP(710.0))
	onerr off
	raise 1
	e1::
	onerr off
	if err<>KErrOverflow%
		rem print err,err$(err)
		rem pause 40
		raise 2
	endif
endp


proc year:
	rem SubTest:("YEAR")
	if year<1900
		rem print "year=";year
		rem pause 40
		raise 1
	endif
endp


proc CompStr:(a$,b$,raise%)
	IF a$<>b$
		RAISE raise%
	ENDIF
endp



proc posbugs:
	rem SubTest:("POSBUGS")
	p1:
	p2:
	p3:
	rem print "About to return from posbugs:!"	
endp


proc p1:
	trap delete "pos.odb"
	create "pos.odb",a,a$
	
	rem print "APPEND 3 records: r1,r2,r3"
	a.a$="r1"
	append :rem print "APPENDED 1 : pos=";pos
	a.a$="r2"
	append :rem print "APPENDED 2 : pos=";pos
	a.a$="r3"
	append :rem print "APPENDED 3 : pos=";pos
	
rem	print "position 2"
	position 2
rem	print "pos=";pos, "a.a$=";a.a$
	if pos <> 2 :raise 1 :endif
	if a.a$ <> "r2" :raise 2 :endif
	rem print "close"
	close
	
	rem print "reopen"
	open "pos.odb",a,a$
	rem print "position 2" 
	position 2
	rem print "pos=";pos, "a.a$=";a.a$
	if pos <> 2 :raise 3 :endif
	if a.a$ <> "r2" :raise 4 :endif
	close
endp


proc p2:
	trap delete "pos.odb"
	create "pos.odb",a,a$
	
	rem print "APPEND 3 records: r1,r2,r3"
	a.a$="r1"
	append :rem print "APPENDED 1 : pos=";pos
	a.a$="r2"
	append :rem print "APPENDED 2 : pos=";pos
	a.a$="r3"
	append :rem print "APPENDED 3 : pos=";pos
	
	rem print "position 0 and NEXT until eof"
	position 0
	if pos <> 1 :raise 1 :endif
	if a.a$ <> "r1" :raise 2 :endif
	while not eof
		rem print "pos=";pos,"a.a$=";a.a$
		if a.a$<>"r"+hex$(pos) :raise 3 :endif
		rem		if a.a$="r3" :break :endif
		next
	endwh
	
	rem print "position 1 and NEXT until eof"
	position 1
	while not eof
		rem print "pos=";pos,"a.a$=";a.a$
		if a.a$<>"r"+hex$(pos) :raise 4 :endif
		rem		if a.a$="r3" :break :endif
		next
	endwh
	
	rem print "FIRST and NEXT until eof"
	FIRST
	while not eof
		rem print "pos=";pos,"a.a$=";a.a$
		if a.a$<>"r"+hex$(pos) :raise 5 :endif
		next
	endwh
	
	rem print "first"
	FIRST
	rem print "position 2"
	position 2
	rem print "pos=";pos,"a.a$=";a.a$
	if a.a$<>"r2" :raise 6 :endif
	if pos<>2 :raise 7 :endif
	
	rem print "last"
	LAST
	if pos<>3 :raise 8 :endif
	if a.a$<>"r3" :raise 9 :endif
	rem print "position 2" 
	position 2
	rem print "pos=";pos,"a.a$=";a.a$
	
	rem last :position 2 :last used to crash with POSITION bug
	last
	position 2
	last
	rem print "pos=";pos,"a.a$=";a.a$
	if pos<>3 :raise 10 :endif
	if a.a$<>"r3" :raise 11 :endif
	close
endp


proc p3:
	local i%
	trap delete "empt.odb"
	create "empt.odb",a,a$
	a.a$="r1"
	append
	a.a$="r2"
	append
	a.a$="r3"
	append
	a.a$="r4"
	append
	
rem	print "Print record and then ERASE it until EOF"
	first
	while not eof
		rem print a.a$
		if a.a$="" :raise 1 :endif
		erase
	endwh
	
	rem print "APPEND 4 records holding r10,r20,r30,r40"
	a.a$="r10"
	append
	a.a$="r20"
	append
	a.a$="r30"
	append
	a.a$="r40"
	append
	
	rem print "Print 1st record and then UPDATE it until EOF"
	while i%<4
		i%=i%+1
		first
		if a.a$<>("r"+hex$(i%)+"0") :raise 2 :endif
		rem print a.a$,"becomes",
		a.a$=a.a$+"_UPDATED"
		update
		rem print a.a$
		if a.a$<>("r"+hex$(i%)+"0"+"_UPDATED") :raise 3 :endif
	endwh
	close
endp

REM End of tHrtm1.tpl
