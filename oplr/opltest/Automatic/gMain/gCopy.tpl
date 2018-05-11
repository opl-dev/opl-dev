REM gCopy.tpl
REM EPOC OPL automatic test code for gCOPY.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gCopy", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gCopy:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogCopy")
	rem hCleanUp%:("CleanUp")
endp


proc dogCopy:
	local pause%
	local bitId%,winId%

	rem pause%=-30 :key
	gUse 1
	gCls
	bitId%=gCreateBit(gWidth,gHeight)
	gCls
	gAt 0,0
	gBox gWidth,gHeight
	gAt 0,0
	gLineBy gWidth,gHeight
	gAt 1,30
	gPrint "This text was written to bitmap"
	winId%=gCreate(0,0,gWidth,gHeight,1)
	gAt 0,0
	gCopy bitId%,0,0,gWidth,gHeight,3
	gat 1,75
	gStyle 9
	gprint " Copied from BITMAP OK "
	rem pause pause% :key
	gCls
	gStyle 0
	gBox gWidth,gHeight
	gAt 1,30
	gPrint "This text was written to window 2"
  rem pause pause% : key
  gUse 1
  gCls
  gOrder 1,1
  gAt 1,20 :gPrint "This is window 1"
  gAt 1,22
	gCopy winId%,0,0,gWidth,gHeight-25,0
	gAt 1,80
	gStyle 9
	gPrint " Copied from WINDOW OK "
	rem pause pause% :key
  gCls
	gStyle 0
  gAt 1,20 :gPrint "This is window 1"
	gAt 0,30
	gCopy gIdentity,0,0,gWidth,30,3
  gAt 0,30
	gCopy gIdentity,0,0,gWidth,30,3
	gAt 10,gHeight-10
	gStyle 9
	gPrint "Copied to self ok"
	rem pause pause% :key
	gCls :cls
	rem print "Test bad window id for gCopy"
	onerr e1
	gCopy 0,0,0,10,10,3
	onerr off
	rem print "Bad id not detected"
	raise 1
e1::
  gClose bitId%
  gClose winId%
	onerr off
  if err<>-2
		rem print "Bad id error NOT detected ("; :print err,err$(err);")"
  		raise 2
  endif
  rem pause -30 :key
endp

REM End of gCopy.tpl

