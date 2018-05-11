REM opltests60.tpl
REM ricka@users.sourceforge.net

REM Run any .OPO program in C:\Opltest folder...
REM The .OPO file must have a main procedure with the same name as the .OPO file.
REM e.g. "testweb.opo" must contain PROC TestWeb. 

DECLARE EXTERNAL
CONST KTestDir$="C:\Opltest\"

PROC OPLTestS60:
	LOCAL file$(200),name$(200)
	LOCAL off%(6)

	IF NOT EXIST(KTestDir$)
		MKDIR KTestDir$
	ENDIF

	file$=DIR$(KTestDir$+"*.opo")
	IF file$=""
		PRINT "No .OPO files in "; KTestDir$
	ELSE
		PARSE$(file$,"",off%())
		name$=MID$(file$,off%(4),off%(5)-off%(4))
REM		PRINT "[";name$;"]", len(name$) :GET
		LOADM file$
		@(name$):
		UNLOADM file$
	ENDIF
	PRINT "Done. Press any key."
	GET
ENDP

REM Brought to you with the aid of the "Led Zeppelin I" album - wow!
