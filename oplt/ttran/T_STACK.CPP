// T_STACK.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#pragma warning( disable : 4710 )	// Function not inlined
#include <e32base.h>
#include <opltdef.h>
#include <oplstack.h>
#include <e32test.h>

LOCAL_D RTest test(_L("T_STACK"));
const TUint KItemBufSize=0x10;


class TStackTestItem
	{
public:
	TStackTestItem(TInt aNo);
	inline TStackTestItem() {}
	TStackTestItem &operator=(TInt aNo);
	TInt operator==(TInt aNo);	
	inline TInt operator==(TStackTestItem anItem) {return *this==anItem.iNo;}	
	inline TInt operator<=(TInt aNo) { return iNo<=aNo;}
	inline TInt operator<(TInt aNo) { return iNo<aNo;}
	inline TInt operator++() { Set(++iNo); return iNo;}
	inline TInt operator>=(TInt aNo) { return iNo>=aNo;}
	inline TInt operator>(TInt aNo) { return iNo>aNo;}
	inline TInt operator--() { Set(--iNo); return iNo;}
private:
	void Set(TInt aNo);
	TInt iNo;
	TBuf<KItemBufSize> iString;
	};

template <class S,class T>
class TTestStack
	{
public:
	TTestStack();
	void RunTests();
private:
	void AllTests();
	void Test1(); // Constructors
	void Test2(); // Interface
	void Test3(); // Basic behaviour
	void Test4(); // bounds
	static TInt TestDropPanic(TAny *aStack);
	static TInt TestTopPanic(TAny *aStack);
	static TInt TestPopPanic(TAny *aStack);
	static TInt TestPeekPanic(TAny *aStack);
	static TInt TestPickPanic(TAny *aStack);
	};


void TStackTestItem::Set(TInt aNo)
//
//
//
	{
	iNo=aNo;
	iString.Fill(TChar(aNo),KItemBufSize);
	}


TStackTestItem::TStackTestItem(TInt aNo)
//
// CTor
//	
	{

	
	Set(aNo);
	}

TStackTestItem &TStackTestItem::operator=(TInt aNo)
	{
	
	Set(aNo);
	return *this;
	}

TInt TStackTestItem::operator==(TInt aNo)
//
// Comparison to make sure we have the right thing
//
	{
	
	if (aNo!=iNo)
		return (aNo==iNo);
	TBuf<KItemBufSize> tmp;
	tmp.Fill(TChar(aNo),KItemBufSize);
	return iString==tmp ;
	}

template <class S,class T>
TTestStack<S,T>::TTestStack()
	{

	}

template <class S,class T>
void TTestStack<S,T>::Test1()
//
// Interface - cheks everything's in the library
//
	{
	
	__UHEAP_MARK;
	test.Start(_L("Constructor"));
	S *pStack=new S(16);
	test(TRUE);
	
	test.Next(_L("PushL"));
	T seed=0;
	pStack->PushL(&seed);
	test(TRUE);

	test.Next(_L("Pop"));
	T pop;
	pStack->Pop(&pop);
	test(TRUE);

	test.Next(_L("Top"));
	pStack->PushL(&seed);
	T top=pStack->Top();
	test(TRUE);

	test.Next(_L("Depth"));
	test(pStack->Depth()==1);

	test.Next(_L("Peek"));
	top=pStack->Peek(0);
	test(TRUE);

	test.Next(_L("Pick"));
	pStack->Pick(&pop,0);
	test(TRUE);

	test.Next(_L("Drop"));
	pStack->PushL(&seed);
	pStack->Drop();
	test(TRUE);

	test.Next(_L("Compress"));
	pStack->Compress();
	test(TRUE);

	test.Next(_L("Conversion to (CBase *) & Destruct"));
	delete pStack;
	test(TRUE);

	test.End();
	__UHEAP_MARKEND;
	}


