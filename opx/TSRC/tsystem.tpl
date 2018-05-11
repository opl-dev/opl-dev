rem TSYSTEM.OPL
rem
rem Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
rem

include "system.oxh"
include "const.oph"

declare external
external tBackLightOn: 
external tSetBackLightOn:
external tSetBackLightOnTime:
external tSetBacklightBehavior:
external tIsBacklightPresent:
external tSetAutoSwitchOffBehavior:
external tSetAutoSwitchOffTime:
external tResetAutoSwitchOffTimer:
external tSwitchOff:
external tSetSoundEnabled:
external tSetSoundDriverEnabled:
external tSetKeyClickEnabled:
external tSetPointerClickEnabled:
external tSetDisplayContrast:
external tMaxDisplayContrast:

external CreateTestFile:	
external tIsReadOnly:
external tIsHidden:
external tISystem:
external tSetReadOnly:
external tSetHiddenFile:
external tSetSystemFile:
external tVolumeSize:
external tVolumeSpaceFree:
external tVolumeUniqueID:
external tMediaType:
external DeleteTestFile:

external tDisplayTaskList:
external tSetComputeMode:
external tRunApp:
external tRunExe:
external tLogonToThread:
external tTerminateCurrentProcess:
external tTerminateProcess:
external tKillCurrentProcess:
external tKillProcess:

external tMod:
external tXOR:
external tLoadRsc:
external tUnLoadRsc:
external tReadRsc:
external tReadRscLong:
external tCheckUid:
external tSetPointerGrabOn:
external tMachineName:
external tEndTask:
external tKillTask:
external tGetThreadIdFromOpenDoc:
external tGetThreadIdFromAppUid:
external tSetForeground:
external tSetBackground:
external tSetForegroundByThread:
external tSetBackgroundByThread:
external tGetNextWindowGroupName:
external tGetNextWindowId:
external tSendKeyEventToApp:
external tIrDAConnectToSend:
external tIrDAConnectToReceive:
external tIrDAWrite:
external tIrDARead$:
external tIrDAReadA:
external tIrDAWaitForDisconnect:
external tIrDADisconnect:
external tMainBatteryStatus:
external tBackupBatteryStatus:
external tCaptureKey:
external tCancelCaptureKey:
external tSetPointerCapture:
external tClaimPointerGrab:
external tOpenFileDialog:
external tCreateFileDialog:
external tSaveAsFileDialog:
external tIsExternalPowerPresent:

external tGetThreadIdFromCaption:
external tOsVersionMajor:
external tOsVersionMinor:
external tRomVersionMajor:
external tRomVersionMinor:
external tRomVersionBuild:
external tGetFileSize:
external tUniqueFilename:
external tIsPathVisible:
external tKeyClickLoud:
external tKeyClickOverridden:
external tSoundDriverEnabled:

external tBacklightTimer:
external tScreenTimer:
external tPasswordTimer:
external tSaveTimer:

external tGenericTimer:(aTimerId&)

external tRunExeCmd:
external tRunDoc:
external tOPXVersion:
external tLinkEnableWithOptions&:
external _ShowLinkConfig:

const KTestFile$="C:\TSystem.Test"

const KEmptyOwner&=6
const KBlankOwner&=2941
const KStandardDir$="c:\Documents\"
const KBookmark$="c:\Documents\"

