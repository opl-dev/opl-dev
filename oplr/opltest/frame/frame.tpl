REM Frame for OplER1

INCLUDE "Const.oph"
INCLUDE "system.oxh"
INCLUDE "Toolbar.oph"

REM For convience sake, frame.tph is included inline:
REM INCLUDE "frame.tph"

REM Must get a real Uid from Symbian sometime! 
CONST KUidFrame& = &153112B5

CONST KApprsc$    = ".rsc"
CONST KAppEng$		= "Frame"

CONST KAppName$   = "Frame"
REM CONST KRSCroot$   = "Frame"
CONST KAppdir$    = "\system\apps\frame\"
CONST KAppini$    = ".ini"
CONST KApphlp$    = ".hlp"

REM Events
CONST KCSystemEvent%=0
CONST KCUserEvent%=1

REM Menu unassigned options.
CONST KCMenuNoKey1%=1

REM For OpenINIFile%:
REM CONST KCRead%=1
REM CONST KCWrite%=2

REM Debug values
REM Debug window rank.
CONST KCDebugTopRank%=2

REM Color stuff.
CONST kColorMode%=5

REM End of frame.tph inline inclusion.

CONST KReadMode%=0
CONST KWriteMode%=1

CONST KOplAlignment%=1 rem 0 for lbc1, 1 for LBC2.
CONST KUnicodeCharLen%=2 rem 1 for ASCII, 2 for Unicode machines.

REM 7Sep98 Added pointerfilter.

DECLARE EXTERNAL
EXTERNAL Frame:
EXTERNAL FrameMain:
EXTERNAL GetEv%:
EXTERNAL MoveCur%:(akey%,aKeyMod%)
EXTERNAL HandAct%:(aCmd$)
EXTERNAL NewFile:(aEventType%)
EXTERNAL OpenFile:(aEventType%)
EXTERNAL GetName%:(aTitle$,aType%)
EXTERNAL Revert:
EXTERNAL Init:
EXTERNAL Read_Ini:
EXTERNAL ReadINIRec$:(aDefaultValue$)
EXTERNAL Write_Ini:
EXTERNAL WriteINIRec%:(aDefaultValue$)
EXTERNAL OpenINIFile%:(aAppUid&,aMode%)
EXTERNAL GetLang:
EXTERNAL InitTBar:
EXTERNAL InitTopTbar:
EXTERNAL StatVis:
EXTERNAL initStat:
EXTERNAL TopToolVis:
EXTERNAL Help:
EXTERNAL KillHelp:
EXTERNAL WhereIs$:(aPath$)
EXTERNAL UpdateTBarTitle:
EXTERNAL InitScr:
EXTERNAL Pref:
EXTERNAL DebugPref:
EXTERNAL PointerFilterPref:
EXTERNAL Save:
EXTERNAL SaveAs:
EXTERNAL LowSave:(aName$,aInformF%)
EXTERNAL LowLoad%:(aName$)
EXTERNAL CmdE%:
EXTERNAL CmdA%:
EXTERNAL CmdC:
EXTERNAL CmdTBDownP%:
EXTERNAL ToolVis:
EXTERNAL DemoA:
EXTERNAL DemoC:
EXTERNAL DoSomething:
EXTERNAL SelectAll:
EXTERNAL About:
EXTERNAL Copy:
EXTERNAL Delete:
EXTERNAL Export:
EXTERNAL MergeIn:
EXTERNAL Print:
EXTERNAL PrintSetup:
EXTERNAL PageSetup:
EXTERNAL Paste:
EXTERNAL PrintPreview:
EXTERNAL Cut:
EXTERNAL Redo:
EXTERNAL Undo:
EXTERNAL CutPaste:
EXTERNAL IrTrans:
EXTERNAL ZoomIn:
EXTERNAL ZoomOut:
EXTERNAL Color:
EXTERNAL Setcolor:(aR%,aG%,aB%)

CONST NYI$ = " Not yet implemented"

APP Frame, KUidFrame&
	ICON "frameIcon.mbm"
	ICON "frameIconMask.mbm"
	CAPTION KAppName$, KLangEnglish%
	FLAGS 1
ENDA

PROC Frame:
	GLOBAL gScrHt%, gScrWid%

	gScrWid% = gWIDTH
	gScrHt% = gHEIGHT

	REM Enable the receipt of machine switch-on events
	SETFLAGS KSendSwitchOnMessage&
	REM Maybe disable this in future?

	Getlang:

	LOADM "Z:\System\Opl\Toolbar.opo"
	TBarLink:("FrameMain")
	dINIT "Abnormal exit from TBarLink"
	DIALOG
ENDP

