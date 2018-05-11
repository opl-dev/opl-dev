// OT_UTL.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "e32std.h"
#include "ot_std.h"

/*
 Allow sensible translation-time casting of the OPL constants.
 
 Currently, the casting functions of OPLT for numerical 
 constants (i.e. TOplConstant::AsWordL() and such) work 
 in such a way that only the "smaller" types can be
 cast to "larger" types, i.e.
 1. Word and Long can be casted to Real.
 2. Word can be casted to Long.
 3. Anything else Leaves (and is handled by various traps).
 
 At the first glance this seems to be logical (and it is
 logical indeed), but sometimes this can become very awkward.
 For example the following list causes translation-time error
 reporting "Bad array size".
 
 CONST KSomeConstant&=300
 PROC Test:
        LOCAL a%(KSomeConstant&)
 ENDP
 
 Apart from the embarrasing error message (it's actually not
 the size of the array), since OPLT knows (on translation-time)
 the value of KSomeConstant&, there's little point in rejecting
 the declaration like this on the sole basis that  "Long can be
 larger than 32767".
 
 I've just added some sanity check in the casting functions
 to allow casting from larger types to smaller ONLY WHEN IT
 MAKES SENSE.
 
 I still haven't found any occasion where this does some
 harm.
 
 Note that this doesn't affect the run-time type/range
 checking.
 
 Regards,
 Keita http://www.hi-ho.ne.jp/~ktkawabe/densha_e.html 
*/

GLDEF_C void Panic(TOpltPanic aPanic)
//
// Does a Panic from withn the OPL translator
//	
	{
	_LIT(KOpltName,"OPLT");
	User::Panic(KOpltName,aPanic);
	}

GLDEF_C void SyntaxErrorL()
//
// Leaves with a syntax error
//
   {

   User::Leave(EErrSyntax);
   }

GLDEF_C TInt E32Dll(TDllReason /* aReason*/)
//
// DLL entry point
//
	{
	return KErrNone;
	}


GLDEF_C void TypeMismatchL()
//
// Just leaves with EErrTypeMismatch
//
	{
	User::Leave(EErrTypeMismatch);
	}
///////////////////////////////////////////////////
//
// TOplConstant
//
///////////////////////////////////////////////////
EXPORT_C TInt16 TOplConstant::AsWordL() const
//
// Returns the 'word' value of a constant
//
// If it's not 'word' but 'real' or 'long', tries to
// cast to 'word' in a sensible manner.
//
	{
	switch ( Type() )
		{
		case TOplToken::EReal:
			if (iReal<TReal64(KMinTInt16)||iReal>TReal64(KMaxTInt16))
				TypeMismatchL();
			return (TInt16)(iReal);
			break;
		case TOplToken::ELong:
			if (iInt<KMinTInt16||iInt>KMaxTInt16)
				TypeMismatchL();
			/* fall through */
		case TOplToken::EWord:
			return TInt16(iInt);
			break;
		default:
			TypeMismatchL();
			break;
		}
	return TInt16(0); //Can never be reached.
	}

EXPORT_C TInt32 TOplConstant::AsLongL() const
//
// Returns the 'long' value of a constant
//




// If it's not 'long' but 'real' or 'word', tries to
// cast to 'long' in a sensible manner.
//
	{
	switch ( Type() )
		{
		case TOplToken::ELong:
			return TInt32(iInt);
			break;
		case TOplToken::EReal:
			if (iReal<TReal64(KMinTInt32)||iReal>TReal64(KMaxTInt32))
				TypeMismatchL();
			else
				return (TInt32)(iReal);
			break;
		default:
			break;
		}
	return TInt32(AsWordL());
	}

EXPORT_C TReal64 TOplConstant::AsRealL() const
//
// Returns the value as a Real
//
	{
	
	TReal64 realVal;
	switch (Type())
		{
		default:
			TypeMismatchL();
		case TOplToken::EWord:
		case TOplToken::ELong:
			realVal=TReal64(iInt);
			break;
		case TOplToken::EReal:
			realVal=iReal;
			break;
		}
	return realVal;
	}

EXPORT_C const TDesC& TOplConstant::AsStringL() const
//
// Returns the string value
//
	{
	if (Type()!=TOplToken::EString)
		TypeMismatchL();
	return *iString;
	}

EXPORT_C void TOplConstant::NegateL()
//
// Negates the constant (if it's a number)
//
	{
	switch (Type())
		{
		default:
			TypeMismatchL();
		case TOplToken::EWord:
		case TOplToken::ELong:
			iInt=(-iInt);
			break;
		case TOplToken::EReal:
			iReal=(-iReal);
			break;
		}
	}

void TOplConstant::SetWord(TInt aWord)
	{
	iType=TOplToken::EWord;
	iInt=aWord;
	}

void TOplConstant::SetLong(TInt aLong)
	{
	iType=TOplToken::ELong;
	iInt=aLong;
	}

void TOplConstant::SetReal(TReal64 aReal)
	{
	iType=TOplToken::EReal;
	iReal=aReal;
	}

void TOplConstant::SetString(const TDesC& aDes)
	{
	iType=TOplToken::EString;
	iString=&aDes;
	}

EXPORT_C void TOplConstant::ExternalizeL(RWriteStream& aStream) const
	{
	
	RWriteStream& strm=aStream<<Type();
	switch (Type())
		{
		case TOplToken::EWord:
			strm<<TInt16(iInt);
			break;
		case TOplToken::ELong:
			strm<<TInt32(iInt);
			break;
		case TOplToken::EReal:
			strm<<TReal64(iReal);
			break;
		case TOplToken::EString:
			strm<<(*iString);
		default:
			break;
		}
	}

//////////////////////////////////////////////////////////////
//
// TOplField
//
/////////////////////////////////////////////////////////////
void TOplField::ExternalizeL(RWriteStream& aStream) const
	{
	
	aStream<<TOplField::TDevice(iDevice)<<iName;
	}

