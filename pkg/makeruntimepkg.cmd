@rem makeruntimepkg.cmd
@rem Construct the public OPL package for Series 60.
@rem This is the tiny runtime .sis.  

@if not exist \rel mkdir \rel >NUL 2>&1
@if exist \rel\public rmdir \rel\public\/s/q >NUL
@mkdir \rel\public

@call makesis \opl\oplr\pkg\opl-os61.pkg \rel\public\OPL-OS61-030.SIS

@dir \rel\public\
@cd \opl\pkg\
