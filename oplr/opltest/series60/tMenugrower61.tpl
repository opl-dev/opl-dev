Rem Dynamic menu grower
Rem Starts small and grows...

CONST KMaxSize%=14
REM a shortcut of 97 works.
CONST KShortcutOffset%=97 rem %a

DECLARE EXTERNAL
EXTERNAL PopulateMenu:(i%)
EXTERNAL pretest:

PROC tMenuGrower61:
	GLOBAL d$(KMaxSize%,20), d%(KMaxSize%)
	LOCAL size%,m%

rem	pretest:

	at 1,7
	size%=2
	print "alpha" :get
	WHILE size%<KMaxSize%
		PopulateMenu:(size%)
		DO
rem			print d$(1),d%(1)
rem			print d$(2),d%(2)
rem			print "beta" :get
			mInit
			mCARD "Title1", d$(1),d%(1),d$(2),d%(2),d$(3),d%(3),d$(4),d%(4),d$(5),d%(5),d$(6),d%(6),d$(7),d%(7),d$(8),d%(8),d$(9),d%(9),d$(10),d%(10),d$(11),d%(11),d$(12),d%(12),d$(13),d%(13),d$(14),d%(14)
rem			mCARD "Title1", d$(1),%a
rem			mCARD "Title1", d$(1),d%(1),d$(2),d%(2)
rem			print "charlie" :get
			m%=MENU
			AT 1,7
			PRINT "Size=";size%
			PRINT "menu=";m%,
			IF m%=0 :PRINT "Cancelled" :BREAK
			ELSEIF (m%>=1 AND m%<=size%) :PRINT "Valid",m%
			ELSE PRINT "Invalid ***",m%,m%-KShortcutOffset%
			ENDIF
		UNTIL 0
		size%=size%+1
	ENDWH
ENDP

PROC PopulateMenu:(aSize%)
	EXTERNAL d$(),d%()
	LOCAL i%
	i%=1
	DO
rem		print "pop",i%,
		d$(i%)="Item"
rem		print "(";d$(i%);")","0x";HEX$(i%),
		d$(i%)=d$(i%)+HEX$(i%)
rem		print "[";d$(i%);"]",

rem		d%(i%)=KShortcutOffset%+i%
		d%(i%)=i%
rem		print "<";d$(i%);">",d%(i%)

		i%=i%+1
	UNTIL i%=aSize%
ENDP



PROC Pretest:
	minit
	mcard "test", "test2",2,"test3",3
	menu
ENDP

REM End
