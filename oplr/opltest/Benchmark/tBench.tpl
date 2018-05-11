REM tBench.tpl
REM EPOC OPL automatic test code -- benchmarking OPL.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tBench", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP

REM todo: stop using local logging routines, and switch
REM to hUtil logging functions instead.

proc p1:
  rem NULL procedure at start of module tests fastest procedure loading time
  return
endp


proc tBench:
  global path$(9),drv$(2),patha$(9)
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
  drv$="c:"
  path$=drv$
  patha$=drv$+"\Opl1993"
  trap mkdir patha$
	hRunTest%:("doBench")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	REM Keep the folder to read the tBench.txt log report.
	rem TRAP DELETE "c:\Opl1993"
ENDP


proc doBench:
  global tstName$(255)
  global lMargin$(20),dev$(8)
  local dt$(30),len%,dash$(100),hdrMarg$(30)
	local benchlogName$(14)
	
  rem strtest:("Bench Mark Tests")
	
	dev$=patha$+"\"
	trap delete dev$+"*.*" :rem free rampak space
	
	lMargin$=rept$(" ",6)          :rem left margin
	benchlogName$=dev$+"tBench.txt"
	lopen benchlogName$
  dt$=datim$
  dash$=rept$("-",78)
  len%=(78-(len(TstName$)+len(dt$)+1))/2
  hdrMarg$=rept$(" ",len%)
  lprint dash$
  lprint hdrMarg$,TstName$,dt$
  lprint dash$
  lprint
  bnFile:
  bnLproc:
	rem print "LOG FILE =",benchlogName$
  lprint
  lprint TstName$,"completed ok"
  lprint
  lprint dash$
  lclose
endp


REM--------------------------------------------------------------------------

proc bnFile:
  benchlogTest:("Database File handling")
  fileTime:(200,rept$("A",253),"1234","1234") :rem file size approx. 50K
	fileTime:(2000,"abcabca","1234abcd5678","*abcd*") :rem file size approx. 20K
endp


proc fileTime:(loops%,repFld$,last$,find$)
  local i%,j%,rec%,fName$(20)
	
  rem find find$ in loops% records with field repFld$ followed by record with
  rem field last$
  rem last$=="" for no match
	
	fName$=dev$+"t_bench.odb"
	trap delete fName$
  benchlogStr3:("Creating ",fName$,",a,a$")
  create fName$,a,a$
  a.a$=repFld$
 rem  print lMargin$,"Appending",loops%,"records of",a.a$
 rem print lMargin$,"(";len(a.a$);" data bytes per record) ..."
  lprint lMargin$,"Appending",loops%,"records of",a.a$
  lprint lMargin$,"(";len(a.a$);" data bytes per record) ..."
  benchlogStart:
  while i%<loops%
    append
    i%=i%+1
	endwh
  benchlogEnd:
  a.a$=last$
  append
	
	i%=100
	benchlogSIS:("Position to first and last record ",i%," times...")
	benchlogStart:
	while i%
		first
		last
		i%=i%-1
	endwh
  benchlogEnd:
	
	j%=10
	i%=loops%
	benchlogSIS:("Do NEXT ",j%*i%," times...")
	benchlogStart:
	while j%
		j%=j%-1
		first
		while i%
			next
			i%=i%-1
		endwh
	endwh
  benchlogEnd:
	
	j%=10
	i%=loops%
	benchlogSIS:("Do BACK ",j%*i%," times...")
	benchlogStart:
	while j%
		last
		j%=j%-1
		while i%
			back
			i%=i%-1
		endwh
	endwh
  benchlogEnd:
	
	benchlogStr2:("Closing",fName$)
	benchlogStart:
  close
  benchlogEnd:
  testFind:(fName$,find$,loops%,repFld$,last$)
 endp


proc testFind:(fName$,find$,loops%,repFld$,last$)
  local rec%
	benchlogStr2:("Opening",fName$)
	benchlogStart:
  open fName$,a,a$
	benchlogEnd:
  rem print lMargin$,"Finding",find$,"after",loops%,"records of",repFld$
  lprint lMargin$,"Finding",find$,"after",loops%,"records of",repFld$
  benchlogStart:
  rec%=find(find$)
  benchlogEnd:
  rem print lMargin$,"Found",a.a$,"at record",rec%
  rem pause pause% :key
  if last$<>""
    if a.a$<>last$ :raise 1 :endif
  endif
	benchlogStr2:("Closing",fName$)
  close
endp


REM--------------------------------------------------------------------------

proc bnLproc:
  benchlogTest:("Loading procedures")
  tLoop:  :rem In line version of lNull for comparison
  lNull:  :rem NULL procedure (ie. juct returns after loading procedure)
  return
  lG:     :rem Globals
  lE:     :rem Externals 1 frame up
  lP:     :rem Parameters
  lS:     :rem Sub-procedures
  lLS:    :rem Simple locals other than strings
  lLSs:   :rem Simple string locals
  lLA:    :rem Array locals other than strings
  lLAs:   :rem Array string locals
  lAll:   :rem All the above
  lSdSq:  :rem Small DATA small QCODE
  lLdSq:  :rem Large DATA small QCODE
  lSdLq:  :rem Small DATA large QCODE
  lLdLq:  :rem Large DATA large QCODE
  lBounds: :rem Compare loading around sector boundaries
endp

REM--------------------------------------------------------------------------

proc tLoop:
  local i%,dt$(30),loops%
	
  rem In line version of lNull for comparison
	
	loops%=10000
  benchlogSIS:("Loop",loops%,"times...")
  benchlogStr1:("(Opl1984 takes 11 seconds for 10000)")
  benchlogStart:
  while i%<loops%
    i%=i%+1
  endwh
  benchlogEnd:
endp


proc lNull:
  local i%,loops%
	
  loops%=10000
	rem unloadm "t_util"
  benchlogSIS:("Load a NULL procedure",loops%,"times...")
  benchlogStr1:("(Opl1984 takes 130 secs for proc at start of A:, 10000 loops)")
  benchlogStart:
  while i%<loops%
    p1:
    i%=i%+1
  endwh
  benchlogEnd:
	rem loadm "t_util"
endp


REM--------------------------------------------------------------------------
rem Utilities for bench mark test

proc benchlogStr1:(p1$)
	rem print lMargin$,p1$
  lprint lMargin$,p1$
endp


proc benchlogStr2:(p1$,p2$)
	rem print lMargin$,p1$,p2$
  lprint lMargin$,p1$,p2$
endp


proc benchlogStr3:(p1$,p2$,p3$)
  rem print lMargin$,p1$,p2$,p3$
  lprint lMargin$,p1$,p2$,p3$
endp


proc benchlogSIS:(p1$,p2%,p3$)
  rem log string,integer,string
	rem print lMargin$,p1$,p2%,p3$
  lprint lMargin$,p1$,p2%,p3$
endp


proc benchlogStart:
  local l$(100)
  l$=lmargin$+"    Start time:  "+right$(datim$,8)
  rem print  l$
  lprint l$
endp


proc benchlogEnd:
  local l$(100),dt$(30)
  dt$=datim$
  l$=lmargin$+"    End time:    "+right$(dt$,8)
  lprint l$
  rem print l$
  REM beep 3,1000
  rem pause pause% :key
endp


proc benchlogTest:(name$)
  rem print
  rem print name$;":"
  lprint
  lprint name$;":"
endp

REM End of tBench.tpl
