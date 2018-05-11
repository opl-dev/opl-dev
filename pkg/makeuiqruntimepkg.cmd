@rem makeuiqruntimepkg.cmd
@rem Construct the public OPL package for UIQ

@if not exist \rel mkdir \rel >NUL 2>&1
@if exist \rel\public rmdir \rel\public\/s/q >NUL
@mkdir \rel\public

@call makesis \opl\oplr\pkg\opl-uiq.pkg \rel\public\OPL-UIQ-054.SIS

@dir \rel\public\
@cd \opl\pkg\


