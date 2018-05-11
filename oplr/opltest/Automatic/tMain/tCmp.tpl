REM tCmp.tpl
REM EPOC OPL automatic test code for comparisons.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tCmp", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tCmp:
rem	hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("cmpw")
	hRunTest%:("cmpl")
	hRunTest%:("cmpd")
	hRunTest%:("cmps")
endp


proc cmpw:
	if 0<0    				:raise 1	:endif
	if 1<1    				:raise 2	:endif
	if -32767<-32767	:raise 3  :endif
	if 1<0    				:raise 4	:endif
	if 2<1    				:raise 5	:endif
	if 0<-1						:raise 6	:endif
	if -32766<-32767	:raise 7  :endif

	if 0>0    				:raise 8	:endif
	if 1>1    				:raise 9	:endif
	if -32767>-32767	:raise 10  :endif
	if 0>1    				:raise 11	:endif
	if 1>2    				:raise 12	:endif
	if -1>0						:raise 13	:endif
	if -32767>-32766	:raise 14  :endif

  if 1     <= 0			:raise 15	:endif
	if 0     <= -1		:raise 16	:endif
  if 32767 <= 32766	:raise 17		:endif
  if -32766<= -32767	:raise 18	:endif

  if 0     >= 1			:raise 19	:endif
	if -1    >= 0  		:raise 20	:endif
  if 32766 >= 32767	:raise 21		:endif
  if -32767>=-32766	:raise 22	:endif

	if 0=1 :raise 23 :endif
	if 0=-1 :raise 24 :endif
	if 1=-1 :raise 25 :endif
	if 32767=32766 :raise 26 :endif
	if -32767=-32766 :raise 27 :endif

	if 0<>0 :raise 28 :endif
	if 32767<>32767 :raise 29 :endif
	if -32767<>-32767 :raise 30 :endif
	if 1<>1 :raise 31 :endif
endp

proc cmpl:
	if &0<&0    				:raise 41	:endif
	if &1<&1    				:raise 42	:endif
	if &80000000<&80000000	:raise 43  :endif
	if &1<&0    				:raise 44	:endif
	if &2<&1    				:raise 45	:endif
	if &0<&ffffffff						:raise 46	:endif
	if &80000001<&80000000	:raise 47  :endif

	if &0>&0    				:raise 48	:endif
	if &1>&1    				:raise 49	:endif
	if &80000000>&80000000	:raise 50  :endif
	if &0>&1    				:raise 51	:endif
	if &1>&2    				:raise 52	:endif
	if &ffffffff>&0						:raise 53	:endif
	if &80000000>&80000001	:raise 54  :endif

  if &1     <= &0			:raise 55	:endif
	if &0     <= &ffffffff		:raise 56	:endif
  if &7fffffff <= &7ffffffe	:raise 57		:endif
  if &80000001<= &80000000	:raise 58	:endif

  if &0     >= &1			:raise 59	:endif
	if &ffffffff    >= &0  		:raise 60	:endif
  if &7ffffffe >= &7fffffff	:raise 61		:endif
  if &80000000>=&80000001	:raise 62	:endif

	if &0=&1 :raise 63 :endif
	if &0=&ffffffff :raise 64 :endif
	if &1=&ffffffff :raise 65 :endif
	if &7fffffff=&7ffffffe :raise 66 :endif
	if &80000000=&80000001 :raise 67 :endif

	if &0<>&0 :raise 68 :endif
	if &7fffffff<>&7fffffff :raise 69 :endif
	if &80000000<>&80000000 :raise 70 :endif
	if &1<>&1 :raise 71 :endif
endp

