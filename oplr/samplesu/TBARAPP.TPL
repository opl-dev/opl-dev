REM Toolbar app using z:\System\Opl\Toolbar.opo
REM Copyright (c) 1997-1998 Symbian Ltd. All rights reserved.

INCLUDE "system.oxh"
INCLUDE "const.oph"
INCLUDE "toolbar.oph"

rem !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
rem !!!               IMPORTANT  W A R N I N G                         !!!
rem !!! The following UID has been reserved for the Skeleton app       !!!
rem !!! Don't use it for your own app - it will cause serious problems !!!
rem !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
rem The UID and name defined here must be changed to your own official UID
rem before use in your own application.
rem Not changing it will result in incorrect identification of your application
rem and of its documents by the System screen and by other system components.
rem Each application requires its own unique identifier (UID) - contact Symbian
rem to reserve a set of UIDs for use in your applications.
rem Official UIDs have values greater than &10000000

rem !!! Change this UID and this name
const KUidOplSkeletonApp&=&100002C4
const KAppName$="SkelOpl"

APP SkelOpl,KUidOplSkeletonApp&
	ICON "z:\System\Opl\SkelOpl.mbm"
	CAPTION KAppName$,KLangEnglish%
	FLAGS 1
ENDA

DECLARE EXTERNAL
EXTERNAL main:
EXTERNAL AppCont1:
EXTERNAL appInit:
EXTERNAL startScreen:
EXTERNAL initTBar:
EXTERNAL nextEv%:
EXTERNAL cmdTbDownS%:
EXTERNAL ProcessCmdLine:
EXTERNAL openFile:(file$)
EXTERNAL createFile:(file$)
EXTERNAL UpdateTBarTitle:
EXTERNAL border:
EXTERNAL cmdS%:
EXTERNAL cmdsS%:
EXTERNAL cmdC%:
EXTERNAL cmdN%:
EXTERNAL cmdB%:
EXTERNAL cmdT%:
EXTERNAL cmdX%:
EXTERNAL cmdE%:
EXTERNAL offrCmd%:(key&,modif&)
EXTERNAL OpenIniFile%:(AppUid&)
EXTERNAL SetLastUsedFile%:(FileName$,AppUid&)
EXTERNAL GetLastUsedFile$:(AppUid&)

const KNoBorderTbarButton%=1
const KBorderTbarButton%=2

proc main:
	rem app specific variables
	global ScrWid%		rem pixel width initially
	global ScrHght%		rem pixel height initially
	global MenuPos%		rem last menu position

	if KUidOplSkeletonApp&=&100002C4
		if 	KAppName$<>"SkelOpl"
			alert("The UID &"+hex$(KUidOplSkeletonApp&)+" has been reserved", "You need to reserve your own through Symbian")
			rem The UID defined above must be changed. Not changing it will result in incorrect identification of your application
			rem and of its documents by the System screen and by other system components.
			stop
		endif
	endif

	rem must be done before TBarInit:
	ScrWid%=gWidth
	ScrHght%=gHeight

	loadm "z:\system\opl\Toolbar"
	TBarLink:("AppCont1")	rem 'links' toolbar globals and then calls AppCont1
	rem TBarLink: calls AppCont1:
	rem Control does not return to here
endp

proc AppCont1:
	rem Continue from toolbar link TBarLink:

	global wIdMain%		rem main window
	global HasBord%		rem -1 if has border
	local justEntered% rem next drawing not relative to prev point

	rem For portability with Opl1993
	global HotKMod% rem modifier for hot-key
									rem Control-key on er1
	global ExitHK%	rem Hot-key for exit command

	rem Event	data
	global event&(16),evType&

	appInit:						rem also initialises TBar + draws it
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

proc appInit:
	rem set up globals
	external wIdMain%,HasBord%,HotKMod%,ExitHK%,event&()
	wIdMain%=1			rem Just use console for now
	HasBord%=-1
	gUpdate off
 	gSetPenWidth  2
	
	HotKMod%=4		rem Control-key
	ExitHK%=%e		rem e on EPOC machine

	rem Exit hot-key must be set up before toolbar
	initTBar:	rem create toolbar and show it
	startScreen:
	getEvent32 event&()
	gcls
	ProcessCmdLine:

  Border:
	at 1,1
endp

proc startScreen:
	rem set up your main window
	external wIdMain%
	gUse wIdMain%
rem	cursor wIdMain%,10,2,12
	gTMode 3	rem Replace text

