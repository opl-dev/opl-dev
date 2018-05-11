// DATA.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <f32file.h>
#include <s32file.h>
#include <d32dbms.h>
#include <e32base.h>

#include <opx.h>
#include <oplapi.h>
#include <oplerr.h>
#include <opldb.h>
#include <opldoc.h>

#include <eikenv.h>

// Define UIDs to match the correct version of OPL database files
#ifdef _UNICODE
	#define KUidOplInterpreter 0x10005D2E
	#define KUidExternalOplFile 0x1000008A
#else
	#define KUidOplInterpreter 0x10000168
	#define KUidExternalOplFile 0x1000008A
#endif

const TInt KUidOpxOplDb=0x10004EC7;
const TInt KOpxDataVersion=0x600;

class COpxOplDb : public COpxBase 
	{
public:
	static COpxOplDb* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
	// The language extension procedures
	enum TExtensions
		{
		EODbGetTableCount=1,
		EODbGetTableName,
		EODbGetIndexCount,
		EODbGetIndexName,
		EODbGetIndexDescription,
		EODbGetFieldCount,
		EODbGetFieldName,
		EODbGetFieldType,
		EODbGetFieldSize,
		EODbGetCanBeEmpty,
		EODbOpenR,
		EODbOpen,
		EODbStartTable,
		EODbTableField,
		EODbCreateTable,
		EODbGetLength,
		EODbGetString,
		EODbGetInt,
		EODbGetReal,
		EODbGetReal32,
		EODbGetWord,
		EODbGetDateTime,
		EODbGetLong,
		EODbPutEmpty,
		EODbPutString,
		EODbPutInt,
		EODbPutReal,
		EODbPutReal32,
		EODbPutWord,
		EODbPutDateTime,
		EODbPutLong,
		EODbFindWhere,
		EODbUse,
		EODbSeekInt,
		EODbSeekWord,
		EODbSeekText,
		EODbSeekReal,
		EODbSeekDateTime,
		EODbCount,
		EODbFindSql
		};
	void DbGetTableCount() const;
	void DbGetTableName() const;
	void DbGetIndexCount() const;
	void DbGetIndexName() const;
	void DbGetIndexDescription() const;
	void DbGetFieldCount() const;
	void DbGetField(TInt aProcNum) const;
	void DbOpen(TBool allowUpdates) const;
	void DbStartTable();
	void DbTableField();
	void DbCreateTable();
	void DbGet(TInt aProcNum) const;
	void DbPut(TInt aProcNum) const;
	TInt DbFindWhere() const;
	void DbUse() const;
	void DbSeek(TInt aProcNum) const;
	void DbCount() const;
private:
	COpxOplDb(OplAPI& aOplAPI);
	void ConstructL();
	~COpxOplDb();
private:
	CDbColSet* iDbColSet;
	};

//
// Accessing an OPL database
//
class CDbasePtr : public CBase
	{
public:
	static CDbasePtr* NewLC(OplAPI &anOplAPI, const TDesC &aFileName, 
							TBool anUpdate, TBool aCreateIfAbsent);
	~CDbasePtr();
	RDbStoreDatabase& Dbase() { return *iDbasePtr; }
private:
	CDbasePtr();
private:
	RDbStoreDatabase* iDbasePtr;
	RDbStoreDatabase iDbase;
	TBool iOwner;
	CFileStore* iStore;
	};

CDbasePtr::CDbasePtr()
	:iOwner(EFalse)
	{
	}

CDbasePtr::~CDbasePtr()
	{
	if (iOwner)
		{
		iDbase.Close();
		delete iStore;
		}
	}

