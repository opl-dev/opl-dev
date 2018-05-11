// FRAME.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#include "Frame.h"
#include <module.h>
#include <opldbg.h>
#include "oplutil.h"
#include "opldialg.h"

CFrame::CFrame()
	{
	}

_LIT(KOplFrame,"OPL Frame");
#define FPanic(A) User::Panic(KOplFrame,A)

void CFrame::ConstructL(CStack& aStack,COplRuntime& aRuntime,CProcDef* aProcDef,TUint8* aReturnIP,CFrame* aPrevFrame,TInt aFlags)
	{ // the order of these is important
	// any leave here is reported as an error in the calling frame
	__ASSERT_DEBUG(aProcDef!=NULL,FPanic(100));
	iRuntime=&aRuntime;
	iReturnIP=aReturnIP;
	iPrevFrame=aPrevFrame;
	iProcDef=aProcDef;
	iFlags=aFlags;
	iSP=aStack.StackPtr();
	iProcDef->IncCountL();
	CProcedure* procedure=iProcDef->Procedure();
	iMaxStack=procedure->iMaxStack;
	if (aStack.FreeBytes()<iMaxStack)
		User::LeaveNoMemory();
	iFrameCell=(TUint8*)aRuntime.Heap64().AllocL(procedure->iDataSize);
	if (aRuntime.UserFlags()&KOplState64kLimit)
		{
		RHeap& heap=aRuntime.Heap64();
		if (heap.Size()>K64Kbytes)
			{
			heap.Free(iFrameCell);
			iFrameCell=NULL;
			heap.Compress();
			User::LeaveNoMemory();
			}
		}
	Mem::FillZ(iFrameCell,procedure->iDataSize);
	procedure->AddGlobalsL(aRuntime,iFrameCell);
	iFlags|=KGlobalsAdded;
	GetParamsL(aStack,procedure);
	}

void CFrame::FinishConstructionL()
	{ // any leave here is reported as an error in this frame
	CDebuggerBase* debugger=iRuntime->Debugger();
	if (debugger!=NULL)
		{
		iFlags|=KDebuggerSignalled;
		debugger->FrameLoad(iProcDef->ProcName(),iProcDef->ModuleName(),iProcDef->Module().OplName());
		}
	CProcedure* procedure=iProcDef->Procedure();
	GetExternsL(procedure);
	iRuntime->SetIP(procedure->iQCodeCell);
	DoStringFixUps(); //these can not leave
	DoArrayFixUps();
	iStartOfIndTbl=procedure->iTotalTableSize+18;
	}

TAny* CFrame::VarAddrRel(TInt aOffset)
	{
	return iFrameCell+aOffset;
	}

TAny* CFrame::VarAddrInd(TInt aIndex)
	{
	__ASSERT_DEBUG(iIndirectTbl!=NULL,FPanic(5));
	TInt index=(aIndex-iStartOfIndTbl)/2;
	return iIndirectTbl->operator[](index);
	}

SOplSubProc CFrame::GetSubProc(TInt aIndex)
	{
	CArrayFixFlat<SOplSubProc>* iSPT=iProcDef->Procedure()->iSubProcs;
	__ASSERT_DEBUG(iSPT!=NULL,FPanic(7));
	TInt ii=iSPT->Count();
	FOREVER
		{
		ii--;
		__ASSERT_ALWAYS((ii>=0),FPanic(8)); // if not found
		if ((iSPT->operator[](ii)).offset==aIndex) // aIndex is an offset with opl1993 opo
			break;
		}
	return iSPT->operator[](ii);
	}



void CFrame::DoStringFixUps()
	{
	CArrayFixFlat<SStringFixUp>* iSFU;
	if ((iSFU=iProcDef->Procedure()->iStringFixUps)==NULL)
		return;
	SStringFixUp* stringFixUp=&(*iSFU)[0];
	for (TInt ii=iSFU->Count()-1;ii>=0;--ii,stringFixUp++)
		{
		OplUtil::PutWord((iFrameCell+stringFixUp->offset),(TInt16)stringFixUp->maxLen);
		}
	}

void CFrame::DoArrayFixUps()
	{
	CArrayFixFlat<SArrayFixUp>* iAFU;
	if ((iAFU=iProcDef->Procedure()->iArrayFixUps)==NULL)
		return;
	SArrayFixUp* arrayFixUp=&(*iAFU)[0];
	for (TInt ii=iAFU->Count()-1;ii>=0;--ii,arrayFixUp++)
		{
		OplUtil::PutWord((TUint16*)(iFrameCell+arrayFixUp->offset),(TUint16)arrayFixUp->maxIndex);
		}
	}