template <class S,class T>
void TTestStack<S,T>::Test2()
//
// Constructors and granularity
//
	{
	S *pStack=NULL;

	test.Start(_L("Ctor alloc heaven")); // Check constructor works mnand there's nothingleft lying around 
	__UHEAP_MARK;
	pStack=new S(16);
	test(pStack!=NULL);
	delete pStack;
	__UHEAP_MARKEND;		

	
	test.Next(_L("1st push")); // Likewise allocation during first push
	__UHEAP_MARK;	
	pStack=new S(16);
	
	T item=1;
#if defined(_DEBUG)
	__UHEAP_MARK;
	__UHEAP_FAILNEXT(1);
	TRAPD(ret,pStack->PushL(&item));
//#if defined(_UNICODE)
//	test(ret!=KErrNoMemory);
//	__UHEAP_RESET;
//#else
	test(ret==KErrNoMemory);
	__UHEAP_RESET;
	__UHEAP_MARKEND;
	TRAP(ret,pStack->PushL(&item));
//#endif
#else
	TRAPD(ret,pStack->PushL(&item));
#endif
	
	delete pStack;
	__UHEAP_MARKEND;


// Doesn't work as can't currently force ReAlloc to fail.
//	test.Next(_L("Granularity")); // Allocation to do with granularity
//	// Once you've added one item, you should fail gran items later
//	for (TInt gran=4;gran<17;gran++)
//		{
//		__UHEAP_MARK;
//		pStack=new S(gran);
//		T item=0;
//		pStack->PushL(&item);		
//		__UHEAP_FAILNEXT(1);
//		TInt ret,itemNo;
//		for (itemNo=1;;itemNo++)
//			{
//			TRAPD(ret,pStack->PushL(&item));			
//			if (ret!=0)
//				break;
//			}
//		test(ret!=0); // We did fail
//		test(itemNo>gran && itemNo<(gran*2)); // in the right place
//		__UHEAP_RESET;
//		Adt::Destroy(pStack);
//		__UHEAP_MARKEND;
//		}
	test.End();
	}


template <class S,class T>
void TTestStack<S,T>::Test3()
//
// Basic behaviour
//
	{
	
	const TInt KTest3Gran=4;
	const TInt KTest3ItemCount=32; // Must be multiple of 4, also >gran*2 to get in a couple of allocs 

	__UHEAP_MARK;
	test.Start(_L("Push"));
	S *pStack=new(ELeave) S(KTest3Gran);
	T seed=0;
	while (seed<KTest3ItemCount)
		{
		pStack->PushL(&seed);
		++seed;
		test(seed==TInt(pStack->Depth()));
		}
			
	test.Next(_L("Peek")); // Check that everything looks to be in the right place
	TInt i;
	for (i=0;i<KTest3ItemCount;i++)
		test(pStack->Peek(i)==KTest3ItemCount-1-i);
		
	test.Next(_L("Pick")); // slip out all the even ones
	T pick;
	for (i=0;i<KTest3ItemCount;)
		{
		pStack->Pick(&pick,KTest3ItemCount-1-i);
		test(pick==i);
		i+=2;
		test(TInt(pStack->Depth())==KTest3ItemCount-i/2);
		}	

	test.Next(_L("Top, Drop & Pop"));
	for (i=KTest3ItemCount-1;i>=0;)
		{
		test(pStack->Top()==i);
		pStack->Drop();
		i-=2;
		test(pStack->Top()==i);
		pStack->Pop(&pick);
		test(pick==i);					
		i-=2;
		}


	delete pStack;
	test.End();

	__UHEAP_MARKEND;
	}


template <class S,class T>
TInt TTestStack<S,T>::TestDropPanic(TAny *aStack)
//
// Checks that dropping an item off an empty stack fails
//
	{
	
	((S *)aStack)->Drop();
	test(FALSE);
	return KErrNone;
	}

template <class S,class T>
TInt TTestStack<S,T>::TestPopPanic(TAny *aStack)
//
// Checks that popping an item off an empty stack fails
//
	{
	T item;

	((S *)aStack)->Pop(&item);
	test(FALSE); // shouldn't get here
	return KErrNone;
	}


