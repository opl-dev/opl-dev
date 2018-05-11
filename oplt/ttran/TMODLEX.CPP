// TMODLEX.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Tests the lexical scanner for OPL modules.
//
//

#include <e32test.h>
#include <opllex.h>

LOCAL_D RTest test(_L("TMODLEX"));

#define SOURCE(a) TSource source(a,(sizeof(a)-1)/sizeof(TText))
#define CHECK_SOURCE(t) CheckTokens(source,t)
#define SET_CHECK_SOURCE(a,t) source.Set(a,(sizeof(a)-1)/sizeof(TText)); CHECK_SOURCE(t)
#define _MYS(a) ((const TText)L##a)
class TModLexError
	{
public:
	TText **lines;
	TUint lineNo;
	TUint iOffset;
	TInt iError;
	};

class TSource : public MTextSource
	{
public:
	TSource(const TText* anArray, TInt aSize);
	void Set(const TText* anArray, TInt aSize);
	TInt Read(TDes& aBuf,TInt aPos);
	TInt Read(TDes& aBuf);
	void Close();
private:
	TSource& operator=(TSource&);
	TInt iPos;
	TPtrC iBuf;	
	};

class TOplTokenCheck // Used to check token streams
	{
public:
	void CheckNext(COplModuleLexer *aLex) const;
	void Check(COplModuleLexer *aLex) const;
public:	
	TOplToken::TValue iToken;
	TOplToken::TType iType;
	const TAny *iValue;
	};


class TTestModLex : public MTextSourceSystem
	{
public:
	inline TTestModLex() : iSource(NULL) {};
	void RunTests();	
private:
	void Test1(); // Interface
	void Test2(); // Constructor alloc heaven
	void Test3(); // Constants 
	void Test4(); // Operators and punctuation 
	void Test5(); // Identifiers
	void Test6(); // Keywords
	void Test7(); // Functions
private:	
	COplModuleLexer *GetLex();
	void DestroyLex();
	void SetSource(TSource& aSource);
	
	void CheckTokens(TSource &aSource,const TOplTokenCheck *aTokenArray);
	//void CheckErrors(const TCalcLexError *errors);
	
	TOplToken MarkLex(); // Wraps iLex.Lex() in __UHEAP_MARK/__UHEAP_MARKEND
	TInt MarkError(TOplToken &aToken); // Lea is expected to leave with an error - checks start end stuff
	virtual TInt OpenSource(TDes& aFileName,MTextSource*& aTextSource);
private:
	TSource *iSource;
	COplModuleLexer *iLex;
	};

//////////////////////////////////////////////////////////////////
//
// TOplTokenCheck
//
//////////////////////////////////////////////////////////////////
void TOplTokenCheck::CheckNext(COplModuleLexer *aLex) const
//
// Checks that the next token in the input matches this one 
//
	{
	TOplToken next;
	TRAPD(r,next=aLex->LexL());
	test(r==KErrNone);
	
	test(next==iToken);
	switch (next.Class())
		{
		case TOplToken::EOperator:
			break;
		case TOplToken::EIdentifier: // Check the value
			test(aLex->Type()==iType);
			if (iToken==TOplToken::EConstant) // FOR NOW
				{
				switch (iType)
					{
					case TOplToken::EWord:
						test(aLex->Constant().AsWordL()==*(TInt *)iValue);
						break;
					case TOplToken::ELong:
						test(aLex->Constant().AsLongL()==*(TInt32 *)iValue);
						break;
					case TOplToken::EReal:
						test(aLex->Constant().AsRealL()==*(TReal64 *)iValue);
						break;					
					case TOplToken::EString:
						test(aLex->Constant().AsStringL().Compare(*(TDesC *)iValue)==0);
						break;
               default:
                  test(EFalse); 
               }
				}
			break;

		case TOplToken::EPunctuation: 
			break;
		case TOplToken::EKeyword:
		case TOplToken::EReserved:
			break;
		case TOplToken::ECall:
			test(aLex->Type()==iType);
			break;		
		default:
			test(EFalse);
		}
	}


void TOplTokenCheck::Check(COplModuleLexer *aLex) const
//
// Checks that the next token matches this one
// ALWAYS does a Lex, UnLex Lex to check it.
//
	{

	__UHEAP_MARK;
	CheckNext(aLex);
//	aLex->UnLex();
//	CheckNext(aLex);
	__UHEAP_MARKEND;
	}


