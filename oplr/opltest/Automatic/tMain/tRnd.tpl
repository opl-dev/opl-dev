REM tRnd.tpl
REM EPOC OPL automatic test code for RND.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "tRnd", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tRnd:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("RNDtest")
endp


PROC RNDtest:
	local i%,withinRange%,bad%
	i%=2
	DO
		IF Chi2%:
			withinRange%=withinRange%+1
			IF withinRange%=1
				REM Good -- no need to waste time testing further...
				RETURN
			ENDIF
		ELSE
			bad%=bad%+1
		ENDIF
		i%=i%-1
		rem print bad%,withinRange%,100.0*bad%/withinRange%
	UNTIL i%=0
	REM Get this far, means not enough chi2: results are good...
	RAISE 1
ENDP


REM
REM Chi-square goodness-of-fitness test.
REM
REM From Sedgewick's Algorithms - Chapter 35 "Random numbers":
REM
REM The chi2 test checks whether the numbers produced by the random number
REM generator are spread out. For N positive numbers less than R, expect
REM to get N/R numbers of each value -- but the frequencies of occurance
REM should not be the same. Calculating whether a sequence is distributed
REM randomally is simple:
REM
REM		Sum the squares of the frequencies of occurance of each value,
REM		scaled by the expected frequency, then subtract the size of the
REM		sequence.
REM
REM This is the chi2 statistic: if close to R, the numbers are random
REM (Where 'close' is within 2*SQR(R) of R.)
REM

PROC chi2%:
	rem Test RND using Chi squared goodness of fit test.
	global o(201) rem observed frequencies
	local e,d,chi2
	local i%,index%,cells%,samples%
	local minI,maxI,minO,maxO,gMarg%
	local ret%

	gCLS
	gupdate off

	cells%=201
	e=50
	samples%=cells%*e
	gMarg%=5
	do
		index%=rnd*cells%+1
		o(index%)=o(index%)+1
		gAt gMarg%+index%,79-o(index%) :gLineBy 0,0
		i%=i%+1
	until i%=samples%
	gupdate on

	minO=samples% :maxO=0
	i%=1
	while i%<=cells%
		d=o(i%)-e
		if o(i%)<minO     :minO=o(i%) :minI=i%
		elseif o(i%)>maxO :maxO=o(i%) :maxI=i%
		endif
		chi2=chi2 + (d*d/e)
		i%=i%+1
	endwh

rem	at 1,10
rem	print "Chi^2=";chi2
rem	print "Critical region at 80% >= 226.02"
rem	Print "Minimum ";minI;":";minO,"Maximum ";maxI;":";maxO
rem	print "VAR=";var(o(),cells%)
rem	print "STD=";std(o(),cells%)
rem	print "SUM=";sum(o(),cells%)
rem	print "MEAN=";mean(o(),cells%)
rem	print "MIN=";min(o(),cells%)
rem	print "MAX=";max(o(),cells%)
	IF ABS(chi2-200)<26.02
		return kTrue%
	ELSE
		return kFalse%
	ENDIF
endp

REM End of tRnd.tpl
