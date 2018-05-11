// RECOPL.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __RECOPL_H__
#define __RECOPL_H__

#if !defined(__APMREC_H__)
#include <apmrec.h>
#endif
//#include <apaflrec.h>

const TInt KOplRecUidValue=0x10000148;
const TUid KUidOplInterpreter=	{0x10005D2E};
const TUid KUidOplObj=			{0x100055C0};
const TUid KUidOplApp=			{0x100055C1};
const TUid KUidOplDoc=			{0x100055C2};
const TUid KUidOplRecogniser={KOplRecUidValue};


class CApaOplRecognizer : public CApaDataRecognizerType
	{
public: // from CApaDataRecognizerType
	CApaOplRecognizer();
	TUint PreferredBufSize();
	TDataType SupportedDataTypeL(TInt aIndex) const;
private: // from CApaDataRecognizerType
	void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);

//public:
//	enum TRuntimeFlags {ENone=0,EDebug=1,EDegrees=2};
//	TRecognizedType DoRecognizeFileL(RFs& aFs,TUidType aUidType);
//	TThreadId RunL(TApaCommand aCommand,const TDesC* aDocFileName=NULL,const TDesC8* aTailEnd=NULL) const;
	};

#endif