@echo off
rem RSG2OSG.BAT -- Unicode build.
rem Copyright (c) 2000-2002 Symbian Ltd. All rights reserved.
rem
rem Last updated 1 January 2002

perl -S rsg2osg.pl %1 %2 %3 %4 %5 %6 %7 %8 %9
if errorlevel==1 goto CheckPerl
goto End

:CheckPerl
perl -v >NUL
if errorlevel==1 echo Is Perl, version 5.003_07 or later, installed?
goto End

:End