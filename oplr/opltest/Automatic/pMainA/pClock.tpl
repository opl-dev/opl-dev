REM pClock.tpl
REM EPOC OPL automatic test code for gCLOCK
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pClock", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pCLock:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:( "tclock")
endp


proc tclock:
	global id1%,id2%,id3%
	print "Opler1 gClock Tests"
	print "Tests all modes except 11 (see separate test)"
	print
	tinval:
	tval:
	toffset:
	terrors:
	print
	print "Opler1 gClock Tests Finished OK"
	rem pause 30
endp


proc tinval:
	print "Test unused modes 1-5 and 10"
	print 
	tinvalmode:(1,1)
	tinvalmode:(2,3)
	tinvalmode:(3,5)
	tinvalmode:(4,7)
	tinvalmode:(5,9)
	tinvalmode:(10,11)
	print "Test some invalid mode% values"
	print
	tinvalmode:(20,13)
	tinvalmode:(32767,15)
	tinvalmode:(-7,17)
	print "Test some valid modes ORed with invalid values"
	print
	tinvalmode:($17,19)
	tinvalmode:($26,21)
	tinvalmode:($48,23)
	tinvalmode:($88,25)
	print "OK"
	print	
endp


proc tinvalmode:(mode%,r%)
	onerr inval
	gclock on, mode%
	onerr off
	raise r%
inval::
	onerr off
	if err<>-2 : raise r%+1 : endif
endp


proc tval:
	print
	print "Test valid modes 6-9"
	rem pause 30
	cls
	
	id1%=gcreate (0,0,gwidth,gheight,1,2)
	gfont KFontCourierNormal11&

	gat 20,20
	gprint "Mode 6 - system setting"
	gat 320,50
	gclock on,6
	rem pause 30
	gclock off
	gcls

	gat 20,20
	gprint "Mode 7 - analog"
	gat 320,50
	gclock on,7
	rem pause 30
	gclock off
	gcls
	
	gat 20,20
	gprint "Mode 8 - digital"
	gat 320,50
	gclock on,8
	rem pause 30
	gclock off
	gcls

	guse id1%
	gat 20,20
	gprint "Mode 9 - extra large"
	gat 320,50
	gclock on,9
	rem pause 30
	gclock off
	gcls
	gclose id1%
endp


proc toffset:
	print
	print "Test Offset"
	rem pause 20
	cls
	
	id1%=gcreate (10,0,210,gheight,1,2)
	gfont KFontCourierNormal11&
	id2%=gcreate (220,0,210,gheight,1,2)
	gfont KFontCourierNormal11&
	id3%=gcreate (430,0,210,gheight,1,2)
	gfont KFontCourierNormal11&
	
	guse id2%
	gat 20,40
	gprint "1 hour back"
	guse id3%
	gat 20,40
	gprint "1 hour forward"
	offsetclocks:(id1%,id2%,id3%,6,60)
	
	guse id2%
	gat 20,40
	gprint "30 minutes back"
	guse id3%
	gat 20,40
	gprint "30 minutes forward"
	offsetclocks:(id1%,id2%,id3%,7,30)

	guse id2%
	gat 20,40
	gprint "2 hours back"
	guse id3%
	gat 20,40
	gprint "2 hours forward"
	offsetclocks:(id1%,id2%,id3%,8,120)	

	guse id2%
	gat 20,40
	gprint "15 minutes back"
	guse id3%
	gat 20,40
	gprint "15 minutes forward"
	offsetclocks:(id1%,id2%,id3%,9,15)
	
	guse id2%
	gat 20,40
	gprint "24 hours back"
	guse id3%
	gat 20,40
	gprint "24 hours forward"
	offsetclocks:(id1%,id2%,id3%,8,1440)			
	
	rem offset in seconds
if 0
	guse id2%
	gat 20,40
	gprint "5 minutes back"
	guse id3%
	gat 20,40
	gprint "5 minutes forward"
	offsetclocks:(id1%,id2%,id3%,8 OR $100,300)
	
	guse id2%
	gat 20,40
	gprint "30 seconds back"
	guse id3%
	gat 20,40
	gprint "30 seconds forward"
	offsetclocks:(id1%,id2%,id3%,9 OR $100,30)
	
	guse id2%
	gat 20,40
	gprint "24 hours back"
	guse id3%
	gat 20,40
	gprint "24 hours forward"
	offsetclocks:(id1%,id2%,id3%,8 OR $100,86400)			
endif		
	gclose id1% : gclose id2% : gclose id3%
endp


proc offsetclocks:(id1%,id2%,id3%,mode%,offset%)
	guse id1%
	gat 20,20
	gprint "Mode ";mode%
	gat 20,40
	gprint "system time"
	gat 20,75
	gclock on,mode%
	guse id2%
	gat 20,75
	gclock on,mode%,-offset%
	guse id3%
	gat 20,75
	gclock on,mode%,offset%
	rem get
	gclock off
	gcls
	guse id2%
	gclock off
	gcls
	guse id1%
	gclock off
	gcls
endp


proc terrors:
	rem attempt to display two clocks in one window
	print "Attempt to open two clocks of different modes in same window"
	print
	rem pause 20
	id1%=gcreate (0,0,gwidth,gheight,1,2)
	gfont KFontCourierNormal11&	
	gat 20,20
	gprint "Mode 7 - analog"
	gat 20,60
	gclock on,7
	gat 320,20
	gprint "Mode 8 - digital"
	gat 320,60
	gclock on,8
	rem get
	gclock off
	gclose id1%

	print "Attempt to open two clocks of same mode in same window"
	print
	rem pause 20
	id1%=gcreate (0,0,gwidth,gheight,1,2)
	gfont KFontCourierNormal11&	
	gat 20,20
	gprint "Mode 7 - analog"
	gat 20,60
	gclock on,7
	gat 320,20
	gprint "Mode 7 - analog"
	gat 320,60
	gclock on,7
	rem get
	gclock off
	gclose id1%
	
	print "Attempt to use formatting and not mode 11"
	print
	rem pause 20
	id1%=gcreate (0,0,gwidth,gheight,1,2)
	gfont KFontCourierNormal11&	
	gat 20,20
	gprint "Mode 8 - digital"
	gat 320,60
	gclock on,8,0,"%H%:1%T%:2%S",KFontCourierNormal11&,1	
	rem get
	gclock off
	gclose id1%
	
	print "OK"
endp

REM End of pClock.tpl
