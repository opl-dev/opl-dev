// DBASEOPX.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include <apadef.h>
#include "DBaseOpx.h"

CDBaseOpx::CDBaseOpx()
	:iKeyArray(4)
	{
	}

CDBaseOpx::~CDBaseOpx() 
	{
	iKeyArray.ResetAndDestroy();
	}

void CDBaseOpx::NewKey(OplAPI& aOplAPI)
    {
    CDbKey* keyPtr = CDbKey::NewL();
    TRAPD(err,iKeyArray.AppendL(keyPtr));
    if (err)
        {
        delete keyPtr;
        User::Leave(err);
        }
    aOplAPI.Push(TInt32(keyPtr));
    }

void CDBaseOpx::DeleteKey(OplAPI& aOplAPI)
    {
    CDbKey* keyPtr=(CDbKey *)aOplAPI.PopInt32();
    TInt pos=CheckPointerL(keyPtr);
    delete keyPtr;
    iKeyArray.Delete(pos);
    aOplAPI.Push(TReal64(0.0));
    }

TInt CDBaseOpx::CheckPointerL(CDbKey* aKeyPtr)
    {
    TInt count=iKeyArray.Count();
    TInt ii;
    for(ii=0;ii<count;ii++)
        {
        if (iKeyArray[ii]==aKeyPtr)
            return ii;
        }
    User::Leave(KOplErrInvalidArgs);
    return ii;    
    }

LOCAL_C void OpenDbaseShareLC(OplAPI& aOplAPI,TPtrC aDbase,RDbStoreDatabase& aStoreDatabase)
    {
    RFs& fs=aOplAPI.DbManager()->FsSession();
    TParse parser;
	fs.Parse(aDbase,parser);
    TUidType type(KPermanentFileStoreLayoutUid,KUidAppDllDoc,KUidDataApp);
    TEntry entry;
    fs.Entry(parser.FullName(),entry);
    if (entry.iType.MostDerived()==type.MostDerived())
        //data model
        {
        CFileStore* store=CFileStore::OpenLC(fs,parser.FullName(),EFileRead);	// READ ONLY FOR 	// cleaned by caller
        CStreamDictionary* streamDic = CStreamDictionary::NewLC();
        RStoreReadStream inStream;
        inStream.OpenLC(*store,store->Root()); 
        inStream >> *streamDic;
        CleanupStack::PopAndDestroy(); // stream
        TStreamId dataHeadStreamId;
        dataHeadStreamId = streamDic->At(KUidDataHeadStream);
        CleanupStack::PopAndDestroy();  // streamDic
        inStream.OpenLC(*store, dataHeadStreamId); 
        TStreamId dbStreamId;
        inStream >> dbStreamId;
        CleanupStack::PopAndDestroy(); // stream
        aStoreDatabase.OpenL(store,dbStreamId); 
        }
    else
        {
		CFileStore* store=CFileStore::OpenLC(fs,parser.FullName(),EFileRead|EFileWrite);	// file closed on error
		RStoreReadStream inStream;
		inStream.OpenLC(*store,store->Root());
		TOplDocRootStream root;
		inStream>>root;
		CleanupStack::PopAndDestroy();	// inStream
		aStoreDatabase.OpenL(store,root.iStreamId);
		}
    }

LOCAL_C void AddColShare(OplAPI& aOplAPI ,CDBaseOpx& dbo ,TInt32 aTrunc)
    {
    TInt32 order=aOplAPI.PopInt32();
    TPtrC colName=aOplAPI.PopString();
    CDbKey* keyPtr=(CDbKey*)aOplAPI.PopInt32();
    dbo.CheckPointerL(keyPtr);
    if (order!=0 && order!=1)
        User::Leave(KOplErrInvalidArgs);
    TDbKeyCol::TOrder orderb(TDbKeyCol::EAsc);
    if(order==0)
        orderb=TDbKeyCol::EDesc;
    TDbKeyCol ret;
    if (aTrunc==-1)
        {
        TDbKeyCol keyCol(colName,orderb);
        ret=keyCol;
        }
    else
        {
        TDbKeyCol keyCol(colName,aTrunc,orderb);
        ret=keyCol;
        }
    keyPtr->AddL(ret);
    aOplAPI.Push(TReal64(0.0));
    }

void CDBaseOpx::AddField(OplAPI& aOplAPI)
    {
    AddColShare(aOplAPI,*this,-1);
    }                                               

void CDBaseOpx::AddFieldTrunc(OplAPI& aOplAPI)
    {
    TInt32 trunc=aOplAPI.PopInt32();
    if (trunc<0 || trunc>240)
        User::Leave(KOplErrInvalidArgs);
    AddColShare(aOplAPI,*this,trunc);
    }

