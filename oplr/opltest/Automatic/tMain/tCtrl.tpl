REM tCtrl.tpl
REM EPOC OPL automatic test code for control opcodes.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tCtrl", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	rem dINIT "Tests complete" :DIALOG
	GIPRINT "Tests complete" :PAUSE 10
ENDP


proc tCtrl:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tTrap")
	hRunTest%:("tBraFals")
	hRunTest%:("tGoto")
rem	hRunTest%:("tOnerr")
	hRunTest%:("tRaiseN")

	hCleanUp%:("CleanUp")
endp


PROC CleanUp:
	TRAP DELETE "tTrap.odb"
	TRAP DELETE "tTrapR.odb"
	TRAP DELETE "tTrapR2.odb"
ENDP


REM--------------------------------------------------------------------------

proc tBraFals:
	rem Branch if false test
	rem Tests WHILE, UNTIL and IF

	local i%,j%

rem  SubTest:("WHILE,UNTIL,IF")
  i%=0
  while i%<10	: rem WHILE loop ascending
		i%=i%+1
		j%=100
		do				: rem DO loop descending
			j%=j%-1
		until j%=0
		if j%<>0
rem			beep 1,2000
rem			print "Loop 1 error:j%=";j%
rem			pause pause% :key
			raise 1
		endif
	endwh
	if i%<>10
		rem print err$(err)
		rem pause pause%
		raise 2
	endif

	i%=0
  do						: rem DO loop ascending
		i%=i%+1
		j%=100
		while j%>0	: rem WHILE loop descending
			j%=j%-1
		endwh
		if j%<>0 
			rem beep 1,2000
			rem print "Loop 2 error:j%=";j%
			rem pause pause%
			raise 3
		endif
	until i%=100
endp


proc tGoTo:
	REM Test GOTO
	REM Builds a string in m$

  local i%,m$(39) : rem 39 is the length of the string built
	local target$(39)

rem  SubTest:("GOTO")
  target$="This is a strange way to build a string"

initI::
  i%=0
	goto initM::
initM::
	m$=""
	if i%<101 : goto newStr:: :endif

	raise 4

	while i%<100	: rem build string 100 times
		i%=i%+1
		goto initM::
newStr::
this::
		m$="This " : goto is::
toBuildA::
		m$=m$+"to build a " :goto string::
a::
		m$=m$+"a " : goto strange::
strange::
		m$=m$+"strange " :goto way::
is::
		m$=m$+"is " :goto a::
way::
		m$=m$+"way " :goto toBuildA::
string::
		m$=m$+"string"

		if m$<>target$
			goto raise::
    endif

	endwh
	goto end::
	raise 1  : rem Shouldn't reach here
	return
raise::
rem	print "The string built was",m$
rem	print "It should have been",target$
rem	pause 30
	raise 10
end::

endp


proc  tRaiseN:
	REM Raise control test
	local i%

rem  SubTest:("RAISE")
	i%=-128
  while i%<128
    onerr lblRaise::
		raise i%
		onerr off
		print 1/0		: rem force an error
lblRaise::
		onerr off
		if err<>i%
			print 1/0
		endif
    i%=i%+1
  endwh
endp


proc tTrap:
	local badName$(50)
	
	REM Try all the traps (errors returned are checked elsewhere)

