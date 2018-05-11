REM pNewfont.tpl
REM EPOC OPL automatic test code for opler1 font code.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pnewfont", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pnewfont:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tfont")
rem 	hCleanUp%:("CleanUp")
endp


proc tfont:
	global fid%,info%(10),drv$(255)
	
	drv$=hDiskName$:+"\Opltest\Data\" rem "testfont.gdr"
		
	print "Test gLOADFONT for user-defined font"
	print
	rem pause 20
	screeninfo info%()
	print "Test errors before loading any fonts"
	print
	terror:
	cls
	print "In the following tests, there should not be empty lines between print statements"
	print
	rem get
	test1:
	test2:
	screen info%(3),info%(4)
	test3:
	screen info%(3),info%(4)
	test4:
	screen info%(3),info%(4)
	print "Test errors after loading fonts"
	print
	terror:
	
	print
	print "Test gLOADFONT of user-defined font Finished OK"
	rem pause 30
endp


proc test1:
	local i%
	trap gunloadfont fid%
	print "1. Test that loading works and fonts are as expected"
	rem pause 30
	fid%=gLoadfont(drv$+"testfont.gdr")
	cls
	gfont 300000001  rem tstfont1
	gat 10,20
	gprint "TSTFONT1: THIS SHOULD BE SWISS 15, BUT THE FOLLOWING WILL BE GREEK:"
	gat 10,50
	gprint "a b c d e h i k m n o p r t"
	gat 10,80 
	gprint "THIS SHOULD BE SWISS 15:"
	gat 10,110
	gprint "f g j l q s u v w x y z"
	gat 10,140
	gprint "AND THIS:"
	i%=33
	gat 10,170
	while i%<40
		gprint chr$(i%)," "
		i%=i%+1
	endwh
	gat 10,200
	gprint "A mixture of Greek and Roman!"
	gat 10,230
	gprint "FIRST CHAR: ";chr$(6);"  LAST CHAR: ";chr$(255)
	rem get
	gcls
	gfont 300000002		rem tstfont2
	gat 10,30
	gprint "TSTFONT2: This font is mainly the same as Swiss 11"
	gat 10,60
	gprint "But these characters are different"
	gat 10,90
	gprint chr$(6)
	gat 10,120
	gprint "Characters 33-41 should be musical"
	i%=33
	gat 10,150
	while i%<42
		gprint chr$(i%)," "
		i%=i%+1
	endwh
	gat 10,180
	gprint "First char: ";chr$(6);"  Last char: ";chr$(255)
	rem get
	gcls
	
	gat 10,30
	gprint "Try using Opler1 font while TESTFONT loaded"
	gfont 12
	gat 10,60
	gprint "Opler1 Font 12 - Arial 15"
	gfont 10
	gat 10,90
	gprint "Opler1 Font 10 - Arial 12"
	gfont 7
	gat 10,120
	gprint "Opler1 Font 7 - Times 13"
	rem get
	gcls
	gunloadfont fid%
endp

proc test2:
	local w%
	
	trap gunloadfont fid%
		
	print "2. Test can still use fonts after unloaded if font is used while loaded"
	rem pause 30
	cls
	w%=info%(3)/2
	screen 20,5,w%,1
	gfont 7
	gat 10,30
	gprint "Loading TESTFONT"
	
	fid%=gLoadfont(drv$+"testfont.gdr")
	gprint " ... Loaded"
	ttstfont1:(60)
	gat 10,90
	gprint "NOW UNLOADING TESTFONT"
	gunloadfont fid%
	
	gat 10,120
	gprint "STILL TSTFONT1! GREEK: a b c"

if 0	
rem if trybug:("font unloaded")
	onerr font1
	ttstfont2:(150)
	onerr off
	raise 1
font1::
	onerr off
	if err<>-21 : raise 2 : endif
	
	onerr font2
	ttstfont1:(150)
	onerr off
	raise 3
font2::
	onerr off
	if err<>-21 : raise 4 : endif
endif

	topler1font:(150)

if 0
rem if trybug:("font unloaded")
	onerr font3
	ttstfont2:(180)
	onerr off
	raise 5
font3::
	onerr off
	if err<>-21 : raise 6 : endif
	endif
	rem get
	gcls
endp

proc test3:
	local w%

	print "3. Test can still use font after unloading when not used while loaded"
	rem pause 30
	cls
	w%=info%(3)/2
	screen 30,5,w%,1
	gfont 7
	gat 10,30
	gprint "Loading and unloading TESTFONT"
	fid%=gLoadfont(drv$+"testfont.gdr")
	gunloadfont fid%

