# test.mk
# Constructs test files for OPX.

# Builds test *.TPLs and *.OPOs, but only for WINS.
# Populates the target directory with both the EPOC OPL source
# and the translated .OPO output, each constructed from the
# same TPL source file.

SRCDIR=..\tsrc
TARGETDIR=%EPOCROOT%epoc32\wins\C\Opl

$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)"

OPHFILES=\
#	$(TARGETDIR)\e32err.oph

TPHFILES=\
#	$(SRCDIR)\e32err.tph

OPOFILES=\
	$(TARGETDIR)\tsystem.opo \
	$(TARGETDIR)\tsystem2.opo \
	$(TARGETDIR)\tsystem3.opo \
	$(TARGETDIR)\tsystem4.opo \
	$(TARGETDIR)\tappframe.opo \
	$(TARGETDIR)\tdate.opo \
	$(TARGETDIR)\tlocale.opo \
	$(TARGETDIR)\tcontact.opo \
	$(TARGETDIR)\dcontact.opo \
	$(TARGETDIR)\tscomms.opo \
	$(TARGETDIR)\dsendas.opo \
	$(TARGETDIR)\tsendas.opo

OPLFILES=\
	$(TARGETDIR)\tsystem.opl \
	$(TARGETDIR)\tsystem2.opl \
	$(TARGETDIR)\tsystem3.opl \
	$(TARGETDIR)\tsystem4.opl \
	$(TARGETDIR)\tappframe.opl \
	$(TARGETDIR)\tdate.opl \
	$(TARGETDIR)\tlocale.opl \
	$(TARGETDIR)\tcontact.opl \
	$(TARGETDIR)\dcontact.opl \
	$(TARGETDIR)\tscomms.opl \
	$(TARGETDIR)\dsendas.opl \
	$(TARGETDIR)\tsendas.opl

.SUFFIXES: .opo .opl .tpl

{$(SRCDIR)}.TPH{$(TARGETDIR)}.oph:
	-opltran -conv $< -e -o$(TARGETDIR) -q
	if exist $@ del $@

{$(SRCDIR)}.TPL{$(TARGETDIR)}.opl:
	-opltran -conv $< -e -o$(TARGETDIR) -q
	if exist $@ del $@
	REN $* *.opl

{$(SRCDIR)}.TPL{$(TARGETDIR)}.opo:
	-opltran $< -i%EPOCROOT%epoc32\wins\c\system\opl -e -o$(TARGETDIR) -q
	-opltran $< -conv -e -o$(TARGETDIR) -q

do_nothing :
	rem do nothing

#
# The targets invoked by bld...
#

MAKMAKE : do_nothing

RESOURCE : $(TARGETDIR) $(OPHFILES) $(OPOFILES)

SAVESPACE : do_nothing

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing

CLEAN : 
	erase $(OPOFILES)

RELEASABLES : 
	@echo $(OPOFILES)
