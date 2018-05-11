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
!MESSAGE NMAKE /f "TGRAPHIC.MAK" CFG="Win32 Debug"
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
# PROP Output_Dir "\work\CONE\winsd\rel"
# PROP Intermediate_Dir "\work\CONE\winsd\rel"
OUTDIR=\work\CONE\winsd\rel
INTDIR=\work\CONE\winsd\rel

ALL : \e32sys\CONE.DLL $(OUTDIR)/TGRAPHIC.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MD /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /FR /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /D\
 "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"TGRAPHIC.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=\work\CONE\winsd\rel/
# ADD BASE RSC /l 0x1 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o$(OUTDIR)/"TGRAPHIC.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TGRAPHIC.SBR \
	$(INTDIR)/STACK.SBR \
	$(INTDIR)/G_KEYWRD.SBR \
	$(INTDIR)/GRAPHICS.SBR \
	$(INTDIR)/NOT_YET.SBR

$(OUTDIR)/TGRAPHIC.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386
# ADD LINK32 \e32sys\efsrv.lib \e32sys\estat.lib \e32sys\ewins.lib \e32sys\bafl.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/CONE.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/CONE.DLL" /IMPLIB:"\e32sys/CONE.LIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrv.lib \e32sys\estat.lib \e32sys\ewins.lib\
 \e32sys\bafl.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib /NOLOGO\
 /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /INCREMENTAL:no\
 /PDB:"\e32sys/CONE.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE\
 /OUT:"\e32sys/CONE.DLL" /IMPLIB:"\e32sys/CONE.LIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TGRAPHIC.OBJ \
	$(INTDIR)/STACK.OBJ \
	$(INTDIR)/G_KEYWRD.OBJ \
	$(INTDIR)/GRAPHICS.OBJ \
	$(INTDIR)/NOT_YET.OBJ

\e32sys\CONE.DLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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

ALL : \e32sys\TGRAPHIC.PRG \work\oplr\winsd\deb\TGRAPHIC.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /Zp4 /MD /W4 /Zi /YX /Od /I "..\inc" /I "\e32inc" /I "\cone\inc" /I "\eikon\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /FR /Fd"\e32sys/TGRAPHIC.pdb" /c
CPP_PROJ=/nologo /Zp4 /MD /W4 /Zi /YX /Od /I "..\inc" /I "\e32inc" /I\
 "\cone\inc" /I "\eikon\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__"\
 /D "__DLL__" /D "__WINS__" /FR$(INTDIR)/ /Fp$(OUTDIR)/"TGRAPHIC.pch"\
 /Fo$(INTDIR)/ /Fd"\e32sys/TGRAPHIC.pdb" /c 
CPP_OBJS=\work\oplr\wins\deb/
# ADD BASE RSC /l 0x1 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"\work\oplr\winsd\deb/TGRAPHIC.bsc"
BSC32_FLAGS=/nologo /o"\work\oplr\winsd\deb/TGRAPHIC.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TGRAPHIC.SBR \
	$(INTDIR)/STACK.SBR \
	$(INTDIR)/G_KEYWRD.SBR \
	$(INTDIR)/GRAPHICS.SBR \
	$(INTDIR)/NOT_YET.SBR

\work\oplr\winsd\deb\TGRAPHIC.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386
# ADD LINK32 \e32sys\efsrvd.lib \e32sys\edlld.obj \e32sys\euserd.lib \e32sys\w32d.lib \e32sys\gdid.lib \e32sys\bitgdid.lib \e32sys\fbsd.lib \e32sys\estord.lib \e32sys\coned.lib \e32sys\eikond.lib \e32sys\etextd.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/TGRAPHIC.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/TGRAPHIC.PRG" /IMPLIB:"\e32sys/TGRAPHIC.LIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrvd.lib \e32sys\edlld.obj \e32sys\euserd.lib\
 \e32sys\w32d.lib \e32sys\gdid.lib \e32sys\bitgdid.lib \e32sys\fbsd.lib\
 \e32sys\estord.lib \e32sys\coned.lib \e32sys\eikond.lib \e32sys\etextd.lib\
 /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /PDB:"\e32sys/TGRAPHIC.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE\
 /OUT:"\e32sys/TGRAPHIC.PRG" /IMPLIB:"\e32sys/TGRAPHIC.LIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TGRAPHIC.OBJ \
	$(INTDIR)/STACK.OBJ \
	$(INTDIR)/G_KEYWRD.OBJ \
	$(INTDIR)/GRAPHICS.OBJ \
	$(INTDIR)/NOT_YET.OBJ

