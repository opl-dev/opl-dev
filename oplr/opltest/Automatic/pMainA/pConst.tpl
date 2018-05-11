REM PConst.tpl
REM EPOC Opl test code for Opler1 CONSTs

rem Tests const declarations

const KOne%=1
const KTwo%=2
const KThree%=3
const KMinusOne%=-1
const KMinusTwo%=-2
const KMinusThree%=-3
const KIntMax%=32767
const KIntMin%=$8000 rem won't translate in decimal
const KOne&=1
const KTwo&=2
const KThree&=3
const KMinusOne&=-1
const KMinusTwo&=-2
const KMinusThree&=-3
const KLongMax&=2147483647
const KLongMin&=&80000000 rem won't translate in decimal
const KOne=1.0
const KTwo=2.0
const KThree=3.0
const KMinusOne=-1.0
const KMinusTwo=-2.0
const KMinusThree=-3.0
const KFloatMax=1.7976931348263157e+308
const KFloatMin=5E-324
const KFloatLargeNeg=-1.7976931348263157e+308
const KA$="a"
const KB$="b"
const KC$="c"
const KAlpha$="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
const KHello$="Hello"
const KWorld$=" World!"
const KSymbols$="!œ$%^&*()_+:@~#{}"

DECLARE EXTERNAL

