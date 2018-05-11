// TEVLTRAN.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Evaluator translator test program

#include <e32test.h>
#include <f32file.h>
#include <opltran.h>

LOCAL_D RTest test(_L("Eval Translator Test"));

class TTestEvalTran
	{
public:
	void RunTests();
private:
	void Test1(); // Constructors
	void Test2(); // Interface
	void Test3(); // Structure of a module file
	};


void TTestEvalTran::RunTests()
//
// Runs through all the tests
//
	{
	test.Start(_L("Constructor destructor alloc heaven"));
	TRAPD(ret,Test1());
	test(ret==KErrNone);
	
	test.Next(_L("Public interface"));
	TRAP(ret,Test2());
	test(ret==KErrNone);

	test.Next(_L("Basic qcode generation"));
	TRAP(ret,Test3());
	test(ret==KErrNone);

	test.End();
	
	}

void TTestEvalTran::Test1()
//
// Checks out alloc heaven during construction/destruction
//
	{

	test.Start(_L("Construct/Destruct"));
#if defined(__TCBASE_H__)

   T_STATIC0(COplEvalTranslator, COplEvalTranslator*, &COplEvalTranslator::NewL); 
   T_STATIC0(COplEvalTranslator, COplEvalTranslator*, &COplEvalTranslator::NewLC);
   T_STATIC0(COplEvalQcode, COplEvalQcode*, &COplEvalQcode::NewL); 
   T_STATIC0(COplEvalQcode, COplEvalQcode*, &COplEvalQcode::NewLC); 
#endif
	test.End();
	}


void TTestEvalTran::Test2()
//
//  Tests the interface (new & destruct done  in Test1
//
	{
	
//	__UHEAP_MARK; // Can't check the heap on this as the cleanup stack shafts us
			      // by reallocating it's list in the middle of all this

	test.Start(_L("NewLC"));	
	COplEvalTranslator* pEt=NULL;
	pEt=COplEvalTranslator::NewLC();
	test(pEt!=NULL);
	CleanupStack::PopAndDestroy();

	test.Next(_L("NewL"));	
	pEt=NULL;
	TRAPD(ret,pEt=COplEvalTranslator::NewL());
	test(ret==KErrNone);
	test(pEt!=NULL);

	TTranslateError anError;
	test.Next(_L("Translate"));

	CBufFlat *pQc=CBufFlat::NewL(32);
	TInt maxStackDepth;
	ret=pEt->Translate(_L("1+2"),anError,*pQc,maxStackDepth);
	test(ret==KErrNone);
	test(maxStackDepth==16);
	
	test.Next(_L("LocateError"));	
	anError.SetPosition(6); // Addtions
	ret=pEt->LocateError(_L("1+2"),anError);
	test(ret==KErrNone);

	delete pQc;
	delete pEt;	
	
	test.End();
//	__UHEAP_MARKEND;
	}

#define Klbc2 0x00
#define KTypePadding 0x00
// sin(PI)+fred%:-addr(jim)+len(sally$($1))
static TUint8 test3ExpectedQcode[]=
	{
	0x57, 0x8c,  // Function PI
	0x57, 0x8f,  // Function Sin
	0x2b, 0x04, Klbc2, 'F', 0x00, 'R', 0x00, 'E', 0x00, 'D', 0x00, // Constant FRED
	0x6b, 0x00, '%', // Call Proc by name 
	0x7c,		// Convert word to double
	0x4a,		// Add Real
	0xff, 0x32, // Eval left side external
	0x03, Klbc2, 'J', 0x00, 'I', 0x00, 'M', 0x00, // JIM
	0x02, KTypePadding, // Type double
	0x57, 0x00, // Function Addr
	0x7d, // Word to double - Addr returns long under EPOC32
	0x4e, // subtract double
	0x4f, 0x01, // Stack 1 as word
	0xff, 0x31, // Right side eval external
	0x06, Klbc2, 'S', 0x00, 'A', 0x00, 'L', 0x00, 'L', 0x00, 'Y', 0x00, '$', 0x00,
	0x83, KTypePadding, // String array
	0x57, 0x14, // LEN
	0x7c, // Word to double
	0x4a,		// Add Real
	0xff,0x21		// Return
	};

const TInt KTest3MaxStackDepth=16;
void TTestEvalTran::Test3()
	{
//	__UHEAP_MARK;

	test.Start(_L("Getting all the bits together"));
	COplEvalTranslator *pT=COplEvalTranslator::NewLC();
	TTranslateError anError;
	CBufFlat *pQc=CBufFlat::NewL(32);
	CleanupStack::PushL(pQc);
	TInt maxStackDepth;

	test.Next(_L("Translating"));
	TInt ret=pT->Translate(_L("sin(PI)+fred%:-addr(jim)+len(sally$($1))"),anError,*pQc,maxStackDepth);
	test(ret==KErrNone);
	
	test.Next(_L("Checking the results"));
	test(maxStackDepth==KTest3MaxStackDepth);
	test(pQc->Ptr(0).Compare(TPtrC8(test3ExpectedQcode,sizeof(test3ExpectedQcode)))==0);
	CleanupStack::PopAndDestroy(2);
	test.End();
//	__UHEAP_MARKEND;
	}

GLDEF_C TInt E32Main()
//
// Test the file server.
//
	{
	__UHEAP_MARK;
	CTrapCleanup *trapCleanup=CTrapCleanup::New();

	test.Title();
	TTestEvalTran evalTester;
	test.Start(_L("Opl Eval Translator"));
	evalTester.RunTests();
	test.End();
	test.Close();

	delete trapCleanup;
	__UHEAP_MARKEND;
	return(KErrNone);
	}

