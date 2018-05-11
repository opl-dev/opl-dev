REM gFill.tpl
REM EPOC OPL automatic test code for gFILL.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gfill", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gFill:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("fill1")
	hRunTest%:("fill2")

rem	hCleanUp%:("CleanUp")
endp


proc fill1:
	local idBit%,idWin%,winW%

	gCreate(0,0,0,0,0) :rem for gTWidth only
	gTMode 1
	winW%=max(gTWidth("Filled created window")+4,160)
	gClose gIdentity
	idWin%=	gCreate(0,0,winW%,80,1)
	gFill gWidth,gHeight,0			:rem fill created window
	gTMode 1
	gAt 2,30
	gPrint "Filled created window"
	gcls
	idBit%=gCreateBit(winW%,80)
	gFill winW%,80,0
	gTMode 1
	gAt 2,30
	gPrint "Filled bitmap"
	gUse idWin%
	gCopy idBit%,0,0,winW%,80,0
	gclose idwin%
	gclose idbit%
endp


proc fill2:
	local w%,h%,fw%,fh%,i%
	
	gUpdate off
	gUse 1
	fW%=40 		: fH%=20	
	w%=gWidth :	h%=gHeight
	gOrder 1,1
	gCls
	rem at 2,1
	rem print "Filled default window"
	rem while key<>27 and i%<1000
	while i%<1000
		i%=i%+1
		gAt rnd*(w%-fW%-8)+4,rnd*(h%-fH%-12)+10
		gFill fW%,fH%,rnd*3
	endwh
	gcls
	rem print "Done"
	rem pause pause%
endp

REM End of gFill.tpl
