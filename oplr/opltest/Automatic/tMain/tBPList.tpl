REM tBPList.tpl
REM EPOC OPL automatic test code for list functions.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tBPList", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tBPList:
  global prec
	
  prec=1e-6  rem precision : the problem is taking the time to find the 
	rem       true results to compare with opl's results
	
	hRunTest%:("bplist1")
	hRunTest%:("bplist2")

rem	hCleanUp%:("CleanerUpper")
rem	KLog%:(KhLogHigh%,"Some sample text")
endp



proc bplist1:
  rem test the MAX,MIN  functions
  local a,b,c,d
  local e,f,g,h
  local ar(5)
  local i%,a%(5)
  do
		i%=i%+1 :a%(i%)=i% :ar(i%)=i%
  until i%=5
  rem print "Start of BPLIST1"
  if DblNeqG%:(max(3.0,2.0,1.0),3.0,prec) :raise 11 :endif
  a=0.9990 :b=0.9992
  if DblNeqG%:(max(0.9990,a,0.9991,0.9992,0.9993,b),0.9993,prec) :raise 12 :endif
  if max(%A,%B)<>%B :raise 13 :endif
  if max(-1,-2)<>-1 :raise 14 :endif
  if min(1,2,3)<>1 :raise 15 :endif
  if min(3.0,2.0,1.0)<>1.0 :raise 16 :endif
  if min(0.9990,0.9991,0.9992,0.9993)<>0.9990 :raise 17 :endif
	
  if max(min(34,56,78),99,12,33)<>99 :raise 18 :endif
  if min(max(34,56,78),99,12,33)<>12 :raise 19 :endif
  if DblNeqG%:(max(INT(9.9),9.9,FLT(999/10)),99.0,prec) :raise 20 :endif
  a=pi :b=exp(1)
  if max(a,b)<>pi :raise 21 :endif
	
  rem test the sum function
  if DblNeqG%:(sum(1,2,3,4,5),15.,prec) :raise 22 :endif
  if DblNeqG%:(sum(5.1,3.1,1.1,2.1,4.1),15.5,prec) :raise 23 :endif
	
  a=max(12.3,45,7,35,8)
  b=min(99,78.09,0.98,0.002)
  d=sum(a,b,c)
  if DblNeqG%:(d,45.002,prec) :raise 24 :endif
  if DblNeqG%:(sum(a,b,c,d,10*sum(1,4,9)),230.004,prec)  :raise 25 :endif
	
  rem test MEAN,VAR,STD functions
  b=var(1,2,3,4,5,6,7,8,9,10)
  c=std(1,2,3,4,5,6,7,8,9,10)
  if DblNeqG%:(b,9.166666666666667,prec) :raise 26 :endif
  if DblNeqG%:(c,3.0276503541,prec) :raise 27 :endif
  if ABS(b-c*c)>0.00000000001 :raise 28 :endif
	
  if DblNeqG%:(mean(1,2,3,4,5,6,7,8,9,10),5.5,prec) :raise 29 :endif
  if DblNeqG%:(min(1,2,3,4,5,6,7,8,9,10),1.,prec) :raise 30 :endif
  if DblNeqG%:(max(1,2,3,4,5,6,7,8,9,10),10.,prec) :raise 31 :endif
  if DblNeqG%:(sum(1,2,3,4,5,6,7,8,9,10),55.,prec) :raise 32 :endif
	
  if DblNeqG%:(mean(ar(1),ar(2),ar(3),ar(4),ar(5)),3.,prec) :raise 33 :endif
  if DblNeqG%:(std(ar(1),ar(2),ar(3),ar(4),ar(5)),1.58113883009,prec) :raise 34 :endif
  if DblNeqG%:(var(ar(1),ar(2),ar(3),ar(4),ar(5)),2.5,prec) :raise 35 :endif
  if DblNeqG%:(min(ar(1),ar(2),ar(3),ar(4),ar(5)),1.,prec) :raise 36 :endif
  if DblNeqG%:(max(ar(1),ar(2),ar(3),ar(4),ar(5)),5.,prec) :raise 37 :endif
  if DblNeqG%:(sum(ar(1),ar(2),ar(3),ar(4),ar(5)),15.,prec) :raise 38 :endif
	
  if DblNeqG%:(mean(ar(),5),3.,prec) :raise 39 :endif
  if DblNeqG%:(std(ar(),5),1.58113883009,prec) :raise 40 :endif
  if DblNeqG%:(var(ar(),5),2.5,prec) :raise 41 :endif
  if DblNeqG%:(min(ar(),5),1.,prec) :raise 42 :endif
  if DblNeqG%:(max(ar(),5),5.,prec) :raise 43 :endif
  if DblNeqG%:(sum(ar(),5),15.,prec) :raise 44 :endif
	
  if DblNeqG%:(sum(a%(1),a%(2),a%(3)),6.,prec) :raise 45 :endif
  if DblNeqG%:(mean(a%(1),a%(2),a%(3)),2.,prec) :raise 46 :endif
  if DblNeqG%:(min(a%(1),a%(2),a%(3)),1.,prec) :raise 47 :endif
  if DblNeqG%:(max(a%(1),a%(2),a%(3)),3.,prec) :raise 48 :endif
  if DblNeqG%:(var(a%(1),a%(2),a%(3)),1.,prec) :raise 49 :endif
  if DblNeqG%:(std(a%(1),a%(2),a%(3)),1.,prec) :raise 50 :endif
	
  if DblNeqG%:(mean(1,2,3,-1,-2,-3),0.,prec) :raise 51 :endif
  if DblNeqG%:(var(1,2,3,-1,-2,-3),5.6,prec) :raise 52 :endif
  if DblNeqG%:(std(1,2,3,-1,-2,-3),2.36643191324,prec) :raise 53 :endif
	
  rem print "End Of BPLIST1 Test" :pause pause% :key
