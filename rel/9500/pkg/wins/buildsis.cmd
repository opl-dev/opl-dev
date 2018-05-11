@echo off
echo = Building WINS SIS files =
echo OPX files
if exist sis rmdir sis /s/q
mkdir sis
makesis -p RickArjen AgendaOPX_S80.pkg sis\AgendaOPX_S80.sis
makesis -p RickArjen AlarmOPX_S80.pkg sis\AlarmOPX_S80.sis
makesis -p RickArjen ArrayOPX_S80.pkg sis\ArrayOPX_S80.sis
makesis -p RickArjen BitmapOPX_S80.pkg sis\BitmapOPX_S80.sis
makesis -p RickArjen BufferOPX_S80.pkg sis\BufferOPX_S80.sis
makesis -p RickArjen ContactOPX_S80.pkg sis\ContactOPX_S80.sis
makesis -p RickArjen ConvertOPX_S80.pkg sis\ConvertOPX_S80.sis
makesis -p RickArjen DataOPX_S80.pkg sis\DataOPX_S80.sis
makesis -p RickArjen DateOPX_S80.pkg sis\DateOPX_S80.sis
makesis -p RickArjen DBaseOPX_S80.pkg sis\DBaseOPX_S80.sis
makesis -p RickArjen LocaleOPX_S80.pkg sis\LocaleOPX_S80.sis
makesis -p RickArjen MediaServerOPX_S80.pkg sis\MediaServerOPX_S80.sis
makesis -p RickArjen PrinterOPX_S80.pkg sis\PrinterOPX_S80.sis
makesis -p RickArjen SCommsOPX_S80.pkg sis\SCommsOPX_S80.sis
echo OPX stubs
if exist stub rmdir stub /s/q
mkdir stub
makesis -s -p RickArjen AgendaOPX_S80.pkg stub\AgendaOPX_S80.sis
makesis -s -p RickArjen AlarmOPX_S80.pkg stub\AlarmOPX_S80.sis
makesis -s -p RickArjen ArrayOPX_S80.pkg stub\ArrayOPX_S80.sis
makesis -s -p RickArjen BitmapOPX_S80.pkg stub\BitmapOPX_S80.sis
makesis -s -p RickArjen BufferOPX_S80.pkg stub\BufferOPX_S80.sis
makesis -s -p RickArjen ContactOPX_S80.pkg stub\ContactOPX_S80.sis
makesis -s -p RickArjen ConvertOPX_S80.pkg stub\ConvertOPX_S80.sis
makesis -s -p RickArjen DataOPX_S80.pkg stub\DataOPX_S80.sis
makesis -s -p RickArjen DateOPX_S80.pkg stub\DateOPX_S80.sis
makesis -s -p RickArjen DBaseOPX_S80.pkg stub\DBaseOPX_S80.sis
makesis -s -p RickArjen LocaleOPX_S80.pkg stub\LocaleOPX_S80.sis
makesis -s -p RickArjen MediaServerOPX_S80.pkg stub\MediaServerOPX_S80.sis
makesis -s -p RickArjen PrinterOPX_S80.pkg stub\PrinterOPX_S80.sis
makesis -s -p RickArjen SCommsOPX_S80.pkg stub\SCommsOPX_S80.sis
echo OPL runtime stub
makesis -s -p RickArjen Opl_S80.pkg stub\OPL_S80.sis
echo OPL developer package stub
makesis -s -p RickArjen OPLDev_S80.pkg stub\OPLDev_S80.sis
echo DemoOPL application
makesis -p RickArjen DemoOPL_S80.pkg sis\DemoOPL_S80.sis
