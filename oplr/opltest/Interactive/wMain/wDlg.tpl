REM wDlg.tpl
REM EPOC OPL interactive test code for dialogs.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
DECLARE EXTERNAL
EXTERNAL tTextDialog%:
EXTERNAL tTextDialog2%:
EXTERNAL tMixDialog%:
EXTERNAL tMixDialog2%:
EXTERNAL tButtonDialog%:
EXTERNAL tButtonDialog2%:
EXTERNAL InvalidChoice%:
EXTERNAL dialogDifferentProc:
EXTERNAL setDlg:
EXTERNAL setInit:
EXTERNAL setItems:
EXTERNAL doDlg:
EXTERNAL tTooWide%:
EXTERNAL tTooWide2%:


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wDlg", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wDlg:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowdlg")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
	rem hCleanUp%:("Reset")
ENDP


proc dowDlg:
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	REM Text dialog.
	PRINT "Text dialog." :PRINT "Choose first action -- Add new entry"
	IF tTextDialog%:<>2 :RAISE 1 :ENDIF

	PRINT :PRINT "Choose last action -- Update entry"
	IF tTextDialog%:<>6 :RAISE 2 :ENDIF

	REM Text dialog with choice labels.
	PRINT :PRINT "Choose second action -- B Copy entry"
	IF tTextDialog2%:<>3 :RAISE 3 :ENDIF

	REM Mixed dialog.
	PRINT :PRINT "Mixed dialog." :PRINT "Complete the dialog"
	tMixDialog%: REM Will raise errors if incorrect.

	PRINT :PRINT "Second mixed dialog." :PRINT "Complete the dialog"
	tMixDialog2%: REM Will raise errors if incorrect.

	REM Button dialog.
	PRINT :PRINT "Button dialog." :PRINT "Choose Down"
	IF tButtonDialog%:<>%d :RAISE 4 :ENDIF

	PRINT "Second button dialog." :PRINT "Choose second choice, then cancel the dialog"
	IF tButtonDialog2%:<>0 :RAISE 5 :ENDIF

	PRINT :PRINT "Width test." :PRINT "Hit Enter"
	IF tTooWide%:<>1 :RAISE 6 :ENDIF

	PRINT "Second width test." :PRINT "Hit Enter again"
	IF tTooWide2%:<>1 :RAISE 7 :ENDIF
	
	REM Error handling.
	REM Invalid choice
	invalidChoice%:
	dialogDifferentProc:
ENDP


PROC dialogDifferentProc:
	rem dialog called in different procedure to setting up
	rem cls
	rem print "Check handling dialog setup and run"
	rem print "     in different procedures"
	onerr setDlgOk
	setDlg:
	dialog
	onerr off
	raise 1
	setDlgOk::
	onerr off
	if err<>-85 :raise 2 :endif
	onerr setIniOk
	setInit:
	dialog
	onerr off
	raise 3

setIniOk::
	onerr off
	if err<>-85 : 	raise 3 : endif
	dInit "Items set lower"
	onerr setItmOk
	setItems:
	onerr off
	raise 4
	setItmOk::
	onerr off
	if err<>-85 : raise 5 : endif
	onerr dlgDnOk
	dInit "Dialog run in lower proc"
	dText "Should fail","Bug if reaches here"
	doDlg:
	onerr off
	raise 6
	dlgDnOk::
	onerr off
	if err<>-85
		raise 7
	endif
	rem print "Finished ok"
	rem pause -30 :key
ENDP


proc doDlg:
	dialog
endp


proc setItems:
	local d
	dFloat d,"dFloat",-1e99,9e99
endp


proc setInit:
	dInit "Setup lower"
endp


proc setDlg:
	local d
	d=1e99
	dInit "Setup lower"
	dFloat d,"dFloat",-1e99,9e99
endp


PROC tTextDialog%:
	dInit "Select action"
	dText "","Add new entry",$400
	dText "","Copy entry",$400
	dText "","Review entry",$400
	dText "","Delete entry",$400
	dText "","Update entry",$400
	return dialog
endp


PROC tTextDialog2%:
	dInit "Select action"
	dText "A","Add new entry",$400
	dText "B","Copy entry",$400
	dText "C","Review entry",$400
	dText "D","Delete entry",$400
	dText "E","Update entry",$400
	return dialog
ENDP


