@echo off
echo = Building WINS UDEB SIS files =
echo OPX files
if exist sis rmdir sis /s/q
mkdir sis
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
echo OPX stubs
if exist stub rmdir stub /s/q
mkdir stub
makesis -s -p RickArjen AgendaOPX.pkg stub\AgendaOPX.sis
makesis -s -p RickArjen AlarmOPX.pkg stub\AlarmOPX.sis
makesis -s -p RickArjen ArrayOPX.pkg stub\ArrayOPX.sis
makesis -s -p RickArjen BitmapOPX.pkg stub\BitmapOPX.sis
makesis -s -p RickArjen BufferOPX.pkg stub\BufferOPX.sis
makesis -s -p RickArjen CelltrackOPX.pkg stub\CelltrackOPX.sis
makesis -s -p RickArjen ContactOPX.pkg stub\ContactOPX.sis
makesis -s -p RickArjen ConvertOPX.pkg stub\ConvertOPX.sis
makesis -s -p RickArjen DataOPX.pkg stub\DataOPX.sis
makesis -s -p RickArjen DateOPX.pkg stub\DateOPX.sis
makesis -s -p RickArjen DBaseOPX.pkg stub\DBaseOPX.sis
makesis -s -p RickArjen LocaleOPX.pkg stub\LocaleOPX.sis
makesis -s -p RickArjen MediaServerOPX.pkg stub\MediaServerOPX.sis
makesis -s -p RickArjen PrinterOPX.pkg stub\PrinterOPX.sis
makesis -s -p RickArjen SCommsOPX.pkg stub\SCommsOPX.sis
makesis -s -p RickArjen SpellOPX.pkg stub\SpellOPX.sis
echo OPL runtime stub
makesis -s -p RickArjen Opl.pkg stub\OPL.sis
echo OPL developer package stub
makesis -s -p RickArjen OPLDev.pkg stub\OPLDev.sis
echo DemoOPL application
makesis -p RickArjen DemoOPL.pkg sis\DemoOPL.sis
