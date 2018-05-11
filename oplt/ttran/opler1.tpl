rem !!TODO should really be testing apps here...
APP ProtApp,&5a5ac3c3
ENDA

PROC opler1%:
  LOCAL x&(10)

	MODIFY
	INSERT
	CANCEL
	PUT
	DELETE "database"
	DELETE "database","table"
	TRAP MODIFY
	TRAP INSERT
	TRAP CANCEL
	TRAP PUT
	TRAP DELETE "database"
	TRAP DELETE "database","table"

  GETEVENT32  x&()
  GETEVENTA32 x%,x&()
  gColor 1,2,3
  SETFLAGS &1
  SETDOC "FRED"
  DaysToDate &1, y%,m%,d%
  gINFO32 x&()
  IOWAITSTAT32 x&(3)
  COMPACT "fred"
  BEGINTRANS
  COMMITTRANS
  ROLLBACK

	gCircle 1
	gEllipse 1,2
	BookMark
	KillMark 3
	GotoMark 6
  INTRANS
  ERRX$
  GETDOC$

ENDP

DECLARE EXTERNAL
EXTERNAL proc1%:
EXTERNAL proc2&:(arg1)
PROC fred:
  EXTERNAL a%,b&,c,d$
  EXTERNAL aa%(),bb&(),cc(),dd$()

  PRINT a%+b&+c+LEN(d$)
  PRINT aa%(1)+bb&(1)+cc(1)+LEN(dd$(20))
  PRINT proc1%:+proc2&:(c)
ENDP

REM check that function take and return LONG values
REM appropriate to 32-bit OPL
PROC native:
  EXTERNAL l&
  addr(l&)            REM should return namtive
  adjustalloc(1,2,3)  REM return and args should be native
  alloc(1)            REM return and arg should be native
  ioread(1,2,3)       REM arg 2 should be native
  iowrite(1,2,3)       REM arg 2 should be native
  lenalloc(2)         REM arg and return should be native
  peek$(1)          REM arg is native
  peekb(1)          REM arg is native
  peekw(1)          REM arg is native
  peekf(1)          REM arg is native
  peekl(1)          REM arg is native
  realloc(1,2)      REM args & return are native
ENDP


