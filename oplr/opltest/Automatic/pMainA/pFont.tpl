REM pFont.tpl
REM EPOC OPL automatic test code for fonts.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pfont", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pfont:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tFont")
rem	hCleanUp%:("CleanUp")
endp


proc tfont:
	global x%,y%,size%	

	print "Opler1 Font Tests"
	print "Throughout this test, press a key to continue"
	print
	tgfonts:
	font KFontCourierNormal11&,0
	tfonts:
	font KFontCourierNormal11&,0
	tnewgfonts: 
	font KFontCourierNormal11&,0
	tnewfonts:
	font KFontCourierNormal11&,0
	tstyle:
	font KFontCourierNormal11&,0
	tfonterrors:
	font KFontCourierNormal11&,0
	tstyleerrors:
	font KFontCourierNormal11&,0
	print
	print "Opler1 Font Tests Finished OK"
	rem pause -30
endp


proc tgfonts:
	x%=350
	print "Test Opl1993 compatibility for gFONT"
	rem pause -20
	cls
	gprintfont:(5,35,"Type 5 maps to Times 8")
	gprintfont:(6,50,"Type 6 maps to Times 11")
	gprintfont:(7,66,"Type 7 maps to Times 13")
	gprintfont:(8,84,"Type 8 maps to Times 15")
	gprintfont:(9,96,"Type 9 maps to Arial 8")
	gprintfont:(10,111,"Type 10 maps to Arial 11")
	gprintfont:(11,128,"Type 11 maps to Arial 13")
	gprintfont:(12,146,"Type 11 maps to Arial 15")
	gprintfont:(4,158,"Type 4 maps to Courier 8")
	gprintfont:(13,168,"Type 13 maps to Tiny 4 (mono)")
	gprintfont&:(KFontTimesNormal8&,35,"Times 8")
	gprintfont&:(KFontTimesNormal11&,50,"Times 11")
	gprintfont&:(KFontTimesNormal13&,66,"Times 13")
	gprintfont&:(KFontTimesNormal15&,84,"Times 15")
	gprintfont&:(KFontArialNormal8&,96,"Arial 8")
	gprintfont&:(KFontArialNormal11&,111,"Arial 11")
	gprintfont&:(KFontArialNormal13&,128,"Arial 13")
	gprintfont&:(KFontArialNormal15&,146,"Arial 15")
	gprintfont&:(KFontCourierNormal8&,158,"Courier 8")
	gstyle $10
	gprintfont&:(KFontTiny4&,168,"Tiny 4 (mono)")
	rem get
	gcls	
endp

proc tfonts:
	print "Test Opl1993 compatibility for FONT"
	rem pause -20
	x%=30
	printfont:(5,0,"Type 5 maps to Times 8")
	printfont&:(KFontTimesNormal8&,0,"Times 8")
	printfont:(6,0,"Type 6 maps to Times 11")
	printfont&:(KFontTimesNormal11&,0,"Times 11")
	printfont:(7,0,"Type 7 maps to Times 13")
	printfont&:(KFontTimesNormal13&,0,"Times 13")
	printfont:(8,0,"Type 8 maps to Times 15")
	printfont&:(KFontTimesNormal15&,0,"Times 15")
	printfont:(9,0,"Type 9 maps to Arial 8")
	printfont&:(KFontArialNormal8&,0,"Arial 8")
	printfont:(10,0,"Type 10 maps to Arial 11")
	printfont&:(KFontArialNormal11&,0,"Arial 11")
	printfont:(11,0,"Type 11 maps to Arial 13")
	printfont&:(KFontArialNormal13&,0,"Arial 13")
	printfont:(12,0,"Type 12 maps to Arial 15")
	printfont&:(KFontArialNormal15&,0,"Arial 15")
	x%=35
	printfont:(4,0,"Type 4 maps to Courier 8")
	printfont&:(KFontCourierNormal8&,$10,"Courier 8")
	printfont:(13,0,"Type 13 maps to Tiny 4 (mono)")
	printfont&:(KFontTiny4&,$10,"Tiny 4 (mono)")
endp

