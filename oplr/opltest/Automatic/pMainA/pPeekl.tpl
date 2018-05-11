REM pPeekL.tpl
REM EPOC OPL automatic test code for PeekLine.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

rem DECLARE EXTERNAL

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

EXTERNAL TestMode:(aMode%)
EXTERNAL tSwitchDefaultMode:
EXTERNAL tBusy:
EXTERNAL tMoreWindows:
EXTERNAL ColorLine:(aWindowId%,aColorIndex%,aMode%)
EXTERNAL ColorAndBase&:(aColorIndex%,aMode%)


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pPeekL", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pPeekL:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("testPLines")
	rem	print "Opler1 Peekline Tests Finished OK"
ENDP

REM Peek different lengths of line,
REM and in different start locations.

PROC testPLines:
	TestMode:(KColorgCreate2GrayMode%)
	TestMode:(KColorgCreate4GrayMode%)
	TestMode:(KColorgCreate16GrayMode%)

	tBusy:
	tSwitchDefaultMode:
	tMoreWindows:
ENDP


CONST KMaxColorIndex%=20 rem for now...

PROC ColorAndBase&:(aColorIndex%,aMode%)
	EXTERNAL peekmask&
	LOCAL color&,red%,green%,blue%
	LOCAL sumColor%
	LOCAL b%(8) REM Base peek values for each mode.
	LOCAL lookupIndex%
	DO
		VECTOR aColorIndex%
			c0,c1,c2,c3,c4,c5,c6,c7,c8
			c9,c10,c11,c12,c13,c14,c15
			c16,c17,c18,c19
		ENDV
		RAISE 10 REM Parameter too big.
REM                    1=2Gray  2=4Gray   3=16Gray
c0::   color&=&000000 :b%(1)=0 :b%(2)=&0 :b%(3)=&0 :BREAK
c1::   color&=&505050 :b%(1)=0 :b%(2)=&1 :b%(3)=&5 :BREAK
c2::   color&=&A0A0A0 :b%(1)=1 :b%(2)=&2 :b%(3)=&a :BREAK
c3::   color&=&F0F0F0 :b%(1)=1 :b%(2)=&3 :b%(3)=&F :BREAK
c4::   color&=&101010 :b%(1)=0 :b%(2)=&0 :b%(3)=&1 :BREAK
c5::   color&=&202020 :b%(1)=0 :b%(2)=&0 :b%(3)=&2 :BREAK
c6::   color&=&303030 :b%(1)=0 :b%(2)=&0 :b%(3)=&3 :BREAK
c7::   color&=&404040 :b%(1)=0 :b%(2)=&1 :b%(3)=&4 :BREAK
c8::   color&=&606060 :b%(1)=0 :b%(2)=&1 :b%(3)=&6 :BREAK
c9::   color&=&707070 :b%(1)=0 :b%(2)=&1 :b%(3)=&7 :BREAK
c10::  color&=&808080 :b%(1)=1 :b%(2)=&2 :b%(3)=&8 :BREAK
c11::  color&=&909090 :b%(1)=1 :b%(2)=&2 :b%(3)=&9 :BREAK
c12::  color&=&B0B0B0 :b%(1)=1 :b%(2)=&2 :b%(3)=&B :BREAK
c13::  color&=&C0C0C0 :b%(1)=1 :b%(2)=&3 :b%(3)=&C :BREAK
c14::  color&=&D0D0D0 :b%(1)=1 :b%(2)=&3 :b%(3)=&D :BREAK
c15::  color&=&E0E0E0 :b%(1)=1 :b%(2)=&3 :b%(3)=&E :BREAK
c16::  color&=&012345 :b%(1)=0 :b%(2)=&0 :b%(3)=&1 :BREAK
c17::  color&=&6789AB :b%(1)=1 :b%(2)=&2 :b%(3)=&8 :BREAK
c18::  color&=&CDEF01 :b%(1)=1 :b%(2)=&3 :b%(3)=&C :BREAK
c19::  color&=&ED98A1 :b%(1)=1 :b%(2)=&2 :b%(3)=&A :BREAK
	UNTIL 0
	red%=color&/KRgbRedPosition& AND KRgbColorMask&
	green%=color&/KRgbGreenPosition& AND KRgbColorMask&
	blue%=color& AND KRgbColorMask&
	REM print "Color:",red%;"(&";HEX$(red%);")",	green%;"(&";HEX$(green%);")", blue%;"(&";HEX$(blue%);")"
	gCOLOR red%,green%,blue%

	sumColor%=red%+green%+blue%
	IF aMode%=KColorgCreate2GrayMode%
		peekmask&=1 :lookupIndex%=1
	ELSEIF aMode%=KColorgCreate4GrayMode%
		peekmask&=3 :lookupIndex%=2
	ELSEIF aMode%=KColorgCreate16GrayMode%
		peekmask&=15 :lookupIndex%=3
	ELSEIF aMode%=KColorgCreate4KColorMode%
		lookupIndex%=4 : rem peekmask&=???
	ENDIF
	RETURN b%(lookupIndex%)
