// TDEBUG.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#ifndef TDEBUG_H
#define TDEBUG_H

#include <opldbg.h>

class CTestDebugger : public CDebuggerBase
	{
public:
	CTestDebugger(CDebuggerAPI* aDebuggerAPI);
	void FrameLoad(const TDesC& aProcName,const TDesC& aModuleName,const TDesC& aSourceName);
	void FrameUnload(); // always matches the last frame load
	void NextStatement(TInt aStatementNo); // for testing the statement no determines what happens
	void ErrorOccurred(TInt aErrorCode);
private:
	TInt iFrameCount;
	};

#endif
