// EN_ARITH.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include <opcodes.h>
#include "oplutil.h"

void OpCode::AddInt(CStack& aStack, COplRuntime&, CFrame*)
	{
    TInt res=aStack.PopInt16()+aStack.PopInt16();
	if (res<-32768 || res>32767)
		User::Leave(KOplErrOverflow);
	aStack.Push((TInt16)res);
	}
	
void OpCode::AddLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt32();
	TInt left=aStack.PopInt32();
	TInt res=left+right;
	if ((left^right)>=0)
		{					// args same sign
		if ((left^res)<0)
			{				// result different sign
			User::Leave(KOplErrOverflow);
			}
		}
    aStack.Push(TInt32(res));
	}
	
void OpCode::SubtractInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt res=aStack.PopInt16()-right;
	if (res<-32768 || res>32767)
		User::Leave(KOplErrOverflow);
	aStack.Push(TInt16(res));
	}
	
void OpCode::SubtractLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt32();
	TInt left=aStack.PopInt32();
	TInt res=left-right;
	if ((left^right)<0)
		{					// args opposite sign
		if ((left^res)<0)
			{				// result different sign from left (includes case of 0-negmax)
			User::Leave(KOplErrOverflow);
			}
		}
    aStack.Push(TInt32(res));
	}
	
void OpCode::MultiplyInt(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt res=aStack.PopInt16()*aStack.PopInt16();
	if (res<-32768 || res>32767)
		User::Leave(KOplErrOverflow);
	aStack.Push((TInt16)res);
	}
	
void OpCode::MultiplyLong(CStack& aStack, COplRuntime&, CFrame*)
	{
	aStack.Push(OplUtil::MultiplyL(aStack.PopInt32(),aStack.PopInt32()));
	}

void OpCode::DivideInt(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt denominator = aStack.PopInt16();
	if (denominator == 0)
		User::Leave(KOplErrDivideByZero);
	aStack.Push((TInt16)(aStack.PopInt16()/(TInt16)denominator));
	}
	
void OpCode::DivideLong(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt denominator = aStack.PopInt32();
	if (denominator == 0)
		User::Leave(KOplErrDivideByZero);
	aStack.Push((TInt32)(aStack.PopInt32()/(TInt32)denominator));
	}


void FuncOpCode::Uadd(CStack& aStack, COplRuntime&, CFrame*)
	{
    aStack.Push((TInt16)(aStack.PopInt16()+aStack.PopInt16()));
    }

void FuncOpCode::Usub(CStack& aStack, COplRuntime&, CFrame*)
	{
	TInt right=aStack.PopInt16();
	TInt left=aStack.PopInt16();
	aStack.Push(TInt16(left-right));
    }

void OpCode::AndInt(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push((TInt16)(aStack.PopInt16() & aStack.PopInt16()));
	}
	
void OpCode::AndLong(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push((TInt32)(aStack.PopInt32() & aStack.PopInt32()));
	}
	
void OpCode::AndFloat(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt16 res=KOplFalse;
	TReal64 right=aStack.PopReal64();
    TReal64 left=aStack.PopReal64();
    if (left!=0.0 && right!=0.0)
		res=KOplTrue;
	aStack.Push(res);
	}
	
void OpCode::OrInt(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push((TInt16)(aStack.PopInt16() | aStack.PopInt16()));
	}
	
void OpCode::OrLong(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push((TInt32)(aStack.PopInt32() | aStack.PopInt32()));
	}
	
void OpCode::OrFloat(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt16 res=KOplFalse;
    TReal right=aStack.PopReal64();
    TReal left=aStack.PopReal64();
	if (left!=0.0 || right!=0.0)
		res=KOplTrue;
	aStack.Push(res);
	}
	
void OpCode::NotInt(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push((TInt16)(~aStack.PopInt16()));
	}
	
void OpCode::NotLong(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push((TInt32)(~aStack.PopInt32()));
	}
	
void OpCode::NotFloat(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push(TInt16(aStack.PopReal64()==0.0 ? KOplTrue : KOplFalse));
	}
	
void OpCode::UnaryMinusInt(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push((TInt16)-aStack.PopInt16());
	}
	
void OpCode::UnaryMinusLong(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push(-aStack.PopInt32());
	}
	
void OpCode::UnaryMinusFloat(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push(-aStack.PopReal64());
	}
	
void OpCode::AddString(CStack& aStack, COplRuntime&, CFrame*)
	{
    TPtrC right = aStack.PopString();
	TBuf<256> result = aStack.PopString();
	if (result.Length()+right.Length() > 255)
		User::Leave(KOplErrStrTooLong);
	result.Append(right);
	aStack.Push(result);
	}

/* End of $Workfile:   EN_ARITH.CPP  $ */
  
  

