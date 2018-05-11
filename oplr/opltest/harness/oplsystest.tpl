REM OplSysTest.tpl
REM OPL test harness for system test of EPOC OPL.
REM v0.08

CONST KUidOplSysTest&=&10005F90

APP OplSysTest,KUidOplSysTest&
ENDA

DECLARE EXTERNAL

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"


EXTERNAL harnessTop:
EXTERNAL harnessMain:

PROC harnessTop:
	LOADM KhUtils$
	REM We want the OPL log to be in ASCII rather than unicode.
	SETFLAGS KAlwaysWriteAsciiTextFiles&

	GIPRINT "Running OPL test harness"
	hLink:("harnessMain", hThreadIdFromOplAppUid&:(KUidOplSysTest&), KhUserFull%)
rem	hLink:("harnessMain", 	hThreadIdFromOplDoc&:, KhUserFull%)

	BUSY OFF
	dinit "OPL system test has finished"
	dposition KDpositionRight%,KDPositionTop%
	dialog
ENDP


PROC harnessMain:
	rem hLogChangeThreshold%:(KhLogHigh%)
	REM hInitTestHarness:(KhInitDryRunOnly%,KhInitNotUsed%)
	REM hInitTestHarness:(KhInitManualControl%,0)

	REM When debugging into one particular test, it's sometimes good to be able
	REM to start testing from deep within the test suite...
	REM But in this case, start at the first test, test 0.
	REM hInitTestHarness:(KhInitStartAtTestNumber%, 245)
	hInitTestHarness:(KhInitStartAtTestNumber%, 0)

	REM hInitTestHarness:(KhInitStopAtFirstError%,0)
	hInitTestHarness:(KhInitDebugMode%,KhInitNotUsed%)

	REM !!todo Force no trailing backslash.
	hStartTestHarness:(hDiskName$:+"\Opltest\Automatic")
ENDP


REM End of OplSysTest.tpl
