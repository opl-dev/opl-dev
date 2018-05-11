@ECHO OFF
@echo Setting environment for 9500 development.
@rem v1.1
set EPOCROOT=\
rem set PATH=z:\epoc32\gcc\bin;z:\epoc32\tools;%path%
rem  for stdlib.h
set INCLUDE=c:\apps\msvc6\vc98\include

rem for kernel32.lib
set LIB=c:\apps\msvc6\vc98\lib
echo Calling vcvars32...
call vcvars32
Title 9500 on Z: