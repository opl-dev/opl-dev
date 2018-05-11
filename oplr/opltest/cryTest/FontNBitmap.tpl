PROC Main:
	Text:(10,50,"Hello world")
	gFONT·10
	
	Bitmap:(200,30,"Hello world")
	gFONT·10

	Window:(400,30,"Hello world")
	gFONT·10

	dinit "Done" :DIALOG
ENDP

PROC Bitmap:(x%,y%,text$)
	LOCAL id%
	id%=gCREATEBIT(50,50,1)
	gAT 10,30
	gPRINT text$
	return
		
	gUSE·1	
	gAT x%,y%
	gCOPY id%,0,0, 50,50,3
	gCLOSE id%
ENDP


PROC Window:(x%,y%,text$)
	LOCAL id%
	id%=gCREATE(x%,y%, 50,50, 1,1)
rem	gXBORDER 2,1
	gAT 10,30
	gPRINT "W"+text$
	return
	
	gUSE·1	
	gAT x%,y%
	gCOPY id%,0,0, 50,50,3
	gCLOSE id%
ENDP

PROC Text:(x%,y%,text$)
	gAT x%,y%
	gPRINT text$
ENDP
