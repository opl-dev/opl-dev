REM pButtBit.tpl
REM EPOC OPL automatic test code for gBUTTON.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "Bmp.oxh"

DECLARE External
EXTERNAL BmpOpx:


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pButtBit", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pButtBit:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tButtonBitmap")
	hRunTest%:("tBmpOpx")
	hCleanUp%:("CleanUp")
endp

CONST KPath$="c:\Opl1993\"

PROC CleanUp:
	TRAP DELETE kPath$+"*.*"
	TRAP RMDIR kPath$
ENDP


proc tButtonBitmap:
	local wId%,bId%,bIdMask%

	trap mkdir kPath$
	
rem	gupdate off
rem	print "gUPDATE OFF"
	wId%=gcreate(0,0,50,20,0)
	gat 10,10
	gcircle 10
	gBorder 0	
	gSaveBit kPath$+"butbit."

	gcls
	gFill 50,8,2
	gat 0,12
	gfill 50,8,2
	gSaveBit kPath$+"butmask."
	gclose gidentity
	
	bId%=gCreatebit(25,20)
	gBox gwidth,gheight
	gat 2,15
	gprint "Bit"

	bIdMask%=gCreatebit(25,20)
rem	gPatt -1,gwidth,gheight,3
	gFill gWidth,gheight/2-1,2
	gat 0,gheight/2
	gFill gwidth,gheight/2,2
	
	guse 1
rem	print "Keypress allows you to see the button speed on its own"
rem	print "Press a key to continue..."
rem	get

	gat 2,40
	gbutton "No bitmaps",2,100,30,0
	gmove 0,31
	gbutton "OplBit",2,100,30,0,bId%,bIdMask%,3
	gCLOSE bId%
	gCLOSE bIdMask%
endp


PROC tBmpOpx:
	local bit&,mask&
	bit&=bitmapload&:(kpath$+"butbit.",0)
	mask&=bitmapload&:(kpath$+"butmask.",0)

	gmove 0,31
	gbutton "",2,100,30,0,bit&,0,0
	gmove 0,31

	gbutton "No mask",2,200,30,17,bit&

	gat gWidth-202,10
	gbutton "Mask=NULL",2,200,30,0,bit&,mask&
	gmove 0, 31
	gbutton "Layout=0",2,200,30,0,bit&,mask&,0	
	gmove 0, 31
	gbutton "Layout = 1",2,200,45,0,bit&,mask&,1
	gmove 0, 46
	gbutton "Layout   =  2",2,200,45,0,bit&,mask&,2
	gmove 0, 46
	gbutton "Layout    =     3",2,200,45,0,bit&,mask&,3

	gat gWidth-500,10
	gbutton "Excess=$00",2,250,30,0,bit&,mask&,$00
	gmove 0, 31
	gbutton "Excess = $00 again",2,250,30,0,bit&,mask&,$00
	gmove 0, 31
	gbutton "Excess=$10",2,250,30,0,bit&,mask&,$10
	gmove 0, 31
	gbutton "Excess = $10 again",2,250,30,0,bit&,mask&,$10
	gmove 0, 31
	gbutton "Excess=$20",2,250,30,0,bit&,mask&,$20
	gmove 0, 31
	gbutton "Excess = $20 again",2,250,30,0,bit&,mask&,$20
	gmove 0, 31
	gbutton "Excess=$22",2,250,43,0,bit&,mask&,$22

	bitmapunload:(bit&)
	bitmapunload:(mask&)
ENDP


REM End of pButtbit.tpl
