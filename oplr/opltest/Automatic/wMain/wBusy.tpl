REM wBusy.tpl
REM EPOC OPL automatic test code for BUSY.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wBusy", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc wBusy:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("doBusy")
rem	hCleanUp%:("CleanUp")
endp


proc dobusy:
	rem print "Test BUSY ON/OFF"
	busy off
	rem print "busy with 1 arg"
	rem print " busy ""on"""
	busy "on"
	rem pause -50 :key
	rem print " busy """""
	busy ""
	rem pause -40 :key

	rem cls
	rem print "busy with 3 args"
	rem print "top left,delay=0 sec"
	busy "top left,delay=0 sec",0,0
	rem pause -60 :key
	rem print "bottom left, delay=1 sec"
	busy "bot left,delay=1 sec",1,2
	rem pause -80 :key
	rem print "top right,delay=2 secs"
	busy "top right,delay=2 secs",2,4
	rem pause -100 :key
	rem print "bot right,delay=4 secs"
	busy "bot right,delay=4 secs",3,8
	rem pause -140 :key

	rem cls
	rem print "busy with 2 args"
	rem print "top left,delay=def"
	busy "top left,delay=def",0
	rem pause -60 :key
	rem print "bottom left, delay=def"
	busy "bot left,delay=def",1
	rem pause -60 :key
	rem print "top right,delay=def"
	busy "top right,delay=def",2
	rem pause -60 :key
	rem print "bot right,delay=def"
	busy "bot right,delay=def",3
	rem pause -60 :key
	rem busy off
	
	errors:

	rem print "Finished"
	rem pause -100 :key
	busy off
endp


proc errors:
	rem test that busy message longer than 40 chars invalid
	rem print "This is a long busy message of 80 chars"
	busy "01234567890123456789012345678901234567890123456789012345678901234567890123456789"
	rem pause -60
	busy off

	onerr toolong
	REM 81 chars.
	busy "012345678901234567890123456789012345678901234567890123456789012345678901234567890"
	onerr off
	raise 1
toolong::
	onerr off
	if err<>-2 : raise 2 : endif
	rem print "Busy message of 81 characters causes an error"
endp

REM End of wBusy.tpl
