// README.TXT
//
// Copyright (c) 1997-2002 Symbian Ltd.  All rights reserved.
//

DATA.OPX
=========

Contents:

1.  Introduction
2.  File Details
3.  Installation
4.  Using the OPX
5.  Contents of Data.oxh
6.  Procedure Reference
7.  Copyright
8.  Distributing the OPX


INTRODUCTION
------------

This OPX gives access to all the field types supported by DBMS,
including long types (text of arbitrary length).

It also allows complete schema enquiries for OPL databases.

The DbFindWhere: function allows you to perform database searching
using the full power of SQL.  You can search for any SQL expression
normally used in the WHERE part of a SELECT clause.

The OPX also allows you to open and create databases with user-defined
field types. In other words, the field type does not have to be
hard coded into the OPL program.

Finally, it allows for fast seeking to a row in a table using an
index.

Three caveats: 

* With the Get and Put functions in this OPX you can use any of the
  types supported by DBMS.  You lose, however, the security
  provided by the OPL interpreter.  The OPX does no type checking.  If
  you use the wrong Get function, your program will crash.

* The schema enquiry functions cannot access Data databases if they
  are not already open in your OPL program. This shouldn't be a
  problem, because (1) you can always use the Dbase.opx and (2) you
  can simply open the database using OPEN and then call the functions.

* If you use the possibility of specifying that a field cannot be
  empty, the DBMS may rearrange the order of fields in your
  table. This means that a view opened with "SELECT *" will not return
  the fields in the order that you may expect. If you need to rely on
  the fields being in the order in which you specified them, you
  should not use the "Cannot be empty" flag.

FILE DETAILS
------------

The archive consists of the following files:

README.TXT      This file
DATA.SIS        This is the main OPX file in SIS format
DATAW.SIS       This is the WINS version of the OPX in SIS format
DATA.TXH        This is the header file in text format
DDATA.TPL       This is a demonstration program that shows you how the OPX can be used

Data OPX was first released by Symbian for ER5 as Data OPX version 500. This
is version 600 for ER6 devices.


INSTALLATION
------------

1.  Install DATA.SIS.

2.  Create DATA.OXH into the \System\Opl\ folder on either the C: or D: 
drive.  This may be done by either starting a new file called DATA.OXH and 
then using Import text... DATA.TXH, or by making use of OPLTRAN -CONV (see 
the OPL SDK for details).

3.  Create DDATA.OPL from DDATA.TPL anywhere you like.


USING THE OPX
-------------

1.  First compile and run the DDATA.OPL file to make sure everything works 
correctly.

2.  To use the OPX in your program add the following line to the top of the 
code, immediately after the APP...ENDA and before the first procedure

    INCLUDE "DATA.OXH"

3.  You can now use the Data.opx procedures in your program.


CONTENTS OF DATA.OXH
--------------------

CONST KOpxDataUid&=&10004EC7
CONST KOpxDataVersion%=$600

CONST KODbLessThan&=0
CONST KODbLessEqual&=1
CONST KODbEqualTo&=2
CONST KODbGreaterEqual&=3
CONST KODbGreaterThan&=4

CONST KODbColBit&=0
CONST KODbColInt8&=1
CONST KODbColUint8&=2
CONST KODbColInt16&=3
CONST KODbColUint16&=4
CONST KODbColInt32&=5
CONST KODbColUint32&=6
CONST KODbColInt64&=7
CONST KODbColReal32&=8
CONST KODbColReal64&=9
CONST KODbColDateTime&=10
CONST KODbColText8&=11
CONST KODbColText16&=12
CONST KODbColBinary&=13
CONST KODbColLongText8&=14
CONST KODbColLongText16&=15
CONST KODbColLongBinary&=16
CONST KODbColText&=KODbColText16&
CONST KODbColLongText&=KODbColLongText16&

