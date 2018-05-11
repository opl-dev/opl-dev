// GLOBTBL.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <oplr.h>


#ifdef _DEBUG
_LIT(KOplGlobalTable,"OPL Global Table");
#define GPanic(a) User::Panic(KOplGlobalTable,a)
const TInt KIllegalHashValue=-104;
const TInt KGlobalListEmpty=-105;
#endif

	
TOplGlobal::TOplGlobal(const TBufC<KMaxGlobalName> aName,const TInt aType,const TAny* aPtr) :
			iName(aName)
	{
	iType=aType;
	iPtr=aPtr;
	}

CGlobalsTbl::CGlobalsTbl()
	{
	for (TInt ii=0;ii<KGlobalTblSize;ii++)
		{
		iList[ii].SetOffset(_FOFF(TOplGlobal,iLink));
		}
	}



void CGlobalsTbl::AddL(const TBufC<KMaxGlobalName>& aName,const TInt aType,const TAny* aPtr)
	{
	TInt hashValue=GetHashValue(aName);
	__ASSERT_DEBUG(((hashValue>=0)&&(hashValue<KGlobalTblSize)),GPanic(KIllegalHashValue));
	TOplGlobal* aGlobal=new(ELeave) TOplGlobal(aName,aType,aPtr);
	iList[hashValue].AddFirst(*aGlobal);
	}

void CGlobalsTbl::Remove(const TBufC<KMaxGlobalName>& aName)
	{
	TInt hashValue=GetHashValue(aName);
	__ASSERT_DEBUG(((hashValue>=0)&&(hashValue<KGlobalTblSize)),GPanic(KIllegalHashValue));
	__ASSERT_DEBUG(iList[hashValue].IsEmpty()==EFalse,GPanic(KGlobalListEmpty));
	TOplGlobal* first=iList[hashValue].First();
	iList[hashValue].Remove(*first);
	delete first;
	}

const TAny* CGlobalsTbl::Find(const TBufC<KMaxGlobalName>& aName,const TInt aType)
	{
	TOplGlobal* aGlobal;
	TInt hashValue=GetHashValue(aName);
	__ASSERT_DEBUG(((hashValue>=0)&&(hashValue<KGlobalTblSize)),GPanic(KIllegalHashValue));
	TSglQueIter<TOplGlobal> iter=iList[hashValue];
	while ((aGlobal=iter++)!=NULL)
		{
		if (aGlobal->iName.Compare(aName)==0 && aGlobal->iType==aType)
			return aGlobal->iPtr;
		}
	return NULL;
	}

CGlobalsTbl::~CGlobalsTbl()
	{
	for (TInt ii=0;ii<KGlobalTblSize;ii++)
		{
		while (iList[ii].IsEmpty()==EFalse) // NB all entries should really be removed by frames
			{
			TOplGlobal* first=iList[ii].First();
			iList[ii].Remove(*first);
			delete first;
			}
		}
	}

TInt CGlobalsTbl::GetHashValue(const TBufC<KMaxGlobalName>& aName)
	{
	TInt hashValue=0;
	const TText* pp=aName.Ptr();
	for (TInt ii=aName.Length();ii>0;ii--)
		{
		hashValue=hashValue<<1^*pp++;
		}
	return (Abs(hashValue)%KGlobalTblSize);
	}


void CGlobalsTbl::PrepareToAddC()
	{
	for (TInt ii=0;ii<KGlobalTblSize;ii++)
		{
		iListMark[ii]=iList[ii].First();
		}
	TCleanupItem cleanup(RemoveAddedGlobals,this);
	CleanupStack::PushL(cleanup);
	}

void CGlobalsTbl::RemoveAddedGlobals(TAny* aGlobalsTbl)
	{
	CGlobalsTbl* This=STATIC_CAST(CGlobalsTbl*,aGlobalsTbl);
	for (TInt ii=0;ii<KGlobalTblSize;ii++)
		{
		TOplGlobal* first;
		TSglQue<TOplGlobal>& que=This->iList[ii];
		TOplGlobal* mark=This->iListMark[ii];
		while((first=que.First())!=mark)
			{
			que.Remove(*first);
			delete first;
			}
		}
	}

