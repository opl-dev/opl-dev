REM pDialog.tpl
REM EPOC OPL interactive test code for dialogs.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"
DECLARE EXTERNAL
EXTERNAL 	tDlg:
EXTERNAL	tTooLong:
EXTERNAL	tButtonRight:
EXTERNAL	tButtonNoLabel:
EXTERNAL	tButtonPlainHotKey:
EXTERNAL	tDlgText:
EXTERNAL	tNoTitle:
EXTERNAL	tFullScr:
EXTERNAL	tNoDrag:
EXTERNAL	tDense:
EXTERNAL	tCombine:
EXTERNAL	tCheckbox:	
EXTERNAL	tMultiChoice:


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pdialog", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC pdialog:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("tdialog")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
rem	hCleanUp%:("Reset")
ENDP


PROC Reset:
	rem Any clean-up code here.
ENDP


PROC tdialog:
	tDlg:
ENDP


proc tDlg:
	print "Opler1 Dialog Tests"
	print
rem	hlog%:(khlogalways%,"ttoolong")
	tTooLong:
rem	hlog%:(khlogalways%,"tbuttonright")
	tButtonRight:
rem	hlog%:(khlogalways%,"tbuttonnolabel")
	tButtonNoLabel:
rem	hlog%:(khlogalways%,"tbuttonplainhotkey")
	tButtonPlainHotKey:
rem	hlog%:(khlogalways%,"tdlgtext")
	tDlgText:
rem	hlog%:(khlogalways%,"tnotitle")
	tNoTitle:
rem	hlog%:(khlogalways%,"tfullscr")
	tFullScr:
rem	hlog%:(khlogalways%,"tnodrag")
	tNoDrag:
rem	hlog%:(khlogalways%,"tdense")
	tDense:
rem	hlog%:(khlogalways%,"tcombine")
	tCombine:
rem	hlog%:(khlogalways%,"tMultichoice")
	tMultiChoice:
rem	hlog%:(khlogalways%,"tcheckbox")
	tCheckbox:	
	print "Opler1 Dialog Tests Finished OK"
	rem pause 30 
endp


proc tTooLong:
	rem tests normal dialogs which are too long 
		
	dInit "This should have buttons at the bottom"
	dText "This line must be long enough to cover the centre of the screen","1"
	dText "This line must be long enough to cover the centre of the screen","2"
	dText "This line must be long enough to cover the centre of the screen","3"
	dText "This line must be long enough to cover the centre of the screen","4"
	dText "This line must be long enough to cover the centre of the screen","5"
	dText "This line must be long enough to cover the centre of the screen","6"
	dText "This line must be long enough to cover the centre of the screen","7"
	dText "This line must be long enough to cover the centre of the screen","8"
	dText "This line must be long enough to cover the centre of the screen","9"
	dButtons "Visible",13,"Invisible",-27
	dialog
	
	dInit "This should have buttons at the bottom"
	dText "This line must be long enough to cover the centre of the screen","1"
	dText "This line must be long enough to cover the centre of the screen","2"
	dText "This line must be long enough to cover the centre of the screen","3"
	dText "This line must be long enough to cover the centre of the screen","4"
	dText "This line must be long enough to cover the centre of the screen","5"
	dText "This line must be long enough to cover the centre of the screen","6"	
	dButtons "Visible",13,"Invisible",-27
	dialog
endp


proc tButtonRight:
	dInit "This dialog has buttons on the right",$1
	dText "They are normally at the bottom",""
	dButtons "Right",13,"Wrong",-27
	dialog
	
	dInit "This dialog has buttons on the right but is too wide too wide too wide to fit on the screen",$1
	dButtons "Right",13,"Wrong",-27
	dialog
endp


proc tButtonNoLabel:
	dInit "Dialog without labels"
	dText "Do the buttons have labels?",""
	dButtons "Yes",%y or $100,"No",%n or $100,"Cancel",-27
	dialog
	
	dInit "Dialog with buttons on right, with and without labels",$1
	dButtons "Label",13,"No Label",-27
	dialog

	dInit "Dialog with incorrect labellling"
	dText "Cancel should have no label, but is wrongly negated",""
	dButtons "Label",13,"No Label",%n or $100,"Cancel",-$1b or $100
	dialog
endp


proc tButtonPlainHotKey:
	dInit "Dialog wit plain hotkeys"
	dText "Do the buttons have plain hot keys?",""
	dButtons "Yes",13 or $200,"No",-(27 or $200)
	dialog
	
	dInit "Dialog with buttons on right, with plain hot keys",$1
	dButtons "Yes",13 or $200,"No",-(27 or $200)
	dialog

	dInit "Dialog with no labels and plain hot keys"
	dButtons "Yes",%y or $300,"No",-(27 or $300)
	dialog
endp


