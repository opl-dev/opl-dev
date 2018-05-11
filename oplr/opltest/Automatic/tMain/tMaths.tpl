REM tMaths.tpl
REM EPOC OPL automatic test code for maths operations.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tMaths", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tMaths:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	rem strtest:("Maths")  :rem In T_UTIL.OPL
	hRunTest%:("divZero")
	hRunTest%:("overFlow")
	hRunTest%:("arith")
	hRunTest%:("transcnd")
	hRunTest%:("fErrors")
	rem hCleanUp%:("CleanUp")
	rem KLog%:(KhLogHigh%,"Some sample text")
endp


REM---------------------------------------------------------------------------

proc divZero:
	rem SubTest:("Test Division by Zero")
	onerr intOk
	print 1/0
	onerr off :raise 1
intOk::
	onerr off
	if err<>-8
		raise 2
	endif	
	onerr longOk
	print &00000001/&0
	onerr off :raise 3
longOk::
	onerr off
	if err<>-8
		raise 4
	endif
	onerr dblOk
	print 1.0/0.0
	onerr off :raise 5
dblOk::
	onerr off
	if err<>-8
		raise 6
	endif
endp


proc overFlow:
	local i%,l&,r
	rem SubTest:("Test Integer Overflow and Underflow")
	i%=32767/2*2+1
	if i%<>32767 :raise 1 :endif
	i%=-32766/2*2-2
	if i%<>-32768 :raise 2 :endif

	onerr intOver1
	i%=32767+1	:raise 3
intOver1::
	onerr off
	if err<>-6	:raise 4 :endif
	onerr intOver2
	i%=32767*2 :raise 5
intOver2::
	onerr off
	if err<>-6 :raise 6 :endif
	onerr intOver3
	i%=32767-(-1) :raise 7
intOver3::
	onerr off
	if err<>-6 :raise 8 :endif
	onerr intOver4
	i%=32767**2 : raise 9
intOver4::
	onerr off
	if err<>-6 :raise 10 :endif
	onerr intOver5
	i%=32767+32767 :raise 11
intOver5::
	onerr off
	if err<>-6	:raise 12 :endif
	onerr intOver6
	i%=32767*100 :raise 13
intOver6::
	onerr off
	if err<>-6 :raise 14 :endif
	onerr intOver7
	i%=32767-(-32767) :raise 15
intOver7::
	onerr off
	if err<>-6 :raise 16 :endif

	rem Underflow - large negative integers
	onerr intUnder1
	i%=$8000-1 :raise 17
intUnder1::
	onerr off
	if err<>-6 :raise 18 :endif
	onerr intUnder2
	i%=$8000+(-1) :raise 19
intUnder2::
	onerr off
	if err<>-6 :raise 20 :endif
	onerr intUnder3
	i%=$8000*2 :raise 21
intUnder3::
	onerr off
	if err<>-6 :raise 22 :endif
	hlog%:(KhLogalways%,"ERROR !!TODO defect EDNRANS-4KDEFM Skipping Overflow errors.")
goto SKIPOVER::
	onerr intUnder4
	i%=-32768**2 :raise 23
intUnder4::
	onerr off
	if err<>-6 :raise 24 :endif
SKIPOVER::

	onerr intUnder5
	i%=$8000-$7fff :raise 25
intUnder5::
	onerr off
	if err<>-6 :raise 26 :endif
	onerr intUnder6
	i%=$8000+($8000) :raise 27
intUnder6::
	onerr off
	if err<>-6 :raise 28 :endif
	onerr intUnder7
	i%=$8000*$7fff :raise 29
intUnder7::
	onerr off
	if err<>-6 :raise 30 :endif

	rem SubTest:("Test Long Overflow and Underflow")
	
	l&=&7fffffff/2*2+1
	if l&<>&7fffffff :raise 1 :endif
	l&=&80000002/2*2-2
	if l&<>&80000000 :raise 2 :endif

	onerr longOver1
	l&=&7fffffff+1 :raise 3
longOver1::
	onerr off
	if err<>-6 :raise 4 :endif
	onerr longOver2
	l& = &7fffffff-(-1) :raise 5
