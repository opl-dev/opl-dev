REM pSetPenW.tpl
REM EPOC OPL automatic test code for gSetPenWidth
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pSetPenW", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pSetPenW:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("test1")
	hRunTest%:("test2")
	hRunTest%:("test3")
rem	hCleanUp%:("CleanUp")
endp


proc test1:
	local id%
	
	print "Examples with different colours and shapes"
	rem pause 20
	cls
	rem in default window
	draw:(1)
	rem 16 colur window
	id%=gcreate(0,0,gwidth,gheight,1,2)
	draw:(2)
	gclose id%
	rem 2 colour window
	id%=gcreate(0,0,gwidth,gheight,1,0)
	draw:(0)
	gclose id%
endp


proc draw:(mode%)
	local i%,col%,inc%,x%
	
	if mode%=0 : inc%=$f0 : else inc%=$10 : endif
	x%=(2**(mode%+1))
	if mode%<>0 : x%=x%*mode% : endif
	col%=0
	while col%<=$f0
		i%=0
		gcolor 0,0,0
		gat 550,20 : gprint x%;"-colour"	
		gat 550,40 : gprint "$",hex$(col%)
		while i%<=10
			gcolor col%,col%,col%
			gsetpenwidth i%
			gat 40,10 : glineby 50,0
			gat 40,40 : gbox 50,50
			gat 400,60 : gcircle 50
			gat 80,180 : gcircle 50,1
			gat 400,180 : gellipse 100,50
			gat 160,10 : glineto 160,230
			i%=i%+3
			rem pause -5
		endwh
		col%=col%+inc%
		gcls
	endwh
endp


proc test2:
	local id%,a%(9)

	print "Testing using gPEEKLINE"
	rem pause 20
	cls	
	id%=gcreate(0,0,gwidth,gheight,1,2)
	gcolor 0,0,0
	gsetpenwidth 8
	gat 130,10 : glineto 130,230
	gpeekline id%,126,30,a%(),12,2
	if a%(1)<>$000f : raise 1 : endif
	if a%(2)<>$0000 : raise 2 : endif
	if a%(3)<>$fff0 : raise 3 : endif
	gcls
	gsetpenwidth 16
	gat 130,10 : glineto 130,230
	gpeekline id%,122,30,a%(),20,2
	if a%(1)<>$000f : raise 4 : endif
	if a%(2)<>$0000 : raise 5 : endif
	if a%(3)<>$0000 : raise 6 : endif
	if a%(4)<>$0000 : raise 7 : endif
	if a%(5)<>$fff0 : raise 8 : endif
	gcls
	gsetpenwidth 31
	gat 130,10 : glineto 130,230
	gpeekline id%,114,30,a%(),34,2
	if a%(1)<>$000f : raise 9 : endif
	if a%(2)<>$0000 : raise 10 : endif
	if a%(3)<>$0000 : raise 11 : endif
	if a%(4)<>$0000 : raise 12 : endif
	if a%(5)<>$0000 : raise 13 : endif
	if a%(6)<>$0000 : raise 14 : endif
	if a%(7)<>$0000 : raise 15 : endif
	if a%(8)<>$0000 : raise 16 : endif
	if a%(9)<>$00ff : raise 17 : endif
	gcls
	gcolor $80,$80,$80
	gsetpenwidth 8
	gat 130,10 : glineto 130,230
	gpeekline id%,126,30,a%(),12,2
	if a%(1)<>$888f : raise 18 : endif
	if a%(2)<>$8888 : raise 19 : endif
	if a%(3)<>$fff8 : raise 20 : endif
	gcls
	gsetpenwidth 16
	gat 130,10 : glineto 130,230
	gpeekline id%,122,30,a%(),20,2
	if a%(1)<>$888f : raise 21 : endif
	if a%(2)<>$8888 : raise 22 : endif
	if a%(3)<>$8888 : raise 23 : endif
	if a%(4)<>$8888 : raise 24 : endif
	if a%(5)<>$fff8 : raise 25 : endif
	gcls
	gsetpenwidth 31
	gat 130,10 : glineto 130,230
	gpeekline id%,114,30,a%(),34,2
	if a%(1)<>$888f : raise 26 : endif
	if a%(2)<>$8888 : raise 27 : endif
	if a%(3)<>$8888 : raise 28 : endif
	if a%(4)<>$8888 : raise 29 : endif
	if a%(5)<>$8888 : raise 30 : endif
	if a%(6)<>$8888 : raise 31 : endif
	if a%(7)<>$8888 : raise 32 : endif
	if a%(8)<>$8888 : raise 33 : endif
	if a%(9)<>$00ff : raise 34 : endif
	gcls
	gclose id%
endp

const KS5ScreenCharHeight%=21
const KCrystalScreenCharHeight%=18

proc test3:
	local id%

	screen 45,KCrystalScreenCharHeight%,1,1
	print "Testing specific cases"
	rem pause 20
	id%=gcreate(320,0,320,gheight,1,2)
	gborder 0
	gcolor 0,0,0

	guse 1 
	print "Penwidth = 200 drawing 100 square"
	guse id%
	gsetpenwidth 200
	gat 100,100 : gbox 100,100
	rem get
	gcls

	guse 1
	print "Same with invert g-mode"
	guse id%
	ggmode 2
	gsetpenwidth 200
	gat 100,100 : gbox 100,100
	rem get
	gcls

	guse 1
	print "Penwidth = 0 drawing square"
	guse id%
	ggmode 0
	gsetpenwidth 0
	gat 100,100 : gbox 100,100
	rem get
	gcls

	guse 1
	print "Penwidth = 700 drawing 100 square"
	guse id%
	gsetpenwidth 700
	gat 100,100 : gbox 100,100
	rem get
	gcls

	guse 1
	print "Penwidth = 100 drawing 100 square clearing bits on black"
	guse id%
	gfill gwidth,gheight,0
	ggmode 1
	gsetpenwidth 10
	gat 100,100 : gbox 100,100
	rem get
	gcls
	gCLOSE id%
endp


REM End of pSetPenW.tpl

