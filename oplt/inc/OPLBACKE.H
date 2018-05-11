// OPLBACKE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Classes needed to produce an OPL translator back end

#ifndef __OPLBACKE_H__
#define __OPLBACKE_H__

#include <e32base.h>
#include <s32mem.h>
#include <texttran.h>
#include <opltdef.h>
#include <opltoken.h>

//////////////////////////////////////////////////////////////
//
// COplSymbol - A symbol table entry. References are made to
// to these within the PCODE, which is why they appear here.
//
//////////////////////////////////////////////////////////////
class COplSymbol : public CBase
	{
public:
	enum TClass
		{
		EConstant,
		ELabel,
		EGlobal,
		ELocal,
		EArgument,
		EExternalDecl,
		EOpxFunction,
		EProcDecl,
		EExternalRef,
		EProcRef
		};
	virtual const TDesC& Name() const =0;
	virtual TOplToken Token() const =0;
	virtual TOplToken::TType Type() const =0;
	inline COplSymbol::TClass Class() const;
	inline void SetAddress(TUint anAddress);
	inline TUint Address() const;
	inline void ExternalizeL(RWriteStream& aStream) const;
protected:
	COplSymbol::COplSymbol(COplSymbol::TClass aClass);
protected:
	TClass iClass;
	TUint iAddress;

private: // Stuff that enables it to be put in the symbol table.
	friend class COplSymbolTable;
	TInt iScope;
	TSglQueLink iScopeLink;
	TDblQueLink iHashLink;
	};

///////////////////////////////////////////////////////////////////////
//
// Root of the 'real' symbols tree. A symbol of this class has a declaration
// (implicit or explicit) somewhere in the code.
//
///////////////////////////////////////////////////////////////////////
class COplReferenceSymbol;
class COplDeclarationSymbol : public COplSymbol
	{
public:
	static COplDeclarationSymbol *NewLC(COplSymbol::TClass, TOplToken aToken, TOplToken::TType aType,const TDesC& aName);
	COplReferenceSymbol *NewReferenceLC();
	const TDesC& Name() const;
	TOplToken Token() const;
	TOplToken::TType Type() const;
	inline void SetToken(TOplToken aToken);
	inline void SetType(TOplToken::TType aType);
	~COplDeclarationSymbol();
protected:
	COplDeclarationSymbol(COplSymbol::TClass aClass, TOplToken aToken,TOplToken::TType aType);
	void ConstructL(const TDesC& aName);
protected:
	HBufC *iName;
	TOplToken iToken;
	TOplToken::TType iType;
	};

/////////////////////////////////////////////////////////////
//
// COplReferenceSymbol - a reference to a symbol which might be
// implicitly defined
//
/////////////////////////////////////////////////////////////
class COplReferenceSymbol : public COplSymbol
	{
public:
	const TDesC& Name() const;
	TOplToken Token() const;
	TOplToken::TType Type() const;
	inline COplDeclarationSymbol& Declaration() const;
private:
	friend class COplDeclarationSymbol;
	COplReferenceSymbol(COplSymbol::TClass aClass, COplDeclarationSymbol& aSymbol);
private:
	COplReferenceSymbol& operator=(COplReferenceSymbol&); // Stop the compiler whinging
private:
	COplDeclarationSymbol& iDeclaration;
	};

///////////////////////////////////////////////////////////
//
// COplAutomaticSymbol- Things declared on the local stack,
// i.e. LOCAL & GLOBAL
//
///////////////////////////////////////////////////////////
class COplAutomaticSymbol : public COplDeclarationSymbol
   {
public:
   inline TInt StringLength();
   inline TInt ArraySize();
   inline void SetStringLen(TInt aLength);
   inline void SetArraySize(TInt aSize);
private:
   friend class COplDeclarationSymbol;
   inline COplAutomaticSymbol(TClass aClass, TOplToken aToken,TOplToken::TType aType);
private:
   TInt iArraySize;
   TInt iStringLength;
   };

///////////////////////////////////////////////////////////
//
// TOpxAddress Converts a UINT to and OPX address and
// vice versa
//
///////////////////////////////////////////////////////////
class TOpxAddress
   {
public:
   inline TOpxAddress(TUint aModule,TUint aFunction);   
   inline TOpxAddress(TUint anAddress);
   inline operator TUint() const;
   inline TUint Module() const;
   inline TUint Function() const;
private:
   TUint  iModule;
   TUint iFunction;
   };

