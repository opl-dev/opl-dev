REM pInclude.tpl
REM EPOC OPL automatic test code for Include
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pInclude", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP



rem ---- pulled in the header file ----
rem INCLUDE "PINCHEAD.OPL"
rem This is a header file with some constant definitions 
rem and procedure prototypes to be included in PINCTEST

const KConstNo1%=1
const KConstNo2%=2
const KConstNo3%=32767

const KConstNo4&=&4
const KConstNo5&=&5
const KConstNo6&=&7fffffff

const KConstNo7=7.89
const KConstNo8=8.91
const KConstNo9=1.797E308

const KConstNo10$="ten"
const KConstNo11$="eleven"
const KConstNo12$="twelve"

external Procedure1:
external Procedure2:
external Procedure3:
rem ---- end of header file ----

proc pInclude:
	REM print "Opler1 Include Tests"
	REM print
	hRunTest%:("Procedure1")
	hRunTest%:("Procedure2")
	hRunTest%:("Procedure3")
	rem uncomment line below to check that there is a
	rem translate-time error
	rem Procedure4:
	REM print
	REM print "Opler1 Include Tests Finished OK"
	REM pause -30
endp

proc Procedure1:
	local res%,res&
	
	REM print "Test included ints and longs"
	REM print
	
	if KConstNo1%+KConstNo2%<>3 : raise 1 : endif
	if KConstNo2%-KConstNo1%<>1 : raise 2 : endif
	onerr over1
	res%=KConstNo3%+KConstNo1%
	onerr off
	raise 3
over1::
	onerr off
	if err<>0 : if err<>-6 : 
		rem print err$(err) 
		raise 4 : endif : endif

	if KConstNo4&+KConstNo5&<>9 : raise 5 : endif
	if KConstNo5&-KConstNo4&<>1 : raise 6 : endif
	onerr over2
	res&=KConstNo5&+KConstNo6&
	onerr off
	raise 7
over2::
	onerr off
	if err<>0 : if err<>-6 
		REM print err$(err)
		 : raise 8 : endif : endif	
	
	REM print
endp

proc Procedure2:
	local res&
	
	REM print "Test included floats"
	REM print
	
	if KConstNo7+KConstNo8<>7.89+8.91 : raise 1 : endif
	if KConstNo8-KConstNo7<>8.91-7.89 : raise 2 : endif
	onerr over
	res&=KConstNo9*KConstNo7
	onerr off
	raise 3
over::
	onerr off
	if err<>0 : if err<>-6 : 
		rem print err$(err) 
		raise 4 : endif : endif	
	
	REM print
endp

proc Procedure3:
	local str$(255)
	REM print "Test included strings"
	REM print
	str$=KConstNo10$+", "+KConstNo11$+", "+KConstNo12$
	REM print str$
	if str$<>"ten, eleven, twelve"	: raise 1 : endif
	REM print
endp

