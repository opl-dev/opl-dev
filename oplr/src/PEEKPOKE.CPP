// PEEKPOKE.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include <e32std.h>
#include <opcodes.h>
#include "oplutil.h"

LOCAL_C TUint8* GetPtrL(CStack& aStack,RHeap& aHeap,TInt aSize)
	{
	TUint8* ret=OplUtil::PopOffsetAsAddrL(aStack,aHeap,aSize);
	if (ret==NULL)
		User::Leave(KErrArgument);
	return ret;
	}

void FuncOpCode::PeekB(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TInt16)*(GetPtrL(aStack,aRuntime.Heap64(),sizeof(TUint8))));
    }

void FuncOpCode::PeekW(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push(OplUtil::GetWord(GetPtrL(aStack,aRuntime.Heap64(),sizeof(TInt16))));
    }

void FuncOpCode::PeekL(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push(OplUtil::GetLong(GetPtrL(aStack,aRuntime.Heap64(),sizeof(TInt32))));
    }

void FuncOpCode::PeekF(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push(OplUtil::GetFloat(GetPtrL(aStack,aRuntime.Heap64(),sizeof(TReal64))));
    }

void FuncOpCode::PeekStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt addr=aStack.PopInt32();
	RHeap& heap=aRuntime.Heap64();
	TUint8* ptr=OplUtil::OffsetToAddrL(heap,addr,sizeof(TUint8));

	// wrong length checked here: length consists of two bytes with the second bytes being always zero.
	//	if (ptr==NULL || (addr+(*(ptr-KOplAlignment)*sizeof(TText))+1+KOplAlignment)>(TUint)heap.Size()) // position + size of string + leading byte
	if (ptr==NULL || (addr+((*ptr)*sizeof(TText))+1+KOplAlignment)>(TUint)heap.Size()) // position + size of string + leading byte + dummy byte
		User::Leave(KErrArgument);

	aStack.Push((TText*)ptr);
    }


void OpCode::PokeW(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)		
	{
	TInt16 value=aStack.PopInt16();
	RHeap& heap=aRuntime.Heap64();
	OplUtil::PutWord(GetPtrL(aStack,heap,sizeof(TInt16)),value);
    }
	
void OpCode::PokeL(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)		 
	{
	TInt32 value=aStack.PopInt32();
	RHeap& heap=aRuntime.Heap64();
	OplUtil::PutLong(GetPtrL(aStack,heap,sizeof(TInt32)),value);
    }
	
void OpCode::PokeD(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)		 
	{
	TReal64 value=aStack.PopReal64();
	RHeap& heap=aRuntime.Heap64();
	OplUtil::PutFloat(GetPtrL(aStack,heap,sizeof(TReal64)),value);
    }
	
void OpCode::PokeStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)	 
	{
	TPtrC ptr=aStack.PopString();
	TInt size=ptr.Size();
	RHeap& heap=aRuntime.Heap64();
	TUint8* trg=GetPtrL(aStack,heap,size+1+KOplAlignment);
	Mem::Copy((trg+1+KOplAlignment),ptr.Ptr(),size);
	*trg=(TUint8)(size/sizeof(TText));
	// Fix for bug [1500819] PEEK$ doesn't always return the correct value
	// make sure the second byte is always 0
	*(trg+1)=(TUint8)0;
    }
	
void OpCode::PokeB(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)		 
	{
	TInt16 value=aStack.PopInt16();
	if (value<0||value>255)
		User::Leave(KErrArgument);
	RHeap& heap=aRuntime.Heap64();
	*(TUint8*)(GetPtrL(aStack,heap,sizeof(TUint8)))=(TUint8)value;
    }

LOCAL_C void pushCell(CStack& aStack,RHeap& aHeap, TUint8* aCell,TBool aSignExtend)
	{
	TInt32 pos = aCell ? aCell-aHeap.Base() : 0;
	aStack.Push((TInt32)((aSignExtend && pos>0x7fff && pos<0x10000)?(TInt16)pos:pos));
	}

_LIT(KBadField,"Fields not supported");
void FuncOpCode::Addr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
#ifdef _DEBUG
	if (aStack.PopInt16()!=0)
		User::Panic(KBadField,99);
#else
	aStack.PopInt16();
#endif
	pushCell(aStack,aRuntime.Heap64(),(TUint8*)aStack.PopPtr(),aRuntime.UserFlags()&KOplState64kLimit);
    }

void FuncOpCode::SAddr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
#ifdef _DEBUG
	if (aStack.PopInt16()!=0)
		User::Panic(KBadField,99);
#else
	aStack.PopInt16();
#endif


	TText* ptr=aStack.PopRefString();
	aStack.PopInt16(); // remove max length from stack
	pushCell(aStack,aRuntime.Heap64(),(TUint8*)ptr,aRuntime.UserFlags()&KOplState64kLimit);
	}


TBool NoRoomAvailable(RHeap& aHeap,TInt aSize)
	{
	TAny* tempCell=aHeap.Alloc(aSize);
	if (tempCell)
		{
		TInt size=aHeap.Size();
		aHeap.Free(tempCell);
		if (size>K64Kbytes)
			{
			aHeap.Compress();
			tempCell=NULL;
			}
		}
	return (tempCell==NULL);
	}

void FuncOpCode::Alloc(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	RHeap& heap=aRuntime.Heap64();
	TUint8* cell=(TUint8*)heap.Alloc(aStack.PopInt32());
	if (aRuntime.UserFlags()&KOplState64kLimit)
		{
		if (heap.Size()>K64Kbytes)
			{
			heap.Free(cell);
			heap.Compress(); // ensure heap becomes smaller than 64K
			cell=NULL;
			}
		}
	pushCell(aStack,heap,cell,aRuntime.UserFlags()&KOplState64kLimit);
    }


void FuncOpCode::ReAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	RHeap& heap=aRuntime.Heap64();
	TInt newLen=(aStack.PopInt32());
	TUint8* cell=GetPtrL(aStack,heap,sizeof(TUint8));
	if (aRuntime.UserFlags()&KOplState64kLimit && heap.AllocLen(cell)<newLen && NoRoomAvailable(heap,newLen))
		cell=NULL;
	else
		cell=(TUint8*)(heap.ReAlloc(cell,newLen));
	pushCell(aStack,heap,cell,aRuntime.UserFlags()&KOplState64kLimit);
    }

void FuncOpCode::AdjustAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	RHeap& heap=aRuntime.Heap64();
	TInt32 delta=aStack.PopInt32();
	TUint32 offset=aStack.PopInt32();
	TUint8* cell=GetPtrL(aStack,heap,sizeof(TUint8));
	if (aRuntime.UserFlags()&KOplState64kLimit && delta>0 && NoRoomAvailable(heap,heap.AllocLen(cell)+delta))
		cell=NULL;
	else
		cell=(TUint8*)(heap.Adjust(cell,offset,delta));
	pushCell(aStack,heap,cell,aRuntime.UserFlags()&KOplState64kLimit);
    }

void FuncOpCode::LenAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	RHeap& heap=aRuntime.Heap64();
	TUint8* cell=GetPtrL(aStack,heap,sizeof(TUint8));
	aStack.Push((TInt32)heap.AllocLen(cell));
    }

void OpCode::FreeAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	RHeap& heap=aRuntime.Heap64();
	TUint8* cell=OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint8));
	heap.Free(cell);
    }

/* End of $Workfile:   PEEKPOKE.CPP  $ */
