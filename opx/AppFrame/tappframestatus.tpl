rem tappframestatus.tpl
rem Series 60 test code

INCLUDE "const.oph"
INCLUDE "appframe.oxh"

DECLARE EXTERNAL

PROC tappframestatus:
	local vis%
	REM With defect 817867 "reset the default window" fixed,
	REM we don't need to use the AT 1,5 anymore...
rem	AT 1,5
	PRINT "tAppFrameStatus test code"
	PRINT "Press any key to start."
	GET
	AfSetStatusVisible%:(0)
	PRINT "Have set the status invisible."
	PRINT "Press any key."
	GET
	
	PRINT "About to start looping - press C to quit"
	WHILE 1
		PRINT datim$,
		IF vis%
			vis%=0
		ELSE
			vis%=1
		ENDIF
		afsetstatusvisible%:(vis%)
		IF GET=8
			BREAK
		ENDIF
	ENDWH
ENDP
