# DemoOPL.mk
# .SUFFIXES: .OXH .TXH

ROOT=..\..\DemoOPL
TARGETDIR=%EPOCROOT%epoc32\wins\c\System\Apps\DemoOPL
SRCDIR=$(ROOT)\src
BMPDIR=$(ROOT)\data
# Partial fix for 1079304 "DemoOPL.mk modifies src folder"
TMPBLDDIR=$(ROOT)\data
GROUPDIR=$(ROOT)\group
EPOCFORMATTARGETDIR=%EPOCROOT%epoc32\wins\c\OPL\DemoApp
# RSCFILE=$(TARGETDIR)\DemoOPL.r01
MBMFILES=\
	$(TARGETDIR)\DemoOPL.mbm \
	$(TMPBLDDIR)\DemoIcon.mbm
APPFILES=\
	$(TARGETDIR)\DemoOPL.app \
	$(TARGETDIR)\DemoOPL.aif

$(TARGETDIR)\DemoOPL.mbm : $(BMPFILES) $(TARGETDIR)
	@echo Building $@ 
	bmconv /q DemoOPL.mbm /c8$(BMPDIR)\SymbLogo.bmp
	@move DemoOPL.mbm $(TARGETDIR)\ >NUL

$(TMPBLDDIR)\DemoIcon.mbm : $(ICONBMPFILES) $(EPOCFORMATTARGETDIR)
	@echo Building $@
	cd $(TMPBLDDIR)
	bmconv /q DemoIcon.mbm /c8$(BMPDIR)\Icon_Normal.bmp /1$(BMPDIR)\Icon_Normal_Mask.bmp /c8$(BMPDIR)\Icon_List.bmp /1$(BMPDIR)\Icon_List_Mask.bmp
	@copy $(TMPBLDDIR)\DemoIcon.mbm $(EPOCFORMATTARGETDIR)\ >NUL

$(APPFILES) : $(SRCDIR)\DemoOPL.tpl $(TARGETDIR) $(TMPBLDDIR)\DemoIcon.mbm $(EPOCFORMATTARGETDIR)
	opltran $(SRCDIR)\DemoOPL.tpl -conv -o$(EPOCFORMATTARGETDIR) -q
	opltran $(SRCDIR)\DemoOPL.tph -conv -o$(EPOCFORMATTARGETDIR) -q
# Only last -i is used, so have to ensure .tpl and .mbm in same targetdir.
	opltran $(EPOCFORMATTARGETDIR)\DemoOPL. -i$(ROOT)\inc -e -q

TARGETFILES=\
	$(APPFILES)

BMPFILES=\
	$(ROOT)\data\SymbLogo.bmp

ICONBMPFILES=\
	$(ROOT)\data\Icon_List.bmp \
	$(ROOT)\data\Icon_List_Mask.bmp \
	$(ROOT)\data\Icon_Normal.bmp \
	$(ROOT)\data\Icon_Normal_Mask.bmp

$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)"

$(EPOCFORMATTARGETDIR) :
	@perl -S emkdir.pl "$(EPOCFORMATTARGETDIR)"

do_nothing :
	@rem do nothing

CLEAN :
	@erase $(TARGETFILES) >>NUL
	@erase $(MBMFILES) >>NUL
#	@erase $(RSCFILE) >>NUL

FINAL : do_nothing

FREEZE : do_nothing

LIB : do_nothing

MAKMAKE : do_nothing

RESOURCE : $(TARGETDIR) $(RSCFILE) $(MBMFILES)

BLD : $(TARGETFILES)

SAVESPACE : BLD

RELEASABLES :
	@echo $(TARGETFILES)