PROC FrameMain:
	EXTERNAL TbVis%

	GLOBAL gFile$(255), gNFile$(255), gLUFile$(255)
	GLOBAL gExit%
	GLOBAL gEvent&(16)
	GLOBAL gWId%
	GLOBAL gHelpId&, gHStatus&
	GLOBAL gINIfile$(255)
	GLOBAL gDebugF%
	GLOBAL gDeltaF%	REM Bool flag true if a change has been made.
	GLOBAL gFileDeltaF% REM Bool flag true if the file has changed since loading.
	GLOBAL gTopTBVis%	REM Top toolbar visible flag.
	GLOBAL gStatVis%	REM Status window visible.
	REM Pointer Filter stuff
	GLOBAL gPointerFilterF%, gPFFilter%, gPFMask%


	LOCAL EventType&
	LOCAL justInF%
	LOCAL key%, kmod%, menu%
	LOCAL menuPos%
	LOCAL TBon% REM simple bool flag menu toolbar.
	LOCAL TopTBon% REM Top toolbar bool flag.
	LOCAL SWon%	REM Status window on.
	LOCAL RevertDim% REM Grey out revert in menu.
	LOCAL HotF% REM Hotkey flag.
	LOCAL scrDbg%	REM toggle debug screen.

	scrDbg%=1
	justInF%=KTrue%
	Init:

	REM Process command line.
	gNFile$ = CMD$(KCmdUsedFile%)
	HandAct%:( CMD$(KCmdLetter%) )

	REM the main loop.
	DO

		REM Check the change flag.
		IF gDeltaF%
			gFileDeltaF% = KTrue%
			gDeltaF% = KFalse%
			rem if Zoom on
				rem update zoom window
			rem endif
		ENDIF

		GetEv%:

		REM In the future, this functionality will be somewhere
		REM else, so calling GetEv%: will return a key press.

		EventType& = gEvent&(KEvaType%)
		IF gDebugF%
			PRINT "FrameMain: evt="; HEX$(EventType&),
		ENDIF

		IF EventType&=KEvPtr&
			REM PRINT "KEvPtr"
			IF gEvent&(KEvAPtrType%)=KEvPtrDrag&
				IF JustInF%
					gAT gEvent&(KEvAPtrScreenPosX%), gEvent&(KEvAPtrScreenPosY%)
					justInF%=KFalse%
				ENDIF
				gLINETO gEvent&(KEvAPtrScreenPosX%), gEvent&(KEvAPtrScreenPosY%)
				gDeltaF%=KTrue%
			ELSEIF gEvent&(KEvAPtrType%) = KEvPtrPenDown&
				justInF%=KFalse%
				gAT gEvent&(KEvAPtrScreenPosX%), gEvent&(KEvAPtrScreenPosY%)
				gLINEBY 0,0
				gDeltaF%=KTrue%
			ENDIF
			CONTINUE

		ELSEIF EventType&=KEvPtrEnter&
			REM PRINT "KEvPtrEnter&"
			justInF%=KTrue%
			CONTINUE

		ELSEIF EventType&=KevPtrExit&
			REM PRINT "KEvPtrExit&"
			CONTINUE

		ENDIF

		REM To get this far, it must be a keypress.

		key% = gEvent&(KEvaType%)	REM AND $FF
		kmod% = gEvent&(KEvAKMod%)
		PRINT "Key=";key%, "KMod=";kmod%

		REM If its the sidebar (anything other than menu...)
		IF (key%>KKeySidebarMenu%) AND (key%<=KKeySidebarMenu%+4)
			IF key%=KKeySidebarMenu%+1 :CutPaste:
			ELSEIF key%=KKeySidebarMenu%+2 :Irtrans:
			ELSEIF key%=KKeySidebarMenu%+3 :ZoomIn:
			ELSEIF key%=KKeySidebarMenu%+4 :ZoomOut:
			ENDIF
			CONTINUE
		ENDIF

		HotF%=KFalse%
		IF (key%<=26)
			IF kMod% AND 4	REM Ctrl
				HotF%=KTrue%
			ENDIF
		ENDIF

		REM Check for Menu or hotkey.
		IF ( (key%=KKeyMenu%) OR (key%=KKeySidebarMenu%) OR (HotF%=KTrue%) )
			IF key%=KKeyMenu% OR key%=KKeySidebarMenu%
				PRINT "MENU"
			ELSE
				PRINT "HOT"
			ENDIF

			IF ( (key%=KKeyMenu%) OR (key%=KKeySidebarMenu%) )
				REM Build menu ctors.
				IF TbVis% :TbOn%=$2000 :ELSE :TbOn%=0 :ENDIF
				IF gTopTbVis% :TopTbOn%=$2000 :ELSE :TopTbOn%=0 :ENDIF
				IF gStatVis% :SWOn%=$2000 :ELSE :SWOn%=0 :ENDIF
				IF gFileDeltaF% :RevertDim%=0 :ELSE :RevertDim%=KMenuDimmed% :ENDIF

				mINIT
				mCASC "Printing", "Page setup"+CHR$(KEllipsis&), %U, "Print setup"+CHR$(KEllipsis&), %P, "Print preview"+CHR$(KEllipsis&), %V, "Print"+CHR$(KEllipsis&), %p
				mCASC "More", "Save as"+CHR$(KEllipsis&), %S, "Save", %s, "Revert to saved", %r OR RevertDim%, "Merge in"+CHR$(KEllipsis&), %I, "Export"+CHR$(KEllipsis&), %E
				mCARD "File", "Create new file"+CHR$(KEllipsis&),%n, "Open file"+CHR$(KEllipsis&), %o, "Printing>",16, "More>",-16, "Close", %e
				mCARD "Edit","Undo", %z, "Redo", -%y, "Cut", %x, "Copy", %c, "Paste", -%v, "Delete all", %d, "Select all", %a
				mCARD "View", "Zoom in", %m, "Zoom out", -%M, "Show toolbar",%t OR KMenuCheckBox% OR TbOn%,"Show top toolbar",%T OR KMenuCheckBox% OR TopTbOn%, "Show status window", %C OR KMenuCheckBox% OR SWOn%
				mCARD "Tools", "Preferences"+CHR$(KEllipsis&), %k, "Some other stuff here", -KCMenuNokey1%, "Colour", -%u, "About", %A, "Help", %H

				LOCK ON
				menu%=MENU(MenuPos%)
				LOCK OFF

				IF menu%=0
					REM Esc hit.
					CONTINUE
				ENDIF

				key%=menu%

			ELSE
				REM Must be hotkey.
				key% = key% - 1 + %a
				IF kmod% AND 2
					key% = key% + (%A - %a)
				ENDIF
			ENDIF

			PRINT "Now key=";key%

			IF key%=KCMenuNokey1% REM Unassigned menu option #1
				DoSomething:
			ELSEIF key%=%a :SelectAll:
			ELSEIF key%=%A :About:
			ELSEIF key%=%c :Copy:
			ELSEIF key%=%C :StatVis:
			ELSEIF key%=%d :Delete:
			ELSEIF key%=%e :gExit%=KTrue%
			ELSEIF key%=%E :Export:


			ELSEIF key%=%H :Help:
			ELSEIF key%=%I :MergeIn:
			ELSEIF key%=%k :Pref:
			ELSEIF key%=%m :ZoomIn:
			ELSEIF key%=%M :ZoomOut:
			ELSEIF key%=%n :NewFile:(KCUserEvent%)
			ELSEIF key%=%o :OpenFile:(KCUserEvent%)
			ELSEIF key%=%p :Print:
			ELSEIF key%=%P :PrintSetup:
			ELSEIF key%=%r :Revert:
			ELSEIF key%=%s :Save:
			ELSEIF key%=%S :SaveAs:

			ELSEIF key%=%t :ToolVis:
			ELSEIF key%=%T :TopToolVis:
			ELSEIF key%=%u :Color:
			ELSEIF key%=%U :PageSetup:

			ELSEIF key%=%v :Paste:
			ELSEIF key%=%V :PrintPreview:
			ELSEIF key%=%x :Cut:
			ELSEIF key%=%y :Redo:
			ELSEIF key%=%z :Undo:
			ENDIF	REM End of huge if statement.

		REM End of hotkey processing.

		ELSEIF MoveCur%:(key%, kmod%)

		ELSEIF key%=9	REM Tab
			REM Toggle the debug screen.
			IF scrDbg%=KCDebugTopRank%
				scrDbg%=255
			ELSE
				scrDbg%=KCDebugTopRank%
			ENDIF
			gORDER 1,scrDbg%

		ENDIF REM Of key pressing.

