REM gPeekbit.tpl
REM EPOC OPL automatic test code for gPeekbit.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.
REM
REM Included tests for RGB mode in gPEEKLINE (for 
REM feature [734702] Graphics commands.)


INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "gPeekbit", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc gpeekbit:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("pk")
	hRunTest%:("fillLine")
	hRunTest%:("peekRGB")
	hRunTest%:("peeklinetest")
rem	hCleanUp%:("CleanUp")
endp


proc fillLine:
	local x%,y%,eX%,eY%,dx%,gap%
	gCls
	rem at 1,8
	rem print "Fill line between points set"
	x%=1 :y%=40
	gAt x%,y%
	gap%=154
	gLineBy 0,0
	gMove gap%,0
	gLineBy 0,0
	gMove -gap%,0
	dx%=scan:
	rem at 1,1 :print dx%;" points  "
	gLineBy dx%,0
	gMove -dx%,1
	rem pause pause%
	gcls
endp


proc scan:
	local i%,mask%,l%,r%,buf%(1000),perLine%,bit%,word%

	if gWidth>1000 :raise 1 :endif
	gPeekLine gIdentity,gX,gY,buf%(),gWidth-gX
rem 	printBuf:(addr(buf%()))

	perLine%=gWidth/16+1
	while i%<perLine%
		i%=i%+1
		word%=buf%(i%)
		if word%
			mask%=1
			bit%=0
			while bit%<15
				if l%=0
					if word% and mask%
						l%=i%*16+bit%
						word%=word% and (not mask%)	:rem mask out left-hand bit
						continue		:rem test rest of bits in word for right value
					endif
				else
					if word% and mask%
						r%=i%*16+bit%
						return r%-l%
					endif
					if mask%=$4000
						mask%=$8000
					else
						mask%=mask%*2
					endif
					bit%=bit%+1				
				endif				
			endwh
		endif
	endwh
endp


proc printBuf:(pS&)
	local p&,pE&

	p&=pS&
	pE&=pS&+30
	at 1,2
 	while p&<pE&
rem 		print "$";hex$(peekW(p&)),
 		p&=p&+2
 	endwh
 	rem pause pause%
endp


proc pk:
	local buf&(1000),i%, x%,y%

	rem print "gPeekLine bits below"
	x%=10 :y%=20
	gAt x%,y%
	while i%<16
		i%=i%+1
		gLineBy 1,0
		gMove 1,0
	endwh
	gPeekLine 1,x%,y%,#addr(buf&()),32
	rem print "Peeked &";hex$(buf&(1))
	if buf&(1)<>&55555555
		raise 1
		rem print "Expect &55555555" :get :raise 1
	endif
	rem pause pause%
endp

proc peekRGB:
local X%,R%,G%,B%
local D&(15),RP&,GP&,BP&
	X%=0
	R%=0
	G%=0
	B%=0
	gCls
	while X%<15
		if X%<5
			R%=R%+51
		elseif X%<10
			G%=G%+51
		else
			B%=B%+51
		endif
		gColor R%,G%,B%
		gAt X%,0
		gLineBy 0,2
		X%=X%+1
	endwh
	gPeekLine 1,0,0,#ADDR(D&()),15,3
	X%=0
	R%=0
	G%=0
	B%=0
	while X%<15
		if X%<5
			R%=R%+51
		elseif X%<10
			G%=G%+51
		else
			B%=B%+51
		endif
		BP&=D&(X%+1)/&10000 and $FF
		GP&=D&(X%+1)/&100 and $FF
		RP&=D&(X%+1)/&1 and $FF
		if R%<>RP& or G%<>GP& or B%<>BP&
			rem Print "expected ("+NUM$(R%,3)+","+NUM$(G%,3)+","+NUM$(B%,3)+"). Got ("+NUM$(RP&,3)+","+NUM$(GP&,3)+","+NUM$(BP&,3)+")" : Get
			raise 2
		endif
		X%=X%+1
	endwh
	rem Print "Test successful" : Get
endp


PROC PEEKLINETEST:
	TestBitmap:(KgCreate4KColorMode%)
ENDP

PROC TestBitmap:(Mode%)
LOCAL d&(6),ID%
GLOBAL Red&,Green&,Blue&
	ID%=gCREATEBIT(10,10,Mode%)
	gCOLOR 51,153,255
	gAT 0,0
	gFILL 2,10,0
	gAT 2,0
	gCOLOR 204,68,51
	gFILL 2,10,0
	gPEEKLINE ID%,1,3,#ADDR(D&()),3,3
	GetRGB:(D&(1))
	PRINT(NUM$(Red&,10)+","+NUM$(Green&,10)+","+NUM$(Blue&,10))
	GetRGB:(D&(2))
	PRINT(NUM$(Red&,10)+","+NUM$(Green&,10)+","+NUM$(Blue&,10))
	gCLOSE ID%
ENDP

PROC GetRGB:(D&)
LOCAL V&
	Blue&=D&/KRgbRedPosition& AND KRgbColorMask&
	Green&=D&/KRgbGreenPosition& AND KRgbColorMask&
	Red&=D& AND KRgbColorMask&
ENDP



REM End of gPeekbit.tpl
