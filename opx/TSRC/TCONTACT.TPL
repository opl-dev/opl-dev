REM tContact - testing the Contacts Manager OPX.
REM v6.1
REM (c) 1999 Symbian Ltd. All rights reserved.


REM * * * * * *  WARNING  * * * * * *  
REM
REM This code will add items to your existing contacts db.
REM Ensure you have a backup of your contacts before running it.
REM Or, search through the db for names starting with "tCRQ ".
REM
REM * * * * * * * * * * * * * * * * * 

INCLUDE "Const.oph"
INCLUDE "Contact.oxh" 

DECLARE EXTERNAL
EXTERNAL Main:
EXTERNAL tGeneral:
EXTERNAL Test3:(aCount&,aUsingDefaultDb%)
EXTERNAL GenFirstName$:
EXTERNAL GenSurName$:
EXTERNAL GenAddress$:
EXTERNAL GenPhone$:
EXTERNAL rand%:(i%)
EXTERNAL ExerciseAll:
EXTERNAL AddToDefaultContactsDb:

CONST KTestContactFile$="\opl\tcontacts.cdb"
CONST KDefaultContactFile$="c:\system\data\contacts.cdb"
CONST KAddTestEntry&=25
CONST KAddDefaultEntry&=5
CONST kMaxTitleM%=8
CONST kMaxTitleF%=7
CONST kMAXFIRSTM%=11
CONST kMAXFIRSTF%=10
CONST kMAXSECOND%=15

PROC tContact:
	tGeneral:
ENDP

PROC tGroups:
	LOCAL item&

	WHILE item&<=10
		CoCreateItem:(KContactCard&,CoTemplateGoldenId&:)
rem To Do: finish this off
	ENDWH
ENDP


PROC tGeneral:
	LOCAL OkayToAddToDefault%
	OkayToAddToDefault%=KFalse%

	PRINT "WARNING!"
	PRINT
	PRINT "Running this code will add test contacts to your contacts database."
	PRINT "Their names will start with tCRQ."
	PRINT "Press c key to confirm..."
	IF GET=8
		OkayToAddToDefault%=KTrue%
		PRINT "Confirmed."
	ELSE
		PRINT "Will NOT add to contacts"
	ENDIF
	PAUSE 20
	ExerciseAll:
	IF OkayToAddToDefault%
		AddToDefaultContactsDb:
	ENDIF
ENDP



PROC ExerciseAll:
	LOCAL temp%,count&,string$(255),ID&,arrayHandle&
	LOCAL arrayBody&(10)

	IF EXIST(KTestContactFile$)
		TRAP DELETE KTestContactFile$
		PRINT "Deleted",KTestContactFile$
	ENDIF

	COCreateContactFile:(KTestContactFile$)
	PRINT "Contact file created"
	
 	COCloseContactFile:
 	PRINT "Contact file Closed"

 	COOpenContactFile:(KTestContactFile$)
	PRINT "Contact file Opened":
  
	COOpenItem:(COTemplateGoldenId&:)
	PRINT "Template Opened"
  
	count& = CoItemFieldCount&:
	PRINT "Field count =",count&

	COCloseItem:
	PRINT "Template committed"
	
	COCreateItem:(KContactCard&,COTemplateGoldenId&:)
	PRINT "Contact Created"

	CoItemFieldCreate:
	PRINT "Field created",temp%
	
	string$="label"
	CoItemFieldSetLabel:(string$)
	PRINT "Field labelled",string$
  
	CoItemFieldSetMapping:(KUidCntFieldNote&)
	PRINT "Field mapping Set",temp% 
	
	CoItemFieldAddType:(KIntCntFieldVCardMapNOTE&)
	PRINT "Field type added",temp% 
	
	CoItemFieldInsert:(1)
	PRINT "Field inserted at position 1"
	
	CoItemFieldCreate:
	PRINT "Field created",temp%
	
	string$="label 2"
	CoItemFieldSetLabel:(string$)
	PRINT "Field labelled",string$
  
	CoItemFieldSetMapping:(KUidCntFieldNote&)
	PRINT "Field mapping Set",temp% 
	
	CoItemFieldAddType:(KIntCntFieldVCardMapNOTE&)
	PRINT "Field type added",temp% 
	
	CoItemFieldAdd:
	PRINT "Field Added"

	CoItemFieldArrayAt:(1)	
	PRINT "Field set to first in list"

	arrayHandle&=ADDR(arrayBody&(1))
	CoItemFieldcontent:(arrayhandle&)
	PRINT "Content of field found"
	
	count& = arrayBody&(1)
	WHILE count& > 1
		PRINT "array",count&,"=",arrayBody&(count&)
		count&=count&-1
	ENDWH
	
	CoItemFieldMove:(1,5)
	PRINT "Field 1 moved to position 5"
	
	CODeleteItem:
	PRINT "Contact Deleted"

	COCloseItem:
	PRINT "Contact closed"
	
	REM !!!!!! fill up contacts file
	Test3:(KAddTestEntry&,KFalse%) REM Not using default db here.
	temp% = COCompressNeeded%:
	PRINT "Compress needed?",temp% 
	IF temp%=1
		COCompressContactFile:
		PRINT "Compressed"
	ENDIF

	count& = COCountItems&:
	PRINT "Contacts count =",count&
	
	COOpenItem:(5)
	PRINT "contact 5 opened"

	CoItemFieldArrayAt:(4)
	PRINT "Contact Field pos 4 indexed"

	CoItemFieldDelete:
	PRINT "Contact field 4 deleted"

