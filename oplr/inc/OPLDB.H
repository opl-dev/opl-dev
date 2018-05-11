// OPLDB.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#ifndef __OPLDB_H__
#define __OPLDB_H__

#include <e32base.h>
#include <d32dbms.h>
#include <f32file.h>
#include <opldbdef.h>
#include <oplapi.h>

enum TOplCurrRowSet
	{
	EDbNone=-1,
	EDbA,EDbB,EDbC,EDbD,EDbE,EDbF,EDbG,EDbH,EDbI,
	EDbJ,EDbK,EDbL,EDbM,EDbN,EDbO,EDbP,EDbQ,EDbR,
	EDbS,EDbT,EDbU,EDbV,EDbW,EDbX,EDbY,EDbZ
	};

enum TOplDbOpenMode
	{
	EDbOpen,
	EDbCreate
	};

class TOplBookmark 
     {
 public:
     inline void KillMark();
     inline TBool MarkAlive();
     IMPORT_C void SetMark(TDbBookmark aMark);
     inline TDbBookmark GetMark();

 private:
     TDbBookmark iBookmark;
     TBool iAlive;
     };

class TOplFieldInfo;
class COplFieldMap : public CBase
     {		   								  
 public:
     IMPORT_C static COplFieldMap* NewLC(TInt aCount);
     inline TInt GetNoOfFields(); 
     IMPORT_C void AppendFieldL (TDesC& aField, TInt aType);
	 // reads and converts type to dbms type
	 IMPORT_C TDbColType ReadType(TInt aMapIndex);				
	 // reads and converts name to dbms name
	 IMPORT_C void ReadName(TInt aMapIndex,TDes& aColName);
	 IMPORT_C TInt ScanForOrdinal(const TDesC& aOplFieldName);
     IMPORT_C void InsertBlankL(TInt anOrdinal,const TDesC &aName);					
 private: 
	 COplFieldMap(TInt aCount); 
 private:
    CArrayFixFlat<TOplFieldInfo> iFieldInfoArray;
	 };

class COplDbCollection;
class COplDb;
class COplRowSet : public CBase
     {
 public:
	 IMPORT_C static COplRowSet* NewL(COplDbCollection& aDbCOllect, RFs& aFs,const TDesC& aString, COplFieldMap* aFieldMap,TOplDbOpenMode aMode, TBool allowUpdates);
     IMPORT_C ~COplRowSet();
	 inline void SetOplFieldMap(COplFieldMap* aMap);
	 inline RDbRowSet* AccessDbRowSet();
     inline CDbColSet* AccessColSet();
     inline COplFieldMap* AccessMap();
	 inline COplDb* ParentDbase();
	 IMPORT_C TInt GetOrdinal(const TDesC& aOplFieldName);
     inline void SetPosBeforeInsert(TDbBookmark aMark); 
     inline TDbBookmark GetPosBeforeInsert();
     inline void SetPos(TInt aPos);
     inline void SetPosRelative(TInt aPos);
     inline TInt GetPos();
     inline TBool IAmATable();
     inline TBool InModifyOrInsert();
     inline TBool InInsertMode();
     inline TBool InAppendOrUpdate();
     inline TBool RowsAbsent();
     inline void  ResetRowsAbsent();
     inline void  ModifyOrInsert(TBool aState);
     inline void  InsertMode(TBool aState);
     inline void  AppendOrUpdate(TBool aState);
     IMPORT_C void  PrepareForUpdate();
     IMPORT_C void  CopyCurrRowToEnd();
     IMPORT_C void  InsertBlankL();
     IMPORT_C void  ExpandFieldMapToCoverRichText(COplFieldMap& aFieldMap);
	 IMPORT_C static COplRowSet* NewLC(COplDbCollection& aDbCollect, RFs& aFs,const TDesC& aString,COplFieldMap* aFieldMap,TOplDbOpenMode aMode, TBool allowUpdates);
	 IMPORT_C void OpenRowSetL(RDbRowSet* aRowSet,TBool aIsATable,COplFieldMap* aFieldMap);
private:
	 COplRowSet(COplDbCollection& aDbCOllect);
     void ConstructL(RFs& aFs,const TDesC& aDatabase,COplFieldMap* aFieldMap,TOplDbOpenMode aMode,TBool allowUpdates);
     void DoCreateL(TLex& aLex,TParse& aParse ,COplFieldMap& aFieldMap);
     void DoOpenL(const TDesC& aString,COplFieldMap& aFieldMap,TBool allowUpdates);
private: 
	 COplDb* iParentDbase; // cached from COplDbCollection
     COplFieldMap* iMap;
     CDbColSet* iDbColSet;
     COplDbCollection& iDbCollect;
     RDbRowSet* iDbRowSet;
     TDbBookmark iPosBeforeInsert;
     TInt iPos;
     TBool iRowsWereAbsent;
     TBool iAmATable;
     TBool iModifyOrInsert;
     TBool iAppendOrUpdate;
     TBool iInsertMode;
     };

class CLogicalNames : public CBase
	 {
 public:	
	 IMPORT_C static CLogicalNames* NewL();
     IMPORT_C ~CLogicalNames();
	 IMPORT_C void AddRowSet(TInt aLogicalNameNumber,COplRowSet* aRowSet);
	 inline  COplRowSet* FindRowSet(TInt aLogicalFile) const;	//can leave ?
	 IMPORT_C COplRowSet* CloseOplRowSet();
	 inline  void SetCurrLogName(TInt aLogName);
	 inline  TOplCurrRowSet GetCurrLogName();
     inline  TInt Count();
     IMPORT_C void CancelAllUpdates();
     inline CArrayPtrFlat<COplRowSet>* RowSetArray();
 private:
     CLogicalNames();
     void ConstructL();
 private:
 	 CArrayPtrFlat<COplRowSet> iLogNames;
	 TInt iCount;
	 TOplCurrRowSet iCurrLogName;
	 };

