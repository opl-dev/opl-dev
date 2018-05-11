REM pMenu.tpl
REM EPOC OPL interactive test code for menus.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "const.oph"
INCLUDE "hUtils.oph"

DECLARE EXTERNAL
EXTERNAL pMenu:
EXTERNAL Reset:
EXTERNAL pMenuTest:
EXTERNAL tCasc:
EXTERNAL tCascMenu%:
EXTERNAL tDim:
EXTERNAL tDimMenu%:
EXTERNAL tCheck:
EXTERNAL tCheckboxMenu%:
EXTERNAL tOption:
EXTERNAL tOptionMenu%:
EXTERNAL tCombo:
EXTERNAL tComboMenu%:
EXTERNAL tInvalidShortCuts%:
EXTERNAL tTooWide%:

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pMenu", hThreadIdFromOplDoc&:, KhUserLoggingOnly%)
	REM After standalone completion, control returns here.
	dINIT "Interactive tests complete" :DIALOG
ENDP


PROC pMenu:
	REM Comment the following line to let the harness trap and process
	REM any errors RAISEd by this program.
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("pMenuTest")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
rem	hCleanUp%:("Reset")
ENDP


PROC Reset:
	rem Any clean-up code here.
ENDP


PROC pMenuTest:
	tCasc:
	tDim:
	tCheck:
	tOption:
	tCombo:	
	tInvalidShortCuts%:
	tTooWide%:
ENDP


PROC tCasc:
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	REM Default.
	PRINT :PRINT "Default menu selection -- choose File | New"
	IF tCascMenu%:<>%n :RAISE 1 :ENDIF
	REM This also tested the run-time tests of:
	REM Unused casc ignored.
	REM Shortcut on casc.

	REM Normal.
	PRINT :PRINT "Normal menu selection -- choose File | Open"
	IF tCascMenu%:<>%o :RAISE 2 :ENDIF

	REM Cascade.
	PRINT :PRINT "Cascaded selection -- choose Edit | Find | Replace"
	IF tCascMenu%:<>%r :RAISE 3 :ENDIF

	REM Cascade with extra chevron.
	PRINT :PRINT "Casc with extra > -- choose Tests | Extra> | Dummy2"
	IF tCascMenu%:<>%d :RAISE 4 :ENDIF

	REM Choice with no shortcut.
	PRINT :PRINT "No shortcut -- choose Tests | NoShortcut"
	IF tCascMenu%:<>26 :RAISE 5 :ENDIF


REM Negative tests.

	REM Cascade with missing chevon is not displayed.
	PRINT :PRINT "Casc with missing > -- attempt to choose File | MissingChevron | Dummy3"
	IF tCascMenu%:<>%x :RAISE 6 :ENDIF	

REM Error tests:
REM Now handled by pMenuA.tpl
ENDP


PROC tDim:
	CLS
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	REM Default.
	PRINT :PRINT "Dimmed menu selection -- attempt to choose File | New"
	PRINT "Dimmed cascade selection -- and attempt to choose File | More> | Save"
	PRINT "Exit the test -- finally, choose File | Exit"
	IF tDimMenu%:<>%e :RAISE 20 :ENDIF
ENDP


PROC tCascMenu%:
	LOCAL m%
	mInit
	mCasc "More","Save",%s,"Export",%e,"Import",%i
	mCasc "Printing","Page Setup",%u,"Print Preview",%w,"Print",%p
	mCasc "Find","Find Next",%t,"Replace",%r,"Go to",%g
	REM This isn't used.
	mCasc "NotUsed", "Dummy5",24, "Dummy6",25
	REM This won't get displayed.
	mCasc "MissingChevron","Dummy3",22, "Dummy4",23
	mCard "File","New",%n,"Open",%o, "MissingChevron",21,"More>",16,"Printing>",17,"Close",%e
	mCard "Edit","Cut",%x,"Copy",%c,"Paste",%v,"Find>",18
	mCasc "Extra>","Dummy1",21,"Dummy2",%d
	mCasc "ShortcutCasc","Dummy7",28,"Dummy8",29
	mCard "Tests","Extra>>",19, "NoShortcut",26, "ShortcutCasc>",%z
	m%=MENU
	RETURN m%
ENDP