PROC tMixDialog%:
	local time&,date&,ed$(30),xin$(16)
	local ret%,l&,d
	local i%,j%
	local rep&
	
	j%=2
	time&=3600
	date&=days(2,1,1970)
	ed$="dEdit"
	dInit "Test dialog"
	dPosition 1,-1
	dText "dText","body,$400",$400
	dTime time&,"dTime 03:04am",0,59*60,int(22)*60*60+58*60
	dDate date&,"dDate 27/11/1993",days(1,1,1970),days(1,1,2001)
	dEdit ed$,"dEdit Mary had a little lamb",15
	dXInput xin$,"dXinput Secret"

	IF dialog=0 :RAISE 1 :ENDIF
	IF time&<>11040 :rep&=rep&+2 :ENDIF
	IF date&<>34298 :rep&=rep&+4 :ENDIF
	IF ed$<>"Mary had a little lamb" :rep&=rep&+8 :ENDIF
	IF xin$<>"Secret" :rep&=rep&+16 :ENDIF

	IF rep& :RAISE rep& :ENDIF
ENDP


PROC tMixDialog2%:
	local rep&
	local i%,l&,d,j%
	i%=0    :rem dChoice should set to 1
	dInit "Long and float editors"
	dText "dText","default"
	dLong l&,"dLong 314159265",&80000000,&7fffffff
	dFloat d,"dFloat 3.14159265",-9.99999999999999e99,9.99999999999999e99
	dChoice i%,"dChoice1 Choice 1","Choice 1,Choice 2,Choice 3,Choice 4"
	dChoice j%,"dChoice2 4xxx","10xxxxxxxx,9xxxxxxxx,8xxxxxxx,..."
	dChoice j%,"","7xxxxxx,6xxxxx,5xxxx,4xxx,3xx,2x,1"
	IF dialog=0 : RAISE 1 :ENDIF
	IF l&<>314159265 :rep&=rep&+2 :ENDIF
	IF d<>3.14159265 :rep&=rep&+4 :ENDIF
	IF i%<>1 :rep&=rep&+8 :ENDIF
	IF j%<>7 :rep&=rep&+16 :ENDIF
	IF rep& :RAISE rep& :ENDIF
ENDP


PROC tButtonDialog%:
	local n%
	dInit "Alert box"
	dButtons "Up",%U,"Down",%D,"Cancel",%c
	RETURN Dialog
ENDP


PROC tButtonDialog2%:
	local c%,n%
	rem for Opl32 negative values of keys are no longer 
	rem valid except for esc
	
  	dInit "Alert box Esc to Cancel"
	dPosition 1,1
  	dChoice c%,"Choose 2","1,2,3"
	dButtons "Cancel",-27
  	n%=Dialog
	if c%<>0
		RAISE 1
		rem print "PANIC!"
		rem print "Choice changed to ";c%;" when cancelled"
		rem get
	endif
	return n%
ENDP


PROC invalidChoice%:
	local ret%,k%,l%,m%,n%
	
	dInit	"Invalid dChoice"
	onerr e1
	dChoice k%,"dChoice3","m1,m2,,"
	onerr off
	raise 1
	e1::
	onerr off
	if err<>-2 : raise 2 : print err$(err) : raise 2 : endif
	
	dInit	"Invalid dChoice"
	onerr e2
	dChoice l%,"dChoice4",",m2"
	onerr off		
	raise 3
	e2::
	onerr off
	if err<>-2 : raise 4 : endif
	
	dInit	"Invalid dChoice"
	onerr e3
	dChoice m%,"dChoice5","m1,,m3"
	onerr off
	raise 5
	e3::
	onerr off 
	if err<>-2 : raise 6 : endif
	
	dInit	"Invalid dChoice"
	onerr e4
	dChoice n%,"dChoice6",",,"
	onerr off
	raise 6
	e4::
	onerr off
	if err<>-2 : raise 8 : endif
	
	rem print "Errors detected OK"
	rem pause 30
	return 1
ENDP


PROC tTooWide%:
	local d%
	dInit rept$("Title too wide",6)
	onerr e100
	d%=dialog
	onerr off
	rem print "Title too wide not detected (OK for Opler1)"
	rem get
	return d%
e100::
	onerr off
	raise 100
ENDP


PROC tTooWide2%:
	local d%
	dInit "Item too wide"
	onerr e101
	dText "",rept$("Item too wide",6)
	d%=dialog
	onerr off
	rem print "Item too wide not detected (OK for Opler1)"
	rem 	get
	return d%
e101::
	onerr off
	rem print "Error too wide detected ok"
	rem print "This is an error in Opler1"
	raise 101
ENDP


REM End of wDlg.tpl
