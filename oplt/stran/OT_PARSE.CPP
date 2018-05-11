// OT_PARSE.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

// Assorted classes for the OPL translator language parser
//
#include "ot_std.h"
#include "opl16.h"

//////////////////////////////////////////////
//
// COplParseOpx
//
//////////////////////////////////////////////
COplParseOpx *COplParseOpx::NewLC()
//
// Class for storing informatiomn from DECLARE OPX, END DECLARE 
// block.
//
   {
   COplParseOpx *pOpx=new(ELeave) COplParseOpx();
   CleanupStack::PushL(pOpx);
   return pOpx;
   }

COplParseOpx::~COplParseOpx()
//
// Do a bit of tidying up
//
   {
   delete iName;
   }

void COplParseOpx::SetNameL(const TDesC& aName)
//
// Just gets a copy of the name
//
	{
	__ASSERT_DEBUG(iName==NULL,Panic(EOpltParseOpxNameSet));
	iName=aName.AllocL();
	}

COplParseOpx::COplParseOpx()
	{
	}

//////////////////////////////////////////////////////
//
// COplParseProc - Internal model of a procedure
//
//////////////////////////////////////////////////////
COplProcHeader *COplProcHeader::NewLC(TOplToken::TType aType)
//
// 
//
   {

	COplProcHeader *pPh=new(ELeave) COplProcHeader(aType);
	CleanupStack::PushL(pPh);
	return pPh;
	}

void COplProcHeader::AddSymbolL(COplSymbol& aSymbol)
//
// A new symbol is being added to the symbol table.
// This gets to check to see if it wants to added it to a list
//

   {

   CArrayFixFlat<COplSymbol*>* list=NULL;
   switch (aSymbol.Class())
      {
	   case COplSymbol::EArgument:
         list=&iArguments;
         break;
	   case COplSymbol::EExternalRef:
         list=&iExternals;
         break;
	   case COplSymbol::EProcRef:
         list=&iProcCalls;
         break;
	   case COplSymbol::EGlobal:
         list=&iGlobals;
         break;
	   case COplSymbol::ELocal:
         list=&iLocals;
         break;
	   case COplSymbol::ELabel:
         list=&iLabels;
         break;
	   case COplSymbol::EConstant:
	   case COplSymbol::EExternalDecl:
	   case COplSymbol::EProcDecl:
	   case COplSymbol::EOpxFunction:
         break;
      }
   	if (list!=NULL)
		list->AppendL(&aSymbol);
	}

COplProcHeader::COplProcHeader(TOplToken::TType aType)
	:
	iType(aType),
	iArguments(KOplParseProcArrayGran),
	iExternals(KOplParseProcArrayGran),
	iGlobals(KOplParseProcArrayGran),
	iLocals(KOplParseProcArrayGran),
	iProcCalls(KOplParseProcArrayGran),
	iLabels(KOplParseProcArrayGran)
//
// 
//
	{
	}

////////////////////////////////////////////////////////
//
// COplParserBase
//
////////////////////////////////////////////////////////
void COplParserBase::SetTarget(TOplTranTarget aTarget)
//
//
//
	{
	CheckReset();
	iTarget=aTarget;
	}

void COplParserBase::Start(COplLexerBase& aLexer,TTranslateError& anError)
//
//
//
   {

   iLexer=&aLexer;
   iError=&anError;
   }

void COplParserBase::Reset()
//
//
//
	{

	iExplicitDefines=EFalse;	
	if (iSymbols)
		iSymbols->Reset();
	}

COplDeclarationSymbol* COplParserBase::NewSymbolL(COplSymbol::TClass aClass)
//
// Generates and adds a new symbol from the currently lexed token
//
	{
	
	COplDeclarationSymbol *pDecl=COplDeclarationSymbol::NewLC(aClass,Lexer().Token(),Lexer().Type(),Lexer().Name());
	AddSymbolL(*pDecl);
	CleanupStack::Pop(); // Safe now
	return pDecl;
	}


void COplParserBase::AddSymbolL(COplSymbol& aSymbol)
//
// Just Sticks it in the symbol table
//
	{
	
	iSymbols->AddL(aSymbol);
	}


