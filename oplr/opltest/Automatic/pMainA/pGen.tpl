REM pGen.tpl
REM EPOC OPL automatic test code for general OPL changes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pGen", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


rem General Changes Tests

rem globals with long names
const ConstantNameWithThirtyTwoCharas%=1
const ConstantNameWithThirtyTwoCharas&=&1
const ConstantNameWithThirtyTwoCharasF=1.0
const ConstantNameWithThirtyTwoCharas$="a"

proc pGen:
	rem globals with long names
	global GlobalVaNameWithThirtyTwoCharas%
	global GlobalVaNameWithThirtyTwoCharas&
	global GlobalVaNameWithThirtyTwoCharasF
	global GlobalVaNameWithThirtyTwoCharas$(1)
	global drv$(255),path$(255),patha$(255)
	
	drv$="c:"
  path$=drv$
  patha$=path$+"\opler1\"
  trap mkdir patha$

	hRunTest%:("tProcedureNameWithThirtyTwoChars")
	hRunTest%:("t_Underscores")
	hRunTest%:("tFileExt")
	trap delete patha$+"*.*"
	trap rmdir patha$
endp

proc tProcedureNameWithThirtyTwoChars:
	local VariableNameWithThirtyTwoCharas%
	local VariableNameWithThirtyTwoCharas&
	local VariableNameWithThirtyTwoCharasF
	local VariableNameWithThirtyTwoCharas$(1)
	
	REM print "Test 32 character identifiers"
	REM print
		
	VariableNameWithThirtyTwoCharas%=1
	VariableNameWithThirtyTwoCharas&=&1
	VariableNameWithThirtyTwoCharasF=1.0
	VariableNameWithThirtyTwoCharas$="a"
	
	GlobalVaNameWithThirtyTwoCharas%=2
	GlobalVaNameWithThirtyTwoCharas&=&2
	GlobalVaNameWithThirtyTwoCharasF=2.0
	GlobalVaNameWithThirtyTwoCharas$="b"

	LongParam:(VariableNameWithThirtyTwoCharasF)
	
	REM print
	REM print "OK"
	REM print
endp

proc LongParam:(ParameterNameWithThirtyTwoCharas)
	rem procedure with long parameter
	
	REM print "The following value was passed: ";ParameterNameWithThirtyTwoCharas
endp

proc t_Underscores:
	local _%,_&,__,us_%,u_s&,_us
	local _$(2),__us$(2)
	
	REM print "Test underscores allowed in identifiers"
	
	_%=1 : _&=&1 : __=1.0 : _$="ab"
	us_%=2 : u_s&=&2 : _us=2.0 : __us$="cd"
	
	REM print
	REM print "OK"
	REM print
endp

proc tFileExt:
	local A,aa$(255),ab$(255)
	
	REM print "Test default file extensions no longer exist"
	
	trap delete patha$+"opler1.odb"
	trap delete patha$+"opler1"
	
	trap create patha$+"opler1",A,aa$,ab$
	if err<>0 : raise 1 : endif
	if exist(patha$+"opler1.odb") : raise 2 : endif
	if not exist (patha$+"opler1") : raise 3 : endif
	trap close
	
	trap open patha$+"opler1.odb",A,aa$,ab$
	if err<>0 
		rem print err$(err)
	else raise 4 : endif
	
	trap open patha$+"opler1",A,aa$,ab$
	if err<>0 : 
		rem print err$(err) 
		raise 5 : endif
	close

	if exist(patha$+"opler1.odb") : raise 5 : endif
	if not exist (patha$+"opler1") : raise 6 : endif

  trap delete patha$+"opler1.odb"
	if err<>0 : rem print err$(err) 
	else raise 7 : endif
	
	trap delete patha$+"opler1"
	if err<>0 : rem print err$(err) 
	raise 8 : endif
	
	REM print
	REM print "OK"
endp

REM End of pGen.tpl
