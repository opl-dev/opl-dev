@ECHO OFF
@echo Setting environment for 9210 development.
set EPOCROOT=\Symbian\6.0\NokiaCPP\
set PATH=J:\Symbian\6.0\Shared\epoc32\gcc\bin;J:\Symbian\6.0\Shared\epoc32\tools;%path%
rem  for stdlib.h
set INCLUDE=c:\apps\msvc6\vc98\include

rem for kernel32.lib
set LIB=c:\apps\msvc6\vc98\lib
title 9210 on J:
