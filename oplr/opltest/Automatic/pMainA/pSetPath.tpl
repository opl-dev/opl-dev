REM pSetPath.tpl
REM EPOC OPL automatic test code for SETPATH.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL
INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNAL Prepare:
EXTERNAL tLoad:
EXTERNAL tOthers:
EXTERNAL testSETPATH: REM Loaded from pDaysA.

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pSetPath", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pSetPath:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tpSetPath")
	hCleanUp%:("Reset")
endp

CONST kPath$="c:\Opl1993\"

PROC Reset:
	SETPATH "C:\"
	trap delete Kpath$+"test\*.*"
	trap rmdir Kpath$+"test\"
	trap delete Kpath$+"*.*"
	trap rmdir kPath$
ENDP


rem this program requires that the following files are in 
rem the rem same directory: 
rem pface.mbm, putil, pmaina, testfont.gdr


PROC tpsetpath:
	mkdir KPath$
	setpath KPath$

	Prepare:
	tload:
	tothers:

	rem print "Opler1 SETPATH Tests Finished OK"
	rem pause 20
ENDP


PROC Prepare:
	COPY hDiskName$:+"\Opltest\Data\pface.mbm",kPath$
	COPY hDiskName$:+"\Opltest\Data\testfont.gdr",kPath$
rem	COPY hDiskName$:+"\Opltest\Automatic\pMainA\pGen.opo",kPath$
ENDP


proc tload:
	local id%
	rem print "Test Loading"
	REM File access uses the new path...
	id%=gloadbit ("pface.mbm")
	gclose id%

	id%=gloadfont("testfont.gdr")
	gunloadfont id%

	REM This test will not run under the test harness.
	IF NOT hRunningUnderSystemTestApp%:	

		REM ... but loadm is unchanged.
		loadm "pDaysA.opo"
		REM A procedure in that module...
		IF testSETPATH:<>PI
			RAISE 1
		ENDIF
		unloadm "pDaysA.opo"
	ENDIF
endp


proc tothers:
	local newpath$(255)
	rem print "Test copy and exist"
	REM Make sure they work as before.
	newpath$=kPath$+"test\"
	mkdir newpath$
	REM These two tests will not run under the test harness.
	IF NOT hRunningUnderSystemTestApp%:	
		copy "pDaysA.opo",newpath$+"pDaysA.opo"
		if not exist ("pdoc.opo") : raise 2 : endif
	ENDIF
endp


REM End of pSetPath.tpl

