REM tIO.tpl
REM EPOC OPL automatic test code for IO functionality.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

CONST KPath$="c:\Opl1993\"

CONST KNumTextEOLchars%=1 REM Usually OA (or OD).

REM Type information:
CONST KTypeBinary%=1
CONST KTypeText%=2

REM Sizes.
CONST KBinaryBufferSize&=221
CONST KMaxBinaryChunk&=10

CONST KMaxTextLen%=255
CONST KMaxTextLine%=33

CONST KTextBufferSize&=101

CONST KTextFiller$="The quick brown fox jumped over the lazy dog."

DECLARE EXTERNAL

EXTERNAL Prepare:
EXTERNAL CleanUp:
EXTERNAL FreeBuffers:
EXTERNAL BinaryIO:
EXTERNAL TextIO:
EXTERNAL TextAsBinaryIO:
EXTERNAL OpenFile%:(aMode%,aName$) REM Returns handle.
EXTERNAL CloseFile:
EXTERNAL GetBuffer&:(aSize&) REM Returns ptr to buffer.
EXTERNAL PopulateBuffer:(pBuf&,aType%,aLen&)
EXTERNAL PopulateWriteTextBuffer:
EXTERNAL WriteBuffer:(aHandle%,pBuf&,aType%,aLen&)
EXTERNAL ReadBuffer&:(aHandle%,pBuf&,aType%,aMaxLen&) REM Returns len of data read.
EXTERNAL CheckBuffer:(pBuf1&,pBuf2&,aMinimumLen&)
EXTERNAL MemCopy:(pSrc&,pDest&,len%)

EXTERNAL WriteTextBuffer:(aHandle%)
EXTERNAL ReadTextBuffer:(aHandle%)
EXTERNAL CheckTextBuffers:


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "tIO", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC tIO:
	GLOBAL pRBuffer&,pWBuffer& REM Read and write buffer pointers.
	GLOBAL handle% rem Need it when cleaning up...
rem	hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("Prepare")
	hRunTest%:("BinaryIO")
	hRunTest%:("TextIO")
	hRunTest%:("TextAsBinaryReadIO")
	hLog%:(KhLogAlways%,"!!TODO tIO needs to test more IO functionality.")
	hCleanUp%:("CleanUp")
ENDP



rem KIoOpenModeOpen%=$0000
rem KIoOpenModeCreate%=$0001
rem KIoOpenModeReplace%=$0002
rem KIoOpenModeAppend%=$0003 rem KIoOpenModeUnique%=$0004

rem Mode category 2
rem KIoOpenFormatBinary%=$0000
rem KIoOpenFormatText%=$0020

rem Mode category 3
rem KIoOpenAccessUpdate%=$0100
rem KIoOpenAccessRandom%=$0200 rem KIoOpenAccessShare%=$0400


PROC CleanUp:
	EXTERNAL handle%
	local err%
	err%=IOCLOSE(handle%)
	if err%
		hlog%:(khlogalways%,"Error in tio.tpl CleanUp: "+GEN$(err%,6))
	endif
	TRAP DELETE kPath$+"*.*"
	TRAP RMDIR kPath$
	FreeBuffers:
ENDP


PROC FreeBuffers:
	EXTERNAL pRBuffer&,pWBuffer&
	IF pWBuffer& :FREEALLOC pWBuffer& :pWBuffer&=0 :ENDIF
	IF pRBuffer& :FREEALLOC pRBuffer& :pRBuffer&=0 :ENDIF
ENDP


PROC Prepare:
	TRAP MKDIR KPath$
	TRAP DELETE KPath$+"*.*"
ENDP


PROC TextIO:
	EXTERNAL handle%
	LOCAL mode%
	GLOBAL readTextBuffer$(KMaxTextLine%,KMaxTextLen%)
	GLOBAL writeTextBuffer$(KMaxTextLine%,KMaxTextLen%)
	if handle% :CloseFile: :endif
	mode%=KIoOpenModeCreate% OR KIoOpenFormatText% OR KIoOpenAccessUpdate%
	handle%=OpenFile%:(mode%,KPath$+"tIOText2.txt")
	PopulateWriteTextBuffer:
	WriteTextBuffer:(handle%)
	CloseFile:

	REM Verify the file.
	mode%=KIoOpenModeOpen% OR KIoOpenFormatText%
	handle%=OpenFile%:(mode%,KPath$+"tIOText2.txt")
	ReadTextBuffer:(handle%)
	CloseFile:
	CheckTextBuffers:
