REM gScroll.tpl
REM EPOC OPL automatic test code for gscroll.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gscroll", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gscroll:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogscroll")
	rem hCleanUp%:("CleanUp")
endp


proc dogScroll:
	RANDOMIZE 31415
	scrlBit:
	scrlWin:
	scrlSim:
endp


proc scrlSim:
	local i%

	gUse 1
	cls
	trap gClose 2
	trap gClose 3
	trap gClose 4
	gAt 0,13
	while i%<6
		i%=i%+1
		gPrint i%
		gMove -gX,13
	endwh
	gMove 0,-13
	do
		gScroll 0,-13
		i%=i%+1
		gMove -gX,0
		gPrint i%
rem	until key=27	or i%=15
	until i%=15
endp


proc scrlBit:
	local id%,w%,h%

	w%=100 :h%=80
	id%=gCreateBit(w%,h%)
rem	id%=gCreate(gOriginX,gOriginY,w%,h%,1)
	gCls
	gLineBy w%,h%
	gAt 0,0
	gFill w%,h%,2
	gUse 1
	gAt 0,0
	gCopy id%,0,0,w%,h%,0
	gUse id%
	gScroll 0,10,0,0,w%,h%
rem	gScroll 0,10
	gUse 1
	gCls
	gAt 0,0
	gCopy id%,0,0,w%,h%,0	
	gClose id%
endp


proc scrlWin:
	local i%,dx%,dy%,k%,km%
	local ext%(4)
	local id%
	local count%

	ext%(1)=40
	ext%(2)=10
	ext%(3)=50
	ext%(4)=30
	do
		gCls
		gAt ext%(1),ext%(2)
		gBox ext%(3),ext%(4)
		gAt ext%(1),ext%(2)
		gLineBy ext%(3),ext%(4)

rem 	at 1,7 :print "dx>",
rem		do
rem		trap input dx%
rem			if err=-114 :return :endif
rem			if err :at 5,7 :print "           " :at 5,7 :endif		
rem		until err=0
rem
rem		print "dy>",
rem		do
rem			trap input dy%
rem			if err=-114 :return :endif
rem			if err :at 5,8 :print "           " :at 5,8 :endif		
rem		until err=0
		dx%=rnd*30
		dy%=rnd*10
		gScroll dx%,dy%,ext%(1),ext%(2),ext%(3),ext%(4)
		rem at 1,9 :print "Press a key for next";
		count%=count%+1
	until count%=12
endp


proc clrBox:(dx%,dy%,x%,y%,width%,height%,clr%)
	local box%(4),v1%,v2%,v3%,v4%,v5%,v6%,v7%,v8%

	v1%=x%
	v2%=y%
	v3%=x%+width%
	v4%=y%+height%
	v5%=x%+dx%
	v6%=y%+dy%
	v7%=x%+width%+dx%
	v8%=y%+height%+dy%
	box%(1)=v1% :box%(2)=v2% :box%(3)=v3% :box%(4)=v4%
	if abs(dx%)>=width% or abs(dy%)>=height%
		if clr%
			fillRect:(addr(box%()))
		else
			markRect:(addr(box%()))
		endif
	else
		if dx%<>0
			if dx%>0
				box%(3)=v5%
			else
				box%(1)=v7%
			endif
			if clr%
				fillRect:(addr(box%()))
			else
				markRect:(addr(box%()))
			endif
		endif

		box%(1)=v1% :box%(2)=v2% :box%(3)=v3% :box%(4)=v4%
		if dy%<>0
			if dy%>0
				box%(4)=v6%
			else
				box%(2)=v8%
			endif
			if clr%
				fillRect:(addr(box%()))
			else
				markRect:(addr(box%()))
			endif
		endif
	endif
endp		

		
proc markRect:(pBox%)
		rem Marks the rectangle cleared
		local width%,height%,tl%,tr%
		
		tl%=peekW(pBox%)
		tr%=peekW(pBox%+2)
		width%=peekW(pBox%+4)-tl%
		height%=peekW(pBox%+6)-tr%

		gAt tl%,tr%
		gFill width%,height%,0
endp


proc fillRect:(pBox%)
		rem fills a rectangle
		local width%,height%,tl%,tr%
		
		tl%=peekW(pBox%)
		tr%=peekW(pBox%+2)
		width%=peekW(pBox%+4)-tl%
		height%=peekW(pBox%+6)-tr%

		gAt tl%,tr%
		gBox width%,height%
		gAt tl%,tr%
		gFill width%,height%,1
endp

REM End of gScroll.tpl

