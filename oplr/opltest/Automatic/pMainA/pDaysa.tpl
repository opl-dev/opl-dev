REM pDAYSa.tpl
REM EPOC OPL automatic test code for DAYSTODATE
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pDaysa", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pDaysa:
	global day%,month%,year%
	hRunTest%:("t1990")
	hRunTest%:("t1990s")
endp

proc t1990:
	rem test first for days in 1990
	
	daystodate 30,year%,month%,day%
	REM print "30 days to ";day%;"/";month%;"/";year%
	if day%<>31 OR month%<>1 OR year%<>1900 : raise 1 : endif
	daystodate 31,year%,month%,day%
	REM print "31 days to ";day%;"/";month%;"/";year%
	if day%<>1 OR month%<>2 OR year%<>1900 : raise 2 : endif
	daystodate 59,year%,month%,day%
	REM print "59 days to ";day%;"/";month%;"/";year%
	if day%<>1 OR month%<>3 OR year%<>1900 : raise 3 : endif
	daystodate 300,year%,month%,day%
	REM print "300 days to ";day%;"/";month%;"/";year%
	if day%<>28 OR month%<>10 OR year%<>1900 : raise 4 : endif
	daystodate 364,year%,month%,day%
	REM print "364 days to ";day%;"/";month%;"/";year%
	if day%<>31 OR month%<>12 OR year%<>1900 : raise 5 : endif
	REM print
endp


proc t1990s:
	
	daystodate 32872,year%,month%,day%
	REM print "32872 days to ";day%;"/";month%;"/";year%
	if day%<>1 OR month%<>1 OR year%<>1990 : raise 1 : endif
	daystodate 35500,year%,month%,day%
	REM print "35500 days to ";day%;"/";month%;"/";year%
	if day%<>13 OR month%<>3 OR year%<>1997 : raise 2 : endif
	daystodate 35056,year%,month%,day%
	REM print "35056 days to ";day%;"/";month%;"/";year%
	if day%<>25 OR month%<>12 OR year%<>1995 : raise 3 : endif
	daystodate 36213,year%,month%,day%
	REM print "36213 days to ";day%;"/";month%;"/";year%
	if day%<>24 OR month%<>2 OR year%<>1999 : raise 4 : endif
	daystodate 33661,year%,month%,day%
	REM print "33661 days to ";day%;"/";month%;"/";year%
	if day%<>29 OR month%<>2 OR year%<>1992 : raise 5 : endif
	daystodate 34257,year%,month%,day%
	REM print "34257 days to ";day%;"/";month%;"/";year%
	if day%<>17 OR month%<>10 OR year%<>1993 : raise 6 : endif
endp


REM Test procedure used by pSetPathA to test SETPATH.
REM This module is LOADMed and the procedure called.

PROC testSETPATH:
	RETURN PI
ENDP


REM End of pDaysa.tpl

