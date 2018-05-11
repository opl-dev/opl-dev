rem tMenu61.tpl
rem Menu exerciser for Series 60 code running on Symbian OS 6.1

proc tMenu61:
	local m%
	at 1,5
	while 1
		mInit
		mcard "Card1", "item1a",%a,"item1b",%b, "item1c",%c
		mcard "Card2", "item2g",%g,"item2h",%h, "item2i",%i
		mcard "Card3", "item3m",%m,"item3n",%n, "item3o",%o
		m%=MENU
		print "Menu result is",m%
		if m%=0
			break
		endif
	endwh
	print "End of menu test."
endp