////////////////////////////////////////////////////////////
//
// TSoure - text source
//
////////////////////////////////////////////////////////////
TSource::TSource(const TText *anArray, TInt aSize) : iPos(0), iBuf(anArray, aSize)
//
//
//
	{
	}


void TSource::Set(const TText *anArray, TInt aSize)
//
//
//
	{
	
	iBuf.Set(anArray,aSize);
	iPos=0;
	}


TInt TSource::Read(TDes& aBuf, TInt aPos)
//
// Copies the relevant bit into aBuf
//
	{
	
	TInt ret=KErrEof;
	if (aPos<iBuf.Length())
		{
		TInt remainder=iBuf.Length()-aPos;
		if (remainder>aBuf.MaxLength())
			remainder=aBuf.MaxLength();
		aBuf.Copy(iBuf.Mid(aPos,remainder));
		iPos=aPos+remainder;
		ret=KErrNone;
		}	
	return ret;
	}

TInt TSource::Read(TDes& aBuf)
//
// Reads from the end of the last read;
//
	{
	return Read(aBuf,iPos);
	}

void TSource::Close()
//
//
//
	{
	}

////////////////////////////////////////////////////////////////////
//
// TTestModLex
//
////////////////////////////////////////////////////////////////////

COplModuleLexer *TTestModLex::GetLex()
//
// I'm a gonna have me a lexer Pa. Creates a new lexical scanner
//
	{
	
	TRAPD(r,iLex=COplModuleLexer::NewL());
	test(r==KErrNone);
	return iLex;
	}

void TTestModLex::DestroyLex()
//
// Chucks away the lexer
//
	{
	
	delete iLex;
	iLex=NULL;
	}

void TTestModLex::SetSource(TSource& aSource)
//
//
//
	{
	iSource=&aSource;
	}


void TTestModLex::CheckTokens(TSource& aSource,const TOplTokenCheck *aTokenArray)
//
// Checks teh stream of tokens that we get back from aLineArray
//
	{
	TSourceTranslateError anError;

	SetSource(aSource);
	TRAPD(r,iLex->OpenL(*this,_L(""),anError));
	test(r==KErrNone);
	do
		{
		aTokenArray->Check(iLex);
		aTokenArray++;
		} while (aTokenArray->iToken!=TOplToken::EBadToken);
	iLex->Reset();
	}


TOplToken TTestModLex::MarkLex()
//
// Wraps successful lex __UHEAP_MARK/__UHEAP_MARKEND
//
	{
	__UHEAP_MARK;
	TOplToken token;
	TRAPD(r,token=iLex->LexL());
	test(r==KErrNone);
	__UHEAP_MARKEND;
	return token;
	}

TInt TTestModLex::MarkError(TOplToken &aToken)
//
// Wraps failed lex __UHEAP_MARK/__UHEAP_MARKEND
//
	{
	__UHEAP_MARK;
	TRAPD(r,aToken=iLex->LexL());
	__UHEAP_MARKEND;
	return r;	
	}

TInt TTestModLex::OpenSource(TDes& /* for now*/,MTextSource*& aTextSource)
//
//
//
	{
	
	if (iSource==NULL)
		return KErrNotFound;
	aTextSource=iSource; // Think about it - it's not as daft as it seems
	return KErrNone;
	}


LOCAL_D const TText test1Source[]=
//
// Source(s) used for testing basic interface
//
	{
	_MYS("PROC\x2029 1 a%\x2029 1.2 c$\x2029")
	};

