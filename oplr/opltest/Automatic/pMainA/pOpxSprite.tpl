REM pOpxSprite.tpl
REM EPOC OPL automatic test code for Sprite OPX.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
include "bmp.oxh"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pOpxSprite", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP

const kpath$="c:\Opl1993\"


proc pOpxSprite:
	global sp1&,sp2&
	global drv$(255)

	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	drv$=hDiskName$:
	trap mkdir kpath$
	
	hRunTest%:("delbitmaps")
	hRunTest%:("genbitmaps")
	hRunTest%:("texamples")
	
	hRunTest%:("tappend")
	hRunTest%:("tchange")
	hRunTest%:("tdraw")
	hRunTest%:("tpos")
	hRunTest%:("tdelete")
	hRunTest%:("tuse")
	
	hRunTest%:("dispsprite")	
	hRunTest%:("twosprites")
	hRunTest%:("lots")
	hRunTest%:("delbitmaps")

	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	trap delete kpath$+"*.*"
	trap rmdir kpath$
ENDP


proc genbitmaps:
	local id%
	
	id%=gcreatebit(50,50,1)
	gcolor $a0,$a0,$a0
	gat 20,20 : gfill 10,10,0
	gsavebit(kpath$+"grey1.bmp")
	gclose id%
	
	id%=gcreatebit(50,50,1)
	gcolor $50,$50,$50
	gat 10,10 : gfill 30,5,0
	gat 10,15 : gfill 5,25,0
	gat 15,35 : gfill 25,5,0
	gat 35,15 : gfill 5,20,0	
	gsavebit(kpath$+"grey2.bmp")
	gclose id%
	
	id%=gcreatebit(50,50,1)
	gcolor 0,0,0
	gat 0,0 : gfill 50,5,0
	gat 0,5 : gfill 5,45,0
	gat 5,45 : gfill 45,5,0
	gat 45,5 : gfill 5,45,0	
	gsavebit(kpath$+"black.bmp")
	gclose id%
	
	id%=gcreatebit(70,70,0)
	gcolor 0,0,0
	glineby 70,70
	gat 70,0 : glineby -70,70
	gsavebit(kpath$+"crossx.bmp")
	gclose id%

	id%=gcreatebit(70,70,0)
	gcolor 0,0,0
	gat 0,35 : glineby 70,0
	gat 35,0 : glineby 0,70
	gsavebit(kpath$+"cross+.bmp")
	gclose id%
	
	id%=gcreatebit(100,100)
	gcolor 0,0,0 : gfill 100,100,0
	gsavebit(kpath$+"plain.bmp")
	gclose id%

	id%=gcreatebit(100,100)
	stripes:
	gsavebit(kpath$+"stripesdef.bmp")
	gclose id%

	rem b/w
	id%=gcreatebit(100,100,0)
	stripes:
	gsavebit(kpath$+"stripes0.bmp")
	gclose id%
	
	rem 4 col
	id%=gcreatebit(100,100,1)
	stripes:
	gsavebit(kpath$+"stripes1.bmp")
	gclose id%

	rem 16 col
	id%=gcreatebit(100,100,2)
	stripes:
	gsavebit(kpath$+"stripes2.bmp")
	gclose id%
	
	id%=gcreatebit(50,50,1)
	gcolor 0,0,0
	gfill 12,50,0
	gcolor $50,$50,$50
	gat 12,0 : gfill 12,50,0
	gcolor $a0,$a0,$a0
	gat 24,0 : gfill 12,50,0
	gsavebit(kpath$+"stripes.bmp")
	gclose id%
	
	print "Created bitmaps OK"
	rem pause 20
endp

proc stripes:
	local col%,x%,i%
	col%=0
	x%=0
	while i%<16	
		gcolor col%,col%,col%
		gat x%,0
		gfill 6,100,0
		col%=col%+$10
		x%=x%+6
		i%=i%+1
	endwh