REM Can't put much here because pen actions
REM don't get down here these days...
	
	UNTIL gExit%=KTrue%

	IF gFileDeltaF% = KTrue%
		Save:
	ENDIF

	REM Close INI etc.
	Write_INI:

	KillHelp:

	REM If debug window is top, let me see the final
	REM debug trace, otherwise exit.
	IF scrDbg%=KCDebugTopRank%
		PRINT "Exit."
		GET
	ENDIF

	STOP
ENDP

PROC GetEv%:
	EXTERNAL gDebugF%,gPointerFilterF%,gPFFilter%,gPFMask%
	EXTERNAL gExit%,gEvent&(),gWid%,gNFile$

	LOCAL type$(1)
	LOCAL proF%	REM Processed flag.
	LOCAL EventType&

	IF gDebugF%
		PRINT "[";
	ENDIF

	IF gDebugF%
		IF gPointerFilterF%
			POINTERFILTER gPFFilter%, gPFMask%
		ENDIF
	ENDIF

	WHILE gExit% = KFalse%
		GETEVENT32 gEvent&()
		EventType& = gEvent&( KEvaType% )

		IF gDebugF%
			PRINT ">";
		ENDIF
		REM PRINT "Event=";HEX$(EventType&),

		REM Examine eventType
		IF eventType& = KEvPtr&
			REM For Pointer events...
			proF%=TBarOffer%:(gEvent&(KEvAPtrOplWindowId%), gEvent&(KEvAPtrType%), gEvent&(KEvAPtrPositionX%), gEvent&(KEvAPtrPositionY%))
			IF proF%
				REM TB handled it.
			ELSE
				REM Pass it to our handler to check the window...
				IF gDebugF%
					PRINT "KEvPtr",
				ENDIF
				IF gEvent&(KEvAPtrOplWindowId%) = gWId%
					BREAK
				ENDIF
			ENDIF

		ELSEIF eventType& = KEvPtrEnter&
			IF gDebugF%
				PRINT "KEvPtrEnter",
			ENDIF
			IF gEvent&(KEvAPtrOplWindowId%) = gWId%
				BREAK
			ENDIF

		ELSEIF eventType& = KEvCommand&
			gNFile$ = GETCMD$
			type$ = LEFT$(gNFile$,1)
			gNFile$ = MID$(gNFile$,2,255)
			HandAct%:( type$ )

		ELSEIF eventType&=0
			REM Null event.
			CONTINUE

		ELSEIF ( (eventType&=KEvKeyDown&) OR (eventType&=KEvKeyUp&) )
			CONTINUE

		ELSEIF ( (eventType& = KEvFocusGained&) OR (eventType& = KEvFocusLost&) )
			IF gDebugF%
				PRINT "Focus!",
			ENDIF
			CONTINUE

		ELSEIF eventType& = KEvSwitchOn&
			CONTINUE

		ELSEIF (eventType& AND KKeySidebarMenu%)
			REM PRINT "sidebar",
			BREAK

		ELSEIF (eventType& AND $400)=0
			REM PRINT "event_keypress",
			REM keypress.
			BREAK

		ELSE
			PRINT "Ignored event=0x";HEX$(eventType&), EventType&,
		ENDIF
	ENDWH
	IF gDebugF%
		PRINT "]"
	ENDIF
