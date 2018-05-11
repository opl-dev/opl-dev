REM pDFile.tpl
REM EPOC OPL interactive test code for dFILE.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pDFile", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pDFile:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("dopDFile")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
ENDP

CONST KGoodName$="\Opltest\Interactive\pMainI\pDialog.opo"
CONST KBadName$ ="\BadOpltest\BadInteractive\BadwMain\Badfile.bad"


proc dopDFile:
	local goodname$(255),badname$(255),rep&
	local uid1&,uid2&,uid3&
	goodname$=hDiskName$:+KGoodName$
	badname$=hDiskName$:+KBadName$

	uid1&=&10000037
	uid2&=&10000073
	uid3&=&10000168

	dINIT "dFILE tests"
	dFILE goodname$,"File",0,uid1&,uid2&,uid3&
	dFILE badname$,"Badfile",1,uid1&,uid2&,uid3&
	PRINT "Hit Enter."
	IF DIALOG=0 : RAISE 1 :ENDIF
	IF goodname$<>hDiskName$:+KGoodName$ : rep&=rep&+2 :ENDIF
	IF badname$<>hDiskName$:+"\Badfile.bad" : rep&=rep&+4 :ENDIF
	IF rep& : RAISE rep& : ENDIF
	RETURN
endp


proc old_tdfile:
	global flags&,name$(255),uid1&,uid2&,uid3&,useUid%
	print "Opler1 dFile Tests"
	print
	tprompt:
	tdialog:
	print
	print "Opler1 dFile Tests Finished OK"
	pause -30
endp

proc old_tprompt:
	print "Test dFile dialog entry with empty prompts"
	print "This should have no prompts for the first 3 boxes"
	get
	dialog:("","")
endp

proc old_tdialog:
	local ret%
	print
	print "Test dFile selector/editor with flags and uids"
	print "You should check the following:"
	print "1. Flags work as expected, in particular that"
	print "i) Disk z is hidden unless flag $100 used"
	print "ii) Flag $40 does nothing"
	print "iii) Flag $200 shows system folder"
	print "2. UIDs work as expected"
	get
	do 
		dialog:("Edit File,Folder,Disk","Select File,Folder,Disk")
		ret%=printflags:
	until (ret% and $ff)=$1b
endp

proc old_dialog:(promptE$,promptS$)
	rem set up file selector/editor dialog
	dInit "dFile test"
	dPosition 1,1
	if (flags& and 1) or useUid%=2
		dFile name$,promptE$,flags&
	else
		dFile name$,promptS$,flags&,uid1&,uid2&,uid3&
	endif
	dChoice useUid%,"Use Uids Next Time","Yes,No"
	dLong uid1&,"Next Uid1",0,&7fffffff
	dLong uid2&,"Next Uid2",0,&7fffffff
	dLong uid3&,"Next Uid3",0,&7fffffff
	dLong flags&,"Next Flags",0,&7fffffff
	dialog
endp

proc old_printflags:
	local ret%
	cls
	print "Next flag=&";hex$(flags&)
	print "Next uid1=&";hex$(uid1&)
	print "Next uid2=&";hex$(uid2&)
	print "Next uid3=&";hex$(uid3&)
	if flags& and 1
		print "File editor"
	else
		print "File selector"
	endif
	if (flags& and 2)=0
		print "Don't ";
	endif
	print "Allow Folders"
	if (flags& and 4)=0
			print "Not ";
	endif
	print "Folders only"
	
	rem these for file editor only
	if flags& and 1			
		if flags& and 8
			print "Dis";
		endif
		print "allow existing files"
		if (flags& and 16)=0
			print "Don't ";
		endif
		print "Query existing files"
		if (flags& and 32)=0
			print "Don't ";
		endif
		print "Allow null strings"
	endif
	
	if (flags& and 128)=0
		print "Don't ";
	endif
	print "Obey/Allow Wildcards"
	
	rem this for file selector only
	if (flags& and 1)=0
		if (flags& and 256)=0
			print "Don't ";
		endif
		print "Allow ROM Files"
	endif
	
	if (flags& and $200)=0
		print "Don't ";
	endif
	print "Show system folder"
	
	at 1,20 : print "File=";name$
	print "Press Esc to end or Enter to continue"
	ret%=get
	return ret%
endp


REM End of pDFile.tpl
