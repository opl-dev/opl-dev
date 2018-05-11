REM gCurpos.tpl
REM EPOC OPL automatic test code for cursor positioning.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gCurpos", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gCurpos:
	global idW2%,gap&,gap%,pause%
	global chrW%,chrH%  rem character width and height

	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)

	hRunTest%:("prep")
	hRunTest%:("create")
	hRunTest%:("createB")
	hRunTest%:("loadB")
	hRunTest%:("close")
	hRunTest%:("use")
	hRunTest%:("setWin")
	hRunTest%:("visible")
	hRunTest%:("loadFont")
	hRunTest%:("font")
	hRunTest%:("gmode")
	hRunTest%:("tmode")
	hRunTest%:("style")
	hRunTest%:("order")
	hRunTest%:("cls")
	hRunTest%:("at")
	hRunTest%:("move")
	hRunTest%:("twidth")
	hRunTest%:("print")
	hRunTest%:("printCl")
	hRunTest%:("printB")
	hRunTest%:("lineBy")
	hRunTest%:("lineTo")
	hRunTest%:("box")
	hRunTest%:("poly")
	hRunTest%:("fill")
	hRunTest%:("patt")
	hRunTest%:("copy")
	hRunTest%:("scroll")

	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	gClose idW2% :gcls
ENDP	


PROC Prep:
	local info%(10)
	screeninfo info%()
	chrW%=info%(7) :chrH%=info%(8)
	cls
	screen gWidth/chrW%,3,1,1
	pause%=-30
	gap%=(gWidth+15)/160*10
	cls
	gAT gap%*3/2+20,38 :gLineBy -21,0 :gLineBy 0,4 :gLineBy -2,-2 :gMove 2,3 :gLineBy 3,-3 :gMove 20,4
	gFont 10 :gPrint "Initial position"
	idW2%=gCreate(0,45,gWidth,gHeight-40,1)
ENDP


proc setup:(tstName$)
	local x%,y%
	cls
	print upper$(tstName$)
	trap gUse 2
	IF err : raise 1000 :endif
	gOrder 2,1
	gCls
	cursor off

	while x%<gHeight
		gLineBy gWidth,0
		gMove -gWidth,gap%
		x%=x%+gap%
	endwh
	gAt 0,0
	while y%<gWidth
		gLineBy 0,gHeight
		gMove gap%,-gHeight
		y%=y%+gap%
	endwh
	gAt gap%,gap%
	cursor 2,gap%,gap%+1,gap%+1
endp


proc create:
	local id1%,info&(60)
	gUse 1
	gAt 1,2
	setup:("gCreate cursor position")
	id1%=gCreate(gap%*4+2,42+gap%,gap%*10,gap%*3,1)
	gInfo32 info&()
	gap&=int(gap%)
	if info&(26)<>gap& :print info&(26),gap& : raise 1 :endif
	if info&(27)<>gap& :raise 2 :endif
	rem print "gCreate: no move"
	rem pause pause%	
	cursor id1%
	rem at 1,2 :print "Cursor",id1%,"at 0,0"
	rem at 1,3 :print "window",id1%,"based at 0,70"
	if gX<>0 :raise 3 :endif
	if gY<>0 :raise 4 :endif
	rem pause pause%	
	gClose id1% :rem goes to window 1 (closed current window)
	if gX<>1 :raise 5 :endif
	if gY<>2 :raise 6 :endif
	gUse idW2% :gOrder idW2%,1
	if gX<>gap% :raise 7 :endif
	if gY<>gap% :raise 8 :endif
	rem at 1,2 :print "window",id1%,"closed "
	rem at 1,3 :print "cursor should be off       "
	gInfo32 info&()
	if info&(21) :raise 9 :endif	:rem cursor state
endp


proc createB:
	local id%
	setup:("gCreateB cursor position")
	id%=gCreateBit(100,100)
	rem print "gCreateBit: no move"
	
	gClose id%
	gUse idW2%
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gClose: no move"	
endp