ENDP


PROC BinaryIO:
	EXTERNAL pWBuffer&,pRBuffer&
	EXTERNAL handle%
	LOCAL mode%
	LOCAL len&
	if handle% :CloseFile: :endif
	pWBuffer&=GetBuffer&:(KBinaryBufferSize&)
	pRBuffer&=GetBuffer&:(KBinaryBufferSize&)

	mode%=KIoOpenModeCreate% OR KIoOpenFormatBinary% OR KIoOpenAccessUpdate%
	handle%=OpenFile%:(mode%,KPath$+"tIObin1.bin")
	PopulateBuffer:(pWBuffer&,KTypeBinary%,KBinaryBufferSize&)
	WriteBuffer:(handle%,pWBuffer&,KTypeBinary%,KBinaryBufferSize&)
	CloseFile:

	REM Verify the file.
	mode%=KIoOpenModeOpen% OR KIoOpenFormatBinary% OR KIoOpenAccessUpdate%
	handle%=OpenFile%:(mode%,KPath$+"tIObin1.bin")
	len&=ReadBuffer&:(handle%,pRBuffer&,KTypeBinary%,KBinaryBufferSize&)
	CloseFile:
	CheckBuffer:(pWBuffer&,pRBuffer&,MIN(len&,KBinaryBufferSize&))
	FreeBuffers:
ENDP


PROC TextAsBinaryReadIO:
	EXTERNAL pWBuffer&,pRBuffer&
	EXTERNAL handle%
	GLOBAL readTextBuffer$(KMaxTextLine%,KMaxTextLen%)
	GLOBAL writeTextBuffer$(KMaxTextLine%,KMaxTextLen%)
	LOCAL mode%
	LOCAL len&,maxLen&

	if handle% :CloseFile: :endif
	maxLen&=(KMaxTextLine%+KNumTextEOLchars%)*KMaxTextLen%rem *KOplUnicodeFactor%
rem	pWBuffer&=GetBuffer&:(KBinaryBufferSize&)
	pRBuffer&=GetBuffer&:(maxLen&)

	REM Write a text file...
	mode%=KIoOpenModeCreate% OR KIoOpenFormatText% OR KIoOpenAccessUpdate%
	handle%=OpenFile%:(mode%,KPath$+"tIOText3.txt")
	PopulateWriteTextBuffer:
	WriteTextBuffer:(handle%)
	CloseFile:

	REM ... but read it as binary.
	mode%=KIoOpenModeOpen% OR KIoOpenFormatBinary%
	handle%=OpenFile%:(mode%,KPath$+"tIOText3.txt")
	len&=ReadBuffer&:(handle%,pRBuffer&,KTypeBinary%,maxLen&)
	CloseFile:
	rem ConvertBufferToReadTextBuffer:(pRBuffer&,len&)
	rem CheckTextBuffers:
	FreeBuffers:
ENDP


PROC OpenFile%:(aMode%,aName$)
	LOCAL err%
	LOCAL lhandle%
	err%=IOOPEN(lhandle%,aName$,aMode%)
	IF err%
		PRINT "Error ";err%;" opening file ";aName$
		RAISE err%
	ENDIF
	RETURN lhandle%
ENDP


PROC CloseFile:
	EXTERNAL handle%
	LOCAL err%
	err%=IOCLOSE(handle%)
	rem Whatever happens, clear the globlal
	handle%=0
	IF err% :PRINT "Error ";err%;" closing." :RAISE err% :ENDIF
ENDP


PROC GetBuffer&:(aSize&)
	LOCAL buffer&
	buffer&=ALLOC(aSize&)
	IF buffer&=0
		PRINT "Unable to alloc buffer of size",aSize&
		RAISE 100
	ENDIF
	RETURN buffer&
ENDP


