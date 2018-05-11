REM p16col.tpl
REM EPOC OPL automatic test code for 16-color graphics.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "p16col", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc p16col:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("t16colwin")
rem 	hCleanUp%:("CleanUp")
endp


proc t16colwin:
	global id0%,id1%,id2%
	rem print "Opler1 General 16-colour Window Tests"
	rem print
	tgborder:
	tgbox:
	tgcircle:
	tgcopy:
	tgfill:
	tgfont:
	tggmode:
	tginvert:
	tgmove:
	tgorderandrank:
	tgorigin:
	tgpatt:
	tgpoly:
	tgprint:
	tgscroll:
	tgsetwin:
	tgupdate:
	tvis:
	rem print
	rem print "Opler1 General 16-colour Window Tests finished OK"
	rem pause 30
endp


proc tgborder:
	cls
	print "Test gBORDER"
	print "1. These drawables should have shadows"
	print "Press a key to remove the shadow and again to replace shadow"
	border:(1,2,3,4)
	
	print "2. These drawables should have shadows and rounded corners"
	print "Press a key to remove the shadow and again to replace shadow"
	border:($201,$202,$203,$204)
	
	print "3. These are boxes"
	print "Press a key to draw a border with a 1 pixel gap"
	id1%=gcreate (110,70,100,90,1,2)
	gat 20,20 : gbox 60,50 : get%: : gborder $100,60,50
	id2%=gcreate (430,70,100,90,1,2)
	gat 20,20 : gbox 60,50 : get%: : gborder $100,60,50
	pause%:(10)
	gclose id1% : gclose id2%
	cls
	
	print "4. These drawables should have shadows (borders at 1 pixel gap)"
	print "Press a key to remove the shadow and again to replace shadow"
	border:($101,$102,$103,$104)

	print "5. These should have borders with shadows inside the drawable"
	print "Press a key to remove the shadow"
	id1%=gcreate (110,70,100,90,1,2)
	gborder 2 : gat 10,10
	gborder 1,80,70 : get%: : gborder 2,80,70
	id2%=gcreate (430,70,100,90,1,2)
	gborder 4 : gat 10,10
	gborder 3,80,70 : get%: : gborder 4,80,70
	pause%:(10)
	gclose id1% : gclose id2%
	cls	
endp

proc border:(b1%,b2%,b3%,b4%)
	id1%=gcreate (110,70,100,90,1,2)
	gborder b1% : get%: : gborder b2% : get%: : 	gborder b1%
	id2%=gcreate (430,70,100,90,1,2)
	gborder b3% : get%: : gborder b4% : get%: : gborder b3%
	pause%:(10)
	gclose id1% : gclose id2%
	cls
endp

proc tgbox:
	local i%,x%,y%,col%
	print "Test gBOX"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	i%=0
	col%=0
	x%=1
	y%=1
	while i%<16
		gat x%,y%
		gcolor col%,col%,col%
		gbox gwidth-2*x%,gheight-2*y%
		x%=x%+12
		y%=y%+4
		col%=col%+$10
		i%=i%+1
	endwh
	get%:
	gcls
	gclose id1%
	cls
endp

proc tgcircle:
	print "Test gCIRLCE and gELLIPSE"
	print
	print "Testing unfilled and filled"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gcolor $20,$20,$20
	gat 160,60
	gcircle 50
	gcolor $60,$60,$60
	gat 480,60
	gcircle 50
	gcolor $a0,$a0,$a0
	gat 160,180
	gcircle 50,1
	gcolor $e0,$e0,$e0
	gat 480,180
	gcircle 50,-1
	get%:
	gcls	
	gcolor $10,$10,$10
	gat 160,60
	gellipse 50,30
	gcolor $50,$50,$50
	gat 480,60
	gellipse 50,30
	gcolor $90,$90,$90
	gat 160,180
	gellipse 50,30,1
	gcolor $d0,$d0,$d0
	gat 480,180
	gellipse 50,30,-1
	get%:
	gcls
	gclose id1%
	cls
endp

