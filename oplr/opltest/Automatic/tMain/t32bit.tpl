REM t32bit.tpl
REM EPOC OPL automatic test code for opler1 32-bit opcodes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("t32bit", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc t32bit:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("do32bit")
	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	TRAP DELETE "c:\t32bit\*.*"
	TRAP RMDIR "c:\t32bit\"
ENDP


external doAlloc%:
external other32bitTests:
external assignToVarsAtHighMem:
external assignToVar%:(in%)
external assignToArrayItem%:(in%)


PROC do32BIT:
	local p%
	rem print "Opler1 32 bit addressing Tests"
	rem pause 20
	DEFAULTWIN KDefaultWin4KMode%
	p%=doAlloc%:    rem cell > 64k left in existence so other32bitTests
	rem has all vars at memory > 64K
	other32bitTests:
	freeAlloc p%
	rem print "Opler1 32 bit addressing Tests Finished OK"
	rem pause 20
ENDP


PROC doAlloc%:
	REM allocates cell > 64k
	local p%,p2%
	
	REM all the following ALLOC tests very inter-dependent
	REM take extreme care when changing any of it
	
	p%=ALLOC(70000)
	if p%=0 : raise 10 : endif
	
	pokeL p%,&12345678
	if peekL(p%)<>&12345678 : raise 11 : endif
	
	pokeL p%+70000-4,&87654321
	pokeB p%+70000-5,&A9      rem after adjustAlloc, 70000-4 = &a9876543
	if peekL(p%+70000-4)<>&87654321 : raise 12 : endif
	p2%=AdjustAlloc(p%,2,1)
	if p2%=0 : raise 13 : endif
	p%=p2%
	p2%=0
	if peekW(p%+3)<>$1234 : raise 14 : endif
	if peekW(p%)<>$5678 : raise 15 : endif
	if peekL(p%+70000-4)<>&654321a9 : raise 16 : endif
	if lenalloc(p%)<>70004 : raise 17 : endif
	
	p2%=AdjustAlloc(p%,70000-4,-1)
	if p2%=0 : raise 18 : endif
	p%=p2%
	p2%=0
	if (peekL(p%+70000-5) and &ffffff00) <>&65432100 : raise 19 : endif
	
	p2%=REALLOC(p%,70001)
	if p2%=0 : raise 20 : endif
	pokeb p%+70000,$21
	if peekL(p%+70000-4)<>&87654321 : raise 21 : endif
	return p%
ENDP


PROC other32BitTests:
	local v%,v&,v,v$(255),v2$(255)
	local ret%
	local name$(255)
	local p%,p2%
	local h%
	local arr(4)
	local arr$(2,5)
	local choice%,time&,ed$(30),xin$(8),file$(255),l&
	local ev%(16)
	local ev&(16)
	local info%(10)
	local gInfo&(48)
	local stat%,stat&
	local y%,m%,d%,hr%,mn%,s%,dd%
	REM local fontn$(20)
	local a%(16)
	local p%(20),i% 
	
	rem Commands
	
	v%=1234
	if peekW(Addr(v%))<>1234 : raise 100 : endif
	
	gFont KFontSquashed&
	FONT KFontSquashed&, 0
	FONT KFontCourierNormal11&,0
	
	gCLOCK ON,11,0,"%H%:1%T",KFontSquashed& 			rem was font% on opl1993
  pause 30
	gCLOCK OFF
	
	POKE$ addr(v$),"123" 												rem was addr% on Opl1993
	if PEEK$(addr(v$))<>"123" : raise 1 : endif
	POKEF addr(v), 1.23e99
	if PEEKF(addr(v))<>1.23e99 : raise 2 : endif
	POKEL addr(v&), &12345
	if PEEKL(addr(v&))<>&12345 : raise 3 : endif
	POKEW addr(v%), 32767
	if PEEKW(addr(v%))<>32767 : raise 4 : endif
	v%=256
	POKEB addr(v%),255
	if PEEKB(addr(v%)+1)<>1 : raise 5 : endif
	
	rem Functions
	
	name$="c:\t32bit\"
	trap mkdir name$
	ret%=IoOpen(h%,addr(name$),KIoOpenModeUnique% or KIoOpenFormatText% or KIoOpenAccessRandom%)
	if ret%<0 : raise 6 : endif
	
	v$="Check 32 bit addressing for IO functions - 123"
	ret%=IOWRITE(h%,addr(v$)+1+KOplAlignment%,len(v$))
	if ret%<0 : raise 7 : endif
	v&=0 		rem ignored (as on Opl1993)
	
	ret%=ioseek(h%,6,v&)
	if ret%<0 : raise 98 : endif
	
	ioclose(h%)
	ret%=ioopen(h%,name$,KIoOpenModeOpen% or KIoOpenFormatText%)
	if ret%<0 : raise 97 : endif
	
	ret%=IOREAD(h%,addr(v2$)+1+KOplAlignment%,len(v$)-2)
	
	REM The IOREAD should return -43 indicating the text isn't correctly terminated.
	REM However, it appears to read the length of the chars read, irrespective
	REM of whether they're terminated.
			
	if ret%<>-43
		alert("F32 IoRead Error" + num$(ret%,5),err$(ret%))
		raise 8
	endif
	ret%=len(v$)-2
	pokeb addr(v2$),ret%
	if v2$<>left$(v$,ret%)
		print "Expecting v2$=[";v2$;"]"
		print "Got [";left$(v$,ret%);"]"
		raise 9
	endif
	ret%=ioclose(h%)
	if ret%<0
		raise 10
		alert("IoClose Error",num$(ret%,5))
	endif
	rem trap unloadm "t_util"
	
	rem Statistical functions
	
	arr(1)=1
	arr(2)=2
	arr(3)=3
	arr(4)=4
	if MEAN(arr(),4)<>2.5 : raise 22 : endif
	if SUM(arr(),4)<>10.0 : raise 23 : endif
	if STD(arr(),3)<>1.0 : raise 24 : endif
	if VAR(arr(),3)<>1.0 : raise 25 : endif
	if MIN(arr(),4)<>1.0 : raise 26 : endif
	if MAX(arr(),4)<>4.0 : raise 27 : endif
	
	REM Interactive tests now in Interactive\tMain\t32bitI.tpl
	
	y%=100 :m%=100 :d%=100
	DAYSTODATE 396,y%,m%,d%
	if y%<>1901 : raise 51 : endif
	if m%<>2 : raise 52 : endif
	if d%<>1 : raise 53 : endif
	
	SECSTODATE 1234567890,y%,m%,d%,hr%,mn%,s%,dd%
	if y%<>2009 or m%<>2 or d%<>13 or hr%<>23 or mn%<>31 or s%<>30 or dd%<>44
		raise 54
	endif
	
	rem Grammar
	assignToVarsAtHighMem:
	
	rem SCREENINFO
	
	v&=KFontCourierNormal11&
	screeninfo info%()

	if info%(1)<>0 : print info%(1) : raise 55 : endif REM 1
	if info%(2)<>1 : print info%(2) : raise 56 : endif REM 4
	if info%(3)<>80 : print info%(3) : raise 57 : endif REM 91
	if info%(4)<>18 : print info%(4) : raise 58 : endif REM 21
	if info%(5)<>1 : print info%(5) : raise 59 : endif REM 1
	REM Unused.
	if info%(6)<>0 : print info%(6) : raise 60 : endif
	
	if info%(7)<>8 : print info%(7) : raise 61 : endif REM 7
	if info%(8)<>11 : print info%(8) : raise 62 : endif REM 11
	if info%(9)<>peekw(addr(v&)) : raise 63 : endif
	if info%(10)<>peekw(addr(v&)+2) : raise 64 : endif

	rem GINFO32 var i&()

	gcolor 16,16,16
	gfont KFontArialNormal13&
	gstyle $23		rem underlined, bold and italic
	gat 0,0
	cursor 1
	ginfo32 ginfo&()
	rem 1 and 2 reserved
	if ginfo&(3)<>13 : raise 65 : endif
	if ginfo&(4)<>2 : raise 66 : endif
	if ginfo&(5)<>11 : raise 67 : endif
	if ginfo&(6)<>16 : raise 68 : endif
	if ginfo&(7)<>16 : raise 69 : endif
	if ginfo&(8)<>29 : raise 70 : endif
	
	if ginfo&(9)<>268435956 : raise 71 : endif
	
	if ginfo&(18)<>0 :print ginfo&(18): raise 72 : endif
	if ginfo&(19)<>0 : raise 73 : endif
	if ginfo&(20)<>&23: raise 74 : endif
	if ginfo&(21)<>1 : raise 75 : endif
	if ginfo&(22)<>1 : raise 76: endif
	if ginfo&(23)<>2 : raise 77 : endif
	if ginfo&(24)<>13 : raise 78 : endif
	if ginfo&(25)<>11 : raise 79 : endif
	if ginfo&(26)<>0 : raise 80 : endif
	if ginfo&(27)<>0 : raise 81 : endif
	if ginfo&(28)<>0 : raise 82 : endif
	if ginfo&(29)<>4 : raise 83 : endif
	REM Graphics colour mode is specific to emulator window mode.
	REM Graphics colour mode.
	
	if ginfo&(30)<>KDefaultWin4KMode% : print gINFO&(30) : raise 84 : endif
	if ginfo&(31)<>16 : raise 85 : endif
	if ginfo&(32)<>16 : raise 86 : endif
	if ginfo&(33)<>16 : raise 87 : endif
	if ginfo&(34)<>255 : raise 88 : endif
	if ginfo&(35)<>255 : raise 89 : endif
	if ginfo&(36)<>255 : raise 90 : endif
	cursor off
	
	rem  gPOLY	
	
	gcolor 0,0,0
	gcls
	a%(1)=100 : a%(2)=100 : 	a%(3)=6
	a%(4)=100 : a%(5)=0
	a%(6)=50 : a%(7)=43
	a%(8)=-50 : a%(9)=43
	a%(10)=-100 : a%(11)=0
	a%(12)=-50 : a%(13)=-43
	a%(14)=50 : a%(15)=-43
	gpoly a%()
	pause 30
	
	rem GPEEKLINE ix%, x%, y%, %(), ln%[,mode%=-1 (for opler1)]
	
	gpeekline 1,100,100,p%(),100,1
	i%=0
	while i%<6
		i%=i%+1
		if p%(i%)<>0 : raise i% : endif
	endwh
	i%=i%+1
	if p%(i%)<>$ffc0 : raise i% : endif
	while i%<12
		i%=i%+1
		if p%(i%)<>$ffff : raise i% : endif
	endwh
	i%=i%+1
	if p%(i%)<>$ff : raise i% : endif
	
	gcls : cls
	
	rem IOWAITSTAT32 var status&
	
	return
ENDP


PROC assignToVarsAtHighMem:
	if assignToVar%:(1111)<>1111 : raise 1 : endif
	if assignToArrayItem%:(2222)<>2222 : raise 2 : endif
ENDP


PROC assignToVar%:(in%)
	local var%
	var%=in%
	return var%
ENDP


PROC assignToArrayItem%:(in%)
	local arr%(100)
	arr%(100)=in%
	return arr%(100)
ENDP

REM End of t32bit.tpl
