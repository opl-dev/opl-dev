INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, error%, uId&, string$(255)
	string$ = "Test string"
	uId& = 1234
	
	storeId% = SsCreateStore%:("mystore", KStoreDirectFile% OR KStoreUseDictionary%, KDirectFileStoreLayoutUid&, 0, 0, 0)
	print "SsCreateStore", storeId%
	
	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&
	
	error% = SsAssocUId%:(storeId%, streamId&, uId&)
	print "SsAssocUId", error%
	
	error% = SsWriteString%:(storeId%, streamId&, string$, 11)
	print "SsWriteString", error%
	
	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%
	
	error% = SsCloseStore%:(storeId%)
	print "SsCloseStore", error%
	

	storeId% = SsOpenStore%:("mystore", KStoreDirectFile% OR KStoreUseDictionary%, KDirectFileStoreLayoutUid&, 0, 0)
	print "SsOpenStore", storeId%

	streamId& = SsGetStreamId&:(storeId%, uId&)
	print "SsGetStreamId", streamId&
	
	error% = SsOpenStream%:(storeId%, streamId&, KStreamTypeRead%)
	print "SsOpenStream", error%
	
	string$ = SsReadString$:(storeId%, streamId&, 11)
	print "SsReadString", string$

	rem	error% = SsCloseStream%:(storeId%, streamId&)
	rem	print "SsCloseStream", error%

	rem error% = SsCloseStore%:(storeId%)
	rem print "SsCloseStore", error%
	
	get
ENDP