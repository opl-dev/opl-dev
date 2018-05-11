INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, error%
	LOCAL intvalue%, longvalue&, floatvalue
	intvalue% = 100
	longvalue& = 200
	floatvalue = 300

	storeId% = SsCreateStore%:("mystore", KStorePermanentFile%, KPermanentFileStoreLayoutUid&, 0, 0, 0)
	print "SsCreateStore", storeId%

	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&

	error% = SsWriteInt%:(storeId%, streamId&, intvalue%)
	print "SsWriteInt", error%

	error% = SsWriteLong%:(storeId%, streamId&, longvalue&)
	print "SsWriteLong", error%

	error% = SsWriteFloat%:(storeId%, streamId&, floatvalue)
	print "SsWriteFloat", error%

	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%
			
	error% = SsCloseStore%:(storeId%)
	print "SsCloseStore", error%


	storeId% = SsOpenStore%:("mystore", KStorePermanentFile%, KPermanentFileStoreLayoutUid&, 0, 0)
	print "SsOpenStore", storeId%

	error% = SsOpenStream%:(storeId%, streamId&, KStreamTypeRead%)
	print "SsOpenStream", error%

	intvalue% = SsReadInt%:(storeId%, streamId&)
	print "SsReadInt", intvalue%
	
	longvalue& = SsReadLong&:(storeId%, streamId&)
	print "SsReadLong", longvalue&

	floatvalue = SsReadFloat:(storeId%, streamId&)
	print "SsReadFloat", floatvalue

	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%
	
	error% = SsCloseStore%:(storeId%)
	print "SsCloseStore", error%
	
	get
ENDP