rem screen 80,18,2,2
	gfont 11
	gAt 4,16
	gPrint "This sample Opl program shows how to develop OPL32 applications that support a"
	gAt 4,gY+16
	gPrint "menu, a toolbar and pen events. As with all other OPL sample releases, it has been"
	gAt 4,gY+16
	gPrint "written for use by potential OPL32 developers who already know OPL16 or who have"
	gAt 4,gY+16
	gPrint "access to an OPL16 programming manual. The source code has been provided so"
	gAt 4,gY+16
	gPrint "that you can find out how to implement new or changed OPL features. This program"
	gAt 4,gY+16
	gPrint "uses the module TOOLBAR.OPO which provides all the procedures required for"
	gAt 4,gY+16
	gPrint "toolbar support on an EPOC machine. Run TOOLBAR.OPO itself for help on usage."

	gAt 4,gY+32
	gPrint "This program also provides a typical OPL event loop, using GETEVENT32 to get pen"
	gAt 4,gY+16
	gPrint "and other events which are either used by the toolbar, for a menu command or"
	gAt 4,gY+16
	gPrint "passed to the application for its specific use."

	gAt 4, gY+32
	gPrint "For further details on porting to OPL32, see the manual."

	gAt 0,0
 	gBox gWidth, gHeight
	gSetPenWidth 2
 endp

proc initTBar:
	external ScrWid%,ScrHght%,ExitHK%
	TBarInit:("TBAR",ScrWid%,ScrHght%)
	TBarButt:("n",KNoBorderTbarButton%,"No"+chr$(10)+"border",0,&0,&0,KTbFlgLatchStart%)
	TBarButt:("b",KBorderTbarButton%,"Border",0,&0,&0,KTbFlgLatchEnd% or KTbFlgLatched%)
	TBarButt:("s",3,"Save"+chr$(10)+"file",0,&0,&0,KTbFlgCmdOnPtrDown%)
	TBarButt:(chr$(ExitHK%),4,"Close",0,&0,&0,0)
	TBarShow:
endp

proc nextEv%:
	rem Handles menu and toolbar events
	external event&(),wIdMain%,HotKMod%
	local evType&,command$(255),cmdLetter$(1)

	while 1
	  	GetEvent32 event&()
		evType&=event&(KEvaType%)

		if evType&=KEvCommand&
			command$=getcmd$
			cmdLetter$=left$(command$,1)
			command$=right$(command$,len(command$)-1)
			if cmdLetter$="X"
				cmdE%:
			elseif cmdLetter$="O"
				openFile:(command$)
			elseif cmdLetter$="C"
				createFile:(command$)
			endif
		elseif evType&=KEvPtr&
			if not TBarOffer%:(event&(KEvAPtrOplWindowId%),event&(KEvAPtrType%),event&(KEvAPtrPositionX%),event&(KEvAPtrPositionY%)) rem Handles tbar events
				break
			endif
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

proc cmdTbDownS%:
	external ScrWid%,TbWidth%
	local pop%
	
	rem popup next to button with point specifying the top right corner of the popup
	pop%=mPopup(ScrWid%-TbWidth%,97,KMPopupPosTopRight%,"Save",%s,"Save as"+chr$(KEllipsis&),%S)
	if pop%
		if pop%=%s
			cmdS%:
		else
			cmdsS%:
		endif
	endif
endp

proc processCmdLine:
	local file$(255),command$(1)
	command$=cmd$(3)
	if command$="R"
		file$=GetLastUsedFile$:(KUidOplSkeletonApp&)
		if file$<>"" and exist(file$)
			openFile:(file$)
			return
		endif
		if exist(cmd$(2))
			command$="O"
		else
			command$="C"
		endif
	endif
	file$=cmd$(2)
	if command$="O"
		openFile:(file$)
	elseif command$="C"
		createFile:(file$)
	endif
endp

proc openFile:(file$)
	external wIdMain%
	local b%,w%,h%
	setdoc file$
	b%=gloadbit(file$,0,0)
	w%=gWidth
	h%=gHeight
	gUse wIdMain%
	gCopy b%,0,0,w%,h%,3 
	gClose b%
	UpdateTBarTitle:
endp

proc createFile:(file$)
	trap delete file$
	setdoc file$
	UpdateTBarTitle:
endp

proc UpdateTBarTitle:
	local off%(6),file$(255)
	file$=getdoc$
	file$=parse$(file$,"",off%())
	TBarSetTitle:(right$(file$,len(file$)-off%(4)+1))
endp
	

proc border:
	gSetPenWidth 1
	gBorder 0
	gSetPenWidth 2
endp
 
proc cmdS%:
	gSaveBit getDoc$
endp