CDbasePtr* CDbasePtr::NewLC(OplAPI &anOplAPI, const TDesC &aFileName, 
							TBool anUpdate, TBool aCreateIfAbsent)
	{
	COplDb* db;
	TParse parser;
	RFs& aFs = anOplAPI.EikonEnv().FsSession();
	aFs.Parse(aFileName, parser);
	CDbasePtr* self = new (ELeave) CDbasePtr;
	CleanupStack::PushL(self);
	if ((db=anOplAPI.DbManager()->OplDbCollection()->FindOpenDbase(parser.FullName(),EFalse))!=NULL)
		{
		// db is open in OPL program
		self->iDbasePtr = &db->StoreDbase();
		return self;
		}
	// Database file is not currently open.	Does it exist?
	TEntry entry;
	TInt err = aFs.Entry(parser.FullName(),entry);
	if (!err)
		{
		// the file exists, open database
		self->iStore=CFileStore::OpenLC(aFs,parser.FullName(),
			(anUpdate ? (EFileRead|EFileWrite) : EFileRead)); // file closed on error	
			RStoreReadStream inStream;
		inStream.OpenLC(*(self->iStore), self->iStore->Root());
		TOplDocRootStream shellData;
		inStream>>shellData;
		CleanupStack::PopAndDestroy(); // inStream
		self->iDbase.OpenL(self->iStore, shellData.iStreamId);
		self->iOwner = ETrue;
		self->iDbasePtr = &(self->iDbase);
		CleanupStack::Pop(); // iStore now owned by self
		return self;
		}
	// File does not exist
	if (!aCreateIfAbsent)
		User::Leave(KOplErrNotExists);
	// We have to create the file with an empty database
	self->iStore=CPermanentFileStore::CreateLC(aFs,parser.FullName(),EFileRead|EFileWrite);
	TUidType uids(KPermanentFileStoreLayoutUid,TUid::Uid(KUidExternalOplFile));
	self->iStore->SetTypeL(uids);
	// Write the root stream
	TOplDocRootStream shellData;
	shellData.iStreamId=self->iDbase.CreateL(self->iStore);
	shellData.iAppUid=TUid::Uid(KUidOplInterpreter);

	RStoreWriteStream root;
	TStreamId rootId=root.CreateLC(*(self->iStore));
	root<<shellData;
	root.CommitL(); // root stream
	CleanupStack::PopAndDestroy(); // root

	self->iStore->SetRootL(rootId);
	self->iStore->CommitL();
	self->iOwner = ETrue;
	self->iDbasePtr = &(self->iDbase);
	CleanupStack::Pop(); // iStore now owned by self
	return self;
	}

//
// The language extension procedures provided by this OPX
//
void COpxOplDb::DbGetTableCount() const
	{
	TPtrC dbname = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, dbname, EFalse, EFalse);
	CDbTableNames* tables = dbase->Dbase().TableNamesL();
	CleanupStack::PushL(tables);
	TInt32 noTables = tables->Count();
	iOplAPI.Push(noTables);
	CleanupStack::PopAndDestroy(2);	// tables, dbase
	}

void COpxOplDb::DbGetTableName() const
	{
	TInt ind = iOplAPI.PopInt32();
	TPtrC dbname = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, dbname, EFalse, EFalse);
	CDbTableNames* tables = dbase->Dbase().TableNamesL();
	CleanupStack::PushL(tables);
	if (ind < 1 || tables->Count() < ind)
		User::Leave(KOplErrOutOfRange);

	TBufC<255> tname = (*tables)[ind-1];
	iOplAPI.PushL(tname);
	CleanupStack::PopAndDestroy(2);	// tables, dbase
	}

void COpxOplDb::DbGetIndexCount() const
	{
	TPtrC tbname = iOplAPI.PopString();
	TPtrC dbname = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, dbname, EFalse, EFalse);
	CDbIndexNames* indices = dbase->Dbase().IndexNamesL(tbname);
	CleanupStack::PushL(indices);
	TInt32 noIndex = indices->Count();
	iOplAPI.Push(noIndex);
	CleanupStack::PopAndDestroy(2);	// indices, dbase
	}