endp

proc createsprite1&:(wid%,time&,x%,y%,flags&)
	local idb&,idg1&,idg2&,sprid&
	
	idb&=bitmapload&:(kpath$+"black.bmp",0)
	idg2&=bitmapload&:(kpath$+"grey2.bmp",0)
	idg1&=bitmapload&:(kpath$+"grey1.bmp",0)
	
	sprid&=spritecreate&:(wid%,x%,y%,flags&)	
	spriteappend:(time&,idb&,idb&,1,0,0)
	spriteappend:(time&,idg2&,idg2&,1,0,0)		
	spriteappend:(time&,idg1&,idg1&,1,0,0)		
	spriteappend:(time&,idg2&,idg2&,1,0,0)		
	
	bitmapunload:(idb&)
	bitmapunload:(idg1&)
	bitmapunload:(idg2&)	
	
	return sprid&
endp

proc createsprite2&:(wid%,time&,x%,y%,flags&)
	local id1&,id2&,sprid&
	
	id1&=bitmapload&:(kpath$+"crossx.bmp",0)
	id2&=bitmapload&:(kpath$+"cross+.bmp",0)
	
	sprid&=spritecreate&:(wid%,x%,y%,flags&)	
	spriteappend:(time&,id1&,id1&,1,0,0)
	spriteappend:(time&,id2&,id2&,1,0,0)		
	bitmapunload:(id1&)
	bitmapunload:(id2&)
	
	return sprid&
endp

proc texamples:
	local id%
	
	screen 71,3,1,1
	print "Some examples of sprites created with different parameters"
	print 
	print "Sprite with non-flashing frames"
	sp1&=createsprite1&:(1,&7a120,20,40,&0)
	spritedraw:
	gupdate
	at 35,3 : print "Sprite with flashing frames"
	sp2&=createsprite1&:(1,&7a120,340,40,&1)
	spritedraw:
	gupdate
	rem pause 40
	spritedelete:(sp1&)
	spritedelete:(sp2&)
	cls
	
	print "Sprites in three different color modes in default window"
	drawcolsprites:(1)
	
	print "Sprites in three different color modes in 16 colour window"
	id%=gcreate(0,40,640,200,1,2)
	drawcolsprites:(id%)
	gclose id%
	
	print "Sprites in three different color modes in 2 colour window"
	id%=gcreate(0,40,640,200,1,0)
	drawcolsprites:(id%)
	gclose id%

endp

proc drawcolsprites:(wid%)
	local id1&,id2&,sp3&,sp4&
	
	gfont 10
	id1&=bitmapload&:(kpath$+"plain.bmp",0)
	
	id2&=bitmapload&:(kpath$+"stripesdef.bmp",0)
	sp1&=spritecreate&:(wid%,20,40,0)
	spriteappend:(1000000,id1&,id1&,1,0,0)
	spriteappend:(1000000,id2&,id2&,1,0,0)
	spritedraw:
	gupdate
	gat 20,160 : gprint "default"
	bitmapunload:(id2&)
	
	id2&=bitmapload&:(kpath$+"stripes0.bmp",0)
	sp2&=spritecreate&:(wid%,140,40,0)
	spriteappend:(1000000,id1&,id1&,1,0,0)
	spriteappend:(1000000,id2&,id2&,1,0,0)
	spritedraw:
	gupdate
	gat 140,160 : gprint "2 colour"
	bitmapunload:(id2&)
	
	id2&=bitmapload&:(kpath$+"stripes1.bmp",0)
	sp3&=spritecreate&:(wid%,260,40,0)
	spriteappend:(1000000,id1&,id1&,1,0,0)
	spriteappend:(1000000,id2&,id2&,1,0,0)
	spritedraw:
	gupdate
	gat 260,160 : gprint "4 colour"
	bitmapunload:(id2&)
	
	id2&=bitmapload&:(kpath$+"stripes2.bmp",0)
	sp4&=spritecreate&:(wid%,380,40,0)
	spriteappend:(1000000,id1&,id1&,1,0,0)
	spriteappend:(1000000,id2&,id2&,1,0,0)
	spritedraw:
	gupdate
	gat 380,160 : gprint "16 colour"
	bitmapunload:(id2&)
	bitmapunload:(id1&)
	
	pause%:(10)
	spritedelete:(sp1&)
	spritedelete:(sp2&)
	spritedelete:(sp3&)	
	spritedelete:(sp4&)	
	gcls
