include "system.oxh"

PROC Main:
	print "Self killing OPO"
	pause 40
	SyKillCurrentProcess:(-1234)
ENDP