void CDBaseOpx::SetComparison(OplAPI& aOplAPI)
    {
    TInt32 comp=aOplAPI.PopInt32();
    CDbKey* keyPtr=(CDbKey*)aOplAPI.PopInt32();
    CheckPointerL(keyPtr);
    if (comp<0 || comp>2)
        User::Leave(KOplErrInvalidArgs);
//    TDbTextComparison     
    keyPtr->SetComparison((TDbTextComparison)comp);
    aOplAPI.Push(TReal64(0.0));
    }

void CDBaseOpx::MakeUnique(OplAPI& aOplAPI)
    {
    CDbKey* keyPtr=(CDbKey*)aOplAPI.PopInt32();
    CheckPointerL(keyPtr);
    keyPtr->MakeUnique();
    aOplAPI.Push(TReal64(0.0));
    }

void CDBaseOpx::IsUnique(OplAPI& aOplAPI)
    {
    CDbKey* keyPtr=(CDbKey*)aOplAPI.PopInt32();
    CheckPointerL(keyPtr);
    aOplAPI.Push(TInt32(keyPtr->IsUnique()? -1 : 0));
    }

void CDBaseOpx::CreateIndex(OplAPI& aOplAPI)
    {
    TPtrC table=aOplAPI.PopString();
    TPtrC dbase=aOplAPI.PopString();
    CDbKey* keyPtr=(CDbKey*)aOplAPI.PopInt32();
    TPtrC index=aOplAPI.PopString();
    CheckPointerL(keyPtr);
    RDbStoreDatabase thedbase;
    OpenDbaseShareLC(aOplAPI,dbase,thedbase);
    User::LeaveIfError(thedbase.CreateIndex(index,table,*keyPtr));
    aOplAPI.Push(TReal64(0.0));
    thedbase.Close();
    CleanupStack::PopAndDestroy();    // store
    }

void CDBaseOpx::DropIndex(OplAPI& aOplAPI)
    {
    TPtrC table=aOplAPI.PopString();
    TPtrC dbase=aOplAPI.PopString();
    TPtrC index=aOplAPI.PopString();
    RDbStoreDatabase thedbase;
    OpenDbaseShareLC(aOplAPI,dbase,thedbase);
    User::LeaveIfError(thedbase.DropIndex(index,table));
    aOplAPI.Push(TReal64(0.0));
    thedbase.Close();
    CleanupStack::PopAndDestroy();    // store
    }

void CDBaseOpx::Recover(OplAPI& aOplAPI)
    {
    TPtrC dbase=aOplAPI.PopString();
    RDbStoreDatabase thedbase;
    OpenDbaseShareLC(aOplAPI,dbase,thedbase);
    User::LeaveIfError(thedbase.Recover());
    aOplAPI.Push(TReal64(0.0));
    thedbase.Close();
    CleanupStack::PopAndDestroy();    // store
    }

void CDBaseOpx::IsDamaged(OplAPI& aOplAPI)
    {
    TPtrC dbase=aOplAPI.PopString();

    RDbStoreDatabase thedbase;
    OpenDbaseShareLC(aOplAPI,dbase,thedbase);
    aOplAPI.Push(TInt32(thedbase.IsDamaged()));
    thedbase.Close();
    CleanupStack::PopAndDestroy();    // store
    }

void CDBaseOpx::GetFieldCount(OplAPI& aOplAPI)
    {
    TPtrC table=aOplAPI.PopString();
    TPtrC dbase=aOplAPI.PopString();
    RDbStoreDatabase thedbase;
    OpenDbaseShareLC(aOplAPI,dbase,thedbase);
	
	RDbTable tabDBMS;					 
	RDbRowSet::TAccess access=RDbRowSet::EUpdatable;
	User::LeaveIfError(tabDBMS.Open(thedbase,table, access)); // Allow updates default - ETRUE
	CDbColSet* colSet=tabDBMS.ColSetL();
    aOplAPI.Push(TInt32(colSet->Count()));
	tabDBMS.Close();
	thedbase.Close();
	delete colSet;
	CleanupStack::PopAndDestroy();    // store
    }

LOCAL_C void GetFieldShare(OplAPI& aOplAPI,TInt aDo)
    {
	TInt colNo=aOplAPI.PopInt32();
    TPtrC table=aOplAPI.PopString();
    TPtrC dbase=aOplAPI.PopString();
    RDbStoreDatabase thedbase;
    OpenDbaseShareLC(aOplAPI,dbase,thedbase);	
	RDbTable tabDBMS;					 
	RDbRowSet::TAccess access=RDbRowSet::EUpdatable;
	User::LeaveIfError(tabDBMS.Open(thedbase,table, access)); // Allow updates default - ETRUE
    CDbColSet* colSet=tabDBMS.ColSetL();
	CleanupStack::PushL(colSet);
    if (aDo==1)
		aOplAPI.PushL((*colSet)[colNo].iName);
	else
		aOplAPI.Push((TInt32)(*colSet)[colNo].iType);	
	tabDBMS.Close();
    thedbase.Close();
    CleanupStack::PopAndDestroy();    // colset
	CleanupStack::PopAndDestroy();    // store
    }