ENDP

PROC MoveCur%:(key%, kmod%)
	PRINT "^",
ENDP

PROC HandAct%:(e$)
	REM Process EPOC command events, using
	REM the new filename to allow old one
	REM to be saved etc.
	REM The new name always (ends up in) gNFile$.

	EXTERNAL gExit%,gNFile$,gLUFile$

	LOCAL file$(255)
	LOCAL command$(1)

	command$ = e$

	IF e$="X"
		gExit% = KTrue%
		RETURN

	ELSEIF e$=KCmdLetterRun$
		PRINT "HandAct: Info - gNFile$="; gNFile$
		REM Now ignore gNFile$

		file$=gLUFile$ REM GetLastUsedFile$:(KUidFrame&)
		PRINT "HandAct: LU File$ is", file$
		IF file$<>"" AND EXIST(file$)
			gNFile$ = file$
			OpenFile:(KCSystemEvent%)
			RETURN
		ENDIF

		REM If not found, try using the command line arg.
		IF EXIST( gNFile$ )
			command$ = KCmdLetterOpen$
		ELSE
			command$ = KCmdLetterCreate$
		ENDIF
		REM And fall through into next bit...
	ENDIF

	IF command$ = KCmdLetterOpen$
		OpenFile:( KCSystemEvent% )
	ELSEIF command$ = KCmdLetterCreate$
		NewFile:( KCSystemEvent% )
	ENDIF
ENDP

PROC NewFile:( eventType% )
	EXTERNAL gFile$,gWId%,gNFile$
	EXTERNAL gScrWid%,gScrHt%

	PRINT "NewFile:";

	REM First things first. Save the old file
	REM before even talking to the user.
	IF gFile$ = ""
		PRINT "No old file to save"
	ELSE
		PRINT "Saving old file ["; gFile$; "]"
		IF gWId%
			LowSave:( gFile$, KTrue% ) REM Let user know.
		ELSE
			PRINT "NewFile: no gWId to save"
		ENDIF
	ENDIF

	IF eventType%=KCUserEvent%
		PRINT "User event: get name"
		IF GetName%:("Create new file",KDFileEditBox%+KDFileEditorQueryExisting%)
		ELSE
			RETURN
		ENDIF
	ELSE
		PRINT "System event"
	ENDIF
	PRINT "["; gNFile$; "]"

	IF EXIST(gNFile$)
		PRINT "Created file ["; gNFile$;
		PRINT "] already exists"
	ENDIF
	gFile$ = gNFile$
	SETDOC gFile$
	IF gWId%
		gCLOSE gWId% :gWId%=0
	ENDIF

	gWId% = gCREATE(0,0, gScrWid%, gScrHt%, 1, kColorMode%)
	UpdateTBarTitle:
	InitScr:

	REM Save it here, to match fn of Sketch.
	REM This also provides something to Revert to.
	LowSave:( gFile$, KFalse% ) REM Don't tell user.
	REM It's safe to not tell because this new
	REM file can't be very big, therefore it won't
	REM take long to save!
ENDP

PROC OpenFile:(eventType%)
	REM Opens the file in gNFile$, but trys to save
	REM the file in gFile$ first.

	EXTERNAL gFileDeltaF%,gFile$,gNFile$,gWId%
	
	LOCAL newId%

	PRINT "OpenFile:";

	REM Save the current one if it needs it.
	IF gFileDeltaF%=KTrue%
		IF gFile$ = ""
			PRINT "No old file to save"
		ELSE
			PRINT "Saving old file ["; gFile$; "]"
			LowSave:( gFile$, KTrue% ) REM tell user.
		ENDIF
	ENDIF
	
	IF eventType%=KCUserEvent%
		PRINT "User event: get name"		
		IF GetName%:("Open file",0)
		ELSE
			RETURN
		ENDIF
	ELSE
		PRINT "System event"
	ENDIF
	PRINT "["; gNFile$; "]"

	IF NOT EXIST(gNFile$)
		GIPRINT "File does not exist"
		RETURN
	ENDIF

