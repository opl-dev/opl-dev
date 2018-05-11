// STACK.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <oplr.h>
#include "frame.h"
#include "oplutil.h"
#include "oplstack.h"

CStack::CStack()
	{
	}

CStack* CStack::NewL(TUint aSize,COplRuntime* aRuntime)
	{ 
	CStack* pI=new(ELeave) CStack();
	CleanupStack::PushL(pI);
	pI->ConstructL(aSize,aRuntime);
	CleanupStack::Pop();
	return pI;
	}
	
void CStack::ConstructL(TUint aSize,COplRuntime* aRuntime)
	{
	iRuntime=aRuntime;
	aSize+=3;
	aSize&=~(3u);
#ifdef _DEBUG_STACK
	iTypes=new TUint8[(aSize>512) ? aSize : 512];
	if (iTypes==NULL)
		User::LeaveNoMemory();
#endif
	iCell=new TUint8[(aSize>512) ? aSize : 512];
	if (iCell==NULL)
		User::LeaveNoMemory();
	iRoot=iCell+((aSize>512) ? aSize : 512);
	iBase=iCell+256;
	iSP.Uint8=iRoot;
	}

void CStack::Push(const TInt16 aInt16)
	{
	__ASSERT_DEBUG(iSP.Uint8>iBase,User::LeaveNoMemory());
	--iSP.Int;
	*iSP.Int=aInt16;
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=EInt16;
#endif
	}	     
	
TInt16 CStack::PopInt16()
	{
	TInt16 aInt16;
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG(aType==EInt16,StackPanic(KErrStackTypeMismatch));
#endif
	aInt16=OplUtil::GetWord(iSP.Int);
	++iSP.Int;
	return aInt16;
	}
	
void CStack::Push(const TInt32 aInt32)
	{
	__ASSERT_DEBUG(iSP.Uint8>iBase,User::LeaveNoMemory());
	--iSP.Int;
	*iSP.Int=aInt32;
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=EInt32;
#endif
	}	     
	
TInt32 CStack::PopInt32()
	{
	TInt32 aInt32;
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG(aType==EInt32,StackPanic(KErrStackTypeMismatch));
#endif
	aInt32=*iSP.Int; 
	++iSP.Int;
	return aInt32;
	}

void CStack::Push(const TReal64 aReal)
	{
	__ASSERT_DEBUG(iSP.Uint8>iBase,User::LeaveNoMemory());
	--iSP.Real;
	*iSP.Real=aReal;
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=EReal64;
#endif
	}	      
	       
TReal CStack::PopReal64()
	{
	TReal aReal;
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG(aType==EReal64,StackPanic(KErrStackTypeMismatch));
#endif
	aReal=*iSP.Real;
	++iSP.Real;
	return aReal;
	}


