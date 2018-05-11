REM DBUFFER.OPL
REM
REM Copyright (c) 1997-20001 Symbian Ltd. All rights reserved.
REM
REM Demo of Buffer OPX

INCLUDE "BUFFER.OXH"

CONST KMaxlen&=4096

PROC Main:
	local buffer&, wbuf&, length&, string$(240), fold&
	local s1$(20), s2$(50), s3$(10), s4$(120), s5$(255), s6$(12), s7$(20), s8$(20)
	wbuf&=ALLOC(KMaxlen& + 4)
	buffer&=wbuf& + 4
	length&=BufferFromString&:(buffer&, KMaxlen&, "This is a test string")
	POKEL wbuf&, length&
	dINIT "Enter some interesting text"
	dEDITMULTI wbuf&, "", 24, 4, KMaxlen&
	dBUTTONS "Done", %d
	DIALOG
	length&=PEEKL(wbuf&)
	rem Insert * in fifth position
	BufferCopy:(buffer& + 5, buffer& + 4, KMaxlen& - 4)
	POKEB buffer& + 4, asc("*")
	rem Delete 8th character
	BufferCopy:(buffer& + 7, buffer& + 8, KMaxlen& - 8)
	rem Fill a bit
	BufferFill:(buffer& + 50, 20, %X)
	rem Display it again
	dINIT "After first changes"
	dEDITMULTI wbuf&, "", 24, 4, KMaxlen&
	dBUTTONS "Done", %d
	DIALOG
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
	print "Press a key"
	get
ENDP