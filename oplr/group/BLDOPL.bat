@ECHO OFF
rem Build all OPL Components for Crystal
rem
rem Copyright (c) Symbian Ltd. 2000-2002. All Rights Reserved.
rem
rem Version 1.00(011) - Last Updated 21 October 2002.

SET __BLDOPL_VER__=1.00(011)

if "%1"=="/?" goto usage2

rem If no %1 argument, commence building the lot, starting at OPLT.
rem Otherwise, fall through to build_specific...
if "%1"=="" goto oplt

:build_specific
SET __COMP__=%1
SET __RETURN__=done
goto build_comp

:oplt
SET __COMP__=oplt
SET __RETURN__=opltools
goto build_comp

:opltools
SET __COMP__=opltools
SET __RETURN__=oplr
goto build_comp

:oplr
SET __COMP__=oplr
SET __RETURN__=opx
goto build_comp

:opx
SET __COMP__=opx
SET __RETURN__=DemoOPL
goto build_comp

:DemoOPL
SET __COMP__=DemoOPL
SET __RETURN__=texted
goto build_comp

:texted
SET __COMP__=texted
SET __RETURN__=oplrss
goto build_comp

:oplrss
SET __COMP__=oplrss
SET __RETURN__=done
goto build_comp

:build_comp
if "%__COMP__%"=="opx" goto clean_opxs
:build_comp_cleaned_opxs
echo Building %__COMP__%...
cd \%__COMP__%\group
if "%2"=="" goto build_comp_all
if "%3"=="" goto usage
goto build_comp_specified

:build_comp_all
call bldmake bldfiles
call abld reallyclean
:build_comp_specified
call bldmake bldfiles
call abld build %2 %3
echo Building of %__COMP__% completed!
if "%__COMP__%"=="opx" call \%__COMP__%\OPXSIS.BAT
if "%__COMP__%"=="oplr" call \%__COMP__%\rom\buildsis.bat
if "%__COMP__%"=="texted" call \%__COMP__%\rom\buildsis.bat
if "%__COMP__%"=="DemoOPL" call \%__COMP__%\rom\buildsis.bat

goto %__RETURN__%

:usage
echo Error in specified parameters:
:usage2
echo.
echo BLDOPL %__BLDOPL_VER__% - Build OPL components
echo.
echo USAGE
echo -----
echo bldopl			- build all OPL-related components
echo bldopl comp		- build all releases of specified component
echo bldopl comp target var	- build specific release of specified component
echo.
cd\
goto end

rem
rem OPX cleaning only needed whilst newest OPX components aren't part
rem of the latest COAKs (because the old OPXs which are have different
rem names but the same UIDs to may cause problems)
rem
:clean_opxs
echo Cleaning out OPXs...
del /q %EPOCROOT%epoc32\wins\c\system\opl\*.oxh
del /q /s %EPOCROOT%epoc32\release\*.opx
del /q %EPOCROOT%epoc32\release\wins\udeb\z\system\opx\*.*
del /q %EPOCROOT%epoc32\release\wins\urel\z\system\opx\*.*
goto build_comp_cleaned_opxs

:done
rem Finished building - install the COLOUR MBM/AIFs manually for TextEd/Opl:
cd %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\texted\
if not exist texted.aif copy texted.aCL TextEd.aif
cd %EPOCROOT%epoc32\release\wins\urel\z\system\apps\texted\
if not exist texted.aif copy texted.aCL TextEd.aif
cd %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\opl\
if not exist opl.aif copy opl.aCL Opl.aif
cd %EPOCROOT%epoc32\release\wins\urel\z\system\apps\opl\
if not exist opl.aif copy opl.aCL Opl.aif
cd\

:build_all_sis
rem Rebuild SIS files if we've done a complete build (since order of
rem build stops OPL.sis including newest OPX SIS files)
if "%1"=="" echo Component build complete - rebuilding all SIS files
if "%1"=="" call \opx\OPXSIS.BAT
if "%1"=="" call \oplr\rom\buildsis.bat
if "%1"=="" call \texted\rom\buildsis.bat
if "%1"=="" call \DemoOPL\rom\buildsis.bat
if "%1"=="" echo Rebuilding all SIS files complete.

echo.
echo Build complete!
echo.

:end
SET __BLDOPL_VER__=
SET __COMP__=
SET __RETURN__=
