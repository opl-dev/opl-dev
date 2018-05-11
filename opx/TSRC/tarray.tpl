REM tarray.tpl
REM test code for array OPX

INCLUDE "CONST.OPH"
INCLUDE "ARRAY.OXH"
DECLARE EXTERNAL
EXTERNAL PrintArray:(h&,s$)

PROC TestArrayOPX:
	LOCAL array&,i&
	PRINT "TestArrayOPX"
	
	array& = ArrayNew&:
ONERR FreeArray::
	ArrayAdd%:(array&,"First string")
	ArrayAdd%:(array&,"Second string")
	ArrayAdd%:(array&,"Third string")
	ArrayAdd%:(array&,"Fourth string")
	ArrayAdd%:(array&,"Fifth string")
	ArrayAdd%:(array&,"Sixth string")
	ArrayAdd%:(array&,"Seventh string")
	ArrayAdd%:(array&,"Eigth string")
	ArrayAdd%:(array&,"Nineth string")
	ArrayAdd%:(array&,"Tenth string")

	PrintArray:(array&,"Initial array")
	ArrayInsert:(array&,3,"Third and a half string")
	PrintArray:(array&,"Inserted 3 1/2")

	ArraySetCompareType:(array&,KArCmpCollated%)
	ArraySetSortMode:(array&,KArSortAsc%)
	ArraySort:(array&)
	PrintArray:(array&,"Array sorted ascending")

	ArraySetDuplicates:(array&,KArDupAllow%)
	ArrayAdd%:(array&,"K item")
	ArraySetDuplicates:(array&,KArDupIgnore%)
	ArrayAdd%:(array&,"K item")
	PrintArray:(array&,"Added ""K item"" sorted")

  ArrayDelete:(array&,ArrayFind%:(array&,"Second string"))
	PrintArray:(array&,"Deleted ""Second String"" after searching sorted array")

	while ArraySize&:(array&)>3
		ArrayDelete:(array&,0)
	endwh
	PrintArray:(array&,"Deleted all but 3 items")

	ArrayReplace:(array&,1,"This is a replaced string")
	PrintArray:(array&,"Replaced item 1")

	ArraySetSortMode:(array&,KArSortDes%)
	ArraySort:(array&)
	PrintArray:(array&,"Array sorted descending")

	ArrayClear:(array&)
	ArrayAdd%:(array&,"First string")
	ArrayAdd%:(array&,"Second string")
	ArrayAdd%:(array&,"Third string")
	ArrayAdd%:(array&,"Fourth string")
	ArrayAdd%:(array&,"Fifth string")
	ArrayAdd%:(array&,"Sixth string")
	ArrayAdd%:(array&,"Seventh string")
	ArrayAdd%:(array&,"Eigth string")
	ArrayAdd%:(array&,"Nineth string")
	ArrayAdd%:(array&,"Tenth string")

	PrintArray:(array&,"Initial array again (sorted descending)")
	
	ArrayClear:(array&)
	ArraySetSorted:(array&,KFalse%)
	
	REM This will most likely cause a memory error
	CLS
	PRINT "Adding 50000 elements to the array...";
	i&=1
	WHILE i&<=50000
		ArrayAdd%:(array&,"This is item number "+NUM$(i&,10))
		i&=i&+1
	ENDWH
	PRINT "Done."
	PRINT "Sorting the array...";
	PRINT "(Takes about two minutes on the emulator)"
	ArraySetSortMode:(array&,KArSortAsc%)
	ArraySort:(array&)
	PRINT "Size is",ArraySize&:(array&)
	PRINT "Done. Press any key to end"
	GET
FreeArray::
	IF ERR
		ALERT(ERRX$,ERR$(ERR))
	ENDIF
	ArrayFree:(array&)
ENDP

PROC PrintArray:(array&,Title$)
LOCAL i&
	CLS
	PRINT Title$
	PRINT
	i&=0
	WHILE i&<ArraySize&:(array&)
		PRINT ArrayAt$:(array&,i&)
		i&=i&+1
	ENDWH
	PRINT
	PRINT "Press a key to continue..."
	GET
ENDP
