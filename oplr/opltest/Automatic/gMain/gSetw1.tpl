REM gSetW1.tpl
REM EPOC OPL automatic test code for gSetwin/screen.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gSetw1", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gSetW1:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogsetw1")
rem	hCleanUp%:("CleanUp")
endp


PROC dogSetw1:
	GLOBAL chrW%,chrH%
	local id%, originalWidth%, originalHeight%
	local info%(10)
	gUSE 1
	originalWidth%=gWIDTH :originalHeight%=gHEIGHT
	screeninfo info%()
	chrW%=info%(7) :chrH%=info%(8)

	id%=gCREATE(10,10,600,200,1,1)

	try:(0,0, 500,180)
	try:(10,10, 300,100)
	try:(20,20, 300,100)
	try:(100,50, 200,150)
	try:(10,10, 600,200)

	REM Use default window.
	gUSE 1
	gORDER 1,1
	try:(50,20, 400,150)
	try:(10,10, 500,180)
	try:(0,0, originalWidth%,originalHeight%)

	REM End of test.
	gCLOSE id%
	gUSE 1
	gCLS
ENDP


PROC try:(ax%,ay%,awidth%,aheight%)
	LOCAL i%
	LOCAL cw%,ch%, cx%,cy%
	gSETWIN ax%,ay%,awidth%,aheight%
	gCLS
	gXBORDER 2,$202
	gAT 20,20
	gPRINT "width="+GEN$(awidth%,4)+" height="+GEN$(aheight%,4)

	IF gIDENTITY=1 REM Default window
		cw%=awidth%/chrW%
		ch%=aheight%/chrH%
		cx%=1 :cy%=1
		SCREEN cw%,ch%,cx%,cy%

		i%=100
		WHILE i%
			PRINT DATIM$,
			i%=i%-1
		ENDWH
	ENDIF
ENDP




proc olddogsetw1:
	local a$(100),w%,h%,g$(1),factor%,str$(30)
	local chrW%,chrH%  rem character width and height in console
	local buf%(4),info%(10),marginX%,marginY%
	
	screeninfo info%()
	marginX%=info%(1)
	marginY%=info%(2)
	chrW%=info%(7)
	chrH%=info%(8)
	str$="Up,Down,Left,Right,Quit"
	w%=gWidth :h%=gHeight
	gSetWin gWidth/4+marginX%,gHeight/4+marginY%,len(str$)*chrW%+2*marginX%,chrH%+2*marginY%
	screen len(str$),1,1,1
	print "Console xx";
	gCreate(0,0,w%,h%,1)
	gPatt -1,gWidth,gHeight,0
	gAt 2,15 :gTMode 0: gPrint "Graphics window #2"
	gOrder 1,0
	print
	print "Console>";
	edit a$
	gOrder 1,9
	cls
	a$=""
	print "Hidden>"; :edit a$
	gOrder 1,0
	gUse 1
	do
		cls
		print str$;
		g$=upper$(get$)
		if kmod=2
			factor%=10
		else
			factor%=1
		endif
		if g$="U"
			gSetWin gOriginX,gOriginY-factor%
		elseif g$="D"
			gSetWin gOriginX,gOriginY+factor%
		elseif g$="L"
			gSetWin gOriginX-factor%,gOriginY
		elseif g$="R"
			gSetWin gOriginX+factor%,gOriginY
		endif
	until g$="Q"
endp


REM End of gSetw1.tpl
