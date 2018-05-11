@echo off
rem OPLTRAN.BAT -- Unicode build.
rem Copyright (c) 1998-2002 Symbian Ltd. All rights reserved.
rem
rem Last updated 1 January 2002

if not exist %EPOCROOT%epoc32\release\winc\udeb\OplTran.exe goto _help
%EPOCROOT%epoc32\release\winc\udeb\OplTran.exe -e %1 %2 %3 %4 %5 %6 %7 %8 %9
goto _end
:_help
echo Invalid path in environment variable EPOCROOT.
:_end