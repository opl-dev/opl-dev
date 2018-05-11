# OPX.mk
# Make OPL OPX header files
# Copyright (c) 1999-2002 Symbian Ltd. All Rights Reserved

TARGETDIR=%EPOCROOT%epoc32\wins\c\System\OPL

$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)"

# Convert each OXH header from its ASCII text source (TXH) using OPLTRAN,
# with the conversion output directed to the target directory. OPLTRAN
# automatically converts .TXH files to Symbian OS Unicode OXH files.

OXHFILES=\
	$(TARGETDIR)\Agenda.oxh \
	$(TARGETDIR)\Alarm.oxh \
	$(TARGETDIR)\AppFrame.oxh \
	$(TARGETDIR)\Array.oxh \
	$(TARGETDIR)\Bmp.oxh \
	$(TARGETDIR)\Buffer.oxh \
	$(TARGETDIR)\Celltrack.oxh \
	$(TARGETDIR)\Contact.oxh \
	$(TARGETDIR)\Convert.oxh \
	$(TARGETDIR)\Data.oxh \
	$(TARGETDIR)\Date.oxh \
	$(TARGETDIR)\DBase.oxh \
	$(TARGETDIR)\Locale.oxh \
	$(TARGETDIR)\MediaServerOPX.oxh \
	$(TARGETDIR)\Printer.oxh \
	$(TARGETDIR)\Prntst.oxh \
	$(TARGETDIR)\SComms.oxh \
	$(TARGETDIR)\E32Err.oph \
	$(TARGETDIR)\SendAs.oxh \
	$(TARGETDIR)\Spell.oxh \
	$(TARGETDIR)\System.oxh \

$(TARGETDIR)\AppFrame.oxh : ..\AppFrame\AppFrame.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Array.oxh : ..\Array\Array.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\SendAs.oxh : ..\SendAs\SendAs.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\System.oxh : ..\System\System.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Agenda.oxh : ..\Agenda\Agenda.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Alarm.oxh : ..\Alarm\Alarm.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Bmp.oxh : ..\Bmp\Bmp.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Buffer.oxh : ..\Buffer\Buffer.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Celltrack.oxh : ..\Celltrack\Celltrack.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Contact.oxh : ..\Contact\Contact.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Convert.oxh : ..\Convert\Convert.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Data.oxh : ..\Data\Data.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Date.oxh : ..\Date\Date.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\DBase.oxh : ..\DBase\DBase.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Locale.oxh : ..\Locale\Locale.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\MediaServerOPX.oxh : ..\MediaServerOPX\MediaServerOPX.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Printer.oxh : ..\Printer\Printer.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Prntst.oxh : ..\TOPXPRN\Prntst.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\SComms.oxh : ..\SComms\SComms.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\E32Err.oph : ..\SComms\E32Err.tph
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\Spell.oxh : ..\Spell\Spell.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

do_nothing :
	rem do_nothing

#
# The targets invoked by bld...
#

MAKMAKE : do_nothing

RESOURCE : $(TARGETDIR) $(OXHFILES)

SAVESPACE : do_nothing

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing

CLEAN : 
	erase $(OXHFILES)
	erase %EPOCROOT%epoc32\wins\c\System\OPL\E32ERR.oph

RELEASABLES : 
	@echo $(OXHFILES)