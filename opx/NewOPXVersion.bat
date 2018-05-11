@echo off
rem NewOPXVersion.bat
rem Copyright (c) 2000-2002 Symbian Ltd. All rights reserved.
rem
rem Last updated 20 January 2002

if "%1"=="/?" goto Usage
if "%1"=="" goto Usage
if "%2"=="" goto Usage

echo Checking *.PKG out of Perforce...
p4 edit ...*.pkg
echo Done!
echo Re-versioning *.PKG...
FOR /R %%i IN ("*.pkg") DO perl -S NewOPXVersion.pl "%%i" "%1" "%2" "%3" "%4" "%5" "%6" "%7" "%8" "%9"
if errorlevel==1 goto CheckPerl
echo Done!
goto End

:CheckPerl
perl -v >NUL
if errorlevel==1 echo Is Perl, version 5.003_07 or later, installed?
goto End

:Usage
perl -S NewOPXVersion.pl
goto End

:End