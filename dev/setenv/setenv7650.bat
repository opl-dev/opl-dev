@ECHO OFF
@echo Setting environment for 7650 development.
set EPOCROOT=\Symbian\6.1\Series60\
set PATH=s:\Symbian\6.1\Shared\epoc32\gcc\bin;s:\Symbian\6.1\Shared\epoc32\tools;%path%
rem  for stdlib.h
set INCLUDE=c:\apps\msvc6\vc98\include

rem for kernel32.lib
set LIB=c:\apps\msvc6\vc98\lib
title 7650 on S:
