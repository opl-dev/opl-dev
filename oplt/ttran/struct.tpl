PROC ifs%:
	
	IF 0        REM Basic structure
		PI
	ENDIF
	
	IF 0
		PI
	ELSE
		PI
	ENDIF
	
	IF 0
		PI
	ELSEIF 1
		PI
	ELSEIF 2
		PI
	ELSE
		PI
	ENDIF
	
	IF 0        REM nesting
		PI
		IF 1
			RND
		ELSEIF 2
			RND
		ELSE
			RND
		ENDIF
	ELSEIF 2
		PI
	ELSE
		IF 3
			RND
		ENDIF
	ENDIF
	
ENDP
	
PROC  dos%:
	
	DO              REM Basic structure
		PI
	UNTIL 0
	
	DO              REM Nesting
		PI
		DO
			RND
		UNTIL 1
	UNTIL 0
	
	DO              REM Break/Continue
		PI
		BREAK
		DO
			CONTINUE
			RND
			BREAK
		UNTIL 1
		CONTINUE
	UNTIL 0
ENDP
		
	
PROC whiles%:
		
	WHILE 0             REM Basic structure
		PI
	ENDWH
	
	WHILE 0             REM Nesting
		PI
		WHILE 1
			RND
		ENDWH
		PI
	ENDWH
	
	WHILE  0            REM Break and continue
		PI
		BREAK
		WHILE 1
			CONTINUE
			RND
			BREAK
		ENDWH
		CONTINUE
		PI
	ENDWH
ENDP
	
PROC mixed%:
	REM a mixture of do while and ifs
	
	
	IF	0
		DO
			IF 0
				PI
				CONTINUE
			ELSE
				RND
			ENDIF
		UNTIL GET
	ELSEIF 1
		WHILE 3
			DO
				RND
			UNTIL GET
			BREAK
		ENDWH
	ELSE
		DO
			BREAK
			WHILE 3
				PI
				IF 2
					RND
					CONTINUE
				ENDIF
				PI
			ENDWH
			GET
		UNTIL 0
		RND
	ENDIF
ENDP
