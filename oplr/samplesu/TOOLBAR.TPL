rem Toolbar module
rem Provides procedures for implementing a toolbar in OPL programs
rem Copyright (c) 1997-1999 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL

rem These rely on settings in TOOLBAR.OPH
const KTbFlgLatchable%=$02 		rem set in all latch position consts
const KTbFlgLatchPosMask%=$30	rem position excluding latchable
const KTbFlgLatchMask%=$36 		rem for extracting all latch settings (position or latchable or latched)

INCLUDE "System.oxh"
INCLUDE "Date.oxh"
INCLUDE "Const.oph"
INCLUDE "Toolbar.oph"

rem Private procedures
external __doTbarInit:(name$,scrW%,scrH%,width%,winMode%)
external TbBtChng:(pos%,state%)
external TBarBtDown:(comp%)
external TBarBtUp:(comp%)
external TBarCmd%:(comp%)
external TBarDoCmd%:(comp%,cmdRoot$)
external TBarRel:
external TBarComp%:(x&,y&)
external TBarSetDefaultColor:
external TBarSetColorForeground:
external TBarSetColorBackground:

rem Toolbar specific consts
const KTbWidth%=70
const KTbBtTop%=24
const KTbBtH%=37
const KTbClockSize%=70
const KTbNumButtons%=4
const KTbNumComps%=6
const KTbMarginX%=0
const KTbFont&=KFontSquashed&
const KTbTitleFont&=KFontArialNormal11&
const KTbStyle%=1
const KTbClockPosX%=3
const KTbClockHeight%=64
const KTbClockType%=KClockLocaleConformant%
const KTbColorItemForeground%=1
const KTbColorItemBackground%=2
const KTbNumColorItems%=2


proc TBUsage:
	local c%,first%

	first%=-1
	while 1
		if first%
			first%=0
		else
			get
			cls
		endif
		dInit "'Toolbar' is not designed to run stand-alone"
			dText "","Toolbar procedures are called by applications"
			dText "","Use LOADM ""TOOLBAR.OPO"" to make the procedures available"
			dChoice c%,"Select procedure for usage information:","TBarLink:,TBarInit:,TBarInitC:,TBarColor:,TBarSetTitle:,TBarButt:,TBarOffer%:,TBarLatch:,TBarShow:,TBarHide:"
			dButtons "Cancel",-(27 OR KDButtonNoLabel%),"Procedure"+chr$(10)+"information",13 OR KDButtonNoLabel%
		if dialog=0
			break
		endif
		vector c%
			cLink,cInit,cInitC,cColor,cSetName,cButt,cOffer,cLatch,cShow,cHide
		endv
cLink::
		print "Usage: TBarLink:(appLink$)"
		print
		print "Provides all toolbar globals required in your application."
		print "Must be called before TBarInit: and at a higher level"
		print "procedure in your application than the globals are used."
		print
		print "appLink$ is the name of the continuation procedure in the"
		print "main application. This allows the globals to exist until"
		print "the application exits."
		print "appLink$ must represent a procedure with prototype like"
		print "'proc continue:' - taking no parameters and with no"
		print "return-type specification character, so that it can be"
		print "called using '@(appLink$):'."
		continue
cInit::
		print "Usage: TBarInit:(title$,scrW%,scrH%)"
		print
		print "Called at start of application only, this procedure creates"
		print "the toolbar window, which guarantees that there will be "
		print "sufficient memory available to display the toolbar at any"
		print "subsequent time. The toolbar is created using the same window"
		print "mode as the current window of the calling OPL program. The"
		print "toolbar is made invisible when not shown. This procedure also"
		print "draws all toolbar components except the buttons. Turns"
		print "graphics auto-flushing off."
		print
		print "title$ is the title shown in the toolbar."
		print "scrW% is the full-screen width (gWidth at startup)."
		print "scrH% is the full-screen height (gHeight at startup)."
		continue
