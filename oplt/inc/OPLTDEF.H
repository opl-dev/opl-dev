// OPLTDEF.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Some definitions common to the OPL Lexer, Translator (and runtime) header files

#ifndef __OPLTDEF_H__
#define __OPLTDEF_H__
#include <e32base.h>

const TInt KOplMaxIdentifierLength=32;
const TInt KOplOpl1993MaxIdentLength=8;
const TInt KDefaultIdentifierLength=KOplMaxIdentifierLength;

const TUint KOplMaxOpxCount=KMaxTUint8+1;

const TInt KOplMaxStringLength=255;
const TInt KOplMaxLineLength=255;

const TUint KOplMaxFieldCount=32;

const TInt16 KOplDefaultArrayIndex=1;

// Do not change the order of these
// Insert new errors at the end
// The runtime uses these enumerations
enum TOplTranslateError
	{
	EErrFound=1,	       // 1    Found the error
	EErrSuccess=1,		   // 1	   Successful translation
	EErrMissingQuote,      // 2
	EErrBadStringLength,   // 3
	EErrBadIdentifier,     // 4
	EErrIdentifierTooLong, // 5
	EErrBadLogicalDevice,  // 6
	EErrBadFieldName,      // 7
	EErrBadNumber,         // 8
	EErrBadSuffices,       // 9
	EErrBadCharacter,      // 10
	EErrFnArgument,        // 11
	EErrTypeMismatch,      // 12
	EErrMissingLabel,      // 13
	EErrDuplicateName,     // 14
	EErrBadDeclaration,    // 15
	EErrBadArraySize,      // 16
	EErrStructure,         // 17
	EErrUnexpectedEof,     // 18
	EErrSyntax,            // 19
	EErrMismatchedBracket, // 20
	EErrBadFieldList,      // 21
	EErrTooComplex,        // 22
	EErrMissingComma,      // 23
	EErrDataTooLarge,      // 24
	EErrArgNotReferencedByValue, // 25
	EErrSubscript,               // 26
	EErrInconsistentArgs,        // 27
	EErrLineTooLong,             // 28
	EErrAssignToConstant,        // 29
	EErrIncludeProcedure,        // 30

	EErrTooManyOPXs,             // 31 Attempted to declare too many OPXs
	EErrTooManyOpxFunctions,     // 32

	EErrUndefinedVariable,       // 33 DECLARE EXTERNAL has been used and external has not been declared
	EErrUndefinedProcedure,      // 34 DECLARE EXTERNAL has been used and procedure has not been declared

	EErrIconMaskMissing,         // 35

	EErrIncompatibleDeclaration  // 36 Procedure is EXTERNALly declared and then the definition later doesn't match
	};
//
// Target machine variants. Publically, they are mutually exclusive hence
// the enum, but internally they are used as bit flags by, e.g. the lexer, 
// to determine which keywords are valid for a target machine, hence the constants

const TUint KOplTargetOpl1993=0x00000001;
const TUint KOplTargetER1=0x00000004;
enum TOplTranTarget
   {
   EOplTargetOpl1993=KOplTargetOpl1993,
   EOplTargetER1=KOplTargetER1
   };

const TOplTranTarget KOplDefaultTarget=EOplTargetER1;

#endif
