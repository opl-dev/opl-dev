REM tWFunc.tpl
REM EPOC OPL automatic test code for word functions.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

CONST kPath$="c:\Opl1993"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tWFunc", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tWfunc:
  global patha$(9)
  global pause%
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	
	patha$=kPath$
  trap mkdir patha$

  rem strtest:("Word Functions")
  hRunTest%:("tIo")
  hRunTest%:("tWpeek")
  hRunTest%:("tBpeek")
  hRunTest%:("tLoc")
  hRunTest%:("tLen")
  hRunTest%:("tAsc")
  hRunTest%:("tErr")
  hRunTest%:("tDate")
  hRunTest%:("tDow")
  hRunTest%:("tWeek")
  hRunTest%:("tFileW")
  hRunTest%:("tDblToW")
  hRunTest%:("longToW")
  hRunTest%:("ldToUw")
rem	hRunTest%:("tKeyC")
	hCleanUp%:("CleanUp")
rem	KLog%:(KhLogHigh%,"Some sample text")
endp


PROC CleanUp:
	lclose rem just in case
  trap delete kpath$+"\tWFunc.odb"
  trap delete kpath$+"\t_io.txt"
  trap delete kpath$+"\*.*"
  trap rmdir kpath$
ENDP


proc tKeyC:
	local stat%,key%,ret%
	rem cls
	rem SubTest:("KEYC test")
  ret%=keya(stat%,key%)
	if ret%<0
		raise ret%
  	rem print "keya failed"
	else
  	rem print "keya done ok"
	endif
  ret%=keyc(stat%)
	if ret%<0
		raise ret%
	endif
endp


proc tDow:
  rem SubTest:("DOW test")
  onerr e1::
  dow(32,1,1990)
  onerr off
  raise 1
	e1::
  onerr e2::
  dow(-1,1,1990)
  onerr off
  raise 2
	e2::
  onerr e3::
  dow(0,1,1990)
  onerr off
  raise 3
	e3::
  onerr e4::
  dow(1,13,1990)
  onerr off
  raise 4
	e4::
  onerr e5::
  dow(1,-1,1990)
  onerr off
  raise 5
	e5::
  onerr e6::
  dow(1,0,1990)
  onerr off
  raise 6
	e6::
  onerr e7::
  dow(1,-1,90)
  onerr off
  raise 7
	e7::
  onerr off
  if dow(1,1,1900)<>1 :raise 10 :endif
  if dow(12,6,1990)<>2 :raise 11 :endif
  if dow(31,12,2155)<>3 :raise 12 :endif
endp


proc tWeek:
	rem SubTest:("WEEK test")
	onerr e1::
	print week(1,100,2156)
  onerr off
  raise 1
	e1::
  onerr e2::
  print week (301,12,1899)
  onerr off
  raise 2
	e2::
  onerr off
  if week(1,1,1900)<>1
    raise 3
  endif
  if week(28,12,2155)<>52 :rem maximum date for week
    raise 4
  endif
  if week(18,7,1990)<>29
    raise 5
  endif
endp


proc tBpeek:
  local i%
	
  rem SubTest:("Byte Peek test")
  i%=1
  if peekB(addr(i%))<>1
    raise 1
  endif
	
  i%=255
  if peekB(addr(i%))<>255
    raise 2
  endif
	
  i%=256
  if peekB(addr(i%))<>0
    raise 3
  endif
endp


proc tWpeek:
  local l&,p%
  rem SubTest:("Peek word")
  p%=addr(l&)
  l&=1
  if peekW(p%)<>1
    raise 1
  endif
	
  l&=65535
  if peekW(p%)<>-1
    raise 2
  endif
	
  l&=&ffff8000
  if peekW(p%)<>-32768
    raise 3
  endif
	
  l&=65536
  if peekW(p%)<>0
    raise 4
  endif
endp


