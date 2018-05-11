REM TSCOMMS.OXH
REM
REM Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
REM

REM Tests and demonstrates the use of SCOMMS.OPX

INCLUDE "SComms.oxh"
INCLUDE "Const.oph"
INCLUDE "System.oxh"
INCLUDE "E32Err.oph"

DECLARE EXTERNAL

EXTERNAL Main:
EXTERNAL ModemTest:
EXTERNAL LongStringTest:
EXTERNAL CancelTests:
EXTERNAL ReadCancelTest:
EXTERNAL WriteCancelTest:
EXTERNAL ArrayReadTest:
EXTERNAL TranceiverModeDialog%:(title$)
EXTERNAL Connect:
EXTERNAL Disconnect:
EXTERNAL Write&:(text$)
EXTERNAL Read&:(stringAddr&,length&)
EXTERNAL ReadOneOrMore:(stringAddr&)
EXTERNAL QueryReceiveBuffer&:

REM The initial read or write after the irda port is opened will take around 3 secs to complete.
REM It's important not to timeout before a connection is made.
CONST KInitialiseIrdaTimeout& = 20000000 REM microseconds.

REM Subsequent read and writes will be much quicker.
CONST KTimeout& = 10000000 REM microseconds.
CONST KNoTimeout& = 0

REM Modem test
CONST KDefaultSend$="ATI" REM default string to send.
CONST KReceiver%=1
CONST KSender%=2
CONST KChoiceAddCR%=1 REM Yes, please add a CR.
CONST KChoiceTxThenRx%=1 REM Yes, switch to Rx mode after Tx

REM Long string test
CONST KLongString$="This is the start of a long string: The quick fox jumped over the Lazy dog. To be or not to be that is the question. A long time ago, in a galaxy far, far away... Wibble wibble wibble. Have I got to the end yet? This is the end of a long string --->"


PROC Main:
	GLOBAL gWriteStatus&
	GLOBAL gReadStatus&
	GLOBAL gHandle&
	LOCAL dial%

	WHILE 1
		dINIT "SComms Test App"
		dTEXT "","(Esc to cancel)",KDTextCenter%
		dBUTTONS "Modem Test",%M OR KDButtonPlainKey%, "Long String Test",%L OR KDButtonPlainKey%, "Cancel Tests",%C OR KDButtonPlainKey%, "Array Read Test",%A OR KDButtonPlainKey%
		dial%=DIALOG
		IF dial%=0
			GIPRINT "Cancelled"
			RETURN
		ELSEIF dial%=%m
			ModemTest:
		ELSEIF dial%=%l
			LongStringTest:
		ELSEIF dial%=%c
			CancelTests:
		ELSEIF dial%=%a
			ArrayReadTest:
		ENDIF
	ENDWH
ENDP

PROC ModemTest:
	LOCAL in$(KMaxStringLen%)
	LOCAL out$(KMaxStringLen%)
	LOCAL addCr%
	LOCAL swapMode%
	LOCAL mode%
	LOCAL ret&
	
	addCR%=kChoiceAddCR%
	swapMode%=kChoiceTxThenRx%
	Connect:
	DO
		mode% = TranceiverModeDialog%:("Modem Test")
		IF mode%=kSender%
			out$=kDefaultSend$
			dINIT "Text to send (Esc to cancel)"
			dEDIT out$, "Text", 10
			dCHOICE addCR%,"Add <CR> before sending", "Yes,No"
			dCHOICE swapMode%, "Switch to Read Mode after sending", "Yes,No"
			IF DIALOG=0
				BREAK
			ENDIF

			REM Add a newline char?
			IF addCR%=kChoiceAddCR%
				out$=out$+CHR$(13)
			ENDIF
			DO
			UNTIL Write&:(out$) <> KE32ErrTimedOut&
			IF swapMode%=kChoiceTxThenRx%
				mode%=kReceiver%
			ENDIF
		ENDIF
		IF mode%=kReceiver%
			PRINT 
			PRINT "Waiting to receive: (CR to end): "

			in$=""
			DO
				IF QueryReceiveBuffer&: > 0
					ReadOneOrMore:(ADDR(in$))
				ENDIF
			UNTIL LOC(in$,CHR$(13))

			PRINT
		ENDIF

	UNTIL mode%=0
	Disconnect:
ENDP

PROC LongStringTest:
	LOCAL reply$(KMaxStringLen%)
	LOCAL ret&
	LOCAL length%

	Connect:
	DO
		Write&:(KLongString$+chr$(13))
		PRINT "### Send CR to End."
		length% = QueryReceiveBuffer&:
		IF length% > 0
			ret& = Read&:(ADDR(reply$),length%)
			REM SCReadA: will not read past the end of reply$, so this is safe for length%>255
		ENDIF
	UNTIL LOC(reply$,CHR$(13))	
	Disconnect:
ENDP

PROC CancelTests:
	PAUSE 100
	WriteCancelTest:
	PAUSE 100
	ReadCancelTest:
	PAUSE 100
ENDP

PROC WriteCancelTest:
	EXTERNAL gWriteStatus&
	EXTERNAL gHandle&
	LOCAL ret&

	PRINT
	PRINT "* WRITE CANCEL TEST"
	PRINT "*** About to send a test string then cancel it:"
	gHandle& = SCConnect&:(KSCIrCommSvrName$)
	SCWriteA:(gHandle&,KLongString$,KNoTimeout&,gWriteStatus&)
	SCWriteCancel:(gHandle&)

	REM Even though a cancel's been requested, still need to wait for completion 
	IOWAITSTAT32 gWriteStatus&
	PRINT "*** Cancelled. Returning E32 error: "+GEN$(gWriteStatus&,10)
	Disconnect:
