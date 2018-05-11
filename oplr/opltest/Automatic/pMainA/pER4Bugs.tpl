REM pER4bugs.tpl
REM EPOC OPL automatic test code for ER4 OPL bug fixes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "System.oxh"
INCLUDE "Date.oxh"
DECLARE EXTERNAL

EXTERNAL makeTempFolder:
EXTERNAL testMru:
EXTERNAL testSystemFolder:
EXTERNAL testHiddenFolder:
EXTERNAL testDateOPX101:
EXTERNAL Month101:
EXTERNAL Micro101:
EXTERNAL Sec101:
EXTERNAL Min101:
EXTERNAL Hour101:
EXTERNAL SetDay101:
EXTERNAL SetHour101:
EXTERNAL SetMin101:
EXTERNAL SetSec101:
EXTERNAL SetMicro101:
EXTERNAL testSwitchClock:
EXTERNAL testFind17:
EXTERNAL testdfileUid:


const KTempFolder$="c:\ER4Temp\" rem can't use \System\Temp in some cases.
const KUidDirectFileStoreLayout&=268435511
const KUidMultiBitmapFileImage&=268435522


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pER4Bugs", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc per4bugs:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("ter4bugs")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	trap delete ktempfolder$+"*.*"
	trap rmdir ktempfolder$
ENDP


proc ter4bugs:
	makeTempFolder:

	rem test MRU [Recent Files]
	rem	testMru:

	rem test hidden folder dialog
	rem	testHiddenFolder:

	rem test System folder dialog
	rem testSystemFolder:

	rem print "Press any key to test Date OPX 1.01 (Esc to skip)"
	testDateOPX101:

	rem test switching clock.
	rem		testSwitchClock:

	rem test Find with 16+ fields.
	testFind17:

	rem test dFILE with UID.
	rem		testdfileUid:

	rem print "End of pr4bugs - press any key"
	rem get 
endp


proc makeTempFolder:
	rem somewhere sane to keep tmp files.
	if not exist(KTempFolder$)
		trap mkdir KTempFolder$
	endif
endp

proc testMru:
	print
	print "Testing the Most recently used (mru) list"
	print "SETDOC marks a program's document as MRU"
	print "You will need to start up a simple app to test this"
	print
	print "The test procedure is:"
	print " 1. From the Extras bar, start the T_Mru program"
	print " 2. The program reports c:\documents\t_mru has been setdoc'ed and exits"
	print " 3. Check the shell's MRU to confirm the t_mru file is present in the list"
	print "End of MRU test"
	print
endp


proc testSystemFolder:
	local file$(255)
	print "Test system folder bug"
	rem make sure the following folder exists.
	file$="C:\System\Temp\"
	if not exist (file$)
		mkdir file$
	endif
	dinit "Select 'Folder' line then hit Tab"
	dfile file$,"Name,Folder,Disk",KDFileSelectorWithSystem%
	dtext "","On ROM 1.01 and earlier, hitting",2
	dtext "","Tab on Folder will KERN-EXEC 3",2
	dialog
	print "Passed"
	print
endp


proc testHiddenFolder:
	local hidden$(255),folder$(255),file$(255)
	print "Test hidden folder bug"
	hidden$=KTempFolder$+"Hidden\"
	folder$=hidden$+"Fred\"
	if not exist(folder$)
		mkdir folder$
	endif
	SySetHiddenFile:(hidden$,1)
	file$=folder$+"file"
	trap create file$,a,dummy$
	rem For ROM 1.01 and earlier, the following dialog is will
	rem not appear because an EUSER CBase 21 error occurs.
	dinit
	dfile file$,"Name,Folder,Disk",KDFileAllowNullStrings%
	dtext "","If you can read this, the test has",2
	dtext "","passed. Press Enter to continue",2
	dialog
	print "Passed"
	print
	rem clean up
	trap close
	trap delete file$
endp


proc testDateOPX101:
	rem PRINT "Date OPX v1.01 testing"
rem	ONERR DateError::
	Month101:
	Micro101:
	Sec101:
	Min101:
	Hour101:
	SetDay101:
	SetHour101:
	SetMin101:
	SetSec101:
	SetMicro101:
	rem print "Passed"
	rem print
	RETURN

DateError::
	ONERR OFF
	raise 1
	PRINT CHR$(7)
	PRINT "Date 1.01 OPX tests FAILED!"
	PRINT
	rem GET
ENDP


PROC Month101:
	LOCAL dateh&
	rem PRINT "Testing Month bug:",
	ONERR EHandler::
	REM Will fail "out of range" at runtime with Date 1.00
	dateh&=DtNewDateTime&:(1998,5,31,0,0,0,0) REM A valid date.
	ONERR OFF
	rem PRINT "Passed"
	RETURN

EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Failed with Out Of Range bug"
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
	ENDIF
	RAISE ERR
ENDP


