rem Toolbar app using toolbar.opo
rem Copyright (c) 1997-1998 Symbian Ltd. All rights reserved.

INCLUDE "const.oph"
INCLUDE "date.oxh"
INCLUDE "bgopx.oxh"

DECLARE EXTERNAL
EXTERNAL Main:
EXTERNAL AppCont1:
EXTERNAL myInit:
EXTERNAL NextEv%:
EXTERNAL InitTBar:
EXTERNAL MakeWins:

rem Coerce float args to ints
EXTERNAL MakeWin%:(x%,y%)

EXTERNAL cmdE%:
EXTERNAL cmdSE%:
EXTERNAL cmdD%:
EXTERNAL cmdF%:
EXTERNAL cmdSF%:
EXTERNAL cmdV%:
EXTERNAL showEv:
EXTERNAL clrEv:
EXTERNAL cmdSC%:
EXTERNAL cmdS%:
EXTERNAL cmdC%:
EXTERNAL cmdA%:
EXTERNAL cmdG%:
EXTERNAL bands:(width&,initColor&)
EXTERNAL cmdT%:
EXTERNAL cmdTbDownP%:
EXTERNAL offrCmd%:(key&,modif&)

rem Official Uid reserved = &10000145
APP SampApp,&10000145
rem 	ICON "z:\system\opl\TDemo.mbm"
	CAPTION "OplSamp",1
	FLAGS 1
ENDA

proc main:
	rem app specific variables
	global ScrWid%		rem pixel width initially
	global ScrHght%		rem pixel height initially
	global MenuPos%		rem last menu position

	global bitmapId%,mainMbm$(100)
	global id%(12)

	global wIdMain%		rem main window


	rem For portability with Opl1993
	global HotKMod% rem modifier for hot-key
									rem Control-key on er1

	rem Event	data
	global event&(16),evType&

	local justEntered% rem next drawing not relative to prev point

	ScrWid%=gWidth
	ScrHght%=gHeight


	myInit:						rem also initialises TBar + draws it
	font KFontCourierNormal13&,16
	do			  				rem main loop
		nextEv%:
		evType&=event&(KEvaType%)
		gUse wIdMain%
		if event&(KEvAPtrOplWindowId%)=widMain%
			if (evType&=KEvPtrEnter&)
				justEntered%=KTrue%	rem next drawing is not relative to prev point
				continue
			endif
		endif
		if evType&=KEvPtr&
			if event&(KEvAPtrType%)=KEvPtrDrag&
				if justEntered%
					gAt event&(KEvAPtrScreenPosX%),event&(KEvAPtrScreenPosY%)
					justEntered%=KFalse%
				endif
				gLineTo event&(KEvAPtrScreenPosX%),event&(KEvAPtrScreenPosY%)				
			elseif event&(KEvAPtrType%)=KEvPtrButton1Down&
				justEntered%=KFalse%
				gAt event&(KEvAPtrScreenPosX%),event&(KEvAPtrScreenPosY%)
				gLineBy 0,0
			endif	
		else
			if event&(KEvaType%) and &400
				if (evType&<>KEvKeyUp&) and (evType&<>KEvKeyDown&)
					print "Ev(&"+hex$(event&(KEvaType%));")",
				endif
			elseif (event&(KEvaType%)<32 or event&(KEvaType%)>255)
				print "<";event&(KEvaType%);">";
			else
				print chr$(event&(KEvaType%));
			endif
		endif
	until 0
endp

proc myInit:
	rem set up globals
	external wIdMain%,HotKMod%
	wIdMain%=1			rem Just use console for now
	gUpdate off
	gSetPenWidth 2

  HotKMod%=4		rem Control-key for Era
	initTBar:	rem create toolbar and show it
	makeWins:
	gorder 1,255		rem main window background

endp


proc makeWins:
	external MainMbm$,bitmapId%,id%()
	local angle,i%

  MainMbm$="z:\system\opl\talkdemo.mbm"
	bitmapId%=gLoadBit(MainMbm$,0,0)
	while angle<2.0*pi
		angle=angle+pi/6.0
		i%=i%+1
		id%(i%)=makeWin%:(225.0+70.0*sin(angle),80.0-70.0*cos(angle))
	endwh
