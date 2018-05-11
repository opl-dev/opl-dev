@echo off
rem PutTestCode.bat
rem Converts ASCII test files from the OPX component to ER6 OPL format,
rem then copies them to C:\Documents\ on the emulator.

if not exist "\EPOC32\wins\c\documents" md "\EPOC32\wins\c\documents"

call opltran -conv \opx\Buffer\Buffer.txh -o\epoc32\wins\c\documents\ -q
call opltran -conv \opx\Buffer\DBuffer.tpl -o\epoc32\wins\c\documents\ -q
