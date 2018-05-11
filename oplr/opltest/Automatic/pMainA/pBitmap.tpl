REM pBitmap.tpl
REM EPOC OPL automatic test code for bitmaps.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pBitmap", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pBitmap:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tBitmap")
	hCleanUp%:("CleanUp")
endp

CONST KPath$="C:\Opl1993\"

PROC CleanUp:
	trap delete KPath$+"circle"
	trap delete KPath$+"circle.pic"
	trap delete KPath$+"circle.bmp"
	trap delete KPath$+"*.*"
	trap rmdir KPath$
ENDP


proc tbitmap:
	global drv$(255)
	drv$=hDiskName$:+"\Opltest\Data\"
	print "Opler1 Bitmap Tests"
	print
	trap mkdir kpath$
	tfileext:
	t64bitmaps:
	tgbutton:
	tloadbit:
	testerrors:
	print
	print "Opler1 Bitmap Tests Finished OK"
	rem pause 30
endp


proc tfileext:
	local id1%,id2%
	
	print "Test that .pic extension is NOT default"
	id1%=gcreatebit (50,50)
	gat 25,25
	gcircle 20
	gat 10,20
	gsavebit kpath$+"circle"
	gclose id1%
	if not exist (KPath$+"circle") : raise 1 : endif
	if exist (KPath$+"circle.pic") : raise 2 : endif
	
	id1%=gcreatebit (50,50)
	gat 25,25
	gcircle 20
	gat 10,20
	gprint "pic"
	gsavebit KPath$+"circle.pic"
	gclose id1%
	if not exist (KPath$+"circle.pic") : raise 3 : endif
	
	id1%=gloadbit (KPath$+"circle")
	id2%=gcreate (100,100,100,100,1,0)
	gcopy id1%,0,0,50,50,0
	gclose id1%
	print "Should be a circle with no caption"
	gclose id2%
	
	trap delete KPath$+"circle"
	trap delete KPath$+"circle.pic"
	print
	print "OK"
	print
	rem pause 20
endp


proc t64bitmaps:
	local i%,id%(64),id65%
	
	print "Test that 63 bitmaps may be open at the same time"
	rem pause 20
	i%=1
	while i%<64
		id%(i%)=gcreatebit (50,50)
		i%=i%+1
	endwh
	print "63 bitmaps now created + window = 64 drawables"
	print "Attempting to create another"
	
	onerr create65
	id65%=gcreatebit (50,50)
	onerr off
	gclose id65%
	raise 1
create65::
	onerr off
	if err<>-117 : print err : raise 2 : endif
	
	i%=63
	while i%>=1
		gclose id%(i%)	
		i%=i%-1
	endwh
	print
	print "OK"
	print
	rem pause 20
endp

proc tgbutton:
	global idw%,idbm%,idm1%,idm2%
	local caption1$(255),caption2$(255),excess$(255)
	local ev&(16)

	print "Test gButton"
	print
	rem pause 20

	rem create bitmap
	idbm%=gcreatebit (50,50)
	gat 25,25
	gcircle 20
	gcircle 15
	gcircle 10
	gcircle 5
	setdoc KPath$+"circles.bmp"
	gsavebit KPath$+"circles.bmp"
	gclose idbm%
	if not exist (KPath$+"circles.bmp") : raise 1 : endif
	
	idw%=gcreate(0,0,gwidth,gheight,1,2)
	idbm%=gloadbit(KPath$+"circles.bmp")
	guse idw%
	
	rem test buttons sharing excess
	caption1$="Buttons with bitmaps but without masks"
	caption2$="Buttons with identical bitmaps and masks"	
	excess$=" (Excess shared)"	
	gat 20,15
	gfont KFontCourierNormal11&
	gstyle 17
	gprint caption1$+excess$

	gfont KFontSquashed&
	gstyle 0
	gat 20,25
	gbutton "",2,60,60,0,idbm%
	gat 120,25
	gbutton "Circles",2,100,60,0,idbm%,0,0
	gat 260,25
	gbutton "Circles",2,60,80,0,idbm%,0,1
	gat 380,25
	gbutton "Circles",2,60,80,0,idbm%,0,2
	gat 500,25
	gbutton "Circles",2,100,60,0,idbm%,0,3
	gat 20,140
	gbutton "",2,60,60,0,idbm%,idbm%,0
	gat 120,140
	gbutton "Circles",2,100,60,0,idbm%,idbm%,0
	gat 260,140
	gbutton "Circles",2,60,80,0,idbm%,idbm%,1
	gat 380,140
	gbutton "Circles",2,60,80,0,idbm%,idbm%,2
	gat 500,140
	gbutton "Circles",2,100,60,0,idbm%,idbm%,3
	
	gfont KFontCourierNormal11&
	gstyle 16
	gat 20,125
	gprint "No text"
	gat 120,125
	gprint "Text right"
	gat 260,125
	gprint "Text bottom"
	gat 380,125
	gprint "Text top"
	gat 500,125
	gprint "Text left"
	
	gat 20,gheight-5
	gstyle 17
	gprint caption2$+excess$
	rem get
	gcls
	
	rem test excess to text/picture
	excess:(0,$10,$20)
	excess:(1,$10,$20)
	excess:(2,$10,$20)
	excess:(3,$10,$20)
	
	rem test excess shared
	excess:(0,$00,$10)
	excess:(1,$00,$10)
	excess:(2,$00,$10)
	excess:(3,$00,$10)
	
	rem test depression of button
	gfont KFontSquashed&
	gstyle 0
	gat 280,80	
	gbutton "Circles",2,60,80,0,idbm%,idbm%,$1
	gfont KFontCourierNormal11&
	gstyle 16
	gat 20,20
	gprint "Press a key to press button"
	rem get
	gfont KFontSquashed&
	gstyle 0
	gat 280,80
	gbutton "Circles",2,60,80,1,idbm%,idbm%,$1

	rem button state 2 is no longer valid.
	rem gbutton "Circles",2,60,80,2,idbm%,idbm%,$1

	gbutton "Circles",2,60,80,1,idbm%,idbm%,$1
	gbutton "Circles",2,60,80,0,idbm%,idbm%,$1
	rem get
	gcls
	
	rem use some different masks
	rem create new mask
	idm1%=gcreatebit(50,50)
	gat 20,20
	gcolor 0,0,0
	gfill 10,10,0
	setdoc KPath$+"fill1.bmp"
	gsavebit KPath$+"fill1.bmp"
	idm2%=gcreatebit(50,50)
	gat 15,15
	gcolor 0,0,0
	gfill 20,20,0
	setdoc KPath$+"fill2.bmp"
	gsavebit KPath$+"fill2.bmp"

	guse idw%
	caption1$="Buttons with mask1"
	caption2$="Buttons with mask2"	
	excess$=" (Excess shared)"	
	gat 20,15
	gfont KFontCourierNormal11&
	gstyle 17
	gprint caption1$+excess$

	gfont KFontSquashed&
	gstyle 0
	gat 20,25
	gbutton "",2,60,60,0,idbm%,idm1%
	gat 120,25
	gbutton "1 Circle",2,100,60,0,idbm%,idm1%,0
	gat 260,25
	gbutton "1 Circle",2,60,80,0,idbm%,idm1%,1
	gat 380,25
	gbutton "1 Circle",2,60,80,0,idbm%,idm1%,2
	gat 500,25
	gbutton "1 Circle",2,100,60,0,idbm%,idm1%,3
	gat 20,140
	gbutton "",2,60,60,0,idbm%,idm2%
	gat 120,140
	gbutton "2 Circles",2,100,60,0,idbm%,idm2%,0
	gat 260,140
	gbutton "2 Circles",2,60,80,0,idbm%,idm2%,1
	gat 380,140
	gbutton "2 Circles",2,60,80,0,idbm%,idm2%,2
	gat 500,140
	gbutton "2 Circles",2,100,60,0,idbm%,idm2%,3
	
	gfont KFontCourierNormal11&
	gstyle 16
	gat 20,125
	gprint "No text"
	gat 120,125
	gprint "Text right"
	gat 260,125
	gprint "Text bottom"
	gat 380,125
	gprint "Text top"
	gat 500,125
	gprint "Text left"
	gat 20,gheight-5
	gstyle 17
	gprint caption2$+excess$
	rem get
	gcls
	gclose idbm% :gclose idm1% : gclose idm2%
	gclose idw%
