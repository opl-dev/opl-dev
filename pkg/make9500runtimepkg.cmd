@rem make9500runtimepkg.cmd
@rem Construct the public OPL package for Nokia 9500.
@rem This is the tiny runtime .sis, demoopl.sis with optional readme.txt file.  

@REM Core OPXs are required by opl.pkg...
call makesis -p RickArjen \projects\opl1.55\opl\opx\pkg\AppFrameOPX_S80.pkg \projects\opl1.55\opl\rel\S80\public\AppFrameOPX_S80.sis
call makesis -p RickArjen \projects\opl1.55\opl\opx\pkg\SendAsOPX_S80.pkg \projects\opl1.55\opl\rel\S80\public\SendAsOPX_S80.sis
call makesis -p RickArjen \projects\opl1.55\opl\opx\pkg\SystemOPX_S80.pkg \projects\opl1.55\opl\rel\S80\public\SystemOPX_S80.sis

@REM dbase opx needed for demoopl
call makesis -p RickArjen \projects\opl1.55\opl\opx\pkg\DBaseOPX_S80.pkg \projects\opl1.55\opl\rel\S80\public\DBaseOPX_S80.sis

call makesis -p RickArjen \projects\opl1.55\opl\oplr\pkg\Opl_S80_v1.55.pkg \projects\opl1.55\opl\rel\S80\public\Opl_S80_v1.55.sis

call makesis -p RickArjen \projects\opl1.55\opl\demoopl\pkg\DemoOPL_S80.pkg \projects\opl1.55\opl\rel\S80\public\DemoOPL_S80.sis

@rem readme file...
copy \projects\opl1.55\opl\pkg\9500-public-readme.txt \projects\opl1.55\opl\rel\S80\public\readme.txt >NUL

@rem And zip em up...
cd \projects\opl1.55\opl\rel\S80\public
call zip Opl_S80_v1.55.zip Opl_S80_v1.55.sis DemoOPL_S80.sis readme.txt
