REM gOrder.tpl
REM EPOC OPL automatic test code for gORDER.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gOrder", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gOrder:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("gOrderEr")
	hRunTest%:("gOrder1")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	gcls
ENDP

proc gOrderEr:
	local bitId%

	cls
	gUse 1
	rem print
	rem print "gOrder a bitmap"
	bitId%=gCreateBit(100,100)
	onerr e
	gOrder bitId%,1
	onerr off
	raise 1
e::
	onerr off
	if err<>-119 : raise 2 : print err$(err) : raise 2 : endif
	gClose bitId%
endp


proc gOrder1:
	local id%(8),i%,height%,wid1%,r%,tw%
	local chrW%,chrH%  rem character width and height
	local info%(10)
	local tlRow%  rem Y value for SCREEN command

	screeninfo info%()
	chrW%=info%(7) :chrH%=info%(8)

	cls
	gUse 1
	tlRow%=(gHeight+1)/chrH%-1
	screen 13,1,2,tlRow%
	gAt 0,tlRow%*chrH%-10-chrH%
	gBox 15*chrW%,chrH%+20
	wid1%=gWidth
	id%(1)=1	
	i%=1
	height%=20
	gAt 2,gHeight/2-5
	gPrint "WINDOW 1"
	gFont 11
  tw%=gTWidth("Window 0")
	while i%<8
		i%=i%+1
		id%(i%)=gCreate(wid1%-(tw%+90)+i%*10,(height%-10)*(i%-3)+10,tw%+6,height%,1)
		gFont 6
		gBox gWidth,gHeight
		gAt 3,9
		gPrint "window",i%
	endwh

	i%=8
	while i%>=1
		rem print "gOrder",i%;",1"
		gOrder id%(i%),1
		gUse id%(i%)
		if gRank<>1 :raise 1 :endif
		rem pause pause% :key
		i%=i%-1
	endwh

	gUse 1
	i%=0
	while i%<=9
		rem print "gOrder 1,";i%
		gOrder 1,i%
		r%=i%
		if i%=0 : r%=1 :elseif i%>8 :r%=8 :endif
		if gRank<>r%
			hLog%:(khLogAlways%,"ERROR: gRank "+GEN$(gRANK,3)+" <> expected "+GEN$(r%,3))
			rem at 12,1 :print "?" 
			rem print "RANK=";gRank,"exp",r%
			rem raise 2
		endif
		rem pause pause% :key
		i%=i%+1
	endwh

	i%=2
	rem print "Closing..."
	while i%<=8
		gClose id%(i%)
		i%=i%+1
	endwh
endp

REM End of gOrder.tpl

