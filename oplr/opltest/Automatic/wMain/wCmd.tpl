REM wCmd.tpl
REM EPOC OPL automatic test code for CMD.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "System.oxh"
DECLARE EXTERNAL

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wCmd", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc wCmd:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dowcmd")
	rem hCleanUp%:("CleanUp")
endp

CONST KAppName$="OplSysTest"

proc dowcmd:
	LOCAL report%
	IF hRunningUnderSystemTestApp%:
		IF LOWER$(CMD$(1))<>LOWER$(hDiskName$:+"\System\Apps\"+KAppName$+"\"+KAppName$+".app")
			print "1=";CMD$(1)
			print "Expected=";hDiskName$:+"\System\Apps\"+KAppName$+"\"+KAppName$+".app"
			report%=report%+1
		ENDIF
		REM Assumes the default disk is C:.
		IF LOWER$(CMD$(2))<>LOWER$("c:\Documents\"+KAppName$)
			print "2=";CMD$(2)
			print "Expected=";LOWER$("c:\Documents\"+KAppName$)
			report%=report%+2
		ENDIF
	ELSE
		IF LOWER$(CMD$(1))<>LOWER$(hDiskName$:+"\Opltest\Automatic\wMain\wCmd.opo")
			print "1=";CMD$(1)
			print hDiskName$:+"\Opltest\Automatic\wMain\wCmd.opo"
			report%=report%+4
		ENDIF
		IF LOWER$(CMD$(2))<>LOWER$(hDiskName$:+"\Documents\wCmd")
			print "2=";CMD$(2)
			print LOWER$(hDiskName$:+"\Documents\wCmd")
			report%=report%+8
		ENDIF
	ENDIF
	IF CMD$(3)<>KSyRunAppOpl$
		hLog%:(KhLogAlways%,"CMD$(3)=["+CMD$(3)+"]")
		report%=report%+16
	ENDIF

	IF report%
		RAISE report%
	ENDIF
ENDP

REM End of wCmd.tpl

