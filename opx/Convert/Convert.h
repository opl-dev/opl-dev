// Convert.h
//
// Copyright (c) 1999-2000 Symbian Ltd. All rights reserved.

#ifndef __CVOPX_H__
#define __CVOPX_H__

#include <e32base.h>
#include <f32file.h>
#include <opxapi.h>
#include <oplerr.h>            
#include "charconv.h"

// This version number also needs changing in charcnv*.pkg and charcnv.txh
const TInt KOpxVersion=0x100;
const TInt KUidOpxCharCnv=0x10004EF5;

const TInt KOplMaxStringLength=255;

class COpxCharConvWrapper : public CBase
	{
public:
	void CharSetsL(OplAPI& aOplAPI);
	void CharSetNameL(OplAPI& aOplAPI);
	void CharSetUidL(OplAPI& aOplAPI);
	void SelectCharSetL(OplAPI& aOplAPI);
	void ResetToDefaults(OplAPI& aOplAPI);
	void FromUnicodeL(OplAPI& aOplAPI);
	void UnicodeL(OplAPI& aOplAPI);
	void BadCharCount(OplAPI& aOplAPI);
	void FirstBadChar(OplAPI& aOplAPI);
	void SetReplaceChar(OplAPI& aOplAPI);
	void SetLittleEndianL(OplAPI& aOplAPI);
	void SetBigEndianL(OplAPI& aOplAPI);
public:
	~COpxCharConvWrapper();
private:
	void EnsureConverterExistsL(OplAPI& aOplAPI);
public:
	void EnsureArrayOfCharacterSetsAvailableExistsL(OplAPI& aOplAPI);
	enum
		{
		EStateIsUnused=0
		};
	CCnvCharacterSetConverter* iConverter;
	CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iArrayOfCharacterSetsAvailable;
	TUint iCurrentCharacterSet;
	TInt iNumberOfUnconvertibleCharacters;
	TInt iIndexOfFirstUnconvertibleCharacter;
	};

class COpxCharConv : public COpxBase 
    {
public:
    static COpxCharConv* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
    COpxCharConv(OplAPI& aOplAPI);
	void ConstructL();
    ~COpxCharConv();
private:
	enum TExtensions
		{
		ECharSets=1,
		ECharSetName,
		ECharSetUid,
		ESelectCharSet,
		EResetToDefaults,
		EFromUnicode,
		EUnicode,
		EBadCharCount,
		EFirstBadChar,
		ESetReplaceChar,
		ESetLittleEndian,
		ESetBigEndian
		};
	COpxCharConvWrapper* iOpxCharConvWrapper;
    };
   
#endif