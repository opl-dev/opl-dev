REM pPtrFilter.tpl
REM EPOC OPL interactive test code for POINTERFILTER.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pPtrFilter", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pPtrFilter:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("tPointFilt")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
	rem hCleanUp%:("CleanUp")
ENDP



proc tPointFilt:
	global filter%,mask%,ret%
	hLog%:(KLogAlways%,"Skipping pointerfilter tests.")
	return
	
	filter%=0
	mask%=7
	ret%=0
	
	print "Test Opler1 PointerFilter"
	print
	print "This test will raise errors if an event which has been filtered out"
	print "by the pointer filter is recognised by getevent32"
	print
		
	setFilt:
	getEvent:
	
	print
	print "Test Opler1 PointerFilter Finished OK"
	pause -30
endp


PROC CleanUp:
	REM Inter-test action from old pMainI control module.
	pointerfilter 0,0
ENDP


proc setFilt:
	print "Enter value for pointer filter mask "
	input mask%
	print "Enter value for pointer filter "
	input filter%
	pointerfilter filter%,mask%
endp

proc getEvent:
	do
		print
	  print "Waiting for pointer event..."
	  print "(Hit Esc to terminate)"
	  ret%=waitEv:
	  if ret%=1 : break : endif
	until 0
endp

proc waitEv:
	local ev&(16),ev1&

	do
		getEvent32 ev&()
		ev1&=ev&(1)
		
		if (ev1& and $fff)=$01b : return 1 : endif
		
		vector ev1&-&407
			pEvent,pEnter,pExit
		endv
		
		print "Not a pointer event"
		while 1
		break
		
pEvent::
				print
				print "Pointer Event"
				print "Time Stamp = ";ev&(2)
				print "Window ID  = ";ev&(3)
				print "Modifiers  = ";ev&(5)
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
						if ((filter% and mask%) and $f)=$4
							print "Drag events should be filtered out"
							raise 100
						endif
						break
		seven::
	    				print "Move"
						if ((filter% and mask%) and $f)=$2
							print "Move events should be filtered out"
							raise 101
						endif

	    				break
pEnter::
				print
				print "Pointer enter"
				print "Time stamp = ";ev&(2)
				print "Window ID  = ";ev&(3)
				if ((filter% and mask%) and $f)=$1
					print "Enter events should be filtered out"
					raise 103
				endif
				break
pExit::	   
				print
				print "Pointer exit"
				print "Time stamp = ";ev&(2)
				print "Window ID  = ";ev&(3)
				if ((filter% and mask%) and $f)=$1
					print "Exit events should be filtered out"
					raise 104
				endif

				break
	endwh
	until testEvent
	return 0
endp
