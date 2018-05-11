@rem make9210devpkg.cmd
@rem Construct the developer OPL package for Nokia 9210 Communicator series.

@REM construct the release framework
@if not exist \projects\opl1.55\opl\rel mkdir \projects\opl1.55\opl\rel 2>&1
@if exist \projects\opl1.55\opl\rel\dev rmdir \projects\opl1.55\opl\rel\dev\/s/q 
mkdir \projects\opl1.55\opl\rel\dev\
copy \projects\opl1.55\opl\pkg\9500-dev-readme.txt \projects\opl1.55\opl\rel\dev\readme.txt

@REM Binaries
mkdir \projects\opl1.55\opl\rel\dev\binaries\
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\include\
copy %EPOCROOT%epoc32\include\opl*.* \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\include\
copy %EPOCROOT%epoc32\include\opx*.* \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\include\
copy %EPOCROOT%epoc32\include\program.h \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\include\ 
copy %EPOCROOT%epoc32\include\text*.* \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\include\ 

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\marm\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\marm\opx.def \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\marm\
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\opx.def \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\udeb\z\system\data\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\winc\udeb\z\system\data\opltran.rsc \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\udeb\z\system\data\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\winc\udeb\opl*.dll \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\udeb\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\winc\udeb\opl*.exe \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\udeb\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\urel\z\system\data\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\winc\urel\z\system\data\opltran.rsc \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\urel\z\system\data\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\winc\urel\opl*.dll \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\urel\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\winc\urel\opl*.exe \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\winc\urel\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\opl\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\opl\opl.a?? \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\opl\
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\texted\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\texted\texted.a?? \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\texted\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\texted\texted.rsc \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\apps\texted\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\data\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\data\oplr.rsc \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\data\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\install\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\install\*opx.sis \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\install\ 
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\install\opl*.sis \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\install\ 

@rem OPXs
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\opx\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\opx\*.opx \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\opx\ 
@del \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\opx\prntst.opx
@del \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\opx\topx.opx

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\recogs\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\recogs\recopl.rdl \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\z\system\recogs\

copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\opl*.dll \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\opl*.lib \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\udeb\

@rem And the urel...
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\opl\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\opl\opl.a?? \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\opl\
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\texted\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\texted\texted.a?? \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\texted\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\texted\texted.rsc \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\apps\texted\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\data\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\data\oplr.rsc \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\data\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\install\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\install\*opx.sis \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\install\ 
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\install\opl*.sis \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\install\ 

@rem OPXs
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\opx\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\opx\*.opx \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\opx\ 
@del \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\opx\prntst.opx
@del \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\opx\topx.opx

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\recogs\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\z\system\recogs\recopl.rdl \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\z\system\recogs\

copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\opl*.dll \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\
@rem copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\opl*.lib \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\projects\opl1.55\opl\release\wins\urel\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\wins\c\system\apps\demoopl\
copy \projects\opl1.55\opl\demoopl\group\demoopl.mbm \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\wins\c\system\apps\demoopl\

mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\wins\c\system\opl\
copy %EPOCROOT%epoc32\wins\c\system\opl\*.oxh \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\wins\c\system\opl\
copy %EPOCROOT%epoc32\wins\c\system\opl\*.oph \projects\opl1.55\opl\rel\dev\binaries\pc\epoc32\wins\c\system\opl\


@REM TOOLS
mkdir \projects\opl1.55\opl\rel\dev\binaries\pc\tools\
copy \projects\opl1.55\opl\opltools\opltran\opltran.bat \projects\opl1.55\opl\rel\dev\binaries\pc\tools\
copy \projects\opl1.55\opl\opltools\rsg2osg\rsg2osg.* \projects\opl1.55\opl\rel\dev\binaries\pc\tools\
copy \projects\opl1.55\opl\opltools\hrh2oph\*.* \projects\opl1.55\opl\rel\dev\binaries\pc\tools\

@REM PC - SIS
mkdir \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\opx\udeb\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\*opx.sis \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\opx\udeb\
mkdir \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\opx\urel\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\*opx.sis \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\opx\urel\

