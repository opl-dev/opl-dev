// OT_SYMBL.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Symbols & symbol table used by OPL translator

#ifndef __OT_SYMBL_H__
#define __OT_SYMBL_H__

////////////////////////////////////////////////////////////////////////
//
// COplSymbolTable
//
////////////////////////////////////////////////////////////////////////
const TInt KOplScopeGran=64; 
const TInt KOplHashTableSize=31; // must be of the form 2^n-1.

class COplSymbolTable : public CBase
//
//  Hash symbol table used in OPL
//
//
	{
public:
	static COplSymbolTable *NewL();
	~COplSymbolTable();
	void Reset(); // reset's all the data structures, deletes any symbols

	inline void StartScope();
	void EndScope();

	COplSymbol *Find(const TDesC& aName, TOplToken aToken);
	void AddL(COplSymbol& aSymbol);
   void Delete(COplSymbol *pSym);

private:
	COplSymbolTable();
   
	// Hashing 
	TDblQue<COplSymbol>& Hash(const TDesC& aName); // returns the right que within the table.
private:
	TInt iCurrentScope;
	TSglQue<COplSymbol> iScopeQue;
	TDblQue<COplSymbol> iTable[KOplHashTableSize]; // The hash table - a series of queues
	__DECLARE_TEST;
	};

#include "ot_symbl.inl"
#endif