INCLUDE "hUtils.oph"
PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("pConst", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pConst:
	hRunTest%:("tNumeric")
	hRunTest%:("tString")
	hRunTest%:("tError")
endp

proc tNumeric:
	rem a few simple tests to check constants behave as expected!
	
	if KOne%<>1 : raise 100 : endif
	if KTwo%<>2 : raise 101 : endif
	if KThree%<>3 : raise 102 : endif
	if KIntMax%<>32767 : raise 103 : endif
	
	if KMinusOne%<>-1 : raise 104 : endif
	if KMinusTwo%<>-2 : raise 105 : endif
	if KMinusThree%<>-3 : raise 106 : endif
	if KIntMin%<>-32768 : raise 107 : endif
	
	REM print "KOne% + KTwo% = ";KOne%+KTwo%
	if (KOne%+KTwo%)<>KThree% : raise 1 :endif
	REM print "KTwo% + KTwo% = ";KTwo%+KTwo%
	if (KTwo%+KTwo%)<>4 : raise 2 :endif
	REM print "KOne% + KOne% = ";KOne%+KOne%
	if (KOne%+KOne%)<>KTwo% : raise 3 :endif
	REM print "KOne% + KThree% = ";KOne%+KThree%
	if (KOne%+KThree%)<>4 : raise 4 :endif
	REM print "KTwo% + KThree% = ";KTwo%+KThree%
	if (KTwo%+KThree%)<>5 : raise 5 :endif
	REM print "KOne% * KTwo% = ";KOne%*KTwo%
	if (KOne%*KTwo%)<>KTwo% : raise 6 :endif
	REM print "KTwo% * KTwo% = ";KTwo%*KTwo%
	if (KTwo%*KTwo%)<>4 : raise 7 :endif
	REM print "KOne% * KOne% = ";KOne%*KOne%
	if (KOne%*KOne%)<>KOne% : raise 8 :endif
	REM print "KOne% * KThree% = ";KOne%*KThree%
	if (KOne%*KThree%)<>KThree% : raise 9 :endif
	REM print "KTwo% * KThree% = ";KTwo%*KThree%
	if (KTwo%*KThree%)<>6 : raise 10 :endif
	REM print "KOne% / KTwo% = ";KOne%/KTwo%
	if (KOne%/KTwo%)<>0 : raise 11 :endif
	REM print "KTwo% / KTwo% = ";KTwo%/KTwo%
	if (KTwo%/KTwo%)<>KOne% : raise 12 :endif
	REM print "KOne% / KOne% = ";KOne%/KOne%
	if (KOne%/KOne%)<>KOne% : raise 13 :endif
	REM print "KThree%/KOne% = ";KThree%/KOne%
	if (KThree%/KOne%)<>KThree% : raise 14 :endif
	REM print "KThree% / KTwo% = ";KThree%/KTwo%
	if (KThree%/KTwo%)<>KOne% : raise 15 :endif
	REM print "KOne% - KTwo% = ";KOne%-KTwo%
	if (KOne%-KTwo%)<>-KOne% : raise 16 :endif
	REM print "KTwo% - KTwo% = ";KTwo%-KTwo%
	if (KTwo%-KTwo%)<>0	: raise 17 :endif
	REM print "KOne% - KOne% = ";KOne%-KOne%
	if (KOne%-KOne%)<>0 : raise 18 :endif
	REM print "KOne% - KThree% = ";KOne%-KThree%
	if (KOne%-KThree%)<>-KTwo% : raise 19 :endif
	REM print "KTwo% - KThree% = ";KTwo%-KThree%
	if (KTwo%-KThree%)<>-KOne% : raise 20 :endif
	
	if KOne&<>1 : raise 200 : endif
	if KTwo&<>2 : raise 201 : endif
	if KThree&<>3 : raise 202 : endif
	if KLongMax&<>2147483647 : raise 203 : endif

	if KMinusOne&<>-1 : raise 204 : endif
	if KMinusTwo&<>-2 : raise 205 : endif
	if KMinusThree&<>-3 : raise 206 : endif
	if KLongMin&<>-2147483648 : raise 207 : endif
	
	REM print "KOne& + KTwo& = ";KOne&+KTwo&
	if (KOne&+KTwo&)<>KThree& : raise 21 :endif
	REM print "KTwo& + KTwo& = ";KTwo&+KTwo&
	if (KTwo&+KTwo&)<>&4 : raise 22 :endif
	REM print "KOne& + KOne& = ";KOne&+KOne&
	if (KOne&+KOne&)<>KTwo& : raise 23 :endif
	REM print "KOne& + KThree& = ";KOne&+KThree&
	if (KOne&+KThree&)<>&4 : raise 24 :endif
	REM print "KTwo& + KThree& = ";KTwo&+KThree&
	if (KTwo&+KThree&)<>&5 : raise 25 :endif
	REM print "KOne& * KTwo& = ";KOne&*KTwo&
	if (KOne&*KTwo&)<>KTwo& : raise 26 :endif
	REM print "KTwo& * KTwo& = ";KTwo&*KTwo&
	if (KTwo&*KTwo&)<>&4 : raise 27 :endif
	REM print "KOne& * KOne& = ";KOne&*KOne&
	if (KOne&*KOne&)<>KOne& : raise 28 :endif
	REM print "KOne& * KThree& = ";KOne&*KThree&
	if (KOne&*KThree&)<>KThree& : raise 29 :endif
	REM print "KTwo& * KThree& = ";KTwo&*KThree&
	if (KTwo&*KThree&)<>&6 : raise 30 :endif
	REM print "KOne& / KTwo& = ";KOne&/KTwo&
	if (KOne&/KTwo&)<>&0 : raise 31 :endif
	REM print "KTwo& / KTwo& = ";KTwo&/KTwo&
	if (KTwo&/KTwo&)<>KOne& : raise 32 :endif
	REM print "KOne& / KOne& = ";KOne&/KOne&
	if (KOne&/KOne&)<>KOne& : raise 33 :endif
	REM print "KThree&/KOne& = ";KThree&/KOne&
	if (KThree&/KOne&)<>KThree& : raise 34 :endif
	REM print "KThree& / KTwo& = ";KThree&/KTwo&
	if (KThree&/KTwo&)<>KOne& : raise 35 :endif
	REM print "KOne& - KTwo& = ";KOne&-KTwo&
	if (KOne&-KTwo&)<>-KOne& : raise 36 :endif
	REM print "KTwo& - KTwo& = ";KTwo&-KTwo&
	if (KTwo&-KTwo&)<>&0	: raise 37 :endif
	REM print "KOne& - KOne& = ";KOne&-KOne&
	if (KOne&-KOne&)<>&0 : raise 38 :endif
	REM print "KOne& - KThree& = ";KOne&-KThree&
	if (KOne&-KThree&)<>-KTwo& : raise 39 :endif
	REM print "KTwo& - KThree& = ";KTwo&-KThree&
	if (KTwo&-KThree&)<>-KOne& : raise 40 :endif
	
	if KOne<>1.0 : raise 300 : endif
	if KTwo<>2.0 : raise 301 : endif
	if KThree<>3.0 : raise 302 : endif
	if KFloatMax<>1.7976931348263157e+308 : raise 303 : endif
	if KFloatMin<>5E-324 : raise 304 : endif
	if KFloatLargeNeg<>-1.7976931348263157e+308 : raise 3041 : endif

	if KMinusOne<>-1.0 : raise 305 : endif
	if KMinusTwo<>-2.0 : raise 306 : endif
	if KMinusThree<>-3.0 : raise 307 : endif

	REM print "KOne + KTwo = ";KOne+KTwo
	if (KOne+KTwo)<>KThree : raise 41 :endif
	REM print "KTwo + KTwo = ";KTwo+KTwo
	if (KTwo+KTwo)<>4.0 : raise 42 :endif
	REM print "KOne + KOne = ";KOne+KOne
	if (KOne+KOne)<>KTwo : raise 43 :endif
	REM print "KOne + KThree = ";KOne+KThree
	if (KOne+KThree)<>4.0 : raise 44 :endif
	REM print "KTwo + KThree = ";KTwo+KThree
	if (KTwo+KThree)<>5.0 : raise 45 :endif
	REM print "KOne * KTwo = ";KOne*KTwo
	if (KOne*KTwo)<>KTwo : raise 46 :endif
	REM print "KTwo * KTwo = ";KTwo*KTwo
	if (KTwo&*KTwo&)<>4.0 : raise 47 :endif
	REM print "KOne * KOne = ";KOne*KOne
	if (KOne*KOne)<>KOne : raise 48 :endif
	REM print "KOne * KThree = ";KOne*KThree
	if (KOne*KThree)<>KThree : raise 49 :endif
	REM print "KTwo * KThree = ";KTwo*KThree
	if (KTwo*KThree)<>6.0 : raise 50 :endif
	REM print "KOne / KTwo = ";KOne/KTwo
	if (KOne/KTwo)<>0.5 : raise 51 :endif
	REM print "KTwo / KTwo = ";KTwo/KTwo
	if (KTwo/KTwo)<>KOne : raise 52 :endif
	REM print "KOne / KOne = ";KOne/KOne
	if (KOne/KOne)<>KOne : raise 53 :endif
	REM print "KThree/KOne = ";KThree/KOne
	if (KThree/KOne)<>KThree : raise 54 :endif
	REM print "KThree / KTwo = ";KThree/KTwo
	if (KThree/KTwo)<>1.5 : raise 55 :endif
	REM print "KOne - KTwo = ";KOne-KTwo
	if (KOne-KTwo)<>-KOne : raise 56 :endif
	REM print "KTwo - KTwo = ";KTwo-KTwo
	if (KTwo-KTwo)<>0.0	: raise 57 :endif
	REM print "KOne& - KOne& = ";KOne&-KOne&
	if (KOne-KOne)<>0.0 : raise 58 :endif
	REM print "KOne - KThree = ";KOne-KThree
	if (KOne-KThree)<>-KTwo : raise 59 :endif
	REM print "KTwo - KThree = ";KTwo-KThree
	if (KTwo-KThree)<>-KOne : raise 60 :endif	
endp

proc tError:

	rem The following will not translate
	rem test that you can't reassign to a const
	rem KOne%=2
	
	rem test that you can't define a const inside a proc	
	rem const KNoInProc=1.0
	
	rem test that can't declare local/global of same name
	rem local KOne&
endp

proc tString:
	rem some simple string const tests
	REM print
	REM print "The following should print a global greeting!"
	REM print KHello$+KWorld$
	if KHello$+KWorld$<>"Hello World!" : raise 1 : endif
	REM print "This should be the alphabet"	
	REM print KAlpha$
	if KAlpha$<>"ABCDEFGHIJKLMNOPQRSTUVWXYZ" : raise 2 : endif
	REM print
	REM print "Here are some symbols"
	REM print KSymbols$
	if KSymbols$<>"!œ$%^&*()_+:@~#{}" : raise 3 : endif
	REM print
	REM print KA$
	if KA$<>"a" : raise 4 : endif
	REM print KB$
	if KB$<>"b" : raise 5 : endif
	REM print KC$
	if KC$<>"c" : raise 6 : endif
	REM print KA$+KB$
	if KA$+KB$<>"ab" : raise 7 : endif
	REM print KA$+KB$+KC$
	if KA$+KB$+KC$<>"abc"	 : raise 8 : endif
	REM pause 20
endp
