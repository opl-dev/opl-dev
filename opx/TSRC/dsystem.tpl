INCLUDE "SYSTEM.oxh"
INCLUDE "date.oxh"


proc t:
	local i&,k&,c&,g$(225),n&,m&,capK&
	print"Visual tests for SYSTEM OPX"
	print"press any key to go through tests"
  get
	print
	print"Back Light On"
	SetBackLightOn:(1)
	get
	print"Backlight enquiry = ";BackLightOn&:
	print "(value should be -1  i.e. True)"
	get
	print
	print"Back Light Off"
	SetBackLightOn:(0)
	get
	print"Backlight enquiry = ";BackLightOn&:
	print "(value should be 0  i.e. False)"
	get
	print
	print"testing backlight timers"	
	SetBackLightOnTime:(100)
	SetBacklightBehavior:(0)
	SetAutoSwitchOffBehavior:(1)
	SetAutoSwitchOffTime:(500)
	ResetAutoSwitchOffTimer:
	Setactive:(1)
	do
		print"enter a display contrast between 0 and ";MaxDisplayContrast&:
		INPUT I&
		SetDisplayContrast:(i&)
		print "press any key to have another go or Esc to continue"
	UNTIL get=27

  trap delete "EonOpxTest"
	create"EonOpxTest",a,d%
	close
	print
	print"tests for RO, system, hidden files"
  if IsReadOnly&:("c:\EonOpxtest")
	   raise 1
	endif
	if IsHidden&:("c:\EonOpxtest")
		raise  2
	endif
  if	IsSystem&:("c:\EonOpxtest")
		raise 3
	endif
  setsystemfile:("c:\EonOpxtest",1)
	sethiddenfile:("c:\EonOpxtest",1)
	setreadonly:("c:\EonOpxtest",1)
	if IsReadOnly&:("c:\EonOpxtest") <> -1
	   raise 4
	endif
	if IsHidden&:("c:\EonOpxtest") <> -1
		raise  5
	endif
  if	IsSystem&:("c:\EonOpxtest")	<> -1
		raise 6
	endif
  setsystemfile:("c:\EonOpxtest",0)
	sethiddenfile:("c:\EonOpxtest",0)
	setreadonly:("c:\EonOpxtest",0)

	print "All clear - press a key to continue"
	get
	print
	print"enter a drive number (a:=0  b:=1  c:=2 etc)"
	input c&
	print "This drive has,"
	print "Size:      "; VolumeSize&:(c&)
	print	"Space free:";VolumeSpaceFree&:(c&)
	print	"Unique ID: ";VolumeUniqueID&:(c&)
	print	"Media Type:";MediaType&:(c&)
	print "All clear - press a key to continue"
	get
	print
	print "testing get/set file time"
	k&=DTNewdatetime&:(1989,1,1,1,1,1,0)
	trap delete"ppppppp"
	create"ppppppp",a,h&
	append
	close
	setfiletime:("ppppppp",k&)
	getfiletime:("ppppppp",k&)

	if  dtyear&:(k&) <> 1989
		raise 7
	endif
	if  dtday&:(k&) <> 1
		raise 8
	endif
	if  dtmonth&:(k&) <> 1
		raise 9
	endif
	if  dthour&:(k&) <> 1
		raise 10
	endif
	if  dtminute&:(k&) <> 1
		raise 11
	endif
	if   dtsecond&:(k&) <> 1
	print"get set file time secs bug"	rem raise 12
	endif
	if  dtmicro&:(k&) <> 0
		raise 13
	endif
	print "All Clear - press a key to continue"
get
cls
print"display task list"
displaytasklist:
	print "All Clear - press a key to continue"
get
print" testing MOD&: and XOR&:"

	if XOR&:(1,1) <> 0
  	raise 14
	endif
	if XOR&:(1,3) <> 2
  	raise 15
	endif
	if MOD&:(3,1) <> 0
  	raise 16
	endif
	if MOD&:(3,2) <> 1
	  raise 17
	endif

	print "All Clear - press a key to continue"
  print xor&:(3,-1)
  get

	setbackground:
	pause 5
	setforeground:
	k&=RUNAPP&:("opl","","RZ:\system\opl\toolbar.opo",2)
	logontothread:(k&,c&)
	n&=0
	setforeground:
	print "please wait..."
	pause 20
	if k&<>getthreadidfromopendoc&:("Z:\system\opl\toolbar.opo",n&)
		raise 18   
	endif
	if n&<>getnextwindowid&:(k&,0)
		raise 19
	endif
	pause 5
	setbackgroundbythread&:(k&,0)
	pause 5
	setforegroundbythread&:(k&,0)
	m&=0
	print"window group name of toolbar.opo =";getnextwindowgroupname$:(k&,m&)
	print"window id = ";getnextwindowid&:(k&,0)
