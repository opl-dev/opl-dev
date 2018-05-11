// MODULE.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include <oplr.h>
#include <module.h>
#include <opcodes.h>
#include "frame.h"

typedef COpxBase * (*TOpxSetTls)(OplAPI& aAPI);

// to be shared between CModuleCollection and COpxModule
GLDEF_C void ParseName(const RFs& aFsClient,TFileName& aPath,const TDesC& aName,TDes& aFileName,TOplParseType aFlag)
// aPath may be changed, useful for CModuleCollection
	{
	TParse parse;
	User::LeaveIfError(aFsClient.Parse(aName,aPath,parse));
	if (aPath.Length()==0) // top level module!
		aPath=parse.DriveAndPath();
	aFileName=parse.FullName();
	if (!parse.ExtPresent())
		{
		_LIT(KOpo,".OPO");
		_LIT(KOpx,".OPX");
		TBuf<4> ext=KOpo();
		if (aFlag!=EOplParseModule)
			ext=KOpx;
		aFileName.Append(ext);
		}
	}


CProcDef::CProcDef(CModule *aModule,TOpoProcHeader *aProcHeader)
: iModule(aModule)
	{
	iProcName.Set((TText*)aProcHeader->iOplProcName->iModuleName,aProcHeader->iOplProcName->iLength);
	iFileOffset=aProcHeader->iProcOffset;
	iLineInSource=aProcHeader->iLineNumber;
	}

CProcDef::~CProcDef()	
	{
	delete iProcedure;
	}

void CProcDef::IncCountL()
	{
	if (iProcedure==NULL)
		{
		CProcedure* procedure=new(ELeave) CProcedure;
		CleanupStack::PushL(procedure);
		procedure->ConstructL(this,iModule->FileHandle());
		CleanupStack::Pop();
		iProcedure=procedure;
		}
	iModule->IncCount();
	}

void COpxModule::ConstructL(CModuleReader& aModuleReader)
	{
	aModuleReader.SeekOpxHeaderL();
	TInt total=aModuleReader.ReadCountOfOpxsL();
	iOpxs=new(ELeave) CArrayFixFlat<SOpxData>(total);
	for (;iOpxCount<total;iOpxCount++)
		{
		// !!! This must be changed to a CBase class!!!
		SOpxData s;
		s.iTls=NULL; 

		//
		iOpxs->AppendL(s);
		LoadOpxL(aModuleReader,(*iOpxs)[iOpxCount]);
		}
	}

COpxModule *COpxModule::NewL(CModuleReader& aModuleReader)
	{
	COpxModule *This=new(ELeave) COpxModule;
	CleanupStack::PushL(This);
	This->ConstructL(aModuleReader);
	CleanupStack::Pop();
	return This;
	}

COpxModule::~COpxModule()
	{
	TInt i;
	for (i=0;i<(*iOpxs).Count() && (*iOpxs)[i].iTls!=NULL;i++) // relies on last element loaded
		{
		if (((*iOpxs)[i].iTls)->DecAccessCount()==0)    // SOpxData should be a CBase class
														// COplrOpxData (OPXs already use COpxData)
			delete (*iOpxs)[i].iTls;
		(*iOpxs)[i].iHandle.Close();	// ?? Can we close it many times
		}
	delete iOpxs;
	}

void COpxModule::LoadOpxL(CModuleReader& aModuleReader,SOpxData& aOpxData)  
	{
	TOpoOpxHeader *opxHeader=aModuleReader.ReadOpxHeaderL();

	COplRuntime& rt=*TheRuntime();
	TInt ret=aOpxData.iHandle.Load(opxHeader->iOpxFileName);
	if (ret<0)
		{
		rt.SetErrorBuffer(&opxHeader->iOpxFileName);
		User::Leave(ret);
		}
	TOpxSetTls setTls=(TOpxSetTls)aOpxData.iHandle.Lookup(EOpxCreate);
	aOpxData.iTls=(*setTls)(*rt.TheOplAPI());
	aOpxData.iTls->IncAccessCount();
	if (!aOpxData.iTls->CheckVersion(opxHeader->iVersion))
		{
		rt.SetErrorBuffer(&opxHeader->iOpxFileName);
		User::Leave(KOplErrOpxVersion);
		}
	}

