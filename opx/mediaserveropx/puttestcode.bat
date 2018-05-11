@echo off
rem PutTestCode.bat
rem converts ascii test files from the Opx\Mediaserveropx component to ER6 OPL format,
rem then copies them to C:\Documents\tMediaServer\

if not exist "\EPOC32\wins\c\documents" md "\EPOC32\wins\c\documents"
if not exist "\EPOC32\wins\c\documents\tmediaserver" md "\EPOC32\wins\c\documents\tmediaserver"

call opltran -conv \opx\mediaserveropx\mediaserveropx.txh -o\epoc32\wins\c\documents\tmediaserver\ -q
call opltran -conv \opx\mediaserveropx\tmediaserveropx.tpl -o\epoc32\wins\c\documents\tmediaserver\ -q

copy \opx\mediaserveropx\tplay.wav \epoc32\wins\c\documents\tmediaserver\ >nul