DECLARE OPX DATA,KOpxDataUid&,KOpxDataVersion%
	ODbGetTableCount&:(aPath$) : 1
	ODbGetTableName$:(aPath$, aTableNumber&) : 2
	ODbGetIndexCount&:(aPath$, aTable$) : 3
	ODbGetIndexName$:(aPath$, aTable$, anIndexNumber&) : 4
	ODbGetIndexDescription$:(aPath$, aTable$, anIndexName$) : 5
	ODbGetFieldCount&:(aPath$, aTable$) : 6
	ODbGetFieldName$:(aPath$, aTable$, aFieldNumber&) : 7
	ODbGetFieldType&:(aPath$, aTable$, aFieldNumber&) : 8
	ODbGetFieldSize&:(aPath$, aTable$, aFieldNumber&) : 9
	ODbGetCanBeEmpty%:(aPath$, aTable$, aFieldNumber&) : 10
	ODbOpenR:(anId&, anSqlQuery$, aFieldTypes$) : 11
	ODbOpen:(anId&, anSqlQuery$, aFieldTypes$) : 12
	ODbStartTable: : 13
	ODbTableField:(aFieldName$, aFieldType&, aMaximumLength&) : 14
	ODbCreateTable:(aFileName$, aTable$) : 15
	ODbGetLength&:(aFieldNumber&) : 16
	ODbGetString$:(aFieldNumber&) : 17
	ODbGetInt&:(aFieldNumber&) : 18
	ODbGetReal:(aFieldNumber&) : 19
	ODbGetReal32:(aFieldNumber&) : 20
	ODbGetWord&:(aFieldNumber&) : 21
	ODbGetDateTime:(aDateTime&, aFieldNumber&) : 22
	ODbGetLong:(aBuffer&, aLength&, aFieldNumber&) : 23
	ODbPutEmpty:(aFieldNumber&) : 24
	ODbPutString:(aText$, aFieldNumber&) : 25
	ODbPutInt:(anInteger&, aFieldNumber&) : 26
	ODbPutReal:(aFloat, aFieldNumber&) : 27
	ODbPutReal32:(aFloat, aFieldNumber&) : 28
	ODbPutWord:(anUnsignedInteger&, aFieldNumber&) : 29
	ODbPutDateTime:(aDateTime&, aFieldNumber&) : 30
	ODbPutLong:(aBuffer&, aLength&, aFieldNumber&) : 31
	ODbFindWhere%:(anSqlQuery$, aFlags%) : 32
	ODbUse:(anId&) : 33
	ODbSeekInt&:(aSearchInteger&, aTable$, anIndex$, aMode&) : 34
	ODbSeekWord&:(aSearchUnsignedInteger&, aTable$, anIndex$, aMode&) : 35
	ODbSeekText&:(aSearchText$, aTable$, anIndex$, aMode&) : 36
	ODbSeekReal&:(aSearchFloat, aTable$, anIndex$, aMode&) : 37
	ODbSeekDateTime&:(aSearchDateTime&, aTable$, anIndex$, aMode&) : 38
	ODbCount&: : 39
	ODbFindSql&:(anSqlQuery$, aFlags%) : 40
END DECLARE


PROCEDURE REFERENCE
-------------------

ODbGetTableCount&:
------------------
Usage: numberOfTables& = ODbGetTableCount&:(aPath$)

Returns the number of tables in the database with filename aPath$.


ODbGetTableName$:
-----------------
Usage: tableName$ = ODbGetTableName$:(aPath$, aTableNumber&)

Returns the name of a table in database aPath$.  The table is specified 
using aTableNumber&, with 1 being the first table.


ODbGetIndexCount&:
------------------
Usage: indexCount& = ODbGetIndexCount&:(aPath$, aTable$)

Returns the number of indices for the table aTable$ in the database with 
filename aPath$.


ODbGetIndexName$:
-----------------
Usage: indexName$ = ODbGetIndexName$:(aPath$, aTable$, anIndexNumber&)

Returns the name of an index for the table aTable$ in the database with 
filename aPath$.  The index is specified by anIndexNumber&, with 1 being the 
first index.