endp

proc makeWin%:(x%,y%)
	external bitmapId%
	local id%

	id%=gCreate(x%,y%,70,70,1,$111)
	gmove 15,15
	gCopy bitmapId%,0,0,gWidth,gHeight,3
	gxborder 2,3
	return id%
endp


proc initTBar:
	external ScrWid%,ScrHght%
	local mbmTbar$(50),bitmapid1&,bitmapid2&

	mbmTbar$="z:\system\opl\talkdems.mbm"
	bitmapId1&=gLoadBit(mbmTbar$,0,0)
	bitmapId2&=gLoadBit(mbmTbar$,0,1)

rem	TBarInit:("Demo",ScrWid%,ScrHght%)
rem	TBarButt:("c",1,"Clock",0,bitmapid1&,bitmapid1&,0)
rem	TBarButt:("a",2,"Anti",0,bitmapid2&,bitmapid2&,0)
rem	TBarButt:("p",3,"Popup"+chr$(10)+"demo",0,&0,&0,1)
rem	TBarButt:("e",4,"Exit",0,&0,&0,0)
rem	TBarShow:
	
rem	BGSetButton:(aButtonIndex%,aText$,aBitmapId%,aMaskId%,aCallback$) 
	BGSetButton:(0, "Clock", 0,0, "clockCallback")
	BGSetButton:(1, "Anti", 0,0, "antiCallback")
	BGSetButton:(2, "Popup/demo", 0,0, "popupCallback")
	BGSetButton:(3, "Exit", 0,0, "exitCallback")
	BGShow:

rem	BGOffer%:(aEv1&,aEv3&,aEv4&,aEv5&,aEv6&,aEv7&) : 2
rem	BGShow: : 3
rem	BGHide: : 4
rem	BGVisible%: : 5
rem	BGCount%: :6
rem	BGWidth%: :7

endp

proc nextEv%:
	rem Handles menu and toolbar events
	external event&(),wIdMain%,HotKMod%
	local evType&,command$(255)

	while 1
  	GetEvent32 event&()
		evType&=event&(KEvaType%)

		if evType&=KEvCommand&
			command$=getcmd$
			if left$(command$,1)="X"
				stop
			endif
		elseif evType&=KEvPtr&
rem			if not TBarOffer%:(event&(KEvAPtrOplWindowId%),event&(KEvAPtrType%),event&(KEvAPtrPositionX%),event&(KEvAPtrPositionY%)) rem Handles tbar events
rem				break
rem			endif
		elseif evType&=KEvPtrEnter& and (event&(KEvAPtrOplWindowId%)=wIdMain%)
			break
		elseif evType&=0 rem Null event
			continue
		elseif (evType&=KKeySidebarMenu32&) or ((evType& and $400)=0)
			if (evType&<>KKeySidebarMenu32&) and (evType&<>KKeyMenu32&) and ((event&(KEvAKMod%) and HotKMod%)=0)
				break
			endif
			if not offrCmd%:(event&(KEvaType%),event&(KEvAKMod%))	rem ret -1 if command, else 0
				break
			endif
		endif
	endwh
endp

rem Command handlers
rem
proc cmdE%:
	stop
endp

proc cmdSE%:
	rem Text editor
	local s1$(255),s2$(5),d%

	s1$=rept$("1234567890",25)+"12345"
  dInit "Text editor"
	  dEdit s2$,"Edit 5 char string"
		dEdit s1$,"Edit 255 char string",20
	dialog
endp

proc cmdD%:
	rem Date/time editors
	local d1&,d2&,t1&,t2&

	dInit "Date and time editors"
		dDate d1&,"Date",0,&10000
		dTime t1&,"Time",1,0,&10000
		dTime t1&,"Time (no seconds)",0,0,&7fff
	dialog
endp

