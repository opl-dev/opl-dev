REM tBPCal.tpl
REM EPOC OPL automatic test code for dates.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tBPCal", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tBPCal:
	rem hLogTimestamp:
	hRunTest%:("bpcal1")
	rem hLogTimestamp:
	hRunTest%:("bpcal2")
	rem hLogTimestamp:
	hRunTest%:("bpcal3")
	rem hLogTimestamp:
endp



proc bpcal1:
	rem test that calendar functions work on 27/28 Feb
	local d%,m%,y%
	
	rem cls
	rem print "Start of BPCAL1 Test"
	y%=1900 :m%=2 
	rem print "Year="
	do
		rem at 6,2 :print y%
		d%=27
		do
			onerr err::
			bpcal1a:(d%,m%,y%)
			onerr off
			if (d%=29 and (y%<>4*(y%/4) or y%=1900 or y%=2100))
				raise 11
			endif
			inc::
			d%=d%+1
		until d%>29
		y%=y%+1
	until y%>2155
	rem print "End Of BPCAL1 Test"
	return
	
	err::
	onerr off
	if (d%=29 and (y%<>4*(y%/4) or y%=1900 or y%=2100))
		goto inc::
	endif
	fail::
	raise 11
endp


proc bpcal1a:(d%,m%,y%)
	local dn%,wk%,dys
	rem calc the day(0-6), day number and week number
	dn%=dow(d%,m%,y%) 
	wk%=week(d%,m%,y%)
	dys=days(d%,m%,y%)
endp


proc bpcal2:
	rem test of calendar functions over the full range of values
	rem runs for about 7.5 minutes under DOS.
	local d%,m%,y%
	local dn%,ds&,wkn%
	local num%,dys&,wk%
	local errcnt&
	local dt$(10),res$(30)
	
	rem setup:("bpcal2.lis")
	
	y%=1900 :dn%=1
	rem cls :print "Year="
	rem print "[ESC] to exit this test"
	while y%<2156
		rem if key=27
		rem	key :key :key :key :key :key :key :key :key :key :key :key
		rem	shutdown:(errcnt&)
		rem	return
		rem endif
		rem at 6,1 :print y%
		m%=1
		while m%<13
			d%=1
			while d%<33
				onerr err::
				num%=DOW(d%,m%,y%)
				dys&=DAYS(d%,m%,y%)
				wk%=WEEK(d%,m%,y%)
				onerr off
				if num%<>dn%  
					rem lprint num$(d%,2)+"/"+num$(m%,2)+"/"+num$(y%,4)
					rem lprint "DOW failed"
					rem lprint "Expected=";dn%,"Actual=";num%
					rem errcnt&=errcnt&+1
					rem at 1,4 :print "error count:",errcnt&
					rem if errcnt&>1000
					rem		print "TOO MANY ERRORS - ABORTING"
					rem 	shutdown:(errcnt&) :return
					rem endif
					raise 125
				endif
				dn%=dn%+1 :if dn%>7 :dn%=1 :endif
				if dys&<>ds& 
					rem lprint num$(d%,2)+"/"+num$(m%,2)+"/"+num$(y%,4)
					rem lprint "DAYS failed"
					rem lprint "Expected=";ds,"Actual=";dys
					rem errcnt&=errcnt&+1
					rem at 1,4 :print "error count:",errcnt&
					rem if errcnt&>1000
					rem 	print "TOO MANY ERRORS - ABORTING"
					rem 	shutdown:(errcnt&) :return
					rem endif
					raise 126
				endif
				ds&=ds&+1
				if dn%=2 : rem ie. was 1 for monday when WEEK() was done
					wkn%=wkn%+1
					if (m%=12 AND d%>=29) OR (m%=1 AND d%<=4 )
						wkn%=1
					endif
				endif
				if wk%<>wkn% 
					rem temporary fix while no OPL error given for > 28/12/2155 (see err::)
					if d%>28 and m%=12 and y%=2155
						goto inc::
					endif	
					rem lprint num$(d%,2)+"/"+num$(m%,2)+"/"+num$(y%,4)
					rem lprint "WEEK failed"
					rem lprint "Expected=";wkn%,"Actual=";wk%
					rem errcnt&=errcnt&+1
					rem at 1,4 :print "error count:",errcnt&
					rem if errcnt&>1000
					rem 	print "TOO MANY ERRORS - ABORTING"
					rem	shutdown:(errcnt&) :return
					rem endif
					raise 127
				endif
				inc::
				d%=d%+1
			endwh
			m%=m%+1
		endwh

		y%=y%+1
		REM Skip from start 1906 to 1990,
		REM and from start 2007 to 2095,
		REM Finally from start 2108 to 2140.
		if y%=1906
			y%=1990 :ds&=32872
		elseif y%=2007
			y%=2095 :ds&=71223 : dn%=6
		elseif y%=2108
			y%=2140 :ds&=87658 : dn%=5 : wkn%=53
		endif

	endwh
	msg::
	rem shutdown:(errcnt&)
	rem if errcnt&=0 
	rem	print "NO ERRORS"
	rem	pause -100 :key
	rem else
	rem	print "ERRORS in BPCAL2.LIS"
	rem	beep 3,1000
	rem	pause -100 :key
	rem	onerr off :	raise 1
	rem endif
	return
	
