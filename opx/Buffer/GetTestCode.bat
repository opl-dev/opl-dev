@echo off
rem GetTestCode.bat
rem Converts ER6 OPL format test files from the emulator,
rem then copies them to \opx\ locations

call opltran -conv \epoc32\wins\c\documents\Buffer.oxh -o\opx\Buffer\ -q
call opltran -conv \epoc32\wins\c\documents\DBuffer. -o\opx\Buffer\ -q
