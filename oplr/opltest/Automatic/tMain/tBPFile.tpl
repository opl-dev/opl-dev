REM tBPFile.tpl
REM EPOC OPL automatic test code for file handling.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tBPFile", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tBPFile:
	global tstName$(255), pause%
	global path$(9),drv$(2),patha$(9)
	
	drv$="c:"
	path$=drv$
	patha$=drv$+"\Opl1993"
	trap mkdir patha$
	
	hRunTest%:("BPFile1")
	hRunTest%:("BPFile2")
	hRunTest%:("BPFile3")
	hRunTest%:("BPFile4")
	hRunTest%:("BPFile5")
	hRunTest%:("BPFile6")
	hRunTest%:("BPFile7")
	hRunTest%:("BPFile8")
	hRunTest%:("BPFile9")
	hRunTest%:("BPFile10")
	hRunTest%:("BPFile11")
	
	hCleanUp%:("CleanerUpper")
endp


proc CleanerUpper:
	trap delete patha$+"\*.*"
	trap rmdir patha$
	trap delete "a.odb"
	trap delete "ab.odb"
endp


proc BPFILE1:
	REM print "Start Of BPFILE1"
	REM print "Tests creating files and wildcard copying"
	REM print "Note that copying an open file may result in an incorrect copy "
	
	REM print "Deleting all ODB files on c:\Opl1993\"
	trap delete patha$+"\*.ODB"
	if err and err<>-33
		raise err
	endif
	REM print "Deleting ODB files on c:\"
	trap delete "a.odb"
	if err and err<>-33
		raise err
	endif
	trap delete "ab.odb"
	if err and err<>-33
		raise err
	endif
	
	REM print "Creating ODB files on c:\Opl1993\"
	create patha$+"\a.odb",c,c$ :close
	if not exist(patha$+"\a.odb")
		raise 1
	endif
	create patha$+"\ab.odb",a,a$ :close
	if not exist(patha$+"\ab.odb")
		raise 2
	endif
	create patha$+"\abc.odb",b,b$ :close
	if not exist(patha$+"\abc.odb")
		raise 3
	endif
	create patha$+"\abcd.odb",d,b$ :close
	if not exist(patha$+"\abcd.odb")
		raise 4
	endif
	create patha$+"\abcde.odb",d,a$ :close
	if not exist(patha$+"\abcde.odb")
		raise 5
	endif
	
	REM print "Copying ODB files from c:\Opl1993\ to current directory"
	trap copy patha$+"\?.odb","" :rem "a.odb"
	if err :raise 6 :endif
	if not exist("a.odb")
		raise 7
	endif
	trap copy patha$+"\??.odb","" :rem "ab.odb"
	if err :raise 8 :endif
	if not exist("ab.odb")
		raise 9
	endif
	
	REM print "Renaming a:abcde.odb to a:sap.dat"
	trap delete patha$+"\sap.dat"
	rename patha$+"\abcde.odb",patha$+"\sap.dat"
	if not exist(patha$+"\sap.dat")
		raise 10
	endif
	if exist(patha$+"\abcde.odb")
		raise 11
	endif
	
	REM print "End of BPFILE1" :pause -16 :key
endp


proc BPFILE2:
	local a$(255)
	
	REM print "BPFILE2 has been removed becuase of LOC::"
endp


proc BPFILE3:
	local x
	
	REM print "Start of BPFILE3"
	REM print "Tests APPEND,POS,NEXT"
	
	if exist (patha$+"\CHECK")
		REM print "Deleting"
		delete patha$+"\check"
	endif
	create patha$+"\check",a,field$
	REM print "Creating"
	
	REM print "Checking"
	while x<5
		x=x+1
		if POS <>x
			REM print "ERROR in pos before APPEND - is ";POS;" should be ";x
			RAISE 300
		endif
		append
		if POS <>x
			REM print "ERROR in pos after APPEND - is ";POS;" should be ";x
			RAISE 301
		endif
		next
		if POS <>x+1
			REM print "ERROR in pos after NEXT - is ";POS;" should be ";x
			RAISE 302
		endif
	endwh
	
	REM print "Closing"
	close
	REM print "End of BPFILE3" :pause -16 :key
