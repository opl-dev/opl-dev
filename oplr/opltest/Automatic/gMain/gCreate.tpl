REM gCreate.tpl
REM EPOC OPL automatic test code for gCREATE.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gCreate", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gCreate:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogCreate")
rem	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	rem
ENDP


proc dogcreate:
	lots:	
	gen:
endp


proc lots:
	local id%(100),i%
	i%=1
	rem print "Creating 63 windows (+ default window = 64)"
	while i%<64
		id%(i%)=gCreate(400,i%*3,30,30,1)
		gBox 30,30
		i%=i%+1
	endwh
	rem pause -30
	rem print "Try 1 more"
	onerr e
	id%(64)=gCreate(120,i%*5,30,30,1)
	onerr off
	raise 1
e::
	onerr off
	if err<>-117 : raise 2 : print err$(err) : raise 2 : endif
	rem print "Error handled ok"
	rem pause -30
	i%=1
	rem print "Closing 63 windows"
	while i%<64
		gClose id%(i%)
		i%=i%+1
	endwh
	rem pause -30	 :key
	rem print "Attempting to close default (64th) window"
	onerr e1
	gclose 1
	onerr off
	raise 3
e1::
	onerr off
	if err<>-2 : raise 4 : print err$(err) : raise 4 : endif
	rem print "Error handled ok - not closed"
	rem pause -30
endp


proc gen:
	local id2%,id3%,id4%,reg%,i%
	local winW%

	gcls
	gCreate(0,0,0,0,0)
	gStyle 1 or 2
	winW%=gTWidth(" window 3 ")+6
	gClose gIdentity
	
	id2%=gCreate(1,16,winW%,16,1)
	gborder 2
	gAt 3,12
	gStyle 1 or 2
	gprint " window ",2

	id3%=gCreate(1,32,winW%,16,1)
	gborder 2
	gAt 3,12
	gStyle 1 or 2
	gprint " window ",3
	
	id4%=gCreate(1,48,winW%,16,0)
	gborder 2
	gAt 3,12
	gStyle 1 or 2
	gprint " window ",4
	rem at 1,9
	rem print "Window 4 is invisible"
	rem pause -30 :key
	cls
	gVisible on	
	rem print "Now it is visible"
	rem pause -30 :key
	
	cls
	gUse 1
	gTMode 2
	gAt 1,12
	gprint "Window 1 in background"
	at 1,8
	rem print "window 1 is in background"
	rem pause -30 :key
	
	gAt 1,32
	gprint "window 1 in foreground  "
	gOrder 1,0
	gAt 0,60
	gFill gwidth,32,1
	rem at 1,8
	rem pause -30 :key
	
	gcls
	rem print "window 1 in background  "
	gUse id2%
	gOrder 1,9
	gCls
	gAt 3,12
	gprint " in 2"	
	rem at 2,9
	rem print "Closing window 2"
	rem pause -30 :key
	gClose 2
	rem at 2,9
	rem print "Done                   "
	rem pause -50 :key
	gclose id4%
	gclose id3%
rem	gclose id2%
	gUse 1
	gcls
	cls
endp

REM End of gCreate.tpl