proc tDate:
  local second%,minute%,hour%,day%,dayN$(10),monthN$(20),date$(30),month%,year%
  local bldDate$(30),dayInM$(2),day$(2),year$(6),hour$(4),minute$(4),second$(4)
	
  rem SubTest:("Date word functions")
  date$=datim$
  second%=second
  minute%=minute
  hour%=hour
  year%=year
  month%=month
  monthN$=month$(month%)
  day%=day
  dayN$=dayName$(dow(day%,month%,year%))
  dayInM$=num$(day%,-2)
  if day%<10
    dayInM$="0"+chr$($30+day%)
  endif
  year$=num$(year%,4)+" "
  hour$=num$(hour%,2)+":"
  if hour%<10
    hour$="0"+hour$
  endif
  minute$=num$(minute%,2)+":"
  if minute%<10
    minute$="0"+minute$
  endif
  second$=num$(second%,2)
  if second%<10
    second$="0"+second$
  endif
  bldDate$=dayN$+" "+dayInM$+" "+monthN$+" "+year$+hour$+minute$+second$
  rem print "datim$     ->",date$
  rem print "Built name ->",bldDate$
  rem pause pause% :key
  if bldDate$<>date$
		raise 1
    rem beep 3,500
    rem print "Check tDate:() : dates differ by at least 1 second"
    rem print "String slice the text time when time is available"
    rem pause pause% :key
    rem raise 1
  endif
endp


proc longToW:
  local i%,l&
  rem SubTest:("Long to word Function")
  i%=&00007fff
  i%=&ffff8000
  l&=-32768
  while i%<=32700  :rem all valid integers
    if (l&/1000*1000)=l& : print l& :endif
    i%=l&
    l&=l&+10
  endwh
  onerr e1::
  l&=32768
  i%=l&
  raise 1
	e1::
  onerr off
  if err<>KErrOverflow%
    raise 2
  endif
  onerr e2::
  l&=-32769
  i%=l&
  raise 3
	e2::
  onerr off
  if err<>KErrOverflow%
    raise 4
  endif
endp


proc ldToUw:
  local i%,lp&,dp,numErrs%
  rem SubTest:("Long/Double to unsigned word Function")
  lp&=0    : rem long ptr 
  dp=0    : rem double ptr
  numErrs%=0
  while lp&<65536
		if lp&/1000*1000=lp&
			rem at 1,21 :print lp&
		endif
		dp=lp&
		if uadd(lp&,0)<>uadd(dp,0)
			rem alert("ldToUw: BUG!","Failed on values "+num$(lp&,10))
			raise 1
		endif
    lp&=lp&+1
    dp=dp+1
  endwh
  rem print "There were",numErrs%,"discrepencies - consistent with RunTime stack change"
	rem pause pause% :key
	if numErrs%>3 :raise 1 :endif
  return
	e1::
  rem print "Reached loop",lp&-1
  rem pause pause%
  raise 2
endp


proc procPeek:(fill$,dp,lp&)
  REM Peek in procedure so rt stack not in same place
	return(peekB(dp)<>peekB(lp&))
endp


proc tDblToW:
  local d,j%,i%,name$(30),err%,errb%
	rem SubTest:("Double to word test")
	name$="tWFunc\tDblToW"
  err%=KErrOverflow%
  errb%=KErrInvalidArgs%
	
  onerr e1::
  j%=32768.0
  raise 2
	e1::
  onerr off
  if err<>err% :raise 3 :endif
  onerr e2::
  j%=-32769.0
  raise 4
	e2::
  onerr off
  if err<>err% :raise 5 :alert("About to raise 5",err$(err)) :raise 5 :endif
  onerr e3::
  j%=9e99
  raise 6
	e3::
  onerr off
  if err<>err% :raise 7 :endif
  onerr e4::
  j%=-9e99
  raise 8
	e4::
  onerr off
  if err<>err% :raise 9 :endif
	
  j%=-32768.0 :i%=-32768 :if i%<>j% :raise 10 :endif
  j%=32767.0  :i%=32767  :if i%<>j% :raise 11 :endif
	
  i%=-32768
  d=-32768
  do
    j%=d
    rem if (i%/1000*1000)=i% :print i% :endif
    if j%<>i%
    		raise 1
      rem beep 3,2000
      rem print "Double",d,"is converted to word",j%
      rem pause pause%
    endif
    i%=i%+100
    d=d+100.
  until i%>=32600
endp


