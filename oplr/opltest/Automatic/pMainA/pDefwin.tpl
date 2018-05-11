REM pDefwin.tpl
REM EPOC OPL automatic test code for DEFAULTWIN
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pDefwin", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pDefwin:
	global id%,y%,info%(10)
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tdefwin")
	hRunTest%:("tinvalid")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	DEFAULTWIN 1
ENDP


const KDefId%=1


proc tdefwin:
	rem rem screen info%(3),info%(4)/2-3,1,info%(4)/2+3
	print "1. Test DEFAULTWIN by eye"
	print "The following two screens should be identical"
	print "The top is the default window"
	print "The bottom is a graphics window with 4 colour mode"
	rem pause 30
	tstbyfill:(1)
	
	print "DEFAULTWIN 1 should clear the screen"
	rem pause 20
	defaultwin 1
	rem screen info%(3),info%(4)/2-3,1,info%(4)/2+3
	print "The following two screens should still be identical"
	print "The top is the default window"
	print "The bottom is a graphics window with 4 colour mode"
	rem pause 30
	tstbyfill:(1)
	
	print "DEFAULTWIN 0 should clear the screen and switch to 2 colour mode"
	rem pause 20
	defaultwin 0
	rem screen info%(3),info%(4)/2-3,1,info%(4)/2+3
	print "The following two screens should be identical"
	print "The top is the default window"
	print "The bottom is a graphics window with 2 colour mode"
	print "Greys will be mapped to black or white"
	rem pause 30
	tstbyfill:(0)
	
	print "DEFAULTWIN 0 should now just clear the screen"
	rem pause 20
	defaultwin 0
	rem screen info%(3),info%(4)/2-3,1,info%(4)/2+3
	print "The following two screens should be identical"
	print "The top is the default window"
	print "The bottom is a graphics window with 2 colour mode"
	print "Greys will be mapped to black or white"
	rem pause 30
	tstbyfill:(0)
	
	print "DEFAULTWIN 1 should clear the screen and switch to 4 colour mode"
	rem pause 20
	defaultwin 1
	rem screen info%(3),info%(4)/2-3,1,info%(4)/2+3
	print "The following two screens should be identical"
	print "The top is the default window"
	print "The bottom is a graphics window with 4 colour mode"
	rem pause 30
	tstbyfill:(1)
	
	print "DEFAULTWIN 2 should clear the screen and switch to 16 colour mode"
	rem pause 20
	defaultwin 2
	rem screen info%(3),info%(4)/2-3,1,info%(4)/2+3
	print "The following two screens should be identical"
	print "The top is the default window"
	print "The bottom is a graphics window with 16 colour mode"
	rem pause 30
	tstbyfill:(2)
	rem pause 20
	
	print "DEFAULTWIN 2 should just clear the screen"
	rem pause 20
	defaultwin 2
	rem screen info%(3),info%(4)/2-3,1,info%(4)/2+3
	print "The following two screens should be identical"
	print "The top is the default window"
	print "The bottom is a graphics window with 16 colour mode"
	rem pause 30
	tstbyfill:(2)
	
	rem pause 30
	defaultwin 1	
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	
	print "2. Check DEFAULTWIN with gPEEKLINE"
	print "The following two sets of lines should be identical"
	print "The left are in the default (4 colour) window"
	print "The right are in a 4 colour mode graphics window"
	rem pause 30
	tstbypeek:(1)
	
	print "DEFAULTWIN 1 should just clear the screen"
	rem pause 20
	defaultwin 1
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "The following two sets of lines should be identical"
	print "The left are in the default (4 colour) window"
	print "The right are in a 4 colour mode graphics window"
	rem pause 30
	cls
	tstbypeek:(1)

	print "DEFAULTWIN 1 should just clear the screen"
	rem pause 20
	defaultwin 1
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "The following two sets of lines should be identical"
	print "The left are in the default (4 colour) window"
	print "The right are in a 4 colour mode graphics window"
	rem pause 30
	tstbypeek:(1)

	print "DEFAULTWIN 0 should clear the screen and change to two colour mode"
	rem pause 20
	defaultwin 0
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "The following two sets of lines should be identical"
	print "The left are in the default (2 colour) window"
	print "The right are in a 2 colour mode graphics window"
	rem pause 30
	tstbypeek:(0)
	
	print "DEFAULTWIN 1 should clear the screen and return to 4 colour mode"
	rem pause 20
	defaultwin 1
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "The following two sets of lines should be identical"
	print "The left are in the default (4 colour) window"
	print "The right are in a 4 colour mode graphics window"
	rem pause 30
	tstbypeek:(1)

	print "DEFAULTWIN 2 should clear the screen and switch to 16 colour mode"
	rem pause 20
	defaultwin 2
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "The following two sets of lines should be identical"
	print "The left are in the default (16 colour) window"
	print "The right are in a 16 colour mode graphics window"
	rem pause 30
	tstbypeek:(2)
	
	print "DEFAULTWIN 2 should just clear the screen"
	rem pause 20
	defaultwin 2
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "The following two sets of lines should be identical"
	print "The left are in the default (16 colour) window"
	print "The right are in a 16 colour mode graphics window"
	rem pause 30
	tstbypeek:(2)
	
	print "DEFAULTWIN 1 should clear the screen and return to 4 colour mode"
	rem pause 20
	defaultwin 1
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "The following two sets of lines should be identical"
	print "The left are in the default (4 colour) window"
	print "The right are in a 4 colour mode graphics window"
	rem pause 30
	tstbypeek:(1)
	
	gcls