proc main:
	global GRunAppRq&
	global GRunExeRq&
	global GTermCurrProcRq&
	global GKillCurrProcRq&

	print "Test the System.OPX"
	print "-------------------"
	print
	print "Note that this code makes assumptions about what machine the code is running on."
	print "For example it's a UK machine."
	print
	print "This may result in false failures on other machines."
	print

	tBacklightTimer:
	tScreenTimer:
	tPasswordTimer:
	tSaveTimer:

	tSetBackLightOn:
	tSetBackLightOnTime:
	tSetBacklightBehavior:
	tIsBacklightPresent:
	tSetAutoSwitchOffBehavior:
	tSetAutoSwitchOffTime:
	tResetAutoSwitchOffTimer:
	tSetSoundEnabled:
	tSetSoundDriverEnabled:
	tSetKeyClickEnabled:
	tSetPointerClickEnabled:
	tSetDisplayContrast:
	tMaxDisplayContrast:

	CreateTestFile:	
	tIsReadOnly:
	tIsHidden:
	tISystem:
	tSetReadOnly:
	tSetHiddenFile:
	tSetSystemFile:
	tVolumeSize:
	tVolumeSpaceFree:
	tVolumeUniqueID:
	tMediaType:
	DeleteTestFile:

	tDisplayTaskList:
	tSetComputeMode:
	tRunApp:
	tRunExe:
	tLogonToThread:
	tTerminateCurrentProcess:
	tTerminateProcess:
	tKillCurrentProcess:
	tKillProcess:

	tMod:
	tXOR:
	tLoadRsc:
	tUnLoadRsc:
	tReadRsc:
	tReadRscLong:
	tCheckUid:
	tSetPointerGrabOn:
	tMachineName:
	tEndTask:
	tKillTask:
	tGetThreadIdFromOpenDoc:
	tGetThreadIdFromAppUid:
	tSetForeground:
	tSetBackground:
	tSetForegroundByThread:
	tSetBackgroundByThread:
	tGetNextWindowGroupName:
	tGetNextWindowId:
	tSendKeyEventToApp:
	tIrDAConnectToSend:
	tIrDAConnectToReceive:
	tIrDAWrite:
	tIrDARead$:
	tIrDAReadA:
	tIrDAWaitForDisconnect:
	tIrDADisconnect:
	tMainBatteryStatus:
	tBackupBatteryStatus:
	tCaptureKey:
	tCancelCaptureKey:
	tSetPointerCapture:
	tClaimPointerGrab:
	tOpenFileDialog:
	tCreateFileDialog:
	tSaveAsFileDialog:
	tIsExternalPowerPresent:

	tGetThreadIdFromCaption:
	tOsVersionMajor:
	tOsVersionMinor:
	tRomVersionMajor:
	tRomVersionMinor:
	tRomVersionBuild:
	tGetFileSize:
	tUniqueFilename:
	tIsPathVisible:
	tKeyClickLoud:
	tKeyClickOverridden:
	tSoundDriverEnabled:
	get
endp

PROC CreateTestFile:
	LOCAL h%
	LOCAL test$(30)
	test$ = "This is a test file" 
	IOOPEN(h%,KTestFile$,2)
	IOWRITE(h%,ADDR(test$)+1,LEN(test$))
	IOCLOSE(h%)
ENDP

PROC DeleteTestFile:
	trap delete KTestFile$
ENDP


PROC tBacklightTimer:
	tGenericTimer:(KSyTimerBacklightTimeout&)
ENDP

PROC tScreenTimer:
	tGenericTimer:(KSyTimerScreenTimeout&)
ENDP

PROC tPasswordTimer:
	tGenericTimer:(KSyTimerPasswordTimeout&)
ENDP

PROC tSaveTimer:
	tGenericTimer:(KSyTimerSaveTimeout&)
ENDP

PROC tGenericTimer:(aTimerId&)
	LOCAL oldDuration&, newDuration&,check&
	SyGetTimer:(aTimerId&,oldDuration&)
	newDuration&=1
	SySetTimer:(aTimerId&,newDuration&)
	SyGetTimer:(aTimerId&,check&)
	if newDuration&<>check&
		RAISE 1
	endif
	newDuration&=100
	SySetTimer:(aTimerId&,newDuration&)
	SyGetTimer:(aTimerId&,check&)
	if newDuration&<>check&
		RAISE 100
	endif
	SySetTimer:(aTimerId&,oldDuration&)
ENDP


PROC tBackLightOn:
	rem Returns either -1 or 0
	rem No leaves

	rem Any Machine
	LOCAL backlightPresent&,backlightState&
	SyGetHAL&:(KSyBacklight&,backlightPresent&)
	IF backlightPresent&=KSyBacklightNone&
		RETURN 0
	ENDIF

	REM Machine has backlight, so set it on.
	SySetHAL&:(KSyBacklightState&,KSyBacklightStateOn&)
	SyGetHAL&:(KSyBacklightState&,backlightState&)
	IF backlightState&<>KSyBacklightStateOn&
		RAISE 101
	ENDIF
ENDP

PROC tSetBackLightOn:
	rem No leaves
	SySetHAL&:(KSyBacklightState&,KSyBacklightStateOn&)
ENDP


