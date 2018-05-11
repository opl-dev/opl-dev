REM gloadsavebit.tpl
REM EPOC OPL automatic test code for gsavebit/gloadbit commands.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gloadsavebit", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gloadsavebit:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogloadsavebit")
 	hCleanUp%:("CleanUp")
endp


proc cleanup:
	TRAP DELETE "c:\Opl1993\*.*"
	trap rmdir "c:\Opl1993"
	cls
	gcls
endp


proc dogloadsavebit:
	trap mkdir "c:\Opl1993"
	gsavebit:
	gloadbit:
endp


REM ===== from gsavebit =====


proc gSaveBit:
	local id1%,id2%,s$(128)
	global x%,y%,wid%,hgt%
	
	x%=1
	y%=15
	wid%=150
	hgt%=60
	id1%=gCreate(x%,y%,wid%,hgt%,1)
	gfont 5
	s$="c:\Opl1993\gSaveWin"
	save:(s$)
	gClose id1%
	load:(s$)
	id2%=gCreateBit(wid%,hgt%)
	gfont 5
	s$="c:\Opl1993\gSaveBit"
	save:(s$)
	gClose id2%
	load:(s$)
	rem print "Done"
	rem pause -30 :key
endp


proc save:(s$)
	s:(s$+".opb",0,0)
	s:(s$+".pic",0,0)
	s:(s$+".opr",wid%-20,hgt%-20)
endp


proc s:(s$,w%,h%)
	gCls
	gAt 0,0
	if w% or h%
		gBox w%,h%
	else
		gbox wid%,hgt%
	endif
	gAt 5,hgt%/2+10
	gPrint s$,gWidth,gHeight
	gAt 0,0
	gLineBy wid%,hgt%
	if w% or h%
		gAt 0,0								:rem or saves junk
		gSaveBit s$,w%,h%			:rem save a rectangle
	else
		gSaveBit s$						:rem whole bitmap
	endif
	cls
	rem print "saved",s$
	rem pause -30 :key
	gcls
endp


proc load:(s$)
	cls
	l:(s$+".opb")
	l:(s$+".opr")
	l:(s$+".pic")
endp


proc l:(s$)
	local id%,w%,h%
	
	id%=gLoadBit(s$,0,0)
	w%=gWidth
	h%=gHeight
	gUse 1
	gAt 0,15
	gCopy id%,0,0,w%,h%,0
	gClose id%
	rem print "loaded",s$
	rem pause -30 :key
	cls
endp


REM ===== from gloadbit =====


proc gLoadBit:
	op:("c:\Opl1993\gSaveBit.opb")
	op:("c:\Opl1993\gSaveBit.opr")
	op:("c:\Opl1993\gSaveBit.pic")
	write:("c:\Opl1993\gSaveBit.pic")
	optArgs:
endp


proc op:(s$)
	local idBit1%,wBit1%,hBit1%
	gcls
	idBit1%=gLoadBit(s$,1,0)
	gfont 5
	gAt 5,10 :gPrint "gLoadBit",s$,"worked"
	wBit1%=gWidth :hBit1%=gHeight
	gAt 5,20 :gPrint "Width=";wBit1%;"  height=";hBit1%
	gUse 1
	cls
	gCopy idBit1%,0,0,wBit1%,hBit1%,0
	gAt 10,hBit1%+20
	gPrint "gCopy worked ok"
	rem pause -30 :key
	gClose idBit1%
endp


proc write:(s$)
	local idBit1%,wBit1%,hBit1%
	rem open bmp with read only access
	gcls
	idBit1%=gLoadBit(s$,0,0) 
	gfont 5
	gAt 5,10 :gPrint "gLoadBit",s$,"worked"
	wBit1%=gWidth :hBit1%=gHeight
	gAt 5,20 :gPrint "Width=";wBit1%;"  height=";hBit1%
	gUse 1
	cls
	gCopy idBit1%,0,0,wBit1%,hBit1%,0
	gAt 10,hBit1%+20
	gPrint "gCopy worked ok - top text should be missing"
	rem pause -30 :key
	guse idBit1%
	gSavebit(s$)
	rem should have been ignored
	gClose idBit1%

	idBit1%=gLoadBit(s$,1,0) 
	guse 1
	gcls
	gCopy idBit1%,0,0,wBit1%,hBit1%,0
	gAt 10,hBit1%+20
	gPrint "gCopy worked ok - top text should be missing"
	rem pause -30 :key
	gclose idBit1%
endp


proc optArgs:
	local id1%,id2%,id3%,w%,h%
	gCls
	id1%=gLoadBit("c:\Opl1993\gSaveBit.pic",1,0)
	id2%=gLoadBit("c:\Opl1993\gSaveBit.pic",1)
	id3%=gLoadBit("c:\Opl1993\gSaveBit.pic")
	w%=gWidth
	h%=gHeight
	gUse 1
	gAt 0,0
	gCopy id1%,0,0,w%,h%,0
	gAt 0,h%+5
	gCopy id2%,0,0,w%,h%,0
	gAt 0,2*(h%+5)
	gCopy id3%,0,0,w%,h%,0
	rem pause -30 :key
	gclose id3%
	gclose id2%
	gclose id1%
endp


REM End of gloadsavebit.tpl

