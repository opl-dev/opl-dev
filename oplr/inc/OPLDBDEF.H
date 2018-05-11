// OPLDBDEF.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#ifndef __OPLDBDEF_H__
#define __OPLDBDEF_H__

#include <d32dbms.h>

const TUid KUidDataApp={0x10003A61}; // KUidDataApp16 
const TUid KUidDataHeadStream={0x1000012E};

const TInt KOplMaxStringFieldLength=255;
//
const TInt KOplMaxTableName=KDbMaxName;
const TInt KOplMaxFieldName=KDbMaxColName;
const TInt KMaxLogName=26;			// A to Z
const TInt KBookmarkGranularity=3;	

#endif // __OPLDBDEF_H__