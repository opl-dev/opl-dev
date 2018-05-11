# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=TOPX - Win32 Uni Debug
!MESSAGE No configuration specified.  Defaulting to TOPX - Win32 Uni Debug.
!ENDIF 

!IF "$(CFG)" != "TOPX - Win32 Release" && "$(CFG)" != "TOPX - Win32 Debug" &&\
 "$(CFG)" != "TOPX - Win32 Uni Release" && "$(CFG)" != "TOPX - Win32 Uni Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "TOPX.MAK" CFG="TOPX - Win32 Uni Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TOPX - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TOPX - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TOPX - Win32 Uni Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TOPX - Win32 Uni Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "TOPX - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "TOPX - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\epoc32\release\wins\rel"
# PROP Intermediate_Dir "\epoc32\build\oplr\winsd\rel"
OUTDIR=\epoc32\release\wins\rel
INTDIR=\epoc32\build\oplr\winsd\rel

ALL : "$(OUTDIR)\TOPX.OPX"

CLEAN : 
	-@erase "..\..\epoc32\release\wins\rel\z\system\opx\TOPX.OPX"
	-@erase "..\..\epoc32\build\oplr\winsd\rel\TOPX.OBJ"
	-@erase "..\..\epoc32\release\wins\rel\TOPX.lib"
	-@erase "..\..\epoc32\release\wins\rel\TOPX.exp"
	-@erase "..\..\epoc32\build\oplr\winsd\rel\TOPX.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

$(OUTDIR)/TOPX.bsc : $(OUTDIR)  $(BSC32_SBRS)
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp4 /MT /W4 /Ox /Og /Os /Oy- /I "..\inc" /I "\epoc32\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /Ox /Og /Os /Oy- /I "..\inc" /I "\epoc32\include"\
 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=\epoc32\build\oplr\winsd\rel/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TOPX.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 \epoc32\release\wins\rel\EDLL.obj \epoc32\release\wins\rel\EUSER.lib \epoc32\release\wins\rel\OPLR.LIB /nologo /entry:"_E32Dll" /subsystem:windows /dll /map /machine:I386 /nodefaultlib /def:"\oplr\bwins\opx.def" /out:"\epoc32\release\wins\rel\z\system\opx\TOPX.OPX"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=\epoc32\release\wins\rel\EDLL.obj\
 \epoc32\release\wins\rel\EUSER.lib \epoc32\release\wins\rel\OPLR.LIB /nologo\
 /entry:"_E32Dll" /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/TOPX.pdb" /map:"$(INTDIR)/TOPX.map" /machine:I386 /nodefaultlib\
 /def:"\oplr\bwins\opx.def" /out:"$(OUTDIR)/z\system\opx\TOPX.OPX"\
 /implib:"$(OUTDIR)/TOPX.lib" 
LINK32_OBJS= \
	"$(INTDIR)/TOPX.OBJ"

"$(OUTDIR)\TOPX.OPX" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TOPX - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\epoc32\release\wins\deb\"
# PROP Intermediate_Dir "\epoc32\build\oplr\winsd\deb"
OUTDIR=\epoc32\release\wins\deb
INTDIR=\epoc32\build\oplr\winsd\deb

ALL : "$(OUTDIR)\TOPX.OPX" "..\..\epoc32\release\wins\deb\TOPX.bsc"

CLEAN : 
	-@erase "..\..\epoc32\release\wins\rel\TOPX.PDB"
	-@erase "..\..\epoc32\release\wins\rel\TOPX.IDB"
	-@erase "..\..\epoc32\release\wins\deb\TOPX.bsc"
	-@erase "..\..\epoc32\build\oplr\winsd\deb\TOPX.SBR"
	-@erase "..\..\epoc32\release\wins\deb\z\system\opx\TOPX.OPX"
	-@erase "..\..\epoc32\build\oplr\winsd\deb\TOPX.OBJ"
	-@erase "..\..\epoc32\release\wins\deb\z\system\opx\TOPX.ILK"
	-@erase "..\..\epoc32\release\wins\deb\TOPX.lib"
	-@erase "..\..\epoc32\release\wins\deb\TOPX.exp"
	-@erase "..\..\epoc32\release\wins\deb\TOPX.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /Zi /Od /Ob1 /I "..\inc" /I "\epoc32\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /Fr /Fd"\epoc32\release\wins\rel/TOPX.PDB" /c
CPP_PROJ=/nologo /Zp4 /MTd /W4 /Gm /Zi /Od /Ob1 /I "..\inc" /I\
 "\epoc32\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D\
 "__DLL__" /D "__WINS__" /Fr"$(INTDIR)/" /Fo"$(INTDIR)/"\
 /Fd"\epoc32\release\wins\rel/TOPX.PDB" /c 
CPP_OBJS=\epoc32\build\oplr\winsd\deb/
CPP_SBRS=\epoc32\build\oplr\winsd\deb/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TOPX.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/TOPX.SBR"