template <class S,class T>
TInt TTestStack<S,T>::TestTopPanic(TAny *aStack)
//
// Checks that trying to look at the top of an empty stack fails
//
	{

	((S *)aStack)->Top();
	test(FALSE);
	return KErrNone;
	}

template <class S,class T>
TInt TTestStack<S,T>::TestPeekPanic(TAny *aStack)
//
// Checks that dropping an item off an empty stack fails
//
	{
	
	((S *)aStack)->Peek(1);
	test(FALSE);
	return KErrNone;
	}

template <class S,class T>
TInt TTestStack<S,T>::TestPickPanic(TAny *aStack)
//
// Checks that dropping an item off an empty stack fails
//
	{
	T item;
	
	((S *)aStack)->Pick(&item,1);
	test(FALSE);
	return KErrNone;
	}

void RunPanicThread(const TDesC &aName,TThreadFunction aFunc,TAny *aVal)
//
//
//
	{
	RThread thread;
	TRequestStatus stat;

	test(thread.Create(aName,aFunc,KDefaultStackSize,0x200,0x200,aVal)==KErrNone);
	thread.Logon(stat);
	thread.Resume();
	User::WaitForRequest(stat);
	test(thread.ExitReason()!=KErrNone);
	test(thread.ExitType()==EExitPanic);
	thread.Close();
	}

template <class S,class T>
void TTestStack<S,T>::Test4()
//
// bounds checking
//
	{
	// don't want just in time debugging as we trap panics
	TBool justInTime=User::JustInTime(); 
	User::SetJustInTime(EFalse); 

	const TUint KTest4StackGran=0x16;


	__UHEAP_MARK;
	S *pStack=new(ELeave) S(KTest4StackGran);
	
	test.Start(_L("Drop panic"));
	RunPanicThread(_L("TestDropPanic"),TTestStack<S,T>::TestDropPanic,pStack);
	
	test.Next(_L("Top panic"));
	RunPanicThread(_L("TestTopPanic"),TTestStack<S,T>::TestTopPanic,pStack);
	
	test.Next(_L("Pop panic"));
	RunPanicThread(_L("TestPopPanic"),TTestStack<S,T>::TestPopPanic,pStack);


	test.Next(_L("Peek Panic"));
	T item=1; 		
	pStack->PushL(&item);
	pStack->Peek(0);
	RunPanicThread(_L("TestPeekPanic"),TTestStack<S,T>::TestPeekPanic,pStack);

	test.Next(_L("Pick panic"));
	item=2;
	pStack->PushL(&item);
	pStack->Pick(&item,1);
	RunPanicThread(_L("TestPickPanic"),TTestStack<S,T>::TestPickPanic,pStack);

	test.End();
	delete pStack;
 	__UHEAP_MARKEND;

	User::SetJustInTime(justInTime);
 	}




template <class S,class T>
void TTestStack<S,T>::RunTests()
//
// Runs through the tests, once with Flat and Once with segmented
//
	{
	
	__UHEAP_MARK;
	test.Start(_L("Interface"));
	Test1();

	test.Next(_L("Construct/Destruct"));
	Test2();

	test.Next(_L("Basic behaviour"));
	Test3();

	test.Next(_L("Bounds"));
	Test4();

	test.End();
	__UHEAP_MARKEND;
	}

GLDEF_C TInt E32Main()
//
// Test the file server.
//
	{
	test.Title();
	
	test.Start(_L("Flat Integers"));
	TTestStack<CTranStackFlat<TInt>,TInt> tFi;
	tFi.RunTests();
	
	test.Next(_L("Seg Integers"));
	TTestStack<CTranStackSeg<TInt>,TInt> tSi;
	tSi.RunTests();
  
	test.Next(_L("Flat Testers"));
	TTestStack<CTranStackFlat<TStackTestItem>,TStackTestItem> tFt;
	tFt.RunTests();

	test.Next(_L("Seg Testers"));
	TTestStack<CTranStackSeg<TStackTestItem>,TStackTestItem> tSt;
	tSt.RunTests();

	test.End();

	test.Close();
	return(0);
	}

