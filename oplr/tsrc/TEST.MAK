# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug" && "$(CFG)" !=\
 "Win32 Uni Debug" && "$(CFG)" != "Win32 Uni Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "TEST.MAK" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Uni Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Uni Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\work\hcil\winsd\rel"
# PROP Intermediate_Dir "\work\hcil\winsd\rel"
OUTDIR=\work\hcil\winsd\rel
INTDIR=\work\hcil\winsd\rel

ALL : \e32sys\DATA.DLL $(OUTDIR)/TEST.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MD /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /I "\oplr\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /FR /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /I\
 "\oplr\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D\
 "__WINS__" /FR$(INTDIR)/ /Fp$(OUTDIR)/"TEST.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=\work\hcil\winsd\rel/
# ADD BASE RSC /l 0x1 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o$(OUTDIR)/"TEST.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TESTS.SBR \
	$(INTDIR)/TESTMAIN.SBR \
	$(INTDIR)/NOT_YET.SBR \
	$(INTDIR)/STACK.SBR

$(OUTDIR)/TEST.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386
# ADD LINK32 \e32sys\efsrv.lib \e32sys\eexe.lib \e32sys\euser.lib \e32sys\bafl.lib \e32sys\hcil.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/DATA.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/DATA.DLL" /IMPLIB:"\e32sys/HCIL.LIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrv.lib \e32sys\eexe.lib \e32sys\euser.lib\
 \e32sys\bafl.lib \e32sys\hcil.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib\
 /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /INCREMENTAL:no\
 /PDB:"\e32sys/DATA.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE\
 /OUT:"\e32sys/DATA.DLL" /IMPLIB:"\e32sys/HCIL.LIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TESTS.OBJ \
	$(INTDIR)/TESTMAIN.OBJ \
	$(INTDIR)/NOT_YET.OBJ \
	$(INTDIR)/STACK.OBJ

\e32sys\DATA.DLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\work\oplr\wins\deb"
# PROP Intermediate_Dir "\work\oplr\wins\deb"
OUTDIR=\work\oplr\wins\deb
INTDIR=\work\oplr\wins\deb

ALL : \e32sys\TESTD.APP $(OUTDIR)/TESTD.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /Zp4 /MD /W4 /Zi /YX /Od /I "..\inc" /I "\e32inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /FR /Fd"\e32sys/TEST.pdb" /c
CPP_PROJ=/nologo /Zp4 /MD /W4 /Zi /YX /Od /I "..\inc" /I "\e32inc" /D "_DEBUG"\
 /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /FR$(INTDIR)/\
 /Fp$(OUTDIR)/"TEST.pch" /Fo$(INTDIR)/ /Fd"\e32sys/TEST.pdb" /c 
CPP_OBJS=\work\oplr\wins\deb/
# ADD BASE RSC /l 0x1 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"\work\oplr\wins\deb/TESTD.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"\work\oplr\wins\deb/TESTD.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TESTS.SBR \
	$(INTDIR)/TESTMAIN.SBR \
	$(INTDIR)/NOT_YET.SBR \
	$(INTDIR)/STACK.SBR

$(OUTDIR)/TESTD.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386
# ADD LINK32 \e32sys\efsrvd.lib \e32sys\edlld.obj \e32sys\euserd.lib \e32sys\bafld.lib \e32sys\w32d.lib \e32sys\gdid.lib \e32sys\tboxd.lib \e32sys\etextd.lib \e32sys\ealwld.lib \e32sys\clockd.lib \e32sys\hcild.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/TEST.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/TESTD.APP" /IMPLIB:"\e32sys/TEST.LIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrvd.lib \e32sys\edlld.obj \e32sys\euserd.lib\
 \e32sys\bafld.lib \e32sys\w32d.lib \e32sys\gdid.lib \e32sys\tboxd.lib\
 \e32sys\etextd.lib \e32sys\ealwld.lib \e32sys\clockd.lib \e32sys\hcild.lib\
 /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /PDB:"\e32sys/TEST.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE\
 /OUT:"\e32sys/TESTD.APP" /IMPLIB:"\e32sys/TEST.LIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TESTS.OBJ \
	$(INTDIR)/TESTMAIN.OBJ \
	$(INTDIR)/NOT_YET.OBJ \
	$(INTDIR)/STACK.OBJ

