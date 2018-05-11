REM wDPos.tpl
REM EPOC OPL interactive test code for dPOSITION.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
DECLARE EXTERNAL
EXTERNAL jumpinDialogs%:(ax%,ay%)

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wdpos", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wdpos:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowDPos")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


proc dowdpos:
	jumpinDialogs%:(-1,-1)
	jumpinDialogs%:(0,-1)
	jumpinDialogs%:(1,-1)
	
	jumpinDialogs%:(-1,0)
	jumpinDialogs%:(0,0)
	jumpinDialogs%:(1,0)
	
	jumpinDialogs%:(-1,1)
	jumpinDialogs%:(0,1)
	jumpinDialogs%:(1,1)
endp


proc jumpinDialogs%:(ax%,ay%)
	local x%,y%
	x%=ax%+2 :y%=ay%+2
	dInit "Jumpin' dialogs"
	dPosition ax%,ay%
	dChoice x%,"Horizontal","left,centre,right"
	dChoice y%,"Vertical","top,centre,bottom"
	dButtons "Quit",%q,"Jump",13
	Dialog
endp


REM End of wDPos.tpl

