@echo off
set _opl=%1.opl
if exist %1 set _opl=%1
:start
c:\apps\brief\b %_opl%
copy %_opl% test.opl
if exist %2\epoc32\wins\c\oplr\tsrc\test.opo del %2\epoc32\wins\c\oplr\tsrc\test.opo
xcopy %_opl% %2\epoc32\wins\c\oplr\tsrc\test.opl
\epoc32\release\wins\deb\t_tran.exe c:\oplr\tsrc\test.opl
if not exist %2\epoc32\release\wins\deb\z\oplr\nul md %2\epoc32\release\wins\deb\z\oplr
if not exist %2\epoc32\release\wins\deb\z\oplr\tsrc\nul md %2\epoc32\release\wins\deb\z\oplr\tsrc
if exist %2\epoc32\wins\c\oplr\tsrc\test.opo xcopy %2\epoc32\wins\c\oplr\tsrc\test.opo %2\epoc32\release\wins\deb\z\oplr\tsrc\*.*
if exist %2\epoc32\wins\c\oplr\tsrc\test.opo xcopy %2\epoc32\wins\c\oplr\tsrc\test.opo %2\epoc32\release\wins\rel\z\oplr\tsrc\*.*
if not exist %2\epoc32\wins\c\oplr\tsrc\test.opo echo ************ TRANSLATION ERROR *************
echo Control+C quits, other edits %1 again
pause
goto start
