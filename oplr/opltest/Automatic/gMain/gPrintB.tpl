REM gPrintB.tpl
REM EPOC OPL automatic test code for gprintb.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gprintb", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gprintb:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogprintb")
	rem hCleanUp%:("CleanUp")
endp


proc dogprintb:
	local margin%,align%,id2%,s$(255),wideFlg%
	
	id2%=gCreate(0,0,gWidth,gHeight,1)
	gfont 10

	margin%=-8
	gFill gWidth,gHeight,0
	while margin%<8
		margin%=margin%+4
		gAt 3,14 : gTMode 1
		gPrint "MARGIN="
		gAt 50+150*(margin%/4+1),14
		gPrint margin%
		align%=0
		while align%<3
			align%=align%+1
			gTMode 1
			gAt 5,20+align%*16
			if align%=1
				gPrint "right"
			elseif align%=2
				gPrint "left"
			else
				gPrint "centre"
			endif
			gAt 50+150*(margin%/4+1),20+align%*16
			s$="gPrintB(120,"+gen$(align%,1)+",0,0,"+gen$(margin%,2)+")"
			gPrintB s$,120,align%,0,0,margin%
		endwh
	endwh	
	gclose id2%
	gcls
endp


REM End of gprintb.tpl


