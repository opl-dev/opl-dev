REM pMPopup.tpl
REM EPOC OPL interactive test code for mPOPUP.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL
INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
EXTERNAL	tpopup:
EXTERNAL	tpopupcheckbox:
EXTERNAL	tpopupoptions:
EXTERNAL	tpopupdim:
EXTERNAL	popup:(x%,y%,t%)
EXTERNAL	createbutton:
EXTERNAL	onbutton:(x&,y&)
EXTERNAL	depress:(id%)

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pMPopup", hThreadIdFromOplDoc&:, KhUserLoggingOnly%) 
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


PROC pMPopup:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hCall%:("tmpopup")
	hSpoofSetFlagTargetApp%:(CMD$(1),KhDeleteFlag%)
	rem hCleanUp%:("Reset")
ENDP


PROC Reset:
	rem Any clean-up code here.
ENDP



proc tmpopup:
	print "Opler1 mPOPUP Test"
	print
	tpopup:
	tpopupcheckbox:
	tpopupoptions:
	tpopupdim:
	rem tbuttonpop:
	print
	print "Opler1 mPOPUP Test Finished OK"	
	rem pause 30
endp


proc tpopup:
	print "Test popup in various positions and types"	
	rem pause 20
	rem screen 30,10,1,7

	popup:(0,0,0)
	popup:(gwidth,gheight,3)
	gat 320,100
	gcircle 2
	popup:(320,100,0)
	popup:(320,100,1)
	popup:(320,100,2)
	popup:(320,100,3)
	
	print "These are off the screen, but are positioned so they do fit on the screen"
	print
	popup:(800,600,1)
	popup:(0,0,1)
	popup:(0,0,2)
	popup:(0,0,3)
	popup:(gwidth,gheight,0)
	popup:(gwidth,gheight,1)
	popup:(gwidth,gheight,2)
endp


proc popup:(x%,y%,type%)
	local p%, type$(20)

	if type%=0 : type$="  top left" : endif
	if type%=1 : type$="  top right" : endif
	if type%=2 : type$="  bottom left" : endif
	if type%=3 : type$="  bottom right" : endif
	print "At ";x%;",";y%;type$
	rem while 1
		p%=mpopup (x%,y%,type%,"New",%n,"Open",%o,"Save",%s)
	rem	if p%=0 : print "Popup cancelled" : break : endif
		print "You selected $";hex$(p% and $ff);" ('";chr$(p%);"')"
	rem endwh
	print
endp


proc tpopupcheckbox:
	local t%,c%,s%,m%
	cls
	print "Test popup with checkboxes"
	t%=%t OR $0800 : c%=%c OR $0800 : s%=%s OR $0800
	gat 320,100
	gcircle 2
	rem do
		m%=mPopup (320,100,0,"Tabs",t%,"Carriage Returns",c%,"Spaces",s%)
		if m%<>0
			print "You pressed $";hex$(m%);"('";chr$(m% and $ff);"')"
			if m%=%t :	if (t% AND $2000)<>0 : t%=(t% AND (NOT $2000))
								else t%=t% OR $2000 : endif
			endif
			if m%=%c :	if (c% AND $2000)<>0 : c%=(c% AND (NOT $2000))
								else c%=c% OR $2000 : endif
			endif
			if m%=%s :	if (s% AND $2000)<>0 : s%=(s% AND (NOT $2000))
								else s%=s% OR $2000 : endif
			endif
		endif
	rem until m%=0
	rem print "Popup cancelled"
	print
endp