void TTestModLex::Test1()
//
// All public funcitons
//
	{
	__UHEAP_MARK;
	test.Start(_L("New"));
	GetLex();

	test.Next(_L("OpenL"));
	SOURCE(test1Source);
	SetSource(source);
	TSourceTranslateError anError;
	TRAPD(r,iLex->OpenL(*this,_L(""),anError));
	test(r==KErrNone);

	test.Next(_L("Error offset"));
	test(iLex->TokenOffset()==0);
	

	test.Next(_L("Lex"));
	test(MarkLex()==TOplToken::EProc);

	test.Next(_L("Unlex"));
	iLex->UnLex();
	test(iLex->TokenOffset()==0);
	test(MarkLex()==TOplToken::EProc);
	test(MarkLex()==TOplToken::EEos);

	test.Next(_L("Line number/fetching line"));
	test(iLex->LineNumber()==0);
	test(MarkLex()==TOplToken::EConstant); // truck over the edge

	test.Next(_L("Token offset"));
	TInt pos=iLex->TokenOffset();

	test(iLex->LineNumber()==1);
	test.Next(_L("SetLineNumber"));
	iLex->SetLineNumber(1234);
	test(iLex->LineNumber()==1234);

	test.Next(_L("Goto file position"));
	iLex->GotoPosition(pos); // Back to where we were
	test(MarkLex()==TOplToken::EConstant);

	test.Next(_L("SavePos/RestorePos"));
	iLex->Mark();
	test(MarkLex()==TOplToken::ESimple);
	test(MarkLex()==TOplToken::EEos);
	iLex->UnGetToMark();
	test(MarkLex()==TOplToken::ESimple);
	test(MarkLex()==TOplToken::EEos);

	test.Next(_L("Token type"));
	test(MarkLex()==TOplToken::EConstant);
	test(iLex->Type()==TOplToken::EReal);
	test(MarkLex()==TOplToken::ESimple);
	test(iLex->Type()==TOplToken::EString);

	test.Next(_L("Set Target"));
	iLex->SetTarget(EOplTargetOpl1993);

	test.Next(_L("End of file"));
	test(MarkLex()==TOplToken::EEos);
	test(MarkLex()==TOplToken::EEof);
	test(MarkLex()==TOplToken::EEof);
	 
	DestroyLex();
	test.End();
	__UHEAP_MARKEND;
	}


void TTestModLex::Test2()
//
// Alloc heaven in construction
// Fails each alloc in turn.
//
	{
	
	test.Start(_L("Constructors"));
   TUint failNum=1;
   for (;;failNum++)
		{		
		__UHEAP_MARK;
		
		__UHEAP_SETFAIL(RHeap::EDeterministic,failNum);
		TRAPD(ret,iLex=COplModuleLexer::NewL());
		__UHEAP_RESET;
		
		if (ret==0)
			{
			test(iLex!=NULL);
			break;
			}
		test(iLex==NULL);		
		__UHEAP_MARKEND;
      }
#if defined(_DEBUG)
//   test(failNum!=1); // Check that we allocate at least one cell for the lexer (it is C... after all)
#endif	
	test.Next(_L("Destructor"));
	DestroyLex();
	__UHEAP_MARKEND;
	test.End();
	}

const TInt cInt0=0;
const TInt cInt1=1;
const TInt cInt127=127;
const TInt cInt128=128;
const TInt cInt32767=32767;
const TInt cIntM1=-1;
const TInt cIntM127=-127;
const TInt cIntM128=-128;
const TInt cIntM32767=-32767;
const TInt cIntM32768=-32768;

const TInt32 cLong0=0;
const TInt32 cLong1=1;
const TInt32 cLong127=127;
const TInt32 cLong128=128;
const TInt32 cLong32767=32767;
const TInt32 cLong32768=32768;
const TInt32 cLong2etc7=2147483647;
const TInt32 cLongM1=-1;
const TInt32 cLongM127=-127;
const TInt32 cLongM128=-128;
const TInt32 cLongM32767=-32767;
const TInt32 cLongM32768=-32768;
const TInt32 cLongM2etc7=-2147483647;
const TInt32 cLongM2etc8=0x80000000;

const TReal64 cReal1=1.0;
const TReal64 cReal10=10;
const TReal64 cRealP1=0.1;
const TReal64 cReal32767=32767;
const TReal64 cReal32768=32768;
const TReal64 cReal2etc7=2147483647;
const TReal64 cReal2etc8=0x80000000;


LOCAL_D const TText test3DecSource[]=
//
// Line(s) used for testing basic interface
//
	{
	
	_MYS("0 1 127 128 32767\x2029\
	32768 2147483647\x2029\
	1.0	.1	32767.0	32768.0	2147483647.0 2147483648 1e1 1e+1 1e-1\x2029")
	};


