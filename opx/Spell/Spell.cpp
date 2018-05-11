// SPELL.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <e32base.h>
#include <oplapi.h>
#include <opx.h>

#include <s32file.h>
#if !defined(__UIQ__)
#include <lexicon.h>
#endif 
#include <oplerr.h>
#include <eikenv.h>

const TInt KUidOpxSpell=0x10000b92;
const TInt KOpxSpellVersion=0x600;

class COpxSpell : public COpxBase 
	{
public:
	static COpxSpell* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
	// the language extension procedures
	enum TExtensions
		{
		ESpell=1,
		EOpenSpeller,
		ECloseSpeller,
		ESpellUserDict,
		ESpellBuffer,
		ESpellAlternatives,
		ESpellWhere,
		ESpellWildcards,
		ESpellAnagrams
		};
	void Spell();
	void OpenSpeller();
	void CloseSpeller();
	void SpellUserDict() const;
	void SpellBuffer();
	void SpellAlternatives();
	void SpellWhere() const;
	void ExpandWildcards();
	void FindAnagrams();
private:
	void DoSpell(const TUint16* str, TInt size);
	void ConstructL();
	COpxSpell(OplAPI& aOplAPI);
	~COpxSpell();
private:
	RSpellClient iSpell;
	TInt iWhere;
	};

//
// The language extension procedures provided by this OPX
//
void COpxSpell::DoSpell(const TUint16* str, TInt size)
	{
	TInt32 i;
	TInt32 j;
	TBool isCorrect;
	TBuf<255> word;
	TInt error=0;
	for (i=0; i<size; )
		{
		if (TChar(str[i]).IsAlpha())
			{
			// Start of word, scan to end
			for (j = i; j < size && str[j] > ' ' && str[j] != 0xa0; j++);
				// j is now at the end of the word
			isCorrect = EFalse;
			if (j - i <= KSpellMaxWordLength)
				{
				word = TPtrC(str + i, j-i);
				error = iSpell.QuerySpelling(isCorrect, word);
				// User::LeaveIfError(error);
				// Ignore the return value, since it returns error when
				// checking the same word for the second time ???
				}
			else
				{
				word = TPtrC(str + i, ((j-i > 255) ? 255 : j-i));
				}
			if (!isCorrect)
				{
				iWhere = i + 1;
				iOplAPI.PushL(word);
				return;
				}
			i = j;
			}
		else
			{
			i++;
			}
		}
	iWhere = 0;
	iOplAPI.PushL(KNullDesC);
	}

void COpxSpell::Spell()
	{
	TBuf<255> str = iOplAPI.PopString();
	DoSpell(str.Ptr(), str.Length());
	}

void COpxSpell::SpellBuffer()
	{
	TInt size = iOplAPI.PopInt32();
	TInt addr = iOplAPI.PopInt32();
	TUint16* buf = (TUint16*)iOplAPI.OffsetToAddrL(addr, 4);
	DoSpell(buf, size);
	}

void COpxSpell::SpellWhere() const
	{
	iOplAPI.Push(TInt32(iWhere));
	}

void COpxSpell::SpellAlternatives()
	{
	TBufC<255> procedure = iOplAPI.PopString();
	TInt numAlt = iOplAPI.PopInt32();
	TBufC<255> aWord = iOplAPI.PopString();
	if (aWord.Length()<=KSpellMaxWordLength)
		{
		// Tell the Spell server what we want suggestions for
		iSpell.FindAlternatives(aWord);
		TSpellWord nextWord;
		while (numAlt-- > 0 && iSpell.FetchNextWord(nextWord)==KErrNone)
			{
			// Send word to OPL
			iOplAPI.InitCallbackL(procedure);
			iOplAPI.PushParamL(nextWord);
			User::LeaveIfError(iOplAPI.CallProcedure(EReturnLong));
			if (iOplAPI.PopInt32() != 0)
				break;
			}
		}
	iOplAPI.Push(0.0);
	}

void COpxSpell::ExpandWildcards()
	{
	TBufC<255> procedure = iOplAPI.PopString();
	TInt numAlt = iOplAPI.PopInt32();
	TBufC<255> aWord = iOplAPI.PopString();
	if (aWord.Length()<=KSpellMaxWordLength)
		{
		// Tell the Spell server what we want suggestions for
		iSpell.ExpandWildcards(aWord);
		TSpellWord nextWord;
		while (numAlt-- > 0 && iSpell.FetchNextWord(nextWord)==KErrNone)
			{
			// Send word to OPL
			iOplAPI.InitCallbackL(procedure);
			iOplAPI.PushParamL(nextWord);
			User::LeaveIfError(iOplAPI.CallProcedure(EReturnLong));
			if (iOplAPI.PopInt32() != 0)
				break;
			}
		}
	iOplAPI.Push(0.0);
	}

