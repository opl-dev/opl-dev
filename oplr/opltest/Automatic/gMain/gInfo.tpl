REM gInfo.tpl
REM EPOC OPL automatic test code for gInfo.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gInfo", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gInfo:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogInfo")
rem	hCleanUp%:("CleanUp")
rem	KLog%:(KhLogHigh%,"Some sample text")
endp


proc dogInfo:
	local id2%,bit1%
	cursor on
	info:
	gAt 135,15
	cursor 1
	info:
	gFont 11
	gAt 135,15
	cursor 1
	info:
	bit1%=gCreateBit(20,20)
	info:
	id2%=gCreate(140,0,20,40,1)
	gGMode 2
	gTMode 2
	gStyle 2
	gBox 20,40
	gAt 2,15
	gPrint "Hp"
	gAt 2,15
	cursor id2%,10,3,12
	info:
	gClose id2%
	gClose bit1%
	gcls
endp


proc info:
	local a&(50),i%,j%,lbc%

	cls
	i%=1
	gInfo32 a&()
	print "Font Info"
	print i%;". lowest         =";a&(i%) :i%=i%+1
	print i%;". highest        =";a&(i%) :i%=i%+1
	print i%;". height         =";a&(i%) :i%=i%+1
	print i%;". descent        =";a&(i%) :i%=i%+1
	print i%;". ascent         =";a&(i%) :i%=i%+1
	print i%;". width 0        =";a&(i%) :i%=i%+1
	print i%;". maximum width  =";a&(i%) :i%=i%+1
	print i%;". flags          =$";right$(hex$(a&(i%)),4) :i%=i%+1
	print i%;". name=""";peek$(addr(a&(i%)));""""
	
	i%=18
	print
	print "Current GC info"
	print i%;". graphics mode  =";a&(i%) :i%=i%+1
	print i%;". text mode      =";a&(i%) :i%=i%+1
	print i%;". style          =";a&(i%) :i%=i%+1
	print
	print "Cursor info"
	print i%;". state          =";a&(i%) :i%=i%+1
	print i%;". window id      =";a&(i%) :i%=i%+1
	print i%;". width          =";a&(i%) :i%=i%+1
	print i%;". height         =";a&(i%) :i%=i%+1
	print i%;". ascent         =";a&(i%) :i%=i%+1
	print i%;". x position     =";a&(i%) :i%=i%+1
	print i%;". y position     =";a&(i%) :i%=i%+1
	print i%;". bitmap flag    =";a&(i%); :i%=i%+1
	print
endp

REM End of gInfo.tpl
