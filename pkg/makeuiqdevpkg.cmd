rem makeuiqdevpkg.cmd
rem Construct the developer OPL package for UIQ

REM construct the release framework
if not exist \rel mkdir \rel >NUL 2>&1
if exist \rel\dev rmdir \rel\dev\/s/q >NUL
mkdir \rel\dev
copy \opl\pkg\uiq-dev-readme.txt \rel\dev\readme.txt >NUL

REM Binaries
mkdir \rel\dev\binaries\
mkdir \rel\dev\binaries\pc\epoc32\include\
copy %EPOCROOT%epoc32\include\opl*.* \rel\dev\binaries\pc\epoc32\include\ >NUL
copy %EPOCROOT%epoc32\include\opx*.* \rel\dev\binaries\pc\epoc32\include\ >NUL
copy %EPOCROOT%epoc32\include\program.h \rel\dev\binaries\pc\epoc32\include\ >NUL 
copy %EPOCROOT%epoc32\include\text*.* \rel\dev\binaries\pc\epoc32\include\  >NUL

mkdir \rel\dev\binaries\pc\epoc32\release\marm\
copy %EPOCROOT%epoc32\release\marm\opx.def \rel\dev\binaries\pc\epoc32\release\marm\ >NUL
mkdir \rel\dev\binaries\pc\epoc32\release\winscw\
copy %EPOCROOT%epoc32\release\winscw\opx.def \rel\dev\binaries\pc\epoc32\release\winscw\ >NUL

rem No winc for UIQ.

mkdir \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\apps\opl\
copy %EPOCROOT%epoc32\release\winscw\udeb\z\system\apps\opl\opl.a?? \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\apps\opl\ > NUL
mkdir \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\apps\texted\
copy %EPOCROOT%epoc32\release\winscw\udeb\z\system\apps\texted\texted.a?? \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\apps\texted\ > NUL
copy %EPOCROOT%epoc32\release\winscw\udeb\z\system\apps\texted\texted.rsc \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\apps\texted\ > NUL

mkdir \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\data\
copy %EPOCROOT%epoc32\release\winscw\udeb\z\system\data\oplr.rsc \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\data\ > NUL

rem OPXs
mkdir \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\opx\
copy %EPOCROOT%epoc32\release\winscw\udeb\z\system\opx\*.opx \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\opx\  >NUL

mkdir \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\recogs\
copy %EPOCROOT%epoc32\release\winscw\udeb\z\system\recogs\recopl.rdl \rel\dev\binaries\pc\epoc32\release\winscw\udeb\z\system\recogs\ > NUL

copy %EPOCROOT%epoc32\release\winscw\udeb\opl*.dll \rel\dev\binaries\pc\epoc32\release\winscw\udeb\ > NUL
copy %EPOCROOT%epoc32\release\winscw\udeb\opl*.lib \rel\dev\binaries\pc\epoc32\release\winscw\udeb\ > NUL

rem And the urel...
mkdir \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\apps\opl\
copy %EPOCROOT%epoc32\release\winscw\urel\z\system\apps\opl\opl.a?? \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\apps\opl\ > NUL
mkdir \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\apps\texted\
copy %EPOCROOT%epoc32\release\winscw\urel\z\system\apps\texted\texted.a?? \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\apps\texted\ > NUL
copy %EPOCROOT%epoc32\release\winscw\urel\z\system\apps\texted\texted.rsc \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\apps\texted\ > NUL

mkdir \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\data\
copy %EPOCROOT%epoc32\release\winscw\urel\z\system\data\oplr.rsc \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\data\ > NUL

rem OPXs
mkdir \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\opx\
copy %EPOCROOT%epoc32\release\winscw\urel\z\system\opx\*.opx \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\opx\  >NUL

mkdir \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\recogs\
copy %EPOCROOT%epoc32\release\winscw\urel\z\system\recogs\recopl.rdl \rel\dev\binaries\pc\epoc32\release\winscw\urel\z\system\recogs\ > NUL

copy %EPOCROOT%epoc32\release\winscw\urel\opl*.dll \rel\dev\binaries\pc\epoc32\release\winscw\urel\ > NUL
rem copy %EPOCROOT%epoc32\release\winscw\urel\opl*.lib \rel\dev\binaries\pc\epoc32\release\winscw\urel\ > NUL

mkdir \rel\dev\binaries\pc\epoc32\winscw\c\system\apps\demoopl\
copy \opl\demoopl\group\demoopl.mbm \rel\dev\binaries\pc\epoc32\winscw\c\system\apps\demoopl\ > NUL