void COpxOplDb::DbGetIndexName() const
	{
	TInt ind = iOplAPI.PopInt32();
	TPtrC tbname = iOplAPI.PopString();
	TPtrC dbname = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, dbname, EFalse, EFalse);
	CDbIndexNames* indices = dbase->Dbase().IndexNamesL(tbname);
	CleanupStack::PushL(indices);
	if (ind < 1 || indices->Count() < ind)
		User::Leave(KOplErrOutOfRange);

	TBufC<255> iname = (*indices)[ind-1];
	iOplAPI.PushL(iname);
	CleanupStack::PopAndDestroy(2);	// indices, dbase
	}

void COpxOplDb::DbGetIndexDescription() const
	{
	TPtrC ixname = iOplAPI.PopString();
	TPtrC tbname = iOplAPI.PopString();
	TPtrC dbname = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, dbname, EFalse, EFalse);
	CDbKey* aKey = dbase->Dbase().KeyL(ixname, tbname);
	CleanupStack::PushL(aKey);
	TBuf<255> keyDesc;
	TInt i;
	TDbKeyCol aKeyCol;

	_LIT(KOpenBrace," (");
	_LIT(KCloseBrace,") ");
	_LIT(KAscending,"Ascending ");
	_LIT(KDescending,"Descending ");
	_LIT(KUnique," Unique");
	_LIT(KFolded," Folded");
	_LIT(KCollated," Collated");

	for (i = 0; i < aKey->Count(); i++)
		{
		aKeyCol = (*aKey)[i];
		keyDesc.Append(aKeyCol.iName);
		keyDesc.Append(KOpenBrace);
		keyDesc.AppendNum(aKeyCol.iLength);
		keyDesc.Append(KCloseBrace);
		if (aKeyCol.iOrder == TDbKeyCol::EAsc)
			keyDesc.Append(KAscending);
		else
			keyDesc.Append(KDescending);
		}
	if (aKey->IsUnique())
		keyDesc.Append(KUnique);
	switch (aKey->Comparison())
		{
	case EDbCompareNormal:
		break;
	case EDbCompareFolded:
		keyDesc.Append(KFolded);
		break;
	case EDbCompareCollated:
		keyDesc.Append(KCollated);
		break;
		}
	iOplAPI.PushL(keyDesc);
	CleanupStack::PopAndDestroy(2);	// aKey, dbase
	}

void COpxOplDb::DbGetFieldCount() const
	{
	TPtrC tbname = iOplAPI.PopString();
	TPtrC dbname = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, dbname, EFalse, EFalse);
	CDbColSet* aColSet = dbase->Dbase().ColSetL(tbname);
	CleanupStack::PushL(aColSet);
	TInt32 noField = aColSet->Count();
	iOplAPI.Push(noField);
	CleanupStack::PopAndDestroy(2);	// aColSet, dbase
	}

void COpxOplDb::DbGetField(TInt aProcNum) const
	{
	TInt ind = iOplAPI.PopInt32();
	TPtrC tbname = iOplAPI.PopString();
	TPtrC dbname = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, dbname, EFalse, EFalse);
	CDbColSet* aColSet = dbase->Dbase().ColSetL(tbname);
	CleanupStack::PushL(aColSet);
	if (ind < 1 || aColSet->Count() < ind)
		User::Leave(KOplErrOutOfRange);
	// Get the information
	switch (aProcNum)
		{
	case EODbGetFieldName:
		iOplAPI.PushL((*aColSet)[ind].iName);
		break;
	case EODbGetFieldType:
		iOplAPI.Push(TInt32((*aColSet)[ind].iType));
		break;
	case EODbGetCanBeEmpty:
		iOplAPI.Push(((*aColSet)[ind].iAttributes & TDbCol::ENotNull) ? TInt16(0) : TInt16(-1));
		break;
	case EODbGetFieldSize:
		iOplAPI.Push(TInt32((*aColSet)[ind].iMaxLength));
		break;
		}
	CleanupStack::PopAndDestroy(2);	// aColSet, dbase
	}

