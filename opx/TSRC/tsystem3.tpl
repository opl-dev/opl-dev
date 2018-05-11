include "system.oxh"

PROC Main:
	print "Self terminating OPO"
	pause 40
	SyTerminateCurrentProcess:(-1234)
ENDP