proc tnewgfonts:
	x%=30
	print "Test Opler1 fonts"
	rem pause 30
	gcls
	gprintfont&:(KFontArialNormal18&,40,"Arial 18")
	gprintfont&:(KFontArialNormal22&,65,"Arial 22")
	gprintfont&:(KFontArialNormal27&,95,"Arial 28")
	gprintfont&:(KFontArialNormal32&,130,"Arial 32")
	rem get
	gcls
	gprintfont&:(KFontArialBold8&,40,"Arial 8 Bold")
	gprintfont&:(KFontArialBold11&,55,"Arial 11 Bold")
	gprintfont&:(KFontArialBold13&,70,"Arial 13 Bold")
	rem get
	gcls
	gprintfont&:(KFontTimesNormal18&,40,"Times 18")
	gprintfont&:(KFontTimesNormal22&,65,"Times 22")
	gprintfont&:(KFontTimesNormal27&,96,"Times 27")
	gprintfont&:(KFontTimesNormal32&,130,"Times 32")
	rem get
	gcls	
	gprintfont&:(KFontTimesBold8&,40,"Times 8 Bold")
	gprintfont&:(KFontTimesBold11&,55,"Times 11 Bold")
	gprintfont&:(KFontTimesBold13&,70,"Times 13 Bold")
	rem get
	gcls	
	gprintfont&:(KFontTiny1&,10,"Tiny 1")
	gprintfont&:(KFontTiny2&,15,"Tiny 2")
	gprintfont&:(KFontTiny3&,20,"Tiny 3")
	gprintfont&:(KFontTiny4&,25,"Tiny 4")
	gprintfont&:(KFontSquashed&,100,"Squashed")
	rem get
	gcls
	gprintfont&:(KFontCourierNormal8&,35,"Courier 8")
	gprintfont&:(KFontCourierNormal11&,49,"Courier 11")
	gprintfont&:(KFontCourierNormal13&,65,"Courier 13")
	gprintfont&:(KFontCourierNormal15&,82,"Courier 15")
	gprintfont&:(KFontCourierNormal18&,101,"Courier 18")
	gprintfont&:(KFontCourierNormal22&,125,"Courier 22")
	gprintfont&:(KFontCourierNormal27&,155,"Courier 27")
	gprintfont&:(KFontCourierNormal32&,189,"Courier 32")
	rem get
	gcls
	gprintfont&:(KFontCourierBold8&,35,"Courier 8 Bold")
	gprintfont&:(KFontCourierBold11&,49,"Courier 11 Bold")
	gprintfont&:(KFontCourierBold13&,65,"Courier 13 Bold")
	rem get
	gcls
	cls
endp


proc tnewfonts:
	x%=1
	print "Test Opler1 fonts"
	rem pause -30
	cls
	printfont&:(KFontArialNormal18&,0,"Arial 18")
	printfont&:(KFontArialNormal22&,0,"Arial 22")
	printfont&:(KFontArialNormal27&,0,"Arial 28")
	printfont&:(KFontArialNormal32&,0,"Arial 32")
	printfont&:(KFontArialBold8&,0,"Arial 8 Bold")
	printfont&:(KFontArialBold11&,0,"Arial 11 Bold")
	printfont&:(KFontArialBold13&,0,"Arial 13 Bold")
	
	printfont&:(KFontTimesNormal18&,0,"Times 18")
	printfont&:(KFontTimesNormal22&,0,"Times 22")
	printfont&:(KFontTimesNormal27&,0,"Times 27")
	printfont&:(KFontTimesNormal32&,0,"Times 32")
	printfont&:(KFontTimesBold8&,0,"Times 8 Bold")
	printfont&:(KFontTimesBold11&,0,"Times 11 Bold")
	printfont&:(KFontTimesBold13&,0,"Times 13 Bold")
		
	printfont&:(KFontTiny1&,0,"Tiny 1")
	printfont&:(KFontTiny2&,0,"Tiny 2")
	printfont&:(KFontTiny3&,0,"Tiny 3")
	printfont&:(KFontTiny4&,0,"Tiny 4")
	printfont&:(KFontSquashed&,0,"Squashed")	
	
	printfont&:(KFontCourierNormal8&,0,"Courier 9")	
	printfont&:(KFontCourierNormal11&,0,"Courier 11")	
	printfont&:(KFontCourierNormal13&,0,"Courier 13")	
	printfont&:(KFontCourierNormal15&,0,"Courier 15")	
	printfont&:(KFontCourierNormal18&,0,"Courier 18")	
	printfont&:(KFontCourierNormal22&,0,"Courier 22")	
	printfont&:(KFontCourierNormal27&,0,"Courier 27")	
	printfont&:(KFontCourierNormal32&,0,"Courier 32")		
	printfont&:(KFontCourierBold8&,0,"Courier 8 Bold")	
	printfont&:(KFontCourierBold11&,0,"Courier 11 Bold")	
	printfont&:(KFontCourierBold13&,0,"Courier 13 Bold")		
endp


proc tstyle:
	print "Simple test of font styles"
	rem pause -20
	printstyle:(KFontTimesNormal13&,"Times 13")
	font KFontTimesBold13&,$1
	print "This is double bold text"
	rem get
	printstyle:(KFontArialNormal32&,"Arial 32")
	printstyle:(KFontSquashed&,"Squashed")
	
	size%=11
	gprintstyle:(KFontArialNormal11&,"Arial 11")
	gfont KFontArialBold11&
	x%=10 : y%=20
	gprintstyleline:($1,"This is double bold text")
	rem get
	gcls
	size%=27
	gprintstyle:(KFontTimesNormal27&,"Times 27")
	size%=13
	gprintstyle:(KFontCourierNormal13&,"Courier 13")
endp


proc tfonterrors:
	print "Test font errors"
	rem pause 20
	
	onerr errf1
	font 0,0
	onerr off
	raise 1
