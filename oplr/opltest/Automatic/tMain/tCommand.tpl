REM tCommand.tpl
REM EPOC OPL automatic test code for various non-interactive commands.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tCommand", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tCommand:
	global fcb%
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tPoke")
	hRunTest%:("tLopen")
	hRunTest%:("tRand")
	hRunTest%:("tAt")
	hRunTest%:("tBeep")
	hRunTest%:("tCursor")
	hRunTest%:("tClsN")
	hRunTest%:("tSignal")
	hCleanUp%:("CleanerUpper")
endp

PROC CleanerUpper:
	local err%
	rem just in case it's still open...
	err%=ioclose(fcb%)
	TRAP DELETE "\lopen.txt"
ENDP

REM--------------------------------------------------------------------------
REM Start of non-interactive commands

proc tPoke:
  local p%,i%,l&,d,s$(10),j%
  rem j% must come after s$(): s$ poked beyond end!!!
	
  print
	rem print "Poke commands"
  i%=-1
  p%=addr(i%)
  pokeb p%,0
  if i%<>$ff00   :raise 1 :endif
  i%=-1
  pokeb p%+1,0
  if i%<>$00ff   :raise 2 :endif
  pokeb p%,0
  if i%<>0       :raise 101 :endif
  pokeb p%+1,255
  if i%<>$ff00   :raise 3 :endif
	
  onerr err1::
  pokeb p%,i%
  raise 4        : rem 255 maximum
	err1::
  onerr err2::
  pokeb p%,256
  raise 5        : rem 255 maximum
	err2::
  onerr off
	
  i%=0
  pokew p%,255
  if i%<>255 :raise 6 :endif
  pokew p%,-1
  if i%<>-1 :raise 7 :endif
  pokew p%,0
  if i%<>0 :raise 8 :endif
	
  l&=0
  p%=addr(l&)
  pokel p%,1
  if l&<>1 :raise 9 :endif
  pokel p%,-1
  if l&<>-1 :raise 10 :endif
  pokel p%,0
  if l&<>0 :raise 11 :endif
	
  d=0.0
  p%=addr(d)
  pokef p%,1.0
  if d<>1.0 :raise 12 :endif
  pokef p%,9e99
  if d<>9e99 :raise 13 :endif
  pokef p%,0.0
  if d<>0.0 :raise 14 :endif
	
  s$=""
  p%=addr(s$)
  poke$ p%,"1234567890"
  if s$<>"1234567890" :raise 15 :endif
  poke$ p%,"12345678901"
  if s$<>"12345678901" :raise 16 :endif
  poke$ p%,""
  if s$<>"" :raise 17 :endif
endp


proc tRand:
  local d1,d2,d3,d4
	
rem  print
rem	print "Randomize command"
  randomize &1
  d1=rnd
  d2=rnd
	
	rem Very unlikely
  if d1=d2 : raise 1 : endif 
	randomize &1
	d3=rnd
	d4=rnd
	if d1<>d3 : raise 2 : endif
	if d2<>d4 : raise 3 : endif
endp


proc tAt:
	rem Non-interactive procedure
	local row%,col%
	
rem	print
rem	print "AT command"
	while 1
		col%=rnd*100-10    : rem also generates some bad arguments to be caught
		row%=rnd*40-10
		if row%<0 :return :endif
		onerr err1::
		at col%,row%
		rem print "At"
		err1::
		if err
			onerr off
			if err <> KErrInvalidArgs% : raise 1 : endif
		endif
	endwh
endp


proc tClsN:
	local i%,times%
	
rem	print
rem	print "CLS command"
rem	print "-----------"
	times%=10
rem	print "Performing CLS",times%,"times"
rem	pause pause% :key
	while i%<times%
		cls
rem		print i%
rem		pause 5
		i%=i%+1
	endwh
endp


proc tBeep:
	local i%
	rem Should play a tune here too
	i%=1
	do
		beep 2,i%
		i%=i%+500
	until i%>=10000
endp


proc tCursor:
	local i%
rem 	SubTest:("CURSOR ON/OFF command")
	while i%<100
		i%=i%+1
		if rnd < 0.5
			cursor off
		else
			cursor on
		endif
	endwh
endp


proc tSignal:
rem	SubTest:("IOSIGNAL command")
rem	print "Doing IoSignal..."
	iosignal
rem	print "Doing IoWait..."
	iowait
endp


proc tLopen:
	local err%,message$(255),d,i%,l&
	
rem	print
rem	print "Testing LOPEN/LCLOSE command for file only"
rem	print "-----------------------------------------"
	lopen "LOPEN.TXT"
	message$="LOPEN and LPRINT write text to a file ok !"
	d=1.2
	i%=34
	l&=56
rem	print "Now do LPRINT for all variable types"
rem	print "LPRINTing","""";message$;""""
	lprint message$
rem	print "LPRINTing",d :rem This also ensures PRINT doesn't print to the file!
	lprint d
rem	print "LPRINTing",i%
	lprint i%
rem	print "LPRINTing",l&
	lprint l&
	lclose
	err%=ioOpen(fcb%,"LOPEN.TXT",$220) :rem mode open|text|random
	if err%
	rem	print err$(err)
	rem	get
		raise 1
	endif
	err%=readChk:(fcb%,message$)
	if err% :raise 2 :endif
	err%=readChk:(fcb%,"1.2")
	if err% :raise 3 :endif
	err%=readChk:(fcb%,"34")
	if err% :raise 4 :endif
	err%=readChk:(fcb%,"56")
	if err% :raise 5 :endif
	err%=ioClose(fcb%)
	if err% 
		rem print err$(err)
		rem pause pause%
		raise 4
	endif
endp

proc readChk:(aHand%,expect$)
	local err%,buf$(255)
	rem Read line from file and check same as expected
	rem Returns -1 if not as expected else 0
	err%=ioRead(aHand%,addr(buf$)+1+KOplAlignment%,255)
	if err%<0
		rem print err$(err)
		rem pause pause%
		raise 1
	endif
	pokeb addr(buf$),err%
	rem print "Read","""";buf$;"""","back from file"
	if buf$<>expect$
		return(-1)
	endif
endp

REM End of tCommand.tpl