ODbGetIndexDescription$:
------------------------
Usage: description$ = ODbGetIndexDescription$:(aPath$, aTable$, anIndexName$)

Returns a string describing the key of an index.  The index within the 
database is given by anIndexName$.

The format of the returned string is:

    Keyname (KeyTextLength) Ascending|Descending [Unique] [Folded|Collated]

e.g. Name (25) Ascending Folded


ODbGetFieldCount&:
------------------
Usage: count& = ODbGetFieldCount&:(aPath$, aTable$)

Returns the number of fields in one of the records in the table aTable$ of the 
database aPath$. This number can then be used to analyse the contents of the 
record.

This function duplicates DBGetFieldCount& in dBase.OPX, but may be used when a
table within the database is open in your program. 


ODbGetFieldName$:
-----------------
Usage: name$ = ODbGetFieldName$:(aPath$, aTable$, aFieldNumber&)

Returns the name of the field whose number, starting from 1, is aFieldNumber& in 
the table table$ of the database dbase$. This is useful for analysing the 
records of a table.

This function duplicates DBGetFieldName$ in dBase.OPX, but may be used when a 
table within the database is open in your program. 


ODbGetFieldType&:
-----------------
Usage: fieldType& = ODbGetFieldType&:(aPath$, aTable$, aFieldNumber&)

Returns the type of the field whose number is aFieldNumber& in the table 
table$ of the database dbase$. This is useful for analysing the records of a 
table.

The values that may be returned (many of which aren’t supported by OPL 
databases), are defined using the following CONSTs:

Val	CONST				Type
---	-----				----
 0	KODbColBit&			bit
 1	KODbColInt8&		signed byte (8 bits)
 2	KODbColUint8&		unsigned byte (8 bits)
 3	KODbColInt16&		integer (16 bits)
 4	KODbColUint16&		unsigned integer (16 bits)
 5	KODbColInt32&		long integer (32 bits)
 6	KODbColUint32&		unsigned long integer (32 bits)
 7	KODbColInt64&		64-bit integer
 8	KODbColReal32&		single precision floating-point number (32 bits)
 9	KODbColReal64&		double precision floating-point number (64 bits)
10	KODbColDateTime&	date/time object
11	KODbColText8&		ASCII text
12	KODbColText16&		Unicode text
13	KODbColBinary&		Binary
14	KODbColLongText8&	LongText8
15	KODbColLongText16&	LongText16
16	KODbColLongBinary&	LongBinary

	KODbColText& equal to KODbColText16&
	KODbColLongText& equal to KODbColLongText16&

Types KODbColInt16&,KODbColInt32&,KODbColReal64&,KODbColText& correspond to
OPL’s %,&, floating point and $ types respectively.

This function duplicates DBGetFieldName$ in dBase.OPX, but may be used when a 
table within the database is open in your program.  Note that in the OPL 
Manual's description, the type numbers are incorrect. 


ODbGetFieldSize&:
-----------------
Usage: maxSize& = ODbGetFieldSize&:(aPath$, aTable$, aFieldNumber&)

Returns the maximum length of data that can be stored in a Text or Binary 
field. For other field types this value is undefined.


ODbGetCanBeEmpty%:
------------------
Usage: canBeEmpty% = ODbGetCanBeEmpty%:(aPath$, aTable$, aFieldNumber&)

Returns KTrue% if the specified field can be empty, or KFalse% otherwise.


ODbOpenR:
---------
Usage: ODbOpenR:(anId&, anSqlQuery$, aFieldTypes$)

Open a file with read only access.  This function corresponds to the built-in 
OPL command OPENR. The difference is that this function takes the field types 
from the string aFieldTypes$, and that all types supported by DBMS are allowed.

anId& is a number indicating the logical Name to open: 0 is A, 1 is B, 2 is C, 
and so on.

aFieldTypes$ must have exactly one character for every field, with the 
following meaning:

 "$" : an OPL string field
 "%" : an OPL integer field,
 "&" : an OPL long integer field,
 "." : an OPL real field,
 "?" : any other field.