PROC tSetBackLightOnTime:
	rem Valid parameters are any positive time
	LOCAL backlightPresent&,backlightState&

	rem Any machine
	SySetTimer:(KSyTimerBacklightTimeout&,KMaxLong&)
	SySetTimer:(KSyTimerBacklightTimeout&,60)
	SySetTimer:(KSyTimerBacklightTimeout&,0)
	onerr err120::
	SySetTimer:(KSyTimerBacklightTimeout&,-1)
	raise 120
err120::
	onerr off

	rem Machines which do have backlight

	SyGetHAL&:(KSyBacklight&,backlightPresent&)
	IF backlightPresent&=KSyBacklightSupported&
		SySetTimer:(KSyTimerBacklightTimeout&,1)

		SySetHAL&:(KSyBacklightState&,KSyBacklightStateOn&)
		SyGetHAL&:(KSyBacklightState&,backlightState&)
		IF backlightState&<>KSyBacklightStateOn&
			raise 121
		endif
	endif
ENDP


PROC tSetBacklightBehavior:
	rem Valid parameters 0 for off or any value for on
	rem No leaves

	rem Any machine
	local backlightPresent&,backlightState&

	rem Machines which do have backlight
	SyGetHAL&:(KSyBacklight&,backlightPresent&)
	IF backlightPresent&=KSyBacklightSupported&
		SySetTimer:(KSyTimerBacklightTimeout&,1)
		SySetHAL&:(KSyBacklightState&,KSyBacklightStateOn&)
		SyGetHAL&:(KSyBacklightState&,backlightState&)
		IF backlightState&<>KSyBacklightStateOn&
			raise 130
		endif
	endif
ENDP

PROC tIsBacklightPresent:
	rem Returns either -1 or 0
	rem No leaves

	local result&,backlightPresent&
	result&=SyGetHAL&:(KSyBacklight&,backlightPresent&)
	IF result&
		raise 140
	ENDIF
	IF backlightPresent&=KSyBacklightSupported& OR backlightPresent&=KSyBacklightNone&
		RETURN
	ENDIF
	RAISE 141
ENDP

PROC tSetAutoSwitchOffBehavior:
	rem Valid parameters are 0,1,2
	print "To Do: SySetAutoSwitchOffBehavior needs testing in interactive test"
	onerr err150::
	SySetTimer:(KSyTimerBacklightTimeout&,-1)
	raise 150
err150::
	onerr err151::
	SySetTimer:(KSyTimerBacklightTimeout&,3)
	raise 151
err151::
	onerr off
ENDP



PROC tSwitchOff:
	print "To Do: SySwithcOff needs testing in interactive test"
ENDP

PROC tSetSoundEnabled:
	rem Valid paramenters are 0 for off or any other value for on
	print "To Do: SySetSoundEnabled needs testing in interactive test"
	SySetSoundEnabled:(0)
	SySetSoundEnabled:(-1234)
	SySetSoundEnabled:(KMaxLong&)
ENDP

PROC tSetSoundDriverEnabled:
	rem Valid paramenters are 0 for off or any other value for on
	print "To Do: SySetSoundDriverEnabled needs testing in interactive test"
	SySetSoundDriverEnabled:(0)
	SySetSoundDriverEnabled:(-1234)
	SySetSoundDriverEnabled:(KMaxLong&)
ENDP

PROC tSetKeyClickEnabled:
	print "To Do: SySetKeyClickEnabled needs testing in interactive test"	
	rem Valid paramenters are 0 for off or any other value for on
	REM SySetKeyClickEnabled:(0)
	REM SySetKeyClickEnabled:(-1234)
	REM SySetKeyClickEnabled:(KMaxLong&)
ENDP

PROC tSetPointerClickEnabled:
	print "To Do: SySetPointerClickEnabled needs testing in interactive test"	
	rem Valid paramenters are 0 for off or any other value for on
	rem SySetPointerClickEnabled:(0)
	rem SySetPointerClickEnabled:(-1234)
	rem SySetPointerClickEnabled:(KMaxLong&)
ENDP

PROC tSetDisplayContrast:
	rem Valid paramenters are between 0 and max display contrast
	rem SySetDisplayContrast:(0)
	rem SySetDisplayContrast:(SyMaxDisplayContrast&:)
	rem onerr err230::
	rem SySetDisplayContrast:(-1)
	rem raise 230
