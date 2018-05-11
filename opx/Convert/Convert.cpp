// Convert.cpp
//
// Copyright (c) 1999-2000 Symbian Ltd. All rights reserved.

#include "convert.h"
#include <APGTASK.H>
#include <ES_SOCK.H>
#include <eikenv.h>

COpxCharConvWrapper::~COpxCharConvWrapper()
	{
	delete iConverter;
	delete iArrayOfCharacterSetsAvailable;
	}

void COpxCharConvWrapper::CharSetsL(OplAPI& aOplAPI)
	{
	// OPL call: characterSets=CVNumberOfCharacterSets&:
	EnsureArrayOfCharacterSetsAvailableExistsL(aOplAPI);
	aOplAPI.Push(TInt32(iArrayOfCharacterSetsAvailable->Count()));
	}

void COpxCharConvWrapper::CharSetNameL(OplAPI& aOplAPI)
	{
	// OPL call: characterSetName$ = CVCharacterSetName$:(aIndex&)
	TInt iIndexFromZero=aOplAPI.PopInt32()-1;
	EnsureArrayOfCharacterSetsAvailableExistsL(aOplAPI);
	if (iArrayOfCharacterSetsAvailable->At(iIndexFromZero).NameIsFileName())
		{
		TParsePtrC parser(iArrayOfCharacterSetsAvailable->At(iIndexFromZero).Name());
		aOplAPI.PushL(parser.Name());
		}
	else
		{
		aOplAPI.PushL(iArrayOfCharacterSetsAvailable->At(iIndexFromZero).Name());	
		}
	}

void COpxCharConvWrapper::CharSetUidL(OplAPI& aOplAPI)
	{
	// OPL call: characterSetUid& = CVCharacterSetUid&:(aIndex&)
	TUint32 iIndexFromZero=aOplAPI.PopInt32()-1;
	EnsureArrayOfCharacterSetsAvailableExistsL(aOplAPI);
	aOplAPI.Push(TInt32(iArrayOfCharacterSetsAvailable->At(iIndexFromZero).Identifier()));	
	}

void COpxCharConvWrapper::SelectCharSetL(OplAPI& aOplAPI)
	{
	// OPL call: CVSelectCharacterSet:(aCharacterSetUid&)
	TUint32 requestedCharacterSet=aOplAPI.PopInt32();
	EnsureConverterExistsL(aOplAPI);
	EnsureArrayOfCharacterSetsAvailableExistsL(aOplAPI);
	if (iCurrentCharacterSet!=requestedCharacterSet)
		{
		TBool invalid=ETrue;
		for (TInt i=0;i<iArrayOfCharacterSetsAvailable->Count();i++)
			if (requestedCharacterSet==iArrayOfCharacterSetsAvailable->At(i).Identifier())
				invalid=EFalse;
		if (invalid)
			User::Leave(KOplErrInvalidArgs);
  		iConverter->PrepareToConvertToOrFromL(requestedCharacterSet, 
			*iArrayOfCharacterSetsAvailable, aOplAPI.EikonEnv().FsSession());
		iCurrentCharacterSet=requestedCharacterSet;
		}
	aOplAPI.Push(0.0);
	}

void COpxCharConvWrapper::ResetToDefaults(OplAPI& aOplAPI)
	{
	// OPL call: CVResetToDefaults:
	delete iConverter;
	iConverter=NULL;
	delete iArrayOfCharacterSetsAvailable;
	iArrayOfCharacterSetsAvailable=NULL;
	iCurrentCharacterSet=0;
	aOplAPI.Push(0.0);
	}

void COpxCharConvWrapper::FromUnicodeL(OplAPI& aOplAPI)
	{
	// OPL call: bytesInBuffer = CVFromUnicode:(aBuffer&,aBufferLength&,aSource$)
	TPtrC source=aOplAPI.PopString();
	TUint32 bufferLength=aOplAPI.PopInt32();
	TUint32 bufferStart=aOplAPI.PopInt32();
	TPtr8 buffer(aOplAPI.OffsetToAddrL(bufferStart, bufferLength),bufferLength);
	EnsureConverterExistsL(aOplAPI);
	TInt err=iConverter->ConvertFromUnicode(
		buffer,source,
		iNumberOfUnconvertibleCharacters,
		iIndexOfFirstUnconvertibleCharacter);
	if (err)
		User::Leave(KOplErrInvalidArgs);
	aOplAPI.Push(0.0);
	}

void COpxCharConvWrapper::UnicodeL(OplAPI& aOplAPI)
	{
	// OPL call: destination$ = CVToUnicode:(aBuffer&,aBufferLength&)
	TUint32 bufferLength=aOplAPI.PopInt32();
	TUint32 bufferStart=aOplAPI.PopInt32();
	TBuf<KOplMaxStringLength> destination;
	TPtrC8 buffer(aOplAPI.OffsetToAddrL(bufferStart,bufferLength),bufferLength);
	TInt state=CCnvCharacterSetConverter::KStateDefault;
	EnsureConverterExistsL(aOplAPI);
	TInt err=iConverter->ConvertToUnicode(
		destination,buffer,
		state,
		iNumberOfUnconvertibleCharacters,
		iIndexOfFirstUnconvertibleCharacter);
	if (err)
		User::Leave(KOplErrInvalidArgs);
	aOplAPI.PushL(destination);
	}

