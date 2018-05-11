REM SW-60. Think I've fixed this but the following panics
REM from some FBSERV madness.
CONST KAppId%=333
APP xxx,KAppId%
ENDA

rem blank line in OPX

declare OPX ha25,&1,$10
  proc1:(x%) : 2001

end declare

declare OPX ha26,&1,$10
  proc2: : 10
end declare

INCLUDE "incha27.oph"

proc oplha4:
  local i%

  while 0
    if i%=10
      return
    endif
  endwh
endp

proc oplha17:
  LOCAL x% : REM 
ENDP

proc HA350:
  print 1.<>%b
endp

const KSW263%=-4
const KSW263=-1.345
proc bugs%:
  PRINT KSW263%+KSW263
endp

REM ADD CODE HERE

REM The next two  procedures must be at the end of the file
proc p2:
  return
endp

proc p3:
endp
REM DO NOT ADD CODE HERE