proc tgcopy:
	local i%,col%,x%
	print "Test gCOPY"
	print
	print "First copying FROM a 16-colour"
	print "window.  Second copying TO 16"
	print "colour window. In each case the 4"
	print "copies represent 4 copy modes."
	print " In the last three cases of each"
	print " copy, the background is black"
	id2%=gcreate(gwidth/2,0,gwidth/2,160,1,2) : gborder 1
	guse 1
	id1%=gcreate(0,161,gwidth/2,80,1,1) : gborder 1
	guse 1
	id0%=gcreate(gwidth/2,161,gwidth/2,80,1,0) : gborder 1
	rem create something to copy
	guse id2%
	x%=10
	while i%<16
		gat x%,10
		gcolor col%,col%,col%
		gfill 4,64,0
		x%=x%+4
		col%=col%+$10
		i%=i%+1
	endwh
	guse id1% : gat 10,10
	gcopy id2%,10,10,64,64,0	
	guse id0% : gat 10,10
	gcopy id2%,10,10,64,64,0	
	guse id1% : gat 80,10
	gfill 64,64,0
	gcopy id2%,10,10,64,64,1
	guse id0% : gat 80,10
	gfill 64,64,0
	gcopy id2%,10,10,64,64,1
	guse id1% : gat 150,10
	gfill 64,64,0
	gcopy id2%,10,10,64,64,2	
	guse id0% : gat 150,10
	gfill 64,64,0
	gcopy id2%,10,10,64,64,2	
	guse id1% : gat 220,10
	gfill 64,64,0
	gcopy id2%,10,10,64,64,3
	guse id0% : gat 220,10
	gfill 64,64,0
	gcopy id2%,10,10,64,64,3
	get%:
	guse id0% : gcls
	guse id1% : gcls
	guse id2% : gcls	

	rem create something to copy
	guse id1%
	gborder 1
	x%=10
	col%=0
	i%=0
	while i%<16
		gat x%,10
		gcolor col%,col%,col%
		gfill 4,64,0
		x%=x%+4
		col%=col%+$10
		i%=i%+1
	endwh
	guse id0%
	gborder 1
	x%=10
	col%=0
	i%=0
	while i%<16
		gat x%,10
		gcolor col%,col%,col%
		gfill 4,64,0
		x%=x%+4
		col%=col%+$10
		i%=i%+1
	endwh

	guse id2%
	gborder 1
	gcolor 0,0,0
	gat 10,5 : gcopy id0%,10,10,64,64,0	
	gat 10,75 : gcopy id1%,10,10,64,64,0	
	gat 80,5 : gfill 64,64,0
	gcopy id0%,10,10,64,64,1
	gat 80,75 : gfill 64,64,0
	gcopy id1%,10,10,64,64,1
	gat 150,5 : gfill 64,64,0
	gcopy id0%,10,10,64,64,2	
	gat 150,75 : gfill 64,64,0
	gcopy id1%,10,10,64,64,2	
	gat 220,5 : gfill 64,64,0
	gcopy id0%,10,10,64,64,3
	gat 220,75 : gfill 64,64,0
	gcopy id1%,10,10,64,64,3
	get%:
	gclose id0% : gclose id1% : gclose id2%
	cls
endp

proc tgfill:
	print "Test gFILL"
	print "Three modes in succession"
	pause%:(30)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	stripes:(0)
	get%: : gcls
	stripes:(1)
	get%: : gcls
	stripes:(2)
	get%: : gcls
	gclose id1%
	cls
endp

