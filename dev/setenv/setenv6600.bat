@ECHO OFF
@echo Setting environment for 6600 development, using Series60 SDK v2.1
set EPOCROOT=\
set PATH=t:\epoc32\gcc\bin;t:\epoc32\tools;%path%
rem  for stdlib.h
set INCLUDE=c:\apps\msvc6\vc98\include

rem for kernel32.lib
set LIB=c:\apps\msvc6\vc98\lib
echo Calling vcvars32...
call vcvars32
title 6600 on T:
