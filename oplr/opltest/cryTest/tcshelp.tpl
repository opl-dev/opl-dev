REM Testing context-sensitive help...
INCLUDE "Const.oph"

CONST KMyAppMainView$="MyOplApp_Main_view"
CONST KMyAppDialog1$="MyOplApp_Dialog1"

PROC tCSHelp:
	SETHELP KHelpView%,KMyAppMainView$
	rem KHelpDialog%, KHelpMenu%
	SETHELP KHelpDialog%,KMyAppDialog1$
	dINIT "Press Help now!"
	dBUTTONS "Close",27
	DIALOG
	SHOWHELP
	print "Done" :GET
ENDP