COplReferenceSymbol* COplParserBase::ReferenceL(COplSymbol& aSymbol)
//
// Generates a refernce to the declaration symbol we've been passed
//
	{
	COplReferenceSymbol *pRef=((COplDeclarationSymbol&)aSymbol).NewReferenceLC();
	AddSymbolL(*pRef);
	CleanupStack::Pop();
	return pRef;
	}

void COplParserBase::PCodeL(TPcode::TCode aCode)
//
// Puts out the PCODE assuming the current offset
//
	{
	iCode<<TPcode(aCode,iError->Position()); // Where an error would be reported now
	}

void COplParserBase::MustBeL(TOplToken aToken)
//
// Checks that the next token is what is expected
// 
	{

	Lexer().LexL(aToken);
	}

TOplToken COplParserBase::NextL()
//
//
//
	{
	return Lexer().LexL();
	}

TBool COplParserBase::NextIsL(TOplToken aToken)
//
// If the next token is the desired one, returns ETrue having lexed it
// Otherwise returns EFalse
//
   {

   TBool ret=ETrue;
   if (NextL()!=aToken)
      {
      ret=EFalse;
      Lexer().UnLex();
      }
   return ret;
   }

TBool COplParserBase::NextIsCommaL()
//
// Checks for a comma
//
	{
	
	return NextIsL(TOplToken::EComma);
	}

void COplParserBase::CommaL()
//
//
//
	{

	if (!NextIsL(TOplToken::EComma))
		User::Leave(EErrMissingComma);
	}

TBool COplParserBase::TargetIsOpl1993() const
	{
	return iTarget & (KOplTargetOpl1993);
	}

TOplToken::TType COplParserBase::NativeType() const
//
//
//
	{
	return (TargetIsOpl1993() ? TOplToken::EWord : TOplToken::ELong);
	}

TOplToken::TType COplParserBase::UnsignedNativeType() const 
//
// This is a horrible kludge since there is currently no support
// for a cast to unsigned longs in OPL so we just cast 32-bit
// unsigned arguments to 32-bit signed values. This has death lurking in it
// but time is short
//
	{
	return (TargetIsOpl1993() ? TOplToken::EUWord : TOplToken::ELong);
	}

COplParserBase::COplParserBase()
	: iTarget(KOplDefaultTarget), iOperatorStack(KOperatorStackGran)
	{
	}

void COplParserBase::ConstructL()
//
// Constructs the symbol table
//
   {
   
   iSymbols=COplSymbolTable::NewL();
   }

void COplParserBase::CheckReset()
//
// Base parser does not have a reset state
//
	{
	}

COplParserBase::~COplParserBase()
//
// Deletes the symbol table
//
   {

   delete iSymbols;
   }

////////////////////////////////////////////////////////
//
// COplModuleParser
//
///////////////////////////////////////////////////////////
COplModuleParser *COplModuleParser::NewL()
//
// Creates a new module parser
//
	{
	COplModuleParser *pP=new(ELeave) COplModuleParser;
	CleanupStack::PushL(pP);
	pP->ConstructL();
	CleanupStack::Pop();
	return pP;
	}

void COplModuleParser::Start(COplModuleLexer& aLexer,TSourceTranslateError& anError, COplModuleBackEnd& aBackEnd)
//
// Sets lexer & back end and sets up the code stream
//
   {
   CheckReset(); // Will panic if we're not
   COplParserBase::Start(aLexer,anError);
   iBackEnd=&aBackEnd;
   iState=EStarted;
   aLexer.SetSymbolTable(iSymbols);
   iLocatingError=EFalse;
   }

void COplModuleParser::SetErrorLocation(TInt aLineNumber, TInt aQcodeOffset)
//
// Sets things up to find a run-time error in the source
//	
	{

	iLineNumber=aLineNumber;
	iQcodeOffset=aQcodeOffset;
	iLocatingError=ETrue;
	}


void COplModuleParser::Reset()
//
//
//
	{
	
	iBackEnd=NULL;
	COplParserBase::Reset();
	iState=EReset;
	}