\e32sys\TGRAPHIC.PRG : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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
# PROP Output_Dir "\work\CONE\winsd\udeb"
# PROP Intermediate_Dir "\work\CONE\winsd\udeb"
OUTDIR=\work\CONE\winsd\udeb
INTDIR=\work\CONE\winsd\udeb

ALL : \e32sys\CONEUD.DLL $(OUTDIR)/CONEUD.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MT /W4 /GX /Zi /YX /Od /I "..\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "__VC32__" /D "__DLL__" /FR /c
# ADD CPP /nologo /Zp4 /MT /W4 /Zi /YX /Od /I "..\incg" /I "..\inc" /I "\e32inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /D "_UNICODE" /FR /Fp"\work\CONE\winsd\udeb/CONEUD.pch" /Fd"\e32sys/CONEUD.pdb" /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /Zi /YX /Od /I "..\incg" /I "..\inc" /I "\e32inc"\
 /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__"\
 /D "_UNICODE" /FR$(INTDIR)/ /Fp"\work\CONE\winsd\udeb/CONEUD.pch" /Fo$(INTDIR)/\
 /Fd"\e32sys/CONEUD.pdb" /c 
CPP_OBJS=\work\CONE\winsd\udeb/
# ADD BASE RSC /l 0x1 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"\work\CONE\winsd\udeb/CONEUD.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"\work\CONE\winsd\udeb/CONEUD.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TGRAPHIC.SBR \
	$(INTDIR)/STACK.SBR \
	$(INTDIR)/G_KEYWRD.SBR \
	$(INTDIR)/GRAPHICS.SBR \
	$(INTDIR)/NOT_YET.SBR

$(OUTDIR)/CONEUD.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 \e32sys\estatd.lib \e32sys\ewinsd.lib user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386 /OUT:"\CONE\dll/WINCONE.dll"
# SUBTRACT BASE LINK32 /PROFILE /PDB:none
# ADD LINK32 \e32sys\efsrvud.lib \e32sys\estatud.lib \e32sys\ewinsud.lib \e32sys\baflud.lib \e32sys\w32ud.lib \e32sys\gdiud.lib msvcrt.lib kernel32.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/CONEUD.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/CONEUD.DLL" /IMPLIB:"\e32sys/CONEUD.LIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrvud.lib \e32sys\estatud.lib \e32sys\ewinsud.lib\
 \e32sys\baflud.lib \e32sys\w32ud.lib \e32sys\gdiud.lib msvcrt.lib kernel32.lib\
 /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /INCREMENTAL:yes\
 /PDB:"\e32sys/CONEUD.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE\
 /OUT:"\e32sys/CONEUD.DLL" /IMPLIB:"\e32sys/CONEUD.LIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TGRAPHIC.OBJ \
	$(INTDIR)/STACK.OBJ \
	$(INTDIR)/G_KEYWRD.OBJ \
	$(INTDIR)/GRAPHICS.OBJ \
	$(INTDIR)/NOT_YET.OBJ

\e32sys\CONEUD.DLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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
# PROP Output_Dir "\work\CONE\winsd\urel"
# PROP Intermediate_Dir "\work\CONE\winsd\urel"
OUTDIR=\work\CONE\winsd\urel
INTDIR=\work\CONE\winsd\urel

ALL : \e32sys\CONEU.DLL $(OUTDIR)/CONEU.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MT /W4 /GX /YX /O2 /I "..\inc" /D "NDEBUG" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "__VC32__" /D "__DLL__" /FR /c
# ADD CPP /nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /D "_UNICODE" /FR /Fp"\work\CONE\winsd\urel/CONEU.pch" /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /YX /O2 /I "..\incg" /I "..\inc" /I "\e32inc" /D\
 "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /D\
 "_UNICODE" /FR$(INTDIR)/ /Fp"\work\CONE\winsd\urel/CONEU.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=\work\CONE\winsd\urel/
# ADD BASE RSC /l 0x1 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# ADD BSC32 /nologo /o"\work\CONE\winsd\urel/CONEU.bsc"
# SUBTRACT BSC32 /Iu
BSC32_FLAGS=/nologo /o"\work\CONE\winsd\urel/CONEU.bsc" 
BSC32_SBRS= \
	$(INTDIR)/TGRAPHIC.SBR \
	$(INTDIR)/STACK.SBR \
	$(INTDIR)/G_KEYWRD.SBR \
	$(INTDIR)/GRAPHICS.SBR \
	$(INTDIR)/NOT_YET.SBR

