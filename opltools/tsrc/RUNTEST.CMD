@echo off
rem runtest.cmd - execute the tests for opltools executables.

rem Firstly: construct and populate the OPLTRAN test folders.
rem 
rem The test runs from a testdir\ folder, using the following
rem structure:
rem \opltools\tsrc\testdir          -- main test folder
rem                       \subdir   -- include folders
rem \epoc32\winc\opl                -- the WINC common include folder for OPL

if not exist \opltools\tsrc\testdir\NUL md \opltools\tsrc\testdir\
if not exist \opltools\tsrc\testdir\subdir\NUL md \opltools\tsrc\testdir\subdir\
if not exist \epoc32\winc\NUL md \epoc32\winc\
if not exist \epoc32\winc\opl\NUL md \epoc32\winc\opl\

if not exist \epoc32\wins\NUL              md \epoc32\wins\
if not exist \epoc32\wins\c\NUL            md \epoc32\wins\c\
if not exist \epoc32\wins\c\system\NUL     md \epoc32\wins\c\system\
if not exist \epoc32\wins\c\system\opl\NUL md \epoc32\wins\c\system\opl\

REM Test 1 - Opltran TOpltran.tpl - convert an ASCII OPL source file to EPOC OPL format.
copy \opltools\tsrc\tOpltran.tpl \opltools\tsrc\testdir\ >NUL

echo Running test 1
call opltran -conv \opltools\tsrc\testdir\tOpltran.tpl -q
if errorlevel==1 goto error
echo Opltran test 1 passed.

REM Test 2 - Opltran tOpltran.tpl - translate an ASCII OPL source file.
echo Preparing test 2

rem Prepare tLocal1.oph and tLocal2.tph
copy \opltools\tsrc\tlocal?.tph  \opltools\tsrc\testdir\ >NUL
call opltran \opltools\tsrc\testdir\tlocal1.tph -conv -q
if errorlevel==1 goto error
del \opltools\tsrc\testdir\tlocal1.tph 

rem Prepare tInclude1.oph and tInclude2.tph
copy \opltools\tsrc\tInclude?.tph \opltools\tsrc\testdir\subdir\ >NUL
call opltran -conv -q \opltools\tsrc\testdir\subdir\tInclude1.tph
if errorlevel==1 goto error
del \opltools\tsrc\testdir\subdir\tinclude1.tph 

rem Prepare tInclude1.oph and tInclude2.tph
copy \opltools\tsrc\tWinc?.tph \epoc32\winc\opl\ >NUL
call opltran -conv -q \epoc32\winc\opl\twinc1.tph
if errorlevel==1 goto error
del \epoc32\winc\opl\twinc1.tph

rem Prepare tSystem1.oph and tSystem2.tph
copy \opltools\tsrc\tSystem?.tph \epoc32\wins\c\system\opl\ >NUL
call opltran -conv -q \epoc32\wins\c\system\opl\tSystem1.tph
if errorlevel==1 goto error
del \epoc32\wins\c\system\opl\tSystem1.tph

rem And the mbm file...
copy \opltools\tsrc\skelopl.mbm \opltools\tsrc\testdir\skelopl.mbm >NUL
echo Running test 2
call opltran \opltools\tsrc\testdir\tOpltran.tpl -i\opltools\tsrc\testdir\subdir -q
if errorlevel==1 goto error
echo Opltran test 2 passed.

REM Test 3 -- Opltran tOpltran2 - convert an EPOC OPL source file back to ASCII
copy \opltools\tsrc\testdir\tOpltran \opltools\tsrc\testdir\tOpltran2 >NUL
call opltran -conv \opltools\tsrc\testdir\tOpltran2 -q
if errorlevel==1 goto error
echo Opltran test 3 passed.

REM End of Opltran tests

goto end
:error
echo Error: test failed.
:end
