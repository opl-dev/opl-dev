@echo off
if exist sis rmdir sis /s/q >nul
mkdir sis >nul
echo = Building THUMB SIS files =
echo OPX files
makesis -p RickArjen AgendaOPX.pkg sis\AgendaOPX.sis
makesis -p RickArjen AlarmOPX.pkg sis\AlarmOPX.sis
makesis -p RickArjen ArrayOPX.pkg sis\ArrayOPX.sis
makesis -p RickArjen BitmapOPX.pkg sis\BitmapOPX.sis
makesis -p RickArjen BufferOPX.pkg sis\BufferOPX.sis
makesis -p RickArjen CelltrackOPX.pkg sis\CelltrackOPX.sis
makesis -p RickArjen ContactOPX.pkg sis\ContactOPX.sis
makesis -p RickArjen ConvertOPX.pkg sis\ConvertOPX.sis
makesis -p RickArjen DataOPX.pkg sis\DataOPX.sis
makesis -p RickArjen DateOPX.pkg sis\DateOPX.sis
makesis -p RickArjen DBaseOPX.pkg sis\DBaseOPX.sis
makesis -p RickArjen LocaleOPX.pkg sis\LocaleOPX.sis
makesis -p RickArjen MediaServerOPX.pkg sis\MediaServerOPX.sis
makesis -p RickArjen PrinterOPX.pkg sis\PrinterOPX.sis
makesis -p RickArjen SCommsOPX.pkg sis\SCommsOPX.sis
makesis -p RickArjen SpellOPX.pkg sis\SpellOPX.sis
echo OPL runtime
makesis -p RickArjen Opl.pkg sis\OPL.sis
echo OPL developer package
makesis -p RickArjen OPLDev.pkg sis\OPLDev.sis
echo DemoOPL application
makesis -p RickArjen DemoOPL.pkg sis\DemoOPL.sis