PROC PopulateWriteTextBuffer:
	EXTERNAL writeTextBuffer$() rem (KMaxTextLine%,KMaxTextLen%)
	LOCAL buffer$(255)
	LOCAL line%
	line%=1
	DO
		buffer$=RIGHT$("0000"+GEN$(line%,5),5)
		buffer$=buffer$+REPT$("!",KMaxTextLen%-8)
		writeTextBuffer$(line%)=buffer$+"END"
		line%=line%+1
	UNTIL line%>KMaxTextLine%
ENDP


PROC PopulateBuffer:(apBuffer&,aMode%,aSize&)
	LOCAL ptr&,i&,chunk&
	LOCAL filler%
	LOCAL chunk$(KMaxTextLen%)
	ptr&=apBuffer&

	IF aMode%=KTypeText%
		DO
			IF aSize&-i&>KMaxTextLen%
				chunk&=KMaxTextLen%
			ELSE
				chunk&=(aSize&-i&)
			ENDIF

			rem build string to chunk size
			chunk$=RIGHT$("0000"+GEN$(i&,5),5)
			chunk$=chunk$+"%"+REPT$("!",chunk&-9) REM Here % is a mnemonic for binary,
																							REM for the binary text tests.
			chunk$=chunk$+"END"
			rem poke string into buffer
			MemCopy:(ADDR(chunk$)+1+KOplAlignment%, Ptr&+i&, chunk&)
			rem POKE$ ptr&+i&,chunk$

			i&=i&+chunk&
		UNTIL i&=aSize&
	ELSE
		REM Binary
		DO
			POKEB ptr&+i&,filler%
			filler%=filler%+1 :IF filler%=256 :filler%=0 :ENDIF
			i&=i&+1
		UNTIL i&=aSize&
	ENDIF
ENDP


PROC WriteBuffer:(aHand%,apBuffer&,aType%,aSize&)
	EXTERNAL pBuffer&
	LOCAL i&,chunk&
	LOCAL err%
	IF aType%=KTypeText%
		i&=0
		DO
			IF (aSize&-i&)>KMaxTextLen%
				chunk&=KMaxTextLen%
			ELSE
				chunk&=(aSize&-i&)
			ENDIF

			err%=IOWRITE(aHand%, apBuffer&+i&, chunk&)
			IF err%
				PRINT "Error ";err%;" writing data"
				RAISE err%
			ENDIF

			i&=i&+chunk&
		UNTIL i&=aSize&
	
	ELSE REM Binary
		i&=0
		DO
			IF (aSize&-i&)>kMaxBinaryChunk&
				chunk&=kMaxBinaryChunk&
			ELSE
				chunk&=(aSize&-i&)
			ENDIF

			err%=IOWRITE(aHand%, apBuffer&+i&, chunk&)
			IF err%
				PRINT "Error ";err%;" writing data"
				RAISE err%
			ENDIF

			i&=i&+chunk&
		UNTIL i&=aSize&
	ENDIF
ENDP


PROC ReadBuffer&:(aHandle%,apBuffer&,aType%,aMaxLen&)
	LOCAL len&,ret%
	LOCAL maxLen&,chunk&
	LOCAL textBuffer$(KMaxTextLen%)
	IF aType%=KTypeBinary%
		DO
			rem calc max chunk size.
			IF aMaxLen&-len&>KMaxBinaryChunk&
				chunk&=KMaxBinaryChunk&
			ELSE
				chunk&=(aMaxLen&-len&)
			ENDIF

			ret%=IOREAD(aHandle%, (apBuffer&+len&), chunk&)
			IF ret%<0
				IF ret%=KErrEof%
					RETURN len&
				ENDIF
				PRINT "Error ";ret%;" reading file "
				RAISE ret%
			ENDIF
			len&=len&+ret%
		UNTIL len&=aMaxLen&
	ELSE REM Must be text
		DO
			rem calc max chunk size.
			IF aMaxLen&-len&>KMaxTextLen%
				chunk&=KMaxTextLen%
			ELSE
				chunk&=(aMaxLen&-len&)
			ENDIF

			ret%=IOREAD(aHandle%, ADDR(textBuffer$), chunk&)
			IF ret%<0
				IF ret%=KErrEof%
					RETURN len&
				ENDIF
				PRINT "Error ";ret%;" reading file "
				RAISE ret%
			ENDIF
			POKEB ADDR(textBuffer$),ret%
			MemCopy:(ADDR(textBuffer$)+1+KOplAlignment%,apBuffer&+len&,ret%)
			len&=len&+ret%
		UNTIL len&=aMaxLen&
	ENDIF
	RETURN len&
