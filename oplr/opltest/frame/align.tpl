
PROC Align:
	global handle%
	LOCAL filename$(255)
	filename$="C:\testIO.bin"
	open:(filename$)
	write:
	close:
	read:
	dinit "Press any key" :dialog
	TRAP delete filename$
ENDP


PROC Open:(filename$)
	local ret%
	ret%=ioopen(handle%,filename$,$302)
ENDP


PROC Write:
	local a$(200)
	local ret%,len%
	a$="hello world"
	ret%=iowrite(handle%,addr(a$)+2,size(a$))
ENDP


PROC Read:
ENDP

PROC Close:
ENDP