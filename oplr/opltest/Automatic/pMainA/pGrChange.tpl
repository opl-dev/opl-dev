REM pgrchange.tpl
REM EPOC OPL automatic test code for changed opler1 graphics opcodes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

declare external

const KLineTo%=0
const KLineBy%=1

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pgrchange", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pgrchange:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)

	hRunTest%:("t64separatewindows")
	hRunTest%:("t64nestedwindows")
	hRunTest%:("tborder")
	hRunTest%:("tcreate")
	hRunTest%:("tggrey")
	hRunTest%:("tlineto")
	hRunTest%:("tlineby")
	hRunTest%:("txborder")

rem	hCleanUp%:("CleanUp")
endp


proc t64separatewindows:
	local x%,y%,dx%,dy%,w%,h%,id%(64),i%,j%
	print "Test that it is possible to have 64 windows/drawables on the screen"
	rem pause 20
	cls
	dx%=40 : dy%=40 
	x%=40 : y%=20
	i%=1
	w%=20 : h%=20
	while i%<64
		j%=1
		while j%<=15 and i%<64
			id%(i%)=gcreate (x%,y%,w%,h%,1,0)
			gborder 1
			x%=x%+dx%
			j%=j%+1
			i%=i%+1
		endwh
		x%=dx%
		y%=y%+dy%
	endwh
	print "64 windows open"
	
	print "Attempting to open 65th window"
	onerr toomany
	gcreate (x%,y%,w%,h%,1,0)
	onerr off
	raise 1
toomany::
	onerr off
	if err<>-117 : print err$(err) : raise 2 : endif
	print "Error reported correctly"
	
	rem pause 30
	i%=63
	while i%>0
		gclose id%(i%)
		i%=i%-1
	endwh
	rem pause 30
	print "OK"
	print 
endp

proc t64nestedwindows:
	local width%,height%,x%,y%,dx%,dy%,w%,h%,id%(64),i%
	print "Test that it is possible to have 64 windows/drawables on the screen"
	rem pause 20
	print "This test if slow - press Esc to  skip"
	rem if get=27 : return : endif
	width%=gwidth
	height%=gheight
	dx%=width%/(128) : dy%=height%/(128)
	x%=0 : y%=0 : i%=1
	while i%<64
		w%=width%-x%*2
		h%=height%-y%*2
		id%(i%)=gcreate (x%,y%,w%,h%,1,0)
		gborder 1
		x%=x%+dx%
		y%=y%+dy%
		i%=i%+1
	endwh
	print "64 windows open"
	
	print "Attempting to open 65th drawable"
	onerr open65
	w%=width%-x%*2
	h%=height%-y%*2
	id%(i%)=gcreate (x%,y%,w%,h%,1,0)
	gborder 1
	onerr off
	raise 1
open65::
	onerr off
	if err<>-117 : print err$(err) : raise 2 : endif
	print "Error reported correctly"
	rem pause 30
	
	i%=63
	while i%>0
		gclose id%(i%)
		i%=i%-1
	endwh
	rem pause 30
endp

proc tborder:
	local id1%,id2%
	cls
	print "Test gBorder"
	print "1. These drawables should have shadows"
	print "Press a key to remove the shadow and again to replace shadow"
	id1%=gcreate (110,70,100,90,1,1)
	gborder 1
	rem get
	gborder 2
	rem get
	gborder 1
	id2%=gcreate (430,70,100,90,1,1)
	gborder 3
	rem get
	gborder 4
	rem get
	gborder 3
	rem pause 20
	gclose id1%
	gclose id2%
	cls

	print "2. These drawables should have shadows and rounded corners"
	print "Press a key to remove the shadow"	
	id1%=gcreate (110,70,100,90,1,1)
	gborder $201
	rem get
	gborder $202
	id2%=gcreate (430,70,100,90,1,1)
	gborder $203
	rem get
 	gborder $204
	rem pause 20
	gclose id1%
	gclose id2%
	cls
	
	print "3. These are boxes"
	print "Press a key to draw a border with a 1 pixel gap"
	id1%=gcreate (110,70,100,90,1,1)
	gat 20,20
	gbox 60,50 
	rem get
	gborder $100,60,50
	id2%=gcreate (430,70,100,90,1,1)
	gat 20,20
	gbox 60,50
	rem get
	gborder $100,60,50
	rem pause 20
	gclose id1%
	gclose id2%
	cls
	
	print "4. These drawables should have shadows (borders at 1 pixel gap)"
	print "Press a key to remove the shadow"
	id1%=gcreate (110,70,100,90,1,1)
	gborder $101
	rem get
	gborder $102
	id2%=gcreate (430,70,100,90,1,1)
 	gborder $103
	rem get
	gborder $104
	rem pause 20
	gclose id1%
	gclose id2%
	cls

	print "5. These should have borders with shadows inside the drawable"
	print "Press a key to remove the shadows"
	id1%=gcreate (110,70,100,90,1,1)
	gborder 2
	gat 10,10
	gborder 1,80,70
	rem get
	gborder 2,80,70
	id2%=gcreate (430,70,100,90,1,1)
	gborder 4
	gat 10,10
	gborder 3,80,70
	rem get
	gborder 4,80,70
	rem pause 20
	gclose id1%
	gclose id2%
	cls	
endp

