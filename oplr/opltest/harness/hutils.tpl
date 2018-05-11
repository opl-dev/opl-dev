REM hUtils.tpl -- File logging and testing utilities for OPL testing.
REM Copyright (c) 1999-2000 Symbian Ltd. All rights reserved.
REM Portions from T_UTIL by Howard Price, May 1990.
REM v0.26

DECLARE EXTERNAL
INCLUDE "Const.oph"
INCLUDE "System.oxh"
INCLUDE "hUtils.oph"

REM Private functions.
EXTERNAL _hInit:

EXTERNAL _hDoLog%:(aData$)
EXTERNAL _hOpenLog%:
EXTERNAL _hLogErrorFormat$:(aErr%,aErrx$)
EXTERNAL _hLogErrorProcname$:(aErr$)
EXTERNAL _hLogPrepareToUse%:(aFlag%)
EXTERNAL _hLogPrepareToStop:(aFlag%)
EXTERNAL _hException:(a1$,a2$,a3$)
EXTERNAL _hErrorCount%: REM Total number of errors detected.

EXTERNAL _hPreTestCheck%:
EXTERNAL _hPostTestCheck%:
EXTERNAL _hCheckDrawables%:
EXTERNAL _hCheckScreen%:

EXTERNAL _hDescendTree%:(aPath$)
EXTERNAL _hFileLaunch%:(aOpofilename$)
EXTERNAL _hFileIsOpo%:(aPath$)
EXTERNAL _hFilename$:(aOpofilename$)
EXTERNAL _hFilenameExt$:(aPath$)
EXTERNAL _hDirname$:(aPath$)
EXTERNAL _hBasename$:(aPath$)

EXTERNAL _hSetTestFileDesc%:(aPath$)
EXTERNAL _hSetTestProcDesc%:(aPath$)
EXTERNAL _hLogLevelText$:(aLevel%)

EXTERNAL _hThreadId$:	REM Thread Id of this program.

REM Private constants

REM Levels and Ids e.g. Test "Crystal\Color\Sprite test99:"
REM indicates kTestFileDesc="Crystal\Color\Sprite", 
REM using Sprite.tpl and Test99: is the proc being CALLed.

CONST _KhLogProcSep$=":"
CONST _KhLogDirSep$="\"

PROC hLogUsage:
	dINIT "'hUtils' is not designed to run stand-alone"
	dTEXT "","Logging and testing procedures are called by applications"
	dTEXT "","Use LOADM ""KLogTestUtil$"" to make the procedures available"
	DIALOG
ENDP

PROC hLog%:(aLevel%,aData$)
	REM Logs Data$ to text log file, provided the logging level
	REM is lower than the current threshold. Returns any error value.
	EXTERNAL hLogThreshold%
	EXTERNAL _hThreadId$
	LOCAL err%
	LOCAL data$(KMaxStringLen%)

	REM Lose it if its too low...
	IF aLevel%>hLogThreshold%
		RETURN KErrNone%
	ENDIF

REM	data$=LogConvert$:(aData$)

REM Write out the thread Id, then the logged text, then the CR/LF pair.
	data$=_hThreadId$+": "+aData$+CHR$(13)+CHR$(10)
	err%=_hDoLog%:(data$)
	RETURN err%
ENDP


PROC hLink:(aCallbackProcName$,aThreadId&,aUserFlag%)
	REM The main procedure.
	REM !!TODO Document these globals.

	GLOBAL hLogThreshold%			REM Current log priority threshold.
	GLOBAL hCallVerbose%			REM !!TODO dunno???

	GLOBAL _hTestFileDesc$(255)	REM Test OPO/APP ???
	GLOBAL _hTestProcDesc$(255)
	GLOBAL _hTestRoot$(255)		REM Root of current suite. e.g. "\MyTests"

	GLOBAL _hTestNumber%		REM
	GLOBAL _hErrorCount%		REM Total number of errors detected.

	REM Initialisation flag settings.
	REM Boolean flags true for the following conditions:
	GLOBAL _hDebugMode%				REM Test harness code running in debug mode.
	GLOBAL _hDryRunOnly%			REM No tests to be executed--harness will descend test tree but not run tests.
	GLOBAL _hManualControl%		REM User offered control before each test runs.
	GLOBAL _hStartAtTest%			REM Testing begins midway into suite.
	GLOBAL _hStopAtFirstError%	REM Stop test harness if a test raises an error.
	GLOBAL _hLocalErrorHandling%	REM A target app using harness will handle errors itself.

	GLOBAL _hSpoofTargetName$(255)	REM Name of target app.
	GLOBAL _hSpoofTargetThread&			REM Thread id of target app.

	GLOBAL _hThreadId$(5)	REM Harness user id in "12345" format.

	REM Screen size
	GLOBAL _hgWidth%,_hgHeight%

	_hgWidth%=gWIDTH :_hgHeight%=gHEIGHT

	_hThreadId$=RIGHT$("0000"+GEN$(aThreadId&,5),5)
	_hInit:
	_hLogPrepareToUse%:(aUserFlag%)
	REM No error handling around here, otherwise we lose the
	REM benefits of standalone testing under TextEd.
	@(aCallbackProcName$): REM Call back to the tester.
	_hLogPrepareToStop:(aUserFlag%)
	BUSY OFF
