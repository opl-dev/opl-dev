REM pGetEvi.tpl
REM EPOC OPL interactive test code for GETEVENT32
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
EXTERNAL waitEvent:(aev&,sc&,mod&,rep&)

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pGetEvI", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pGetEvI:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("tgetev3")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
	hCleanUp%:("Reset")
ENDP


PROC Reset:
	rem Any clean-up code here.
ENDP


PROC tGetEv3:
	REM Skipping these tests, covered elsewhere.
	return
ENDP


PROC tGetEv2:
	PRINT "Press h once."
	waitEvent:(KEvKeyDown&,0,0,0) 
	waitEvent:(%h,&48,0,0) 
	waitEvent:(KEvKeyUp&,0,0,0) 
	
	PRINT "Change focus"
	waitEvent:(KEvFocusLost&,0,0,0) 
	waitEvent:(KEvFocusGained&,0,0,0) 
	
	dinit "complete" :dialog
ENDP


PROC waitEvent:(aev&,sc&,mod&,rep&)
	LOCAL ev&(16)
	GETEVENT32 ev&()
	PRINT HEX$(ev&(1))
	IF ev&(1)<>aev&
		print "bad event",
		print "expected=";HEX$(aev&),
		print "actual=";HEX$(ev&(1))
	endif
ENDP



proc tgetev:
	global filter%,mask%,ret%
	global ev&(16),ev1&
	mask%=$7
	filter%=0
	ret%=1
		
	print "OPLER1 EVENT MONITOR"
	print "Can now handle pointer events"
	pause 20

	setPointerFilter:	
	while ret%=1
		print
  		print "Waiting for event..."
		print "(Press Esc to end)"
    	ret%=waitEv:
	endwh
	pause 30
endp

proc setPointerFilter:
	print
	print "Do you want to set a pointer filter?"
	if get=$79
		print "Enter pointer filter mask:"
		input mask%
		print "Enter pointer filter:"
		input filter%
	endif
	pointerfilter filter%,mask%
endp

proc waitEv:
	do
		getEvent32 ev&()
		ev1&=ev&(1)
		if (ev1& and $400)=0
			if ev1&=27 : return 0 : endif
			print
			print "Keypress"
			print "key        = &";hex$(ev&(1)),chr$(ev&(1))
			print "Time stamp = ";ev&(2)
			print "Scan code  = &";hex$(ev&(3))
			print "Modifier   = &";hex$(ev&(4))
			print "Repeats    = ";ev&(5)/256
		else
			print
			print "Event &";hex$(ev1&),"received"
		
		  vector ev1&-$400
		  		fGain,fLoss,swchOn,term,unknown
		  		kDown,kUp,pEvent,pEnter,pExit
		  endv
  			print
	  		print "Illegal event" 	  
		  while 1
  				break
fGain::
	    print "Focus Gain"
	    	print "Time stamp = ";ev&(2)
	    break
fLoss::
      print "Focus Loss"
      print "Time stamp = ";ev&(2)
	    break
swchOn::
	    print "Machine Switched On"
	    print "Time stamp = ";ev&(2)
	    break
term::
	    print "Terminated"
	    vector ev&(3)
		    	shutd,bUStart,bUComp
	    	endv
	    print "Unknown reason"
	    break
	    shutd::
	    				   print "Shutdown"
	    				   break
	    	bUStart::
	    					 print "Backup Starting"
	    					 break
	    	bUComp::
	    					 print "Backup Complete"
	    					 break
unknown::
			print "Unknown Event"
			break	    					 
kDown::
			print "Key Down"
			print "Time Stamp = ";ev&(2)
			print "Scan Code  = &";hex$(ev&(3))
			print "Modifiers  = &";hex$(ev&(4))
			break
kUp::
			print "Key Up"
			print "Time Stamp = ";ev&(2)
			print "Scan Code  = &";hex$(ev&(3))
			print "Modifiers  = &";hex$(ev&(4))
			break
pEvent::
			print "Pointer Event"
			print "Time Stamp = ";ev&(2)
			print "Window ID  = &";hex$(ev&(3))
			print "Modifiers  = &";hex$(ev&(5))
			print "x-coord    = ";ev&(6)
			print "y-coord    = ";ev&(7)
			print "Type is "		
			vector ev&(4)+1
				zero,one,two,three,four,five,six,seven
			endv
			print "Unknown type"
			break
			zero::
							print "Pen or button 1 down"
							break
			one::
							print "Pen or button 1 up"
							break
			two::
							print "Button 2 down"
							break
			three::
							print "Button 2 up"
							break
			four::
							print "Button 3 down"
							break
			five::
							print "Button 3 up"
							break
			six::
							print "Drag"
							break
			seven::
		    				print "Move"
	  		  				break
pEnter::
			print "Pointer enter"
			print "Time stamp = ";ev&(2)
			print "Window ID  = &";hex$(ev&(3))
			break
pExit::	   
			print "Pointer exit"
			print "Time stamp = ";ev&(2)
			print "Window ID  = &";hex$(ev&(3))
			break
		endwh
	endif
	until testEvent	
	return 1
endp


REM End of pGetEvI.tpl

