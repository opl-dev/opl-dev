// Array.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.


#include <e32std.h>
#include <opxapi.h>
#include <oplerr.h>

#include "OPXUTIL.H"

#include "Array.h"

TAny* TKeyArrayHBufPtr::At(TInt aIndex) const
	{
	if (aIndex==KIndexPtr)
		return *(TUint8**)iPtr+iKeyOffset;
	return *(TUint8**)iBase->Ptr(aIndex*sizeof(TUint8**)).Ptr()+iKeyOffset;
	}

TInt TKeyArrayHBufPtr::Compare(TInt aLeft,TInt aRight) const
	{
	if (iHBufType)
		{
		TInt retVal;

		if (iCompareType==ECompareNormal)
			retVal=(*(HBufC**)At(aLeft))->Compare(**((HBufC**)At(aRight)));
		else if (iCompareType==ECompareCollated)
			retVal=(*(HBufC**)At(aLeft))->CompareC(**((HBufC**)At(aRight)));
		else
			retVal=(*(HBufC**)At(aLeft))->CompareF(**((HBufC**)At(aRight)));

		return (iSortMode==ESortAscending?1:-1)*retVal;
		}
	else
		return TKeyArrayFix::Compare(aLeft,aRight);

	}

void TKeyArrayHBufPtr::SetCompareType(TCompareType aCompareType)
	{ 
	iCompareType = aCompareType; 
	}

void TKeyArrayHBufPtr::SetSortMode(TSortMode aSortMode)
	{ 
	iSortMode = aSortMode; 
	}

// CElement
CElement::CElement()
	{
	}

CElement::~CElement()
	{
	delete iBuf;
	}

void CElement::SetTextL(const TDesC& aText)
	{
	if (!iBuf)
		{
		iBuf  = HBufC::NewL(aText.Length());
		*iBuf = aText;	
		return;
		}

	if (aText.Length() > iBuf->Length())
		{
		delete iBuf;
		iBuf  = HBufC::NewL(aText.Length());
		*iBuf = aText;
		}
	else
		{
		*iBuf = aText;
		if (aText.Length() < iBuf->Length())
			iBuf  = iBuf->ReAllocL(aText.Length()); // reclaim space
		}
	}

//CArray
CArray::CArray() 
	{ 
	iItems=NULL;
	iSortMode = ESortAscending;
	iCompareType = ECompareNormal;
	iDuplicates = EDuplicatesAllow;
	iSorted = EFalse;
	}

CArray::~CArray()
	{
	if (iItems!=NULL)
		{
		iItems->ResetAndDestroy();
		delete iItems;
		}
	}

void CArray::ConstructL()
	{
	iItems = new (ELeave) CArrayPtrFlat<CElement>(16);
	}

void CArray::SetCompareType(TCompareType aCompareType)
	{
	iCompareType = aCompareType;
	}

void CArray::SetSortMode(TSortMode aSortMode)
	{
	iSortMode = aSortMode;
	}

void CArray::SetSorted(TBool aSorted, TBool aResortAlways)
	{
	if ((iSorted!=aSorted) || (aResortAlways))
		{
		iSorted = aSorted;
		if (iSorted)
			{
			TKeyArrayHBufPtr key=TKeyArrayHBufPtr(_FOFF(CElement,iBuf));
	
			key.SetCompareType(iCompareType);
			key.SetSortMode(iSortMode);
		
			iItems->Sort(key);
			}
		}
	}

void CArray::SetDuplicates(TDuplicates aDuplicates)
	{
	iDuplicates = aDuplicates;
	}

// CArrayOpx
CArrayOpx::CArrayOpx()
	{
	}

CArrayOpx::~CArrayOpx()
	{
	}

void CArrayOpx::ArrayNewL(OplAPI& aOplAPI) const
	{ //ArrayNew&:
	CArray* aArray;

    aArray=new (ELeave) CArray;
	aArray->ConstructL();

	aOplAPI.Push(TInt32(aArray));
	}

