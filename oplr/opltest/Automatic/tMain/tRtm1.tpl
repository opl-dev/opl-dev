REM tRtm1.tpl
REM EPOC OPL automatic test code for misc runtime ops.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tRtm1", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tRtm1:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("dortm1")
	rem hCleanUp%:("CleanUp")
	rem KLog%:(KhLogHigh%,"Some sample text")
endp


proc dortm1:
	rem test all the simple primitives
	global a%,a,a$(8),a%(77),a(77),a$(77,8)
	global m,q,r,c$(1)
	local i%,ii
	
	onerr escr::
	screen 40,16,1,1
	escr::
	do
		q=65536
		r=198
		m=1000*rnd
		c$=chr$(256*rnd)
		ii=rtm1a:(1,1.,"rtm") :cls
		ii=rtm1b:(1,2.,"rtm") :cls
		ii=rtm1c:(1,3.,"rtm") :cls
		ii=rtm1d:(1,4.,"rtm") :cls
		i%=i%+1
	until i%>1
rem	print "ALL GOOD" :beep 20,1000 :pause 99
endp


proc rtm1a:(p%,p,p$)
	local i%,b%,b,b$(8),b%(77),b(77),b$(77,8),i
	do
		rem print p$,p%;".";p
		randomize m
		b%=(rnd-.5)*q
		b=10**(r*(rnd-.5))
		b$=rept$(c$,rnd*8+1)
		i=0
		while i<77
			i=i+1
			b%(i)=(rnd-.5)*q
			b(i)=10**(r*(rnd-.5))
			b$(i)=rept$(c$,rnd*8+1)
		endwh
		
		a%=(rnd-.5)*q
		a=10**(r*(rnd-.5))
		a$=rept$(c$,rnd*8+1)
		i=0
		while i<77
			i=i+1
			a%(i)=(rnd-.5)*q
			a(i)=10**(r*(rnd-.5))
			a$(i)=rept$(c$,rnd*8+1)
		endwh
		
		randomize m
		if b%<>(rnd-.5)*q :endif
		if b<>10**(r*(rnd-.5)) :endif
		if b$<>rept$(c$,rnd*8+1) :endif
		i=0
		while i<77
			i=i+1
			if b%(i)<>(rnd-.5)*q :endif
			if b(i)<>10**(r*(rnd-.5)) :endif
			if b$(i)<>rept$(c$,rnd*8+1) :endif
		endwh
		
		if a%<>(rnd-.5)*q :endif
		if a<>10**(r*(rnd-.5)) :endif
		if a$<>rept$(c$,rnd*8+1) :endif
		i=0
		while i<77
			i=i+1
			if a%(i)<>(rnd-.5)*q :endif
			if a(i)<>10**(r*(rnd-.5)) :endif
			if a$(i)<>rept$(c$,rnd*8+1) :endif
		endwh
		i%=i%+1
	until i%>1
endp


proc rtm1b:(p%,p,p$)
	rem test the commands
	local a%,i%
	do
		rem print p$,p%;".";p
		at rnd*40+1,rnd*16+1
		cursor off
		cursor on
		escape off
		escape on
		onerr a::
		raise -1
		print "err" : beep 10,10 :get
		a::
		onerr off
		pause 10
		i%=i%+1
	until i%>1
endp


proc rtm1c:(p%,p,p$)
	rem tests the functions
	local tem,tem$(255),i%
	do
		rem print p$,p%;".";p
		tem=(1 and 2) or not(1. and 2.)
		tem=err+second+minute+hour+day+month+year
		tem$=datim$+err$(-513)
		tem=abs(rnd)+atan(rnd)+cos(rnd)+deg(rnd)+exp(rnd)+int(rnd)+intf(rnd)
		tem=ln(rnd+.1)+log(rnd)+pi+rad(rnd)+sin(rnd)+sqr(rnd)+tan(rnd)
		tem=flt(addr(a%))+addr(a)+addr(a$)+addr(a%(1))+addr(a(1))+addr(a$(1))
		tem=asc(chr$(rnd*256))+len(datim$)+loc(datim$,datim$)+val("2")
		tem$=lower$(datim$)+upper$(datim$)
		tem$=left$(datim$,1)+mid$(datim$,1,1)+right$(datim$,1)+rept$(datim$,1)
		tem$=gen$(-2.3,5)+num$(-23.4,5)+sci$(-567e3,2,-10)+fix$(99e90,2,10)
		i%=i%+1
	until i%>1