proc tgfont:
	print "Test gFONT"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gprintfont:(4,0,20,"Type 4 maps to Courier 8")
	gprintfont:(5,0,33,"Type 5 maps to Times 8")
	gprintfont:(6,0,49,"Type 6 maps to Arial 11")
	gprintfont:(7,0,67,"Type 7 maps to Times 13")
	gprintfont:(8,0,87,"Type 8 maps to Times 15")
	gprintfont:(9,0,100,"Type 9 maps to Arial 8")
	gprintfont:(10,0,116,"Type 10 maps to Arial 11")
	gprintfont:(11,0,134,"Type 11 maps to Arial 13")
	gprintfont:(12,0,154,"Type 12 maps to Arial 15")
	gprintfont:(13,0,164,"Type 13 maps to Tiny 4 (mono)")
	get%:
	gcls
	gprintfont:(10,1,20,"Bold")
	gprintfont:(10,2,35,"Underline")
	gprintfont:(10,4,50,"Inverse")
	gprintfont:(10,8,80,"Double Height")
	gprintfont:(10,16,95,"Mono")
	gprintfont:(10,32,110,"Italic")
	gprintfont:(10,$3f,140,"All Styles")
	get%:
	gcls
	gfont 12 : gstyle 0
	gat 10,20
	gprint "Multicoloured text!"
	gat 10,50
	gcolor 0,0,0 : gprint "a"
	gcolor $10,$10,$10 : gprint "b"
	gcolor $20,$20,$20 : gprint "c"
	gcolor $30,$30,$30 : gprint "d"
	gcolor $40,$40,$40 : gprint "e"
	gcolor $50,$50,$50 : gprint "f"
	gcolor $60,$60,$60 : gprint "g"
	gcolor $70,$70,$70 : gprint "h"
	gcolor $80,$80,$80 : gprint "i"
	gcolor $90,$90,$90 : gprint "j"
	gcolor $A0,$A0,$A0 : gprint "k"
	gcolor $B0,$B0,$B0 : gprint "l"
	gcolor $C0,$C0,$C0 : gprint "m"
	gcolor $D0,$D0,$D0 : gprint "n"
	gcolor $E0,$E0,$E0 : gprint "o"
	gcolor $F0,$F0,$F0 : gprint "p"
	get%:
	gcls
	gclose id1%
	cls
endp

proc gprintfont:(font%,style%,pos%,str$)
	gfont font%
	gstyle style%
	gat 30,pos%
	gprint str$
endp

proc tggmode:
	print "Test gGMODE"
	print "Initially 0 - set"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gat 50,50 : gcolor 0,0,0 : gbox 50,50
	gat 150,50 : gcolor $80,$80,$80 : gbox 50,50
	gat 250,50 : gcolor $F0,$F0,$F0 : gbox 50,50
	get%: : gcls
	gorder 1,1
	print "Now 1 - cleared (on white)"
	pause%:(20)
	gorder id1%,1
	ggmode 1
	gat 50,50 : gcolor 0,0,0 : gbox 50,50
	gat 150,50 : gcolor $80,$80,$80 : gbox 50,50
	gat 250,50 : gcolor $F0,$F0,$F0 : gbox 50,50
	get%: : gcls
	gorder 1,1
	print "Now 1 - cleared (on black)"
	pause%:(20)
	gorder id1%,1
	ggmode 0
	gcolor 0,0,0
	gfill gwidth,gheight,0
	ggmode 1
	gat 50,50 : gcolor 0,0,0 : gbox 50,50
	gat 150,50 : gcolor $30,$30,$30 : gbox 50,50
	gat 250,50 : gcolor $F0,$F0,$F0 : gbox 50,50
	get%: : gcls
	gorder 1,1
	print "Now 2 - inverted (on black)"
	pause%:(20)
	gorder id1%,1
	ggmode 0
	gcolor 0,0,0
	gfill gwidth,gheight,0
	ggmode 2
	gat 50,50 : gcolor 0,0,0 : gbox 50,50
	gat 150,50 : gcolor $30,$30,$30 : gbox 50,50
	gat 250,50 : gcolor $F0,$F0,$F0 : gbox 50,50
	get%: : gcls
	gclose id1%
	cls
endp

proc stripes:(mode%)
	local col%,x%,i%
	col%=0
	x%=0
	while i%<16	
		gcolor col%,col%,col%
		gat x%,0
		gfill 40,gheight,mode%
		col%=col%+$10
		x%=x%+40
		i%=i%+1
	endwh
endp

proc tginvert:
	print "Test gINVERT"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	rem draw something to invert
	stripes:(0)
	get%:
	gat 0,0
	ginvert gwidth,gheight
	get%:
	ginvert gwidth,gheight
	get%:
	gcls
	gclose id1%
	cls
endp