An example: The built-in OPL line

 OPENR "c:\testdb SELECT name, age, income, height FROM Employee", C, f1$, f2%, f3&, f4

would correspond to the OPX function call:

 ODbOpenR:(2, "C:\testdb SELECT name, age, income, height FROM Employee", "$%&.")

The two expressions are really completely identical. You can use C.F1$, C.F2% 
etc to access the values of the fields, or, alternatively, you can use the 
ODbGetXxx and ODbPutXxx functions described below.

On the other hand:

 ODbOpenR:(3, "C:\testdb SELECT name, married, children, birthday, cv FROM Employees", "$????")

could be used if name is a string field, married a boolean field (Yes/No), 
children a byte field, birthday a DateTime object, and cv a long text field. 
You can use D.F1$ to access the name, but you will need to use ODbGetWord&:, 
ODbGetInt&:, ODbGetDateTime:, and ODbGetLong: to read the values of the other 
fields in this database.

Use CLOSE to close a database opened using this function.

See ODbOpen:


ODbOpen:
--------
Usage: ODbOpen:(anId&, anSqlQuery$, aFieldTypes$)

This function works in an identical way to ODbOpenR:, with the exception that 
the file is open for read/write access.  This means that the file cannot 
already be opened by another application.

See ODbOpenR:


ODbStartTable:
--------------
Usage: ODbStartTable:

Used together with ODbTableField: and ODbCreateTable: to create a new table.  
A call to ODbStartTable begins the process and must be made before calls to 
ODbTableField.


ODbTableField:
--------------
Usage: ODbTableField:(aFieldName$, aFieldType&, aMaximumLength&)

Used together with ODbStartTable: and ODbCreateTable: to create a new table.  
Once ODbStartTable has been called, call ODbTableField: for each field to be 
in the table.  The field type is as defined in ODbGetFieldType& above.  

aMaximumLength& specified the largest item which can be stored in a text or 
binary fields (types 12 and 13).  For other field types, the aMaximumLength& 
parameter is used to specify whether the field may be empty.  Pass 0 if the 
field may be empty, or 1 if it may not.


ODbCreateTable:
---------------
Usage: ODbCreateTable:(aFileName$, aTable$)

The ODbCreateTable: call actually creates the table. If aFileName$ does not 
exist, it is created as an OPL database file. This mechanism does not handle 
the SETDOC command. If you want to create an OPL document file, you will need 
to create it using OPL's create command, and then add a table using 
ODbCreateTable:. 

Note that ODbCreateTable does not open a new view on the database. An empty 
table is created in the database. If you want to write to it, you have to open 
it using ODbOpen:. 


ODbGetLength&:
--------------
Usage: fieldLength& = ODbGetLength&:(aFieldNumber&)

Returns the length of the field aFieldNumber& in the current database. If the
field is empty, zero is returned, otherwise it returns 1 for any numeric
field (types 0 to 10), and the length in bytes for text, binary, long
text, and long binary fields.


ODbGetString$:
--------------
Usage: text$ = ODbGetString$:(aFieldNumber&)

Return the contents of the field aFieldNumber& in the current database as a 
string. This is normally used for ASCII text and binary fields (types 11 and 
13), but can actually be used for any type except for long types.  Fields are 
counted starting from one.

The usual OPL syntax:
  name$ = C.nam$
would translate to:
  USE C
  name$ = ODbGetString$:(1)


ODbGetInt&:
-----------
Usage: integer& = ODbGetInt&:(aFieldNumber&)

Return the contents of the field aFieldNumber& in the current database. This 
has to be a signed integer field (types 1, 3, 5).

The usual OPL syntax:
  age% = C.age%
  salary& = C.sal&
would translate to:
  USE C
  age% = ODbGetInt&:(2)
  salary& = ODbGetInt&:(3)


ODbGetReal:
-----------
Usage: real = ODbGetReal:(aFieldNumber&)

