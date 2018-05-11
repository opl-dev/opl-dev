// PUSHOC.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <e32std.h>
#include <opcodes.h>
#include <module.h>
#include <oplrdef.h>
#include "frame.h"
#include "oplutil.h"

void OpCode::SimpleDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetWord(aFramePtr->VarAddrRel(aRuntime.IP16())));
	}

void OpCode::SimpleDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetLong(aFramePtr->VarAddrRel(aRuntime.IP16())));
	}

void OpCode::SimpleDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetFloat(aFramePtr->VarAddrRel(aRuntime.IP16())));
	}

void OpCode::SimpleDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TText*)(aFramePtr->VarAddrRel(aRuntime.IP16())));
	}

void OpCode::SimpleDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt16*)(aFramePtr->VarAddrRel(aRuntime.IP16())));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::SimpleDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt32*)(aFramePtr->VarAddrRel(aRuntime.IP16())));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::SimpleDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TReal64*)(aFramePtr->VarAddrRel(aRuntime.IP16())));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::SimpleDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint8* ptr=(TUint8*)aFramePtr->VarAddrRel(aRuntime.IP16()); // ptr can be misaligned.
	__ASSERT_ISALIGNED(ptr);
	TInt16 maxLen=OplUtil::GetWord(ptr-(1+KOplAlignment));
	__ASSERT_DEBUG(maxLen>0,AlignPanic(KErrOplAlignBadLength));
	__ASSERT_DEBUG(maxLen<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
	aStack.Push(maxLen);
	aStack.PushRef((TText*)ptr);
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::SimpleInDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetWord(aFramePtr->VarAddrInd(aRuntime.IP16())));
	}

void OpCode::SimpleInDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetLong(aFramePtr->VarAddrInd(aRuntime.IP16())));
	}

void OpCode::SimpleInDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetFloat(aFramePtr->VarAddrInd(aRuntime.IP16())));
	}

void OpCode::SimpleInDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TText*)(aFramePtr->VarAddrInd(aRuntime.IP16())));
	}

LOCAL_C const TAny* _ptrGlobal(COplRuntime& aRuntime,TOplType& aType)
	{
	TUint8* ip=(TUint8*)aRuntime.IP();
	// Get a word-aligned copy of the name at the current IP.
	TBuf<KMaxGlobalName> name;
	name.SetLength(*ip);
	Mem::Copy((TUint8*)name.Ptr(),ip+1+KOplAlignment,*ip*sizeof(TText));

	aRuntime.IncIP(*ip*sizeof(TText)+1+KOplAlignment);
	aType=TOplType(aRuntime.IP16());
	const TAny* ptr=aRuntime.GlobalTbl().Find(name,aType);
	if (ptr==NULL)
		{
		aRuntime.SetErrorBuffer(&name);
		User::Leave(KOplErrUndef);
		}
	return ptr;
	}

void OpCode::EvalExternalRightSideRef(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TOplType type;
	TUint8* ptr=(TUint8*)_ptrGlobal(aRuntime,type);
	switch (type)
		{
	case EWord:
		aStack.Push(OplUtil::GetWord(ptr));
		break;
	case ELong:
		aStack.Push(OplUtil::GetLong(ptr));
		break;
	case EReal:
		aStack.Push(OplUtil::GetFloat(ptr));
		break;
	case EString:
		aStack.Push((OplString)ptr);
		break;
	case EWordArray:
		aStack.Push(*(TInt16*)GetElementL(ptr,aStack.PopInt16(),sizeof(TInt16)));
		break;
	case ELongArray:
		aStack.Push(*(TInt32*)GetElementL(ptr,aStack.PopInt16(),sizeof(TInt32)));
		break;
	case ERealArray:
		aStack.Push(*(TReal64*)GetElementL(ptr,aStack.PopInt16(),sizeof(TReal64)));
		break;
	case EStringArray:
		{
		TInt index=aStack.PopInt16();
		if (index>OplUtil::GetWord(ptr-(3+KOplAlignment))||index<1)
			User::Leave(KOplErrSubs);
		TInt len=*(ptr-(1+KOplAlignment));
		aStack.Push((TText*)JumpToIndex(ptr,index,len*sizeof(TText)+1+KOplAlignment));
		break;
		}
	default:
		User::Leave(KOplErrInvalidArgs);
		break;
		}
	}

