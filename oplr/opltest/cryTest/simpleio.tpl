INCLUDE "Const.oph"

DECLARE EXTERNAL

EXTERNAL doOpen:(aMode%)
EXTERNAL doWrite:
EXTERNAL doRead:
EXTERNAL doClose:

CONST KFilename$="C:\testIO.bin"
CONST KGreeting$="Hello world"
CONST KWrite%=KTrue%
CONST KRead%=KFalse%

PROC Main:
	GLOBAL gHandle%
	doOpen:(KWrite%)
	doWrite:
	doClose:
	doOpen:(KRead%)
	doRead:
	doClose:
	rem TRAP DELETE KFilename$
ENDP

PROC doOpen:(aMode%)
	EXTERNAL gHandle%
	LOCAL ret%,mode%
	LOCAL name$(255)
	
	name$=KFilename$
	IF aMode%=KWrite%
		mode%=KIoOpenModeReplace% OR KIoOpenAccessUpdate%
	ELSE
		mode%=KIoOpenModeOpen% OR KIoOpenAccessShare%
	ENDIF
	mode%=mode% OR KIoOpenFormatBinary%
	ret%=IOOPEN(gHandle%,name$,mode%)
	IF ret%<0 :RAISE ret% :ENDIF
ENDP

PROC doWrite:
	EXTERNAL gHandle%
	LOCAL ret%,addr&,len%
	LOCAL buffer$(200)
	addr&=ADDR(buffer$)
	buffer$=KGreeting$
	len%=LEN(buffer$)
	ret%=IOWRITE(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
ENDP


PROC doRead:
	EXTERNAL gHandle%
	LOCAL ret%,addr&,len%
	LOCAL buffer$(200)
	addr&=ADDR(buffer$)
	buffer$=KGreeting$
	len%=LEN(buffer$)
	ret%=IOREAD(gHandle%,addr&,len%)
	IF ret%<0 :RAISE ret% :ENDIF
ENDP

PROC doClose:
	EXTERNAL gHandle%
	LOCAL ret%
	ret%=IOCLOSE(gHandle%)
	IF ret%<0 :RAISE ret% :ENDIF
ENDP