ENDP


PROC TestMode:(aMode%)
	LOCAL id%,colorIndex%
	REM PRINT "TestMode: aMode%=";aMode%
	id%=gCREATE(400,0, 100,100, 1,aMode%)
	IF id%=0 : RAISE aMode% : ENDIF
	DO
		colorIndex%=colorIndex%+1
		ColorLine:(id%,colorIndex%,aMode%)
	UNTIL colorIndex%=KMaxColorIndex%
	gCLOSE id%
ENDP


PROC ColorLine:(aWindowId%,aColorIndex%,aMode%)
	GLOBAL peekmask&
	LOCAL a%(640),len%,mode%,base&
	len%=80 :mode%=aMode%
REM LEN 1, 15, 16, 17, 31,32, 99
REM START 0,100
	base&=ColorAndBase&:(aColorIndex%,aMode%)
	REM Use the color index for the y position.
	gAT 0,aColorIndex%
	gLINEBY 30,0
	gPEEKLINE aWindowId%, 2,aColorIndex%, a%(), len%, mode%
rem	print "Peek123: "+HEX$(a%(1)),HEX$(a%(2)),HEX$(a%(3)),
	IF (a%(1) AND peekmask&) <> base&
		print "Failed for colorindex="+GEN$(aColorIndex%,4)
		print "Expecting Base of &"+HEX$(base&)
		print "Got a%(1) masked(%"+hBIN$:(a%(1) AND peekmask&)+")",
		print "where mask=&"+HEX$(peekmask&)
		RAISE aColorIndex%
		REM IF GET=27 :STOP :ENDIF
	ENDIF
ENDP


PROC tBusy:
	BUSY "Busy text",KBusyBottomRight%
	TestMode:(KColorgCreate2GrayMode%)
	TestMode:(KColorgCreate4GrayMode%)
	TestMode:(KColorgCreate16GrayMode%)
	BUSY OFF
ENDP


PROC tSwitchDefaultMode:
	DEFAULTWIN KColorDefWin16GrayMode%
	TestMode:(KColorgCreate16GrayMode%)
ENDP


PROC tMoreWindows:
	LOCAL id%
	id%=gCREATE(0,0,gWIDTH,gHEIGHT,1,KColorgCreate16GrayMode%)
	REM Remind us that it's there...
	gXBORDER 2,$194
	TestMode:(KColorgCreate16GrayMode%)
	gCLOSE id%
ENDP


REM ///////////////////////////////////////////////////////////////////////////
REM ///////////////////////////////////////////////////////////////////////////
REM ///////////////////////////////////////////////////////////////////////////
REM Ignoring the following code:


proc OLDpPeekL:
	global mode%,a%(64)
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	rem print "These tests are automated"
	rem print "For 4 and 16 colour tests only array values which relate to the"
	rem print "line are shown, but all values are checked"

	hRunTest%:("testlines")
	hRunTest%:("testerror")
	rem	print "Opler1 Peekline Tests Finished OK"
