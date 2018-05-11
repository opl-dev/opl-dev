REM tString.tpl
REM EPOC OPL automatic test code for strings.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

DECLARE EXTERNAL
EXTERNAL strAdd:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tString", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tString:
	rem hInitTestHarness:(KhInitTargetHandlesErrors%, KhInitNotUsed%)

	hRunTest%:("doString")
rem	hCleanUp%:("CleanerUpper")
endp


REM--------------------------------------------------------------------------

proc doString:
rem  strtest:("String Opcodes")
	strAdd:
	rem	strAss:	:rem does not exist
rem  endTestX:
endp

REM--------------------------------------------------------------------------

proc strAdd:
	local even8$(8),odd9$(9),bigEven$(254),bigOdd$(255)

	even8$="A"+"B"
	rem print "expecting AB",even8$
	if even8$<>"AB" : raise 1 :endif

	even8$="A"+"Bc"
	rem print "expecting ABc",even8$
	if even8$<>"ABc" : raise 2 :endif

	even8$="Ab"+"Cd"
	rem print "expecting AbCd",even8$
	if even8$<>"AbCd" : raise 3 :endif

	even8$="Ab"+"C"
	rem print "expecting AbC",even8$
	if even8$<>"AbC" : raise 4 :endif

	even8$="12345678"+""
	if even8$<>"12345678" : raise 5 : endif
	even8$="1234567"+"8"
	if even8$<>"12345678" : raise 6 : endif
	even8$="123456"+"78"
	if even8$<>"12345678" : raise 7 : endif
	even8$="12345"+"678"
	if even8$<>"12345678" : raise 8 : endif
	even8$="1234"+"5678"
	if even8$<>"12345678" : raise 9 : endif
	even8$="123"+"45678"
	if even8$<>"12345678" : raise 10 : endif
	even8$="12"+"345678"
	if even8$<>"12345678" : raise 11 : endif
	even8$="1"+"2345678"
	if even8$<>"12345678" : raise 12 : endif
	even8$=""+"12345678"
	if even8$<>"12345678" : raise 13 : endif
		
	odd9$="123456789"+""
	if odd9$<>"123456789" : raise 14 : endif
	odd9$="1234567"+"89"
	if odd9$<>"123456789" : raise 15 : endif
	odd9$="123456"+"789"
	if odd9$<>"123456789" : raise 16 : endif
	odd9$="12345"+"6789"
	if odd9$<>"123456789" : raise 17 : endif
	odd9$="1234"+"56789"
	if odd9$<>"123456789" : raise 18 : endif
	odd9$="123"+"456789"
	if odd9$<>"123456789" : raise 19 : endif
	odd9$="12"+"3456789"
	if odd9$<>"123456789" : raise 20 : endif
	odd9$="1"+"23456789"
	if odd9$<>"123456789" : raise 21 : endif
	odd9$=""+"123456789"
	if odd9$<>"123456789" : raise 22 : endif
	
	bigEven$=rept$("X",254)+""
	if bigEven$<>rept$("X",254) : raise 23 : endif
	bigEven$=""+rept$("X",254)
	if bigEven$<>rept$("X",254) : raise 24 : endif
	bigEven$=""+""+""+""+rept$("X",254)+""+""+""+""
	if bigEven$<>rept$("X",254) : raise 25 : endif
	bigEven$=rept$("X",127)+rept$("X",127)
	if bigEven$<>rept$("X",254) : raise 26 :endif
	bigEven$=rept$("X",127)+rept$("X",126)
	if bigEven$<>rept$("X",253) : raise 27 :endif

	bigOdd$=rept$("X",255)+""
	if bigOdd$<>rept$("X",255) : raise 28 : endif
	bigOdd$=""+rept$("X",255)
	if bigOdd$<>rept$("X",255) : raise 29 : endif
	bigOdd$=""+""+""+""+rept$("X",255)+""+""+""+""
	if bigOdd$<>rept$("X",255) : raise 30 : endif
	bigOdd$=rept$("X",128)+rept$("X",127)
	if bigOdd$<>rept$("X",255) : raise 31 :endif
	bigOdd$=rept$("X",127)+rept$("X",128)
	if bigOdd$<>rept$("X",255) : raise 32 :endif
	bigOdd$=rept$("X",127)+rept$("X",127)
	if bigOdd$<>rept$("X",254) : raise 33 :endif

	onerr err1::
	even8$="ABCD"+"EFGHI"
	raise 34	: rem should be too big
err1::
	onerr off
	if err<>KErrStrTooLong%
		rem print err$(err)
		rem pause 30
		raise 35
	endif

	onerr err2::
	bigEven$=rept$("Y",128)+rept$("Z",127)
	raise 36: rem should be too big
err2::
	onerr off
	if err<>KErrStrTooLong%
		rem print err$(err)
		rem pause 30
		raise 37
	endif
	onerr err3::
	bigEven$=rept$("Y",127)+rept$("Z",128)
	raise 38	: rem should be too big
err3::
	onerr off
	if err<>KErrStrTooLong%
		rem print err$(err)
		rem pause 30
		raise 39
	endif

	onerr err4::
	bigOdd$=rept$("Y",128)+rept$("Z",128)
	raise 40	: rem should be too big
err4::
	onerr off
	if err<>KErrStrTooLong%
		rem print err$(err)
		rem pause 30
		raise 41
	endif
	onerr err5::
	bigOdd$=rept$("Y",127)+rept$("Z",129)
	raise 42	: rem should be too big
err5::
	onerr off
	if err<>KErrStrTooLong%
		rem print err$(err)
		rem pause 30
		raise 43
	endif
	onerr err6::
	bigOdd$=rept$("Y",129)+rept$("Z",130)
	raise 44	: rem should be too big
err6::
	onerr off
	if err<>KErrStrTooLong%
		rem print err$(err)
		rem pause 30
		raise 45
	endif
endp

REM End of tString.tpl
