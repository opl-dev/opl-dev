REM tLongF.tpl
REM EPOC OPL automatic test code for more long ints.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tLongF", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tLongF:
	global l&(100),r&(100),ix%
  rem strtest:("LONG INTEGER tests")
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tLmul")
	hRunTest%:("tladd")
	hRunTest%:("tlpow")
endp


proc tLmul:
  local i%,res&,res
  rem SubTest:("Long integer multiplication test")
	ix%=0
	setLR&:(int(0),int(1))
	setLR&:(int(0),65536)
	setLR&:(int(0),65537)
	setLR&:(int(1),int(0))
	setLR&:(65536,int(0))
	setLR&:(65537,int(0))
	setLR&:(-65536,int(0))
	setLR&:(-65537,int(0))

	setLR&:(int(-1),int(1))
	setLR&:(int(1),int(-1))
	setLR&:(int(1),int(1))
	setLR&:(int(-1),int(-1))
	setLR&:(int(-2),int(2))
	setLR&:(int(2),int(-2))
	setLR&:(int(2),int(2))
	setLR&:(int(-2),int(-2))
	setLR&:(32768,int(-32767))
	setLR&:(-32768,int(32767))
	setLR&:(32768,int(32767))
	setLR&:(int(-32768),int(-32767))
	setLR&:(&80000000,int(1))    :rem maximum negative
	setLR&:(-1073741824,int(2))  :rem maximum negative
	setLR&:(int(-32768),int(-32768))
	setLR&:(32768,32768)
	setLR&:(65536,int(1))
	setLR&:(-65536,int(-1))
	setLR&:(65536,int(-1))
	setLR&:(-65536,int(1))
	setLR&:(int(1),65536)
	setLR&:(int(-1),-65536)
	setLR&:(int(-1),65536)
	setLR&:(int(1),-65536)
	setLR&:(&1234567,int(10))
	setLR&:(int(10),&1234567)
	setLR&:(-(&1234567),int(-10))
	setLR&:(int(-10),-(&1234567))
	setLR&:(int(10),-(&1234567))
	setLR&:(int(-10),(&1234567))
	setLR&:(65536,int(32767))

	while i%<ix%
		i%=i%+1
		res&=lmul&:(l&(i%),r&(i%))
		res=fmul:( flt(l&(i%)), flt(r&(i%)) )
 		if int(res)<>res&
			raise i%
		endif
	endwh

	rem print "Check overflow detected correctly"

	setLR&:(int(2),&7fffffff)
	setLR&:(&7fffffff,int(2))
	setLR&:(&80000000,int(-2))
	setLR&:(&80000000,int(-1))
	setLR&:(65536,32768)

  onerr e1::
	while i%<ix%
		i%=i%+1
		res&=lmul&:(l&(i%),r&(i%))
		onerr off
		raise i%
e1::
		rem print err$(err)
		if err<>-6 :rem overflow
			rem print "PANIC on calculating ",l&(i%),"*",r&(i%)
			get
			onerr off
			raise i%
		else
			rem print "as required"
		endif
	endwh
endp

proc tLadd:
  local i%,res&,res

  rem SubTest:("Long integer addition test")
	ix%=0
	setLR&:(int(-1),int(1))
	setLR&:(int(1),int(-1))
	setLR&:(int(1),int(1))
	setLR&:(int(-1),int(-1))
	setLR&:(int(-2),int(2))
	setLR&:(int(2),int(-2))
	setLR&:(int(2),int(2))
	setLR&:(int(-2),int(-2))
	setLR&:(32768,int(-32767))
	setLR&:(int(-32768),int(32767))
	setLR&:(32768,int(32767))
	setLR&:(-32768,int(-32767))
	setLR&:(&80000001,int(-1))              :rem maximum negative
	setLR&:(-1073741824,-1073741824)   :rem maximum negative
	setLR&:(int(-32768),int(-32768))
	setLR&:(32768,32768)
	setLR&:(65536,int(1))
	setLR&:(-65536,int(-1))
	setLR&:(65536,int(-1))
	setLR&:(-65536,int(1))
	setLR&:(int(1),65536)
	setLR&:(int(-1),-65536)
	setLR&:(int(-1),65536)
	setLR&:(int(1),-65536)
	setLR&:(&1234567,int(10))
	setLR&:(int(10),&1234567)
	setLR&:(-(&1234567),int(-10))
	setLR&:(int(-10),-(&1234567))
	setLR&:(int(10),-(&1234567))
	setLR&:(int(-10),(&1234567))
	setLR&:(65536,int(32767))
	setLR&:(int(&7fff),int(1))
	setLR&:(int(&7fff),int(&7fff))
	setLR&:(&08000,int(-1))
	setLR&:(&08000,&08000)
	setLR&:(&7ffffffe,int(1))
	setLR&:(&10000,&10000)
	setLR&:(&80000001,int(-1))
	setLR&:(&c0000000,&c0000000)

	while i%<ix%
		i%=i%+1
		res&=ladd&:(l&(i%),r&(i%))
		res=fadd:( flt(l&(i%)), flt(r&(i%)) )
 		if int(res)<>res&
			raise i%
		endif
	endwh

	rem print "Check overflow detected correctly"

	setLR&:(int(2),&7fffffff)
	setLR&:(&7fffffff,int(2))
	setLR&:(&80000000,int(-2))
	setLR&:(&80000000,int(-1))
	setLR&:(&7fffffff,&7fffffff)
	setLR&:(&40000000,&40000000)
	setLR&:(&c0000000,&bfffffff)
	setLR&:(&80000000,&80000000)

  onerr e1::
	while i%<ix%
		i%=i%+1
		res&=ladd&:(l&(i%),r&(i%))
		onerr off
		raise i%
