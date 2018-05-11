PROC words%:
	REM All the word functions
	
	LOCAL l1%,l2&,l3,l4$(255)
	LOCAL a1%(2),a2&(2),a3(2),a4$(2,255)
		
	REM -- WORD functions
	
	l1%=ADDR(l1%)+ADDR(a2&(1))+ADDR(l3)+ADDR(a4$(2))
	l1%=ASC("ONE")+ASC(l4$)
REM	l1%=CALL(l1%)+CALL(1,&2,3.)
	COUNT
	DAY
	l1%=DOW(1,2,3)
	EOF
	EXIST("AFILENAME")
	FIND("A RECORD")
	GET
	ERR
	IOA(1,2,l1%,l2&,l3)
	IOW(1,2,l2&,l3)
	IOOPEN(l1%,"ADEVICE",$2001)
	IOWRITE(l1%,addr(l4$),20)
	IOREAD(l1%,addr(l4$),20)
	IOCLOSE(l1%)
	IOWAIT
	HOUR
	KEY
	LEN("A STRING")
	LOC("MAIN STRING","SUBSIDUARY")
	MINUTE
	MONTH
	PEEKB($c000)
	PEEKW(&c000)
	POS
REM	RECSIZE
	SECOND
REM	USR(l1%,l2&,a1%(1),a2&(2),&ffff)
	YEAR
	WEEK(1,2,3)
	IOSEEK(l1%,$0,l2&)
	KMOD
	KEYA(l1%,l1%)
	KEYC(l1%)
ENDP
	
PROC longs&:
	REM all the long functions
	
	
	DAYS(1,2,3)
	IABS(&7ffffff)
	INT(1.2)
	PEEKL($ffff)
	SPACE
ENDP
	
PROC doubles:
	REM all the double functions
	
	LOCAL a3(2),l3
	
	ABS(2.3)
	ACOS(.1)
	ASIN(.1)
	ATAN(1.)
	COS(90)
	DEG(1)
	EXP(2.0)
	FLT(&2)
	INTF(1.2)
	LN(2.0)
	LOG(3.0)
	PEEKF(&1)
	PI
	RAD(2.0)
	RND
	SIN(2.)
	SQR(2.)
	TAN(2.)
	VAL("1.5")
	l3=MAX(a3(),2)+MAX(1,2,3,4,4)
	l3=MEAN(a3(),2)+MEAN(1,2,3,4,4)
	l3=MIN(a3(),2)+MIN(1,2,3,4,4)
	l3=STD(a3(),2)+STD(1,2,3,4,4)
	l3=SUM(a3(),2)+SUM(1,2,3,4,4)
	l3=VAR(a3(),2)+VAR(1,2,3,4,4)
ENDP
	
PROC strings$:
	REM All the string functions
	
	CHR$(23)
	DATIM$
	DAYNAME$(2)
	DIR$("PATH")
	ERR$(12)
	FIX$(1.4,2,2)
	GEN$(1.2,1)
	GET$
	HEX$(&7fff)
	LEFT$("STRING",2)
	LOWER$("STRING")
	MID$("STRING",1,1)
	MONTH$(2)
	NUM$(1.2,2)
	PEEK$($7fff)
	REPT$("STRING",1)
	RIGHT$("STRING",1)
	SCI$(1.2,1,1)
	UPPER$("string")
REM	USR$(1,2.,3,4,&5)
ENDP
