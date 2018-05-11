// EN_CTRL.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <opcodes.h>
#include "oplutil.h"

void OpCode::BranchIfFalse(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	if (aStack.PopInt16()==KOplFalse)
		aRuntime.DoGoto();
	else
		aRuntime.IncIP(2);
	}

void OpCode::GoTo(CStack&, COplRuntime& aRuntime, CFrame* )
	{
	aRuntime.DoGoto();
	}

void OpCode::NextOpcodeTable(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	OpCodeTable[aRuntime.IP8()+256](aStack,aRuntime,aFramePtr);
	}


void OpCode::Stop(CStack&, COplRuntime& aRuntime, CFrame* )
	{
	User::Leave((aRuntime.StateFlags()&KInCallback)?KOplErrStopInCallback:0);
    }


void OpCode::Vector(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
/*
Vector to label
In:		pc0.uw	maxIndex
		pc1.w	offset to label 1
		pcN.w	offset to label N
		stack0	index% into list
Note: The offset is relative to the Opcode of the Qcode itself
*/
	{
    TUint maxIndex=aRuntime.IP16();	// now pointing to start of table
	TUint index=aStack.PopInt16();
	if (index==0 || index>maxIndex)
		aRuntime.IncIP(maxIndex*2);	// skip maxIndex ints
	else
		{
		TInt offset=OplUtil::GetWord((((TInt16*)aRuntime.IP())+(index-1)));
		aRuntime.IncIP(offset-3);	// can't overflow in TInt
		}
    }

		
void OpCode::ReturnFromEval(CStack&, COplRuntime&, CFrame*)
	{
	// break out of sequencer loop back to EVAL keyword
	User::Leave(KErrNone);
	}