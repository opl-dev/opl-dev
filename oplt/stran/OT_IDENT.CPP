// OT_IDENT.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "ot_std.h"

/////////////////////////////////////////////////////////////////
//
// COplParserBase
//
/////////////////////////////////////////////////////////////////

void COplParserBase::IdentifierReferenceL(TOplToken aToken, TIndexFlag aFlag, TPcode::TIdentifierSide aSide)
//
//
//
	{

	switch (aToken)
		{
		case TOplToken::EArray:
		case TOplToken::ESimple:
			SimpleOrArrayRefL(aToken,aFlag, aSide);
			break;
		case TOplToken::EField:
			FieldReferenceL(aSide); // Virtual - Eval version just does syntax error
			break;
		case TOplToken::EConstant:
			if (aSide==TPcode::ELeft) // Constants have to be on the right hand side.
				SyntaxErrorL();
			ConstantReferenceL(Lexer().Constant());
			break;
#if defined (_DEBUG)
		default:
			Panic(EOpltUnknownIdentifier);
#endif
		}
	}

void COplParserBase::LeftSideReferenceL(TOplToken anIdent, TIndexFlag aFlag)
//
// Just a syntax error, should perhaps panic
//
	{
	
	IdentifierReferenceL(anIdent,aFlag,TPcode::ELeft);
	}


void COplParserBase::RightSideReferenceL(TOplToken anIdent)
//
// The only base RightSideReferences are constants
//
	{

	
	IdentifierReferenceL(anIdent,EForceIndices,TPcode::ERight);
	}

void COplParserBase::SimpleOrArrayRefL(TOplToken aToken,TIndexFlag aFlag,TPcode::TIdentifierSide aSide)
//
// Reference to a simple or array identifier - have lexed out the identifier
//
// SimpleOrArrayRef := simpleIdent
//					 | arrayIdent [index] ')'
//

	{
	
	// First see if it exists in the symbol table
	COplSymbol *pSym=iSymbols->Find(Lexer().Name(),aToken);
	
	// If it's not in the symbol table, this reference is also the 
	//      declaration of the symbol as an external.
	// Otherwise
	//      check to make sure that it's not already defined as a CONST.
	if (!pSym)
		{
		if (!pSym && iExplicitDefines) // The symbol should have been declared explicitly 
			User::Leave(EErrUndefinedVariable);
		pSym=NewSymbolL(COplSymbol::EExternalDecl);
		}
	else if (pSym->Class()==COplSymbol::EConstant)
		Panic(EOpltConstantSymbol);
	
	// If the symbol we see is an External Declaration then we have to add a reference 
	// to record the fact that it has actually been used in this procedure.
	// This is because it may have been explicity declared outside the
	// procedure scope.
	// In future, the reference will obscure the Declaration and will
	// result in the symbol getting an address in the procedure header.
	if (pSym->Class()==COplSymbol::EExternalDecl)
		pSym=ReferenceL(*pSym);

	// Can't get a left side reference to an argument
	COplSymbol::TClass symbolClass=pSym->Class();
	if (symbolClass==COplSymbol::EArgument && aSide==TPcode::ELeft)
		User::Leave(EErrArgNotReferencedByValue); // Can't assign to arguments

	// Array indices (forced or "au naturale").
	if (aToken==TOplToken::EArray) // arrayIdent [index] ')
		{
		if (NextIsL(TOplToken::ECloseBracket)) // [index] is missing
			{
			if (aFlag==EForceIndices) // Not allowed to miss them out
				User::Leave(EErrSubscript);

			PCodeL(TPcode::EConstant);
			iCode<<TOplConstant(KOplDefaultArrayIndex);
			}
		else
			{
			ExpressionL(TOplToken::EWord);
			if (!NextIsL(TOplToken::ECloseBracket))
				User::Leave(EErrSubscript);
			}
		}
	
	// Finally put out that PCode.
	PCodeL(IdentifierReferencePcode());
	iCode<<pSym<<aSide;
	}

void COplParserBase::ConstantReferenceL(const TOplConstant& aConstant)
//
//
//
	{
	PCodeL(TPcode::EConstant);
	iCode<<aConstant;
	}

////////////////////////////////////////////////////////////////
//
// COplModuleParser
//
/////////////////////////////////////////////////////////////////

void COplModuleParser::IdentifierStatementL(TOplToken aToken)
//
// IdentifierStatement := LeftSideRef '=' Expression
//
	{
	
	
	// So that we can check the sides match.
	TOplToken::TType type=Lexer().Type(); 
	
	// Parse it 
	LeftSideReferenceL(aToken,EForceIndices);	// LeftSideRef
	MustBeL(TOplToken::EEqual);					// =
	ExpressionL(type);							// Expression
	
	// Assignment Pcode.
	PCodeL(TPcode::EAssign);
	iCode<<type;
	}


TPcode::TCode COplModuleParser::IdentifierReferencePcode() const
//
// Pcode for an identifier reference. Different in the evaluator.
//
	{
	
	return TPcode::EIdentifier;
	}

void COplModuleParser::FieldReferenceL(TPcode::TIdentifierSide aSide)
//
// Reference to a field e.g. A.A$
//
	{
	
	PCodeL(TPcode::EField);
	iCode<<Lexer().Field()<<Lexer().Type()<<aSide;
	}


////////////////////////////////////////////////////////////////
//
// COplEvalParser
//
/////////////////////////////////////////////////////////////////
TPcode::TCode COplEvalParser::IdentifierReferencePcode() const
//
// PCode for an external reference during EVAL.
//
	{
	return TPcode::EEvalIdentifier;
	}

void COplEvalParser::FieldReferenceL(TPcode::TIdentifierSide /*aSide*/)
//
// Field references are not allowed in EVAL expressions
//
	{
	SyntaxErrorL();
	}
