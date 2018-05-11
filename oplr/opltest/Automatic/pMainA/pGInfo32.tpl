REM pGINFO32.tpl
REM EPOC OPL automatic test code for ginfo
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"
include "const.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pginfo32", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pginfo32:
	global info&(48),id%,i%
	rem hInitTestHarness:(KhInitLocalErrorHandling%,KhInitNotUsed%)
	REM print "Opler1 gINFO32 Test"
	REM print
	hRunTest%:("t16")
	hRunTest%:("t4")
	REM print
	REM print "Opler1 gINFO32 Test Finished OK"
	REM pause 30
endp

proc t16:
	REM print "Test 16 colour window"
	REM pause 20
	id%=gcreate(0,0,gwidth,gheight,1,2) 
	gcolor 16,16,16
	gfont KFontArialNormal11&
	gstyle $23		rem underlined, bold and italic
	cursor id%
	ginfo32 info&()
	REM pause 40	
	gclose id%
	
	REM print "Height of font ";info&(3)
 	if info&(3)<>11 : raise 1 : endif
 	REM print "Descent of font ";info&(4)
 	if info&(4)<>2 : raise 2 : endif
 	REM print "Ascent of font ";info&(5)
 	if info&(5)<>9 : raise 3 : endif
  	REM print "Width of 0 ";info&(6)
 	if info&(6)<>14 : raise 4 : endif
 	REM print "Maximum character width ";info&(7)
 	if info&(7)<>14 : raise 5 : endif
	REM print "Font flags ";info&(8)
 	if info&(8)<>29 : raise 6 : endif
 	REM print "Font UID ";info&(9)
 	if info&(9)<>KFontArialNormal11& : raise 70 : endif
 	i%=10
 	while i%<18
	 	if info&(i%)<>0 : raise 70+i% : endif
	 	i%=i%+1
	endwh
	REM print "Current graphics mode ";info&(18)
 	if info&(18)<>0 : raise 7 : endif
 	REM print "Current text mode ";info&(19)
 	if info&(19)<>0 : raise 8 : endif
	REM print "Current style ";info&(20)
 	if info&(20)<>&23: raise 9 : endif
 	REM print "Cursor state ";info&(21)
 	if info&(21)<>1 : raise 10 : endif
	REM print "Id of window with cursor ";info&(22)
 	if info&(22)<>int(id%) : raise 11: endif
	REM print "Cursor width ";info&(23)
 	if info&(23)<>2 : raise 12 : endif
	REM print "Cursor height ";info&(24)
 	if info&(24)<>11 : raise 13 : endif
	REM print "Cursor ascent ";info&(25)
 	if info&(25)<>9 : raise 14 : endif
	REM print "Cursor x-coord ";info&(26)
 	if info&(26)<>0 : raise 15 : endif
	REM print "Cursor y-coord ";info&(27)
 	if info&(27)<>0 : raise 16 : endif
	REM print "Bitmap or window ";info&(28)
 	if info&(28)<>0 : raise 17 : endif
 	REM print "Cursor effects ";info&(29)
 	if info&(29)<>4 : raise 18 : endif
 	REM print "Graphics colour-mode ";info&(30)
	if info&(30)<>2 : raise 181 : endif
	REM print "red% of foreground ";info&(31)
 	if info&(31)<>16 : raise 19 : endif
	REM print "green% of foreground ";info&(32)
	if info&(32)<>16 : raise 20 : endif
	REM print "blue% of foreground ";info&(33)
	if info&(33)<>16 : raise 21 : endif
	REM print "red% of background ";info&(34)
	if info&(34)<>255 : raise 22 : endif
	REM print "green% of background ";info&(35)
	if info&(35)<>255 : raise 23 : endif
	REM print "blue% of background ";info&(36)
	if info&(36)<>255 : raise 24 : endif
	REM pause 40
	REM print
endp

const KWidth0Classic%=16
const KWidth0Crystal%=17

proc t4:
	REM print "Test 4 colour window"
	REM print
	id%=gcreate(0,0,gwidth,gheight,1,1) 
	cursor id%,25,5,30,2
	gcolor 128,255,0
	gfont KFontTimesNormal15&
	gstyle $1c		rem mono, double height, inverse
	gat 20,20
	ginfo32 info&()
	REM pause 30
	gclose id%
	
	REM print "Height of font ";info&(3)
 	if info&(3)<>30 : raise 25 : endif
 	REM print "Descent of font ";info&(4)
 	if info&(4)<>6 : raise 26 : endif
 	REM print "Ascent of font ";info&(5)
 	if info&(5)<>24 : raise 27 : endif
  REM print "Width of 0 ";info&(6)

 	if info&(6)<>KWidth0Crystal% : raise 28 :	endif
 	REM print "Maximum character width ";info&(7)
 	if info&(7)<>17 : raise 29 : endif
	REM print "Font flags ";info&(8)
 	if info&(8)<>33 : raise 30 : endif

 	REM print "Font UID ";info&(9)
 	if info&(9)<>KFontTimesNormal15& : raise 70 : endif	
 	i%=10
 	while i%<18
	 	if info&(i%)<>0 : raise 70+i% : endif
	 	i%=i%+1
	endwh
	REM print "Current graphics mode ";info&(18)
 	if info&(18)<>0 : raise 31 : endif
 	REM print "Current text mode ";info&(19)
 	if info&(19)<>0 : raise 32 : endif
	REM print "Current style ";info&(20)
 	if info&(20)<>&1c: raise 33 : endif
 	REM print "Cursor state ";info&(21)
 	if info&(21)<>1 : raise 34 : endif
	REM print "Id of window with cursor ";info&(22)
 	if info&(22)<>int(id%) : raise 35 : endif
	REM print "Cursor width ";info&(23)
 	if info&(23)<>5 : raise 36 : endif
	REM print "Cursor height ";info&(24)
 	if info&(24)<>30 : raise 37 : endif
	REM print "Cursor ascent ";info&(25)
 	if info&(25)<>25 : raise 38 : endif
	REM print "Cursor x-coord ";info&(26)
 	if info&(26)<>20 : raise 39 : endif
	REM print "Cursor y-coord ";info&(27)
 	if info&(27)<>20 : raise 40 : endif
	REM print "Bitmap or window ";info&(28)
 	if info&(28)<>0 : raise 41 : endif
 	REM print "Cursor effects ";info&(29)
 	if info&(29)<>6 : raise 42 : endif
 	REM print "Graphics colour-mode ";info&(30)
	if info&(30)<>1 : raise 421 : endif
	REM print "red% of foreground ";info&(31)
	if info&(31)<>128 : raise 43 : endif
	REM print "green% of foreground ";info&(32)
	if info&(32)<>255 : raise 44 : endif
	REM print "blue% of foreground ";info&(33)
	if info&(33)<>0 : raise 45 : endif
	REM print "red% of background ";info&(34)
	if info&(34)<>255 : raise 46 : endif
	REM print "green% of background ";info&(35)
	if info&(35)<>255 : raise 47 : endif
	REM print "blue% of background ";info&(36)
	if info&(36)<>255 : raise 48 : endif
	REM pause 30
endp

REM End of pGINFO32.tpl 
