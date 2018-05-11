REM pIOInfo.tpl
REM EPOC OPL automatic test code for pIOinfo.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pIOinfo", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pIOInfo:
	global sw%,sh%
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	rem print "Opler1 test io functions give correct info"
	hRunTest%:("tfsense")
	hRunTest%:("tfset")
	hRunTest%:("tfinq")
rem	hCleanUp%:("CleanUp")
endp

const KS5ScreenTextWidth%=91
const KS5ScreenTextHeight%=21
const KCrystalScreenTextWidth%=80
const KCrystalScreenTextHeight%=18

proc tfsense:
	local buf%(6)
	
	rem print "FSENSE"
	rem pause 20
	rem should return coords of console window and cursor
	rem one less that expected because of top left being 1,1
	cls
	cursor on
	iow(-2,8,buf%(),#0)
	if buf%(1)<>0 : raise 1 : endif
	if buf%(2)<>0 : raise 2 : endif
	if buf%(3)<>KCrystalScreenTextWidth% : print buf%(3) : raise 3 : endif
	if buf%(4)<>KCrystalScreenTextHeight% : print buf%(4) : raise 4 : endif	
	if buf%(5)<>0 : raise 5 : endif
	if buf%(6)<>0 : raise 6 : endif	
	
	sw%=buf%(3) : sh%=buf%(4)
	
 	screen 30,10,10,5
	at 20,10
	iow(-2,8,buf%(),#0)
	if buf%(1)<>9 : raise 7 : endif 
	if buf%(2)<>4 : raise 8 : endif
	if buf%(3)<>39 : raise 9 : endif
	if buf%(4)<>14 : raise 10 : endif
	if buf%(5)<>19 : raise 11 : endif
	if buf%(6)<>9 : raise 12 : endif
	rem pause 30
	resetscr:
endp


proc tfset:
	local buf%(6),func%,true%,false%

	rem print "FSET"
	rem pause 30
	cls
	print rept$("0123456789",25);
	print rept$("0123456789",25);
	
	rem SCR_SCROLL
	busy "SCR_SCROLL"
	func%=1
	buf%(1)=10 : buf%(2)=1
	buf%(3)=20 : buf%(4)=8
	buf%(5)=0 : buf%(6)=10
	iow(-2,7,func%,buf%())
	gupdate
	rem pause 20
	buf%(1)=10 : buf%(2)=11
	buf%(3)=20 : buf%(4)=18
	buf%(5)=10 : buf%(6)=0
	iow(-2,7,func%,buf%())
	gupdate
	rem pause 20
	buf%(1)=20 : buf%(2)=11
	buf%(3)=30 : buf%(4)=18
	buf%(5)=-10 : buf%(6)=-10
	iow(-2,7,func%,buf%())
	gupdate
	rem pause 20
	
	rem SCR_CLR
	busy "SCR_CLR"
	func%=2
	buf%(1)=10 : buf%(2)=1
	buf%(3)=20 : buf%(4)=8
	iow(-2,7,func%,buf%())
	gupdate
	rem pause 20
	buf%(1)=80 : buf%(2)=2
	buf%(3)=100 : buf%(4)=6
	iow(-2,7,func%,buf%())
	gupdate
	rem pause 20
	cls
	
	rem SCR_SLOCK
	busy "SCR_SLOCK"
	func%=6
	true%=1
	false%=0
	screen 20,10
	iow(-2,7,func%,true%)
	print rept$("0123456789",20)
	print "Now scroll lock on"
	rem pause 20
	print "ABCDEFG"
	rem pause 20
	iow(-2,7,func%,false%)
	print "Now scroll lock off"
	gupdate
	rem pause 40
	resetscr:
	
	rem SCR_WLOCK
	busy "SCR_WLOCK"
	func%=7
	true%=1
	false%=0
	print "Auto wrap is on"
	iow(-2,7,func%,true%)
	print rept$("0123456789",9);
	print "ABCDEFG"
	print "Now it is off"
	iow(-2,7,func%,false%)
	print rept$("0123456789",9);
	print "ABCDEFG"
	gupdate
	rem get
	iow(-2,7,func%,true%)
	cls
	
	rem SCR_NEL
	busy "SCR_NEL"
	func%=8
	print "; at the end of the next print statement:"
	print rept$("0123456789",10);
	iow(-2,7,func%,#0)
	print "This should be on the next line";
	print " and this is still on the same line"
	gupdate
	rem get
	cls
	screen 10,10
	print rept$("0123456789",11);
	iow(-2,7,func%,#0)
	print "Should have scrolled up"
	gupdate
	rem get
	cls
	func%=6
	iow(-2,7,func%,true%)
	print rept$("0123456789",10);
	func%=8
	iow(-2,7,func%,#0)
	print "Won't scroll"
	gupdate
	rem pause 20
	iow(-2,7,func%,false%)
	func%=6
	iow(-2,7,func%,false%)
	resetscr:
endp

const KS5CharWidthInPixels%=7
const KCrystalCharWidthInPixels%=8

proc tfinq:
	local buf%(4), info%(10)
	
	cls
	print "FINQ"
	rem should be the same as SCREENINFO
	iow(-2,12,buf%(),#0)
	screeninfo info%()
	rem check against screeninfo
	if info%(5)<>buf%(1) : raise 1 : endif
	if info%(6)<>buf%(2) : raise 2 : endif  rem both unused
	if info%(8)<>buf%(3) : raise 3 : endif
	if info%(7)<>buf%(4) : raise 4 : endif
	rem check values
	if buf%(1)<>1 : raise 5 : endif
	rem buf%(2) is unused
	if buf%(3)<>11 : raise 7 : endif
	if buf%(4)<>KCrystalCharWidthInPixels% : print buf%(4) : raise 8 : endif
	rem pause 20
	resetscr:
endp


proc resetscr:
	local raise%
	busy off
	cursor off
	if sw%=0 :raise%=1 :endif
	if sh%=0 :raise%=raise%+2 :endif
	if raise% :raise 100+raise% :endif
	screen sw%,sh%
	cls
endp

REM End of pIoInfo.tpl
