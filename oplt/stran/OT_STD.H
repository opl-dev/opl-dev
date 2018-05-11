// OT_STD.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __OT_STD_H__
#define __OT_STD_H__

#include <e32base.h>
#include <opllex.h>
#include <opltran.h>
#include <oplbacke.h>
#include "ot_symbl.h"
#include "ot_parse.h"

enum TOpltPanic
	{


	// TOplField
	EOpltBadLogicalDevice,

	// TOplSignature
	EOpltTooManyArguments,
	EOpltIllegalArgIndex,
	
	// Lexer
	EOpltLexerReentrantOpen,
	
	// Symbols
	EOpltIllegalDeclClass,
	EOpltIllegalReferenceClass,

	// Symbol Table
	EOpltScopeUnderflow,
	EOpltQuesInconsistent,

	// Translator
	EOpltUnknownTranTarget,

	// ParseOPX
	EOpltParseOpxNameSet,

	// ParseProc
	EOpltParseProcReentrant,
	
	// Parser - General 
	EOpltParserNotReset, // Tried to do somethign when the parser _should_ be reset
	EOpltParserReset, // Tried to do something while the parser is RESET.
	EOpltBadRightSideType, // Unknown type of symbol in a rght side reference
	EOpltUnknownIdentifier, // Default panic in switch on identifier type
	EOpltConstantSymbol, // Somehow a constant symbol made it through to the parser!!

	// Parser - STRUCT
	EOpltParserUnknownStructure, // Expecting IF, DO, WHILE or VECTOR & got something else

	// Parser - KEYWORDS	
	EOpltKeywordTypedAndQualified,
	
	// Parser - CALLS
	EOpltUnknownFunctionOverload,
	EOpltUnknownCallType,

	// PCode Reader
   EOpltReaderUnknownType,
	EOpltReaderUnknownSize
	};
   
GLREF_C void Panic(TOpltPanic aPanic);
GLREF_C void SyntaxErrorL();
GLREF_C void TypeMismatchL();

#endif
