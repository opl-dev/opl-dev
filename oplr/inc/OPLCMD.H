// OPLCMD.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __OPLCMD_H__
#define __OPLCMD_H__

#include <apadef.h>
#include <oplflg.h>
#include <program.h>

class COplCommandLine : public CBase
	{
public:
	~COplCommandLine();
	void CrackCommandLineL(const TDesC8& aCmd);
	TDesC& ModuleName() const {return *iModuleName;}
	TDesC& DocumentName() const {return *iDocumentName;}
	TInt Command() const {return iCommand;}
	const TUid& AppUid() const {return iAppUid;}
	void SetAppUid(const TUid aUid) {iAppUid=aUid;}
	TChar TailEndCommand() const {return iTailEndCommand;}
	void SaveCommandLineParametersL(TApaCommand aCommand,TFileName& aDocumentName, const TDesC8& aTail);
	TRuntimeParams& RuntimeParams() {return iRuntimeParams;}
private:
	TUid iAppUid;
	HBufC* iModuleName;
	HBufC* iDocumentName;
	TChar iCommand;
	TRuntimeParams iRuntimeParams;	
	TChar iTailEndCommand;
	};

#endif
