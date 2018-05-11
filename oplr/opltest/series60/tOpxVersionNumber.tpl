REM tOpxVersionNumber.tpl
REM Demonstrates how to check OPX version number.
REM For use if you require a particular version of an OPX in your OPL program...

INCLUDE "Const.oph"
INCLUDE "System.oxh"

DECLARE EXTERNAL
EXTERNAL IsOpxValid%:

PROC Main:
	PRINT "tOpxVersionNumber"
	IF NOT IsOpxValid%:
	  PRINT "The OPX is not the correct version."
	  PRINT "Please install version 1.52 or later."
	ENDIF
	PRINT "Done. Press any key."
	GET
ENDP


CONST KOpxName$="\System\OPX\Mediaserveropx.opx"
REM v1.52 = &0152
CONST KRequiredVersion&=&0152

PROC IsOpxValid%:
	LOCAL drive$(2),found%
	LOCAL version&
	drive$="D:"
	IF EXIST(drive$+KOpxName$)
		found%=KTrue%
	ELSE
		drive$="C:"
		IF EXIST(drive$+KOpxName$)
			found%=KTrue%
		ENDIF
	ENDIF
	IF NOT found%
		RETURN KFalse%
	ENDIF
	version&=SyGetOpxVersion&:(drive$+KOpxName$)
	REM PRINT drive$+KMediaServerOpxName$
	REM PRINT version&
	IF version&<KRequiredVersion&
		RETURN KFalse%
	ENDIF
	RETURN KTrue%
ENDP

