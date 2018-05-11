REM Crystal Gobble2


DECLARE EXTERNAL

INCLUDE "Const.oph"
INCLUDE "System.oxh"

EXTERNAL AllocDialog%:
EXTERNAL ChangeCell%:
EXTERNAL AvailableMem&:

external assert:(a&,b&)

app Gobble,91212456
enda

PROC Main:
	GLOBAL pCell&	rem Ptr to alloced cell
	GLOBAL free&	rem Bytes to keep free.
	GLOBAL exit%

	pCell&=0
	DO
		IF AllocDialog%:
			ChangeCell%:
		ENDIF
	UNTIL exit%
ENDP


PROC AllocDialog%:
	REM Show user current memory sizes and
	REM get choice for new free amount.
	REM Return True if cell size is to change.
	EXTERNAL pCell&
	EXTERNAL free&
	EXTERNAL exit%
	LOCAL allocatedSize&
	LOCAL available&
	LOCAL dialog%,ret%

	ret%=KTrue%
	IF pCell&=0
		allocatedSize&=0
	ELSE
		allocatedSize&=LENALLOC(pCell&)
	ENDIF

	available&=AvailableMem&:
	dINIT "Crystal gobble"
	dTEXT "Total memory:",NUM$(available&+allocatedSize&,10)
	dTEXT "Bytes available:",NUM$(available&,10)
	dTEXT "Bytes in gobble cell:",NUM$(allocatedSize&,10)
	dLONG free&,"Bytes to leave free (0 for none):",0,available&+allocatedSize&
	dTEXT "Esc to exit (and free cell)",""
	dBUTTONS "OK",13,"Refresh",%r,"Cancel",-27
	dialog%=DIALOG
	IF dialog%=0
		exit%=KTrue%
		ret%=KFalse%
	ENDIF

	IF dialog%=%r REM Refresh
		ret%=KFalse%
	ENDIF

	RETURN ret%
ENDP


PROC ChangeCell%:
	REM Change the alloced cell size so that only free& 
	REM bytes remain available.
	EXTERNAL pCell&
	EXTERNAL free&
	LOCAL totalAvailable&
	LOCAL newCellSize&,pNewCell&
	LOCAL currentCellSize&

	IF pCell&
		currentCellSize&=LENALLOC(pCell&)
	ENDIF
	
	totalAvailable&=AvailableMem&:+currentCellSize&
	newCellSize&=totalAvailable&-free&
	IF newCellSize&<0 :newCellSize&=0 :ENDIF
	IF pCell&
		pNewCell&=REALLOC(pCell&,newCellSize&)
		IF pNewCell&=0
				ALERT("Failed to alloc","press Enter to retry"): STOP
		ENDIF
		rem assert:(lenalloc(pcell&),newCellSize&)
	ELSE
		pCell&=ALLOC(newCellSize&)
	ENDIF
ENDP


PROC AvailableMem&:
	LOCAL cell&
	LOCAL low&,high&,mid&
	BUSY "Finding available memory"
	low&=0
	high&=67000000
	DO
rem		print "Low=";low&, " high=";high&, "diff=",high&-low&
		mid&=(low&+high&)/2

		cell&=ALLOC(mid&)
		IF cell& rem mid&<available&
rem			print "Got one at", mid&
			low&=mid&+1
		ELSE
rem			print "Missed one at", mid&
			high&=mid&-1
		ENDIF
		FREEALLOC(cell&)
	UNTIL low&>=high&
	BUSY OFF
	RETURN low&
ENDP




proc assert:(a&,b&)
	if a&-b&>4
		print "assert failed", a&-b&
		get
	endif
endp

REM Ends