///////////////////////////////////////////////////////////
//
// Something which is callable, i.e. it has an associated
// Argument signature
//
///////////////////////////////////////////////////////////
class COplCallSymbol : public COplDeclarationSymbol
   {
public:
   inline TOplSignature& Signature();
private:
   friend class COplDeclarationSymbol;
   inline COplCallSymbol(TClass aClass, TOplToken aToken,TOplToken::TType aType);
private:
   TOplSignature iSignature; 
   };

//////////////////////////////////////////////////////////
//
// A Constant define.
//
//////////////////////////////////////////////////////////
class COplConstantSymbol : public COplDeclarationSymbol
   {
public:
	~COplConstantSymbol();
	inline TOplConstant& Value();
private:
	friend class COplDeclarationSymbol;
	inline COplConstantSymbol(TClass aClass, TOplToken aToken,TOplToken::TType aType);
private:
	TOplConstant iValue;
	};

///////////////////////////////////////////////////////////////
//
// TLabelReference - a refernce to a label. It has position in a stream
// and it has a base from which to calculate the relative distance to teh label
//
////////////////////////////////////////////////////////////////
class TLabelReference
	{
public:
	inline TLabelReference(TStreamPos aPos, TInt aBase) : iPos(aPos), iBase(aBase) {}
	inline TStreamPos Position() const { return iPos;}
	inline TInt Base() const { return iBase;}
private:
	TStreamPos iPos;
	TInt iBase;
	};

//////////////////////////////////////////////////////////////
//
// A Label symbol
//
//////////////////////////////////////////////////////////////

const TUint KOplLabelReferenceGran=8;
class COplLabelSymbol : public COplDeclarationSymbol
	{
public:
	inline void Define();
	inline TBool IsDefined() const;
	inline void AddReferenceL(const TLabelReference& aReference);
	inline const CArrayFixFlat<TLabelReference>& References() const;
private:
	friend class COplDeclarationSymbol;
	COplLabelSymbol(TClass aClass, TOplToken aToken, TOplToken::TType aType);
private:
	TBool isDefined;
	CArrayFixFlat<TLabelReference> iReferences;
	};

////////////////////////////////////////////
//
// COplParseApp - base class for an app
//
////////////////////////////////////////////
class COplModuleLexer;
class COpl16ModuleBackEnd;
class COplParseApp : public CBase
   {
public:
	virtual void ParseL(COpl16ModuleBackEnd& aBackEnd,TBool aLocatingError)=0;
	};

////////////////////////////////////////////
//
// COplParseOpx - Declaration of an OPX
//
////////////////////////////////////////////
const TInt KOplParseOpxFunctionGran=8;
class COplParseOpx : public CBase
   {
public:
	static COplParseOpx *NewLC();
	~COplParseOpx();
	void SetNameL(const TDesC& aName);
	inline const TDesC& Name() const;
	inline void SetUid(TUid aUid);
	inline TUid Uid() const;
	inline void SetVersion(TUint aVersion);
	inline TUint Version() const;
private:
	COplParseOpx();
private:
	HBufC *iName;
	TUid iUid;
	TUint iVersion;
	};

////////////////////////////////////////////
//
// COplProcHeader - Internal model of a Procedure header
//
////////////////////////////////////////////
const TInt KOplParseProcArrayGran=8;
class COplProcHeader : public CBase
   {
public:
	inline TOplToken::TType Type();
	inline const CArrayFix<COplSymbol*>& Arguments() const;
	inline const CArrayFix<COplSymbol*>& Externals() const;
	inline const CArrayFix<COplSymbol*>& Globals() const;
	inline const CArrayFix<COplSymbol*>& Locals() const; 
	inline const CArrayFix<COplSymbol*>& ProcCalls() const;
	inline const CArrayFix<COplSymbol*>& Labels() const;
private:
	friend class COplModuleParser;
	static COplProcHeader *NewLC(TOplToken::TType aType);
	void AddSymbolL(COplSymbol& pSym);
private:
	COplProcHeader(TOplToken::TType aType);
private:
	TOplToken::TType iType;    // Return type of the procedure
	CArrayFixFlat<COplSymbol*> iArguments;        // The arguments to the procedure
	CArrayFixFlat<COplSymbol*> iExternals;        // Externals erferences by the procedure
	CArrayFixFlat<COplSymbol*> iGlobals; // Globals declared within the prpocedure 
	CArrayFixFlat<COplSymbol*> iLocals;  // Locals declared within this procedure
	CArrayFixFlat<COplSymbol*> iProcCalls;        // Procedures called within this procedure
	CArrayFixFlat<COplSymbol*> iLabels;      // Labels within the Qcode
	};