void COpxSpell::FindAnagrams()
	{
	TBufC<255> procedure = iOplAPI.PopString();
	TInt numAlt = iOplAPI.PopInt32();
	TInt minLen = iOplAPI.PopInt32();
	TBufC<255> aWord = iOplAPI.PopString();
	if (aWord.Length()<=KSpellMaxWordLength)
		{
		// Tell the Spell server what we want suggestions for
		iSpell.FindAnagrams(aWord, minLen);
		TSpellWord nextWord;
		while (numAlt-- > 0 && iSpell.FetchNextWord(nextWord)==KErrNone)
			{
			// Send word to OPL
			iOplAPI.InitCallbackL(procedure);
			iOplAPI.PushParamL(nextWord);
			User::LeaveIfError(iOplAPI.CallProcedure(EReturnLong));
			if (iOplAPI.PopInt32() != 0)
				break;
			}
		}
	iOplAPI.Push(0.0);
	}

void COpxSpell::OpenSpeller()
	{
	TBool loadUser = (iOplAPI.PopInt32() != 0);
	User::LeaveIfError(iSpell.Connect());
	User::LeaveIfError(iSpell.OpenSpeller());
	if (loadUser)
		{
		TFileName userDicName;
		if (iSpell.UserDictionaryName(userDicName)==KErrNotFound)
			{
			// User dic not open
			TInt error = iSpell.OpenUserDictionary(RSpellClient::DefaultUserDictionaryName());
			if (error != KErrNone)
				{
				iSpell.CreateUserDictionary();
				error = iSpell.SaveUserDictionaryAs(RSpellClient::DefaultUserDictionaryName(),EFalse);
				}
			}
		}
	iWhere = 0;
	iOplAPI.Push(0.0);
	}

void COpxSpell::CloseSpeller()
	{
	iSpell.CloseSpeller();
	iSpell.Close();
	iOplAPI.Push(0.0);
	}

void COpxSpell::SpellUserDict() const
	{
	// See if we should use the user dict
	CDictionaryStore* sysIni=CDictionaryFileStore::SystemLC(iOplAPI.EikonEnv().FsSession());
	TBool useUserDict=EFalse;
	if (sysIni->IsPresentL(KUidSystemUserDictionaryEnabled))
		{
		RDictionaryReadStream stream;
		stream.OpenLC(*sysIni,KUidSystemUserDictionaryEnabled);
		useUserDict = stream.ReadInt8L();
		CleanupStack::PopAndDestroy(); // stream
		}
	else
		useUserDict = ETrue;
	CleanupStack::PopAndDestroy(); // sysIni
	iOplAPI.Push(TInt32(useUserDict? 1 : 0));
	}

//
// The members of COpxSpell which are not language extension procedures
//
COpxSpell::COpxSpell(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	// __DECLARE_NAME(_S("COpxSpell"));
	}

COpxSpell* COpxSpell::NewLC(OplAPI& aOplAPI)
	{
	COpxSpell* This=new(ELeave) COpxSpell(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	return This;
	}

void COpxSpell::ConstructL()
	{
	// Do whatever is required to construct component members in COpxSpell
	} 

COpxSpell::~COpxSpell()
	{
	// Required so that Tls is set to zero on unloading the OPX in UNLOADM
	Dll::FreeTls();
	}

//
// COpxBase implementation
//
void COpxSpell::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case ESpell:
		Spell();
		break;
	case EOpenSpeller:
		OpenSpeller();
		break;
	case ECloseSpeller:
		CloseSpeller();
		break;
	case ESpellUserDict:
		SpellUserDict();
		break;
	case ESpellBuffer:
		SpellBuffer();
		break;
	case ESpellAlternatives:
		SpellAlternatives();
		break;
	case ESpellWhere:
		SpellWhere();
		break;
	case ESpellWildcards:
		ExpandWildcards();
		break;
	case ESpellAnagrams:
		FindAnagrams();
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxSpell::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xff00)>(KOpxSpellVersion & 0xff00))	// major version must be <= OPX's version
		return EFalse; // Bad version
	else
		return ETrue; // OK
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	COpxSpell* tls=((COpxSpell*)Dll::Tls());
	// tls is NULL on loading an OPX DLL (also after unloading it)
	if (tls==NULL)
		{
		tls=COpxSpell::NewLC(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop(); // tls
		}
	return (COpxBase*)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxSpellVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	//
	// DLL entry point
	//
	{
	return(KErrNone);
	}
