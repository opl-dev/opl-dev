// TMODTRAN.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Module translator test program

#include <e32test.h>
#include <f32file.h>
#include <fbs.h>
#include <opltran.h>
#include "textfile.h"
#include "oplreadr.h"
/*
#if defined(_DEBUG)
#define __TCBASE_H__
#include "tcbase.h"
#endif
*/

const TUint KSizeofSignature=16;
const TUint KSizeofHeader1=20;
const TUint KSizeofHeader2=12;
const TUint KQcodeSizeOffset=2;

LOCAL_D RTest test(_L("Module Translator Test"));
RFs TheFileServer;
ROplTextSystem TheTextSystem;

class TTestModTran
	{
public:
	void RunTests();
	TTestModTran();
private:
	void Test1(); // Constructors
	void Test2(); // Interface
	void Test3(); // Structure of a module file
	void Test4(); // Runtime Error location
	void TestTranslate(COplModuleTranslator *pMt,const TDesC& aName); // Does the translation of one of the test modules
	void CompareFilesL(const TDesC& aFile,const TDesC& anOther);
	void RuntimeErrors(TInt *, TInt);
private:
	};

class MAppCallBack : public MTranAppCB
	{
public:
	void OplTargetIsAppL(TFileName& aName);
	};

void MAppCallBack::OplTargetIsAppL(TFileName&)
	{
	}

TTestModTran::TTestModTran()
	{
	}


///////////////////////////////////////////////////////////
//
// TTestModTran
//
///////////////////////////////////////////////////////////

void TTestModTran::Test1()
//
// Checks out alloc heaven during construction/destruction
//
	{

	test.Start(_L("Construct/Destruct"));
#if defined(__TCBASE_H__)
   MAppCallBack junkTargetCB;

   T_STATIC1(COplModuleTranslator, COplModuleTranslator*, &COplModuleTranslator::NewL, MTranAppCB *,NULL); 
   T_STATIC1(COplModuleTranslator, COplModuleTranslator*, &COplModuleTranslator::NewL, MTranAppCB *,&junkTargetCB); 
#endif
	test.End();
	}

void TTestModTran::Test2()
//
//  Tests the interface (new & destruct done  in Test1
//
	{
	
	__UHEAP_MARK;

	test.Start(_L("NewL"));
	COplModuleTranslator* pMt=NULL;
	TRAPD(ret,pMt=COplModuleTranslator::NewL());
	test(ret==KErrNone);
	test(pMt!=NULL);
	
	test.Next(_L("Set Target"));
	TRAP(ret,pMt->SetTarget(EOplTargetER1));
	test(ret==KErrNone);
	
	test.Next(_L("Start Translate"));	
	TSourceTranslateError anError;
	_LIT(KOplName,"C:\\tOPLT\\EMPTY.OPL");
	TRAP(ret,pMt->StartTranslateL(TheTextSystem,KOplName(),_L("C:\\OPO\\EMPTY.OPO"),CTextTranslator::ERelease,anError));
	test(ret==KErrNone); // No procedures
			
	test.Next(_L("TranslateChunk"));

	ret=pMt->TranslateChunk();
	test(ret!=KErrNone && ret!=KErrEof); // A real error

	test.Next(_L("Error value"));
	test(anError.Error()==EErrSyntax);

	test.Next(_L("Error position"));
	test(anError.Position()==0); // ???

//	test.Next(_L("LocateError"));
	
//	anErrPos.SetLine(0);
//	anErrPos.SetOffset(0);
	
	test.Next(_L("Destruct"));
	delete pMt;	
	
	test.End();
	__UHEAP_MARKEND;
	}

const TInt KCleanupStackSeedDepth=100;
void PrimeCleanupStack()
	{
	// Before we start we pump up the Cleanup stack so that we don't allocat half way theough the test
	for (TInt i=0;i<KCleanupStackSeedDepth;i++)
		CleanupStack::PushL((TAny *)NULL);
	CleanupStack::Pop(KCleanupStackSeedDepth);
	}

