// OT_PARSE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Classes comprising the OPL language parser & it's various interfaces

#ifndef __OT_PARSE_H__
#define __OT_PARSE_H__

#include <s32mem.h>
#include <opltoken.h>

class COplModuleLexer;
class COplModuleBackEnd;
class COplSymbol;
class COplAutomaticSymbol;
class COplCallSymbol;
class COplLabelSymbol;

/////////////////////////////////////////////////////////////////////
//
// TOplOpStackItem - Item in the ioperator stack used in the expression evaluator
//
///////////////////////////////////////////////////////////////////////

const TUint KOperatorStackGran=16; // at 16 bytes per item => approx 256 byte alloc cells
class TStackedOperator
	{
public:
	inline TStackedOperator();
	inline TStackedOperator(TOplToken anOp,TOplToken::TType aType,TStreamPos aPos,TUint anOffset);
	inline void SetOperator(TOplToken anOp);
	inline TOplToken Operator();	
	inline operator TOplToken::TValue();
	inline void SetType(TOplToken::TType aType);
	inline TOplToken::TType Type();
	inline TStreamPos PcodePos();
	inline TUint Offset();
private:
	TOplToken iOperator; // The operator
	TOplToken::TType iType; // The type of the expression on it's left
	TStreamPos iPcodePos; // Where the expression on the left is in the Pcode (so can patch casts).
	TUint iOffset;  // Offset where the operator occured in the source
	};


/////////////////////////////////////////////////
//
// COplParserBase  - Root expression evaluation parser
//
/////////////////////////////////////////////////
const TUint KOplPcodeGran=0x100;
class TEvaluatorState;
class COplParserBase : public CBase
   {
public:
   ~COplParserBase();
	void SetTarget(TOplTranTarget aTarget);
	inline TOplTranTarget Target() const;
protected:
	void Start(COplLexerBase& aLexer,TTranslateError& anError);
	void Reset();
	virtual void AddSymbolL(COplSymbol& aSymbol);
	COplDeclarationSymbol *NewSymbolL(COplSymbol::TClass aClass); // Takes the current leer token and adds an appropriate symbol
	COplReferenceSymbol *ReferenceL(COplSymbol& aSymbol);

	// Expression evaluation
	TOplToken::TType ExpressionL();
	void StackOperatorL(TEvaluatorState& aState,TOplToken anOperator,TOplToken::TType aType);
	void FindOperatorL(TEvaluatorState& aState);
	TBool ActOnOperatorL(TEvaluatorState& aState);
	TStreamPos CodeSizeL(); // Gets the size of the code so far
	TOplToken::TType PromoteL(TOplToken::TType from,TOplToken::TType to,TStreamPos qcodeOffset);
	void OutputOperatorL(TEvaluatorState& aState);
	void ExpressionL(TOplToken::TType);
	void WordExpressionL();
	void LongExpressionL();
	void StringExpressionL(); // An expression evaluating to a string
	void NativeExpressionL(); // An expression evaluating to the native type (Word or Long).

	// Operands & Functions
	TOplToken::TType CallL(TOplToken aCallToken);
	TBool NextIsArgumentSeparatorL();
	TUint ExplicitArgumentListL(TPcode::TCode anArgsCode,TOplSignature& aSignature, TOplFunction* aFunction=NULL);
	TUint ImplicitArgumentListL(TPcode::TCode anArgsCode,TOplSignature* aSignature=NULL);
	TOplToken::TType ArgumentByRefL();
	void ArgumentByRefL(TOplToken::TType);
	virtual void ProcCallPreambleL(const TDesC& aName,TOplToken::TType aType);
	virtual void ProcCallPostambleL(const COplReferenceSymbol *pRef)=0;

	// General identifier references
	enum TIndexFlag
		{
		ESupplyIndices,
		EForceIndices
		};
	void IdentifierReferenceL(TOplToken aToken, TIndexFlag aFlag, TPcode::TIdentifierSide aSide);
	void LeftSideReferenceL(TOplToken aToken, TIndexFlag aFlag);
	void RightSideReferenceL(TOplToken aToken);
		
	// Specific types of identifiers.
	void SimpleOrArrayRefL(TOplToken aToken, TIndexFlag aFlag, TPcode::TIdentifierSide aSide);
	virtual TPcode::TCode IdentifierReferencePcode() const =0; // Do a different thing for evaluator & module parser
	virtual void FieldReferenceL(TPcode::TIdentifierSide aSide)=0;
	void ConstantReferenceL(const TOplConstant& aConstant);

	// Code emission stuff
	void PCodeL(TPcode::TCode aCode);
	//	void PCodeOffsetL(TPcode::TCode aCode,TUint anOffset);
	void PCodeCommandL(TOplKeyword::TCode aKeyword); 
	void PCodeCastL(TOplToken::TType aFromType, TOplToken::TType aToType);
	void PCodeOperatorL(TOplToken::TValue anOperator,TPcodeOperator::TArity anAriness,TOplToken::TType anOperandType, TOplToken::TType aResultingType);
	void PCodeArgumentL(TPcode::TCode, TOplSignature::TArgType anArgType);
	void PCodeProcByNameL(TOplToken::TType aType, TUint anArgumentCount);

	// Useful bits & bobs.
	inline COplLexerBase& Lexer();
	TOplToken NextL();
	TBool NextIsL(TOplToken aToken);
	TBool NextIsCommaL();
	void MustBeL(TOplToken aToken);
	void CommaL();
	TBool TargetIsOpl1993() const;	
	TOplToken::TType NativeType() const;
	TOplToken::TType UnsignedNativeType() const;

	virtual void CheckReset();
protected:
	COplParserBase();
	void ConstructL();
protected:
	// State
	TBool iExplicitDefines;
	TOplTranTarget iTarget;
	
	// References
	COplLexerBase *iLexer;
	TTranslateError *iError;
	
	// Owned
	CTranStackFlat<TStackedOperator> iOperatorStack;
	RBufWriteStream iCode;
	COplSymbolTable *iSymbols;
	};


