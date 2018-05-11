REM pDays.tpl
REM EPOC OPL automatic test code for DAYSTODATE
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pDays", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pDays:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tDate")
rem	hCleanUp%:("CleanUp")
endp


proc tDate:
	local d1&
	local day%,month%,year%
	local rep&
	d1&=60504 	rem 27/8/2065	
	daystodate d1&,year%,month%,day%
	if day%<>27 : rep&=rep&+1 :endif
	if month%<>8 : rep&=rep&+2 :endif
	if year%<>2065 : rep&=rep&+4 :endif
	if rep& : raise rep& : endif
endp

REM End of pDays.tpl