rem	arrayBody&(1)=5
rem	arrayBody&(2)=KUidCntFieldPhoneNumber&
rem	arrayBody&(3)=KIntCntFieldVCardMapTEL&
rem	arrayBody&(4)=KIntCntFieldVCardTypeHOME&
rem	arrayBody&(5)=KIntCntFieldVCardTypeCELL&
rem	arrayHandle&=ADDR(arrayBody&(1))
rem	temp% = CoItemFieldFind%:(arrayHandle&)
rem	CoItemFieldArrayAt:(temp%)	
rem	PRINT "found at",temp%

	COSetContactHidden:(1)
	PRINT "Contact set to hidden"
	
	CoItemFieldReset:
	PRINT "Contact Fields reset"
	
	count& = CoItemFieldCount&:
	PRINT "Contact field count =",count&
	
	COCloseItem:
	PRINT "Contact Committed" 
	
	COOpenItem:(4)
	PRINT "Contact Opened" 
		
	arrayBody&(1)=1
	arrayBody&(2)=KUidCntFieldGivenName&
	arrayHandle&=ADDR(arrayBody&(1))
	COSortItems:(arrayHandle&)
	PRINT "Contacts file sorted" 

 	COSortedItemsAt:(1)
	PRINT "Sorted file index at first element" 
	
	string$="Alice"
rem	COFindItem&:(string$,arrayHandle&)
	PRINT "Contact '";string$;"' found" 
	
	CODeleteItem:
	PRINT "Contact Deleted"
	
	COCloseContactFile:
	PRINT "Contacts file Closed. FINISHED"
	GET 
ENDP


PROC AddToDefaultContactsDb:
	PRINT "Adding to default contacts db."

   	COOpenContactFile:(KDefaultContactFile$)
	PRINT "Contact file opened":

	Test3:(KAddDefaultEntry&,KTrue%) REM This is the default database.

	COCloseContactFile:
	PRINT "Contacts file Closed. FINISHED"
	GET
ENDP



PROC Test3:(aCount&,aUsingDefaultDb%)
	LOCAL counter&,tmp%
	LOCAL entryh&,temp2&
	LOCAL surName$(255),firstName$(255),address$(255),phone$(255)
	LOCAL mobile$(255)
	LOCAL birthd&,days&,arrayBody&(5),arrayHandle&,temp%
	counter&=0
	days&=1341
	PRINT "Generating", aCount&,"entries. Please wait..." 
	DO
		COCreateItem:(KContactCard&,COTemplateGoldenID&:)
		firstName$=GenFirstName$: 		
		surName$=GenSurName$:
		address$=GenAddress$:
		phone$=GenPhone$:
		mobile$=GenPhone$:

		REM tCRQ is a uniq identifier to quickly find
		REM contacts for deleting, when adding to default db.
		IF aUsingDefaultDb%
			firstName$="tCRQ "+firstName$
		ENDIF
		
		REM !! first name !!
		arrayBody&(1)=3
		arrayBody&(2)=KUidCntFieldGivenName&
		arrayBody&(3)=KIntCntFieldVCardMapUnusedN&
		arrayHandle&=ADDR(arrayBody&(1))
		temp% = CoItemFieldFind%:(arrayHandle&)
		CoItemFieldArrayAt:(temp%)	
		CoItemFieldSetText:(firstName$)
		
		REM !! surname !!
		arrayBody&(1)=3
		arrayBody&(2)=KUidCntFieldFamilyName&
		arrayBody&(3)=KIntCntFieldVCardMapUnusedN&
		arrayHandle&=ADDR(arrayBody&(1))
		temp% = CoItemFieldFind%:(arrayHandle&)
		CoItemFieldArrayAt:(temp%)	
		CoItemFieldSetText:(surName$)

		arrayBody&(4)=0
		arrayBody&(5)=0
	