proc tpopupoptions:
	local s%,r%,h%,t%,m%
	cls
	print "Test popups with option buttons"
	s%=%s OR $0900 OR $2000 : r%=%r OR $0A00: h%=%h OR $0A00 : t%=%t OR $0B00 
	rem do
		m%=mPopup (320,100,1,"Swiss",s%,"Roman",r%,"Helvetia",h%,"Times",t%)
		if m%<>0
			print "You pressed $";hex$(m%);"('";chr$(m% and $ff);"')"
			if m%=%s :	if (s% AND $2000)<>0 : s%=(s% AND (NOT $2000)) : r%=r% OR $2000 : h%=h% OR $2000 : t%=t% OR $2000 
								else s%=s% OR $2000 : r%=(r% AND (NOT $2000)) : h%=(h% AND (NOT $2000)) : t%=(t% AND (NOT $2000)) : endif
			endif
			if m%=%r :	if (r% AND $2000)<>0 : r%=(r% AND (NOT $2000)) : s%=s% OR $2000 : h%=h% OR $2000 : t%=t% OR $2000 
								else r%=r% OR $2000 : s%=(s% AND (NOT $2000)) : h%=(h% AND (NOT $2000)) : t%=(t% AND (NOT $2000)) : endif
			endif
			if m%=%h :	if (h% AND $2000)<>0 : h%=(h% AND (NOT $2000)) : r%=r% OR $2000 : s%=s% OR $2000 : t%=t% OR $2000
								else h%=h% OR $2000 : r%=(r% AND (NOT $2000)) : s%=(s% AND (NOT $2000)) : t%=(t% AND (NOT $2000)) : endif
			endif
			if m%=%t :	if (t% AND $2000)<>0 : t%=(h% AND (NOT $2000)) : r%=r% OR $2000 : s%=s% OR $2000 : h%=h% OR $2000
								else t%=t% OR $2000 : r%=(r% AND (NOT $2000)) : s%=(s% AND (NOT $2000)) : h%=(h% AND (NOT $2000)) : endif
			endif
		endif
	rem until m%=0
	rem print "Popup cancelled"
endp


proc tpopupdim:
	local p%
	cls
	print "Test popup with dimmed items"
	rem while 1
		p%=mpopup (320,100,2,"New",%n or KMenuDimmed%,"Open",%o,"Save",%s or KMenuDimmed%)
	rem	if p%=0 : print "Popup cancelled" : break : endif
		print "You selected $";hex$(p% and $ff);" ('";chr$(p%);"')"
	rem endwh
	print
endp


proc tbuttonpop:
	local id%,ev&(16),p%
	cls
	print "Test button with popup menu"
	pause 20
	rem draw button
	id%=createbutton:
	rem wait for button to be depressed
	pointerfilter 2,7  rem filter out "move" events
	while 1
		while 1
			getevent32 ev&()
			if ev&(1)=&408 and ev&(4)=0 and onbutton:(ev&(6),ev&(7))
				break
			endif
			if (ev&(1) and $ff)=$1b : break : endif
		endwh
		if (ev&(1) and $ff)=$1b : break : endif
		depress:(id%)
		screen 30,10,1,7	
		p%=mpopup (330,150,0,"New",%n,"Open",%o,"Save",%s)
		if p%=0 : print "Popup cancelled"
		else print "You selected $";hex$(p% and $ff);" ('";chr$(p%);"')"
		endif
	endwh
	gclose id%
	gcls
endp

proc createbutton:
	local id%
	
	rem create bitmap
	id%=gcreatebit(30,30)
	gat 15,15
	gcircle 14
	gat 5,5
	gfill 20,20,0
	
	rem create button
	guse 1
	gcls
	gat 250,100
	gfont KFontSquashed&
	gbutton "Choices",2,80,50,0,id%,id%
	return id%
endp

proc onbutton:(x&,y&)
	local true%,false%
	true%=1 : false%=0
	if x&<250 or x&>330 : return false% : endif
	if y&<100 or y&>150 : return false% : endif
	return true%
endp

proc depress:(id%)
	gat 250,100
	gbutton "Choices",2,80,50,0,id%,id%
	gbutton "Choices",2,80,50,1,id%,id%
	gbutton "Choices",2,80,50,2,id%,id%
	gbutton "Choices",2,80,50,1,id%,id%
	gbutton "Choices",2,80,50,0,id%,id%
endp

REM End of pmPopup.tpl
