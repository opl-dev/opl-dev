REM wDChoice.tpl
REM EPOC OPL interactive test code for dCHOICE.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wDChoice", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wDChoice:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowDChoice")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


proc dowDChoice:
	tDChoice:
	TCancel:
endp


proc tDChoice:
	global p255$(255),list1$(255),list2$(255)
	local d%,c1%,c2%,c3%,c4%,c5%,c6%,c7%,c8%,i%
	local rep&

	p255$=rept$("Prompt    ",25)+"12345"
	
	list2$="A"+rept$(",X",126)+",Z"
	rem print list2$
	rem print "is the 4th list passed !!!"
	rem get
	rem cls
	list1$=chr$(0)
	i%=1
	do
		list1$=list1$+","
		if i%= %,
			list1$=list1$+"X"
		else
			list1$=list1$+chr$(i%)
		endif
		i%=i%+1
	until len(list1$)>253
	rem print list1$
	rem print "is the 5th list passed !!!"
	rem get
	rem cls
	c1%=0
	c2%=3
	c3%=10
	c4%=128
	c5%=%Z+1
	c6%=1
	rem do
		dInit "dChoice Test" REM : [ESC] for next"
		dChoice c1%,"Normal list of 3: initially 0 (Choice 2)", "Choice 1,Choice 2,Choice 3"
		dChoice c2%,"Normal list of 3: initially 3 (Choice 2)", "Choice 1,Choice 2,Choice 3"
		dChoice c3%,"Normal list of 3: initially 10 (Choice 2)","Choice 1,Choice 2,Choice 3"
		dChoice c4%,"128 normal characters:init Z (X)",list2$
		dChoice c5%,"128 0 to 128 ascii:init Z (X)",list1$
		dChoice c6%,"CHR$(0)",chr$(0)
	rem	d%=dialog
	rem until d%=0
	IF DIALOG=0 :RAISE 1 :ENDIF
	IF c1%<>2 :rep&=rep&+2 :ENDIF
	IF c2%<>2 :rep&=rep&+4 :ENDIF
	IF c3%<>2 :rep&=rep&+8 :ENDIF
	IF c4%<>127 :rep&=rep&+16 :ENDIF
	IF c5%<>%Y :rep&=rep&+32 :ENDIF
	IF c6%<>1 :rep&=rep&+64 :ENDIF
	IF rep& :RAISE rep& :ENDIF
	
	c1%=1
	c2%=1
	rem do
	PRINT : PRINT "Hit Esc."
		dInit
		dText "No title","[ESC] for next"
		dChoice c1%,"prompt","body"
		dChoice c2%,"","no prompt,choice 2"
	rem	d%=dialog
	rem	until d%=0
	REM Hit Esc, so dialog should be zero.
	IF dialog<>0 : RAISE 100 :ENDIF
	rem cls
	rem print "Test dChoice errors"
	rem print "Testing no body causes error"
	c1%=1
	dInit
	onerr e1
	dChoice c1%,"no body",""
	dialog
	onerr off
	rem print "wdchoice\tdchoice : Error! No Body should not work"
	RAISE 101
e1::
	onerr off
	rem print "Testing prompt too long is truncated"
	c1%=1
	dInit
	onerr e2
	dChoice c1%,p255$,"Body"
	dialog
	onerr off
	rem print "wdchoice\tdchoice : Error! Prompt too long not truncated"
	rem print "This is OK on Opler1"
	rem get
	return

	e2::
	onerr off
	RAISE 102
endp



proc tCancel:
	local n%,c%
	
	rem cls
	rem print "Test that Esc cancels changes"
	rem print "Initial choice passed = 0"
	rem print "Hit escape to check unchanged"
	rem print
	IF c%<>0 :RAISE 200 :ENDIF
	dInit "[ESC] should not changes live variables"
	dPosition 1,1
	rem print "c=";c%
	IF c%<>0 :RAISE 201 :ENDIF
	dChoice c%,"Initially 0","1,2,3"
	IF c%<>0 :RAISE 202 :ENDIF
	rem print "c=";c%
	IF c%<>0 :RAISE 203 :ENDIF
	dialog
	IF c%<>0 :RAISE 204 :ENDIF
	rem print "c=";c%
	if c%<>0
		rem print "PANIC!"
		rem print "Choice changed to ";c%;" when cancelled"
		rem get
	endif
endp


REM End of wDChoice.tpl

