// OT_CALL.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#include "ot_std.h"

///////////////////////////////////////////////////////////////////
//
// TSignature - Holds the argument & returns type signature of a
// function or procedure
//
///////////////////////////////////////////////////////////////////
void TOplSignature::Initialize(TOplToken::TType aReturnType)
	{
	iReturnType=TUint8(aReturnType);
	iNeedsCount=ETrue;
	iPermittedArgumentCounts=0;
	iArgumentCount=0;
	Mem::Fill(iArgTypes,KOplMaxArgCount,TChar((TUint)EArgBadType));
	}

void TOplSignature::SetArgumentCount(TUint anArgumentCount)
//
// Sets the argument count and allows it.
// Used in the case of implicit argument lists where there
// is no doubt about the number of permitted arguments
//
	{
	
	iArgumentCount=TUint8(anArgumentCount);
	AllowArgumentCount(anArgumentCount);
	}

void TOplSignature::AllowArgumentCount(TUint anArgCount)
	{
	// A call needs an argument count unless the function
	// only permits one argument count.
	iNeedsCount=TUint8(!!iPermittedArgumentCounts); 
	AllowArgumentCountByPass(anArgCount);
	}

void TOplSignature::AllowArgumentCountByPass(TUint anArgCount)
//
// This by passes the check for whether or not the 
// function has mutiple allowed arguments (and so needs the argument count)
// adding to the qcode. This is used by one of the mad overloaded functions
// (this language is such a mess)
//
	{
	iPermittedArgumentCounts|=(1<<anArgCount);
	}

void TOplSignature::AddArgument(TUint anArgument,TArgType aType)
//
// Adds an argument to the signature
// 
	{
	__ASSERT_ALWAYS(anArgument<(TUint)KOplMaxArgCount,Panic(EOpltTooManyArguments));
	iArgTypes[anArgument]=(TUint8)aType;
	}

TInt TOplSignature::operator==(const TOplSignature& aSignature) const
//
// Memberwise comparison
//
	{

	return (iReturnType==aSignature.iReturnType &&
	iNeedsCount==aSignature.iNeedsCount &&
	iArgumentCount==aSignature.iArgumentCount &&
	iPermittedArgumentCounts==aSignature.iPermittedArgumentCounts &&
	Mem::Compare(iArgTypes,KOplMaxArgCount,aSignature.iArgTypes,KOplMaxArgCount)==0);
	}

TOplSignature::TArgType TOplSignature::operator[](TInt anArg) const
//
// Returns the argument type of the passed argument number
//
	{
	__ASSERT_ALWAYS(anArg<KOplMaxArgCount && anArg>=0,Panic(EOpltIllegalArgIndex));
	return TArgType(iArgTypes[anArg]);
	}

TOplToken::TType COplParserBase::ArgumentByRefL()
//
// Parses an identifier that is going to be passed by reference.
// Puts out the left side and then the call to Addr
// ArgumentByRef := # WordExpression
//				  | LeftSideReference
//
	{
	
	TOplToken next=NextL();
	TOplToken::TType type=TOplToken::EBadType; // Returned to signal it's actually a #Reference 
	if (next==TOplToken::EHash)
		NativeExpressionL();
	else if (next.Class()!=TOplToken::EIdentifier || next==TOplToken::EConstant || next==TOplToken::ELabel)
		User::Leave(EErrFnArgument);
	else
		{
		PCodeL(TPcode::EArgList);
		type=Lexer().Type();
		LeftSideReferenceL(next,ESupplyIndices);
	
		PCodeArgumentL(TPcode::EFunctionArg,TOplSignature::EArgAnyRef);
		
		TOplFunction::TNumber addr=TOplFunction::EAddr;
		if (type==TOplToken::EString)
			addr=TOplFunction::ESAddr;
		
		PCodeL(TPcode::EFixedFunctionCall);
		iCode<<TUint8(1)<<NativeType()<<addr;
		}
	return type;
	}

void COplParserBase::ArgumentByRefL(TOplToken::TType argType)
//
// Parse a function argument that is an identifier passed by reference.
// Checks that the type of the identifier is as required
//
	{

	TOplToken::TType type=ArgumentByRefL();
	// EBadType indicates that it was actually a #expression reference,
	// Otherwise the type must be as expected
	if (type!=TOplToken::EBadType && type!=argType)
		User::Leave(EErrFnArgument);
	}