longOver2::
	onerr off
	if err<>-6 :raise 6 :endif
	onerr longOver3
	l& = &7fffffff*2 :raise 7
longOver3::
	onerr off
	if err<>-6 :raise 8 :endif
	onerr longOver4
	l& = &7fffffff**2 :raise 9
longOver4::
	onerr off
	if err<>-6 :raise 10 :endif
	onerr longOver5
	l&=&7fffffff+&7fffffff :raise 11
longOver5::
	onerr off
	if err<>-6 :raise 12 :endif
	onerr longOver6
	l& = &7fffffff-(&80000000) :raise 13
longOver6::
	onerr off
	if err<>-6 :raise 14 :endif
	onerr longOver7
	l& = &7fffffff*&7fffffff :raise 15
longOver7::
	onerr off
	if err<>-6 :raise 16 :endif

	rem Underflow - large negative longs
	onerr longUndr1
	l& = &80000000-1 :raise 17
longUndr1::
	onerr off
	if err<>-6 :raise 18 :endif
	onerr longUndr2
	l& = &80000000+(-1) :raise 19
longUndr2::
	onerr off
	if err<>-6	:raise 20 :endif
	onerr longUndr3
	l& = &80000000*2 :raise 21
longUndr3::
	onerr off
	if err<>-6 :raise 22 :endif
	onerr longUndr4
	l& = &80000000**2 :raise 23
longUndr4::
	onerr off
	if err<>-6 :raise 24 :endif
		onerr longUndr5
	l& = &80000000-&7fffffff :raise 25
longUndr5::
	onerr off
	if err<>-6 :raise 26 :endif
	onerr longUndr7
	l& = &80000000+(&80000000) :raise 27
longUndr7::
	onerr off
	if err<>-6	:raise 28 :endif
	onerr longUndr8
	l& = &80000000*&7fffffff :raise 29
longUndr8::
	onerr off
	if err<>-6 :raise 30 :endif
	
	rem SubTest:("Test Real Overflow and Underflow")	
	hlog%:(KhLogalways%,"ERROR !!TODO defect EDNRANS-4KDEFM Skipping Overflow errors.")
goto EndofTRealTests::
	
	onerr realOver1
	r=-1e308-1e308 :raise 37
realOver1::
	onerr off
	if err<>-6 :raise 38 :endif
	onerr realOver2
	r=1e308+1e308 :raise 39
realOver2::
	onerr off
	if err<>-6 :raise 40 :endif
	onerr realOver3
	r=1e308*3.0 :raise 41
realOver3::
	onerr off
	if err<>-6	:raise 42 :endif
	onerr realOver4
	r=1e308/0.4 :raise 43
realOver4::
	onerr off
	if err<>-6 :raise 44 :endif
	onerr realOver5
	r=1e308**2.0 :raise 45
realOver5::
	onerr off
	if err<>-6 :raise 46 :endif
	
	rem Underflow not achievable by subtraction or addition 
	rem due to lack of precision
	onerr realUndr1
	r=3e-324*0.2 :raise 47
realUndr1::
	onerr off
	if err<>-5 :raise 48 :endif
	onerr realUndr2
	r=5E-324/3.0 :raise 49
realUndr2::
	onerr off
	if err<>-5 :raise 50 :endif
	onerr realUndr3
	r=5E-324**2.0 :raise 51
realUndr3::
	onerr off
	if err<>-5 :raise 52 :endif
EndOfTrealTests::
endp


