#
#	Build the OPL help files
#	================================
#
#	Also, compare the (definitive) opl.hlp.hrh exported from the source tree
#	against the one generated from the RTF file, and throw an error if anything 
#	is different.
#
#	Assumptions:
#	The generated HRH file created by manually building help the first time is
#	kept as part of the source for the component and exported to 
#	epoc32\include\cshelp. This is so that this .mk file can pick up the file 
#	for verfication against the newly generated one.
#
#	NB2 There are items in here which are also defined in the project file -
#	attempts to modify this file to build for other components will require
#	matching modifications in the project file. These are marked with
#	"# Also defined in opl.xml"
#
#	NB3 Since I had to use the pseudotarget COMPARISON, the comparison of the 
#	generated and exported HRH files will happen EVERY TIME the final stage 
#	is run for this component.
#

#---------------------------------------#
#										#
#	Build tools							#
#										#
#---------------------------------------#


PREPROCESSORTOOL=\epoc32\gcc\bin\cpp.exe
HELPCOMPILERTOOL=\epoc32\tools\cshlpcmp

#---------------------------------------#
#										#
#	Help project file					#
#										#
#---------------------------------------#

HELPPROJECTFILE=.\group\opl.xml

#---------------------------------------#
#										#
#	Intermediate build directory		#
#										#
#---------------------------------------#

PROJECT=texted

INTERMEDIATEDIR=..\..\epoc32\build\$(PROJECT)\help	# Also defined in opl.xml
													# as the <output> and <working> fields.
													# (Has to be defined here as a relative
													# path because it is passed as a 
													# parameter to the GCC preprocessor)

$(INTERMEDIATEDIR) :
	if not exist "\epoc32" md "\epoc32"
	if not exist "\epoc32\build" md "\epoc32\build"
	if not exist "\epoc32\build\$(PROJECT)" md "\epoc32\build\$(PROJECT)"
	if not exist "\epoc32\build\$(PROJECT)\help" md "\epoc32\build\$(PROJECT)\help"

#---------------------------------------#
#										#
#	Help build tool output				#
#										#
#---------------------------------------#

HELPTOOLTARGETNAME=opl.hlp							#	Also defined in opl.xml


#	Define the anticipated output files we're interested in

HELPFILE=$(INTERMEDIATEDIR)\$(HELPTOOLTARGETNAME)
HELPFILEHRH=$(INTERMEDIATEDIR)\$(HELPTOOLTARGETNAME).hrh
HELPFILEOPH=$(INTERMEDIATEDIR)\$(HELPTOOLTARGETNAME).oph
HELPFILEOPHTXT=$(INTERMEDIATEDIR)\$(HELPTOOLTARGETNAME).oph.txt

HELPTOOLOUTPUT=$(HELPFILE) $(HELPFILEHRH) $(HELPFILEOPH) $(HELPFILEOPHTXT)

#	Build the help files

$(HELPTOOLOUTPUT) : $(INTERMEDIATEDIR) $(HELPPROJECTFILE)
	$(HELPCOMPILERTOOL) $(HELPPROJECTFILE)

#---------------------------------------#
#										#
#	Final destinations for output		#
#										#
#---------------------------------------#

!IF "$(PLATFORM)"=="WINS"
HELPTARGETDIR=\EPOC32\release\$(PLATFORM)\$(CFG)\z\system\help
OPHTARGETDIR=\EPOC32\release\$(PLATFORM)\$(CFG)\z\system\opl
!ELSE
HELPTARGETDIR=\EPOC32\release\$(PLATFORM)\$(CFG)
OPHTARGETDIR=\EPOC32\release\$(PLATFORM)\$(CFG)
!ENDIF


#	Create the destination directory for the helpfile

#	NB Unpleasant use of IF WINS block below avoids error U4004
#	in ARMI builds where the two paths are the same.

$(HELPTARGETDIR) :
	@if not exist "\EPOC32" md "\EPOC32"
	@if not exist "\EPOC32\release" md "\EPOC32\release"
	@if not exist "\EPOC32\release\$(PLATFORM)\" md "\EPOC32\release\$(PLATFORM)\"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)" md "\EPOC32\release\$(PLATFORM)\$(CFG)"