TBool COplParserBase::NextIsArgumentSeparatorL()
	{

	return (NextIsCommaL() || NextIsL(TOplToken::ESemiColon));
	}


void COplParserBase::PCodeArgumentL(TPcode::TCode anArgCode, TOplSignature::TArgType anArgType)
//
//
//
	{
	PCodeL(anArgCode);
	iCode<<anArgType;
	}

TUint COplParserBase::ExplicitArgumentListL(TPcode::TCode anArgCode,TOplSignature& aSignature, TOplFunction* aFunction)
//
// Parse an argument list to something callable.
//
	{
	
	PCodeL(TPcode::EArgList);
	TUint argCount=0;
	if (aSignature.IsStatistical()) // Stats functions like Min/Max 
		{ // Which can either take an ArrayReference & count, or a number of arguments
		MustBeL(TOplToken::EOpenBracket);
		
		TBool isArray=EFalse;
		Lexer().Mark(); // We're going to have a quick sniff and see which case we have
		if (NextIsL(TOplToken::EArray) && Lexer().Type()==TOplToken::EReal &&  NextIsL(TOplToken::ECloseBracket))
			isArray=ETrue;
		Lexer().UnGetToMark(); // OK put it all back.
	
		if (isArray)	// RealArrayId ) , WordExpression
			{
			LeftSideReferenceL(NextL(),ESupplyIndices);
			PCodeArgumentL(anArgCode,TOplSignature::EArgWordRef); // \\\???
			if (!NextIsArgumentSeparatorL())
				User::Leave(EErrFnArgument);
			WordExpressionL();
			PCodeArgumentL(anArgCode,TOplSignature::EArgWord);
			}
		else do  // RealExpressionL[,RealExpression]*
			{
			ExpressionL(TOplToken::EReal);
			PCodeArgumentL(anArgCode,TOplSignature::EArgReal);
			argCount++;
			} while (NextIsArgumentSeparatorL());
		MustBeL(TOplToken::ECloseBracket);
		}
	else // It's a regular Min/Max kind of argument list.
		{
		if (NextIsL(TOplToken::EOpenBracket)) // Appears to have a regular argument lisr
			{
			if (aFunction!=NULL)
				{
				if (aFunction->Number()==TOplFunction::EMPopup)
					aSignature.AllowArgumentCount(5);
				}
			do	// each argument
				{
				TOplSignature::TArgType argType=aSignature[argCount];
				switch (argType)
					{
					case TOplSignature::EArgWord:
					case TOplSignature::EArgLong:
					case TOplSignature::EArgReal:
					case TOplSignature::EArgString:
					case TOplSignature::EArgUword:
						ExpressionL(TOplToken::TType(argType-TOplSignature::EArgWord+TOplToken::EWord));
						break;
					case TOplSignature::EArgNative:
						NativeExpressionL();
						break;
					case TOplSignature::EArgUNative:
						ExpressionL(UnsignedNativeType());
						break;
					case TOplSignature::EArgWordRef:
					case TOplSignature::EArgLongRef:
					case TOplSignature::EArgRealRef:
					case TOplSignature::EArgStringRef:
						ArgumentByRefL(TOplToken::TType(argType-TOplSignature::EArgWordRef+TOplToken::EWord));
						break;
					case TOplSignature::EArgAnyRef:
						ArgumentByRefL();
						break;
					case TOplSignature::EArgOverLoad: // Special case where the argument type changes the function number
						__ASSERT_ALWAYS(aFunction!=NULL,Panic(EOpltUnknownFunctionOverload));
						switch (aFunction->Number())
							{
							case TOplFunction::EAddr:
								{
								TOplToken next=NextL();
								if (next.Class()!=TOplToken::EIdentifier)
									SyntaxErrorL();
								argType=TOplSignature::TArgType(TOplSignature::EArgWordRef+Lexer().Type()-TOplToken::EWord);
								if (argType==TOplSignature::EArgStringRef)
									aFunction->SetNumber(TOplFunction::ESAddr);
								LeftSideReferenceL(next,ESupplyIndices);
								}
								break;
							case TOplFunction::EIoOpen:
								{
								argType=TOplSignature::TArgType(ExpressionL());
								if (argType!=TOplSignature::EArgString)
									{
									aFunction->SetNumber(TOplFunction::EIoOpenX);
									PCodeCastL(TOplToken::TType(argType),NativeType());
									argType=TOplSignature::EArgNative;
									}
								}
								break;
							case TOplFunction::EgCreate:
								WordExpressionL();
								argType=TOplSignature::EArgWord;
								aFunction->SetNumber(TOplFunction::EgCreateGrey);
								aSignature.AllowArgumentCountByPass(argCount+1);
								break;
							case TOplFunction::ELoc:
								WordExpressionL();
								argType=TOplSignature::EArgWord;
								aFunction->SetNumber(TOplFunction::ELocX);
								aSignature.AllowArgumentCountByPass(argCount+1);
								break;
							case TOplFunction::EMenu:
								ArgumentByRefL(TOplToken::EWord);
								argType=TOplSignature::EArgWordRef;
								aFunction->SetNumber(TOplFunction::EMenuX);
								aSignature.AllowArgumentCountByPass(argCount+1);
								break;
							case TOplFunction::EMPopup: // mPopup(x%,y%,posType%,n1$,k1%[,n$,k%]+)
								do
									{
									StringExpressionL();
									PCodeArgumentL(anArgCode,TOplSignature::EArgString);
								    if (!NextIsArgumentSeparatorL())
										User::Leave(EErrFnArgument);
									WordExpressionL();
									PCodeArgumentL(anArgCode,TOplSignature::EArgWord);
									argCount+=2;				// count pairs
									} while (NextIsArgumentSeparatorL());
								argCount--;						// 1 added later
								aSignature.AllowArgumentCount(argCount+1);
								break;
							case TOplFunction::EgCreateBit: // gCreateBit (w%,h%) for OPL1993, gCreateBit(w%,h%[,mode%]) for opler1
								WordExpressionL();
								if (!TargetIsOpl1993()) // Variable arguments
									aSignature.AllowArgumentCount(3); // Can now have 2 or 3 arguments - hence count
								break;
							default:
								Panic(EOpltUnknownFunctionOverload);
							}
							break;
						default:
							User::Leave(EErrFnArgument);
						
					}
				PCodeArgumentL(anArgCode,argType);
				argCount++;
				} while (NextIsArgumentSeparatorL());
			MustBeL(TOplToken::ECloseBracket);
			}
		if (!aSignature.IsPermittedArgumentCount(argCount))
			User::Leave(EErrFnArgument);
		}
	return argCount;
	}

