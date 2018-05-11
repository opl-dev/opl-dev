REM pgr32.tpl
REM EPOC OPL automatic test code for opler1 graphics.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pgr32", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pgr32:
	rem test new graphics keywords
	rem print "Opler1 gCOLOR, gCIRCLE and gELLIPSE Tests"
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)

	hRunTest%:("tgcolor")
	hRunTest%:("tgcirc")
	hRunTest%:("tgellip")
	hRunTest%:("squashcirc")

rem	hCleanUp%:("CleanUp")
endp


proc tgcolor:
	local col%,col1%,ii%,dif%,width&,id%
	local a%(64),i%
	
	print "Test gCOLOR"
	print "16 colour window showing all shades of grey"
	rem pause 20
	id%=gcreate (0,0,gwidth,gheight,1,2)
	width&=16 : dif%=16
	col%=0
	ii%=(gwidth+width&-1)/width&
	while ii%>0
		gcolor col%,col%,col%
		gfill width&,gheight,0
		gmove width&,0
		col%=col%+dif%
		if col%>255
			col%=col%-32
			dif%=-16
		elseif col%<0
			col%=col%+32
			dif%=16
		endif
		ii%=ii%-1
	endwh
	rem pause 20
	gcls
	gclose id%
	
	print "16 colour window with red biased colours"
	rem pause 20
	id%=gcreate (0,0,gwidth,gheight,1,2)
	width&=32
	col%=0 : col1%=16
	ii%=(gwidth+width&-1)/width&
	while ii%>0
		gcolor col1%,col%,col%
		gfill width&,gheight,0
		gmove width&,0
		col%=col%+dif%
		col1%=col1%+dif%
		if col1%>255
			col1%=col1%-32
			col%=col%-32
			dif%=-16
		elseif col%<0
			col%=col%+32
			col1%=col1%+32
			dif%=16
		endif
		ii%=ii%-1
	endwh
	rem pause 20
	gcls
	gclose id%
	
	print "16 colour window with red and blue biased colours"
	rem pause 20
	id%=gcreate (0,0,gwidth,gheight,1,2)
	col%=0 : col1%=16
	ii%=(gwidth+width&-1)/width&
	while ii%>0
		gcolor col1%,col1%,col%
		gfill width&,gheight,0
		gmove width&,0
		col%=col%+dif%
		col1%=col1%+dif%
		if col1%>255
			col%=col%-32
			col1%=col1%-32
			dif%=-16
		elseif col%<0
			col%=col%+32
			col1%=col1%+32
			dif%=16
		endif
		ii%=ii%-1
	endwh
	rem pause 20
	gcls
	gclose id%
	
	print "16 colour window with blue biased colours"
	rem pause 20
	id%=gcreate (0,0,gwidth,gheight,1,2)
	col%=0 : col1%=16
	ii%=(gwidth+width&-1)/width&
	while ii%>0
		gcolor col%,col%,col1%
		gfill width&,gheight,0
		gmove width&,0
		col%=col%+dif%
		col1%=col1%+dif%
		if col1%>255
			col%=col%-32
			col1%=col1%-32
			dif%=-16
		elseif col%<0
			col%=col%+32
			col1%=col1%+32
			dif%=16
		endif
		ii%=ii%-1
	endwh
	rem pause 20
	gcls
	gclose id%
	print
endp	

