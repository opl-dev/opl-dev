REM plb_Dbas.tpl
REM EPOC OPL automatic test code for DBMS
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("plb_dbas", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP

PROC pLB_DBAS:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("plb_dbas1")
	hCleanUp%:("CleanUp")
ENDP

CONST KPath$="c:\"

PROC CleanUp:
	global path$(32)
	path$=KPath$
	TRAP CLOSE
	TRAP CLOSE
	TRAP CLOSE
	TRAP CLOSE
	DELETE path$+"Ben.dbf"
	DELETE path$+"Benb.dbf"
	DELETE path$+"Modify.dbf"
	DELETE path$+"Position.dbf"
ENDP

proc pLB_DBAS1:
  global exp%,val%,te%,te$(32)
  global path$(32)
  local c%,d%
  path$=kpath$
  REM print" Tests for LB_DBASE compatibity "
  IF exist (path$+"BEN.dbf")
 		delete path$+"BEN.dbf"
 	ENDIF

  IF exist (path$+"BENB.dbf")
 		delete path$+"BENB.dbf"
 	ENDIF

  create path$+"BEN.dbf",C, string1$,string2$,long&,int%,float
  create path$+"BENB.dbf",D,string1$,string2$,long&,int%,float

  onerr e1
  REM print "testing err: 'file/device in use' new behaviour"
  trap open path$+"BENB.dbf",a,string1$,string2$,long&,int%,float
e1::
	onerr off
	if err<>0 rem File in use
		REM print "Got unexpected error", err,err$(err)
		hLog%:(KhLogAlways%,"Part 1 error="+ERR$(ERR))
		raise 1
	endif

onerr e2
  REM print "testing err: 'file not exist'"
  open path$+"notexist.dbf",b,nam$,no%
  REM print "Failed to get error"
e2::
	onerr off
	if err<>-33 rem File NOT EXIST
		REM print "Got unexpected error", err,err$(err)
		raise 2
	endif


  REM print"testing count pos eof"
  use C

  te%=1
  val%=count
  exp%=0
  test:

  te%=2
  val%=pos
  exp%=1
  test:

  te%=3
  val%=eof
  exp%=-1
  test:

  c%=1
  while C%<10
     C.long&=c%*int(1001)
     C.string1$="dbase c"
     C.int%=C%
     c.float=C%*3.1457
     append
     c%=c%+1
  ENDWH

  te%=4
  val%=count
  exp%=9
  test:

  te%=5
  val%=pos
  exp%=9
  test:

  te%=6
  val%=eof
  exp%=0
  test:

  first

  te%=7
  val%=count
  exp%=9
  test:

  te%=8
  val%=pos
  exp%=1
  test:

  te%=9
  val%=eof
  exp%=0
  test:

  last

  te%=10
  val%=count
  exp%=9
  test:

  te%=11
  val%=pos
  exp%=9
  test:

  te%=12
  val%=eof
  exp%=0
  test:

  next
  te%=13
  val%=count
  exp%=9
  test:

  te%=14
  val%=pos
  exp%=10
  test:

  te%=15
  val%=eof
  exp%=-1
  test:

  first

  c%=1
  while C%<10
     d.long&=C.long&
     d.string1$="dbase d"
     d.string2$=c.string1$
     d.int%=C.int%
     d.float=c.float
     use d
     append
     use c
     next
     c%=c%+1
   ENDWH

REM print "testing Append Update and navigation"

  use d
  te%=16
  val%=count
  exp%=9
  test:

  first
  back

  te%=17
  val%=pos
  exp%=1
  test:

  first
  te%=18
  val%=find("dba?e c")
  exp%=1
  test:


  te%=19
  val%=find(" dd")
  exp%=0
  test:

  te%=20
  val%=eof
  exp%=-1
  test:

  position 4

  te%=21
  val%=pos
  exp%=4
  test:

  te%=22
  val%=d.int%
  exp%=4
  test:

  d.string2$="dbase c updated"
  use c
  first
  use d
  update
  first

  te%=23
  val%=find("*up*")
  exp%=9
  test:

  d.string2$="dbase c updated2"

  position 9

  update

  te%=24
  val%=find("*d2")
  exp%=0
  test:

  position 10

  te%=25
  val%=eof
  exp%=-1
  test:

  te%=24
  val%=find("*d2")
  exp%=0                  rem inserted to tests to check for finding from eof
  test:

  te%=25
  val%=eof
  exp%=-1
  test:

  position 0

  te%=26
  val%=pos
  exp%=1
  test:

  position 100

  te%=27
  val%=pos
  exp%=10
  test:

 REM print"testing find functions"
  te%=28
  val%=findfield("dbase C",1,1,2)
  exp%=0
  test:   rem should be zero

  te%=29
  val%=pos
  exp%=1
  test:

  te%=30
  val%=findfield("dbas? C",1,2,2)
  exp%=8
  test:

  te%=31
  val%=findfield("dbas? C",2,1,2)
  exp%=8
  test:

  te%=301  rem  ****** inserted extension
  val%=pos
  exp%=8
  test:

  d.string1$="BEN"
  append

  last
  next

  te%=302
  val%=eof
  exp%=-1
  test:

  te%=303
  val%=findfield("BEN",1,1,2)
  exp%=count
  test:

  last
  next

  te%=304
  val%=eof
  exp%=-1
  test:

  back
  te%=305
  val%=findfield("BeN",1,1,16)
  exp%=0
  test:

  te%=306
  val%=pos
  exp%=1
  test:
  last
  next


  te%=307
  val%=eof
  exp%=-1
  test:

  te%=308
  val%=pos
  exp%=count+1
  test:

  te%=309
  val%=findfield("BEN",1,1,0)
  exp%=count
  test:

last
next
  te%=310
  val%=findfield("BEN",1,1,1)
  exp%=0
  test:

  te%=311
  val%=pos
  exp%=count+1
  test:

  back
  erase
  close
  close

onerr e3
  REM print "testing err: 'argument error'"
  open path$+"BENB.dbf",a,nam$,no%
  REM print "Failed to get error"
e3::
	onerr off
	if err<>-2 rem WRONG ARGS
		REM print "Got unexpected error", err,err$(err)
		raise 3
	endif

onerr e4
  REM print "testing err: 'file not open'"
  a.nam$="s"
  REM print "Failed to get error"
e4::
	onerr off
	if err<>-102 rem File not open
		REM print "Got unexpected error", err,err$(err)
		raise 4
	endif



  open path$+"BENB.dbf",b,a$,b$,c&,d%,e	

  te%=32
  val%=count
  exp%=9
  test:

  te%=33
  val%=pos
  exp%=1
  test:

 erase
  te%=34
  val%=pos
  exp%=1
  test:

  te%=35
  val%=count
  exp%=8
  test:
 position 5
  te%=36
  val%=pos
  exp%=5
  test:

 erase
  te%=37
  val%=pos
  exp%=5
  test:

  te%=38
  val%=count
  exp%=7
  test:

 last
  te%=39
  val%=pos
  exp%=7
  test:

 erase
  te%=40
  val%=pos
  exp%=7
  test:

  te%=41
  val%=count
  exp%=6
  test:

  last
  next

  te%=42
  val%=eof
  exp%=-1
  test:

  te%=43
  val%=pos
  exp%=7
  test:
onerr e5
  REM print "testing err: 'End of file '"
  erase
  REM print "Failed to get error"
e5::
	onerr off
	if err<>-36
		REM print "Got unexpected error", err,err$(err)
		raise 5
	endif
  te%=44
  val%=count
  exp%=6
  test:

  te%=45
  val%=pos
  exp%=7
  test:


 open path$+"BENB.dbf select string1s,inti from Table1",a, s$,i%
  te%=46
  val%=count
  exp%=6
  test:

 close

trap open path$+"BENB.dbf",a,nam$,no%



REM print"Thorough Tests for LB_DBASE POS"

  trap delete path$+"POSITION.dbf"


  create path$+"POSITION.dbf",A, POS$

  te%=50
  val%=pos
  exp%=1
  test:

position -10

  te%=51
  val%=pos
  exp%=1
  test:

position 100

  te%=52
  val%=pos
  exp%=1
  test:

  c%=1
  while c%<10
     A.POS$=num$(c%,2)
     append
     te%=52+c%
     val%=pos
     exp%=c%
     test:
     c%=c%+1
  ENDWH

  c%=1
  d%=9
  while c%<10
      te%=62+c%
      first
      val%=Find(num$(d%,2))
      exp%=d%
      test:
      te%=72+c%
      val%=pos
      exp%=d%
      test:
      d%=d%-1
      c%=c%+1
  ENDWH

  c%=1
  d%=9
  first
  while c%<10
      te%=82+c%
      erase
      val%=pos
      exp%=1
      test:
      te%=92+c%
      val%=count
      exp%=d%-1
      test:
      d%=d%-1
      c%=c%+1
  ENDWH
 close
trap close
trap close


	REM print"complete"
	REM print "print press a key to text opl32 specific dbase code"
	REM get
mod:
endp

proc mod:
  global exp%,val%,te%,te$(32)
  global path$(32),val$(2),exp$(2)
  local c%,d%,b%(40),e%

  path$=KPath$
  REM cls
  REM print"testing DBASE: modify, insert, put, cancel + bookmarks"
  REM print datim$

  trap delete path$+"modify.dbf"


  create path$+"modify.dbf",A, POS$, i%
  close
  create path$+"modify.dbf FIELDS name , age , height TO Table2",B,n$,a%,h%
  close
  create path$+"modify.dbf FIELDS name , age , height TO Table3",c,n$,a%,h%
  close

  Open path$+"modify.dbf",A, POS$, i%
  Open path$+"modify.dbf Select name , age , height From Table2",B,n$,a%,h%
  Open path$+"modify.dbf Select * From Table3",c,n$,a%,h%

c%=1
  while c%<10
     use a
     insert
     A.POS$=num$(c%,2)
     a.i%=c%
     use b
     insert
     B.n$=num$(c%*10,2)
     b.a%=c%*10
     b.h%=c%
     use c
     insert
     c.n$=num$(c%*10,2)
     c.a%=c%*10
     c.h%=c%
     put
     use a
     put
     use b
     put
     c%=c%+1
  ENDWH

  use c
  first
  use b
  first
  use a
  first

  c%=1
  while c%<10
      use a
      te%=5+c%
      val%=a.i%
      exp%=c%
      test:
      next

      use c
      te%=15+c%
      val%=c.a%
      exp%=c%*10
      test:
      next

      use b
      te%=25+c%
      val%=b.h%
      exp%=c%
      test:
      next
      c%=c%+1
   ENDWH


  use c
  first
  use b
  first
  use a
  first

  c%=1
  while c%<10
      use a
      modify
      a.i%=10-c%
      put
      next
      use b
      modify
      b.a%=10-c%
      put
      next
      use c
      modify
      c.a%=10-c%
      put
      next
      C%=c%+1
  ENDWH


  use c
  first
  use b
  first
  use a
  first
  c%=1
  while c%<10

      use a
      te%=35+c%
      val%=a.i%
      exp%=10-c%
      test:
      next

      use b
      te%=45+c%
      val%=b.a%
      exp%=10-c%
      test:
      next

      use c
      te%=55+c%
      val%=c.a%
      exp%=10-c%
      test:
      next
   c%=c%+1
  ENDWH

  use c
  first
  use b
  first
  use a
  first

  c%=1
  while c%<10
      use a
      modify
      a.i%=100-c%
      cancel
      next
      use b
      modify
      b.a%=100-c%
      cancel
      next
      use c
      modify
      c.a%=100-c%
      cancel
      next

   c%=c%+1
  ENDWH

  use c
  first
  use b
  first
  use a
  first
  c%=1
  while c%<10

      use a
      te%=65+c%
      val%=a.i%
      exp%=10-c%
      test:
      next

      use b
      te%=75+c%
      val%=b.a%
      exp%=10-c%
      test:
      next

      use c
      te%=85+c%
      val%=c.a%
      exp%=10-c%
      test:
      next

  c%=c%+1
  ENDWH
onerr e1
  REM print "testing err: 'file already open for delete table'"
  Delete path$+"modify.dbf","Table1"
  REM print "Failed to get error"
e1::
	onerr off
	if err<>-101 rem File already open
		REM print "Got unexpected error", err,err$(err)
		raise 6
	endif
close
close
close
  Delete path$+"modify.dbf","Table1"

  Delete path$+"modify.dbf","Table3"
  Open path$+"modify.dbf SELECT * FROM Table2",B,n$,a%,h%


onerr e2
  REM print "testing err: 'table not exist'"
  Open path$+"modify.dbf SELECT * FROM Table3",d,n$,a%,h%
  REM print "Failed to get error"
e2::
	onerr off
	if err<>-33 rem File file does not exist
		REM print "Got unexpected error", err,err$(err)
		raise 7
	endif

close          rem need dbase close to create table

create path$+"modify.dbf FIELDS age, name , height TO Table3",c,a%,n$,h%

use c
insert
c.a%=100
put
append
append

      te%=100
      val%=count
      exp%=3
      test:
first
modify
c.a%=50

onerr e3
  REM print "testing err: 'INCOMPATIBLE UPDATE MODE'"
  append
  REM print "Failed to get error"
e3::
	onerr off
	if err<>-125
  REM print "Got unexpected error", err,err$(err)
  raise 8
	endif

 put
 te%=101
 val%=c.a%
 exp%=50
 test:

c.a%=51
onerr e4
  REM print "testing err: 'INCOMPATIBLE UPDATE MODE'"
  MODIFY
  REM print "Failed to get error"
e4::
	onerr off
	if err<>-125
  REM print "Got unexpected error", err,err$(err)
  raise 9
	endif

CANCEL
MODIFY
C.a%=90
put

 te%=102
 val%=c.a%
 exp%=90
 test:
close

REM print"testing Bookmarks!"
create path$+"modify.dbf FIELDS age,name TO Table9",a,a%,n$

  c%=1
    while c%<21
       insert
       a.a%=c%
       put
       b%(c%)=bookmark
        te%=102+c%
        val%=b%(c%)
        exp%=c%
        test:
      c%=c%+1
    ENDWH


    c%=1
     while c%<21
        gotoMark 21-c%
         te%=123+c%
         val%=a.a%
         exp%=21-c%
         test:
         c%=c%+1
      ENDWH

    c%=1
    while c%<11
  	   killMark b%(c%)
         c%=c%+1
      ENDWH

    c%=1
     while c%<11
  	   first
  	   position 11-c%
  	   b%(c%)=bookmark
         te%=133+c%
         val%=b%(c%)
         exp%=c%
         test:
         c%=c%+1
      ENDWH

      c%=1
     while c%<11
    	   gotoMark b%(c%)
         te%=c%+143
         val%=a.a%
         exp%=11-c%
         test:
         c%=c%+1
      ENDWH

   Open path$+"modify.dbf Select name  From Table2",B,n$

   d%=count
   c%=0
   first
   while c%<d%
      erase
      c%=c%+1
   ENDWH

   te%=154
   val%=count
   exp%=0
   test:

   te%=155
   val%=eof
   exp%=-1
   test:

   use b
  d%=count
  c%=0
  first
  while c%<d%
      erase
      c%=c%+1
  ENDWH

  te%=156
  val%=count
  exp%=0
  test:

  te%=157
  val%=eof
  exp%=-1
  test:

  c%=1
  while c%<21
      use a
      A.n$=num$(c%,2)
      append
      use b
      b.n$=num$(c%,2)
      append
      c%=c%+1
   ENDWH

   c%=1
   d%=9


   while c%<21
   use a
       first
       Find(num$(c%,2))
       b%((c%*2)-1)=bookmark
       use b
rem       first
rem       FindField(num$(c%,2),1,1,32)   wanna use it but translator broken
       last
       e%=1
       while b.n$<>num$(c%,2)
             if e%>30
                BREAK
             else
                back
             endif
       e%=e%+1
       endwh
       b%(c%*2)=bookmark
       c%=c%+1
   ENDWH

   c%=1
   while c%<21
       use a
       gotomark b%((c%*2)-1)
    te%=157+c%
    val$=a.n$
    exp$=num$(c%,2)
    tests:
       use b
       gotomark b%(c%*2)
    te%=177+c%
    val$=B.n$
    exp$=num$(c%,2)
    tests:
       c%=c%+1
   ENDWH


onerr e5
  REM print "testing err: 'bad bookmark'"

  use a
  gotomark b%(2)
  REM print "Failed to get error"
  if 0
e5::
  	onerr off
	  if err<>-33 rem bad mark
    REM print "Got unexpected error", err,err$(err)
    		raise 10
	  endif
  endif
onerr e6
  REM print "testing err: 'bad bookmark 2'"

  use a
  gotomark 999
  REM print "Failed to get error"
  if 0
e6::
  	onerr off
	  if err<>-2 rem syntax
		  REM print "Got unexpected error", err,err$(err)
		  raise 11
	  endif
  endif


  trap close
  trap close
  trap close
  trap close
  trap delete path$+"BEN.dbf"

  create path$+"BEN.dbf",a,x%,y%
  c%=1
  while c%<10
  a.x%=c%
  a.y%=c%

  te%=202+c%
  val%=a.x%
  exp%=c%
  test:

  te%=212+c%
  val%=a.y%
  exp%=c%
  test:

  te%=222+c%
  val%=a.y%
  exp%=c%
  test:

  append
  modify

  a.x%=a.x%*2
  a.y%=a.y%*2

  te%=232+c%
  val%=a.x%
  exp%=c%*2
  test:

  te%=242+c%
  val%=a.y%
  exp%=c%*2
  test:

  put

  c%=c%+1
  endwh

  REM print te%;" tests complete"
  REM get

endp


proc tests:
 if exp$<>val$
 REM print "error found in test:", te%
 REM print " press a key to continue"
 REM get
 	raise 100
 endif
endp


proc test:
 if exp%<>val%
 REM print "error found in test:", te%
 REM print "value received was ",val%
 REM print"were expecting ",exp%
 REM print " press a key to continue"
 REM get
 	raise 101
 endif
endp


REM End of plb_dbas.tpl