proc tgmove:
	print "Test gMOVE"
	print "Draw circles at each new point to indicate move"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gat 200,100 : gcircle 1
	gmove 100,50 : gcircle 112
	gmove -75,-50 : gcircle 90
	get%:
	gcls
	gclose id1%
	cls
endp

proc tgorderandrank:
	local w%,h%,ida%(4),r%,i%
	print "Test gORDER and gRANK"
	pause%:(20)
	rem create several windows to use for this first
	w%=gwidth : h%=gheight
	ida%(1)=gcreate(0,0,w%,h%,1,2)
	gborder 1
	ida%(2)=gcreate(w%/4,h%/4,gwidth/2,gheight/2,1,2)
	gborder 1
	ida%(3)=gcreate(3*w%/8,3*h%/8,gwidth/2,gheight/2,1,2)
	gborder 1
	ida%(4)=gcreate(7*w%/16,7*h%/16,gwidth/2,gheight/2,1,2)
	gborder 1
	
	gorder 1,1
	guse 1
	r%=grank
	if r%<>1 : raise 100 : endif
	i%=1
	while i%<5
		guse ida%(i%)
		r%=grank 
		if r%<>(6-i%)
			hLog%:(khLogAlways%,"ERROR: gRANK "+GEN$(r%,3)+" <> expected "+GEN$(6-i%,3))
			rem raise i%
		endif
		i%=i%+1
	endwh
	
	i%=1
	while i%<5
		gorder ida%(i%),i%
		i%=i%+1
	endwh	
	guse 1
	r%=grank
	if r%<>5 : raise 200 : endif
	i%=1
	while i%<5
		guse ida%(i%)
		r%=grank 
		if r%<>i% : raise i% : endif
		i%=i%+1
	endwh

	i%=1
	while i%<5
		gorder ida%(i%),5-i%
		i%=i%+1
	endwh	
	guse 1
	r%=grank
	if r%<>5 : raise 300 : endif
	i%=1
	while i%<5
		guse ida%(i%)
		r%=grank 
		if r%<>(5-i%) : raise i% : endif
		i%=i%+1
	endwh
	get%:
	i%=1
	while i%<5 
		gclose ida%(i%)
		i%=i%+1
	endwh
	cls
endp

proc tgorigin:
	local x%,y%
	print "Test gORIGINX and gORIGINY"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gborder 1
	id2%=gcreate(350,100,100,50,1,2)
	gborder 1
	x%=goriginx : y%=goriginy
	if (x%<>350 or y%<>100) : raise 1 : endif
	guse id1%
	x%=goriginx : y%=goriginy
	if (x%<>0 or y%<>0) : raise 2 : endif
	pause%:(20)
	gclose id1% : gclose id2%
	cls
endp

proc tgpatt:
	print "Test gPATT"
	print "Setting pixels:"
	print "First using two 16-colour windows"
	pattern:(2,2,0)
	print "Now using pattern in a 4-colour window copied to a 16-colour window"
	pattern:(2,1,0)
	print "Now using pattern in a 16-colour window copied to a 4-colour window"
	pattern:(1,2,0)
	print "Now using pattern in a 2-colour window copied to a 16-colour window"
	pattern:(2,0,0)
	print "Now using pattern in a 16-colour window copied to a 2-colour window"
	pattern:(0,2,0)	
	print "Clearing pixels: (on black)"
	print "First using two 16-colour windows"
	pattern:(2,2,1)
	print "Now using pattern in a 4-colour window copied to a 16-colour window"
	pattern:(2,1,1)
	print "Now using pattern in a 16-colour window copied to a 4-colour window"
	pattern:(1,2,1)
	print "Now using pattern in a 2-colour window copied to a 16-colour window"
	pattern:(2,0,1)
	print "Now using pattern in a 16-colour window copied to a 2-colour window"
	pattern:(0,2,1)
	print "Inverting pixels: (on black)"
	print "First using two 16-colour windows"
	pattern:(2,2,2)
	print "Now using pattern in a 4-colour window copied to a 16-colour window"
	pattern:(2,1,2)
	print "Now using pattern in a 16-colour window copied to a 4-colour window"
	pattern:(1,2,2)
	print "Now using pattern in a 2-colour window copied to a 16-colour window"
	pattern:(2,0,2)
	print "Now using pattern in a 16-colour window copied to a 2-colour window"
	pattern:(0,2,2)	
	print "Replacing pixels: (on black)"
	print "First using two 16-colour windows"
	pattern:(2,2,3)
	print "Now using pattern in a 4-colour window copied to a 16-colour window"
	pattern:(2,1,3)
	print "Now using pattern in a 16-colour window copied to a 4-colour window"
	pattern:(1,2,3)
	print "Now using pattern in a 2-colour window copied to a 16-colour window"
	pattern:(2,0,3)
	print "Now using pattern in a 16-colour window copied to a 2-colour window"
	pattern:(0,2,3)	
	cls