void COpxOplDb::DbOpen(TBool allowUpdate) const
	{
	TPtrC theTypes = iOplAPI.PopString();
	TPtrC string = iOplAPI.PopString();
	TInt aLog = iOplAPI.PopInt32();
	if (aLog < EDbA || EDbZ < aLog)
		User::Leave(KOplErrOutOfRange);
	COplDbManager* dbManager = iOplAPI.DbManager();
	if (dbManager->GetiLogNames()->FindRowSet(aLog)!=NULL)				 
		User::Leave(KOplErrOpen);
	COplFieldMap* aFieldMap = COplFieldMap::NewLC(theTypes.Length());
	TInt i;
	TBuf<20> fieldName;
	_LIT(KFieldFormat,"F%d");
	for (i = 0; i < theTypes.Length(); i++)
		{
		fieldName.Format(KFieldFormat, i + 1);
		switch (theTypes[i])
			{
		case '%':
			fieldName.Append('%');
			aFieldMap->AppendFieldL(fieldName, 0);
			break;
		case '&':
			fieldName.Append('&');
			aFieldMap->AppendFieldL(fieldName, 1);
			break;
		case '.':
			aFieldMap->AppendFieldL(fieldName, 2);
			break;
		case '$':
			fieldName.Append('$');
			aFieldMap->AppendFieldL(fieldName, 3);
			break;
		case '?':
			aFieldMap->AppendFieldL(fieldName, 4);
			break;
		default:
			User::Leave(KOplErrOutOfRange);
			}
		}
	COplRowSet* aRowSet = COplRowSet::NewL(*(dbManager->OplDbCollection()), 
		iOplAPI.EikonEnv().FsSession(), string, aFieldMap,
		EDbOpen, allowUpdate);
	aRowSet->SetOplFieldMap(aFieldMap);
	CleanupStack::Pop(); // aFieldMap (aRowSet has taken ownership)
	dbManager->GetiLogNames()->AddRowSet(aLog, aRowSet);
	dbManager->UseLog(aLog);
	dbManager->NextL();
	iOplAPI.Push(0.0);
	}

void COpxOplDb::DbStartTable()
	{
	delete iDbColSet;
	iDbColSet = NULL;
	iDbColSet=CDbColSet::NewL();
	iOplAPI.Push(0.0);
	}

void COpxOplDb::DbTableField()
	{
	TInt32 length = iOplAPI.PopInt32();
	TDbColType type = TDbColType(iOplAPI.PopInt32());
	TPtrC fieldName(iOplAPI.PopString());
	if (length < 0 || length > KOplMaxStringFieldLength)
		User::Leave(KOplErrSyntax);
	TDbCol aCol(fieldName, type);
	if (type == EDbColText || type == EDbColBinary)
		aCol.iMaxLength = length;
	else if (type <= EDbColDateTime)
		{
		if (1 < length)
			User::Leave(KOplErrSyntax);
		if (length == 1)
			aCol.iAttributes = TDbCol::ENotNull;
		}
	iDbColSet->AddL(aCol);
	iOplAPI.Push(0.0);
	}

void COpxOplDb::DbCreateTable()
	{
	TPtrC aTableName = iOplAPI.PopString();
	TPtrC aFileName = iOplAPI.PopString();
	CDbasePtr* dbase = CDbasePtr::NewLC(iOplAPI, aFileName, ETrue, ETrue);
	dbase->Dbase().CreateTable(aTableName, *iDbColSet);
	delete iDbColSet;
	iDbColSet = NULL;
	CleanupStack::PopAndDestroy(); // dbase
	iOplAPI.Push(0.0);
	}

void COpxOplDb::DbCount() const
	{
	iOplAPI.OpenCheckL();
	COplDbManager* dbManager = iOplAPI.DbManager();
	COplRowSet* aOplRs=dbManager->TheCurrentOplRs();
	if (aOplRs==NULL)
		User::Leave(KOplErrClosed);
	TInt32 aCount = aOplRs->AccessDbRowSet()->CountL(); 
	iOplAPI.Push(aCount);
	}

