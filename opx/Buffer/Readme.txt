// README.TXT
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

BUFFER.OPX
==========
Contents:

1.  Introduction
2.  File Details
3.  Installation
4.  Using the OPX
5.  Contents of Buffer.oxh
6.  Procedure Reference
7.  Copyright
8.  Distributing the OPX

INTRODUCTION
------------
This OPX gives you access to a number of functions for manipulating the 
contents of memory buffers.

FILE DETAILS
------------
The archive consists of the following files:

README.TXT      This file.
BUFFER.SIS      This is the main OPX file in SIS format.
BUFFERW.SIS     This is the WINS version in SIS format.
BUFFER.TXH      This is the header file in text file format.
DBUFFER.TPL     This is a demonstration program that shows you how the OPX can be used.

INSTALLATION
------------
1.  Install BUFFER.SIS.

2.  Create BUFFER.OXH into the \System\Opl\ folder on either the C: or D: 
drive. This may be done by either starting a new file called BUFFER.OXH and 
then using Import text... BUFFER.TXH, or by making use of OPLTRAN -CONV  (see 
the OPL SDK for details).

3.  Create DBUFFER.OPL from DBUFFER.TPL any where you like.

USING THE OPX
-------------
1.  First compile and run the BUFFER.OPL file to make sure everything works 
correctly.

2.  To use the OPX in your program add the following line to the top of the 
code, immediately after the APP...ENDA and before the first procedure:

    INCLUDE "BUFFER.OXH"

3.  You can now use the Buffer.opx procedures in your program.

CONTENTS OF BUFFER.TXH
----------------------
CONST KBufferNoFoldOrCollate&=0
CONST KBufferFold&=1
CONST KBufferCollate&=2

CONST KBufferNotFound&=-1

DECLARE OPX BUFFER,KOpxBufferUid&,KOpxBufferVersion%
    BufferCopy:(aTargetBuffer&, aSourceBuffer&, aLength&) : 1
    BufferFill:(aBuffer&, aLength&, aChar%) : 2
    BufferAsString$:(aSourceBuffer&, aLength&) : 3
    BufferFromString&:(aTargetBuffer&, aLength&, aSource$) : 4
    BufferMatch&:(aBuffer&, aLength&, aPattern$, aFoldMode&) : 5
    BufferFind&:(aBuffer&, aLength&, aString$, aFoldMode&) : 6
    BufferLocate&:(aBuffer&, aLength&, aChar%, aFoldMode&) : 7
    BufferLocateReverse&:(aBuffer&, aLength&, aChar%, aFoldMode&) : 8
END DECLARE

PROCEDURE REFERENCE
-------------------

BufferCopy:
-----------
Usage: BufferCopy:(aTargetBuffer&, aSourceBuffer&, aLength&)

Copies aLength& bytes from aSourceBuffer& to aTargetBuffer&. The procedure 
detects and handles the case of overlapping buffers.

BufferFill:
-----------
Usage: BufferFill:(aBuffer&, aLength&, aChar%)

Fills a buffer with a character. The function assumes the char is non-Unicode.
For example, filling a 4-byte buffer with 0x0031 will result in 0x31313131.

BufferAsString$:
----------------
Usage: string$ = BufferAsString$:(aSourceBuffer&, aLength&)

Returns the contents of the buffer as a string. aLength& should be <=255.

BufferFromString&:
------------------
Usage: buffer& = BufferFromString&:(aTargetBuffer&, aLength&, aSource$)

Assigns the buffer from the string, returning the length of the string in the 
buffer.

BufferMatch&:
-------------
Usage: position& = BufferMatch&:(aBuffer&, aLength&, aPattern$, aFoldMode&)

Matches the buffer with a pattern. The pattern may contain the usual wildcards 
? and *. Returns the position (starting with zero) of the first matching 
character, or -1 if the pattern is not found. 

aFoldMode& may be set to KBufferNoFoldOrCollate&, KBufferFold& or 
KBufferCollate&. 

Folding means the removal of differences between characters that are deemed 
unimportant for the purposes of inexact or case-insensitive matching. As well 
as ignoring differences of case, folding ignores any accent on a character. By 
convention, folding converts lower case characters into upper case and removes 
any accent.

Collating means the removal of differences between characters that are deemed 
unimportant for the purposes of ordering characters into their collating 
sequence. For example, collate two strings if they are to be arranged in 
properly sorted order; this may be different from a strict alphabetic order.

NOTE: To test for the existence of a pattern within a text string, the pattern 
must start and end with an `*'.

Example:  A buffer has the contents: "abcdefghijklmnopqrstuvwxyz"

BufferMatch&:(buffer&, len&, "*ijk*", 0)       returns -> 8
BufferMatch&:(buffer&, len&, "*i?k*", 0)               -> 8
BufferMatch&:(buffer&, len&, "ijk*", 0)                -> -1
BufferMatch&:(buffer&, len&, "abcd", 0)                -> -1
BufferMatch&:(buffer&, len&, "*i*mn*", 0)              -> 8
BufferMatch&:(buffer&, len&, "abcdef*", 0)             -> 0
BufferMatch&:(buffer&, len&, "*", 0)                   -> 0
BufferMatch&:(buffer&, len&, "*y*", 0)                 -> 24
BufferMatch&:(buffer&, len&, "*i??k*", 0)              -> -1

BufferFind&:
------------
Usage: position& = BufferFind&:(aBuffer&, aLength&, aString$, aFoldMode&)

Searches for the string$ in the buffer. Returns the position (starting with 
zero) of the first occurrence, or -1 if the string is not found. Unlike 
BufferMatch&: wildcards cannot be used.

aFoldMode& may be set to KBufferNoFoldOrCollate&, KBufferFold& or 
KBufferCollate&. 

BufferLocate&:
--------------
Usage: position& = BufferLocate&:(aBuffer&, aLength&, aChar%, aFoldMode&)

Searches for the character with code char% in the buffer. Returns the position 
(starting with zero) of the first occurrence, or -1 if the character is not 
found. 

aFoldMode& may be set to KBufferNoFoldOrCollate&, KBufferFold& or 
KBufferCollate&. 

BufferLocateReverse&:
---------------------
Usage: position& = BufferLocateReverse&:(aBuffer&, aLength&, aChar%, aFoldMode&)

Searches backwards from the end of the buffer for the character with code 
char% in the buffer. Returns the position (starting with zero) of the first 
occurrence, or -1 if the character is not found. 

aFoldMode& may be set to KBufferNoFoldOrCollate&, KBufferFold& or 
KBufferCollate&. 

Copyright
---------
Buffer.opx is Copyright (c) 1997-2002 Symbian Ltd. All rights reserved. It 
forms part of the OPL SDK and is subject to the License contained therein.

Distribution
------------
Buffer.opx should only be distributed to end users in one of the SIS files 
included in this release. This ensures that older versions will not be 
installed over newer versions without warning.

The best way to do this is to include the Buffer SIS within your applications 
PKG file. This means you can distribute your application as a single SIS file.

For MARM distributions use this line in your PKG file:  
@"Buffer.SIS",(0x10004EC9)

For WINS distributions use this line: @"BufferW.SIS",(0x10004EC9)