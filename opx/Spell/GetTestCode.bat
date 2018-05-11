@echo off
rem GetTestCode.bat
rem Converts ER6 OPL format test files from the emulator,
rem then copies them to \opx\ locations

call opltran -conv \epoc32\wins\c\documents\Spell.oxh -o\opx\Spell\ -q
call opltran -conv \epoc32\wins\c\documents\DSpell. -o\opx\Spell\ -q
