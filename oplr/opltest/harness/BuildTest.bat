@echo off
REM BuildTest.bat -- Prepare opltest harness for execution.
REM Pulls .tpl files from \opl\oplr\opltest directory into the test 
REM suite tree structure, translating them as it goes.

set _dest=%EPOCROOT%epoc32\wins\c\Opltest
set _src=\opl\oplr\opltest

echo Preparing opl test harness (from %_src%) into %_dest%

if not exist %_dest%\nul md %_dest%

REM Ensure utils.oph exists.
if not exist %_dest%\harness\nul md %_dest%\Harness
call opltran -conv %_src%\harness\hUtils.tph -o%_dest%\harness\ -q



REM Automatic

rem echo Automatic...
if not exist %_dest%\Automatic\nul md %_dest%\Automatic
if not exist %_dest%\Automatic\gMain\nul md %_dest%\Automatic\gMain
if not exist %_dest%\Automatic\pMainA\nul md %_dest%\Automatic\pMainA
if not exist %_dest%\Automatic\xpMainI\nul md %_dest%\Automatic\xpMainI
if not exist %_dest%\Automatic\r5Main\nul md %_dest%\Automatic\r5Main
if not exist %_dest%\Automatic\tMain\nul md %_dest%\Automatic\tMain
if not exist %_dest%\Automatic\xtMainI\nul md %_dest%\Automatic\xtMainI
if not exist %_dest%\Automatic\wMain\nul md %_dest%\Automatic\wMain
if not exist %_dest%\Automatic\xwMainI\nul md %_dest%\Automatic\xwMainI
if not exist %_dest%\Automatic\zzLast\nul md %_dest%\Automatic\zzLast

call opltran -conv %_src%\Automatic\gMain\* -o%_dest%\Automatic\gMain -q
call opltran -i%_dest%\harness\ %_src%\Automatic\gMain\*.tpl -o%_dest%\Automatic\gMain -q
call opltran -conv %_src%\Automatic\pMainA\* -o%_dest%\Automatic\pMainA -q
call opltran -i%_dest%\harness\ %_src%\Automatic\pMainA\*.tpl -o%_dest%\Automatic\pMainA -q 
call opltran -conv %_src%\Automatic\xpMainI\* -o%_dest%\Automatic\xpMainI -q
call opltran -i%_dest%\harness\ %_src%\Automatic\xpMainI\*.tpl -o%_dest%\Automatic\xpMainI -q 
call opltran -conv %_src%\Automatic\r5Main\* -o%_dest%\Automatic\r5Main -q
call opltran -i%_dest%\harness\ %_src%\Automatic\r5Main\*.tpl -o%_dest%\Automatic\r5Main -q 
call opltran -conv %_src%\Automatic\tMain\* -o%_dest%\Automatic\tMain -q
call opltran -i%_dest%\harness\ %_src%\Automatic\tMain\*.tpl -o%_dest%\Automatic\tMain -q 
call opltran -conv %_src%\Automatic\xtMainI\* -o%_dest%\Automatic\xtMainI -q
call opltran -i%_dest%\harness\ %_src%\Automatic\xtMainI\*.tpl -o%_dest%\Automatic\xtMainI -q 
call opltran -conv %_src%\Automatic\wMain\* -o%_dest%\Automatic\wMain -q
call opltran -i%_dest%\harness\ %_src%\Automatic\wMain\*.tpl -o%_dest%\Automatic\wMain -q 
call opltran -conv %_src%\Automatic\xwMainI\* -o%_dest%\Automatic\xwMainI -q
call opltran -i%_dest%\harness\ %_src%\Automatic\xwMainI\*.tpl -o%_dest%\Automatic\xwMainI -q 
call opltran -conv %_src%\Automatic\zzLast\* -o%_dest%\Automatic\zzLast -q
call opltran -i%_dest%\harness\ %_src%\Automatic\zzLast\*.tpl -o%_dest%\Automatic\zzLast -q 

REM Benchmark

rem echo Benchmark...
if not exist %_dest%\Benchmark\nul md %_dest%\Benchmark
call opltran -conv %_src%\Benchmark\* -o%_dest%\Benchmark -q
call opltran -i%_dest%\harness\ %_src%\Benchmark\*.tpl -o%_dest%\Benchmark -q 

REM Data

rem echo Data...
if not exist %_dest%\Data\nul md %_dest%\Data
copy %_src%\Data\*.* %_dest%\Data >NUL

REM harness

rem echo Harness...
call opltran -conv %_src%\harness\hUtils.tpl -o%_dest%\harness -q
call opltran -conv %_src%\harness\OplSysTest.tpl -o%_dest%\harness -q
call opltran -conv %_src%\harness\_InstallhUtils.tpl -o%_dest%\harness -q
call opltran -i%_dest%\harness\ %_src%\harness\*.tpl -o%_dest%\harness -q 

REM harness\tHarn

if not exist %_dest%\harness\tHarn\nul md %_dest%\harness\tHarn
if not exist %_dest%\harness\tHarn\Interactive\nul md %_dest%\harness\tHarn\Interactive
call opltran -conv %_src%\harness\tHarn\Interactive\* -o%_dest%\harness\tHarn\Interactive -q
call opltran -conv %_src%\harness\tHarn\* -o%_dest%\harness\tHarn -q

REM temp harness\useful bag of bits.

if not exist %_dest%\harness\useful\nul md %_dest%\harness\useful
call opltran -conv %_src%\harness\useful\* -o%_dest%\harness\useful -q

REM Interactive

rem echo Interactive...
if not exist %_dest%\Interactive\nul md %_dest%\Interactive
if not exist %_dest%\Interactive\pMainI\nul md %_dest%\Interactive\pMainI
if not exist %_dest%\Interactive\tMain\nul md %_dest%\Interactive\tMain
if not exist %_dest%\Interactive\wMain\nul md %_dest%\Interactive\wMain

call opltran -conv %_src%\Interactive\pMainI\* -o%_dest%\Interactive\pMainI -q
call opltran -i%_dest%\harness\ %_src%\Interactive\pMainI\*.tpl -o%_dest%\Interactive\pMainI -q 
call opltran -conv %_src%\Interactive\tMain\* -o%_dest%\Interactive\tMain -q
call opltran -i%_dest%\harness\ %_src%\Interactive\tMain\*.tpl -o%_dest%\Interactive\tMain -q 
call opltran -conv %_src%\Interactive\wMain\* -o%_dest%\Interactive\wMain -q
call opltran -i%_dest%\harness\ %_src%\Interactive\wMain\*.tpl -o%_dest%\Interactive\wMain -q 

:end
set _dest=
set _src=
