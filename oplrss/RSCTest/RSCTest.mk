# RSCTest.mk
# Copyright (c) Symbian Ltd. 2000-2001. All Rights Reserved.
# Last updated 26 July 2001
#
# When customising this file for use in your own applications
# change only the file names and paths - everything else
# should be left untouched.

SOURCEROOT=\OPLRSS
SOURCEDIR=$(SOURCEROOT)\RSCTest
TARGETDIR=$(SOURCEDIR)\Output
RSGHEADERFILE=$(TARGETDIR)\RSCTest.rsg
OSGHEADERFILE=$(TARGETDIR)\RSCTest.osg
TSGHEADERFILE=$(TARGETDIR)\RSCTest.tsg
RSCFILES=\
	$(TARGETDIR)\RSCTest.r01 \
	$(TARGETDIR)\RSCTest.r10 \
	$(TARGETDIR)\RSCTest.r02 \
	$(TARGETDIR)\RSCTest.r03

$(TARGETDIR)\RSCTest.r01 : $(SOURCEDIR)\RSCTest.rss
	@echo Building $@
	@epocrc -u $? -I%EPOCROOT%epoc32\include -DLANGUAGE_01 -o$@ -h$(RSGHEADERFILE)

$(TARGETDIR)\RSCTest.r10 : $(SOURCEDIR)\RSCTest.rss
	@echo Building $@
	@epocrc -u $? -I%EPOCROOT%epoc32\include -DLANGUAGE_10 -o$@ -h$(RSGHEADERFILE)

$(TARGETDIR)\RSCTest.r02 : $(SOURCEDIR)\RSCTestFR.rss
	@echo Building $@
	@epocrc -u $? -I%EPOCROOT%epoc32\include -DLANGUAGE_02 -o$@ -h$(RSGHEADERFILE)

$(TARGETDIR)\RSCTest.r03 : $(SOURCEDIR)\RSCTestGE.rss
	@echo Building $@
	@epocrc -u $? -I%EPOCROOT%epoc32\include -DLANGUAGE_03 -o$@ -h$(RSGHEADERFILE)

$(TARGETDIR) :
	@if not exist "..\RSCTest"		md "..\RSCTest"
	@if not exist "..\RSCTest\Output"	md "..\RSCTest\Output"

do_nothing :
	@rem do nothing

CLEAN :
	@erase $(RSCFILES) >>NUL
	@erase $(RSGHEADERFILE) >>NUL
	@erase $(OSGHEADERFILE) >>NUL
	@erase $(TSGHEADERFILE) >>NUL

FINAL : do_nothing

FREEZE : do_nothing

LIB : do_nothing

MAKMAKE : do_nothing

RESOURCE : $(TARGETDIR) $(RSCFILES)
	@echo Converting resource header to OPL format
	@rsg2osg $(RSGHEADERFILE)

BLD : do_nothing

SAVESPACE : di_nothing

RELEASABLES :
	@echo $(RSCFILES)
	@echo $(RSGHEADERFILE)
	@echo $(OSGHEADERFILE)
	@echo $(TSGHEADERFILE)