proc cmdF%:
	rem Filename editor
	local f1$(255)
	local flags&,flags$(30),label$(8)

	flags$="$1 or $80"
	while 1
		dInit "Filename editor/selector(flags=$"+hex$(flags&)+"): Esc quits"
			if flags& and 1	rem bit 1 set for editor
				label$="Edit"
			else
				label$="Select"
			endif
			onerr e1

			dFile f1$,label$+" file:",flags&
			onerr off
			dEdit flags$,"Set next flags",20
			dText "Flag values:","$1->editor,$2->allow dirs"
			dText "and:","$4->dirs only,$8-> disallow existing (e)"
			dText "and:","$10->query existing (e),$20->allow null string"
			dText "and:","$40->no extension (s),$80->allow wildcards"
		if dialog=0
			break
		endif
		flags&=eval(flags$)
		giprint f1$
		continue
	e1::
		giprint "Bad flags:"+err$(err)
	endwh
endp

proc cmdSF%:
	rem Dialog flags used in dINIT
  local curFlags%,d%
  local f% rem new flag setting

  while 1
    dInit "Dialog flags",curFlags%
		if curFlags%=0
			dText "The default setting","No flags set"
		else
			if curFlags% and KDlgButRight%
				dText "Buttons right","KDlgButRight%"
			endif
			if curFlags% and KDlgNoTitle%
				dText "No title","KDlgNoTitle%"
			endif
			if curFlags% and KDlgFillScreen%
				dText "Full screen dialog","KDlgFillScreen%"
			endif
			if curFlags% and KDlgNoDrag%
				dText "Dragging disabled","KDlgNoDrag%"
			endif
			if curFlags% and KDlgDensePack%
				dText "Densely packed","KDlgDensePack% set"
			else
				dText "Loosely packed","KDlgDensePack% clear"			
			endif
		endif
	 dButtons "Buttons",%b,"Title",%t,"Screen",%s,"Packing",%p,"Dragging",%d
	 d%=dialog
    if d%=0
      break
    endif
    if d%=%b
      f%=KDlgButRight%
    elseif d%=%t
      f%=KDlgNoTitle%
    elseif d%=%s
      f%=KDlgFillScreen%
    elseif d%=%d
      f%=KDlgNoDrag%
    elseif d%=%p
      f%=KDlgDensePack%
    endif
    if curFlags% and f%
      curFlags%=curFlags% and (not f%)  rem remove flag if set
    else
      curFlags%=curFlags% or f%          rem add flag if not set
    endif
  endwh
endp

proc cmdV%:
	rem Get events
	global ev&(16)

	busy "Modal demo (toolbar inactive) Esc quits",2
	gUse 1
	gCls
	at 1,5
	while 1
		clrEv:	rem clear the array
		getevent32 ev&()
		if (ev&(1)=27) and (ev&(4)=0)
			rem Esc with no modifier
			break
		endif
		showEv:
	endwh
	gUse 1
  gCls
  at 1,1
	busy off
endp

proc showEv:
	external ev&()
	local ev1&,mod%,rep%
	local i%,cnt%
	local t&

	ev1&=ev&(1)
	cnt%=10	
	if (ev1& and $400)=0
		mod%=ev&(4)
		rep%=ev&(5)
		print "Key:  ";
		print num$(ev1&,-4),
		if ev1&<$1000
			print " ";
			if ev1&<$100
				print " ";
				if ev1&<$10
					print " ";
				endif
			endif
		endif
		print "(0x";hex$(ev1&);")",
		if ev1&>=32
			print "= '";chr$(ev1&);"'",
		else
			print "    ",
		endif
		print "  Modif=0x";hex$(mod%),
	      print "Repeats=";rep%;"  "
	else
		print "Event:",hex$(ev1&),
		i%=2
		print "  ";
		vector ev1&-&400
			l401,l402,l403,l404,l405,l406,l407,l408,l409,l410
		endv
    if ev1&<>&400   rem not null event
      print "Unknown",hex$(ev1&)
			return
		endif
		while 0
l401::
			print "Focus gained"
			break
l402::
			print "Focus lost"
			break			
