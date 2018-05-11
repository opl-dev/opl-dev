REM gSize.tpl
REM EPOC OPL automatic test code for gSize.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gsize", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gSize:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogsize")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	trap delete "c:\Opl1993\gsize.*"
ENDP


proc dogSize:
	local idW1%,idB1%,idB2%,idB3%
	trap mkdir "c:\Opl1993"
	checkSz:("Default size=",0,0)
	idW1%=gCreate(100,10,40,10,1)
	gBox 40,10
	checkSz:("W1",40,10)
	idB1%=gCreateBit(1,2)
	checkSz:("B1",1,2)
	gUse 1
	checkSz:("Def",0,0)
	gUse 2
	checkSz:("W1",40,10)
	gUse 3
	checkSz:("B1",1,2)

	gSaveBit "c:\Opl1993\gsize.opb",1,2
	idB2%=gLoadBit("c:\Opl1993\gsize.opb",0,0)
	checkSz:("B2",1,2)

	gSaveBit "c:\Opl1993\gsize.opr",1,1
	idB3%=gLoadBit("c:\Opl1993\gsize.opr",0,0)
	checkSz:("B3",1,1)
	
	gUse idW1%
	gSetWin 100,30,41,11
	gAt 0,0
	gBox 41,11
	checkSz:("W1 mod",41,11)
	rem print "Done" :pause -30 :key

	gclose idW1%
	gclose idb1%
	gclose idb2%
	gclose idb3%
	gcls
endp


proc checkSz:(s$,w%,h%)
rem  if w%=0, just displays the real width
	if w%
		if w%<>gWidth :raise 1 :endif
		if h%<>gHeight :raise 2 :endif
	endif
	rem print s$,gWidth;"x";gHeight
endp

REM End of gSize.tpl