void OpCode::EvalExternalLeftSideRef(CStack& /*aStack*/, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	// EvalExternalLeftSideRef not yet supported
	User::Leave(KOplErrInvalidArgs);
	}

void OpCode::SimpleInDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt16*)(aFramePtr->VarAddrInd(aRuntime.IP16())));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::SimpleInDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt32*)(aFramePtr->VarAddrInd(aRuntime.IP16())));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::SimpleInDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TReal64*)(aFramePtr->VarAddrInd(aRuntime.IP16())));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::SimpleInDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint8* ptr=(TUint8*)(aFramePtr->VarAddrInd(aRuntime.IP16()));
	__ASSERT_ISALIGNED(ptr);
	TInt16 maxLen=OplUtil::GetWord(ptr-(1+KOplAlignment));
	__ASSERT_DEBUG(maxLen!=0,AlignPanic(KErrOplAlignBadLength));
	__ASSERT_DEBUG(maxLen<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
	aStack.Push(maxLen);
	aStack.PushRef((TText*)ptr);
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetWord(GetElementL(aFramePtr->VarAddrRel(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt16))));
	}

void OpCode::ArrayDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetLong(GetElementL(aFramePtr->VarAddrRel(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt32))));
	}

void OpCode::ArrayDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetFloat(GetElementL(aFramePtr->VarAddrRel(aRuntime.IP16()),aStack.PopInt16(),sizeof(TReal64))));
	}

void OpCode::ArrayDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint8* ptr=(TUint8*)aFramePtr->VarAddrRel(aRuntime.IP16());
	TInt index=aStack.PopInt16();
	TInt16 maxIndex=OplUtil::GetWord(ptr-(3+KOplAlignment));
	if (index>maxIndex||index<1)
		User::Leave(KOplErrSubs);
	aStack.Push((TText*)JumpToIndex(ptr,index,OplUtil::GetWord(ptr-(1+KOplAlignment))*sizeof(TText)+1+KOplAlignment));
	}

void OpCode::ArrayDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt16*)(GetElementL(aFramePtr->VarAddrRel(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt16))));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt32*)GetElementL(aFramePtr->VarAddrRel(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt32)));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TReal64*)GetElementL(aFramePtr->VarAddrRel(aRuntime.IP16()),aStack.PopInt16(),sizeof(TReal64)));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint8* ptr=(TUint8*)aFramePtr->VarAddrRel(aRuntime.IP16());
	TInt index=aStack.PopInt16();
	TInt16 maxIndex=OplUtil::GetWord(ptr-(3+KOplAlignment));
	if (index>maxIndex||index<1)
		User::Leave(KOplErrSubs);
	TInt16 maxLen=OplUtil::GetWord(ptr-(1+KOplAlignment));
	aStack.Push(maxLen); //len of string in chars
	aStack.PushRef((TText*)JumpToIndex(ptr,index,maxLen*sizeof(TText)+1+KOplAlignment));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayInDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetWord(GetElementL(aFramePtr->VarAddrInd(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt16))));
	}

void OpCode::ArrayInDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetLong(GetElementL(aFramePtr->VarAddrInd(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt32))));
	}

void OpCode::ArrayInDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(OplUtil::GetFloat(GetElementL(aFramePtr->VarAddrInd(aRuntime.IP16()),aStack.PopInt16(),sizeof(TReal64))));
	}