void COpxOplDb::DbSeek(TInt aProcNum) const
	{
	RDbTable::TComparison aComparison = RDbTable::TComparison(iOplAPI.PopInt32());
	TPtrC anIndexName = iOplAPI.PopString();
	TPtrC aTableName = iOplAPI.PopString();

	iOplAPI.OpenCheckL();
	COplDbManager* dbManager = iOplAPI.DbManager();
	COplRowSet* aOplRs=dbManager->TheCurrentOplRs();
	if (aOplRs==NULL)
		User::Leave(KOplErrClosed);

	RDbStoreDatabase &aDbase = aOplRs->ParentDbase()->StoreDbase();
	RDbTable aTable;
	User::LeaveIfError(aTable.Open(aDbase, aTableName)); //, RDbRowSet::EReadOnly));
	User::LeaveIfError(aTable.SetIndex(anIndexName));

	TDbSeekKey aKey;

	switch (aProcNum)
		{
	case EODbSeekInt:
		aKey.Add(TInt(iOplAPI.PopInt32()));
		break;
	case EODbSeekWord:
		aKey.Add(TUint(iOplAPI.PopInt32()));
		break;
	case EODbSeekReal:
		aKey.Add(TReal64(iOplAPI.PopReal64()));
		break;
	case EODbSeekText:
		aKey.Add(iOplAPI.PopString());
		break;
	case EODbSeekDateTime:
		aKey.Add(TTime(*((TDateTime*) iOplAPI.PopInt32())));
		break;
		}
 
	TBool aSeekResult = aTable.SeekL(aKey, aComparison);

	if (!aSeekResult)
		{
		// Not found
		aTable.Close();
		iOplAPI.Push(TInt32(0));
		return;
		}
	// Found
	TDbBookmark aBookMark = aTable.Bookmark();
	aTable.Close();
	aOplRs->AccessDbRowSet()->GotoL(aBookMark);
	iOplAPI.Push(TInt32(1));
	}

void COpxOplDb::DbGet(TInt aProcNum) const
	{
	TInt ind = iOplAPI.PopInt32();
	iOplAPI.OpenCheckL();
	COplDbManager* dbManager = iOplAPI.DbManager();
	COplRowSet* aOplRs=dbManager->TheCurrentOplRs();
	if (aOplRs==NULL)
		User::Leave(KOplErrClosed);
	if(!aOplRs->InAppendOrUpdate() && !aOplRs->InModifyOrInsert() && aOplRs->AccessDbRowSet()->AtRow())
		aOplRs->AccessDbRowSet()->GetL();
	RDbRowSet* aRowSet = aOplRs->AccessDbRowSet();
	switch (aProcNum)
		{
	case EODbGetLength:
		iOplAPI.Push(TInt32(aRowSet->ColLength(ind)));
		break;
	case EODbGetInt:
		iOplAPI.Push(aRowSet->ColInt32(ind));
		break;
	case EODbGetReal:
		iOplAPI.Push(aRowSet->ColReal64(ind));
		break;
	case EODbGetString:
		iOplAPI.PushL(aRowSet->ColDes(ind));
		break;
	case EODbGetReal32:
		iOplAPI.Push(TReal64(aRowSet->ColReal32(ind)));
		break;
	case EODbGetWord:
		iOplAPI.Push(TInt32(aRowSet->ColUint32(ind)));
		break;
	case EODbGetDateTime:
		{
		TDateTime* dtime = (TDateTime*) iOplAPI.PopInt32();
		*dtime = aRowSet->ColTime(ind).DateTime();
		iOplAPI.Push(0.0);
		}
		break;
	case EODbGetLong:
		{
		RDbColReadStream longStream;
		longStream.OpenLC(*aRowSet, ind);
		TInt32 length = iOplAPI.PopInt32();
		TUint8* buf = iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 2);
		while (length--)
			{
			longStream >> *buf++;
			}
		CleanupStack::PopAndDestroy();
		iOplAPI.Push(0.0);
		}
		break;
		}
	}