ENDP



PROC hThreadIdFromOplDoc&:
	REM Return the thread id of current app.
	LOCAL prev&
	RETURN SyThreadIdFromOpenDoc&:(CMD$(1),prev&)
ENDP


PROC hThreadIdFromOplAppUid&:(aUid&)
	REM Thread id of app whose UID is aUid&.
	LOCAL prev&
	RETURN SyThreadIdFromAppUid&:(aUid&,prev&)
ENDP


PROC hLogChangeThreshold%:(aNewLevel%)
	REM Change the threshold level at which log notes are
	REM written to the log file. Returns the old level.
	REM The smaller the level, the less info logged.
	EXTERNAL hLogThreshold%
	LOCAL oldLevel%,delta$(255)
	oldLevel%=hLogThreshold%
	hLogThreshold%=aNewLevel%
	REM Report when going more verbose. Heck, report all the time.
	delta$=_hLogLevelText$:(oldLevel%)+" to "+_hLogLevelText$:(aNewLevel%)
	hLog%:(KhLogAlways%,"Logging threshold level changed from "+delta$+".")
	RETURN oldLevel%
ENDP


PROC hLogTimestamp:
	REM Time/date stamp for simple timing.
	hLog%:(KhLogAlways%,DATIM$)
ENDP


PROC hTestNumber%:
	REM Return current test number.
	EXTERNAL _hTestNumber%
	RETURN _hTestNumber%
ENDP


PROC hTestDesc$:
	REM Return test description.
	EXTERNAL _hTestFileDesc$,_hTestProcDesc$
	REM Running standalone?
	IF _hTestFileDesc$=""
		RETURN _hTestProcDesc$
	ENDIF
	RETURN _hTestFileDesc$+" "+_hTestProcDesc$
ENDP