\e32sys\TESTD.APP : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_Un"
# PROP BASE Intermediate_Dir "Win32_Un"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\work\hcil\winsd\udeb"
# PROP Intermediate_Dir "\work\hcil\winsd\udeb"
OUTDIR=\work\hcil\winsd\udeb
INTDIR=\work\hcil\winsd\udeb

ALL : \e32sys\DATAUD.DLL $(OUTDIR)/HCILUD.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /YX /Od /I "..\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "__VC32__" /D "__DLL__" /FR /c
# ADD CPP /nologo /Zp4 /MT /W4 /Zi /YX /Od /I "..\incg" /I "..\inc" /I "\e32inc" /I "\oplr\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /D "_UNICODE" /FR /Fp"\work\hcil\winsd\udeb/HCILUD.pch" /Fd"\e32sys/HCILUD.pdb" /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /Zi /YX /Od /I "..\incg" /I "..\inc" /I "\e32inc"\
 /I "\oplr\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__"\
 /D "__WINS__" /D "_UNICODE" /FR$(INTDIR)/ /Fp"\work\hcil\winsd\udeb/HCILUD.pch"\
 /Fo$(INTDIR)/ /Fd"\e32sys/HCILUD.pdb" /c 
CPP_OBJS=\work\hcil\winsd\udeb/
# ADD BASE RSC /l 0x1 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"\work\hcil\winsd\udeb/HCILUD.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"\work\hcil\winsd\udeb/HCILUD.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TESTS.SBR \
	$(INTDIR)/TESTMAIN.SBR \
	$(INTDIR)/NOT_YET.SBR \
	$(INTDIR)/STACK.SBR

$(OUTDIR)/HCILUD.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 \e32sys\eexed.lib \e32sys\euserd.lib user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386 /OUT:"\HCIL\dll/WINHCIL.dll"
# SUBTRACT BASE LINK32 /PROFILE /PDB:none
# ADD LINK32 \e32sys\efsrvud.lib \e32sys\eexeud.lib \e32sys\euserud.lib \e32sys\baflud.lib \e32sys\w32ud.lib \e32sys\gdiud.lib \e32sys\hcilud.lib msvcrt.lib kernel32.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/DATAUD.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/DATAUD.DLL" /IMPLIB:"\e32sys/HCILUD.LIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrvud.lib \e32sys\eexeud.lib \e32sys\euserud.lib\
 \e32sys\baflud.lib \e32sys\w32ud.lib \e32sys\gdiud.lib \e32sys\hcilud.lib\
 msvcrt.lib kernel32.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL\
 /INCREMENTAL:yes /PDB:"\e32sys/DATAUD.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB\
 /FORCE /OUT:"\e32sys/DATAUD.DLL" /IMPLIB:"\e32sys/HCILUD.LIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TESTS.OBJ \
	$(INTDIR)/TESTMAIN.OBJ \
	$(INTDIR)/NOT_YET.OBJ \
	$(INTDIR)/STACK.OBJ

\e32sys\DATAUD.DLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_U0"
# PROP BASE Intermediate_Dir "Win32_U0"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\work\hcil\winsd\urel"
# PROP Intermediate_Dir "\work\hcil\winsd\urel"
OUTDIR=\work\hcil\winsd\urel
INTDIR=\work\hcil\winsd\urel

ALL : \e32sys\DATAU.DLL $(OUTDIR)/HCILU.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MT /W4 /GX /YX /O2 /I "..\inc" /D "NDEBUG" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "__VC32__" /D "__DLL__" /FR /c
# ADD CPP /nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /I "\oplr\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /D "_UNICODE" /FR /Fp"\work\hcil\winsd\urel/HCILU.pch" /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /I\
 "\oplr\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D\
 "__WINS__" /D "_UNICODE" /FR$(INTDIR)/ /Fp"\work\hcil\winsd\urel/HCILU.pch"\
 /Fo$(INTDIR)/ /c 
