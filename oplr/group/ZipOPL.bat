@ECHO OFF
rem Zip all OPL Components for Crystal
rem
rem Copyright (c) Symbian Ltd. 2000. All Rights Reserved.
rem
rem Version 1.00(003) - Last Updated 14 November 2000.

if "%1%"=="" goto no_rubbish

c:\Apps\WinZip\WZZip.exe -ex -r -P "OPLBins (With Debug Files).zip" @OPLFiles.txt
goto done

:no_rubbish
c:\Apps\WinZip\WZZip.exe -ex -r -P "OPLBins (Minimal).zip" -x*.pdb -x*.ilk -x*.lib -x*.obj -x*.bsc @OPLFiles.txt

:done
echo Zipping of files related to OPL completed.