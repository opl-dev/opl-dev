REM tSFunc.tpl
REM EPOC OPL automatic test code for string functions.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tSFunc", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem dINIT "Tests complete" :DIALOG
	gIPRINT "Tests complete" :PAUSE 10
ENDP


proc tSFunc:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
  rem Strtest:("String Functions")

	hRunTest%:("tHexStr")
  hRunTest%:("tPeekStr")
  hRunTest%:("tReptStr")
  hRunTest%:("tLowerStr")
  hRunTest%:("tUpperStr")
  hRunTest%:("tChrStr")
  hRunTest%:("tNumStr")
  hRunTest%:("tErrStr")
	hRunTest%:("tDateStr")
  hRunTest%:("tFixStr")
  hRunTest%:("tGenStr")
  hRunTest%:("tSlice")
  hRunTest%:("tDirN")
  hRunTest%:("tSciStr")

rem	hCleanUp%:("CleanUp")
rem	KLog%:(KhLogHigh%,"Some sample text")
endp



REM--------------------------------------------------------------------------
proc tSlice:
	tMid:
	tLeft:
	tRight:
endp


proc tMid:
    local a$(255),i%,j%
    rem SubTest:("MID$ test")

    rem print mid$("123",1,0)
    rem print mid$("123",1,1)
    rem print mid$("123",1,2)
    rem print mid$("123",1,3)
    rem print mid$("123",1,4)
    if mid$("123",1,0)<>"" :raise 1 :endif
    if mid$("123",1,1)<>"1" :raise 2 :endif
    if mid$("123",1,2)<>"12" :raise 3 :endif
    if mid$("123",1,3)<>"123" :raise 4 :endif
    if mid$("123",1,4)<>"123" :raise 5 :endif

    rem print mid$("123",2,0)
    rem print mid$("123",2,1)
    rem print mid$("123",2,2)
    rem print mid$("123",2,3)
    if mid$("123",2,0)<>"" :raise 6 :endif
    if mid$("123",2,1)<>"2" :raise 7 :endif
    if mid$("123",2,2)<>"23" :raise 8 :endif
    if mid$("123",2,3)<>"23" :raise 9 :endif

    rem print mid$("123",3,0)
    rem print mid$("123",3,1)
    rem print mid$("123",3,2)
    if mid$("123",3,0)<>"" :raise 10 :endif
    if mid$("123",3,1)<>"3" :raise 11 :endif
    if mid$("123",3,2)<>"3" :raise 12 :endif

    rem print mid$("123",4,0)
    rem print mid$("123",4,1)
    if mid$("123",4,0)<>"" :raise 13 :endif
    if mid$("123",4,1)<>"" :raise 14 :endif

    if mid$("",1,1)<>"" :raise 100 :endif

    onerr e1::
    print mid$("123",0,1)
    onerr off
    raise 16
e1::
    onerr off
    if err<>KErrInvalidArgs% :raise 17 :endif

    onerr e2::
    print mid$("123",-1,1)
    onerr off
    raise 18
e2::
    onerr off
    if err<>KErrInvalidArgs% :raise 19 :endif

    onerr e3::
    print mid$("123",1,-1)
    onerr off : raise 20
e3::
    onerr off
    if err<>KErrInvalidArgs%
        raise 21
    endif

cont::
    a$="1"+rept$("X",253)+"2"
    if mid$(a$,1,1)<>"1"
      raise 22
    endif
    if mid$(a$,255,1)<>"2"
      raise 23
    endif
    if mid$(a$,2,253)<>rept$("X",253)
      raise 24
    endif
    if mid$(a$,1,255)<>a$
      raise 25
    endif
    if mid$(a$,1,32766)<>a$
      raise 26
    endif
endp