REM		REM !! Mobile !!
REM		arrayBody&(1)=5
REM		arrayBody&(2)=KUidCntFieldPhoneNumber&
REM		arrayBody&(3)=KIntCntFieldVCardMapTEL&
REM		arrayBody&(4)=KIntCntFieldVCardTypeHOME&
REM		arrayBody&(5)=KIntCntFieldVCardTypeCELL&
REM		arrayHandle&=ADDR(arrayBody&(1))
REM		temp% = CoItemFieldFind%:(arrayHandle&)
REM		CoItemFieldArrayAt:(temp%)	
REM		CoItemFieldSetText:(mobile$)
		
		arrayBody&(5)=0		

rem		REM !! Home phone !!
rem		arrayBody&(1)=4
rem		arrayBody&(2)=KUidCntFieldPhoneNumber&
rem		arrayBody&(3)=KIntCntFieldVCardMapTEL&
rem		arrayBody&(4)=KIntCntFieldVCardTypeHome&
rem		arrayHandle&=ADDR(arrayBody&(1))
rem		temp% = CoItemFieldFind%:(arrayHandle&)
rem		CoItemFieldArrayAt:(temp%)	
rem		CoItemFieldSetText:(phone$)


		REM !! Home Street / Address !!
		arrayBody&(1)=4
		arrayBody&(2)=KUidCntFieldAddress&
		arrayBody&(3)=KIntCntFieldVCardMapADR&
		arrayBody&(4)=KIntCntFieldVCardTypeHome&
		arrayHandle&=ADDR(arrayBody&(1))
		temp% = CoItemFieldFind%:(arrayHandle&)
		CoItemFieldArrayAt:(temp%)	
		CoItemFieldSetText:(address$)

rem		REM !! Work Phone !!
rem		arrayBody&(1)=4
rem		arrayBody&(2)=KUidCntFieldPhoneNumber&
rem		arrayBody&(3)=KIntCntFieldVCardMapTEL&
rem		arrayBody&(4)=KIntCntFieldVCardTypeWork&
rem		arrayHandle&=ADDR(arrayBody&(1))
 rem		temp% = CoItemFieldFind%:(arrayHandle&)
	rem	CoItemFieldArrayAt:(temp%)	
rem		CoItemFieldSetText:(phone$)
		
		REM !! Work Fax !!
		arrayBody&(1)=5
		arrayBody&(2)=KUidCntFieldFax&
		arrayBody&(3)=KIntCntFieldVCardMapTEL&
		arrayBody&(4)=KIntCntFieldVCardTypeWork&
		arrayBody&(5)=KIntCntFieldVCardTypeFAX&
		arrayHandle&=ADDR(arrayBody&(1))
		temp% = CoItemFieldFind%:(arrayHandle&)
		CoItemFieldArrayAt:(temp%)	
		CoItemFieldSetText:(phone$)

		arrayBody&(4)=0
		arrayBody&(5)=0		

		REM !! Notes !!
		arrayBody&(1)=3
		arrayBody&(2)=KUidCntFieldNote&
		arrayBody&(3)=KIntCntFieldVCardMapNOTE&
		arrayHandle&=ADDR(arrayBody&(1))
		temp% = CoItemFieldFind%:(arrayHandle&)
		CoItemFieldArrayAt:(temp%)	
		CoItemFieldSetText:("This is the notes section.")
		
		COAddNewItem:

		counter&=counter&+1
	UNTIL counter&=aCount&
	PRINT "Generated",aCount&,"entries."
ENDP