CPP_OBJS=\work\hcil\winsd\urel/
# ADD BASE RSC /l 0x1 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"\work\hcil\winsd\urel/HCILU.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"\work\hcil\winsd\urel/HCILU.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TESTS.SBR \
	$(INTDIR)/TESTMAIN.SBR \
	$(INTDIR)/NOT_YET.SBR \
	$(INTDIR)/STACK.SBR

$(OUTDIR)/HCILU.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 \e32sys\eexe.lib \e32sys\euser.lib user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386 /OUT:"\HCIL\dll/WINHCIL.dll" /WARN:2
# SUBTRACT BASE LINK32 /PDB:none /NODEFAULTLIB
# ADD LINK32 \e32sys\efsrvu.lib \e32sys\eexeu.lib \e32sys\euseru.lib \e32sys\baflu.lib \e32sys\hcilu.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/DATAU.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/DATAU.DLL" /IMPLIB:"\e32sys/HCILULIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrvu.lib \e32sys\eexeu.lib \e32sys\euseru.lib\
 \e32sys\baflu.lib \e32sys\hcilu.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib\
 /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /INCREMENTAL:no\
 /PDB:"\e32sys/DATAU.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE\
 /OUT:"\e32sys/DATAU.DLL" /IMPLIB:"\e32sys/HCILULIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TESTS.OBJ \
	$(INTDIR)/TESTMAIN.OBJ \
	$(INTDIR)/NOT_YET.OBJ \
	$(INTDIR)/STACK.OBJ

\e32sys\DATAU.DLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\TESTS.CPP
DEP_TESTS=\
	.\TESTS.H\
	\oplr\inc\OPCODES.H\
	\oplr\inc\OPLR.H\
	\oplr\inc\HCILSTDO.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLPAN.H\
	\oplr\inc\MODULE.H\
	\E32INC\E32STD.H\
	\E32INC\F32FILE.H\
	\E32INC\BAFLSTD.H\
	\E32INC\E32SVR.H\
	\E32INC\W32STD.H\
	\E32INC\EDITTEXT.H\
	\E32INC\GLBLTEXT.H\
	\E32INC\LAYOUT.H\
	\E32INC\FRMTVIEW.H\
	\E32INC\HCIRES.HRH\
	\E32INC\HCIDEF.H\
	\E32INC\CLISTARR.H\
	\E32INC\HCIMATCH.H\
	\E32INC\HCISCHED.H\
	\E32INC\HCISCNEL.H\
	\E32INC\HCIWIN.H\
	\E32INC\HCICTRL.H\
	\E32INC\HCIBUT.H\
	\E32INC\HCIMWIN.H\
	\E32INC\HCIMSG.H\
	\E32INC\HCIWOC.H\
	\E32INC\HCIMCTRL.H\
	\E32INC\HCIPROGI.H\
	\E32INC\HCICHLST.H\
	\E32INC\HCICOMBO.H\
	\E32INC\HCITBAR.H\
	\E32INC\HCIDIALG.H\
	\E32INC\HCIMPDLG.H\
	\E32INC\HCIDEFLD.H\
	\E32INC\HCIMENU.H\
	\E32INC\HCIBWIN.H\
	\E32INC\HCISCRLL.H\
	\E32INC\HCIEBOX.H\
	\E32INC\HCIMFNE.H\
	\E32INC\HCIFPNE.H\
	\E32INC\HCICMAP.H\
	\E32INC\HCICALDS.H\
	\E32INC\HCICALEN.H\
	\E32INC\HCICLOCK.H\
	\E32INC\HCICLK.H\
	\E32INC\HCICONSO.H\
	\E32INC\HCILBOX.H\
	\E32INC\FLATLIST.H\
	\E32INC\HIERLIST.H\
	\E32INC\LISTIMG.H\
	\E32INC\HCIFDC.H\
	\E32INC\HCIWLDSL.H\
	\E32INC\HCIACTAB.H\
	\E32INC\HCIEVMAN.H\
	\E32INC\HCISCRPT.H\
	\E32INC\HCIDEBUG.H\
	\E32INC\HCISERVC.H\
	\E32INC\HCIENV.H\
	\E32INC\HCIDLGS.H\
	\E32INC\HCILSTD.INL\
	\oplr\inc\STACK.INL\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\E32INC\E32DEF.H\
	\E32INC\E32DES8.H\
	\E32INC\E32DES16.H\
	\E32INC\E32STD.INL\
	\E32INC\E32ADT.H\
	\E32INC\BARSC.H\
	\E32INC\E32VIRT.H\
	\E32INC\BITDEV.H\
	\E32INC\S32STRM.H\
	\E32INC\EDITTEXT.INL\
	\E32INC\LAYDOC.H\
	\E32INC\FMTLAYER.H\
	\E32INC\GLBLTEXT.INL\
	\E32INC\FORMLINE.H\
	\E32INC\LAYOUT.INL\
	\E32INC\FRMTVIEW.INL\
	\E32INC\HCIMATCH.INL\
	\E32INC\HCIBUT.INL\
	\E32INC\S32STOR.H\
	\E32INC\S32FSTR.H\
	\E32INC\T32WLD.H\
	\E32INC\CLOCK.H\
	\E32INC\HCIWLDSL.INL\
	\E32INC\E32ADT.INL\
	\E32INC\GDI.H\
	\E32INC\BITSTD.H\
	\E32INC\FBS.H\
	\E32INC\S32STRM.INL\
	\E32INC\TFORMAT.H\
	\E32INC\FMTLAYER.INL\
	\E32INC\TBOX.H\
	\E32INC\S32STOR.INL\
	\E32INC\S32FSTR.INL\
	\E32INC\T32WLD.INL\
	\E32INC\BITBASE.H\
	\E32INC\GDSTORE.H\
	\E32INC\BITMAP.H\
	\E32INC\S32UTL.H\
	\E32INC\TBOX.INL\
	\E32INC\S32UTL.INL

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/TESTS.OBJ :  $(SOURCE)  $(DEP_TESTS) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/TESTS.OBJ :  $(SOURCE)  $(DEP_TESTS) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