proc loadB:
	local id%,id2%
	setup:("gSave/loadBit cursor position")
	id%=gCreateBit(10,10)
	gSaveBit "gCurPos.pic"
	id2%=gLoadBit("gCurPos.pic")
	gUse idW2%
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gSave/loadBit: no move"	
	delete "gCurPos.pic"
	gclose id2%
	gclose id%
endp


proc close:
	local id%
	setup:("gClose cursor position")
	id%=gCreate(0,65,180,26,1)
	gClose id%
	gUse idW2%
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gClose: no move"	
endp


proc use:
	local info&(50)
	setup:("gUse cursor position")
	gUse 1
	gInfo32 info&()
	if info&(21)=0 :raise 2 :endif			:rem cursor state
	if info&(22)<>idW2% :raise 3 :endif	:rem cursor window
	if info&(26)<>gap& :raise 4 :endif	:rem x pos of cursor
	if info&(27)<>gap& :raise 5 :endif	:rem y pos of cursor
	rem print "gUse 1: no move"
	gUse idW2%
	if gX<>gap% :raise 6 :endif
	if gY<>gap% :raise 7 :endif
	rem print "gUse",idW2%,": no move"
endp


proc setWin:
	local x%,y%,w%,h%
	setup:("gSetWin cursor position")
	x%=gOriginX
	y%=gOriginY
	w%=gWidth
	h%=gHeight
	gSetWin x%+10,y%+10,w%-10,h%-10
	rem print "gSetWin",x%+10;",";y%+10;",";w%-10;",";h%-10
	rem pause pause%
	gUse idW2%
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "No move"
	gSetWin x%,y%,w%,h%
endp


proc visible:
	setup:("gVisible cursor position")
	gVisible off
	rem print "gVisible off"
	rem pause pause%
	gVisible on
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gVisible on: no move"
endp


proc loadFont:
	local id%
	setup:("gLoadFont cursor position")
	id%=gLoadFont(hDiskName$:+"\Opltest\Data\testfont.gdr")
	rem print "gLoadFont: no move"
	gUnloadFont	id%
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gUnloadFont: no move"
endp


proc font:
	setup:("gFont cursor position")
	gFont 5
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gFont 5: no move"
	gFont 9
	if gX<>gap% :raise 3 :endif
	if gY<>gap% :raise 4 :endif
	rem print "gFont 9: no move"
endp


proc gmode:
	setup:("gGmode cursor position")
	gGMode 2
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gGMode 2: no move"
	gGMode 0
	if gX<>gap% :raise 3 :endif
	if gY<>gap% :raise 4 :endif
	rem print "gGMode 0: no move"
endp


proc tmode:
	setup:("gTmode cursor position")
	gTMode 1
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gTMode 1: no move"
	gTMode 3
	if gX<>gap% :raise 3 :endif
	if gY<>gap% :raise 4 :endif
	rem print "gTMode 3: no move"
endp


proc style:
	setup:("gStyle cursor position")
	gStyle 7
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gStyle 7: no move"
	gStyle 0
	if gX<>gap% :raise 3 :endif
	if gY<>gap% :raise 4 :endif
	rem print "gStyle 0: no move"
endp


proc order:
	setup:("gOrder cursor position")
	gOrder 1,1
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gOrder 1,1 :no move"
	gOrder idW2%,1
	if gX<>gap% :raise 3 :endif
	if gY<>gap% :raise 4 :endif
	rem print "gOrder 2,1 :no move"
endp


proc cls:
	setup:("gCls cursor position")
	gCls
	if gX<>0 :raise 1 :endif
	if gY<>0 :raise 2 :endif
	rem print "gCls: cursor 1 at 0,0"
endp


proc at:
	local x%,y%
	setup:("gAt cursor position")
	while x%<=gWidth
		y%=0
		while y%<=gHeight
			gAt x%,y%
			y%=y%+gap%
		endwh
		x%=x%+gap%
	endwh
	x%=3*gap% :y%=2*gap%
	gAt x%,y%
	if gX<>x% :raise 1 :endif
	if gY<>y% :raise 2 :endif
	rem print "gAt ";x%;",";y%	
endp