rem	subTest:("TRAP control")
	badName$="c::bad:name"
	rem beep 1,2000
	rem pause pause% :key

	trap close	:rem ensure all closed so that all following generate errors
	trap close
	trap close
	trap close
	trap close	:rem ensures TRAP CLOSE is tested when 4 files are open above
	if err<>-102 : raise 1 :endif
	trap append
	if err<>-102 : raise 2 :endif
	trap back
	if err<>-102 : raise 3 :endif
	trap copy badName$,"any.odb"
	if err<>-38 : raise 4 :endif
	trap create badName$,a,a$
	if err<>-38 : raise 5 :endif
	trap delete "badName$"
	if err<>-33 : raise 6 :endif
	trap erase
	if err<>-102: raise 7 :endif
	trap first
	if err<>-102 : raise 8 :endif
	trap last
	if err<>-102 : raise 9 :endif
	trap next
	if err<>-102 : raise 10 :endif
	trap open "badName$",a,a$
	if err<>-33 : raise 11 :endif
	trap openR "badName$",a,a$
	if err<>-33 : raise 12 :endif
	trap position 2
	if err<>-102 : raise 13 :endif
	trap rename "badName$","any.odb"
	if err<>-33 : raise 14 :endif
	trap update
	if err<>-102 : raise 15 :endif
	trap use a
	if err<>-102 : raise 16 :endif
	trap loadm "NotExist"
	if err<>-106 : raise 17 :endif
	trap unloadm "NotExist"
	if err<>-108 : raise 18 :endif
	
	REM trap edit is tested in t_Commnd.opl
	REM trap input "   "    "     "

  	REM Test that each switches the trap flag off on success
 
	trap delete "tTrap.odb"
	onerr e1::
	use c
	raise 1			: rem DELETE does not clear trap
e1::
   onerr off
	rem DELETE clears trap
 
   trap create "tTrap.odb",a,a$,b$
	if err
		rem print err$(err)
		rem print err
		rem pause pause%
		raise 2
	endif
	onerr e2::
	use c
	raise 3
e2::
  onerr off
  rem CREATE clears trap

	close
	trap open "tTrap.odb",a,a$,b$
	onerr e97::
	use c
	raise 97
e97::
  onerr off
	rem OPEN clears trap

	a.a$="ar1f1"
	append
	a.a$="ar2f1"
	append
	a.a$="ar3f1"
	trap append	: rem 3 records
	onerr e4::
	use c
	raise 5
e4::
  onerr off
	rem APPEND clears trap

	trap back
	onerr e6::
	use c
	raise 6
e6::
  onerr off
	rem BACK clears trap

	trap erase	:rem 2 records
	onerr e7::
	use c
	raise 7
e7::
  onerr off
	rem ERASE clears trap

	trap first
	onerr e8::
	use c
	raise 8
e8::
  onerr off
	rem FIRST clears trap

	trap last
	onerr e9::
	use c
	raise 9
e9::
  onerr off
	rem LAST clears trap

	trap next
	onerr e10::
	use c
	raise 10
e10::
  onerr off
	rem NEXT clears trap

	trap position 1
	onerr e11::
	use c
	raise 11
e11::
  onerr off
	rem POSITION clears trap

	a.a$="UPDATE"
	trap update
	onerr e12::
	use c
	raise 12
e12::
  onerr off
	rem UPDATE clears trap

	trap use a
	onerr e21::
	use c
	raise 21
e21::
  onerr off
	rem USE clears trap

	trap delete "tTrapR2.odb"	:rem for read only
	create "tTrapR2.odb",b,a$,b$
	trap close
	onerr e98::
	use c
	raise 98
e98::
  onerr off
	rem CLOSE clears trap

	trap rename "tTrapR2.odb","tTrapR.odb"
	onerr e95::
	use c
	raise 95
e95::
	onerr off
	rem RENAME clears trap

	trap copy "tTrapR3.odb","tTrapR.odb"
	onerr e99::
	use c
	raise 99
e99::
  onerr off
	rem COPY clears trap

	trap openR "tTrapR.odb",b,a$,b$	: rem open for read only
	onerr e3::
	use c
	raise 4
e3::
  onerr off
	rem OPENR clears trap

	trap loadm "tCommand"
	onerr e94::
	use c
	raise 94
e94::
	onerr off
	rem LOADM clears trap
	trap unloadm "tCommand"
	onerr e93::
	use c
	raise 93
e93::
	onerr off
	rem UNLOADM clears trap

  trap close
  if err<>0 : raise 20 :endif
  trap close
  if err<>0 : raise 21 :endif
  trap close
  if err<>-102 : raise 22 :endif
  trap close
  if err<>-102 : raise 23 :endif
	rem pause pause% :key
rem	hLog%:(KhLogAlways%,"Finished")
endp

REM--------------------------------------------------------------------------
rem proc  tOnerr:
rem
rem 				tOnerr is in T_UTIL.OPL for use by T_MAIN.OPL
rem
rem endp
REM--------------------------------------------------------------------------