void CDBaseOpx::GetFieldType(OplAPI& aOplAPI)
    {
	TInt toDo=0;
	GetFieldShare(aOplAPI,toDo);      
    }

void CDBaseOpx::GetFieldName(OplAPI& aOplAPI)
    {
	TInt toDo=1;
	GetFieldShare(aOplAPI,toDo);      
    }

void CDBaseOpx::RefreshDatabase(OplAPI& aOplAPI)
    { // DbRefresh:
	COplRowSet* currentRowSet=aOplAPI.DbManager()->TheCurrentOplRs();
	if (currentRowSet==NULL)
		User::Leave(KOplErrNotOpen); // No db available.
	RDbView* dbView=STATIC_CAST(RDbView*,currentRowSet->AccessDbRowSet());
	if (dbView==NULL)
		User::Leave(KOplErrRecord); // No view
	dbView->Reset();
	User::LeaveIfError(dbView->EvaluateAll());
	aOplAPI.DbManager()->FirstL(); // position to first record.
	aOplAPI.Push(TReal64(0.0));
    }

CTlsDataOPXDBase::CTlsDataOPXDBase(OplAPI& aOplAPI) : COpxBase(aOplAPI)
	{
	}

CTlsDataOPXDBase* CTlsDataOPXDBase::NewL(OplAPI& aOplAPI)
    {
    CTlsDataOPXDBase* This=new(ELeave) CTlsDataOPXDBase(aOplAPI);
    CleanupStack::PushL(This);
    This->ConstructL();
    CleanupStack::Pop();
    return This;
    }

void CTlsDataOPXDBase::ConstructL()
    {
    iDBaseHandle= new(ELeave) CDBaseOpx;
    } 

CTlsDataOPXDBase::~CTlsDataOPXDBase()
    {
    delete iDBaseHandle;
	Dll::FreeTls();
    }

void CTlsDataOPXDBase::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case EAddField:
         iDBaseHandle->AddField(iOplAPI);
         break;
	case EAddFieldTrunc:
         iDBaseHandle->AddFieldTrunc(iOplAPI);
         break;
	case ECreateIndex:
         iDBaseHandle->CreateIndex(iOplAPI);
         break;
	case EDeleteKey:
         iDBaseHandle->DeleteKey(iOplAPI);
         break;
    case EDropIndex:
         iDBaseHandle->DropIndex(iOplAPI);
         break;
	case EGetFieldCount:
         iDBaseHandle->GetFieldCount(iOplAPI);
         break;
	case EGetFieldName:
         iDBaseHandle->GetFieldName(iOplAPI);
         break;
	case EGetFieldType:
         iDBaseHandle->GetFieldType(iOplAPI);
         break;
	case EIsDamaged:
         iDBaseHandle->IsDamaged(iOplAPI);
         break;
    case EIsUnique:
         iDBaseHandle->IsUnique(iOplAPI);
         break;
    case EMakeUnique:
         iDBaseHandle->MakeUnique(iOplAPI);
         break;
	case ENewKey:
         iDBaseHandle->NewKey(iOplAPI);
         break;
	case ERecover:
         iDBaseHandle->Recover(iOplAPI);
         break;
	case ESetComparison:
	     iDBaseHandle->SetComparison(iOplAPI);
         break;
	case ERefreshDatabase:
	     iDBaseHandle->RefreshDatabase(iOplAPI);
         break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

//
// OPX loading interface
//

TBool CTlsDataOPXDBase::CheckVersion(TInt aVersion)
// To check whether the opx is a compatible version
// *** Change as required ***
	{
	if ((aVersion & 0xff00)>(KOpxVersion & 0xff00))	// major version must be <= OPX's version
		return EFalse; // bad version
	else
		return ETrue; // ok
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
// Creates a COpxBase instance as required by the OPL runtime
// This object is to be stored in the OPX's TLS as shown below
	{
	CTlsDataOPXDBase* tls=((CTlsDataOPXDBase*)Dll::Tls());
	if (tls==NULL)
		{
        tls=CTlsDataOPXDBase::NewL(aOplAPI);
	    Dll::SetTls(tls);
        }
    return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return(KErrNone);
	}