endp


proc testlines:
	mode%=-1
	line:(100,0)
	line:(50,0)

if 1
	line:(300,0)
	line:(10,0)
endif

	mode%=0
	line:(100,0)
	line:(50,0)

if 1
	line:(300,0)
	line:(10,0)
endif

	mode%=1
	line:(60,0)	

if 0
return
endif

	line:(60,$10)
	line:(60,$20)
	line:(60,$30)
	line:(60,$40)
	line:(60,$50)	
	line:(60,$60)
	line:(60,$70)
	line:(60,$80)
	line:(60,$90)
	line:(60,$A0)
	line:(60,$B0)
	line:(60,$C0)
	line:(60,$D0)
	line:(60,$E0)
	line:(60,$F0)
	mode%=2
	line:(60,0)	
	line:(60,$10)
	line:(60,$20)
	line:(60,$30)
	line:(60,$40)
	line:(60,$50)	
	line:(60,$60)
	line:(60,$70)
	line:(60,$80)
	line:(60,$90)
	line:(60,$A0)
	line:(60,$B0)
	line:(60,$C0)
	line:(60,$D0)
	line:(60,$E0)
	line:(60,$F0)
endp


proc testerror:
	local id%
	
	rem print "Test ORing id% with $8000 raises error"
	
	id%=gcreate (0,0,gwidth,gheight,1,1)
	gcolor 16,16,16
	gat 100,100
	glineto 200,100
	onerr inval
	gpeekline (id% or $8000),100,100,a%(),256,1
	onerr off
	raise 1
inval::
	onerr off
	if err<>-2
		rem print err$(err)
		raise 2
	endif
	gclose id%
	
	pause 20
endp


proc line:(len%,r%)
	local id%,col%,kid%
	
	rem draws a horizontal line of length len% from 100,100
	rem in the colour given by gcolor r%,r%,r% and peeks 
	rem 256 pixel line from 100,100 in the specified mode%
	
	if mode%=-1 or mode%=0 : col%=0 
	else col%=mode% : endif
	id%=gcreate (0,0,gwidth,gheight,1,col%)
	kid%=id%
	gcolor r%,r%,r%
	gat 100,100
	glineby len%,0
	gpeekline id%,100,100,a%(),256,mode%
	id%=kid%
	gcls
	gclose id%
	
	if mode%=-1 : check2b:(len%) : endif
	if mode%=0 : check2w:(len%) : endif
	if mode%=1 : check4:(len%,r%) : endif
	if mode%=2 : check16:(len%,r%) : endif
	
	REM pause 10
	cls
endp


proc check2b:(len%)
	local set%,rem%,end%,i%
	
	set%=len%/16
	rem%=len%-set%*16
	while rem%>0
		rem%=rem%-1
		end%=end%+2**rem%
	endwh	
	
	rem print "Mode = ";mode%
	rem print
	
	i%=1
	while i%<10
		rem print i%;".  ";right$(hex$(a%(i%)),4)
		if i%<=set%
			if a%(i%)<>$ffff : raise i% : endif
		else
			if i%=set%+1
				if a%(i%)<>end% : raise i% : endif
				else if a%(i%)<>0 : raise i% : endif
			endif
		endif
		i%=i%+1
	endwh
	while i%<=16
		rem print i%;". ";right$(hex$(a%(i%)),4)
				if i%<=set%
			if a%(i%)<>$ffff : raise i% : endif
		else
			if i%=set%+1
				if a%(i%)<>end% : raise i% : endif
				else if a%(i%)<>0 : raise i% : endif
			endif
		endif
		i%=i%+1
	endwh
endp