void TTestModTran::Test3()
//
//
//
	{
	TRAPD(ret,PrimeCleanupStack());

	__UHEAP_MARK;
	test.Start(_L("Test Modules"));
	
	COplModuleTranslator* pMt=NULL;
	TRAP(ret,pMt=COplModuleTranslator::NewL());
	test(ret==KErrNone);
		
	TRAP(ret,pMt->SetTarget(EOplTargetER1));

	test(ret==KErrNone);
	test.Next(_L("Identifiers"));
//	__UHEAP_MARK; // Can't check the first one as this has some arrays which allocate their base cell
	TestTranslate(pMt,_L("IDENTS"));
	//	__UHEAP_MARKEND;
	
	test.Next(_L("Functions"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("FUNC"));
	__UHEAP_MARKEND;
	
	test.Next(_L("Structure"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("STRUCT"));
	__UHEAP_MARKEND;
	
	test.Next(_L("Keywords"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("KEYS"));
	__UHEAP_MARKEND;

	test.Next(_L("Extensions"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("EXTENDS"));
	__UHEAP_MARKEND;

	test(ret==KErrNone);
	test.Next(_L("Const"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("CONST"));
	__UHEAP_MARKEND;

#ifndef _DONT_TEST_THIS_
#pragma message("tmodtran.cpp: OPLT test code failing to handle WINDOWS/THUMB INCLUDE correctly...")
#else
   test.Next(_L("File Inclusion"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("INCLUDE"));
	__UHEAP_MARKEND;
#endif

#ifndef _DONT_TEST_THIS_
#pragma message("tmodtran.cpp: OPLT test code problems with OPX declarations.")
#else
   test.Next(_L("OPX declarations"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("DECL"));
	__UHEAP_MARKEND;
#endif

   test.Next(_L("Other Opler1 Extensions"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("OPLER1"));
	__UHEAP_MARKEND;

#ifndef _DONT_TEST_THIS_
#pragma message("tmodtran.cpp: OPLT test code failing to handle BUGS.TPL")
#else
   test.Next(_L("Regression Testing"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("BUGS"));
	__UHEAP_MARKEND;
#endif

	test.Next(_L("Casting"));
	__UHEAP_MARK;
	TestTranslate(pMt,_L("CASTING"));
	__UHEAP_MARKEND;

   delete pMt;	
	test.End();
	__UHEAP_MARKEND;
	}


void TTestModTran::TestTranslate(COplModuleTranslator *pMt,const TDesC& aName)
//
//
//
	{
	TFileName aSourceName=_L("C:\\tOPLT\\");
	aSourceName+=aName;
	TFileName aTargetName=_L("C:\\TEMP\\");

	aTargetName+=aName;
	aTargetName+=_L(".OPO");

	TSourceTranslateError anError;
	TRAPD(ret,pMt->StartTranslateL(TheTextSystem,aSourceName,aTargetName,CTextTranslator::ERelease,anError));
	test(ret==KErrNone);
	do
		{
		ret=pMt->TranslateChunk();
		test.Printf(_L("."));
		} while (ret==KErrNone);
	test.Printf(_L(" - %d, %d\r\n"), ret, anError.Position());
	test (ret==KErrGeneral);
	test (anError.Error()==EErrSuccess);
	
	TFileName aCompareName=_L("C:\\tOPLT\\");
	aCompareName+=aName;
	aCompareName+=_L(".REF");
	TRAP(ret,CompareFilesL(aTargetName,aCompareName));
	test(ret==KErrNone);
	TheFileServer.Delete(aTargetName); 
	}

void TTestModTran::CompareFilesL(const TDesC& aFile,const TDesC& anOther)
//
// It is not enough to do a byte by byte comparision since the file name is embedded
// at the start of the file. 
// The files used here are not that large so we just read in the whole thing each time.
//
	{
	test.Start(_L("Comparing files"));
	test.Next(_L("Opening Opos"));
	COpo* anOpo=COpo::NewLC(TheFileServer,aFile);
	COpo* anOtherOpo=COpo::NewLC(TheFileServer,anOther);
	test.Next(_L("Comparing contents"));
	test(*anOpo==*anOtherOpo);
	CleanupStack::PopAndDestroy(2);
	test.End();
	}

// Each of the arrays below, contain the source offsets corresponding to the 
// index qcode offset for each of the procedures.

LOCAL_D TInt procSw3_1257Offsets[]=
	{
	0x9f, 0x9f,		// 0000: Stack byte - 2E as Word 
	0xa1,			// 0002: Print Word 
	0xa1,			// 0003: Print CrLf 
	0xaa, 0xaa,		// 0004: Stack byte - 24 as Word 
	0xac,			// 0006: Print Word 
    0xac,			// 0007: Print CrLf 
    0xb5, 0xb5,		// 0008: Stack byte - 01 as Word 
	0xb6,			// 000A: Convert Word to Double 
	0xb7, 0xb7,		// 000B: Stack byte - 02 as Word 
	0xb8,			// 000D: Convert Word to Double 
	0xb9, 0xb9,		// 000E: Stack byte - 03 as Word 
	0xba,			// 0010: Convert Word to Double 
	0xbb, 0xbb,		// 0011: Stack byte - 25 as Word 
	0xba,			// 0013: Convert Word to Double 
	0xba,			// 0014: Percent Operator * 
	0xb8,			// 0015: Percent Operator + 
	0xb6,			// 0016: Percent Operator < 
	0xc0,			// 0017: Print Double 
	0xc0,			// 0018: Print CrLf 
	0xc2,			// 0019: Double Return Null 
	-1
	};

LOCAL_D TInt procXOffsets[]=
	{
	0xe9, 0xe9, 0xe9, // 0000: Direct Left Simple Double operand address -  0014: a
	0xeb, 0xeb, 0xeb, // 0003: Direct Right Simple Double operand address -  001C: b 
	0xed, 0xed, 0xed, // 0006: Direct Right Simple Double operand address -  0024: c 
    0xef, 0xef, 0xef, // 0009: Indirect Right Simple Double operand address -  0012: d
    0xee,			  // 000C: / operator acting on Double 
	0xec,             // 000D: + operator acting on Double 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, // 000E: Double constant  value - 27 
	0xf0,             // 0017: - operator acting on Double 
	0xf2,             // 0018: Assign Double 
	0xfd, 0xfd,       // 0019: Stack byte - 01 as Word 
	0xff, 0xff,       // 001B: Stack byte - 02 as Word 
	0x100,			  // 001D: Beep 
	0x102,             // 001E: Double Return Null 
	-1			      // That's all folks
	};

LOCAL_D TInt procYOffsets[]=
	{
	0x129, 0x129, 0x129, 0x129, 0x129, 0x129, 0x129, 0x129, 0x129,  // 0000: Double constant  value - 1 
	0x12d, 0x12d,			// 0009: Function - number 8F
	0x12d,				// 000B: Drop Double 
	0x138, 0x138, 0x138, 0x138, 0x138, 0x138, 0x138, 0x138, 0x138, // 000C: Double constant  value - 1 
	0x13d, 0x13d, 0x13d, 0x13d, 0x13d, 0x13d, 0x13d, 0x13d, 0x13d, // 0015: Double constant  value - 27 
	0x13c,				// 001E: + operator acting on Double 
	0x13e,				// 001F: Print Double 
	0x13e,				// 0020: Print CrLf 
	0x14e, 0x14e, 0x14e, 0x14e, 0x14e, 0x14e, 0x14e, 0x14e, 0x14e, // 0021: Double constant  value - 27 
	0x14f,				// 002A: Convert Double to Word 
	0x14f, 0x14f, 0x14f, 0x14f, 0x14f, // 002B: OpxCall  Opx Number:0  Opx function:1 
	0x150,				// 0030: Convert Long to Double 
	0x150, 0x150, 		// 0031: Function - number 8F
	0x150,				// 0033: Drop Double 
	0x153,				// 0034: Double Return Null 
	-1
	};

LOCAL_D TInt procZOffsets[]=
	{
	0x185, 0x185, 0x185,   // 0000: Direct Right Simple Double operand address -  0012: 
	0x187, 0x187, 0x187,   // 0003: Direct Right Simple Word operand address -  001A: 
    0x18a, 0x18a, 0x18a,   // 0006: Direct Right Simple String operand address -  001D: 
	0x18c, 0x18c, 0x18c, 0x18c, 0x18c, //0009: OpxCall  Opx Number:0  Opx function:2 
	0x18c,				// 000E: Drop Long 
	0x199, 0x199, 0x199,	// 000F: Direct Left Simple Double operand address -  0012: 
	0x199, 0x199, 		// 0012: Function - number 00
	0x19b, 0x19b, 0x19b,	// 0014: Direct Right Simple Word operand address -  001A: 
	0x19e, 0x19e, 0x19e,// 0017: Direct Right Simple String operand address -  001D: 
	0x1a0, 0x1a0, 0x1a0, 0x1a0, 0x1a0, // 001A: OpxCall  Opx Number:0  Opx function:3 
	0x1a0,              // 001F: Drop Long 
    0x1a3,              // 0020: Double Return Null 
	-1
	};

void TTestModTran::RuntimeErrors(TInt *aSourceOffsets,TInt aProcLine)
//
// sourceOffsets is an array containing the source offsets that
// correspond to integer qcode offsets in the proce starting at aProcLine
//
	{
	TRAPD(ret,PrimeCleanupStack());
	__UHEAP_MARK;

	COplModuleTranslator* pMt=NULL;
	TRAP(ret,pMt=COplModuleTranslator::NewL());
	test(ret==KErrNone);
	for (TInt offset=0;;offset++,aSourceOffsets++)
		{
		TSourceTranslateError anError;
		anError.SetPosition(offset);
		TRAP(ret,pMt->LocateErrorL(TheTextSystem,_L("C:\\tOPLT\\RUNLOC."),_L("C:\\TMP.OPO"),
			CTextTranslator::ERelease,anError,aProcLine));
		test(ret==KErrNone);
		do
			{
			ret=pMt->TranslateChunk();
			} while (ret==KErrNone);
		test.Printf(_L("."));
		if (*aSourceOffsets<0) // reached the end
			{
			test(ret==KErrGeneral); // So it should all translate fine
			test(anError.Error()==EErrSuccess);
			break;
			}
		test(ret==KErrGeneral);
		test(anError.Error()==EErrFound);
		test(anError.Position()==*aSourceOffsets);
		}
	test.Printf(_L("\r\n"));
	delete pMt;
	__UHEAP_MARKEND;
	}

struct LocTest
	{
	TInt* iOffsets;
	TInt iLineNumber;
	};

LOCAL_D LocTest rLocTests[]=
	{
	{procSw3_1257Offsets,4},
	{procXOffsets,13},
	{procYOffsets,23},
	{procZOffsets,29},
	{NULL,0},
	};

void TTestModTran::Test4()
//
// Checks out the runtime error location.
//
	{

	test.Start(_L(""));
	
	LocTest *pLocTest=rLocTests;
	while (pLocTest->iOffsets!=NULL)
		{
		test.Next(_L("Next"));
		RuntimeErrors(pLocTest->iOffsets,pLocTest->iLineNumber);
		pLocTest++;
		};
	test.End();
	}

void TTestModTran::RunTests()
//
// Runs through all the tests
//
	{
	
	test.Start(_L("Constructor destructor alloc heaven"));
	Test1();
	
	test.Next(_L("Public interface"));
	Test2();

	test.Next(_L("Basic module structure"));
	Test3();

#ifndef _DONT_TEST_THIS_
#pragma message("tmodtran.cpp: OPLT test code having problems with offset values")
// Is this a problem with old (ascii) offsets that weren't updated in the port to unicode???
#else
	test.Next(_L("Runtime error location"));
	Test4();
#endif

	test.End();
	}


GLDEF_C TInt E32Main()
//
// Test the file server.
//
	{
	CTrapCleanup *trapCleanup=CTrapCleanup::New();

	TheFileServer.Connect();
	TheTextSystem.Connect();
	// added by howard
	FbsStartup();
	RFbsSession::Connect();
	
	test.Title();
	TTestModTran moduleTester;
	test.Start(_L("Opl Module Translator"));
	moduleTester.RunTests();
	test.End();
	test.Close();
	
	RFbsSession::Disconnect();
	TheTextSystem.Close();
	TheFileServer.Close();
	delete trapCleanup;
	return(KErrNone);
	}

