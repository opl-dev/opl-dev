REM tBPPerc.tpl
REM EPOC OPL automatic test code for %age.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tBPPerc", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tBPPerc:
	hRunTest%:("doPerc")
endp


proc doPerc:
  global prec
  local b%(38)
  local a%,a

  rem percent operator translator test

  prec=1e-10  :rem precision

rem  print "Start OF BPPERC Test"

  rem check operators
  if dblneq%:(1+2.%,1.02) :raise 1 :endif
  if dblneq%:(1.-5.%,.95) :raise 2 :endif
  if dblneq%:(1.*5.%,.05) :raise 3 :endif
  if dblneq%:(1./.05%,2000.) :raise 4 :endif
  if dblneq%:(1.15<15.%,.15) :raise 5 :endif
  if dblneq%:(1.15>15.%,1.) :raise 6 :endif
  if dblneq%:(FLT(%%),37.) :raise 7 :endif

  rem check types
  a%=100+2% :if dblneq%:(FLT(a%),102.) :raise 8 :endif
  a=1.+2% :if dblneq%:(a,1.02) :raise 9 :endif
  a=1+2.% :if dblneq%:(a,1.02) :raise 10 :endif

  rem check associativity/precedence
  a=10.+10.+2% :if dblneq%:(a,20.4) :raise 11 :endif
  a=10.+10.*2% :if dblneq%:(a,10.2) :raise 12 :endif
  a=11.+10.<5% :if ABS(a-1)>0.00000000001 :raise 13 :endif
  a=10.*10.+2% :if dblneq%:(a,102.) :raise 14 :endif
  a=10.*10.*5% :if dblneq%:(a,5.) :raise 15 :endif
  a=10.*10.2<2% :if dblneq%:(a,2.) :raise 16 :endif
  a=10.<10.+2% :if dblneq%:(a,-1.) :raise 17 :endif
  a=10.<10.*5% :if dblneq%:(a,0.) :raise 18 :endif
  a=10.<100<100% :if dblneq%:(a,-0.5) :raise 19 :endif

  rem %A and unary minus
  a=100.+5%-2 :if dblneq%:(a,103.) :raise 20 :endif
  a=165-%A :if dblneq%:(a,100.) :raise 21 :endif
  a=(65+%A)/2<>%A :if dblneq%:(a,0.) :raise 22 :endif
  a%=100+-5% :if dblneq%:(FLT(a%),95.) :raise 23 :endif
  a%=100+NOT 5.% :if dblneq%:(FLT(a%),100.) :raise 24 :endif
  a%=13 :a%=100+a% % :if dblneq%:(FLT(a%),113.) :raise 25 :endif

  a%=0
  while a%<%%
   a%=a%+1 :b%(a%)=a%
  endwh

  if dblneq%:(FLT(b%(%%)),FLT(%%)) :raise 26 :endif
  a%=-3 :if dblneq%:(FLT(b%(-a%)),3.) :raise 27 :endif

  REM test for errors
	onerr a::
  a=100/0% :onerr off :raise 28
a:: onerr off :if err<>-8 :raise err :endif

rem print "End of BPPERC Test"
rem	pause pause% :key

endp


proc dblneq%:(l,r)
  return abs(l-r)>=prec
endp


REMEnd of tBPPerc.tpl
