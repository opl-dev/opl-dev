// Eval.h
//
// Copyright (c) 2000 Symbian Ltd. All rights reserved.

#ifndef __EVAL_H__
#define __EVAL_H__

#include <opxapi.h>
#include <oplerr.h>

// This version number also needs changing in .pkg and .txh files.
const TInt KOpxVersion=0x100;
const TInt KUidOpxEval=0x77774EF5;

const TInt KOplMaxStringLength=255;

class COpxEvalWrapper : public CBase
	{
public:
	void EvalL(OplAPI& aOplAPI);

public:
	~COpxEvalWrapper();
private:
//	void EnsureConverterExistsL(OplAPI& aOplAPI);
public:
//	void EnsureArrayOfCharacterSetsAvailableExistsL(OplAPI& aOplAPI);
//	enum
//		{
//		EStateIsUnused=0
//		};
//	CCnvCharacterSetConverter* iConverter;
//	CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iArrayOfCharacterSetsAvailable;
//	TUint iCurrentCharacterSet;
//	TInt iNumberOfUnconvertibleCharacters;
//	TInt iIndexOfFirstUnconvertibleCharacter;
	};

class COpxEval : public COpxBase 
	{
public:
	static COpxEval* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
	COpxEval(OplAPI& aOplAPI);
	void ConstructL();
	~COpxEval();
private:
	enum TExtensions
		{
		EEval=1
		};
	COpxEvalWrapper* iOpxEvalWrapper;
	};

#endif