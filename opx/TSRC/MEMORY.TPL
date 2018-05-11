INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, error%
	LOCAL string$(255), length%
	string$ = "String in memory store"

	storeId% = SsCreateStore%:("djstore", KStoreMemory%, 0, 0, 0, 10)
	print "SsCreateStore", storeId%

	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&

	error% = SsWriteInt%:(storeId%, streamId&, LEN(string$))
	print "SsWriteInt", error%

	error% = SsWriteString%:(storeId%, streamId&, string$, LEN(string$))
	print "SsWriteString", error%

	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%
	
	error% = SsOpenStream%:(storeId%, streamId&, KStreamTypeRead%)
	print "SsOpenStream", error%
		
	length% = SsReadInt%:(storeId%, streamId&)
	print "SsReadInt", length%
	
	string$ = SsReadString$:(storeId%, streamid&, length%)
	print "SsReadString", string$

	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%

	error% = SsCloseStore%:(storeId%)
	print "SsCloseStore", error%

	get
ENDP