endp

proc pattern:(m1%,m2%,mode%)
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,m1%)
	id2%=gcreate(0,0,gwidth,40,1,m2%)
	stripes:(0)
	get%:
	gorder id1%,1
	guse id1%
	if mode%<>0
		gcolor 0,0,0
		gfill gwidth,gheight,0
	endif
	gpatt id2%,gwidth,gheight,mode%
	get%:
	gclose id1% : gclose id2%
endp

proc tgpoly:
	local a%(20)
	print "Test gPOLY"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	a%(1)=20 : a%(2)=20
	a%(3)=4
	a%(4)=200 : a%(5)=0
	a%(6)=0 : a%(7)=100
	a%(8)=-200 : a%(9)=0
	a%(10)=0 : a%(11)=-100
	gcolor $0,$0,$0
	gpoly a%()
	a%(1)=140 : a%(2)=20
	gcolor $30,$30,$30
	gpoly a%()
	a%(1)=260 : a%(2)=20
	gcolor $70,$70,$70
	gpoly a%()
	a%(1)=380 : a%(2)=20
	gcolor $C0,$C0,$C0
	gpoly a%()
	a%(1)=500 : a%(2)=20
	gcolor $F0,$F0,$F0
	gpoly a%()
	get%:
	gclose id1%
	cls
endp

proc tgprint:
	local wp%,wc%,text$(200)
	print "Test text printing"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gfill gwidth,gheight,0
	gat 5,20
	gprintB "Text in a cleared box of half screen-width",gwidth/2
	gat 5,40
	gprintB "Centred in a cleared box of full screen-width",gwidth,3
	gat 5,60
	gprintB "This is right-aligned",gwidth/2,1
	gat 5,80
	gprintB "This is left-aligned",gwidth/2,2
	gat 5,105
	gprintB "Top clearance 5, bottom clearance 10",gwidth/2,2,5,10
	gat 5,145
	gprintB "This is centred, m%=200",gwidth,3,0,0,200
	gat 5,165
	gprintB "This is centred, m%=-200",gwidth,3,0,0,-200
	gat 5,185
	gprintB "This is right-aligned, m%=30",gwidth,1,0,0,30
	gat 5,205
	gprintB "This is left-aligned, m%=10",gwidth/2,2,0,0,10
	get%:
	gcls
	gat 5,20
	wc%=gprintclip("This text will be too long to fit in the width allowed",200)
	gat 5,40
	gprint "The number of characters in the above text is ";wc%
	wp%=gtwidth("This test will be too long")
	gat 5,60
	gprint "The length in pixels of the above test is ";wp%
	if 200-wp%>(wp%/wc%) : gprint " ... Incorrect!" : endif
	text$="mmmmmmmmmm"
	wp%=gtwidth(text$)
	gat 5,80
	gprint "Width of "+text$+" is ";wp%
	text$="iiiiiiiiii"
	wp%=gtwidth(text$)
	gat 5,100
	gprint "Width of "+text$+" is ";wp%
	gat 5,120
	gprint "Now in mono"
	gstyle 16
	text$="mmmmmmmmmm"
	wp%=gtwidth(text$)
	gat 5,140
	gprint "Width of "+text$+" is ";wp%
	text$="iiiiiiiiii"
	wp%=gtwidth(text$)
	gat 5,160
	gprint "Width of "+text$+" is ";wp%
	get%:
	gcls
	gstyle 0
	gat 5,20
	gprint "Plain text"
	gat 5,40
	gxprint "Plain text",0
	gat 5,60
	gxprint "Inverse text",1
	gat 5,80
	gxprint "Inverse except corners",2
	gat 5,100
	gxprint "Thin inverse",3
	gat 5,120
	gxprint "Thin inverse excpet corners",4
	gat 5,140
	gxprint "Underlined",5
	gat 5,160
	gxprint "Thin underlined",6
	get%:
	gcls
	gstyle 4
	gat 5,20
	gxprint "Inverse with inverse style",1
	gat 5,40
	gxprint "Inverse except corners with inverse style",2
	gat 5,60
	gxprint "Thin inverse with inverse style",3
	gat 5,80
	gxprint "Thin inverse except corners",4
	gstyle 2
	gat 5,100
	gxprint "Underlined with underlined style",5
	gat 5,120
	gxprint "Thin underlined with underlined style",6
	get%:
	gcls
	gfill gwidth/2, gheight,0
	gstyle 0
	gtmode 1
	gat 5,20
	gprint "Text mode 1 - pixels cleared"
	gat 325,20
	gprint "This should be invisible"
	gtmode 2
	gat 5,50
	gprint "Text mode 2 - pixels inverted"
	gat 325,50
	gprint "This should be visible"
	gtmode 3
	gat 5,80
	gprint "Text mode 3 - pixels replaced"
	gat 325,80
	gprint "This should be visible"
	get%:
	gclose id1%
	cls
