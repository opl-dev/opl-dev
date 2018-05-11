REM gtwidth.tpl
REM EPOC OPL automatic test code for gtwidth.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gtwidth", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gtwidth:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogTWidth")
endp


proc dogTWidth:
	local a$(10),b$(10),i%,j%,x%
	
	rem print "     Test gTWidth"
	gAt 1,20
	j%=0
	i%=%a
	while i%<=%z
		while x%<gHeight/17 and i%<=%z
			a$=chr$(i%)
			i%=i%+1
			x%=x%+1
			gAt 20+j%*100-gTWidth(a$), 10+16*x%
			gPrint a$,":",gTWidth(a$)
		endwh
		x%=0
		j%=j%+1
	endwh
rem	gStyle 3
rem	gFont 1
rem	if gTWidth(chr$(%i))<>3 :raise 1 :endif
rem	pause -30 :key
	if gTWidth(chr$(%l))>gTWidth(chr$(%m))
		raise 1
	endif
	gcls
endp

REM End of gTwidth.tpl
