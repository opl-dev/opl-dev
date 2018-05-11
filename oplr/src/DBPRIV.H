// DBPRIV.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#ifndef __DBPRIV_H__
#define __DBPRIV_H__

#include <opldbdef.h>
#include <opldb.h>

class TOplFieldInfo 
     {
 public:
	 inline TOplFieldInfo(const TDesC& aOplName=KNullDesC,TUint aType=0) : iOplName(aOplName),iType(aType) {;}
     inline void SetType(TInt aType) {iType=aType;};
     inline TPtrC OplName() const;
 	 inline TInt Type() const ; 
 private:

	 TBufC<KOplMaxFieldName> iOplName;
	 TInt iType;
	 };


// FieldInfo 
inline TInt TOplFieldInfo::Type()	const
 	{return iType;}

inline TPtrC TOplFieldInfo::OplName() 	const
    {return iOplName;}

inline TInt COplFieldMap::GetNoOfFields()
    {return (iFieldInfoArray.Count());}

#endif // __DBPRIV_H__