PROC GenFirstName$:
	REM The dummy subject name generator.
	LOCAL firstm$(kMAXFIRSTM%,20), firstf$(kMAXFIRSTF%,20)
	LOCAL sex%, pt$(20), pf$(20), ps$(30)

	firstm$(1) = "Paul"
	firstm$(2) = "Phil"
	firstm$(3) = "Mark"
	firstm$(4) = "John"
	firstm$(5) = "Arthur"
	firstm$(6) = "Andy"
	firstm$(7) = "Mike"
	firstm$(8) = "Terry"
	firstm$(9) = "Jeremy"
	firstm$(10) = "David"
	firstm$(11) = "Leslie"

	firstf$(1) = "Carole"
	firstf$(2) = "Rachael"
	firstf$(3) = "Rose"
	firstf$(4) = "Pamela"
	firstf$(5) = "Anna"
	firstf$(6) = "Alice"
	firstf$(7) = "Samantha"
	firstf$(8) = "Victoria"
	firstf$(9) = "Elizabeth"
	firstf$(10) = "Sue"

	sex%=rand%:(2)

	IF sex% = 1 
		pf$ = firstm$(rand%:(kMAXFIRSTM%))
	ELSE 
		pf$ = firstf$(rand%:(kMAXFIRSTF%))
	ENDIF

	RETURN pf$
ENDP

PROC GenSurName$:
	REM The dummy subject name generator.
	LOCAL second$(kMAXSECOND%,30),ps$(255)
	second$(1) = "Bloggs"
	second$(2) = "Smith"
	second$(3) = "Brown"
	second$(4) = "Bell"
	second$(5) = "Green"
	second$(6) = "Reynolds"
	second$(7) = "Cooper"
	second$(8) = "Palmer"
	second$(9) = "Davies"
	second$(10) = "Smedly"
	second$(11) = "Hugelong-doublebarrelledname"
	second$(12) = "Parkinson"
	second$(13) = "Smyth"
	second$(14) = "Watkins"
	second$(15) = "Parker"
	ps$ = second$(rand%:(kMAXSECOND%))
	RETURN ps$
ENDP

PROC Rand%:(m%)
	LOCAL v%
	v%=1+rnd*m%
	RETURN v%
ENDP


PROC GenAddress$:
	LOCAL number$(3)
	LOCAL street$(60)
	LOCAL towncode$(30)
	
	number$=NUM$(rand%:(999),3)

	VECTOR rand%:(10)
	s1,s2,s3,s4,s5,s6,s7,s8,s9,s10
	ENDV
	s1::
		street$="High Street,"
		goto end1
	s2:: 
		street$="Old Street,"
		goto end1
	s3:: 
		street$="Castle Hill,"
		goto end1
	s4:: 
		street$="Long Lane,"
		goto end1
	s5:: 
		street$="Town Place,"
		goto end1
	s6:: 
		street$="Elder Road,"
		goto end1
	s7:: 
		street$="Albert Street,"
		goto end1
	s8:: 
		street$="Spring Gardens,"
		goto end1
	s9:: 
		street$="Good Street,"
		goto end1
	s10:: 
		street$="Park Parade,"
		goto end1

	end1::
	VECTOR rand%:(10)
		t1,t2,t3,t4,t5,t6,t7,t8,t9,t10
	ENDV
	t1:: 
		towncode$="London"
		goto end2
	t2:: 
		towncode$="Reading"
		goto end2
	t3:: 
		towncode$="Leeds"
		goto end2
	t4:: 
		towncode$="Edinburgh"
		goto end2
	t5:: 
		towncode$="Cardiff"
		goto end2
	t6:: 
		towncode$="Worcester"
		goto end2
	t7:: 
		towncode$="Belfast"
		goto end2
	t8:: 
		towncode$="Otley"
		goto end2
	t9:: 
		towncode$="Richmond"
		goto end2
	t10:: 
		towncode$="New Town"
		goto end2
	
	end2::
	RETURN number$+" "+street$+towncode$
ENDP


PROC GenPhone$:
	LOCAL phone$(30)
	LOCAL std%
	std%=rand%:(7)
	VECTOR std%
		l1,l2,l3,l4,l5,l6
	ENDV
	l1::
		phone$="0123"
		goto end3
	l2:: 
		phone$="0987"
		goto end3
	l3:: 
		phone$="0111"
		goto end3
	l4:: 
		phone$="01222"
		goto end3
	l5:: 
		phone$="083456"
		goto end3
	l6:: 
		phone$="01888"
		goto end3
	
	end3::
	phone$=phone$+" "+NUM$(1000000+rnd*8999999,7)
	RETURN phone$
ENDP


REM End.