REM Attempt to load it before changing names etc.
REM OK, for a second you need 2x memory for bitmaps
REM but with a 8MB machine...
REM One day, make this an option.

	REM Untrusted new file.
	newId% = LowLoad%:( gNFile$ )
	IF newId%=0
		GIPRINT "Error loading file"
	ELSE
		gFile$ = gNFile$
		IF gWid%
			gCLOSE gWId% :gWId%=0
		ENDIF
		gWId% = newId%
		SETDOC gFile$
		UpdateTBarTitle:
		InitScr:
	ENDIF
ENDP

PROC GetName%:(title$, type%)
	EXTERNAL gNFile$
	LOCAL dial%
	dINIT title$
	dFILE gNFile$, "File,Folder,Disk", type% OR KDFileSelectorWithSystem%, 0,0,KUidFrame&
	LOCK ON
	dial%=DIALOG
	LOCK OFF
	IF dial%
		RETURN KTrue%
	ELSE
		RETURN KFalse%
	ENDIF
ENDP

PROC Revert:
	EXTERNAL gFileDeltaF%,gFile$,gNFile$
	LOCAL dial%
	IF gFileDeltaF% = KFalse%
		GIPRINT "File has not changed", KBusyTopRight%
		RETURN
	ENDIF

	REM In case gFile$ does not exist for new files.
	IF NOT EXIST(gFile$)
		GIPRINT "No file to revert to", KBusyTopRight%
		RETURN
	ENDIF

	dINIT "Revert to saved?"
	dTEXT "", "All changes will be lost"
	dBUTTONS "No",-(%N OR KDButtonNoLabel% OR KDButtonPlainKey%), "Yes", (%Y OR KDButtonNoLabel% OR KDButtonPlainKey%)
	LOCK ON
	dial%=DIALOG
	LOCK OFF
	IF dial%<>%y
		RETURN
	ENDIF

	REM Revert it by pretending there's a system
	REM event causing it to open.
	gFileDeltaF% = KFalse%	REM Prevent current file being saved.
	REM (Openfile needs gNFile!)
	gNFile$=gFile$
	OpenFile:(KCSystemEvent%)
ENDP

PROC Init:
	EXTERNAL gWId%,gDebugF%
	REM Nice courier, mono-spaced font.
	gWId%=0
	
	Read_ini:	REM This sets the debug flag.

	IF gDebugF%
		FONT KFontCourierNormal13&, 16
		SCREEN 64,18, 1,1
	ELSE
		REM switch screen off?
		REM !!TODO
	ENDIF

	DEFAULTWIN 5
	
	initTBar:
	initTopTBar:
	REM initStat:

	REM Main window into background.
	REM gORDER 1,255
ENDP

PROC Read_ini:
	EXTERNAL gLUFile$,gDebugF%,TBVis%,gTopTBVis%,gStatVis%
	GLOBAL glh%	REM INI handle
	LOCAL check$(10)

	ONERR closeit::
	glh% = OpenINIFile%:(KUidFrame&,KReadMode%)

	PRINT "Read_ini: Opened INI="; glh%

	
	IF glh% :gLUFile$ = readINIrec$:( "" ) :ENDIF
	IF glh% :gDebugF% = VAL( readINIrec$:( "0" )) :ENDIF
	IF glh% :TBVis% = VAL( readINIrec$:( "-1" )) :ENDIF
	IF glh% :gTopTBVis% = VAL( readINIrec$:( "1" )) :ENDIF
	IF glh% :gStatVis% = VAL( readINIrec$:( "0" )) :ENDIF
	IF glh% :check$ = readINIrec$:( "101" ) :ENDIF

	IF gDebugF%
		print "read_ini: LastUsedFile="; gLUFile$
		PRINT "read_ini: debug="; gDebugF%
	ENDIF

closeit::
	REM End of INI file.
	ONERR OFF
	IOCLOSE(glh%)
ENDP

PROC ReadINIrec$:(default$)
	EXTERNAL glh%
	LOCAL value$(255), ret%
	IF glh%
		ret% = IOREAD(glh%, ADDR(value$), 256)
		IF ret%<>256
			PRINT "readInirec: duff read=";ret%
			if ret%<0 :print "readinirec: "+ERR$(ret%) :endif
			IOCLOSE(glh%)
			glh%=0
		ELSE
			print "readinirec: read=";value$
		ENDIF
	ELSE
		print "readini: no handle, using default"
		value$ = default$
	ENDIF
	RETURN value$
ENDP

PROC Write_ini:
	EXTERNAL gFile$,gDebugF%,TBVis%,gTopTBVis%,gStatVis%
	GLOBAL glh%
	ONERR closeit::
	glh% = OpenINIFile%:(KUidFrame&,KWriteMode%)
	PRINT "Write_INI: handle="; glh%
	IF glh%=0
		REM Can't do anything here.
		print "write_ini: no handle, abort"
		RETURN
	ENDIF

	REM Last used name is one active when program is closed.
	IF glh% :writeINIrec%:( gFile$ ) :ENDIF
	IF glh% :writeINIrec%:( NUM$(gDebugF%,5) ) :ENDIF
	IF glh% :writeINIrec%:( NUM$(TBVis%,5) ) :ENDIF
	IF glh% :writeINIrec%:( NUM$(gTopTBVis%,5) ) :ENDIF
	IF glh% :writeINIrec%:( NUM$(gStatVis%,5) ) :ENDIF
	REM And the check...
	IF glh% :writeINIrec%:( "314" ) :ENDIF

