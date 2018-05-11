REM gCreatebit.tpl
REM EPOC OPL automatic test code for gCreatebit.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gCreatebit", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gcreatebit:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogCreatebit")
rem	hCleanUp%:("CleanUp")
endp


proc dogCreateBit:
    local id%,w%,h%,d&(2)

   rem print "Test gCreateB"
    w%=gWidth :h%=gHeight
    id%=gCreateBit(w%,h%)
    gCls
    gBox w%,h%
    gAt 1,h%/2
    gPrint "gPrinted to bitmap"
    gUse 1
    gCls
    gCopy id%,0,0,w%,h%,0
    gClose id%
    gPeekLine 1,0,0,#addr(d&()),32
    if d&(1)<>&ffffffff
    		raise 1
    endif
    rem giprint "gCreateB finished ok"
  		rem pause -20 :key
  		guse 1
  		gcls
endp

REM End of gCreatebit.tpl

