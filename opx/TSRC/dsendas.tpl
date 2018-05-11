REM dSendAs.tpl
REM Copyright (c) 1999-2001 Symbian Ltd. All rights reserved.
REM Demonstration code for the SendAs OPX.

INCLUDE "sendas.oxh"
INCLUDE "prntst.oxh"

PROC Main:
	GLOBAL RichText&
	GLOBAL menuBugFix% rem Skip over first menu card.
	LOCAL k%
	menuBugFix%=257
	RichText&=GetRichText&:
	CreateTestFile:
	WHILE 1
		k%=DisplayMenu%:
		Action:(k%)
	ENDWH
ENDP

PROC CreateTestFile:
	LOCAL h%
	LOCAL test$(30)
	test$ = "This is a test attachment" 
	IOOPEN(h%,"C:\tSendAsOpx.txt",2)
	IOWRITE(h%,ADDR(test$)+2,LEN(test$))
	IOCLOSE(h%)
ENDP

PROC DisplayMenu%:
	external menuBugFix%
	local k%
	mINIT
	mCARD "Dummy card","To skip over",1,"the first pane bug",1
	mCASC "Send",SaCasc$:(1),SaKey%:(1),SaCasc$:(2),SaKey%:(2),SaCasc$:(3),SaKey%:(3),SaCasc$:(4),SaKey%:(4),SaCasc$:(5),SaKey%:(5),SACasc$:(6),SaKey%:(6),SaCasc$:(7),SaKey%:(7)
	mCARD "File","Send>",1
	return MENU(menuBugFix%)
ENDP

PROC Action:(aKey%)
	if aKey%=SAKey%:(KSaEmail&)
		print "EMail"
		SAPrepareMessage:(KSaEmail&)
		SASetBody:(RichText&)
		SAAddFile:("C:\System\Data\Shell.ini")
		SALaunchSend:
	elseif aKey%=SAKey%:(KSaSyncMail&)
		print "SyncMail"
		SAPrepareMessage:(KSaSyncMail&)
		SASetBody:(RichText&)
		SALaunchSend:
	elseif aKey%=SAKey%:(KSaSMS&)
		print "SMS"
		SAPrepareMessage:(KSaSMS&)
		SASetBody:(RichText&)
		SALaunchSend:
	elseif aKey%=SAKey%:(KSaIr&)
		print "IR"
		SAPrepareMessage:(KSaIr&)
		SASetBody:(RichText&)
		SALaunchSend:
	elseif aKey%=SAKey%:(KSaWAP&)
		print "WAP"
		SASetBody:(RichText&)
		SAPrepareMessage:(KSaWAP&)
		SALaunchSend:
	elseif aKey%=SAKey%:(KSaBluetooth&)
		print "Bluetooth"
		SASetBody:(RichText&)
		SAPrepareMessage:(KSaBluetooth&)
		SALaunchSend:
	endif
ENDP