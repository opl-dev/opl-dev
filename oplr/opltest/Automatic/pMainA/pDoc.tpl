REM pDoc.tpl
REM EPOC OPL automatic test code for SetDoc/GetDoc
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pDoc", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pDoc:
	global id%
	REM print "Opler1 SetDoc and GetDoc Test"
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tCreate")
	hRunTest%:("tgSavebit")
	hCleanUp%:("CleanUp")
endp


PROC CLeanUp:
	IF id% :gCLOSE id% :id%=0 :ENDIF
ENDP


proc tCreate:
	rem create a document and check it is current
	local doc$(255)

	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)

	trap delete "opltest1.odb"
	trap delete "opltest2.odb"
	trap delete "opltest3.odb"
	trap delete "opltest4.odb"

	rem set and create doc
	setdoc "opltest1.odb"
	create "opltest1.odb",a,aa$,ab$
	REM PRINT "Created opltest1.odb and set to current document"
	doc$=getdoc$
	if doc$<>"opltest1.odb" : raise 1 : endif
	REM PRINT "Current document is "+doc$
	REM print
	if not exist ("opltest1.odb") : raise 2 : endif
	REM pause 20
	
	rem create doc without setting
	trap create "opltest2.odb",b,ba$,bb$
	REM print "Created opltest2.odb but not set to current document"
	doc$=getdoc$
	if doc$<>"opltest1.odb" : raise 3 : endif
	REM print "Current document is "+doc$	
	REM print
	if not exist ("opltest2.odb") : raise 4 : endif
	REM pause 20
	
	rem set and create docs of different names
	setdoc "opltest3.odb"
	create "opltest4.odb",c,ca$,cb$
	REM print "Created opltest4.odb and set opltest3.odb (uncreated) to current document"
	doc$=getdoc$
	if doc$<>"opltest3.odb" : raise 5 : endif
	REM print "Current document is "+doc$
	if exist ("opltest3.odb") : raise 6 : endif
	if not exist ("opltest4.odb") : raise 7 : endif
	REM print
	REM pause 20
		
	trap close
	trap close
	trap close
	trap close
	trap delete "opltest1.odb"
	trap delete "opltest2.odb"
	trap delete "opltest4.odb"
endp


proc tgSavebit:
	rem save a bitmap and check it is current
	
	local doc$(255)
	
	trap delete "opltest1.bmp"
	trap delete "opltest2.bmp"
	trap delete "opltest3.bmp"
	trap delete "opltest4.bmp"
	
	rem set and save doc
	createbmp:
	setdoc "opltest1.bmp"
	gsavebit "opltest1.bmp"
	gclose id% :id%=0
	REM print "Saved opltest1.bmp and set to current document"
	doc$=getdoc$
	if doc$<>"opltest1.bmp" : raise 1 : endif
	REM print "Current document is "+doc$
	REM print
	if not exist ("opltest1.bmp") : raise 2 : endif
	REM pause 20
	
	rem save doc without setting
	createbmp:
	gsavebit "opltest2.bmp"
	gclose id% :id%=0
	REM print "Saved opltest2.bmp but not set to current document"
	doc$=getdoc$
	if doc$<>"opltest1.bmp" : raise 3 : endif
	REM print "Current document is "+doc$	
	REM print
	if not exist ("opltest2.bmp") : raise 4 : endif
	REM pause 20
	
	rem set and save docs of different names
	setdoc "opltest3.bmp"
	createbmp:
	gsavebit "opltest4.bmp"
	gclose id% :id%=0
	REM print "Saved opltest4.bmp and set opltest3.bmp (unsaved) to current document"
	doc$=getdoc$
	if doc$<>"opltest3.bmp" : raise 5 : endif
	REM print "Current document is "+doc$
	if exist ("opltest3.bmp") : raise 6 : endif
	if not exist ("opltest4.bmp") : raise 7 : endif
	REM print
	REM pause 20

	trap delete "opltest1.bmp"
	trap delete "opltest2.bmp"
	trap delete "opltest4.bmp"
endp


proc createbmp:
	rem create bitmap
	id%=gcreatebit (50,50)
	gat 25,25
	gcircle 20
	gcircle 15
	gcircle 10
	gcircle 5
endp


REM End of pDoc.tpl