endp

proc tgscroll:
	print "Test gSCROLL"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gat 100,100
	gcircle 80
	gcircle 60
	gcircle 40
	gcircle 20
	get%:
	gscroll 100,0
	get%:
	gscroll -100,0
	get%:
	gscroll 0,50
	get%:
	gscroll 0,-50
	get%:
	gscroll 100,0,58,58,84,84
	get%:
	gscroll -100,0,158,58,84,84
	get%:
	gscroll 0,50,58,58,84,84
	get%:
	gscroll 0,-50,58,108,84,84
	get%:
	gcls
	stripes:(0)
	get%:
	gscroll 80,0
	get%:
	gscroll -80,0
	get%:
	gscroll -80,0
	get%:
	gscroll 80,0
	get%:
	gcls
	gclose id1%
	cls
endp

proc tgsetwin:
	print "Test gSETWIN"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	stripes:(0)
	get%:
	gsetwin 0,0,gwidth/2,gheight/2
	get%:
	gsetwin 320,120
	get%:
	gcls
	gclose id1%
	cls
endp

proc tgupdate:
	print "Test gUPDATE"
	pause%:(20)
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	gat 5,20 
	gfont 10
	gprint "Update is ON"
	gat 5,60
	gfill 20,20,0
	pause%:(40)
	gat 5,80
	gcolor $A0,$A0,$A0
	gfill 20,20,0
	gcolor 0,0,0
	gat 325,20
	gprint "Now Turning update OFF and doing the same ..."
	gupdate off
	gat 325,60
	gfill 20,20,0
	pause%:(20)
	gat 325,80
	gcolor $A0,$A0,$A0
	gfill 20,20,0
	gupdate
	get%:
	gclose id1%
	cls
	gupdate on
endp

proc tvis:
	print "Test visibility"
	print "Creating an invisible window and drawing on it"
	id1%=gcreate(0,0,gwidth,gheight,0,2)
	stripes:(0)
	print "Now making the window visible on keypress and invisible again on a second keypress"
	get%:
	gvisible on
	get%:
	gvisible off
	print "Now it's invisible again!"
	gcls
	gclose id1%
	print
	print "Press a key to create a visible window and draw on it"
	print "Press another key to make it invisible"
	get%:
	id1%=gcreate(0,0,gwidth,gheight,1,2)
	stripes:(0)
	get%:
	gvisible off
	print "And press a key to make it visible again"
	get%:
	gvisible on
	pause%:(30)
	gcls
	gclose id1%
	cls
endp


PROC Get%:
	REM get
ENDP


PROC pause%:(duration%)
	rem pause duration%
ENDP

REM End of p16col.tpl
