REM wMenu.tpl
REM EPOC OPL automatic test code for wMenu.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wMenu", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc wMenu:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dowMenu")
	rem hCleanUp%:("CleanUp")
endp


proc dowmenu:
	rem	cls
	rem print "Test Menu Too Wide"; :print

	mInit
	onerr e1
	mCard rept$("N",255),"A",%a,"B",%b
	menu
	onerr off
	rem print "wmenu\m2 : BUG 1! - Menu card title too wide not detected";
	rem print
	RAISE 1
	rem get
e1::
	onerr e2
	mInit
	mCard "A",rept$("I",255),%a
	menu
	onerr off
	rem print "wmenu\m2 : BUG 2! - Menu card item too wide not detected";
	rem print
	RAISE 2
	rem get
e2::
	onerr off
endp

REM End of wMenu.tpl
