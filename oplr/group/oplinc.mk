# OplInc.mk
# Create OPL source header files.
# Copyright (c) 1999-2002 Symbian Ltd. All Rights Reserved

.SUFFIXES: .OPH .TPH

TARGETDIR=%EPOCROOT%epoc32\wins\c\System\OPL

$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)"

SRCDIR=..\samplesu

$(TARGETDIR)\Const.oph : $(SRCDIR)\Const.tph $(TARGETDIR)
	OPLTRAN -conv $(SRCDIR)\Const.tph -e -o$(TARGETDIR) -q

OPHFILES=\
	$(TARGETDIR)\Const.oph

do_nothing :
	rem do nothing

#
# The targets invoked by bld...
#

MAKMAKE : do_nothing

RESOURCE : $(TARGETDIR) $(OPHFILES)

SAVESPACE : do_nothing

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing

CLEAN : 
	erase $(OPHFILES)

RELEASABLES : 
	@echo $(OPHFILES)