void COplModuleParser::ParseChunkL()
//
// Does the next chunk of the module
// Module := [AppDefinition] [Proc|OpxDEclaration|ConstDeclaration|Declare External]+ EndOfFile
//
// A chunk is either an APP ENDA construct or a Procedure
// (By now we know that we're not in the EReset state)
	{

	__ASSERT_ALWAYS(iState!=EReset,Panic(EOpltParserReset));
	TInt declCount=KOplDeclarationsPerChunk;
	TBool doMore;
	do
		{
		doMore=EFalse;
		TOplToken peek=SkipBlankLinesL();
		switch (peek)
			{
			case TOplToken::EApp:	
				if (iState!=EStarted) // Can only get App right at the start
					SyntaxErrorL();
				AppL();
				iState=EAppSeen;
				break;
			case TOplToken::EDeclare: // DECLARE EXTERNAL or DECLARE OPX
				if (NextIsL(TOplToken::EExternal))
					{
					EosL();
					iExplicitDefines=ETrue;
					doMore=--declCount;
					break;
					}
				else // Better be DECLARE OPX
					OpxDeclareBlockL();
				break;
			case TOplToken::EExternal: // EXTERNAL procname:[(arglist)]
				ProcedureDeclarationL(COplSymbol::EProcDecl,EDeclaration);
				doMore=--declCount;
				break;
			case TOplToken::EProc:
				{
				if (!Lexer().InRootSource()) // Can only declare prcedures in the 'root' sourcce file
					User::Leave(EErrIncludeProcedure);
				ProcL();
				iState=EProcSeen;
				break;
				}
			case TOplToken::EInclude:
				{
				MustBeL(TOplToken::EConstant);
				if (Lexer().Type()!=TOplToken::EString)
					SyntaxErrorL();
				MustBeL(TOplToken::EEos);
				Lexer().IncludeL(Lexer().Name());
				doMore=ETrue;
				break;
				}
			case TOplToken::EConst: // Will never occur when doing OPL1993 stuff
				{
				ConstantDeclarationL();
				doMore=--declCount;
				break;
				}
			case TOplToken::EEof:
				if (iState==EProcSeen) // Must have at least one procedure
					{
					iBackEnd->EndOfFileL();
					User::Leave(EErrSuccess); // All worked out nicely
					}
			default:		// NB: Drop through from TOplToken::EEof is deliberate
				SyntaxErrorL();
			}
		} while (doMore);
	}

void COplModuleParser::CheckReset()
//
// ASSERTS That we're in teh Reset state
//
	{
	__ASSERT_ALWAYS(iState==EReset,Panic(EOpltParserNotReset));
	}

COplModuleParser::COplModuleParser()
//
//
//
	{
	}

void COplModuleParser::AddSymbolL(COplSymbol& aSymbol)
//
//
//
	{
	// hp: changed the order of these to avoid symbol being left queued on OOM
	if (iProcHeader!=NULL) // Parsing a procedure at the moment
		iProcHeader->AddSymbolL(aSymbol);
	COplParserBase::AddSymbolL(aSymbol);
	}



void COplModuleParser::AppL()
//
// Deals with parsing the APP/ENDA thing. Delegates this to
// a class created by the back end, following the rationale
// that APP/ENDA is target specific in a way that the rest of the
// language isn't
//
	{
	COplParseApp *app=iBackEnd->GetAppLC(iTarget,Lexer());
	if (!iLocatingError)
		STATIC_CAST(COpl16ModuleBackEnd*,iBackEnd)->OplTargetIsApp();		// backend must tell texted
															// now before doing ParseL() because
															// ICON keyword needs full target path
	app->ParseL(*STATIC_CAST(COpl16ModuleBackEnd*,iBackEnd),iLocatingError);
	iBackEnd->AppL(app); // Takes ownership of the App object if it succeeds
	CleanupStack::Pop();
	}

