@echo off
set s=r:
rem The version for the group
call ..\group\setver.cmd

set _par=%2
if "%_par%"=="" set _par=%_ver%

set _vgroup=oplr
set _vproj=TOPX

if "%1"=="?" goto help
if "%1"=="" goto help
goto %1

:difsrc
call pvdiff @li
goto end

:putsrc
set _par=-M"V%_ver%"
goto dopvcs

:unlock
:lock
:dopvcs
call t:\pvcs\%1.bat %_vgroup% %_vproj% LI %_par%
goto end

:wholock
vlog -bl li.prj
goto end

:listver
vlog -bv li.prj
goto end

:reput
put -l -v%_ver% -M"_" %2
goto end

:check
check li >check.lis
echo Created check listing in check.lis
goto end

:backup
if not exist u:\%_vgroup%\nul md u:\%_vgroup%
if %2?==? goto update
if exist u:\%_vgroup%\%_vproj%.zip del u:\%_vgroup%\bak.zip
pkzip u:\%_vgroup%\%vproj%.zip *.* -wHS -x*.EXE -x*.DMP -x*.LIB -x*.BSC -x*.VCP -x*.EXP -x*.PCH -x*.PDB -x*.DLL -x*.ILK
attrib \%_vgroup%\*.* -A /S
goto end
:update
pkzip u:\%_vgroup%\%_vproj%.zip *.* -i -wHS -x*.EXE -x*.DMP -x*.LIB -x*.BSC -x*.VCP -x*.EXP -x*.PCH -x*.PDB -x*.DLL -x*.ILK
goto end

:help
echo Parameters are:
echo DIFSRC    to difference the product release control files to .\vdiff.lis
echo PUTSRC    to PVCS the product release control files
echo UNLOCK    to unlock the control files from %vcsid%
echo LOCK      to get the lock of the control files to %vcsid%
echo WHOLOCK   to find who has the lock (of li.prj)
echo ------------------------------------------------------------------------
echo LISTVER   to list all logged versions (of li.prj)
echo BACKUP    to backup to U:\%_vgroup% ("mnt backup all" for non-incremental)
echo CHECK     to check that all the files in the group are in a .PRJ file
:end
call r:\oplr\clrver.cmd
set _par=
set _vgroup=
set _vproj=