l403::
			print "Switched on"
			break
l404::
			print "Exit command received"  rem Not yet implemented
			break
l405::
			print "Date changed"
			break
l406::
			print "Key down, scan code=";ev&(3);"                    "
			break
l407::
			print "Key up,   scan code=";ev&(3);"                      "
			break
l408::
			t&=ev&(KEvAPtrType%)
			print "Pointer event ";t&,
			vector t&+1
				lp0,lp1,lp2,lp3,lp4,lp5,lp6,lp7,lp8,lp9
			endv
			print "Unknown type ",
			while 0
	lp0::
	lp1::
	lp2::
	lp3::
	lp4::
	lp5::		print "Button";t&/2+1,
				if t& and 1
					print "up  ",
				else
					print "down",
				endif
				break
	lp6::		print "Drag",
				break
	lp7::		print "Move",
				break
	lp8::		print "Button repeat",
				break
	lp9::		print "Switch on",
				break
			endwh
			print "at (";ev&(KEvAPtrPositionX%);",";ev&(KEvAPtrPositionY%);")",
			print "WinId=";ev&(KEvAPtrOplWindowId%)
			break
l409::
			print "Pointer entered window",ev&(KEvAPtrOplWindowId%)
			break
l410::
			print "Pointer exited window",ev&(KEvAPtrOplWindowId%)
			break
		endwh
	endif
endp

proc clrEv:
	external ev&()
	local i%

	while i%<16
		i%=i%+1
		ev&(i%)=0
	endwh
endp

proc cmdSC%:
	external TbWinId%
	gOrder TbWinId%,1
	gOrder 1,2
	gUse 1
	gCls
	at 1,1
endp

proc cmdS%:
	rem Stopwatch demo using microsecond timing in DATE.OPX
	external TbVis%
	local s&,e&,d&,a%(2),micro$(6),k%
	local y&,mo&,day&,h&,mi&,se&,m&
	local savedTbVis%
	
	savedTbVis%=TbVis%
	if savedTbVis%
		BGHide:
	endif
  cls
  print
  print " The Opl32 DateTime OPX gives extended access to E32's time functions"
  print
  print " Allows OPL programmers to achieve microsecond timing resolution"
  print " in their programs, as demonstrated below. "
  print
  print
  print " Press any key to start / 'lap time' the clock  :  Esc to quit"
  if get<>27
		s&=DTNOW&:
	  while 1
  	 	e&=DTNOW&:
    	DTDateTimeDiff:(s&,e&,y&,mo&,day&,h&,mi&,se&,m&)
	    at 30,15
  	  micro$=num$(m&/1000,-3)
    	print h&;":";mi&;":";se&;":"; micro$;
		  print "                     "
  	 	k%=key
	  	if k%=27
      	break
   		endif

	   	if k%
      	at 1,17
   	  	print"LAP TIME      ";day&;"/";mo&;"/";y&,h&;":";mi&;":";se&;" and ";m&;" microseconds          "
 	  	endif
  	endwh
  endif
	if savedTbVis%
		BGShow:
	endif
  cls
endp

proc cmdC%:
	rem Clockwise demo
	external id%()
	local i%

	gUpdate on
	while i%<12
		i%=i%+1
		gOrder id%(i%),1	
	endwh
	gUpdate off
endp

proc cmdA%:
	rem Anti-clockwise demo
	external id%()
	local i%

	gUpdate on
	i%=11
	while i%>0
		gOrder id%(i%),1	
		i%=i%-1
	endwh
	gOrder id%(12),1	rem top
	gUpdate off
endp

