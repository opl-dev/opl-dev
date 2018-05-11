REM gTMode.tpl
REM EPOC OPL automatic test code for gTMode
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gtmode", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gtmode:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dogtmode")
rem	hCleanUp%:("CleanUp")
rem	KLog%:(KhLogHigh%,"Some sample text")
endp


proc dogTMode:
	local TMode%,ind%
	local val%(12),ix%,len%

	rem Expected values 0=clear,1=mixed,2=set
	rem Odd indices for clear background, even indices for filled

	gCreate(0,0,gWidth,gHeight-9,1)
	val%(1)=1	:rem default GC (big font,textmode=set)
	val%(2)=2
	val%(3)=1	:rem textMode=set
	val%(4)=2
	val%(5)=0	:rem textMode=clear
	val%(6)=1
	val%(7)=1	:rem textMode=toggle
	val%(8)=1
	val%(9)=1	:rem textMode=replace
	val%(10)=1

	rem Use default GC (font=big,textMode=set)
	gAt 0,15
	ix%=1
	gPrint "tmode=default"
	pkCheck%:(val%(ix%),ix%,1,2)

	ind%=gWidth/2
	gAt ind%,0
	gFill ind%,gHeight,0
	gAt ind%,15
	ix%=ix%+1
	gPrint "tmode=default"
	pkCheck%:(val%(ix%),ix%,1,2)

 	gFont 4				:rem use mono font
	tmode%=0
	while tmode%<=3
		gtmode tmode%
		gAt 0,gY+13
		gPrint "tmode=";tmode%
		ix%=ix%+1
		pkCheck%:(val%(ix%),ix%,1,2)	:rem 1 is pixels up from baseline to "|" with mono font
		gAt ind%,gY

		ix%=ix%+1
		gPrint "tmode=";tmode%
		pkCheck%:(val%(ix%),ix%,1,2)

		tmode%=tmode%+1
	endwh

	rem at 12,9 :print "Done";
	rem pause -50 :key
	gclose gidentity
endp


proc pkCheck%:(val%,ix%,len%,up%)
		local bits%(2),mask%,oldId%
		local setting$(3,10)
		local success%

		setting$(1)="Clear"
		setting$(2)="Mixed"
		setting$(3)="Set"
		gPeekLine gIdentity,gX-16,gY-4,#addr(bits%()),16
		if val%=0
			success%=(bits%(1)=0)
		elseif val%=2
			success%=(bits%(1)=$ffff)
		else
			success%=((bits%(1)<>0) and (bits%(1)<>$ffff))
		endif
		if not success%
				oldId%=gIdentity
				gOrder 1,1
				raise 1
				rem at 1,8
				rem print "Peeked",bits%(1),"and expect",setting$(val%+1)
				rem print "val() index=";ix%
				rem get
				gOrder oldId%,1
				cursor oldId%
				rem get
				cursor off
		endif
endp

REM End of gtmode.tpl
