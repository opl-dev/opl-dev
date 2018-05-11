REM gOrigin.tpl
REM EPOC OPL automatic test code for gORIGIN.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gorigin", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gOrigin:
	global id1%,id2%
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogorigin")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	trap gclose id1%
	trap gclose id2%
ENDP


proc dogOrigin:
	guse 1
	gBox gWidth,gHeight
	rem at 2,2
	rem print "OriginX=",gOriginX
	rem at 2,3
	rem print "OriginY=",gOriginY
	rem hLog%:(KhLogAlways%,"about to create started.")

	id1%=gCreate(50,30,100,40,1)
	gBox gWidth,gHeight
	gFont 5
	gAt 2,13 :gPrint "OriginX=";gOriginX
	gAt 2,26 :gPrint "OriginY=";gOriginY
	rem pause -30 :key
	if gOriginX<>50 or gOriginY<>30 : raise 1 : endif
	id2%=gCreateBit(20,20)
	
	rem hLog%:(KhLogAlways%,"starting error handling.")

	onerr e1
	print gOriginX
	onerr off
	raise 2
e1::
	onerr off
	if err<>-119 :raise 3 :endif
	onerr e2::
	print gOriginY
	onerr off
	raise 4
e2::
	onerr off
	if err<>-119 :raise 5 : cls : print err,err$(err) :get :raise 5 :endif
	rem cls : print "Done"
	rem pause -30
endp

REM End of gOrigin.tpl
