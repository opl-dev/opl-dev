REM gScreen.tpl
REM EPOC OPL automatic test code for screen.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gscreen", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gScreen:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogscreen")
rem	hCleanUp%:("CleanUp")
endp


PROC dogScreen:
	local info%(10),originalWidth%,originalHeight%
	screeninfo info%()

	originalWidth%=info%(KSInfoAScrW%)
	originalHeight%=info%(KSInfoAScrH%)
	
	try:(20,10,30,3)
	try:(10,10,1,1)
	try:(80,18,0,0)
	
	REM Restore.
	try:(originalWidth%,originalHeight%,0,0)
	cls
endp


proc try:(aW%,aH%,aX%,aY%)
	local i%
	print aw%,ah%,ax%,ay%
	screen aw%,ah%,ax%,ay%
	do
		print datim$,
		i%=i%+1
	until i%=100
ENDP



proc originalgScreen:
	global chrW%,chrH%  rem character width and height
	global maxCol%,maxRow%,margX%,margY%
	global font&
	global info%(10)
	local err%
	
	trap loadm "g3bplus"
	err%=err
	if err%=0 or err%=-104
		scrInfoX:(addr(info%()))
		chrW%=info%(7) :chrH%=info%(8)
		font&=int(info%(9))+int(info%(10))*(&2**&10)
		busyXX:("")
	endif

	type:
	gBars:
	
	trap unloadm "g3bplus"
endp


proc gBars:
	margX%=info%(1)
	margY%=info%(2)
	maxCol%=(gWidth-2*margX%)/chrW%
	maxRow%=(gHeight-2*margY%)/chrH%
	gCls

	doScr:(10,1)
	doScr:(10,2)
	doScr:(maxCol%-2,maxRow%-2)
	doScr:(maxCol%-2,maxRow%-1)
	doScr:(maxCol%-2,maxRow%)
	doScr:(maxCol%-1,maxRow%-2)
	doScr:(maxCol%-1,maxRow%-1)
	doScr:(maxCol%-1,maxRow%)
	doScr:(maxCol%,maxRow%-2)
	doScr:(maxCol%,maxRow%-1)
	doScr:(maxCol%,maxRow%)
endp


proc doScr:(w%,h%)
	local x%,y%,info$(255),i%

	cls
	if maxCol% and $1		rem odd
		x%=1+maxCol%/2-w%/2
	else							rem even
		x%=1+maxCol%/2-(w%+1)/2
	endif
	y%=1+maxRow%/2-h%/2
	
	info$=num$(w%,3)+","+num$(h%,3)+","+num$(x%,3)+","+num$(y%,3)
	screen w%,h%,x%,y%
	print info$;
	i%=0
	while i%<h%-1
		print rept$("x",w%);
		i%=i%+1
	endwh
	print rept$("x",w%-len(info$));
	gBorder 0
	get
	
	cls
	screen w%,h%
	info$=num$(w%,3)+","+num$(h%,3)
	print info$;
	i%=0
	while i%<h%-1
		print rept$("x",w%);
		i%=i%+1
	endwh
	print rept$("x",w%-len(info$));
	gBorder 0	
	get
endp

proc type:
	local g%,w%,h%,x%,y%
	local margX%,margY%

	margX%=info%(1)
	margY%=info%(2)
	w%=20 :h%=3 :x%=2 :y%=2
	cursor on
	gFont font&
	gStyle 16 :rem mono
	gTMode 3
	cls
	gAt chrW%-2+margX%,chrH%-1+margY% :gBox w%*chrW%+3,h%*chrH%+2
	screen w%,h%,x%,y%
	print rept$(rept$("x",w%),h%)
	print "screen ";w%;",";h%;",";x%;",";y%
	get
	cls
	at 1,1
	gAt chrW%,60
	do
		g%=get
		print chr$(g%);		
		if g%=13
			gAt chrW%,50
		elseif g%=8
			if gX>chrW%
				gMove -chrW%,0
			endif
		else
			gPrint chr$(g%);
			if gX>=w%*chrW%+5 :gAt chrW%,50 :endif
		endif
	until g%=27
endp


PROC Reset:
	gSetwin 0,0,lcdW%,lcdH%
	screen scrnW%,scrnH%,1,1
	gFont 12
	gStyle 0
	gGMode 0
	gTMode 0
	cls
	gCls
	gUpdate on
	gUpdate
ENDP

REM End of gScreen.tpl
