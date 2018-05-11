REM pclock11.tpl
REM EPOC OPL automatic test code for more gclock functionality.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pclock11", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pclock11:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tgclock11")
endp


proc tgclock11:
	global id1%,id2%,id3%
	print "Opler1 gCLOCK mode 11 Tests"
	print
	rem pause 20
	t24hour:
	t12hour:
	tsystem:
	tseps:
	tabbrev:
	tlocaledate:
	tnonlocaledate:
	tAB:
	toffset:
	terrors:
	print
	print "Opler1 gCLOCK mode 11 Tests Finished OK"
	rem pause 30
endp


proc t24hour:
	print "Test 24 hour clock"
	rem pause 20
	createwin:
	formclock:("%H:%T:%S")
	formclock:("%H:%T:%S%A")
rem	formclock:("%H:%T:%S%B")
rem	formclock:("%A%H:%T:%S")
	formclock:("%B%H:%T:%S")
	formclock:("%H:%T:%S%-A")
rem	formclock:("%H:%T:%S%-B")
rem	formclock:("%H:%T:%S%+A")
	formclock:("%H:%T:%S%+B")
	formclock:("%-A%H:%T:%S")
rem	formclock:("%-B%H:%T:%S")
rem	formclock:("%+A%H:%T:%S")
	formclock:("%+B%H:%T:%S")
	closewin:
endp
	
	
proc t12hour:
	print "Test 12 hour clock"
	rem pause 20
	createwin:
	formclock:("%I:%T:%S")
	formclock:("%I:%T:%S%A")
rem	formclock:("%I:%T:%S%B")
rem	formclock:("%A%I:%T:%S")
	formclock:("%B%I:%T:%S")
	formclock:("%I:%T:%S%-A")
rem	formclock:("%I:%T:%S%-B")
rem	formclock:("%I:%T:%S%+A")
	formclock:("%I:%T:%S%+B")
	formclock:("%-A%I:%T:%S")
rem	formclock:("%-B%I:%T:%S")
rem	formclock:("%+A%I:%T:%S")
	formclock:("%+B%I:%T:%S")
	closewin:
endp


proc tsystem:
	print "Test system clock"
	rem pause 20
	createwin:
	formclock:("%J:%T:%S")
	formclock:("%J:%T:%S%A")
rem	formclock:("%J:%T:%S%B")
rem	formclock:("%A%J:%T:%S")
	formclock:("%B%J:%T:%S")
	formclock:("%J:%T:%S%-A")
rem	formclock:("%J:%T:%S%-B")
rem	formclock:("%J:%T:%S%+A")
	formclock:("%J:%T:%S%+B")
	formclock:("%-A%J:%T:%S")
rem	formclock:("%-B%J:%T:%S")
rem	formclock:("%+A%J:%T:%S")
	formclock:("%+B%J:%T:%S")
	closewin:
endp


proc tseps:
	print "Test system separators"
	rem pause 20
	createwin:
	rem test %%
	formclock:("%H%%%T%%%S")
	rem test %:
	formclock:("%H%:1%T%:2%S")
	formclock:("%H%:0%T%:3%S")
	rem test %/
	formclock:("%1%/1%2%/2%3")
	formclock:("%1%/0%2%/3%3")

	onerr e3
	formclock:("%H%:4%T%:3%S")
	onerr off
	raise 5
e3::
	onerr off
	if err<>-1 : raise 6 : endif
	onerr e4
	formclock:("%1%/-1%2%/2%3")
	onerr off
	raise 7
e4::
	onerr off
	if err<>-1 : raise 8 : endif
	closewin:
endp


proc tabbrev:
	print "Test time abbreviations"
	rem pause 20
	createwin:
	formclock:("%*J:%T:%S")
	formclock:("%*I:%T:%S")
	formclock:("%*H:%*T:%*S")
	formclock:("%I:%T:%S%*A")
	formclock:("%I:%T:%S%*B")
	formclock:("%*A%I:%T:%S")
rem	formclock:("%*B%I:%T:%S")
rem	formclock:("%I:%T:%S%*+A")
	formclock:("%I:%T:%S%*+B")
	
	onerr e1
	formclock:("%I:%T:%S%+*A")
	onerr off
	raise 1
e1::
	onerr off 
	if err<>-1 : raise 2 : endif
	onerr e2
	formclock:("%I:%T:%S%+*B")
	onerr off
	raise 3
e2::
	onerr off 
	if err<>-1 : raise 4 : endif
	
	formclock:("%*-A%I:%T:%S")
	formclock:("%*-B%I:%T:%S")
	
	onerr e3
	formclock:("%-*A%I:%T:%S")
	onerr off
	raise 5
