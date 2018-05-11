REM gCls.tpl
REM EPOC OPL automatic test code for gCLS.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("gCls", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gCls:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogCls")
	rem hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	rem
ENDP


proc dogCls:
	local idw2%
	local chrW%  rem character width in console
	local info%(10)
	local otherId%
	
	screeninfo info%()
	chrW%=info%(7)

rem	screen gWidth/chrW%-3,1,2,2
	gFill gWidth,gHeight,0
rem	print "Clearing Default window"
rem	pause -10 :key
	gCls
rem	pause -10 :key	

	gBox gWidth,26
	gAt 0,27
	gPatt -1,gWidth,gHeight,0
rem	pause -10
	idw2%=gCreate(1,28,gWidth-2,gHeight-29,1)
rem	print "Created window"
rem	pause -30 :key

	test:(0,idW2%)
	
	otherId%=gCreateBit(gWidth,gHeight)
rem	print "Created bitmap"
rem	pause -30 :key

	test:(1,idW2%)
	gclose idw2%
	gclose otherId%
	gCLS
endp


proc test:(bitFlg%,id%)
	local buf%(17)

	gOrder id%,0
	
	gFill gWidth,gHeight,0
	bitCpy:(bitFlg%,id%,gIdentity)
rem	print "Filled",
rem	if bitFlg% :print "and copied" :else :print :endif
rem	pause -30 :key
	
	gCls
	bitCpy:(bitFlg%,id%,gIdentity)
rem	print "gCls'd",
rem	if bitFlg% :print "and copied" :else :print :endif
rem	pause -30 :key

	gPeekLine gIdentity,0,0,buf%(),16
	if buf%(1)<>$0 :raise 1 :endif
	gPeekLine gIdentity,0,gHeight-1,buf%(),16
	if buf%(1)<>$0 :print hex$(buf%(1)) :raise 2 : get :endif
	gPeekLine gIdentity,gWidth-1-16,0,buf%(),16
	if buf%(1)<>$0 :print hex$(buf%(1)) :raise 3 :get :endif
	gPeekLine gIdentity,gWidth-1-16,gHeight-1,buf%(),16
	if buf%(1)<>$0 :print hex$(buf%(1)) :raise 4 :get :endif
endp


proc bitCpy:(bitflg%,destId%,srcId%)
	rem does nothing if bitflg%=0
	if bitFlg%
		gUse destId%
		gAt 0,0
		gCopy srcId%,0,0,gWidth,gHeight,3
		gUse srcId%
	endif
endp

REMEnd of gCls.tpl