mkdir \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\other\udeb\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\udeb\demoopl.sis \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\other\udeb\
mkdir \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\other\urel\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\wins\urel\demoopl.sis \projects\opl1.55\opl\rel\dev\binaries\sisfiles\pc\other\urel\

@REM TARGET - SIS
mkdir \projects\opl1.55\opl\rel\dev\binaries\sisfiles\target\opx\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\thumb\urel\*opx.sis \projects\opl1.55\opl\rel\dev\binaries\sisfiles\target\opx\
mkdir \projects\opl1.55\opl\rel\dev\binaries\sisfiles\target\other\
copy %EPOCROOT%epoc32\projects\opl1.55\opl\release\thumb\urel\*opl*.sis \projects\opl1.55\opl\rel\dev\binaries\sisfiles\target\other\

@REM Example source
mkdir \projects\opl1.55\opl\rel\dev\examplesrc\
mkdir \projects\opl1.55\opl\rel\dev\examplesrc\pc\
@rem mkdir \projects\opl1.55\opl\rel\dev\examplesrc\pc\demoopl\
@xcopy \projects\opl1.55\opl\demoopl \projects\opl1.55\opl\rel\dev\examplesrc\pc\demoopl /e/i/q
@xcopy \projects\opl1.55\opl\oplrss \projects\opl1.55\opl\rel\dev\examplesrc\pc\oplrss /e/i/q

mkdir \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\agenda\agenda.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\alarm\alarm.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\array\aarray.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\appframe\appframe.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\bmp\bmp.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\buffer\buffer.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\oplr\samplesu\const.tph \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\contact\contact.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\convert\convert.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\data\data.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\date\date.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\dbase\dbase.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\locale\locale.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\mediaserveropx\mediaserveropx.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\printer\printer.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\scomms\scomms.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\sendas\sendas.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\spell\spell.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\
copy \projects\opl1.55\opl\opx\system\system.txh \projects\opl1.55\opl\rel\dev\examplesrc\pc\system\opl\

mkdir \projects\opl1.55\opl\rel\dev\examplesrc\target\system\opl\
copy %EPOCROOT%epoc32\wins\c\system\opl\*.oxh \projects\opl1.55\opl\rel\dev\examplesrc\target\system\opl\
copy %EPOCROOT%epoc32\wins\c\system\opl\*.oph \projects\opl1.55\opl\rel\dev\examplesrc\target\system\opl\

mkdir \projects\opl1.55\opl\rel\dev\examplesrc\target\system\apps\demoopl\
copy \projects\opl1.55\opl\demoopl\group\demoopl.mbm \projects\opl1.55\opl\rel\dev\examplesrc\target\system\apps\demoopl\

mkdir \projects\opl1.55\opl\rel\dev\examplesrc\target\demoopl\supportfiles\
copy %EPOCROOT%epoc32\wins\c\opl\demoapp\demoopl.hlp.oph \projects\opl1.55\opl\rel\dev\examplesrc\target\demoopl\supportfiles\
copy \projects\opl1.55\opl\demoopl\src\demoicon.mbm \projects\opl1.55\opl\rel\dev\examplesrc\target\demoopl\
copy %EPOCROOT%epoc32\wins\c\opl\demoapp \projects\opl1.55\opl\rel\dev\examplesrc\target\demoopl\


@REM Core OPXs are required by opl.pkg...
@rem call makesis \opl\opx\pkg\appframeopx.pkg %EPOCROOT%epoc32\projects\opl1.55\opl\release\thumb\urel\appframeopx.sis


@rem call makesis \opl\oplr\pkg\opl.pkg %EPOCROOT%epoc32\projects\opl1.55\opl\release\thumb\urel\opl.sis

@rem readme file...
@rem copy \projects\opl1.55\opl\pkg\9500-public-readme.txt %EPOCROOT%epoc32\projects\opl1.55\opl\release\thumb\urel\readme.txt

@rem And zip em up...
@rem cd %EPOCROOT%epoc32\projects\opl1.55\opl\release\thumb\urel\
@rem del public.zip
@rem call pkzip public.zip opl.sis demoopl.sis readme.txt
@rem dir *.zip
@rem cd \projects\opl1.55\opl\pkg\
