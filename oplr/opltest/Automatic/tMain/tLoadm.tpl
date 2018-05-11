REM tLoadm.tpl
REM EPOC OPL automatic test code for LOADM.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tLoadm", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tLoadm:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("doLoadm0")
	hRunTest%:("doLoadm1")
	rem hCleanUp%:("CleanUp")
endp


REM---------------------------------------------------------------------------

CONST Kpath$="\Opltest\Automatic\tMain\"

proc doloadm0:
	REM strtest:("LOADM")
	trap loadm "notexist"
	IF err<>KErrNoMod% :RAISE 100 :ENDIF
	trap loadm Kpath$+"tLoadm"
	IF err<>KErrModLoad% :RAISE 101 :ENDIF
	REM Already loaded.

	loadm KPath$+"tArray"
	loadm KPath$+"tBPCal"
	loadm KPath$+"tBPFile"
	loadm KPath$+"tBPList"
	loadm KPath$+"tBPPerc"

	REM Running under harness app gives:
	REM 1. harness app.
	REM 2. hUtils.opo
	REM 3-8. tArray-TBPerc.opo above.

	REM So, if running standalone, we need to get one more.
	IF hRunningUnderSystemTestApp%:=KFalse%
		loadm KPath$+"tCmp"
	ENDIF
	
	trap loadm KPath$+"tDFunc"
	IF err<>KErrMaxLoad% :RAISE 102 :ENDIF

	trap unloadm "tDFunc"
	IF err<>KErrModNotLoaded% :RAISE 103 :ENDIF

	tArrayLoadmTest:	:rem Ensure that a procedure can be called (from tArray)

	unloadm KPath$+"tArray"
	unloadm KPath$+"tBPCal"
	unloadm KPath$+"tBPFile"
	unloadm KPath$+"tBPList"
	unloadm KPath$+"tBPPerc"

	IF hRunningUnderSystemTestApp%:=KFalse%
		unloadm KPath$+"tCmp"
	ENDIF
endp


proc doloadm1:
	local hdr$(22),ret%,ret1%,fcb%,mode%,fname$(20)
	trap delete "c:\t_loadm.odb"
	create "c:\t_loadm.odb",a,a$
	close
	trap loadm "c:\t_loadm.odb"         :rem wrong header
	rem alert("1")
	rem print err
	raiseIfN:(KErrBadFileType%)           :rem Bad file type
	rem alert("2")
	trap delete "c:\t_loadm.odb"

	hdr$="OPLObjectFile**"+chr$(0)
	fName$="c:\t_corrhd.opo"
	mode%=$102                   :rem UPDATE|STREAM|REPLACE
	ret%=ioOpen(fcb%,fName$,mode%)
	if ret%<0 : raise ret% :endif
	ret1%=ioWrite(fcb%,addr(hdr$)+1+KOplAlignment%,16) :rem header too short
	ret%=ioClose(fcb%)
	if ret1%<0 :raise 1 :endif
	if ret%<0 : raise ret% :endif
	trap loadm fName$            :rem corrupt header
	raiseIfN:(KErrBadFileType%)           :rem Bad file type
	delete fName$
endp


proc raiseIfN:(e%)
  rem raise error if it is not the same as the parameter passed
	if err=0 :raise 100 :endif
	if err<>e% :onerr off :raise err :endif
endp


REM End of tLoadM.tpl
