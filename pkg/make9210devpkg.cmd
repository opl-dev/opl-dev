@rem make9210devpkg.cmd
@rem Construct the developer OPL package for Nokia 9210 Communicator series.

@REM construct the release framework
@if not exist \rel mkdir \rel >NUL 2>&1
@if exist \rel\dev rmdir \rel\dev\/s/q >NUL 
@mkdir \rel\dev\
@copy \opl\pkg\9210-dev-readme.txt \rel\dev\readme.txt >NUL

@REM Binaries
@mkdir \rel\dev\binaries\
@mkdir \rel\dev\binaries\pc\epoc32\include\
@copy %EPOCROOT%epoc32\include\opl*.* \rel\dev\binaries\pc\epoc32\include\ >NUL
@copy %EPOCROOT%epoc32\include\opx*.* \rel\dev\binaries\pc\epoc32\include\ >NUL
@copy %EPOCROOT%epoc32\include\program.h \rel\dev\binaries\pc\epoc32\include\ >NUL 
@copy %EPOCROOT%epoc32\include\text*.* \rel\dev\binaries\pc\epoc32\include\  >NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\marm\
@copy %EPOCROOT%epoc32\release\marm\opx.def \rel\dev\binaries\pc\epoc32\release\marm\ >NUL
@mkdir \rel\dev\binaries\pc\epoc32\release\wins\
@copy %EPOCROOT%epoc32\release\wins\opx.def \rel\dev\binaries\pc\epoc32\release\wins\ >NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\winc\udeb\z\system\data\
@copy %EPOCROOT%epoc32\release\winc\udeb\z\system\data\opltran.rsc \rel\dev\binaries\pc\epoc32\release\winc\udeb\z\system\data\ >NUL
@copy %EPOCROOT%epoc32\release\winc\udeb\opl*.dll \rel\dev\binaries\pc\epoc32\release\winc\udeb\ >NUL
@copy %EPOCROOT%epoc32\release\winc\udeb\opl*.exe \rel\dev\binaries\pc\epoc32\release\winc\udeb\ >NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\winc\urel\z\system\data\
@copy %EPOCROOT%epoc32\release\winc\urel\z\system\data\opltran.rsc \rel\dev\binaries\pc\epoc32\release\winc\urel\z\system\data\ >NUL
@copy %EPOCROOT%epoc32\release\winc\urel\opl*.dll \rel\dev\binaries\pc\epoc32\release\winc\urel\ >NUL
@copy %EPOCROOT%epoc32\release\winc\urel\opl*.exe \rel\dev\binaries\pc\epoc32\release\winc\urel\ >NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\apps\opl\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\opl\opl.a?? \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\apps\opl\ > NUL
@mkdir \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\apps\texted\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\texted\texted.a?? \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\apps\texted\ > NUL
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\texted\texted.rsc \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\apps\texted\ > NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\data\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\data\oplr.rsc \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\data\ > NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\install\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\install\*opx.sis \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\install\  >NUL
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\install\opl*.sis \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\install\  >NUL

@rem OPXs
@mkdir \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\opx\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\opx\*.opx \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\opx\  >NUL
@del \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\opx\prntst.opx > NUL
@del \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\opx\topx.opx > NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\recogs\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\recogs\recopl.rdl \rel\dev\binaries\pc\epoc32\release\wins\udeb\z\system\recogs\ > NUL

@copy %EPOCROOT%epoc32\release\wins\udeb\opl*.dll \rel\dev\binaries\pc\epoc32\release\wins\udeb\ > NUL
@copy %EPOCROOT%epoc32\release\wins\udeb\opl*.lib \rel\dev\binaries\pc\epoc32\release\wins\udeb\ > NUL

@rem And the urel...
@mkdir \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\apps\opl\
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\apps\opl\opl.a?? \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\apps\opl\ > NUL
@mkdir \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\apps\texted\
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\apps\texted\texted.a?? \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\apps\texted\ > NUL
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\apps\texted\texted.rsc \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\apps\texted\ > NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\data\
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\data\oplr.rsc \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\data\ > NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\install\
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\install\*opx.sis \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\install\  >NUL
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\install\opl*.sis \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\install\  >NUL

@rem OPXs
@mkdir \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\opx\
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\opx\*.opx \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\opx\  >NUL
@del \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\opx\prntst.opx > NUL
@del \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\opx\topx.opx > NUL

@mkdir \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\recogs\
@copy %EPOCROOT%epoc32\release\wins\urel\z\system\recogs\recopl.rdl \rel\dev\binaries\pc\epoc32\release\wins\urel\z\system\recogs\ > NUL

@copy %EPOCROOT%epoc32\release\wins\urel\opl*.dll \rel\dev\binaries\pc\epoc32\release\wins\urel\ > NUL
@rem copy %EPOCROOT%epoc32\release\wins\urel\opl*.lib \rel\dev\binaries\pc\epoc32\release\wins\urel\ > NUL

@mkdir \rel\dev\binaries\pc\epoc32\wins\c\system\apps\demoopl\
@copy \opl\demoopl\group\demoopl.mbm \rel\dev\binaries\pc\epoc32\wins\c\system\apps\demoopl\ > NUL

