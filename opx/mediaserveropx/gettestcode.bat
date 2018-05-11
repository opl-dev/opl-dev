@echo off
rem GetTestCode.bat
rem converts ER6 OPL format test files from the emulator,
rem then copies them to \opx\ locations

call opltran -conv \epoc32\wins\c\documents\tmediaserver\mediaserveropx.oxh -o\opx\mediaserveropx\ -q
call opltran -conv \epoc32\wins\c\documents\tmediaserver\tmediaserveropx. -o\opx\mediaserveropx\ -q
