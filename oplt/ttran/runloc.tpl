INCLUDE "C:\tOPLT\decl.oxh"

CONST  e=27

PROC sw3_1257:
REM bug locating errors in files where
REM there is a procedure which uses
REM %. or %$
 PRINT %.
 PRINT %$
 PRINT 1<2+3*%%%%%
ENDP

PROC x:
  LOCAL a,b,c,d%

  a=b+c/d-e

  beep 1,2
endp

CONST fred=1

PROC y:
  sin(fred)
  PRINT fred+e
  sin(proc2&:(e))
ENDP

PROC z:
  LOCAL x,y%,z$(10)

  proc3&:(x,y%,z$)
  proc4&:(x,y%,z$)
endp


