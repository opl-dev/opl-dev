REM pIO.tpl
REM EPOC OPL interactive test code for I/O functions.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pIO", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pIO:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("tIOc")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
rem	hCleanUp%:("Reset")
ENDP

PROC Reset:
	rem Any clean-up code here.
ENDP


proc tIoc:
  local tHand%,tStat%,ret%,tout&
  local kStat%,key%(3)
  local signals%,i%
	local start%,end%

  busy "Testing IOC/IOCANCEL...",3
  tHand%=$1234      :rem Ensure passed to IOOPEN in TD
  ret%=ioopen(tHand%,"TIM:",-1)
  if ret%<0 :print "Failed to open tim:" :get :return :endif
  tout&=10                      :rem 1 second timeout
  print "Testing IOC() - 1 second timer"
 
  print datim$
  start%=SECOND
  ioc(tHand%,1,tStat%,tout&)
  iowaitstat tStat%
  end%=SECOND
  print datim$
  IF start%>end% :end%=end%+60 :ENDIF
  IF end%-start%>2 : print (end%-start%) : RAISE 1 :ENDIF
  print "Ok"
  rem pause -30 :key

  dinit "Waiting for sync keypress"
  dBUTTONS "Continue",13
  dialog

  cls
  tOut&=30   :rem 3 seconds
	do
    ioc(tHand%,1,tStat%,tout&)
    key%(1)=0
    rem ioc(-2,1,kStat%,key%())
    keya(kStat%,key%())
    cls
		if i%=0
			print "Press key after 2 seconds"
		else
			print "Wait 3 seconds for timeout"
		endif
	  rem print "<esc> quits test"
    do
      iowait
      at 1,4
      print rept$(" ",80)
      print rept$(" ",80)
      at 1,4
			if tStat%<>-46
				if tStat%<>0
					rem beep 5,500
					print "Timer error",err$(tStat%)
					RAISE 10
				else
					print "Timer expired"
					IF i%=0	 rem Keypress should have interrupted timer...
						RAISE 20
					ENDIF
				endif
        rem iocancel(-2)
        rem iowaitstat kstat%
        keyc(kStat%)
        print "Key read cancelled"
				break
      elseif kStat%<>-46
        if kStat%<>0
          rem beep 5,500
          print "Key read error:",err$(kStat%)
          RAISE 40
        else
          print "Key pressed - ";key%(1),chr$(key%(1)),"modifier=$";hex$(key%(2) and $ff),"repeats=";key%(2)/256
          IF i%=1
          	REM Not expecting keypress during second test.
          		RAISE 30
          ENDIF
        endif
        iocancel(tHand%)
        iowaitstat tstat%
        print "Timer cancelled"
        break
      else
        signals%=signals%+1
        rem beep 5,300
        hLog%:(KhLogAlways%,"!!ERROR: tIOc: Stray signal death!!!")
        rem RAISE 50
      endif
    until 0
    while signals% :signals%=signals%-1 :iosignal :endwh
    if key%(1)=27 :break :endif
    i%=i%+1
  until i%=2
  ioclose(tHand%)
  busy ""
  cls
  REM Allow the harness script time to kill this program...
  PAUSE 100
endp


REM End of pIO.tpl
