INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, string$(255), length%, length2%, address&
	LOCAL index%
	address& = ALLOC(4)
	
	storeId% = SsOpenStore%:("c:\Welcome to ER1", KStoreDirectFile% OR KStoreUseDictionary%, KDirectFileStoreLayoutUid&, KUidAppDllDoc&, KUidWordApp&)

	streamId& = SsGetStreamId&:(storeId%, KUidEditableTextStream&)

rem Read in the length of the plain text
rem
	SsOpenStream%:(storeId%, streamId&, KStreamTypeRead%)
	SsReadData%:(storeId%, streamId&, address&, 1)
	
rem The length may be stored as one or more bytes
rem See TCardinality::InternalizeL() for the method
rem
	length% = PEEKB(address&)
	IF ((length% AND 1) = 0)
		length% = length% / 2
	ELSEIF ((length% AND 2) = 0)
		SsReadData%:(storeId%, streamId&, address& + 1, 1)
		length% = PEEKW(address&) / 4
	ELSEIF ((length% AND 4) = 0)
		SsReadData%:(storeId%, streamId&, address& + 1, 3)
		length% = PEEKL(address&) / 8
	ENDIF

rem Display the text in blocks of 255
rem taking paragraph separators (CHR$(6)) into account
rem
	WHILE (length% > 0)
		IF(length% > 255)
			string$ = SsReadString$:(storeId%, streamId&, 255)
		ELSE
			string$ = SsReadString$:(storeId%, streamId&, length%)
		ENDIF
		length% = length% - 255
		index% = 1
		WHILE(index% <= LEN(string$))
			IF(MID$(string$, index%, 1) = CHR$(6))
				PRINT
			ELSE
				PRINT MID$(string$, index%, 1);
			ENDIF
			index% = index% + 1
		ENDWH
		get
	ENDWH
ENDP