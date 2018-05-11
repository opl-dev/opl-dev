// TCALCLEX.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Test COplLineLex
//
//

#include <e32test.h>
#include <opllex.h>

LOCAL_D RTest test(_L("TCALCLEX"));


class TLineLexerError
	{
public:
	const TText *iText;
	TInt iError;
	TInt iOffset;
	};

class TTestLineLexer
	{
public:
	inline TTestLineLexer() : iLex(NULL) {};
	void RunTests();	
private:
	void Test1(); // Interface
	void Test2(); // Constructor alloc heaven
	void Test3(); // Constants 
	void Test4(); // Operators and punctuation 
	void Test5(); // Identifiers
	void Test7(); // Functions
private:	
	COplLineLexer *GetLex();
	void DestroyLex();
	void CheckLine(const TText * const aLine, TUint aCount, TOplToken aToken, TOplToken::TType aType);
	void CheckErrors(const TLineLexerError *errors);
	TOplToken MarkLex(); // Wraps iLex.Lex() in __UHEAP_MARK/__UHEAP_MARKEND
	TInt MarkError(TOplToken &aToken); // Lea is expected to leave with an error - checks start end stuff
private:
	COplLineLexer *iLex;
	};



COplLineLexer *TTestLineLexer::GetLex()
//
// gets a lexer 
//
	{
	
	TRAPD(r,iLex=COplLineLexer::NewL());
	test(r==KErrNone);
	return iLex;
	}

void TTestLineLexer::DestroyLex()
//
// Chucks away the lexer
//
	{
	
	delete iLex;
	iLex=NULL;
	}


TOplToken TTestLineLexer::MarkLex()
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

TInt TTestLineLexer::MarkError(TOplToken &aToken)
//
// Wraps failed lex __UHEAP_MARK/__UHEAP_MARKEND
//
	{
	
	__UHEAP_MARK;
	TRAPD(err,aToken=iLex->LexL());
	__UHEAP_MARKEND;
	return err;	
	}

void TTestLineLexer::CheckErrors(const TLineLexerError *errors)
//
// Trucks through an array of errors checking we get teh right error in the right place
//
	{
	
	while (errors->iText!=NULL)
		{
		iLex->SetExpression(TPtrC(errors->iText));

		TInt err;
		TOplToken token;
		do
			{
			err=MarkError(token);
			} while (err==KErrNone && token!=TOplToken::EEos);

		test(err==errors->iError);
		test(iLex->TokenOffset()==errors->iOffset); // Check the rror occured in the right place
		errors++;
		}	
	}

void TTestLineLexer::CheckLine(const TText * const aLine,TUint aCount,TOplToken aToken,TOplToken::TType aType)
//
// Line should contain aCount tokens aToken.
//	
	{

	
	__UHEAP_MARK;
	iLex->SetExpression(TPtrC(aLine));
	while (aCount--)
		{
		test(MarkLex()==aToken);
		test(iLex->Type()==aType);
		}
	test(MarkLex()==TOplToken::EEos);
	__UHEAP_MARKEND;
	}


void TTestLineLexer::Test1()
//
// All public funcitons
//
	{

	__UHEAP_MARK;
	
	test.Start(_L("New"));
	GetLex();
	
	test.Next(_L("Set expression"));
	iLex->SetExpression(_L("1"));

	test.Next(_L("Error offset"));
	test(iLex->TokenOffset()==0);
	
	test.Next(_L("Lex"));
	test(MarkLex()==TOplToken::EConstant);
	
	test.Next(_L("Type"));
	test(iLex->Type()==TOplToken::EReal);

	test.Next(_L("UnLex"));
	iLex->UnLex();
	test(iLex->TokenOffset()==0);

	test(MarkLex()==TOplToken::EConstant);
	test(iLex->Type()==TOplToken::EReal);

	test.Next(_L("Set Target"));
	iLex->SetTarget(EOplTargetOpl1993);

	DestroyLex();
	test.End();
	__UHEAP_MARKEND;
	}


void TTestLineLexer::Test2()
//
// Alloc heaven in construction
// Fails each alloc in turn.
//
	{
	
	test.Start(_L("Constructors"));
	
	iLex=NULL; // Just to be sure 
   TUint failNum=1;
   FOREVER
		{		
		__UHEAP_MARK;
		
		__UHEAP_SETFAIL(RHeap::EDeterministic,failNum);
		TRAPD(ret,iLex=COplLineLexer::NewL());
		__UHEAP_RESET;
		
		if (ret==0)
			{
			test(iLex!=NULL);
			break;
			}
			
		__UHEAP_MARKEND;
       failNum++; 
      }
#if defined(_DEBUG)
//	test(failNum!=1); // Check that we allocate at least one cell fo rthe lexer (it is CBase after all)
#endif
	test.Next(_L("Destructor"));
	DestroyLex();
	__UHEAP_MARKEND;
	
	test.End();
	}


