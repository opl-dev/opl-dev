REM extends.tpl

rem The app declaration is causing problems for the CompareFilesL() test code...
rem APP fred,31131312
rem	FLAGS $1
	rem PATH "\FRED\JIM"
	rem EXT "JIM"
	rem ICON "C:\tOPLT\test.pic"
rem ENDA
	
	
	
PROC bugs%:
	REM Bugs in translator that have been fixed
	REM Also additions
	REM stack death on lots of minuses
	l%=--------------------------------------------------------------------------------------------------------------------------------1
		
	REM death caused by passing foreign characters to isReserved
	rem was: æ=1:å=2 but these fold to the same character i.e. to 'A' in code page 1252
	rem now: use char 230 µ and char 228 õ, which are mu and o with a tilde above 
	rem in code page 1252 ("windows latin 1")
	rem 230,228 µ,õ
	µ=1:õ=2
	
	REM unlex problem with field references
	print a.p&
	
	REM print on it's own used to do nothing now gives CrLf
	print
	
	REM Addr etc can now take the base value for arrays
	addr(a())
	addr(a(1))
	ioa(fcb%,0,status%,arg1(),arg2$())
		
	REM two different versions of ioopen depending on type of second argument
	ioopen(fcb%,"AString",0)
	ioopen(fcb%,aString$,0)
	ioopen(fcb%,addr(aString$),0)
	ioopen(fcb%,FLT(addr(AString$)),0)  rem just to be perverse
	
	REM os function added
REM	os(1,addr(a%()),addr(b%()))
REM	os(1,2)
	
	REM IO ADDITIONS
	IoYield
	IoWaitStat #23
	IoWaitStat status%
	
	l1::
	l2::
	l3::
	l4::
	l5::
	l6::
	l7::
	l8::
	REM Label references
	onerr OFF
	goto l1::
	goto l1
	onerr l1::
	onerr l1
	
	REM VECTOR
	VECTOR i%
	l1,l2,l3,l4::,l5
	REM check can deal with blank lines in vector statements
	l6::
	l7,l8::
	ENDV
	
	REM path and parsing functions
	print parse$(a1$,a2$,a%())
	print parse$(a1$,a2$,a%(1))
	print parse$(a1$,a2$,a%)
	    	
	REM odd extras brought with pan 
	rmDir "fred"
	mkDir "fred"
	setPath "fred"
	secsToDate s&,m%,m%,m%,m%,#1,#2,#3
	dateToSecs(1,2,3,4,5,6)
	
	REM indirect procedure calls
	@%("fred"):
	@&("jim"):(1)
	REM check can uses a proc names and arguments
	@(@$("sally"):(@%("fred"):,@&("jim"):(@("shiva"):))):
	ENDP
	
	PROC fbug:
	REM
	REM ancient translator bug 
	b.tcode$=a.tcode$		REM MUST BE FIRST LINE OF PROC
	ENDP
	PROC tgraph:
	REM Test the graphics functions added to the translator
	
  gCreate(1,2,3,4,5)
	gCreateBit(1,2)
	gsaveBit "fred"
	gSaveBit "fred",1,2
	gClose 1
	gUse 2
	gVisible ON
	gVisible OFF
	gLoadFont("fred")
	gFont 2
	gUnloadFont 2
	gGMode 17
	gTMode 12
	gStyle 12
	gOrder 1, 2
	gRank
	gIdentity
	gX
	gY
	gWidth
	gHeight
	gOriginX
	gOriginY
	gInfo32 a&()
	gInfo32 a&(10)
	gInfo32 #12+23
	gCls
	gAt 1,2
	gMove 1,2
	gTWidth("fred")
	gPrint	1,&2;3.2,"fred"
	gPrintClip("fred",12)
	gPrintB "fred",1
	gPrintB "fred",1,2
	gPrintB "fred",1,2,3
	gPrintB "fred",1,2,3,4
	gPrintB "fred",1,2,3,4,5
	gLineBy 1,2
	gLineTo 1,2
	gBox 1,2
	REM gCircle 12
	REM gEllipse 23,25
	gPoly a%()
	gPoly a%(12)
	gPoly #12
	gFill 1,2,3
	gPatt 1,2,3,4
	gCopy 1,2,3,4,5,6
	gScroll 1,2
	gScroll 1,2,3,4,5,6
	gUpDate
	gUpDate ON
	gUpDate OFF
	cursor ON
	cursor OFF
	cursor 1
	cursor 1,2,3,4
	screen 1,2
	screen 1,2,3,4
	gpeekLine 0,1,2,#3,4
	gpeekLine 0,1,2,a%(),4
	gpeekLine 0,1,2,a%(12),4
	gSetwin 1,2
	gSetWin 1,2,3,4
	gLoadBit("fred",1,2)
	gLoadBit("fred")
	gLoadBit("fred",1)
ENDP
	
PROC events:
	REM check for testevent and getevent
	
	
	GetEvent a%()
	GetEvent a%(1)
	GetEvent #3
	TestEvent
ENDP
	