proc tRight:
    local a$(255)
    rem SubTest:("RIGHT$ test")

    rem print right$("123",0)
    rem print right$("123",1)
    rem print right$("123",2)
    rem print right$("123",3)
    rem print right$("123",4)
    if right$("123",0)<>"" :raise 1 :endif
    if right$("123",1)<>"3" :raise 2 :endif
    if right$("123",2)<>"23" :raise 3 :endif
    if right$("123",3)<>"123" :raise 4 :endif
    if right$("123",4)<>"123" :raise 5 :endif

    if right$("",1)<>"" :raise 100 :endif

    onerr e3::
    print right$("123",-1)
    onerr off : raise 20
e3::
    onerr off
    if err<>KErrInvalidArgs%
        raise 21
    endif

cont::
    a$="1"+rept$("X",253)+"2"
    if right$(a$,1)<>"2"
      raise 22
    endif
    if right$(a$,255)<>a$
      raise 23
    endif
    if right$(a$,32766)<>a$
      raise 24
    endif
endp


proc tLeft:
    local a$(255)

    rem SubTest:("LEFT$ test")

    rem print left$("123",0)
    rem print left$("123",1)
    rem print left$("123",2)
    rem print left$("123",3)
    rem print left$("123",4)
    if left$("123",0)<>"" :raise 1 :endif
    if left$("123",1)<>"1" :raise 2 :endif
    if left$("123",2)<>"12" :raise 3 :endif
    if left$("123",3)<>"123" :raise 4 :endif
    if left$("123",4)<>"123" :raise 5 :endif

    if left$("",1)<>"" :raise 100 :endif

    onerr e3::
    print left$("123",-1)
    onerr off : raise 20
e3::
    onerr off
    if err<>KErrInvalidArgs%
        raise 21
    endif

cont::
    a$="1"+rept$("X",253)+"2"
    if left$(a$,1)<>"1"
      raise 22
    endif
    if left$(a$,255)<>a$
      raise 23
    endif
    if left$(a$,32766)<>a$
      raise 24
    endif
endp


REM--------------------------------------------------------------------------
proc tDirN:
  local n$(128),spec$(130),num%,fName1$(12),fName2$(12)
  rem subTest:("Directory Test")

  spec$="*.xyz"
	trap delete "*.xyz"
  fName1$="dir1.xyz"
  fName2$="dir2.xyz"

	lopen fName1$
	lprint fName1$
	lclose
	lopen fName2$
	lprint fName2$
	lclose

  rem print "Doing directory of",spec$
  n$=dir$(spec$)
  num%=0
  while n$<>""
    rem print n$
    num%=num%+1
    n$=dir$("")
  endwh

  if num%=2
    rem print "Completed OK"
  else
    rem print num%,"files found" :beep 3,1000 :get
    raise 1
  endif
	delete fName1$
	delete fName2$
  return
endp


REM-------------------------------------------------------------------------
proc tDateStr:
  local second%,minute%,hour%,day%,dayN$(10),monthN$(20),date$(30),month%,year%
  local bldDate$(30),dayInM$(2),day$(2),year$(6),hour$(4),minute$(4),second$(4)

  rem SubTest:("Date String Functions")
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
    rem beep 3,500
    rem print "Check tDate$:() : dates differ by at least 1 second"
    rem print "String slice the text time when time is available"
    rem pause pause% :key
    raise 1
  endif

  onerr e1::
	print dayName$(0)
	onerr off
  raise 2
e1::
	onerr off
  if err<>KErrInvalidArgs% :raise 3 :endif

  onerr e2::
	print dayName$(8)
	onerr off
  raise 4
e2::
	onerr off
  if err<>KErrInvalidArgs% :raise 5 :endif

  onerr e3::
	print dayName$(257)
	onerr off
  raise 6
e3::
	onerr off
  if err<>KErrInvalidArgs% :raise 7 :endif

  onerr e4::
	print dayName$(-1)
	onerr off
  raise 8
e4::
	onerr off
  if err<>KErrInvalidArgs% :raise 9 :endif

  onerr e5::
	print month$(0)
	onerr off
  raise 10