cInitC::
		print "Usage: TBarInitC:(title$,scrW%,scrH%,winMode%)"
		print
		print "Similar to TBarInit:(), this procedure creates the toolbar"
		print "window using the window mode specified. See TBarInit:() for"
		print "more information."
		print
		print "title$ is the title shown in the toolbar."
		print "scrW% is the full-screen width (gWidth at startup)."
		print "scrH% is the full-screen height (gHeight at startup)."
		print "winMode% is the window mode required (see gColorInfo"
		print "for more infomation)."
		continue
cColor::
		print "Usage: TBarColor:(fgR%,fgG%,fgB%,bgR%,bgG%,bgb%)"
		print
		print "Called to set the colors used by the toolbar when initially"
		print "drawing its windows."
		print "fgR%,fgG%,fgB% are the red,green,blue parts of the foreground color"
		print "bgR%,bgG%,bgB% are the RGB components of the background color."
		continue
cSetName::
		print "TBarSetTitle:(title$)"
		print
		print "Sets the name in the toolbar."
		print
		print "name$ is the name of your document, or the name of your"
		print "application for no-file applications."
		continue
cButt::
		print "Usage: TBarButt:(shortCut$,pos%,text$,state%,bit&,mask&,flags%)"
		print
		print "Adds a button to the previously initialised toolbar."
		print
		print "shortCut$ is the menu command shortcut for your application,"
		print "which is used by TBarCmd%: to perform the command when a"
		print "button is selected. shortCut$ is case sensitive in the sense"
		print "described under TBarCmd%:."
		print "pos% is the button position, with pos%=1 for the top button."
		print "text$ and state% are as for gButton."
		print "bit& and mask& are the button icon bitmap and mask, used in"
		print "in the same way as for gButton."
		print "flags%=1 for calling ""CmdTbDown""+shortCut$ on pen down. In this"
		print "case the shortcut is not case-sensitive."
		continue
cOffer::
		print "Usage: TBarOffer%:(winId&,ptrType&,ptrX&,ptrY&)"
		print
		print "Offers pointer event to toolbar. Returns -1 if used, 0"
		print "if not used."
		print
		print "winId& is ID of window that received the pointer event."
		print "ptrType& is pointer event type (up,down or move)."
		print "ptrX&,ptrY& is coordinate of pointer event."
		continue
cLatch::
		print "Usage: TBarLatch:(button%)"
		print
		print "Latches down a button in a latchable set, unlatching"
		print "any previously latched button."
		print
		print "button%=1 for the top button in the toolbar."
		continue
cShow::
		print "Usage: TBarShow:"
		print
		print "Make the toolbar visible. The toolbar and buttons must exist"
		print "before calling this procedure."
		continue
cHide::
		print "Usage: TBarHide:"
		print
		print "Make the toolbar invisible."
		continue
	endwh
endp

proc TbBtChng:(pos%,state%)
	rem Draw an existing button at position
	rem pos%=1,2,3,...,KTbNumButtons%
	rem Just redraws in new state

	external TbVis%,TbState%()
	external TbCrCmp%,TbWinId%
	external TbBtH%,TbBtTxt$(),TbBtBit&(),TbBtMsk&(),TbBtFlags%()

	local y%,orgWId%
  local st%     rem state depends on latching

  st%=state%
  if TbBtFlags%(pos%) and KTbFlgLatched%
    st%=st%+1
  endif
  if pos%<=0 or pos%>KTbNumButtons%
		raise -2
	endif
	if TbState%(pos%+2)=st%
		return		rem unchanged
	endif
	if (TbCrCmp%<>0)
		if (pos%<>TbCrCmp%)
			return	rem Can only change component that is current
		endif
	endif
  orgWId%=gIdentity
	gUse TbWinId%
	gFont KTbFont&
	y%=KTbBtTop%+(pos%-1)*TbBtH% rem from top
	gAt KTbMarginX%,y%
	gButton TbBtTxt$(pos%),1,gWidth-2*KTbMarginX%,TbBtH%+1,st%,TbBtBit&(pos%),TbBtMsk&(pos%),KButtExcessToText%
	TbState%(pos%+2)=st%
	gUse orgWId%
