// OPLBACKE.INL
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// COplSymbol
inline COplSymbol::TClass COplSymbol::Class() const { return iClass;}
inline void COplSymbol::SetAddress(TUint anAddress) { iAddress=anAddress;}
inline TUint COplSymbol::Address() const { return iAddress;}
inline void COplSymbol::ExternalizeL(RWriteStream& aStream) const { aStream.WriteUint32L(TUint(this));}

// COplDeclarationSymbol
inline void COplDeclarationSymbol::SetToken(TOplToken aToken) {iToken=aToken;}
inline void COplDeclarationSymbol::SetType(TOplToken::TType aType) {iType=aType;}

// COplReferenceSymbol
inline COplDeclarationSymbol& COplReferenceSymbol::Declaration() const { return iDeclaration;}

// COplAutomaticSymbol
inline void COplAutomaticSymbol::SetStringLen(TInt aLength) {iStringLength=aLength;}
inline void COplAutomaticSymbol::SetArraySize(TInt aSize) {iArraySize=aSize;}
inline TInt COplAutomaticSymbol::StringLength() {return iStringLength;}
inline TInt COplAutomaticSymbol::ArraySize() {return iArraySize;}
inline COplAutomaticSymbol::COplAutomaticSymbol(TClass aClass, TOplToken aToken, TOplToken::TType aType)
    : COplDeclarationSymbol(aClass, aToken, aType) {}


// TOpxAddress
inline TOpxAddress::TOpxAddress(TUint aModule, TUint aFunction) : iModule(aModule), iFunction(aFunction) {}
inline TOpxAddress::TOpxAddress(TUint anAddress) : iModule(anAddress>>16), iFunction(anAddress&0xffff) {}
inline TOpxAddress::operator TUint() const { return ((iModule<<16)|iFunction); }
inline TUint TOpxAddress::Module() const { return iModule;}
inline TUint TOpxAddress::Function() const { return iFunction;}

// COplCallSymbol
inline TOplSignature& COplCallSymbol::Signature() { return iSignature;}
inline COplCallSymbol::COplCallSymbol(COplSymbol::TClass aClass, TOplToken aToken, TOplToken::TType aType)
	 : COplDeclarationSymbol(aClass, aToken, aType) {}

// COplConstantSymbol
inline TOplConstant& COplConstantSymbol::Value() { return iValue;}
inline COplConstantSymbol::COplConstantSymbol(TClass aClass, TOplToken aToken, TOplToken::TType aType)
	: COplDeclarationSymbol(aClass, aToken, aType) {}

// COplLabelSymbol
inline void COplLabelSymbol::Define() { isDefined=ETrue;}
inline TBool COplLabelSymbol::IsDefined() const { return isDefined;}
inline void COplLabelSymbol::AddReferenceL(const TLabelReference& aReference) { iReferences.AppendL(aReference);}
inline const CArrayFixFlat<TLabelReference>& COplLabelSymbol::References() const { return iReferences;}

// COplParseOpx
inline const TDesC& COplParseOpx::Name() const { return *iName;}
inline void COplParseOpx::SetUid(TUid aUid) { iUid=aUid;}
inline TUid COplParseOpx::Uid() const { return iUid;}
inline void COplParseOpx::SetVersion(TUint aVersion) { iVersion=aVersion;}
inline TUint COplParseOpx::Version() const { return iVersion;}

// COplProcHeader
inline TOplToken::TType COplProcHeader::Type() { return iType;}
inline const CArrayFix<COplSymbol*>& COplProcHeader::Arguments() const { return iArguments;}
inline const CArrayFix<COplSymbol*>& COplProcHeader::Externals() const { return iExternals;}
inline const CArrayFix<COplSymbol*>& COplProcHeader::Globals() const { return iGlobals;}
inline const CArrayFix<COplSymbol*>& COplProcHeader::Locals() const {return iLocals; }
inline const CArrayFix<COplSymbol*>& COplProcHeader::ProcCalls() const {return iProcCalls;}
inline const CArrayFix<COplSymbol*>& COplProcHeader::Labels() const {return iLabels;}