endp

proc bplist2:
  local i%,j%,a(500),b(600)
  local s,m,v,sd,mn,mx,it
	
  rem test the list functions with arrays as parameter
	
  rem print "Start BPLIST2"
  j%=1
  do
		i%=i%+1 :j%=j%*-1 :a(i%)=j%*i%
  until i%=500
	
  if DblNeqG%:(sum(a(),50),25.,prec) :raise 54 :endif
  if DblNeqG%:(mean(a(),50),0.5,prec) :raise 55 :endif
  if DblNeqG%:(min(a(),50),-49.,prec) :raise 56 :endif
  if DblNeqG%:(max(a(),50),50.,prec) :raise 57 :endif
  if DblNeqG%:(var(a(),50),875.765306122,prec)
  		RAISE 58
		rem print "BPLIST2 - raise 58"
		rem print "expecting 875.765306122 not",var(a(),50)
		rem print "Exit",
	endif
  if DblNeqG%:(std(a(),50),29.5933321227,prec) :raise 59 :endif
	
  if DblNeqG%:(sum(a(),500),250.,prec) :raise 60 :endif
  if DblNeqG%:(mean(a(),500),0.5,prec) :raise 61 :endif
  if DblNeqG%:(min(a(),500),-499.,prec) :raise 62 :endif
  if DblNeqG%:(max(a(),500),500.,prec) :raise 63 :endif
  if DblNeqG%:(var(a(),500),83750.751503,prec) :raise 64 :endif
  if DblNeqG%:(std(a(),500),289.397220967,prec) :raise 65 :endif
	
  i%=30
  if DblNeqG%:(sum(a(),sqr(i%)),-3.,prec) :raise 66 :endif
  if DblNeqG%:(mean(a(),i%*i%/5),0.5,prec) :raise 67 :endif
  if DblNeqG%:(min(a(),i%/30),-1.,prec) :raise 68 :endif
  if DblNeqG%:(max(a(),i%+16),46.,prec) :raise 69 :endif
  if DblNeqG%:(var(a(),i%),325.775862069,prec) :raise 70 :endif
  if DblNeqG%:(std(a(),2*i%/2),18.0492620921,prec) :raise 71 :endif
	
  if sum(b(),600)<>0 :raise 72 :endif
  if mean(b(),600)<>0 :raise 73 :endif
  if min(b(),600)<>0 :raise 74 :endif
  if max(b(),600)<>0 :raise 75 :endif
  if var(b(),600)<>0 :raise 76 :endif
  if std(b(),600)<>0 :raise 77 :endif
	
  Rem check errors
	
rem  print "Check errors"
  print
  onerr e1::
  print sum(b(),2000)
  onerr off
  raise 78
e1::
	if err<>KErrSubs%
		onerr off
		raise err
	endif
  onerr e2::
  print mean(b(),32767)
  onerr off
  raise 79
e2::
	if err<>KErrSubs%
		onerr off
		raise err
	endif
  onerr e3::
  print var(b(),0)
  onerr off
  raise 80
e3::
	if err<>KErrSubs%
		onerr off
		raise err
	endif
	
  onerr e4::
  print std(b(),-1)
  onerr off
  raise 81
e4::
	if err<>KErrSubs%
		onerr off
		raise err
	endif
  onerr e5::
  print min(b(),50000)
  onerr off
  raise 82
e5::
	if err<>KErrOverflow%
		onerr off
		raise err
	endif
  onerr e6::
  print max(b(),-32768)
  onerr off
  raise 83
e6::
	if err<>KErrSubs%
		onerr off
		raise err
	endif
	
  rem print "End Of BPLIST2 Test" :pause pause% :key
endp

proc DblNeqG%:(l,r,prec)
  local dif,res%

  dif=abs(l-r)
rem  print "Dif=";dif
	res%=(dif>=prec)
rem  if res%
rem    pause pause% :key
rem  endif
  return res%
endp


REM End of tBPList.tpl