ENDP

PROC ReadCancelTest:
	EXTERNAL gReadStatus&
	EXTERNAL gHandle&
	LOCAL ret&
	LOCAL buffer$(KMaxStringLen%)

	PRINT
	PRINT "* READ CANCEL TEST"
	Connect:
 	PRINT "*** About to request a read then cancel it:"
	SCReadA:(gHandle&,ADDR(buffer$),KMaxStringLen%,KNoTimeout&,gReadStatus&)
	SCReadCancel:(gHandle&)
	
	REM Even though a cancel's been requested, still need to wait for completion 
	IOWAITSTAT32 gReadStatus&
	PRINT "*** Cancelled. Returning E32 error: "+GEN$(gReadStatus&,10)
	Disconnect:
ENDP

PROC ArrayReadTest:
	rem 5 strings of length 10
	LOCAL stringArray$(5,10)
	LOCAL line& 
	EXTERNAL gHandle&
	EXTERNAL gReadStatus&

	Connect:
	Print "Waiting to recieve 5 lines of text..."
	line&=1
	WHILE line&<=5
		SCReadA:(gHandle&, ADDR(stringArray$(line&)), 10, KTimeOut&, gReadStatus&)
		IOWAITSTAT32 gReadStatus&
		line&=line&+1
	ENDWH
	Disconnect:
	line&=1
	WHILE line&<=5
		PRINT stringArray$(line&)
		line&=line&+1
	ENDWH
ENDP

PROC TranceiverModeDialog%:(aTitle$)
	LOCAL dial%
	
	dINIT aTitle$
	dTEXT "","(Esc to cancel)",KDTextCenter%
	dBUTTONS "Sender",%S OR KDButtonPlainKey%, "Receiver",%R OR KDButtonPlainKey%
	dial%=DIALOG
	IF dial%=0
		GIPRINT "Cancelled"
		RETURN
	ELSEIF dial%=%s
		RETURN kSender%
	ELSEIF dial%=%r
		RETURN kReceiver%
	ENDIF
ENDP

PROC Connect:
	EXTERNAL gWriteStatus&
	EXTERNAL gHandle&
	LOCAL dummy$(1)
	
	DO
		PRINT "*** Trying to connect... ";
		gHandle& = SCConnect&:(KSCIrCommSvrName$)

		REM Irda takes up to 3 seconds to initialise, so write an empty string 
		REM to wait till the port is open.
		PRINT "Trying a dummy write... ";
		DO
			SCWriteA:(gHandle&,dummy$,KInitialiseIrdaTimeout&,gWriteStatus&)
			IOWAITSTAT32 gWriteStatus&
			IF gWriteStatus& = KE32ErrNone&
				PRINT "connected."
			ELSEIF gWriteStatus& = KE32ErrTimedOut&
				PRINT "timed out."
			ELSEIF gWriteStatus& = KE32ErrInUse&
				PRINT "Port in use. Try switching off the remote link."
				PRINT "Press any key to retry..."
				SCDisconnect:(gHandle&)
				GET
			ELSE
				PRINT
				PRINT "*** Unknown return value from SCConnect:"
			ENDIF
			PRINT
		UNTIL gWriteStatus&=KE32ErrNone& OR gWriteSTatus&=KE32ErrInUse& 
	UNTIL gWriteStatus&=KE32ErrNone&
ENDP

PROC Disconnect:
	EXTERNAL gHandle&
	
	PRINT "*** Trying to disconnect... ";
	SCDisconnect:(gHandle&)
	PRINT "disconnected."
ENDP

PROC Write&:(text$)
	EXTERNAL gHandle&
	EXTERNAL gWriteStatus&
	
	PRINT "*** Trying to send:" 
	PRINT text$
	SCWriteA:(gHandle&,text$,KTimeout&,gWriteStatus&)
	IOWAITSTAT32 gWriteStatus&
	IF gWriteStatus&=KE32ErrTimedOut&
		PRINT "*** Timed out."
	ELSE	
		PRINT "*** Sent."
	ENDIF
	RETURN gWriteStatus&
ENDP

PROC Read&:(stringAddr&, length&)
	EXTERNAL gHandle&
	EXTERNAL gReadStatus&
	
	PRINT "*** Trying to read", length&, "bytes... ";
	SCReadA:(gHandle&,stringAddr&,length&,KTimeOut&,gReadStatus&)
	IOWAITSTAT32 gReadStatus&
	IF gReadStatus&=KE32ErrTimedOut&
		PRINT "*** Timed out."
	ELSE	
		PRINT PEEK$(stringAddr&)
		PRINT "*** read."
	ENDIF	
	RETURN gReadStatus&
ENDP

PROC ReadOneOrMore:(stringAddr&)
	EXTERNAL gHandle&
	EXTERNAL gReadStatus&
	
	PRINT "*** Trying to read one or more bytes... [";
	SCReadOneOrMoreA:(gHandle&,stringAddr&,gReadStatus&)
	IOWAITSTAT32 gReadStatus&
	IF gReadStatus&=KE32ErrTimedOut&
		PRINT
		PRINT "*** Timed out."
	ELSE	
		PRINT PEEK$(stringAddr&)
		PRINT "] read."
	ENDIF
ENDP

PROC QueryReceiveBuffer&:
	EXTERNAL gHandle&
	LOCAL remainingChars&
	
	remainingChars& = SCQueryReceiveBuffer&:(gHandle&)
	IF remainingChars& > 0
		PRINT "*** Found",remainingChars&,"bytes in read buffer."
	ENDIF
	RETURN remainingChars&
ENDP