$(INTDIR)/TESTS.OBJ :  $(SOURCE)  $(DEP_TESTS) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

$(INTDIR)/TESTS.OBJ :  $(SOURCE)  $(DEP_TESTS) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TESTMAIN.CPP
DEP_TESTM=\
	\E32INC\E32STD.H\
	.\TESTS.H\
	\E32INC\HCIEON.H\
	\E32INC\E32DEF.H\
	\E32INC\E32DES8.H\
	\E32INC\E32DES16.H\
	\E32INC\E32STD.INL\
	\oplr\inc\OPCODES.H\
	\oplr\inc\OPLR.H\
	\oplr\inc\HCILSTDO.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLPAN.H\
	\oplr\inc\MODULE.H\
	\E32INC\F32FILE.H\
	\E32INC\BAFLSTD.H\
	\E32INC\E32SVR.H\
	\E32INC\W32STD.H\
	\E32INC\EDITTEXT.H\
	\E32INC\GLBLTEXT.H\
	\E32INC\LAYOUT.H\
	\E32INC\FRMTVIEW.H\
	\E32INC\HCIRES.HRH\
	\E32INC\HCIDEF.H\
	\E32INC\CLISTARR.H\
	\E32INC\HCIMATCH.H\
	\E32INC\HCISCHED.H\
	\E32INC\HCISCNEL.H\
	\E32INC\HCIWIN.H\
	\E32INC\HCICTRL.H\
	\E32INC\HCIBUT.H\
	\E32INC\HCIMWIN.H\
	\E32INC\HCIMSG.H\
	\E32INC\HCIWOC.H\
	\E32INC\HCIMCTRL.H\
	\E32INC\HCIPROGI.H\
	\E32INC\HCICHLST.H\
	\E32INC\HCICOMBO.H\
	\E32INC\HCITBAR.H\
	\E32INC\HCIDIALG.H\
	\E32INC\HCIMPDLG.H\
	\E32INC\HCIDEFLD.H\
	\E32INC\HCIMENU.H\
	\E32INC\HCIBWIN.H\
	\E32INC\HCISCRLL.H\
	\E32INC\HCIEBOX.H\
	\E32INC\HCIMFNE.H\
	\E32INC\HCIFPNE.H\
	\E32INC\HCICMAP.H\
	\E32INC\HCICALDS.H\
	\E32INC\HCICALEN.H\
	\E32INC\HCICLOCK.H\
	\E32INC\HCICLK.H\
	\E32INC\HCICONSO.H\
	\E32INC\HCILBOX.H\
	\E32INC\FLATLIST.H\
	\E32INC\HIERLIST.H\
	\E32INC\LISTIMG.H\
	\E32INC\HCIFDC.H\
	\E32INC\HCIWLDSL.H\
	\E32INC\HCIACTAB.H\
	\E32INC\HCIEVMAN.H\
	\E32INC\HCISCRPT.H\
	\E32INC\HCIDEBUG.H\
	\E32INC\HCISERVC.H\
	\E32INC\HCIENV.H\
	\E32INC\HCIDLGS.H\
	\E32INC\HCILSTD.INL\
	\oplr\inc\STACK.INL\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\E32INC\E32ADT.H\
	\E32INC\BARSC.H\
	\E32INC\E32VIRT.H\
	\E32INC\BITDEV.H\
	\E32INC\S32STRM.H\
	\E32INC\EDITTEXT.INL\
	\E32INC\LAYDOC.H\
	\E32INC\FMTLAYER.H\
	\E32INC\GLBLTEXT.INL\
	\E32INC\FORMLINE.H\
	\E32INC\LAYOUT.INL\
	\E32INC\FRMTVIEW.INL\
	\E32INC\HCIMATCH.INL\
	\E32INC\HCIBUT.INL\
	\E32INC\S32STOR.H\
	\E32INC\S32FSTR.H\
	\E32INC\T32WLD.H\
	\E32INC\CLOCK.H\
	\E32INC\HCIWLDSL.INL\
	\E32INC\E32ADT.INL\
	\E32INC\GDI.H\
	\E32INC\BITSTD.H\
	\E32INC\FBS.H\
	\E32INC\S32STRM.INL\
	\E32INC\TFORMAT.H\
	\E32INC\FMTLAYER.INL\
	\E32INC\TBOX.H\
	\E32INC\S32STOR.INL\
	\E32INC\S32FSTR.INL\
	\E32INC\T32WLD.INL\
	\E32INC\BITBASE.H\
	\E32INC\GDSTORE.H\
	\E32INC\BITMAP.H\
	\E32INC\S32UTL.H\
	\E32INC\TBOX.INL\
	\E32INC\S32UTL.INL

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/TESTMAIN.OBJ :  $(SOURCE)  $(DEP_TESTM) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/TESTMAIN.OBJ :  $(SOURCE)  $(DEP_TESTM) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

