REM wDText.tpl
REM EPOC OPL interactive test code for wDText.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wDText", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wDText:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowDText")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


proc dowDText:
	global t255$(255),p255$(255),b255$(255)
	local d%,raise%
	
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	t255$=rept$("Title     ",25)+"12345"
	p255$=rept$("Prompt    ",25)+"12345"
	b255$=rept$("Body      ",25)+"12345"

	dInit "dText Alignment Test"
rem	dText "Prompt","Default flags"
rem	dText "Align","Left",0
rem	dText "Align","Right",1
rem	dText "Align","Centre",2
	dText "","Left",0
	dText "","Right",1
	dText "","Centre",2
	PRINT :PRINT "Hit Enter."
	d%=dialog
	if d% and d%<>1
		RAISE 1
		rem print "PANIC 0! Dialog returned",d%
		rem get
	endif

  rem busy "Enter quits"
  rem do 
  	dInit "dText Single Mask Test"
	  dText "Mask","Bold (ignored in Eikon)",$100
	  dText "Mask","Underline follows",$200
	  dText "Mask","Selectable",$400
	  PRINT :PRINT "Hit Enter."
	  d%=dialog
  rem until d%<>0
	rem  busy ""
	if d%<>4
		RAISE 2
		rem print "PANIC 1! Dialog returned",d%
		rem get
	endif
	
	dInit "dText Combined Masks Test 1"
	dText "Masks","Bold|Under",$200 OR $100
	dText "Masks","Bold|Select",$400 OR $100
	dText "Masks","Under|Select",$400 OR $200
	dText "Masks","Bold|Under|Select",$400 OR $200 OR $100
	dText "", "Underline above this"
	PRINT :PRINT "Hit Enter."
	d%=dialog
	rem	if d%=1 or d%=2
	if d%<>3
		RAISE 3
		rem print "PANIC 2! Dialog returned",d%
		rem get
	endif

	dInit "dText Combined Masks Test 2"
	dText "Masks","Bold|Under",$300
	dText "", "Underline above this"
	PRINT :PRINT "Hit Enter."
	If dialog<>1 : RAISE 4 :ENDIF 

	dInit "dText Combined Masks Test 3"
	dText "Masks","Under|Select",$600
	dText "", "Underline above this"
	PRINT :PRINT "Hit Enter."
	IF dialog<>2 : RAISE 5 :ENDIF 

	dInit "dText Combined Masks Test 4"
	dText "Masks","Bold|Under|Select",$700
	dText "", "Underline above this"
	PRINT :PRINT "Hit Enter."
	IF dialog<>2 : RAISE 6 :ENDIF 

	onerr errHand::
	rem print "Test dialog too wide"
	raise%=7
	dInit t255$
	PRINT :PRINT "Hit Enter."
	dialog
	onerr off
	rem print "wdtext\tDText : Title too wide not detected as an error"
	rem print "This is OK on Opler1"
	rem get

	onerr errHand::
	raise%=8
	PRINT :PRINT "Hit Enter."
	dInit "Prompt 255 long"
	dText p255$,"b"
	dialog
	onerr off
	rem print "wdtext\tDText : Prompt too wide not detected as an error"
	rem print "This is OK on Opler1"
	rem get

	onerr errHand::
	raise%=9
	PRINT :PRINT "Hit Enter."
	dInit "Body 255 long"
	dText "",b255$
	dialog
	onerr off
	rem print "wdtext\tDText : Body too wide not detected as an error"
	rem print "This is OK on Opler1"
	rem get

	onerr errHand::
	raise%=10
	PRINT :PRINT "Hit Enter."
	dInit "Prompt and body each 255"
	dText p255$,b255$
	dialog
	onerr off
	rem print "wdtext\tDText : Prompt and body too wide not detected as an error"
	rem print "This is OK on Opler1"
	rem get
	return

errHand::
	onerr off
	raise raise%
endp


REM End of wDText.tpl

