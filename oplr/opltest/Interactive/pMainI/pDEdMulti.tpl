REM pDEdMulti.tpl
REM EPOC OPL interactive test code for dEDITMULTI.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"
DECLARE EXTERNAL
EXTERNAL allocdlg:
EXTERNAL arraydlg:
EXTERNAL wrongparams:
EXTERNAL printinput:(ad&,a&)

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pDEdMulti", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pDEdMulti:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("tdeditmulti")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
rem	hCleanUp%:("Reset")
ENDP


PROC Reset:
	rem Any clean-up code here.
ENDP


proc tdeditmulti:
	print "Opler1 dEDITMULT Test"
	print
	rem pause 20

	allocdlg:
	arraydlg:
	wrongparams:

	print "Opler1 dEDITMULT Test Finished OK"
	rem pause 20
endp


proc allocdlg:
	local p&,len&,d%,a%,i&,i%,r%
	
	print "Using allocated cells"

	p&=alloc(208)
	if p&=0 :raise -10 :endif

	pokel p&,0
	dinit "Multi-line editors alloc cell"
	deditmulti p&,"3 lines, 100 chars",11,3,100
	dbuttons "Done",13,"Cancel",-27
	dialog

	dinit "Multi-line editor using alloc cell"
	dtext "","Type in the alphabet in lower case"
	deditmulti p&,"6 lines, 100 chars",12,6,100
	dbuttons "Done",13,"Cancel",-27
	d%=dialog
	
	rem check alphabet correct
	if d%=13
		a%=%a
		i&=p&+8
		r%=1
		while a%<=%z
			REM Possible raises 1-26
			if peekw(i&)<>a% : raise r% : endif
			a%=a%+1 : r%=r%+1
			i&=i&+2
		endwh
		print "Correct"
	else
		print "Cancelled"
		raise 210
	endif
	cls
	
	freealloc(p&)
	p&=alloc(208)
	if p&=0 : raise -10 : endif
	pokel p&,0

	dinit "Multi-line editor using alloc cell"
	dtext "","Type in the alphabet in UPPER case"
	deditmulti p&,"6 lines, 100 chars",12,6,100
	dbuttons "Done",13,"Cancel",-27
	d%=dialog
	
	rem check alphabet correct
	if d%=13
		a%=%A
		i&=p&+8
		r%=50
		while a%<=%Z
			REM Possible raises 50-75
			if peekw(i&)<>a% : raise r% : endif
			a%=a%+1
			i&=i&+2
			r%=r%+1
		endwh
		print "Correct"
	else
		print "Cancelled"
		raise 220
	endif
	rem pause 10
	cls

	dinit "Multi-line editor using same cell"
	deditmulti p&,"2 lines, 100 chars",12,2,100
	dbuttons "Done",13,"Cancel",-27
	dialog

	print "Setting buffer to 0123456789,.;'/#"
	i&=p&+8
	a%=%0
	while a%<=%9
		pokew i&,a% :i&=i&+2 :a%=a%+1
	endwh
	a%=%, :pokew i&,a% :i&=i&+2
	a%=%. :pokew i&,a% :i&=i&+2
	a%=%; :pokew i&,a% :i&=i&+2
	a%=%' :pokew i&,a% :i&=i&+2
	a%=%/ :pokew i&,a% :i&=i&+2
	a%=%# :pokew i&,a% :i&=i&+2
	pokel p&,16
	
	dinit "Multi-line editor using set buffer"
	deditmulti p&,"2 lines, 100 chars",12,2,100
	dbuttons "Done",13,"Cancel",-27
	dialog
	
	pokel p&,10
	i&=p&+8
	a%=%0
	while a%<=%9
		pokew i&,a%
		i&=i&+2
		a%=a%+1
	endwh
	cls
	
	dinit "Multi-line editor using set buffer"
	dtext "","Cancel this dialog to check unchanged!"
	deditmulti p&,"2 lines, 100 chars",12,2,100
	dbuttons "Cancel",-27
	dialog

	if peekl(p&)<>10 : raise 100 : endif
	i&=p&+8
	a%=%0
	r%=101
	while a%<=%9
		REM Possible raises 101-126
		if a%<>peekw(i&) : raise r% : endif
		i&=i&+2
		a%=a%+1
		r%=r%+1
	endwh
	print "OK"
	rem pause 10
	
	cls
	freealloc(p&)
	p&=alloc(16)
	if p&=0 : raise -10 : endif
	pokel p&,0
	
	dinit "Multi-line editor using short cell"
	deditmulti p&,"1 line, 4 chars",4,1,4
	dbuttons "Done",13,"Cancel",-27
	dialog
endp


