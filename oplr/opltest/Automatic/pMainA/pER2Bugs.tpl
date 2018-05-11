REM pER2Bugs.tpl
REM EPOC OPL automatic test code for ER2 OPL bugfixes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
include "system.oxh"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pER2bugs", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc per2bugs:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tConst")
rem	hCleanUp%:("CleanUp")
endp


proc tconst:
	local k&

	rem print "Check new consts ok"
	k&=KKeyMenu32&
	if k&<>&f836
		raise 1
	endif
	k&=KKeyDownArrow32&
	if k&<>&f80a
		raise 2
	endif
endp


REM Skip these tests.
PROC inter:
	print "Interactive tests"
	tconsti:
	texternalpower:
	tbeep:
ENDP


proc tconsti:
	local ev&(16)
	
	while 1
		getevent32 ev&()
		if ev&(1)=27 :break :endif
		print ev&(1)
	endwh
endp


PROC texternalpower:
	local isExt&,wasExt&

	print "Test external power detection"
	wasExt&=2	rem will become -1 when present
	while get<>27
		isExt&=isExternalPowerPresent&:
		if isExt&<>wasExt&
			wasExt&=isExt&
			if isExt&
				print "External power present"
			else
				print "External power not present"
			endif
		else
			print ".";
		endif
	endwh
	print
ENDP


PROC tbeep:
	print "Set system beep setting loud and press key (esc quits)"
	while get<>27
		trybeep:
	endwh
	print "Set system beep setting quiet and press key (esc quits)"
	while get<>27
		trybeep:
	endwh
	print "Set system beep setting off and press key (esc quits)"
	while get<>27
		trybeep:
	endwh
ENDP

PROC trybeep:
	beep 10,500
ENDP

REM End of pER2bugs.tpl

