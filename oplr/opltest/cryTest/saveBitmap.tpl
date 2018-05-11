REM·SaveBitmap.tpl -- test saving a bitmap with no document
REM name specified using SETDOC.

PROC Main:
	LOCAL id%
	id%=gCREATE(0,0,300,100,1,1)
	gXBORDER·2,4
	gAT 20,20
	gPRINT "This is a bitmap"
	gSAVEBIT "SomeBitmap"
	gIPRINT "Saved."
	gCLOSE id%
	
	
ENDP