ENDP


PROC CheckBuffer:(pBuf1&,pBuf2&,aMinimumLen&)
	LOCAL i&,a&,b&
	DO
		a&=PEEKB(pBuf1&+i&)
		b&=PEEKB(pBuf2&+i&)
		IF a&<>b&
			PRINT a&,b&,"@",i&
			RAISE 1000
		ENDIF
		i&=i&+1
	UNTIL i&=aMinimumLen&
ENDP


PROC MemCopy:(apSrc&,apDest&,aLen%)
	LOCAL i&
	WHILE i&<aLen%
		POKEB apDest&+i&,PEEKB(apSrc&)
		i&=i&+1
	ENDWH
ENDP


PROC WriteTextBuffer:(aHand%)
	EXTERNAL writeTextBuffer$() rem (KMaxTextLine%,KMaxTextLen%)
	LOCAL line%,lineLen%,err%
	LOCAL ptr&
	line%=1
	DO
		lineLen%=LEN(writeTextBuffer$(line%))
		ptr&=ADDR(writeTextBuffer$(line%))+1+KOplAlignment%
		IF ptr& AND 1
			hLog%:(KhLogAlways%,"ERROR: WriteTextBuffer: Bad alignment on $"+HEX$(ptr&)+" with line%="+GEN$(line%,5))
		ENDIF
		err%=IOWRITE(aHand%, ptr&, lineLen%)
		IF err%
			PRINT "Error ";err%;" writing text"
			RAISE err%
		ENDIF
		line%=line%+1
	UNTIL line%>KMaxTextLine%
ENDP


PROC ReadTextBuffer:(aHand%)
	EXTERNAL readTextBuffer$() rem (KMaxTextLine%,KMaxTextLen%)
	LOCAL line%,ret%
	LOCAL ptr&
	line%=1
	DO
		ptr&=ADDR(readTextBuffer$(line%))+1+KOplAlignment%
		ret%=IOREAD(aHand%, ptr&, 255)
		IF ret%<0
			PRINT "Error ";ret%;" reading text"
			RAISE ret%
		ENDIF
		POKEB ADDR(readTextBuffer$(line%)),ret%
		line%=line%+1
	UNTIL line%>KMaxTextLine%
ENDP


PROC CheckTextBuffers:
	EXTERNAL writeTextBuffer$() rem (KMaxTextLine%,KMaxTextLen%)
	EXTERNAL readTextBuffer$()
	
	LOCAL line&,i&,a%,b%
	line&=1
	DO
		rem print "Len=";LEN(writeTextBuffer$(line&))
		rem print "Len=";LEN(readTextBuffer$(line&))
		rem print "[";writeTextBuffer$(line&);"]"
		rem print "[";readTextBuffer$(line&);"]"
		i&=1
		DO
			a%=ASC(MID$(writeTextBuffer$(line&),i&,1))
			b%=ASC(MID$(readTextBuffer$(line&),i&,1))
			IF a%<>b%
				PRINT "Write=";a%,"Read=";b%,"@ location",i&
				RAISE 1000
			ENDIF
			i&=i&+1
		UNTIL i&=KMaxTextLen%
		line&=line&+1
	UNTIL line&=KMaxTextLine%
ENDP


rem OPEN 	open_existing		Create_new 	Create_replace	[Unique]
rem        			ab					cd					ef
rem FILE		   binary							text
rem 		      	1  2							3 4
rem ACCESS    Update			Random-IOSEEK    [Share,readonly]
rem C1 New binary update      C2 New binary random.         checks
rem D3 New text update        D4 New text random.            ch
rem E1 Replace binary update  E2 Replace binary random.     ch
rem F3 Replace text update    F4 Replace text random.        ch
rem A1 Open binary update     A2 Open binary random.                
rem B3 Open text update       B4 Open text random              
rem 13=Unique
rem 14=Share (two readers, one text, one binary)

REM End of tIO.tpl