endp

const KS5ScreenCharHeight%=21
const KCrystalScreenCharHeight%=18

proc tappend:
	local sp&,id&(200),i%,n%,dx%,dy%,j%
	local id&,id1&,id2&,id3&,time&
	
	print "Test SPRITEAPPEND:"
	rem pause 20
	cls
	rem draw in limits
	gat 5,5  : glineby 0,25
	gat 505,5 : glineby 0,25
	gat 155,30 : glineby 0,10

	drv$=hDiskname$:+"\Opltest\Data\lots.mbm"
	 
	rem use PC generated bitmaps
	sp&=spritecreate&:(1,5,5,0)
	busy "Creating..."
	while i%<200
		n%=i%
		i%=i%+1
		id&(i%)=bitmapload&:(drv$,n%)
		if i%<=100 : dy%=5 : else dy%=10 : endif	
		if i%<=100 : dx%=dx%+5 : else dx%=dx%-5 : endif
		spriteappend:(500,id&(i%),id&(i%),1,dx%,dy%)
		bitmapunload:(id&(i%))
	endwh	
	busy off
	spritedraw:
	pause%:(10)
	spritepos:(0,15)
	pause%:(10)
	spritepos:(150,25)
	pause%:(10)
	spritedelete:(sp&)
	gcls
	
	rem now try changing parameters
	time&=2000000
	id1&=bitmapload&:(kpath$+"plain.bmp",0)
	id2&=bitmapload&:(kpath$+"stripes2.bmp",0)	
	screen 36,KCrystalScreenCharHeight%,35,1

	print "time&=";time&
	print "spriteappend:(time&,id1&,id1&,0,0,0)"
	print "spriteappend:(time&,id1&,id2&,0,0,0)"		
	print "spriteappend:(time&,id2&,id2&,0,0,0)"		
	print "spriteappend:(time&,id2&,id1&,0,0,0)"		
	print "spriteappend:(time&,id1&,id1&,1,0,0)"
	print "spriteappend:(time&,id1&,id2&,1,0,0)"		
	print "spriteappend:(time&,id2&,id2&,1,0,0)"		
	print "spriteappend:(time&,id2&,id1&,1,0,0)"
	
	sp1&=spritecreate&:(1,20,40,0)	
	spriteappend:(time&,id1&,id1&,0,0,0)
	spriteappend:(time&,id1&,id2&,0,0,0)		
	spriteappend:(time&,id2&,id2&,0,0,0)		
	spriteappend:(time&,id2&,id1&,0,0,0)		
	spriteappend:(time&,id1&,id1&,1,0,0)
	spriteappend:(time&,id1&,id2&,1,0,0)		
	spriteappend:(time&,id2&,id2&,1,0,0)		
	spriteappend:(time&,id2&,id1&,1,0,0)			
	bitmapunload:(id1&)
	bitmapunload:(id2&)	
	spritedraw:
	pause%:(10)
	spritedelete:(sp1&)
	cls
	
	time&=500000
	id1&=bitmapload&:(kpath$+"black.bmp",0)
	id2&=bitmapload&:(kpath$+"grey2.bmp",0)
	id3&=bitmapload&:(kpath$+"grey1.bmp",0)
	sp1&=spritecreate&:(1,20,40,0)	
	spriteappend:(time&,id1&,id1&,1,0,0)
	spriteappend:(time&,id2&,id2&,1,10,10)		
	spriteappend:(time&,id3&,id3&,1,20,20)		
	spriteappend:(time&,id2&,id2&,1,10,10)			
	bitmapunload:(id1&)
	bitmapunload:(id2&)
	bitmapunload:(id3&)	
	spritedraw:	
	pause%:(10)
	spritedelete:(sp1&)

	rem try to append when no current sprite
	id&=bitmapload&:(kpath$+"black.bmp",0)
	onerr e1
	spriteappend:(500000,id&,id&,1,0,0)
	onerr off
	raise 1
