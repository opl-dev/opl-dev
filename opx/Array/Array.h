// ARRAY.H
//
// Copyright (c) 2005 Arjen Broeze. All rights reserved.

#ifndef __ARRAY_H__
#define __ARRAY_H__

const TInt KArrayOpxVersion = 0x120;

const TInt KOplMaxStringLength=255;

enum TCompareType 
	{
	ECompareNormal=1,
	ECompareFolded,
	ECompareCollated
	};

enum TSortMode
	{
	ESortAscending=1,
	ESortDescending
	};

enum TDuplicates
	{
	EDuplicatesAllow=1,
	EDuplicatesIgnore,
	EDuplicatesError
	};

enum TFindPartial
	{
	EFindSubString=1,
	EFindMatch
	};

class TKeyArrayHBufPtr : public TKeyArrayFix
	{
	public:
	inline TKeyArrayHBufPtr(TInt aOffset, TKeyCmpText aType) 
		:TKeyArrayFix(aOffset, aType) {iHBufType=EFalse;}
	inline TKeyArrayHBufPtr(TInt aOffset, TKeyCmpText aType, TInt aLength) 
		:TKeyArrayFix(aOffset, aType, aLength) {iHBufType=EFalse;}
	inline TKeyArrayHBufPtr(TInt aOffset, TKeyCmpNumeric aType) 
		:TKeyArrayFix(aOffset, aType) {iHBufType=EFalse;}
	inline TKeyArrayHBufPtr(TInt aOffset)
		:TKeyArrayFix(aOffset, TKeyCmpText()) 
		{
		iHBufType=ETrue;
		iSortMode=ESortAscending;
		iCompareType=ECompareNormal;
		}

	virtual TAny* At(TInt aIndex) const;
	virtual TInt Compare(TInt aLeft,TInt aRight) const; 
	void SetCompareType(TCompareType aCompareType);
	void SetSortMode(TSortMode aSortMode);
	private:
	TBool iHBufType;
	TSortMode iSortMode;
	TCompareType iCompareType;
	};

class CElement : public CBase
	{
public :
	CElement();
	~CElement();
	void SetTextL(const TDesC& aText);
public :
	HBufC* iBuf;
	};

class CArray : public CBase
	{
public:
	CArray();
	~CArray();
	void ConstructL();
	inline CArrayPtrFlat<CElement>* Items() { return iItems; }

	void SetCompareType(TCompareType aCompareType);
	void SetSortMode(TSortMode aSortMode);
	void SetDuplicates(TDuplicates aDuplicates);
	void SetSorted(TBool aSorted, TBool aResortAlways=EFalse);

	TCompareType CompareType() { return iCompareType; }
	TSortMode SortMode() { return iSortMode; }
	TBool Sorted() { return iSorted; }
	TBool Duplicates() { return iDuplicates; }
private:
	CArrayPtrFlat<CElement>* iItems;
	TSortMode iSortMode;
	TCompareType iCompareType;
	TDuplicates iDuplicates;
	TBool iSorted;
	};


class CTlsDataOPXArray;
class CArrayOpx;

class CArrayOpx :public CBase
	{
public:
	CArrayOpx();
	~CArrayOpx();

	void ArrayNewL(OplAPI& aOplAPI) const;
	void ArrayFreeL(OplAPI& aOplAPI) const;
	void ArrayClearL(OplAPI& aOplAPI) const;
	void ArraySetDuplicatesL(OplAPI& aOplAPI) const;
	void ArrayGetDuplicatesL(OplAPI& aOplAPI) const;
	void ArraySetCompareTypeL(OplAPI& aOplAPI) const;
	void ArrayGetCompareTypeL(OplAPI& aOplAPI) const;
	void ArraySetSortModeL(OplAPI& aOplAPI) const;
	void ArrayGetSortModeL(OplAPI& aOplAPI) const;
	void ArraySetSortedL(OplAPI& aOplAPI) const;
	void ArrayGetSortedL(OplAPI& aOplAPI) const;
	void ArrayAddL(OplAPI& aOplAPI, TBool Int32) const;
	void ArrayAddSortedL(OplAPI& aOplAPI, TBool Int32) const;
	void ArrayInsertL(OplAPI& aOplAPI, TBool Int32) const;
	void ArrayReplaceL(OplAPI& aOplAPI, TBool Int32) const;
	void ArrayDeleteL(OplAPI& aOplAPI, TBool Int32) const;
	void ArraySizeL(OplAPI& aOplAPI, TBool Int32) const;
	void ArrayAtL(OplAPI& aOplAPI, TBool Int32) const;
	void ArrayFindL(OplAPI& aOplAPI, TBool Int32) const;
	void ArraySortL(OplAPI& aOplAPI) const;
	void ArrayFindPartialL(OplAPI& aOplAPI, TBool aInt32);
	};


class CTlsDataOPXArray : public COpxBase 
	{
public:
	static CTlsDataOPXArray* NewL(OplAPI& aOplAPI);
	void ConstructL();
	CTlsDataOPXArray(OplAPI& aOplAPI);
	~CTlsDataOPXArray() ;
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
	CArrayOpx* iArrayHandle;

	// the language extension procedures
	enum TExtensions
		{
		EArrayNew=1,
		EArrayFree,
		EArrayClear,
		
		EArraySetDuplicates,
		EArrayGetDuplicates,
		EArraySetCompareType,
		EArrayGetCompareType,
		EArraySetSortMode,
		EArrayGetSortMode,
		EArraySetSorted,
		EArrayGetSorted,

		EArrayAdd,
		EArrayInsert,
		EArrayReplace,
		EArrayDelete,
		EArraySize,
		EArrayAt,
		EArrayFind,
		EArraySort,
		EArraySearch,

		EArrayAddL,
		EArrayInsertL,
		EArrayReplaceL,
		EArrayDeleteL,
		EArraySizeL,
		EArrayAtL,
		EArrayFindL,
		EArraySearchL
		};

	};


inline CTlsDataOPXArray* TheTls() { return((CTlsDataOPXArray *)Dll::Tls()); }
inline void SetTheTls(CTlsDataOPXArray *theTls) { Dll::SetTls(theTls); }

#endif
