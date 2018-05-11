// OPX.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#ifndef __OPX_H__
#define __OPX_H__

const TInt KUidOpxValue=0x10003A7B;
const TUid KUidOpx={KUidOpxValue};

class OplAPI;
class COpxModule;
class COpxBase : public CBase
	{
	friend class COpxModule;
public:
	IMPORT_C COpxBase(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum)=0;				// run extension procedure aProcNum
	virtual TBool CheckVersion(TInt aVersion) =0;	// check DECLARE OPX version is ok
protected:
	OplAPI& iOplAPI;
private:
	IMPORT_C void IncAccessCount();
	IMPORT_C TInt DecAccessCount();
private:
	TInt iAccessCount;
	};

// Exports required by the runtime
enum TOpxLibFuncs
	{
	EOpxCreate=1
	};

#endif