err230::
	rem onerr err231::
	rem SySetDisplayContrast:(SyMaxDisplayContrast&:+1)
	rem raise 231
err231::
	onerr off
ENDP

PROC tMaxDisplayContrast:
	rem if SyMaxDisplayContrast&:<>100
	rem	print "SyMaxDisplayContrast returned:",SyMaxDisplayContrast&:
	rem	raise 240
	rem endif 
ENDP

PROC tIsReadOnly:
	SySetReadOnly:(KTestFile$,KTrue%)
	if not SyIsReadOnly&:(KTestFile$)
		raise 250
	endif
	SySetReadOnly:(KTestFile$,KFalse%)
	if SyIsReadOnly&:(KTestFile$)
		raise 251
	endif
	onerr err252::
	SyIsReadOnly&:("Non-existant file")
	raise 252
err252::
	onerr off
ENDP

PROC tIsHidden:
	SySetHiddenFile:(KTestFile$,KTrue%)
	if not SyIsHidden&:(KTestFile$)
		raise 260
	endif
	SySetHiddenFile:(KTestFile$,KFalse%)
	if SyIsHidden&:(KTestFile$)
		raise 261
	endif
	onerr err262::
	SyIsHidden&:("Non-existant file")
	raise 262
err262::
	onerr off
ENDP

PROC tISystem:
	SySetSystemFile:(KTestFile$,KTrue%)
	if not SyIsSystem&:(KTestFile$)
		raise 270
	endif
	SySetSystemFile:(KTestFile$,KFalse%)
	if SyIsSystem&:(KTestFile$)
		raise 271
	endif
	onerr err272::
	SyIsSystem&:("Non-existant file")
	raise 272
err272::
	onerr off
ENDP

PROC tSetReadOnly:
	onerr err280::
	SySetReadOnly:("Non-existant file",0)
	raise 280
err280::
	onerr off
ENDP

PROC tSetHiddenFile:
	onerr err290::
	SySetHiddenFile:("Non-existant file",0)
	raise 290
err290::
	onerr off
ENDP

PROC tSetSystemFile:
	onerr err300::
	SySetSystemFile:("Non-existant file",0)
	raise 300
err300::
	onerr off
ENDP

PROC tVolumeSize:
	local d%
	local s&
	while d%<=25
		if d%=0 or d%=1 or d%>2 and d%<=24
			onerr errVolumeSize::
		endif
		s&=SyVolumeSize&:(d%)
		if d%=3 or d%=25
			if s&<1000 or s&>&10000000
				raise 310
			endif
		elseif d%=4
			if s&>&10000000
				raise 311
			endif
		else
			if s&>&10000000
				raise 312
			endif
		endif
errVolumeSize::
		onerr off
		d%=d%+1
	endwh
ENDP

PROC tVolumeSpaceFree:
	local d%
	local s&
	while d%<=25
		if d%=0 or d%=1 or d%>2 and d%<=24
			onerr errVolumeSpaceFree::
		endif
		s&=SyVolumeSpaceFree&:(d%)
		if d%=3 or d%=25
			if s&<1000 or s&>&10000000
				raise 320
			endif
		elseif d%=4
			if s&>&10000000
				raise 321
			endif
		else
			if s&>&10000000
				raise 322
			endif
		endif
errVolumeSpaceFree::
		d%=d%+1
	endwh
ENDP

PROC tVolumeUniqueID:
	local d%
	local s&
	while d%<=25
		if d%=0 or d%=1 or d%>2 and d%<=24
			onerr errVolumeUniqueId::
		endif
		s&=SyVolumeUniqueId&:(d%)
		if d%=3 or d%=25
			if s&<1000 or s&>&10000000
				raise 330
			endif
		elseif d%=4
			if s&>&10000000
				raise 331
			endif
		else
			if s&>&10000000
				raise 332
			endif
		endif
errVolumeUniqueId::
		d%=d%+1
	endwh
ENDP

PROC tMediaType:
	local d%
	local t&
	while d%<=25
		t&=SyMediaType&:(d%)
		if d%=2
			if t&<>5
				raise 340
			endif
		elseif d%=3
			if t&<>3
				raise 340
			endif
		elseif d%=25
			if t&<>7
				raise 341
			endif
		else
			if t&<>0
				raise 342
			endif
		endif
		d%=d%+1
	endwh