proc arith:
	local ires%,l&,lres&
	rem SubTest:("Real Arithmetic Operators")
	rem print "2.0+3.0=";2.0+3.0
	if (2.0+3.0)  <>  5.0 :raise 1 :endif
	rem print "2.0-3.0=";2.0-3.0
	if (2.0-3.0)  <>  -1.0 :raise 2 :endif
	rem print "2.0*3.0=";2.0*3.0
	if (2.0*3.0)  <>  6.0 :raise 3 :endif
	rem print "3.0/2.0=";3.0/2.0
	if (3.0/2.0)  <>  1.5 :raise 4 :endif
	rem print "2.0**3.0=";2.0**3.0
	if DblNeqG%:((2.0**3.0),8.0,1E-15) :raise 5 :endif

	rem print "1.0**10.0=";1.0**10.0
	if 1.0**10.0 <> 1.0 :raise 6 :endif
	rem print "2.0**0.0=";2.0**0.0
	if 2.0**0.0 <> 1.0 :raise 7 :endif
	rem print "0.0**1.0=";0.0**1.0
	if 0.0**1.0 <> 0.0 :raise 8 :endif
	rem print "2.0 and 3.0=";2.0 and 3.0
	if (2.0 and 3.0)=0 :raise 9 :endif
	rem print "0.0 and 3.0=";0.0 and 3.0
	if (0.0 and 3.0)=-1 :raise 10 :endif
	rem print "2.0 or 3.0=";2.0 or 3.0
	if (2.0 or 3.0)=0 :raise 11 :endif
	rem print "2.0 or 0.0=";2.0 or 0.0
	if (2.0 or 0.0)=0 :raise 12 :endif
	rem print "0.0 or 0.0=";0.0 or 0.0
	if (0.0 or 0.0)= -1 :raise 13 :endif
	rem print "not 3.0=";not 3.0
	if	(not 3.0)=-1 :raise 14 :endif
	rem print "not 0.0=";not 0.0
	if (not 0.0)=0 :raise 15 :endif
	rem print "-3.0=";-(3.0)
	if -(3.0)<>0.0-3.0 :raise 16 :endif	

	rem SubTest:("Integer Arithmetic Operators")

	rem print "2+3=";2+3
	if (2+3) <> 5 :raise 1 :endif
	rem print "2-3=";2-3
	if (2-3) <> -1 :raise 2 :endif
	rem print "2*3=";2*3
	if (2*3)  <>  6 :raise 3 :endif
	rem print "3/2=";3/2
	if (3/2)  <>  1 :raise 4 :endif
	rem print "2**3=";2**3
	if 2**3 <> 8 :raise 5 :endif
	rem print "1**10=";1**10
	if 1**10 <> 1 :raise 6 :endif
	rem print "2**0=";2**0
	if 2**0 <> 1 :raise 7 :endif
	rem print "0**1=";0**1
	if 0**1 <> 0 :raise 8 :endif
	rem print "2 and 3=";2 and 3
	if (2 and 3)=0 :raise 9 :endif
	rem print "0 and 3=";0 and 3
	if (0 and 3)=-1 :raise 10 :endif
	rem print "2 or 3=";2 or 3
	if (2 or 3)=0 :raise 11 :endif
	rem print "2 or 0=";2 or 0
	if (2 or 0)=0 :raise 12 :endif
	rem print "0 or 0=";0 or 0
	if (0 or 0)= -1 :raise 13 :endif
	rem print "not 3=";not 3
	if  (not 3)=-1 :raise 14 :endif
	rem print "not 0=";not 0
	if (not 0)=0 :raise 15 :endif
	rem print "-3=";-(3)
	if -(3)<>0-3 :raise 16 :endif	

	onerr intpow
	ires%=0**-1
	raise 17
