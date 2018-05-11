REM pCreatebit.tpl
REM EPOC OPL automatic test code for gCREATEBIT
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pCreatebit", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pCreatebit:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tCreatebit")
	hCleanUp%:("CleanUp")
endp

CONST KPath$="C:\Opl1993\"

PROC Cleanup:
	trap delete KPath$+"*.*"
	trap rmdir KPath$
ENDP


proc tcreatebit:
	trap mkdir KPath$
	print "Opler1 gCREATEBIT with Colour Mode Tests"
	print
	genbitmaps:
	drawbitmaps:
	tinvalid:
	delbitmaps:
	print
	print "Opler1 gCREATEBIT with Colour Mode Tests Finished OK"
	rem pause 30
endp


proc genbitmaps:
	local id%
	
	id%=gcreatebit(100,100)
	stripes:(0,0)
	gsavebit(KPath$+"stripesdef.bmp")
	gclose id%

	rem b/w
	id%=gcreatebit(100,100,0)
	stripes:(0,0)
	gsavebit(KPath$+"stripes0.bmp")
	gclose id%
	
	rem 4 col
	id%=gcreatebit(100,100,1)
	stripes:(0,0)
	gsavebit(KPath$+"stripes1.bmp")
	gclose id%

	rem 16 col
	id%=gcreatebit(100,100,2)
	stripes:(0,0)
	gsavebit(KPath$+"stripes2.bmp")
	gclose id%
	
	print "Created bitmaps OK"
	rem pause 20
endp


proc stripes:(x%,y%)
	local col%,i%,xx%
	xx%=x%
	col%=0
	while i%<16	
		gcolor col%,col%,col%
		gat xx%,y%
		gfill 6,100,0
		col%=col%+$10
		xx%=xx%+6
		i%=i%+1
	endwh
endp


proc drawbitmaps:
	local id%,winid%
	
	cls
	print "Drawing in default window"
	draw:(1)	
	gcls : cls
	
	print "Drawing in 16-colour window"	
	winid%=gcreate(0,20,gwidth,gheight-20,1,2)
	draw:(winid%)
	gclose winid%
	gcls : cls
	
	print "Drawing in 4-colour window"	
	winid%=gcreate(0,20,gwidth,gheight-20,1,1)
	draw:(winid%)
	gclose winid%
	gcls : cls
	
	print "Drawing in 2-colour window"	
	winid%=gcreate(0,20,gwidth,gheight-20,1,0)
	draw:(winid%)
	gclose winid%
	gcls : cls
endp


proc draw:(w%)
	local id%
	
	gfont 10
	id%=gloadbit(KPath$+"stripesdef.bmp")
	guse w% : gat 20,40 : gcopy id%,0,0,100,100,0
	gat 20,160 : gprint "default"
	gclose id%
	id%=gloadbit(KPath$+"stripes0.bmp")
	guse w% : gat 140,40 : gcopy id%,0,0,100,100,0
	gat 140,160 : gprint "2-colour"
	gclose id%
	id%=gloadbit(KPath$+"stripes1.bmp")
	guse w% : gat 260,40 : gcopy id%,0,0,100,100,0
	gat 280,160 : gprint "4-colour"
	gclose id%
	id%=gloadbit(KPath$+"stripes2.bmp")
	guse w% : gat 380,40 : gcopy id%,0,0,100,100,0
	gat 400,160 : gprint "16-colour"
	gclose id%
	stripes:(520,40)
	gcolor 0,0,0
	gat 520,160 : gprint "drawn to window"
	rem get
endp

proc tinvalid:
	local id%
	
	print "Test Bitmap Too Big gives error"

	onerr e3
	rem watch out for those 64Mb machines!
	id%=gcreatebit(32767,32767,2)
	onerr off
	raise 5
e3::
	onerr off
	if err<>-10 : print err : raise 6 : endif	
endp


proc delbitmaps:
	
	trap delete KPath$+"stripesdef.bmp"
	trap delete KPath$+"stripes0.bmp"
	trap delete KPath$+"stripes1.bmp"
	trap delete KPath$+"stripes2.bmp"
endp

REM End of pCreatebit.tpl