errf1::
	onerr off
	if err<>-21 : print err$(err) : raise 2 : endif
	onerr errf2
	font 14,0
	onerr off
	raise 3
errf2::
	onerr off
	if err<>-21 : print err$(err) : raise 4 : endif
	onerr errf3
	font 30000000,0
	onerr off
	raise 5
errf3::	onerr off
	if err<>-21 : print err$(err) : raise 6 : endif
	onerr errf4
	font -10,0
	onerr off
	raise 7
errf4::
	onerr off
	if err<>-21 : print err$(err) : raise 8 : endif
		
	onerr errf5
	gfont 0
	onerr off
	raise 9
errf5::
	onerr off
	if err<>-21 : print err$(err) : raise 10 : endif
	onerr errf6
	gfont 14
	onerr off
	raise 11
errf6::
	onerr off
	if err<>-21 : print err$(err) : raise 12 : endif
	onerr errf7
	gfont 30000000
	onerr off
	raise 13
errf7::
	onerr off
	if err<>-21 : print err$(err) : raise 14 : endif
	onerr errf8
	gfont -10
	onerr off
	raise 15
errf8::
	onerr off
	if err<>-21 : print err$(err) : raise 16 : endif
	
	print "OK"
	print
	rem pause 20
endp


proc tstyleerrors:
	print "Test invalid styles leave style unchanged"
	rem pause 20
	font 10,0
	print "This is font 10,0 - plain"
	rem get
	font 10,$40
	print "This is font 10,$40 - plain"
	rem get
	font 10,$8000
	print "This is font 10,$8000 - plain"
	rem get
	font 10,-1
	print "This is font 10,$ffff - all styles"
	rem get
	font 10,$1111
	print "This is font 10,$1111 - bold and mono"
	rem get
	font 10,$40
	print "This is font 10,$40 - plain"
	rem get

	cls
	gfont 10	
	gstyle 0
	gat 10,15
	gprint "This is font 10, style 0 - plain"
	gstyle $40
	gat 10,35
	gprint "This is font 10, style $40 - plain"
	gstyle $8000
	gat 10,55
	gprint "This is font 10, style $8000 - plain"
	gstyle -1
	gat 10,85
	gprint "This is font 10, style $ffff - all styles"
	gstyle $1111
	gat 10,105
	gprint "This is font 10, style $1111 - bold and mono"
	gstyle $40
	gat 10,120
	gprint "This is font 10, style $40 - plain"	
	rem get
	gcls
endp


proc gprintfont:(font%,pos%,name$)
	gfont font%
	gat 30,pos%
	gprint name$
endp


proc gprintfont&:(font&,pos%,name$)
	gfont font&
	gat x%,pos%
	gprint name$
endp


proc printfont:(font%,style%,name$)
	font font%,style%
	print name$
	rem get
endp


proc printfont&:(font&,style%,name$)
	font font&,style%
	at x%,1
	print name$
	rem get
endp


proc printstyle:(uid&,name$)
	font uid&,$0
	print "Font is "+name$
	rem get
	print "This is plain text"
	rem get
	font uid&,$1
	print "This is bold text"
	rem get
	font uid&,$2
	print "This is underlined text"
	rem get
	cls
	font uid&,$4
	print "This is inverse text"
	rem get
	cls
	font uid&,$8
	print "This is double height text"
	rem get
	cls
	font uid&,$10
	print "This is mono text"
	rem get
	cls
	font uid&,$20
	print "This is italic text"
	rem get
	font uid&,$23
	print "This is bold, underlined, italic text"
	rem get
	font uid&,$5
	print "This is inverse, bold text"
	rem get
	font uid&,$18
	print "This is double height, mono text"
	rem get
	font uid&,$14
	print "This is mono inverse text"
	rem get
	cls
	font 10,16
endp


proc gprintstyle:(uid&,name$)
	x%=20 : y%=10
	font 10,16
	print "Font is "+name$
	rem get
	cls
	gfont uid&
	gprintstyleline:(0,"This is plain text")
	gprintstyleline:($1,"This is bold text")
	gprintstyleline:($2,"This is underlined text")
	gprintstyleline:($4,"This is inverse text")
	rem get
	gcls
	x%=20 : y%=10
	y%=y%+size%
	gprintstyleline:($8,"This is double height text")
	gprintstyleline:($10,"This is mono text")
	gprintstyleline:($20,"This is italic text")
	gprintstyleline:($23,"This is bold, underlined, italic text")
	rem get
	gcls
	x%=20 : y%=10
	gprintstyleline:($5,"This is inverse, bold text")
	y%=y%+size%
	gprintstyleline:($18,"This is double height, mono text")
	gprintstyleline:($14,"This is mono inverse text")
	rem get
	gcls
endp


proc gprintstyleline:(style%,line$)
	y%=y%+size%+10
	gat x%,y%
	gstyle style%
	gprint line$
endp


REM End of pFont.tpl

