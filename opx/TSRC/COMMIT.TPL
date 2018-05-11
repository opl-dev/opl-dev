INCLUDE "sstore.oxh"

PROC main:
	LOCAL storeId%, streamId&, error%

	storeId% = SsCreateStore%:("mystore", KStorePermanentFile%, KPermanentFileStoreLayoutUid&, 0, 0, 0)
	print "SsCreateStore", storeId%

	error% = SsRevert%:(storeId%)
	print "SsRevert", error%

	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&

	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%

	error% = SsCommit%:(storeId%)
	print "SsCommit", error%

	streamId& = SsCreateStream&:(storeId%)
	print "SsCreateStream", streamId&

	error% = SsCloseStream%:(storeId%, streamId&)
	print "SsCloseStream", error%

	error% = SsRevert%:(storeId%)
	print "SsRevert", error%
	
rem This will fail as the revert will remove the stream
rem
	error% = SsOpenStream%:(storeId%, streamId&, KStreamTypeRead%)
	print "SsOpenStream", error%
	
	get
ENDP