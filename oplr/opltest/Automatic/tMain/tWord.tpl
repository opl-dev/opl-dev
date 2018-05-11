REM tWord.tpl
REM EPOC OPL automatic test code for word opcodes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tWord", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tWord:
	global l%(50),r%(50),ix%
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("twPow")
rem	hCleanUp%:("CleanUp")
endp


proc tWpow:
	local i%,res%,res

	rem SubTest:("INTEGER POWER test")
	ix%=0
	setLR%:(1,1)      :rem l%(ix%)=1 : r%(ix%)=1 :ix%=ix%+1
	setLR%:(1,-1)
	setLR%:(-1,1)
	setLR%:(-1,-1)
	setLR%:(2 ,2)
	setLR%:(2 ,-2)
	setLR%:(-2,2)
	setLR%:(-2,-2)
	setLR%:(2,14)
	setLR%:(-2,15)
	setLR%:(181,2)
	setLR%:(32767,1)
	setLR%:($8000,1)
	setLR%:(-8,5)

	while i%<ix%
		i%=i%+1
		res%=ipow:(l%(i%),r%(i%))
		res=fpow:( flt(l%(i%)), flt(r%(i%)) )
		if res>0
			res=res+0.5
		else
			res=res-0.5
		endif
 		if int(res)<>res%
			raise i%
		endif
	endwh

	rem print "Check overflow detected correctly"
	setLR%:(2,16)
	setLR%:(256,2)
	setLR%:(-256,2)
	setLR%:(8,5)	 :rem 32768

  onerr e1::
	while i%<ix%
		i%=i%+1
		res%=ipow:(l%(i%),r%(i%))
		onerr off
		raise i%
e1::
		rem print err$(err)
		if err<>-6 :rem overflow
			onerr off
			raise i%
			rem print "PANIC on calculating ",l%(i%),"**",r%(i%)
			rem get
		else
			rem print "as required"
		endif
	endwh
endp


proc setLR%:(lx%,rx%)
	REM Increment global ix% and set global array elements l%(ix%) and r%(ix%)
	ix%=ix%+1
	l%(ix%)=lx%  :  r%(ix%)=rx%
endp


proc iPow:(l%,r%)
  local res%
	rem print "Integer power:",l%,"**",r%,"=",
	res%=l%**r%
  rem print res%
	return res%
endp


proc fPow:(l,r)
  local res
	rem print "Double power:",l,"**",r,"=",
	res=l**r
  rem print res
	return res
endp

REMEnd of tWord.tpl