PROC tDimMenu%:
	LOCAL m%
	mInit
	mCasc "More","Save",%s,"Export",%e OR KMenuDimmed%,"Import",%i
	mCasc "Printing","Page Setup",%g,"Print Preview",%v,"Print",%p OR KMenuDimmed%
	mCasc "Find","Find Next",%n OR KMenuDimmed%,"Replace",%n,"Go to",%t
	mCard "File","New",%n OR KMenuDimmed%,"Open",%o,"More>",16 OR KMenuDimmed%,"Printing>",17,"Close",%e
	mCard "Edit","Cut",%x,"Copy",%c OR KMenuDimmed%,"Paste",%v,"Find>",18
	m%=MENU
	rem print "keycode=$";hex$(m%);"('";chr$(m% and $ff);"')"
	RETURN m%
ENDP


PROC tCheck:
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	REM Check box toggling.
	PRINT :PRINT "Check box -- choose Display | Carriage Returns"
	IF tCheckboxMenu%:<>%c :RAISE 30 :ENDIF
ENDP


PROC tCheckboxMenu%:
	LOCAL m%
	mInit
	mCard "Display","Tabs",%t OR KMenuCheckBox% OR KMenuSymbolOn%,"Carriage Returns",%c OR KMenuCheckBox%OR KMenuSymbolOn%,"Spaces",%s OR KMenuCheckBox%
	mCard "Allow","Para breaks",%b OR KMenuCheckBox% OR KMenuSymbolOn%,"Orphaned lines",%o OR KMenuCheckBox% OR KMenuSymbolOn%,"Page numbers",%n OR KMenuCheckBox% OR KMenuSymbolOn%
	m%=MENU
	rem print "keycode=$";hex$(m%);"('";chr$(m% and $ff);"')"
	RETURN m%
ENDP


PROC tOption:
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	REM Option button list.
	PRINT :PRINT "Option button -- choose Fonts | Roman"
	IF tOptionMenu%:<>%r :RAISE 40 :ENDIF
ENDP


PROC tOptionMenu%:
	LOCAL m%,r%,s%,h%,t%
	s%=%s OR KMenuOptionStart% OR KMenuSymbolOn%
	r%=%r OR KMenuOptionMiddle%
	h%=%h OR KMenuOptionMiddle%
	t%=%t OR KMenuOptionEnd% 
	mInit
	mCard "Font","Swiss",s%,"Roman",r%,"Helvetia",h%,"Times",t%
	m%=menu
	rem print "keycode=$";hex$(m%);"('";chr$(m% and $ff);"')"
	RETURN m%
ENDP


PROC tCombo:
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 
	PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT :PRINT 

	REM Combination menu.
	PRINT :PRINT "Combo menu -- choose Fonts | Helv"
	IF tComboMenu%:<>%h :RAISE 50 :ENDIF
ENDP


PROC tComboMenu%:
	local menu%,s%,r%,h%,g%,m%,i%,b%,u%,t%,a%,d%
	s%=%s OR KMenuOptionStart% OR KMenuSymbolOn% : r%=%r OR KMenuOptionMiddle% : h%=%h OR KMenuOptionEnd%
	g%=%g OR KMenuOptionStart% : m%=%m OR KMenuOptionEnd% OR KMenuSymbolOn%
	i%=%i OR KMenuCheckBox% : b%=%b OR KMenuCheckBox% : u%=%u OR KMenuCheckBox% OR KMenuDimmed%
	t%=%t OR KMenuCheckBox% : d%=%d OR KMenuCheckBox%  OR KMenuSymbolIndeterminate% : a%=%a OR KMenuCheckBox%
	mInit
	mCasc "More","Save",%s,"Export",%e OR KMenuDimmed%,"Import",%i
	mCasc "Find","Find Next",31,"Replace",30,"Go to",29
	mCasc "Font","Swiss",s%,"Roman",r%,"Helvetia",h%
	mCasc "Style","Italic",i%,"Bold",b%,"Underline",u%
	mCard "File","New",%n OR KMenuDimmed%,"Open",%o,"More>",16,"Print",%p,"Close",%e
	mCard "Edit","Cut",%x,"Copy",%c,"Paste",%v,"Find>",18
	mCard "Text","Font>",17,"Big",g%,"Small",m%,"Style>",18
	mCard "Display","Tabs",t%,"Carriage Returns",d%,"Spaces",a% OR KMenuDimmed%
	menu%=menu
	RETURN menu%
ENDP


