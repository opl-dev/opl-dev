INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, error%, uId&, storeId2%, streamId2&
	LOCAL string$(255), intvalue%, longvalue&, floatvalue
	intvalue% = 1
	longvalue& = 2
	floatvalue = 3
	string$ = "Test string"
	uId& = 268435718

	storeId% = SsCreateStore%:("mystore", KStoreDirectFile%, KDirectFileStoreLayoutUid&, 0, 0, 0)
	print "SsCreateStore", storeId%

	storeId2% = SsCreateStore%:("mystore2", KStoreDirectFile%, KDirectFileStoreLayoutUid&, 0, 0, 0)
	print "SsCreateStore", storeId2%

	storeId% = SsCreateStore%:("mystore3", KStoreDirectFile%, KDirectFileStoreLayoutUid&, 0, 0, 0)
	print "SsCreateStore", storeId%

	error% = SsCloseStore%:(storeId2%)
	print "SsCloseStore", error%

	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&

	storeId2% = SsCreateEmbedStore%:(storeId%, streamId&, 0)
	print "SsCreateEmbedStore", storeId2%

	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&

	storeId2% = SsCreateEmbedStore%:(storeId%, streamId&, 0)
	print "SsCreateEmbedStore", storeId2%

	get
ENDP