COplSymbol& COplModuleParser::ProcedureDeclarationL(COplSymbol::TClass aClass,TDeclarationFlag aFlag)
//
// Parses a proc or opx function declaration
// proc-name[( [arg-decl [, arg-decl]*)]
// Constructs the signature of the function/procedure
//
	{

		
	//proc-name
	MustBeL(TOplToken::EProcId);
	HBufC *name=Lexer().Name().AllocLC();

	// Record the signature - the return type is the type of the proc-name symbol
	TOplSignature signature;
	signature.Initialize(Lexer().Type());

	// [ ( [arg-decl [,arg-decl]*] ) ]
	TInt argCount=0;
	if (NextIsL(TOplToken::EOpenBracket))  // OPL HA-26 bug fix: allow no brackets in declaration
		{ // ( [arg-decl [,arg-decl]*] )
		if (!NextIsL(TOplToken::ECloseBracket))
			{ // arg-decl [,arg-decl] *
			do 
				{ // arg-decl := [BYREF] simple-id
				TOplSignature::TArgType typeBase=TOplSignature::EArgWord; // [BYREF]
				TBool isByRef=NextIsL(TOplToken::EByRef);
				if (isByRef)
					typeBase=TOplSignature::EArgWordRef;
				MustBeL(TOplToken::ESimple); // simple-id
				if (isByRef && Lexer().Type()==TOplToken::EString)
					SyntaxErrorL();
		
				signature.AddArgument(argCount,TOplSignature::TArgType(typeBase+Lexer().Type()-TOplToken::EWord));
				argCount++;
				} while (NextIsL(TOplToken::EComma));
			MustBeL(TOplToken::ECloseBracket);
			}
		}
	signature.SetArgumentCount(argCount);
	
	// If this is a definition of a PROC, them there is allowed to be an existing
	// declaration _PROVIDED_ that it has the same signature
	if (aFlag==EDefinition)
		{
		COplSymbol* declaration=iSymbols->Find(*name,TOplToken::EProcId);
		if (declaration) // There's an existing symbol
			{
			if (declaration->Class()!=COplSymbol::EProcDecl) // But it's not a declaration
				User::Leave(EErrDuplicateName);
			// Signatures must match
			if (!(signature==STATIC_CAST(COplCallSymbol *,declaration)->Signature()))
				User::Leave(EErrIncompatibleDeclaration);
			iSymbols->Delete(declaration);
			}
		}
	
	// Now get the symbol and set the signature
	COplCallSymbol *symbol=STATIC_CAST(COplCallSymbol *,COplDeclarationSymbol::NewLC(aClass,TOplToken::EProcId,signature.ReturnType(),*name));
	symbol->Signature()=signature;
	AddSymbolL(*symbol);
	CleanupStack::Pop(); // Safe now
	
	CleanupStack::PopAndDestroy();  // function name
	return *symbol;
	}

void COplModuleParser::ConstantDeclarationL()
//
// CONST simple-id = [-] constant EOS
//
// Already seen the leading CONST
//
	{

	MustBeL(TOplToken::ESimple);	// simpleId
	
	COplConstantSymbol &symbol=*(COplConstantSymbol *)NewSymbolL(COplSymbol::EConstant);
	
	MustBeL(TOplToken::EEqual);		// '='
	
	TBool negate=EFalse;			// [-]
	if (NextIsL(TOplToken::EMinus))
		negate=ETrue;

	MustBeL(TOplToken::EConstant);	// Constant
	
	// Deliberately don't try and assign the TOplConstant directly.
	// as the assignments contain any casts between what was declared for the vrable type
	// and what was actually given forthe initializer
	TOplConstant& constant=Lexer().Constant();
	if (negate)
		constant.NegateL();

	switch (symbol.Type())
		{
		case TOplToken::EWord:
			symbol.Value()=constant.AsWordL();
			break;
		case TOplToken::ELong:
			symbol.Value()=constant.AsLongL();
			break;
		case TOplToken::EReal:
			symbol.Value()=constant.AsRealL();
			break;
		case TOplToken::EString:
			symbol.Value()=*constant.AsStringL().AllocL();
		default: // To keep GCC happy - but we know better 
			break;
		}
	EosL();
	}