LOCAL_D TLineLexerError badDecimals[]=
	{
	{_S("1 ."),EErrBadNumber,2},
	{_S("1 1.2e"),EErrBadNumber,2},
	{_S("33  1.2e"),EErrBadNumber,4},
	{_S("1e1000"),EErrBadNumber,0},
	{_S("1e-1000"),EErrBadNumber,0},
	{NULL,0,0}
	};

LOCAL_D TLineLexerError badHexadecimals[]= // FOR NOW - note these are all one too big because of the Mark bugs
	{
	{_S("$"),EErrBadNumber,0},
	{_S("1 &"),EErrBadNumber,2},
	{_S("$1 $g"),EErrBadNumber,3},
	{_S("&1 &g"),EErrBadNumber,3},
	{_S("$ffff $10000"),EErrBadNumber,6},
	{_S("&ffffffff &100000000"),EErrBadNumber,10},
	{NULL,0,0}
	};

LOCAL_D TLineLexerError badStrings[]=
	{
	{_S("\""),EErrMissingQuote,0},
	{_S("12 \"abcd"),EErrMissingQuote,3},
	{_S("\"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef\
0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef\
0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef\
0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef\""),EErrBadStringLength,0},
	{NULL,0,0}
	};

void TTestLineLexer::Test3()
//
// Numeric constants
//
	{
	__UHEAP_MARK;
	GetLex();
	
	test.Start(_L("Decimal"));

	CheckLine(_S("0 1 127 128 32767"),5,TOplToken::EConstant,TOplToken::EReal);
	CheckLine(_S("32768 65535 65536 2147483647"),4,TOplToken::EConstant,TOplToken::EReal);
	CheckLine(_S("1.0 .1 32767.0 32768.0 217483647.0 214783648 1e1 1E+1 1e-1"),9,TOplToken::EConstant,TOplToken::EReal);

	test.Next(_L("Decimal Errors"));
	CheckErrors(badDecimals);

	test.Next(_L("Hex"));	
	CheckLine(_S("$0 $1 $7f $80 $7fff $ffff $ff81 $ff80 $8000"),9,TOplToken::EConstant,TOplToken::EWord);
	CheckLine(_S("&8000 &7fffffff &ffffffff &ffffff81 &ffffff80 &ffff8001 &ffff8000"),7,TOplToken::EConstant,TOplToken::ELong);

	test.Next(_L("Hex errors"));
	CheckErrors(badHexadecimals);
	
	test.Next(_L("Strings"));
	CheckLine(_S("\"\" \"abcd\" \"1234\" \"\"\"\""),4,TOplToken::EConstant, TOplToken::EString);
	
	test.Next(_L("String errors"));
	CheckErrors(badStrings);

	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}

 
LOCAL_D TLineLexerError badCharacters[]=
	{
	{_S("~"),EErrBadCharacter,0},
	{_S("`"),EErrBadCharacter,0},
	{_S("!"),EErrBadCharacter,0},
	{_S("^"),EErrBadCharacter,0},
	{_S("["),EErrBadCharacter,0},
	{_S("]"),EErrBadCharacter,0},
	{_S("|"),EErrBadCharacter,0},
	{_S("\\"),EErrBadCharacter,0},
	{_S("'"),EErrBadCharacter,0},
	{_S("?"),EErrBadCharacter,0},
	{NULL,0,0}
	};

LOCAL_D TLineLexerError badPBN[]=
	{
	{_S("@"),EErrSyntax,0},
	{_S("@%"),EErrSyntax,0},
	{_S("@&"),EErrSyntax,0},
	{_S("@$"),EErrSyntax,0},
	{_S("@^("),EErrSyntax,0},
	{_S("@%( @"),EErrSyntax,4},
	{_S("@$( @%"),EErrSyntax,4},
	{_S("@( @&"),EErrSyntax,3},
	{_S("@$( @$"),EErrSyntax,4},
	{_S("@&( @^("),EErrSyntax,4},
	{NULL,0,0}
	};

