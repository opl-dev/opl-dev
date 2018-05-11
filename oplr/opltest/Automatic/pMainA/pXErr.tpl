REM pXerr.tpl
REM EPOC OPL automatic test code for XERR
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pXErr", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pxerr:
	hRunTest%:("pxerr1")
	hCleanUp%:("CleanUp")
ENDP

PROC CleanUp:
	DELETE "file1.odb"
ENDP

PROC pxerr1:
	local x$(255)
	local a&,b&      rem wrongly defined as local rather than global
	REM print "Opler1 Extended Error Message Tests"
	REM print
	tdataerror:

	REM print "Test undefined external errors"
	onerr undef1
	tundefvar1:
	onerr off
	raise 1
undef1::
	onerr off
	x$=errx$
	REM print x$
	REM print
	if x$<>"Error in PXERR\TUNDEFVAR1,M%,B&,A&,M&"	 : raise 2 : endif
 
	onerr undef2
	tundefvar2:
	onerr off
	raise 3
undef2::
	onerr off
	x$=errx$
	REM print x$
	REM print
	if x$<>"Error in PXERR\TUNDEFVAR2,M,N,A$,B$" : raise 4 : endif

	onerr undef3
	tundefproc:
	onerr off
	raise 5
undef3::
	onerr off
	x$=errx$
	REM print x$
	REM print
 	if x$<>"Error in PXERR\TUNDEFPROC,UNDEFPROC1" : raise 6 : endif

	tnotdeclared:

	REM print
	REM print "Opler1 Extended Error Message Tests Finished OK"
	REM pause -30
endp

proc tdataerror:
	REM print "Test datafile errors"
	REM print
	trap create "file1.odb",a,a$,b$,d$
	if exist("file2.odb")
		delete "file2.odb"
	endif
	
	REM print
	REM print "Create file which already exists"
	REM Assume this should be file1.odb and not file.odb,
	REM as the former already exists, and the latter doesn't.
	trap create "file1.odb",a,a$,b$,c$
	REM print errx$
	if errx$<>"Error in PXERR\TDATAERROR" : raise 1 : endif
	USE A :CLOSE
	REM print
	REM print "Open file which does not exists"
	trap open "file2.odb",b,d$,e$,f$ 
	REM print errx$
	if errx$<>"Error in PXERR\TDATAERROR" : raise 2 : endif
	REM print
	REM print "Create file with invalid name"
	trap open "12345.odb",b,d$,e$,f$ 
	REM print errx$
	if errx$<>"Error in PXERR\TDATAERROR" : raise 3 : endif
	REM print
endp

proc tundefvar1:
	local a%,b%
	rem Tests 'undefined external' errors
	rem undefined externals are m%,a&,b&,m&
	
	b%=2
	a%=b%+m%
	m%=a%*b%
	b&=3
	a&=b&+m&
endp

proc tundefvar2:
	local a,b
	rem Tests 'undefined external' errors
	rem undefined externals are m,n,a$,b$
	
	b=2.1234
	a=b+m
	m=a*b
	n=4.5678
	m=n*a
	print a$+b$
endp

proc tundefproc:
	rem Test undefined external errors
	rem undefined procedures undefproc1 and undefproc2
	
	undefproc1:
	undefproc2:
endp

proc tNotDeclared:
	local a%,b&,c,d$(10)
	REM print "Test undeclared procedures give runtime errors when no DECLARE EXTERNAL"
		
	onerr e1
	undec1:
	onerr off
	raise 1
e1::
	if err<>-99 : 
		REM print err$(err)
		raise 1 : endif
	onerr e2
	undec2:(a%)
	onerr off
	raise 3
e2::
	if err<>-99 : 
	rem print err$(err) 
	: raise 4 : endif
	onerr e3
	undec3:(b&)
	onerr off
	raise 5
e3::
	if err<>-99 : 
	rem print err$(err) 
	: raise 6 : endif
	onerr e4
	undec4:(c)
	onerr off
	raise 7
e4::
	if err<>-99 : 
	rem print err$(err) 
	: raise 8 : endif
	onerr e5
	undec5:(d$)
	onerr off
	raise 9
e5::
	if err<>-99 : 
	rem print err$(err) 
	: raise 10 : endif
	onerr e6
	undec6:(a%,b&,c,d$)
	onerr off
	raise 11
e6::
	if err<>-99 : 
	rem print err$(err) 
	: raise 12 : endif
endp

REM End of pXErr.tpl