LOCAL_D const TOplTokenCheck test3DecTokens[]=
	{
	{ TOplToken::EConstant,TOplToken::EWord,&cInt0}, // Line 1 - word constants
	{ TOplToken::EConstant,TOplToken::EWord,&cInt1},
	{ TOplToken::EConstant,TOplToken::EWord,&cInt127},
	{ TOplToken::EConstant,TOplToken::EWord,&cInt128},
	{ TOplToken::EConstant,TOplToken::EWord,&cInt32767},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EConstant,TOplToken::ELong,&cLong32768}, // Line 2 - word constants
	{ TOplToken::EConstant,TOplToken::ELong,&cLong2etc7},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EConstant,TOplToken::EReal,&cReal1}, // Line 4 - real constants
	{ TOplToken::EConstant,TOplToken::EReal,&cRealP1},
	{ TOplToken::EConstant,TOplToken::EReal,&cReal32767},
	{ TOplToken::EConstant,TOplToken::EReal,&cReal32768},
	{ TOplToken::EConstant,TOplToken::EReal,&cReal2etc7},
	{ TOplToken::EConstant,TOplToken::EReal,&cReal2etc8},
	{ TOplToken::EConstant,TOplToken::EReal,&cReal10},
	{ TOplToken::EConstant,TOplToken::EReal,&cReal10},
	{ TOplToken::EConstant,TOplToken::EReal,&cRealP1},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EEof,TOplToken::EBadType,NULL},
	{ TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};


LOCAL_D const TText test3HexSource[]=
	{
	_MYS("$0 $1 $7f $80 $7fff\x2029\
	$ffff $ff81 $ff80 $8001 $8000\x2029\
	&0 &1 &7f &80 &7fff &8000 &7fffffff\x2029\
	&ffffffff &ffffff81 &ffffff80 &ffff8001 &ffff8000 &80000001 &80000000\x2029")
	};
	
LOCAL_D const TOplTokenCheck test3HexTokens[]=
	{
	{ TOplToken::EConstant,TOplToken::EWord,&cInt0}, // Line 1 - word constants
	{ TOplToken::EConstant,TOplToken::EWord,&cInt1},
	{ TOplToken::EConstant,TOplToken::EWord,&cInt127},
	{ TOplToken::EConstant,TOplToken::EWord,&cInt128},
	{ TOplToken::EConstant,TOplToken::EWord,&cInt32767},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EConstant,TOplToken::EWord,&cIntM1}, // Line 2 - negative words
	{ TOplToken::EConstant,TOplToken::EWord,&cIntM127},
	{ TOplToken::EConstant,TOplToken::EWord,&cIntM128},
	{ TOplToken::EConstant,TOplToken::EWord,&cIntM32767},
	{ TOplToken::EConstant,TOplToken::EWord,&cIntM32768},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EConstant,TOplToken::ELong,&cLong0}, // Line 3 - long constants
	{ TOplToken::EConstant,TOplToken::ELong,&cLong1},
	{ TOplToken::EConstant,TOplToken::ELong,&cLong127},
	{ TOplToken::EConstant,TOplToken::ELong,&cLong128},
	{ TOplToken::EConstant,TOplToken::ELong,&cLong32767},
	{ TOplToken::EConstant,TOplToken::ELong,&cLong32768},
	{ TOplToken::EConstant,TOplToken::ELong,&cLong2etc7},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EConstant,TOplToken::ELong,&cLongM1}, // Line 4 - negative longs
	{ TOplToken::EConstant,TOplToken::ELong,&cLongM127},
	{ TOplToken::EConstant,TOplToken::ELong,&cLongM128},
	{ TOplToken::EConstant,TOplToken::ELong,&cLongM32767},
	{ TOplToken::EConstant,TOplToken::ELong,&cLongM32768},
	{ TOplToken::EConstant,TOplToken::ELong,&cLongM2etc7},
	{ TOplToken::EConstant,TOplToken::ELong,&cLongM2etc8},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EEof,TOplToken::EBadType,NULL},
	{ TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};

LOCAL_D const TText test3StringSource[]=
//
//
//
	{
	_MYS("\"\" \"aString\" \"a quote\"\"\" \"two\"\"\"\"\"\x2029\
	\"012346789abcdef012346789abcdef012346789abcdef012346789abcdef\
012346789abcdef012346789abcdef012346789abcdef012346789abcdef\
012346789abcdef012346789abcdef012346789abcdef012346789abcdef\
012346789abcdef012346789abcdef012346789abcdef012346789abcde\"\x2029")
	};

