@ECHO OFF
@echo Setting environment for 7710 development.
set EPOCROOT=\
set PATH=x:\epoc32\gcc\bin;x:\epoc32\tools;%path%
rem  for stdlib.h
set INCLUDE=c:\apps\msvc6\vc98\include

rem for kernel32.lib
set LIB=c:\apps\msvc6\vc98\lib
echo Calling vcvars32...
call vcvars32
Title 7710 on X: