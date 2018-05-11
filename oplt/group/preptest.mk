# preptest.mk
# Prepares OPLT test suite

TARGETDIR=%EPOCROOT%epoc32\wins\c\toplt\

$(TARGETDIR) :
	@perl -S emkdir.pl "$(TARGETDIR)"

# Convert each ASCII text source file (both .txh and .tpl) using OPLTRAN.

TARGETFILES=\
	$(TARGETDIR)\alice.oph \
	$(TARGETDIR)\bob.oph \
	$(TARGETDIR)\bugs.opl \
	$(TARGETDIR)\casting.opl \
	$(TARGETDIR)\const.opl \
	$(TARGETDIR)\decl.opl \
	$(TARGETDIR)\decl.oxh \
	$(TARGETDIR)\empty.opl \
	$(TARGETDIR)\extends.opl \
	$(TARGETDIR)\func.opl \
	$(TARGETDIR)\idents.opl \
	$(TARGETDIR)\incha27.oph \
	$(TARGETDIR)\include.opl \
	$(TARGETDIR)\keys.opl \
	$(TARGETDIR)\opler1.opl \
	$(TARGETDIR)\runloc.opl \
	$(TARGETDIR)\struct.opl \
	$(TARGETDIR)\ted.oph \

$(TARGETDIR)\alice.oph : ..\ttran\alice.tph
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\bob.oph : ..\ttran\bob.tph
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\bugs.opl : ..\ttran\bugs.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\const.opl : ..\ttran\const.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\decl.opl : ..\ttran\decl.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\decl.oxh : ..\ttran\decl.txh
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\empty.opl : ..\ttran\empty.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\extends.opl : ..\ttran\extends.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\func.opl : ..\ttran\func.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\idents.opl : ..\ttran\idents.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\incha27.oph : ..\ttran\incha27.tph
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\include.opl : ..\ttran\include.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\keys.opl : ..\ttran\keys.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\opler1.opl : ..\ttran\opler1.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\runloc.opl : ..\ttran\runloc.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\struct.opl : ..\ttran\struct.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\ted.oph : ..\ttran\ted.tph
	@opltran -conv $? -e -o$(TARGETDIR) -q

$(TARGETDIR)\casting.opl : ..\ttran\casting.tpl
	@opltran -conv $? -e -o$(TARGETDIR) -q

do_nothing :
	rem do_nothing

#
# The targets invoked by bld...
#

MAKMAKE : do_nothing

RESOURCE : $(TARGETDIR) $(TARGETFILES)

SAVESPACE : do_nothing

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing

CLEAN : 
	erase $(TARGETDIR)\alice.oph 
	erase $(TARGETDIR)\bob.oph 
	erase $(TARGETDIR)\bugs.
	erase $(TARGETDIR)\casting.
	erase $(TARGETDIR)\const. 
	erase $(TARGETDIR)\decl.
	erase $(TARGETDIR)\decl.oxh 
	erase $(TARGETDIR)\empty.
	erase $(TARGETDIR)\extends.
	erase $(TARGETDIR)\func. 
	erase $(TARGETDIR)\idents.
	erase $(TARGETDIR)\incha27.oph 
	erase $(TARGETDIR)\include.
	erase $(TARGETDIR)\keys. 
	erase $(TARGETDIR)\opler1.
	erase $(TARGETDIR)\runloc.
	erase $(TARGETDIR)\struct.
	erase $(TARGETDIR)\ted.oph 

RELEASABLES : 
	@echo $(TARGETFILES)
