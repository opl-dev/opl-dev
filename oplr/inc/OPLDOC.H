// OPLDOC.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#ifndef __OPLDOC_H__
#define __OPLDOC_H__

#include <e32std.h>
#include <s32file.h>

class TOplDocRootStream
// This will be stored as the root stream
// The shell needs the Uid of the runtime to be 1st
// The following streamid is eg. the DBMS database head stream
	{
public:
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C void InternalizeL(RReadStream& aStream);
public:
	TUid iAppUid; // usually KUidOplInterpreter (but may want to support other?)
	TStreamId iStreamId;
	};

#endif  // __OPLDOC_H__
/* End of $Workfile:   OPLDOC.H  $ */
  

                                