$(OUTDIR)/CONEU.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 \e32sys\estat.lib \e32sys\ewins.lib user32.lib gdi32.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386 /OUT:"\CONE\dll/WINCONE.dll" /WARN:2
# SUBTRACT BASE LINK32 /PDB:none /NODEFAULTLIB
# ADD LINK32 \e32sys\efsrvu.lib \e32sys\estatu.lib \e32sys\ewinsu.lib \e32sys\baflu.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib /NOLOGO /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /PDB:"\e32sys/CONEU.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE /OUT:"\e32sys/CONEU.DLL" /IMPLIB:"\e32sys/CONEULIB" /WARN:1
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=\e32sys\efsrvu.lib \e32sys\estatu.lib \e32sys\ewinsu.lib\
 \e32sys\baflu.lib user32.lib gdi32.lib msvcrt.lib kernel32.lib /NOLOGO\
 /ENTRY:"_E32Dll" /SUBSYSTEM:windows /DLL /INCREMENTAL:no\
 /PDB:"\e32sys/CONEU.PDB" /DEBUG /MACHINE:I386 /NODEFAULTLIB /FORCE\
 /OUT:"\e32sys/CONEU.DLL" /IMPLIB:"\e32sys/CONEULIB" /WARN:1  
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/TGRAPHIC.OBJ \
	$(INTDIR)/STACK.OBJ \
	$(INTDIR)/G_KEYWRD.OBJ \
	$(INTDIR)/GRAPHICS.OBJ \
	$(INTDIR)/NOT_YET.OBJ

\e32sys\CONEU.DLL : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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

SOURCE=.\TGRAPHIC.CPP
DEP_TGRAP=\
	\e32inc\E32STD.H\
	\oplr\inc\GRAPHICS.H\
	.\TGRAPHIC.H\
	\eikon\inc\EIKENV.H\
	\eikon\inc\EIKAPPC.H\
	\e32inc\E32DEF.H\
	\e32inc\E32DES8.H\
	\e32inc\E32DES16.H\
	\e32inc\E32STD.INL\
	\e32inc\COEMAIN.H\
	\oplr\inc\OPCODES.H\
	\eikon\inc\EIKREQD.H\
	\e32inc\COEAPPC.H\
	\eikon\inc\EIKCMOBS.H\
	\e32inc\COEREQD.H\
	\oplr\inc\OPLR.H\
	\e32inc\COECSTCK.H\
	\e32inc\E32ADT.H\
	\e32inc\GDI.H\
	\e32inc\W32STD.H\
	\e32inc\BARSC.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLPAN.H\
	..\slib2\GLOBTBL.H\
	\eikon\inc\EIKCONSO.H\
	\eikon\inc\EIKCOLOR.HRH\
	\oplr\inc\MODULE.H\
	\e32inc\COECNTRL.H\
	\e32inc\E32ADT.INL\
	\e32inc\S32STRM.H\
	\e32inc\BITDEV.H\
	\oplr\inc\STACK.INL\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\e32inc\COEWIN.H\
	\e32inc\E32SVR.H\
	\e32inc\S32STRM.INL\
	\e32inc\FBS.H\
	\e32inc\BITSTD.H\
	\e32inc\E32VIRT.H\
	\e32inc\S32UTL.H\
	\e32inc\FNTSTORE.H\
	\e32inc\BITMAP.H\
	\e32inc\BITBASE.H\
	\e32inc\S32UTL.INL\
	\e32inc\S32STOR.H\
	\e32inc\S32FSTR.H\
	\e32inc\S32STOR.INL\
	\e32inc\F32FILE.H\
	\e32inc\S32FSTR.INL

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/TGRAPHIC.OBJ :  $(SOURCE)  $(DEP_TGRAP) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/TGRAPHIC.OBJ :  $(SOURCE)  $(DEP_TGRAP) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

$(INTDIR)/TGRAPHIC.OBJ :  $(SOURCE)  $(DEP_TGRAP) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

