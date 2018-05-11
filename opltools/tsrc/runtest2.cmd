@echo off
rem runtest2.cmd - execute the tests for opltools executables.

rem IMPORTANT: need to run this test twice.
rem Once with \epoc32\tools\opltran.bat in place, to test the 'local' configuration.
rem Then delete \epoc32\tools\opltran.bat, to ensure d:\rtools\opltran.bat (or whatever
rem the name of your 'remote' central tool) is picked up instead.
rem And re-run the test.
rem Good luck!

rem Firstly: construct and populate the OPLTRAN test folders.
rem 
rem The test runs from a testdir\ folder, using the following
rem structure:
rem \topltools\tsrc\testdir          -- main test folder
rem                        \subdir   -- include folders
rem \epoc32\winc\opl                 -- the WINC common include folder for OPL

if not exist \topltools\tsrc\testdir\NUL md \topltools\tsrc\testdir\
if not exist \topltools\tsrc\testdir\subdir\NUL md \topltools\tsrc\testdir\subdir\
if not exist \epoc32\winc\NUL md \epoc32\winc\
if not exist \epoc32\winc\opl\NUL md \epoc32\winc\opl\

if not exist \epoc32\wins\NUL              md \epoc32\wins\
if not exist \epoc32\wins\c\NUL            md \epoc32\wins\c\
if not exist \epoc32\wins\c\system\NUL     md \epoc32\wins\c\system\
if not exist \epoc32\wins\c\system\opl\NUL md \epoc32\wins\c\system\opl\

REM ====================================================================================
REM Test 1 - Opltran TOpltran.tpl - convert an ASCII OPL source file to EPOC OPL format.
copy \opl\opltools\tsrc\tOpltran.tpl \topltools\tsrc\testdir\ >NUL

echo Running test 1
call opltran -conv \topltools\tsrc\testdir\tOpltran.tpl -q
if errorlevel==1 goto error
echo Opltran test 1 passed.


REM ====================================================================================
REM Test 2 - Opltran tOpltran.tpl - translate an ASCII OPL source file.
echo Preparing test 2

rem Prepare tLocal1.oph and tLocal2.tph
copy \opl\opltools\tsrc\tlocal?.tph  \topltools\tsrc\testdir\ >NUL
call opltran \topltools\tsrc\testdir\tlocal1.tph -conv -q
if errorlevel==1 goto error
del \topltools\tsrc\testdir\tlocal1.tph 

rem Prepare tInclude1.oph and tInclude2.tph
copy \opl\opltools\tsrc\tInclude?.tph \topltools\tsrc\testdir\subdir\ >NUL
call opltran -conv -q \topltools\tsrc\testdir\subdir\tInclude1.tph
if errorlevel==1 goto error
del \topltools\tsrc\testdir\subdir\tinclude1.tph 

rem Prepare tInclude1.oph and tInclude2.tph
copy \opl\opltools\tsrc\tWinc?.tph \epoc32\winc\opl\ >NUL
call opltran -conv -q \epoc32\winc\opl\twinc1.tph
if errorlevel==1 goto error
del \epoc32\winc\opl\twinc1.tph

rem Prepare tSystem1.oph and tSystem2.tph
copy \opl\opltools\tsrc\tSystem?.tph \epoc32\wins\c\system\opl\ >NUL
call opltran -conv -q \epoc32\wins\c\system\opl\tSystem1.tph
if errorlevel==1 goto error
del \epoc32\wins\c\system\opl\tSystem1.tph

rem And the mbm file...
copy \opl\opltools\tsrc\skelopl.mbm \topltools\tsrc\testdir\skelopl.mbm >NUL
echo Running test 2
call opltran \topltools\tsrc\testdir\tOpltran.tpl -i\topltools\tsrc\testdir\subdir -q
if errorlevel==1 goto error
echo Opltran test 2 passed.


REM ====================================================================================
REM Test 3 -- Opltran tOpltran2 - convert an EPOC OPL source file back to ASCII
copy \topltools\tsrc\testdir\tOpltran \topltools\tsrc\testdir\tOpltran2 >NUL
echo Running test 3
call opltran -conv \topltools\tsrc\testdir\tOpltran2 -q
if errorlevel==1 goto error
echo Opltran test 3 passed.


REM ====================================================================================
REM Test 4 -- an error from OPLTRAN.RSC
copy \opl\opltools\tsrc\tError.tpl \topltools\tsrc\ >NUL
rem Expect an error from this one...
echo Running test 4
call opltran \topltools\tsrc\tError.tpl -q
if errorlevel==1 goto t4passed
echo Opltran test 4 failed
goto error

:t4passed
echo Opltran test 4 passed


REM ====================================================================================
REM Test 5 -- an error from EIKCORE.RSC
copy \opl\opltools\tsrc\tErrInc.tpl \topltools\tsrc\ >NUL
rem Expect an error from this one...
echo Running test 5
call opltran \topltools\tsrc\tErrInc.tpl -q
if errorlevel==1 goto t5passed
echo Opltran test 5 failed
goto error

:t5passed
echo Opltran test 5 passed


goto end
:error
echo Error: Runtest2 test failed.
:end
echo End of Opltran tests