void COpxCharConvWrapper::BadCharCount(OplAPI& aOplAPI)
	{
	// OPL Call: CVNumberOfUnconvertibleCharacters&:
	aOplAPI.Push(TInt32(iNumberOfUnconvertibleCharacters));
	}

void COpxCharConvWrapper::FirstBadChar(OplAPI& aOplAPI)
	{
	// OPL call: CVIndexOfFirstUnconvertibleCharacter&:
	aOplAPI.Push(TInt32(iIndexOfFirstUnconvertibleCharacter+1));
	}

void COpxCharConvWrapper::SetReplaceChar(OplAPI& aOplAPI)
	{
	// OPL Call: CVSetReplacementForUnknownCharacters:(aReplacementCharacter$)
	EnsureConverterExistsL(aOplAPI);
	TBuf8<1> replacementForUnconvertibleUnicodeCharacters;
	replacementForUnconvertibleUnicodeCharacters.Append(STATIC_CAST(TUint8,aOplAPI.PopInt32()));
	iConverter->SetReplacementForUnconvertibleUnicodeCharactersL(replacementForUnconvertibleUnicodeCharacters);
	aOplAPI.Push(0.0);
	}

void COpxCharConvWrapper::SetLittleEndianL(OplAPI& aOplAPI)
	{
	EnsureConverterExistsL(aOplAPI);
	iConverter->SetDefaultEndiannessOfForeignCharacters(CCnvCharacterSetConverter::ELittleEndian);
	aOplAPI.Push(0.0);
	}

void COpxCharConvWrapper::SetBigEndianL(OplAPI& aOplAPI)
	{
	EnsureConverterExistsL(aOplAPI);
	iConverter->SetDefaultEndiannessOfForeignCharacters(CCnvCharacterSetConverter::EBigEndian);
	aOplAPI.Push(0.0);
	}

void COpxCharConvWrapper::EnsureConverterExistsL(OplAPI& aOplAPI)
	{
	if (!iConverter)
		{
		iConverter=CCnvCharacterSetConverter::NewL();
		iCurrentCharacterSet=KCharacterSetIdentifierAscii;
		iConverter->PrepareToConvertToOrFromL(iCurrentCharacterSet,aOplAPI.EikonEnv().FsSession());
		}
	}

void COpxCharConvWrapper::EnsureArrayOfCharacterSetsAvailableExistsL(OplAPI& aOplAPI)
	{
	if (!iArrayOfCharacterSetsAvailable)
		{
 		iArrayOfCharacterSetsAvailable=CCnvCharacterSetConverter::
			CreateArrayOfCharacterSetsAvailableL(aOplAPI.EikonEnv().FsSession());
		}
	}

COpxCharConv::COpxCharConv(OplAPI& aOplAPI) 
	: COpxBase(aOplAPI)
    {
    }

COpxCharConv* COpxCharConv::NewLC(OplAPI& aOplAPI)
    {
    COpxCharConv* This=new(ELeave) COpxCharConv(aOplAPI);
    CleanupStack::PushL(This);
    This->ConstructL();
    return This;
    }

void COpxCharConv::ConstructL()
    {
	iOpxCharConvWrapper=new(ELeave) COpxCharConvWrapper;
    } 

COpxCharConv::~COpxCharConv()
    {
	delete iOpxCharConvWrapper;
	Dll::FreeTls();
	}							  

void COpxCharConv::RunL(TInt aProcNum)
	{
	switch (aProcNum)
		{
	case ECharSets:
		iOpxCharConvWrapper->CharSetsL(iOplAPI);
		break;
	case ECharSetName:
		iOpxCharConvWrapper->CharSetNameL(iOplAPI);
		break;
	case ECharSetUid:
		iOpxCharConvWrapper->CharSetUidL(iOplAPI);
		break;
	case ESelectCharSet:
		iOpxCharConvWrapper->SelectCharSetL(iOplAPI);
		break;
	case EResetToDefaults:
		iOpxCharConvWrapper->ResetToDefaults(iOplAPI);
		break;
	case EFromUnicode:
		iOpxCharConvWrapper->FromUnicodeL(iOplAPI);
		break;
	case EUnicode:
		iOpxCharConvWrapper->UnicodeL(iOplAPI);
		break;
	case EBadCharCount:
		iOpxCharConvWrapper->BadCharCount(iOplAPI);
		break;
	case EFirstBadChar:
		iOpxCharConvWrapper->FirstBadChar(iOplAPI);
		break;
	case ESetReplaceChar:
		iOpxCharConvWrapper->SetReplaceChar(iOplAPI);
		break;
	case ESetLittleEndian:
		iOpxCharConvWrapper->SetLittleEndianL(iOplAPI);
		break;
	case ESetBigEndian:
		iOpxCharConvWrapper->SetBigEndianL(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxCharConv::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	{
	COpxCharConv* tls=((COpxCharConv*)Dll::Tls());
	if (tls==NULL)    // tls is NULL on loading an OPX DLL (also after unloading it)
		{
        tls=COpxCharConv::NewLC(aOplAPI);
	    User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop();    // tls
        }
    return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}