endp

proc TBarRel:
	rem Release a component when moving to another
	rem or exiting toolbar
	external TbCrCmp%,TbVis%,TbState%()
	local state%
	
	if (TbCrCmp%>=1)					rem if button
		TbBtChng:(TbCrCmp%,0)		rem just release, no command
	elseif TbCrCmp% rem only if there is a current component
		TbState%(iabs(TbCrCmp%))=0 rem just release
	endif
endp

proc TBarOffer%:(winId&,ptrType&,ptrX&,ptrY&)
	rem Offer pointer event to toolbar
	rem Return 0 if not used
  rem winId& is window ID where pointer event occurred
  rem ptrType& is pointer event type:
	rem     KEvPtrDown&->down,KEvPtrUp&->up,KEvPtrDrag&->move
  rem ptrX&,ptrY& is coordinate of pointer event

	external TbWinId%,TbCrCmp%,TbState%(),TbBtFlags%(),TbShortcut$(),TbPtrCapture%

	local btShortCut$(1)
	local comp%
	local orgWId% rem original window
	local clockFormat&

	orgWId%=gIdentity
	if winId&<>TbWinId%
		TBarRel:		rem release current component
		rem Still remains current unless pen lifted up
		if (ptrType&=KEvPtrPenUp&) or (ptrType&=KEvPtrPenDown&)
			TbCrCmp%=0
			TbPtrCapture%=0
			rem giprint "Pointer freed"
		endif
		return 0	rem not in toolbar
	endif
	comp%=TBarComp%:(ptrX&,ptrY&)
	if ptrType&=KEvPtrPenDown&	rem Down
		TbPtrCapture%=-1
		rem giprint "Pointer captured"
		if comp%>=1		rem button down
			TBarBtDown:(comp%)
		elseif comp%=-1
			DisplayTaskList:
		elseif comp%=-2
			gUse TbWinId%
			clockFormat&=LCClockFormat&:
			if clockFormat&=KLCAnalogClock&
				clockFormat&=KLCDigitalClock&
			else
				clockFormat&=KLCAnalogClock&
			endif
			LCSetClockFormat:(clockFormat&)
			gUse orgWId%
		endif
	elseif ptrType&=KEvPtrPenUp&	rem Up
		TbPtrCapture%=0
		rem giprint "Pointer freed"
		if comp%>=1
			TBarBtUp:(comp%)
		endif
		TbCrCmp%=0			rem also clear clock and title
		return -1
	elseif ptrType&=KEvPtrDrag&
		if TbPtrCapture%
			if (TbCrCmp%<>0) and (comp%<>TbCrCmp%)
				rem dragged onto non-current component (TBarRel: checks for state unchanged and then doesn't redraw)
				TBarRel:
			else
				TbCrCmp%=comp%
				if comp%>=1
					TbBtChng:(comp%,1)
				else
					TbState%(iabs(comp%))=2
				endif
			endif
		endif
	else
		return 0
	endif
	gUse orgWId%
	return -1
endp

proc TBarBtUp:(comp%)
  external TbCrCmp%,TbBtFlags%()
  local f%,c%

  if TbCrCmp%<>comp%
    TbCrCmp%=0
    return
  endif
  f%=TbBtFlags%(comp%)
  if f% and KTbFlgLatchable%
    if (f% and KTbFlgLatched%)=0  rem if not latched
      TBarDoCmd%:(comp%,"cmd")
    else
      TbBtChng:(comp%,0)  rem redraw latched
    endif
  else
    if (f% and KTbFlgCmdOnPtrDown%)=0
      TBarCmd%:(comp%)
    endif
  endif
  TbCrCmp%=0
endp

proc TBarLatch:(comp%)
	external TbBtFlags%(),TbCrCmp%
  local f%,fCur%,c%
  local found%

  fCur%=TbBtFlags%(comp%) and KTbFlgLatchMask%
  rem free others in set
  c%=comp%-1
  while c%>0
    f%=TbBtFlags%(c%) and KTbFlgLatchMask%
    rem KTbFlgLatchStart%<KTbFlgLatchMiddle%<KTbFlgLatchEnd%
    if (f%=0) or (f%>fCur%)
      break     rem reached start of set
    endif
    if f% and KTbFlgLatched%
      found%=-1
      break
    endif
    c%=c%-1
  endwh
  if not found%
    c%=comp%+1
    while c%<KTbNumButtons%
      f%=TbBtFlags%(c%) and KTbFlgLatchMask%
      rem KTbFlgLatchStart%<KTbFlgLatchMiddle%<KTbFlgLatchEnd%
      if (f%=0) or (f%<fCur%)
        break     rem reached end of set
      endif
      if f% and KTbFlgLatched%
        found%=-1
        break
      endif
      c%=c%+1
    endwh
  endif
  if found%
	  rem Unlatch currently latched one
	  TbCrCmp%=c%     rem TbBtChng: only changes current button
	  TbBtFlags%(c%)=TbBtFlags%(c%) and not KTbFlgLatched%
	  TbBtChng:(c%,0)
  endif
  rem and latch new one
  TbBtFlags%(comp%)=TbBtFlags%(comp%) or KTbFlgLatched%
  TbCrCmp%=comp%
  TBarRel:
  TbCrCmp%=0
endp

proc TBarBtDown:(comp%)
	external TbCrCmp%,TbBtFlags%(),TbPtrCapture%

	TbCrCmp%=comp%
	TbBtChng:(comp%,1)
	if TbBtFlags%(comp%) and KTbFlgCmdOnPtrDown%
		TBarDoCmd%:(comp%,"CmdTbDown")
		TbPtrCapture%=0
	endif
endp

proc TBarLink:(AppLink$)
	rem 'Links' toolbar globals into app.
	rem appLink$ is the name of the continuation
	rem procedure in the main app.

	rem Toolbar state variables
	global TbScrW%				rem fullscreen width: gWidth at startup
	global TbWinId%			rem toolbar win
	global TbVis%				rem true if visible
	global TbMenuSym%		rem check box symbol for show toolbar item
	global TbCrCmp%			rem current component
	global TbBtH%				rem button height
	global TbShortcut$(KTbNumButtons%,1)	rem associated shortcut-key
	global TbBtTxt$(KTbNumButtons%,20)		rem 20 enough?
	global TbBtBit&(KTbNumButtons%)
	global TbBtMsk&(KTbNumButtons%)
	global TbState%(KTbNumComps%)	rem pressed/released
	global TbBtFlags%(KTbNumButtons%)
	global TbHght%
	global TbWidth%
	global TbPtrCapture%	rem true when pointer captured by toolbar, only when toolbar has a pen down event
	global TbDefIcon%			rem default icon for buttons
	global TbWindowMode%		rem Window mode.
	global TbColor&(kTbNumColorItems%)	rem Toolbar item colors.
	
	TBarSetDefaultColor: rem For backwards compatibility.
	@(AppLink$):	rem link back to main app
endp

proc __doTbarInit:(name$,scrW%,scrH%,width%,winMode%)
	external TbWinId%,TbHght%,TbWidth%,TbBtH%
	external TbScrW%,TbScrH%
	external tbWindowMode%

	local orgId%
	
	if TbWinId%
		raise -1 rem Initialised already
	endif
	SetComputeMode:(KComputeModeOff&)
	orgId%=gIdentity
	rem non-consts
	TbScrW%=scrW%
	TbHght%=scrH%
	TbWidth%=width%
	TbBtH%=KTbBtH%
	tBWindowMode%=winMode%
	gUpdate off
	TbWinId%=gCreate(TbScrW%-TbWidth%,0,TbWidth%,TbHght%,0,tbWindowMode%) rem invisible
	TBarSetColorForeground:
	TBarSetColorBackground:
	gcls
	gBox gWidth,gHeight
	TBarSetTitle:(name$)
	gFont KTbFont&
	gstyle KTbStyle%
	gAt KTbClockPosX%,gHeight-KTbClockHeight%
	TBarSetColorBackground:
	gClock on,KTbClockType%
	gUse orgId%
endp

proc TBarInitNonStd:(name$,scrW%,scrH%,width%)
	local winMode%,Info&(48)
	gInfo32 info&()
	winMode%=info&(30)
	__doTBarInit:(name$,scrW%,scrH%,width%,winMode%)
endp

proc TBarInit:(name$,scrW%,scrH%)
	rem Creates invisible Toolbar window
	rem with title and clock. Buttons added later
	rem before window shown.
	rem Turns updating off
	local winMode%,Info&(48)
	gInfo32 info&()
	winMode%=info&(30)
	__doTBarInit:(name$,scrW%,scrH%,KTbWidth%,winMode%)
endp

proc TBarInitC:(name$,scrW%,scrH%,winMode%)
	rem Creates invisible Toolbar window
	rem with title and clock. Buttons added later
	rem before window shown.
	rem Turns updating off
	__doTBarInit:(name$,scrW%,scrH%,KTbWidth%,winMode%)
endp

proc TBarSetTitle:(title$)
	external TbWinId%,TbWidth%
	local align%
	local orgId%

	orgId%=gIdentity
	gUse TbWinId%
	gAt 1,KTbBtTop%-8
	gFont KTbTitleFont&
	gStyle 1
	align%=3	rem Centred by default
	if gTWidth(title$)>TbWidth%-2
		align%=2		rem left aligned if too long
	endif
	TBarSetColorForeground:
	TBarSetColorBackground:
	gPrintB title$,TbWidth%-2,align%,6,6,0
	gUse orgId%
endp

proc TBarButt:(shortCut$,pos%,text$,state%,bit&,mask&,flags%)
  rem Constructs a button at position pos%=1,2,3,4,5,6
  rem Draws button

  external TbWidth%,TbBtBit&(),TbBtMsk&(),TbWinId%,TbBtH%,TbShortcut$(),TbBtTxt$(),TbState%(),TbBtFlags%()
  external TbDefIcon%

  local y%
  local origTb% rem origin of toolbar window
  local fLatch%
  local st%     rem state depends on latching
  local orgWid%

  fLatch%=flags% and KTbFLgLatchMask%
  if fLatch% and KTbFlgLatchable%
    if (fLatch% and KTbFlgLatchPosMask%)=0
      raise -2  rem position must be provided
    endif
  endif
  orgWid%=gIdentity
  st%=state%
  if flags% and KTbFlgLatched%
    st%=st%+1
  endif
	if bit&=0		rem default bitmap
		if TbDefIcon%=0
			TbDefIcon%=gCreateBit(24,24) rem create it once and for all
			gCls
			gBorder 0
		endif
		TbBtBit&(pos%)=TbDefIcon%
		TbBtMsk&(pos%)=TbDefIcon%
	else
		TbBtBit&(pos%)=bit&
		TbBtMsk&(pos%)=mask&
	endif
	gUse TbWinId%
	gFont KTbFont&
	y%=KTbBtTop%+(pos%-1)*TbBtH% rem from top
	gAt KTbMarginX%,y%
	gButton text$,1,TbWidth%-2*KTbMarginX%,TbBtH%+1,st%,TbBtBit&(pos%),TbBtMsk&(pos%),KButtExcessToText%
	TbShortcut$(pos%)=shortCut$
	TbBtTxt$(pos%)=text$
	TbState%(pos%+2)=st%        rem title+clock 1st
	TbBtFlags%(pos%)=flags%
	gUse orgWid%
endp

proc TBarDoCmd%:(comp%,rootCmd$)
	external TbShortcut$(),TbCrCmp%
  local btShortcut$(1)
  local ret%

  btShortCut$=TbShortcut$(comp%)
	rem print " Button ";comp%,"up, shortcut=",btShortCut$
	if btShortCut$>"Z"
		ret%=@%(rootCmd$+btShortCut$):
	else
		ret%=@%(rootCmd$+"s"+btShortCut$):
	endif
  TBarRel:
	TbCrCmp%=0
	return ret%
endp

proc TBarCmd%:(comp%)
	TbBtChng:(comp%,0)
  return TBarDoCmd%:(comp%,"cmd")
endp

proc TBarShow:
	external TbVis%,TbCrCmp%,TbWinId%,TbMenuSym%
	local orgWId%

	orgWId%=gIdentity
	TbVis%=-1
	gUse TbWinId%
	gOrder gIdentity,1
	gVisible on
	TbCrCmp%=0
	TbMenuSym%=KMenuCheckBox% or KMenuSymbolOn%
	gUse orgWId%
endp

proc TBarHide:
	external TbVis%,TbCrCmp%,TbWinId%,TbMenuSym%
	local orgWId%

	orgWId%=gIdentity
	TbVis%=0
	gUse TbWinId%
	TBarRel:	
	gVisible off
	TbMenuSym%=KMenuCheckBox%
	gUse orgWId%
endp

proc TBarComp%:(x&,y&)
	rem x& is the y-co-ordinate of the event in tbar window
	rem y& is the y-co-ordinate of the event in tbar window
	rem Return the toolbar component
	rem Name->-1,Clock->-2,ButtonN->N (N=1 to KTbNumButtons%)
	rem Not in toolbar->0

	external TbWidth%,TbHght%,TbBtH%
	local comp%

	if x&<0 or x&>=TbWidth% or y&<0 or y&>=TbHght%
		return 0
	endif
	if y&<KTbBtTop%
		comp%=-1
	elseif y&>=TbHght%-KTbClockSize%
		comp%=-2
	else
		comp%=1+(y&-KTbBtTop%)/TbBtH%
		if comp%>KTbNumButtons% rem extra pixels in clock?
			comp%=-2
		endif
	endif
	return comp%
endp

proc TBarSetDefaultColor:
	external TbColor&()
	TbColor&(KTbColorItemForeground%)=KRgbBlack&
	TbColor&(KTbColorItemBackground%)=KRgbWhite&
endp

proc TBarColor:(fgR%,fgG%,fgB%,bgR%,bgG%,bgB%)
	external TbColor&()
	TbColor&(1)=fgR%*kRgbRedPosition&+fgG%*kRgbGreenPosition&+fgB%
	TbColor&(2)=bgR%*kRgbRedPosition&+bgG%*kRgbGreenPosition&+bgB%
endp

proc TBarSetColorForeground:
	external TbColor&()
	local rgb&,red%,green%,blue%
	rgb&=TbColor&(kTbColorItemForeground%)
	red%=rgb&/kRgbRedPosition& AND kRgbColorMask&
	green%=rgb&/kRgbGreenPosition& AND kRgbColorMask&
	blue%=rgb& AND kRgbColorMask&
	gcolor red%,green%,blue%
endp

proc TBarSetColorBackground:
	external TbColor&()
	local rgb&,red%,green%,blue%
	rgb&=TbColor&(KTbColorItemBackground%)
	red%=rgb&/kRgbRedPosition& AND kRgbColorMask&
	green%=rgb&/kRgbGreenPosition& AND kRgbColorMask&
	blue%=rgb& AND kRgbColorMask&
	gcolorbackground red%,green%,blue%
endp


rem End of Toolbar module
