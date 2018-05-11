// LB_DBASE.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <s32file.h>
#include <opcodes.h>
#include <opldoc.h>
#include "dbpriv.h"
#include <opldb.h>
#include "oplutil.h"

#pragma message("lb_dbase.cpp: Need to up LBC for dbase use???")

void OpCode::Create(CStack& , COplRuntime& aRuntime, CFrame*)
/* At the IP: Logicalnumber, field(1) type, field (1) lbc for name, field(1)name, field(2),... -1
Types -1 end, 0 int, 1 long, 2 double, 3 string */
	{
 	aRuntime.DbManager()->OpenL(EDbCreate,ETrue);
	aRuntime.ClearTrap();
	}

void OpCode::Open(CStack& , COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenL(EDbOpen,ETrue);
	aRuntime.ClearTrap();
	}
void OpCode::OpenR(CStack& , COplRuntime& aRuntime, CFrame* )
	{
	aRuntime.DbManager()->OpenL(EDbOpen,EFalse);
	aRuntime.ClearTrap();
	}

void OpCode::FieldRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	COplRowSet* oplRs;
	TInt ordinal=aRuntime.DbManager()->GetRightSideRowSet(oplRs);
	if(oplRs->AccessDbRowSet()->AtRow())
		aStack.Push(oplRs->AccessDbRowSet()->ColInt16(ordinal));
	else
		aStack.Push(TInt16(0));
	}

void OpCode::FieldRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	COplRowSet* oplRs;
	TInt ordinal=aRuntime.DbManager()->GetRightSideRowSet(oplRs);
	if(oplRs->AccessDbRowSet()->AtRow())
		aStack.Push(oplRs->AccessDbRowSet()->ColInt32(ordinal));
	else
		aStack.Push(TInt32(0));
	}

void OpCode::FieldRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	COplRowSet* oplRs;
	TInt ordinal=aRuntime.DbManager()->GetRightSideRowSet(oplRs);
	if(oplRs->AccessDbRowSet()->AtRow())
		aStack.Push(oplRs->AccessDbRowSet()->ColReal64(ordinal));
	else
		aStack.Push(TReal64(0));
	}

void OpCode::FieldRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	COplRowSet* oplRs;
	TInt ordinal=aRuntime.DbManager()->GetRightSideRowSet(oplRs);
	if(oplRs->AccessDbRowSet()->AtRow())
		aStack.Push(oplRs->AccessDbRowSet()->ColDes(ordinal));
	else
		aStack.Push(KNullDesC);
	}

TInt COplDbManager::GetRightSideRowSet(COplRowSet*& aOplRs)
	{
	TInt log(iRuntime.IP8());
	aOplRs=(iLogicalNames->FindRowSet(log));
	if (aOplRs==NULL)
		User::Leave(KOplErrClosed);
	if(!aOplRs->InAppendOrUpdate() && !aOplRs->InModifyOrInsert() && aOplRs->AccessDbRowSet()->AtRow())
		aOplRs->AccessDbRowSet()->GetL();
	return(aOplRs->GetOrdinal(iStack.PopString()));
	}

EXPORT_C TInt COplRowSet::GetOrdinal(const TDesC& aOplFieldName)
	{
	return(iMap->ScanForOrdinal(aOplFieldName));
	}

void OpCode::FieldLeftSide(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->LeftSide(aRuntime.IP8());
	}

void FuncOpCode::Find(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->FindString(aStack.PopString());
	}

void FuncOpCode::FindField(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->FindField();
	}

void OpCode::Last(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->LastL();
	aRuntime.ClearTrap();
	}

void OpCode::First(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
 	aRuntime.DbManager()->FirstL();
	aRuntime.ClearTrap();
	}

void OpCode::Next(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->NextL();
	aRuntime.ClearTrap();
	}

void OpCode::Back(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->BackL();
	aRuntime.ClearTrap();
	}
	
void OpCode::Erase(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->Erase();
	aRuntime.ClearTrap();
	}

void OpCode::Close(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->Close();
	aRuntime.ClearTrap();
	}

void OpCode::Use(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->UseLog(aRuntime.IP8());
	aRuntime.ClearTrap();
	}

void FuncOpCode::Count(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	COplRowSet* r=aRuntime.DbManager()->TheCurrentOplRs(); 
	if(r->InModifyOrInsert() || r->InAppendOrUpdate())
		User::Leave(KOplErrIncompatibleUpdateMode);
	aStack.Push(TInt16(aRuntime.DbManager()->Count()));
	}

void OpCode::Position(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CancelIfUpdate();
	aRuntime.DbManager()->PositionL(TUint(aStack.PopInt16()));
	aRuntime.ClearTrap();
	} // moves

void FuncOpCode::Pos(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->PosL(); // findout
	}

void FuncOpCode::Eof(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aStack.Push(TInt16(aRuntime.DbManager()->EOF() ? -1 : 0));
	}

void OpCode::Update(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->UpdateL(); 
	aRuntime.ClearTrap();
	}
	
void OpCode::Append(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->AppendL();
	aRuntime.ClearTrap();
	}

void FuncOpCode::Space(CStack& aStack, COplRuntime& aRuntime, CFrame*)
/*
Returns LONG of bytes left on the device where the current logical name is from
  */
	{
	// just fix stack for now
	aRuntime.DbManager()->OpenCheckL();
	aStack.Push(aRuntime.DbManager()->Space());
	}

void FuncOpCode::Bookmark(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aStack.Push(TInt16(aRuntime.DbManager()->Bookmark()));
	aRuntime.ClearTrap();
	}

void OpCode::GotoMark(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->GotoMark(aStack.PopInt16());
	aRuntime.ClearTrap();
	}

void OpCode::KillMark(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->KillMark(aStack.PopInt16());
	aRuntime.ClearTrap();
	}

void OpCode::Rollback(CStack& , COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->Rollback();
	}

void OpCode::CommitTrans(CStack& , COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->CommitTrans();
	}

void OpCode::BeginTrans(CStack& , COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->BeginTrans();
	}

void FuncOpCode::InTrans(CStack& aStack,COplRuntime& aRuntime,CFrame *)
	{
	aRuntime.DbManager()->OpenCheckL();
	aStack.Push(TInt16(aRuntime.DbManager()->InTrans()));
	}

void OpCode::Compact(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->CompactL(aStack.PopString());
	}

void OpCode::Modify(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->ModifyL();
	aRuntime.ClearTrap();
	}

void OpCode::Insert(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->InsertL();
	aRuntime.ClearTrap();
	}

void OpCode::Put(CStack&, COplRuntime& aRuntime, CFrame*)
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->PutL();
	aRuntime.ClearTrap();
	}

void OpCode::Cancel(CStack&, COplRuntime& aRuntime, CFrame*)		
	{
	aRuntime.DbManager()->OpenCheckL();
	aRuntime.DbManager()->Cancel();
	}

