// TGRAPHIC.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#ifndef __TESTS_H__
#define __TESTS_H__


// Defined for testing keywords stand-alone

#define NO_SEQUENCER

#include <opcodes.h>

class COplTest : public CBase
	{
public:
	 virtual void DoTest(TInt aTestNo, CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)=0;
	};

class CTestGraphics	:public COplTest
	{
public:
	virtual void DoTest(TInt aTestNo, CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test1(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test2(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test3(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test4(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test5(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test6(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test7(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test8(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test9(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test10(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test11(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test12(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test13(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test14(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test15(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test16(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test17(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test18(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test19(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test20(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Test21(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	void Destruct();
private:
	};


#endif
