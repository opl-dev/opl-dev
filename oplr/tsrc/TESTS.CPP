// TESTS.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "tests.h"

class CTestAbs	:public COplTest
	{
public:
	virtual void DoTest();
private:
	void AbsTest(TInt errNo,TReal64 expect,TReal64 value);
	};

void CTestAbs::AbsTest(TInt errNo,TReal64 expect,TReal64 value)
	{
	COplRuntime& rt=*TheRuntime();
	rt.Stack().Push(value); // push arguments
	FuncOpCode::Abs(rt.Stack(),rt,(CFrame*)NULL); // this is the fuction on test

	TReal returned=rt.Stack().PopReal64(); // pop result

	if (COplTest::CmpReal(expect,returned))
		{
		TBuf<80> buf;
		buf.Format(_L("Error in Abs :  %d \r\n"),errNo);
		rt.Console().Write(buf);
		}
	}


typedef struct
	{
	TInt errNo;
	TReal expect;
	TReal value;
	} TEST_ABS;

void CTestAbs::DoTest()
	{
	TEST_ABS absData[]=
		{
		{ 1,0.5,0.5},
		{ 2,2.7,-2.7},
		{ 3,20000.0,-47.9} // this should produce an error!!!
		};
	TEST_ABS *p,*pE;
    for (p=absData,pE=absData+(sizeof(absData)/sizeof(TEST_ABS));p<pE;p++)
        AbsTest(p->errNo,p->expect,p->value);
	}


TInt COplTest::CmpReal(TReal aReal1,TReal aReal2)
	{
	TRealFormat format;
	format.iType=EGeneral;
	format.iWidth=12;
	format.iDecimalPlaces=0;
	format.iPoint='.';
	format.iTriad=',';
	format.iTriLen=3;
	TBuf<80> buf1,buf2;
	buf1.Num(aReal1,format);
	buf2.Num(aReal2,format);
	return buf1.Compare(buf2);
	}

void COplRuntime::RunTests()	   // This is where to add your tests
	{
	iTest=new(ELeave) CTestAbs;	// create it
	iTest->DoTest();			// test it
	Adt::Destroy(iTest);		// destroy it
	}

