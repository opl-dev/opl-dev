INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, error%, storeId2%, streamId2&
	LOCAL string$(255), intvalue%, longvalue&, floatvalue
	intvalue% = 1
	longvalue& = 2
	floatvalue = 3
	string$ = "Test string"

	storeId% = SsCreateStore%:("mystore", KStoreDirectFile%, KDirectFileStoreLayoutUid&, 0, 0, 0)
	print "SsCreateStore", storeId%

	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&

	storeId2% = SsCreateEmbedStore%:(storeId%, streamId&, 0)
	print "SsCreateEmbedStore", storeId2%
	
	streamId2& = SsCreateStream&:(storeId2%)
	print "SsCreateStream", streamId2&
	
	error% = SsWriteInt%:(storeId2%, streamId2&, intvalue%)
	print "SsWriteInt", error%

	error% = SsWriteLong%:(storeId2%, streamId2&, longvalue&)
	print "SsWriteLong", error%

	error% = SsWriteFloat%:(storeId2%, streamId2&, floatvalue)
	print "SsWriteFloat", error%

	error% = SsWriteString%:(storeId2%, streamId2&, string$, LEN(string$))
	print "SsWriteString", error%

	error% = SsCloseStream%:(storeId2%, streamId2&)
	print "SsCloseStream", error%
	
	error% = SsCloseStore%:(storeId2%)
	print "SsCloseStore", error%
	
	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%

	error% = SsCloseStore%:(storeId%)
	print "SsCloseStore", error%
		
		
		
	storeId% = SsOpenStore%:("mystore", KStoreDirectFile%, KDirectFileStoreLayoutUid&, 0, 0)
	print "SsOpenStore", storeId%
	
	error% = SsOpenStream%:(storeId%, streamId&, KStreamTypeRead%)
	print "SsOpenStream", error%

	storeId2% = SsOpenEmbedStore%:(storeId%, streamId&, 0)
	print "SsOpenEmbedStore", storeId2%
	
	error% = SsOpenStream%:(storeId2%, streamId2&, KStreamTypeRead%)
	print "SsOpenStream", error%
	
	intvalue% = SsReadInt%:(storeId2%, streamId2&)
	print "SsReadInt", intvalue%

	longvalue& = SsReadLong&:(storeId2%, streamId2&)
	print "SsReadLong", longvalue&

	floatvalue = SsReadFloat:(storeId2%, streamId2&)
	print "SsReadFloat", floatvalue

	string$ = SsReadString$:(storeId2%, streamId2&, 11)
	print "SsReadString", string$

	rem error% = SsCloseStream%:(storeId2%, streamId2&)
	rem print "SsCloseStream", error%
	
	rem error% = SsCloseStore%:(storeId2%)
	rem print "SsCloseStore", error%
	
	rem error% = SsCloseStream%:(storeId%, streamId&)
	rem print "SsCloseStream", error%

	rem error% = SsCloseStore%:(storeId%)
	rem print "SsCloseStore", error%
	
	get
ENDP