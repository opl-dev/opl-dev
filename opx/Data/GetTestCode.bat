@echo off
rem GetTestCode.bat
rem Converts ER6 OPL format test files from the emulator,
rem then copies them to \opx\ locations

call opltran -conv \epoc32\wins\c\documents\Data.oxh -o\opx\data\ -q
call opltran -conv \epoc32\wins\c\documents\DData. -o\opx\data\ -q
