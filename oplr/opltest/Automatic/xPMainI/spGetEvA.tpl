REM spGetEvA.tpl
REM EPOC OPL script for interactive pGetEvA.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL 

REM For modifier consts.
INCLUDE "System.oxh" 	

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNAL GetEvAScript%:
EXTERNAL EventScript%:
EXTERNAL PointerScript%:
EXTERNAL PointerPosScript%:
EXTERNAL TimingScript%:

EXTERNAL SpoofSanityCheck%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("spGetEvA", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP

CONST kTarget$="\Opltest\Interactive\pMainI\pGetEvA.opo"


PROC spGetEvA:
rem	hLogChangeThreshold%:(KhLogHigh%)
	hRunTest%:("sGetEvA01")
	hCleanUp%:("CleanUp")
ENDP


PROC sGetEvA01:
	LOCAL thread&
	thread&=hSpoofRunTargetApp&:("OPL", "", KSyRunAppOpl$+hDiskName$:+KTarget$,KSyRunAppRun%)
	IF thread&=0
		RAISE 100
	ENDIF

	hSpoofSetFlagTargetApp%:(KTarget$,KFalse%)

	EventScript%:
	PointerScript%:
	PointerPosScript%:
	TimingScript%:
rem	SpoofSanityCheck%:

	PAUSE KhCloseAppDelay%
	IF hSpoofGetFlagTargetApp%:(KTarget$) :RAISE 31313 :ENDIF
ENDP


PROC CleanUp:
	hSpoofCloseApp%:
ENDP


PROC EventScript%:
	rem Press any key to start.
	hSpace%: :hSendkey&:(KEvKeyUp&) 

	REM 'a'
	hSendKey&:(KEvKeyDown&) :hSendKeyRich&:(%a,&41,0,0) :hSendKey&:(KEvKeyUp&)

	REM 'h'
	hSendKey&:(KEvKeyDown&) :hSendKeyRich&:(%h,&48,0,0) :hSendKey&:(KEvKeyUp&)

	REM Shift+w
	hSendKey&:(KEvKeyDown&) :	hSendKey&:(KEvKeyDown&)
	hSendKeyRich&:(%W,&57,KSyModifierShift&,0)
	hSendKey&:(KEvKeyUp&) :	hSendKey&:(KEvKeyUp&)
		
	REM 6
	hSendKey&:(KEvKeyDown&) :hSendKeyRich&:(%6,&36,0,0) :hSendKey&:(KEvKeyUp&)

	REM Shift+!
	hSendKey&:(KEvKeyDown&) :	hSendKey&:(KEvKeyDown&)
	hSendKeyRich&:(%!,&31,KSyModifierShift&,0)
	hSendKey&:(KEvKeyUp&) :	hSendKey&:(KEvKeyUp&)

	REM Ctrl+k
	hSendKey&:(KEvKeyDown&) :	hSendKey&:(KEvKeyDown&)
	hSendKeyRich&:(11,&4B,KSyModifierCtrl&,0)
	hSendKey&:(KEvKeyUp&) :	hSendKey&:(KEvKeyUp&)
	
	REM Alt/Fn
	hSendKeyRich&:(KEvKeyDown&,0,KSyModifierFunc&,0) 
	hSendKeyRich&:(KEvKeyup&,0,0,0) 
	
	REM Esc
	hSendKey&:(KEvKeyDown&) :hSendKeyRich&:(27,&4,0,0) :hSendKey&:(KEvKeyUp&)

	REM Enter
	hSendKey&:(KEvKeyDown&) :hSendKeyRich&:(13,&3,0,0) :hSendKey&:(KEvKeyUp&)

	REM Tab
	hSendKey&:(KEvKeyDown&) :hSendKeyRich&:(9,&2,0,0) :hSendKey&:(KEvKeyUp&)

	REM Delete
	hSendKey&:(KEvKeyDown&) :hSendKeyRich&:(8,&1,0,0) :hSendKey&:(KEvKeyUp&)

	REM Shift
	hSendKey&:(KEvKeyDown&) :	hSendKey&:(KEvKeyUp&)
ENDP


PROC PointerScript%:
	REM Not implemented, so log it and carry on.
	hLog%:(KhLogHigh%,"spGetEvA: Skipping over pointer tests.")
	hEsc%: 
ENDP


PROC PointerPosScript%:
	REM Not implemented, so log it and carry on.
	hLog%:(KhLogHigh%,"spGetEvA: Skipping over pointerPos tests.")
	hEsc%: 
ENDP


PROC TimingScript%:
	rem Press any key to start.
	hSpace%: :hSendkey&:(KEvKeyUp&) 
	
	REM Press two keys, approx. 2 secs apart.
	hSendKey&:(KEvKeyDown&) :hSendKey&:(%a) :hSendKey&:(KEvKeyUp&)
	PAUSE 40 REM two seconds.
	hSendKey&:(KEvKeyDown&) :hSendKey&:(%b) :hSendKey&:(KEvKeyUp&)
	
	REM Press two keys, approx. 5 secs apart.
	hSendKey&:(KEvKeyDown&) :hSendKey&:(%c) :hSendKey&:(KEvKeyUp&)
	PAUSE 100 REM five seconds.
	hSendKey&:(KEvKeyDown&) :hSendKey&:(%d) :hSendKey&:(KEvKeyUp&)

	RETURN
		
	REM Hold pen(!) down for 5 secs.
	rem hSendKeyRich&:(KEvPtr&,KEvPtrPenDown&,0,0)
	rem PAUSE 100 REM Five secs.
	rem hSendKeyRich&:(KEvPtr&,KEvPtrPenUp&,0,0)
ENDP


PROC SpoofSanityCheck%:
	rem confirm it worked.
	hLog%:(KhLogHigh%,"tSpoofTest\SpoofSanityCheck%: running.")
	dinit "Some sanity check"
	dialog
ENDP


REM End of spGetEvA.tpl