void COplModuleParser::OpxDeclareBlockL()
//
// declare-block := DECLARE OPX <name>,<version> EOS
//						declare-function-list
//					END DECLARE
//
// functionList:= [procId [( [arg-decl[,arg-decl]*] )] : function-number Eos]* 
// Fix OPL Ha-26 allow no arguments in declare
// Fix: remove alias and add OPX function number so linkage is by
//		ordinal.
//
// arg-decl := [BYREF] simple-id
//
// We've already seen the leading DECLARE.
//
	{

	TInt opxNumber=iBackEnd->OpxCount();
	if (opxNumber==(TInt)KOplMaxOpxCount)
		User::Leave(EErrTooManyOPXs);
	

	// DECLARE OPX <name>, <uid>, <version> EOS
		{
		COplParseOpx* opx=COplParseOpx::NewLC();
		MustBeL(TOplToken::EOpx);
		MustBeL(TOplToken::ESimple);   // <name>
		if (Lexer().Type()!=TOplToken::EReal)
			SyntaxErrorL();
		opx->SetNameL(Lexer().Name());
		CommaL();
		MustBeL(TOplToken::EConstant); //  <uid>
		opx->SetUid(TUid::Uid(Lexer().Constant().AsLongL()));
		CommaL();					   // comma
		MustBeL(TOplToken::EConstant); //  <version>
		opx->SetVersion(Lexer().Constant().AsWordL());
		EosL();
		iBackEnd->OpxL(opx); // This takes ownership of the object IF it succeeds
		CleanupStack::Pop(); // OPX is safe or destroyed
		}

	// functionList:= [procId ( [arg-decl [,arg-decl]*] ) : function-number Eos]*
	while (SkipBlankLinesL()==TOplToken::EProcId)
		{
		Lexer().UnLex();

		COplSymbol& symbol=ProcedureDeclarationL(COplSymbol::EOpxFunction,EDeclaration); // procId ( [arg-decl [,arg-decl]*] )
		MustBeL(TOplToken::EColon);  // : 
		MustBeL(TOplToken::EConstant); //  function-number
		symbol.SetAddress(TOpxAddress(opxNumber,Lexer().Constant().AsWordL()));

		// EOS
		EosL();
		}
	Lexer().UnLex();

	// END DECLARE EOS
	MustBeL(TOplToken::EEnd);
	MustBeL(TOplToken::EDeclare);
	EosL();
	}

void COplModuleParser::ProcL()
//
// PROC proc-id [(decl-list)] EOS
//	auto-decl-lists
//	statement-list
// ENDP
//
// auto-decl-lists := [GLOBAL | LOCAL decl-list EOS]*
//
// NOTE _ We've already had the PROC
//
	{
	
	// If we're trying to find a run-time error
	TInt errorOffset=KOplNoErrorLocation;
	if (iLocatingError)
		{
		if (iLineNumber!=Lexer().LineNumber()) // This is not the one we're looking for
			{
			// So just skip through to the ENDP
			TOplToken next;
			FOREVER
				{
				TRAPD(ret, next=SkipToEndL());
				if (ret==KErrNone)
					break;
				// Got an error skipping through the tokens. Assuming that 
				// the runtime is asking us to find an error in a legitimate function
				// this can only happen because the lexer doesn't understand the character after
				// %. So we gobble the next character and press on.
				TRAP(ret,Lexer().NextChar());
				}
			if (next==TOplToken::EEof)
				User::Leave(EErrUnexpectedEof);
			return;
			}
		// This is the one that we're looking for.
		errorOffset=iQcodeOffset;
		}


	// ... proc-id  [(decl-list)] EOS
	// Now we parse the name and argument list once to get the declaration
	// i.e. record and check the signature
	Lexer().Mark();
	ProcedureDeclarationL(COplSymbol::EProcDecl,EDefinition);
	
	// We do it again, primarily to get the symbols for the arguments
	// into the symbol table and within the right scope.
	// It's a bit of a kludge but it's the safest fix
	Lexer().UnGetToMark();
	MustBeL(TOplToken::EProcId); // proc-id
	iBackEnd->StartProcL(Lexer().Name(),Lexer().LineNumber(),errorOffset); // Checks for duplicate definitions
	
	// Various house-keeping things.
	iProcHeader=COplProcHeader::NewLC(Lexer().Type());
	CBufSeg *codeBuf=CBufSeg::NewL(KOplPcodeGran);
	CleanupStack::PushL(codeBuf);
	iCode.Insert(*codeBuf,0); // Open it as an insert stream so we can come back an insert stuff at a later time
	Mem::FillZ(&iBranches,sizeof(TBranches));

	// Variables declared within the PROC..ENDP are scoped within PROC ENDP.
	iSymbols->StartScope();

	// [ ( arg-decl-list) ] EOS
	if (NextIsL(TOplToken::EOpenBracket))
		{
		DeclarationListL(COplSymbol::EArgument);
		MustBeL(TOplToken::ECloseBracket);
		}
	EosL();

	// auto-decl-lists
	FOREVER
		{
		TOplToken next=SkipBlankLinesL();
		COplSymbol::TClass declClass=COplSymbol::EConstant; // Used to indicate that it is not a declation
		switch (next)
			{
			case TOplToken::ELocal:
				declClass=COplSymbol::ELocal;
				break;
			case TOplToken::EGlobal:
				declClass=COplSymbol::EGlobal;
				break;
			case TOplToken::EExternal:
				declClass=COplSymbol::EExternalDecl;
			default:
				break;
			}
		if (declClass==COplSymbol::EConstant)
			{
			Lexer().UnLex();
			break;
			}
		DeclarationListL(declClass);
		EosL();
		}

	// statement-list
	// ENDP
	if (StatementListL()!=TOplToken::EEndP)
		SyntaxErrorL();
	
	// Procedures always return a value
	if (!iLastWasReturn)
		PCodeTypedCommandL(TOplKeyword::EReturnNull,iProcHeader->Type());
		
	// Pass it all through to the back end - which take ownership of the objects
	iBackEnd->ProcL(iProcHeader,codeBuf);
	CleanupStack::Pop(2); // Code buf & Procedure Header
	iProcHeader=NULL;
	iSymbols->EndScope();
	}

