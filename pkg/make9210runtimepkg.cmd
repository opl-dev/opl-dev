@rem makeruntimepkg.cmd
@rem Construct the public OPL package.
@rem This is the tiny runtime .sis, demoopl.sis with optional readme.txt file.  

@REM Core OPXs are required by opl.pkg...
@call makesis \opl\opx\pkg\appframeopx.pkg %EPOCROOT%epoc32\release\thumb\urel\appframeopx.sis
@call makesis \opl\opx\pkg\sendasopx.pkg %EPOCROOT%epoc32\release\thumb\urel\sendasopx.sis
@call makesis \opl\opx\pkg\systemopx.pkg %EPOCROOT%epoc32\release\thumb\urel\systemopx.sis

@REM dbase opx needed for demoopl
@call makesis \opl\opx\pkg\dbaseopx.pkg %EPOCROOT%epoc32\release\thumb\urel\dbaseopx.sis

@call makesis \opl\oplr\pkg\opl.pkg %EPOCROOT%epoc32\release\thumb\urel\opl.sis
@call makesis -s \opl\oplr\pkg\opl.pkg %EPOCROOT%epoc32\release\wins\udeb\z\system\install\opl.sis
@call makesis -s \opl\oplr\pkg\opl.pkg %EPOCROOT%epoc32\release\wins\urel\z\system\install\opl.sis

@call makesis "\opl\demoopl\rom\demoopl (thumb).pkg" %EPOCROOT%epoc32\release\thumb\urel\demoopl.sis
@call makesis "\opl\demoopl\rom\demoopl (wins udeb).pkg" %EPOCROOT%epoc32\release\wins\udeb\demoopl.sis
@call makesis "\opl\demoopl\rom\demoopl (wins urel).pkg" %EPOCROOT%epoc32\release\wins\urel\demoopl.sis
@call makesis -s "\opl\demoopl\rom\demoopl (wins udeb).pkg" %EPOCROOT%epoc32\release\wins\udeb\z\system\install\demoopl.sis
@call makesis -s "\opl\demoopl\rom\demoopl (wins urel).pkg" %EPOCROOT%epoc32\release\wins\urel\z\system\install\demoopl.sis

@rem readme file...
@copy \opl\pkg\9210-public-readme.txt %EPOCROOT%epoc32\release\thumb\urel\readme.txt >NUL

@rem And zip em up...
@cd %EPOCROOT%epoc32\release\thumb\urel\
@call zip public.zip opl.sis demoopl.sis readme.txt
@rmdir \rel\public/s/q
@mkdir \rel\public
@copy public.zip \rel\public\ > NUL
@dir \rel\public\*.zip
@cd \opl\pkg\
