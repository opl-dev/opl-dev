// OPLTBAS.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// The public interface classes to the OPL translator objects.

#ifndef __OPLTBAS_H__
#define __OPLTBAS_H__

#include <e32base.h>
#include <texttran.h>
#include <opltdef.h>


// App callback. The owner is called back if the source file turns out
// to contain an APP...ENDA construct. Allows the owner to change the
// name of the output file.

class MTranAppCB
   {
public:
   virtual void OplTargetIsAppL(TFileName& aName)=0;
   };

//
//  COplTranslatorBase defines the interface used by OplEdit
//

class COplTranslatorBase : public CTextTranslator
	{
public:
	virtual void LocateErrorL(MTextSourceSystem& aTextSystem,const TFileName& aSourceName,const TFileName& aTargetName,CTextTranslator::TDebugFlag aDebugFlag,TSourceTranslateError& anError,TInt aLineNumber)=0;
	virtual void SetAppCB(MTranAppCB *anAppCB)=0;
	virtual void SetTarget(TOplTranTarget aTarget)=0;
	};


#endif