void OpCode::ArrayInDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint8* ptr=(TUint8*)aFramePtr->VarAddrInd(aRuntime.IP16());
	TInt index=aStack.PopInt16();
	TInt16 maxIndex=OplUtil::GetWord(ptr-(3+KOplAlignment));
	if (index>maxIndex||index<1)
		User::Leave(KOplErrSubs);
	aStack.Push((TText*)JumpToIndex(ptr,index,OplUtil::GetWord(ptr-(1+KOplAlignment))*sizeof(TText)+1+KOplAlignment));
	}

void OpCode::ArrayInDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt16*)(GetElementL(aFramePtr->VarAddrInd(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt16))));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayInDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TInt32*)GetElementL(aFramePtr->VarAddrInd(aRuntime.IP16()),aStack.PopInt16(),sizeof(TInt32)));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayInDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.PushRef(*(TReal64*)GetElementL(aFramePtr->VarAddrInd(aRuntime.IP16()),aStack.PopInt16(),sizeof(TReal64)));
	aStack.Push((TInt16)0); // not a field
	}

void OpCode::ArrayInDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint8* ptr=(TUint8*)aFramePtr->VarAddrInd(aRuntime.IP16());
	TInt index=aStack.PopInt16();
	TInt16 maxIndex=OplUtil::GetWord(ptr-(3+KOplAlignment));
	if (index>maxIndex||index<1)
		User::Leave(KOplErrSubs);
	TInt16 len=OplUtil::GetWord(ptr-(1+KOplAlignment));
	aStack.Push(len);
	aStack.PushRef((TText*)JumpToIndex(ptr,index,len*sizeof(TText)+1+KOplAlignment));
	aStack.Push((TInt16)0); // not a field
	}

TAny* OpCode::GetElementL(TAny* aArray,TInt aIndex,TInt aElementSize)
	{
	if (aIndex>(OplUtil::GetWord((TUint8*)aArray-2))||aIndex<1)
		User::Leave(KOplErrSubs);
	return JumpToIndex(aArray,aIndex,aElementSize);
	}

TAny* OpCode::JumpToIndex(TAny* aArray,TInt aIndex,TInt aElementSize)
	{
	return ((TUint8*)aArray+(aIndex-1)*aElementSize); //ElementSize is in bytes, not chars.
	}

void OpCode::ConstantInt(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	aStack.Push((TInt16)aRuntime.IP16());
	}

void OpCode::ConstantLong(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	aStack.Push((TInt32)aRuntime.IP32());
	}

void OpCode::ConstantFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	aStack.Push(aRuntime.IPReal());
	}

void OpCode::ConstantString(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	aStack.Push((TText*)aRuntime.IP());
	TUint16 length=*aRuntime.IP();
	aRuntime.IncIP(length*sizeof(TText)+1+KOplAlignment);
	}

void OpCode::StackByteAsWord(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aStack.Push((TInt16)(TInt8)aRuntime.IP8());
	}
	
void OpCode::StackByteAsLong(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	aStack.Push((TInt32)(TInt8)aRuntime.IP8());
	}
	
void OpCode::StackWordAsLong(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	aStack.Push((TInt32)(TInt16)aRuntime.IP16());
	}
	
void OpCode::DropInt(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.PopInt16();
	}
	
void OpCode::DropLong(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.PopInt32();
	}
	
void OpCode::DropFloat(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.PopReal64();
	}
	
void OpCode::DropString(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.PopString();
	}		

void OpCode::RunProcedure(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	SOplSubProc subProc=aFramePtr->GetSubProc(aRuntime.IP16());
	CProcDef* procDef=aRuntime.ModulesCollection().FindProcedure(subProc.name);
	aStack.Push((TInt16)subProc.noOfParam);
	CFrame* frame=new(ELeave) CFrame;
	CleanupStack::PushL(frame);
	frame->ConstructL(aStack,aRuntime,procDef,aRuntime.IP(),aFramePtr);
	aRuntime.SetFrame(frame);
	CleanupStack::Pop();
	frame->FinishConstructionL();
	}

