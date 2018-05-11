rem arraytst.opl
REM test code for the dynamic array opx.

INCLUDE "ArrayOpx.oxh"
INCLUDE "Cdescriptor.oxh"

DECLARE EXTERNAL 
EXTERNAL Main:
EXTERNAL Rand%:(i%)

CONST KMaxElem&= 100

PROC Main:

	LOCAL numberHandle&	rem handle of Array object.
	LOCAL error%
	LOCAL addr&,numDes&,s&,i&,err&
	LOCAL str$(255)
	addr&=ALLOC(255) REM person details...
	
	numberHandle&=ARInit&:(KMaxElem&,0)
	IF numberHandle&=0
		PRINT "No memory for array." : GET :	STOP
	ENDIF
	
	PRINT "Hash Table Array Created"
	numDes&=NewDes&:(addr&,0,32)
	s&=1
	PRINT "Hashing..."
	WHILE s& < 101
		i&=rand%:(KMaxElem&)
		DesNum:(numDes&,i&)
		error%=ARSetElement%:(numberHandle&,i&,numDes&)		
		s&=s&+1
	ENDWH

	REM DISPLAY ARRAY	
	i&=1
	WHILE i& < KMaxElem&+1
		str$=" "
		DesCopyStr:(numDes&,str$)
	  error%=ARElement%:(numberHandle&,i&,numDes&)
	  str$=DesStr$:(numDes&)
		print (i&);" = ";(str$)
		i&=i&+1
	ENDWH

	PRINT "Expand table.."
	err&=ARSetCount&:(numberHandle&,KMaxElem&+250)
	IF err& <> 0
	  PRINT "ok."
	ELSE
		PRINT "failed."
	ENDIF
	
	PRINT "Contract table.."
	err&=ARSetCount&:(numberHandle&,KMaxElem&-50)
	IF err& <> 0
	  PRINT "ok."
	ELSE
		PRINT "failed."
	ENDIF
	
	str$="55"
	PRINT "Set Element.. ";(str$);" to position 15"
	DesCopyStr:(numDes&,str$)
	error%=ARSetElement%:(numberHandle&,15,numDes&)
	IF error% <> 0
	  PRINT "ok."
	ELSE
		PRINT "failed."
	ENDIF
	
	
	PRINT "Get element at location 15"
	error%=ARElement%:(numberHandle&,15,numDes&)
	IF error% <> 0
	  str$=DesStr$:(numDes&)
	  PRINT "Location 15 = ";(str$)
	  PRINT "ok."
	ELSE
		PRINT "failed."
	ENDIF
	
	PRINT "Get Largest element.."
	err&=ARSizeMaxElement&:(numberHandle&)
	IF error% <> 0
	  PRINT "Largest element is ";(err&);" bytes long."
	  PRINT "ok."
	ELSE
		PRINT "failed."
	ENDIF
	
	PRINT "Count number of elements in the array"
	err&=ARCount&:(numberHandle&)
	IF error% <> 0
	  PRINT "There are ";(err&);" elements in the table."
	  PRINT "ok."
	ELSE
		PRINT "failed."
	ENDIF
		
	PRINT "Sort hash table.."
	error%=ARSort%:(numberHandle&)
	PRINT "OK"
	
	PRINT "Searching Table for num 55"
	DesCopyStr:(numDes&,"55")
	err&=ARSearch&:(numberHandle&,numDes&)
	IF err&=0
		PRINT "Entry Not found."
	ELSE
			str$=DesStr$:(numDes&)	
			PRINT "Entry found at {";(err&);"} .. ";(str$)
			PRINT "ok"
	endif
	
	PRINT "Deleting element {";(err&);"}"
	ARDeleteElement&:(numberHandle&,err&)
	PRINT "ok"
	
	
	DeleteDes:(numDes&)
	PRINT "DESCRIPTOR DELETED"
 	ARDelete&:(numberHandle&)
 	PRINT "HASH TABLE DELETED":get
	FREEALLOC(addr&)
	
ENDP

PROC Rand%:(m%)
	LOCAL v%
	v%=1+rnd*m%
	RETURN v%
ENDP
