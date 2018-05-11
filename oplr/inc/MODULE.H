// MODULE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef MODULE_H
#define MODULE_H

#include <opx.h>
#include <oplfile.h>

const TInt KMaxModules=8;

enum  TOplTranVersion
	{
	EOplTranVersionOpl1993=0x111F,
	EOplTranVersionOpler1=0x200A,
	EOplTranVersionCrystal=0x3000
	};

// runtime handles this module file version : incompatible version if iMinRuntimeVersion is greater
const TUint KOplRuntimeModuleVersion = EOplTranVersionCrystal; 

enum  TOplParseType
	{
	EOplParseModule,
	EOplParseOpx
	};

struct SOpxData
	{
	COpxBase	*iTls; 
	RLibrary	iHandle;
	};

class COpxModule : public CBase	  
	{
public:
	~COpxModule();
	static COpxModule* NewL(CModuleReader& aModuleReader);
	void ConstructL(CModuleReader& aModuleReader);
	COpxBase& TheTls(TInt aOpxNo){return *(*iOpxs)[aOpxNo].iTls;};
	inline void CallOpxFunc(TInt aOpxNo,TInt aProcNo)
		{TheTls(aOpxNo).RunL(aProcNo);}
private:
	void LoadOpxL(CModuleReader& aModuleReader,SOpxData& aOpxData);  
	TInt iOpxCount;
	CArrayFixFlat<SOpxData> *iOpxs;
	};

class CProcedure;
class CProcDef;
class CModule : public CBase	  
	{
public:
	static CModule *NewL(RFs &aFsClient,const TDesC &aOpoName);
	void ConstructL(RFs &aFsClient);
	~CModule();
	const TFileName &OpoName() const {return iOpoName;};
	const TFileName &OplName() const {return iOplName;};
	CProcDef* FindProcedure(const TDesC &aProcName);
	CProcDef* FindFirstProcedure();
	COpxModule* Opxs() const {return iOpxs;};
	inline void IncCount() {iAccessCount++;};
	void DecCount();
	inline TInt Count() const {return iAccessCount;};
	inline RFile FileHandle() const {return iFileHandle;};
	inline TUid Uid() const {return iUid;}
	inline CTextTranslator::TDebugFlag DebugFlag() const {return iDebugFlag;}
private:
	HBufC8* iProcTable;
	TBool SignatureOK(const TDesC &aFileSignature) const;		
	RFile iFileHandle;
	TUint  iOplVersion;
	TInt iAccessCount;
	TFileName iOpoName;
	TFileName iOplName;
	COpxModule* iOpxs;
	TSglQue<CProcDef> iProcsQueHeader;	// was Dbl
	TUid iUid;
	CTextTranslator::TDebugFlag iDebugFlag;
	};

class CModulesCollection : public CBase
	{
public:
	void Init(RFs &aFsClient);
	void LoadModuleL(const TDesC &aFileName);
	TInt UnloadModule(const TDesC &aFileName);
	CProcDef *FindProcedure(const TDesC &aProcName);
 	CProcDef *FindFirstProcedure();
	~CModulesCollection();
	//inline CModule *NextModule(TInt i) {if (i<iNumModulesLoaded) return iModule[i]; else return NULL;}  
	inline TUid Uid() {return iUid;}
private:
	CModule* FindModule(const TFileName &aModuleName) const;
	TInt FindAndDestructModule(const TFileName &aFileName);
	TFileName iModulePath;
	RFs	iFsClient;
	TInt iNumModulesLoaded;		
	CModule *iModule[KMaxModules];
	TUid iUid;
	};

class CProcDef : public CBase
	{
public:
	TSglQueLink	iLink;					// was Dbl and Link : don't move it around
	CProcDef(CModule *aModule,TOpoProcHeader *aProcHeader);
	inline const TPtrC& ProcName() const {return iProcName;}
	~CProcDef();
	void IncCountL();
	inline CProcedure* Procedure() {return iProcedure;};

	inline void DecCount()  {iModule->DecCount();};
	inline TInt FileOffset() {return iFileOffset;};
	inline TInt LineInSource() {return iLineInSource;};
	inline CModule& Module() {return *iModule;};
	inline const TFileName& ModuleName() const {return iModule->OpoName();};
private:
	CModule *iModule;
	TPtrC iProcName;
	TInt iFileOffset;
	TInt iLineInSource;
	CProcedure *iProcedure;		  // The only owned member
	};

GLREF_C TFileName ParseName(const RFs& aFsClient,TFileName& aPath,const TDesC& aName,TOplParseType aFlag);
#endif