const TPtrC cTextEmpty(_S(""));
const TPtrC cTextString(_S("aString"));
const TPtrC cTextQuote(_S("a quote\""));
const TPtrC cText2Quote(_S("two\"\""));
const TPtrC cTextMaxLen(_S("012346789abcdef012346789abcdef012346789abcdef012346789abcdef\
012346789abcdef012346789abcdef012346789abcdef012346789abcdef\
012346789abcdef012346789abcdef012346789abcdef012346789abcdef\
012346789abcdef012346789abcdef012346789abcdef012346789abcde"));

LOCAL_D const TOplTokenCheck test3StringTokens[]=
	{
	{ TOplToken::EConstant,TOplToken::EString,&cTextEmpty}, // Line 1 - word constants
	{ TOplToken::EConstant,TOplToken::EString,&cTextString},
	{ TOplToken::EConstant,TOplToken::EString,&cTextQuote},
	{ TOplToken::EConstant,TOplToken::EString,&cText2Quote},
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EConstant,TOplToken::EString,&cTextMaxLen}, // Line 2 - negative words
	{ TOplToken::EEos,TOplToken::EBadType,NULL},

	{ TOplToken::EEof,TOplToken::EBadType,NULL},
	{ TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};


void TTestModLex::Test3()
//
// Numeric constants
//
	{
	__UHEAP_MARK;
	GetLex();
	
	test.Start(_L("Decimal"));
	SOURCE(test3DecSource);
	CHECK_SOURCE(test3DecTokens);
	
	test.Next(_L("Decimal Errors"));
	// FOR NOW

	test.Next(_L("Hex"));
	SET_CHECK_SOURCE(test3HexSource,test3HexTokens);
  
	test.Next(_L("Hex Errors"));
	// FOR NOW
	
	test.Next(_L("Strings"));
	SET_CHECK_SOURCE(test3StringSource,test3StringTokens);

	test.Next(_L("Strings Errors"));
	// FOR NOW
	
	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}

LOCAL_D const TText test4OperatorSource[]=
	{
	_MYS("< > = + - * / % ( )\x2029\
	<= >= <> **\x2029\
	aNd oR NoT\x2029")
	};