proc cmdG%:
	rem Grey scales
	external ScrWid%,ScrHght%
	local win1%,win2%
	local width&,rgb&
	local ev&(16)

	width&=40
	rgb&=255
	dInit " Enter width of each band"
		dLong width&,"Width",1,ScrWid%
		dLong rgb&,"Initial colours",0,255
		dButtons "Cancel",-(27 OR $100),"OK",13 OR $100
	if dialog=0
		return
	endif
	win1%=gcreate(0,0,ScrWid%,ScrHght%/2,1,1)
	guse 1
	win2%=gcreate(0,ScrHght%/2,ScrWid%,ScrHght%/2,1,2)
	guse win1%
	bands:(width&,rgb&)
	guse win2%
	bands:(width&,rgb&)
	do
		getevent32 ev&()
	until ((ev&(KEvAType%) and $400) =0) or (ev&(KEvAType%)=KEvPtr& and ev&(KEvAPtrType%)=KEvPtrPenDown&)
	gClose win2%
	gClose win1%
	
endp

proc bands:(width&,initColor&)
	local col%,ii%,dif%
	dif%=16
	col%=initColor&
	ii%=(gwidth+width&-1)/width&
	gat 0,0
	while ii%>0
		gcolor col%,col%,col%
		gfill width&,gheight,0
		gmove width&,0
		col%=col%+dif%
		if col%>255
			col%=col%-32
			dif%=-16
		elseif col%<0
			col%=col%+32
			dif%=16
		endif
		ii%=ii%-1
	endwh
endp

proc cmdT%:
	external TbVis%
	if TbVis%
		BGHide:
	else
		BGShow:
	endif
endp

proc cmdTbDownP%:
	external ScrWid%,TbWidth%
	local pop%

	rem popup next to button with point specifying the top right corner of the popup
	pop%=mPopup(ScrWid%-TbWidth%,97,KMPopupPosTopRight%,"Item a",%a,"Item b",%b)
	if pop%=0
		giprint "Popup cancelled"
	else
		giprint "Item "+chr$(pop%)+" selected"
	endif
endp

proc offrCmd%:(key&,modif&)
	rem Returns -1 if command (menu or hot-key)
	rem also -1 if menu-key and cancelled

	external TbVis%,menuPos%,hotKMod%,TbWinId%
	local isMenuK%
	local m%,k&,cmdRoot$(4)
	local mod%
	local hotK%
	local tbOn%

	mod%=modif&
	k&=key&
	if (k&=KKeyMenu32&) or (k&=KKeySidebarMenu32&)
		isMenuK%=-1
		if TbVis%
			TbOn%=$2000
		endif
		mInit
			mCard "App","Exit",%e
			mCard "Edit","Clear screen",%C
			mCasc "Edit boxes","Text editor",%E,"Date/time",%d ,"Filename",%f
			mCard "Dialogs","New flags",%F,"Edit boxes>",16
			mCard "View","Show toolbar",%t or KMenuCheckBox% or TbOn%
			mCard "Graphics","Clockwise",%c,"Anti-clockwise",-%a,"Grey scales",%g
			mCard "Events","Display events",%v
			mCard "Opx usage","Stopwatch demo",%s
		m%=menu(MenuPos%)
		if m%
			hotK%=m%
			mod%=HotKMod% rem convert to accelerator
			if hotK%<=%Z
				mod%=mod% or 2	rem Shift
			endif
		else
			return -1
		endif
	else	rem not menu key
		hotK%=k&-1+%a rem Control+a/A -> 1
	endif
	if mod% and HotKMod%	rem Hotkey modif, so maybe accelerator
		cmdRoot$="cmd"
		if mod% and 2
			cmdRoot$="cmds"
		endif
		rem print "call ";cmdRoot$+chr$(hotK%),hotK%
		onerr eNotCmd
		if ((mod% and 2)=0) and ((hotK%=%c) or (hotK%=%a))
			gOrder 1,255				rem Text window to back
		else
			gOrder TbWinId%,1		rem Toolbar to front
			gOrder 1,2					rem Text window behind toolbar
		endif
		@%(cmdRoot$+chr$(hotK%)):
		gUse 1
		return -1
	endif
eNotCmd::
	if isMenuK% or err<>-99
		giprint "Bug: Proc "+cmdRoot$+chr$(hotK%)+"%:,"+err$(err)
		if err=-98	rem missing externals
			alert("ERRX$: "+ErrX$,err$(err))
		endif
		return -1
	endif
	return 0
endp