TUint COplParserBase::ImplicitArgumentListL(TPcode::TCode anArgCode,TOplSignature* aSignature)
//
// Parses an, as yet, undefined signature. A defining argument list
// I.e. the argument list for an implicitly declared procedure
//
	{

	PCodeL(TPcode::EArgList);
	TInt argCount=0;
	if (NextIsL(TOplToken::EOpenBracket))
		{
		do
			{
			TOplSignature::TArgType argType=TOplSignature::TArgType(ExpressionL());
			if (aSignature!=NULL)
				aSignature->AddArgument(argCount,argType);
			PCodeArgumentL(anArgCode,argType);
			argCount++;
			} while (NextIsArgumentSeparatorL());
		MustBeL(TOplToken::ECloseBracket);
		}
	if (aSignature!=NULL)
		aSignature->SetArgumentCount(argCount);
	return argCount;
	}

TOplToken::TType COplParserBase::CallL(TOplToken aToken)
//
// Don't put stuff on the stack unnecessarily as this can be horribly
// recursive.
// Something callable - procedure/function/OPX
// In all cases we have just lexed the first token.
// Call := CallId [ argument-list]
	{
	
	TOplToken::TType returnType=Lexer().Type();
	switch (aToken)
		{
		case TOplToken::EOpxFunction:
			{
			COplCallSymbol *pSym=Lexer().OpxSymbol();
			ExplicitArgumentListL(TPcode::EOpxArg,pSym->Signature(),NULL);
	
			PCodeL(TPcode::EOpxCall);
			iCode<<(COplSymbol *)pSym;
			break;
			}
		case TOplToken::EProcId:  // ProcId ProcArgumentList
			{
			ProcCallPreambleL(Lexer().Name(),returnType);

			COplSymbol *pSym=iSymbols->Find(Lexer().Name(),aToken);
			COplReferenceSymbol *pRef=NULL;
			if (pSym==NULL) // This is the first reference so it actually counts as the declaration
				{
				if (iExplicitDefines) // BUT banned from implicit definitions
					User::Leave(EErrUndefinedProcedure);
				pSym=NewSymbolL(COplSymbol::EProcDecl);

				// Since this is the first time, we use this to determine the signature
				TOplSignature& signature=((COplCallSymbol*)pSym)->Signature();
				signature.Initialize(pSym->Type());
				ImplicitArgumentListL(TPcode::EProcArg,&signature); // Get the signature for the symbol
				
				// Finally add the reference
				pRef=ReferenceL(*pSym); // And stick in the reference
				}
			else
				{
				pRef=(pSym->Class()==COplSymbol::EProcRef ? (COplReferenceSymbol *)pSym : ReferenceL(*pSym));
				ExplicitArgumentListL(TPcode::EProcArg,((COplCallSymbol&)pRef->Declaration()).Signature(),NULL);
				}

			ProcCallPostambleL(pRef);
			}
			break;
		case TOplToken::EProcByName: // ProcByName StringExression ) : ProcedureArgumentList
			{
			StringExpressionL();				// The procedure name 
			PCodeL(TPcode::EProcCallName);     // Marks the name on the stack for the Munger's benefit

			MustBeL(TOplToken::ECloseBracket);
			MustBeL(TOplToken::EColon);

			PCodeProcByNameL(returnType,ImplicitArgumentListL(TPcode::EProcArg));
			}
			break;
		case TOplToken::EFunction: // [(FunctionArgumentList|StatsArgumentList)]
			{
			// We allocate a copy of the function since it's only held temporarily
			// within the lexer & this is all horribly reursive
			TOplFunction *pF=(TOplFunction *)User::AllocLC(sizeof(TOplFunction));
			new(pF) TOplFunction(Lexer().Function());

			TUint argCount=ExplicitArgumentListL(TPcode::EFunctionArg,pF->Signature(),pF);

			PCodeL(pF->Signature().NeedsArgumentCount() ? TPcode::ECountedFunctionCall : TPcode::EFixedFunctionCall);
			if (returnType==TOplToken::ENativeReturn)
				returnType=NativeType();
			iCode<<TUint8(argCount)<<returnType<<pF->Number();

			CleanupStack::PopAndDestroy(); // Kill off function details
			break;
			}
		default:
			Panic(EOpltUnknownCallType);
		}
	return returnType;
	}

