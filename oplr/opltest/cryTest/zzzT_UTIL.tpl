Rem   Written by Howard Price, May 1990
Rem
Rem   T_UTIL is a utility module for all the other test modules
Rem

REM--------------------------------------------------------------------------

proc DontRun:

  beep 1,500
  print "T_UTIL.OPL is a utility module and cannot be run stand-alone"
  pause -30 :key
endp

proc setScr:
  return
endp

REM--------------------------------------------------------------------------

proc SetDbg%:(r%,pErrBuf%)

  rem Called directly by the RunTime when DEBUGsetDbg switch is on when
  rem RunTime is assembled.
  rem Sets up a flag for rDummy to be called from rEngineWatch in r_engine.asm
  rem User must set a break point at rDummy.
  rem The runTime will restart and automatically breaks when it reaches same
  rem Qcode absolute address with the same Opcode value.

  rem In:    r%         =  ptr to RunTime's R_DATA cell
  rem        pErrBuf%    =  ptr to Error Buffer returned from runtime
  rem                      (in r% cell but passed for convenience)

  rem Returns   DbgErrVal ($7f) if debugging required
  rem           DbgCount  ($7e) to count occurrence
  rem           0               to restore original error

  rem DEBUGsetDbg switch must be defined when runtime is assembled for this
  rem routine to be called

  rem   variables peeked from R_DATA
  local occur%,pOpcode%,opcode%

  rem   pointers to variables in R_DATA
  local pOccur%,pBOccur%

  local title$(80)
  local errBuf$(100)
  local i%,p%,char%,pSrc%,pDest%,choice$(1)

  onerr er::

  rem Set up all relevant data from R_DATA structure
  rem
  opCode%  = peekB(r%+$5bb)             : rem r->opCode
  pOpcode% = peekW(r%+$5b5)             : rem r->pOpcode
	occur%   = peekW(r%+$5bf)             : rem r->dbgOccur
  pOccur%  = r%+$5bf                    : rem ptr to r->dbgOccur
  pBOccur% = r%+$5b7                    : rem ptr to r->dbgBreakOccur

  print
  print
  title$="    T_UTIL Utility Module V1.0                             May,1990"

  print rept$("*",len(title$))
  print title$
  print rept$("*",len(title$))
  print
  beep 2,500

  pSrc%=pErrBuf% : pDest%=addr(errbuf$) : p%=pDest% : i%=0
  while i%<64
    i%=i%+1
    char%=peekB(pSrc%)
    pokeB p%,char%
    if char%=0
      break
    endif
    pSrc%=pSrc%+1
    p%=p%+1
  endwh
  pokeB pDest%-1,i% : rem lbc

  if i%=1
    print "OPL'S error buffer is empty!"
  elseif i%>=64
    print "OPL RUNTIME's error buffer is too long! Maximum is 64"
    print "Buffer contains:"
  endif

  print errBuf$
  print
  print err$(err),":",err
  print
  print "Occurrence:",
  if occur% :  print occur% : else : print "Unknown" : endif
  pokeW pOccur%, 0                                        : rem Re-initialise
  print
  print "The error occurred on opcode=$";hex$(opCode%),
  print "loaded at address $";hex$(pOpcode%)
  print

  print "(Q)uit (H)elp (C)ount (B)reakAlways",
  if occur%
    print "(O)ccurrence";occur%,"(S)pecify",
  else
    print "(S)pecify",
  endif
  while 1
    choice$=upper$(get$)
    if choice$="Q"
      return(0)
    elseif choice$="H"
      break
    elseif choice$="C"
      return($7e)
    elseif choice$="B"
      occur%=1
    elseif choice$="S"
      print "Occurrence?",
      input occur%
    elseif choice$<>"O"
      continue
    endif
    if occur%=0
      continue
    endif
    pokeW pBOccur%, occur%
    return($7f)
  endwh

  print
  print "This routine causes rDummy to be called whenever the RunTime is about to call"
  print "the same opcode loaded at the same memory address as when the above error"
  print "occurred."
  print

  if occur%=0
    print "If the error is inside a loop, the error may not occur"
    print "on the first pass."
    print "Do you want to count the occurrence?",
    if yesNo$: = "Y"
      print
      print "The RunTime will start again now and return here when the error"
      print "next occurs with the occurence known."
      print
      print "Press a key to continue"
      get
      return($7e)
    endif
  endif

  print "Break on occurrence",occur%;"?",
  if yesNo$: = "N"
    print
    print "Do you want to specify occurrence to break on?",
    if yesNo$: = "N"
      print
      print "Ok" : pause 15
      return(0)  : rem no break
	  endif
    print
    print "Occurrence?",
    input occur%
  endif

	pokeW pBOccur%, occur%
  print
  print "You must NOW set a break point at rDummy"
  print
  print "- the RunTime will then restart and break when the OpCode that failed"
  print "  is about to be run again on occurrence",occur%
  print
  print "- the next vectored call from Global EngW in the watch engine after the"
  print "  break point is reached is the QCODE OpCode that failed"
  print
  print "Press a key when you have set the break point ..." :beep 1,500
  get
  return($7f)                     :rem   User wants to debug

