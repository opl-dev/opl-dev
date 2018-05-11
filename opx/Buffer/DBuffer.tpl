rem DBUFFER.OPL
rem
rem Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.
rem
rem Demo of Buffer OPX

DECLARE EXTERNAL

INCLUDE "BUFFER.OXH"
EXTERNAL tInteractive:
EXTERNAL tAutomatic:

CONST KMaxlen&=4096

PROC Main:
	tAutomatic:
	tInteractive:
ENDP

CONST KFill$="A set of automatic tests to demo the Buffer.OPX API"
rem 123456789012345678901234567890123456789012345678901

rem A set of automatic tests to demo the API of Buffer.OPX API
PROC tAutomatic:
	LOCAL buffer&,buffLen&,buffer2&
	LOCAL newString$(255),loc&
	buffer&=ALLOC(KMaxLen&)

	rem Fill the buffer with a string
	buffLen&=BufferFromString&:(buffer&,LEN(KFill$),KFill$)
	IF buffLen&<>LEN(KFill$) :RAISE 1 :ENDIF
	rem Sanity check.
	IF CHR$(PEEKW(buffer&+6))<>"e" :RAISE 2: ENDIF
	IF CHR$(PEEKW(buffer&+8))<>"t" :RAISE 3: ENDIF

	rem Copy the string buffer to a second buffer
	buffer2&=ALLOC(KMaxLen&)
	BufferCopy:(buffer2&, buffer&, 2*buffLen&)
	rem And check.
	IF CHR$(PEEKW(buffer2&+6))<>"e" :RAISE 4: ENDIF
	IF CHR$(PEEKW(buffer2&+8))<>"t" :RAISE 5: ENDIF

	rem Fill the first 4 chars of buffer with 0xFF.
	BufferFill:(buffer&, 4*2, $FF)
	IF PEEKW(buffer&+6) <> &FFFFFFFF :RAISE 6: ENDIF
	IF CHR$(PEEKW(buffer&+8)) <> "t" :RAISE 7: ENDIF

	rem Check the untouched second buffer can be used as a string.
	newString$=BufferAsString$:(buffer2&,buffLen&)
	IF newString$<>KFill$ :RAISE 6: ENDIF

	rem Exercise the comparison functions...
	loc&=BufferMatch&:(buffer2&, buffLen&, "*set*", KBufferNoFoldOrCollate&)
	IF loc&<>2 :RAISE 8 :ENDIF
	loc&=BufferMatch&:(buffer2&, buffLen&, "*autom?tic*", KBufferNoFoldOrCollate&)
	IF loc&<>9 :RAISE 9 :ENDIF

	loc&=BufferFind&:(buffer2&, buffLen&, "set", KBufferNoFoldOrCollate&)
	IF loc&<>2 :RAISE 10 :ENDIF
	loc&=BufferFind&:(buffer2&, buffLen&, "automatic", KBufferNoFoldOrCollate&)
	IF loc&<>9 :RAISE 11 :ENDIF

	rem search for 'O'
	loc&=BufferLocate&:(buffer2&, buffLen&, %O, KBufferNoFoldOrCollate&)
	IF loc&<>44 :RAISE 12 :ENDIF
	
	rem Reverse search for 'o'
	loc&=BufferLocateReverse&:(buffer2&, buffLen&, %o, KBufferNoFoldOrCollate&)
	IF loc&<>31 :RAISE 13 :ENDIF

	PRINT "Automatic tests passed."
ENDP

PROC tInteractive:
	local buffer&, wbuf&, length&, string$(240), fold&
	local s1$(20), s2$(50), s3$(10), s4$(120), s5$(255), s6$(12), s7$(20), s8$(20)
	local loc&
	wbuf&=ALLOC(KMaxlen& + 4)
	buffer&=wbuf& + 4
	length&=BufferFromString&:(buffer&, KMaxlen&, "This is a test string")
	POKEL wbuf&, length&

	dINIT "Enter some interesting text"
	dEDITMULTI wbuf&, "", 24, 4, KMaxlen&
	dBUTTONS "Done", %d
	IF DIALOG=0 :STOP :ENDIF

	length&=PEEKL(wbuf&)
	PRINT "Insert * in fifth position.",
	loc&=5*2
	BufferCopy:(buffer& + loc&, buffer& + (loc&-2), KMaxlen& - (loc&-2))
	POKEW buffer& + (loc&-2), asc("*")

	PRINT "Delete 8th character.",
	loc&=8*2
	BufferCopy:(buffer& + (loc&-2), buffer& + loc&, KMaxlen& - loc&)

	PRINT "Fill a bit with X."
	BufferFill:(buffer& + 24, 6, %X)

	rem Display it again
	dINIT "After first changes"
	dEDITMULTI wbuf&, "", 24, 4, KMaxlen&
	dBUTTONS "Done", %d
	IF DIALOG=0 :STOP :ENDIF

	length&=PEEKL(wbuf&)
	print "Here are the first 10 characters:",BufferAsString$:(buffer&, 10)
	print "Now testing some matching:"
	Print "Enter fold& : (0/1/2) : ";
	input fold&
	do
		print "Enter Pattern: ";
		input string$
		if string$ <> ""
			print "BufferMatch returns: ";BufferMatch&:(buffer&, length&, string$, fold&)
			print "BufferFind returns: ";BufferFind&:(buffer&, length&, string$, fold&)
		endif
	until string$=""
	print "Now let's search for characters:"
	do
		print "Enter character:",
		input string$
		if string$ <> ""
			print "BufferLocate returns: ";BufferLocate&:(buffer&, length&, asc(string$), fold&)
			print "BufferLocateReverse returns: ";BufferLocateReverse&:(buffer&, length&, asc(string$), fold&)
		endif
	until string$=""
	print "Demo complete."
	print "Press a key"
	get
ENDP