///////////////////////////////////////////////////////////////////////
//
// TPcode - An internal Pcode
//
///////////////////////////////////////////////////////////////////////

inline RReadStream& operator>>(RReadStream& aStream, TOplToken& aValue) { aValue=TOplToken(TOplToken::TValue(aStream.ReadUint16L()));return aStream;}
inline RReadStream& operator>>(RReadStream& aStream, TOplToken::TType& aType) { aType=TOplToken::TType(aStream.ReadUint8L()); return aStream;}
inline RReadStream& operator>>(RReadStream& aStream, TOplKeyword::TCode& aCode) { aCode=TOplKeyword::TCode(aStream.ReadUint8L()); return aStream;}
inline RReadStream& operator>>(RReadStream& aStream, TOplSignature::TArgType& aType) { aType=TOplSignature::TArgType(aStream.ReadUint8L()); return aStream;}
inline RReadStream& operator>>(RReadStream& aStream, TOplFunction::TNumber& aNumber) { aNumber=TOplFunction::TNumber(aStream.ReadUint8L()); return aStream;}
inline RReadStream& operator>>(RReadStream& aStream, TOplField::TDevice& aDevice) {aDevice=TOplField::TDevice(aStream.ReadUint8L());return aStream;}
inline RReadStream& operator>>(RReadStream& aStream, COplSymbol*& aSymbol){aSymbol=(COplSymbol *)aStream.ReadUint32L();return aStream;}

inline RWriteStream& operator<<(RWriteStream& aStream, TOplToken::TValue aValue) { aStream.WriteUint16L(aValue); return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, TOplToken::TType aType) { aStream.WriteUint8L(aType); return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, TOplKeyword::TCode aCode) { aStream.WriteUint8L(aCode); return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, TOplSignature::TArgType aType) { aStream.WriteUint8L(aType); return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, TOplFunction::TNumber aNumber) { aStream.WriteUint8L(aNumber); return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, TOplField::TDevice aDevice) {aStream.WriteUint8L(aDevice);return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, COplSymbol* aSymbol){aStream.WriteUint32L(TUint32(aSymbol));return aStream;}

// Used to shift and encode the number of bits in the offset
const TInt KOplPcodeTCodeMask=0x3f;
const TInt KOplPcodeSizeMask=0xc0;
const TInt KOplPcodeSizeShift=6;
class TPcode
	{
public:
	enum TCode // Tokens in the Parse code
		{
		// Used to mark statement starts for the debugger
		EStatement,
		
		// Various operand references
		EConstant, EIdentifier, EField,EEvalIdentifier,
		
		// Proc, Function & OPX calls.
		EProcCallName,		// Marks the name for call by name.
		EArgList,			// Marks the start of an argument list
		EProcArg, EOpxArg, EFunctionArg, // Argument types
		
		EProcCall, EProcByNameCall, EOpxCall, // The actual calls
		EFixedFunctionCall, ECountedFunctionCall,

		// Operator and expression stuff
		EDrop, EAssign, ECast, EOperator,

		// Command keywords 
		ECommand, ETypedCommand, EQualifiedCommand,
		EFieldName, EEndFieldList, // Fields name for Open, Create etc
		
		// Structural stuff
		ELabel, EBranchFalse, EBranchAlways, EOnErr,
		EVector, EVectorLabel, EEndVector
		};
public:
	inline TPcode::TPcode() {}
	inline TPcode::TPcode(TPcode::TCode aCode,TUint32 anOffset) : iCode(aCode), iOffset(anOffset) {}
	inline TPcode::TCode Code() { return iCode;}
	inline TUint32 Offset() { return iOffset;}
	void ExternalizeL(RWriteStream& aStream) const; // Should be needed outside the parser
	IMPORT_C void InternalizeL(RReadStream& aStream); // Needed by anyone writing a back end
	// Some odds an ends enums
	enum TIdentifierSide {ELeft, ERight};
private:
	enum TOffsetSize {EOffset8Bits=0,EOffset16Bits=(1<<KOplPcodeSizeShift), EOffset32Bits=(2<<KOplPcodeSizeShift) };
private:
	TCode iCode; // actual Pcode
	TUint32 iOffset; // Corresponding offset in the source code
	};

