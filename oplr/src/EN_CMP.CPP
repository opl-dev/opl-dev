// EN_CMP.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#include <opcodes.h>

void OpCode::CompareLessThanInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt left=aStack.PopInt16();
	if (left < right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareLessThanLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt32 right=aStack.PopInt32();
	TInt32 left=aStack.PopInt32();
	if (left < right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareLessThanFloat(CStack& aStack, COplRuntime&, CFrame*)
	{
	TReal right=aStack.PopReal64();
	TReal left=aStack.PopReal64();
	if (left < right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareLessThanString(CStack& aStack, COplRuntime&, CFrame*)
	{
	TPtrC right=aStack.PopString();
	TPtrC left=aStack.PopString();
	if (left < right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareLessOrEqualInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt left=aStack.PopInt16();
	if (left <= right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareLessOrEqualLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt32 right=aStack.PopInt32();
	TInt32 left=aStack.PopInt32();
	if (left <= right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareLessOrEqualFloat(CStack& aStack, COplRuntime&, CFrame*)
	{
	TReal right=aStack.PopReal64();
	TReal left=aStack.PopReal64();
	if (left <= right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareLessOrEqualString(CStack& aStack, COplRuntime&, CFrame*)
	{
	TPtrC right=aStack.PopString();
	TPtrC left=aStack.PopString();
	if (left<=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterThanInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt left=aStack.PopInt16();
	if (left>right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterThanLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt32 right=aStack.PopInt32();
	TInt32 left=aStack.PopInt32();
	if (left>right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterThanFloat(CStack& aStack, COplRuntime&, CFrame*)
	{
	TReal right=aStack.PopReal64();
	TReal left=aStack.PopReal64();
	if (left>right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterThanString(CStack& aStack, COplRuntime&, CFrame*)
	{
	TPtrC right=aStack.PopString();
	TPtrC left=aStack.PopString();
	if (left>right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterOrEqualInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt left=aStack.PopInt16();
	if (left>=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterOrEqualLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt32 right=aStack.PopInt32();
	TInt32 left=aStack.PopInt32();
	if (left>=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterOrEqualFloat(CStack& aStack, COplRuntime&, CFrame*)
	{
	TReal right=aStack.PopReal64();
	TReal left=aStack.PopReal64();
	if (left>=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareGreaterOrEqualString(CStack& aStack, COplRuntime&, CFrame*)
	{
	TPtrC right=aStack.PopString();
	TPtrC left=aStack.PopString();
	if (left>=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
//40h

void OpCode::CompareEqualInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt left=aStack.PopInt16();
	if (left==right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareEqualLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt32 right=aStack.PopInt32();
	TInt32 left=aStack.PopInt32();
	if (left==right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareEqualFloat(CStack& aStack, COplRuntime&, CFrame*)
	{
	TReal right=aStack.PopReal64();
	TReal left=aStack.PopReal64();
	if (left==right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareEqualString(CStack& aStack, COplRuntime&, CFrame*)
	{
	TPtrC right=aStack.PopString();
	TPtrC left=aStack.PopString();
	if (left==right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareNotEqualInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt left=aStack.PopInt16();
	if (left!=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareNotEqualLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt32 right=aStack.PopInt32();
	TInt32 left=aStack.PopInt32();
	if (left!=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareNotEqualFloat(CStack& aStack, COplRuntime&, CFrame*)
	{
	TReal right=aStack.PopReal64();
	TReal left=aStack.PopReal64();
	if (left!=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
void OpCode::CompareNotEqualString(CStack& aStack, COplRuntime&, CFrame*)
	{
	TPtrC right=aStack.PopString();
	TPtrC left=aStack.PopString();
	if (left!=right)
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
	}
	
/* End of $Workfile:   EN_CMP.CPP  $ */
  
  