$(INTDIR)/TESTMAIN.OBJ :  $(SOURCE)  $(DEP_TESTM) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

$(INTDIR)/TESTMAIN.OBJ :  $(SOURCE)  $(DEP_TESTM) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\oplr\seng\NOT_YET.CPP
DEP_NOT_Y=\
	\E32INC\E32STD.H\
	\oplr\inc\OPCODES.H\
	\E32INC\E32DEF.H\
	\E32INC\E32DES8.H\
	\E32INC\E32DES16.H\
	\E32INC\E32STD.INL\
	\oplr\inc\OPLR.H\
	\oplr\inc\HCILSTDO.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLPAN.H\
	\oplr\inc\MODULE.H\
	\E32INC\F32FILE.H\
	\E32INC\BAFLSTD.H\
	\E32INC\E32SVR.H\
	\E32INC\W32STD.H\
	\E32INC\EDITTEXT.H\
	\E32INC\GLBLTEXT.H\
	\E32INC\LAYOUT.H\
	\E32INC\FRMTVIEW.H\
	\E32INC\HCIRES.HRH\
	\E32INC\HCIDEF.H\
	\E32INC\CLISTARR.H\
	\E32INC\HCIMATCH.H\
	\E32INC\HCISCHED.H\
	\E32INC\HCISCNEL.H\
	\E32INC\HCIWIN.H\
	\E32INC\HCICTRL.H\
	\E32INC\HCIBUT.H\
	\E32INC\HCIMWIN.H\
	\E32INC\HCIMSG.H\
	\E32INC\HCIWOC.H\
	\E32INC\HCIMCTRL.H\
	\E32INC\HCIPROGI.H\
	\E32INC\HCICHLST.H\
	\E32INC\HCICOMBO.H\
	\E32INC\HCITBAR.H\
	\E32INC\HCIDIALG.H\
	\E32INC\HCIMPDLG.H\
	\E32INC\HCIDEFLD.H\
	\E32INC\HCIMENU.H\
	\E32INC\HCIBWIN.H\
	\E32INC\HCISCRLL.H\
	\E32INC\HCIEBOX.H\
	\E32INC\HCIMFNE.H\
	\E32INC\HCIFPNE.H\
	\E32INC\HCICMAP.H\
	\E32INC\HCICALDS.H\
	\E32INC\HCICALEN.H\
	\E32INC\HCICLOCK.H\
	\E32INC\HCICLK.H\
	\E32INC\HCICONSO.H\
	\E32INC\HCILBOX.H\
	\E32INC\FLATLIST.H\
	\E32INC\HIERLIST.H\
	\E32INC\LISTIMG.H\
	\E32INC\HCIFDC.H\
	\E32INC\HCIWLDSL.H\
	\E32INC\HCIACTAB.H\
	\E32INC\HCIEVMAN.H\
	\E32INC\HCISCRPT.H\
	\E32INC\HCIDEBUG.H\
	\E32INC\HCISERVC.H\
	\E32INC\HCIENV.H\
	\E32INC\HCIDLGS.H\
	\E32INC\HCILSTD.INL\
	\oplr\inc\STACK.INL\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\E32INC\E32ADT.H\
	\E32INC\BARSC.H\
	\E32INC\E32VIRT.H\
	\E32INC\BITDEV.H\
	\E32INC\S32STRM.H\
	\E32INC\EDITTEXT.INL\
	\E32INC\LAYDOC.H\
	\E32INC\FMTLAYER.H\
	\E32INC\GLBLTEXT.INL\
	\E32INC\FORMLINE.H\
	\E32INC\LAYOUT.INL\
	\E32INC\FRMTVIEW.INL\
	\E32INC\HCIMATCH.INL\
	\E32INC\HCIBUT.INL\
	\E32INC\S32STOR.H\
	\E32INC\S32FSTR.H\
	\E32INC\T32WLD.H\
	\E32INC\CLOCK.H\
	\E32INC\HCIWLDSL.INL\
	\E32INC\E32ADT.INL\
	\E32INC\GDI.H\
	\E32INC\BITSTD.H\
	\E32INC\FBS.H\
	\E32INC\S32STRM.INL\
	\E32INC\TFORMAT.H\
	\E32INC\FMTLAYER.INL\
	\E32INC\TBOX.H\
	\E32INC\S32STOR.INL\
	\E32INC\S32FSTR.INL\
	\E32INC\T32WLD.INL\
	\E32INC\BITBASE.H\
	\E32INC\GDSTORE.H\
	\E32INC\BITMAP.H\
	\E32INC\S32UTL.H\
	\E32INC\TBOX.INL\
	\E32INC\S32UTL.INL

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/NOT_YET.OBJ :  $(SOURCE)  $(DEP_NOT_Y) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/NOT_YET.OBJ :  $(SOURCE)  $(DEP_NOT_Y) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

