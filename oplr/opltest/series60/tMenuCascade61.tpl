rem tMenuCascade61.tpl
rem Menu exerciser for Series 60 code running on Symbian OS 6.1

DECLARE EXTERNAL

proc tMenuCascade61:
	local m%
	mInit
	mCasc "CascTitle1","Casc1a",%a,"Casc1b",%b
	mCasc "CascTitle2","Casc2a",%f,"Casc2b",%g, "Casc2c", %h, "Casc2d", %i 
	mCasc "CascTitle3","Casc3a",%k,"Casc3b",%l, "Casc3c", %m

	mCard "Card1", "Item1",%s, "CascTitle1>",%x, "CascTitle2>",%y, "Item2",%t, "CascTitle3>",%z, "Item3",%u, "Item4",%v
	m%=MENU
	print "Menu result is",m%
	print "End of menu test."
endp

rem		mCasc "Printing","Page Setup",%u,"Print Preview",%w,"Print",%p
rem		mCasc "Find","Find Next",%t,"Replace",%r,"Go to",%g
rem		REM This isn't used.
rem		mCasc "NotUsed", "Dummy5",24, "Dummy6",25
rem		REM This won't get displayed.
rem		mCasc "MissingChevron","Dummy3",22, "Dummy4",23
rem		mCard "File","New",%n,"Open",%o, "MissingChevron",21,"More>",16,"Printing>",17,"Close",%e
rem		mCard "Card1","New",%a,"Open",%b, "More",%c
rem		mCard "Edit","Cut",%x,"Copy",%c,"Paste",%v,"Find>",18
rem		mCasc "Extra>","Dummy1",21,"Dummy2",%d
rem		mCasc "ShortcutCasc","Dummy7",28,"Dummy8",29
rem		mCard "Tests","Extra>>",19, "NoShortcut",26, "ShortcutCasc>",%z