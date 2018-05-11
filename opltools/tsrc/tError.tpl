rem tError.tpl
rem An erroneous opl file which forces use of OPLTRAN.RSC error msg.

PROC Main:
	PRINT "Hello world"

rem Missing ENDP here == force error from opltran.rsc file.
rem ENDP
