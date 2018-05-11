// UT_FILE.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <oplr.h>
#include <f32file.h>
#include "oplutil.h"

enum TWildCardFlags 
	{ENoWildCards=0,EWildCardInFile=1,EWildCardInExt=2,EWildCardInBoth=3};
 
class TOplParseOffset
	{
public:
	TInt16 iFilSysOff;
	TInt16 iDeviceOff;
	TInt16 iPathOff;
	TInt16 iNameOff;
	TInt16 iExtOff;
	TInt16 iWildCardFlags;
	};

GLDEF_C void ut_parse(COplRuntime& aRuntime,TPtrC aBase,TPtrC aRel,
					  TDes& aParsedName,TOplParseOffset& aOffset)
	{ 
	TParse parser;
	RFs& iFsClient=aRuntime.ConEnv()->FsSession();
	User::LeaveIfError(iFsClient.Parse(aBase,aRel,parser));
	aParsedName = parser.FullName();
	OplUtil::PutWord(&aOffset.iFilSysOff,1);
	OplUtil::PutWord(&aOffset.iDeviceOff,1);
	OplUtil::PutWord(&aOffset.iPathOff,(TInt16)(parser.Drive().Length()+OplUtil::GetWord(&aOffset.iDeviceOff)));
	OplUtil::PutWord(&aOffset.iNameOff,(TInt16)(OplUtil::GetWord(&aOffset.iPathOff)+parser.Path().Length()));
	OplUtil::PutWord(&aOffset.iExtOff,(TInt16)(OplUtil::GetWord(&aOffset.iNameOff)+parser.Name().Length()));
	OplUtil::PutWord(&aOffset.iWildCardFlags,ENoWildCards);
	if (parser.IsNameWild())
		OplUtil::PutWord(&aOffset.iWildCardFlags,(TInt16)(OplUtil::GetWord(&aOffset.iWildCardFlags)|EWildCardInFile));
	if (parser.IsExtWild())
		OplUtil::PutWord(&aOffset.iWildCardFlags,(TInt16)(OplUtil::GetWord(&aOffset.iWildCardFlags)|EWildCardInExt));
	} 

