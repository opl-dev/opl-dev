REM wAlert.tpl
REM EPOC OPL interactive test code for wAlert.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wAlert", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC wAlert:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("doWalert")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


proc dowAlert:
	local r%

	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	PRINT "Hit Esc."
	IF alert("messg1")<>1 :RAISE 1: ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","messg2")<>1 :RAISE 2 :ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","messg2","resp1")<>1 :RAISE 3 :ENDIF

	PRINT "Hit Enter."
	IF alert("messg1","messg2","resp1","resp2")<>2 :RAISE 4 :ENDIF
	PRINT "Hit Esc."
	IF alert("messg1","messg2","resp1","resp2")<>1 :RAISE 5 :ENDIF

	PRINT "Hit Enter."
	IF alert("messg1","messg2","resp1","resp2","resp3")<>2 :RAISE 6: ENDIF
	PRINT "Hit Space."
	IF alert("messg1","messg2","resp1","resp2","resp3")<>3 :RAISE 7: ENDIF
	PRINT "Hit Esc."
	IF alert("messg1","messg2","resp1","resp2","resp3")<>1 :RAISE 8: ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","")<>1 :RAISE 9: ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","","resp1")<>1 :RAISE 10: ENDIF

	PRINT "Hit Enter."
	IF alert("messg1","2","resp1","resp2")<>2 :RAISE 11 :ENDIF
	PRINT "Hit Esc."
	IF alert("messg1","","resp1","resp2")<>1 :RAISE 12 :ENDIF

	PRINT "Hit Enter."
	IF alert("messg1","","resp1","resp2","resp3")<>2 :RAISE 13: ENDIF
	PRINT "Hit Space."
	IF alert("messg1","","resp1","resp2","resp3")<>3 :RAISE 14: ENDIF
	PRINT "Hit Esc."
	IF alert("messg1","","resp1","resp2","resp3")<>1 :RAISE 15: ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","","")<>1 :RAISE 16: ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","","","")<>1 :RAISE 17: ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","","","","")<>1 :RAISE 18: ENDIF

	PRINT "Hit Esc."
	IF alert("messg1","","","junk")<>1 :RAISE 19: ENDIF
	rem print "This case does the wrong thing in 032!"

	PRINT "Hit Esc."
	IF alert("messg1","","","","junk")<>1 :RAISE 20: ENDIF
endp


REM End of wAlert.tpl
