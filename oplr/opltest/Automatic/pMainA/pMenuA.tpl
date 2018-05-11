REM pMenuA.tpl
REM EPOC OPL automatic test code for MENU
REM Copyright (c) 2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pMenuA", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pMenuA:
	hRunTest%:("tMenuCascTooWide")
	hRunTest%:("tMenuDimmedCascTooWide")
endp


PROC tMenuCascTooWide:
	LOCAL m%
	onerr menuErr
	mInit
	mCasc "1234567890123456789012345678901234567890","Save",%s,"Export",%e,"Import",%i
	mCasc "Printing","Page Setup",%u,"Print Preview",%w,"Print",%p
	mCard "File","New",%n,"Open",%o,"1234567890123456789012345678901234567890>",16,"Printing>",17,"Close",%e
	mCard "Edit","Cut",%x,"Copy",%c,"Paste",%v,"Find",%f
	REM The Menu keyword should raise a KErrTooWide% error.
	m%=menu
	ONERR OFF
	RAISE 1
menuErr::
	onerr off 
	if err<>KErrTooWide%
		RAISE ERR
	endif
ENDP


PROC tMenuDimmedCascTooWide:
	LOCAL m%
	onerr menuErr2
	mInit
	mCasc "1234567890123456789012345678901234567890","Save",%s,"Export",%e,"Import",%i
	mCasc "Printing","Page Setup",%u,"Print Preview",%w,"Print",%p
	mCard "File","New",%n,"Open",%o,"1234567890123456789012345678901234567890>",16 OR KMenuDimmed%,"Printing>",17,"Close",%e
	mCard "Edit","Cut",%x,"Copy",%c,"Paste",%v,"Find",%f
	REM The Menu keyword should raise a KErrTooWide% error.
	m%=menu
	ONERR OFF
	RAISE 2
menuErr2::
	onerr off 
	if err<>KErrTooWide%
		RAISE ERR
	endif
ENDP


REM End of PMenuA.tpl
