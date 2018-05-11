// README.TXT
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

SPELL.OPX
=========

Contents:

1.	Introduction
2.	File Details
3.	Installation
4.	Using the OPX
5.  Contents of Spell.oxh
6.  Procedure Reference
7.  Distributing the OPX


INTRODUCTION
------------

This OPX gives you access to a number of functions of the EPOC Spell checker 
to enable you to access them from within an OPL program.  

FILE DETAILS
------------

The archive consists of the following files:

README.TXT		This file
SPELL.SIS		This is the main OPX file in SIS format
SPELLW.SIS		This is the WINS version of the OPX in SIS format
SPELL.TXH		This is the header file in text format
DSPELL.TPL		This is a demonstration program that shows you how the OPX can 
be used


INSTALLATION
------------

1.	Install SPELL.SIS

2.  Create SPELL.OXH in the \System\Opl\ folder on either the C: or D: drive.  
This may be done by either starting a new file called SUBST.OXH and then using 
Import text... SUBST.TXH, or by making use of OPLTRAN -CONV (see the OPL SDK 
for details).

3.	Copy DSPELL.OPL any where you like


USING THE OPX
-------------

1.	First compile and run the SPELL.OPL file to make sure everything works and 
it communicates with SPELL Suite correctly.

2.	To use the OPX in your program add the following line to the top of the 
code, immediately after the APP...ENDA and before the first procedure

	INCLUDE "SPELL.OXH"

3.	You can now use the following SystInfo.opx procedures in your program.


CONTENTS OF SPELL.OXH
---------------------

CONST KSpellNoUserDictionary&=0
CONST KSpellUserDictionary&=1

CONST KSpellContinue&=0
CONST KSpellNoMore&=1

DECLARE OPX SPELL,KOpxSpellUid&,KOpxSpellVersion%
	Spell$:(aString$) : 1
	OpenSpeller:(aUserDictionary&) : 2
	CloseSpeller: : 3
	SpellUserDict&: : 4
	SpellBuffer$:(aBuffer&, aLength&) : 5
	SpellAlternatives:(aWord$, aMaximumAlternatives&, aCallback$) : 6
	SpellWhere&: : 7
	SpellWildcards:(aWord$, aMaximumAlternatives&, aCallback$) : 8
	SpellAnagrams:(aWord$, aMinimumLength&, aMaximumAlternatives&, aCallback$) : 9
END DECLARE


PROCEDURE REFERENCE
-------------------

Spell$:
-------
Usage: first$ = Spell$:(aString$)

Spell checks every word in the string. An empty string is returned if the 
spelling appears to be correct. Otherwise, the first misspelled word is 
returned.


OpenSpeller:
------------
Usage: OpenSpeller:(aUserDictionary&)

Starts a connection with the spell checker. You must call this before calling 
Spell$: or SpellBuffer$:.  The aUserDictionary& parameter is used to specify 
whether the user dictionary is also to be checked for spellings.  The 
constants KSpellUserDictionary& and KSpellNoUserDictionary& are defined in 
SPELL.OXH and may be used to specify this parameter.


CloseSpeller:
-------------
Usage: CloseSpeller:

Closes the connection with the spelling checker.  It is important to call this 
when you have finished using the dictionary, as it cannot be shared with 
another application.


SpellUserDict&:
---------------
Usage: userDict& = SpellUserDict&:

Enquires whether the user dictionary is being used for checking spellings.  
This corresponds to the aUserDictionary& parameter in OpenSpeller: and you may 
use the same constants (KSpellUserDictionary& and KSpellNoUserDictionary&) to 
compare with the returned value.


SpellBuffer$:
-------------
Usage: first$ = SpellBuffer$:(aBuffer&, aLength&)

This checks the spelling of every word in the buffer at address aBuffer&. The 
length of the buffer is passed in aLength&.  An empty string is returned if 
the spelling appears to be correct. Otherwise, it returns the first word that 
is misspelled.


SpellAlternatives:
------------------
Usage: SpellAlternatives:(aWord$, aMaximumAlternatives&, aCallBack$)

Looks for alternative (more correct) spellings of the word aWord$. If 
alternative spellings are found, a callback function named in aCallback$ is 
called for each one found (up to a maximum of aMaximumAlternatives&).  

The callback procedure can decide to stop the processing of further words by 
returning KSpellNoMore&.  To continue the callback returns KSpellContinue&.  

Because of the return value, the callback procedure needs to end in &, 
although the & isn't included in aCallback$.  See the demo program DSPELL.


SpellWhere&:
------------
Usage: position& = SpellWhere&:

Returns the position (starting from 1) of the last misspelled word found. 
Useful to restart spelling, or for marking the incorrect word.


SpellWildcards:
---------------
Usage: SpellWildcards:(aWord$, aMaximumAlternatives&, aCallBack$)

Given a aWord$ containing the wildcard characters "*" and "?", this looks for 
spellings matching the pattern. If matching words are found, a callback 
function named in aCallback$ is called for each matching word found (up to a 
maximum of aMaximumAlternatives&).  

The callback procedure can decide to stop the processing of further words by 
returning KSpellNoMore&.  To continue the callback returns KSpellContinue&.  

Because of the return value, the callback procedure needs to end in &, 
although the & isn't included in aCallback$.  See the demo program DSPELL.


SpellAnagrams:
--------------
Usage: SpellAnagrams:(aString$, aMinimumLength&, aMaximumAlternatives&, aCallback$)

Finds anagrams using the letters in aString$.  Only anagrams of 
aMinimumLength& or larger are returned, and only up to a maximum of 
aMaximumAlternatives. If anagrams are found, a callback function named in 
aCallback$ is called for each matching word found. 

The callback function call decide to stop the processing of further words by 
returning KSpellNoMore&.  To continue the callback returns KSpellContinue&.  

Because of the return value, the callback procedure needs to end in &, 
although the & isn't included in aCallback$.  See the demo program DSPELL.


COPYRIGHT
---------

Spell.opx is Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.  It 
forms part of the OPL SDK and is subject to the license contained therein.


DISTRIBUTING THE OPX 
--------------------

Spell.opx should only be distributed to end users in one of the SIS files 
included in this release.  This ensures that older versions will not be 
installed over newer versions without warning.

The best way to do this is to include the Spell SIS within your applications 
PKG file.  This means you can distribute your application as a single SIS file.

For MARM distributions use this line in your PKG file:  
@"Spell.SIS",(0x10000b92)

For WINS distributions use this line: 
@"SpellW.SIS",(0x10000b92)