e5::
	onerr off
  if err<>KErrInvalidArgs% :raise 11 :endif

  onerr e6::
	print month$(13)
	onerr off
  raise 12
e6::
	onerr off
  if err<>KErrInvalidArgs% :raise 13 :endif

  onerr e7::
	print month$(257)
	onerr off
  raise 14
e7::
	onerr off
  if err<>KErrInvalidArgs% :raise 15 :endif

  onerr e8::
	print month$(-1)
	onerr off
  raise 16
e8::
	onerr off
  if err<>KErrInvalidArgs% :raise 17 :endif
endp

REM-------------------------------------------------------------------------

proc tErrStr:
    onerr e1::
    print 1/0
    onerr off : raise 1
e1::
    onerr off
    if err$(err)<>"Divide by zero"
      raise 2
    endif
endp

REM-------------------------------------------------------------------------

proc tSciStr:
  local a$(255),name$(10)

  name$="SCI$ test"
  rem SubTest:(name$)

  a$=sci$(123,2,8)
  rem print "sci$(123,2,8)->";a$
  testErr:(a$,"1.23E+02",1,name$)

  a$=sci$(123,2,-8)
  rem print "sci$(123,2,-8)->";a$
  testErr:(a$,"1.23E+02",2,name$)

  a$=sci$(123,2,-20)
  rem print "sci$(123,2,-20)->";a$
  testErr:(a$,"            1.23E+02",3,name$)

  a$=sci$(123,2,7)
  rem print "sci$(123,2,7)->";a$
  testErr:(a$,"*******",4,name$)

  a$=sci$(123,0,5)
  rem print "sci$(123,0,5)->";a$
  testErr:(a$,"1E+02",5,name$)

  a$=sci$(123,0,4)
  rem print "sci$(123,0,4)->";a$
  testErr:(a$,"****",6,name$)

  a$=sci$(123,128,3)
  rem print "sci$(123,128,3)->";a$
  testErr:(a$,"***",7,name$)

  a$=sci$(-123.456,2,9)
  rem print "sci$(-123.456,2,9)->";a$
  testErr:(a$,"-1.23E+02",8,name$)

  a$=sci$(-123.456,2,8)
  rem print "sci$(-123.456,2,8)->";a$
  testErr:(a$,"********",9,name$)

  a$=sci$(-123.456,0,6)
  rem print "sci$(-123.456,0,6)->";a$
  testErr:(a$,"-1E+02",10,name$)

  a$=sci$(-123.456,0,5)
  rem print "sci$(-123.456,0,5)->";a$
  testErr:(a$,"*****",11,name$)

  a$=sci$(-123.456,128,3)
  rem print "sci$(-123.456,128,3)->";a$
  testErr:(a$,"***",12,name$)

  a$=sci$(0.000123456,2,-8)
  rem print "sci$(0.000123456,2,-8)->";a$
  testErr:(a$,"1.23E-04",13,name$)

  a$=sci$(0.000123456,2,-10)
  rem print "sci$(0.000123456,2,-10)->";a$
  testErr:(a$,"  1.23E-04",14,name$)

  a$=sci$(-0.000123456,2,-9)
  rem print "sci$(-0.000123456,2,-9)->";a$
  testErr:(a$,"-1.23E-04",15,name$)

  a$=sci$(-0.000123456,2,-8)
  rem print "sci$(-0.000123456,2,-8)->";a$
  testErr:(a$,"********",16,name$)
  
	a$=sci$(1.7976931348623157e308,14,30)
  rem print "sci$(1.7976931348623157e308,14,30)->";a$
  testErr:(a$,"1.79769313486232E+308",17,name$)
  
  a$=sci$(1.7976931348623157e308,14,-30)
  rem print "sci$(1.7976931348623157e308,14,-30)->";a$
  testErr:(a$,"         1.79769313486232E+308",18,name$)
  
  a$=sci$(1.23456789E200,5,16)
  rem print "sci$(1.23456789E200,5,16)->";a$
  testErr:(a$,"1.23457E+200",19,name$)
  
  a$=sci$(1.23456789E200,5,-16)
  rem print "sci$(1.23456789E200,5,-16)->";a$
  testErr:(a$,"    1.23457E+200",20,name$)

  a$=sci$(1E100,0,10)
  rem print "sci$(1E100,0,10)->";a$
  testErr:(a$,"1E+100",21,name$)
  
  a$=sci$(1E100,0,-10)
  rem print "sci$(1E100,0,-10)->";a$
  testErr:(a$,"    1E+100",22,name$)

	a$=sci$(2.2250738585072015e-308,14,30)
  rem print "sci$(2.2250738585072015e-308,14,30)->";a$
  testErr:(a$,"2.22507385850720E-308",23,name$)
  
  a$=sci$(2.2250738585072015e-308,14,-30)
  rem print "sci$(2.2250738585072015e-308,14,-30)->";a$
  testErr:(a$,"         2.22507385850720E-308",24,name$)
  
  a$=sci$(1.23456789E-200,5,16)
  rem print "sci$(1.23456789E-200,5,16)->";a$
  testErr:(a$,"1.23457E-200",25,name$)
  
  a$=sci$(1.23456789E-200,5,-16)
  rem print "sci$(1.23456789E-200,5,-16)->";a$
  testErr:(a$,"    1.23457E-200",26,name$)

  a$=sci$(1E-100,0,10)
  rem print "sci$(1E-100,0,10)->";a$
  testErr:(a$,"1E-100",27,name$)
  
  a$=sci$(1E-100,0,-10)
  rem print "sci$(1E-100,0,-10)->";a$
  testErr:(a$,"    1E-100",28,name$)

  a$=sci$(5E-324,0,10)
  rem print "sci$(5E-324,0,10)->";a$
  testErr:(a$,"5E-324",29,name$)
  
  a$=sci$(5E-324,0,-10)
  rem print "sci$(5E-324,0,-10)->";a$
  testErr:(a$,"    5E-324",28,name$)
