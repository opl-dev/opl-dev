proc getter:
	local a%
	at 1,5
	while 1
		print "Press any key (C to exit)"  
		a%=GET
		print "0x";HEX$(a%)
		if a%=8
			break
		endif
	endwh
endp