ENDP

PROC tDisplayTaskList:
	print "To Do: test Tasklist in interactive test"
ENDP

PROC tSetComputeMode:
	print "To Do: test SetComputeMode in interactive test"
	SySetComputeMode:(0)
	SySetComputeMode:(2)
	onerr err380::
	SySetComputeMode:(-1)
	raise 380
err380::
	onerr off
	onerr err381::
	SySetComputeMode:(3)
	raise 381
err381::
	onerr off
	SySetComputeMode:(1)
ENDP

PROC tRunApp:
	external GRunAppRq&
	local thread&
	local next&
	thread&=SyRunApp&:("OPL","","Rc:\opl\tsystem2.opo",2)
	pause 20
	if thread&<>SyThreadIdFromOpenDoc&:("c:\opl\tsystem2.opo",next&)
		raise 390
	endif
	SyLogonToThread:(thread&,GRunAppRq&)
	if GRunAppRq&<>KStatusPending32&
		print "Status",GRunAppRq&
		raise 391
	endif
	SyKillTask&:(thread&,0)
	if GRunAppRq&<>KStatusPending32&
		print "Status",GRunAppRq&
		raise 392
	endif
ENDP

PROC tRunExe:
	print "To Do: find an EXE to test RunEXE with"	
ENDP

PROC tLogonToThread:
	rem Tested by tRunApp and tTerminateCurrentProcess
ENDP

PROC tTerminateCurrentProcess:
	external GTermCurrProcRq&
	local thread&
	local next&
	thread&=SyRunApp&:("OPL","","Rc:\opl\tsystem3.opo",2)
	pause 20
	SySetForeground:
	SyLogonToThread:(thread&,GTermCurrProcRq&)
	if GTermCurrProcRq&<>KStatusPending32&
		print "Status",GTermCurrProcRq&
		raise 420
	endif
	pause 40
	print "TO DO: investigate why LogonToThread does not give the correct return result here."
rem	if rq&<>-1234
rem		print "Status",GTermCurrProcRq&
rem		raise 421
rem	endif
ENDP

PROC tTerminateProcess:
	print "To Do: find EXE to test Terminate Process"
ENDP

PROC tKillCurrentProcess:
	external GKillCurrProcRq&
	local thread&
	local next&
	thread&=SyRunApp&:("OPL","","Rc:\opl\tsystem3.opo",2)
	pause 20
	SySetForeground:
	SyLogonToThread:(thread&,GKillCurrProcRq&)
	if GKillCurrProcRq&<>KStatusPending32&
		print "Status",GKillCurrProcRq&
		raise 440
	endif
	pause 40
	print "TO DO: investigate why LogonToThread does not give the correct return result here."
rem	if rq&<>-1234
rem		print "Status",GKillCurrProcRq&
rem		raise 421
rem	endif
ENDP

PROC tKillProcess:
	print "To Do: find EXE to test Kill Process"
ENDP

PROC tMod:
	if SyMod&:(3,1) <> 0
		raise 460
	endif
	if SyMod&:(3,2) <> 1
		raise 461
	endif
ENDP

PROC tXOR:
	if SyXor&:(1,1) <> 0
		raise 470
	endif
	if SyXor&:(1,3) <> 2
		raise 471
	endif
ENDP

PROC tLoadRsc:
	local r&
	r&=SyLoadRsc&:("Z:\System\Data\OPLR.RSC")
	SyUnloadRsc:(r&)
	onerr err480::
	SyLoadRsc&:("DoesNotExist")
	raise 480
err480::
	onerr off
ENDP

PROC tUnLoadRsc:
	rem Partially tested in tLoadRsc
	print "To Do: make sure an unknown resouce leaves rather than panics (CONE 13)"
rem	SyUnloadRsc:(1234)
rem	raise 490
err480::
	onerr off
ENDP

PROC tReadRsc:
	local r&
	local res$(KMaxStringLen%)
	r&=SyLoadRsc&:("Z:\System\Data\OPLR.RSC")
	res$=SyReadRsc$:(&4b03300c)
	if res$<>"In use"
		print res$
		raise 500
	endif
	SyUnloadRsc:(r&)
ENDP

PROC tReadRscLong:
	print "To Do: find a long int resource" 
ENDP

