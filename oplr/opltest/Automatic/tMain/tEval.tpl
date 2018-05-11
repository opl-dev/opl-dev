REM tEval.tpl
REM EPOC OPL automatic test code for EVAL keyword.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tEval", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tEval:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("doEval1")
	hRunTest%:("doEval2")
endp


PROC doEval1:
	global x
	local expr$(255)
	global si%,sl&,sd,ss$(10)
	global int%(3),l&(2),d(2)
	global s$(3,4)
	local r%,r

	rem nice simple eval test...
	r=eval("1+1")
	if r<>2.0
		raise 111
	endif
	hlog%:(khlogAlways%,"ERROR: !!TODO In tEval.tpl, defect EDNRANS-4K8LFE Skipping rich EVAL() tests for now...") :return

	r=eval("add:(2.0,3.0)")
	if r<>5.0
		raise 1002
	endif
	ss$="1"
	si%=10
	sl&=100
	sd=1000
	r=eval("val(ss$)+si%+sl&+sd")
	rem print r
	if r<>1111.0
		raise 1000
	endif

	s$(1)="1"
	s$(2)="02"
	s$(3)="003"
	r%=EVAL("len(s$(1))")
	rem print r%
	if r%<>1
		raise 1
	endif
	r%=EVAL("len(s$(2))")
	rem print r%
	r%=EVAL("len(s$(3))")
	rem print r%
	if r%<>3
		raise 2
	endif
	r=EVAL("val(s$(1))")
	rem print r
	r=EVAL("val(s$(2))")
	rem print r
	if (r<>2) or (r<>val(s$(2)))
		raise 3
	endif
	r=EVAL("val(s$(3))")
	rem print r

	onerr e1
	rem Need this PRINT to hit the error.
	print eval("len(s$(4))")
	onerr off
	if err<>-111
		raise 4
	endif
e1::
	onerr off
	int%(1)=1
	int%(2)=2
	int%(3)=3
	rem print eval("int%(1)")
	rem print eval("int%(2)")
	r%=eval("int%(3)")
	rem print r%
	if r%<>3
		raise 100
	endif		

	rem print "Access beyond array"
	onerr e2
	rem Need this print for error.
	print eval("int%(4)")	
	onerr off
	raise 5
e2::
	onerr off

	d(1)=1e307
	l&(2)=32768

	hLog%:(KhLogAlways%,"ERROR: !!TODO ER5 defect EDNRANS-4FMG2Y Skipping over eval(""d(1)"").")
	IF 0
		REM Appears that attempting to eval d(1) will
		REM panic OPLR with Kern-exec 3.

		if eval("d(1)")<>1e307
			raise 101
		endif

		if eval("l&(2)")<>32768
			raise 102
		endif	
	ENDIF
	
	rem ExpressionCalculator:

	r=0.37571931012392753
	if eval("sqr(.141165)")<>r
		raise 200
	endif
endp


PROC ExpressionCalculator:
	local expr$(255)
	gUpdate off
	while 1
		dInit "Calculator"
			dEdit expr$,"Expression",30
		if dialog=0
			break
		endif
		gCls
		gAt 0,gHeight/2
		onerr e3
		while gx<640
			x=gX
			gLineTo gX+1,gHeight/2+(gHeight/2)*eval(expr$)
			continue
e3::
			gMove 1,0
		endwh
	endwh
endp


proc doEval2:
  local ret

  if eval("1+2")<>3 rem simple case
    raise 1
  endif
	hlog%:(khlogAlways%,"ERROR: !!TODO In tEval.tpl, defect EDNRANS-4K8LFE Skipping rich EVAL() tests for now...") :return
  if eval("@(""inc""):($2)")<>3 rem indirect procedure call
    raise 2
  endif
  if eval("inc:($3)")<>4 rem procedure call
    raise 3
  endif
  onerr e1
  ret = eval("1/0") rem runtime error
  raise 4
e1::
  onerr off
  if err<>-8
    raise 5
  endif
  onerr e2
  ret=eval("1+")  rem translate error
  raise 6
e2::
  onerr off
  rem print "Finished some EVAL tests"
	REM get
endp


proc inc:(val%)
  return val%+1
endp


proc add:(x,y)
	return x+y
endp

REM End of tEval.tpl 