closeit::
	ONERR OFF
	REM End of INI file.
	IOCLOSE(glh%)
	glh%=0
ENDP

PROC WriteINIrec%:(aValue$)
	EXTERNAL glh%
	LOCAL ret%, value$(255)
	IF glh%=0
		print "writeinirec: no handle"
	ELSE
		value$ = aValue$
		print "WriteINIrec%: writing [";value$;"]"
		ret% = IOWRITE (glh%, ADDR(value$), 256)
		IF ret%<0
			PRINT "WriteINIrec: failed to write ";value$;
			PRINT " Error="; ret%; " = "; ERR$(ret%)
			IOCLOSE(glh%)
			glh%=0
		ENDIF
	ENDIF
ENDP

PROC OpenIniFile%:(AppUid&,aMode%)
	LOCAL IniFile$(255),handle%,ret%,uidType$(16),mode%
	mode%=KIoOpenModeOpen% OR KIoOpenFormatBinary%
	IniFile$="c:\system\apps\frame\frame.ini"
	print "OpenINIfile:",inifile$

	IF aMode%=KReadMode%
		print "Read mode"
		IF NOT EXIST(IniFile$) :print "no exist" :RETURN 0 :ENDIF
		ret%=IOOPEN(handle%,IniFile$,mode%)
		IF ret%<0 :print ERR$(ret%) :RETURN 0 :ENDIF
		ret%=IOREAD(handle%,ADDR(uidType$)+1+KOplAlignment%,16*KUnicodeCharLen%)
		if ret%<>16*KUnicodeCharLen% :print "ioread=",ret% :ioclose(handle%) :return 0 :endif
		POKEB ADDR(uidType$),16 rem KUnicodeCharLen%
		if (uidType$<>CheckUid$:(0,0,AppUid&))
			print "bad UIDs"
			print "[";uidType$;"][";CheckUid$:(0,0,AppUid&)
			ioclose(handle%)
			return 0
		endif
	ELSE
		mode%=KIoOpenModeReplace% OR KIoOpenFormatBinary% OR KIoOpenAccessUpdate%
		print "opening for write."
		ret%=ioopen(handle%,IniFile$,mode%)
		if ret%<0 :print ERR$(ret%) :return 0 :endif
		uidType$=CheckUid$:(0,0,AppUid&)
		print "writing checkuid"
		ret%=IOWRITE(handle%,ADDR(uidType$)+1+KOplAlignment%,16*KUnicodeCharLen%)
		IF ret%<0 :print err$(ret%) :ioclose(handle%) :RETURN 0 :ENDIF
	ENDIF
	RETURN handle%
ENDP

PROC GetLang:
	PRINT "!!TODO:GetLang:"
ENDP

PROC initTBar:
	EXTERNAL gScrWid%, gScrHt%, TBVis%

	REM set up the Toolbar buttons and actions
	LOCAL mbmTbar$(50), bitmapid1&, bitmapid2&
	LOCAL c1&,c2&,c3&,c4&,c5&,c6&

	REM Have to deliver this file to end-user.
	mbmTbar$="c:"+KAppDir$+"buttons.mbm"

	bitmapId1&=gLoadBit(mbmTbar$,0,0)
	bitmapId2&=gLoadBit(mbmTbar$,0,1)

	rem !!TODO lose this. for visibility (screen too narrow!)
	rem c1&=kRgbMagenta&
	rem c2&=KRgbYellow&
	rem c3&=KRgbDarkRed&
	rem c4&=kRgbYellow&
	rem c5&=kRgbDarkBlue&
	rem c6&=kRgbDarkmagenta&
	REM New in 1.02
	rem TBarColor:(KColorgCreate256ColorMode%, c1&, c2&, c3&, c4&, c5&, c6&)

	REM If that wasn't called, the default toolbar mode/colors are used...

	REM Set title above Toolbar.
	REM Use appName for the docname for now...
	TBarInit:(kAppname$, gScrWid%, gScrHt%)

	TBarButt:("c",1,"Color",0,bitmapid1&,bitmapid1&,0)
	TBarButt:("a",2,"Rubber",0,bitmapid2&,bitmapid2&,0)
	TBarButt:("p",3,"Popup"+chr$(10)+"demo",0,&0,&0,1)
	TBarButt:("e",4,"Exit",0,&0,&0,0)

	IF TbVis%
		TBarShow:
	ELSE
			TBarHide:
	ENDIF
ENDP

PROC initTopTBar:
	EXTERNAL gTopTBVis%
	gTopTBVis%=KFalse%
ENDP

PROC StatVis:
	EXTERNAL gStatVis%
	IF gStatVis%=KTrue%
		gStatVis%=KFalse%
		REM kill stat window
	ELSE
		gStatVis%=KTrue%
		initStat:
	ENDIF
ENDP

PROC initStat:
	REM Initialise the status window.
	GIPRINT "initStat:" + NYI$
ENDP


PROC TopToolVis:
	EXTERNAL gTopTBVis%
	GIPRINT "TopToolVis" + NYI$
	IF gTopTBVis%=KTrue%
		gTopTBVis%=KFalse%
	ELSE
		gTopTBVis%=KTrue%
	ENDIF
ENDP

