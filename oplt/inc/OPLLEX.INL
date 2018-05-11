// OPLLEX.INL
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


//
// COplLexerBase
//
inline void COplLexerBase::SetError(TTranslateError* anError) { iError=anError;}
inline TOplToken COplLexerBase::Token() const { return iToken;}
inline TOplToken::TType COplLexerBase::Type() const { return iType;}
inline const TDesC& COplLexerBase::Name() const {return iText;}

inline TOplConstant& COplLexerBase::Constant() { return *(TOplConstant *)iConstant;}
inline const TOplField& COplLexerBase::Field() const {return *(TOplField *)iField;}
inline const TOplFunction& COplLexerBase::Function() const { return *(TOplFunction*)iFunction;}
inline const TOplKeyword& COplLexerBase::Keyword() const { return *(TOplKeyword*)iKeyword;}
inline COplCallSymbol* COplLexerBase::OpxSymbol() const { return iOpxSymbol;}

//
// COplLineLex
// 
inline void COplLineLexer::SetExpression(const TDesC& anExpr) { iLex.Assign(anExpr); }

//
// COplModuleLexer
//
inline TInt COplModuleLexer::LineNumber() const { return iCurrentSource.iLineNumber;}
inline void COplModuleLexer::SetLineNumber(TInt aLineNumber) {iCurrentSource.iLineNumber=aLineNumber;}
inline const TDesC& COplModuleLexer::FileName() const { return *iCurrentSource.iName;}
