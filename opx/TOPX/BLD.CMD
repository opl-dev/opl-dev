@echo off
if %1?==? goto usage
if %2?==? goto usage
goto b%1

:b
:bdeb
set _what_=Win32 Debug
goto doit
:brel
set _what_=Win32 Release
goto doit
:budeb
set _what_=Win32 Uni Debug
goto doit
:burel
set _what_=Win32 Uni Release
:doit
call eikbld %2 %3 %4 %5 %6 %7 %8 %9
set _what_=
goto end
:usage
echo Build OPX for WINS
echo Arg1=DEB, REL, UREL, UDEB
echo Arg2=The Opx makefile name (no .MAK)

:end