if 0
rem if trybug:("font unloaded")
	onerr font1
	ttstfont1:(60)
	onerr off
	raise 1 
font1::
	onerr off
	if err<>-21 : raise 2 : endif
	
	onerr font2
	ttstfont2:(60)
	onerr off
	raise 3
font2::
	onerr off
	if err<>-21 : raise 4 : endif
endif

	topler1font:(60)

if 0
rem if trybug:("font unloaded")	
	onerr font3
	ttstfont2:(90)
	onerr off
	raise 5
font3::
	onerr off
	if err<>-21 : raise 6 : endif
endif
	rem get
	gcls
endp

proc test4:
	local w%
	print "4. Test can use font after unloading when not used while loaded when opler1 font is used while loaded"
	rem pause 30
	cls
	w%=info%(3)/2
	screen 30,5,w%,1
	gfont 7
	gat 10,30
	gprint "Loading TESTFONT"
	fid%=gLoadfont(drv$+"testfont.gdr")
	topler1font:(60)
	gat 10,90
	gprint "Unloading TESTFONT"
	gunloadfont fid%
if 0
rem if trybug:("font unloaded")
	onerr font1
	ttstfont1:(120)
	onerr off
	raise 1
font1::
	onerr off
	if err<>-21 : raise 2 : endif
	
	onerr font2
	ttstfont2:(120)
	onerr off
	raise 3
font2::
	onerr off
	if err<>-21 : raise 4 : endif
endif

	topler1font:(120)

if 0
rem if trybug:("font unloaded")	
	onerr font3
	ttstfont2:(150)
	onerr off
	raise 5
font3::
	onerr off
	if err<>-21 : raise 6 : endif
endif

	rem get
	gcls
endp


proc terror:
	local fid1%,fid2%
	goto skipunloadtests::	


	trap gunloadfont fid%
	print "Test non-existent font file cannot be loaded"
	print 
	if exist (drv$+"testfont1.gdr") : alert("File exists - invalid test, please change") : endif
	onerr nexist
	fid%=gloadfont (drv$+"testfont1.gdr")
	onerr off
	raise 1
nexist::
	onerr off
	if err<>-33 : print err$(err) : raise 2 : endif
	
	print "Test cannot use unloaded fonts"
	print
	onerr nload1
	ttstfont1:(60)
	onerr off
	raise 3
nload1::
	onerr off
	if err<>-21 : raise 4 : endif
	onerr nload2
	ttstfont2:(90)
	onerr off
	raise 5
nload2::
	onerr off
	if err<>-21 : raise 6 : endif

skipunloadtests::	
	print "Cannot use fileid% with gfont"
	print
	fid%=gloadfont(drv$+"testfont.gdr")
	onerr fid
	gfont fid%
	onerr off
	raise 7
fid::
	onerr off
	if err<>-21 : raise 8 : endif
	gunloadfont fid%

	print "Cannot use fileId% with gclose"
	print
	fid%=gloadfont(drv$+"testfont.gdr")
	onerr tclose
	gclose fid%
	onerr off
	raise 9
tclose::
	onerr off
	if err<>-118 and err<>-2 : print err$(err) : raise 10 : endif
	gunloadfont fid%

	print "Can use gLOADFONT with SETPATH"	
	setpath drv$
	fid%=gLoadfont("testfont.gdr")
	gunloadfont fid%
	
	print "Unloading a font which is not loaded causes an error"
	trap gunloadfont fid%    rem just to make sure unloaded
	fid%=4096
	onerr nload
	gunloadfont fid%
	onerr off
	raise 11
nload::
	onerr off
	if err<>-2 : raise 12 : endif
	
	print "Can load the same font twice without error"
	trap gunloadfont fid%
	print "Loading once ..."
	fid1%=gLoadfont(drv$+"testfont.gdr")
	print "Loading twice ..."
	fid2%=gLoadfont(drv$+"testfont.gdr")
	trap gunloadfont fid%
	trap gunloadfont fid%
	
	print
	print "OK"
	rem pause 20
	cls
endp


proc ttstfont1:(starty%)
	gfont 300000001
	gat 10,starty%
	gprint "USING TSTFONT1! GREEK: a b c"
endp

proc ttstfont2:(starty%)
	gfont 300000002
	gat 10,starty%
	gprint "USING TSTFONT2! Music: ",chr$(33),chr$(35),chr$(37)
endp

proc topler1font:(starty%)
	gfont 7
	gat 10,starty%
	gprint "This is Opler1 font 7 - Times 13"
endp

REM End of pNewFont.tpl