er::
  onerr off
  beep 1,500
  print "Error in t_util.opo\setDbg% !!!"
  print err$(err)
  pause 50
  raise 1
endp

proc rtmErrs:
  local baserr%

  rem THE CALLING PROCEDURE MUST DECLARE THE FOLLOWING GLOBALS

  rem global KErrIllegal%,KErrNumArg%,KErrUndef%,KErrNoProc%,KErrNoFld%,EOPEN%,KErrClosed%,KErrRecord%
  rem global KErrModLoad%,KErrMaxLoad%,KErrNoMod%,KErrNewVer%,KErrModNotLoaded%,KErrBadFileType%,KErrTypeViol%,KErrSubs%
  rem global KErrStrTooLong%,KErrDevOpen%,KErrEsc%,KErrMaxDraw%,KErrDrawNotOpen%,KErrOverflow%,EPEOF%,ENOSPC%
  rem global EDEVNP%,KErrInvalidArgs%,NOMEM%,KErrDivideByZero%,KErrName%,ROOTFUL%,EFAIL%
  rem global PAUSE%

  baserr% =-96
  KErrIllegal% = baserr%
  KErrNumArg%= baserr%-1
  KErrUndef% = baserr%-2
  KErrNoProc%= baserr%-3
  KErrNoFld% = baserr%-4
  KErrOpen%= baserr%-5
  KErrClosed%= baserr%-6

  KErrModLoad% = baserr%-8
  KErrMaxLoad%= baserr%-9
  KErrNoMod% = baserr%-10
  KErrNewVer%= baserr%-11
  KErrModNotLoaded% = baserr%-12
  KErrBadFileType% = baserr%-13
  KErrTypeViol%= baserr%-14
  KErrSubs%  = baserr%-15
  KErrStrTooLong% = baserr%-16
  KErrDevOpen% = baserr%-17
  KErrEsc%   = baserr%-18
  KErrMaxDraw%= baserr%-19
  KErrDrawNotOpen%= baserr%-20
  EFAIL%  =  -1
  KErrInvalidArgs% =  -2
  KErrOverflow%=  -6
  KErrDivideByZero%=  -8
  NOMEM%  = -10
  EPEOF%  = -36
  ENOSPC% = -37
  KErrName%= -38
  EDEVNP% = -41
  KErrRecord%= -43
  ROOTFUL%= -64
  PAUSE%  =  10     : rem Pause duration used for endtest:
endp

proc strtest:(n$)
  print
  print
  tstName$=n$
  print "TESTING",tstName$
  print "========";rept$("=",len(n$))
endp

proc subTest:(s$)
  print
  print s$
  print rept$("-",len(s$))
  print
endp

proc subTestX:(s$)
  beep 5,2000
  print
  print s$,"- TO BE EXTENDED"
  print rept$("-",len(s$))
  print
  pause pause% :key
endp

proc endTest:
  print
  print tstName$,"finished OK"
  print
  print
  pause pause% :key
endp

proc EndTestX:
  beep 5,2000
  print
  print tstName$,"Test WILL BE EXTENDED"
  print "but OK so far !!!"
  print
  pause pause% :key