proc check2w:(len%)
	local rem%,end%,i%,set%
	
	set%=len%/16
	rem%=len%-set%*16
	while rem%>0
		rem%=rem%-1
		end%=end%+2**rem%
	endwh	
	end%=$ffff-end%
	
	rem print "Mode = ";mode%
	rem print
	
	i%=1
	while i%<10
		rem print i%;".  ";right$(hex$(a%(i%)),4)
		if i%<=set%
			if a%(i%)<>0 : raise i% : endif
		else
			if i%=set%+1
				if a%(i%)<>end% : raise i% : endif
				else if a%(i%)<>$ffff : raise i% : endif
			endif
		endif
		i%=i%+1
	endwh
	while i%<=16
		rem print i%;". ";right$(hex$(a%(i%)),4)
				if i%<=set%
			if a%(i%)<>0 : raise i% : endif
		else
			if i%=set%+1
				if a%(i%)<>end% : raise i% : endif
				else if a%(i%)<>$ffff : raise i% : endif
			endif
		endif
		i%=i%+1
	endwh
endp


proc check4:(len%,r%)
	local val&,rem%,end&,i%,set%,pix&,twopix&
	
	set%=(len%*2)/16
	
	pix&=r%/80
	twopix&=pix&+pix&*(2**2)
	if (pix&*80-r%)<>0 : twopix&=twopix&+1 : endif
	val&=twopix& + twopix&*(2**4) + twopix&*(2**8) + twopix&*(2**12)
	if val& and &8000 : val&=val& or &ffff0000 : endif
	rem%=len%-(set%*16)/2
	end&=0
	i%=0
	while i%<rem%
		end&=end&+twopix&*(2**(i%*2))
		i%=i%+2
	endwh	
	if rem%=0 : end&=&ffffffff : endif
	if rem%=1 : end&=&fffffffc or end& : endif
	if rem%=2 : end&=&fffffff0 or end& : endif
	if rem%=3 : end&=&ffffffc0 or end& : endif
	if rem%=4 : end&=&ffffff00 or end& : endif
	if rem%=5 : end&=&fffffc00 or end& : endif
	if rem%=6 : end&=&fffff000 or end& : endif
	if rem%=7 : end&=&ffffc000 or end& : endif
	check:(r%,32,set%,val&,end&)
endp


proc check16:(len%,r%)
	local val&,rem%,end&,i%,set%,pix&
	
	set%=(len%*4)/16
	
	pix&=r%/16
	val&=pix& + pix&*(2**4) + pix&*(2**8) + pix&*(2**12)
	if val& and &8000 : val&=val& or &ffff0000 : endif
	rem%=len%-(set%*16)/4
	end&=0
	i%=0
	while i%<rem%
			end&=end&+pix&*(2**(i%*4))
			i%=i%+1
	endwh	
	if rem%=0 : end&=&ffffffff : endif
	if rem%=1 : end&=&fffffff0 or end& : endif
	if rem%=2 : end&=&ffffff00 or end& : endif
	if rem%=3 : end&=&fffff000 or end& : endif
	check:(r%,64,set%,val&,end&)
endp


proc check:(r%,n%,set%,val&,end&)
	local i%
	rem print "Mode = ";mode%
	rem print "Colour = $";hex$(r%)
	rem print
	i%=1
	while i%<10
		if i%<=set%
			rem print i%;".  ";right$(hex$(a%(i%)),4)
			if int(a%(i%))<>val& : raise i% : endif
		else
			if i%=set%+1
				rem print i%;".  ";right$(hex$(a%(i%)),4)
				if int(a%(i%))<>end& : raise i% : endif
				else if int(a%(i%))<>&ffffffff : raise i% : endif
			endif
		endif
		i%=i%+1
	endwh
	while i%<=n%
		if i%<=set%
			rem print i%;". ";right$(hex$(a%(i%)),4)
			if int(a%(i%))<>val& : raise i% : endif
		else
			if i%=set%+1
				rem print i%;". ";right$(hex$(a%(i%)),4)
				if int(a%(i%))<>end& : raise i% : endif
				else if int(a%(i%))<>&ffffffff : raise i% : endif
			endif
		endif
		i%=i%+1
	endwh
endp

REM End of pPeekL.tpl