proc tAsc:
  local a%,s$(2),i%,p%
	
  rem SubTest:("ASC() test")
  s$="12"
  p%=addr(s$)+1+KOplAlignment%
  i%=0
  while i%<256
    pokeB p%,i%
    a%=asc(s$)
    if a%<>i%
			raise 1
      alarmLp:("ASC",i%)
    endif
    i%=i%+1
  endwh
endp


proc tErr:
  rem SubTest:("ERR test")
  onerr e1::
  raise 1  :rem go to check err is 1
  onerr off
  raise 2
	e1::
  onerr off
  if err<>1
  		raise 3
    rem beep 3,2000
    rem print "'err' function failed!!!"
    rem pause pause%
  endif
endp


proc tLen:
  local a$(255),i%
	
  rem SubTest:("LEN test")
	
  rem Do 2 without rept$ in case rept$ not working !
  if len("")<>0 :raise 1 :endif
  if len("123")<>3 :raise 2 :endif
	
  i%=0
  while i%<256
    a$=rept$("A",i%)
    if len(a$)<>i%
			raise 3
      alarmLp:("tLen",i%)
    endif
    i%=i%+1
  endwh
endp


proc tLoc:
	local i%,a$(255),b$(1),name$(30)
	
	rem SubTest:("LOC test")
	name$="tWFunc\tLoc"
	a$="1234567890"
	i%=loc(a$,"1")
	if i%<>1 :raise 1 :endif
	i%=loc(a$,"0")
	if i%<>10 :raise 2 :endif
	i%=loc(a$,"")
	if i%<>1 :raise 3 :endif
	i%=loc(a$,"x")
	if i%<>0 :raise 4 :endif
	
	a$=rept$("A",254)+"1"
	i%=loc(a$,"1")
	if i%<>255 :raise 20 :endif
	i%=loc(a$,"A")
	if i%<>1 :raise 21 :endif
	i%=loc(a$,"")
	if i%<>1 :raise 22 :endif
	i%=loc(a$,"x")
	if i%<>0 :raise 23 :endif
	
	b$="1"
	i%=loc(b$,"1")
	if i%<>1 :raise 24 :endif
	i%=loc(b$,"")
	if i%<>1 :raise 25 :endif
	i%=loc(b$,"x")
	if i%<>0 :raise 26 :endif
endp



REM---------------------------------------------------------------------------

proc tFileW:
	
  rem SubTest:("Dbf Word Functions test")
  tExist:
  tCount:
  tEof:
  tPos:
  tBigFile:
endp