$(INTDIR)/TGRAPHIC.OBJ :  $(SOURCE)  $(DEP_TGRAP) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\oplr\seng\STACK.CPP
DEP_STACK=\
	\oplr\inc\OPLR.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\OPLPAN.H\
	\eikon\inc\EIKENV.H\
	\eikon\inc\EIKAPPC.H\
	..\slib2\GLOBTBL.H\
	\eikon\inc\EIKCONSO.H\
	\eikon\inc\EIKCOLOR.HRH\
	\oplr\inc\MODULE.H\
	\e32inc\E32STD.H\
	\e32inc\E32ADT.H\
	\oplr\inc\STACK.INL\
	\eikon\inc\EIKREQD.H\
	\e32inc\COEMAIN.H\
	\e32inc\COEAPPC.H\
	\eikon\inc\EIKCMOBS.H\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\e32inc\E32DEF.H\
	\e32inc\E32DES8.H\
	\e32inc\E32DES16.H\
	\e32inc\E32STD.INL\
	\e32inc\E32ADT.INL\
	\e32inc\COEREQD.H\
	\e32inc\COECSTCK.H\
	\e32inc\GDI.H\
	\e32inc\W32STD.H\
	\e32inc\BARSC.H\
	\e32inc\COECNTRL.H\
	\e32inc\S32STRM.H\
	\e32inc\BITDEV.H\
	\e32inc\COEWIN.H\
	\e32inc\E32SVR.H\
	\e32inc\S32STRM.INL\
	\e32inc\FBS.H\
	\e32inc\BITSTD.H\
	\e32inc\E32VIRT.H\
	\e32inc\S32UTL.H\
	\e32inc\FNTSTORE.H\
	\e32inc\BITMAP.H\
	\e32inc\BITBASE.H\
	\e32inc\S32UTL.INL\
	\e32inc\S32STOR.H\
	\e32inc\S32FSTR.H\
	\e32inc\S32STOR.INL\
	\e32inc\F32FILE.H\
	\e32inc\S32FSTR.INL

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
################################################################################
# Begin Source File

SOURCE=.\G_KEYWRD.CPP
DEP_G_KEY=\
	\e32inc\E32STD.H\
	\oplr\inc\GRAPHICS.H\
	\oplr\inc\OPCODES.H\
	\e32inc\E32DEF.H\
	\e32inc\E32DES8.H\
	\e32inc\E32DES16.H\
	\e32inc\E32STD.INL\
	\e32inc\COEMAIN.H\
	\oplr\inc\OPLR.H\
	\e32inc\COEREQD.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLPAN.H\
	\eikon\inc\EIKENV.H\
	\eikon\inc\EIKAPPC.H\
	..\slib2\GLOBTBL.H\
	\eikon\inc\EIKCONSO.H\
	\eikon\inc\EIKCOLOR.HRH\
	\oplr\inc\MODULE.H\
	\e32inc\E32ADT.H\
	\e32inc\GDI.H\
	\e32inc\W32STD.H\
	\e32inc\BARSC.H\
	\oplr\inc\STACK.INL\
	\eikon\inc\EIKREQD.H\
	\e32inc\COEAPPC.H\
	\eikon\inc\EIKCMOBS.H\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\e32inc\E32ADT.INL\
	\e32inc\S32STRM.H\
	\e32inc\BITDEV.H\
	\e32inc\COECSTCK.H\
	\e32inc\S32STRM.INL\
	\e32inc\FBS.H\
	\e32inc\BITSTD.H\
	\e32inc\COECNTRL.H\
	\e32inc\S32UTL.H\
	\e32inc\FNTSTORE.H\
	\e32inc\BITMAP.H\
	\e32inc\BITBASE.H\
	\e32inc\COEWIN.H\
	\e32inc\E32SVR.H\
	\e32inc\S32UTL.INL\
	\e32inc\S32STOR.H\
	\e32inc\S32FSTR.H\
	\e32inc\E32VIRT.H\
	\e32inc\S32STOR.INL\
	\e32inc\F32FILE.H\
	\e32inc\S32FSTR.INL

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/G_KEYWRD.OBJ :  $(SOURCE)  $(DEP_G_KEY) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/G_KEYWRD.OBJ :  $(SOURCE)  $(DEP_G_KEY) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

$(INTDIR)/G_KEYWRD.OBJ :  $(SOURCE)  $(DEP_G_KEY) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

