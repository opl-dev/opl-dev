REM tCelltrack.tpl
REM test code for the celltrack opx.
REM v1.50 Feb 2004.

INCLUDE "celltrack.oxh"

DECLARE EXTERNAL

EXTERNAL tProfile:
EXTERNAL tCbs:
EXTERNAL tPhoneInfo:

PROC tCellTrack:
	tProfile:
	tCbs:
	tPhoneInfo:
	PRINT "Done. Press any key."
	GET
ENDP


PROC tProfile:
	LOCAL indexedName$(32)
	LOCAL newProfileName$(32), oldProfileName$(32)
	LOCAL index%, count%, newIndex%
	LOCAL err%

	PRINT "tProfile"
	index%=ActiveProfile%:
	indexedName$=ProfileName$:(index%) 
	count%=ProfileCount%:
	PRINT "Profile index=";index%;" out of ";count% 
	PRINT "indexedName=[";indexedName$;"]"
	PRINT "Len indexedName=";LEN(indexedName$)

	PRINT
	oldProfileName$=ActiveProfileName$:

	REM Pick new profile name.
	newIndex%=index%+1
	IF newIndex%>count% :newIndex%=1 :ENDIF
	newProfileName$=ProfileName$:(newIndex%)
	PRINT "Switching profile to [";newProfileName$;"]"
	err%=SwitchProfile%:(newProfileName$)
	IF err%
		PRINT "Error!"
		PRINT "Error",err%," Unable to switch profile to [";newProfileName$;"]"
	ENDIF
	PRINT "Switching profile back to", oldProfileName$ 
	err%=SwitchProfile%:(oldProfileName$)
	IF err%
		PRINT "Error!"
		PRINT "Error",err%," Unable to switch profile to [";oldProfileName$;"]"
	ENDIF

	PRINT "ExtraProfileName$(1) is [";ExtraProfileName$:(1);"]"
ENDP


PROC tCbs:
	LOCAL result%
	PRINT "tCbs"
	result%=CBSOnOffStat%:(KCBSstat%)
	PRINT "CBS status is",result%

	REM CBSMessageRead$:(aViewlist$,aCellid%,aIndex%)
	REM CBSMessageInfo&:(aViewlist$,aChoice%,aIndex%) 
	REM 	CBSListsOpenCloseStat%:(aChoice%) 
	REM CBSListsInit%:(aViewlist$)
ENDP


PROC tPhoneInfo:
	PRINT "tPhoneInfo"
	PRINT "Cell id=";CellInfo&:(KCellId%) 
	PRINT "Netstat=";CellInfo&:(KNetStat%) 
	PRINT "Serial=";Serial$:(KSerial%)
	PRINT "Provider=";Serial$:(KProvider%)
	REM Info&:(aInfoId&) :202 : rem phoneonoff=1,netavailable/not=2,profile=3
	REM InfoUid&:(aInfoUId&) :203 : rem &10005000-&10006000
ENDP


REM Ends.
