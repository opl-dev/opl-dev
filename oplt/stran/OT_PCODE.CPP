// OT_PCODE.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Translator internal parse code routines.
// Routines accessing the internal parse code.

#include "ot_std.h"

/////////////////////////////////////////////////////////////////
//
// TOplPcode
//
/////////////////////////////////////////////////////////////////
void TPcode::ExternalizeL(RWriteStream& aStream) const
//
// Writes out a Pcode to the stream
//
	{

	if (iOffset<=KMaxTUint8)
		{
		aStream.WriteUint8L(iCode|EOffset8Bits);
		aStream.WriteUint8L(iOffset);
		}
	else if (iOffset<=KMaxTUint16)
		{
		aStream.WriteUint8L(iCode|EOffset16Bits);
		aStream.WriteInt16L(iOffset);
		}
	else
		{
		aStream.WriteUint8L(iCode|EOffset32Bits);
		aStream.WriteUint32L(iOffset);
		}
	}

EXPORT_C void TPcode::InternalizeL(RReadStream& aStream)
//
// Gets that sucker back
//
	{
	TUint code=aStream.ReadUint8L();
	iCode=TPcode::TCode(code&KOplPcodeTCodeMask);
	switch (TOffsetSize(code&KOplPcodeSizeMask))
		{
		case EOffset8Bits:
			iOffset=aStream.ReadUint8L();
			break;
		case EOffset16Bits:
			iOffset=aStream.ReadUint16L();
			break;
		case EOffset32Bits:
			iOffset=aStream.ReadUint32L();
			break;
		}
	}

/////////////////////////////////////////////////////////////////////
   //
// CPcodeConstant
//
/////////////////////////////////////////////////////////////////////
EXPORT_C CPcodeConstant *CPcodeConstant::NewLC(RReadStream& aStream)
//
// Reads a constant out of the stream
//
	{
	CPcodeConstant *pC=new(ELeave) CPcodeConstant;
	CleanupStack::PushL(pC);
	aStream>>pC->iType;
	switch (pC->iType)
		{
		case TOplToken::EWord:
			pC->iInt=aStream.ReadInt16L();
			break;
		case TOplToken::ELong:
			pC->iInt=aStream.ReadInt32L();
			break;
		case TOplToken::EReal:
			pC->iReal=aStream.ReadReal64L();
			break;
		case TOplToken::EString:
			pC->iString=HBufC::NewL(aStream,KOplMaxStringLength);
         break;
      default:
         Panic(EOpltReaderUnknownType);
		}
	return pC;
	}

EXPORT_C CPcodeConstant::~CPcodeConstant()
//
// Deletes the string
// 
	{
	if (iType==TOplToken::EString)
		delete iString;
	}

////////////////////////////////////////////////////////////////////
//
// TPcodeSymbolReference
//
///////////////////////////////////////////////////////////////////
EXPORT_C void TPcodeSymbolReference::InternalizeL(RReadStream& aStream)
//
// gets it out of the Pcode
//
	{
	
	aStream>>iSymbol>>iSide;
	}

///////////////////////////////////////////////////////////////////
//
// CPcodeFieldReference
//
///////////////////////////////////////////////////////////////////
EXPORT_C CPcodeFieldReference* CPcodeFieldReference::NewLC(RReadStream& aStream)
//
//
//
	{
	CPcodeFieldReference* field=new(ELeave) CPcodeFieldReference();
	CleanupStack::PushL(field);
	aStream>>field->iDevice;
	field->iName=HBufC::NewL(aStream,KOplMaxIdentifierLength);
	aStream>>field->iType>>field->iSide;
	return field;
	}

EXPORT_C CPcodeFieldReference::~CPcodeFieldReference()
//
//
//
	{
	delete iName;	
	}

/////////////////////////////////////////////////////////////
//
// TPcodeProcByName
//
/////////////////////////////////////////////////////////////
EXPORT_C void TPcodeProcByName::InternalizeL(RReadStream& aStream)
//
// 
//
	{

	iArgumentCount=(aStream>>iType).ReadUint8L();
	}

/////////////////////////////////////////////////////////////
//
// TPcodeFunctionCall
//
/////////////////////////////////////////////////////////////
EXPORT_C void TPcodeFunctionCall::InternalizeL(RReadStream& aStream)
//
// 
//
	{

	iArgumentCount=aStream.ReadUint8L();
	aStream>>iType>>iNumber;
	}

/////////////////////////////////////////////////////////////
//
// TPcodeOperator
//
/////////////////////////////////////////////////////////////
EXPORT_C void TPcodeOperator::InternalizeL(RReadStream& aStream)
//
// 
//
	{

	aStream>>iOperandType>>iResultingType>>iArity>>iOper;
	}

/////////////////////////////////////////////////////////////
//
// TPcodeTypedCommand
//
/////////////////////////////////////////////////////////////
EXPORT_C void TPcodeTypedCommand::InternalizeL(RReadStream& aStream)
//
// 
//
	{

	TPcodeCommand::InternalizeL(aStream);
	aStream>>iType;
	}

/////////////////////////////////////////////////////////////
//
// TPcodeQualifiedCommand
//
/////////////////////////////////////////////////////////////

EXPORT_C void TPcodeQualifiedCommand::InternalizeL(RReadStream& aStream)
//
// 
//
	{

	TPcodeCommand::InternalizeL(aStream);
	iQualifier=aStream.ReadUint8L();
	}

/////////////////////////////////////////////////////////////
//
// CPcodeFieldName - field declaration as part of OPEN/CREATE etc
//
/////////////////////////////////////////////////////////////

EXPORT_C CPcodeFieldName *CPcodeFieldName::NewLC(RReadStream& aStream)
//
//
//
	{
	CPcodeFieldName* field=new(ELeave) CPcodeFieldName;
	CleanupStack::PushL(field);
	aStream>>field->iType;
	field->iName=HBufC::NewL(aStream,KOplMaxIdentifierLength);
	return field;
	}

EXPORT_C CPcodeFieldName::~CPcodeFieldName()
	{
	delete iName;
	}
