@echo off
call 9500
if "%1"=="" goto :usage
if "%4"=="clean" goto :doclean
if "%4"=="CLEAN" goto :doclean
if "%3"=="build" goto :dobuild
if "%3"=="BUILD" goto :dobuild
goto :mkrelease

:doclean
copy opl-target.mmpi ..\..\ /y >nul
pushd ..\..
echo cleaning...
call cleanopl >rel\9500\clean.log 2>&1
popd

:dobuild
copy opl-target.mmpi ..\..\ /y >nul
pushd ..\..
echo building...
echo build start at
time /t
call makeopl nopause >rel\9500\build.log 2>&1
echo build finished at
time /t
popd

:mkrelease
echo creating release...
call mk9500release \Symbian\7.0s\S80_DP2_0_SDK\epoc32 \Projects\OPL1.55\opl %1 %2 >release.log 2>&1

echo Zipping developer package >>release.log
pushd %1\DevPack
zip -R 9500-opl-devpack-2006-06-17.zip *.* >nul
move *.zip .. >nul
popd
echo Zipping mini developer package >>release.log
pushd %1\DevMini
zip -R 9500-opl-devmini-2006-06-17.zip *.* >nul
move *.zip .. >nul
popd
echo Zipping user package >>release.log
pushd %1\UserPack
zip -R 9500-opl-public-2006-06-17.zip *.* >nul
move *.zip .. >nul
popd
echo Copying user package sis >>release.log
copy %1\DevMini\sisfiles\other\OPL_S80.SIS %1\9500-opl-public-2006-06-17.sis >nul

echo done.
goto :end
:usage
echo Usage: dorelease version [sis] [build] [clean]

:end