void COplParserBase::ProcCallPreambleL(const TDesC& /*aName*/,TOplToken::TType /* aType*/)
//
// Does nothing in the base
//
	{
	}

void COplParserBase::PCodeProcByNameL(TOplToken::TType aType,TUint anArgumentCount)
//
// Puts out the Pcode for calling a proc by name
//
	{
	PCodeL(TPcode::EProcByNameCall);
	iCode.WriteUint8L(aType);
	iCode.WriteUint8L(anArgumentCount);
	}

///////////////////////////////////////////////////////////////////
//
// COplModuleParser
//
///////////////////////////////////////////////////////////////////
void COplModuleParser::CallStatementL(TOplToken aToken)
//
// Can invoke something callable as a statement. Call it and drop the returned type.
//
	{

    TOplToken::TType returnType=CallL(aToken);
	PCodeL(TPcode::EDrop);
	iCode<<returnType;
	}


void COplModuleParser::ProcCallPostambleL(const COplReferenceSymbol* aSymbol)
//
// Puts out the Pcode for the procedure call
//
	{
	
	PCodeL(TPcode::EProcCall);
	iCode<<(COplSymbol*)aSymbol;
	}

/////////////////////////////////////////////////////////////////////
//
// COplEvalParser
//
/////////////////////////////////////////////////////////////////////
void COplEvalParser::ProcCallPreambleL(const TDesC& aName,TOplToken::TType aType)
//
// Evaluator converts fred%: into @%("Fred"):
//
	{

	// Drop teh trailing %, & or $ off the proc name
	TInt len=aName.Length();
	if (aType!=TOplToken::EReal)
		len-=1;
	TPtrC strippedName=aName.Left(len);
	
	// And put this out as a constant expression on the stack
	ConstantReferenceL(TOplConstant(strippedName));
	PCodeL(TPcode::EProcCallName);     // Marks the name on the stack for the Munger's benefit
	}

void COplEvalParser::ProcCallPostambleL(const COplReferenceSymbol* aSymbol)
//
//
//
	{
	
	COplCallSymbol& call=STATIC_CAST(COplCallSymbol&, aSymbol->Declaration());
	PCodeProcByNameL(call.Type(),call.Signature().ArgumentCount());
	}
