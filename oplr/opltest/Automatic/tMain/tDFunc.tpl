REM tDFunc.tpl
REM EPOC OPL automatic test code for double funcs.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tDFunc", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tDFunc:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
  hRunTest%:("tVal")
  hRunTest%:("tPeekF")	
  hRunTest%:("tIntf")
  hRunTest%:("tPi")
  hRunTest%:("tAbs")
  hRunTest%:("tDeg")
  hRunTest%:("tRad")
  hRunTest%:("tDegRad")
	hCleanUp%:("CleanUp")
	rem KLog%:(KhLogHigh%,"Some sample text")
endp


proc CleanUp:
	trap lclose
	trap delete "val.txt"
endp


proc tVal:
	local v$(255),v,f%

rem	print "Fix v3.67:2"
rem	print
	lopen "val.txt"

	lprint "val(""123.4"")=";val("123.4")

	onerr e99::
	f%=1
	lprint "val("" 123.4"")=";val(" 123.4")
	f%=0
e99::
	if f% :raise err :endif

	onerr e98::
	f%=1
	lprint "val(""  123.4"")=";val("  123.4")
	f%=0
e98::
	if f% :raise err :endif

	onerr e97::
	f%=1
	v$=rept$(" ",252)+"1.2"
	lprint "val(";v$;")=";val(v$)
	f%=0
e97::
	if f% :raise err :endif

	onerr e96::
	f%=1
	v$=rept$(" ",254)+"1"
	lprint "val(";v$;")=";val(v$)
	f%=0
e96::
	if f% :raise err :endif

	onerr e0
	v$=rept$(" ",255)
	lprint "val(";v$;")=";val(v$)
	raise 100
	rem print "Panic! Error not detected!" :get
e0::
	onerr off
	rem print "Error detected ok:",err$(err)

	onerr e1
	lprint "val("""")=";val("")
	raise 101
	rem print "Panic! Error not detected!" :get
e1::
	onerr off
	rem print "Error detected ok:",err$(err)

	onerr e2
	lprint "val("" "")=";val(" ")
	raise 103
	rem print "Panic! Error not detected!" :get
e2::
	onerr off
	rem print "Error detected ok:",err$(err)
endp


proc tDeg:
	if DblNeq%:(deg(2*pi),360.0) :raise 1 :endif
	if DblNeq%:(deg(0),0.0) :raise 2 :endif
endp


proc tRad:
	if DblNeq%:(rad(360.0),2*pi) :raise 1 :endif
	if DblNeq%:(rad(0),0.0) :raise 2 :endif
endp


proc tAbs:
	if abs(0.0)<>0.0 :raise 1 :endif
	if abs(1.0)<>1.0 :raise 2 :endif
	if abs(9.99999999999999e99)<>9.99999999999999e99 :raise 3 :endif
  if abs(1.0e-99)<>1.0e-99 :raise 4 :endif
	if abs(-0.0)<>0.0 :raise 5 :endif
	if abs(-1.0)<>1.0 :raise 6 :endif
	if abs(-9.99999999999999e99)<>9.99999999999999e99 :raise 7 :endif
  if abs(-1.0e-99)<>1.0e-99 :raise 8 :endif
endp


proc tPi:
	if int(pi*10000)<>31415 :raise 1 :endif
	if pi<>3.14159265358979323846 :raise 2 :endif
endp


proc tPeekF:
		local d,p%
		p%=addr(d)
		d=0.0
		if peekF(p%)<>0.0      :raise 1 :endif
		d=9.99999999999999e99
		if peekF(p%)<>9.99999999999999e99  :raise 2 :endif
		d=-9.99999999999999e99
		if peekF(p%)<>-9.99999999999999e99  :raise 3 :endif
		d=1.0e-99
		if peekF(p%)<>1.0e-99  :raise 4 :endif
		d=-1.0e-99
		if peekF(p%)<>-1.0e-99  :raise 5 :endif
endp


proc tIntf:
	if intf(1.1)<>1.0 :raise 1 :endif
	if intf(32768.9)<>32768.0 :raise 2 :endif
	if intf(-32768.9)<>-32768.0 :raise 3 :endif
	if intf(9.99999999999999e99)<>9.99999999999999e99 :raise 4 :endif
	if intf(-9.9e99)<>-9.9e99 :raise 5 :endif
	if intf(1.0e-99)<>0.0 :raise 6 :endif
	if intf(-1.0e-99)<>0.0 :raise 7 :endif
	if intf(1234567890123.9)  <> 1234567890123.0 :raise 8 :endif
	if intf(-1234567890123.9) <> -1234567890123.0 :raise 9 :endif
endp


proc tdegrad:
	local i%,x,y
	randomize 1
	while i%<10000
		i%=i%+1
		x=rnd*360
		y=abs(deg(rad(x))-x)
		if y > 5e-13
			rem print i%;": PANIC: rad(deg(";x;"))-";x;") =",y
			rem get
			raise 1
		endif
		rem if i%/100*100=i%
		rem 	print i%
		rem endif
		rem if key
		rem 	if get=27 :return :endif
		rem endif
	endwh
endp


proc DblNeq%:(l,r)
	local precisn
  precisn=1e-10  :rem precision (the problem is taking the time to find the
                 :rem            true results to compare with opl's results)
  return abs(l-r)>=precisn
endp

REM End of tDFunc.tpl
