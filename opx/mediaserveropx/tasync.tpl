rem tAsync.tpl
rem test code for mediaserver.opx
rem (actually, I'm just interested in using the testasync stuff for now...)

INCLUDE "Mediaserveropx.oxh"
DECLARE EXTERNAL
CONST KDelay&=1 rem 1 millisecond.

PROC Main:
	GLOBAL gStatus&
	LOCAL delay&, priority&
rem	randomize second+1*minute+1
	PRINT "About to call TestAsync:() with delay",KDelay&
	priority&=1
	TestAsync:(priority&, KDelay&, gStatus&)
	if 0>1
		IOWAITSTAT32 gStatus&
		PRINT "Done. Press any key"
		GET
	endif

	print "Watch this hang... (but tap a key when it does!)"
	while 1
		print HEX$(gStatus&),
	endwh
ENDP