endp


proc BPFILE4:
	local x%
	
	REM print "Start of BPFILE4"
	REM print "Tests APPEND,POS,LAST,BACK" :print
	
	REM print "Checking for existence"
	if exist (patha$+"\CHECK")
		delete patha$+"\check"
		REM print "Deleting"
	endif
	REM print "Creating"
	create patha$+"\check",a,num%,field$
	
	REM print "Appending"
	while x%<12
		x%=x%+1
		a.num%=x% :a.field$=month$(x%) :append
	endwh
	
	last :x%=pos
	REM print "Positioning to end of file"
	while x%
		REM print pos,a.num%,a.field$
		back :x%=x%-1
	endwh
	
	REM print "Closing"
	close
	REM print "End of BPFILE4" :pause -16 :key
endp


proc BPFILE5:
	local x%
	local oldpos,newpos
	local oldnum%,newnum%
	local oldfield$(255),newfield$(255)
	
	REM print "Start of BPFILE5"
	REM print "Tests APPEND,POS,NEXT,BACK" :print
	
	REM print "Checking for existence"
	if exist (patha$+"\CHECK")
		delete patha$+"\check"
		REM print "Deleting"
	endif
	create patha$+"\check",a,num%,field$
	REM print "Creating"
	
	REM print "Appending"
	while x%<12
		x%=x%+1
		a.num%=x% :a.field$=month$(x%) :append
	endwh
	
	first
	while not eof
		REM print pos,a.num%,a.field$
		oldpos=POS :oldnum%=a.num% :oldfield$=a.field$
		next :back
		newpos=POS :newnum%=a.num% :newfield$=a.field$
		REM print pos,a.num%,a.field$," - should be same as line above"
		IF oldpos<>newpos OR oldnum%<>newnum% OR oldfield$<>newfield$
			RAISE 500
		ENDIF
		next
	endwh
	
	REM print "Closing"
	close
	REM print "End of BPFILE5" :pause -16 :key
endp


proc BPFILE6:
	local x%
	
	REM print "Start of BPFILE6"
	REM print "Tests POSITION" :REM print
	
	REM print "Checking for existence"
	if exist (patha$+"\CHECK")
		delete patha$+"\check"
		REM print "Deleting"
	endif
	create patha$+"\check",a,num%,field$
	REM print "Creating"
	
	REM print "Appending"
	while x%<12
		x%=x%+1
		a.num%=x% :a.field$=month$(x%) :append
	endwh
	
	first :x%=2
	while not eof
		position x%
		REM print x%,pos,a.num%,a.field$
		x%=x%+2
	endwh
	
	REM print "Closing"
	close
	REM print "End of BPFILE6" :pause -16 :key
endp


proc BPFILE7:
	local x%
	
	REM print "Start of BPFILE7"
	REM print "Tests OPENR,POSITION" :REM print
	
	REM print "Checking for existence"
	if not exist (patha$+"\CHECK")
		create patha$+"\check",a,num%,field$
		REM print "Creating"
		REM print "Appending"
		while x%<12
			x%=x%+1
			a.num%=x% :a.field$=month$(x%) :append
		endwh
		close
	endif
	
	openr patha$+"\check",a,num%,field$
	REM print "Opening Read Only"
	
	first :x%=2
	while not eof
		position x%
		REM print x%,pos,a.num%,a.field$
		x%=x%+2
	endwh
	
	onerr err::
	a.num%=666 :a.field$="LAST" :append
	REM print "ERROR"
	RAISE 700
	return
	
	err::
	onerr off
	REM print "Can't append to a Read Only file"
	first :x%=1
	while not eof
		position x%
		REM print x%,pos,a.num%,a.field$
		x%=x%+1
	endwh
	
	REM print "Closing"
	close
	REM print "End of BPFILE7" :pause -16 :key
endp

