REM tLFunc.tpl 
REM EPOC OPL automatic test code for long functions.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tLFunc", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tLFunc:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tIabs")
	hRunTest%:("tDays")
	hRunTest%:("tInt")
	hRunTest%:("tPeekL")
	hRunTest%:("tSpace")
rem	hCleanUp%:("CleanUp")
endp


proc tInt:
		rem SubTest:("INT test")

		if int(32769.99999999999)<>32769   :raise 1 :endif
		if int(-32770.99999999999)<>-32770 :raise 2 :endif
		if int(0.0)<>0 : raise 3 :endif
		if int(-1.0)<>-1 : raise 4 :endif
		if int(1.0)<>1 : raise 5 :endif
		if int(2.0)<>2 : raise 6 :endif
		if int(3.0)<>3 : raise 7 :endif
		if int(4.0)<>4 : raise 8 :endif
		if int(5.0)<>5 : raise 9 :endif
		if int(6.0)<>6 : raise 10 :endif
		if int(7.0)<>7 : raise 11 :endif
		if int(8.0)<>8 : raise 12 :endif
		if int(9.0)<>9 : raise 13 :endif
		if int(10.0)<>10 : raise 14 :endif
endp


proc tDays:
		rem SubTest:("DAYS test")
    if days(1,1,1900)<>0
      raise 1
    endif
    if days(2,1,1900)<>1
      raise 2
    endif
endp


proc tIabs:
  local a&
  rem SubTest:("IABS Test")
  a&=iabs(32769)
  if a&<>32769 :raise 1 :print a& :endif
  a&=iabs(-32769)
  if a&<>32769 :raise 2 :print a& :endif
  a&=iabs(0)
  if a&<>0 :raise 3 :print a& :endif
  a&=iabs(1)
  if a&<>1 :raise 4 :print a& :endif
  a&=iabs(-1)
  if a&<>1 :raise 5 :print a& :endif
endp


proc tPeekL:
		local a&,p%
		rem SubTest:("PEEKL test")
		p%=addr(a&)
		a&=0
		if peekL(p%)<>0      :raise 1 :endif
		a&=32767
		if peekL(p%)<>32767  :raise 2 :endif
		a&=-32768
		if peekL(p%)<>-32768 :raise 3 :endif
		a&=32768
		if peekL(p%)<>32768  :raise 4 :endif
		a&=-32769
		if peekL(p%)<>-32769 :raise 5 :endif
endp


proc tSpace:
	local size&
	rem SubTestX:("SPACE test")
	trap delete "space.odb"
	create "space.odb",a,a$
	size&=space
	close
	delete "space.odb"

	trap close  :rem close all database files
	trap close
	trap close

	onerr e1::
	size&=space
	onerr off
	raise 1 :rem space should fail when no files are open
e1::
	onerr off
endp

REM End of tLFunc.tpl