!IF "$(PLATFORM)"=="WINS"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)\z" md "\EPOC32\release\$(PLATFORM)\$(CFG)\z"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System" md "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System\help" md "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System\help"

#	Create the destination directory for the OPH file

$(OPHTARGETDIR) :
	@if not exist "\EPOC32" md "\EPOC32"
	@if not exist "\EPOC32\release" md "\EPOC32\release"
	@if not exist "\EPOC32\release\$(PLATFORM)\" md "\EPOC32\release\$(PLATFORM)\"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)" md "\EPOC32\release\$(PLATFORM)\$(CFG)"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)\z" md "\EPOC32\release\$(PLATFORM)\$(CFG)\z"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System" md "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System"
	@if not exist "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System\opl" md "\EPOC32\release\$(PLATFORM)\$(CFG)\z\System\opl"
!ENDIF


#	Copy the helpfile to its final destination

HELPFILEFINAL=$(HELPTARGETDIR)\$(HELPTOOLTARGETNAME)

$(HELPFILEFINAL) : $(HELPTARGETDIR) $(HELPFILE)
	copy $(HELPFILE) $@


#	Copy the helpfile OPH to its final destination

OPHFILEFINAL=$(OPHTARGETDIR)\$(HELPTOOLTARGETNAME).oph

$(OPHFILEFINAL) : $(OPHTARGETDIR) 
	copy $(HELPFILEOPH) $@

#---------------------------------------#
#										#
#	Preprocess and compare the two		#
#	HRH files (one exported from		#
#	source, one generated).				#
#										#
#---------------------------------------#


#	Preprocess the generated HRH file

PPGENERATEDHELPFILEHRH=$(HELPFILEHRH).gen.pp

$(PPGENERATEDHELPFILEHRH) : $(HELPTOOLOUTPUT)
	$(PREPROCESSORTOOL) -P $(HELPFILEHRH) $(PPGENERATEDHELPFILEHRH)


#	Preprocess the exported HRH file

EXPORTEDHELPFILEHRH=..\..\epoc32\include\cshelp\$(HELPTOOLTARGETNAME).hrh
# NB Has to be defined as a relative path because it is going to be used
# as a parameter for the GCC preprocessor.

PPEXPORTEDHELPFILEHRH=$(INTERMEDIATEDIR)\$(HELPTOOLTARGETNAME).hrh.exp.pp

$(PPEXPORTEDHELPFILEHRH) : $(INTERMEDIATEDIR) $(EXPORTEDHELPFILEHRH)
	@$(PREPROCESSORTOOL) -P $(EXPORTEDHELPFILEHRH) $(PPEXPORTEDHELPFILEHRH)


#	Do the comparison
#	NB. we have to use the -call so that we can use the errorlevel
#	returned by fc4bat without breaking NMAKE

COMPARISON : $(PPEXPORTEDHELPFILEHRH) $(PPGENERATEDHELPFILEHRH)
	@-call compare.cmd $(PPEXPORTEDHELPFILEHRH) $(PPGENERATEDHELPFILEHRH)

#---------------------------------------#
#										#
#	The targets invoked by bld...		#
#										#
#---------------------------------------#

do_nothing :

MAKMAKE : do_nothing

BLD : do_nothing

SAVESPACE : BLD

#CLEAN :
	#@if exist $(HELPFILE) erase $(HELPFILE)
	#@if exist $(HELPFILEHRH) erase $(HELPFILEHRH)
	#@if exist $(HELPFILEOPH) erase $(HELPFILEOPH)
	#@if exist $(HELPFILEOPHTXT) erase $(HELPFILEOPHTXT)
	#@if exist $(OPHFILEFINAL) erase $(OPHFILEFINAL)
	#@if exist $(HELPFILEFINAL) erase $(HELPFILEFINAL)
	#@if exist $(PPEXPORTEDHELPFILEHRH) erase $(PPEXPORTEDHELPFILEHRH)
	#@if exist $(PPGENERATEDHELPFILEHRH) erase $(PPGENERATEDHELPFILEHRH)

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

RELEASABLES :
	@echo $(OPHFILEFINAL)
	@echo $(HELPFILEFINAL)

FINAL : COMPARISON $(OPHFILEFINAL) $(HELPFILEFINAL) 
	