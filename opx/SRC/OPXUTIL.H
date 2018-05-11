#ifndef __OPXUTIL_H__
#define __OPXUTIL_H__

#include <e32def.h>

/**
 * The OpxUtil class provides some static functions of use to OPX programmers
 */
class OpxUtil
	{
public:
	static inline TInt16 OplBool16(const TBool aTBool);
	static inline TInt32 OplBool32(const TBool aTBool);
	static inline TBool CppBool(const TInt anOplBool);
	static inline TInt OplIndex(const TInt aCppIndex);
	static inline TInt CppIndex(const TInt anOplIndex);
	};

/**
 * Converts a boolean value to the OPL boolean format.
 */
inline TInt16 OpxUtil::OplBool16(const TBool aTBool)
	{
	return (TInt16(aTBool? -1 : 0));
	}

inline TInt32 OpxUtil::OplBool32(const TBool aTBool)
	{
	return (TInt16(aTBool? -1 : 0));
	}
/**
 * Converts a boolean value to the CPP boolean format.
 */
inline TBool OpxUtil::CppBool(const TInt anOplBool)
	{
	// Used an if here rather than a conditional assignment to avoid
	// a gcc "mismatched enums" error.
	if (anOplBool==0)
		return EFalse;
	else
		return ETrue;
	}

/**
 * Converts a CPP index (starts at 0) to an OPL index (starts at 1). 
 */
inline TInt OpxUtil::OplIndex(const TInt aCppIndex)
	{
	return (aCppIndex+1);
	}

/**
 * Converts an OPL index (starts at 1) to a CPP index (starts at 0). 
 */
inline TInt OpxUtil::CppIndex(const TInt anOplIndex)
	{
	return (anOplIndex-1);
	}


#endif // __OPXUTIL_H__