class CFileStore;
class COplDb : public CBase
     {
 public:	 
     static COplDb* NewLC(const TDesC& aName);
     ~COplDb();
     void CreateL(RFs& aFs,const TDesC& aDbaseName);
     void OpenL(RFs& aFs,const TDesC& aDbaseName,const TBool aUpdate);
	 IMPORT_C void Close();
	 IMPORT_C TInt CreateTable(const TDesC& aTableName, const CDbColSet &aColSet);
     inline RDbStoreDatabase& StoreDbase();
     inline TBool DataApp();
     inline const RDbStoreDatabase& Dbase() const;		   // return iDbase

     inline TPtrC Name() const;
	 inline TInt& AccessCount(); // cant reference stack Int ?
 private:
     COplDb(const TDesC& aName);
 private:
 	 TBufC<KMaxFileName> iName;
	 CFileStore* iStore;
	 RDbStoreDatabase iDbase;
	 TInt iAccessCount;
     TBool iDataApp;
	 };

class COplDbCollection : public CBase
	 {
 public:
	 static COplDbCollection* NewL();
	 ~COplDbCollection();
	 IMPORT_C COplDb* FindOpenDbase(const TDesC& aName,const TBool aIncAccessCount);
     IMPORT_C COplDb* CreateOrOpenDbaseL(const TDesC &aName,const TOplDbOpenMode aMode,const TBool aUpdate);
	 IMPORT_C void CloseDbIfNotInUseL(COplDb* aParent,COplRuntime& aRuntime);
 private:
     COplDbCollection();
 private:
	 CArrayPtrFlat<COplDb> iOplDb;
 	 };

class COplDbManager : public CBase
     {
 public:
 	enum KLeftSideType {ELeftSideField =-1, ELeftSideVariable};
	static COplDbManager* NewL(COplRuntime& aRuntime,CStack& aStack);
	~COplDbManager();
    inline COplDbCollection* OplDbCollection();
    IMPORT_C void OpenCheckL();
    IMPORT_C TInt NoOfStrFields();
    void OpenL(TOplDbOpenMode aMode, TBool allowUpdates); // handles create, open and openr keyword					  		
    inline COplRowSet* TheCurrentOplRs();
    IMPORT_C void GenerateSQL(TInt aFirstString, TInt aNoOfStrings,const TDesC& String,TDes& aQuery);
    IMPORT_C void LeftSide(TInt aLog);
	IMPORT_C void FirstL();
    IMPORT_C void LastL();
	IMPORT_C void NextL();
	inline TInt Count();
	IMPORT_C TBool EOF();
    IMPORT_C TBool SOF();	
    IMPORT_C TBool SOF(RDbRowSet* aRs);
    IMPORT_C void PositionL(TUint aPos);
    IMPORT_C void PosL();
    IMPORT_C void BackL();
	IMPORT_C void Erase();
	IMPORT_C void Close();
    TInt GetRightSideRowSet(COplRowSet* &aOplRs);
	void Assign(TInt16 aVal);
	void Assign(TInt32 aVal);
	void Assign(TReal64 aReal);
	void Assign(TDesC& aString);
	IMPORT_C void UseLog(TInt aLogicalName);
	IMPORT_C void FindString(const TDesC& aString);
    IMPORT_C void FindField();
    IMPORT_C void AppendL();
    IMPORT_C void UpdateL();
    IMPORT_C void ModifyL();
    IMPORT_C void InsertL();
    IMPORT_C void PutL();
    IMPORT_C void Cancel();
    IMPORT_C void CancelIfUpdate();
    IMPORT_C TInt Bookmark();
    IMPORT_C void GotoMark(TInt aMark);
    IMPORT_C void KillMark(TInt aMark);
    IMPORT_C void DeleteTable(TPtrC aTable,TPtrC aDbase);
    IMPORT_C void BeginTrans();
    IMPORT_C void CommitTrans();
    IMPORT_C void Rollback();
    IMPORT_C TInt InTrans();
	IMPORT_C void CompactL(TPtrC aDbase);

    IMPORT_C TInt32 Space();
    IMPORT_C void StringFieldValueL(TDes& aBuf);
    inline CStack& Stack();
    inline RFs& FsSession();
    IMPORT_C void HandleAutoTransactionRollback();
    IMPORT_C void ResetRelatedViews();             
    IMPORT_C void SafelyPutL();                    
    IMPORT_C void SafetyDeleteL();                    
	IMPORT_C void SetiLogNames(CLogicalNames* aLogicalNames);                    
	inline CLogicalNames* GetiLogNames();
 private:
    COplDbManager(COplRuntime& aRuntime,CStack& aStack);
	void ConstructL();
 	TBool operator=(const COplDbManager&);
    RDbRowSet& PreAssign(TDbColNo& aField);
	CBufFlat* GenerateSQLBufLC(TInt aFirstString, TInt aNoOfStrings,const TDesC& String);
    //HBufC* GenerateSQLBufLC(TInt aFirstString, TInt aNoOfStrings,const TDesC& String);


 private:
    CLogicalNames* iLogicalNames;    
	COplDbCollection* iOplDbCollection;
    COplRowSet* iCurrOplRowSet;	//cached for speed
	RDbRowSet* iCurrDbRowSet;   //cached for speed
    COplRuntime& iRuntime;
    CStack& iStack;
    CArrayFixFlat<TOplBookmark> iBookmarkArray;
    RFs& iFs;
    }; 

#include <OPLDB.inl>
#endif  
  

                                
