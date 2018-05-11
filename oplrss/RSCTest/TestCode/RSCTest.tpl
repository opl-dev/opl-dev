rem RSCTest.tpl
rem
rem Last updated 26 July 2001.
rem Copyright (c) 2000-2001 Symbian Ltd. All rights reserved.
rem
rem Test code for RSCTest resource file example on OPL SDK

DECLARE EXTERNAL
INCLUDE "SYSTEM.OXH"
INCLUDE "RSCTEST.OSG"

EXTERNAL DoResourceStuff:(aName$)

PROC Main:
	PRINT "Loading UK English:"
	DoResourceStuff:("C:\RSCTest.r01")
	PRINT "Loading US English:"
	DoResourceStuff:("C:\RSCTest.r10")
	PRINT "Loading French:"
	DoResourceStuff:("C:\RSCTest.r02")
	PRINT "Loading German:"
	DoResourceStuff:("C:\RSCTest.r03")
ENDP

PROC DoResourceStuff:(aName$)
LOCAL Id&
	Id&=SyLoadRsc&:(aName$)
	PRINT SyReadRSC$:(LANGUAGE_NAME&)
	PRINT SyReadRSC$:(RESOURCE_LAST_UPDATED_ON&)
	PRINT SyReadRSC$:(TEST_RESOURCE_1&)
	PRINT SyReadRSC$:(TEST_RESOURCE_2&)
	PRINT SyReadRSC$:(TEST_RESOURCE_3&)
	PRINT SyReadRSC$:(TEST_RESOURCE_4&)
	PRINT SyReadRSC$:(TEST_RESOURCE_5&)
	GET
	CLS
	SyUnLoadRsc:(Id&)
ENDP