proc move:
	local x%,y%,dx%,dy%
	setup:("gMove cursor position")
	gMove -gap%,-gap%
	if gX<>0 :raise 1 :endif
	if gY<>0 :raise 2 :endif
	rem print "gMove ";gap%;",";gap%;" to";gX;",";gY	
	dx%=gap%*3 :dy%=gap%*2
	gMove dx%,dy%
	if gX<>dx% :raise 1 :endif
	if gY<>dy% :raise 2 :endif
	rem print "gMove ";dx%;",";dy%,"to";gX;",";gY	
endp


proc twidth:
	local l%
	setup:("gTwidth cursor position")
	l%=gTWidth("1234567890")
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gTWidth: no move"
endp


proc print:
	local s$(30)
	setup:("gPrint cursor position")
	s$="Cursor after this"
	gPrint s$
	if gX<>gap%+gTWidth(s$) :raise 1 :endif
	if gY<>gap% :raise 2 :endif
endp


proc printCl:
	local w%
	setup:("gPrintCl cursor position")
	gPrintClip("gPrintClip(x$,100) is too long to fit in 100 pixels",100)
	if gX<>gap%+100 : raise 1 : print "PANIC! gX=";gx;"  gap%=";gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gPrintClip: beyond box"
endp


proc printB:
	setup:("gPrintB cursor position")
	gPrintB "gPrintB x$,100",100
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gPrintB: no move"
endp


proc lineBy:
	local dx%,dy%
	setup:("gLineBy cursor position")
	dx%=4*gap% :dy%=2*gap%
	gLineBy dx%,dy%
	rem print "gLineBy ";dx%;",";dy%
	if gX<>gap%+dx% :raise 1 :endif
	if gY<>gap%+dy% :raise 2 :endif
	rem print "Cursor after line"
endp


proc lineTo:
	local x%,y%
	setup:("gLineTo cursor position")
	x%=gap%*4 :y%=gap%*3
	gLineTo x%,y%
	rem print "gLineTo ";x%;",";y%
	if gX<>x% :raise 1 :endif
	if gY<>y% :raise 2 :endif
	rem print "Cursor after line"
endp


proc box:
	local dx%,dy%
	setup:("gBox cursor position")
	dx%=(gap%*25)/10
	dy%=(gap%*15)/10
	gBox dx%,dy%
	rem print "gBox ";dx%;",";dy%
	if gX<>gap% :raise 1 : print "PANIC! gX=";gX :get :raise 1 :endif
	if gY<>gap% :raise 2 : print "PANIC! gY=";gY :get :raise 2 :endif
	rem print "gBox: no move"
endp


proc poly:
	local a%(100)
	setup:("gPoly cursor position")
	a%(1)=gX+gap%
	a%(2)=gY
	a%(3)=2
	a%(4)=gap%*4
	a%(5)=gap%*2
	a%(6)=gap%*4
	a%(7)=-gap%*2
	gPoly a%()
	rem print "gPoly V shape"
	if gX<>gap%+gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "Cursor ";gap%;" right"
endp


proc fill:
	local dx%,dy%
	setup:("gFill cursor position")
	dx%=3*gap% :dy%=2*gap%
	gFill dx%,dy%,0
	rem print "gFill ";dx%;",";dy%;",0"
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gFill: no move"	
endp


proc patt:
	local dx%,dy%
	setup:("gPatt cursor position")
	dx%=3*gap% :dy%=2*gap%
	gPatt -1,dx%,dy%,0
	rem print "gPatt -1,";dx%;",";dy%;",0"
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gPatt: no move"
endp


proc copy:
	setup:("gCopy cursor position")
	trap gCopy 1,0,0,180,20,0
	if err : raise 100 : endif
	rem print "gCopy 1,0,0,180,20,0"
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "gCopy: no move"		
endp


proc scroll:
	local l%
	setup:("gScroll cursor position")
	l%=gap%*2+1
	gScroll 1,0,gap%,gap%,l%,l%
	rem print "gScroll 1,0,";gap%;",";gap%;",";l%;",";l%
	if gX<>gap% :raise 1 :endif
	if gY<>gap% :raise 2 :endif
	rem print "Cursor not moved"
endp


REM End of gCurpos.tpl