PROC tInvalidShortCuts%:
	local m%
	
	rem cls
	rem print "Menu with invalid short-cuts should raise an error"	

	onerr toobigincasc
	mInit
	mCasc "More","Save - should not appear",33,"Export",20,"Import",21
	mCard "Card1","More>",16
	menu
	onerr off
	raise 100

toobigincasc::
	onerr off
	if err<>-2 : print err : raise 102 : endif

REM 0 is now a valid shortcut.
rem	onerr toosmallincasc	
rem	minit
rem	mCasc "Printing","Page Setup - should not appear",0,"Print Preview",24,"Print",25
rem	mCard "Card1","Printing>",17
rem	menu
rem	onerr off
rem	raise 103
rem
rem toosmallincasc::
rem	onerr off
rem	if err<>-2 : raise 104 : endif

	onerr toobig
	minit	
	mCard "File","New - should not appear",33,"Open",17,"More>",18,"Printing>",22,"Close",26
	menu
	onerr off
	raise 105

toobig::
	if err<>-2 : raise 106 : endif

REM 0 is now valid.
rem	onerr toosmall	
rem	minit
rem	mCard "Edit","Cut",27,"Copy",28,"Paste",29,"Find - should not appear",0
rem	menu
rem	onerr off
rem	raise 107
rem
rem toosmall::
rem	onerr off
rem	if err<>-2 : raise 108 : endif

	onerr notallowed1
	minit
	mCard "Style","Bold",%b,"Italic",%i,"Underline",%_
	menu
	onerr off
	raise 109

notallowed1::
	if err<>-2 : raise 110 : endif

	onerr notallowed2
	minit
	mCard "Font","Arial",%A or KMenuOptionStart%,"Times",%* or KMenuOptionMiddle%,"Courier",%C or KMenuOptionEnd%
	menu
	onerr off
	raise 111

notallowed2::
	if err<>-2 : raise 112 : endif
	
	onerr someOK1
	mInit
	mCard "Edit","Cut",27,"Copy",28,"Paste",29,"Find",30      rem OK
	mCard "Style","Bold",%b,"Italic",%i,"Underline",%_				rem Invalid
	onerr off
	raise 113

someOK1::
	onerr off
	if err<>-2 : raise 114 : endif

REM 0 is now valid
rem	onerr someOK2
rem	mInit
rem	mCasc "Style","Bold",%b,"Italic",%i,"Underline",%u							rem OK
rem	mCard "Edit","Cut",27,"Copy",28,"Paste",29,"Find",30,"Style>",0  rem Invalid
rem	onerr off
rem	raise 115
rem
rem someOK2::
rem	onerr off
rem	if err<>-2 : raise 116 : endif

	rem print "Errors detected OK"
	rem pause -20	
endp


PROC tTooWide%:
	local m%
	rem print "Test dimmed cascade titles too wide cause errors"
	onerr m1
	mInit
	mCasc "1234567890123456789012345678901234567890","Save",%s,"Export",%e OR KMenuDimmed%,"Import",%i
	mCasc "Printing","Page Setup",%g,"Print Preview",%v,"Print",%p OR KMenuDimmed%
	mCasc "Find","Find Next",%n OR KMenuDimmed%,"Replace",%n,"Go to",%t
	mCard "File","New",%n OR KMenuDimmed%,"Open",%o,"1234567890123456789012345678901234567890>",16 OR KMenuDimmed%,"Printing>",17,"Close",%c
	mCard "Edit","Cut",%x,"Copy",%c OR KMenuDimmed%,"Paste",%v,"Find>",18
	m%=menu
	onerr off
	raise 200

m1::
	onerr off 		
	if err<>-22 : raise 300 : print err : print err$(err)	: endif
	rem	print "Menu cancelled"
	rem	pause -20

	rem print "Test that cascade title too wide causes an error"
	onerr m2
	mInit
	mCasc "1234567890123456789012345678901234567890","Save",1,"Export",2,"Import",3
	mCasc "Printing","Page Setup",4,"Print Preview",5,"Print",6
	mCard "File","New",7,"Open",8,"1234567890123456789012345678901234567890>",18,"Printing>",22,"Close",26
	mCard "Edit","Cut",27,"Copy",28,"Paste",29,"Find",30
	m%=menu
	onerr off
	raise 400

m2::
	onerr off 		
	if err<>-22 : raise 500: print err : print err$(err)	: endif
	rem print "Menu cancelled"
	REM pause -30
endp

REM End of pMenu.tpl