proc BPFILE8:
	local x%
	
	REM print "Start of BPFILE8"
	REM print "Tests ERASE,POSITION" :REM print
	
	REM print "Checking for existence"
	if exist (patha$+"\CHECK")
		delete patha$+"\check"
		REM print "Deleting"
	endif
	create patha$+"\check",a,num%,field$
	REM print "Creating"
	
	REM print "Appending"
	x%=1
	while x%<13
		a.num%=x% :a.field$=month$(x%) :append
		x%=x%+1
	endwh
	
	REM print "Erasing"
	first :x%=2
	while x%<=count
		position x% :erase
		x%=x%+2
	endwh
	
	REM print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	
	REM print "Closing"
	close
	REM print "End of BPFILE8" :pause -16 :key
endp

proc BPFILE9:
	local f%
	
	REM print "Start of BPFILE9"
	REM print "Tests FIND,POS,APPEND" :REM print
	
	REM print "Checking for existence"
	if exist (patha$+"\CHECK")
		delete patha$+"\check"
		REM print "Deleting"
	endif
	create patha$+"\check",a,num%,field$
	REM print "Creating"
	
	REM print "Appending"
	f%=1
	while f%<13
		a.num%=f% :a.field$=month$(f%) :append
		f%=f%+1
	endwh
	
	first
	REM print "Finding"
	do
		f%=find("*?a?")
		REM print f%,pos,a.num%,a.field$
		next
	until f%=0
	
	REM print "Erasing"
	first :f%=2
	while f%<=count
		position f% :erase
		f%=f%+2
	endwh
	
	first
	f%=find("*Feb*")
	if f%
		REM print "ERROR1"
		RAISE 900
	endif
	
	f%=find("6")
	if f%<>0
		REM print "ERROR2"
		RAISE 901
	else
		REM print f%,pos,a.num%,a.field$
	endif
	
	REM print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	
	REM print "Closing"
	close
	REM print "End of BPFILE9" :pause -16 :key
endp


proc BPFILE10:
	local x%
	
	REM print "Start of BPFILE10"
	REM print "Tests ERASE,POSITION" :REM print
	
	REM print "Checking for existence"
	if exist (patha$+"\CHECK.odb")
		delete patha$+"\*.odb"
		REM print "Deleting"
	endif
	create patha$+"\check.odb",a,num%,field$
	REM print "Creating"
	
	REM print "Appending"
	x%=1
	while x%<13
		a.num%=x% :a.field$=month$(x%) :append
		x%=x%+1
	endwh
	
	REM print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	
	REM print "Erasing"
	first :x%=2
	while x%<=count
		position x% :erase
		x%=x%+2
	endwh
	
	REM print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	
	REM print "Closing"
	close
	
	REM print "Compacting"
	compact patha$+"\check"
	open patha$+"\check",a,num%,field$
	REM print "Opening"
	
	REM print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	REM print "Closing"
	close
	
	REM print "End of BPFILE10" :pause -16 :key
endp


proc BPFILE11:
	local x%,y%
	
	REM print "Start of BPFILE11"
	REM print "Tests ERASE,POSITION,CLOSE" :REM print
	
	REM print "Checking for existence"
	if exist (patha$+"\CHECK.odb")
		delete patha$+"\*.odb"
		REM print "Deleting"
	endif
	create patha$+"\check.odb",a,num%,field$
	REM print "Creating"
	
	print "Appending"
	x%=1
	while x%<301
		a.num%=x%
		y%= x%-12*(x%/12)
		if y%=0
			y%=12
		endif
		a.field$=month$(y%) :append
		x%=x%+1
	endwh
	
	print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	
	print "Erasing"
	first :x%=2
	while x%<=count
		position x% :erase
		x%=x%+2
	endwh
	
	print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	
	REM print "Closing"
	close
	
	print "Opening"
	open patha$+"\check.odb",a,num%,field$
	
	print "Displaying"
	first
	while not eof
		REM print pos,a.num%,a.field$
		next
	endwh
	print "Closing"
	close
	
	REM print "End of BPFILE11" :pause -16 :key
endp


REM End of tBPFile.tpl