endp


proc rtm1d:(p%,p,p$)
	rem test the numeric and string equalities
	global a%,b%,c,d,e$(255),f$(255),i%
	do
		rem print p$,p%;".";p
		a%=31234
		b%=31233
		if a%<b% or a%<=b% or a%=b% or not(a%<>b%) or not(a%>=b%) or not(a%>b%)
			raise 1
		endif
		a%=-32222
		b%=31000
		if not(a%<b%) or not(a%<=b%) or a%=b% or not(a%<>b%) or a%>=b% or a%>b%
			raise 2
		endif
		a%=31233
		b%=31233
		if a%<b% or not(a%<=b%) or not(a%=b%) or a%<>b% or not(a%>=b%) or a%>b%
			raise 3
		endif
		a%=31234
		b%=31233
		if a%<b% or a%<=b% or a%=b% or not(a%<>b%) or not(a%>=b%) or not(a%>b%)
			raise 4
		endif
		a%=-32222
		b%=7
		if not(a%<b%) or not(a%<=b%) or a%=b% or not(a%<>b%) or a%>=b% or a%>b%
			raise 5
		endif
		a%=31233
		b%=31233
		if a%<b% or not(a%<=b%) or not(a%=b%) or a%<>b% or not(a%>=b%) or a%>b%
			raise 6
		endif
		if not(a%)<>(-a%-1) or not(not(a%))<>a% or (a% and b%) <> a% or (a% or b%) <> a%
			raise 7
		endif
		if ((a% and not(a%)) <> 0) or ((a% or not(a%)) <> -1)
			raise 8
		endif
		rem bcd compares
		c=999999.9999e87
		d=999999.9998e87
		if c<d or c<=d or c=d or not(c<>d) or not(c>=d) or not(c>d)
			raise 9
		endif
		c=-789.1233e90
		d=31233e-90
		if not(c<d) or not(c<=d) or c=d or not(c<>d) or c>=d or c>d
			raise 10
		endif
		c=123.4567890
		d=123.4567890
		if c<d or not(c<=d) or not(c=d) or c<>d or not(c>=d) or c>d
			raise 11
		endif
		if not(c)<>0 or not(not(c))<>-1 or (c and d) <> -1 or (c or d) <> -1
			raise 12
		endif
		if (c and not(c)) <> 0 or (c or not(c)) <> -1
			raise 13
		endif
		e$="kjhfunb"
		f$="97xvkjnas"
		if e$<f$ or e$<=f$ or e$=f$ or not(e$<>f$) or not(e$>=f$) or not(e$>f$)
			raise 14
		endif
		e$="a"
		f$="ZZZZZZZZZ"
		if e$<f$ or e$<=f$ or e$=f$ or not(e$<>f$) or not(e$>=f$) or not(e$>f$)
			raise 15
		endif
		f$=""
		e$="97xvkjnas"
		if e$<f$ or e$<=f$ or e$=f$ or not(e$<>f$) or not(e$>=f$) or not(e$>f$)
			raise 16
		endif
		f$=""
		e$=""
		if e$<f$ or not(e$<=f$) or not(e$=f$) or e$<>f$ or not(e$>=f$) or e$>f$
			raise 17
		endif
		f$="z"
		e$="z"
		if e$<f$ or not(e$<=f$) or not(e$=f$) or e$<>f$ or not(e$>=f$) or e$>f$
			raise 18
		endif
		e$=chr$(255)
		f$=chr$(254)
		if e$<f$ or e$<=f$ or e$=f$ or not(e$<>f$) or not(e$>=f$) or not(e$>f$)
			raise 19
		endif 
		e$=rept$("Z",255)
		f$="Z"
		if e$<f$ or e$<=f$ or e$=f$ or not(e$<>f$) or not(e$>=f$) or not(e$>f$)
			raise 20
		endif
		f$=""
		e$=rept$(chr$(255),255)
		if e$<f$ or e$<=f$ or e$=f$ or not(e$<>f$) or not(e$>=f$) or not(e$>f$)
			raise 21
		endif
		f$=chr$(128)
		e$=chr$(128)
		if e$<f$ or not(e$<=f$) or not(e$=f$) or e$<>f$ or not(e$>=f$) or e$>f$
			raise 22
		endif
		i%=i%+1
	until i%>1
endp


REM End of tRtm1.tpl 