PROC Help:
	EXTERNAL gHelpId&,gHStatus&
	LOCAL fullpath$(255), previous&, ret&

	IF gHelpId& = 0
		fullpath$ = WhereIs$:( KAppdir$ + KAppname$ + KApphlp$ )
		IF fullpath$ = ""
			GIPRINT "Help file not found"
			RETURN
		ENDIF
		gHelpId& = RUNAPP&:("Data", fullpath$, "", 0)
		LOGONTOTHREAD:(gHelpId&, gHStatus&)
	ELSE
		REM Allow status word to update.
		IOYIELD
		REM If it's still running...
		IF gHStatus& = KStatusPending32&
			ret&=SETFOREGROUNDBYTHREAD&:(gHelpId&,previous&)
		ELSEIF gHStatus& = 0
			gHelpId& = 0
			REM Recall this proc to start Help.
			Help:
		ENDIF
	ENDIF
	RETURN
ENDP

PROC KillHelp:
	EXTERNAL gHelpId&
	REM On exit, make sure the help thread dies.
	REM GIPRINT "!!TODO - KillHelp"
	IF gHelpId&<>0
		ONERR dieDieDie::
		ENDTASK&:(gHelpId&,0)
	ENDIF
diediedie::
	ONERR OFF
	RETURN
ENDP

PROC whereIs$:( path$ ) REM Hunt a file on different disks.
	LOCAL fullpath$(255), off%(6), disk$(2)
	LOCAL search$(2), search%
	
	REM List of disks to search, in search order.
	REM (Remember default disk gets checked first anyway.)
	search$="CD"

	search%=0
	DO
		REM Check the default disk first.
		IF search%=0
			disk$ = ""
		ELSE
			disk$ = MID$( search$, search%, 1 ) + ":"
		ENDIF
		
		fullpath$ = PARSE$( path$, disk$, off%())
		IF EXIST(fullpath$)
			RETURN fullpath$
		ENDIF
		
		search%=search%+1
	UNTIL search%>LEN(search$)
	RETURN ""
ENDP

PROC UpdateTBarTitle:
	local off%(6),file$(255)
	file$=GETDOC$
	file$=PARSE$(file$,"",off%())
	TBarSetTitle:(RIGHT$(file$, LEN(file$)-off%(4)+1))
ENDP

PROC InitScr:
	EXTERNAL gWid%,TBVis%
	gUSE gWId%
	IF TBVis%
		TBarShow:
	ELSE
		TBarHide:
	ENDIF
	gSETPENWIDTH 10
ENDP

PROC Pref:
	EXTERNAL gDebugF%
	LOCAL dial%
	dINIT "Preferences"
rem	No longer supported in Crystal
rem	dCHECKBOX gDebugF%, "Use debug mode"
	dCHOICE gDebugF%,"Use debug mode","No,Yes"
	dBUTTONS "Cancel",-(KDButtonEsc% OR KDButtonNoLabel% OR KDButtonPlainKey%), "OK", (KDButtonEnter% OR KDButtonNoLabel% OR KDButtonPlainKey%)
	LOCK ON
	dial%=DIALOG
	LOCK OFF
	IF gDebugF%=2 rem Yes
		gDebugF%=KTrue%
		DebugPref:
	ELSE
		gDebugF%=KFalse%
	ENDIF
ENDP

PROC DebugPref:
	EXTERNAL gPointerFilterF%
	LOCAL dial%
	dINIT "DEBUG Preferences"
rem	No longer supported in Crystal
rem	dCHECKBOX gPointerFilterF%, "Use PointerFilter"
	dCHOICE gPointerFilterF%, "Use PointerFilter","No,Yes"
	dBUTTONS "Cancel",-(KDButtonEsc% OR KDButtonNoLabel% OR KDButtonPlainKey%), "OK", (KDButtonEnter% OR KDButtonNoLabel% OR KDButtonPlainKey%)
	LOCK ON
	dial%=DIALOG
	LOCK OFF
	IF gPointerFilterF%=2
		gPointerFilterF%=KTrue%
		PointerFilterPref:
	ELSE
		gPointerFilterF%=KFalse%
	ENDIF
ENDP

PROC PointerFilterPref:
	EXTERNAL gPFFilter%,pPFMask%,gPFMask%
	LOCAL dial%,filter&,mask&
	filter&=gPFFilter%
	mask&=gPFMask%
	dINIT "Pointer Filter Preferences"
	dLONG filter&, "Filter",0,7
	dLONG mask&, "Mask", 0,7
	dBUTTONS "Cancel",-(KDButtonEsc% OR KDButtonNoLabel% OR KDButtonPlainKey%), "OK", (KDButtonEnter% OR KDButtonNoLabel% OR KDButtonPlainKey%)
	LOCK ON
	dial%=DIALOG
	LOCK OFF
	IF dial%
		gPFFilter%=filter&
		gPFMask%=mask&
	ENDIF
ENDP

PROC Save:
	EXTERNAL gDeltaF%,gFileDeltaF%,gFile$
	LowSave:( gFile$, KTrue% ) REM Inform user.
	REM And reset the delta flag.
	gDeltaF%=KFalse%
	gFileDeltaF%=KFalse%
ENDP

