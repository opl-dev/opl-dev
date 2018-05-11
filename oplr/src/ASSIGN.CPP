// ASSIGN.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <e32std.h>
#include <opcodes.h>
#include <opldb.h>
#include "oplutil.h"

void OpCode::AssignInt(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16 integer=aStack.PopInt16();
	if (aStack.PopInt16()==KLeftSideField)
		{
		aRuntime.DbManager()->Assign(integer);
		return;
		}
	OplUtil::PutWord(&(aStack.PopRefInt16()),integer);
	}
	
void OpCode::AssignLong(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt32 longInt=aStack.PopInt32();
	if (aStack.PopInt16()==KLeftSideField)
		{
		aRuntime.DbManager()->Assign(longInt);
		return;
		}		
	OplUtil::PutLong(&(aStack.PopRefInt32()),longInt);
	}

	
void OpCode::AssignFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TReal64 real=aStack.PopReal64();
	if (aStack.PopInt16()==KLeftSideField)
		{
		aRuntime.DbManager()->Assign(real);
		return;
		}		
	OplUtil::PutFloat(&aStack.PopRefReal64(),real);
	}
	
void OpCode::AssignString(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPtrC src(aStack.PopString());
	if (aStack.PopInt16()==KLeftSideField)
		{
		aRuntime.DbManager()->Assign(src);
		return;
		}
	TUint8* trg=(TUint8*)aStack.PopRefString();
	TUint16 len=(TUint16)src.Length();
	__ASSERT_ISALIGNED(trg);
	__ASSERT_DEBUG(len<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
	if (len>aStack.PopInt16())
		User::Leave(KOplErrStrTooLong);
	*trg++=(TUint8)len;
	trg++; //skip lbc2
	Mem::Copy((TUint8*)trg,(TUint8*)src.Ptr(),len<<1);
	}

