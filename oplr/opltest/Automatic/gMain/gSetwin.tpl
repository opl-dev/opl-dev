REM gSetwin.tpl
REM EPOC OPL automatic test code for gsetwin.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gsetwin", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gsetwin:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogsetwin")
rem	hCleanUp%:("CleanUp")
endp


proc dogSetwin:
	local id%,iScrX%,iScrY%,gScrX%,gScrY%,fact,times%

	fact=2
	iScrX%=gWidth :iScrY%=gHeight
	gScrX%=10 :gScrY%=4
	id%=gCreate(iScrX%/2+2,iScrY%/2,gScrX%,gScrY%,1)

	while times%<20
		times%=times%+1
		doBox:
		rem pause -10
		rem if key=27 :break :endif
		gScrX%=gScrX%*fact
		gScrY%=gScrY%*fact
		gSetWin (iScrX%-gScrX%)/2+2,(iScrY%-gScrY%)/2,gScrX%,gScrY%
		if  gScrX%<=4 or gScrY%<=4
			fact=2
		elseif gScrX%>iScrX% or gScrY%>iScrY%
			fact=0.5
		endif			
	endwh
	doBox:
	rem print "Done"
	rem pause -50 :key
	gclose id%
	gcls
endp


proc doBox:
		gAt 0,0
		gBox gWidth,gHeight
		gAt 1,gHeight-2
		gprint "THIS IS THE 2nd WINDOW"
endp

REM End of gsetwin.tpl
