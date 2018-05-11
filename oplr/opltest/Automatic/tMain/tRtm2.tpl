REM tRtm2.tpl
REM EPOC OPL automatic test code for further misc runtime ops.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tRtm2", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tRtm2:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("doRtm2")
	hCleanUp%:("CleanUp")
ENDP


PROC CleanUp:
	trap close : trap close : trap close : trap close : trap close
	trap delete "c:\Opl1993\*.*"
	trap rmdir "c:\Opl1993"
ENDP


proc doRtm2:
	global q,r,m,c$(1)
	global free
	local i%,t%,ii
  global path$(9),drv$(2),patha$(9)

  drv$="c:"
  path$=drv$
  patha$=drv$+"\Opl1993"
  trap mkdir patha$
	
	do
		q=65536
		r=198
		m=1000*rnd
		c$=chr$(256*rnd)
		rem loadm "t_rtm2a"
		ii=rtm2a:(2,1.,"rtm") :cls
		rem unloadm "t_rtm2a"
		rem loadm "t_rtm2b"
		ii=rtm2b:(2,2.,"rtm") :cls
		rem unloadm "t_rtm2b"
		rem loadm "t_rtm2c"
		ii=rtm2c:(2,3.,"rtm") :cls
		rem unloadm "t_rtm2c"
		i%=i%+1
	until i%>1
	rem print "ALL GOOD" :beep 3,1000 :pause 99
endp


proc rtm2a:(p%,p,p$)
	local i%,tem,tem1,tem2,int1%,int2%,int3%
	do
		rem print p$,p%;".";p,"F:";free
		int1%=100*(rnd-.5)
		int2%=100*(rnd-.5)
		tem=rnd
		tem1=rnd
		if (1 and 2) <> 0 :raise 1 :endif
		if (1 and 2.) <> -1 :raise 2 :endif
		if (1. or 0) <> -1 :raise 3 :endif
		if (0 or 0.) <> 0 :raise 4 :endif
		if ($12 or $7) <> $17 :raise 5 :endif
		if not(2) <> -3 :raise 6 :endif
		if not(2.) <> 0 :raise 7 :endif
		if sin(tem)+sin(-tem) > 1e-9 :raise 8 : endif
		if cos(tem)-cos(-tem) > 1e-9 :raise 9 : endif
		if sin(tem)**2+cos(tem)**2-1 > 1e-9 :raise 10 : endif
		if tem - atan(tan(tem)) > 1e-9 :raise 11 :endif
		if tem - ln(exp(tem)) > 1e-9 :raise 12 :endif
		if tem - log(10**tem) > 1e-9 :raise 13 :endif
		if tem - rad(deg(tem)) > 1e-9 :raise 14 :endif
		if tem - sqr(tem)*sqr(tem) > 1e-9 :raise 15 :endif
		if tem - tem1-tem+tem1 > 1e-9 :raise 16 :endif
		if tem - tem1*(tem/tem1) > 1e-9 :raise 17 :endif
		if tem - sqr(tem**2)  > 1e-9 :raise 18 :endif
		if intf(tem) > 1e-9 :raise 19 :endif
		if int(tem) > 1e-9 :raise 20 :endif
		if 1/2 <> 0 :raise 21 :endif
		if 17/3 <> 5 :raise 22 :endif
		
		rem CHANGE FROM ORGANISER    if -17/3 <> -6 :raise 23 :endif
		if -17/3 <> -5 :raise 23 :endif
		
		if int1%*int2% <> flt(int1%)*int2% :raise 24 :endif
		if int1%+int2%-int2% <> int1% :raise 25 :endif
		if 2*int1% <> int1%+int1% :raise 26 :endif
		int2%=abs(int2%)+25
		while int1%<16000
			int1%=abs(int1%+int2%)
			if 2*int1% <> int1%+int1% :raise 27 :endif
			if 2*-int1% <> -int1%-int1% :raise 28 :endif
			int3%=int1%/int2%
			rem print int1%,int2%,int3% :get
			if abs(int1%-int3%*int2%)>abs(int1%) :raise 29 :endif
		endwh
		
		int1%=20
		do
			tem=(rnd-.5)*(40**rnd)
			tem1=(rnd-.5)*(40**rnd)
			if abs(tem1) > abs(tem) : tem2=abs(tem1)
			else :tem2=abs(tem) :endif
			if abs(((tem*tem1)/tem1)-tem) > tem2*(10.**-10) :raise 30 :endif
			if abs(((tem+tem1)-tem1)-tem) > tem2*(10.**-10) :raise 31 :endif
			int1%=int1%-1
		until int1%=0
		
		tem=rnd
		onerr clerr::
		raise 0  :rem clear ERR() variable
		clerr::
		
		rem print"In t_rtm2a all Opl1993 memory tests have been remmed for Opler1"
		rem  onerr noMems::
		rem  m0=tem
		rem  onerr off
		rem  m1=-m0
		rem  m2=-m1
		rem  m3=-m2
		rem  m4=-m3
		rem  m5=-m4
		rem  m6=-m5
		rem  m7=-m6
		rem  m8=-m7
		rem  m9=-m8
		rem  if m9<>-tem :raise 32 :endif
		rem noMems::
		rem  onerr off
		rem  if err
		rem   if err<>-98 :raise 33 :endif :rem not missing external M0 (which is allowed on machines with no calculator memories)
		rem  endif
		i%=i%+1
	until i%>1
