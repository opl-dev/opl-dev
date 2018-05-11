INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, string1$(255), string2$(255)
	LOCAL address&, length%
	address& = ALLOC(5)
	string1$ = "Word document created from within OPL" + CHR$(6)
	string2$ = "using the stream store OPX (Courier 12)"	
	storeId% = SsCreateStore%:("myword", KStoreDirectFile% OR KStoreUseDictionary%, KDirectFileStoreLayoutUid&, KUidAppDllDoc&, KUidWordApp&, 0)
	streamId& = SsCreateStream&:(storeId%)
	SsAssocUId%:(storeId%, streamId&, KUidParagraphStyleStream&)

rem CParagraphFormatLayer
	SsWriteLong%:(storeId%, streamId&, 0)

rem CCharFormatLayer
	rem total length
		SsWriteLong%:(storeId%, streamId&, 14)
	rem font height attribute
		POKEB address&, KAttFontHeight%
		rem font height
			POKEL address& + 1, 12 * 20
			SsWriteData%:(storeId%, streamId&, address&, 5)
	rem font name attribute
		POKEB address&, KAttFontTypeFace%
		rem font name length
			POKEB address& + 1, 8
		rem font name
		SsWriteData%:(storeId%, streamId&, address&, 2)
		SsWriteString%:(storeId%, streamId&, "Courier", 7)

rem iNormalStyleShortCut
	SsWriteLong%:(storeId%, streamId&, 0)

rem shorcut count
	POKEB address&, 0
	SsWriteData%:(storeId%, streamId&, address&, 1)
	
rem CStyleList
rem count
	POKEB address&, 0
	SsWriteData%:(storeId%, streamId&, address&, 1)
	SsCloseStream%:(storeId%, streamId&)

rem May need to increase this as necessary to avoid
rem Word corrupt message
rem
	streamId& = SsCreateStream&:(storeId%)
	SsAssocUId%:(storeId%, streamId&, KUidPrintSetupStream&)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsWriteLong%:(storeId%, streamId&, 0)
	SsCloseStream%:(storeId%, streamId&)

rem See TCardinality::ExternalizeL()
rem
	streamId& = SsCreateStream&:(storeId%)
	SsAssocUId%:(storeId%, streamId&, KUidEditableTextStream&)

	length% = LEN(string1$) + LEN(string2$) + 1
	IF(length% <= 127)
		length% = length% * 2
		POKEB address&, length%
		SsWriteData%:(storeId%, streamId&, address&, 1)
	ELSEIF(length% <= 16383)
		length% = length% * 4 + 1
		POKEW address&, length%
		SsWriteData%:(storeId%, streamId&, address&, 2)
	ELSE
		length% = length% * 8 + 3
		POKEL address&, length%
		SsWriteData%:(storeId%, streamId&, address&, 4)
	ENDIF

	SsWriteString%:(storeId%, streamId&, string1$, LEN(string1$))
	SsWriteString%:(storeId%, streamId&, string2$, LEN(string2$))
	SsCloseStream%:(storeId%, streamId&)
	SsCloseStore%:(storeId%)

ENDP