endp


proc tFixStr:
  local a$(255),name$(12)

  name$="FIX$ test"
  rem SubTest:(name$)

  a$=fix$(123456.127,2,9)
  rem print "fix$(123456.127,2,9)->";a$
  testErr:(a$,"123456.13",1,name$)

  a$=fix$(1,2,-5)
  rem print "fix$(1,2,-5)->";a$
  testErr:(a$," 1.00",2,name$)

  a$=fix$(1.234,2,3)
  rem print "fix$(123,2,7)->";a$
  testErr:(a$,"***",4,name$)

  a$=fix$(123,0,5)
  rem print "fix$(123,0,5)->";a$
  testErr:(a$,"123",5,name$)

  a$=fix$(123,0,2)
  rem print "fix$(123,0,2)->";a$
  testErr:(a$,"**",6,name$)

  a$=fix$(123,128,3)
  rem print "fix$(123,128,3)->";a$
  testErr:(a$,"***",7,name$)

  a$=fix$(-123.456,2,9)
  rem print "fix$(-123.456,2,9)->";a$
  testErr:(a$,"-123.46",8,name$)

  a$=fix$(-123.456,2,6)
  rem print "fix$(-123.456,2,6)->";a$
  testErr:(a$,"******",9,name$)

  a$=fix$(0.000123456,10,-13)
  rem print "fix$(0.000123456,10,-13)->";a$
  testErr:(a$," 0.0001234560",10,name$)

  a$=fix$(-0.000123456,10,-12)
  rem print "fix$(-0.000123456,10,-12)->";a$
  testErr:(a$,"************",11,name$)
  
  	a$=fix$(1.7976931348623157e308,14,30)
  rem print "fix$(1.7976931348623157e308,14,30)->";a$
  testErr:(a$,"******************************",12,name$)
  
  a$=fix$(1.7976931348623157e308,14,-30)
  rem print "fix$(1.7976931348623157e308,14,-30)->";a$
  testErr:(a$,"******************************",13,name$)
  
  a$=fix$(1.23456789E200,5,16)
  rem print "fix$(1.23456789E200,5,16)->";a$
  testErr:(a$,"****************",14,name$)
  
  a$=fix$(1.23456789E200,5,-16)
  rem print "fix$(1.23456789E200,5,-16)->";a$
  testErr:(a$,"****************",15,name$)

  a$=fix$(1E100,0,10)
  rem print "fix$(1E100,0,10)->";a$
  testErr:(a$,"**********",16,name$)
  
  a$=fix$(1E100,0,-10)
  rem print "fix$(1E100,0,-10)->";a$
  testErr:(a$,"**********",17,name$)
  
	a$=fix$(2.2250738585072015e-308,14,30)
  rem print "fix$(2.2250738585072015e-308,14,30)->";a$
  testErr:(a$,"0.00000000000000",18,name$)
  
  a$=fix$(2.2250738585072015e-308,14,-30)
  rem print "fix$(2.2250738585072015e-308,14,-30)->";a$
  testErr:(a$,"              0.00000000000000",19,name$)
  
  a$=fix$(1.23456789E-200,5,16)
  rem print "fix$(1.23456789E-200,5,16)->";a$
  testErr:(a$,"0.00000",20,name$)
  
  a$=fix$(1.23456789E-200,5,-16)
  rem print "fix$(1.23456789E-200,5,-16)->";a$
  testErr:(a$,"         0.00000",21,name$)

  a$=fix$(1E-100,0,10)
  rem print "fix$(1E-100,0,10)->";a$
  testErr:(a$,"0",22,name$)
  
  a$=fix$(1E-100,0,-10)
  rem print "fix$(1E-100,0,-10)->";a$
  testErr:(a$,"         0",23,name$)

  a$=fix$(5E-324,0,10)
  rem print "fix$(5E-324,0,10)->";a$
  testErr:(a$,"0",24,name$)
  
  a$=fix$(5E-324,0,-10)
  rem print "fix$(5E-324,0,-10)->";a$
  testErr:(a$,"         0",25,name$)