Return the contents of the field aFieldNumber& in the current database. This 
has to be an OPL real field (type 9).


ODbGetReal32:
-------------
Usage: shortReal = ODbGetReal32:(aFieldNumber&)

Return the contents of the field aFieldNumber& in the current database. This 
has to be a short real field (type 8).


ODbGetWord&:
------------
Usage: unsignedInteger& = ODbGetWord&:(aFieldNumber&)

Return the contents of the field aFieldNumber& in the current database. This 
has to be an unsigned integer field (types 0, 2, 4, 6).


ODbGetDateTime:
---------------
Usage: ODbGetDateTime:(aDateTime&, aFieldNumber&)

Get the contents of the field aFieldNumber& in the current database, and place 
it in the date/time object aDateTime& (which must have been created using 
DTNewDateTime&: or DTNow&: from the Date.opx). The field must be a date/time 
field (type 10).


ODbGetLong:
-----------
Usage: ODbGetLong:(aBuffer&, aLength&, aFieldNumber&)

Get aLength& bytes from the field aFieldNumber& in the current database, and 
place it in the buffer aBuffer&. The field must be a long field type (types 
14, 15, 16). Normally you would first read the length of a long field using 
ODbGetLength&: and use this as the aLength& argument for this function.


ODbPutEmpty:
------------
Usage: ODbPutEmpty:(aFieldNumber&)

Makes field aFieldNumber& in the current database empty.


ODbPutString:
-------------
Usage: ODbPutString:(aText$, aFieldNumber&)

Assigns the value of aText$ to the field aFieldNumber& in the current 
database. This would normally be used for string or binary fields, or for long 
text or long binary fields, but it can actually be used for any type of field.


ODbPutInt:
----------
Usage: ODbPutInt:(anInteger&, aFieldNumber&)

Assigns the value of anInteger& to the field aFieldNumber& in the current 
database. The field must be a signed integer field (types 1, 3, 5).


ODbPutReal:
-----------
Usage: ODbPutReal:(aFloat, aFieldNumber&)

Assigns the value of aFloat to the field aFieldNumber& in the current 
database. The field must be an OPL real field.


ODbPutReal32:
-------------
Usage: ODbPutReal32:(aFloat, aFieldNumber&)

Assigns the value of aFloat to the field aFieldNumber& in the current 
database. The field must be a short real field (type 8).


ODbPutWord:
-----------
Usage: ODbPutWord:(anUnsignedInteger&, aFieldNumber&)

Assigns the value of no& to the i&th field in the current database. The field 
must be an unsigned integer field (types 0, 2, 4, 6).  


ODbPutDateTime:
---------------
Usage: ODbPutDateTime:(aDateTime&, aFieldNumber&)

Assigns the value of the DateTime object aDateTime& to the field aFieldNumber& 
in the current database. The field must be a date/time field (type 10).


ODbPutLong:
-----------
Usage: ODbPutLong:(aBuffer&, aLength&, aFieldNumber&)

Assigns the contents of the buffer aBuffer& of length aLength& to the field 
aFieldNumber& in the current database.  The field must be a long field (type 
14, 15, 16).


ODbUse:
-------
Usage: ODbUse:(anId&)

Equivalent to the OPL command USE, but takes a number from 0 to 25
instead of a letter from A-Z as an argument.

So:
  USE C
is identical to
  ODbUse:(2)


ODbSeekInt&:
------------
Usage: ODbSeekInt&:(aSearchInteger&, aTable$, anIndex$, aMode&)

Seek to a row in the table aTable$ using the index anIndex$. aTable$ must be a 
table in the current view, and anIndex$ must be the name of an index for this 
table. The index must have been created for a single field in the table, and 
this field must be an integer field. Mode& is one of the constants 
KODbLessThan&, KODbLessEqual&, KODbEqualTo&, KODbGreaterThan&, 
KODbGreaterEqual&. The function returns a non zero value if a row is found, 
zero otherwise.