proc arraydlg:
	local a&(52),b&(4),d%,a%,i&,i%,r%
	print "Using arrays"
	rem pause 10
	cls
	
	a&(1)=0
		
	dinit "Multi-line editor using array"
	dtext "","Type in the alphabet in lower case"
	deditmulti addr(a&()),"6 lines, 100 chars",12,6,100
	dbuttons "Done",13,"Cancel",-27
	d%=dialog
		
	rem check alphabet correct
	if d%=13
		a%=%a
		i&=addr(a&(3))
		
		r%=1
		while a%<=%z
			REM 1-26
			if peekw(i&)<>a%
				print HEX$(peekw(i&)) 
				raise r%
			endif
			a%=a%+1
			i&=i&+2
			r%=r%+1
		endwh
		print "Correct"
	else
		print "Cancelled"
		raise 310
	endif
	cls
	
	i%=1
	while i%<=51
		a&(i%)=0
		i%=i%+1
	endwh

	dinit "Multi-line editor using array"
	dtext "","Type in the alphabet in UPPER case"
	deditmulti addr(a&()),"6 lines, 100 chars",12,6,100
	dbuttons "Done",13,"Cancel",-27
	d%=dialog
	
	rem check alphabet correct
	if d%=13
		a%=%A
		i&=addr(a&(3))
		r%=50
		while a%<=%Z
			REM 50-75
			if peekw(i&)<>a% : raise r% : endif
			a%=a%+1
			i&=i&+2
			r%=r%+1
		endwh
		print "Correct"
	else
		print "Cancelled"
		raise 320
	endif
	rem pause 10
	cls

	dinit "Multi-line editor using same array"
	deditmulti addr(a&()),"2 lines, 100 chars",12,2,100
	dbuttons "Done",13,"Cancel",-27
	dialog
		
	print "Setting buffer to 0123456789,.;'/#"
	i&=addr(a&(3))
	a%=%0
	while a%<=%9
		pokew i&,a%
		i&=i&+2
		a%=a%+1
	endwh
	a%=%, :pokew i&,a% :i&=i&+2
	a%=%. :pokew i&,a% :i&=i&+2
	a%=%; :pokew i&,a% :i&=i&+2
	a%=%' :pokew i&,a% :i&=i&+2
	a%=%/ :pokew i&,a% :i&=i&+2
	a%=%# :pokew i&,a% :i&=i&+2
	a&(1)=16
	
	dinit "Multi-line editor using set buffer"
	deditmulti addr(a&()),"2 lines, 100 chars",12,2,100
	dbuttons "Done",13,"Cancel",-27
	dialog

	a&(1)=10
	i&=addr(a&(3))
	a%=%0
	while a%<=%9
		pokew i&,a%
		i&=i&+2
		a%=a%+1
	endwh
	
	dinit "Multi-line editor using set buffer"
	dtext "","Cancel this dialog to check unchanged!"
	deditmulti addr(a&()),"2 lines, 100 chars",12,2,100
	dbuttons "Cancel",-27
	dialog

	if a&(1)<>10 : raise 200 : endif
	i&=addr(a&(3))
	a%=%0
	r%=201
	while a%<=%9
		REM 201-210
		if a%<>peekw(i&) : raise r% : endif
		i&=i&+2
		a%=a%+1
		r%=r%+1
	endwh
	print "OK"
	rem pause 10
	
	cls
	b&(1)=0
	
	dinit "Multi-line editor using short array"
	deditmulti addr(b&()),"1 line, 4 chars",4,1,4
	dbuttons "Done",13,"Cancel",-27
	dialog
endp


proc wrongparams:
	local a&(20),b&(4),d%,ad&,i%,p&
	
	dinit "Multi-line editor with too much space"
	dtext "","Check that only 36 chars can be input"
	deditmulti addr(a&()),"5 lines, 8 chars",8,5,36
	dbuttons "Done",13,"Cancel",-27
	dialog
	
	dinit "Multi-line editor with too much space"
	dtext "","Check that only 4 chars can be input"
	deditmulti addr(b&()),"1 line, 10 chars",10,1,4
	dbuttons "Done",13,"Cancel",-27
	dialog
	
	dinit "Multi-line editor with buffer too long"
	dtext "","WARNING: Press Enter only to avoid memory corruption!"
	dtext "","100 chars may be input although buffer only 4!"
	deditmulti addr(a&()),"5 lines, 8 chars",8,5,100
	dbuttons "Done",13,"Cancel",-27
	dialog

	dinit "Multi-line editor with empty prompt"
	dtext "","36 char buffer"
	deditmulti addr(a&()),"",9,4,36
	dbuttons "Done",13,"Cancel",-27
	dialog
	
	i%=1
	while i%<=10
		a&(i%)=0
		i%=i%+1
	endwh

	dinit "Multi-line editor 0x0 edit box"
	deditmulti addr(a&()),"36 char buffer",0,0,36
	dbuttons "Done",13,"Cancel",-27
	d%=dialog
	
	if d%=%d
		ad&=addr(a&(2))
		printinput:(ad&,&24)
		rem get
	else 
		print "Cancelled"
		rem pause 10
	endif
	cls
	
	p&=alloc(80)
	if p&=0 : raise -10 : endif
	pokel p&,0
	
	dinit "Multi-line editor using with too much space for cell"
	dtext "","Check that only 36 chars can be input"
	deditmulti p&,"5 lines, 8 chars",8,5,36
	dbuttons "Done",13,"Cancel",-27
	dialog
	
	freealloc(p&)
	p&=alloc(16)
	pokel p&,0

	dinit "Multi-line editor using with too much space for cell"
	dtext "","Check that only 4 chars can be input"
	deditmulti p&,"1 line, 10 chars",10,1,4
	dbuttons "Done",13,"Cancel",-27
	dialog
		
	freealloc(p&)
	p&=alloc(80)
	pokel p&,0

	dinit "Multi-line editor 0x0 edit box"
	deditmulti p&,"36 char buffer",0,0,36
	dbuttons "Done",13,"Cancel",-27
	d%=dialog
	
	if d%=13
		ad&=uadd(p&,4)
		rem printinput:(ad&,&24)
		rem get
	else 
		print "Cancelled"
		rem pause 10
		raise 10
	endif
	cls
endp


proc printinput:(ad&,len&)
	local p%,l&,i&
	
	print "You typed..."
	l&=len&+ad&
	i&=ad&
	while i&<=l&
		p%=peekb(i&)
		print chr$(p%);
		i&=uadd(i&,1)
	endwh	
	print
endp

REM End of pDEdMulti.tpl
