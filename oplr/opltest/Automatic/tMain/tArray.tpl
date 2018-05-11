REM tArray.tpl
REM EPOC OPL automatic test code for arrays.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tArray", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tArray:
  hRunTest%:("arrs")
  hRunTest%:("arri")
  hRunTest%:("arrl")
  hRunTest%:("arrd")
  hRunTest%:("arrErrs")
endp


proc arrs:
  local i%
  local x1$(40,100)
  local x2$(40,100)

  rem print "TESTING STRING ARRAYS"

  x1$(1)="1"
  x1$(2)="999"
  if x1$(1)<>"1"       : raise 1 :endif
  if x1$(2)<>"999"     : raise 2 :endif

  i%=1
  while i%<=40
    x1$(i%)=chr$(%a+i%)
    i%=i%+1
  endwh

  x2$(1)="next array"

  i%=1
  while i%<=40
rem    print x1$(i%),
    if x1$(i%)<>chr$(%a+i%)
      print
      print "PANIC in arrs: x1$(";i%;")=",x1$(i%)
      get
      raise 3
    endif
    i%=i%+1
  endwh
  rem print
endp

proc arri:
  local i%
  local x1%(40)
  local x2%(40)

rem  print "  TESTING INTEGER ARRAYS"

  x1%(1)=$1
  x1%(2)=$7fff
  if x1%(1)<>$1        : raise 1 :endif
  if x1%(2)<>$7fff     : raise 2 :endif

  i%=1
  while i%<=40
    x1%(i%)=i%
    i%=i%+1
  endwh

  x2%(1)=100

  i%=1
  while i%<=40
    rem print x1%(i%),
    if x1%(i%)<>i%
      print
      print "PANIC in arri: x1%(";i%;")=",x1%(i%)
      raise 3
    endif
    i%=i%+1
  endwh

rem  print
endp

proc arrl:
  local i&
  local x1&(40)
  local x2&(40)

rem  print "TESTING LONG INTEGER ARRAYS"

  x1&(1)=&1
  x1&(2)=&7fffffff
  if x1&(1)<>&1        : raise 1 :endif
  if x1&(2)<>&7fffffff : raise 2 :endif

  i&=1
  while i&<=40
    x1&(i&)=i&
    i&=i&+1
  endwh

  x2&(1)=100

  i&=1
  while i&<=40
rem    print x1&(i&),
    if x1&(i&)<>i&
      print
      print "PANIC in arrs: x1&(";i&;")=",x1&(i&)
      raise 3
    endif
    i&=i&+1
  endwh

rem  print
endp


proc arrd:
  local i%
  local x1(40)
  local x2(40)

rem  print "TESTING DOUBLE ARRAYS"

  x1(1)=1
  x1(2)=9e99
  if x1(1)<>1          : raise 1 :endif
  if x1(2)<>9e99       : raise 2 :endif

  i%=1
  while i%<=40
    x1(i%)=i%
    i%=i%+1
  endwh

  x2(1)=100

  i%=1
  while i%<=40
rem    print x1(i%),
    if x1(i%)<>i%
      print
      print "PANIC in arrs: x1(";i%;")=",x1(i%)
      raise 3
    endif
    i%=i%+1
  endwh

rem  print
endp

REM---------------------------------------------------------------------------
REM Array errors

proc arrErrs:
  local i%(2),l&(2),d(2),s$(2,3)

rem  print " Testing Array Error Handling"
  onerr err1::
  i%(3)=10
  raise 1
 err1::
  onerr off
  if err<>KErrSubs%
  	print err$(err)
  	rem get
  	raise 101
  endif

  onerr err2::
  l&(3)=10
  raise 2
 err2::
  onerr off
  if err<>KErrSubs%
  	print err$(err)
  	rem get
  	raise 103
  endif

  onerr err3::
  d(3)=10
  raise 104
 err3::
  onerr off
  if err<>KErrSubs%
  	print err$(err)
  	rem get
  	raise 105
  endif

  onerr err4::
  s$(3)="10"
  raise 106
 err4::
  onerr off
  if err<>KErrSubs%
  	print err$(err)
  	rem get
  	raise 107
  endif
endp


PROC tArrayLoadmTest:
	IF tArrayLoadmTest01:<>10
		RAISE 1000
	ENDIF
ENDP

PROC tArrayLoadmTest01:
	RETURN 10
ENDP

REM End of tArray.tpl
