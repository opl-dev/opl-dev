REM gprint.tpl
REM EPOC OPL automatic test code for gprint.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gprint", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP

proc gprint:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogprint")
endp

proc dogPrint:
	local i%
	local x%,y%,length%
	x%=0: y%=30 :length%=40

	reset:

	CLS
	IF hPeekLine&:(x%,y%,length%)
		RAISE 1
	ENDIF

	print "The following should wrap:"
	print rept$("1234567890",10)

	rem gat x%,y% :glineby length%,0
	
	IF hPeekLine&:(x%,y%,length%)
	ELSE
		RAISE 2
	ENDIF

	cls
	gFont 9		:rem small font???
	while i%<8
		i%=i%+1
		gAt 1,10*i%
		gPrint flt(i%)+.1,int(i%)+&8000,i%
	endwh
	gAt gWidth/2+5,gHeight/2
	gPrint "That's all"
	gCLS
endp


PROC Reset:
	DEFAULTWIN 1
	gStyle 0
	gGMode 0
	gTMode 0
	cls
	gCls
ENDP

REM End of gprint.tpl