void CFrame::GetParamsL(CStack& aStack,CProcedure* aProcedure)
	{
	CArrayFixFlat<TInt>* pParamList=aProcedure->iParamList;
	if (pParamList!=NULL)
		{
		iIndirectTbl=new(ELeave) CArrayFixFlat<TAny*>(1);
		TInt count=pParamList->operator[](0);
		aStack.CheckParamCountL(count);
		for (TInt ii=1;ii<=count;++ii)
			{
			TAny* aPtr=aStack.NextParamL(pParamList->operator[](ii));
			iIndirectTbl->InsertL(0,aPtr);
			}
		aStack.SetStackPtr(iSP);
		}
	else
		{
		aStack.CheckParamCountL(0);
		aStack.SetStackPtr(iSP);
		}
	}

void CFrame::GetExternsL(CProcedure* aProcedure)
	{
	CArrayFixFlat<SOplGlobal>* pExtern=aProcedure->iExternals;
	if (pExtern!=NULL) // none to resolve
		{
		if (iIndirectTbl==NULL) //no params so create table here
			iIndirectTbl=new(ELeave) CArrayFixFlat<TAny*>(1);
		TBool globalNotFound=FALSE;
		CGlobalsTbl& globTbl=iRuntime->GlobalTbl();
		TInt count=pExtern->Count();
		for (TInt ii=0;ii<count;++ii)
			{
			SOplGlobal global=pExtern->operator[](ii);
			const TAny* ptr=globTbl.Find(global.name,global.type);
			if (ptr==NULL)
				{
				TBuf<256> buf(global.name);
				_LIT(KBrackets,"()");
				if (global.type&0x80)
					buf.Append(KBrackets);
				if (!globalNotFound)
					{
					iRuntime->SetErrorBuffer(&buf);
					globalNotFound=TRUE;
					}
				else
					{
					_LIT(KComma,",");
					if (iRuntime->AppendErrorText(KComma)||iRuntime->AppendErrorText(buf))
						break; // error buffer full
					}
				continue;
				}
			iIndirectTbl->AppendL(CONST_CAST(TAny*,ptr));
			}
		if (globalNotFound)
			User::Leave(KOplErrUndef);
		}
	}

void COplRuntime::SetFrame(CFrame* aFrame)
	{
	iFrame=aFrame;
	}


CFrame::~CFrame()
	{
	COplDialog* dialog=iRuntime->Dialog();
	if (dialog&&(dialog->InitFrame()==this))
		{
		delete dialog;
		iRuntime->SetDialog(NULL);
		}
	CProcedure* proc=iProcDef->Procedure();
	if (proc!=NULL)
		{
		CArrayFixFlat<SOplGlobal>* globals=proc->iGlobals;
		if (globals!=NULL && (iFlags&KGlobalsAdded))
			{
			TInt count=globals->Count();
			for (TInt ii=0;ii<count;++ii)
				{
				const SOplGlobal *global=&globals->At(ii);
				iRuntime->GlobalTbl().Remove(global->name);
				}
			}
		iProcDef->DecCount();
		}
	if (iFlags&KDebuggerSignalled) // debugger exists!
		iRuntime->Debugger()->FrameUnload();
	iRuntime->SetFrame(iPrevFrame);
	iRuntime->SetIP(iReturnIP);
	delete iIndirectTbl;
	iRuntime->Heap64().Free(iFrameCell);
	}

CProcedure::CProcedure()
	{
	}