TInt COplModuleParser::DeclarationIndexL()
//
// Deals with an index for an array or string
//
	{
	
	if (!NextIsL(TOplToken::EConstant))
		User::Leave(EErrBadArraySize);
	TInt size=0;
	TRAPD(ret,size=Lexer().Constant().AsWordL());
	if (ret!=KErrNone || size<=0)
		User::Leave(EErrBadArraySize);	
	return size;		
	}

void COplModuleParser::DeclarationListL(COplSymbol::TClass aClass)
//
// decl-list := decl [,decl]*
//
//
	{
	
	TBool isAutomatic=(aClass==COplSymbol::ELocal || aClass==COplSymbol::EGlobal);
	do
		{
		TOplToken id=NextL();
		if (id!=TOplToken::ESimple && id!=TOplToken::EArray)
			User::Leave(EErrBadDeclaration);

		COplSymbol* pSymbol=NewSymbolL(aClass);

		if (id==TOplToken::ESimple)
			{
			// Strings automatics  must have a length (so look like arrays)
			if (pSymbol->Type()==TOplToken::EString && isAutomatic)
				User::Leave(EErrBadDeclaration);
			}
		else // It's an array - or at least a String declaration
			{
			if (!isAutomatic) // External or Argument
				{
				// Arguments can't be arrays
				if (aClass==COplSymbol::EArgument) 
					User::Leave(EErrBadDeclaration);
				}
			else // 'Proper' automatic declaration - need to get array size
				{
				COplAutomaticSymbol *pAuto=STATIC_CAST(COplAutomaticSymbol *,pSymbol);
				TInt arraySize=DeclarationIndexL();
				if (pAuto->Type()==TOplToken::EString)
					{
					TInt stringLength=0;
					if (NextIsL(TOplToken::EComma)) // It's a string array
						stringLength=DeclarationIndexL();
					else // Actually just a simpl string declaration, rather than an array
						{
						stringLength=arraySize;
						arraySize=0;
						pAuto->SetToken(TOplToken(TOplToken::ESimple));
						}
					if (stringLength>KOplMaxStringLength)
						User::Leave(EErrBadStringLength);
					pAuto->SetStringLen(stringLength);
					}
				pAuto->SetArraySize(arraySize);
				}
			if (!NextIsL(TOplToken::ECloseBracket))
				User::Leave(EErrBadDeclaration);
			}
		} while (NextIsL(TOplToken::EComma));
	}

