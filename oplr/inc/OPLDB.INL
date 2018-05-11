// OPLDB.INL
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// class COplROwSet
inline COplDb* COplRowSet::ParentDbase()
     {return iParentDbase;}

// class COplDb
inline TInt& COplDb::AccessCount()
	 {return iAccessCount;}

inline const RDbStoreDatabase&  COplDb::Dbase() const
	{return iDbase;}

inline TPtrC  COplDb::Name() const
    {return iName;}

// class COplDbManager


inline TInt COplDbManager::Count()
	{return iCurrDbRowSet->CountL();} 

inline void CLogicalNames::SetCurrLogName(TInt aLogName)
	{iCurrLogName=TOplCurrRowSet(aLogName);}

inline  TOplCurrRowSet CLogicalNames::GetCurrLogName()
	{return iCurrLogName;}
inline TInt CLogicalNames::Count()
	{return iCount;}

inline  COplRowSet* CLogicalNames::FindRowSet(TInt aLogicalFile) const
	{return iLogNames[aLogicalFile];} 

inline COplDbCollection* COplDbManager::OplDbCollection() 
	{return iOplDbCollection;}

inline COplRowSet* COplDbManager::TheCurrentOplRs() 
	{return iCurrOplRowSet;}

inline CStack& COplDbManager::Stack()
	{return iStack;};

inline RFs& COplDbManager::FsSession()
	{return iFs;};

inline CLogicalNames* COplDbManager::GetiLogNames()
	{return iLogicalNames;};

// Bookmarks
inline TBool TOplBookmark::MarkAlive() 
	{return iAlive;}

inline TDbBookmark TOplBookmark::GetMark() 
	{return iBookmark;}
inline void TOplBookmark::KillMark()
 {iAlive=EFalse;}

// COplRowSet
inline void COplRowSet::SetPos(TInt aPos) 
	{iPos=aPos;}
inline void COplRowSet::SetPosRelative(TInt aPos)
	{iPos=iPos+aPos;}
inline void COplRowSet::SetPosBeforeInsert(TDbBookmark aMark)
	{iPosBeforeInsert=aMark;}
inline void COplRowSet::SetOplFieldMap(COplFieldMap* aMap)
	{iMap=aMap;}
inline void  COplRowSet::ResetRowsAbsent()
	{iRowsWereAbsent=EFalse;}
inline void  COplRowSet::ModifyOrInsert(TBool aState) 
	{iModifyOrInsert=aState;}
inline void  COplRowSet::InsertMode(TBool aState)	
	{iInsertMode=aState;}
inline void  COplRowSet::AppendOrUpdate(TBool aState) 
	{iAppendOrUpdate=aState;}

inline RDbRowSet* COplRowSet::AccessDbRowSet() 
	{return iDbRowSet;}
inline CDbColSet* COplRowSet::AccessColSet() 
	{return iDbColSet;}
inline COplFieldMap* COplRowSet::AccessMap()
	{return iMap;}
inline TDbBookmark COplRowSet::GetPosBeforeInsert() 
	{return iPosBeforeInsert;}
inline TInt COplRowSet::GetPos()
	{return iPos;}
inline TBool COplRowSet::IAmATable() 
	{return iAmATable;}
inline TBool COplRowSet::InModifyOrInsert() 
	{return iModifyOrInsert;}
inline TBool COplRowSet::InInsertMode()
	{return iInsertMode;}
inline TBool COplRowSet::InAppendOrUpdate() 
	{return iAppendOrUpdate;}
inline TBool COplRowSet::RowsAbsent()
	{return iRowsWereAbsent;}

inline CArrayPtrFlat<COplRowSet>* CLogicalNames::RowSetArray() 
	{return &iLogNames;}

//COPLDB
inline RDbStoreDatabase& COplDb::StoreDbase()
	{return iDbase;}
inline TBool COplDb::DataApp()
	{return iDataApp;}



