// TDEBUG.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include "tdebug.h"
#include <e32svr.h>

CTestDebugger::CTestDebugger(CDebuggerAPI* aDebuggerAPI)
: CDebuggerBase(aDebuggerAPI)
	{
	}


void CTestDebugger::FrameLoad(const TDesC& aProcName,const TDesC& aModuleName,const TDesC& /*aSourceName*/)
	{
#if defined(_DEBUG)
	RDebug::Print(_L("%S\\%S - Frame No %d"),&aModuleName,&aProcName,++iFrameCount);
#endif
	}

void CTestDebugger::FrameUnload()
	{
#if defined(_DEBUG)
	RDebug::Print(_L("Unloaded Frame No %d"),iFrameCount--);
#endif
	}

void CTestDebugger::NextStatement(TInt aStatementNo)
	{
#if defined(_DEBUG)
	RDebug::Print(_L("Statement number %d"),aStatementNo);
#endif
	}

void CTestDebugger::ErrorOccurred(TInt aErrorCode)
	{
	TInt16 err=iDebuggerAPI->MapError(aErrorCode);
	TBuf<32> buf=_L("Error Occurred - ");
	buf.AppendNum(err);
	TBuf<256> buf2;
	iDebuggerAPI->GetErrorMessage(buf2,err);
#if defined(_DEBUG)
	RDebug::Print(_L("%S %S"),&buf,&buf2);
#endif
	}

EXPORT_C CDebuggerBase* NewDebuggerL(CDebuggerAPI* aDebuggerAPI)
	{
	return new(ELeave) CTestDebugger(aDebuggerAPI);
	}

GLDEF_C TInt E32Dll(TDllReason /* aReason */)
	{
	return(KErrNone);
	}
