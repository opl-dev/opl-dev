// OT_PARSE.INL
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// TStackedOperator
inline TStackedOperator::TStackedOperator(TOplToken anOp,TOplToken::TType aType,TStreamPos aPos,TUint anOffset)
	: iOperator(anOp), iType(aType), iPcodePos(aPos), iOffset(anOffset)
	{}
inline TStackedOperator::TStackedOperator() 
	{}
inline void TStackedOperator::SetOperator(TOplToken anOp) {iOperator=anOp;}
inline TOplToken TStackedOperator::Operator() { return iOperator;}	
inline TStackedOperator::operator TOplToken::TValue() { return TOplToken::TValue(iOperator);}
inline void TStackedOperator::SetType(TOplToken::TType aType) { iType=aType;}
inline TOplToken::TType TStackedOperator::Type() { return iType;}
inline TStreamPos TStackedOperator::PcodePos() { return iPcodePos;}
inline TUint TStackedOperator::Offset() { return iOffset;}

// COplParserBase
inline COplLexerBase& COplParserBase::Lexer() { return *iLexer;}
inline TOplTranTarget COplParserBase::Target() const { return iTarget;}

// TBranches - State of current branches for Structural stuff
inline void TBranches::EnterLoop() {iLoop++;}
inline void TBranches::LeaveLoop() {iLoop--;}
inline TBool TBranches::InLoop() {return iLoop;}

// COplModuleParser
inline COplModuleLexer& COplModuleParser::Lexer() { return *(COplModuleLexer *)iLexer;}

