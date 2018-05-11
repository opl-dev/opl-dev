REM testoplt.tpl
REM A test file for OPLTRAN
REM Copyright (c) 1998-2000 Symbian Ltd. All rights reserved.

REM Just for easy identification for this test program, the include
REM files are marked with an oph/oxh extension for native EPOC
REM file formats, and tph/txh extensions for text files. Note that
REM the OPLTRAN tool does not use the extension to identify the
REM file type.

REM Include a native file from the same folder as this OPL source file.
INCLUDE "tlocal1.oph"
INCLUDE "tlocal2.tph"

REM Include from folder -I on command line.
INCLUDE "tinclude1.oph"
INCLUDE "tinclude2.tph"

REM Include from \epoc32\winc\opl folder
INCLUDE "twinc1.oph"
INCLUDE "twinc2.tph"

REM Include from \epoc32\release\wins\udeb\z\system\opl folder
INCLUDE "tsystem1.oph"
INCLUDE "tsystem2.tph"

APP AnApp, 12345
	ICON "skelopl.mbm"
ENDA

PROC main:

REM xxxx an error here please!
	print "Testing OPLTRAN"

	PRINT "tlocal1.oph has", Ktlocal1%
	PRINT "tlocal2.txh has", Ktlocal2%

	PRINT "tinclude1.oxh has", Ktinclude1%
	PRINT "tinclude2.tph has", Ktinclude2%

	PRINT "twinc1.tph has", Ktwinc1%
	PRINT "twinc2.oxh has", Ktwinc2%

	PRINT "tsystem1.txh has", Ktsystem1%
	PRINT "tsystem2.oph has", Ktsystem2%

REM    xxxx an error here please!
	GET

REM Comment out the ENDP for the EOF test.
ENDP