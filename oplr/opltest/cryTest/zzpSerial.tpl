rem does not currently work for Opler1

proc testread:
	local ret%,pbuf%,buf$(255),end%,len%
	print "Testing reading from serial port"
	lopen "TTY:A"
	rem receive at 57600 without handshake
	rsset:(11,0,8,1,0,&04002000)
	pbuf%=addr(buf$)
	do
		rem read max 255 bytes, after leading byte count
		len%=255
		ret%=iow(-1,1,#uadd(pbuf%,1),len%)
		pokeb pbuf%,len%		rem len%=length actually read
										rem including terminator char
		end%=loc(buf$,chr$(26))	rem nonzero for ctrl-z
		if ret%<0 and ret%<>-43
			beep 3,500
			print
			print "Serial read error: ";err$(ret%)
		endif
		if ret%<>-43						rem if received with terminator
			pokeb pbuf%,len%-1		rem remove terminator
			print buf$					rem echo with CRLF
		else
			print buf$;					rem echo without CRLF
		endif
	until end%
	print "End of session" : pause -30 : key
endp

proc rsset:(baud%,parity%,data%,stop%,hand%,term&)
	local frame%,srchar%(6),dummy%,err%
	frame%=data%-5
	if stop%=2 : frame%=frame% or 16 : endif
	if parity% : frame%=frame% or 32 : endif
	srchar%(1)=baud% or (baud%*256)
	srchar%(2)=frame% or (parity%*256)
	srchar%(3)=(hand% and 255) or $1100
	srchar%(4)=$13
	pokel addr(srchar%(5)),term&
	err%=iow(-1,7,srchar%(1),dummy%)
	if err% : raise err% : endif
endp