void CArrayOpx::ArrayFreeL(OplAPI& aOplAPI) const
	{ //ArrayFree:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	delete aArray;
	
	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArrayClearL(OplAPI& aOplAPI) const
	{ //ArrayClear:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aArray->Items()->ResetAndDestroy();
	
	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArraySetDuplicatesL(OplAPI& aOplAPI) const
	{ //ArraySetDuplicates:(ArrayId&,duplicates%)
	TDuplicates duplicates=TDuplicates(aOplAPI.PopInt16());
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aArray->SetDuplicates(duplicates);

	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArrayGetDuplicatesL(OplAPI& aOplAPI) const
	{ //ArrayGetDuplicates%:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aOplAPI.Push(TInt16(aArray->Duplicates()));

	}

void CArrayOpx::ArraySetCompareTypeL(OplAPI& aOplAPI) const
	{ //ArraySetCompareType:(ArrayId&,compare%)
	TCompareType compare=TCompareType(aOplAPI.PopInt16());
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aArray->SetCompareType(compare);

	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArrayGetCompareTypeL(OplAPI& aOplAPI) const
	{ //ArrayGetCompareType%:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aOplAPI.Push(TInt16(aArray->CompareType()));

	}

void CArrayOpx::ArraySetSortModeL(OplAPI& aOplAPI) const
	{ //ArraySetSortMode:(ArrayId&)
	TSortMode sort=TSortMode(aOplAPI.PopInt16());
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aArray->SetSortMode(sort);

	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArrayGetSortModeL(OplAPI& aOplAPI) const
	{ //ArrayGetSortMode%:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aOplAPI.Push(TInt16(aArray->SortMode()));

	}

void CArrayOpx::ArraySetSortedL(OplAPI& aOplAPI) const
{
	TBool sorted = OpxUtil::CppBool(aOplAPI.PopInt16());
	CArray* aArray = (CArray *)aOplAPI.PopInt32();
	
	aArray->SetSorted(sorted);

	aOplAPI.Push(TReal64(0.0));
}

void CArrayOpx::ArrayGetSortedL(OplAPI& aOplAPI) const
	{ //ArrayGetSorted%:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aOplAPI.Push(TInt16(OpxUtil::OplBool16(aArray->Sorted())));
	}

void CArrayOpx::ArrayAddL(OplAPI& aOplAPI, TBool Int32) const
	{ //ArrayAdd:(ArrayId&,value$)
	TPtrC value=aOplAPI.PopString();
	CArray* aArray = (CArray *)aOplAPI.PopInt32();
	CElement* aElement;
	TInt index=-1;
	TBool destroyElement=EFalse;

	aElement=new (ELeave) CElement;
	aElement->SetTextL(value);

	CleanupStack::PushL(aElement);

	if (aArray->Sorted())
		{
		TKeyArrayHBufPtr key=TKeyArrayHBufPtr(_FOFF(CElement,iBuf));
	
		key.SetCompareType(aArray->CompareType());
		key.SetSortMode(aArray->SortMode());

		if (aArray->Duplicates()==EDuplicatesAllow)
			{
			index=aArray->Items()->InsertIsqAllowDuplicatesL(aElement,key);
			}
		else
			{
			TRAPD(err,index=aArray->Items()->InsertIsqL(aElement,key));
			if (err)
				{
				if ((aArray->Duplicates()==EDuplicatesIgnore) && (err==KErrAlreadyExists))
				{
					index=-1;
					destroyElement=ETrue;
				}
				else
					User::Leave(err);
				}
			}
	}
	else
		{
		aArray->Items()->AppendL(aElement);
		index=aArray->Items()->Count()-1;
		}

	if (destroyElement) // aElement
		CleanupStack::PopAndDestroy();
	else
		CleanupStack::Pop();

	if (Int32)
		aOplAPI.Push(TInt32(index));
	else
		aOplAPI.Push(TInt16(index));
	}

void CArrayOpx::ArrayInsertL(OplAPI& aOplAPI, TBool Int32) const
	{ //ArrayInsert:(ArrayId&,index%,value$)
	TPtrC value=aOplAPI.PopString();
	TInt index=Int32?aOplAPI.PopInt32():aOplAPI.PopInt16();
	CArray* aArray = (CArray *)aOplAPI.PopInt32();
	CElement* aElement;

	if ((index<0) || (index>=aArray->Items()->Count()))
		User::Leave(KOplErrInvalidArgs);

	aElement=new (ELeave) CElement;
	CleanupStack::PushL(aElement);

	aElement->SetTextL(value);
	aArray->Items()->InsertL(index,aElement);
	
	CleanupStack::Pop();

	aArray->SetSorted(EFalse);
	aOplAPI.Push(TReal64(0.0));
	}


void CArrayOpx::ArrayReplaceL(OplAPI& aOplAPI, TBool Int32) const
	{ //ArrayReplace:(ArrayId&,index%,value$)
	TPtrC value=aOplAPI.PopString();
	TInt index=Int32?aOplAPI.PopInt32():aOplAPI.PopInt16();
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	if ((index<0) || (index>=aArray->Items()->Count()))
		User::Leave(KOplErrOutOfRange);

	aArray->Items()->At(index)->SetTextL(value);
	aArray->SetSorted(EFalse);

	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArrayDeleteL(OplAPI& aOplAPI,TBool Int32) const
	{ //ArrayDelete:(ArrayId&,index%)
	TInt index=Int32?aOplAPI.PopInt32():aOplAPI.PopInt16();
	CArray* aArray = (CArray *)aOplAPI.PopInt32();
	CElement *aElement;

	if ((index<-1) || (index>=aArray->Items()->Count()))
		User::Leave(KOplErrOutOfRange);

	if (index!=-1)
		{
		aElement=aArray->Items()->At(index);
		aArray->Items()->Delete(index);
		delete aElement;
		}

	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArraySizeL(OplAPI& aOplAPI,TBool Int32) const
	{ //ArraySize%:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	if (Int32)
		aOplAPI.Push(TInt32(aArray->Items()->Count()));
	else
		aOplAPI.Push(TInt16(aArray->Items()->Count()));
	}

void CArrayOpx::ArrayAtL(OplAPI& aOplAPI, TBool Int32) const
	{ //ArrayAt$:(ArrayId&,index%)
	TInt index=Int32?aOplAPI.PopInt32():aOplAPI.PopInt16();
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	if ((index<0) || (index>=aArray->Items()->Count()))
		User::Leave(KOplErrOutOfRange);

	aOplAPI.PushL(*(aArray->Items()->At(index)->iBuf));
	}

void CArrayOpx::ArrayFindL(OplAPI& aOplAPI,TBool Int32) const
	{ //ArrayFind&:(ArrayId&,value$)
	TPtrC value=aOplAPI.PopString();
	CArray* aArray = (CArray *)aOplAPI.PopInt32();
	CElement* aElement;
	TInt index;
	TKeyArrayHBufPtr key=TKeyArrayHBufPtr(_FOFF(CElement,iBuf));
	
	key.SetCompareType(aArray->CompareType());

	aElement = new (ELeave) CElement;
	CleanupStack::PushL(aElement);

	aElement->SetTextL(value);

	if (aArray->Sorted())
		{
		key.SetSortMode(aArray->SortMode());
		if (aArray->Items()->FindIsq(aElement,key,index))
			index=-1;
		}
	else
		{
		if (aArray->Items()->Find(aElement,key,index))
			index=-1;
	}

	CleanupStack::PopAndDestroy(); // aElement;

	if (Int32)
		aOplAPI.Push(TInt16(index));
	else
		aOplAPI.Push(TInt16(index));
	}

void CArrayOpx::ArraySortL(OplAPI& aOplAPI) const
	{ //ArraySort:(ArrayId&)
	CArray* aArray = (CArray *)aOplAPI.PopInt32();

	aArray->SetSorted(ETrue,ETrue);

	aOplAPI.Push(TReal64(0.0));
	}

void CArrayOpx::ArrayFindPartialL(OplAPI& aOplAPI, TBool aInt32)
	{ //ArraySearch%:(ArrayId&,aStartPos%,aValue$,aSearchMode%)
	TFindPartial findPartial = (TFindPartial)aOplAPI.PopInt16();
	TPtrC value=aOplAPI.PopString();
	TInt index=aInt32?aOplAPI.PopInt32():aOplAPI.PopInt16();
	CArray* aArray = (CArray *)aOplAPI.PopInt32();
	TInt i,pos;
	
	if (index<0)
		User::Leave(KOplErrOutOfRange);

	i=index;
	while (i<aArray->Items()->Count())
		{
		if (findPartial==EFindSubString)
		{
			if (aArray->CompareType()==ECompareNormal)
				pos=aArray->Items()->At(i)->iBuf->Find(value);
			else if (aArray->CompareType()==ECompareCollated)
				pos=aArray->Items()->At(i)->iBuf->FindC(value);
			else
				pos=aArray->Items()->At(i)->iBuf->FindF(value);
		}
		else
		{
			if (aArray->CompareType()==ECompareNormal)
				pos=aArray->Items()->At(i)->iBuf->Match(value);
			else if (aArray->CompareType()==ECompareCollated)
				pos=aArray->Items()->At(i)->iBuf->MatchC(value);
			else
				pos=aArray->Items()->At(i)->iBuf->MatchF(value);
		}
		if (pos!=KErrNotFound)
			{
			if (aInt32)
				aOplAPI.Push(TInt32(i));
			else
				aOplAPI.Push(TInt16(i));
			return;
			}
		i++;
		}
	if (aInt32)
		aOplAPI.Push(TInt32(-1));
	else
		aOplAPI.Push(TInt16(-1));
	}

// CTlsDataOPXArray
CTlsDataOPXArray::CTlsDataOPXArray(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

CTlsDataOPXArray* CTlsDataOPXArray::NewL(OplAPI& aOplAPI)
	{
	CTlsDataOPXArray* This=new(ELeave) CTlsDataOPXArray(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	CleanupStack::Pop();
	return This;
	}

void CTlsDataOPXArray::ConstructL()
	{
	iArrayHandle= new(ELeave) CArrayOpx;
	} 

CTlsDataOPXArray::~CTlsDataOPXArray()
	{
	delete iArrayHandle;
	Dll::FreeTls();
	}

void CTlsDataOPXArray::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case EArrayNew:
		iArrayHandle->ArrayNewL(iOplAPI);
		break;
	case EArrayFree:
		iArrayHandle->ArrayFreeL(iOplAPI);
		break;
	case EArrayClear:
		iArrayHandle->ArrayClearL(iOplAPI);
		break;

	case EArraySetDuplicates:
		iArrayHandle->ArraySetDuplicatesL(iOplAPI);
		break;
	case EArrayGetDuplicates:
		iArrayHandle->ArrayGetDuplicatesL(iOplAPI);
		break;
	case EArraySetCompareType:
		iArrayHandle->ArraySetCompareTypeL(iOplAPI);
		break;
	case EArrayGetCompareType:
		iArrayHandle->ArrayGetCompareTypeL(iOplAPI);		
		break;
	case EArraySetSortMode:
		iArrayHandle->ArraySetSortModeL(iOplAPI);
		break;
	case EArrayGetSortMode:
		iArrayHandle->ArrayGetSortModeL(iOplAPI);
		break;
	case EArraySetSorted:
		iArrayHandle->ArraySetSortedL(iOplAPI);
		break;
	case EArrayGetSorted:
		iArrayHandle->ArrayGetSortedL(iOplAPI);
		break;

	case EArrayAdd:
		iArrayHandle->ArrayAddL(iOplAPI,EFalse);
		break;
	case EArrayInsert:
		iArrayHandle->ArrayInsertL(iOplAPI,EFalse);
		break;

	case EArrayReplace:
		iArrayHandle->ArrayReplaceL(iOplAPI,EFalse);
		break;
	case EArrayDelete:
		iArrayHandle->ArrayDeleteL(iOplAPI,EFalse);
		break;

	case EArraySize:
		iArrayHandle->ArraySizeL(iOplAPI,EFalse);
		break;

	case EArrayAt:
		iArrayHandle->ArrayAtL(iOplAPI,EFalse);
		break;

	case EArrayFind:
		iArrayHandle->ArrayFindL(iOplAPI,EFalse);
		break;

	case EArraySort:
		iArrayHandle->ArraySortL(iOplAPI);
		break;

	case EArraySearch:
		iArrayHandle->ArrayFindPartialL(iOplAPI,EFalse);
		break;

	case EArrayAddL:
		iArrayHandle->ArrayAddL(iOplAPI,ETrue);
		break;
	case EArrayInsertL:
		iArrayHandle->ArrayInsertL(iOplAPI,ETrue);
		break;

	case EArrayReplaceL:
		iArrayHandle->ArrayReplaceL(iOplAPI,ETrue);
		break;
	case EArrayDeleteL:
		iArrayHandle->ArrayDeleteL(iOplAPI,ETrue);
		break;

	case EArraySizeL:
		iArrayHandle->ArraySizeL(iOplAPI,ETrue);
		break;

	case EArrayAtL:
		iArrayHandle->ArrayAtL(iOplAPI,ETrue);
		break;

	case EArrayFindL:
		iArrayHandle->ArrayFindL(iOplAPI,ETrue);
		break;

	case EArraySearchL:
		iArrayHandle->ArrayFindPartialL(iOplAPI,ETrue);
		break;

	default:
		User::Leave(KOplErrOpxProcNotFound);
		}

	}

//
// OPX loading interface
//
TBool CTlsDataOPXArray::CheckVersion(TInt aVersion)
	// To check whether the opx is a compatible version
	// *** Change as required ***
	{
	if ((aVersion & 0xff00)>(KArrayOpxVersion & 0xff00)) // Major version must be <= OPX's version
		return EFalse;	// Bad version
	else
		return ETrue;	// OK
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	CTlsDataOPXArray* tls=((CTlsDataOPXArray*)Dll::Tls());
	if (tls==NULL)
		{
		tls=CTlsDataOPXArray::NewL(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KArrayOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	//
	// DLL entry point
	//
	{
	return(KErrNone);
	}