mkdir \rel\dev\binaries\pc\epoc32\winscw\c\system\opl\
copy %EPOCROOT%epoc32\winscw\c\system\opl\*.oxh \rel\dev\binaries\pc\epoc32\winscw\c\system\opl\ > NUL
copy %EPOCROOT%epoc32\winscw\c\system\opl\*.oph \rel\dev\binaries\pc\epoc32\winscw\c\system\opl\ > NUL

REM TOOLS
mkdir \rel\dev\binaries\pc\tools\
copy \opl\opltools\opltran\opltran.bat \rel\dev\binaries\pc\tools\ >NUL
copy \opl\opltools\rsg2osg\rsg2osg.* \rel\dev\binaries\pc\tools\ >NUL
copy \opl\opltools\hrh2oph\*.* \rel\dev\binaries\pc\tools\ >NUL

REM TARGET - SIS
mkdir \rel\dev\binaries\sisfiles\target\opx\
mkdir \rel\dev\binaries\sisfiles\target\other\

REM Create all-in-one OPX .SIS for UIQ
REM (This needs to change as soon as OPLTRAN for UIQ is available.)
call makesis \opl\opx\pkg\uiqopx.pkg %EPOCROOT%epoc32\release\thumb\urel\OPX-UIQ-054.sis

REM Package up the developer target SIS files.
call makesis \opl\oplr\pkg\opl-uiq.pkg %EPOCROOT%epoc32\release\thumb\urel\OPL-UIQ-054a.sis
call makesis \opl\texted\pkg\texted-uiq.pkg %EPOCROOT%epoc32\release\thumb\urel\Texted-UIQ-054.sis

copy %EPOCROOT%epoc32\release\thumb\urel\*opx*.sis \rel\dev\binaries\sisfiles\target\opx\ > NUL
copy %EPOCROOT%epoc32\release\thumb\urel\*opl*.sis \rel\dev\binaries\sisfiles\target\other\ > NUL
copy %EPOCROOT%epoc32\release\thumb\urel\*texted*.sis \rel\dev\binaries\sisfiles\target\other\ > NUL

REM Example source
mkdir \rel\dev\examplesrc\
mkdir \rel\dev\examplesrc\pc\
rem mkdir \rel\dev\examplesrc\pc\demoopl\
xcopy \opl\demoopl \rel\dev\examplesrc\pc\demoopl /e/i/q >NUL
xcopy \opl\oplrss \rel\dev\examplesrc\pc\oplrss /e/i/q >NUL

mkdir \rel\dev\examplesrc\pc\system\opl\
copy \opl\opx\agenda\agenda.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\alarm\alarm.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\appframe\appframe.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\bmp\bmp.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\buffer\buffer.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\oplr\samplesu\const.tph \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\contact\contact.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\convert\convert.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\data\data.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\date\date.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\dbase\dbase.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\locale\locale.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\mediaserveropx\mediaserveropx.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\printer\printer.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\scomms\scomms.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\sendas\sendas.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\spell\spell.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
copy \opl\opx\system\system.txh \rel\dev\examplesrc\pc\system\opl\ >NUL

mkdir \rel\dev\examplesrc\target\system\opl\
copy %EPOCROOT%epoc32\winscw\c\system\opl\*.oxh \rel\dev\examplesrc\target\system\opl\ > NUL
copy %EPOCROOT%epoc32\winscw\c\system\opl\*.oph \rel\dev\examplesrc\target\system\opl\ > NUL

mkdir \rel\dev\examplesrc\target\system\apps\demoopl\
copy \opl\demoopl\group\demoopl.mbm \rel\dev\examplesrc\target\system\apps\demoopl\ > NUL

mkdir \rel\dev\examplesrc\target\demoopl\supportfiles\
copy %EPOCROOT%epoc32\winscw\c\opl\demoapp\demoopl.hlp.oph \rel\dev\examplesrc\target\demoopl\supportfiles\ > NUL
copy \opl\demoopl\src\demoicon.mbm \rel\dev\examplesrc\target\demoopl\ > NUL
copy %EPOCROOT%epoc32\winscw\c\opl\demoapp \rel\dev\examplesrc\target\demoopl\ > NUL

rem readme file...
rem copy \opl\pkg\-public-readme.txt %EPOCROOT%epoc32\release\thumb\urel\readme.txt

rem And zip em up...
cd %EPOCROOT%epoc32\release\thumb\urel\
del public.zip
echo pkzip public.zip opl.sis demoopl.sis readme.txt
rem dir *.zip

dir \rel\dev\
cd \opl\pkg\