endp


proc tNumStr:
	global raiseNo%
  local name$(12)
  
  name$="NUM$ test"
  rem SubTest:(name$)
	numChk$:(1.1,6,"1",name$)
	numChk$:(1.1,-6,"     1",name$)
	numChk$:(-1.1,6,"-1",name$)
	numChk$:(-1.1,-6,"    -1",name$)
	numChk$:(1.1,1,"1",name$)
	numChk$:(-1.1,2,"-1",name$)
  numChk$:(1.1e99,6,"******",name$)
  numChk$:(-1.1e99,6,"******",name$)
  numChk$:(1.1e-99,-6,"     0",name$)
  numChk$:(-1.1e-99,-6,"     0",name$)
  numChk$:(1.5,6,"2",name$)
  numChk$:(-1.5,6,"-2",name$)
  numChk$:(1.6,6,"2",name$)
  numChk$:(-1.6,6,"-2",name$)
  numChk$:(1.4,6,"1",name$)
  numChk$:(-1.4,6,"-1",name$)
  numChk$:(1.0,0,"",name$)
  numChk$:(-1.0,0,"",name$)
  numChk$:(1.797E308,5,"*****",name$)
  numChk$:(1E100,6,"******",name$)
   
	onerr e1
	print num$(1,256)
	onerr off
	raise 100
e1::
	onerr e2
	print num$(1,65535)
	onerr off
	raise 101
e2::

endp


proc numChk$:(dVal,width%,res$,name$)
	local a$(255)

	raiseNo%=raiseNo%+1
  a$=num$(dVal,width%)
  rem print "num$(";dVal;",";width%;") ->",a$
  testErr:(a$,res$,raiseNo%,name$)
