@echo off

rem %1 = path to release folder
rem cleans the created files

del *.log >nul
ECHO == Removing SIS files ==
pushd pkg\thumb
if exist sis rmdir sis /q/s
popd
pushd pkg\wins\udeb
if exist sis rmdir sis /q/s
if exist stub rmdir stub /q/s
popd
pushd pkg\wins\urel
if exist sis rmdir sis /q/s
if exist stub rmdir stub /q/s
popd
if "%1"=="" goto :end
ECHO == Removing copied files ==
if exist %1 rmdir %1 /q/s
ECHO == Done ==
goto :end
:end
