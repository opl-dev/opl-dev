// README.TXT
//
// Copyright (c) 1997-2000 Symbian Ltd.  All rights reserved.
//

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!TODO This is unfinished and should not form part of a release product
!!TODO until completed.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


PRINTER.OPX
===========

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
rem PRINTER.OXH version 1.0
rem Header File for PRINTER.OPX
rem Copyright (c) 1997-1998 Symbian Ltd. All rights reserved.


rem CONSTANT DECLARATIONS
rem
rem	These consts are used for setting formats

rem Character Format constants

	rem Font Posture

	CONST KPostureUpright% = 0
	CONST KPostureItalic% = 1
	

	rem Font Stroke Weights
	
	CONST KStrokeWeightNormal% = 0
	CONST KStrokeWeightBold% = 1	

	rem Font Print Position
	
	CONST KPrintPosNormal% = 0
	CONST KPrintPosSuperscript% = 1
	CONST KPrintPosSubscript% = 2
	
	rem Font Underline
	
	CONST KUnderlineOff% = 0
	CONST KUnderlineOn% = 1
	
	rem Strikethrough

	CONST KStrikethroughOff% = 0
	CONST KStrikethroughOn% = 1


		
rem Paragraph Format constants

rem LineSpacingControl

	CONST KLineSpacingAtLeastInTwips% = 0
	CONST KLineSpacingExactlyInTwips% = 1
	rem CONST KLineSpacingAtLeastInPixels% = 2
	rem CONST KLineSpacingExactlyInPixels% = 3

rem Alignment

	CONST KPrintLeftAlign% = 0	
	CONST KPrintTopAlign% = 0
	CONST KPrintCenterAlign% = 1
	CONST KPrintRightAlign% = 2
	CONST KPrintBottomAlign% = 2
	CONST KPrintJustifiedAlign% = 3
	CONST KPrintUnspecifiedAlign% = 4
rem		CONST KPrintCustomAlign% = 5


	rem New OPL API

const KUidOpxPrinter&=&1000025D
const KOpxPrinterVersion%=$100

DECLARE OPX PRINTER,KUidOpxPrinter&,KOpxPrinterVersion%

	SendStringToPrinter:(string$) : 1
	InsertString:(string$,pos&) : 2
	SendNewParaToPrinter: : 3
	InsertNewPara:(pos&) : 4
	SendSpecialCharToPrinter:(character%) : 5
	InsertSpecialChar:(character%, pos&) : 6
	SetAlignment:(alignment%) : 7
	InitialiseParaFormat:(Red%, Green%, Blue%,	LeftMarginInTwips&,	RightMarginInTwips&,	IndentInTwips&,	HorizontalAlignment%,	VerticalAlignment%, LineSpacingInTwips&,	LineSpacingControl%,			SpaceBeforeInTwips&,	SpaceAfterInTwips&, KeepTogether%,	KeepWithNext%,	StartNewPage%,	WidowOrphan%,	Wrap%,	BorderMarginInTwips&,	DefaultTabWidthInTwips&) : 8
	SetLocalParaFormat: : 9
	SetGlobalParaFormat: : 10
	RemoveSpecificParaFormat: : 11
	SetFontName:(name$) : 12
	SetFontHeight:(height%) : 13
	SetFontPosition:(pos%) : 14
	SetFontWeight:(weight%) : 15
	SetFontPosture:(posture%) : 16
	SetFontStrikethrough:(strikethrough%) : 17
	SetFontUnderline:(underline%) : 18
	SetGlobalCharFormat: : 19
	RemoveSpecificCharFormat: : 20
	SendBitmapToPrinter:(bitmapHandle&) : 21
	InsertBitmap:(bitmapHandle&, pos&) : 22
	SendScaledBitmapToPrinter:(bitmapHandle&, xScale&, yScale&) : 23
	InsertScaledBitmap:(bitmapHandle&, pos&, xScale&, yScale&) : 24
	PrinterDocLength&: : 25
	SendRichTextToPrinter:(richTextAddress&) : 26
	ResetPrinting: : 27
	PageSetupDialog: : 28
	PrintPreviewDialog: : 29
	PrintRangeDialog: : 30
	PrintDialog: : 31
	SendBufferToPrinter:(addr&) : 32
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