void CStack::Push(const TDesC16& aDes)
	{
	__ASSERT_DEBUG(iSP.Uint8>iBase,User::LeaveNoMemory());
	TInt length=aDes.Length();
	__ASSERT_DEBUG(length<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
	const TUint8* ptr=(TUint8*)aDes.Ptr();
	iSP.Uint8-=(length*sizeof(TText)+KOplAlignment+4u)&~(3u); //no length byte here.
	if (iBase+iRuntime->Frame()->MaxStack()>iSP.Uint8)
		User::LeaveNoMemory();
	Mem::Copy(iSP.Uint8+1+KOplAlignment,ptr,length*sizeof(TText));
	*iSP.Uint16=(TUint16)(length); //LBC2
	__ASSERT_DEBUG(*(iSP.Uint8+KOplAlignment)==0,AlignPanic(KErrOplAlignPostStackCheck));
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=EStringType;
#endif
	}

void CStack::Push(const OplString aLbc)
	{
	__ASSERT_DEBUG(iSP.Uint8>iBase,User::LeaveNoMemory());
	const TUint8* ptr=(TUint8*)aLbc;
	TInt16 len=OplUtil::GetWord((TAny*)ptr); // lbc
	__ASSERT_DEBUG(len<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
	iSP.Uint8-=((len*sizeof(TText))+KOplAlignment+4u)&~(3u); //!!TODO Test OOM with max stack.
	if (iBase+iRuntime->Frame()->MaxStack()>iSP.Uint8)
		User::LeaveNoMemory();
	Mem::Copy(iSP.Uint8,ptr,KOplAlignment+1+len*sizeof(TText));
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=EStringType;
#endif
	}

TPtrC CStack::PopString()
	{
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG((aType==EStringType),StackPanic(KErrStackTypeMismatch));
	__ASSERT_DEBUG(*iSP.Uint16<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
#endif
	TPtrC16 buf((TUint16*)(iSP.Uint8+1+KOplAlignment),*iSP.Uint16); // LBC2
	iSP.Uint8+=(*iSP.Uint16*sizeof(TText)+KOplAlignment+4u)&~(3u);
	return buf;
	}

void CStack::Push(TAny* aPtr)
	{
	__ASSERT_DEBUG(iSP.Uint8>iBase,User::LeaveNoMemory());
	--iSP.Any;
	*iSP.Any=aPtr;
	}	      
	       
TAny* CStack::PopPtr()
	{
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
	return *iSP.Any++;
	}	       
	
void CStack::PushRef(TInt16& aInt16Ptr)
	{
	Push(&aInt16Ptr);
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=ERefInt16;
#endif
	}	     
	
TInt16& CStack::PopRefInt16()
	{
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG(aType==ERefInt16,StackPanic(KErrStackTypeMismatch));
#endif
	return *(TInt16*)PopPtr();
	}

void CStack::PushRef(TInt32& aInt32Ptr)
	{
	Push(&aInt32Ptr);
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=ERefInt32;
#endif
	}	     
	
TInt32& CStack::PopRefInt32()
	{
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG(aType==ERefInt32,StackPanic(KErrStackTypeMismatch));
#endif
	return *(TInt32*)PopPtr();
	}

void CStack::PushRef(TReal64& aRealPtr)
	{
	Push(&aRealPtr);
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=ERefReal64;
#endif
	}	      
	       
TReal& CStack::PopRefReal64()
	{
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG(aType==ERefReal64,StackPanic(KErrStackTypeMismatch));
#endif
	return *(TReal64*)PopPtr();
	}	       


void CStack::PushRef(OplString aLbc)
	{
	Push((TAny*)aLbc);
#ifdef _DEBUG_STACK
	*(iTypes-iCell+iSP.Uint8)=ERefStringType;
#endif
	}


TUint16* CStack::PopRefString()
	{
	__ASSERT_DEBUG(iSP.Uint8<iRoot,StackPanic(KErrStackEmpty));
#ifdef _DEBUG_STACK
	TInt aType=*(iTypes-iCell+iSP.Uint8);
	__ASSERT_DEBUG(aType==ERefStringType,StackPanic(KErrStackTypeMismatch));
#endif
	return (TUint16*)PopPtr();
	}

TInt CStack::FreeBytes() const
	{
	return (TInt)(iSP.Uint8-iBase);
	}

TStackPtr CStack::StackPtr() const
	{
	return iSP;
	}

void CStack::SetStackPtr(const TStackPtr &aSP)
	{
	__ASSERT_DEBUG(aSP.Uint8>=iCell&&aSP.Uint8<=iRoot,StackPanic(KErrInvalidStackPtr));
	iSP.Uint8=aSP.Uint8;
	}
	                         
void CStack::CheckParamCountL(TInt aCount)
	{
	if (aCount!=PopInt16())
		User::Leave(KOplErrNumArg);
	}

TAny* CStack::NextParamL(TInt aType)
	{
	if (aType!=PopInt16())
		User::Leave(KOplErrTypeViol);
	TAny* ptr=iSP.Any;
	JumpParam(aType);
	return ptr;
	}


#ifdef _DEBUG_STACK
void CStack::JumpParam(TInt aType)
	{
	switch (aType)
		{
		case (0):
			{
			PopInt16();
			break;
			}
		case (1):
			{
			PopInt32();
			break;
			}
		case (2):
			{
			PopReal64();
			break;
			}
		case (3):
			{
			PopString();
			break;
			}
		default:
			StackPanic(KErrBadType);
		}
	}

#else

void CStack::JumpParam(TInt aType)
	{
	switch (aType)
		{
		case (0):
		case (1):
			++iSP.Int;
			break;
		case (2):
			++iSP.Real;
			break;
		case (3):
			iSP.Uint8+=((*iSP.Uint16)*sizeof(TText)+KOplAlignment+4u)&~(3u);
			break;
		case (8):
			iSP.Uint8+=((*iSP.Uint16)*sizeof(TText)+KOplAlignment+4u)&~(3u);
			break;
		}
	}
#endif


void CStack::UnwindParams()
	{
	TInt count=PopInt16();
	while (count-->0)
		{
		JumpParam(PopInt16());
		}
	}
	
CStack::~CStack()
	{
	User::Free(iCell);
#ifdef _DEBUG_STACK
	User::Free(iTypes);
#endif
	}	
	