endp

proc raiseIfN:(e%)
  rem raise error if it is not the same as the parameter passed

	if err=0 :raise 100 :endif
  if err<>e% :onerr off :raise err :endif
endp

proc cls:
  rem TMPCON.LDD doesn't do cls
  local i%

  while i%<25
    print
    i%=i%+1
  endwh
endp

REM--------------------------------------------------------------------------
REM Test ONERR handling

proc tOnerr:
  local i%,l&

  rem Raises an error if ONERR <label>:: fails
  rem Returns 0 if ONERR OFF succeeds else 1

	SubTest:("Testing ONERR handling")
  onerr err1::
  i%=1/0
  onerr off :raise 1  : rem if onerr off fails it will be caught later
err1::
  onerr err2::
  if err<>KErrDivideByZero%
    raise 2
  endif
  tOnerr2:        :rem Generate error in subprocedure for onerr err2::
                  :rem and return 1 if ONERR OFF fails
  return(1)        :rem ONERR OFF failed
err2::
  onerr off
  if err<>KErrDivideByZero%
    raise 4
  endif

	onerr err3:: :raise 0	:rem clear value returned by err
err3::
	onerr off
	if err :raise 5 :endif  :rem failed with label before the onerr
  onerr err3::		:rem try with label (err3) before the onerr
  return(0)        :rem Succeeded
endp

proc tOnerr2:
  local i%,count%

  rem Generate an error which should be caught by ONERR in calling routine
  rem Also checks ONERR OFF works by keeping a count of times onerr label
  rem is reached


  count%=0
  onerr err1::  : rem First set ONERR ON so it can be switched off !!!
  i%=1/0
  onerr off : raise 1
err1::
  count%=count%+1
  if count%>1
    return    : rem ONERR OFF failed so returns to a RAISE in tOnerr:
  endif
  onerr off
  i%=1/0      : rem returns to caller if ONERR is off correctly
  raise 3
  return(0)    : rem ONERR OFF ok
endp

proc yesNo$:
  local yesNo$(1)

  print "(Y/N)",
  while yesNo$<>"Y" and yesNo$<>"N"
    yesNo$=upper$(get$)
  endwh
  return(yesNo$)
endp