inline RReadStream& operator>>(RReadStream& aStream, TPcode::TIdentifierSide& aSide) { aSide=TPcode::TIdentifierSide(aStream.ReadUint8L()); return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, TPcode::TIdentifierSide aSide) { aStream.WriteUint8L(aSide); return aStream;}
//////////////////////////////////////////////////////////////////
//
// Things useful for reading Pcode.
//
//////////////////////////////////////////////////////////////////
class CPcodeConstant : public CBase
	{
public:
	IMPORT_C static CPcodeConstant *CPcodeConstant::NewLC(RReadStream& aStream);
	IMPORT_C ~CPcodeConstant();
	inline TOplToken::TType Type() const { return iType;}
	inline TInt Int() const { return iInt;}
	inline TReal64 Real() const { return iReal;}
	inline const TDesC& String() const { return *iString;}
private:
	CPcodeConstant() {;}
private:
	TOplToken::TType iType;
	union
		{
		TInt iInt;
		TReal64 iReal;
		HBufC* iString;
		};
	};

class TPcodeSymbolReference
	{
public:
	IMPORT_C void InternalizeL(RReadStream& aStream);
	inline TPcode::TIdentifierSide Side() const { return iSide;}
	inline COplSymbol* Symbol() const { return iSymbol;}
private:
	TPcode::TIdentifierSide iSide;
	COplSymbol* iSymbol;
	};

class CPcodeFieldReference : public CBase
	{
public:
	IMPORT_C static CPcodeFieldReference* CPcodeFieldReference::NewLC(RReadStream& aStream);
	IMPORT_C ~CPcodeFieldReference();
	inline TOplField::TDevice Device() const { return iDevice;}
	inline TOplToken::TType Type() const { return iType;}
	inline const TDesC& Name() const { return *iName;}
	inline TPcode::TIdentifierSide Side() const { return iSide;}
private:
	CPcodeFieldReference() {;}
private:
	TOplField::TDevice iDevice;
	TOplToken::TType iType;
	HBufC* iName;
	TPcode::TIdentifierSide iSide;
	};

class TPcodeCallArgument
	{
public:		
	inline void InternalizeL(RReadStream& aStream) { iType=TOplSignature::TArgType(aStream.ReadUint8L());}
	inline TOplSignature::TArgType Type() const { return iType;}
private:
	TOplSignature::TArgType iType;
	};

class TPcodeCall
	{
public:
	inline void InternalizeL(RReadStream& aStream) { iSymbol=(COplCallSymbol *)(aStream.ReadUint32L());}
	inline COplCallSymbol* Symbol() const {return iSymbol;}
private:
	COplCallSymbol *iSymbol;
	};

class TPcodeProcByName
	{
public:	
	IMPORT_C void InternalizeL(RReadStream& aStream);
	inline TInt ArgumentCount() const { return iArgumentCount;}
	inline TOplToken::TType Type() const { return iType;}
private:
	TInt iArgumentCount;
	TOplToken::TType iType;
	};

class TPcodeFunctionCall
	{
public:
	IMPORT_C void InternalizeL(RReadStream& aStream);
	inline TOplFunction::TNumber Number() const  { return iNumber;} 
	inline TOplToken::TType Type() const { return iType;}
	inline TUint ArgumentCount() const  {return iArgumentCount;}
private:
	TOplFunction::TNumber iNumber;
	TOplToken::TType iType;
	TUint iArgumentCount;
	};

class TPcodeDrop
	{
public:
	inline void InternalizeL(RReadStream& aStream) {aStream>>iType;}
	inline TOplToken::TType Type() const { return iType;}
private:
	TOplToken::TType iType;
	};