void CModule::ConstructL(RFs &aFsClient)
	{
 	CModuleReader *moduleReader=new(ELeave) CModuleReader;
	CleanupStack::PushL(moduleReader);
	iFileHandle=moduleReader->CreateL(aFsClient,iOpoName);

#ifdef INTERNAL_RELEASE
	if (!moduleReader->IsOpler1OPO() && !moduleReader->SignatureOK())
		User::Leave(KOplErrBadFileType);
#endif
	TOplName *pOplName=NULL;
	if ((pOplName=moduleReader->ReadOplNameL())==NULL)
		User::Leave(KOplErrBadFileType);
	iUid=moduleReader->Uid();
	iOplName.Copy((TText*)pOplName->iModuleName,pOplName->iLength);
	const TOpoRootStream& root=moduleReader->RootStream();
	if (moduleReader->IsOpler1OPO())
		{
		iOplVersion=root.iTransVersion;
		if (root.iMinRunVersion>KOplRuntimeModuleVersion)	 
			// error if requires rt to handle module version greater than it does 
			// (ie. incompatible module from future)
			User::Leave(KOplErrNewVer);
		}
#ifdef INTERNAL_RELEASE
	else
		{
		TOpoModuleHeader16 *pModuleHeader=moduleReader->ReadModuleHeader16L();
		iOplVersion=pModuleHeader->iTransVersion;
		if (iOplVersion!=EOplTranVersionOpl1993 &&	// error if not opl1993 module or incompat module from future
								pModuleHeader->iMinRunVersion>KOplRuntimeModuleVersion)
			User::Leave(KOplErrNewVer);
		}
#endif
	iProcsQueHeader.SetOffset(_FOFF(CProcDef,iLink));  // was Link
	moduleReader->SeekProcHeaderL();

	moduleReader->ReadProcTableL(iProcTable);
	TUint8* pProcInfo=(TUint8*)iProcTable->Ptr();

	TOpoProcHeader *pProcHeader=NULL;

	while ((pProcHeader=moduleReader->ReadProcHeaderL(pProcInfo))!=NULL)
		iProcsQueHeader.AddLast(*(new(ELeave) CProcDef(this,pProcHeader)));

	if (root.iOpxTableId!=NULL)
		iOpxs=COpxModule::NewL(*moduleReader);
	iDebugFlag=root.iDebugFlag;
	CleanupStack::PopAndDestroy();
	}

CModule *CModule::NewL(RFs &aFsClient,const TDesC &aOpoName)
	{
	CModule *This=new(ELeave) CModule;
	CleanupStack::PushL(This);
	This->iOpoName.Copy(aOpoName);
	This->ConstructL(aFsClient);
	CleanupStack::Pop();
	return This;
	}

CModule::~CModule()
	{
	CProcDef *pProc;
 	TSglQueIter<CProcDef> iterator(iProcsQueHeader);	// was TDblQueIter
	while ((pProc=iterator++)!=NULL)
		delete pProc; 
	delete iOpxs;
	iFileHandle.Close();
	User::Free(iProcTable);
	}

CProcDef *CModule::FindFirstProcedure() 
	{
	if (iProcsQueHeader.IsEmpty())
		return NULL;
	else
		return iProcsQueHeader.First();
	}

CProcDef *CModule::FindProcedure(const TDesC &aProcName) 
	{
	CProcDef *pProc;
	TSglQueIter<CProcDef> iterator(iProcsQueHeader);  // was TDblQueIter
	while ((pProc=iterator++)!=NULL)
		{
		TInt lenProcName=aProcName.Length()*sizeof(TText);
		TInt lenNameInModule=pProc->ProcName().Length()*sizeof(TText);
		if (lenProcName==lenNameInModule)
			{
			const TUint8* pProcName=(TUint8*)aProcName.Ptr();
			const TUint8* pNameInModule=(TUint8*)pProc->ProcName().Ptr();
			if (Mem::Compare(pNameInModule,lenNameInModule,pProcName,lenProcName)==0)
				break;
			}
		}
	return pProc;							
	}
void CModule::DecCount() 
	{
	if ((--iAccessCount)==0)
		delete this;
	}

