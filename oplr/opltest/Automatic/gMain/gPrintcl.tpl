REM gPrintcl.tpl
REM EPOC OPL automatic test code for gprint clipping.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gprintcl", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gprintcl:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogprintcl")
rem	hCleanUp%:("CleanUp")
endp


proc dogPrintCl:
	local l%,boxX%,boxW%,a$(255),i%,j%,gap%
	
	gfont 10
	gAt 1,13 :gPrint "Print clipped text (clipped to box)"
	a$="Clipped inside box"
	j%=18

	gap%=8
	i%=0
	boxX%=8
	while i%<=gTWidth(a$)
		boxW%=i%+1
		gAt boxX%,j%
		gBox boxW%,20
		gAt boxX%+1,j%+14
		gfont 10
		l%=gPrintClip(a$,i%)
		gAt boxX%+1,j%+30 :gfont 9
		gPrint l%
		boxX%=boxX%+boxW%+gap%
		i%=i%+1
		if boxX%+i% > gWidth
			j%=j%+32
			boxX%=8
		endif
	endwh
	gcls
endp

REM End of gprintcl.tpl