e1::
	onerr off
	if err<>-33 : raise 2 : endif
	bitmapunload:(id&)

	rem try appending bitmap and mask different sizes
	sp1&=spritecreate&:(1,20,40,0)
	id1&=bitmapload&:(kpath$+"black.bmp",0)
	id2&=bitmapload&:(kpath$+"stripes1.bmp",0)
	onerr e2
	spriteappend:(500000,id1&,id2&,1,0,0)
	onerr off
	raise 3
e2::
	onerr off
	if err<>-2 : raise 4 : endif
	bitmapunload:(id1&)
	bitmapunload:(id2&)
	spritedelete:(sp1&)
	
	rem try appending with unloaded bitmap
	sp&=spritecreate&:(1,20,40,0)
	id1&=2 : id2&=3
	onerr e3
	spriteappend:(500000,id1&,id2&,1,0,0)
	onerr off
	raise 5
e3::
	onerr off
	if err<>-33 : raise 6 : endif
	
	print "OK"
	rem pause 20
	gcls : cls
endp

proc tchange:
	local id&,idm&,idmm&
	 
	print "Test SPRITECHANGE:"
	
	print "Test changing bitmaps and position"
	sp1&=createsprite1&:(1,&7a120,20,20,&0)
	spritedraw:
	pause%:(10)
	id&=bitmapload&:(kpath$+"grey1.bmp",0)
	spritechange:(0,&7a120,id&,id&,1,10,10)
	bitmapunload:(id&)
	pause%:(10)
	id&=bitmapload&:(kpath$+"grey2.bmp",0)
	spritechange:(1,&7a120,id&,id&,1,10,10)
	pause%:(10)
	spritechange:(3,&7a120,id&,id&,1,10,10)
	bitmapunload:(id&)
	pause%:(10)
	id&=bitmapload&:(kpath$+"black.bmp",0)
	spritechange:(2,&7a120,id&,id&,1,10,10)
	pause%:(10)
	
	print "Test changing mask invert mode"
	spritechange:(2,&7a120,id&,id&,0,10,10)
	pause%:(10)
	idm&=bitmapload&:(kpath$+"stripes.bmp",0)
	spritechange:(2,&7a120,id&,idm&,1,10,10)
	pause%:(10)
	spritechange:(2,&7a120,id&,id&,1,10,10)
	bitmapunload:(idm&)	
	pause%:(10)
	
	print "Test changing time"
	spritechange:(2,1000000,id&,id&,1,10,10)
	pause%:(10)
	spritechange:(2,10000000,id&,id&,1,10,10)
	pause%:(10)
	spritechange:(2,500000,id&,id&,1,10,10)
	pause%:(10)
		
	rem try to change to 2 different sized bitmaps
	idm&=bitmapload&:(kpath$+"stripes1.bmp",0)
	onerr e1
	spritechange:(2,&7a120,id&,idm&,1,10,10)
	onerr off
	raise 1
e1::
	onerr off
	if err<>-2 : raise 2 : endif
	bitmapunload:(id&)
	bitmapunload:(idm&)
		
	rem try changing member which does not exist
	id&=bitmapload&:(kpath$+"grey1.bmp",0)
	onerr e2
	spritechange:(5,&7a120,id&,id&,1,10,10)
	onerr off
	raise 3
