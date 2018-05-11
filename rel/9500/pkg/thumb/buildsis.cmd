@echo off
if exist sis rmdir sis /s/q
mkdir sis
echo = Building THUMB SIS files =
echo OPX files
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
echo OPL runtime
makesis -p RickArjen Opl_S80.pkg sis\OPL_S80.sis
echo OPL developer package
makesis -p RickArjen OPLDev_S80.pkg sis\OPLDev_S80.sis
echo DemoOPL application
makesis -p RickArjen DemoOPL_S80.pkg sis\DemoOPL_S80.sis
