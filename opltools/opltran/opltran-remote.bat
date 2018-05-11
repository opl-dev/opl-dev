@echo off
rem opltran-remote.bat 
rem Copyright (c) 1998-2002 Symbian Ltd. All rights reserved.
rem
rem Last updated May 2005
rem
rem IMPORTANT: replace "d:\rtools\" with the location of 
rem your tools folder. And rename this to 'opltran.bat'

if not exist d:\rtools\opltran\opltran.exe goto _help
d:\rtools\opltran\opltran.exe -e %1 %2 %3 %4 %5 %6 %7 %8 %9 -epocroot%EPOCROOT%
goto _end
:_help
echo Unable to find OPLTRAN.EXE
:_end