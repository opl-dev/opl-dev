// DEBUGGER.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <opcodes.h>
#include "oplutil.h"
#include "frame.h"
#include <opldbg.h>


const TInt KTypeSimple=0;
const TInt KTypeArray=0x80;
const TInt KTypeInt16=0;
const TInt KTypeInt32=1;
const TInt KTypeReal64=2;
const TInt KTypeString=3;


CDebuggerAPI::CDebuggerAPI(COplRuntime* aRuntime)
	{
	iRuntime=aRuntime;
	}

EXPORT_C TInt16 CDebuggerAPI::MapError(TInt aError)
	{
	return OplUtil::MapError(aError);
	}

EXPORT_C void CDebuggerAPI::GetErrorMessage(TDes& aDes,TInt16 aError)
	{
	iRuntime->GetErrStr(aDes,aError);
	}


TAny* CDebuggerAPI::GetLocalPtrL(TInt aOffset,TInt aIndex) const
	{
	if (aOffset<0)
		{
		if (aIndex!=0)
			User::Leave(KErrArgument);
		return iRuntime->Frame()->VarAddrInd(-1-aOffset);
		}
	else
		return iRuntime->Frame()->VarAddrRel(aOffset);
	}

TAny* CDebuggerAPI::GetGlobalPtrL(const TDesC& aName,TInt aType) const
	{
	TAny* ptr=NULL;
	if (aName.Length()<=KMaxGlobalName)
		{
		TBufC<KMaxGlobalName> name=aName;
		ptr=CONST_CAST(TAny*,(iRuntime->GlobalTbl().Find(name,aType)));
		}
	if (ptr==NULL)
		User::Leave(KOplErrUndef);
	return ptr;
	}

EXPORT_C TInt16 CDebuggerAPI::ExamineLocalInt16L(TInt aOffset,TInt aIndex) const
	{
	TAny* ptr=GetLocalPtrL(aOffset,aIndex);
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt16));
	return OplUtil::GetWord(ptr);
	}

EXPORT_C TInt32 CDebuggerAPI::ExamineLocalInt32L(TInt aOffset,TInt aIndex) const
	{
	TAny* ptr=GetLocalPtrL(aOffset,aIndex);
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt32));
	return OplUtil::GetLong(ptr);
	}

EXPORT_C TReal64 CDebuggerAPI::ExamineLocalReal64L(TInt aOffset,TInt aIndex) const
	{
	TAny* ptr=GetLocalPtrL(aOffset,aIndex);
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TReal64));
	return OplUtil::GetFloat(ptr);
	}

EXPORT_C TPtrC CDebuggerAPI::ExamineLocalStringL(TInt aOffset,TInt aIndex) const
	{
	TText8* ptr8=(TText8*)GetLocalPtrL(aOffset,aIndex);
	TText* ptr=(TText*)ptr8;
	if (aIndex>0)
		{
		TInt16 maxIndex=OplUtil::GetWord(ptr8-(3+KOplAlignment));
		if (aIndex>maxIndex)
			User::Leave(KOplErrSubs);
		ptr=(TText*)OpCode::JumpToIndex(ptr8,aIndex,*(ptr8-(1+KOplAlignment))*sizeof(TText)+1+KOplAlignment);
		}
	TInt len=*ptr++; // skips LBC2 also.
	TPtrC ret(ptr,len);
	return ret;
	}

EXPORT_C TInt16 CDebuggerAPI::ExamineGlobalInt16L(const TDesC& aDes,TInt aIndex) const
	{
	TAny* ptr=GetGlobalPtrL(aDes,KTypeInt16|((aIndex>0)?KTypeArray:KTypeSimple));
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt16));
	return OplUtil::GetWord(ptr);
	}

EXPORT_C TInt32 CDebuggerAPI::ExamineGlobalInt32L(const TDesC& aDes,TInt aIndex) const
	{
	TAny* ptr=GetGlobalPtrL(aDes,KTypeInt32|((aIndex>0)?KTypeArray:KTypeSimple));
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt32));
	return OplUtil::GetLong(ptr);
	}

EXPORT_C TReal64 CDebuggerAPI::ExamineGlobalReal64L(const TDesC& aDes,TInt aIndex) const
	{
	TAny* ptr=GetGlobalPtrL(aDes,KTypeReal64|((aIndex>0)?KTypeArray:KTypeSimple));
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TReal64));
	return OplUtil::GetFloat(ptr);
	}

EXPORT_C TPtrC CDebuggerAPI::ExamineGlobalStringL(const TDesC& aDes,TInt aIndex) const
	{
	TText8* ptr8=(TText8*)GetGlobalPtrL(aDes,KTypeString|((aIndex>0)?KTypeArray:KTypeSimple));
	TText* ptr=(TText*)ptr8;
	if (aIndex>0)
		{
		TInt16 maxIndex=OplUtil::GetWord(ptr8-(3+KOplAlignment));
		if (aIndex>maxIndex)
			User::Leave(KOplErrSubs);
		ptr=(TText*)OpCode::JumpToIndex(ptr8,aIndex,*(ptr8-(1+KOplAlignment))*sizeof(TText)+1+KOplAlignment);
		}
	TInt len=*ptr++; //skips LBC
	TPtrC ret(ptr,len);
	return ret;
	}

