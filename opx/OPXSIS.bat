@ECHO OFF
rem Build all SIS files for Crystal OPXs
rem Version 1.51(001) - Last Updated 28 February 2004.

SET __OPXSIS_VER__=1.55(001)

if "%1"=="/?" goto usage2

rem If no %1 argument, commence building the lot, starting at Agenda.
rem Otherwise, fall through to build_specific...
rem
rem All environment variables we create are prefixed with OPX
rem (e.g. OPXCOMP and OPXRETURN) to ensure this .BAT doesn't interfere
rem with other ones such as BLDOPL if it is invoked from there.
if "%1"=="" goto Agenda

:build_specific
SET __OPXCOMP__=%1
SET __OPXRETURN__=done
goto build_comp

:Agenda
SET __OPXCOMP__=Agenda
SET __OPXRETURN__=Alarm
goto build_comp

:Alarm
SET __OPXCOMP__=Alarm
SET __OPXRETURN__=AppFrame
goto build_comp

:AppFrame
SET __OPXCOMP__=AppFrame
SET __OPXRETURN__=Array
goto build_comp

:Array
SET __OPXCOMP__=Array
SET __OPXRETURN__=Bmp
goto build_comp

:Bmp
SET __OPXCOMP__=Bmp
SET __OPXRETURN__=Buffer
goto build_comp

:Buffer
SET __OPXCOMP__=Buffer
SET __OPXRETURN__=Celltrack
goto build_comp

:Celltrack
SET __OPXCOMP__=Celltrack
SET __OPXRETURN__=Contact
goto build_comp

:Contact
SET __OPXCOMP__=Contact
SET __OPXRETURN__=Convert
goto build_comp

:Convert
SET __OPXCOMP__=Convert
SET __OPXRETURN__=Data
goto build_comp

:Data
SET __OPXCOMP__=Data
SET __OPXRETURN__=Date
goto build_comp

:Date
SET __OPXCOMP__=Date
SET __OPXRETURN__=DBase
goto build_comp

:DBase
SET __OPXCOMP__=DBase
SET __OPXRETURN__=Locale
goto build_comp

:Locale
SET __OPXCOMP__=Locale
SET __OPXRETURN__=MediaServerOPX
goto build_comp

:MediaServerOPX
SET __OPXCOMP__=MediaServerOPX
SET __OPXRETURN__=Printer
goto build_comp

:Printer
SET __OPXCOMP__=Printer
SET __OPXRETURN__=SComms
goto build_comp

:SComms
SET __OPXCOMP__=SComms
SET __OPXRETURN__=SendAs
goto build_comp

:SendAs
SET __OPXCOMP__=SendAs
SET __OPXRETURN__=Spell
goto build_comp

:Spell
SET __OPXCOMP__=Spell
SET __OPXRETURN__=System
goto build_comp

:System
SET __OPXCOMP__=System
SET __OPXRETURN__=done
goto build_comp

:build_comp
rem echo Building SIS files for %__OPXCOMP__%...
cd \projects\opl1.55\opl\opx\%__OPXCOMP__%\pkg
if "%__OPXCOMP__%"=="Bmp" SET __OPXCOMP__=Bitmap
if "%__OPXCOMP__%"=="MediaServerOPX" SET __OPXCOMP__=MediaServer
SET __OPXTARGET__=
SET __OPXVARIANT__=udeb
if "%2"=="" goto build_comp_all
if "%3"=="" goto usage
SET __OPXTARGET__=%2
SET __OPXVARIANT__=%3
goto build_comp_specified

:build_comp_all
SET __OPXNEXTRETURN__=%__OPXRETURN__%
SET __OPXRETURN__=build_comp_all_return
:build_comp_all_return
if "%__OPXVARIANT__%"=="" SET __OPXRETURN__=%__OPXNEXTRETURN__%
if "%__OPXVARIANT__%"=="" goto %__OPXRETURN__%
if "%__OPXTARGET__%"=="wins" goto check_next_variant
if "%__OPXTARGET__%"=="thumb" SET __OPXTARGET__=wins
rem if "%__OPXTARGET__%"=="armi" SET __OPXTARGET__=thumb
rem if "%__OPXTARGET__%"=="arm4" SET __OPXTARGET__=armi
if "%__OPXTARGET__%"=="" SET __OPXTARGET__=thumb


:build_comp_specified
@rem Skip thumb udeb on Nokia 9200 SDK v1.2
if "%__OPXTARGET__%"=="thumb" goto skip_maybe
goto do_it_now

:skip_maybe
if "%__OPXVARIANT__%"=="udeb" goto %__OPXRETURN__%

:do_it_now
rem echo !! DEBUG INFO: "%__OPXTARGET__% - %__OPXVARIANT__% - %__OPXCOMP__%OPX.pkg"
rem pause

rem Generate real SIS files
if exist "%EPOCROOT%epoc32\release\%__OPXTARGET__%\%__OPXVARIANT__%\%__OPXCOMP__%OPX.sis" del "%EPOCROOT%epoc32\release\%__OPXTARGET__%\%__OPXVARIANT__%\%__OPXCOMP__%OPX.sis"
makesis "%__OPXTARGET__% - %__OPXVARIANT__% - %__OPXCOMP__%OPX.pkg" "%EPOCROOT%epoc32\release\%__OPXTARGET__%\%__OPXVARIANT__%\%__OPXCOMP__%OPX.sis"
rem Generate the stub SIS files for WINS only
if "%__OPXTARGET__%"=="wins" del "%EPOCROOT%epoc32\release\wins\%__OPXVARIANT__%\z\system\install\%__OPXCOMP__%OPX.sis" > NUL 2>&1
if "%__OPXTARGET__%"=="wins" makesis -s "%__OPXTARGET__% - %__OPXVARIANT__% - %__OPXCOMP__%OPX.pkg" "%EPOCROOT%epoc32\release\wins\%__OPXVARIANT__%\z\system\install\%__OPXCOMP__%OPX.sis"

rem echo Building of %__OPXCOMP__% completed.
goto %__OPXRETURN__%

:usage
echo Error in specified parameters:
:usage2
echo.
echo OPXSIS %__OPXSIS_VER__% - Build SIS files for Crystal OPXs
echo.
echo USAGE
echo -----
echo opxsis			- build all SIS variants for all OPXs
echo opxsis opx		- build all SIS variants of specified OPX
echo opxsis comp target var	- build specific SIS variant of specified OPX
echo.
cd\
goto end

:check_next_variant
if "%__OPXVARIANT__%"=="urel" SET __OPXVARIANT__=
if "%__OPXVARIANT__%"=="udeb" SET __OPXVARIANT__=urel
SET __OPXTARGET__=
goto %__OPXRETURN__%

:done
rem Do anything needed after SIS files have been built
cd\opl\opx\

echo.
echo OPX SIS files built.
echo.

:end
SET __OPXSIS_VER__=
SET __OPXCOMP__=
SET __OPXTARGET__=
SET __OPXVARIANT__=
SET __OPXRETURN__=
SET __OPXNEXTRETURN__=
