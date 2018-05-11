// OT_STD.INL
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// TOplOpStackItem
inline TOplOpStackItem::TOplOpStackItem(TOplToken anOp,TOplToken::TType aType,TUint aPos,TUint anOffset)
	: iOperator(anOp), iType(aType), iQcodePos(aPos), iLineOffset(anOffset)
	{}
inline TOplOpStackItem::TOplOpStackItem() 
	{}
inline void TOplOpStackItem::SetOperator(TOplToken anOp) {iOperator=anOp;}
inline TOplToken TOplOpStackItem::Operator() { return iOperator;}	
inline TOplOpStackItem::operator TOplToken::TValue() { return TOplToken::TValue(iOperator);}
inline void TOplOpStackItem::SetType(TOplToken::TType aType) { iType=aType;}
inline TOplToken::TType TOplOpStackItem::Type() { return iType;}
inline TUint TOplOpStackItem::QcodePos() { return iQcodePos;}
inline TUint TOplOpStackItem::LineOffset() { return iLineOffset;}

// TBranches - State of current branches for Structural stuff
inline void TBranches::EnterLoop() {iLoop++;}
inline void TBranches::LeaveLoop() {iLoop--;}
inline TBool TBranches::InLoop() {return iLoop;}

// COplParserBase
COplLexBase& COplParserBase::Lexer() {return *iLexer;}
COplBackEndBase& COplParserBase::BackEnd() { return *iBackEnd;}

// COpxDeclaration
/*
inline const TDesC& COpxDeclaration::Name() {return *iName;}
inline void COpxDeclaration::AddFunctionL(HBufC *aName) {iFunctionNames->AppendL(aName);}
inline TInt COpxDeclaration::Version() { return iVersion;}
inline void COpxDeclaration::SetVersion(TInt aVersion) {iVersion=aVersion;}
inline TInt COpxDeclaration::FunctionCount() {return iFunctionNames->Count();}
inline const TDesC& COpxDeclaration::operator[](TInt anIndex) { return *((*iFunctionNames)[anIndex]);}
*/
// COplModParser
COplModLex& COplModParser::Lexer() { return *(COplModLex*)iLexer;}
COplModBackEnd& COplModParser::BackEnd() { return *(COplModBackEnd*)iBackEnd;}
