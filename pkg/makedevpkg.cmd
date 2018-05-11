@rem makedevpkg.cmd
@rem Construct the developer OPL package for Series 60 mobile phones.

@REM construct the release framework
@if not exist \rel mkdir \rel >NUL 2>&1
@if exist \rel\dev rmdir \rel\dev\/s/q >NUL 
@mkdir \rel\dev\
@copy \opl\pkg\dev-readme.txt \rel\dev\readme.txt >NUL

@REM Binaries
@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\winc\udeb\ 
@copy %EPOCROOT%epoc32\release\winc\udeb\oplt.dll \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\winc\udeb\ >NUL
@copy %EPOCROOT%epoc32\release\winc\udeb\opltran.exe \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\winc\udeb\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\winc\udeb\z\system\data\
@copy %EPOCROOT%epoc32\release\winc\udeb\z\system\data\opltran.rsc \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\winc\udeb\z\system\data\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\ 
@copy %EPOCROOT%epoc32\release\wins\udeb\opl?.dll \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\apps\opl\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\opl\opl.a?? \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\apps\opl\ >NUL
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\opl\opl*.rsc \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\apps\opl\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\apps\opltests60\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\opltests60\opltests60.a?? \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\apps\opltests60\ >NUL
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\apps\opltests60\opltests60*.rsc \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\apps\opltests60\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\recogs\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\recogs\recopl.rdl \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\recogs\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\opx\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\opx\appframe.opx \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\opx\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\data\
@copy %EPOCROOT%epoc32\release\wins\udeb\z\system\data\oplr.rsc \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\release\wins\udeb\z\system\data\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\tools\
@copy %EPOCROOT%epoc32\tools\opltran.bat \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\tools\ >NUL
@copy %EPOCROOT%epoc32\tools\rsg2osg.* \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\tools\ >NUL
@copy %EPOCROOT%epoc32\tools\hrh2oph\*.* \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\tools\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\shared\epoc32\tools\
@copy \symbian\6.1\shared\epoc32\tools\opltran.bat \rel\dev\binaries\pc\symbian\6.1\shared\epoc32\tools\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\system\opl\
@copy %EPOCROOT%epoc32\wins\c\system\opl\appframe.oxh \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\system\opl\ >NUL
@copy %EPOCROOT%epoc32\wins\c\system\opl\const.oph \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\system\opl\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\opltest\
@copy \opl\opx\appframe\tappframestatus.tpl \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\opltest\ >NUL
@copy \opl\opx\appframe\appframe.txh \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\opltest\ >NUL
@copy \opl\oplr\samplesu\const.tph \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\opltest\ >NUL
@copy \opl\oplr\oplwrapper0\opltests60.tpl \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\opltest\ >NUL
@copy %EPOCROOT%epoc32\wins\c\opltest\*.* \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\opltest\ >NUL

@mkdir \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\system\apps\opltests60\
@copy %EPOCROOT%epoc32\wins\c\system\apps\opltests60\opltests60.opo \rel\dev\binaries\pc\symbian\6.1\series60\epoc32\wins\c\system\apps\opltests60\ >NUL

@REM TARGET - SIS
@mkdir \rel\dev\binaries\sisfiles\target\other\
@copy \rel\public\opl*.sis \rel\dev\binaries\sisfiles\target\other\ > NUL

dir \rel\dev
@echo Now zip up \rel\dev into OPL-OS61-WINS-999.ZIP where 999 is the current version.

@rem Ends