////////////////////////////////////////////////////////////////////////
//
// TBranches - Used to keep track of where we are in the structural nesting
// In OT_STRUCT.CPP
///////////////////////////////////////////////////////////////////////
const TUint KOplMaxStructureDepth=8;
class TBranches // Used to keep track of branches for structure stuff
	{
	friend class COplModuleParser;
private:
	enum TBranch
		{
		EFalse=0,
		EBreak,
		EContinue,
		EExit,
		EMaxBranches
		};
	inline TBool InLoop();
	inline void EnterLoop();
	inline void LeaveLoop();
	TUint Branch(TBranch aBranch);
	TUint NewBranch(TBranch aBranch);
	void SaveBranchesL(TUint anArray[EMaxBranches]);
	void RestoreBranches(TUint anArray[EMaxBranches]);
private:
	TBool iLoop;
	TUint iDepth;
	TUint iNextBranch;
	TUint iBranches[EMaxBranches];
	};


//////////////////////////////////////////////////////////////////
//
// COplModuleParser
//
//////////////////////////////////////////////////////////////////
const TInt KOplDeclarationsPerChunk=20;

class COplModuleParser : public COplParserBase /*, public MOplLexerLineCB */
	{
public:
	static COplModuleParser* NewL();
	void Start(COplModuleLexer& aLexer, TSourceTranslateError& anError,COplModuleBackEnd& aBackEnd);
	void SetErrorLocation(TInt aLineNumber, TInt aQcodeOffset);
	void ParseChunkL();
	void Reset();
private:
	// Structural stuff
	enum TState
		{
		EReset=0,	// Reset state. Re-enter this state after completion, and error or Cancel()
		EStarted,	// StartTranslate has been called, awaiting TranslateChunkL
		EAppSeen,	// Seen APP ... ENDA construct
		EProcSeen	// Seen PROC ... ENDP
		};
	void CheckReset();
	COplModuleParser();
	void AddSymbolL(COplSymbol& aSymbol);
	inline COplModuleLexer& Lexer();
	
	// Language Structure
	void AppL();
	void ConstantDeclarationL();

	enum TDeclarationFlag // Is it pure declaration or is it from a definition
		{
		EDeclaration,
		EDefinition
		};
	COplSymbol& ProcedureDeclarationL(COplSymbol::TClass aClass, TDeclarationFlag);
	void OpxDeclareBlockL();
	void ProcL();
	TInt DeclarationIndexL();
	void DeclarationListL(COplSymbol::TClass aClass);
	TOplToken StatementListL();

	// Statement Types
	void IdentifierStatementL(TOplToken anIdent);
	void KeywordStatementL();
	void CallStatementL(TOplToken aCall);
	void StructureL(TOplToken aStructure);

	// Labels, Branches & Conditionals
	COplSymbol *LabelSymbolL(const TDesC& aLabel);
	void DefineLabelL(const TDesC& aLabel);
	void LabelReferenceL(TOplToken aToken);
	void LabelReferenceL();
	void NeedBranch(TBranches::TBranch aBranch); // A structure:DO WHILE or IF may need to use a branch.
	void BranchName(TBranches::TBranch aBranch,TDes& aLabel);
	void SetBranchL(TBranches::TBranch aBranch);
	void DoBranchL(TBranches::TBranch aBranch,TPcode::TCode aBranchCode);
	void ConditionalExpressionL();

	// Procedure calls
	virtual void ProcCallPostambleL(const COplReferenceSymbol *pRef);

	// Identifiers
	virtual TPcode::TCode IdentifierReferencePcode() const;
	virtual void FieldReferenceL(TPcode::TIdentifierSide aSide);
	
	// Language fragments - Keywords	
	void NumericDialogItemL(TOplToken::TType);
	TUint CountWordExpressionsL();
	void WordExpressionsL(TInt aCount); // Comma separated list of word expressions
	TOplField::TDevice LogicalDeviceL();
	void IdentifierRefL(TOplToken::TType aType); // Type safe identifer reference
	void WordArrayRefL();
	void LongArrayRefL();
	void ArrayRefL(TOplToken::TType aType);

	// Token checking
	TOplToken SkipBlankLinesL();
	void EosL();
	static TBool IsEos(TOplToken aToken);
	TBool TestEosL();
	TBool NextIsOffL();
	TBool NextIsOffOrOnL(TInt32& aQualifier);
	TOplToken SkipToEndL(); // Skips to EndP or Eof

	// Code emmission stuff
//	virtual void PCodeOffsetL(TPcode::TCode aCode,TUint anOffset);
	void PCodeTypedCommandL(TOplKeyword::TCode aKeyword,TOplToken::TType aType);
/*	// From MOplLexerLineCB FOR NOW !!
	void NewLineL(TInt aLineNumber, const TDesC& aFileName, TInt aFileOffset);
*/
private:
	TState iState;
	TBool iLastWasReturn;

	// Runtime error location
	TBool iLocatingError;
	TInt iLineNumber;
	TInt iQcodeOffset;

	TBranches iBranches;
	COplProcHeader *iProcHeader;
	COplModuleBackEnd *iBackEnd;
	};

//////////////////////////////////////////////////////////////////
//
// COplEvalParser
//
//////////////////////////////////////////////////////////////////
const TUint KOplEvalProcArrayGran=8;

class COplEvalParser : public COplParserBase
//
// Does the translation of an expression for eval
//
	{
public:
	static COplEvalParser *NewL();
	void ParseExpressionL(COplLineLexer& aLexer,TTranslateError& anError,COpl16EvalBackEnd& aBackEnd);
protected:
	virtual TPcode::TCode IdentifierReferencePcode() const;
	virtual void FieldReferenceL(TPcode::TIdentifierSide aSide);
	virtual void ProcCallPreambleL(const TDesC& aName,TOplToken::TType aType);
	virtual void ProcCallPostambleL(const COplReferenceSymbol *pRef);
	};

#include "ot_parse.inl"
#endif