e1::
		rem print err$(err)
		if err<>-6 :rem overflow
			rem print "PANIC on calculating ",l&(i%),"+",r&(i%)
			get
			onerr off
			raise i%
		else
			rem print "as required"
		endif
	endwh
endp

proc tLpow:
	local i%,res&,res

	rem SubTest:("LONG INTEGER POWER test")
	ix%=0
	setLR&:(int(0),int(1))
	setLR&:(int(0),65536)
	setLR&:(int(0),65537)
	setLR&:(int(1),int(0))
	setLR&:(65536,int(0))
	setLR&:(65537,int(0))
	setLR&:(-65536,int(0))
	setLR&:(-65537,int(0))

	setLR&:(int(1),int(1))      : rem l&(ix%)=1 : r&(ix%)=1 :ix%=ix%+1
	setLR&:(int(1),int(-1))
	setLR&:(int(-1),int(1))
	setLR&:(int(-1),int(-1))
	setLR&:(int(2 ),int(2))
	setLR&:(int(2 ),int(-2))
	setLR&:(int(-2),int(2))
	setLR&:(int(-2),int(-2))
	setLR&:(int(2),int(14))
	setLR&:(int(-2),int(15))
	setLR&:(int(2),int(30))
	setLR&:(int(-2),int(31))
	setLR&:(int(181),int(2))	:rem 181**2 ~ 32767
	setLR&:(int(-181),int(2))
	setLR&:(46340,int(2)) :rem 46340**2 ~ &7fffffff
	setLR&:(-46340,int(2))
	setLR&:(int(32767),int(1))
	setLR&:(int($8000),int(1))
	setLR&:(&7fffffff,int(1))
	setLR&:(&80000000,int(1))
	setLR&:(int(-8),int(5))
	setLR&:(int(8),int(5))
	setLR&:(int(-7),int(11))
	setLR&:(int(7),int(11))

	while i%<ix%
		i%=i%+1
		res&=lpow&:(l&(i%),r&(i%))
		res=fpow:( flt(l&(i%)), flt(r&(i%)) )
		if res>0
			res=res+0.5
		else
			res=res-0.5
		endif
 		if int(res)<>res&
			rem print "PANIC!" :get
			raise i%
		endif
	endwh

	rem print "Check overflow detected correctly"
	setLR&:(int(2),int(31))
	setLR&:(65536,int(2))
	setLR&:(-65536,int(2))

  onerr e1::
	while i%<ix%
		i%=i%+1
		res&=lpow&:(l&(i%),r&(i%))
		rem print "Overflow error NOT detected!"
		goto panic1::
e1::
		rem print err$(err)
		if err<>-6 :rem overflow
panic1::
			rem print "PANIC on calculating ",l&(i%),"**",r&(i%)
			get
			onerr off
			raise i%
		else
			rem print "as required"
		endif
	endwh

	rem print "Checking argument errors"
	setLR&:(int(0),int(0))
	setLR&:(int(0),int(-1))
	setLR&:(int(0),int(-32768))
	setLR&:(int(0),int(-32769))
	setLR&:(int(0),-65536)
	setLR&:(int(0),-65537)

  onerr e2::
	while i%<ix%
		i%=i%+1
		res&=lpow&:(l&(i%),r&(i%)) :rem drop thru if no error to panic
		rem print "Argument error NOT detected!"
		goto panic2::
e2::
		rem print err$(err)
		if err<>KErrInvalidArgs%
panic2::
			rem print "PANIC on calculating ",l&(i%),"**",r&(i%)
			get
			onerr off
			raise i%
		else
			rem print "as required"
		endif
	endwh
endp


REM -------------------------------------------------------------------------

proc lmul&:(l&,r&)
  local res&
	rem print "LONG   mult:",l&,"*",r&,"=",
	res&=l&*r&
  rem print res&
	return res&
endp


proc fmul:(l,r)
  local res
	rem print "DOUBLE mult:",l,"*",r,"=",
	res=l*r
  rem print res
	return res
endp


proc ladd&:(l&,r&)
  local res&
	rem print "LONG   addition",l&,"+",r&,"=",
	res&=l&+r&
  rem print res&
	return res&
endp


proc fadd:(l,r)
  local res
	rem print "DOUBLE addition:",l;".0 +",r;".0 =",
	res=l+r
  rem print res
	return res
endp


proc lpow&:(l&,r&)
  local res&
	rem print "LONG   power",l&,"**",r&,"=",
	res&=l&**r&
  rem print res&
	return res&
endp


proc fpow:(l,r)
  local res
	rem print "DOUBLE power",l,"**",r,"=",
	res=l**r
  rem print res
	return res
endp


proc setLR&:(lx&,rx&)
	REM Increment global ix% and set global array elements l&(ix%) and r&(ix%)
	ix%=ix%+1
	l&(ix%)=lx&  :  r&(ix%)=rx&
endp

REMEnd of tlongf.tpl