endp


proc tGenStr:
  local a$(255),name$(10)

  name$="GEN$ test"
  rem SubTest:(name$)

  a$=gen$(1.1,6)
  rem print "gen$(1.1,6) ->",a$
  testErr:(a$,"1.1",1,name$)

  a$=gen$(-1.1,6)
  rem print "gen$(-1.1,6) ->",a$
  testErr:(a$,"-1.1",2,name$)

  a$=gen$(1.1e99,7)
  rem print "gen$(1.1e99,7) ->",a$
  testErr:(a$,"1.1E+99",3,name$)

  a$=gen$(-1.1e-99,-9)
  rem print "gen$(-1.1e-99,-9) ->",a$
  testErr:(a$," -1.1E-99",4,name$)

  a$=gen$(10000.0,5)
  rem print "gen$(10000.0,5) ->",a$
  testErr:(a$,"10000",5,name$)

  a$=gen$(1.1e5,-8)
  rem print "gen$(1.1e5,-8) ->",a$
  testErr:(a$,"  110000",6,name$)

  a$=gen$(10000.1,7)
  rem print "gen$(10000.1,7) ->",a$
  testErr:(a$,"10000.1",7,name$)
  
  a$=gen$(1.7976931348623157e308,30)
  rem print "gen$(1.7976931348623157e308,30)->";a$
  testErr:(a$,"1.79769313486232E+308",8,name$)
  
  a$=gen$(1.7976931348623157e308,-30)
  rem print "gen$(1.7976931348623157e308,-30)->";a$
  testErr:(a$,"         1.79769313486232E+308",9,name$)
  
  a$=gen$(1.23456789E200,12)
  rem print "gen$(1.23456789E200,12)->";a$
  testErr:(a$,"1.23457E+200",10,name$)
  
  a$=gen$(1.23456789E200,-12)
  rem print "gen$(1.23456789E200,-12)->";a$
  testErr:(a$,"1.23457E+200",11,name$)

  a$=gen$(1E100,10)
  rem print "gen$(1E100,10)->";a$
  testErr:(a$,"1E+100",12,name$)
  
  a$=gen$(1E100,-10)
  rem print "gen$(1E100,-10)->";a$
  testErr:(a$,"    1E+100",13,name$)

	a$=gen$(2.2250738585072015e-308,30)
  rem print "gen$(2.2250738585072015e-308,30)->";a$
  testErr:(a$,"2.2250738585072E-308",14,name$)
  
  a$=gen$(2.2250738585072015e-308,-30)
  rem print "gen$(2.2250738585072015e-308,-30)->";a$
  testErr:(a$,"          2.2250738585072E-308",15,name$)
  
  a$=gen$(1.23456789E-200,12)
  rem print "gen$(1.23456789E-200,12)->";a$
  testErr:(a$,"1.23457E-200",16,name$)
  
  a$=gen$(1.23456789E-200,-12)
  rem print "gen$(1.23456789E-200,-12)->";a$
  testErr:(a$,"1.23457E-200",17,name$)

  a$=gen$(1E-100,10)
  rem print "gen$(1E-100,10)->";a$
  testErr:(a$,"1E-100",18,name$)
  
  a$=gen$(1E-100,-10)
  rem print "gen$(1E-100,-10)->";a$
  testErr:(a$,"    1E-100",19,name$)

	hlog%:(KhLogalways%,"ERROR !!TODO defect EDNRANS-4KDEFM Skipping rounding errors.")

  a$=gen$(5E-324,10)
  rem print "gen$(5E-324,10)->";a$
  testErr:(a$,"5E-324",20,name$)
  
  a$=gen$(5E-324,-10)
  rem print "gen$(5E-324,-10)->";a$
  testErr:(a$,"    5E-324",21,name$)
endp