proc tdlgtext:
	rem test text settings in dialogs
	
	dInit "dText Flags used with empty prompt",$10
	dText "","This text is left aligned",0
	dText "","This text is right aligned",1
	dText "","This text is centred",2
	dText "","This text is not bold",$100
	dText "","This text has a line below it",$200
	dText "","This text may not be selected",$400
	dText "","The line below is a separator"
	dText "","",$800
	dText "","The line above is a separator"
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	
	dInit "dText Flags used with empty body",$10
	dText "This text is not left aligned","",0
	dText "This text is not right aligned","",1
	dText "This text is not centred","",2
	dText "This text not is bold","",$100
	dText "This text has a line below it","",$200
	dText "This text may be selected","",$400
	dText "The line below is a separator",""
	dText "","",$800 
	dText "The line above is a separator",""
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	
	dInit "dText Flags used with prompt and body",$10
	dText "1.","This text is not left aligned",0
	dText "2.","This text is not right aligned",1
	dText "3.","This text is not centred",2
	dText "4.","This text not is bold",$100
	dText "5.","This text has a line below it",$200
	dText "This text may be selected","This can't",$400
	dText "7.","The line below is a separator"
	dText "","",$800 
	dText "9.","The line above is a separator"
	dBUTTONS "OK",13,"Cancel",-27
	dialog
endp


proc tNoTitle:
	local d1$(20)
	rem untitled dialogs
	
	dInit "This title should NOT appear",$2
	dText "This dialog","should be untitled"
	dBUTTONS "OK",13,"Cancel",-27
	dialog

	dInit "Nor should this",$2
	dEdit d1$,"No Title"
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	if d1$<>"Sunday" : raise 10 :endif
	dInit "",$2
	dText "This dialog should NOT be titled!",""
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	dInit ""
	dText "This dialog has an empty title",""
	dBUTTONS "OK",13,"Cancel",-27
	dialog
endp	


proc tFullScr:
	local d1$(20),d2%
	rem full screen dialogs
	
	dInit "This is a full screen dialog",$4
	dText "So there is lots of room!",""
	dEdit d1$,"Edit Box:",10
	dChoice d2%,"Choice:","Choice1,Choice2,Choice3"
	dBUTTONS "OK",13,"Cancel",-27
	dialog

	dInit "This is another, which is positioned bottom right!",$4
	dPosition 1,1
	dText "The position should not affect this dialog",""
	dBUTTONS "OK",13,"Cancel",-27
	dialog

	dInit "This is another, which is positioned top left!",$4
	dPosition -1,-1
	dText "The position should not affect this dialog",""
	dBUTTONS "OK",13,"Cancel",-27
	dialog

	dInit "This one is too long and will run off the bottom of the screen",$4
	dText "Line number","1"
	dText "Line number","2"
	dText "Line number","3"
	dText "Line number","4"
	dText "Line number","5"
	dText "Line number","6"
	dText "Line number","7"
	dText "Line number","8"
	dText "Line number","9"
	dText "Line number","10"	
	dText "Line number","11"
	dText "Line number","12"
	dBUTTONS "OK",13,"Cancel",-27
	dialog

	dInit "This should have buttons at the bottom",$4
	dText "This line must be long enough to cover the centre of the screen","1"
	dText "This line must be long enough to cover the centre of the screen","2"
	dText "This line must be long enough to cover the centre of the screen","3"
	dText "This line must be long enough to cover the centre of the screen","4"
	dText "This line must be long enough to cover the centre of the screen","5"
	dText "This line must be long enough to cover the centre of the screen","6"
	dText "This line must be long enough to cover the centre of the screen","7"
	dText "This line must be long enough to cover the centre of the screen","8"
	dText "This line must be long enough to cover the centre of the screen","9"
	dButtons "Visible",13,"Invisible",-27
	dialog
endp


proc tNoDrag:
	local d1$(20),d2$(20)
	rem dialogs with no drag
	
	dInit "This dialog has dragging"
	dText "To compare only!",""
	dText "Try dragging this dialog",""
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	
	dInit "No dragging in this dialog",$8
	dText "Now try dragging this dialog",""
	dBUTTONS "OK",13,"Cancel",-27
	dialog
endp


proc tDense:
	rem dense packed dialogs
	
	dInit "This is a normal dialog"
	dText "Line number","1"
	dText "Line number","2"
	dText "Line number","3"
	dButtons "Continue",13
	dialog

	dInit "This is a dense packed, but short dialog",$10
	dText "Line number","1"
	dText "Line number","2"
	dText "Line number","3"
	dButtons "Continue",13
	dialog
	
	dInit "This is not dense packed"
	dText "Line number","1"
	dText "Line number","2"
	dText "Line number","3"
	dText "Line number","4"
	dText "Line number","5"
	dText "Line number","6"
	dText "Line number","7"
	dButtons "Continue",13
	dialog
	
	dInit "This IS dense packed",$10
	dText "Line number","1"
	dText "Line number","2"
	dText "Line number","3"
	dText "Line number","4"
	dText "Line number","5"
	dText "Line number","6"
	dText "Line number","7"
	dButtons "Continue",13
	dialog