void OpCode::DeleteTable(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
/*
In:	stack0  = table$
	stack1  = dbase$
*/
	{
	TPtrC table=aStack.PopString();
	TPtrC dbase=aStack.PopString();
	aRuntime.DbManager()->DeleteTable(table,dbase);
	aRuntime.ClearTrap();
	}

/**************************************
  CONSTRUCTORS / DESTRUCTORS
**************************************/

COplDbManager* COplDbManager::NewL(COplRuntime& aRuntime,CStack& aStack)
	{
	COplDbManager* self= new (ELeave) COplDbManager(aRuntime,aStack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

COplDbManager::COplDbManager(COplRuntime& aRuntime,CStack& aStack)
	:iRuntime(aRuntime), iStack(aStack),iBookmarkArray(KBookmarkGranularity),iFs(aRuntime.ConEnv()->FsSession())
	{
	}

void COplDbManager::ConstructL()
	{
	iLogicalNames=CLogicalNames::NewL();
	iOplDbCollection=COplDbCollection::NewL();
//	TOplBookmark p;
//	p.KillMark();
//	for(TInt ii=0;ii<KBookmarkGranularity;ii++)
//		iBookmarkArray.AppendL(p);
	}
	
COplDbManager::~COplDbManager()
	{
	if (iLogicalNames)
		{
		iLogicalNames->CancelAllUpdates();
		delete (iLogicalNames);
		}
	delete (iOplDbCollection);
	}

COplDbCollection* COplDbCollection::NewL()
	{
	COplDbCollection* self= new (ELeave) COplDbCollection;
	return self;
	}

COplDbCollection::COplDbCollection()
	:iOplDb(KMaxLogName)
	{
	}

COplDbCollection::~COplDbCollection()
	{
	for (TInt ii=0;ii<iOplDb.Count();ii++)
		{
		iOplDb[ii]->Close();
		delete iOplDb[ii];
		}
	}

COplDb* COplDb::NewLC(const TDesC& aName)
	{
	COplDb* self= new (ELeave) COplDb(aName);
	CleanupStack::PushL(self);
	return self;
	}

COplDb::COplDb(const TDesC& aName) 
	{
	iName=aName;
	iStore=NULL;
	iDataApp=EFalse;
	}

COplDb::~COplDb()
	{
	delete iStore;
	}

EXPORT_C CLogicalNames* CLogicalNames::NewL()
	{
	CLogicalNames* self= new (ELeave) CLogicalNames;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CLogicalNames::ConstructL()
	{
	COplRowSet* p=NULL;
	for (TInt ii=0;ii<KMaxLogName;ii++)
		iLogNames.AppendL(p);
	}

CLogicalNames::CLogicalNames()
	:iLogNames(KMaxLogName)
	{
	iCurrLogName=EDbNone;
	iCount=0;
	} 

EXPORT_C CLogicalNames::~CLogicalNames()
	{
	for(TInt ii=0;ii<KMaxLogName;ii++)
		{
		if(iLogNames[ii]&&iLogNames[ii]->InAppendOrUpdate())
			{
			iLogNames[ii]->AccessDbRowSet()->Cancel();
			iLogNames[ii]->AccessDbRowSet()->Close();
			iLogNames[ii]->AppendOrUpdate(EFalse);
			}
		}
	iLogNames.ResetAndDestroy();
	}

COplFieldMap::COplFieldMap(TInt aCount)
	:iFieldInfoArray(aCount)
	{
	}

EXPORT_C COplFieldMap* COplFieldMap::NewLC(TInt fCount)
	{
	COplFieldMap* self= new (ELeave) COplFieldMap(fCount);
	CleanupStack::PushL(self);
	return self;
	}

COplRowSet::COplRowSet(COplDbCollection& aDbCollect)
	:iDbCollect(aDbCollect)
	{
	}

void COplRowSet::ConstructL(RFs& aFs,const TDesC& aDatabase,COplFieldMap* aFieldMap,TOplDbOpenMode aMode, TBool allowUpdates)
	{
	iRowsWereAbsent=EFalse;
	TLex lex(aDatabase);
	TFileName dbName;
	TParse parser;

	lex.SkipSpaceAndMark();
	if (lex.Peek()=='"')
		{
		TInt posA=aDatabase.Locate('"');
		TInt posB=aDatabase.LocateReverse('"');
		lex=aDatabase.Mid(posB+1);
		dbName=aDatabase.Mid(posA+1,posB-posA-1);
		}
	else
		dbName=lex.NextToken();

	aFs.Parse(dbName,parser);

	if (aMode==EDbOpen)
		{
		iParentDbase=iDbCollect.CreateOrOpenDbaseL(parser.FullName(),EDbOpen,allowUpdates); 
		}
	else
		DoCreateL(lex,parser,*aFieldMap);
	}

EXPORT_C COplRowSet* COplRowSet::NewLC(COplDbCollection& aDbCollect, RFs& aFs,const TDesC& aDatabase,COplFieldMap* aFieldMap,TOplDbOpenMode aMode, TBool allowUpdates)
//
// Partial construction when opening, full construction on creation
//
	{
	COplRowSet* self= new (ELeave) COplRowSet(aDbCollect);
	CleanupStack::PushL(self);
	self->ConstructL(aFs,aDatabase,aFieldMap,aMode, allowUpdates);
	return self;
	}

EXPORT_C void COplRowSet::OpenRowSetL(RDbRowSet* aRowSet,TBool aIsATable,COplFieldMap* aFieldMap)
//
// Complete opening of a rowset
//
	{
	iDbRowSet=aRowSet;		// take ownership
	iAmATable=aIsATable;
	iDbColSet=iDbRowSet->ColSetL();
	if (iParentDbase->DataApp())
		ExpandFieldMapToCoverRichText(*aFieldMap);
	TInt oplFields=aFieldMap->GetNoOfFields(); 
	if (oplFields>(iDbColSet->Count()))			// check no extra opl handles
		User::Leave(KOplErrInvalidArgs);
	for (TInt ii=1;ii<=oplFields;ii++)			// check handles are of correct type
		{
		if ((aFieldMap->ReadType(ii-1))!=EDbColBit)
			{
			if ((aFieldMap->ReadType(ii-1))!=((*iDbColSet)[ii].iType))
			User::Leave(KOplErrInvalidArgs);
			}
		}
	}

EXPORT_C COplRowSet* COplRowSet::NewL(COplDbCollection& aDbCollect, RFs& aFs, const TDesC& aString, COplFieldMap* aFieldMap,TOplDbOpenMode aMode, TBool allowUpdates)
	{
	COplRowSet* self=NewLC(aDbCollect,aFs,aString,aFieldMap,aMode,allowUpdates);
	if (aMode==EDbOpen)
		self->DoOpenL(aString,*aFieldMap,allowUpdates);	// complete standard rowset opening
	CleanupStack::Pop();
	return self;
	}

EXPORT_C COplRowSet::~COplRowSet()
	{
	delete iDbColSet;
	delete iMap;
	if (iDbRowSet)
		{
		iDbRowSet->Close();
		delete iDbRowSet;
		}
	if (iParentDbase)
		{
		TRAPD(err,iDbCollect.CloseDbIfNotInUseL(iParentDbase,*TheRuntime()));
		}
	}

EXPORT_C void COplDbManager::OpenCheckL()
	{
	if (iLogicalNames->Count()==0)
		User::Leave(KOplErrClosed);
	}

EXPORT_C TInt COplFieldMap::ScanForOrdinal(const TDesC& aOplFieldName)
	{
	TInt count=iFieldInfoArray.Count();
	for (TInt kk=0;kk<count;kk++)
		{
		if (aOplFieldName==iFieldInfoArray[kk].OplName())
			return kk+1;
		}
	User::Leave(KOplErrNoFld);		
 	return -1; // WHY?
	}

EXPORT_C void COplDbManager::StringFieldValueL(TDes& aBuf)
	{
	TInt16 log(iStack.PopInt16());
	COplRowSet* oplRs=(iLogicalNames->FindRowSet(log));
	if (oplRs==NULL)
		User::Leave(KOplErrClosed);
	TPtrC fieldName(iStack.PopString());
	TInt ordinal=oplRs->GetOrdinal(fieldName);
	iStack.Push(fieldName);
	iStack.Push(log);
	if(!oplRs->InAppendOrUpdate())
		oplRs->AccessDbRowSet()->GetL();
	aBuf=(oplRs->AccessDbRowSet()->ColDes(ordinal));
	}

LOCAL_C COplFieldMap* buildFieldMapLC(COplRuntime& aRuntime)
	{
	TUint8* pMap=aRuntime.IP();
	TInt fCount=0;
	while (aRuntime.IP8()!=0x0ff)
		{
		TInt lbc=aRuntime.IP16(); //includes lbc2
		__ASSERT_DEBUG(lbc<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
		aRuntime.IncIP(lbc*sizeof(TText));
		++fCount;
		}
	COplFieldMap* fm=COplFieldMap::NewLC(fCount);

	TInt type;
	while ((type=*pMap++)!=0x0ff)				
		{
		TBuf<KOplMaxFieldName> fieldname;
		TInt16 len=OplUtil::GetWord(pMap);
		__ASSERT_DEBUG(len<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
		pMap+=2;
		for (TInt ii=len;ii;--ii)
			{
			TChar cc=*pMap++;
			cc=cc|*pMap++<<8;
			fieldname.Append(cc);
			}
		fm->AppendFieldL(fieldname,type);
		}
	return fm;
	} 

EXPORT_C void COplFieldMap::AppendFieldL(TDesC &aName, TInt aType)
	{
	TOplFieldInfo info(aName, aType);
	iFieldInfoArray.AppendL(info);
	}

EXPORT_C TDbColType COplFieldMap::ReadType(TInt aMapIndex)
/*
Reads type from Opl map and returns Dbms equivelant 				
*/ 
	{
	if (aMapIndex>=iFieldInfoArray.Count())
		return EDbColInt16;
	// K.K	static const TDbColType types[] = {EDbColInt16,EDbColInt32,EDbColReal64,EDbColText8,EDbColBit};
			static const TDbColType types[] = {EDbColInt16,EDbColInt32,EDbColReal64,EDbColText,EDbColBit};
	return types[iFieldInfoArray[aMapIndex].Type()];
	}

EXPORT_C void COplFieldMap::ReadName(TInt aMapIndex, TDes& aColName)
/*
Reads name from Opl map and generates acceptable dbms name
*/ 
	{
	TLex name=(iFieldInfoArray[aMapIndex].OplName());
	TChar cc;
	while (!name.Eos())										
		{
		cc=name.Get();										
		switch(cc)
			{
		case '%':
			cc='i';
			break;
		case '&':
			cc='l';
			break;
		case '$':
			cc='s';
			break;				
		default:
			break;
			} 
		aColName.Append(cc);									
		}
	}

LOCAL_C TPtrC nextCol(TLex& aColSpec,TInt& aStringLength,TDbColType& aType)
/*
 PARSE COMMAND STRING TO BUILD DBMS COL. SET
*/
	{
	TChar c;
	aColSpec.SkipSpaceAndMark();							
	while (!aColSpec.Eos() && ((c=aColSpec.Peek())!=','))	
		{
		if (c==EKeySpace || c==EKeyTab || c=='(')									
			break;
		aColSpec.Inc();
		}

	TPtrC ret= aColSpec.MarkedToken();
	aStringLength=KOplMaxStringFieldLength;
	aColSpec.SkipSpace();
	if ((c=aColSpec.Peek())=='(')
		{
		if (aType!=EDbColText)
			User::Leave(KOplErrSyntax);						
		aColSpec.Get();
		aColSpec.SkipSpaceAndMark();							
		while (!aColSpec.Eos() && ((c=aColSpec.Peek())!=')'))	
			{
			if (c==EKeySpace || c==EKeyTab )									
			break;
			aColSpec.Inc();	
			}

		TLex lex(aColSpec.MarkedToken());

		aColSpec.SkipSpace();
		if (aColSpec.Get()!=')')
			User::Leave(KOplErrSyntax);
		if (lex.Val(aStringLength)<0)
			User::Leave(KOplErrSyntax);
		if (aStringLength>KOplMaxStringFieldLength)
			User::Leave(KOplErrSyntax);
		}
	return ret;
	}

LOCAL_C void parseColSpecL(TLex& aColSpec, CDbColSet& aDbColSet, COplFieldMap& aFieldMap)
// Parse column specification into aDbColSet
	{
	aDbColSet.Clear();
	TInt i(0);
	TInt stringLength;
	FOREVER
		{
		TDbColType type=aFieldMap.ReadType(i);
		TPtrC ptrCol = nextCol(aColSpec,stringLength,type);
		if (!ptrCol.Length())										
			break;
		TDbCol aCol(ptrCol,type,stringLength); 
		aDbColSet.AddL(aCol); 											
		aColSpec.SkipSpace();											
		i++;															
		if (aColSpec.Peek()!=',')										
			break;
		aColSpec.Inc();
		}
	if (!i)
		User::Leave(KOplErrSyntax);
	if (aDbColSet.Count()!=aFieldMap.GetNoOfFields())
		User::Leave(KOplErrSyntax);
	}

_LIT(KTo,"TO");
_LIT(KSelectSpace,"SELECT ");
_LIT(KComma,",");
_LIT(KFrom," FROM ");
_LIT(KFields,"FIELDS");
_LIT(KTable1,"Table1");
_LIT(KFolded,"FOLDED");
_LIT(KCollated,"COLLATED");
_LIT(KSelect,"SELECT");
_LIT(KOr," OR ");
_LIT(KLikeQuote," LIKE \'");
_LIT(KQuote,"\'");

LOCAL_C TPtrC parseTableName(TLex& aColSpecRem)
// Parse the table name
	{
	TPtrC keyword2;
	keyword2.Set(aColSpecRem.NextToken());
	if (keyword2.CompareF(KTo))
		User::Leave(KOplErrSyntax);
	return(aColSpecRem.NextToken());
	}

LOCAL_C void makeQuery(CDbColSet& aDbColSet,TDesC& aTableName,TDes& aQuery)
// make/generate query
	{
 	TInt count=aDbColSet.Count();
	aQuery.Append(KSelectSpace);
	for(TInt i=1;i<count;i++)
		{
		aQuery.Append(aDbColSet[i].iName);
		aQuery.Append(KComma);
		}
	aQuery.Append(aDbColSet[count].iName);
	aQuery.Append(KFrom);
	aQuery.Append(aTableName);
	}

LOCAL_C void generateColSpecL(CDbColSet& aDbColSet, COplFieldMap& aFieldMap)
//GENERATE COL. SPEC TO MAKE DBMS COL. SET
	{
	TInt c=aFieldMap.GetNoOfFields();
	aDbColSet.Clear();									
	for(TInt i=0;i<c;i++)
		{
		TBuf<KDbMaxColName> aColName;
		aFieldMap.ReadName(i,aColName);
		TDbCol aCol(aColName,aFieldMap.ReadType(i),KOplMaxStringFieldLength);
		aDbColSet.AddL(aCol); 							
		}
	}

void COplRowSet::DoCreateL(TLex& aLex,TParse& aParse,COplFieldMap& aFieldMap)
	{
 	iParentDbase=iDbCollect.CreateOrOpenDbaseL(aParse.FullName(),EDbCreate,ETrue); 
	iDbColSet=CDbColSet::NewL();
	TPtrC keyword1;
	keyword1.Set(aLex.NextToken());
	if (keyword1.Length()!=0)
		{
		if (keyword1.CompareF(KFields))
			User::Leave(KOplErrSyntax);
		parseColSpecL(aLex,*iDbColSet,aFieldMap);
		TBufC<KOplMaxTableName> tableName=parseTableName(aLex);
		User::LeaveIfError(iParentDbase->CreateTable(tableName,*iDbColSet));
		TBuf<256> aQuery;
		makeQuery(*iDbColSet,tableName,aQuery);
 		iDbRowSet = new (ELeave) RDbView;
		User::LeaveIfError(((RDbView*)iDbRowSet)->Prepare((RDbStoreDatabase&)iParentDbase->Dbase(),aQuery));			
		User::LeaveIfError(((RDbView*)iDbRowSet)->EvaluateAll());			
		iAmATable=EFalse;
		}
 	else
		{
		generateColSpecL(*iDbColSet,aFieldMap);
		TBufC<KOplMaxTableName> tableName(KTable1);
		User::LeaveIfError(iParentDbase->CreateTable(tableName,*iDbColSet)); 
		iDbRowSet = new (ELeave) RDbTable;
		User::LeaveIfError(((RDbTable*)iDbRowSet)->Open((RDbStoreDatabase&)iParentDbase->Dbase(),tableName));		
		iAmATable=ETrue;
		}
	}

void COplRowSet::DoOpenL(const TDesC& aString,COplFieldMap& aFieldMap, TBool allowUpdates)
	{
	TLex lex(aString);

	lex.SkipSpaceAndMark();
	if (lex.Peek()=='"')
		{
		TInt posB=aString.LocateReverse('"');
		lex=aString.Mid(posB+1);
		}
	else
		lex.NextToken();

	TPtrC query(lex.Remainder());
	TPtrC keyword1;
	RDbRowSet::TAccess access=(allowUpdates)?RDbRowSet::EUpdatable:RDbRowSet::EReadOnly;
	TBool isTable;
	keyword1.Set(lex.NextToken()); // FIND OUT ABOUT TOKEN LENGTH
	if (keyword1.Length()!=0)
		{
		TDbTextComparison comp=EDbCompareNormal;
		if (keyword1.CompareF(KFolded)==0)
			{
			comp=EDbCompareFolded;
			query.Set(lex.Remainder());
			keyword1.Set(lex.NextToken()); 
			}
		else if (keyword1.CompareF(KCollated)==0)
			{
			comp=EDbCompareCollated;
			query.Set(lex.Remainder());
			keyword1.Set(lex.NextToken()); 
			}
		if (keyword1.CompareF(KSelect))
			User::Leave(KOplErrSyntax);
		RDbView* view = new (ELeave) RDbView;
		iDbRowSet = view;
		TDbQuery dbQuery(query,comp);
		User::LeaveIfError(view->Prepare((RDbStoreDatabase&)iParentDbase->Dbase(),dbQuery, access));
		User::LeaveIfError(view->EvaluateAll());
		isTable=EFalse;
		}
 	else
		{
		RDbTable* table = new (ELeave) RDbTable;
		iDbRowSet=table;
		User::LeaveIfError(table->Open((RDbStoreDatabase&)iParentDbase->Dbase(),KTable1, access));	// Allow updates default - ETRUE
		isTable=ETrue;
		}
	OpenRowSetL(iDbRowSet,isTable,&aFieldMap);
	}
 
EXPORT_C void COplRowSet::ExpandFieldMapToCoverRichText(COplFieldMap& aFieldMap)
	{
	for (TInt ii=1;ii<=iDbColSet->Count();ii++)			// check handles are of correct type
		{
		TDbColType type=((*iDbColSet)[ii].iType);
		if (type!=EDbColInt16 && type!=EDbColInt32 && type!=EDbColReal64 && type!=EDbColText && ii<=(aFieldMap.GetNoOfFields()+1))
			aFieldMap.InsertBlankL(ii-1,(*iDbColSet)[ii].iName);
		}
	}

EXPORT_C void COplFieldMap::InsertBlankL(TInt anOrdinal,const TDesC &aName)					
	{ //for data app files rich text + other type problems
	TOplFieldInfo ins(aName,4);
	ins.SetType(4);
	iFieldInfoArray.InsertL(anOrdinal,ins);
	}

void COplDbManager::OpenL(TOplDbOpenMode aMode,TBool allowUpdates)
// handle Create, Open, OpenR keyword 
	{
	TInt logName=iRuntime.IP8();
	COplFieldMap* fieldMap=buildFieldMapLC(iRuntime);
	if (iLogicalNames->FindRowSet(logName)!=NULL)
		User::Leave(KOplErrOpen);
	TPtrC stringPtr(iStack.PopString());
	COplRowSet* oplRowSet=COplRowSet::NewL(*iOplDbCollection,iFs,stringPtr,fieldMap,aMode,allowUpdates);
	oplRowSet->SetOplFieldMap(fieldMap);
	CleanupStack::Pop(); // fieldMap
	iLogicalNames->AddRowSet(logName,oplRowSet); // append dbase to collection
	iLogicalNames->SetCurrLogName(logName);
	iCurrOplRowSet=oplRowSet; // cached copy
	iCurrDbRowSet=oplRowSet->AccessDbRowSet(); // cached copy
	NextL();
	iRuntime.ClearTrap();
	}

EXPORT_C void COplDbManager::UseLog(TInt aLogicalName)
	{
	COplRowSet* rs;
	if ((rs=iLogicalNames->FindRowSet(aLogicalName))==NULL)
		User::Leave(KOplErrClosed);
	iLogicalNames->SetCurrLogName(aLogicalName);
	iCurrOplRowSet=rs;
	iCurrDbRowSet=rs->AccessDbRowSet();
	}

EXPORT_C void COplDbManager::SetiLogNames(CLogicalNames* aLogicalNames)
	{
	iLogicalNames=aLogicalNames;
	TOplCurrRowSet curr=iLogicalNames->GetCurrLogName();
	if (curr!=EDbNone)
		{
		iCurrOplRowSet=iLogicalNames->FindRowSet(curr);
		iCurrDbRowSet=iCurrOplRowSet->AccessDbRowSet();
		}
	}

EXPORT_C void COplDbManager::Erase()
	{
	if (iCurrDbRowSet->AtRow())
		{
		SafetyDeleteL();
		iCurrDbRowSet->NextL();
		}
	else
		User::Leave(KOplErrEof);
	}

EXPORT_C void COplDbManager::LeftSide(TInt log)
// Leaves with KOplErrClosed if logical file not open
// To be stacked:
//  logical file name (byte at IP)
//  ELeftSideField as integer
//  
	{
	COplRowSet* rs;
	if ((rs=iLogicalNames->FindRowSet(log))==NULL)
		User::Leave(KOplErrClosed);			
	iStack.Push(TInt16(log));
	iStack.Push(TInt16(ELeftSideField));
	}

EXPORT_C void COplRowSet::InsertBlankL()
	{ // insert zero filled field as last record
	iDbRowSet->InsertL();
	iDbRowSet->PutL();
	SetPos(iDbRowSet->CountL());
	}

EXPORT_C void COplRowSet::CopyCurrRowToEnd()
	{
	// current row remembered and copied to end
	SetPosBeforeInsert(iDbRowSet->Bookmark());
	iDbRowSet->InsertCopyL();
	}

EXPORT_C void COplRowSet::PrepareForUpdate()
	{
	// Gets row set being aasigned to, copies (if necessary) 
	// the current row to the end of 
	// rowSet, locks this row ready for updating
	if(!InAppendOrUpdate())
		{
		if(!iDbRowSet->CountL()) 
			iRowsWereAbsent=ETrue;
		if(iDbRowSet->AtRow()) 
			CopyCurrRowToEnd();
		else
			iDbRowSet->InsertL();
		AppendOrUpdate(ETrue);
		}
	}

RDbRowSet& COplDbManager::PreAssign(TDbColNo& aField)	
// T.O.Stack = TInt16 logicalFileName (0) 
//  next = OplString fieldName 
//  NOT NECESSARILY CURRENT ROWset
	{
	COplRowSet* oplRs=iLogicalNames->FindRowSet(iStack.PopInt16());
	aField=oplRs->GetOrdinal(iStack.PopString());
	if(!oplRs->InModifyOrInsert())
		{
		oplRs->PrepareForUpdate();
		return *oplRs->AccessDbRowSet();
		}
	else
		return *iCurrDbRowSet;
	}

void COplDbManager::Assign(TInt16 aVal)
	{
	TDbColNo field;
	RDbRowSet& row=PreAssign(field);
	row.SetColL(field,aVal);
	}

void COplDbManager::Assign(TInt32 aVal)
	{
	TDbColNo field;
	RDbRowSet& row=PreAssign(field);
	row.SetColL(field,aVal);
	}

void COplDbManager::Assign(TReal64 aReal)
	{
	TDbColNo field;
	RDbRowSet& row=PreAssign(field);
	row.SetColL(field,aReal);
	}

void COplDbManager::Assign(TDesC& aString)
	{
	TDbColNo field;
	RDbRowSet& row=PreAssign(field);
	row.SetColL(field,aString);
	}
				
EXPORT_C void COplDbManager::CancelIfUpdate()
	{ 
	if (iLogicalNames->Count())
		{
		if (iCurrOplRowSet->InAppendOrUpdate())
			{
			iCurrDbRowSet->Cancel();
			iCurrOplRowSet->AppendOrUpdate(EFalse);
			iCurrOplRowSet->ResetRowsAbsent();
			}
		if (iCurrOplRowSet->InModifyOrInsert())
			{
			iCurrDbRowSet->Cancel();
			iCurrOplRowSet->ModifyOrInsert(EFalse);
			}
		}
	}

EXPORT_C void CLogicalNames::CancelAllUpdates()
	{
	if (iCount!=0)		
		{
		for(TInt ii=0;ii<iCount;ii++)
			{
			while(iLogNames[ii]==NULL)
				ii++;
			iLogNames[ii]->AccessDbRowSet()->Cancel();
			iLogNames[ii]->AppendOrUpdate(EFalse);
			}
		}
	}

EXPORT_C void COplDbManager::ModifyL()
	{
	if(!iCurrDbRowSet->AtRow())
		User::Leave(KOplErrEof);
	if ((!iCurrOplRowSet->InAppendOrUpdate()) && (!iCurrOplRowSet->InModifyOrInsert()))
		{
		iCurrOplRowSet->ModifyOrInsert(ETrue); // set mode if in neither
		iCurrDbRowSet->UpdateL();
		}
	else
		User::Leave(KOplErrIncompatibleUpdateMode);				// error if in any mode already
	}

EXPORT_C void COplDbManager::InsertL()
	{
	if ((!iCurrOplRowSet->InAppendOrUpdate()) && (!iCurrOplRowSet->InModifyOrInsert()))
		{
		iCurrDbRowSet->InsertL();
		iCurrOplRowSet->ModifyOrInsert(ETrue); // set mode if in neither
		iCurrOplRowSet->InsertMode(ETrue);
		}
	else
		User::Leave(KOplErrIncompatibleUpdateMode);				// error if in any mode already

	}

EXPORT_C void COplDbManager::PutL()
	{
	if(!iCurrOplRowSet->InModifyOrInsert() || iCurrOplRowSet->InAppendOrUpdate())
		User::Leave(KOplErrIncompatibleUpdateMode);// cant append if in wrong mode
	SafelyPutL();
	iCurrOplRowSet->ModifyOrInsert(EFalse);
	if(iCurrOplRowSet->InInsertMode())
		{
		iCurrOplRowSet->SetPos(iCurrDbRowSet->CountL());
		iCurrOplRowSet->InsertMode(EFalse);
		}
	}

EXPORT_C void COplDbManager::SafelyPutL()
	{
	TRAPD(err,iCurrDbRowSet->PutL());		
	if (err)
		{
		Cancel();
		User::Leave(err);
		}
	}

EXPORT_C void COplDbManager::SafetyDeleteL()
	{
	TRAPD(err,iCurrDbRowSet->DeleteL());		
	if (err)
		{
		HandleAutoTransactionRollback();
		User::Leave(err);
		}
	}

EXPORT_C void COplDbManager::Cancel()
	{
	iCurrDbRowSet->Cancel();
	HandleAutoTransactionRollback();
	iCurrOplRowSet->AppendOrUpdate(EFalse);
	iCurrOplRowSet->ModifyOrInsert(EFalse);
	iCurrOplRowSet->InsertMode(EFalse);
	}

EXPORT_C void COplDbManager::HandleAutoTransactionRollback()
	{
	if(iCurrOplRowSet->ParentDbase()->StoreDbase().InTransaction())
		return;
	TRAPD(err,iCurrDbRowSet->CountL(RDbRowSet::EQuick));
	if (err==KErrNotReady)
		ResetRelatedViews();
	}

EXPORT_C void COplDbManager::ResetRelatedViews()
	{
	TPtrC parentName=iCurrOplRowSet->ParentDbase()->Name();
	for(TInt ii=0;ii<KMaxLogName;ii++)
		{
		COplRowSet* temp=(*(iLogicalNames->RowSetArray()))[ii];
		if (temp!=NULL)
			{
			if(temp->ParentDbase()->Name()==parentName) 
				{
				RDbRowSet* tempRowset=temp->AccessDbRowSet();
				TDbBookmark mark=tempRowset->Bookmark();
				tempRowset->Reset();
				TRAPD(err,tempRowset->GotoL(mark));
				if (err)
					temp->SetPos(1);
				}
			}
		}
	}

EXPORT_C void COplDbManager::AppendL()
	{
	if (iCurrOplRowSet->InModifyOrInsert())
		User::Leave(KOplErrIncompatibleUpdateMode); // Can't append if in wrong mode
	else
		{
		if (iCurrOplRowSet->InAppendOrUpdate())
			{
			iCurrOplRowSet->AppendOrUpdate(EFalse); // set false changes be altered
			iCurrOplRowSet->ResetRowsAbsent();
			SafelyPutL();
			iCurrOplRowSet->SetPos(iCurrDbRowSet->CountL());
			}
		else if(!EOF()) //eof insert blank at end 
			{ 
			iCurrOplRowSet->CopyCurrRowToEnd();  // Come straight but allowable
			SafelyPutL();
			iCurrOplRowSet->SetPos(iCurrDbRowSet->CountL());
			}
		else
			iCurrOplRowSet->InsertBlankL();
		}

	}

EXPORT_C void COplDbManager::UpdateL()
	{
	if (iCurrOplRowSet->InModifyOrInsert())
		User::Leave(KOplErrIncompatibleUpdateMode);				// cant update if in wrong mode
	if (iCurrOplRowSet->InAppendOrUpdate())	// if something inserted at EOF
		{
		iCurrOplRowSet->AppendOrUpdate(EFalse); 
		if(!iCurrOplRowSet->RowsAbsent()) // if there were rows before app/up
			{
			SafelyPutL();
			iCurrDbRowSet->GotoL(iCurrOplRowSet->GetPosBeforeInsert());
			SafetyDeleteL();			//remove updated
			LastL();
			}
		else									
			{									// have been updating an empty row
			iCurrOplRowSet->ResetRowsAbsent();
			iCurrDbRowSet->Cancel();
			User::Leave(KOplErrEof);
			}
		}
	else
		{ // calling updating with no insert
		if(!EOF()) // empty -> error
			{										// copy current to end and current
			iCurrOplRowSet->CopyCurrRowToEnd(); 
			SafelyPutL();
			iCurrDbRowSet->GotoL(iCurrOplRowSet->GetPosBeforeInsert());
			SafetyDeleteL();
			LastL();
			}
		else
			User::Leave(KOplErrEof);
		}
	}

EXPORT_C TBool COplDbManager::EOF()
	{
	return iCurrDbRowSet->AtEnd();
	}	
 
EXPORT_C TBool COplDbManager::SOF(RDbRowSet* aRs)
	{
	return aRs->AtBeginning();
	}	

EXPORT_C TBool COplDbManager::SOF()
	{
	return iCurrDbRowSet->AtBeginning();
	}	

EXPORT_C void COplDbManager::BackL()
// Back 1 record but Stays if at row 1 already 	
	{
	if (!iCurrDbRowSet->AtBeginning())	
		{
		iCurrDbRowSet->PreviousL();
		iCurrOplRowSet->SetPosRelative(-1);
		}
	if (iCurrDbRowSet->AtBeginning())
		{
		iCurrDbRowSet->NextL();
		iCurrOplRowSet->SetPos(1);
		}
	}

EXPORT_C void COplDbManager::FirstL()	
	{
	iCurrDbRowSet->FirstL();
	iCurrOplRowSet->SetPos(1);
	}

EXPORT_C void COplDbManager::NextL()
	{
	if(!iCurrDbRowSet->AtEnd())
		{
		iCurrDbRowSet->NextL();
		iCurrOplRowSet->SetPosRelative(+1);
		}
	}

EXPORT_C void COplDbManager::LastL()
	{
	iCurrDbRowSet->LastL();
	iCurrOplRowSet->SetPos(iCurrDbRowSet->CountL());
	if(iCurrDbRowSet->AtBeginning())
		{
		iCurrDbRowSet->NextL();
		iCurrOplRowSet->SetPos(1);
		}

	}

EXPORT_C void COplDbManager::PositionL(TUint aPos)
	{ 
	PosL();
	TInt currPos(Stack().PopInt16());	
	TUint count=TUint(iCurrDbRowSet->CountL());
	if (aPos==0)
		aPos=1;
	if (count+1<aPos)
		aPos=count+1;
	TInt steps=aPos-currPos;
	TInt loopTo(0);
	TBool direction(ETrue);
	if (steps>0) // going forward
		{
		TInt stepsToHalfWayToEnd=(count+1-currPos)/2; // test if destination is closer to EOF
		if (stepsToHalfWayToEnd>steps)
			{
			loopTo=steps;
			direction=ETrue;
			}
		else // go to end and move back
			{
			iCurrDbRowSet->LastL();
			iCurrDbRowSet->NextL(); //EOF
			loopTo=(count+1-currPos-steps);
			direction=EFalse;
			}
		}
	if (steps<0)
		{
		TInt StepsToHalfWayToStart=(currPos)/2;
		if (StepsToHalfWayToStart>(-steps))	// move backwards
			{
			loopTo=-steps;
			direction=EFalse;
			}
		else // go to start and move forwards
			{
			iCurrDbRowSet->FirstL();
			loopTo=aPos-1;
			direction=ETrue;
			}
		}
	for(TInt pp=0;pp<loopTo;pp++)
		{
		if (direction)
			iCurrDbRowSet->NextL(); 
		else
			iCurrDbRowSet->PreviousL(); 
		}
	iCurrOplRowSet->SetPos(aPos);
	}	

EXPORT_C void COplDbManager::PosL()
	{
	iStack.Push(TInt16(iCurrOplRowSet->GetPos()));
	}

EXPORT_C void COplDbManager::Close()		
// close rowSet
	{
	iCurrDbRowSet->Close();
	COplRowSet* newRs=iLogicalNames->CloseOplRowSet();
	iCurrOplRowSet=newRs;
	if (newRs)
		iCurrDbRowSet=newRs->AccessDbRowSet();		
	else
		iCurrDbRowSet = NULL; // cached copy
	}

EXPORT_C COplRowSet* CLogicalNames::CloseOplRowSet()
	{
	delete iLogNames[iCurrLogName];
	COplRowSet* newRs=NULL; // cached copy							
	iLogNames[iCurrLogName]=newRs;					
	iCurrLogName=EDbNone;							
	TInt ii=0;
	iCount--;
	if (iCount!=0)		
		{
		while(iLogNames[ii]==NULL)
			ii++;
		newRs=iLogNames[ii];
		iCurrLogName=TOplCurrRowSet(ii);	
		}
	return newRs;						
	}

EXPORT_C void COplDbCollection::CloseDbIfNotInUseL(COplDb* aParent,COplRuntime& aRuntime)
// decrease access count,if zero close dbase and remove from collection 
	{
	if (--aParent->AccessCount()==0) 
		{
		aParent->Close();			
		TInt count=iOplDb.Count();
		TFileName file=aParent->Name();
		delete aParent; // CLose file allowing auto compaction
		for (TInt ii = 0; ii<count; ii++)
			{
			if (iOplDb[ii]==aParent)
				{								
				iOplDb.Delete(ii);
				break;
				}
			}
		if (aRuntime.UserFlags() & KOplStateAutoCompact)
			aRuntime.DbManager()->CompactL(file);
		}	
	}

EXPORT_C void CLogicalNames::AddRowSet(TInt aLogName, COplRowSet* anOplRowSet)
	{
	iLogNames[aLogName]=anOplRowSet;
	iCount+=1;
	}

EXPORT_C COplDb* COplDbCollection::FindOpenDbase(const TDesC &aName,const TBool aIncAccessCount)
	{
	TInt count;
	TInt ii;
	count = iOplDb.Count();
	TFileName upper(aName);
	upper.UpperCase();
	for (ii = 0; ii<count; ii++)
		{
		if (((iOplDb)[ii])->Name()==upper) 
			{
			if(aIncAccessCount)
			(iOplDb[ii]->AccessCount()++);
			return (iOplDb)[ii]; 
			}
		} 
	return(NULL);
	}

EXPORT_C COplDb* COplDbCollection::CreateOrOpenDbaseL(const TDesC &aName,const TOplDbOpenMode aMode,const TBool aUpdate)
	{
	COplDb *db;
	RFs &fs=TheRuntime()->ConEnv()->FsSession();
	if ((db=FindOpenDbase(aName,ETrue))==NULL)				
		{
		TFileName upper(aName);
		upper.UpperCase();
		db=COplDb::NewLC(upper);
		iOplDb.AppendL(db);
		TRAPD(err,db->OpenL(fs,aName,aUpdate));		
		if((aMode==EDbCreate) && err==KErrNotFound)
			{
			TRAP(err,db->CreateL(fs,aName));
			if (err)
				fs.Delete(aName);
			}
		if (err)	
			{
			iOplDb.Delete(iOplDb.Count()-1);
			User::Leave(err);		
			}

		db->AccessCount()=1;
		CleanupStack::Pop();	// db
		}

	return db;
	}
 							
void COplDb::CreateL(RFs& aFs,const TDesC& aDbaseName)
	{
	iStore=CPermanentFileStore::CreateL(aFs,aDbaseName,EFileRead|EFileWrite);
	COplRuntime* rt=TheRuntime();
	// Make it an apparch document only if name matches the SETDOC name$ setting
	TUidType uids(KPermanentFileStoreLayoutUid,KUidExternalOplFile); //default to no doc
	if (rt->CurrentDocumentName().CompareF(aDbaseName)==0)
		uids=TUidType(KPermanentFileStoreLayoutUid,KUidOplDoc,rt->CommandLine().AppUid());
	iStore->SetTypeL(uids);
	// Write the root stream
	TOplDocRootStream shellData;
	shellData.iStreamId=iDbase.CreateL(iStore);
	shellData.iAppUid=KUidOplInterpreter;

	RStoreWriteStream root;
	TStreamId rootId=root.CreateLC(*iStore);

	root<<shellData;
	root.CommitL(); // root stream
	CleanupStack::PopAndDestroy();	// root

	iStore->SetRootL(rootId);
	iStore->CommitL();
	}

void COplDb::OpenL(RFs& aFs,const TDesC& aDbaseName,const TBool aUpdate)
 	{
	TUidType type(KPermanentFileStoreLayoutUid,KUidAppDllDoc,KUidDataApp);
	TEntry entry;
	aFs.Entry(aDbaseName,entry);
	if (entry.iType.MostDerived()==type.MostDerived())
		//data model
		{
		iStore=CFileStore::OpenL(aFs,aDbaseName,EFileRead);	// READ ONLY FOR 	
//		iDaModel=CDaModel::NewL(*iStore);
		iDataApp=ETrue;
//		iDaModel->OpenDatabaseL(iDbase,*iStore);
		CStreamDictionary* streamDic = CStreamDictionary::NewLC();
		RStoreReadStream inStream;
		inStream.OpenLC(*iStore,iStore->Root()); 
		inStream >> *streamDic;
		CleanupStack::PopAndDestroy(); // stream
		TStreamId dataHeadStreamId;
		dataHeadStreamId = streamDic->At(KUidDataHeadStream);
		CleanupStack::PopAndDestroy(); // streamDic
		inStream.OpenLC(*iStore, dataHeadStreamId); 
		TStreamId dbStreamId;
		inStream >> dbStreamId;
		CleanupStack::PopAndDestroy(); // stream
		iDbase.OpenL(iStore,dbStreamId); 
		}
	else
		{
		iStore=CFileStore::OpenL(aFs,aDbaseName,aUpdate?EFileRead|EFileWrite:EFileRead);	// file closed on error	
		RStoreReadStream inStream;
		inStream.OpenLC(*iStore,iStore->Root());
		TOplDocRootStream shellData;
		inStream>>shellData;
		CleanupStack::PopAndDestroy();	// inStream
		iDbase.OpenL(iStore,shellData.iStreamId);
		iDataApp=EFalse;
		}
	}

EXPORT_C void COplDb::Close()
	{
	if (iStore)
		iDbase.Close();
	}

EXPORT_C TInt COplDb::CreateTable(const TDesC& aTableName,const CDbColSet& aColSet)
	{
	return (iDbase.CreateTable(aTableName,aColSet));
	}

EXPORT_C void COplDbManager::GenerateSQL(TInt aFirstString, TInt aNoOfStrings,const TDesC& aString,TDes& aSQL)
// Generate where clause
	{
	aSQL.Zero();
	TInt ff(0);
	TInt numberOfFields=((iCurrOplRowSet->AccessMap())->GetNoOfFields());
	for(TInt jj=0;jj<aFirstString;jj++)
		{
		if (jj>0) // takes us to first string field (or 2nd) =ff
			ff++;
		while ((iCurrOplRowSet->AccessMap())->ReadType(ff)!=EDbColText)
			ff++;
		}
	for(TInt ii=0;ii<aNoOfStrings;ii++)
		{
		if (ii>0)
			{
			aSQL.Append(KOr);
			ff++;
			}
		while ((iCurrOplRowSet->AccessMap())->ReadType(ff)!=EDbColText)
			{
			ff++;
			if (ff>=numberOfFields)
				User::Leave(KOplErrSyntax); 
			}
		aSQL.Append(((*iCurrOplRowSet->AccessColSet())[ff+1]).iName);
		aSQL.Append(KLikeQuote);
		aSQL.Append(aString);
		aSQL.Append('\'');
		}
	}

CBufFlat* COplDbManager::GenerateSQLBufLC(TInt aFirstString, TInt aNoOfStrings,const TDesC& aString)
// Generate WHERE clause in dynamic buffer
// New function added to fix bug SW1-397 : FIND and FINDFIELD were using a TBuf<256> for the query
	{
	CBufFlat* sqlBuf=CBufFlat::NewL(128);
	CleanupStack::PushL(sqlBuf);
	TInt ff(0);
	TInt numberOfFields=((iCurrOplRowSet->AccessMap())->GetNoOfFields());
	for(TInt jj=0;jj<aFirstString;jj++)
		{
		if (jj>0) // takes us to first string field (or 2nd) =ff
			ff++;
		while ((iCurrOplRowSet->AccessMap())->ReadType(ff)!=EDbColText)
			{
			ff++;
			if (ff>=numberOfFields)
				User::Leave(KOplErrSyntax); //!!todo -- check this.
			}
		}
	for(TInt ii=0;ii<aNoOfStrings;ii++)
		{
		if (ii>0)
			{
			TPtrC bufOr=KOr();
			sqlBuf->InsertL(sqlBuf->Size(),bufOr.Ptr(),bufOr.Size());
			ff++;
			}
		while ((iCurrOplRowSet->AccessMap())->ReadType(ff)!=EDbColText)
			{
			ff++;
			if (ff>=numberOfFields)
				User::Leave(KOplErrSyntax); 
			}
			TPtrC name=((*iCurrOplRowSet->AccessColSet())[ff+1]).iName;
			sqlBuf->InsertL(sqlBuf->Size(),name.Ptr(),name.Size());
			TPtrC bufLike=KLikeQuote();
			sqlBuf->InsertL(sqlBuf->Size(),bufLike.Ptr(),bufLike.Size());
			sqlBuf->InsertL(sqlBuf->Size(),aString.Ptr(),aString.Size());
			TPtrC bufQuote=KQuote();
			sqlBuf->InsertL(sqlBuf->Size(),bufQuote.Ptr(),bufQuote.Size());
		}
	return sqlBuf;
	}

EXPORT_C TInt COplDbManager::NoOfStrFields()
	{
	TInt numberOfFields=((iCurrOplRowSet->AccessMap())->GetNoOfFields());
	TInt num(0);
	for(TInt ii=0;ii<numberOfFields;ii++)
		{
		if(iCurrOplRowSet->AccessMap()->ReadType(ii)==EDbColText)
			num++;
		}
	return num;
	}

EXPORT_C void COplDbManager::FindString(const TDesC& aString)
	{
	TInt num=NoOfStrFields();
	// Fix for bug SW1-397 : FIND and FINDFIELD were using a TBuf<256> for the query
	CBufFlat* sql=GenerateSQLBufLC(1,num,aString);
	//!!TODO Does this comparefolded thing need changing for Unicode?
	TPtrC8 sqlPtr=sql->Ptr(0);

	TPtr newBuf((TText*)sqlPtr.Ptr(),sql->Size()>>1,sql->Size()>>1);
	TDbQuery dbQuery(newBuf,EDbCompareFolded);

	TInt iterations(KErrNotFound);
	if(iCurrDbRowSet->AtRow())
		iterations=iCurrDbRowSet->FindL(RDbRowSet::EForwards, dbQuery);
	if (iterations==KErrNotFound)
		{
		iCurrOplRowSet->SetPos(iCurrDbRowSet->CountL()+1);
		iStack.Push(TInt16(0));
		}
	else
		{
		iCurrOplRowSet->SetPosRelative(iterations);
		iStack.Push(TInt16(iCurrOplRowSet->GetPos()));
		}
	CleanupStack::PopAndDestroy();	// sql
	return;
	}

const TInt KOplFindForward = 0x01;
const TInt KOplFindFromAnEnd = 0x02;
const TInt KOplFindCaseDependent = 0x10;
const TInt KOplFindFieldMask = KOplFindForward|KOplFindFromAnEnd|KOplFindCaseDependent;

EXPORT_C void COplDbManager::FindField()
	{
	TInt flag=iStack.PopInt16();
	TInt num=iStack.PopInt16();
	TInt start=iStack.PopInt16();
	TPtrC string=iStack.PopString();
	//!!TODO Does this comparefolded thing need changing?
	TDbTextComparison caseDependancy(EDbCompareFolded); // o -> not , 1 -> dependant
	RDbRowSet::TDirection direction;
	TInt numMax=NoOfStrFields();

	if ((start+num-1>numMax) || (flag & (~KOplFindFieldMask)))
		User::Leave(KOplErrInvalidArgs);
	if (flag&KOplFindForward)
		{
		direction=RDbRowSet::EForwards;
		if (flag&KOplFindFromAnEnd)
			FirstL(); // use dbms , NO
		if (!iCurrDbRowSet->AtRow())
			{
			iStack.Push(TInt16(0));
			iCurrOplRowSet->SetPos(iCurrDbRowSet->CountL()+1);
			return;
			}
		}
	else
		{
		direction=RDbRowSet::EBackwards;
		if ((flag&KOplFindFromAnEnd) || (!iCurrDbRowSet->AtRow()))
			LastL();

		}
	if (flag&KOplFindCaseDependent)
		caseDependancy=EDbCompareNormal;

	// Fix for bug SW1-397 : FIND and FINDFIELD were using a TBuf<256> for the query
	CBufFlat* sql=GenerateSQLBufLC(start,num,string);
	TPtrC8 sqlPtr=sql->Ptr(0);
	TPtr newBuf((TText*)sqlPtr.Ptr(),sql->Size()>>1,sql->Size()>>1);
	TDbQuery dbQuery(newBuf,caseDependancy);

	TInt iterations=iCurrDbRowSet->FindL(direction, dbQuery);
	if (iterations==KErrNotFound)
		{
		iStack.Push(TInt16(0));
		if(direction==RDbRowSet::EForwards)
			iCurrOplRowSet->SetPos(iCurrDbRowSet->CountL()+1);
		else
			FirstL();
		}
	else
		{
		iCurrOplRowSet->SetPosRelative(direction==RDbRowSet::EForwards?iterations:-iterations);
		TInt16 ret=TInt16(iCurrOplRowSet->GetPos());
		iStack.Push(ret);
		}
	CleanupStack::PopAndDestroy();	// sql
	}

EXPORT_C TInt COplDbManager::Bookmark()
	{
	TInt count=iBookmarkArray.Count();
	TInt ret(-1);
	TOplBookmark mark;
	for(TInt ii=0;ii<count;ii++)
		{
		if (!iBookmarkArray[ii].MarkAlive())
			{
			ret=ii;
			mark.SetMark(iCurrDbRowSet->Bookmark());
			iBookmarkArray[ret]=(mark);
			break;
			}
		}
	if(ret<0)
		{
		ret=count;
		mark.SetMark(iCurrDbRowSet->Bookmark());
		iBookmarkArray.AppendL(mark);
		}
	return (ret+1);
	}

EXPORT_C void COplDbManager::GotoMark(TInt aMark)
	{
	if(aMark<1 || aMark>iBookmarkArray.Count() || !iBookmarkArray[aMark-1].MarkAlive())
		User::Leave(KOplErrInvalidArgs);
	else
		iCurrDbRowSet->GotoL(iBookmarkArray[aMark-1].GetMark());
	}

EXPORT_C void COplDbManager::KillMark(TInt aMark)
	{
	if (aMark<1)
		User::Leave(KOplErrInvalidArgs);
	iBookmarkArray[aMark-1].KillMark();
	}

EXPORT_C void TOplBookmark::SetMark(TDbBookmark aMark)
	{
	iBookmark=aMark;
	iAlive=ETrue;
	}

EXPORT_C TInt32 COplDbManager::Space()
/*
Returns LONG of bytes left on the device where the current logical name is from
  */
	{
	TVolumeInfo info;
	iFs.Volume(info,TDriveUnit(iCurrOplRowSet->ParentDbase()->Name()));
	return info.iFree.Low();
	}

EXPORT_C void COplDbManager::DeleteTable(TPtrC aTable,TPtrC aDbase)
	{
	TFileName dbName(aDbase);
	TParse parser;
	iFs.Parse(dbName,parser);
	TBufC<KMaxFileName> name(parser.FullName());
	COplDb *db;
	if ((db=iOplDbCollection->FindOpenDbase(name,EFalse))==NULL)				
		{											
		db=COplDb::NewLC(name);
		db->OpenL(iFs,name,ETrue);		
		}
	else 
		User::Leave(KOplErrOpen);
	db->StoreDbase().DropTable(aTable);
	db->Close();
	CleanupStack::PopAndDestroy();	// db
	}

EXPORT_C void COplDbManager::BeginTrans()
	{				
	RDbStoreDatabase& store=iCurrOplRowSet->ParentDbase()->StoreDbase();
	if ((!iCurrOplRowSet->InAppendOrUpdate()) && (!iCurrOplRowSet->InModifyOrInsert()) && (!store.InTransaction()))
		store.Begin();
	else
		User::Leave(KOplErrInTransaction);				// error if in any mode already	
	}

EXPORT_C void COplDbManager::CommitTrans()
	{
	RDbStoreDatabase& store=iCurrOplRowSet->ParentDbase()->StoreDbase();
	if ((!iCurrOplRowSet->InAppendOrUpdate()) && (!iCurrOplRowSet->InModifyOrInsert()) && (store.InTransaction()))
		User::LeaveIfError(store.Commit());
	else
		User::Leave(KOplErrInTransaction);				// error if in any mode already
	}

EXPORT_C void COplDbManager::Rollback()
	{
	RDbStoreDatabase& store=iCurrOplRowSet->ParentDbase()->StoreDbase();
	if ((!iCurrOplRowSet->InAppendOrUpdate()) && (!iCurrOplRowSet->InModifyOrInsert()) && (store.InTransaction()))
			store.Rollback();
	else
		User::Leave(KOplErrInTransaction);				// error if in any mode already
	ResetRelatedViews();
	}

EXPORT_C TInt COplDbManager::InTrans()
	{
	return (iCurrOplRowSet->ParentDbase()->StoreDbase().InTransaction() ? -1 : 0);
	}

EXPORT_C void COplDbManager::CompactL(TPtrC aDbase)
	{	
	TParse parser;
	iFs.Parse(aDbase,parser);

	CFileStore* store=CFileStore::OpenLC(iFs,parser.FullName(),EFileRead|EFileWrite);	// file closed on error
	store->CompactL();
	store->CommitL();
	CleanupStack::PopAndDestroy();	// store
	}