ODbSeekWord&:
-------------
Usage: ODbSeekWord&:(aSearchUnsignedInteger&, aTable$, anIndex$, aMode&)

Seek to a row in the table aTable$ using the index anIndex$. aTable$ must be a 
table in the current view, and anIndex$ must be the name of an index for this 
table. The index must have been created for a single field in the table, and 
this field must be an unsigned integer field. Mode& is one of the constants 
KODbLessThan&, KODbLessEqual&, KODbEqualTo&, KODbGreaterThan&, 
KODbGreaterEqual&. The function returns a non zero value if a row is found, 
zero otherwise.


ODbSeekText&:
-------------
Usage: ODbSeekText&:(aSearchText$, aTable$, anIndex$, aMode&)

Seek to a row in the table aTable$ using the index anIndex$. aTable$ must be a 
table in the current view, and anIndex$ must be the name of an index for this 
table. The index must have been created for a single field in the table, and 
this field must be a text field. Mode& is one of the constants KODbLessThan&, 
KODbLessEqual&, KODbEqualTo&, KODbGreaterThan&, KODbGreaterEqual&. The 
function returns a non zero value if a row is found, zero otherwise.



ODbSeekReal&:
-------------
Usage: ODbSeekReal&:(aSearchFloat, aTable$, anIndex$, aMode&)

Seek to a row in the table aTable$ using the index anIndex$. aTable$ must be a 
table in the current view, and anIndex$ must be the name of an index for this 
table. The index must have been created for a single field in the table, and 
this field must be a float field. Mode& is one of the constants 
KODbLessThan&, KODbLessEqual&, KODbEqualTo&, KODbGreaterThan&, 
KODbGreaterEqual&. The function returns a non zero value if a row is found, 
zero otherwise.


ODbSeekDateTime&:
-----------------
Usage: ODbSeekDateTime&:(aSearchDateTime&, aTable$, anIndex$, aMode&)

Seek to a row in the table aTable$ using the index anIndex$. aTable$ must be a 
table in the current view, and anIndex$ must be the name of an index for this 
table. The index must have been created for a single field in the table, and 
this field must be a date/time field. Mode& is one of the constants 
KODbLessThan&, KODbLessEqual&, KODbEqualTo&, KODbGreaterThan&, 
KODbGreaterEqual&. The function returns a non zero value if a row is found, 
zero otherwise.


ODbCount&:
----------
Usage: ODbCount&:

Returns the number of rows in the current view.  This is similar to
the COUNT command in OPL, but returns a long integer and therefore
works even if the number of rows is larger than 65536.


ODbFindSql&:
------------
Usage: ODbFindSql&:(anSqlQuery$, aFlags%)

These functions are used in a similar way to the built-in function FINDFIELD, 
in particular the aFlags% argument and the return value have the same meaning. 
The difference is that with this procedure you can have a query like:

    ODbFindSql&:("name LIKE '*Miller*' AND (height > 1.80 OR salary < 20000)", 1)

The following useful constants defined in const.oph can be used for aFlags%:

    KFindCaseDependent%
    KFindBackwards%
    KFindForwards%
    KFindBackwardsFromEnd%
    KFindForwardsFromStart%

ODbFindWhere%:
--------------
Usage: ODbFindWhere%:(anSqlQuery$, aFlags%)

Obsolete, and included for backwards compatibility only.


COPYRIGHT
---------
Data.opx is Copyright (c) 1997-2002 Symbian Ltd.  All rights reserved.  It 
forms part of the OPL SDK and is subject to the License contained therein.


DISTRIBUTION 
------------ 
Data.opx should only be distributed to end users in one of the SIS files 
included in this release.  This ensures that older versions will not be 
installed over newer versions without warning.

The best way to do this is to include the Data SIS within your applications 
PKG file.  This means you can distribute your application as a single SIS file.

For ARM distributions use this line in your PKG file:  
@"Data.SIS",(0x10004EC7)

For WINS distributions use this line: 
@"DataW.SIS",(0x10004EC7)
