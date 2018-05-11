// OPLRDEF.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __OPLRDEF_H__
#define __OPLRDEF_H__

#include <e32std.h>
const TInt KOplTypeArray=0x80;
enum TOplType
	{
	EBadType=(-1),
	EWord,
	ELong,
	EReal,
	EString,
	EWordArray=EWord|KOplTypeArray,
	ELongArray=ELong|KOplTypeArray,
	ERealArray=EReal|KOplTypeArray,
	EStringArray=EString|KOplTypeArray
    };

#endif

