@echo off
rem Must allow .OPL to be added
if exist %1 goto warn
if "%2"=="" goto usage
goto doIt
:warn
echo %1 exists ... .OPL will be added
pause
:doIt
set _first=TRUE
:start
set _opl=%1.OPL
set _srcpath=%2\opl
set _destpath=%2\opltest
set _srcpathE32=c:\opl
c:\apps\brief\b %_opl% \epoc32\include\e32std.h \oplt\inc\opltdef.h
if not exist %_srcpath%\nul md %_srcpath%
if not exist %_destpath%\nul md %_destpath%
copy %_opl% %_srcpath%\*.*
if exist %_srcpath%\%1.opo del %_srcpath%\%1.opo
echo Control+C quits, other translates %1
pause
\epoc32\release\wins\deb\t_tran %_srcpathE32%\%1
if exist %_srcpath%\%1.opo xcopy %_srcpath%\%1.opo %_destpath%\*.*
if not exist %_srcpath%\%1.opo echo ************ TRANSLATION ERROR *************
set _opl=
set _srcpath=
set _destpath=
set _srcpathE32=
echo Control+C quits, other edits %1 again
pause
goto start
:usage
echo Copies source to E32's c:\opl
echo Copies OPO to c:\opltest
echo Leaves APPs in APP\arg1 directory
echo .
echo Usage: Arg1 = Source name (or *) without .OPL extension which is added here
echo        Arg2 = E32 c: path or its subst (eg. \epoc32\wins\c  or o:)

