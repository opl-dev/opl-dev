INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, error%, address&, address2&

	address& = ALLOC(16)
	POKEL address&, 1000
	POKEL address& + 4, 2000
	POKEL address& + 8, 3000
	POKEL address& + 12, 4000
		
	storeId% = SsCreateStore%:("mystore", KStoreDirectFile% OR KStoreUseDictionary%, KDirectFileStoreLayoutUid&, 0, 0, 0)
	streamId& = SsCreateStream&:(storeId%)
	SsWriteData%:(storeId%, streamId&, address&, 16)
	SsCloseStream%:(storeId%, streamId&)
	SsCloseStore%:(storeId%)


	address2& = ALLOC(16)
	storeId% = SsOpenStore%:("mystore", KStoreDirectFile% OR KStoreUseDictionary%, KDirectFileStoreLayoutUid&, 0, 0)
	SsOpenStream%:(storeId%, streamId&, KStreamTypeRead%)
	SsReadData%:(storeId%, streamId&, address2&, 16)
	SsCloseStream%:(storeId%, streamId&)
	SsCloseStore%:(storeId%)

	print PEEKL(address2&)
	print PEEKL(address2& + 4)
	print PEEKL(address2& + 8)
	print PEEKL(address2& + 12)
	get
		
ENDP