intpow::
	onerr off
	if err<>-2 :raise 18 :endif 

	rem SubTest:("Long Arithmetic Operators")

	rem print "2+3=";&2+&3
	if (&2+&3) <> &5 :raise 1 :endif
	rem print "2-3=";&2-&3
	if (&2-&3) <> &ffffffff :raise 2 :endif
	rem print "2*3=";&2*&3
	if (&2*&3)	<>  &6 :raise 3 :endif
	rem print "3/2=";&3/&2
	if (&3/&2)  <>  &1 :raise 4 :endif
	rem print "2**3=";&2**&3
	if (&2**&3) <> &8 :raise 5 :endif
	rem print "1**10=";&1**&10
	if (&1**&10) <> &1 :raise 6 :endif
	rem print "2**0=";&2**&0
	if (&2**&0) <> &1 :raise 7 :endif
	rem print "0**1=";&0**&1
	if (&0**&1) <> &0 :raise 8 :endif
	rem print "2 and 3=";&2 and &3
	if (&2 and &3)=&0 :raise 9 :endif
	rem print "0 and 3=";&0 and &3
	if (&0 and &3)=&ffffffff :raise 10 :endif
	rem print "2 or 3=";&2 or &3
	if (&2 or &3)=&0 :raise 11 :endif
	rem print "2 or 0=";&2 or &0
	if (&2 or &0)=&0 :raise 12 :endif
	rem print "0 or 0=";&0 or &0
	if (&0 or &0)= &ffffffff :raise 13 :endif
	rem print "not 3=";not &3
	if	(not &3)=&ffffffff :raise 14 :endif
	rem print "not 0=";not &0
	if (not &0)=&0 :raise 15 :endif
	rem print "-3=";-(&3)
	if -(&3)<>&0-&3 :raise 16 :endif	

	onerr longpow
	lres&=&0**&ffffffff
	raise 17
longpow::
	onerr off
	if err<>-2 :raise 18 :endif 
endp


proc transcnd:
	local lessprec,prec
	rem SubTest:("Transcendental Functions")
	prec=1e-15
	lessprec=1E-14
	rem Trig functions
	if sin(pi/2.0) <> 1.0 :raise 1 :endif
	if sin(0.0) <> 0.0 :raise 2 :endif
	if DblNeqG%:(sin(pi/6.0),0.5,prec) :raise 3 :endif
	if DblNeqG%:(sin(pi/4.0),1.0/sqr(2),prec) :raise 4 :endif
	if DblNeqG%:(sin(pi/3.0),sqr(3)/2.0,prec) :raise 5 :endif
	if DblNeqG%:(cos(pi/2.0),0.0,prec) : raise 6 :endif
	if DblNeqG%:(cos(0.0),1.0,prec) :raise 7 :endif
	if DblNeqG%:(cos(pi/6.0),sqr(3)/2.0,prec) :raise 8 :endif
	if DblNeqG%:(cos(pi/4.0),1.0/sqr(2.0),prec) :raise 9 :endif
	if DblNeqG%:(cos(pi/3.0),0.5,prec) :raise 10 :endif
	if DblNeqG%:(tan(pi),0.0,prec) :raise 11 :endif
	if tan(0.0) <> 0.0 :raise 12 :endif
	if DblNeqG%:(tan(pi/4.0),1.0,prec) :raise 13 :endif
	
	rem Inverse trig functions
	if asin(0.0) <> 0.0 :raise 14 :endif
	if DblNeqG%:(asin(1.0),pi/2.0,prec) :raise 15 :endif
	if DblNeqG%:(asin(0.5),pi/6.0,prec) :raise 16 :endif
	if DblNeqG%:(asin(1.0/sqr(2)),pi/4.0,prec) :raise 17 :endif
	if DblNeqG%:(asin(sqr(3)/2.0),pi/3.0,prec) :raise 18 :endif
	if DblNeqG%:(asin(sin(0.6)),0.6,prec) :raise 19 :endif
	if asin(sin(pi/2)) <> pi/2 :raise 20 :endif
	if acos(0.0) <> pi/2:raise 21 :endif
	if DblNeqG%:(acos(1.0),0.0,prec) :raise 22 :endif
	if DblNeqG%:(acos(sqr(3)/2.0),pi/6.0,prec) :raise 23 :endif
	if DblNeqG%:(acos(1.0/sqr(2.0)),pi/4.0,prec) :raise 24 :endif
	if DblNeqG%:(acos(0.5),pi/3.0,prec) :raise 25 :endif
	if DblNeqG%:(acos(cos(0.6)),0.6,prec) :raise 26 :endif
	if atan(0.0) <> 0.0 :raise 27 :endif
	if atan(tan(pi/4)) <> pi/4 :raise 28 :endif
	if DblNeqG%:(atan(tan(0.6)),0.6,prec) :raise 29 :endif

	rem Other functions
	if log(100.0) <> 2.0 :raise 31 :endif
	if log(10.0) <> 1.0 :raise 32 :endif
	if log(10.0**50.0) <> 50.0 :raise 33 :endif
	if exp(ln(2.0)) <> 2.0 :raise 34 :endif
	if exp(0.0) <> 1.0 :raise 35 :endif
	if DblNeqG%:(exp(ln(10.0)),10.0,lessprec) :raise 36 :endif	
	if ln(exp(1.0)) <> 1.0 :raise 37 :endif
	if ln(exp(2.0)) <> 2.0 :raise 38 :endif
	if ln(exp(1.23456)) <> 1.23456 :raise 39 :endif
	if sqr(4.0) <> 2.0 :raise 40 :endif
	if sqr(81.0) <> 9.0 :raise 41 :endif
	if sqr(9*9) <>	9.0 :raise 42 :endif
	if deg(pi) <> 180 :raise 43 :endif
	if rad(180) <> pi :raise 44 :endif
	if deg(rad(pi/3)) <> pi/3 :raise 45 :endif
	if deg(-pi/2) <> -90 :raise 46 :endif
	if rad(-45) <> -pi/4 :raise 47 :endif
	if deg(rad(-pi/6)) <> -pi/6 :raise 48 :endif
	if deg(rad(0.0)) <> 0.0 :raise 49 :endif
