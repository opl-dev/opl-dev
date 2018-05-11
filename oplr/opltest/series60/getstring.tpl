proc getstring:
	local a$(20)
	at 1,5
	while 1
		print "Enter string ('a' to exit)"
		INPUT a$
		print a$
		if a$="a"
			break
		endif
	endwh
endp