LOCAL_D const TOplTokenCheck test4OperatorTokens[]=
	{
	{TOplToken::ELessThan,TOplToken::EBadType,NULL},
	{TOplToken::EGreaterThan,TOplToken::EBadType,NULL},
	{TOplToken::EEqual,TOplToken::EBadType,NULL},
	{TOplToken::EPlus,TOplToken::EBadType,NULL},
	{TOplToken::EMinus,TOplToken::EBadType,NULL},
	{TOplToken::EMultiply,TOplToken::EBadType,NULL},
	{TOplToken::EDivide,TOplToken::EBadType,NULL},
	{TOplToken::EPerc,TOplToken::EBadType,NULL},
	{TOplToken::EOpenBracket,TOplToken::EBadType,NULL},
	{TOplToken::ECloseBracket,TOplToken::EBadType,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},
	
	{TOplToken::ELessThanEq,TOplToken::EBadType,NULL},
	{TOplToken::EGreaterThanEq,TOplToken::EBadType,NULL},
	{TOplToken::ENotEqual,TOplToken::EBadType,NULL},
	{TOplToken::EPower,TOplToken::EBadType,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EAnd,TOplToken::EBadType,NULL},
	{TOplToken::EOr,TOplToken::EBadType,NULL},
	{TOplToken::ENot,TOplToken::EBadType,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EEof,TOplToken::EBadType,NULL},
	{TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};

LOCAL_D const TText test4PunctuationSource[]=
	{
	_MYS("; : , #\x2029")
	};

LOCAL_D const TOplTokenCheck test4PunctuationTokens[]=
	{
	{TOplToken::ESemiColon,TOplToken::EBadType,NULL},
	{TOplToken::EColon,TOplToken::EBadType,NULL},
	{TOplToken::EComma,TOplToken::EBadType,NULL},
	{TOplToken::EHash,TOplToken::EBadType,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EEof,TOplToken::EBadType,NULL},
	{TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};

void TTestModLex::Test4()
//
// Operators
//
	{
	__UHEAP_MARK;
	GetLex();

	test.Start(_L("Operators"));
	SOURCE(test4OperatorSource);
	CHECK_SOURCE(test4OperatorTokens);

	test.Next(_L("Punctuation"));
	SET_CHECK_SOURCE(test4PunctuationSource,test4PunctuationTokens);
	 
	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}



LOCAL_D const TText test5VariableSource[]=
	{
	_MYS("a% a1% abcdefg% a2b4c6h% µ% µµµµµµµ%\x2029\
	a& a1& abcdefg& a2b4c6h& µ& µµµµµµµ&\x2029\
	a a1 abcdefgh a2b4c6h8 µ µµµµµµµµ\x2029\
	a$ a1$ abcdefg$ a2b4c6h$ µ$ µµµµµµµ$\x2029\
	a%( a1%( abcdefg%( a2b4c6h%( µ%( µµµµµµµ%(\x2029\
	a&( a1&( abcdefg&( a2b4c6h&( µ&( µµµµµµµ&(\x2029\
	a( a1( abcdefgh( a2b4c6h8( µ( µµµµµµµµ(\x2029\
	a$( a1$( abcdefg$( a2b4c6h$( µ$( µµµµµµµ$(\x2029\
	a.a% b.a1% c.abcdefg% d.a2b4c6h% a.µ% b.µµµµµµµ%\x2029\
	c.a& d.a1& a.abcdefg& b.a2b4c6h& c.µ& d.µµµµµµµ&\x2029\
	a.a b.a1 c.abcdefgh d.a2b4c6h8 a.µ b.µµµµµµµµ\x2029\
	c.a$ d.a1$ a.abcdefg$ b.a2b4c6h$ c.µ$ d.µµµµµµµ$\x2029")
	};

LOCAL_D const TOplTokenCheck test5VariableTokens[]= // For now - no value checking
	{
	{TOplToken::ESimple,TOplToken::EWord,NULL},	
	{TOplToken::ESimple,TOplToken::EWord,NULL},
	{TOplToken::ESimple,TOplToken::EWord,NULL},	
	{TOplToken::ESimple,TOplToken::EWord,NULL},
	{TOplToken::ESimple,TOplToken::EWord,NULL},
	{TOplToken::ESimple,TOplToken::EWord,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::ESimple,TOplToken::ELong,NULL},	
	{TOplToken::ESimple,TOplToken::ELong,NULL},
	{TOplToken::ESimple,TOplToken::ELong,NULL},	
	{TOplToken::ESimple,TOplToken::ELong,NULL},
	{TOplToken::ESimple,TOplToken::ELong,NULL},
	{TOplToken::ESimple,TOplToken::ELong,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::ESimple,TOplToken::EReal,NULL},	
	{TOplToken::ESimple,TOplToken::EReal,NULL},
	{TOplToken::ESimple,TOplToken::EReal,NULL},	
	{TOplToken::ESimple,TOplToken::EReal,NULL},
	{TOplToken::ESimple,TOplToken::EReal,NULL},
	{TOplToken::ESimple,TOplToken::EReal,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::ESimple,TOplToken::EString,NULL},
	{TOplToken::ESimple,TOplToken::EString,NULL},
	{TOplToken::ESimple,TOplToken::EString,NULL},
	{TOplToken::ESimple,TOplToken::EString,NULL},
	{TOplToken::ESimple,TOplToken::EString,NULL},
	{TOplToken::ESimple,TOplToken::EString,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EArray,TOplToken::EWord,NULL},	
	{TOplToken::EArray,TOplToken::EWord,NULL},
	{TOplToken::EArray,TOplToken::EWord,NULL},	
	{TOplToken::EArray,TOplToken::EWord,NULL},
	{TOplToken::EArray,TOplToken::EWord,NULL},
	{TOplToken::EArray,TOplToken::EWord,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EArray,TOplToken::ELong,NULL},	
	{TOplToken::EArray,TOplToken::ELong,NULL},
	{TOplToken::EArray,TOplToken::ELong,NULL},	
	{TOplToken::EArray,TOplToken::ELong,NULL},
	{TOplToken::EArray,TOplToken::ELong,NULL},
	{TOplToken::EArray,TOplToken::ELong,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EArray,TOplToken::EReal,NULL},	
	{TOplToken::EArray,TOplToken::EReal,NULL},
	{TOplToken::EArray,TOplToken::EReal,NULL},	
	{TOplToken::EArray,TOplToken::EReal,NULL},
	{TOplToken::EArray,TOplToken::EReal,NULL},
	{TOplToken::EArray,TOplToken::EReal,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EArray,TOplToken::EString,NULL},	
	{TOplToken::EArray,TOplToken::EString,NULL},
	{TOplToken::EArray,TOplToken::EString,NULL},	
	{TOplToken::EArray,TOplToken::EString,NULL},
	{TOplToken::EArray,TOplToken::EString,NULL},
	{TOplToken::EArray,TOplToken::EString,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EField,TOplToken::EWord,NULL},	
	{TOplToken::EField,TOplToken::EWord,NULL},
	{TOplToken::EField,TOplToken::EWord,NULL},	
	{TOplToken::EField,TOplToken::EWord,NULL},
	{TOplToken::EField,TOplToken::EWord,NULL},
	{TOplToken::EField,TOplToken::EWord,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EField,TOplToken::ELong,NULL},	
	{TOplToken::EField,TOplToken::ELong,NULL},
	{TOplToken::EField,TOplToken::ELong,NULL},	
	{TOplToken::EField,TOplToken::ELong,NULL},
	{TOplToken::EField,TOplToken::ELong,NULL},
	{TOplToken::EField,TOplToken::ELong,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},


	{TOplToken::EField,TOplToken::EReal,NULL},	
	{TOplToken::EField,TOplToken::EReal,NULL},
	{TOplToken::EField,TOplToken::EReal,NULL},	
	{TOplToken::EField,TOplToken::EReal,NULL},
	{TOplToken::EField,TOplToken::EReal,NULL},
	{TOplToken::EField,TOplToken::EReal,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EField,TOplToken::EString,NULL},	
	{TOplToken::EField,TOplToken::EString,NULL},
	{TOplToken::EField,TOplToken::EString,NULL},	
	{TOplToken::EField,TOplToken::EString,NULL},
	{TOplToken::EField,TOplToken::EString,NULL},
	{TOplToken::EField,TOplToken::EString,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EEof,TOplToken::EBadType,NULL},
	{TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};


LOCAL_D const TText test5ProcedureSource[]=
	{
	_MYS("a%: a1%: abcdefg%: a2b4c6h%: µ%: µµµµµµµ%:\x2029\
	a&: a1&: abcdefg&: a2b4c6h&: µ&: µµµµµµµ&:\x2029\
	a: a1: abcdefgh: a2b4c6h8: µ: µµµµµµµµ:\x2029\
	a$: a1$: abcdefg$: a2b4c6h$: µ$: µµµµµµµ$:\x2029\
	@%( @&( @( @$(\x2029")
	};

LOCAL_D const TOplTokenCheck test5ProcedureTokens[]= // For now - no value checking
	{
	{TOplToken::EProcId,TOplToken::EWord,NULL},	
	{TOplToken::EProcId,TOplToken::EWord,NULL},
	{TOplToken::EProcId,TOplToken::EWord,NULL},	
	{TOplToken::EProcId,TOplToken::EWord,NULL},
	{TOplToken::EProcId,TOplToken::EWord,NULL},
	{TOplToken::EProcId,TOplToken::EWord,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EProcId,TOplToken::ELong,NULL},	
	{TOplToken::EProcId,TOplToken::ELong,NULL},
	{TOplToken::EProcId,TOplToken::ELong,NULL},	
	{TOplToken::EProcId,TOplToken::ELong,NULL},
	{TOplToken::EProcId,TOplToken::ELong,NULL},
	{TOplToken::EProcId,TOplToken::ELong,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EProcId,TOplToken::EReal,NULL},	
	{TOplToken::EProcId,TOplToken::EReal,NULL},
	{TOplToken::EProcId,TOplToken::EReal,NULL},	
	{TOplToken::EProcId,TOplToken::EReal,NULL},
	{TOplToken::EProcId,TOplToken::EReal,NULL},
	{TOplToken::EProcId,TOplToken::EReal,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EProcId,TOplToken::EString,NULL},
	{TOplToken::EProcId,TOplToken::EString,NULL},
	{TOplToken::EProcId,TOplToken::EString,NULL},
	{TOplToken::EProcId,TOplToken::EString,NULL},
	{TOplToken::EProcId,TOplToken::EString,NULL},
	{TOplToken::EProcId,TOplToken::EString,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EProcByName,TOplToken::EWord,NULL},	
	{TOplToken::EProcByName,TOplToken::ELong,NULL},	
	{TOplToken::EProcByName,TOplToken::EReal,NULL},	
	{TOplToken::EProcByName,TOplToken::EString,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EEof,TOplToken::EBadType,NULL},
	{TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};

LOCAL_D const TText test5LabelSource[]=
	{
	_MYS("a:: a1:: abcdefgh:: a2b4c6h8:: µ:: µµµµµµµµ::\x2029")
	};


LOCAL_D const TOplTokenCheck test5LabelTokens[]= // For now - no value checking
	{
	{TOplToken::ELabel,TOplToken::EWord,NULL},	
	{TOplToken::ELabel,TOplToken::EWord,NULL},
	{TOplToken::ELabel,TOplToken::EWord,NULL},	
	{TOplToken::ELabel,TOplToken::EWord,NULL},
	{TOplToken::ELabel,TOplToken::EWord,NULL},
	{TOplToken::ELabel,TOplToken::EWord,NULL},	
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EEof,TOplToken::EBadType,NULL},
	{TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};


void TTestModLex::Test5()
//
// Identifiers

// 
	{
	__UHEAP_MARK;
	GetLex();

	test.Start(_L("Variables"));
	SOURCE(test5VariableSource);
	CHECK_SOURCE(test5VariableTokens);

	test.Next(_L("Procedures"));
	SET_CHECK_SOURCE(test5ProcedureSource,test5ProcedureTokens);

	test.Next(_L("Labels"));
	SET_CHECK_SOURCE(test5LabelSource,test5LabelTokens);

	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}

LOCAL_D const TText test6KeywordSource[]=
	{
	_MYS("pRoC\x2029")
	};

LOCAL_D const TOplTokenCheck test6KeywordTokens[]= // For now - no value checking
	{
	{TOplToken::EProc,TOplToken::EBadType,NULL},
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EEof,TOplToken::EBadType,NULL},
	{TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};

void TTestModLex::Test6()
//
// checks all the keywords - these should all fail for the calculator
//
	{
	__UHEAP_MARK;
	GetLex();

	test.Start(_L("Keywords"));
	SOURCE(test6KeywordSource);
	CHECK_SOURCE(test6KeywordTokens);

	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}

LOCAL_D const TText test7FunctionSource[]=
	{
	_MYS("pI\x2029")
	};

LOCAL_D const TOplTokenCheck test7FunctionTokens[]= // For now - no value checking
	{
		{TOplToken::EFunction,TOplToken::EReal,NULL}, // PI 
	{TOplToken::EEos,TOplToken::EBadType,NULL},

	{TOplToken::EEof,TOplToken::EBadType,NULL},
	{TOplToken::EBadToken,TOplToken::EBadType,NULL}
	};

void TTestModLex::Test7()
//
// Function names - FOR NOW
//
	{
	
	__UHEAP_MARK;
	GetLex();

	test.Start(_L("All the names"));
	SOURCE(test7FunctionSource);
	CHECK_SOURCE(test7FunctionTokens);

	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}



void TTestModLex::RunTests()
//
// Runs all the tests for the OPL calculator lexer
//
	{
	
	test.Start(_L("Public interface"));
	Test1();
	
	test.Next(_L("Construct/Destruct alloc heaven"));
	Test2();

	test.Next(_L("Constants"));
	Test3();

	test.Next(_L("Operators and punctuation"));
	Test4();

	test.Next(_L("Identifiers"));
	Test5();

	test.Next(_L("Keywords"));
	Test6();
	
	test.Next(_L("Functions"));
	Test7();

	test.End();
	}

GLDEF_C TInt E32Main()
//
// Test the TBuf type.
//
	{
	CTrapCleanup *trapCleanup=CTrapCleanup::New();
	test.Title();

	test.Start(_L("OPL Module Lexer"));
	TTestModLex moduleTester;
	moduleTester.RunTests();

	test.End();
	test.Close();
	delete trapCleanup;
	return(0);
	}