void OpCode::CallProcByStringExpr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt16 noOfParams=aRuntime.IP8();
	TUint8 type=aRuntime.IP8();
	aStack.Push(noOfParams);
	OpCode::DoCallProcByStringExpr(aStack,aRuntime,aFramePtr,type);
	}

void OpCode::DoCallProcByStringExpr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr,TUint8 aType)
	{
	TStackPtr tempSP=aStack.StackPtr();
	aStack.UnwindParams();
	TBuf<256> procName=aStack.PopString();	//string could be 255 long!
	aStack.SetStackPtr(tempSP);
	if (procName.Length()!=0)
		{
		procName.UpperCase(); 
		TPtrC last=procName.Right(1); // last char in string passed
		_LIT(KPercent,"%");
		_LIT(KAmpersand,"&");
		_LIT(KDollar,"$");
		if (last!=KPercent&&last!=KAmpersand&&last!=KDollar)
			{
			if (aType!=0)
				procName.Append(aType);
			CProcDef* procDef=aRuntime.ModulesCollection().FindProcedure(procName);
			CFrame* frame=new(ELeave) CFrame;
			CleanupStack::PushL(frame);
			frame->ConstructL(aStack,aRuntime,procDef,aRuntime.IP(),aFramePtr,KCalledByString);
			aRuntime.SetFrame(frame);
			CleanupStack::Pop();
			frame->FinishConstructionL();
			return;
			}
		}
	aRuntime.SetErrorBuffer(&procName);
	User::Leave(KOplErrNoProc);
	}
	
void OpCode::ZeroReturnInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	RemoveFrame(aStack,aFramePtr);	
	aStack.Push((TInt16)0);
	CheckExitL(aRuntime);
	}
	
void OpCode::ZeroReturnLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	RemoveFrame(aStack,aFramePtr);	
	aStack.Push((TInt32)0);
	CheckExitL(aRuntime);
	}
	
void OpCode::ZeroReturnFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	RemoveFrame(aStack,aFramePtr);	
	aStack.Push(0.0);
	CheckExitL(aRuntime);
	}
	
void OpCode::NullReturnString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	RemoveFrame(aStack,aFramePtr);	
	aStack.Push(KNullDesC);
	CheckExitL(aRuntime);
	}
	
void OpCode::Return(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint8* pVal=aStack.StackPtr().Uint8;
#ifdef _DEBUG_STACK
	TUint8 type=aStack.Type();
#endif
	TStackPtr aSP=aFramePtr->StackPtr();
	aStack.SetStackPtr(aSP);  //make this in debug only ??????? see RemoveFrame's __ASSERT_DEBUG
	TInt sizeOfVal=aSP.Uint8-pVal;
	RemoveFrame(aStack,aFramePtr);	// also removes params
	aSP.Uint8=aStack.StackPtr().Uint8-sizeOfVal;
	Mem::Copy(aSP.Uint8,pVal,sizeOfVal);
	aStack.SetStackPtr(aSP);
#ifdef _DEBUG_STACK
	aStack.SetType(type);
#endif
	CheckExitL(aRuntime);
	}

void OpCode::RemoveFrame(CStack& aStack, CFrame* aFramePtr)
	{
#if defined(_DEBUG)
	_LIT(KOplOpcodes,"OPL OpCodes");
#endif
	__ASSERT_DEBUG(aStack.StackPtr().Uint8==aFramePtr->StackPtr().Uint8,User::Panic(KOplOpcodes,9));
	TInt flags=aFramePtr->Flags();
	delete aFramePtr;
	aStack.UnwindParams(); // clean stack before leaving in case a callback
	if (flags&KCalledByString)
		aStack.PopString();
	}

void OpCode::CheckExitL(COplRuntime& aRuntime)
	{
	if (aRuntime.Frame()==NULL)
		{
		User::Leave(KErrNone);
		}
	}