PROC tCheckUid:
	local u$(16)
	u$=SyUidCheckSum$:(2364981,12089734,189275)
print "To Do: Make sure checkuid$ returns a reasonable value for unicode."
	if u$<>""
rem		print u$
rem		raise 520
	endif
ENDP

PROC tSetPointerGrabOn:
	print "To Do: Test pointer grab with interactive test"
ENDP

PROC tMachineName:
	LOCAL id
	rem SyMachineUniqueId:(BYREF high&,BYREF low&) 
	rem if SyMachineName$:<>"WINS Pc"
	rem		print SyMachineName$:
	rem		raise 540
	rem endif
ENDP


PROC tEndTask:
	print "To Do: SyEndTask responds to shutdown events - write a lemming opo to test"
ENDP

PROC tKillTask:
	print "To Do: SyKillTask - write a lemming opo to test"
ENDP

PROC tGetThreadIdFromOpenDoc:
	rem Tested in tRunApp
ENDP

PROC tGetThreadIdFromAppUid:
	print "To Do: test GetThreadIdFromAppUid"	
ENDP

PROC tSetForeground:
	print "To Do: test SetForeground in interactive tests"	
ENDP

PROC tSetBackground:
	print "To Do: test SetBackground in interactive tests"	
ENDP

PROC tSetForegroundByThread:
	print "To Do: test SetForegroundByThread in interactive test"	
ENDP

PROC tSetBackgroundByThread:
	print "To Do: test SetBackgroundByThread in interactive test"	
ENDP

PROC tGetNextWindowGroupName:
	print "To Do: test GetNextWindowGroupName: in interactive test"	
ENDP

PROC tGetNextWindowId:
	print "To Do: test GetNextWindowId in interactive test"	
ENDP

PROC tSendKeyEventToApp:
	print "To Do: write reciever for SendKeyEvenToApp"
ENDP

PROC tIrDAConnectToSend:
	print "To Do: test IrDaConnectToSend in interactive test"	
ENDP

PROC tIrDAConnectToReceive:
	print "To Do: test IrDAConnectToRecieve in interactive test"	
ENDP

PROC tIrDAWrite:
	print "To Do: test IrDAWrite in interactive test"	
ENDP

PROC tIrDARead$:
	print "To Do: test IrDARead in interactive test"	
ENDP

PROC tIrDAReadA:
	print "To Do: test IrDAReadA in interactive test"	
ENDP

PROC tIrDAWaitForDisconnect:
	print "To Do: test IrDAWaintForDisconnect in interactive test"	
ENDP

PROC tIrDADisconnect:
	print "To Do: test IrDADisconnect"	
ENDP

PROC tMainBatteryStatus:
	print "To Do: test MainBatteryStatus in interactive test"
rem	if SyMainBatteryStatus&:<>KBatteryGood&
rem		print SyMainBatteryStatus&:
rem		raise 740
rem	endif		
ENDP

PROC tBackupBatteryStatus:
	print "To Do: test BackupBatteryStatus in interactive test"
rem	if SyBackupBatteryStatus&:<>KBatteryGood&
rem		print SyBackupBatteryStatus&:
rem		raise 740
rem	endif		
ENDP

PROC tCaptureKey:
	print "To do: test tCaptureKey in interactive test"
ENDP

PROC tCancelCaptureKey:
	print "To do: test tCancelCaptureKey in interactive test"
ENDP

PROC tSetPointerCapture:
	print "To do: test tSetPointerCapture in interactive test"
ENDP

PROC tClaimPointerGrab:
	print "To do: test tClaimPointerGrab in interactive test"
ENDP

PROC tOpenFileDialog:
	print "To do: test tOpenFileDialog in interactive test"
ENDP

PROC tCreateFileDialog:
	print "To do: test tCreateFileDialog in interactive test"
ENDP

PROC tSaveAsFileDialog:
	print "To do: test tSaveAsFileDialog in interactive test"
ENDP

PROC tIsExternalPowerPresent:
	local p&
	print "To do: test tIsExternalPowerPresent in interactive test"
rem	p&=SyIsExternalPowerPresent&:
rem	if p&<-1 or p&>0
rem		raise 820
rem	endif
ENDP

proc tGetThreadIdFromCaption:
	local id&
	local prev&
	id&=SyThreadIdFromCaption&:("System",prev&)
	print "Shell ThreadId is:",id&
	if id&<=0
		raise 850
	endif