PROC Micro101:
	LOCAL dateh&
	rem PRINT "Testing Microsecond bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,0,0,0,1000000) REM invalid microsecond.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 500
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC Sec101:
	LOCAL dateh&
	rem PRINT "Testing Second bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,0,0,60,0) REM invalid second.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 501
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC Min101:
	LOCAL dateh&
	rem PRINT "Testing Minute bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,0,60,0,0) REM invalid minute.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 502
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC Hour101:
	LOCAL dateh&
	rem PRINT "Testing Hour bug:",
	IF KOpxDateVersion%<$101
		PRINT "Invalid Date OPX version - need Date v1.01 ++"
		RAISE 	KErrOpxVersion%
	ENDIF
	ONERR EHandler::
	REM Date opx 1.00 will panic with this.
	dateh&=DtNewDateTime&:(1998,6,20,24,0,0,0) REM invalid hour.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 503
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP


PROC SetDay101:
	LOCAL dateh&
	rem PRINT "Testing SetDay bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetDay:(dateh&,31) REM 31st May is a valid date.
	ONERR OFF
	rem PRINT "Passed"
	RETURN
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		PRINT "Failed with Out Of Range error"
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
	ENDIF
	RAISE ERR
ENDP

PROC SetHour101:
	LOCAL dateh&
	rem PRINT "Testing SetHour bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetHour:(dateh&,24) REM 24th hour should be invalid.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 504
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC SetMin101:
	LOCAL dateh&
	rem PRINT "Testing SetMinute bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetMinute:(dateh&,60) REM 60th minute is invalid.
	ONERR OFF
	PRINT "Failed - an error should have been reported"
	RAISE 505
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC SetSec101:
	LOCAL dateh&
	rem PRINT "Testing SetSecond bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetSecond:(dateh&,60) REM 60th second is invalid.
	ONERR OFF
	PRINT "Failed - an error should have been reported"
	RAISE 506
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP

PROC SetMicro101:
	LOCAL dateh&
	rem PRINT "Testing SetMicro bug:",
	dateh&=DtNewDateTime&:(1998,5,20,0,0,0,0)

	ONERR EHandler::
	DtSetMicro:(dateh&,1000000) REM 1 million microseconds is invalid.
	ONERR OFF
	PRINT "Failed - an error should have been detected"
	RAISE 507
EHandler::
	ONERR OFF
	IF ERR=KErrOutOfRange%
		rem PRINT "Passed (Out Of Range error sucessfully reported)"
		RETURN
	ELSE
		PRINT "Failed with unexpected error:", ERR$(ERR)
		RAISE ERR REM Hit some unexpected runtime error.
	ENDIF
ENDP


PROC testSwitchClock:
rem Panic switching clock format in low memory
	print
	print "Test that switching clock formats in low memory no longer panics."
	print
	print "You will need to use GOBBLE (or some other method) to reduce memory to a"
	print "low level. And you'll need the demo OPL program as shipped in ROM (or any"
	print "OPL program which uses the Toolbar."
	print
	print "The test procedure is:"
	print " 1. Start the demo OPL program"
	print " 2. Start the GOBBLE program to consume memory"
	print " 3. Toggle the Toolbar clock from digital to analog and back"
	print " 4. Confirm the OPL program does not panic."
	print "End of test"
	print
ENDP


PROC testFind17:
	rem panic using find with > 16 fields
	local file$(KDFileNameLen%)

	rem print "Test FIND no longer panics with > 16 fields"
	file$=KTempFolder$+"find17"
	trap delete file$
	create file$+" FIELDS s1(2),s2(2),s3(2),s4(2),s5(2),s6(2),s7(2),s8(2),s9(2),s10(2),s11(2),s12(2),s13(2),s14(2),s15(2),s16(2),s17(2) to table2",A,f1$,f2$,f3$,f4$,f5$,f6$,f7$,f8$,f9$,f10$,f11$,f12$,f13$,f14$,f15$,f16$,f17$
	a.f17$="r1"
	append
	a.f17$="r2"
	append
	first

	rem For early releases, the following Find will cause
	rem an User error 23 panic.
	find("*r*")
	rem print "Passed."

	rem clean-up
	trap close
	trap delete file$
	print
ENDP


PROC testdfileUid:
	local file$(KDFileNameLen%)
	local uid1&,uid2&,uid3&
	local flag%

	print "Test dFILE with UID selector"
	uid1&=KUidDirectFileStoreLayout&
	uid2&=KUidMultiBitmapFileImage&
	uid3&=0
	file$="Z:\System\OPL\*"

	rem For earlier releases, the following dialog shows
	rem all the files in the folder, not just the ones
	rem matching the UIDs.

	dinit "dFILE with UID selector"
	dposition KDPositionRight%,KDPositionCenter%
	dtext "", "Press Enter if only MBM files are", 2
	dtext "", "visible in this file selector.", 2
	flag%=KDFileAllowWildCards% OR KDFileSelectorWithRom% OR KDFileSelectorWithSystem%
	dfile file$,"File,Folder,Disk",flag%,uid1&,uid2&,uid3&
	if dialog
		print "Passed"
	else
		print "Failed!"
	endif
	print
ENDP

REM End of pER4bugs.tpl
