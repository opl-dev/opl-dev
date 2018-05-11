REM gXPrint.tpl
REM EPOC OPL automatic test code for gXPRINT.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gxprint", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gxprint:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogxprint")
	rem hCleanUp%:("CleanUp")
endp


proc dogxprint:
	local flags%,erNum%(12)

	gStyle 2	: rem underline
	rem at 2,5 : print "gXPrint Test"
	gAt gWidth/2,20	
	flags%=-1
	do
		onerr errh
		gXPrint "flags="+num$(flags%,1),flags%
		onerr off
		gMove 0,20
loop::
		flags%=flags%+1
	until flags%>=8
	onerr off
	rem print "Errors with flags="
	flags%=-1
	do
		if erNum%(flags%+2)
			rem print flags%,
			if flags%>=0 and flags%<=6
				raise 1 :rem had error when flags in range
			endif
		elseif flags%<0 or flags%>6
			raise 2 :rem no error when flags out of range
		endif
		flags%=flags%+1
	until flags%>=8
	rem pause -30 :key
	gcls
	return
errH::
	erNum%(flags%+2)=err
	goto loop
endp

REM End of gXPrint.tpl