endp


proc tCombine:
	rem test combination of types
	
	dInit "This should not appear",$7
	dText "This dialog has no title",""
	dText "it is full-screen",""
	dText "and it has right buttons",""
	dButtons "Right",13,"Wrong",-27
	dialog

	dInit "This should not appear",$b
	dText "This dialog has no title",""
	dText "cannot be dragged",""
	dText "and it has right buttons",""
	dButtons "Right",13,"Wrong",-27
	dialog

	dInit "This should not appear",$2
	dText "This dialog has no title",""
	dText "so it cannot be dragged",""
	dButtons "Right",13,"Wrong",-27
	dialog	
endp


proc tcheckbox:
	local chk1%,chk2%,chk3%,chk4%,chk5%
	rem test dialog checkboxes
	hLog%:(KhLogAlways%,"ERROR !!TODO Skipping checkboxes due to defect EDNRANS-4KGDS7")
	Raise 1000

	chk1%=1 : chk2%=0 : chk3%=32767
	chk4%=-32768 : chk5%=-1
	print "Test that when reentering the checkbox, preferences are remembered"
	do
		dInit "Test dialog checkboxes",$1
rem dCHECKBOX removed from the runtime (before release of Crystal?)
rem So this code won't translate without errors.
rem		dCheckbox chk1%,"This one is set initially"
rem		dCheckbox chk2%,"This is not set"
rem		dCheckbox chk3%,"This is set"
rem		dCheckbox chk4%,"So is this"
rem		dCheckbox chk5%,"And this"
		dButtons "Check",13,"Done",-27
	until dialog=0
	cls
	print "Test dialog with checkboxes off screen"
	do
		dInit "Test very very very wide dialog with checkboxes off the screen"
rem		dCheckbox chk1%,"This checkbox has the symbol set initially but the prompt is so long that it is invisible"
rem		dCheckbox chk2%,"This is not set"
rem		dCheckbox chk3%,"This is set"
rem		dCheckbox chk4%,"So is this"
rem		dCheckbox chk5%,"And this"
		dButtons "Check",13,"Done",-27
	until dialog=0
endp


proc tMultiChoice:
	local ch1%,ch2%,ch3%,ch4%
	
	cls
	print "Test dialog with choice list of more than three"	
	do 
		dinit "Test dChoice with >3 choices"
		dchoice ch1%,"There are four choices","Choice1,Choice2,Choice3,..."
		dchoice ch1%,"","Choice4"
		dchoice ch2%,"There are five choices","Choice1,Choice2,Choice3,..."
		dchoice ch2%,"","Choice4,Choice5"
		dchoice ch3%,"There are six choices","Choice1,Choice2,..."
		dchoice ch3%,"","Choice3,Choice4,Choice5,..."
		dchoice ch3%,"","Choice6"
		dchoice ch4%,"There are twenty choices","Choice1,Choice2,Choice3,..."
		dchoice ch4%,"","Choice4,Choice5,Choice6,..."
		dchoice ch4%,"","Choice7,Choice8,Choice9,..."
		dchoice ch4%,"","Choice10,Choice11,Choice12,..."
		dchoice ch4%,"","Choice13,Choice14,Choice15,..."
		dchoice ch4%,"","Choice16,Choice17,Choice18,..."
		dchoice ch4%,"","Choice19,Choice20"				
		dBUTTONS "OK",13,"Cancel",-27
	until dialog=0
	
	print "Testing dChoice errors"
	
	dinit "Different prompts"
	dchoice ch1%,"There are four choices","Choice1,Choice2,Choice3,..."
	dchoice ch1%,"This is another prompt","Choice4"
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	
	dinit "Missing ,..."
	dchoice ch1%,"There are four choices","Choice1,Choice2,Choice3"
	dchoice ch1%,"","Choice4"
	dBUTTONS "OK",13,"Cancel",-27
	dialog

	print ",... after last choice gives structure fault"
	onerr sfault
	dinit "This is a fault"
	dchoice ch1%,"There are four choices","Choice1,Choice2,Choice3,..."
	dchoice ch1%,"","Choice4,..."
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	onerr off
	raise 1
sfault::
	onerr off
	if err<>-85 : raise 2 : endif
	
	print "Using different variables gives invalid argument error"
	onerr inval
	dinit "This is invalid"
	dchoice ch1%,"There are four choices","Choice1,Choice2,Choice3,..."
	dchoice ch2%,"","Choice4"
	dBUTTONS "OK",13,"Cancel",-27
	dialog
	onerr off
	raise 3
inval::
	onerr off
	if err<>-2 : raise 4 : endif

	print "OK"
	print
	rem pause 30
endp


rem end of pDialog.tpl