$(INTDIR)/NOT_YET.OBJ :  $(SOURCE)  $(DEP_NOT_Y) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

$(INTDIR)/NOT_YET.OBJ :  $(SOURCE)  $(DEP_NOT_Y) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\oplr\seng\STACK.CPP
DEP_STACK=\
	\oplr\inc\HCILSTDO.H\
	\oplr\inc\STACK.H\
	\E32INC\E32STD.H\
	\E32INC\F32FILE.H\
	\E32INC\BAFLSTD.H\
	\E32INC\E32SVR.H\
	\E32INC\W32STD.H\
	\E32INC\EDITTEXT.H\
	\E32INC\GLBLTEXT.H\
	\E32INC\LAYOUT.H\
	\E32INC\FRMTVIEW.H\
	\E32INC\HCIRES.HRH\
	\E32INC\HCIDEF.H\
	\E32INC\CLISTARR.H\
	\E32INC\HCIMATCH.H\
	\E32INC\HCISCHED.H\
	\E32INC\HCISCNEL.H\
	\E32INC\HCIWIN.H\
	\E32INC\HCICTRL.H\
	\E32INC\HCIBUT.H\
	\E32INC\HCIMWIN.H\
	\E32INC\HCIMSG.H\
	\E32INC\HCIWOC.H\
	\E32INC\HCIMCTRL.H\
	\E32INC\HCIPROGI.H\
	\E32INC\HCICHLST.H\
	\E32INC\HCICOMBO.H\
	\E32INC\HCITBAR.H\
	\E32INC\HCIDIALG.H\
	\E32INC\HCIMPDLG.H\
	\E32INC\HCIDEFLD.H\
	\E32INC\HCIMENU.H\
	\E32INC\HCIBWIN.H\
	\E32INC\HCISCRLL.H\
	\E32INC\HCIEBOX.H\
	\E32INC\HCIMFNE.H\
	\E32INC\HCIFPNE.H\
	\E32INC\HCICMAP.H\
	\E32INC\HCICALDS.H\
	\E32INC\HCICALEN.H\
	\E32INC\HCICLOCK.H\
	\E32INC\HCICLK.H\
	\E32INC\HCICONSO.H\
	\E32INC\HCILBOX.H\
	\E32INC\FLATLIST.H\
	\E32INC\HIERLIST.H\
	\E32INC\LISTIMG.H\
	\E32INC\HCIFDC.H\
	\E32INC\HCIWLDSL.H\
	\E32INC\HCIACTAB.H\
	\E32INC\HCIEVMAN.H\
	\E32INC\HCISCRPT.H\
	\E32INC\HCIDEBUG.H\
	\E32INC\HCISERVC.H\
	\E32INC\HCIENV.H\
	\E32INC\HCIDLGS.H\
	\E32INC\HCILSTD.INL\
	\oplr\inc\STACK.INL\
	\E32INC\E32DEF.H\
	\E32INC\E32DES8.H\
	\E32INC\E32DES16.H\
	\E32INC\E32STD.INL\
	\E32INC\E32ADT.H\
	\E32INC\BARSC.H\
	\E32INC\E32VIRT.H\
	\E32INC\BITDEV.H\
	\E32INC\S32STRM.H\
	\E32INC\EDITTEXT.INL\
	\E32INC\LAYDOC.H\
	\E32INC\FMTLAYER.H\
	\E32INC\GLBLTEXT.INL\
	\E32INC\FORMLINE.H\
	\E32INC\LAYOUT.INL\
	\E32INC\FRMTVIEW.INL\
	\E32INC\HCIMATCH.INL\
	\E32INC\HCIBUT.INL\
	\E32INC\S32STOR.H\
	\E32INC\S32FSTR.H\
	\E32INC\T32WLD.H\
	\E32INC\CLOCK.H\
	\E32INC\HCIWLDSL.INL\
	\E32INC\E32ADT.INL\
	\E32INC\GDI.H\
	\E32INC\BITSTD.H\
	\E32INC\FBS.H\
	\E32INC\S32STRM.INL\
	\E32INC\TFORMAT.H\
	\E32INC\FMTLAYER.INL\
	\E32INC\TBOX.H\
	\E32INC\S32STOR.INL\
	\E32INC\S32FSTR.INL\
	\E32INC\T32WLD.INL\
	\E32INC\BITBASE.H\
	\E32INC\GDSTORE.H\
	\E32INC\BITMAP.H\
	\E32INC\S32UTL.H\
	\E32INC\TBOX.INL\
	\E32INC\S32UTL.INL

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/STACK.OBJ :  $(SOURCE)  $(DEP_STACK) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/STACK.OBJ :  $(SOURCE)  $(DEP_STACK) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

$(INTDIR)/STACK.OBJ :  $(SOURCE)  $(DEP_STACK) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

$(INTDIR)/STACK.OBJ :  $(SOURCE)  $(DEP_STACK) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ENDIF 

# End Source File
# End Group
# End Project
################################################################################
