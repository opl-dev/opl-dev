// OPLDBG.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <e32base.h>

const TInt KUidOplDebuggerValue=0x10000269;
const TUid KUidOplDebugger={KUidOplDebuggerValue};

#ifndef __OPLR_H__
class COplRuntime;
#endif

class CDebuggerAPI : public CBase
	{
public:
	CDebuggerAPI(COplRuntime* aRuntime);
	// error handling related functions
	IMPORT_C TInt16 MapError(TInt aErrorCode);
	IMPORT_C void GetErrorMessage(TDes& aDes,TInt16 aError);
	// variable handling functions (for non-array vars use index==0)
	IMPORT_C TInt16 ExamineLocalInt16L(TInt aOffset,TInt aIndex) const;
	IMPORT_C TInt32 ExamineLocalInt32L(TInt aOffset,TInt aIndex) const;
	IMPORT_C TReal64 ExamineLocalReal64L(TInt aOffset,TInt aIndex) const;
	IMPORT_C TPtrC ExamineLocalStringL(TInt aOffset,TInt aIndex) const;

	IMPORT_C TInt16 ExamineGlobalInt16L(const TDesC& aName,TInt aIndex) const;
	IMPORT_C TInt32 ExamineGlobalInt32L(const TDesC& aName,TInt aIndex) const;
	IMPORT_C TReal64 ExamineGlobalReal64L(const TDesC& aName,TInt aIndex) const;
	IMPORT_C TPtrC ExamineGlobalStringL(const TDesC& aName,TInt aIndex) const;

	IMPORT_C void SetLocalInt16L(TInt aOffset,TInt aIndex,TInt16 aValue);
	IMPORT_C void SetLocalInt32L(TInt aOffset,TInt aIndex,TInt32 aValue);
	IMPORT_C void SetLocalReal64L(TInt aOffset,TInt aIndex,TReal64 aValue);
	IMPORT_C void SetLocalStringL(TInt aOffset,TInt aIndex,const TDesC& aValue);

	IMPORT_C void SetGlobalInt16L(const TDesC& aName,TInt aIndex,TInt16 aValue);
	IMPORT_C void SetGlobalInt32L(const TDesC& aName,TInt aIndex,TInt32 aValue);
	IMPORT_C void SetGlobalReal64L(const TDesC& aName,TInt aIndex,TReal64 aValue);
	IMPORT_C void SetGlobalStringL(const TDesC& aName,TInt aIndex,const TDesC& aValue);

private:
	TAny* GetLocalPtrL(TInt aOffset,TInt aIndex) const;
	TAny* GetGlobalPtrL(const TDesC& aName,TInt aType) const;
	COplRuntime* iRuntime;
	};




//
// Debugger base class
//

class CDebuggerBase : public CBase
	{
public:
	IMPORT_C CDebuggerBase(CDebuggerAPI* aDebuggerAPI);
	virtual void FrameLoad(const TDesC& aProcName,const TDesC& aModuleName,const TDesC& aSourceName)=0;
	virtual void FrameUnload()=0; // always matches the last frame load
	virtual void NextStatement(TInt aStatementNo)=0; // NB may become offset in source file
	virtual void ErrorOccurred(TInt aErrorCode)=0;
protected:
	CDebuggerAPI* iDebuggerAPI;
	};

#endif

