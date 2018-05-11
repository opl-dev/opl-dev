REM gCurs.tpl
REM EPOC OPL automatic test code for graphics/text cursor.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gcurs", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gCurs:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tgcurs")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	TRAP gCLOSE 2
	TRAP gCLOSE 3
	gCLS
	CLS
ENDP


proc tGCurs:
	local row%,col%,idW2%,idB1%,g$(1)

	cls
	idB1%=gCreateBit(40,40)
	onerr e::
	cursor idB1%
	onerr off
	raise 1	:rem cursor must fail in bitmap
e::
	onerr off
	if err<>-119 :raise 2 :endif	:rem "Invalid window"
	idW2%=gCreate(20,20,40,40,1)
	gAt 0,0
	gBox 40,40
	gAt 20,20 :gPrint "X"
	cursor idW2%
	rem pause -30 :key
	gOrder 1,1
	gUse 1	
	escape on
	rem print "Console cursor test"
	rem at 1,3 :print "Cursor on" :cursor on :pause -30 :key
	rem at 1,3 :print "Cursor off" :cursor off :pause -30 :key
	cls
	rem print "Graphic cursor test"
	rem pause 20
	doCurs:(1,2,10,5)
	doCurs:(1,127,255,255)
	doCurs:(1,-2,10,10)
	doCursD:(1)
	cls
	rem print "Default curs after ""Hello"""
	gAt 1,45
	gPrint"Hello"
	rem pause -30 :key
	cls
	rem print "and cursor off"
	cursor off
	rem pause -30 :key
	cursor 1
	cls
	rem print "Default cursor"
	col%=1
	row%=1
	gFont 9
	gAt 1,30
	while row%<3
rem		g$=get$
		g$=chr$(rnd*10+$30)
		rem pause -5
	   rem  if key=27: break :endif
		if col%=20
			col%=1
			row%=row%+1
			gAt 1,row%*13+17
			rem pause -20
	    rem    if key=27: break :endif
		endif
		gPrint g$
		col%=col%+1
	endwh

	rem pause -30 :key
	cursor 1,30,35,35
	cls
	rem print "cursor 1,30,35,35"
	col%=1
	row%=1
	gAt 1,row%+30
	while gY<gHeight
rem		g$=get$
		g$=chr$(rnd*10+$30)
		rem pause -5
	  rem if key=27: break :endif
		if col%=4
			col%=1
			row%=row%+1
			gAt 1,row%*13+17
			rem pause -2
	    rem if key=27: break :endif
		endif
		gPrint g$
		col%=col%+1
	endwh
	rem pause -30 :key
  cursor off
endp


proc doCurs:(id%,asc%,width%,height%)
	gCls
	rem at 1,1
	rem print "id ascent width height"
	rem print " ";id%;"  ";asc%;"      ";width%;"    ";height%
	gAt 0,45 :gLineBy 180,0
	gAt 1,45
	cursor id%,asc%,width%,height%
	rem pause -50 :key
endp


proc doCursD:(id%)
	gCls
	rem at 1,1
	rem print "id ascent width height"
	rem print " ";id%;"     d e f a u l t s "
	gAt 0,45 :gLineBy 180,0
	gAt 1,45
	cursor id%
	rem pause -50 :key
endp

REM End of gCurs.tpl