proc DblNeq%:(l,r)
	local precisn

  precisn=1e-10  :rem precision (the problem is taking the time to find the
                 :rem            true results to compare with opl's results)
  return abs(l-r)>=precisn
endp

proc DblNeqG%:(l,r,prec)
  local dif,res%

  dif=abs(l-r)
  print "Dif=";dif
	res%=(dif>=prec)
  if res%
    pause pause% :key
  endif
  return res%
endp


REM--------------------------------------------------------------------------

PROC testdump:
REM tests dump: dumping stream files to screen

	LOCAL file$(128)

	setScr:
	DO
		cls
		print "File to dump ";
		input file$
		IF len(file$)=0
			break
		ENDIF
		dumpFile:(file$)
	UNTIL 0
ENDP

PROC dumpFile:(fName$)
REM dumps a file to the screen

	LOCAL fcb%,ret%,buf%($80)
	LOCAL pB%,base&
	
	ret%=ioopen(fcb%,fName$,$400)		REM share,open,stream
	IF (ret%)
		raise ret%
	ENDIF
	pB%=addr(buf%(1))
	DO
		ret%=ioread(fcb%,pB%,$100)
		IF (ret%<=0)
			IF ret%=-36	or ret%=0		REM found Eof
				break
			ENDIF
			raise ret%
		ELSEIF disppage:(fName$,pB%,ret%,base&)=27
			break
		ENDIF
	base&=base&+$100
	UNTIL ret%<>$100
	ret%=ioclose(fcb%)
	if ret%
		raise ret%
	endif
ENDP

		
PROC disppage:(text$,pBytes%,size%,base&)
REM displays a page of the dump to the screen
REM p% is the base address of the bytes in ram
REM size% is the number of bytes 1-256
REM base& is the base address of the page in file
	
	LOCAL line$(80),char$(16)
	LOCAL b&,p%,pE%,pos%,b%
	
	cls
	print text$
	print
	print "    addr:  00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f    ................"
	print "  ------  ------------------------------------------------    ----------------"

	p%=pBytes%
	pE%=p%+size%
	b&=base&
	
	DO
		line$="  "+right$("000000"+hex$(b&),6)+": "
		char$=""
		pos%=0
		WHILE pos%<16
			IF p%=pE%
				line$=line$+"   "
				char$=char$+" "
			ELSE
				b%=peekb(p%)
				line$=line$+" "+right$("00"+hex$(b%),2)
				IF b%<32 or b%>127
					char$=char$+"."
				ELSE
					char$=char$+chr$(b%)
				ENDIF
				p%=p%+1
			ENDIF
			pos%=pos%+1
		ENDWH
		print line$,"  ",char$
		b&=b&+16
	UNTIL b&-base&=&100 or p%=pE%
	print
	print "                           Press any key to continue"
	return get
endp	
		
proc rJust$:(s$,n%)
	return(pad$:(s$,n%)+s$)
endp

proc lJust$:(s$,n%)
	return(s$+pad$:(s$,n%))
endp

proc pad$:(s$,n%)
	return(rept$(" ",n%-len(s$)))
endp

proc prtSpace:(inDev$)
	local s&

	s&=space&:(inDev$)
	print "[";left$(inDev$,1);"]";
	if s&<0
		print err$(s&);
	else
		print s&;"    ";
	endif
endp

proc fmSpace$:(inDev$)
	local s&,ret$(100)

	s&=space&:(inDev$)
	ret$="["+left$(inDev$,1)+"]"
	if s&<0
		ret$=ret$+err$(s&)
	else
		ret$=num$(s&,10)
	endif
	return ret$
endp



proc peekZT$:(p%)
	Rem Peek the zero-terminated (ZT) string at p% into a normal OPL
	Rem leading byte-count string, preserving the initial input string

	local res$(255),c%,pRes%

	pRes%=addr(res$)
	c%=peekB(p%)
	if c%=0
		res$=""
	else
		pokeB p%,255					:rem make lbc for peek$()
		res$=peek$(p%)					:rem all except 1st letter
		pokeB p%,c%						:rem restore input string
		pokeB pRes%,loc(res$,chr$(0))-1	:rem length except for 1st letter
		res$=chr$(c%)+res$				:rem insert 1st letter
	endif
	return res$
endp


proc exstDbf%:

	return (exist(dbf1$) and exist(dbf2$) and exist(dbf3$))
endp

proc getDbf:
	local g$(1),sp$(128)

	sp$="a:\"
	do
	    print "(C)opy (R)etry (Q)uit"
        g$=lower$(get$)
	    if g$="c"
	        print "Source path>"; :edit sp$
	        if right$(sp$,1)<>"\"
	            sp$=sp$+"\"
	        endif
	        trap copy sp$+"*.dbf",path$+"\"
	        if err :print err$(err) :endif
	    elseif g$="r"
	        if not exstDbf%:
	            print "Still not found"
	        endif
	    elseif g$="q"
	        stop
	    endif
	until exstDbf%:
endp

rem ------------------------------------------------------------------
rem Some more utilities for Opler1

proc tryBug:(procNam$)
  print "Try test """;procNam$;"""? Raised error in previous releases!!!"
  print "Press <Enter> to try it, <Esc> to skip"
  if get=27
    return 0.0
  endif
  return -1.0
endp

proc tryPanic:(procNam$)
  print "Try test """;procNam$;"""? Panicked in previous releases!!!"
  print "Press <Enter> to try it, <Esc> to skip"
  if get=27
    return 0.0
  endif
  return -1.0
endp

proc tryAViol:(procNam$)
  print "Try test """;procNam$;"""? Access violation in previous releases!!!"
  print "Press <Enter> to try it, <Esc> to skip"
  if get=27
    return 0.0
  endif
  return -1.0
endp

proc stopNow:(procNam$)
  print "Stop tests? """;procNam$
  print "Press <Enter> to stop tests, <Esc> to continue"
  if get=27
    return 0.0
  endif
  return -1.0
endp