proc tgcirc:
	local id%
	
	rem test drawing of circles
	print "Test gCIRCLE"
	rem pause 20
	cls
	gfont KFontCourierNormal11&
	gstyle 0
	
	drawcircle:(0,0,40)
	drawcircle:(100,100,100)
	drawcircle:(320,120,120)	
	drawcircle:(320,110,320)	
	drawcircle:(420,180,10)
	drawcircle:(640,230,30)
	drawcircle:(300,50,32767)
	drawcircle:(320,115,0)
	drawcircle:(360,70,-20)
	
	drawfilledcircle:(0,0,40,1)
	drawfilledcircle:(100,100,100,1)
	drawfilledcircle:(320,120,120,1)	
	drawfilledcircle:(320,110,320,1)	
	drawfilledcircle:(420,180,10,1)
	drawfilledcircle:(640,230,30,1)
	drawfilledcircle:(300,50,32767,1)
	drawfilledcircle:(320,115,0,1)
	drawfilledcircle:(360,70,-20,1)
	
	drawfilledcircle:(0,0,40,0)
	drawfilledcircle:(100,100,100,0)
	drawfilledcircle:(320,120,120,0)	
	drawfilledcircle:(320,110,320,0)	
	drawfilledcircle:(420,180,10,0)
	drawfilledcircle:(640,230,30,0)
	drawfilledcircle:(300,50,32767,0)
	drawfilledcircle:(320,115,0,0)
	drawfilledcircle:(360,70,-20,0)
	
	gcolor $0,$0,$0
	print "Colour is black"
	drawfilledcircle:(320,120,40,-1)
	gcolor $50,$50,$50
	print "Colour is dark grey"
	drawfilledcircle:(320,120,40,1)
	gcolor $a0,$a0,$a0
	print "Colour is light grey"
	drawfilledcircle:(320,120,40,1)
	gcolor $f0,$f0,$f0
	print "Colour is white"
	drawfilledcircle:(320,120,40,1)
	gcolor $80,$80,$80
	print "Colour is dithered grey"
	drawfilledcircle:(320,120,40,1)
	
	drawfilledcircle:(320,120,40,-32768)
	drawfilledcircle:(320,120,40,32767)

	onerr circ1
	drawcircle:(400,50,32768)
	onerr off
	raise 1
circ1::
	onerr off
	if err<>-6 : print err$(err) : raise 2 : endif
	print "Drawing circle with radius 32768 causes overflow"
	
	onerr circ2
	drawfilledcircle:(400,50,30,32768)
	onerr off
	raise 3
circ2::
	onerr off
	if err<>-6 : print err$(err) : raise 4 : endif
	print "Drawing circle with fill param 32768 causes overflow"
	
	print
	rem pause 20
endp

proc drawcircle:(x%,y%,r%)
	rem draws circle at (x%,y%) with radius r%
	gat x%,y%
	gcircle r%
	gprint "Circle radius = ";r%
	if r%<0 : gprint " does nothing" : endif
	rem pause -20
	gcls
endp

proc drawfilledcircle:(x%,y%,r%,f%)
	rem draws circle at (x%,y%) with radius r% filled if f%<>0
	gat x%,y%
	gcircle r%,f%
	gcolor 0,0,0
	print "Circle radius = ";r%;", Fill = ";f%;
	if r%<0 : print " does nothing" : endif
	rem pause -20
	gcls
	cls
endp