void CModulesCollection::Init(RFs &aFsClient)
	{
	iNumModulesLoaded=0;
	iFsClient=aFsClient;
	}

CProcDef* CModulesCollection::FindProcedure(const TDesC &aProcName)
	{
	TInt i;
	CProcDef *pProc=NULL;
	for (i=0; i<iNumModulesLoaded; i++)
		{
		if ((pProc=iModule[i]->FindProcedure(aProcName))!=NULL)
			break;
		}
	if (i==iNumModulesLoaded) //not found
		{
		TheRuntime()->SetErrorBuffer(&aProcName);
		User::Leave(KOplErrNoProc);
		}
	return pProc;
	}

CProcDef* CModulesCollection::FindFirstProcedure()
	{
	return	iModule[0]->FindFirstProcedure();
	}

CModule* CModulesCollection::FindModule(const TFileName &aFileName)	const
	{
	TInt i;
	for (i=0; i<iNumModulesLoaded; i++)
		{
		if (aFileName.CompareF(iModule[i]->OpoName())==0)
			break;
		}
	if (i<iNumModulesLoaded)
		return iModule[i];	   
	else
		return NULL;							
	}

TInt CModulesCollection::FindAndDestructModule(const TFileName &aFileName)
	{
	TInt i;
	for (i=0; i<iNumModulesLoaded; i++)
		{
		if (aFileName.CompareF(iModule[i]->OpoName())==0)
			break;
		}
	if (i<iNumModulesLoaded)
		{
		iModule[i]->DecCount();
		for (; i<(iNumModulesLoaded-1); i++)
			iModule[i]=iModule[i+1];
		iModule[i]=NULL;
		iNumModulesLoaded--; 
		return KErrNone;
		}	   
	else
		return KOplErrModNotLoaded;							
	}

void CModulesCollection::LoadModuleL(const TDesC &aFileName) 
	{
	TFileName fileName;
	ParseName(iFsClient,iModulePath,aFileName,fileName,EOplParseModule);
	
	if (fileName.Length() == 0)
		User::Leave(KErrBadName);
	
	if (FindModule(fileName)!=NULL)	
		User::Leave(KOplErrModLoad);				// module is loaded
	if (iNumModulesLoaded==KMaxModules)
		User::Leave(KOplErrMaxLoad);
	CModule* mod=iModule[iNumModulesLoaded]=CModule::NewL(iFsClient,fileName); 	
	iModule[iNumModulesLoaded++]->IncCount();
	if (iUid==KNullUid)
		iUid=mod->Uid();
	}

TInt CModulesCollection::UnloadModule(const TDesC &aFileName)
	{
	TFileName fileName;
	ParseName(iFsClient,iModulePath,aFileName,fileName,EOplParseModule);
	if (fileName.Length() == 0)
		return KErrBadName;
	return FindAndDestructModule(fileName);	
	}

CModulesCollection::~CModulesCollection()
	{
	TInt i;
	for (i=iNumModulesLoaded-1; i>=0; i--)
		delete (iModule[i]);
	iNumModulesLoaded=0;
	}

void OpCode::LoadM(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aRuntime.ModulesCollection().LoadModuleL(aStack.PopString());
	aRuntime.ClearTrap();
	}
	
void OpCode::UnLoadM(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	User::LeaveIfError(aRuntime.ModulesCollection().UnloadModule(aStack.PopString()));
	aRuntime.ClearTrap();
	}

void OpCode::CallOpxFunc(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	COpxModule* opxs=aFramePtr->ProcDef().Module().Opxs();
	TInt opxNo=aRuntime.IP8();

	opxs->CallOpxFunc(opxNo,aRuntime.IP16());
	aRuntime.ConEnv()->Cancel(); // in case the opx leaves a Wserv request pending
	}

//////////////////////////////////////////////////////////////////////////
// COpxBase class

EXPORT_C void COpxBase::IncAccessCount()
    {
    ++iAccessCount;
    }

EXPORT_C TInt COpxBase::DecAccessCount()
    {
    return --iAccessCount;
    }

EXPORT_C COpxBase::COpxBase(OplAPI& aOplAPI)
: iOplAPI(aOplAPI)
    {
    ;
    }