endp


proc excess:(textpos%,ex1%,ex2%)
	local pex$(50),tex$(50),sex$(50),title1$(50),title2$(50)
	pex$="Excess space to picture"
	tex$="Excess space to text"
	sex$="Share excess space"
	if ex1%=0 : title1$=sex$ :endif
	if ex1%=$10 : title1$=tex$ :endif
	if ex1%=$20 : title1$=pex$ :endif
	if ex2%=0 : title2$=sex$ :endif
	if ex2%=$10 : title2$=tex$ :endif
	if ex2%=$20 : title2$=pex$ :endif
	gfont KFontCourierNormal11&
	gstyle 16
	gat 20,15
	gprint title1$
	gat 320,15
	gprint title2$
	gat 20,20
	gfont KFontSquashed&
	gstyle 0
	gbutton "Circles",2,200,200,0,idbm%,0,textpos% or ex1%
	gat 320,20
	gbutton "Circles",2,200,200,0,idbm%,0,textpos% or ex2%	
	rem get
	gcls	
endp


proc tloadbit:
	local idw%,idbm%
	print "Test gLOADBIT of mbm created by BMCONV"
	if drv$="0" : print "Cannot find bitmap" : return : endif
	print "Should display a happy face!"
	print
	rem pause 20
	idbm%=gloadbit(drv$+"pface.mbm")
	idw%=gcreate (0,0,gwidth,gheight,1,2)
	gcopy idbm%,0,0,gwidth,gheight,2
	gclose idbm%
	pause 15
	gclose idw%	
endp


proc testerrors:
	local idw1%,idw2%,idbm%
	
	print "Test invalid argument when use a window on a button"
	print
	rem pause 20
	
	rem create window instead of bitmap
	idw1%=gcreate(500,100,50,50,1,2)
	gat 25,25
	gcircle 20
	gcircle 15
	gcircle 10
	gcircle 5
	
	guse 1
	idw2%=gcreate(0,0,gwidth,gheight,1,0)
	gfont KFontSquashed&
	gstyle 0
	gat 20,25
	onerr inval1
	gbutton "Circles",2,60,100,0,idw1%
	onerr off
	raise 1
inval1::
	onerr off
	if err<>-2 : print err : raise 2 : endif
	
	idbm%=gloadbit(KPath$+"circles.bmp")
	onerr inval2
	gbutton "Circles",2,60,100,0,idbm%,idw1%
	onerr off
	raise 3
inval2::
	onerr off
	if err<>-2 : print err : raise 4 : endif
	
	gclose idbm% : gclose idw1% : gclose idw2%
	
	rem test cannot load a non-existent bitmap
	onerr inval3
	idbm%=gloadbit (KPath$+"nexist.bmp")
	onerr off
	raise 5
inval3::
	onerr off 
	if err<>-33 : raise 6 : endif
	
	rem test can save bitmap when default window current
	gsavebit KPath$+"default.bmp"
	trap delete KPath$+"default.bmp"
		
	print "OK"
	rem pause 20
endp


REM End of pBitmap.tpl

