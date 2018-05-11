@echo off
@rem Single script to build all OPL
@cd oplt\group
@call bldmake bldfiles
@rem If there are any env var problems, they should have appeared by now...
if "%1"=="" goto :dopause
goto :startbuild
:dopause
@pause
:startbuild
@call abld build
@cd ..\..\opltools\group
@call bldmake bldfiles
copy ..\opltran\opltran.bat %EPOCROOT%\epoc32\tools
@call abld build
@cd ..\..\oplr\group
@call bldmake bldfiles
@call abld build
@cd ..\..\opx\group
@call bldmake bldfiles
@call abld build
@cd ..\..\texted\group
@call bldmake bldfiles
@call abld build
@cd ..\..\demoopl\group
@call bldmake bldfiles
@call abld build
@cd ..\..
