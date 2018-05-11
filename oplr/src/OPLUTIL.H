// OPLUTIL.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef OPLUTIL_H
#define OPLUTIL_H

#include <e32std.h>
class TAlgStyle;

const TInt KOplAlignment=1;

class OplUtil
	{
public:
	// these are for getting and putting values to non-aligned addresses
	static void PutLong(TAny* aPtr,TInt32 aLong);
	static TInt32 GetLong(TAny* aPtr);
	static void PutFloat(TAny* aPtr,TReal64 aFloat);
	static TReal64 GetFloat(TAny* aPtr);
	static void PutWord(TAny* aPtr,TInt16 aWord);
	static TInt16 GetWord(TAny* aPtr);
	static TUint8* OffsetToAddrL(const RHeap& aHeap,TInt aOffset,TInt aSize);
	static TUint8* PopOffsetAsAddrL(CStack& aStack,const RHeap& aHeap,TInt aSize);
	static TInt16 MapError(TInt aE32Error);
	static TInt GetErrResource(TInt aError);
	// arithmetic with overflow checking
	static TInt16 MultiplyL(TInt16 aArg1,TInt16 aArg2);
	static TInt32 MultiplyL(TInt32 aArg1,TInt32 aArg2);
	// mapping of font ids
	static TUid MapFontId(TInt aFontId,TAlgStyle& aStyle);
#if defined(__SERIES60__)||defined(__UIQ__)
	// True if one of the standard OS 6.1 fonts. e.g. DenseFont
	static TBool IsStandardFontUid(const TUid aFontId);
	static const CFont* MapFontUidToStandardFontL(const TUid aFontId);
#endif
	// mapping opl1993-type errors.
	static TInt16 ErrorEraToOpl1993(TInt aE32Error);
#ifndef __USING_OPL1993UTIL__
private:
	static const TInt16 E16Errors[];
#endif
	};

#endif