$(INTDIR)/G_KEYWRD.OBJ :  $(SOURCE)  $(DEP_G_KEY) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GRAPHICS.CPP
DEP_GRAPH=\
	\oplr\inc\GRAPHICS.H\
	\oplr\inc\OPLR.H\
	\e32inc\COEMAIN.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLPAN.H\
	\eikon\inc\EIKENV.H\
	\eikon\inc\EIKAPPC.H\
	..\slib2\GLOBTBL.H\
	\eikon\inc\EIKCONSO.H\
	\eikon\inc\EIKCOLOR.HRH\
	\oplr\inc\MODULE.H\
	\e32inc\COEREQD.H\
	\e32inc\E32STD.H\
	\e32inc\E32ADT.H\
	\oplr\inc\STACK.INL\
	\eikon\inc\EIKREQD.H\
	\e32inc\COEAPPC.H\
	\eikon\inc\EIKCMOBS.H\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\e32inc\GDI.H\
	\e32inc\W32STD.H\
	\e32inc\BARSC.H\
	\e32inc\E32DEF.H\
	\e32inc\E32DES8.H\
	\e32inc\E32DES16.H\
	\e32inc\E32STD.INL\
	\e32inc\E32ADT.INL\
	\e32inc\COECSTCK.H\
	\e32inc\S32STRM.H\
	\e32inc\BITDEV.H\
	\e32inc\COECNTRL.H\
	\e32inc\S32STRM.INL\
	\e32inc\FBS.H\
	\e32inc\BITSTD.H\
	\e32inc\COEWIN.H\
	\e32inc\E32SVR.H\
	\e32inc\S32UTL.H\
	\e32inc\FNTSTORE.H\
	\e32inc\BITMAP.H\
	\e32inc\BITBASE.H\
	\e32inc\E32VIRT.H\
	\e32inc\S32UTL.INL\
	\e32inc\S32STOR.H\
	\e32inc\S32FSTR.H\
	\e32inc\S32STOR.INL\
	\e32inc\F32FILE.H\
	\e32inc\S32FSTR.INL

!IF  "$(CFG)" == "Win32 Release"

$(INTDIR)/GRAPHICS.OBJ :  $(SOURCE)  $(DEP_GRAPH) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

$(INTDIR)/GRAPHICS.OBJ :  $(SOURCE)  $(DEP_GRAPH) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Debug"

$(INTDIR)/GRAPHICS.OBJ :  $(SOURCE)  $(DEP_GRAPH) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Uni Release"

$(INTDIR)/GRAPHICS.OBJ :  $(SOURCE)  $(DEP_GRAPH) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\oplr\seng\NOT_YET.CPP
DEP_NOT_Y=\
	\e32inc\E32STD.H\
	\oplr\inc\OPCODES.H\
	\e32inc\E32MATH.H\
	\e32inc\E32DEF.H\
	\e32inc\E32DES8.H\
	\e32inc\E32DES16.H\
	\e32inc\E32STD.INL\
	\oplr\inc\OPLR.H\
	\oplr\inc\OPLFLG.H\
	\oplr\inc\STACK.H\
	\oplr\inc\OPLPAN.H\
	\eikon\inc\EIKENV.H\
	\eikon\inc\EIKAPPC.H\
	..\slib2\GLOBTBL.H\
	\eikon\inc\EIKCONSO.H\
	\eikon\inc\EIKCOLOR.HRH\
	\oplr\inc\MODULE.H\
	\e32inc\E32ADT.H\
	\oplr\inc\STACK.INL\
	\eikon\inc\EIKREQD.H\
	\e32inc\COEMAIN.H\
	\e32inc\COEAPPC.H\
	\eikon\inc\EIKCMOBS.H\
	\oplr\inc\OPLFILE.H\
	..\slib2\FRAME.H\
	\e32inc\E32ADT.INL\
	\e32inc\COEREQD.H\
	\e32inc\COECSTCK.H\
	\e32inc\GDI.H\
	\e32inc\W32STD.H\
	\e32inc\BARSC.H\
	\e32inc\COECNTRL.H\
	\e32inc\S32STRM.H\
	\e32inc\BITDEV.H\
	\e32inc\COEWIN.H\
	\e32inc\E32SVR.H\
	\e32inc\S32STRM.INL\
	\e32inc\FBS.H\
	\e32inc\BITSTD.H\
	\e32inc\E32VIRT.H\
	\e32inc\S32UTL.H\
	\e32inc\FNTSTORE.H\
	\e32inc\BITMAP.H\
	\e32inc\BITBASE.H\
	\e32inc\S32UTL.INL\
	\e32inc\S32STOR.H\
	\e32inc\S32FSTR.H\
	\e32inc\S32STOR.INL\
	\e32inc\F32FILE.H\
	\e32inc\S32FSTR.INL

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
# End Group
# End Project
################################################################################
