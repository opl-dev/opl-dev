// DBASEOPX.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#ifndef __DBASEOPX_H__
#define __DBASEOPX_H__

#include <e32base.h>

#ifndef __OPLAPI_H__
#include <oplapi.h>
#endif

#ifndef __OPXAPI_H__
#include <opxapi.h>
#endif

#ifndef __OPLDOC_H__
#include <opldoc.h>
#endif

#include <fbs.h>
#include <w32std.h>
#include <s32file.h>
#include <oplerr.h>            
#include <OplDb.h>

#define KOpxVersion 0x600

const TInt KOplDontTruncate=-1;

class CTlsDataOPXDBase;

/// procedures for provideing extra database commands
class CDBaseOpx :public CBase
    {
public:
    CDBaseOpx();
    ~CDBaseOpx();
    TInt CheckPointerL(CDbKey* aKeyptr);

    void NewKey(OplAPI& aOplAPI);
    void DeleteKey(OplAPI& aOplAPI);
    void AddField(OplAPI& aOplAPI );    
    void AddFieldTrunc(OplAPI& aOplAPI );
    void IsUnique(OplAPI& aOplAPI );
    void MakeUnique(OplAPI& aOplAPI );
    void SetComparison(OplAPI& aOplAPI );
    void CreateIndex(OplAPI& aOplAPI);
    void DropIndex(OplAPI& aOplAPI);
    void IsDamaged(OplAPI& aOplAPI);
    void Recover(OplAPI& aOplAPI);
    void GetFieldCount(OplAPI& aOplAPI);
    void GetFieldName(OplAPI& aOplAPI);
	void GetFieldType(OplAPI& aOplAPI);
	void RefreshDatabase(OplAPI& aOplAPI);
    CArrayPtrFlat<CDbKey> iKeyArray;
    };

class CTlsDataOPXDBase : public COpxBase 
    {
public:
    static CTlsDataOPXDBase* NewL(OplAPI& aOplAPI);
    void ConstructL();
    CTlsDataOPXDBase(OplAPI& aOplAPI);
    ~CTlsDataOPXDBase() ;
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
    CDBaseOpx* iDBaseHandle;
private:
	// the language extension procedures
	enum TExtensions
		{
		 EAddField=1,
		 EAddFieldTrunc,
		 ECreateIndex,
		 EDeleteKey,
		 EDropIndex,
		 EGetFieldCount,
		 EGetFieldName,
		 EGetFieldType,
		 EIsDamaged,
		 EIsUnique,
		 EMakeUnique,
		 ENewKey,
		 ERecover,
		 ESetComparison,
		 ERefreshDatabase
		};
    };

inline CTlsDataOPXDBase* TheTls() { return((CTlsDataOPXDBase *)Dll::Tls()); }
inline void SetTheTls(CTlsDataOPXDBase *theTls) { Dll::SetTls(theTls); }

#endif __DBASEOPX_H__