void COpxOplDb::DbPut(TInt aProcNum) const
	{
	TInt ind = iOplAPI.PopInt32();
	iOplAPI.OpenCheckL();
	COplDbManager* dbManager = iOplAPI.DbManager();
	COplRowSet* aOplRs=dbManager->TheCurrentOplRs();
	if (aOplRs==NULL)
		User::Leave(KOplErrClosed);
	RDbRowSet* aRowSet = aOplRs->AccessDbRowSet();
	switch (aProcNum)
		{
	case EODbPutEmpty:
		aRowSet->SetColNullL(ind);
		break;
	case EODbPutInt:
		aRowSet->SetColL(ind, iOplAPI.PopInt32());
		break;
	case EODbPutReal:
		aRowSet->SetColL(ind, iOplAPI.PopReal64());
		break;
	case EODbPutString:
		aRowSet->SetColL(ind, iOplAPI.PopString());
		break;
	case EODbPutReal32:
		aRowSet->SetColL(ind, TReal32(iOplAPI.PopReal64()));
		break;
	case EODbPutWord:
		aRowSet->SetColL(ind, TUint32(iOplAPI.PopInt32()));
		break;
	case EODbPutDateTime:
		{
		TDateTime* dtime = (TDateTime*) iOplAPI.PopInt32();
		aRowSet->SetColL(ind, TTime(*dtime));
		}
		break;
	case EODbPutLong:
		{
		RDbColWriteStream longStream;
		longStream.OpenLC(*aRowSet, ind);
		TInt32 length = iOplAPI.PopInt32();
		TUint8* buf = iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 2);
		while (length--)
			{
			longStream << *buf++;
			}
		longStream.CommitL();
		CleanupStack::PopAndDestroy(); // longstream
		}
		break;
		}
	iOplAPI.Push(0.0);
	}

const TInt KOplFindForward = 0x01;
const TInt KOplFindFromAnEnd = 0x02;
const TInt KOplFindCaseDependent = 0x10;
const TInt KOplFindFieldMask = KOplFindForward|KOplFindFromAnEnd|KOplFindCaseDependent;

TInt COpxOplDb::DbFindWhere() const
	{
	TInt flag=iOplAPI.PopInt16();
	TPtrC string=iOplAPI.PopString();
	// 0 -> not , 1 -> dependant
	TDbTextComparison caseDependancy(EDbCompareFolded);		 
	RDbRowSet::TDirection direction;
	
	iOplAPI.OpenCheckL();
	COplDbManager* dbManager = iOplAPI.DbManager();
	COplRowSet* aOplRs=dbManager->TheCurrentOplRs();
	if (aOplRs==NULL)
		User::Leave(KOplErrClosed);
	RDbRowSet* aRowSet = aOplRs->AccessDbRowSet();
	
	if (flag&KOplFindForward)
		{
		direction=RDbRowSet::EForwards;
		if (flag&KOplFindFromAnEnd)
			dbManager->FirstL();// use dbms , NO
		if (!aRowSet->AtRow())
			{
			aOplRs->SetPos(aRowSet->CountL()+1);
			return 0;
			}
		}
	else
		{
		direction=RDbRowSet::EBackwards;
		if ((flag&KOplFindFromAnEnd) || (!aRowSet->AtRow()))
			dbManager->LastL();	
		}
	if (flag&KOplFindCaseDependent)
		caseDependancy=EDbCompareNormal;
	
	TDbQuery dbQuery(string, caseDependancy);
	TInt iterations=aRowSet->FindL(direction, dbQuery);
	if (iterations==KErrNotFound)
		{
		if(direction==RDbRowSet::EForwards)
			aOplRs->SetPos(aRowSet->CountL()+1);
		else	
			dbManager->FirstL();
		return 0;
		}
	else
		{
		aOplRs->SetPosRelative(direction==RDbRowSet::EForwards ? iterations : -iterations);
		return aOplRs->GetPos();
		}
	}