PROC adds:
	REM Additions for pan
	
	gInvert 1,2
	gXPrint s$,f%
	CMD$(1)
	ENDP
	rem TEST VArious PAN specific opl extensions
	
	
	PROC test:
	
	gBorder $1
	gBorder w%,h%,f%
	
	gClock OFF
	gClock ON
	gClock ON,a%
	gClock ON,a%,b%
	gIPrint "Fred"
	gIPrint "fred",0
	ENDP
	proc x:
	REM - all the additions for Opl1993
	local size%
	
	gCreate(1,2,3,4,5)
	gCreate(1,2,3,4,5,6)
	
	cursor OFF
	cursor ON
	cursor 1
	cursor 1,2,3,4
	cursor 1,2,3,4,5
	
	gGrey 1
	defaultWin 1
	
	mCard "f2","f",1,"f",1,"f",1,"f",1,"f",1,"f",1,"f",1,"f",1
	
	menu
	menu(size%)
	menu(#size%)
	menu(#1+2)
	
rem	diamInit
rem	diamInit 1,"A"
rem	diamInit 1,"A","B"
rem	diamInit 1,"A","B","C"
rem	diamInit 1,"A","B","C","D","E","F","G","H","I","J","K","L","M"
	
rem	diamPos 1
rem	diamPos size%
	
	Font 1,2
	Style 3
endp
	
	
proc phse2:
	LOCAL a%,b%,c&,d,e$(10)
	LOCAL bitMap$(1,2)
	
	gClock ON,1,2,"FRED"
	gClock ON,1,2,"FRED",3
	gClock ON,1,2,"FRED",3,4
	
rem	createSprite
rem	a%=createSprite
	
rem	loadLib(b%,"FRED",1)
rem	a%=loadLib(#1,e$,b%)
	
rem	unloadLib(a%)
rem	y%=unloadLib(1)
	
	
rem	findLib(x%,"FRED")
rem	y%=findLib(#1,e$)
	
rem	getLibH(a%)
rem	b%=getLibH(12)
	
rem	newObj(1,2)
rem	a%=newObj(a%,b%)
	
rem	send(1,2)
rem	send(1,2,#3)
rem	send(1,2,#3,e$)
rem	send(1,2,e$,d,c&)
rem	send(1,2,c&,#2,d)
	
rem	enterSend(1,2)
rem	enterSend(1,2,#3)
rem	enterSend(1,2,#3,e$)
rem	enterSend(1,2,e$,d,c&)
rem	enterSend(1,2,c&,#2,d)
	
rem	enterSend0(1,2)
rem	enterSend0(1,2,#3)
rem	enterSend0(1,2,#3,e$)
rem	enterSend0(1,2,e$,d,c&)
rem	enterSend0(1,2,c&,#2,d)
	
	alloc(2)
	realloc(2,3)
	adjustAlloc(1,2,3)
	
	
rem	useSprite 1
rem	appendSprite 1,bitMap$()
rem	appendSprite 1,bitMap$(),2,3
	
rem	drawSprite 1,3
	
rem	changeSprite 1,2,bitmap$()
rem	changeSprite 1,2,bitmap$(),3,4
	
rem	posSprite 1,2
	
rem	closeSprite 1
	
	FreeAlloc 1
endp
	
	REM 
	REM Test file for wimp based opl commands
	REM As with other test files WILL NOT RUN
	
PROC dialogs:
	REM
	REM checks the dialog commands work properly
	REM
	LOCAL s1$(10),s2$(20),s3$(30)
	LOCAL l1&,l2&,l3&
	LOCAL i1%,i2%,i3%
	LOCAL f1,f2,f3
	
	dInit
	dInit "Fred"
	dText s1$,s2$
	dText s1$,s2$,i1%
	dChoice i1%,s1$,s2$
	dLong l1&,s1$,l2&,l3&
	dTime l1&,s1$,i1%,l2&,l3&
	dDate l1&,s1$,l2&,l3&
	dEdit s1$,s2$
	dEdit s1$,s2$,i1%
	dXinput s1$,s2$
	dFile s1$,s2$,i1%
	dButtons s1$,i1%
	dButtons s1$,i1%,s2$,i2%
	dButtons s1$,i1%,s2$,i2%,s3$,i3%
	dFloat f1,s1$,f2,f3
	dialog
ENDP
	
PROC Menus:
	REM
	REM Checks menus translate OK
	REM
	LOCAL s1$(10),s2$(10),s3$(10),s4$(10),s5$(10),s6$(10),t$(10)
	
	mInit
	mCard t$,s1$,1
	mCard t$,s1$,1,s2$,2
	mCard t$,s1$,1,s2$,2,s3$,3
	mCard t$,s1$,1,s2$,2,s3$,3,s4$,4
	mCard t$,s1$,1,s2$,2,s3$,3,s4$,4,s5$,5
	mCard t$,s1$,1,s2$,2,s3$,3,s4$,4,s5$,5,s6$,6
	menu
ENDP
	
	
PROC setS:
	
rem	setName "FRED"
rem	statusWin ON
rem	statusWin OFF
	busy OFF
	busy "Fred"
	busy "fred",-1
	busy "fred",-1,20
	lock ON
	lock OFF
	GETCMD$
ENDP