err::
	if (d%>28 and m%=12 and y%=2155)
		goto msg::
	endif
	if ((m%=1 and d%>31) or (m%=2 and d%>29) or (m%=3 and d%>31) or (m%=4 and d%>30) or (m%=5 and d%>31) or (m%=6 and d%>30) or (m%=7 and d%>31) or (m%=8 and d%>31) or (m%=9 and d%>30) or (m%=10 and d%>31) or (m%=11 and d%>30) or (m%=12 and d%>31))
		goto inc::
	endif
	if (m%=2 and (d%=29 and (y%<>4*(y%/4) or y%=1900 or y%=2100)))
		goto inc::
	endif 
	rem lprint num$(flt(d%),2)+"/"+num$(flt(m%),2)+"/"+num$(flt(y%),4)
	rem lprint "Invalid Date"
	rem errcnt&=errcnt&+1
	rem at 1,4 :print "error count:",errcnt&
	rem if errcnt&>1000
  rem  print "TOO MANY ERRORS - ABORTING"
  rem  shutdown:(errcnt&) :return
	rem endif
	raise 128
	goto inc::
endp


proc bpcal3:
	rem test calendar functions on some known dates
	local d%,m%,y%
	rem print "Start Of BPCAL3 Test"
	d%=1 :m%=1 :y%=1900
	if DOW(d%,m%,y%)<>1 :raise 11 :endif
	if DAYS(d%,m%,y%)<>0 :raise 11 :endif
	
	d%=13 :m%=2 :y%=1925
	if DOW(d%,m%,y%)<>5 :raise 11 :endif
	if DAYS(d%,m%,y%)<>9174 :raise 11 :endif
	
	d%=19 :m%=7 :y%=1944
	if DOW(d%,m%,y%)<>3 :raise 11 :endif
	if DAYS(d%,m%,y%)<>16270 :raise 11 :endif
	
	d%=27 :m%=11 :y%=1950
	if DOW(d%,m%,y%)<>1 :raise 11 :endif
	if DAYS(d%,m%,y%)<>18592 :raise 11 :endif
	
	d%=24 :m%=8 :y%=1965
	if DOW(d%,m%,y%)<>2 :raise 11 :endif
	if DAYS(d%,m%,y%)<>23976 :raise 11 :endif
	
	d%=31 :m%=5 :y%=1975
	if DOW(d%,m%,y%)<>6 :raise 11 :endif
	if DAYS(d%,m%,y%)<>27543 :raise 11 :endif
	
	d%=15 :m%=9 :y%=2000
	if DOW(d%,m%,y%)<>5 :raise 11 :endif
	if DAYS(d%,m%,y%)<>36782 :raise 11 :endif
	
	d%=2 :m%=6 :y%=2024
	if DOW(d%,m%,y%)<>7 :raise 11 :endif
	if DAYS(d%,m%,y%)<>45443 :raise 11 :endif
	
	d%=19 :m%=3 :y%=2053
	if DOW(d%,m%,y%)<>3 :raise 11 :endif
	if DAYS(d%,m%,y%)<>55960 :raise 11 :endif
	
	d%=16 :m%=12 :y%=2069
	if DOW(d%,m%,y%)<>1 :raise 11 :endif
	if DAYS(d%,m%,y%)<>62076 :raise 11 :endif
	
	rem print "End Of BPCAL3 Test"
endp

proc setup:(file$)
	rem lopen file$
	rem lprint "Output file:";file$
	rem lprint "Test started ";datim$
	rem lprint
endp

proc shutdown:(errors&)
	rem lprint
	rem lprint "Test finished ";datim$
	rem lprint "with ";errors&;" errors"
	rem lclose
endp

REM End of tBPCal.tpl