e2::
	onerr off
	if err<>-2 : raise 4 : endif
	bitmapunload:(id&)

	rem try to change to unloaded bitmap
	id&=2
	onerr e4
	spritechange:(2,&7a120,id&,id&,1,10,10)
	onerr off
	raise 7
e4::
	onerr off
	if err<>-33 : raise 8 : endif
	
	spritedelete:(sp1&)

	rem try to change a sprite member when none is created
	id&=bitmapload&:(kpath$+"grey1.bmp",0)
	onerr e3
	spritechange:(1,&7a120,id&,id&,1,10,10)
	onerr off
	raise 5
e3::
	onerr off
	if err<>-33 : raise 6 : endif
	bitmapunload:(id&)

	print
	print "OK"
	rem pause 20
	gcls : cls
endp

proc tdraw:
	
	print "Test SPRITEDRAW:"
	print

	rem try to draw without current sprite
	onerr e1
	spritedraw:
	onerr off
	raise 1
e1::
	onerr off
	if err<>-33 : raise 2 : endif

	rem try to draw twice - second one should do nothing
	sp1&=createsprite1&:(1,&7a120,20,2,&1)
	spritedraw:
	spritedraw:
	pause%:(10)
	spritedelete:(sp1&)
	
	rem try to draw when none created
	onerr e2
	spritedraw:
	onerr off
	raise 1
e2::
	onerr off
	if err<>-33 : raise 2 : endif

	print "OK"
	rem pause 20
	gcls : cls
endp

proc tpos:
	
	print "Test SPRITEPOS:"
	
	sp1&=createsprite1&:(1,&7a102,20,20,&0)
	spritedraw:
	pause%:(10)
	rem try to position off screen
	spritepos:(1000,1000)
	pause%:(10)
	spritepos:(20,20)
	pause%:(10)
	spritepos:(-20,-20)
	pause%:(10)
	spritepos:(20,20)
	pause%:(10)
	spritedelete:(sp1&)
	
	rem try to position when no current sprite
	onerr e2
	spritepos:(20,20)
	onerr off
	raise 3
e2::
	onerr off
	if err<>-33 : raise 4 : endif

	print "OK"
	rem pause 20
	gcls : cls
endp

proc tdelete:

	print "Test SPRITEDELETE:"
	
	rem delete a sprite which doesn't exist
	sp1&=2
	onerr e1
	spritedelete:(sp1&)
	onerr off
	raise 1
e1::
	onerr off
	if err<>-33 : raise 2 : endif
	
	print "OK"
	rem pause 20
	gcls : cls		
endp

const KCrystalScreenHeight%=16
const KS5ScreenHeight%=19

proc tuse:
	
	screen 71,2,1,KCrystalScreenHeight%
	print "Test SPRITEUSE:"
	sp1&=createsprite1&:(1,&7a120,20,20,&0)
	sp2&=createsprite2&:(1,&7a120,300,20,&1)
	
	print "Drawing sprite 1"
	spriteuse:(sp1&)
	spritedraw:
	pause%:(10)
	print "Drawing sprite 2"
	spriteuse:(sp2&)
	spritedraw:
	pause%:(10)

	print "Moving sprite 1"
	spriteuse:(sp1&)
	spritepos:(70,40)
	pause%:(10)
	print "Moving sprite 2"
	spriteuse:(sp2&)
	spritepos:(300,90)
	pause%:(10)
	
	print "Deleting sprite 1 and moving sprite 2"
	spriteuse:(sp2&)
	spritedelete:(sp1&)
	spritepos:(300,20)
	pause%:(10)
	print "Try to use sprite 1"
	onerr e1
	spriteuse:(sp1&)
	onerr off
	raise 1
e1::
	onerr off
	if err<>-33 : raise 2 : endif
	
	print "Deleting sprite 2"
	spritedelete:(sp2&)
	pause%:(10)
	
	rem trying to use a sprite when none exists
	sp1&=2
	onerr e2
	spriteuse:(sp1&)
	onerr off
	raise 3