e3::
	onerr off 
	if err<>-1 : raise 6 : endif
	onerr e4
	formclock:("%-*B%I:%T:%S")
	onerr off
	raise 7
e4::
	onerr off 
	if err<>-1 : raise 8 : endif
	
	closewin:
	print "Test date abbreviations"
	rem pause 20
	createwin:
	formclock:("%*E %*D%X%*N%*Y %1 %2 %3")
	formclock:("%*1%D/%*2%M/%*3%Y")
	formclock:("%*4%D/%*5%M")
	formclock:("%*1/%*2/%*3")
	formclock:("%*4/%*5")
	formclock:("%1%*D/%2%*M/%3%*Y")
	formclock:("%4%*D/%5%*M")
	formclock:("Week: %*W")
	formclock:("Day: %*Z")
	rem without locale format
	formclock:("%F%*D/%*M/%*Y")
	formclock:("%F%*D/%*M")
	formclock:("%F%*1/%*2/%*3")
	formclock:("%F%*4/%*5")
	formclock:("%*F%D/%M/%Y")
	formclock:("%*F%D/%M")
	closewin:
endp


proc tlocaledate:
	print "Test dates using locale format"
	rem pause 20
	createwin:
	rem matching numbers and letters
	formclock:("%1%D/%2%M/%3%Y")
	formclock:("%2%M/%1%D/%3%Y")
rem	formclock:("%3%Y/%2%M/%1%D")
	formclock:("%4%D/%5%M/%3%Y")
	formclock:("%1%D/%2%M")
rem	formclock:("%4%D/%5%M")
	formclock:("%5%M/%4%D")
	rem numbers and letters do not match
	formclock:("%1%M/%2%Y/%3%D")
rem	formclock:("%2%D/%1%Y/%3%M")
	formclock:("%4%M/%5%Y")
	formclock:("%4%Y/%5%D")
	rem numbers and with time letters 
	formclock:("%1%H/%2%T/%3%S")
	formclock:("%4%E/%5%W")
	formclock:("%4%N/%5%Z")
	rem numbers only
	formclock:("%1/%2/%3")
rem	formclock:("%2/%1/%3")
	formclock:("%3/%2/%1")
	formclock:("%4/%5/%3")
	formclock:("%1/%2")
rem	formclock:("%4/%5")
rem	formclock:("%5/%4")
	formclock:("%1/%2/%3")
	rem using names of months and days
	formclock:("%E %X%*1 %N%2 %3%Y")
	formclock:("%*E %X%*1%D %*N%2 %3%Y")
	formclock:("%X%1 %1")
	formclock:("%N%2 %2")
	rem day no and week no
	formclock:("Week: %W")
	formclock:("Day: %Z")
	closewin:
endp


proc tnonlocaledate:
	print "Test non-locale dates"
	rem pause 20
	createwin:
	formclock:("%F%D/%M/%Y")
	formclock:("%F%M/%D/%Y")
	formclock:("%F%Y/%M/%D")
rem	formclock:("%F%D/%M/%Y")
	formclock:("%F%D/%M")
	formclock:("%F%D/%M")
rem	formclock:("%F%M/%D")
	rem use numbers in non-locale format
	formclock:("%F%1%D/%2%M/%3%Y")
	formclock:("%F%4%D/%5%M")
	formclock:("%F%1/%2/%3")
rem	formclock:("%F%4/%5")
	rem use %F with time
	formclock:("%F%H:%T:%S")
	formclock:("%F%I:%T:%S")
rem	formclock:("%F%J:%T:%S")
	rem using names of months and days
	formclock:("%F%E %*D%X %N %Y")
	formclock:("%F%*E %*D%X %*N %Y")
	formclock:("%F%D%X %D")
rem	formclock:("%F%N %M")
	formclock:("%F%D%X %D")
	closewin:
endp


proc tAB:
	print "Test %A and %B used with dates"
	rem pause 20
	createwin:
	formclock:("%1/%2/%3%A")
rem	formclock:("%1/%2/%3%B")
rem	formclock:("%1/%2/%3%+A")
	formclock:("%1/%2/%3%+B")
	formclock:("%1/%2/%3%-A")
rem	formclock:("%1/%2/%3%-B")
rem	formclock:("%A%1/%2/%3")
	formclock:("%B%1/%2/%3")
	formclock:("%+A%1/%2/%3")