proc cmpd:
	if 0.<0    				:raise 81	:endif
	if 1.<1    				:raise 82	:endif
	if -32767. <-32767	:raise 83  :endif
	if 1.<0    				:raise 84	:endif
	if 2.<1    				:raise 85	:endif
	if 0.<-1						:raise 86	:endif
	if -32766. <-32767	:raise 87  :endif

	if 0.>0    				:raise 88	:endif
	if 1.>1    				:raise 89	:endif
	if -32767. >-32767	:raise 90  :endif
	if 0.>1    				:raise 91	:endif
	if 1.>2    				:raise 92	:endif
	if -1.>0						:raise 93	:endif
	if -32767. >-32766	:raise 94  :endif

  if 1.0     <= 0			:raise 95	:endif
	if 0.0     <= -1		:raise 96	:endif
  if 32767.0 <= 32766	:raise 97		:endif
  if -32766.0 <= -32767	:raise 98	:endif

  if 0.0     >= 1			:raise 99	:endif
	if -1.0    >= 0  		:raise 100	:endif
  if 32766.0 >= 32767	:raise 101		:endif
  if -32767.0 >=-32766	:raise 102	:endif

	if 0.0=1 :raise 103 :endif
	if 0.0=-1 :raise 104 :endif
	if 1.0=-1 :raise 105 :endif
	if 32767.0=32766 :raise 106 :endif
	if -32767.0=-32766 :raise 107 :endif

	if 0.0<>0 :raise 108 :endif
	if 32767.0<>32767 :raise 109 :endif
	if -32767.0<>-32767 :raise 110 :endif
	if 1.0<>1 :raise 111 :endif
endp

proc cmps:
	local max1$(255),max2$(255)
	local big1$(254),big2$(254)

	if ""  < "" :raise 201 :endif
	if "1" < "" :raise 202 :endif
	if "a" < "A" :raise 203 :endif
	if "1" < "1" :raise 204 :endif
  if "2" < "1" :raise 205 :endif
	if chr$(0) < "" :raise 206 :endif		:rem chr$(0) has lbc == 1

	if ""  > "" :raise 207 :endif
	if ""  > "1" :raise 208 :endif
	if "A" > "a" :raise 209 :endif
	if "1" > "1" :raise 210 :endif
	if "1" > "2" :raise 211 :endif
	if ""  > chr$(0) :raise 212 :endif

	if "1" <= "" :raise 213 :endif
	if "2" <= "1" :raise 214 :endif
	if "a" <= "A" :raise 215 :endif
	if chr$(0)<="" :raise 216 :endif

	if ""  >= "1" :raise 217 :endif
	if "1" >= "2" :raise 218 :endif
	if "A" >= "a" :raise 219 :endif
	if ""  >= chr$(0) :raise 220 :endif

	if ""=chr$(0) :raise 221 :endif
	if "1"="2"	 :raise 222 :endif
	if chr$(0)="" :raise 223 :endif
	if chr$(0)=chr$(1) :raise 224 :endif

	if ""<>"" :raise 225 :endif
	if "1"<>"1" :raise 226 :endif
	if chr$(0)<>chr$(0) :raise 227 :endif

	max1$=rept$("1",255)
	max2$=rept$("2",255)
	big1$=rept$("1",254)
	big2$=rept$("2",254)

	if max1$ < max1$ : raise 301 :endif
	if max2$ < max1$ : raise 302 :endif
	if max1$ < big1$ : raise 303 :endif
	if big2$ < max1$ : raise 304 :endif

	if max1$ > max1$ : raise 305 :endif
	if max1$ > max2$ : raise 306 :endif
	if max1$ > big2$ : raise 307 :endif
	if big1$ > max1$ : raise 308 :endif

	if max2$ <= max1$ :raise 309 :endif
	if max1$ <= big1$ :raise 310 :endif
	if big2$ <= max1$ :raise 311 :endif

	if max1$>=max2$ :raise 312 :endif
	if big1$>=max1$ :raise 313 :endif
	if max1$>=big2$ :raise 314 :endif

	if max1$=max2$ :raise 315 :endif
	if max1$=big1$ :raise 316 :endif
	if big1$=big2$ :raise 317 :endif

	if max1$<>max1$ :raise 318 :endif
	if max2$<>max2$ :raise 319 :endif
	if big1$<>big1$ :raise 320 :endif
	if big2$<>big2$ :raise 321 :endif

  if "A">chr$(193) :raise 322 :endif
  if "A"=chr$(193) :raise 323 :endif

  if "A">chr$(194) :raise 324 :endif
  if "A"=chr$(194) :raise 325 :endif
endp

REMEnd of tCmp.tpl
