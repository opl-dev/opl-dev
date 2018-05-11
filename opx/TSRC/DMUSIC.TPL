REM DMUSIC.OPL
REM
REM Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
REM
REM Demo code for Music OPX

INCLUDE "MUSIC.OXH"

PROC Main:
	MusicSetVolume:(3)
	MusicDTMF:("0123456")
	Scale:
ENDP

PROC Scale:
	local freq&, n%, amp%
	amp% = 100
	n%= 3
	while n% < 16
		freq& = 512000/(440*2**(n%/12.0))-1
		MusicNote:(freq&, amp%)
		amp% = 150 - amp%
		print "Press a key"
		get
		n% = n% + 1
	endwh
	MusicStop:
ENDP