endp


proc rtm2b:(p%,p,p$)
	rem test the string slicing
	local a$(255),r%,i%,c$(1)
	do
		rem print p$,p%;".";p,"F:";free
		do
			a$=dir$("a")
		until not(len(a$))
		r%=rnd*256
		if len(c$)<>0 :raise 1 :endif
		if r%<>asc(chr$(r%)) :raise 2 :endif
		if r%<>len(rept$("a",r%)) :raise 3 :endif
		a$="Martin Smith"
		if left$(a$,6)<>"Martin" :raise 4 :endif
		if right$(a$,5)<>"Smith" :raise 5 :endif
		if mid$(a$,5,4)<>"in S" :raise 6 :endif
		if loc(a$,mid$(a$,5,4))<>5 :raise 7 :endif
		if loc(a$,"xdf")<>0 :raise 8 :endif
		a$=rept$("Martin Smith",21)+"xyz"
		if left$(a$,6)<>"Martin" :raise 9 :endif
		if right$(a$,5)<>"mpxyz" :raise 10 :endif
		if mid$(a$,5,4)<>"in S" :raise 11 :endif
		if loc(a$,mid$(a$,5,4))<>5 :raise 12 :endif
		if len(rept$(a$,0)) :raise 13 :endif
		i%=i%+1
	until i%>1
endp


proc rtm2c:(p%,p,p$)
	rem test the file handling
	local i%,j%,rrr
	rem print p$,p%;".";p,"F:";free
	do
		trap delete patha$+"\Mon"
		create patha$+"\Mon",a,a%,a,a$
		rrr=10*rnd
		randomize rrr
		j%=0
		do
			rem at 1,2 :print j%
			a.a%=(rnd-.5)*65000
			a.a=(rnd-.5)*(10**(10*(rnd-.5)))
			a.a$=rept$("ABC",rnd*30)
			append
			j%=j%+1
		until j%>=100
		j%=0
		rem at 1,2 :print "       "
		first
		randomize rrr
		do
			rem at 2,2 :print j%
			if a.a%<>int((rnd-.5)*65000) :raise 1 :endif
			if a.a<>(rnd-.5)*(10**(10*(rnd-.5))) :raise 2 :endif
			if a.a$<>rept$("ABC",rnd*30) :raise 3 :endif
			next
			j%=j%+1
		until j%>=100
		position 101
		if not(eof) :raise 4 :endif
		first
		if find("zxasd"): raise 5 :endif
		if not(eof) :raise 6 :endif
		j%=0
		rem at 1,2 :print "       "
		do
			rem at 3,2 :print j%
			position 100*rnd+1
			a.a%=j%*j%
			a.a$=rept$("end",rnd*30)
			update
			j%=j%+1
		until j%>50
		first
		do
			find("*end*")
			erase
		until eof
		j%=0
		rem at 1,2 :print "       "
		onerr a::
		do
			rem at 4,2 :print count,space,
			a.a%=(rnd-.5)*65000
			a.a=(rnd-.5)*(10**(10*(rnd-.5)))
			a.a$=rept$("ABC",rnd*30)
			append
			j%=j%+1
		until j%>4
		rem was until j%>4000
		raise 7
a::
		onerr off :first :erase :close :delete patha$+"\Mon"
		
		trap delete patha$+"\a"
		trap delete patha$+"\b"
		trap delete patha$+"\c"
		trap delete patha$+"\d"
		create patha$+"\a",a,a
		create patha$+"\b",b,a
		create patha$+"\c",c,a
		create patha$+"\d",d,a
		use a :use b :use c :use d
		space :close :space :close :space :close :space :close
		open patha$+"\a",a,a
		trap use b : space :close
		delete patha$+"\a"
		delete patha$+"\b"
		delete patha$+"\c"
		delete patha$+"\d"
		
		rem Organiser created file with less fields than opened with
		rem - not allowed on OPL1989
		create patha$+"\a",a,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p
		a.j=123
		append
		close
		open patha$+"\a",a,a,b,c,d,e,f,g,h,i,j
		if a.j<>123 :raise 8 :endif
		a.j=789
		if a.j<>789 :raise 9 :endif
		close
		delete patha$+"\a"
		
		i%=i%+1
		rem at 1,2 :print p$,p%;".";p,"F:";free
	until i%>1
endp

REM End of tRtm2.tpl