proc tcreate:
	local id1%,id2%,id3%,id4%
	print "Test gCreate with shadows"
	
	print "1. Windows with shadows"
	id1%=gcreate (110,35,100,50,1,$412)
	gborder 1
	id2%=gcreate (430,35,100,50,1,$411)
	gborder 1
	id3%=gcreate (110,150,100,50,1,$810)
	gborder 1
	id4%=gcreate (430,150,100,50,1,$811)
	gborder 1
	rem pause 30
	gclose id1% : gclose id2% : gclose id3% : gclose id4%
	cls
	
	print "2. Windows with shadows disabled"
	id1%=gcreate (110,35,100,50,1,$402)
	gborder 2
	id2%=gcreate (430,35,100,50,1,$401)
	gborder 2
	id3%=gcreate (110,150,100,50,1,$800)
	gborder 2
	id4%=gcreate (430,150,100,50,1,$801)
	gborder 2
	rem pause 30
	gclose id1% : gclose id2% : gclose id3% : gclose id4%

	cls	
	print "3. Nested windows with shadows"
	id1%=gcreate (50,35,540,160,1,$802)
	gborder 2
	id2%=gcreate (80,65,480,100,1,$611)
	gborder 2
	id3%=gcreate (100,85,440,60,1,$410)
	gborder 2
	id4%=gcreate (120,95,400,40,1,$211)
	gborder 2
	rem pause 30
	gclose id1% : gclose id2% : gclose id3% : gclose id4%
	cls
endp

proc tggrey:
	local id%,i%,x%
	
	print "Test gGREY"
	print "Filling rectangles of the screen with gGREY i%"
	rem pause 30	
	while i%<16
		gat x%,0
		ggrey i%
		gfill 40,gwidth,0
		i%=i%+1
		x%=x%+40
	endwh
	rem pause 30
	cls : gcls
endp


proc tline:(aType%)
	local a%(26),i%
	gat 50,50
	if aType%=KLineTo%
		glineto 100,50
		gpeekline 1,50,50,a%(),208,1
	else
		glineby 50,0
		gpeekline 1,50,50,a%(),208
	endif
	i%=1
	while i%<=4
		if aType%=KLineTo%
			if i%<=6 and a%(i%)<>$0 : raise i% : endif
			if i%=7 and a%(i%)<>$ffc0 : raise i% : endif
			if i%>7 and a%(i%)<>$ffff : raise i% : endif
		else
			if i%<=3 and a%(i%)<>$ffff : raise i% : endif
			if i%=4 and a%(i%)<>$3 : raise i% : endif
			if i%>4 and a%(i%)<>0 : raise i% : endif
		endif
		i%=i%+1
	endwh
	gcls
	
	print "These shapes should have pixels set at bottom right of all lines"
	print "Checked by gPeekline"
	gat 50,50
	if aType%=KLineTo%
		glineto 50,150
		glineto 200,150
		glineto 200,50
		glineto 50,50
	else
		glineby 0,100
		glineby 150,0
		glineby 0,-100
		glineby -150,0
	endif
	gpeekline 1,50,50,a%(),208,-1
	i%=1
	while i%<=9
		if a%(i%)<>$ffff : raise i% : endif
		i%=i%+1
	endwh
	if a%(10)<>$7f : raise 10 : endif
	gpeekline 1,50,150,a%(),208,-1
	i%=1
	while i%<=9
		if a%(i%)<>$ffff : raise i%+10 : endif
		i%=i%+1
	endwh
	if a%(10)<>$7f : raise 20 : endif

	gat 400,20
	if aType%=KLineTo%
		glineto 460,80
		glineto 400,140
		glineto 340,80
		glineto 400,20
	else
		glineby 60,60
		glineby -60,60
		glineby -60,-60
		glineby 60,-60
	endif
	gpeekline 1,340,20,a%(),208,-1
	i%=1
	while i%<>4 and i%<=13
		if a%(i%)<>0 : raise i% : endif
		i%=i%+1
	endwh
	if a%(4)<>$1000 : raise 7 : endif
	gpeekline 1,340,80,a%(),208,-1
	if a%(1)<>$1 : print a%(1) : raise 1 : endif
	i%=2
	while i%<=7
		if a%(i%)<>0 : raise i% : endif
		i%=i%+1
	endwh
	if a%(8)<>$100 : raise 13 : endif
	gpeekline 1,340,140,a%(),208,-1
	i%=1
	while i%<>4 and i%<=13
		if a%(i%)<>0 : raise i% : endif
		i%=i%+1
	endwh
	if a%(4)<>$1000 : raise 7 : endif
	gcls
endp

proc tlineto:
	tline:(KLineTo%)
endp

proc tlineby:
	tline:(KLineBy%)
endp

proc drawxborder:(flag%)
	local id%
	id%=gcreate (110,70,420,100,1,1)
	gxborder 2,flag%
	rem pause 30
	gclose id%
	cls
endp

proc txborder:
	local id1%,id2%
	print "Test gXBorder"
	print "1. No border"
	drawxborder:(0)
	print "2. Single black border"
	drawxborder:(1)
	print "3. Shallow-sunken border"
	drawxborder:($42)
	print "4. Deep-sunken border"
	drawxborder:($44)
	print "5. Deep-sunken border with outline"
	drawxborder:($54)
	print "6. Shallow-raised border"
	drawxborder:($82)
	print "7. Deep-raised border"
	drawxborder:($84)
	print "8. Deep-raised border with outline"
	drawxborder:($94)
	print "9. Vertical bar"
	id1%=gcreate (320,70,210,100,1,1)
	gxborder 2,$22
	id2%=gcreate (110,70,210,100,1,1)
	gxborder 2,$22
	rem pause 30
	gclose id1% : gclose id2%
	cls
	print "10. Horizontal bar"
	id1%=gcreate (110,70,420,50,1,1)
	gxborder 2,$2a
	id2%=gcreate (110,120,420,50,1,1)
	gxborder 2,$2a
	rem pause 30
	gclose id1% : gclose id2%
	cls
endp


REM End of pGrChange.tpl