"..\..\epoc32\release\wins\deb\TOPX.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 \epoc32\release\wins\deb\EDLL.obj \epoc32\release\wins\deb\EUSER.lib \epoc32\release\wins\deb\OPLR.LIB /nologo /entry:"_E32Dll" /subsystem:windows /dll /debug /machine:I386 /nodefaultlib /def:"\oplr\bwins\opxd.def" /out:"\epoc32\release\wins\deb\z\system\opx\TOPX.OPX"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=\epoc32\release\wins\deb\EDLL.obj\
 \epoc32\release\wins\deb\EUSER.lib \epoc32\release\wins\deb\OPLR.LIB /nologo\
 /entry:"_E32Dll" /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/TOPX.pdb" /debug /machine:I386 /nodefaultlib\
 /def:"\oplr\bwins\opxd.def" /out:"$(OUTDIR)/z\system\opx\TOPX.OPX"\
 /implib:"$(OUTDIR)/TOPX.lib" 
LINK32_OBJS= \
	"$(INTDIR)/TOPX.OBJ"

"$(OUTDIR)\TOPX.OPX" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TOPX___"
# PROP BASE Intermediate_Dir "TOPX___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\epoc32\release\wins\urel"
# PROP Intermediate_Dir "\epoc32\build\oplr\winsd\urel"
# PROP Target_Dir ""
OUTDIR=\epoc32\release\wins\urel
INTDIR=\epoc32\build\oplr\winsd\urel

ALL : "$(OUTDIR)\TOPX.OPX"

CLEAN : 
	-@erase "..\..\epoc32\release\wins\urel\z\system\opx\TOPX.OPX"
	-@erase "..\..\epoc32\build\oplr\winsd\urel\TOPX.OBJ"
	-@erase "..\..\epoc32\release\wins\urel\TOPX.lib"
	-@erase "..\..\epoc32\release\wins\urel\TOPX.exp"
	-@erase "..\..\epoc32\release\wins\urel\TOPX.pdb"
	-@erase "..\..\epoc32\build\oplr\winsd\urel\TOPX.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /Zp4 /MT /W4 /Ox /Og /Os /Oy- /I "..\inc" /I "\epoc32\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /c
# ADD CPP /nologo /Zp4 /MT /W4 /Ox /Og /Os /Oy- /I "..\inc" /I "\epoc32\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /c
CPP_PROJ=/nologo /Zp4 /MT /W4 /Ox /Og /Os /Oy- /I "..\inc" /I "\epoc32\include"\
 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=\epoc32\build\oplr\winsd\urel/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TOPX.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 \epoc32\release\wins\rel\edbms.lib \epoc32\release\wins\rel\edll.obj \epoc32\release\wins\rel\EUSER.lib \epoc32\release\wins\rel\OPLR.LIB \epoc32\release\wins\rel\fbscli.lib \epoc32\release\wins\rel\ws32.lib /nologo /entry:"_E32Dll" /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib /out:"\oplr\tsrc\TOPX.OPX"
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes /force
# ADD LINK32 \epoc32\release\wins\urel\EDLL.lib \epoc32\release\wins\urel\EUSER.lib \epoc32\release\wins\urel\OPLR.LIB /nologo /entry:"_E32Dll" /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib /out:"\epoc32\release\wins\urel\z\system\opx\TOPX.OPX"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=\epoc32\release\wins\urel\EDLL.lib\
 \epoc32\release\wins\urel\EUSER.lib \epoc32\release\wins\urel\OPLR.LIB /nologo\
 /entry:"_E32Dll" /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/TOPX.pdb" /map:"$(INTDIR)/TOPX.map" /debug /machine:I386\
 /nodefaultlib /out:"$(OUTDIR)/z\system\opx\TOPX.OPX"\
 /implib:"$(OUTDIR)/TOPX.lib" 
LINK32_OBJS= \
	"$(INTDIR)/TOPX.OBJ"

"$(OUTDIR)\TOPX.OPX" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TOPX__0"
# PROP BASE Intermediate_Dir "TOPX__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\epoc32\release\wins\udeb\"
# PROP Intermediate_Dir "\epoc32\build\oplr\winsd\udeb"
# PROP Target_Dir ""
OUTDIR=\epoc32\release\wins\udeb
INTDIR=\epoc32\build\oplr\winsd\udeb

ALL : "$(OUTDIR)\TOPX.OPX" "..\..\epoc32\release\wins\udeb\TOPX.bsc"

