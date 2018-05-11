rem TRADOPX.OPL
rem
rem Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
rem

INCLUDE "const.oph"
INCLUDE "radopx.oxh"

CONST KPass&=-1
CONST KFail&=0

PROC Main:
	UIStart:("Initialise")
ENDP

PROC Initialise:
	print "Hello world!"
	UIEnd:
ENDP