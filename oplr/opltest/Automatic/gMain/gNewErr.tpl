REM gnewerr.tpl
REM EPOC OPL automatic test code for 'new' OPL errors.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gnewerr", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gnewerr:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("invWinEr")
	hRunTest%:("drwClsEr")
	rem hCleanUp%:("CleanUp")
endp

	
proc drwClsEr:
	onerr e1
	cursor 8
	onerr off
	raise 1
e1::
	onerr off
	if err<>-118
		raise 2
		rem print err,err$(err) :get :raise 2
	endif
	
	onerr e2
	guse 8
	onerr off
	raise 3
e2::
	onerr off
	if err<>-118
		raise 4
		rem print err,err$(err) :get :raise 4
	endif
endp


proc invWinEr:
	local id%
	id%=gCreatebit(10,10)
	onerr e1::
	gSetWin 10,10,20,20
	onerr off
	raise 1
e1::
	onerr off
	if err<>-119
		raise 2
	endif

	onerr e2::
	gVisible on
	onerr off
	raise 3
e2::
	onerr off
	if err<>-119
		raise 4
	endif

	onerr e3::
	gVisible off
	onerr off
	raise 5
e3::
	onerr off
	if err<>-119
		raise 6
	endif

	onerr e4::
	gorder gIdentity,1
	onerr off
	raise 3
e4::
	onerr off
	if err<>-119
		raise 4
		rem print err,err$(err) :get :raise 4
	endif
	gClose id%	
endp

REM End of gnewerr.tpl

