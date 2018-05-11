# test.mk
# OPLR custom makefile, DOES NOT generates opltest test suite.
# WINS only.

OPOFILES=\
#	$(TARGETDIR)\tsystem.OPO

OPLFILES=\
#	$(TARGETDIR)\tsystem.OPL 

.SUFFIXES: .OPO .OPL .TPL

SOURCEDIR=\opx\tsrc
TARGETDIR=%EPOCROOT%epoc32\wins\c\opl

{$(SOURCEDIR)}.TPL{$(TARGETDIR)}.OPL:
	OPLTRAN -conv $< -e -o$(TARGETDIR)
	if exist $@ del $@
	REN $* *.opl

{$(SOURCEDIR)}.TPL{$(TARGETDIR)}.OPO:
	OPLTRAN $< -i%EPOCROOT%epoc32\release\wins\$(CFG)\z\system\opl -e -o$(TARGETDIR)

$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)"

do_nothing :
	rem do nothing
	
CLEAN :
	-erase $(OPOFILES)
	
FINAL : do_nothing
	
FREEZE : do_nothing
	
LIB : do_nothing
	
MAKMAKE : do_nothing
	
RESOURCE : do_nothing
	
BLD : $(TARGETDIR)
	
SAVESPACE : BLD

RELEASABLES :
	echo $(OPOFILES)
