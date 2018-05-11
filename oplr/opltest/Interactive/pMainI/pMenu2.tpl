REM pMenu2.tpl
REM EPOC OPL interactive test code for further menu tests.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

DECLARE EXTERNAL
EXTERNAL menueg%:(init%)

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pMenu2", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pMenu2:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("tMenuInit")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
	rem hCleanUp%:("Reset")
ENDP


PROC Reset:
	rem Any clean-up code here.
ENDP


proc tmenuinit:
	local i%,mult%,init%
	
	cls

	print :print :print :print :print :print :print :print :print 
	print :print :print :print :print :print :print :print :print 

	print "Testing initialized choice of menu item"

REM First menu card.
	PRINT :PRINT "Choose File | New"
	hLog%:(Khlogalways%,"File | New")
	IF menueg%:(256+0)<>%n :raise 1 :endif

	PRINT :PRINT "Choose File | Open"
	hLog%:(Khlogalways%,"File | Open")
	IF menueg%:(256+1)<>%o :raise 2 :endif

	PRINT :PRINT "Choose File | More | Save"
	hLog%:(Khlogalways%,"File | More | Save")
	IF menueg%:(256+2)<>%s :raise 3 :endif

	PRINT :PRINT "Choose File | Printing | Page setup"
	hLog%:(Khlogalways%,"File | Printing | Page setup")
	IF menueg%:(256+3)<>%u :raise 4 :endif

	PRINT :PRINT "Choose File | Close"
	hLog%:(Khlogalways%,"File | Close")
	IF menueg%:(256+4)<>%e :raise 5 :endif

	REM Back to the top again.
	PRINT :PRINT "Choose File | New"
	hLog%:(Khlogalways%,"File | New")
	IF menueg%:(256+5)<>%n :raise 6 :endif

	REM 256-1
	PRINT :PRINT "Choose File | New"
	hLog%:(Khlogalways%,"File | New")
	IF menueg%:(256+255)<>%n :raise 10 :endif

REM Second card.
	PRINT :PRINT "Choose Edit | Cut"
	hLog%:(Khlogalways%,"Edit | Cut")
	IF menueg%:(256+256)<>%x :raise 11 :endif

	PRINT :PRINT "Choose Edit | Copy"
	hLog%:(Khlogalways%,"Edit | Copy")
	IF menueg%:(256+257)<>%c :raise 12 :endif

	PRINT :PRINT "Choose Edit | Paste"
	hLog%:(Khlogalways%,"Edit | Paste")
	IF menueg%:(256+258)<>%v :raise 13 :endif

	PRINT :PRINT "Choose Edit | Find | Find next"
	hLog%:(Khlogalways%,"Edit | Find | Find next")
	IF menueg%:(256+259)<>%t :raise 14 :endif

	REM Back to the top again.
	PRINT :PRINT "Choose Edit | Cut"
	hLog%:(Khlogalways%,"Edit | Cut")
	IF menueg%:(256+260)<>%x :raise 15 :endif
	hLog%:(Khlogalways%,"That's the end...")
endp


proc menueg%:(init%)
	local i%,m%
	i%=init%
	mInit
	mCasc "More","Save",%s,"Export",%e,"Import",%i
	mCasc "Printing","Page setup",%u,"Print preview",%w,"Print",%p
	mCasc "Find","Find next",%t,"Replace",%r,"Go to",%g
	mCard "File","New",%n,"Open",%o,"More>",16,"Printing>",17,"Close",%e
	mCard "Edit","Cut",%x,"Copy",%c,"Paste",%v,"Find>",18
	m%=menu(i%)
	RETURN m%
endp


REM End of menu2.tpl