rem	formclock:("%+B%1/%2/%3")
rem	formclock:("%-A%1/%2/%3")
	formclock:("%-B%1/%2/%3")
	closewin:
	print "Test %A and %B used without hours in time"
	rem pause 20
	createwin:
	formclock:("%T:%S %A")
	formclock:("%T:%S %B")
	formclock:("%A %T:%S")
rem	formclock:("%B %T:%S")
rem	formclock:("%T:%S%-A")
	formclock:("%T:%S%-B")
	formclock:("%T:%S%+A")
rem	formclock:("%T:%S%+B")
rem	formclock:("%-A%T:%S")
	formclock:("%-B%T:%S")
	formclock:("%+A%T:%S")
	formclock:("%+B%T:%S")
	closewin:
endp


proc toffset:
	print "Test offset"
	rem pause 20
	createwin:
	
	guse id1%
	gat 5,40
	gprint "1 hour behind system time"
	guse id3%
	gat 5,40
	gprint "1 hour ahead of system time"
	offsetclocks:("%J%:1%T%:2%S%B",&3c,0)
	
	guse id1%
	gat 5,40
	gprint "24 hours behind system time"
	guse id3%
	gat 5,40
	gprint "24 hours ahead of system time"
	offsetclocks:("%1%/1%2%/2%3",&5a0,0)
	
	rem offset in seconds
	guse id1%
	gat 5,40
	gprint "30 seconds behind system time"
	guse id3%
	gat 5,40
	gprint "30 seconds ahead of system time"
	offsetclocks:("%H%:1%T%:2%S",&1e,$100)
	
	guse id1%
	gat 5,40
	gprint "24 hours behind system time"
	guse id3%
	gat 5,40
	gprint "24 hours ahead of system time"
	offsetclocks:("%1%/1%2%/2%3  %H%:1%T%:2%S",&15180,$100)
	closewin:
endp


proc offsetclocks:(format$,offset&,or%)
	guse id1%
	gat 5,20
	gprint format$
	gat 220,20
	gclock on,11 OR or%,-offset&,format$
	guse id2%
	gat 5,20
	gprint format$
	gat 5,40
	gprint "system time"
	gat 220,20
	gclock on,11 OR or%,0,format$
	guse id3%
	gat 5,20
	gprint format$
	gat 220,20
	gclock on,11 OR or%,offset&,format$
	rem get
	guse id1% : gcls
	guse id2% : gcls
	guse id3% : gcls
endp


proc terrors:
	rem attempt to display two clocks in one window
	print "Attempt to open two clocks of different modes in same window"
	rem pause 20
	cls
	id1%=gcreate (0,0,gwidth,gheight,1,0)
	gfont KFontCourierNormal11&	
	gat 20,20
	gprint "Mode 7 - analog"
	gat 20,60

	gclock on,7
	gat 320,20
	gprint "Mode 11 - formatted digital"
	gat 320,60
	gclock on,11,0,"%J%:1%T%:2%S%B"
	rem get
	gclock off
	gclose id1%

	print "Attempt to open two clocks of mode 11 in same window"
	print
	rem pause 20
	cls
	id1%=gcreate (0,0,gwidth,gheight,1,2)
	gfont KFontCourierNormal11&	
	gat 20,20
	gprint "Mode 11 - formatted digital"
	gat 20,60
	gclock on,11,0,"%J%:1%T%:2%S%B"
	gat 320,20
	gprint "Mode 11 - formatted digital"
	gat 320,60
	gclock on,11,0,"%1%/1%2%/2%3"
	rem get
	gclock off
	gclose id1%
endp


proc createwin:
	cls
	id1%=gcreate (0,0,gwidth,80,1,2)
	gfont KFontCourierNormal11&
	id2%=gcreate (0,80,gwidth,80,1,2)
	gfont KFontCourierNormal11&
	id3%=gcreate (0,160,gwidth,80,1,2)
	gfont KFontCourierNormal11&
endp


proc closewin:
	gclose id1%
	gclose id2%
	gclose id3%
endp


proc formclock:(format$)
	guse id1%
	gat 5,20
	gprint format$
	gat 200,20
	gclock on,11,0,format$
	
	guse id2%
	gat 5,20
	gprint format$
	gat 200,20
	gclock on,11,0,format$,KFontTimesNormal27&,$22
	
	guse id3%
	gat 5,20
	gprint format$
	gat 200,20
	gclock on,11,0,format$,KFontCourierNormal13&,$C
	
	rem get
	guse id1% : gcls
	guse id2% : gcls
	guse id3% : gcls
endp

REM End of pclock11.tpl