PROC hRunTest%:(aProcname$)
	REM Automatic execution of the proc named aProcname.
	EXTERNAL hCallVerbose%
	EXTERNAL _hDebugMode%
	EXTERNAL _hTestDesc$()
	EXTERNAL _hTestNumber%
	EXTERNAL _hDryRunOnly%,_hStartAtTest%,_hStopAtFirstError%
	EXTERNAL _hErrorCount%
	EXTERNAL _hLocalErrorHandling%

	REM !!TODO manual mode?

	REM Copy of current test id.
	_hSetTestProcDesc%:(aProcname$+_KhLogProcSep$)
	_hTestNumber%=_hTestNumber%+1
	
	REM Are we skipping?
	IF _hStartAtTest%
		IF _hStartAtTest%>_hTestNumber%
			hLog%:(hCallVerbose%, "Skipping test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"'")
			RETURN
		ENDIF
	ENDIF

	hLog%:(hCallVerbose%, "Test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"'")

	REM Bit of feedback...
	BUSY "Test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"'", KBusyBottomRight%

	IF NOT _hLocalErrorHandling%
		ONERR ErrorHandlerCall::
	ENDIF

	IF NOT _hDryRunOnly%
		IF _hDebugMode%
			_hPreTestCheck%:
		ENDIF
		@(aProcName$):
		IF _hDebugMode%
			_hPostTestCheck%:
		ENDIF
	ENDIF
	RETURN
ErrorHandlerCall::
	ONERR OFF
	hLog%:(KhLogAlways%, "!!! ERROR: test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"'")
	hLog%:(KhLogAlways%, _hLogErrorFormat$:(ERR,ERRX$))
	_hErrorCount%=_hErrorCount%+1
	IF _hStopAtFirstError%
		RAISE ERR
	ENDIF
ENDP


PROC hCall%:(aProcname$)
	REM Call the proc aProcname$ in an interactive program.
	REM To be used by log users only.
	EXTERNAL hCallVerbose%, _hLocalErrorHandling%
	rem hLog%:(hCallVerbose%, "Calling '"+aProcname$+"'")

	IF NOT _hLocalErrorHandling%
		ONERR ErrorHandlerCall::
	ENDIF
	@(aProcName$):
	RETURN

ErrorHandlerCall::
	ONERR OFF
	hLog%:(KhLogAlways%, "!!! ERROR: target proc '"+aProcname$+"'")
	hLog%:(KhLogAlways%, _hLogErrorFormat$:(ERR,ERRX$))
ENDP


PROC hCleanUp%:(aCleanUpProcname$)
	REM Execute the clean-up proc named in aCleanUpProcname$
	EXTERNAL hCallVerbose%
	EXTERNAL _hTestDesc$()
	EXTERNAL _hTestNumber%
	EXTERNAL _hDryRunOnly%,_hStartAtTest%
	EXTERNAL _hErrorCount%
	REM !!TODO manual mode?

	REM Copy of current test id.
	_hSetTestProcDesc%:(aCleanUpProcname$+_KhLogProcSep$)
	rem _hTestNumber%=_hTestNumber%+1
	
	REM Are we skipping?
	IF _hStartAtTest%
		IF _hStartAtTest%>_hTestNumber%
			rem hLog%:(hCallVerbose%, "Skipping test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"'")
			RETURN
		ENDIF
	ENDIF

	hLog%:(hCallVerbose%, "Test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"' [CleanUp]")
	ONERR ErrorHandlerCall::
	IF NOT _hDryRunOnly%
		@(aCleanUpProcName$):
	ENDIF
	RETURN
ErrorHandlerCall::
	ONERR OFF
	hLog%:(KhLogAlways%, "!!! ERROR: in test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"'")
	hLog%:(KhLogAlways%, _hLogErrorFormat$:(ERR,ERRX$))
	_hErrorCount%=_hErrorCount%+1
ENDP


PROC hStartTestHarness:(aRoot$)
	REM Start executing a test suite whose location is the folder aRoot$
	EXTERNAL _hTestRoot$
	_hTestRoot$=aRoot$
	IF RIGHT$(_hTestRoot$,1)<>"\"
		_hTestRoot$=_hTestRoot$+"\"
	ENDIF
	hLog%:(KhLogAlways%,"Test harness starting at "+_hTestRoot$)
	_hDescendTree%:(aRoot$)
ENDP


PROC hDiskName$:
	REM Harness disk name e.g. "c:" 
	LOCAL off%(6),p$(255),disk$(255)
	p$=PARSE$(CMD$(1),"",off%())
	IF off%(KParseAOffPath%)>1
		disk$=LEFT$(p$,off%(KParseAOffPath%)-1)
	ELSE
		disk$="c:"
	ENDIF
	RETURN disk$
ENDP


PROC hRunningUnderSystemTestApp%:
	REM Bool returning True if running under system test.
	EXTERNAL _hTestRoot$
	IF LEN(_hTestRoot$)
		RETURN KTrue%
	ELSE
		RETURN KFalse%
	ENDIF
ENDP


PROC hInitTestHarness:(aFlag1%,aFlag2%)
	REM Change test harness flags.
	EXTERNAL _hDebugMode%
	EXTERNAL _hDryRunOnly%,_hManualControl%,_hStartAtTest%
	EXTERNAL _hStopAtFirstError%, _hLocalErrorHandling%

	IF aFlag1% AND KhInitDebugMode%
		_hDebugMode%=KTrue%
		hLog%:(KhLogAlways%,"Init: KhInitDebugMode% set.")
		hLog%:(KhLogAlways%,"Entering debug mode.")
	ELSEIF aFlag1% AND KhInitDryRunOnly%
		_hDryRunOnly%=KTrue%
		hLog%:(KhLogAlways%,"Init: KhInitDryRunOnly% set.")
		hLog%:(KhLogAlways%,"Dry run only.")
	ELSEIF aFlag1% AND KhInitManualControl%
		_hManualControl%=KTrue%
		hLog%:(KhLogAlways%,"Init: KhInitManualControl% set.")
		hLog%:(KhLogAlways%,"Manual control.")
	ELSEIF aFlag1% AND KhInitStartAtTestNumber%
		_hStartAtTest%=aFlag2%
		hLog%:(KhLogAlways%,"Init: KhInitStartAtTestNumber% set.")
		hLog%:(KhLogAlways%,"Starting at test "+GEN$(_hStartAtTest%,5)+".")
	ELSEIF aFlag1% AND KhInitStopAtFirstError%
		_hStopAtFirstError%=KTrue%
		hLog%:(KhLogAlways%,"Init: KhInitStopAtFirstError% set.")
		hLog%:(KhLogAlways%,"Stop at first error.")
	ELSEIF aFlag1% AND KhInitLocalErrorHandling%
		_hLocalErrorHandling%=KTrue%
		hLog%:(KhLogAlways%,"Init: KhInitLocalErrorHandling% set.")
		hLog%:(KhLogAlways%,"Target app will handle errors itself.")
	ENDIF
ENDP


PROC hSpoofRunTargetApp&:(aLib$,aDoc$,aTail$,aCmd&)
	REM Launch a target app, which may receive spoofed keys.
	REM "OPL","","RZ:\System\Opl\Toolbar.opo",2
	REM "Data","Z:\System\Data\Help","",0

	EXTERNAL _hSpoofTargetName$
	EXTERNAL _hSpoofTargetThread&

	LOCAL thread&
	LOCAL targetFilename$(255)

	REM Identify the app that will receive the key events.
	IF LOWER$(aLib$)="opl"
		_hSpoofTargetName$=aTail$
		targetFilename$=RIGHT$(_hSpoofTargetName$,LEN(_hSpoofTargetName$)-1)
	ELSE
		_hSpoofTargetName$=aLib$
		targetFilename$=aLib$
	ENDIF

	REM Check the target exists.
	IF NOT EXIST (targetFilename$)
		hLog%:(KhLogAlways%,"Target not found: "+targetFilename$)
		RAISE KErrNotExists%
	ENDIF

	thread&=SyRunApp&:(aLib$,aDoc$,aTail$,aCmd&)
	hLog%:(KhLogAlways%, "Thread "+GEN$(thread&,5)+" is "+_hSpoofTargetName$)

	PAUSE KhLaunchDelay%

	IF aCmd&=KSyRunAppBackground%
		REM !!TODO push it to background????
	ENDIF

	REM Store target thread.
	_hSpoofTargetThread&=thread&
	RETURN thread&
ENDP


PROC hSpoofCloseApp%:
	REM Close a launched app.
	LOCAL previous&
	LOCAL thread&
	thread&=hSpoofTargetThread&:
	hLog%:(KhLogMedium%,"hSpoofCloseApp%: running.")
	hLog%:(KhLogMedium%,"EndTask&:("+GEN$(thread&,6)+") called.")
	previous&=0
	SyKillTask&:(thread&,previous&)
	REM !!TODO Try EndTask&: before KillTask&: as it's nicer.
	REM !!TODO Handle any errors here, like the thread already being dead.
	rem Finally, bring the test harness to the foreground...
	hHarnessSetForeground:
ENDP


PROC hSpoofTargetName$:
	REM Return the current spoof app name.
	EXTERNAL _hSpoofTargetName$
	REM Returns the name of the current receiver app.
	RETURN _hSpoofTargetName$
ENDP


PROC hSpoofTargetThread&:
	EXTERNAL _hSpoofTargetThread&
	REM Returns the thread id of the current receiver app.
	RETURN _hSpoofTargetThread&
ENDP


PROC hSpoofSetFlagTargetApp%:(aTargetApp$,aDeleteFlag%)
	LOCAL lockfile$(255),err%,handle%
	LOCAL ret% REM first error detected, if any.
rem	lockfile$=_hBasename$:(aTargetApp$)+_hFilenameExt$:(aTargetApp$)+".flag"
	lockfile$=aTargetApp$+".flag"
	TRAP DELETE lockfile$

	IF aDeleteFlag%
		err%=ERR
		IF err%
			hLog%:(KhLogAlways%,"Error: '"+ERR$(err%)+"' when deleting "+lockfile$)
			ret%=err%
		ENDIF
		RETURN ret%
	ENDIF

	err%=IOOPEN(handle%,lockfile$,KIoOpenModeCreate% OR KIoOpenFormatBinary%)
	IF err%
		hLog%:(KhLogAlways%,"Error: '"+ERR$(err%)+"' when creating "+lockfile$)
		IF NOT ret%
			ret%=err%
		ENDIF
	ENDIF
	err%=IOCLOSE(handle%)
	IF err%
		hLog%:(KhLogAlways%,"Error: '"+ERR$(err%)+"' when closing "+lockfile$)
		IF NOT ret%
			ret%=err%
		ENDIF
	ENDIF
	RETURN ret%
ENDP


PROC hSpoofGetFlagTargetApp%:(aTargetApp$)
	LOCAL lockfile$(255)
	lockfile$=aTargetApp$+".flag"
	IF EXIST(lockfile$)
		RETURN KTrue%
	ENDIF
	RETURN KFalse%
ENDP


PROC hSendString&:(aBuffer$)
	REM Send a string to the current spoof target.
	LOCAL ret&,len%,i%
	len%=LEN(aBuffer$)
	i%=1
	WHILE i%<=len%
		ret&=hSendKeyRich&:(ASC(MID$(aBuffer$,i%,1)),0,0,0)
		i%=i%+1
	ENDWH
	PAUSE KhSendStringDelay%
	RETURN ret&
ENDP


PROC hSendKey&:(aCode&)
	LOCAL ret&
	ret&=hSendKeyRich&:(aCode&,0,0,0)
	PAUSE KhSendKeyDelay%
	RETURN ret&
ENDP


PROC hSendKeyRich&:(aCode&,aScancode&,aMod&,aRpt&)
	LOCAL prev&
	PAUSE KhSendKeyRichDelay%
	prev&=0
rem 	hLog%:(KhLogAlways%,"hSendKeyRich&: sending code="+GEN$(aCode&,5)+	" sc="+GEN$(aScanCode&,3)+" mod="+GEN$(aMod&,3)+	" rpt="+GEN$(aRpt&,3)+" to thread	"+GEN$(hSpoofTargetThread&:,6)+".")
	RETURN SySendKeyEventToApp&:(hSpoofTargetThread&:, prev&, aCode&, aScanCode&, aMod&, aRpt&)
ENDP

REM Quick shortcuts...

PROC hSpace%:
	hSendKey&:(KKeySpace%)
ENDP

PROC hTab%:
	hSendKey&:(KKeyTab%)
ENDP

PROC hEsc%:
	hSendKey&:(KKeyEsc%)
ENDP

PROC hMenu%:
	hSendKey&:(KKeyMenu32&)
ENDP

PROC hEnter%:
	hSendKey&:(KKeyEnter%)
ENDP

PROC hDown%:
	hSendKey&:(KKeyDownArrow32&)
ENDP

PROC hUp%:
	hSendKey&:(KKeyUpArrow32&)
ENDP

PROC hRight%:
	hSendKey&:(KKeyRightArrow32&)
ENDP

PROC hLeft%:
	hSendKey&:(KKeyLeftArrow32&)
ENDP


PROC hPageDown%:
	hSendKey&:(KKeyPageDown32&)
ENDP

PROC hPageUp%:
	hSendKey&:(KKeyPageUp32&)
ENDP

PROC hPageRight%:
	hSendKey&:(KKeyPageRight32&)
ENDP

PROC hPageLeft%:
	hSendKey&:(KKeyPageLeft32&)
ENDP



PROC hHarnessSetForeground:
	SySetForeground:
ENDP

PROC hHarnessSetBackground:
	SySetBackground:
ENDP

CONST K2GrayBlackMode%=-1

PROC hPeekLine&:(ax%,ay%,aLength%)
	LOCAL d%(999)
	LOCAL total&, mask&, elem&
	LOCAL word%, oddbits%, flash%
	
	gPEEKLINE gIDENTITY,ax%,ay%,d%(),aLength%,K2GrayBlackMode%
	word%=aLength%/16
	REM Check the flash bits at the end...
	oddbits%=alength%-(word%*16)
	IF oddbits%
		flash%=d%(word%+1)
		mask&=&2**oddbits%-1
		flash%=flash% AND mask&
		total&=flash%
	ENDIF

	REM And the main body...
	WHILE word%
		elem&=d%(word%)
		if elem&<0
			elem&=elem&+65536
		endif
		total&=total&+elem&
		word%=word%-1
	ENDWH
	RETURN total&
ENDP

REM Formatting...
PROC hBIN$:(aValue&)
	REM Like HEX$ but different.
	LOCAL binary$(32),add$(1),value&,test&,append%
	append%=KFalse%
	value&=avalue&
	IF value& AND &80000000
		binary$="1" :append%=KTrue%
	ENDIF
	test&=&40000000
	DO
		IF value& AND test&
			add$="1" :append%=KTrue%
		ELSE
			add$="0"
		ENDIF
		IF append%
			binary$=binary$+add$
		ENDIF
		test&=test&/2
	UNTIL test&=0
	IF NOT append%
		binary$="0"
	ENDIF
	RETURN binary$
ENDP


REM Private functions


PROC _hPreModuleCheck%:
	REM Can't think of any just yet...
ENDP


PROC _hPostModuleCheck%:
	_hCheckDrawables%:
	_hCheckScreen%:
ENDP


PROC _hPreTestCheck%:
ENDP


PROC _hPostTestCheck%:
ENDP


PROC _hCheckDrawables%:
	EXTERNAL _hgWidth%,_hgHeight%
	LOCAL i%,err%
	i%=64
	DO
		TRAP gUSE i%
		IF ERR
			err%=err%+1
		ELSE
			IF i%=1 REM default window.
				IF gWIDTH<>_hgWidth% OR gHEIGHT<>_hgHeight%
					hLog%:(KhLogAlways%, "Bad size default window. Width="+GEN$(gwidth,4)+",height="+GEN$(gheight,4))
					gSETWIN 0,0,_hgWidth%,_hgHeight%
				ENDIF
			ELSE
				rem get info
				hLog%:(KhLogAlways%, "Stale window. Id%="+GEN$(i%,2)+",gwidth="+GEN$(gwidth,4)+",gheight="+GEN$(gheight,4))
				gCLOSE i%
			ENDIF
		ENDIF
		i%=i%-1
	UNTIL i%=0
	REM Should be 63 errors, as all but one of the 64 drawables are closed.
	IF err%<>63
		RAISE 10000
	ENDIF
ENDP


PROC _hCheckScreen%:
	REM Check screen details:
	REM  mode etc.
	LOCAL c&(8)
	gUSE 1
	gCOLORINFO c&()
	hLog%:(KhLogAlways%,"Checking windows.")
	IF c&(1)<>KDisplayModeColor4K%
		hLog%:(KhLogAlways%,"Bad color depth.")
		RAISE 5000
	ENDIF
ENDP


PROC _hDoLog%:(aData$)
	REM Private fn to perform the log. Returns error value.
	GLOBAL _HLogHandle%
	LOCAL data$(KMaxStringLen%)
	LOCAL ret%

	ret%=_hOpenLog%:
	IF ret%<KErrNone%
		_hException:("Error: Unable to open file log.","Log open error='"+ERR$(ret%)+"'", "Test harness will now stop.")
		STOP
	ENDIF

	data$=aData$
	ret%=IOWRITE(_hLogHandle%,ADDR(data$)+1+KOplAlignment%,LEN(data$))
	REM report any error.
	IF ret%<KErrNone%
		_hException:("Error: Unable to write to file log.","Log write error='"+ERR$(ret%)+"'", "Test harness will now stop.")
		STOP
	ENDIF

	ret%=IOCLOSE(_HLogHandle%)
	IF ret%<KErrNone%
		REM RAISE ret%
		_hException:("Error: Unable to close file log.","Log close error='"+ERR$(ret%)+"'", "Test harness will now stop.")
		STOP
	ENDIF
	RETURN ret%
ENDP

PROC _hException:(a1$,a2$,a3$)
	REM !!TODO Use a dialog for this for now...
	dINIT "OPL TEST HARNESS EXCEPTION"
	dTEXT "",a1$
	dTEXT "",a2$
	dTEXT "",a3$
	DIALOG
ENDP

CONST KhMaxFailCount%=9 REM Attempts at opening file.

PROC _hOpenLog%:
	EXTERNAL _hLogHandle%
	LOCAL mode%,ret%
	LOCAL failCount%

	IF EXIST(KhLogName$)
		REM Append to any existing log.
		mode%=KIoOpenModeAppend% OR KIoOpenFormatText% OR KIoOpenAccessUpdate%
	ELSE
		mode%=KIoOpenModeCreate% OR KIoOpenFormatText% OR KIoOpenAccessUpdate%
	ENDIF

	DO
		ret%=IOOPEN(_hLogHandle%,KhLogName$,mode%)
		IF ret%>=KErrNone%
			BREAK REM Complete.
		ENDIF
		REM May have a 'KErrInUse%' error.
		failCount%=failCount%+1
		REM BEEP 1,100
		IF failCount%>KhMaxFailCount%
			_hLogHandle%=0
			BREAK REM Let the _hDoLog%: handle the error.
		ENDIF
		PAUSE 1+RND*7 REM Back off a little.
	UNTIL 0
	RETURN ret%
ENDP


PROC _hLogPrepareToUse%:(aUserFlag%)
	REM Prepares the log file. Returns error value.
	LOCAL ret%
	
	REM If logging only, keep the log as empty as possible.
	IF (aUserFlag% AND KhUserLogHeaders%)=0
		RETURN ret%
	ENDIF

	ret%=hLog%:(KhLogAlways%,"Logging start at "+DATIM$)
	IF ret%<KErrNone%
		dINIT "Unable to use log file"
		dTEXT "","Press any key"
		DIALOG
	ENDIF
	RETURN ret%
ENDP


PROC _hLogPrepareToStop:(aUserFlag%)
	LOCAL ret%,total%
	IF (aUserFlag% AND KhUserLogHeaders%)=0
		RETURN
	ENDIF
	hLog%:(KhLogAlways%,"Logging stop at "+DATIM$)
	total%=hTestNumber%:
	REM Log might be closed from non-test app.
	IF total%
		hLog%:(KhLogAlways%,GEN$(total%,5)+" tests run, "+GEN$(_hErrorCount%:,5)+" errors.")
	ENDIF
	hLog%:(KhLogAlways%,"---")
ENDP



PROC _hTestPath$:(aPath$)
	EXTERNAL _hTestRoot$
	LOCAL off%(6),p$(255)
	p$=PARSE$(aPath$,"",off%())
	p$=MID$(p$,off%(KParseAOffPath%),off%(KParseAOffFilename%)-off%(KParseAOffPath%))
	p$=RIGHT$(p$,LEN(p$)-LEN(_hTestRoot$)+2)
	RETURN p$
ENDP


PROC _hSetTestProcDesc%:(aName$)
	EXTERNAL _hTestProcDesc$
	_hTestProcDesc$=aName$
ENDP


PROC _hDescendTree%:(aPath$)
	EXTERNAL _hErrorCount%
	LOCAL prev$(255)
	LOCAL branch$(255)
	
	LOCAL dummyc%,curdep%

	prev$=aPath$+"\"
	ONERR ErrorHandlerDescend::
	branch$=Dir$(prev$)
rem	ONERR OFF

	rem probably a subdirectory name!
rem	_hPush%:
rem	hSetTestDesc%:(_hDirname$:(branch$))
	
	curdep%=0
	WHILE branch$<>""
		ONERR EHContinue::
		_hDescendTree%:(branch$)
		GOTO Skippy::
		
EHContinue::
		ONERR OFF
		rem PRINT "DEBUG: error in descendTree looper."
		
Skippy::
		REM Do peers, by restoring dir state.
		DIR$(prev$)
		dummyc%=0
		WHILE dummyc%<curdep%
			DIR$("")
			dummyc%=dummyc%+1
		ENDWH
		curdep%=curdep%+1
		branch$=DIR$("")
	ENDWH
rem	_hPop%:
	RETURN

ErrorHandlerDescend::
	ONERR OFF
	IF ERR<>KErrDir%
		hLog%:(KhLogAlways%, _hLogErrorFormat$:(ERR,ERRX$))
		_hErrorCount%=_hErrorCount%+1
		rem RAISE ERR
		RETURN
	ENDIF
	IF _hFileIsOpo%:(aPath$)
		_hFileLaunch%:(aPath$)
	ENDIF
ENDP


PROC _hFileIsOpo%:(aPath$)
	LOCAL extn$(4)
	IF LEN(aPath$)<5
		RETURN KFalse%
	ENDIF
	IF LOWER$(RIGHT$(aPath$,4))=".opo"
		RETURN KTrue%
	ENDIF
	RETURN KFalse%
ENDP



PROC _hFileLaunch%:(aOpofilename$)
	EXTERNAL _hErrorCount%
	EXTERNAL _hDebugMode%
	LOCAL procname$(KMaxStringLen%)
	procname$=_hFilename$:(aOpofilename$)
	LOADM aOpofilename$
	hLog%:(KhLogLow%,"Launching "+aOpofilename$+" using "+procname$+_KhLogProcSep$)

	REM Keep the test details.
	_hSetTestFileDesc%:(_hTestPath$:(aOpoFilename$)+procname$)
	_hSetTestProcDesc%:(procname$+_KhLogProcSep$)
	ONERR ERLaunch::

	IF _hDebugMode%
		_hPreModuleCheck%:
	ENDIF
	@(procname$):
	IF _hDebugMode%
		_hPostModuleCheck%:
	ENDIF

	GOTO Skippy::
ERLaunch::
	ONERR OFF
	hLog%:(KhLogAlways%, "!!! ERROR: test "+GEN$(hTestNumber%:,6)+" '"+hTestDesc$:+"'")
	hLog%:(KhLogAlways%, _hLogErrorFormat$:(ERR,ERRX$))
	_hErrorCount%=_hErrorCount%+1
Skippy::
	UNLOADM aOpofilename$
ENDP


PROC _hDirName$:(afile$)
	REM Just the folder name of the file, no other path details.
	LOCAL off%(6), p$(255), rel$(255)
	LOCAL len%,i%
	p$=PARSE$(afile$,rel$,off%())
	p$=MID$(p$,off%(KParseAOffPath%),off%(KParseAOffFilename%)-off%(KParseAOffPath%))
	len%=LEN(p$)
	REM Minimum name is "\a\"
	IF len%<3 :RETURN p$ :ENDIF
	
	REM Now start at last char, not the trailing slash.
	i%=len%-1
	DO
		IF MID$(p$,i%,1)="\"
			RETURN MID$(p$,i%+1,len%-i%)
		ENDIF
		i%=i%-1
	UNTIL i%=0
	REM Give up here.
	RETURN p$
ENDP


PROC _hFilename$:(afile$)
	REM Return filename without extension.
	LOCAL off%(6), p$(255)
	p$=PARSE$(afile$,"",off%())
	RETURN MID$(p$,off%(KParseAOffFilename%),off%(KParseAOffExt%)-off%(KParseAOffFilename%))
ENDP


PROC _hFilenameExt$:(aFile$)
	REM Returns filename with extension.
	LOCAL o%(6)
	PARSE$(aFile$,"",o%())
	RETURN MID$(aFile$, o%(KParseAOffFilename%), o%(KParseAOffFilename%)-o%(KParseAOffPath%)+1)
ENDP


PROC _hBase$:(aFile$)
	REM Returns full pathname of folder.
	LOCAL o%(6)
	PARSE$(aFile$,"",o%())
	RETURN MID$(aFile$, 1, o%(KParseAOffFilename%)-1)
ENDP


PROC _hSetTestFileDesc%:(aPath$)
	EXTERNAL _hTestFileDesc$
	_hTestFileDesc$=aPath$
ENDP


PROC _hErrorCount%:
	EXTERNAL _hErrorCount%
	RETURN _hErrorCount%
ENDP


PROC _hInit:
	REM Initialise the harness utils.
	EXTERNAL hLogThreshold%
	EXTERNAL hCallVerbose%,_hDryRunOnly%
	REM Verbose controls the level at which calls are logged.
	REM Currently, all test calls are logged; this is the same as
	REM the RTest class in C++.
	hLogThreshold%=KhLogAlways%
	hCallVerbose%=KhLogAlways%
	_hDryRunOnly%=KFalse%
ENDP


PROC _hLogErrorFormat$:(aErr%,aErr$)
	LOCAL formattedErr$(KMaxStringLen%)
	IF aErr%=KErrNoProc%
		formattedErr$=_hlogErrorProcname$:(aErr$)
	ELSE
		formattedErr$=aErr$
	ENDIF
	RETURN "Error: "+ERR$(aErr%)+" ("+formattedErr$+")"
ENDP


PROC _hLogErrorProcname$:(aErr$)
	LOCAL len%,i%
	IF LOC(aErr$,",")=0
		REM No comma here (unusual!) so give up.
		RETURN aErr$
	ENDIF

	REM Start from end and scan backwards for last comma
	len%=LEN(aErr$)
	i%=len%
	
	WHILE i%>0
		IF MID$(aErr$,i%,1)=","
			RETURN MID$(aErr$,i%+1,len%-i%+1)
		ENDIF
		i%=i%-1
	ENDWH
	rem never get here.
	RETURN aErr$
ENDP


PROC _hLogLevelText$:(aLevel%)
	IF aLevel%=KhLogLow% :RETURN "Low"
	ELSEIF aLevel%=KhLogMedium% :RETURN "Medium"
	ELSEIF aLevel%=KhLogHigh% :RETURN "High"
	ELSE RETURN "Always"
	ENDIF
ENDP


REM End of hUtils.tpl

