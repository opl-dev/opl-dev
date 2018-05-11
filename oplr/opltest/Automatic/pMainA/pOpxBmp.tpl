REM pOpxBmp.tpl
REM EPOC OPL automatic test code for BMP opx.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
include "bmp.oxh"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "popxbmp", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP

CONST KPath$="c:\Opl1993\"

proc pOpxBmp:
	global drv$(255)
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("genbitmaps")
	hRunTest%:("tload")
	hRunTest%:("tdispmode")
	hRunTest%:("delbitmaps")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	trap delete kpath$+"*.*"
	trap rmdir kpath$	
ENDP


proc genbitmaps:
	local id%
	trap mkdir kpath$
	rem generate three bitmaps with different modes
	id%=gcreatebit(50,50)
	glineby 50,50
	gsavebit(kpath$+"mode.bmp")
	gclose id%
	id%=gcreatebit(50,50,0)
	glineby 50,50
	gsavebit(kpath$+"mode0.bmp")
	gclose id%
	id%=gcreatebit(50,50,1)
	glineby 50,50
	gsavebit(kpath$+"mode1.bmp")
	gclose id%
	id%=gcreatebit(50,50,2)
	glineby 50,50
	gsavebit(kpath$+"mode2.bmp")
	gclose id%	
	
	rem also generate a datafile
	trap delete kpath$+"data.odb"
	trap create kpath$+"data.odb",A,int%,str1$,str2$
	A.int%=1
	A.str1$="A datafile for testing bitmap OPX"
	A.str1$="Created OK"
	append
	close	
endp


proc tload:
	local bmpId&,bmpId1&,bmpId2&
	local id&(201),i%,n%
	
	print "Testing BITMAPLOAD&: and BITMAPUNLOAD: functions"
	print
	
	print "Non-existent files"
	onerr e1
	bmpId&=bitmapload&:(kpath$+"notexist.bmp",0)
	onerr off
	raise 1
e1::
	onerr off
	if err<>-33 : raise 2 : endif
	
	print "Database files"
	onerr e2
	bmpId&=bitmapload&:(kpath$+"data.odb",0)
	onerr off
	raise 2
e2::
	onerr off
	if err<>-4: print err : raise 4 : endif
	
	rem unload non-existent 
	bmpId&=2
	onerr e3
	bitmapunload:(bmpId&)
	onerr off
	raise 5
e3::
	onerr off
	if err<>-33 : raise 6 : endif
	
	print "Loading past end of file"
	print
	rem load more than 1 bitmap from 1 bitmap file
	bmpId1&=bitmapload&:(kpath$+"mode0.bmp",0)
	onerr e4
	bmpId2&=bitmapload&:(kpath$+"mode0.bmp",1)
	onerr off
	raise 7
e4::
	onerr off 
	if err<>-36 : print err : raise 8 : endif
	bitmapunload:(bmpId1&)

	drv$=hDiskName$:+"\Opltest\Data\lots.mbm"
	print 
	print "Loading 200 bitmaps from PC generated MBM file"
	busy "Loading..."
	while i%<200
		n%=i%
		i%=i%+1		
		id&(i%)=bitmapload&:(drv$,n%)
	endwh
	busy off
	rem one more
	print 

	onerr e5
	id&(i%+1)=bitmapload&:(drv$,200)
	onerr off
	raise 9
e5::
	onerr off
	if err<>-36 : print err : raise 10 : endif
		
	i%=0
	while i%<200
		i%=i%+1
		bitmapunload:(id&(i%))
	endwh		
	print
	print "OK"	
	print
endp


proc tdispmode:
	local bmpId&,mode&,id%
	
	print "Testing BITMAPDISPLAYMODE&:"
	print
	
	bmpId&=bitmapload&:(kpath$+"mode0.bmp",0)
	mode&=bitmapdisplaymode&:(bmpId&)
	if mode&<>0 : print mode& : raise 1 : endif
	bitmapunload:(bmpId&)
	
	bmpId&=bitmapload&:(kpath$+"mode1.bmp",0)
	mode&=bitmapdisplaymode&:(bmpId&)
	if mode&<>1 : print mode& : raise 2 : endif
	bitmapunload:(bmpId&)
	
	bmpId&=bitmapload&:(kpath$+"mode2.bmp",0)
	mode&=bitmapdisplaymode&:(bmpId&)
	if mode&<>2 : print mode& : raise 3 : endif
	bitmapunload:(bmpId&)

	rem this will fail if default changed	
	bmpId&=bitmapload&:(kpath$+"mode.bmp",0)
	mode&=bitmapdisplaymode&:(bmpId&)
	if mode&<>0 : print mode& : raise 4 : endif
	bitmapunload:(bmpId&)
	
	bmpId&=bitmapload&:(drv$,0)
	mode&=bitmapdisplaymode&:(bmpId&)
	if mode&<>1 : print mode& : raise 5 : endif
	bitmapunload:(bmpId&)

	drv$=hDiskName$:+"\Opltest\Data\pface.mbm"
	bmpId&=bitmapload&:(drv$,0)
	mode&=bitmapdisplaymode&:(bmpId&)
	if mode&<>2 : print mode& : raise 20 : endif
	bitmapunload:(bmpId&)

	print "Test with unloaded bitmap and window"
	bmpId&=2
	onerr e1
	mode&=bitmapdisplaymode&:(bmpId&)
	onerr off
	raise 6
e1::
	onerr off 
	if err<>-33 : print err : raise 7 : endif
	
	id%=gcreate(0,0,10,10,1,2)
	onerr e2
	mode&=bitmapdisplaymode&:(int(id%))
	onerr off
	raise 8
e2::
	onerr off 
	gclose id%
	if err<>-33 : raise 9 : print err : endif

	print
	print "OK"
	print 
endp


proc delbitmaps:
	rem delete bitmaps
	trap delete kpath$+"mode.bmp"
	trap delete kpath$+"mode0.bmp"
	trap delete kpath$+"mode1.bmp"
	trap delete kpath$+"mode2.bmp"
	trap delete kpath$+"data.odb"
endp

REM End of pOpxBmp.tpl