proc tgellip:
	local id%
	
	print "Test gELLIPSE"
	rem pause 20
	gfont KFontCourierNormal11&
	gstyle 0
	cls
	
	drawellipse:(100,40,100,40)
	drawellipse:(320,115,320,115)
	drawellipse:(320,115,20,100)
	drawellipse:(320,115,20,200)
	drawellipse:(250,115,-20,100)
	drawellipse:(250,115,20,-100)
	drawellipse:(250,115,-20,-100)
	drawellipse:(320,115,0,100)
	drawellipse:(320,115,20,0)
	drawellipse:(320,115,0,0)	
	drawellipse:(320,115,50,50)
	
	drawfilledellipse:(100,40,100,40,1)
	drawfilledellipse:(320,115,320,115,1)
	drawfilledellipse:(320,115,20,100,1)
	drawfilledellipse:(320,115,20,200,1)
	drawfilledellipse:(250,115,-20,100,1)
	drawfilledellipse:(250,115,20,-100,1)
	drawfilledellipse:(250,115,-20,-100,1)
	drawfilledellipse:(320,115,0,100,1)
	drawfilledellipse:(320,115,20,0,1)
	drawfilledellipse:(320,115,0,0,1)	
	drawfilledellipse:(320,115,50,50,1)

	drawfilledellipse:(100,40,100,40,0)
	drawfilledellipse:(320,115,320,115,0)
	drawfilledellipse:(320,115,20,100,0)
	drawfilledellipse:(320,115,20,200,0)
	drawfilledellipse:(250,115,-20,100,0)
	drawfilledellipse:(250,115,20,-100,0)
	drawfilledellipse:(250,115,-20,-100,0)
	drawfilledellipse:(320,115,0,100,0)
	drawfilledellipse:(320,115,20,0,0)
	drawfilledellipse:(320,115,0,0,0)	
	drawfilledellipse:(320,115,50,50,0)
	
	gcolor $0,$0,$0
	print "Colour is black"
	drawfilledellipse:(320,120,100,40,-1)
	gcolor $50,$50,$50
	print "Colour is dark grey"
	drawfilledellipse:(320,120,100,40,1)
	gcolor $a0,$a0,$a0
	print "Colour is light grey"
	drawfilledellipse:(320,120,100,40,-1)
	gcolor $80,$80,$80
	print "Colour is dithered grey"
	drawfilledellipse:(320,120,100,40,1)
	gcolor $f0,$f0,$f0
	print "Colour is white"
	drawfilledellipse:(320,120,100,40,-1)
	
	onerr ellip1
	drawellipse:(320,115,32768,100)
	onerr off
	raise 1
ellip1::
	onerr off
	if err<>-6 : print err$(err) : raise 2 : endif

	onerr ellip2
	drawellipse:(320,115,100,32768)
	onerr off
	raise 3
ellip2::
	onerr off
	if err<>-6 : print err$(err) : raise 4 : endif
	
	onerr ellip3
	drawellipse:(320,115,32768,32768)
	onerr off
	raise 5
ellip3::
	onerr off
	if err<>-6 : print err$(err) : raise 6 : endif

	print "Drawing ellipse with either or both radii 32768 causes overflow"
	
	onerr ellip4
	drawfilledellipse:(320,115,32768,100,32768)
	onerr off
	raise 7
ellip4::
	onerr off
	if err<>-6 : print err$(err) : raise 8 : endif

	print "Drawing ellipse with fill param 32768 causes overflow"
	print
	rem pause 20
endp

proc drawellipse:(x%,y%,w%,h%)
	rem draws ellipse at (x%,y%) with width w% and height h%
	gat x%,y%
	gellipse w%,h%
	gprint "Ellipse width = ";w%;", height = ";h%
	if w%<0 or h%<0 : gprint " does nothing" : endif
	rem pause -20
	gcls
endp

proc drawfilledellipse:(x%,y%,w%,h%,f%)
	rem draws ellipse at (x%,y%) with width w% and height h% and fill%=f%
	gat x%,y%
	gellipse w%,h%,f%
	print "Ellipse width = ";w%;", height = ";h%;", fill = ";f%;
	if w%<0 or h%<0 : print " does nothing" : endif
	rem pause -20
	gcls
	cls
endp

proc squashcirc:
	local id%
	
	print "More Circle and Ellipse Tests"
	rem pause 20
	id%=gcreate (0,0,gwidth,gheight,1,0)
	gfont KFontCourierNormal11&
	gstyle 0
	gat 320,115
	gcircle 90
	gcls
	gat 320,115
	gellipse 100,80
	gcls
	gat 320,115
	gellipse 110,70
	gcls
	gat 320,115
	gellipse 120,60
	gcls
	gat 320,115
	gellipse 130,50
	gcls
	gat 320,115
	gellipse 140,40
	gcls
	gat 320,115
	gellipse 150,30
	gcls
	gat 320,115
	gellipse 160,20
	rem pause -20
	gcls
	gat 10,15
	gprint "This tests position is maintained across calls"
	gat 320,115
	gcircle 90
	gellipse 100,80
	gellipse 110,70
	gellipse 120,60
	gellipse 130,50
	gellipse 140,40
	gellipse 150,30
	gellipse 160,20
	rem pause 30
	gclose id%
endp


REM End of pgr32.tpl