PROC SaveAs:
	EXTERNAL gFile$,gDeltaF%,gFileDeltaF%
	LOCAL f$(255), offset%(6)
	f$ = GETDOC$
	f$ = PARSE$(f$, "" ,offset%())
	f$ = LEFT$(f$, offset%(4)-1) rem Just the drive and path
	f$ = SaveAsFileDialog$:(f$,#0)
	IF LEN(f$)>0
		gFile$ = f$
		SETDOC gFile$
		UpdateTBarTitle:
		LowSave:( gFile$, KTrue% ) REM Inform user.
		gDeltaF%=KFalse%
		gFileDeltaF%=KFalse%
	ENDIF
ENDP

PROC LowSave:( fn$, InformUserF% )
	EXTERNAL gWId%
	REM In fact, I can't think of a single instance
	REM where the user /shouldn't/ be told.

	IF gWId%
		gUSE gWId%
		IF InformUserF%
			BUSY "Saving file " + CHR$(KEllipsis&), KBusyBottomRight%, 15 REM 0.75 seconds
		ENDIF
		gSAVEBIT fn$
		IF InformUserF%
			BUSY OFF
			GIPRINT "File saved", KBusyTopRight%
		ENDIF
	ENDIF
ENDP

PROC LowLoad%:(fn$)
	LOCAL bitmap%, width%, height%
	LOCAL newMain%
	ONERR exit::
	IF EXIST(fn$)
		bitmap% = gLOADBIT(fn$,0,0)
		width% = gWIDTH
		height% = gHEIGHT
		newMain% = gCREATE(0,0, width%, height%, 1, kColorMode%)
		gCOPY bitmap%, 0,0 ,width%,height%,3
		gCLOSE bitmap%
		RETURN newMain%
	ENDIF
exit::
	ONERR OFF
	RETURN 0
ENDP

PROC CmdE%: REM toolbar Exit
	EXTERNAL gExit%
	gExit%=KTrue%
ENDP

PROC CmdA%:
	SetColor:(255,255,255)
ENDP

PROC CmdC%:
	Color:
ENDP

PROC CmdTBDownP%:
	EXTERNAL gScrWid%,TBWidth%
	LOCAL pop%
	REM           x    ,    y       ,   corner
	pop%=mPOPUP(gScrWid%-TbWidth%,97,KMPopupPosTopRight%,"Item a",%a,"Item b",%b)
	IF pop%=0
		REM GIPRINT "Popup cancelled"
	ELSE
		GIPRINT "Item "+chr$(pop%)+" selected"
	ENDIF
ENDP

PROC ToolVis:
	EXTERNAL TbVis%
	REM Toolbar visibility toggle on/off
	IF TbVis%
		TBarHide:
	ELSE
		TBarShow:
	ENDIF
ENDP

PROC DemoA:
	PRINT "!!TODO - demoa"
ENDP

PROC Democ:
	PRINT "!!TODO - democ"
ENDP

PROC DoSomething:
	GIPRINT "DoSomething:" + NYI$
ENDP

PROC SelectAll:
	GIPRINT "SelectAll:" + NYI$
ENDP

PROC About:
	GIPRINT "About:" + NYI$
ENDP

PROC Copy:
	GIPRINT "Copy:" + NYI$
ENDP

PROC Delete:
	GIPRINT "Delete:" + NYI$
ENDP

PROC Export:
	GIPRINT "Export:" + NYI$
ENDP

PROC MergeIn:
	GIPRiNT "MergeIn:" + NYI$
ENDP

PROC Print:
	GIPRINT "Print:" + NYI$
ENDP

PROC PrintSetup:
	GIPRINT "PrintSetup:" + NYI$
ENDP

PROC PageSetup:
	GIPRINT "PageSetup:" + NYI$
ENDP

PROC Paste:
	GIPRINT "Paste:" + NYI$
ENDP

PROC PrintPreview:
	GIPRINT "PrintPreview:" + NYI$
ENDP

PROC Cut:
	GIPRINT "Cut:" + NYI$
ENDP

PROC Redo:
	GIPRINT "Redo:" + NYI$
ENDP

PROC Undo:
	GIPRINT "Undo:" + NYI$
ENDP

PROC CutPaste:
	LOCAL pop%
	pop%=mPOPUP(1,48, KMPopupPosTopLeft%, "Cut",%x, "Copy",%c, "Paste",%v)
	IF pop%=0
		REM GIPRINT "Popup cancelled"
	ELSE
		IF pop%=%x: Cut:
		ELSEIF pop%=%c: Copy:
		ELSE : Paste:
		ENDIF
	ENDIF
ENDP

PROC Irtrans:
	GIPRINT "Irtrans:" + NYI$
ENDP

PROC ZoomIn:
	GIPRINT "ZoomIn:" + NYI$
ENDP

PROC ZoomOut:
	GIPRINT "ZoomOut:" + NYI$
ENDP

PROC Color:
	LOCAL red%,green%,blue%
	red%=rnd*256
	green%=rnd*256
	blue%=rnd*256
	SetColor:(red%,green%,blue%)
ENDP

PROC SetColor:(red%,green%,blue%)
	EXTERNAL gWId%
	gUSE gWId%
	gCOLOR red%,green%,blue%
ENDP