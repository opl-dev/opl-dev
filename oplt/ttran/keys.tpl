PROC keys:
	REM All the other keywords
	
	
	APPEND
	AT 1,1
	BACK
	BEEP 1,2
	CLOSE
	CLS
REM	COMPRESS l$,"FRED"
	COPY "FRED",l$
	CREATE "A:\AFILE",A,a%,b&,c,d$
	CURSOR	ON :CURSOR OFF
	DELETE l$
	ERASE
	ESCAPE ON :ESCAPE OFF
	before::
	FIRST
	GOTO before:: :GOTO after::
	INPUT l% :INPUT l& :INPUT l :INPUT l$
	LAST
	LCLOSE
	LOADM "ALIB"
	LOPEN "ADEVICE"
	LPRINT a%,a;a$;
	NEXT
	OFF :OFF 12
	ONERR	before:: :ONERR after:: :ONERR OFF
	OPEN "A:\FILE",B,f1,f2,f3,f4,f5,f65,f7,f8,f9,f10
	PAUSE 267
	POKEB &7,0
	POKEW &50,1
	POKEL &2,&1
	POKEF &0,1.0
	POKE$ &0,"STRING"
	POSITION 2
	PRINT a$
	after::
	RAISE 3
	RANDOMIZE &a5
	RENAME "fred",l$
	RETURN 2:RETURN
	STOP
	UPDATE
	USE C
	UNLOADM l$
	EDIT l$
	SCREEN 2, 20
	OPENR "AFILE",D,f$
	IOSIGNAL
	
	REM traps
	
	TRAP APPEND
	TRAP BACK
	TRAP CLOSE
REM	TRAP COMPRESS l$,"F"
	TRAP COPY l$,"F"
	TRAP CREATE "AFILE",A,A$
	TRAP DELETE "AFILE"
	TRAP ERASE
	TRAP FIRST
	TRAP INPUT l$
	TRAP LAST
	TRAP LCLOSE
	TRAP LOADM "F"
	TRAP LOPEN "D"
	TRAP NEXT
	TRAP OPEN "AFILE",A,A$
	TRAP POSITION 3
	TRAP RENAME "F",l$
	TRAP UPDATE
	TRAP UNLOADM "F"
	TRAP EDIT l$
	TRAP OPENR "AFILE",A,A$
	TRAP USE D
ENDP	
		
PROC testRefs:(pHandle%,pStatus%)
	REM Checks out arguments to functions taking values by reference
	REM Also new # opreator
	
	LOCAL a%,b&,c,d$(1)
	
	IOA(1,2,a%,b&,c)
	IOA(1,2,#pStatus%,#4,#5)
	
	IOW(1,2,d$,a%)
	IOW(1,2,#b&,#c)
	
	IOOPEN(a%,"A file",1)
	IOOPEN(#pHandle%,"A File",1)
		
	IOSEEK(1,1,b&)
	IOSEEK(1,1,#sin(40)*atan(90))    REM silly but it MUST work 
	
	KEYA(#pStatus%,key%)
	KEYA(a%,#phandle%)
	
	KEYC(#pStatus%)
	KEYC(a%)
	ENDP
	
	PROC sepers%:
	REM Check that all functions can now take ; as well as , argument separator
	REM This is a side effect of allowing , for decimal separator in 
	REM calculator
	
	LOCAL a(10)
	
	MEAN(a(),10)
	MEAN(a();10)
		
	MEAN(1,2,3,4,5)
	MEAN(1;2;3;4;5)
	MEAN(1,2;3,4;5)
	
	aProc%:(1,2,3,4,5)
	aProc%:(1;2;3;4;5)
	aProc%:(1;2,3;4,5)
	
	fix$(1.23,3,2)
	fix$(1.23;3;2)
	fix$(1.23,3;2)
	
ENDP