void COpxOplDb::DbUse() const
	{
	TInt32 aLog = iOplAPI.PopInt32();
	iOplAPI.DbManager()->UseLog(aLog);
	iOplAPI.Push(0.0);
	}

//
// The members of COpxOplDb which are not language extension procedures
//
COpxOplDb::COpxOplDb(OplAPI& aOplAPI) : COpxBase(aOplAPI), iDbColSet(NULL)
	{
	//	__DECLARE_NAME(_S("COpxOplDb"));
	}

COpxOplDb* COpxOplDb::NewLC(OplAPI& aOplAPI)
	{
	COpxOplDb* This=new(ELeave) COpxOplDb(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	return This;
	}

void COpxOplDb::ConstructL()
	{
	// Do whatever is required to construct component members in COpxOplDb
	}

COpxOplDb::~COpxOplDb()
	{
	delete iDbColSet;
	// Required so that Tls is set to zero on unloading the OPX in UNLOADM
	Dll::FreeTls();
	}

//
// COpxBase implementation
//
void COpxOplDb::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case EODbGetTableCount:
		DbGetTableCount();
		break;
	case EODbGetTableName:
		DbGetTableName();
		break;
	case EODbGetIndexCount:
		DbGetIndexCount();
		break;
	case EODbGetIndexName:
		DbGetIndexName();
		break;
	case EODbGetIndexDescription:
		DbGetIndexDescription();
		break;
	case EODbGetFieldCount:
		DbGetFieldCount();
		break;
	case EODbGetFieldName:
	case EODbGetFieldType:
	case EODbGetFieldSize:
	case EODbGetCanBeEmpty:
		DbGetField(aProcNum);
		break;
	case EODbOpenR:
		DbOpen(EFalse);
		break;
	case EODbOpen:
		DbOpen(ETrue);
		break;
	case EODbStartTable:
		DbStartTable();
		break;
	case EODbTableField:
		DbTableField();
		break;
	case EODbCreateTable:
		DbCreateTable();
		break;
	case EODbGetLength:
	case EODbGetInt:
	case EODbGetReal:
	case EODbGetString:
	case EODbGetWord:
	case EODbGetReal32:
	case EODbGetDateTime:
	case EODbGetLong:
		DbGet(aProcNum);
		break;
	case EODbPutEmpty:
	case EODbPutInt:
	case EODbPutReal:
	case EODbPutString:
	case EODbPutWord:
	case EODbPutReal32:
	case EODbPutDateTime:
	case EODbPutLong:
		DbPut(aProcNum);
		break;
	case EODbFindWhere:
		iOplAPI.Push(TInt16(DbFindWhere()));
		break;
	case EODbFindSql:
		iOplAPI.Push(TInt32(DbFindWhere()));
		break;
	case EODbUse:
		DbUse();
		break;
	case EODbSeekInt:
	case EODbSeekWord:
	case EODbSeekText:
	case EODbSeekReal:
	case EODbSeekDateTime:
		DbSeek(aProcNum);
		break;
	case EODbCount:
		DbCount();
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxOplDb::CheckVersion(TInt aVersion)
	// To check whether the opx is a compatible version
	{
	if ((aVersion & 0xff00)>(KOpxDataVersion & 0xff00)) // Major version must be <= OPX's version
		return EFalse; // Bad version
	else
		return ETrue; // OK
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	COpxOplDb* tls=((COpxOplDb*)Dll::Tls());
	// tls is NULL on loading an OPX DLL (also after unloading it)
	if (tls==NULL)
		{
		tls=COpxOplDb::NewLC(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop(); // tls
		}
	return (COpxBase*)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxDataVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}