proc testErr:(conv$,expect$,err%,name$)
	if conv$<>expect$
		hLog%:(khLogAlways%,"ConvertedResult=["+conv$+"]")
		hLog%:(khLogAlways%,"Expected result=["+expect$+"]")
		raise err
		rem print "Error in",name$
		rem pause 50
	endif
endp

REM--------------------------------------------------------------------------

proc tChrStr:
	rem SubTest:("CHR$ test")
	if chr$($31)<>"1" :raise 1 :endif
	REM No longer true on Unicode system
	REM if chr$($131)<>"1" :raise 2 :endif
endp

REM--------------------------------------------------------------------------

proc tHexStr:
	local h$(10)

	rem SubTest:("HEX$ test")
	h$ = hex$(0)
	if h$ <> "0"
		raise 1
  endif

	h$ = hex$($ff)
	if h$ <> "FF"
		raise 2
  endif

	h$ = hex$(&ffff)
	if h$ <> "FFFF"
		raise 3
  endif

	h$ = hex$(&0ffff)
	if h$ <> "FFFF"
		raise 4
  endif

	h$ = hex$(&fedcba98)
	if h$ <> "FEDCBA98"
		raise 5
  endif

endp

REM--------------------------------------------------------------------------

proc tLowerStr:
  local l$(255)

  rem SubTest:("LOWER$ test")
  l$=lower$("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
  if l$<>"abcdefghijklmnopqrstuvwxyz"
		raise 1
	endif
  if lower$("")<>"" :raise 2 :endif
  l$=rept$("A",255)
  if lower$(l$)<>rept$("a",255) :raise 3 :endif
endp

REM--------------------------------------------------------------------------

proc tUpperStr:
  local u$(255)
  rem SubTest:("UPPER$ test")
  u$=upper$("abcdefghijklmnopqrstuvwxyz")
  if u$<>"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		raise 1
	endif
  if upper$("")<>"" :raise 2 :endif
  u$=rept$("a",255)
  if upper$(u$)<>rept$("A",255) :raise 3 :endif
endp


REM--------------------------------------------------------------------------

proc tReptStr:
	local s$(255),x$(25)
  rem SubTest:("REPT$ test")
	x$=rept$("X",25)
  if x$ <> "XXXXXXXXXXXXXXXXXXXXXXXXX"
		raise 1
	endif
  s$=rept$("X",255)
  if s$<>x$+x$+x$+x$+x$+x$+x$+x$+x$+x$+"XXXXX"
		raise 2
	endif
	if rept$("AB",3)<>"ABABAB"
		raise 3
	endif

  s$=rept$("X",1)
  if s$<>"X"
		raise 4
	endif

  s$=rept$("",1000)
  if s$<>""
		raise 5
	endif

	onerr e1::
	print rept$("A",256)
  onerr off :raise 6
e1::
  onerr off
  if err<>KErrStrTooLong%
		raise 7
	endif

	onerr e2::
	print rept$("AB",128)
  onerr off :raise 8
e2::
  onerr off
  if err<>KErrStrTooLong%
		raise 9
	endif

	s$ = rept$(rept$("ABCDE",10),5)+"12345"

	onerr e3::
	print rept$(rept$("ABCDE",10),5)+"123456"
  onerr off :raise 10
e3::
  onerr off
  if err<>KErrStrTooLong%
		raise 11
	endif

endp

REM-------------------------------------------------------------------------


proc tPeekStr:
	local s$(255),p&
	rem SubTest:("PEEK$ test")
	p&=addr(s$)
	s$="12345"
	if peek$(p&)<>s$
		raise 1
	endif
	s$=rept$("x",255)
	if peek$(p&)<>s$
		raise 2
	endif
	s$=rept$("y",254)
	if peek$(p&)<>s$
		raise 3
	endif
	s$="12345"+rept$("z",245)+"67890"
	if peek$(p&)<>s$
		raise 4
	endif
	s$=""
	if peek$(p&)<>s$
		raise 5
	endif
endp

REM End of tSFunc.tpl
