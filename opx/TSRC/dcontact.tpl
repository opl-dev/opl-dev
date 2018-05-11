REM dContact - Demonstrating the Contacts Manager OPX.
REM v5.1
REM (c) 1999 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "Contact.oxh"

DECLARE EXTERNAL
EXTERNAL Main:
EXTERNAL DoMenu:
EXTERNAL DoCmd:(key%)
EXTERNAL NewFile:
EXTERNAL OpenFile:
EXTERNAL DisplayRecord:
EXTERNAL DoDisplayRecord:

PROC Main:
	GLOBAL MenuState%
	GLOBAL DatabaseName$(255)
	GLOBAL RecordIndex&
	LOCAL key%
	LOCAL finished%
	LOCAL ev&(16)
	FONT KFontTiny4&,0
	WHILE NOT finished%
		DisplayRecord:		
		GETEVENT32 ev&()
		IF ev&(1)=&F700 OR ev&(1)=&F836
			key%=DoMenu:
		ENDIF
		IF (ev&(1) AND &400)=0
			key%=ev&(1) AND &FF
			DoCmd:(key%)
		ENDIF
	ENDWH
ENDP

PROC DoMenu:
	GLOBAL MenuState%
	LOCAL key%
	mINIT
	mCard "File","Create new database…",%n,"Open database…",%o
	key%=MENU(MenuState%)
	IF key%=%n
		NewFile:
	ELSEIF key%=%o
		OpenFile:
	ENDIF	
ENDP

PROC DoCmd:(key%)
	IF key%=%n
		NewFile:
	ELSEIF key%=%o
		OpenFile:
	ENDIF	
ENDP

PROC OpenFile:
	EXTERNAL DatabaseName$
	EXTERNAL RecordIndex&
	LOCAL file$(KDFileNameLen%)
	LOCAL sort&(1)
	dINIT "Open contact database"
	dFILE file$,"File,Folder,Disk",KDFileSelectorWithSystem%
	IF DIALOG
		COCloseContactFile:
		COOpenContactFile:(file$)
		DatabaseName$=file$
		sort&(1)=0
		COSortItems:(ADDR(sort&(1)))
		RecordIndex&=0
		CoReadItem:(RecordIndex&)
	ENDIF
ENDP

PROC NewFile:
	EXTERNAL DatabaseName$
	LOCAL file$(KDFileNameLen%)
	LOCAL sort&(1)
	dINIT "Create new contact database"
	dFILE file$,"File,Folder,Disk",KDFileEditBox%+KDFileEditorDisallowExisting%+KDFileSelectorWithSystem%
	IF DIALOG
		COCloseContactFile:
		COCreateContactFile:(file$)
		DatabaseName$=file$
		sort&(1)=0
		COSortItems:(ADDR(sort&(1)))
	ENDIF
ENDP


PROC DisplayRecord:
	EXTERNAL DatabaseName$
	CLS
	IF DatabaseName$<>""
		DoDisplayRecord:
	ENDIF
ENDP


PROC DoDisplayRecord:
	EXTERNAL DatabaseName$
	EXTERNAL RecordIndex&
	LOCAL countField&
	LOCAL countContent&
	LOCAL content&(20)
	PRINT DatabaseName$,"record:",RecordIndex&;
	IF COItemType&:=KContactCard&
		PRINT "(Card)"
	ELSEIF COItemType&:=KContactCard&
		PRINT "(Own Card)"
	ELSEIF COItemType&:=KContactOwnCard&
		PRINT "(Own Card)"
	ELSEIF COItemType&:=KContactGroup&
		PRINT "(Group)"
	ELSEIF COItemType&:=KContactTemplate&
		PRINT "(Template)"
	ENDIF
	PRINT
	countField&=1
	WHILE countField&<=COItemFieldCount&:
		COItemFieldArrayAt:(countField&)
		COItemFieldContent:(ADDR(content&(1)))
		countContent&=2
		WHILE countContent&<content&(1)
			PRINT HEX$(content&(countContent&));", ";
			countContent&=countContent&+1
		ENDWH
		PRINT HEX$(content&(countContent&))
		countField&=countField&+1
	ENDWH
	PRINT  
ENDP

PROC NextRecord:
	EXTERNAL RecordIndex&
	IF RecordIndex&<COCountItems&:
		RecordIndex&=RecordIndex&+1
		COSortedItemsAt:(RecordIndex&)
	ENDIF	
ENDP

PROC PrevRecord:
	GLOBAL RecordIndex&
	IF RecordIndex&>1
		RecordIndex&=RecordIndex&-1
		COSortedItemsAt:(RecordIndex&)
	ENDIF	
ENDP