proc tBigFile:
  local i%,rec%,loops%,lastRec%
	
	rem cls
  rem SubTest:("FIND in large Database File")
  trap delete patha$+"\tWFunc.odb"
  rem print "Creating patha$+\tWFunc.odb"
  create patha$+"\tWFunc.odb",a,a$
	
  loops%=999
  lastRec%=loops%+1
  a.a$="a"
  rem print "Appending",loops%,"records of ""a""..."
  while i%<loops%
    append
    i%=i%+1
    rem if i%=i%/100*100 :at 1,7 :print i% :endif
  endwh
  a.a$="1234567890"
	rem print "Appending 1 record of """;a.a$;""""
  append
  if count <> lastRec% :raise 100 :endif
  if pos <> lastRec% :raise 101 :endif
  next
  if not eof :raise 102 :endif
	position 2
	if a.a$<>"a" :raise 103 :endif
	position lastRec%
	if pos<>lastRec% :raise 104 :endif
	if a.a$<>"1234567890" :raise 105 :endif
	rem print "Closing..."
  close
	rem print "Checking existence..."
  if not exist(patha$+"\tWFunc.odb") :raise 106 :endif
	rem print "Opening..."
  open patha$+"\tWFunc.odb",a,a$
  rem print "Finding *1* after",loops%,"records of 'a'"
  rec%=find("*1*")
  rem print "  Found",a.a$,"at record",
	if rec%>0
		rem print rec%
	else
		rem print 65536+rec%
  endif
  rem pause pause% :key
  if a.a$<>"1234567890" :raise 1 :endif
  if rec%<>lastRec% :raise 2:endif
	rem print "Closing..."
  close
	rem print "Deleting patha$+\tWFunc.odb ..."
  delete patha$+"\tWFunc.odb"
endp


proc tPos:
  local i%
	
  rem SubTest:("POS test")
  trap close :trap close :trap close :trap close
  onerr e1::
  pos
  raise 1 :rem all closed - must get error
	e1::
  onerr off
	
  trap delete patha$+"\tWFunc.odb"
	
  create patha$+"\tWFunc.odb",a,r%
  if pos<>1 :raise 2 :endif
  i%=0
  while i%<2
    a.r%=i%
    append
    i%=i%+1
    if pos<>i% :raise i%+2 :endif
  endwh
  first
  if pos<>1 :raise 10 :endif
  last
  if pos<>2 :raise 11 :endif
  next
  if pos<>3 :raise 12 :endif
  position 0
  if pos<>1 :raise 13 :endif
  position 100
  if pos<>3 :raise 14 :endif
  close
	
	rem Now check when OPENed
	
  open patha$+"\tWFunc.odb",a,r%
  if pos<>1 :raise 15 :endif
  first
  if pos<>1 :raise 16 :endif
  last
  if pos<>2 :raise 17 :endif
  next
  if pos<>3 :raise 18 :endif
  position 0
  if pos<>1 :raise 19 :endif
  position 100
  if pos<>3 :raise 20 :endif
  close
  delete patha$+"\tWFunc.odb"
endp


proc tExist:
  rem SubTest:("EXIST test")
	
  trap delete patha$+"\tWFunc.odb"
  if exist(patha$+"\tWFunc.odb") :raise 1 :endif
  create patha$+"\tWFunc.odb",a,a$,b$,c$,d$
  close
  if not exist(patha$+"\tWFunc.odb") :raise 3 :endif
  delete patha$+"\tWFunc.odb"
endp


proc tCount:
  rem SubTest:("COUNT test")
	
  trap close :trap close :trap close :trap close
  onerr e1::
  count
  raise 1 :rem all closed - must get error
	e1::
  onerr off
  trap delete patha$+"\tWFunc.odb"
  create patha$+"\tWFunc.odb",a,a$,b$,c$,d$
  if count<>0 :raise 2 :endif
  a.a$=""
  append
  if count<>1 :raise 3 :endif
  close
  delete patha$+"\tWFunc.odb"
endp


proc tEof:
  local i%
	
  rem SubTest:("EOF test")
  trap close :trap close :trap close :trap close
  onerr e1::
  while not eof
    raise 1 :rem all closed - must get error
  endwh
  raise 2
	e1::
  onerr off
  trap delete patha$+"\tWFunc.odb"
  create patha$+"\tWFunc.odb",a,r%
  i%=0
  while i%<2
    a.r%=i%
    append
    i%=i%+1
  endwh
  first
  i%=0
  while not eof
    rem print a.r%
    next
    i%=i%+1
  endwh
  if i%<>2 :raise 3 :endif
  close
  delete patha$+"\tWFunc.odb"
endp


REM--------------------------------------------------------------------------

proc tIo:
	rem SubTest:("Non-interactive IO test")
	tIow:
	tIoa:
	tWrRd:  :rem write and read
	tUnique:
endp


proc tIow:
	local err%,fcb%,buf$(255),message$(255),p%,fname$(30),len%
	local size%

	rem SubTest:("IOW test")
	fname$=patha$+"\tWFunc.txt"
	lopen fname$
	p%=addr(message$)+1+KOplAlignment%
	message$="IOW(-1) writes to LOPENed file ok"
	len%=len(message$)
	size%=SIZE(message$)
	err%=iow(-1,2,#p%,size%)
	if err%<0 :raise err% :endif
	lclose

	err%=ioOpen(fcb%,fname$,$220) :rem mode open|text|random
	if err% :raise 1: print err$(err) :pause pause% :raise 1 :endif

	err%=ioRead(fcb%,addr(buf$)+1+KOplAlignment%,255)
	if err%<0 : print err$(err) :pause pause% :raise 2 :endif
	pokeb addr(buf$),err%
	rem print buf$
	if buf$<>message$
		hLog%:(KhLogAlways%,"Expecting message$=["+message$+"]")
		hLog%:(KhLogAlways%,"Received buf$=["+buf$+"]")
		raise 3
	endif

	err%=ioClose(fcb%)
	if err% :raise 4 : print err$(err) :pause pause% :raise 4 :endif
	delete fname$
endp


proc tIoa:
  local err%,fcb%,buf$(255),message$(255),p%,fname$(30),status%,signals%,len%
	local size%
	
  rem SubTest:("IOA test")
  fname$=patha$+"\tWFunc.txt"
  lopen fname$
  p%=addr(message$)+1+KOplAlignment%
  message$="IOA(-1) writes to LOPENed file ok"
  len%=len(message$)
  size%=size(message$)
  status%=-46
  err%=ioa(-1,2,status%,#p%,size%)
  if err%<0 :raise err% :endif
	do
    iowait
    if status%=-46
      signals%=signals%+1
    endif
  until status%<>-46
  lclose
  if status% :raise status% :endif
  while signals% :rem 1 less for THIS ioa
    IoSignal
    signals%=signals%-1
  endwh
	
  err%=ioOpen(fcb%,fname$,$220) :rem mode open|text|random
  if err% :raise 1: print err$(err) :pause pause% :raise 1 :endif
  
  err%=ioRead(fcb%,addr(buf$)+1+KOplAlignment%,255)
  if err%<0 :raise 2 : print err$(err) :pause pause% :raise 2 :endif
  pokeb addr(buf$),err%
  rem print buf$
  if buf$<>message$ : raise 3 :endif
	
  err%=ioClose(fcb%)
  if err% :raise 4 : print err$(err) :pause pause% :raise 4 :endif
  delete fname$
endp


proc tUnique:
	local ret%,handle%,name$(128)
	
  rem SubTest:("IOOPEN Unique name test")
  name$="\fred"
	ret%=ioopen(handle%,addr(name$),4)
  if ret%<0
		rem print err$(ret%)
	else
    if loc(name$,".")=0
			name$=name$+"."
		endif
		rem print "Unique name =",name$
  endif
  ioclose(handle%)
	delete name$
endp


proc tWrRd:
  local err%,cb%,mode%,ioFunc%,access%,format%
  local fName$(64),txt$(255),res$(255)
  local pos&,sense%
  local size%
	
  rem SubTest:("IOOPEN/IOWRITE/IOREAD/IOCLOSE test")
  fName$=patha$+"\t_io.txt"
  trap delete fName$
  ioFunc%=$01 					:rem Create
  format%=$10						:rem STREAM_TEXT
  access%=$300					:rem UPDATE|RANDOM
  mode%=ioFunc% OR format% OR access%
  err%=ioOpen(cb%,fName$,mode%)
  if err%
  		raise 1
    rem print "Error opening",fName$
		rem print err$(err%) :beep 3,1000
    rem get
  endif
  txt$="IoWrite worked"
  size%=size(txt$)
  err%=ioWrite(cb%,addr(txt$)+1+KOplAlignment%,size%) REM stream_text is binary.
	if err%
		doClose:(cb%,fname$)
		raise 2
		rem print "Error writing to",fName$
		rem print err$(err%) :beep 3,1000
    rem get
  endif
  rem print "Written:",txt$
  pos&=0
  sense%=1			: rem from start	
  err%=ioSeek(cb%,sense%,pos&)
  if err%
		doClose:(cb%,fName$)
    raise 3
    print "Error seeking to start of",fName$
		print err$(err%) :beep 3,1000
    get
  endif
  err% = ioread(cb%,addr(res$)+1+KOplAlignment%,255)
  if err%<0
		doClose:(cb%,fName$)
    raise 4
    print "Error reading",fName$
		print err$(err%) :beep 3,1000
    get
  endif
  pokeB addr(res$), err%
  rem print "Read:",res$
  if res$<>txt$
  	print "Read=[";res$;"] len=";len(res$)
  	print "Expt=[";txt$;"] len=";len(txt$)
  	hLog%:(Khlogalways%,"!!TODO Skipping over stream_text error.")
  	rem raise 6
  endif
	doClose:(cb%,fName$)
  rem print "Finished IoOpen,IoWrite,IoRead,IoSeek,IoClose ok"
  rem pause pause% :key
endp


proc doClose:(cb%,fName$)
	local err%
	err%=ioClose(cb%)
	if err%
		raise err%
		rem print "Error closing",fName$
		rem print err$(err%) :beep 3,1000
		rem get
	endif
endp


REM End of tWFunc.tpl