endp

proc tOsVersionMajor:
	local romver&
	romver&=SyROMVersionMajor&:
	print "OS major version: ",romver&
	if romver&<>1
		raise 890
	endif
endp

proc tOsVersionMinor:
	local romver&
	romver&=SyROMVersionMinor&:
	print "OS minor version: ",romver&
	if romver&<0 or romver&>5
		raise 900
	endif
endp

proc tRomVersionMajor:
	local romver&
	romver&=SyROMVersionMajor&:
	print "Rom major version: ",romver&
	if romver&<>1
		raise 910
	endif
endp

proc tRomVersionMinor:
	local romver&
	romver&=SyROMVersionMinor&:
	print "Rom minor version: ",romver&
	if romver&<0 or romver&>5
		raise 920
	endif
endp

proc tRomVersionBuild:
	local romver&
	romver&=SyROMVersionBuild&:
	print "Rom build version: ",romver&
	if romver&<>1 and (romver&<145 or romver&>500)
		raise 930
	endif
endp

proc tGetFileSize:
	local size&
	size&=SyFileSize&:("z:\system\opl\const.oph")
	if size&<15000 or size&>20000
		raise 940
	endif
endp

proc TUniqueFilename:
local h%
local a$(KMaxStringLen%)
local expectedFilename$(KMaxStringLen%)
local dot&
	dot&=loc(KTestFile$,".")
	print dot&
	expectedFilename$=left$(KTestFile$,dot&-1)+"(01)"+mid$(KTestFile$,dot&,255)
	ioopen(h%,KTestFile$,2)
	ioclose(h%)
	a$=SyUniqueFilename$:(KTestFile$)
	delete KTestFile$
	if a$<>expectedFilename$
	        print "*** Unique Filename test failed: Unique Filename:",a$
        	raise 1030
	else
		print "Unique Filename:",a$
	endif
endp

proc TIsPathVisible:
	print "Path Visible:";
	print SyIsPathVisible&:("c:\Documents\Test")
endp

proc TMemory:
	local TotalRamInBytes&
	local TotalRomInBytes&
	local MaxFreeRamInBytes&
	local FreeRam&
	local InternalDiskRamInBytes&

	rem SyMemory:(TotalRamInBytes&,TotalRomInBytes&,MaxFreeRamInBytes&,FreeRam&,InternalDiskRamInBytes&)
	print "Total Ram:",TotalRamInBytes&
	print "Total Rom:",TotalRomInBytes&
	print "Max Free Ram:",MaxFreeRamInBytes&
	print "Free Ram:",FreeRam&
	print "Internal Ram:",InternalDiskRamInBytes&
endp

proc TKeyClickLoud:
	REM print "Key Click Loud:",SyKeyClickLoud%:
endp

proc TKeyClickOverridden:
	REM print "Key Click Overridden:",SyKeyClickOverridden%:
endp

proc TSoundDriverEnabled:
	REM print "Sound driver enabled:",SySoundDriverEnabled%:
endp

proc TDisplaySize:
	local displayWidthInPixels&
	local displayHeightInPixels&
	local xYInputWidthInPixels&
	local xYInputHeightInPixels&
	local physicalScreenWidth&
	local physicalScreenHeight&
	REM SyDisplaySize:(displayWidthInPixels&, displayHeightInPixels&, xYInputWidthInPixels&, xYInputHeightInPixels&, physicalScreenWidth&, physicalScreenHeight&)
	print "Display Width In Pixels:", displayWidthInPixels& 
	print "Display Height In Pixels:", displayHeightInPixels&
	print "XY Input Width In Pixels:", xYInputWidthInPixels& 
	print "XY Input Height In Pixels:", xYInputHeightInPixels&
	print "Physical Screen Width:", physicalScreenWidth&
	print "Physical Screen Height:", physicalScreenHeight& 
endp

CONST KJVMPath$="Z:\System\Programs\Java.exe"
CONST KJVMCmdLine$=""

PROC tRunExeCmd:
	PRINT "Running an exe with command line tail - thread:",SyRunExeWithCmd&:(KJVMPath$,KJVMCmdLine$)
ENDP

