REM wDFile.tpl
REM EPOC OPL interactive test code for dFILE.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "wdFile", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	REM Start looping, think this test is finishing too soon.
	DO
		dINIT "Tests complete" :DIALOG
		print DATIM$, "I'm still alive and I should be dead."
	UNTIL 0
ENDP


PROC wDFile:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dowDFile")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP


CONST KGoodName$="\Opltest\Interactive\wMain\"
CONST KBadName$ ="\BadOpltest\BadInteractive\BadwMain\"


proc dowDFile:
	local goodname$(255),badname$(255),rep&
	goodname$=hDiskName$:+KGoodName$
	badname$=hDiskName$:+KBadName$

	dINIT "dFILE tests"
	dFILE goodname$,"File",4
	dFILE badname$,"Badfile",4
	PRINT "Hit Enter."
	IF DIALOG=0 : RAISE 1 :ENDIF
	IF goodname$<>hDiskName$:+KGoodName$ : rep&=rep&+2 :ENDIF
	IF badname$<>hDiskName$:+"\" : rep&=rep&+4 :ENDIF
	IF rep& : RAISE rep& : ENDIF
	RETURN
endp


proc old:
	local name$(255),flags&
	
	do
		cls
		print "Next flags=&";hex$(flags&)
		if flags& and 1
			print "file editor"
		else
			print "file selector"
		endif
		if (flags& and 2)=0
			print "don't ";
		endif
		print "allow directories"
		if (flags& and 4)=0
			print "not ";
		endif
		print "directories only"
		if flags& and 1			:rem rest for file editor only
			if flags& and 8
				print "dis";
			endif
			print "allow existing files"
			if (flags& and 16)=0
				print "don't ";
			endif
			print "query existing files"
			if (flags& and 32)=0
				print "don't ";
			endif
			print "allow null strings"
		endif
		at 1,9 :print "Name=";name$;
		get
		dInit "dFile test"
		dPosition 1,1
		if flags& and 1
			dFile name$,"Editor",flags&
		else
			dFile name$,"Selector",flags&
		endif
		dLong flags&,"Next Flags",0,&7fffffff
	until dialog=0
endp

REM End of wDFile.tpl
