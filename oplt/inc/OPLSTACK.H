// OPLSTACK.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Extracted from OPLTDEF.H by HP, January 1997
// Translator stack classes - used by everything

#ifndef __OPLSTACK_H__
#define __OPLSTACK_H__

#include <e32base.h>

class CTranStackBase : public CArrayFixBase
	{
public:
	IMPORT_C void Drop();				   // Drops the top item from the stack
	inline TUint Depth() const; //Total depth of the stack - number of items.
protected:
	inline CTranStackBase(TBufRep aRep,TUint aLength,TUint aGranularity); // Avoids constructor warning
	inline void PushL(const TAny* aPtr); // Push an item on the stack
	inline void Pop(TAny* aPtr); 		   // Pop 'top' item off stack
	inline const TAny* Top() const;			   // What's at the Top of the stack
	IMPORT_C const TAny* Peek(TInt aDepth) const;	   // Peek at any depth, Peek(0)==Top()
	IMPORT_C void Pick(TAny* aPtr,TInt aDepth);       // Remove item from the middle of the stack
private:
	TUint Index(TInt aDepth) const;
	};

template <class T>
class CTranStack : public CTranStackBase
	{
public:
	inline void PushL(const T* aPtr);
	inline void Pop(T* aRef);
	inline T& Top();
	inline T& Peek(TInt aDepth);
	inline void Pick(T* aRef, TInt aDepth);
protected:
	inline CTranStack(TBufRep aRep, TUint aGranularity);
	};

template <class T>
class CTranStackFlat : public CTranStack<T>
	{
public:
	inline CTranStackFlat(TUint aGranularity);	
	};

template <class T>
class CTranStackSeg : public CTranStack<T>
	{
public:
	inline CTranStackSeg(TUint aGranularity);	
	};

#include <OPLstack.inl>
#endif
