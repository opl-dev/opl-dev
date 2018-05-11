REM wDir.tpl
REM EPOC OPL automatic test code for DIR, SETROOT etc.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wdir", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc wdir:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dowdir")
	hCleanUp%:("CleanUp")
endp

CONST kLoc$="c:\Opl1993_tdir\"

proc Cleanup:
	trap delete kLoc$+"*.*"
	trap rmdir kLoc$
endp

proc dowDir:
	local nRoot$(128),n$(128),s$(50)

	nRoot$=kLoc$
	n$=kLoc$+"wdir\"
	trap delete n$+"wdir"
	trap rmdir n$
	trap delete nRoot$+"*.*"
	trap rmdir nRoot$

	mkdir nRoot$
	setpath nRoot$
	mkdir n$
	setPath n$
	create n$+"wdir",a,s$
	s$="Created "+kLoc$+"\wdir\wdir.odb successfully"
	a.s$=s$
	append
	close

	open kLoc$+"wdir\wdir",a,s$
	rem print a.s$
	close
	rem print "dir$(""c:\wdir\wdir\wdir"")=";dir$("c:\wdir\wdir\wdir")
	delete kLoc$+"wdir\wdir"
	setpath nRoot$
	rmDir n$
	setPath "c:\"
	rmDir nRoot$
	onerr errHand::
	if dir$(kLoc$)<>""
		raise 1
		rem print "rmDir failed"
		rem get
	endif
	rem print "Finished ok"
	return
	
errHand::
	onerr off
	if err<>-42
		print err
		raise err
	endif
endp


REM End of wDir.tpl

