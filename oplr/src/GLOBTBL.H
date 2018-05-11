// GLOBTBL.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


  
#ifndef GLOBTBL_H
#define GLOBTBL_H

#include <e32base.h>

const TInt KMaxGlobalName=32;
const TInt KGlobalTblSize=16;

class TOplGlobal
	{
public:
	TOplGlobal(const TBufC<KMaxGlobalName> aName,const TInt aType,const TAny* aPtr);
private:
	TSglQueLink iLink;
	TBufC<KMaxGlobalName> iName;
	TInt iType;
	const TAny* iPtr;
friend class CGlobalsTbl;
	};

class CGlobalsTbl : public CBase
	{
public:
	CGlobalsTbl();
	~CGlobalsTbl();
	void AddL(const TBufC<KMaxGlobalName>& aName,const TInt aType,const TAny* aPtr);
	void Remove(const TBufC<KMaxGlobalName>& aName);
	const TAny* Find(const TBufC<KMaxGlobalName>& aName,const TInt aType);
	void PrepareToAddC();
private:
	static void RemoveAddedGlobals(TAny* aGlobalsTbl);
	TOplGlobal* iListMark[KGlobalTblSize];
private:
	TInt GetHashValue(const TBufC<KMaxGlobalName>& aName);
	TSglQue<TOplGlobal> iList[KGlobalTblSize];
	};



#endif