proc cmdsS%:
	local f$(255),offset%(6)
	f$=getDoc$
	f$=parse$(f$,"",offset%())
	f$=left$(f$,offset%(4)-1) rem Just the drive and path
	f$=SaveAsFileDialog$:(f$,#0)
	if len(f$)>0
		setDoc f$
		gSaveBit f$
		UpdateTBarTitle:
	endif	
endp

proc cmdC%:
	external wIdMain%,HasBord%
	local x%,y%

	gUse wIdMain%
	x%=gX
	y%=gY
	gcls
	cls
	if HasBord%
		border:
	endif
	gAt x%,y%
endp

proc cmdN%:
	external wIdMain%,HasBord%
	gUse wIdMain%
	if HasBord%
		TBarLatch:(KNoBorderTbarButton%)
		HasBord%=0
		gGMode 2	rem invert
		border:
		gGMode 0	rem set
	endif
endp

proc cmdB%:
	external wIdMain%,HasBord%
	gUse wIdMain%
	if HasBord%=0
		TBarLatch:(KBorderTbarButton%)
		HasBord%=-1
		border:
	endif
endp

proc cmdT%:
	external TbVis%
	if TbVis%
		TBarHide:
	else
		TBarShow:
	endif
rem	if autoSz%
rem		doMnSz%:
rem	endif
endp

proc cmdX%:
	external ExitHK%
	if ExitHK%=%x
		stop
	endif
	rem If %x on S5 defined, do it here
	raise -99	rem procedure not found
endp

proc cmdE%:
	rem Always saves
	external ExitHK%
	cmdS%:
	SetLastUsedFile%:(getdoc$,KUidOplSkeletonApp&)
	if ExitHK%=%e
		stop
	endif
	rem If %e on opl1993 defined, do it here
	raise -99	rem procedure not found
endp


proc offrCmd%:(key&,modif&)
	rem Returns -1 if command (menu or hot-key)
	rem also -1 if menu-key and cancelled

	external ExitHK%,TbMenuSym%,menuPos%,HotKMod%
	local isMenuK%
	local m%,k%,cmdRoot$(4)
	local mod%
	local hotK%

	mod%=modif&
	k%=key&
	if (k%=KKeyMenu32&) or (k%=KKeySidebarMenu32&)
		isMenuK%=-1
		mInit
			mCard "File","Save",%s,"Save as"+chr$(KEllipsis&),%S,"Close",ExitHK%
			mCard "View","Show toolbar",%t or TbMenuSym%,"Clear screen",-%c,"Border",%b,"No border",%n
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
		hotK%=k%-1+%a rem Control+a/A -> 1
	endif
	if mod% and HotKMod%	rem Hotkey modif, so maybe accelerator
		cmdRoot$="cmd"
		if mod% and 2
			cmdRoot$="cmds"
		endif
		rem print "call ";cmdRoot$+chr$(hotK%),hotK%
		onerr eNotCmd
		@%(cmdRoot$+chr$(hotK%)):
		return -1
	endif
eNotCmd::
	if isMenuK% or err<>-99
		giprint "Bug: Proc "+cmdRoot$+chr$(hotK%)+"%:,"+err$(err)
		return -1
	endif
	return 0
endp

proc OpenIniFile%:(AppUid&)
	local IniFile$(255),IniExt$(4),offset%(6)
	local handle%,ret%,uidType$(16)
	IniExt$=".ini"
	IniFile$=parse$(IniExt$,Cmd$(1),offset%())
	if exist(IniFile$)
		ret%=ioopen(handle%,IniFile$,$0300)
		if ret%
			if ret%=-39 rem Access denied
				return ret%
			else
				goto replace::
			endif
		endif
		ret%=ioread(handle%,addr(uidType$)+1,16)
		if ret%<>16 :goto replace:: :endif
		pokeb addr(uidType$),16
		if (uidType$<>CheckUid$:(0,0,AppUid&))
			goto replace::
		endif
		return handle%
replace::
		ioclose(handle%)
	endif
	ret%=ioopen(handle%,IniFile$,$0302)
	if ret% :return ret% :endif
	uidType$=CheckUid$:(0,0,AppUid&)
	ret%=iowrite(handle%,addr(uidType$)+1,16)
	if ret%
		ioclose(handle%)
		return ret%
	endif
	return handle%
endp

proc SetLastUsedFile%:(FileName$,AppUid&)
	local f$(255),handle%,ret%
	f$=FileName$
	handle%=OpenIniFile%:(AppUid&)
	if handle%<0
		return handle%
	endif
	ret%=iowrite(handle%,addr(f$),256)
	ioclose(handle%)
	return ret%
endp	

proc GetLastUsedFile$:(AppUid&)
	local f$(255),handle%,ret%
	handle%=OpenIniFile%:(AppUid&)
	if handle%<0
		return ""
	endif
	ret%=ioread(handle%,addr(f$),256)
	ioclose(handle%)
	if ret%<>256
		return ""
	endif
	return f$
endp
