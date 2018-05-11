@echo off
if %1?==? goto usage
if %2?==? goto usage
set _frz=-z..\..\epoc32\include\opx.frz
if "%2"=="TOPX" set _uid3=0x10000259
if "%2"=="TESTOPX" set _uid3=1
goto b%1

:bdeb
set _what_=DEB
goto doarm
:brel
set _what_=REL
goto doarm
:budeb
set _what_=UDEB
goto doarm
:burel
set _what_=UREL
:doarm
call eikarm %2 %_frz% -d -p"-uid2 0x1000005d -uid3 %_uid3%" %3 %4 %5 %6 %7 %8
set _what_=
goto end
:usage
echo Build OPX for ARM
echo Arg1=DEB REL etc
echo Arg2=The Opx makefile name (no .MAK)

:end
set _frz=
set _uid3=