e2::
	onerr off
	if err<>-33 : raise 4 : endif
	
	print "OK"
	rem pause 20 
	screen 71,KCrystalScreenHeight%
	gcls : cls
endp

const KS5ScreenWidth%=91
const KCrystalScreenWidth%=80

proc dispsprite:
	
	screen KCrystalScreenWidth%,KCrystalScreenHeight%
	print "A simple test with one sprite"
	rem pause 20
	cls
	gcls
	rem background for sprite 
	gcolor 0,0,0
	gat 320,0 : gfill 320,120,0
	gcolor $50,$50,$50
	gat 0,120 : gfill 320,120,0
	gcolor 0,0,0
	gat 320,120 : glineto 640,240
	
	sp1&=createsprite1&:(1,&186a0,135,35,&0)
	spritedraw:
	gupdate
	rem pause 40
	spritepos:(455,35)
	gupdate
	rem pause 40
	spritepos:(455,155)
	gupdate
	rem pause 40
	spritepos:(135,155)
	gupdate
	rem pause 40
	spritedelete:(sp1&)
	gcls
	cls
endp

proc twosprites:
	local wid%,ret%,sp3&
	local t1&,t2&,t3&,col&,col%,f1&,f2&,f3&
	
	t1&=1000000
	t2&=1000000
	t3&=1000000
	col&=255
	
	wid%=gcreate(0,0,gwidth,gheight,1,2)

if 0	
	dinit "Enter options for sprites"
	dLong t1&,"Time interval for sprite 1 in microsecs",&0,&7fffffff
	dLong t2&,"Time interval for sprite 2 in microsecs",&0,&7fffffff
	dLong t3&,"Time interval for sprite 3 in microsecs",&0,&7fffffff
	dLong f1&,"Flags for sprite 1",&0,&1
	dLong f2&,"Flags for sprite 2",&0,&1
	dLong f3&,"Flags for sprite 3",&0,&1
	dLong col&,"Colour for background",&0,&100		
	ret%=dialog
	rem if ret%=0 : break : endif
endif

		col%=col&
		gat 0,0 : gcolor col%,col%,col%
		gfill gwidth,gheight,0
	
		sp1&=createsprite1&:(wid%,t1&,20,20,f1&)
		spritedraw:
		sp2&=createsprite1&:(wid%,t2&,250,20,f2&)
		spritedraw:
		sp3&=createsprite2&:(wid%,t3&,480,20,f3&)
		spritedraw:
		pause%:(10)
		spritedelete:(sp1&)
		spritedelete:(sp2&)
		spritedelete:(sp3&)
rem	until 0
	
	gclose wid%
endp

proc lots:
	local sprid&(100),i%,x%,y%,n%
	
	n%=20
	x%=10
	y%=10
	while i%<n%
		i%=i%+1
		if x%>=590
			x%=10
			y%=y%+60
		endif
		sprid&(i%)=createsprite1&:(1,int(200000.0),x%,y%,&0)	
		spritedraw:
		x%=x%+60
	endwh	
	pause%:(10)

	i%=0
	while i%<n%
		i%=i%+1
		spritedelete:(sprid&(i%))
	endwh	
endp

proc delbitmaps:
	
	trap delete kpath$+"black.bmp"
	trap delete kpath$+"grey1.bmp"
	trap delete kpath$+"grey2.bmp"	
	trap delete kpath$+"crossx.bmp"
	trap delete kpath$+"cross+.bmp"
	trap delete kpath$+"plain.bmp"
	trap delete kpath$+"stripesdef.bmp"
	trap delete kpath$+"stripes0.bmp"
	trap delete kpath$+"stripes1.bmp"
	trap delete kpath$+"stripes2.bmp"
endp


PROC pause%:(duration%)
	pause duration%
ENDP


REM End of pOpxSprite.tpl
