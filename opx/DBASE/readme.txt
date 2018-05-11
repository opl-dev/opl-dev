// README.TXT
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!TODO This is unfinished and should not form part of a release product
!!TODO until completed.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

DBASE.OPX
=========

Contents:

1.	Introduction
2.	File details
3.	Installation
4.	Using the OPX
5.  OXH contents
6.  Procedure reference
7.  Copyright
8.	Distributing the OPX


INTRODUCTION
------------

This OPX <intro of what OPX does in a few lines>.
<Include any warnings etc.>


FILE DETAILS
------------

The archive consists of the following files:

README.TXT      This file
<OPXNAME>.SIS   This is the main OPX file in SIS format
xxxxxxxW.SIS    This is the WINS version in SIS format
xxxxxxx.TXH     This is the header file in text file format
tXxxxxxx.TPL    This is a demonstration program that shows you how the OPX can 
be used

<Don't forget to include the example source etc.>


INSTALLATION
------------

<<<All installation instructions here.

For example: 

1.  Install AGENDA2.SIS.

2.  Create AGENDA2.OXH into the \System\Opl\ folder on either the C: or D: 
drive.  This may be done by either starting a new file called AGENDA2.OXH and 
then using Import text... AGENDA2.TXH, or by making use of OPLTRAN -CONV (see 
the OPL SDK for details).

3.  Create DAGENDA2.OPL from DAGENDA2.TPL any where you like.

>>>


USING THE OPX
-------------

<How to use the OPX>
1.  First translate and run the DAGENDA2.OPL file to make sure everything correctly.

2.  To use the OPX in your program add the following line to the top of the 
code, immediately after the APP...ENDA and before the first procedure

    INCLUDE "AGENDA2.OXH"

3.  You can now use the Agenda2 OPX procedures in your program.


CONTENTS OF OXH
---------------
rem DBASE.OXH version 1.0
rem Header File for DBASE.OPX
rem Copyright (c) 1997-1998 Symbian Ltd. All rights reserved.

const  KDbCompareNormal&=0
const  KDbCompareFolded&=1
const  KDbCompareCollated&=2
const  KDbAscending&=1
const  KDbDescending&=0

const KUidOpxDBase&=&1000025B
const KOpxDBaseVersion%=$100

DECLARE OPX DBASE,KUidOpxDBase&,KOpxDBaseVersion%
    DbAddField:(keyPtr&,fieldName$,order&) : 1
    DbAddFieldTrunc:(keyPtr&,fieldName$,order&,trunc&) : 2
    DbCreateIndex:(index$,keyPtr&,dbase$,table$) : 3
    DbDeleteKey:(keyPtr&) : 4
    DbDropIndex:(index$,dbase$,table$) : 5
    DbGetFieldCount&:(dbase$,table$) : 6
    DbGetFieldName$:(dbase$,table$,fieldNum&) : 7
    DbGetFieldType&:(dbase$,table$,fieldNum&) : 8
    DbIsDamaged&:(dbase$) : 9
    DbIsUnique&:(keyPtr&) : 10
    DbMakeUnique:(keyPtr&) : 11
    DbNewKey&: : 12
    DbRecover:(dbase$) : 13
    DbSetComparison:(KeyPtr&,comp&) : 14
END DECLARE


PROCEDURE REFERENCE
-------------------

<<<List of all procedures and notes in the following format:

AgnOpen:
--------
Usage: AgnOpen:(filename$)

Open the Agenda file.  You need to ensure that the file is not open in Agenda 
before using AgnOpen. See the demo OPL code for details of how to 
automatically close and reopen Agenda.

>>>


COPYRIGHT
---------

<OPXNAME>.opx is Copyright (c) 1997-2000 Symbian Ltd.  All rights reserved.  It 
forms part of the Symbian OPL SDK and is subject to the License contained therein.


DISTRIBUTION 
------------ 

<<<How to distribute this OPX
Agenda2.opx should only be distributed to end users in one of the SIS files 
included in this release.  This ensures that older versions will not be 
installed over newer versions without warning.

The best way to do this is to include the Alarm SIS within your applications 
PKG file.  This means you can distribute your application as a single SIS file.

For MARM distributions use this line in your PKG file:  
@"Agenda2.SIS",(0x10000547)

For WINS distributions use this line: 
@"Agenda2.SIS",(0x10000547)

>>>