PROC tRunDoc:
LOCAL File$(255),Dia%,SwitchToIfRunning%
	SwitchToIfRunning%=KFalse%
	DO
		dINIT "Select document to run"
		dFILE File$,"Name,Folder,Disk",$100+$200
		dCHECKBOX SwitchToIfRunning%,"Switch to if running"
		dBUTTONS "Cancel",-(KKeyEsc% OR KDButtonNoLabel%),"OK",KKeyEnter% OR KDButtonNoLabel%
		LOCK ON
		Dia%=DIALOG
		LOCK OFF
		IF Dia%<>KKeyEnter%
			BREAK
		ENDIF
		PRINT "ThreadID =",SyRunDocument&:(File$,SwitchToIfRunning%)
	UNTIL 0
ENDP

PROC tOPXVersion:
	ONERR Skipped::
	PRINT "Version of Z:\System\OPX\SendAs.opx =",HEX$(SyGetOPXVersion&:("Z:\System\OPX\SendAs.opx"))
	PRINT "Version of Z:\System\OPX\AppFrame.opx =",HEX$(SyGetOPXVersion&:("Z:\System\OPX\AppFrame.opx"))
	PRINT "Version of Z:\System\OPX\Alarm.opx =",HEX$(SyGetOPXVersion&:("Z:\System\OPX\Alarm.opx"))
	GOTO Skipped2::
	Skipped::
	PRINT "Error (";ERR;") No Version() EXPORT?"
	Skipped2::
	ONERR OFF

	ONERR ZSysram1::
	PRINT "Version of Z:\System\OPX\System.opx =",HEX$(SyGetOPXVersion&:("Z:\System\OPX\System.opx"))
	GOTO ZSysram1_OK::
	ZSysram1::
	PRINT "Error (";ERR;")"
	ZSysram1_OK::
	ONERR OFF
	
	ONERR CSysram1::
	PRINT "Version of C:\System\OPX\System.opx =",HEX$(SyGetOPXVersion&:("C:\System\OPX\System.opx"))
	GOTO CSysram1_OK::
	CSysram1::
	PRINT "Error (";ERR;")"
	CSysram1_OK::
	ONERR OFF
	
	PRINT "Trying to get OPX version of non OPX file..."
	ONERR NonOPX::
	PRINT "Version of Z:\System\Apps\OPL\OPL.app =",HEX$(SyGetOPXVersion&:("Z:\System\Apps\OPL\OPL.app"))
	GOTO NonOPX_OK::
	NonOPX::
	PRINT "Error (";ERR;")"
	NonOPX_OK::
	ONERR OFF
ENDP

proc TLinkEnableWithOptions&:
local err%

	err%=_ShowLinkConfig:
	
	TryCable::
	ONERR CableErr::
	PRINT "Enabling remote link to Cable/115200...."
	SyRemoteLinkEnableWithOptions:(KLinkTypeCable%,KLinkBps115200%)
	err%=_ShowLinkConfig:
	PRINT "....please check it from System and then press any key back here. "
	GET
	SyRemoteLinkDisable:
	GOTO TryIr::
	CableErr::
	PRINT "*FAILED*"
	err%=-1

	ONERR OFF
	TryIr::
	ONERR IrErr::
	PRINT "Enabling remote link to IrDA/57600...."
	SyRemoteLinkEnableWithOptions:(KLinkTypeIrDA%,KLinkBps57600%)
	err%=_ShowLinkConfig:
	PRINT "....please check it from System and then press any key back here. "
	GET
	SyRemoteLinkDisable:
	GOTO End::
	IrErr::
	PRINT "*FAILED*"
	err%=-1

	End::
	RETURN err%
endp

proc _ShowLinkConfig:
local baud%,type%

	ONERR LinkNotOn::
	PRINT "Getting current link config (SyRemoteLinkConfig):"
	SyRemoteLinkConfig:(type%,baud%)
	PRINT "Type id =",type%,", Baud id =",baud%
	PRINT "type%=0 for unknown, type%=1 for cable, type%=2 for IrDA"
	PRINT "baud%=0 for unknown, baud%=1 for 9600, baud%=2 for 19200"
	PRINT "baud%=3 for 38400, baud%=4 for 57600, baud%=5 for 115200"
	GOTO End::
	LinkNotOn::
	PRINT "*FAILED* -",ERR$(ERR)
	return -1
	
	End::
	return 0
endp