endp


proc fErrors:
	local d
	onerr e1
	d=cos(1E50)
	raise 1
e1::
	onerr off
	if err<>-2
		raise 2
	endif
	onerr e2
	d=cos(-1E50)
	raise 3
e2::
	onerr off
	if err<>-2
		raise 4
	endif
	onerr e3
	d=sin(1E50)
	raise 5
e3::
	onerr off
	if err<>-2
		raise 6
	endif	
	onerr e4
	d=sin(-1E50)
	raise 7
e4::
	onerr off
	if err<>-2
		raise 8
	endif

	onerr e7
	d=tan(1E20)
	raise 13
e7::
	onerr off
	if err<>-2
		raise 14
	endif
	onerr e8
	d=tan(-1E15)
	raise 15
e8::
	onerr off
	if err<>-2
		raise 16
	endif
	onerr e9
	d=asin(2.0)
	raise 17
e9::
	onerr off
	if err<>-2
		raise 18
	endif
	onerr e10
	d=acos(2.0)
	raise 19
e10::
	onerr off
	if err<>-2
		raise 20
	endif
	hlog%:(KhLogalways%,"ERROR !!TODO defect EDNRANS-4KDEFM Skipping Underflow errors.")
GOTO SKIPUNDER::

	onerr e12
	d=rad(5E-324)
	raise 23
e12::
	onerr off
	if err<>-5
		raise 24
	endif
SKIPUNDER::

	onerr e14
	d=ln(-1)
	raise 27
e14::
	onerr off
	if err<>-2
		raise 28
	endif
	onerr e15
	d=log(-1)
	raise 29
e15::
	onerr off
	if err<>-2
		raise 30
	endif
	onerr e16
	d=sqr(-1)
	raise 31
e16::
	onerr off
	if err<>-2
		raise 32
	endif	

	hlog%:(KhLogalways%,"ERROR !!TODO defect EDNRANS-4KDEFM Skipping Overflow errors.")
goto EndOfFError::

	REM overflow tests.
	onerr e5
	d=tan(pi/2.0)
	raise 9
e5::
	onerr off
	if err<>-6
		print d,err
		raise 10
	endif
	onerr e6
	d=tan(-pi/2)
	raise 11
e6::
	onerr off
	if err<>-6
		raise 12
	endif
	onerr e11
	d=deg(1E308)
	raise 21
e11::
	onerr off
	if err<>-6
		raise 22
	endif

	onerr e13
	d=exp(800.0)
	raise 25
e13::
	onerr off
	if err<>-6
		raise 26
	endif

EndofFError::
endp


proc DblNeqG%:(l,r,prec)
	local dif,res%
	dif=abs(l-r)
	rem print "Dif=";dif
	res%=(dif>=prec)
	rem if res%
		rem	pause pause% :key
	rem endif
	return res%
endp



REM End of tMaths.tpl 