void CProcedure::ConstructL(CProcDef* aProcDef,const RFile& aFile)
	{
	TInt offset=aProcDef->FileOffset();
	TBuf8<2> aBuf;
	User::LeaveIfError(aFile.Read(offset,aBuf,2));
	TInt dataDefSize=*(TUint16*)(aBuf.Ptr()); //implicit cast to TInt
	offset+=2;
	TUint8* dataPtr=(TUint8*)User::AllocLC(dataDefSize);
	TPtr8 des(dataPtr,dataDefSize);	
	User::LeaveIfError(aFile.Read(offset,des,dataDefSize));
	TInt QCodeOffset=dataDefSize+offset;
	offset=0;
	iDataSize=ReadWordL(offset,dataPtr);
	LoadQCodeL(QCodeOffset,ReadWordL(offset,dataPtr),aFile);
	iMaxStack=ReadWordL(offset,dataPtr);

	TInt count=ReadWordL(offset,dataPtr);

	if (count>0)
		{
		iParamList=new(ELeave) CArrayFixFlat<TInt>(1);
		iParamList->AppendL(count);
		for (;count>0;--count)
			{
			TInt aByte=ReadWordL(offset,dataPtr);
			iParamList->AppendL(aByte);
			}
		}
	TInt tableSize=ReadWordL(offset,dataPtr);
	TInt startOfGlobals=offset;

	if (tableSize>0)
		{
		iGlobals=new(ELeave) CArrayFixFlat<SOplGlobal>(1);
		tableSize+=offset;
		while (tableSize>offset)
			{
			SOplGlobal aGlobal;
			aGlobal.name=ReadStringL(offset,dataPtr);
			aGlobal.type=ReadWordL(offset,dataPtr);
			aGlobal.offset=ReadWordL(offset,dataPtr);
			iGlobals->AppendL(aGlobal);
			}
		__ASSERT_DEBUG(tableSize==offset,FPanic(2));
		}
	tableSize=ReadWordL(offset,dataPtr);
	iTotalTableSize=offset-startOfGlobals+tableSize-2;

	if (tableSize>0)
		{
		iSubProcs=new(ELeave) CArrayFixFlat<SOplSubProc>(1);
		tableSize+=offset;
		while (tableSize>offset)
			{
			SOplSubProc aSubProc;
			aSubProc.offset=offset-startOfGlobals+16;//18 header bytes - 2 for the table size
			aSubProc.name=ReadStringL(offset,dataPtr);
			aSubProc.noOfParam=ReadWordL(offset,dataPtr);
			iSubProcs->AppendL(aSubProc);
			}
		__ASSERT_DEBUG(tableSize==offset,FPanic(3));
		}
	if (ReadWordL(offset,dataPtr)>0)
		{
		offset-=(1+KOplAlignment); // "put back" first string's LBC
		iExternals=new(ELeave) CArrayFixFlat<SOplGlobal>(1);
		SOplGlobal aGlobal;
		aGlobal.name=ReadStringL(offset,dataPtr);
		aGlobal.type=ReadWordL(offset,dataPtr);
		iExternals->AppendL(aGlobal);
		while (ReadWordL(offset,dataPtr)>0)
			{
			offset-=(1+KOplAlignment); // "put back" string's LBC
			aGlobal.name=ReadStringL(offset,dataPtr);
			aGlobal.type=ReadWordL(offset,dataPtr);
			iExternals->AppendL(aGlobal);
			}
		}
	if (ReadWordL(offset,dataPtr)>0)
		{
		offset-=2;
		iStringFixUps=new(ELeave) CArrayFixFlat<SStringFixUp>(1);
		SStringFixUp stringFixUp;
		while ((stringFixUp.offset=ReadWordL(offset,dataPtr))>0)
			{
			stringFixUp.maxLen=ReadWordL(offset,dataPtr);
			iStringFixUps->AppendL(stringFixUp);
			}
		}
	if (ReadWordL(offset,dataPtr)>0)
		{
		offset-=2;
		iArrayFixUps=new(ELeave) CArrayFixFlat<SArrayFixUp>(1);
		SArrayFixUp arrayFixUp;
		while ((arrayFixUp.offset=ReadWordL(offset,dataPtr))>0)
			{
			arrayFixUp.maxIndex=ReadWordL(offset,dataPtr);
			iArrayFixUps->AppendL(arrayFixUp);
			}
		}
	CleanupStack::PopAndDestroy(); // dataDef
	}

void CProcedure::AddGlobalsL(COplRuntime& aRuntime,TUint8* aCell)
	{
	if (iGlobals==NULL)
		return;
	TInt count=iGlobals->Count();
	aRuntime.GlobalTbl().PrepareToAddC(); // will restore state on a leave
	for (TInt ii=0;ii<count;++ii)
		{
		const SOplGlobal *global=&iGlobals->At(ii);
		aRuntime.GlobalTbl().AddL(global->name,global->type,(aCell+global->offset));
		}
	CleanupStack::Pop(); // globals table
	}


TInt CProcedure::ReadWordL(TInt& aOffset,const TUint8* aDataPtr)
	{
	TInt word=(TUint16)OplUtil::GetWord((TUint16*)(aDataPtr+aOffset));
	aOffset+=2;
	return word;
	}

TBuf<KMaxGlobalNameLen> CProcedure::ReadStringL(TInt& aPos,const TUint8* aDataPtr)
	{
	TInt strLen=ReadWordL(aPos,aDataPtr);
	__ASSERT_ALWAYS(strLen<=KMaxGlobalNameLen,FPanic(strLen));
	TPtrC ptr((TText*)(aDataPtr+aPos),strLen);
	TBuf<KMaxGlobalNameLen> aBuf(ptr);
	aPos+=strLen*sizeof(TText);
	return aBuf;
	}

void CProcedure::LoadQCodeL(TInt aPos,TInt aLength,const RFile& aFile)
	{
	TUint8* aQCodeCell=(TUint8*)User::AllocLC(aLength);
	TPtr8 ptr(aQCodeCell,aLength);
	User::LeaveIfError(aFile.Read(aPos,ptr,aLength));
	CleanupStack::Pop();
	iQCodeCell=aQCodeCell;
	}

CProcedure::~CProcedure()
	{
	User::Free(iQCodeCell);
	delete iParamList;
	delete iGlobals;
	delete iSubProcs;
	delete iExternals;
	delete iStringFixUps;
	delete iArrayFixUps;
	}