@mkdir \rel\dev\binaries\pc\epoc32\wins\c\system\opl\
@copy %EPOCROOT%epoc32\wins\c\system\opl\*.oxh \rel\dev\binaries\pc\epoc32\wins\c\system\opl\ > NUL
@copy %EPOCROOT%epoc32\wins\c\system\opl\*.oph \rel\dev\binaries\pc\epoc32\wins\c\system\opl\ > NUL


@REM TOOLS
@mkdir \rel\dev\binaries\pc\tools\
@copy \opl\opltools\opltran\opltran.bat \rel\dev\binaries\pc\tools\ >NUL
@copy \opl\opltools\rsg2osg\rsg2osg.* \rel\dev\binaries\pc\tools\ >NUL
@copy \opl\opltools\hrh2oph\*.* \rel\dev\binaries\pc\tools\ >NUL

@REM PC - SIS
@mkdir \rel\dev\binaries\sisfiles\pc\opx\udeb\
@copy %EPOCROOT%epoc32\release\wins\udeb\*opx.sis \rel\dev\binaries\sisfiles\pc\opx\udeb\ >NUL
@mkdir \rel\dev\binaries\sisfiles\pc\opx\urel\
@copy %EPOCROOT%epoc32\release\wins\urel\*opx.sis \rel\dev\binaries\sisfiles\pc\opx\urel\ >NUL

@mkdir \rel\dev\binaries\sisfiles\pc\other\udeb\
@copy %EPOCROOT%epoc32\release\wins\udeb\demoopl.sis \rel\dev\binaries\sisfiles\pc\other\udeb\ >NUL
@mkdir \rel\dev\binaries\sisfiles\pc\other\urel\
@copy %EPOCROOT%epoc32\release\wins\urel\demoopl.sis \rel\dev\binaries\sisfiles\pc\other\urel\ >NUL

@REM TARGET - SIS
@mkdir \rel\dev\binaries\sisfiles\target\opx\
@copy %EPOCROOT%epoc32\release\thumb\urel\*opx.sis \rel\dev\binaries\sisfiles\target\opx\ > NUL
@mkdir \rel\dev\binaries\sisfiles\target\other\
@copy %EPOCROOT%epoc32\release\thumb\urel\*opl*.sis \rel\dev\binaries\sisfiles\target\other\ > NUL

@REM Example source
@mkdir \rel\dev\examplesrc\
@mkdir \rel\dev\examplesrc\pc\
@rem mkdir \rel\dev\examplesrc\pc\demoopl\
@xcopy \opl\demoopl \rel\dev\examplesrc\pc\demoopl /e/i/q >NUL
@xcopy \opl\oplrss \rel\dev\examplesrc\pc\oplrss /e/i/q >NUL

@mkdir \rel\dev\examplesrc\pc\system\opl\
@copy \opl\opx\agenda\agenda.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\alarm\alarm.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\appframe\appframe.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\bmp\bmp.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\buffer\buffer.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\oplr\samplesu\const.tph \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\contact\contact.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\convert\convert.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\data\data.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\date\date.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\dbase\dbase.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\locale\locale.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\mediaserveropx\mediaserveropx.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\printer\printer.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\scomms\scomms.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\sendas\sendas.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\spell\spell.txh \rel\dev\examplesrc\pc\system\opl\ >NUL
@copy \opl\opx\system\system.txh \rel\dev\examplesrc\pc\system\opl\ >NUL

@mkdir \rel\dev\examplesrc\target\system\opl\
@copy %EPOCROOT%epoc32\wins\c\system\opl\*.oxh \rel\dev\examplesrc\target\system\opl\ > NUL
@copy %EPOCROOT%epoc32\wins\c\system\opl\*.oph \rel\dev\examplesrc\target\system\opl\ > NUL

@mkdir \rel\dev\examplesrc\target\system\apps\demoopl\
@copy \opl\demoopl\group\demoopl.mbm \rel\dev\examplesrc\target\system\apps\demoopl\ > NUL

@mkdir \rel\dev\examplesrc\target\demoopl\supportfiles\
@copy %EPOCROOT%epoc32\wins\c\opl\demoapp\demoopl.hlp.oph \rel\dev\examplesrc\target\demoopl\supportfiles\ > NUL
@copy \opl\demoopl\src\demoicon.mbm \rel\dev\examplesrc\target\demoopl\ > NUL
@copy %EPOCROOT%epoc32\wins\c\opl\demoapp \rel\dev\examplesrc\target\demoopl\ > NUL


@REM Core OPXs are required by opl.pkg...
@rem call makesis \opl\opx\pkg\appframeopx.pkg %EPOCROOT%epoc32\release\thumb\urel\appframeopx.sis


@rem call makesis \opl\oplr\pkg\opl.pkg %EPOCROOT%epoc32\release\thumb\urel\opl.sis

@rem readme file...
@rem copy \opl\pkg\9210-public-readme.txt %EPOCROOT%epoc32\release\thumb\urel\readme.txt

@rem And zip em up...
@rem cd %EPOCROOT%epoc32\release\thumb\urel\
@rem del public.zip
@rem call pkzip public.zip opl.sis demoopl.sis readme.txt
@rem dir *.zip
@rem cd \opl\pkg\