TOplToken COplModuleParser::StatementListL()
//
//
//
	{
	TOplToken next;
	iLastWasReturn=EFalse; // For empty lists
	FOREVER
		{
		FOREVER
			{
			next=NextL();
			if (next!=TOplToken::ELabel)
				break;
			DefineLabelL(Lexer().Name());
			}

		if (next.Class()==TOplToken::EReserved) // A structural word
			break;
		
		iLastWasReturn=EFalse;		// Assume next statement is not a return.
		PCodeL(TPcode::EStatement); // IN debug mode this will put out the statement code for the debugger
		switch (next.Class())
			{
			case TOplToken::EOperator:
				SyntaxErrorL();
			case TOplToken::EPunctuation:
				if (next==TOplToken::EEof)
					User::Leave(EErrUnexpectedEof);
				if (!IsEos(next))
					SyntaxErrorL();
				continue;
			case TOplToken::EIdentifier:
				IdentifierStatementL(next);
				break;
			case TOplToken::EKeyword:
				KeywordStatementL(); // Will set iLastWasReturn appropriately
				break;
			case TOplToken::ECall:
				CallStatementL(next);
				break;
			case TOplToken::EStructure:
				StructureL(next);
				iLastWasReturn=EFalse; // Otherwise, can get fooled by while 0 : if 0 : return endif endwh endp
			case TOplToken::EReserved: // Here solely to stop the compiler winging about stuff 
				break;
			}
		EosL();
		}
	return next;	
	}

TOplToken COplModuleParser::SkipBlankLinesL()
//
// Skips to the next token that isn't Eos
//
	{

	TOplToken next;
	do
		{
		next=Lexer().LexL();
		} while (IsEos(next));
	return next;
	}

TBool COplModuleParser::IsEos(TOplToken aToken)
//
// Deals with the fact that both TOPlToken::EEos && TOplToken::EColon can be EEos
// within the Module Parser
//
	{
	return (aToken==TOplToken::EEos || aToken==TOplToken::EColon);
	}

void COplModuleParser::EosL()
//
// Next thing must be the end of a statement
//
	{
	TOplToken next=NextL();

	if (!IsEos(next))
		{
		if (next==TOplToken::EOpenBracket || next==TOplToken::ECloseBracket)
			User::Leave(EErrMismatchedBracket);
		SyntaxErrorL();
		}
	}

TBool COplModuleParser::TestEosL()
//
// Peeks (lex & unlex) and checks if it is Eos
//
	{
	TOplToken next=NextL();
	Lexer().UnLex();
	return IsEos(next);
	}

TBool COplModuleParser::NextIsOffL()
//
// Checks to see if the next thing is off
//
	{

	TBool ret=EFalse;
	if (NextIsL(TOplToken::EKeywordToken))
		{
		if (Lexer().Keyword().Code()!=TOplKeyword::EOff)
			Lexer().UnLex();
		else
			ret=ETrue;
		}
	return ret;
	}

TBool COplModuleParser::NextIsOffOrOnL(TInt32& aQualifier)
//
// If OFF returns TRUE
// ELSE
//	 qualifier is incremented
//   IF ON returns TRUE
//   ELSE returns False
//
//
	{
	
	TBool ret=ETrue;
	if (!NextIsOffL())
		{
		aQualifier++;
		ret=NextIsL(TOplToken::EOn);
		}
	return ret;
	}

TOplToken COplModuleParser::SkipToEndL()
//
// Skips to EndP or Eof
//
	{
	TOplToken next;
	do
		{
		next=NextL();
		} while (next!=TOplToken::EEndP && next!=TOplToken::EEof);
	return next;
	}


/////////////////////////////////////////////////////////////////////
//
// COplEvalParser
//
/////////////////////////////////////////////////////////////////////
COplEvalParser *COplEvalParser::NewL()
//
// Creates a new eval parser
//
	{
	COplEvalParser *pP=new(ELeave) COplEvalParser;
	CleanupStack::PushL(pP);
	pP->ConstructL();
	CleanupStack::Pop();
	return pP;
	}

void COplEvalParser::ParseExpressionL(COplLineLexer& aLexer, TTranslateError& anError, COpl16EvalBackEnd& aBackEnd)
//
// Parses an expression
//
	{
	// Get a Pcode Buffer
	CBufSeg *codeBuf=CBufSeg::NewL(KOplPcodeGran);
	CleanupStack::PushL(codeBuf);
	iCode.Insert(*codeBuf,0);
	
	// Set up the lexer
	Start(aLexer,anError);
	
	// Parse the expression
	ExpressionL(TOplToken::EReal); // Do a real expression
	PCodeCommandL(TOplKeyword::EReturnFromEval);

	// And convert it into Qcode with the appropriate references
	aBackEnd.ProcessExpressionL(*codeBuf);

	CleanupStack::PopAndDestroy(); // PCode buffer and do a reset
	}