void TTestLineLexer::Test4()
//
// Operators
//
	{
	
	__UHEAP_MARK;
	GetLex();
	
	test.Start(_L("Single character operators"));
	CheckLine(_S("<"),1,TOplToken::ELessThan,TOplToken::EBadType);
	CheckLine(_S(">"),1,TOplToken::EGreaterThan,TOplToken::EBadType);
	CheckLine(_S("="),1,TOplToken::EEqual,TOplToken::EBadType);
	CheckLine(_S("+"),1,TOplToken::EPlus,TOplToken::EBadType);
	CheckLine(_S("-"),1,TOplToken::EMinus,TOplToken::EBadType);
	CheckLine(_S("*"),1,TOplToken::EMultiply,TOplToken::EBadType);
	CheckLine(_S("/"),1,TOplToken::EDivide,TOplToken::EBadType);
	CheckLine(_S("%"),1,TOplToken::EPerc,TOplToken::EBadType);
	CheckLine(_S("("),1,TOplToken::EOpenBracket,TOplToken::EBadType);
	CheckLine(_S(")"),1,TOplToken::ECloseBracket,	TOplToken::EBadType);

	test.Next(_L("Double character operators"));
	CheckLine(_S("<="),1,TOplToken::ELessThanEq,TOplToken::EBadType);
	CheckLine(_S(">="),1,TOplToken::EGreaterThanEq,TOplToken::EBadType);
	CheckLine(_S("<>"),1,TOplToken::ENotEqual,TOplToken::EBadType);
	CheckLine(_S("**"),1,TOplToken::EPower,TOplToken::EBadType);
	
	test.Next(_L("Text operators"));
	CheckLine(_S("and AND aNd"),3,TOplToken::EAnd,TOplToken::EBadType);
	CheckLine(_S("or OR oR"),3,TOplToken::EOr,TOplToken::EBadType);
	CheckLine(_S("not NOT NoT"),3,TOplToken::ENot,TOplToken::EBadType);
	
	test.Next(_L("Punctuation"));
	CheckLine(_S(":"),1,TOplToken::EColon,TOplToken::EBadType);
	CheckLine(_S(";"),1,TOplToken::ESemiColon,TOplToken::EBadType);
	CheckLine(_S(","),1,TOplToken::EComma,TOplToken::EBadType);
	CheckLine(_S("#"),1,TOplToken::EHash,TOplToken::EBadType);
	

	test.Next(_L("Bad characters"));
	CheckErrors(badCharacters);
		

	test.Next(_L("Proc by name"));
	CheckLine(_S("@%("),1,TOplToken::EProcByName,TOplToken::EWord);
	CheckLine(_S("@&("),1,TOplToken::EProcByName,TOplToken::ELong);
	CheckLine(_S("@("),1,TOplToken::EProcByName,TOplToken::EReal);
	CheckLine(_S("@$("),1,TOplToken::EProcByName,TOplToken::EString);

	test.Next(_L("Bad PBN"));
	CheckErrors(badPBN);

	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}


LOCAL_D TLineLexerError badProcs[]=
	{
	{_S("a%%:"),EErrBadSuffices,0},
	{_S("A%::"),EErrBadSuffices,0},
	{_S("fedcba9876543210fedcba9876543210%:"),EErrIdentifierTooLong,0},
	{_S("fedcba9876543210fedcba9876543210a:"),EErrIdentifierTooLong,0},
	{_S("a1234567890123456789012345678901%:"),EErrIdentifierTooLong,0},
	{_S("a12345678901234567890123456789012:"),EErrIdentifierTooLong,0},
	{_S("A%: a%%:"),EErrBadSuffices,4},
	{_S("a%: A%::"),EErrBadSuffices,4},
	{_S("FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210\
FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210\
FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210\
FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"),EErrIdentifierTooLong,0},
	{NULL,0,0}
	};

void TTestLineLexer::Test5()
//
// Identifiers - mostly fail for the calculator.
// Allows memory names and procedure calls.
// 
	{
	__UHEAP_MARK;
	GetLex();

	test.Start(_L("Procedure Names"));
	CheckLine(_S("a%: a1%: abcdefg%: a234567%: µ%: µµµµµµµ%:"),6,TOplToken::EProcId,TOplToken::EWord);
	CheckLine(_S("a&: a1&: abcdefg&: a234567&: µ&: µµµµµµµ&:"),6,TOplToken::EProcId,TOplToken::ELong);
	CheckLine(_S("a: a1: abcdefgh: a2345678: µ: µµµµµµµµ:"),6,TOplToken::EProcId,TOplToken::EReal);
	CheckLine(_S("a$: a1$: abcdefg$: a234567$: µ$: µµµµµµµ$:"),6,TOplToken::EProcId,TOplToken::EString);
	
	test.Next(_L("Bad procedures"));
 	CheckErrors(badProcs);

	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}

void TTestLineLexer::Test7()
//
// Function names - FOR NOW
//
	{
	
	__UHEAP_MARK;
	GetLex();

	test.Start(_L("All the names"));
	CheckLine(_S("PI pI pi"),3, TOplToken::EFunction,TOplToken::EReal);

	test.End();
	DestroyLex();
	__UHEAP_MARKEND;
	}



void TTestLineLexer::RunTests()
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

// There is no longer a Test 6 - perhaps there should be.
	
	test.Next(_L("Functions"));
	Test7();

	test.End();
	}

GLDEF_C TInt E32Main()
//
// Test the TBuf type.
//
	{

	test.Title();

	test.Start(_L("OPL Calculator Lexer"));
	TTestLineLexer calcTester;
	calcTester.RunTests();

	test.End();
	
	test.Close();
	return(0);
	}