class TPcodeAssign
	{
public:
	inline void InternalizeL(RReadStream& aStream) {aStream>>iType;}
	inline TOplToken::TType Type() const { return iType;}
private:
	TOplToken::TType iType;
	};

class TPcodeCast
	{
public:
	inline void InternalizeL(RReadStream& aStream) { aStream>>iFrom>>iTo;}
	inline TOplToken::TType From() const { return iFrom;}
	inline TOplToken::TType To() const{ return iTo;}
private:
	TOplToken::TType iFrom;
	TOplToken::TType iTo;
	};

class TPcodeOperator
	{
public:
	IMPORT_C void InternalizeL(RReadStream& aStream);
	enum TArity { EUnary, EBinary };
	inline TOplToken::TValue Oper() const { return TOplToken::TValue(iOper);}
	inline TOplToken::TType OperandType() const { return iOperandType;}
	inline TOplToken::TType ResultingType() const { return iResultingType;}
	inline TPcodeOperator::TArity Arity() const { return iArity;}
private:
	TOplToken iOper;
	TOplToken::TType iOperandType;
	TOplToken::TType iResultingType;
	TPcodeOperator::TArity iArity;
	};
inline RReadStream& operator>>(RReadStream& aStream, TPcodeOperator::TArity& anAry) { anAry=TPcodeOperator::TArity(aStream.ReadUint8L()); return aStream;}
inline RWriteStream& operator<<(RWriteStream& aStream, TPcodeOperator::TArity anAry) { aStream.WriteUint8L(anAry); return aStream;}

class TPcodeCommand
	{
public:
	inline void InternalizeL(RReadStream& aStream) {aStream>>iCode;}
	inline TOplKeyword::TCode Code() const { return iCode;}	
private:
	TOplKeyword::TCode iCode;
	};

class TPcodeTypedCommand : public TPcodeCommand
	{
public:
	IMPORT_C void InternalizeL(RReadStream& aStream);
	inline TOplToken::TType Type() const { return iType;}
private:
	TOplToken::TType iType;
	};

class TPcodeQualifiedCommand : public TPcodeCommand
	{
public:
	IMPORT_C void InternalizeL(RReadStream& aStream);
	inline TUint Qualifier() const { return iQualifier;}
private:
	TUint32 iQualifier;
	};


class TPcodeLabel
	{
public:
	inline void InternalizeL(RReadStream& aStream) { iSymbol=(COplLabelSymbol *)aStream.ReadUint32L();}	
	inline COplLabelSymbol* Symbol() const { return iSymbol;}
private:
	COplLabelSymbol* iSymbol;
	};

class CPcodeFieldName : public CBase
	{
public:	
	IMPORT_C static CPcodeFieldName* NewLC(RReadStream& aStream);
	IMPORT_C ~CPcodeFieldName();
	inline const TDesC& Name() const { return *iName;}
	inline TOplToken::TType Type() const { return iType;}
private:
	CPcodeFieldName() {;}
private:
	HBufC * iName;
	TOplToken::TType iType;
	};
////////////////////////////////////////////////////////////
//
// COplModuleBackEnd - Pure virtual base class for 
// all Module translator back ends.
//
///////////////////////////////////////////////////////////
class MTranAppCB;
class COplModuleLexer;
class COplModuleBackEnd : public CBase
   {
public:
	virtual void StartL(const TDesC& aSourceName, CTextTranslator::TDebugFlag aDebugFlag,TSourceTranslateError& anError)=0;
	virtual COplParseApp* GetAppLC(TOplTranTarget aTarget,COplLexerBase& aLexer)=0;
	virtual void AppL(COplParseApp* anApp)=0;		// App Declaration
	virtual void OpxL(COplParseOpx* anOpx)=0;		// Opx Declaration
	virtual TInt OpxCount()=0;							// Number of OPX's seen so far.
	virtual void StartProcL(const TDesC& aName, TInt aLineNumber, TInt aQcodeOffset)=0;
	virtual void ProcL(COplProcHeader* aHeader,CBufSeg* aCodeBlock)=0;	// Proc declaration
	virtual void EndOfFileL()=0;
	};
#include <oplbacke.inl>
#endif