CLEAN : 
	-@erase "..\..\epoc32\release\wins\udeb\TOPX.PDB"
	-@erase "..\..\epoc32\build\oplr\winsd\udeb\TOPX.OBJ"
	-@erase "..\..\epoc32\release\wins\udeb\TOPX.IDB"
	-@erase "..\..\epoc32\release\wins\udeb\TOPX.bsc"
	-@erase "..\..\epoc32\build\oplr\winsd\udeb\TOPX.SBR"
	-@erase "..\..\epoc32\release\wins\udeb\z\system\opx\TOPX.OPX"
	-@erase "..\..\epoc32\release\wins\udeb\z\system\opx\TOPX.ILK"
	-@erase "..\..\epoc32\release\wins\udeb\TOPX.lib"
	-@erase "..\..\epoc32\release\wins\udeb\TOPX.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /Zp4 /MTd /W4 /Gm /Zi /Od /Ob1 /I "..\inc" /I "\epoc32\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /Fr /Fd"\epoc32\release\wins\rel/TOPX.PDB" /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /Zi /Od /Ob1 /I "..\inc" /I "\epoc32\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D "__DLL__" /D "__WINS__" /Fr /Fd"\epoc32\release\wins\udeb/TOPX.PDB" /c
CPP_PROJ=/nologo /Zp4 /MTd /W4 /Gm /Zi /Od /Ob1 /I "..\inc" /I\
 "\epoc32\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__VC32__" /D\
 "__DLL__" /D "__WINS__" /Fr"$(INTDIR)/" /Fo"$(INTDIR)/"\
 /Fd"\epoc32\release\wins\udeb/TOPX.PDB" /c 
CPP_OBJS=\epoc32\build\oplr\winsd\udeb/
CPP_SBRS=\epoc32\build\oplr\winsd\udeb/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TOPX.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/TOPX.SBR"

"..\..\epoc32\release\wins\udeb\TOPX.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 \epoc32\release\wins\deb\edbms.lib \epoc32\release\wins\deb\efsrv.lib \epoc32\release\wins\deb\estor.lib \epoc32\release\wins\deb\edll.obj \epoc32\release\wins\deb\EUSER.lib \epoc32\release\wins\deb\OPLR.LIB \epoc32\release\wins\deb\fbscli.lib \epoc32\release\wins\deb\ws32.lib /nologo /entry:"_E32Dll" /subsystem:windows /dll /pdb:"\epoc32\release\wins\rel/TOPX.pdb" /debug /machine:I386 /nodefaultlib /out:"\oplr\tsrc\TOPX.OPX"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 \epoc32\release\wins\udeb\EDLL.lib \epoc32\release\wins\udeb\EUSER.lib \epoc32\release\wins\udeb\OPLR.LIB /nologo /entry:"_E32Dll" /subsystem:windows /dll /debug /machine:I386 /nodefaultlib /out:"\epoc32\release\wins\udeb\z\system\opx\TOPX.OPX"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=\epoc32\release\wins\udeb\EDLL.lib\
 \epoc32\release\wins\udeb\EUSER.lib \epoc32\release\wins\udeb\OPLR.LIB /nologo\
 /entry:"_E32Dll" /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/TOPX.pdb" /debug /machine:I386 /nodefaultlib\
 /out:"$(OUTDIR)/z\system\opx\TOPX.OPX" /implib:"$(OUTDIR)/TOPX.lib" 
LINK32_OBJS= \
	"$(INTDIR)/TOPX.OBJ"

"$(OUTDIR)\TOPX.OPX" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "TOPX - Win32 Release"
# Name "TOPX - Win32 Debug"
# Name "TOPX - Win32 Uni Release"
# Name "TOPX - Win32 Uni Debug"

!IF  "$(CFG)" == "TOPX - Win32 Release"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Debug"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Release"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\TOPX.CPP
DEP_CPP_TOPX_=\
	".\TOPX.H"\
	".\..\inc\oplerr.h"\
	"\epoc32\include\e32base.h"\
	".\..\inc\oplapi.h"\
	".\..\inc\opx.h"\
	"\epoc32\include\e32std.h"\
	"\epoc32\include\e32base.inl"\
	"\epoc32\include\e32def.h"\
	"\epoc32\include\e32des8.h"\
	"\epoc32\include\e32des16.h"\
	"\epoc32\include\e32std.inl"\
	

!IF  "$(CFG)" == "TOPX - Win32 Release"


"$(INTDIR)\TOPX.OBJ" : $(SOURCE) $(DEP_CPP_TOPX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TOPX - Win32 Debug"


"$(INTDIR)\TOPX.OBJ" : $(SOURCE) $(DEP_CPP_TOPX_) "$(INTDIR)"

"$(INTDIR)\TOPX.SBR" : $(SOURCE) $(DEP_CPP_TOPX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Release"


"$(INTDIR)\TOPX.OBJ" : $(SOURCE) $(DEP_CPP_TOPX_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Debug"


"$(INTDIR)\TOPX.OBJ" : $(SOURCE) $(DEP_CPP_TOPX_) "$(INTDIR)"

"$(INTDIR)\TOPX.SBR" : $(SOURCE) $(DEP_CPP_TOPX_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TOPX.TXH

!IF  "$(CFG)" == "TOPX - Win32 Release"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Debug"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Release"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TOPX.OPL

!IF  "$(CFG)" == "TOPX - Win32 Release"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Debug"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Release"

!ELSEIF  "$(CFG)" == "TOPX - Win32 Uni Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