endp


proc tstbyfill:(mode%)
	local x%,col%,i%
	gcolor 0,0,0   rem black
	gat 0,0
	gfill 40,gheight/2,0
	i%=1
	while i%<16
		x%=40*i%
		col%=$10*i%
		gat x%,0
		gcolor col%,col%,col%
		gfill 80,gheight/2,0
		i%=i%+1
	endwh
	id%=gcreate(0,gheight/2,gwidth,gheight/2,1,mode%)
	gcolor 0,0,0   rem black
	gfill 40,gheight,0
	i%=1
	while i%<16
		x%=40*i%
		col%=$10*i%
		gat x%,0
		gcolor col%,col%,col%
		gfill 80,gheight,0
		i%=i%+1
	endwh
	rem pause 30
	gcls
	gclose id%
endp


proc tinvalid:
	onerr valover
	defaultwin 32768
	onerr off
	raise 100
valover::
	onerr off 
	if err<>-6 : raise 200 : endif

	defaultwin -1
	defaultwin 32767
	
	rem screen info%(3),info%(4)/2-5,1,info%(4)/2+5
	print "Tested invalid values - OK"
endp


proc tstbypeek:(mode%)
	id%=gcreate(gwidth/2,0,gwidth/2,gheight/2,1,mode%)
	y%=1
	line:(0,mode%)	
	line:($10,mode%)
	line:($20,mode%)
	line:($30,mode%)
	line:($40,mode%)
	line:($50,mode%)	
	line:($60,mode%)
	line:($70,mode%)
	line:($80,mode%)
	line:($90,mode%)
	line:($A0,mode%)
	line:($B0,mode%)
	line:($C0,mode%)
	line:($D0,mode%)
	line:($E0,mode%)
	line:($F0,mode%)
	rem pause 30
	gclose id%
endp


proc line:(r%,mode%)
	local a%(10),b%(10)
	local i%
		
	guse KDefId%
	gcolor r%,r%,r%
	gat 40,y%
	glineby 40,0
	gpeekline KDefId%,40,y%,b%(),40,2
	
	guse id%
	gcolor r%,r%,r%
	gat 40,y%
	glineby 40,0
	gpeekline id%,40,y%,a%(),40,2
	
	y%=y%+6

	i%=1
	while i%<11
		if b%(i%)<>a%(i%) : raise r%/16	:	endif
		i%=i%+1
	endwh
endp

REM End of pDefwin.tpl

