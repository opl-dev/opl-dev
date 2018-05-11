REM gBordi.tpl
REM EPOC OPL automatic test code for gborder.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gbordi", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gbordi:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogbordi")
rem	hCleanUp%:("CleanUp")
endp


proc dogBordI:
	local singleDouble% REM 1-4
	local onePixelGap% REM $100
	local roundCorners% REM $200
	local flag%
	
	roundCorners%=0
	do
		onePixelGap%=0
		do
			singleDouble%=0
			do
				REM Build the flag.
				flag%=0
				if roundCorners%
					flag%=flag% OR $200
				endif
				if onePixelGap%
					flag%=flag% OR $100
				endif
				flag%=flag% OR singleDouble%
				rem print HEX$(flag%)
				gat 10,10
				onerr SkipIt::
				gborder flag%
				gborder flag%,100,100
SkipIt::
				onerr off
				singleDouble%=singleDouble%+1
			until singleDouble%=6 REM tests the illegal '5' too.
			onePixelGap%=onePixelGap%+1
		until onePixelGap%=2	
		roundCorners%=roundCorners%+1
	until roundCorners%=2
	gCLS
ENDP


REM End of gbordi.tpl