onerr f1::
	print"endtask";endtask&:(k&,0)
f1::	
onerr off
	print"endtask";killtask&:(k&,0)
	iowaitstat32 c&
	setforeground:
	print"machine name = ";machinename$:
	machineuniqueid:(n&,m&)
	print"machine unique id = ",n&;m&
g1::
	print"press a key for battery info"
	get
	cls
e1::	
	print"main battery ";MainBatterystatus&:
	print"Backup battery ";BackupBatterystatus&:
	print "backlight present ";IsBackLightPresent&:
	print
capk::
	print "Capture key test - "
	print "press any key to go to background and then press Ctrl+Shift+a+any to capture and bring foreground" 
	get
	setbackground:
	capK& = capturekey&:(1,&000480,&00480)
	k&=get
	setForeground:
  if k&<>1
	  raise 21
  endif
	if (kmod and &6) <> &6
		raise 22
	endif

	print "Cancelling capture key"
	print "Press a key to go to background"
	print "Ctrl+Shift+a+any *shouldn't be captured .. you'll have to wait 10 seconds"
  get
  setbackground:
	cancelcapturekey:(capK&)
	pause -200
	setforeground:
  if (k&=1) and (kmod and &6) = &6
		raise 23
	endif		
	endp

rem **************************************************************************************************

PROC bugs113:
	local pause%
	
	PRINT "Test bugs in ROM 113"
	auto:
	inter:
	PRINT "End test bugs in ROM 113"
	pause pause%
ENDP

PROC auto:
	print "Automatic tests"
	tconst:
ENDP

proc tconst:
	local k%

	print "Check new consts ok"
	k%=KKeyMenu%
	print "KKeyMenu%=",k%
	if k%<>4150
		raise 1
	endif
	k%=KKeyDownArrow32%
	print "KKeyDownArrow32%=",k%
	if k%<>4106
		raise 2
	endif
endp

PROC inter:
	print "Interactive tests"
	tconsti:
	texternalpower:
	tbeep:
ENDP

proc tconsti:
	local ev&(16)
	
	while 1
		getevent32 ev&()
		if ev&(1)=27 :break :endif
		print ev&(1)
	endwh
endp

PROC texternalpower:
	local isExt&,wasExt&

	print "Test external power detection"
	wasExt&=2	rem will become -1 when present
	while get<>27
		isExt&=isExternalPowerPresent&:
		if isExt&<>wasExt&
			wasExt&=isExt&
			if isExt&
				print "External power present"
			else
				print "External power not present"
			endif
		else
			print ".";
		endif
	endwh
	print
ENDP

PROC tbeep:
	print "Set system beep setting loud and press key (esc quits)"
	while get<>27
		trybeep:
	endwh
	print "Set system beep setting quiet and press key (esc quits)"
	while get<>27
		trybeep:
	endwh
	print "Set system beep setting off and press key (esc quits)"
	while get<>27
		trybeep:
	endwh
ENDP

PROC trybeep:
	beep 10,500
ENDP

rem ***************************************************************************
rem popxirrv

include "system.oxh"
PROC ir:
	local k&,th%,ret%,tstat%,fn%,tm&,op&
	local r$(255)

	K&=&80000000
	print"testing ir connect"
	irdaconnecttoreceive:("IrTinyTP",8,k&)
	print"waiting to connect and receive"
	RET%=IOOPEN(th%,"tim:",-1)
	if ret%<0
		raise ret%
	endif
	fn%=1
	tm&=100
	ioc(th%,fn%,tstat%,tm&)
	iowait
	if tStat%<>-46
		print "Timed out"
		stop
	elseif k&<>&80000000
		print "Connected"
	endif

r$=irdaread$:
print len(r$),"'";r$;"'"
r$=irdaread$:
print len(r$),"'";r$;"'"

print
print "type some text"
input r$
irdawrite:(r$,k&)
iowaitstat32 k&

irdawaitfordisconnect:
print"disconnected"
get
ENDP

rem ******************************************************************
rem popxirsend