EXPORT_C void CDebuggerAPI::SetLocalInt16L(TInt aOffset,TInt aIndex,TInt16 aValue)
	{
	__ASSERT_ALWAYS(aOffset>=0,User::Leave(KErrArgument));
	TAny* ptr=GetLocalPtrL(aOffset,aIndex);
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt16));
	OplUtil::PutWord(ptr,aValue);
	}

EXPORT_C void CDebuggerAPI::SetLocalInt32L(TInt aOffset,TInt aIndex,TInt32 aValue)
	{
	__ASSERT_ALWAYS(aOffset>=0,User::Leave(KErrArgument));
	TAny* ptr=GetLocalPtrL(aOffset,aIndex);
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt32));
	OplUtil::PutLong(ptr,aValue);
	}

EXPORT_C void CDebuggerAPI::SetLocalReal64L(TInt aOffset,TInt aIndex,TReal64 aValue)
	{
	__ASSERT_ALWAYS(aOffset>=0,User::Leave(KErrArgument));
	TAny* ptr=GetLocalPtrL(aOffset,aIndex);
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TReal64));
	OplUtil::PutFloat(ptr,aValue);
	}

EXPORT_C void CDebuggerAPI::SetLocalStringL(TInt aOffset,TInt aIndex,const TDesC& aDes)
	{
	__ASSERT_ALWAYS(aOffset>=0,User::Leave(KErrArgument));
	TText8* ptr8=(TText8*)GetLocalPtrL(aOffset,aIndex);
	TText* ptr=(TText*)ptr8;
	TInt maxLen=*(ptr8-(1+KOplAlignment));
	TInt len=aDes.Length();
	if (len>maxLen)
		User::Leave(KOplErrStrTooLong);
	if (aIndex>0)
		{
		TInt16 maxIndex=OplUtil::GetWord(ptr8-(3+KOplAlignment));
		if (aIndex>maxIndex||aIndex<1)
			User::Leave(KOplErrSubs);
		ptr=(TText*)OpCode::JumpToIndex(ptr8,aIndex,maxLen*sizeof(TText)+1+KOplAlignment);
		}
	*ptr++=(TUint16)len; // Skips LBC2 too
	Mem::Copy((TUint8*)ptr,(TUint8*)aDes.Ptr(),len<<1);
	}

EXPORT_C void CDebuggerAPI::SetGlobalInt16L(const TDesC& aName,TInt aIndex,TInt16 aValue)
	{
	TAny* ptr=GetGlobalPtrL(aName,KTypeInt16|((aIndex>0)?KTypeArray:KTypeSimple));
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt16));
	OplUtil::PutWord(ptr,aValue);
	}

EXPORT_C void CDebuggerAPI::SetGlobalInt32L(const TDesC& aName,TInt aIndex,TInt32 aValue)
	{
	TAny* ptr=GetGlobalPtrL(aName,KTypeInt32|((aIndex>0)?KTypeArray:KTypeSimple));
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TInt32));
	OplUtil::PutLong(ptr,aValue);
	}

EXPORT_C void CDebuggerAPI::SetGlobalReal64L(const TDesC& aName,TInt aIndex,TReal64 aValue)
	{
	TAny* ptr=GetGlobalPtrL(aName,KTypeReal64|((aIndex>0)?KTypeArray:KTypeSimple));
	if (aIndex>0)
		ptr=OpCode::GetElementL(ptr,aIndex,sizeof(TReal64));
	OplUtil::PutFloat(ptr,aValue);
	}

EXPORT_C void CDebuggerAPI::SetGlobalStringL(const TDesC& aName,TInt aIndex,const TDesC& aDes)
	{
	TText8* ptr8=(TText8*)GetGlobalPtrL(aName,KTypeString|((aIndex>0)?KTypeArray:KTypeSimple));
	TText* ptr=(TText*)ptr8;
	TInt maxLen=*(ptr8-(1+KOplAlignment));
	TInt len=aDes.Length();
	if (len>maxLen)
		User::Leave(KOplErrStrTooLong);
	if (aIndex>0)
		{
		TInt16 maxIndex=OplUtil::GetWord(ptr8-(3+KOplAlignment));
		if (aIndex>maxIndex||aIndex<1)
			User::Leave(KOplErrSubs);
		ptr=(TText*)OpCode::JumpToIndex(ptr8,aIndex,maxLen*sizeof(TText)+1+KOplAlignment);
		}
	*ptr++=(TUint16)len; //skips LBC
	Mem::Copy((TUint8*)ptr,(TUint8*)aDes.Ptr(),len<<1);
	}

EXPORT_C CDebuggerBase::CDebuggerBase(CDebuggerAPI* aDebuggerAPI)
	{
	iDebuggerAPI=aDebuggerAPI;
	}

void OpCode::Statement16(CStack& /*aStack*/,COplRuntime& aRuntime,CFrame* /*aFrame*/)
	{
	TInt statementPos=aRuntime.IP16();
	CDebuggerBase* debugger=aRuntime.Debugger();
	if (debugger!=NULL)
		debugger->NextStatement(statementPos);
	}

void OpCode::Statement32(CStack& /*aStack*/,COplRuntime& aRuntime,CFrame* /*aFrame*/)
	{
	TInt statementPos=aRuntime.IP32();
	CDebuggerBase* debugger=aRuntime.Debugger();
	if (debugger!=NULL)
		debugger->NextStatement(statementPos);
	}
