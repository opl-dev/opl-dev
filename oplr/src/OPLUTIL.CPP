// OPLUTIL.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <bldvariant.hrh>
#if defined(__SERIES60__)
#include "aknutils.h"
#else
#include <fontids.h>
#endif

#include <fntstore.h>
#include <oplerr.h>
#include <oplstack.h>
#include <oplr.rsg>
#include "oplutil.h"
#include "program.h"
#include "oplr.h"

_LIT(KDebuggerFile,"\\System\\Libs\\Opldbg.dll");
_LIT(KOplRscFile,"\\System\\Data\\Oplr.rsc");

EXPORT_C TPtrC DebuggerDllFilename()
	{
	return KDebuggerFile();
	}

EXPORT_C TFileName RuntimeResourceFile()
	{
	return KOplRscFile();
	}

void OplUtil::PutWord(TAny* aPtr,TInt16 aWord)
	{
	TUint8* trg=(TUint8*)aPtr;
	TUint8* src=(TUint8*)&aWord;
	*trg++=*src++;
	*trg++=*src++;
	}

TInt16 OplUtil::GetWord(TAny* aPtr)
	{
	TUint8* src=(TUint8*)aPtr;
	TInt16 theWord;
	TUint8* trg=(TUint8*)&theWord;
	*trg++=*src++;
	*trg++=*src++;
	return theWord;
	}

void OplUtil::PutLong(TAny* aPtr,TInt32 aLong)
	{
	TUint8* trg=(TUint8*)aPtr;
	TUint8* src=(TUint8*)&aLong;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	}

TInt32 OplUtil::GetLong(TAny* aPtr)
	{
	TUint8* src=(TUint8*)aPtr;
	TInt32 theLong;
	TUint8* trg=(TUint8*)&theLong;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	return theLong;
	}

void OplUtil::PutFloat(TAny* aPtr,TReal64 aFloat)
	{
	TUint8* trg=(TUint8*)aPtr;
	TUint8* src=(TUint8*)&aFloat;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;

	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	}

TReal64 OplUtil::GetFloat(TAny* aPtr)
	{
	TUint8* src=(TUint8*)aPtr;
	TReal64 theFloat;
	TUint8* trg=(TUint8*)&theFloat;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;

	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	*trg++=*src++;
	return theFloat;
	}

TUint8* OplUtil::OffsetToAddrL(const RHeap& aHeap,TInt aOffset,TInt aSize)
	{
	if (aOffset<0)
		{
		if (aOffset<TInt(0xffff8000))
			User::Leave(KErrArgument);
		aOffset=(TUint16)aOffset;
		}
	if ((aOffset>=0x40&&(aOffset+aSize)<=aHeap.Size())||(aOffset>=0x28&&(aOffset+aSize)<=0x36))
		return (aHeap.Base()+aOffset);
	if (aOffset!=0)
		User::Leave(KErrArgument);
	return NULL;
	}

TUint8* OplUtil::PopOffsetAsAddrL(CStack& aStack,const RHeap& aHeap,TInt aSize)
	{
	TInt offset=aStack.PopInt32();
	return OffsetToAddrL(aHeap,offset,aSize);
	}

TInt16 OplUtil::MapError(TInt aE32Error)
	{
	if ((aE32Error&KOplErrorMask)==KOplErrorBase)
		return ((TInt16)aE32Error); // left with KOplErrXXX
	return ErrorEraToOpl1993(aE32Error);
	}

//
// OPL1993 Constants for errors
//
const TInt16 E_GEN_FAIL =-1;
const TInt16 E_GEN_ARG =-2;
const TInt16 E_GEN_OS =-3;
const TInt16 E_GEN_NSUP =-4;
const TInt16 E_GEN_UNDER =-5;
const TInt16 E_GEN_OVER =-6;
const TInt16 E_GEN_RANGE =-7;
const TInt16 E_GEN_DIVIDE =-8;
const TInt16 E_GEN_INUSE =-9;
const TInt16 E_GEN_NOMEMORY =-10;
const TInt16 E_GEN_NOSEGMENTS =-11;
const TInt16 E_GEN_NOSEM =-12;
const TInt16 E_GEN_NOPROC =-13;
const TInt16 E_GEN_OPEN =-14;
const TInt16 E_GEN_NOTOPEN =-15;
const TInt16 E_GEN_IMAGE =-16;
const TInt16 E_GEN_RECEIVER =-17;
const TInt16 E_GEN_DEVICE =-18;
const TInt16 E_GEN_FSYS =-19;
const TInt16 E_GEN_START =-20;
const TInt16 E_GEN_NOFONT =-21;
const TInt16 E_GEN_TOOWIDE =-22;
const TInt16 E_GEN_TOOMANY =-23;
const TInt16 E_FILE_EXIST =-32;
const TInt16 E_FILE_NXIST =-33;
const TInt16 E_FILE_WRITE =-34;
const TInt16 E_FILE_READ =-35;
const TInt16 E_FILE_EOF =-36;
const TInt16 E_FILE_FULL =-37;
const TInt16 E_FILE_NAME =-38;
const TInt16 E_FILE_ACCESS =-39;
const TInt16 E_FILE_LOCKED =-40;
const TInt16 E_FILE_DEVICE =-41;
const TInt16 E_FILE_DIR =-42;
const TInt16 E_FILE_RECORD =-43;
const TInt16 E_FILE_RDONLY =-44;
const TInt16 E_FILE_INV =-45;
const TInt16 E_FILE_PENDING =-46;
const TInt16 E_FILE_VOLUME =-47;
const TInt16 E_FILE_CANCEL =-48;
const TInt16 E_FILE_ALLOC =-10;
const TInt16 E_FILE_DISC =-50;
const TInt16 E_FILE_CONNECT =-51;
const TInt16 E_FILE_RETRAN =-52;
const TInt16 E_FILE_LINE =-53;
const TInt16 E_FILE_INACT =-54;
const TInt16 E_FILE_PARITY =-55;
const TInt16 E_FILE_FRAME =-56;
const TInt16 E_FILE_OVERRUN =-57;
const TInt16 E_MDM_CONFAIL =-58;
const TInt16 E_MDM_BUSY =-59;
const TInt16 E_MDM_NOANS =-60;
const TInt16 E_MDM_BLACKLIST =-61;
const TInt16 E_FILE_NOTREADY =-62;
const TInt16 E_FILE_UNKNOWN =-63;
const TInt16 E_FILE_DIRFULL =-64;
const TInt16 E_FILE_PROTECT =-65;
const TInt16 E_FILE_CORRUPT =-66;
const TInt16 E_FILE_ABORT =-67;
const TInt16 E_FILE_ERASE =-68;
const TInt16 E_FILE_INVALID =-69;

const TInt KEraToOpl1993TableSize = 45;

static const TInt16 EraToOpl1993Table[KEraToOpl1993TableSize]=
	{
	0,					// KErrNone (0)
	E_FILE_NXIST,		// KErrNotFound(-1)
	E_GEN_FAIL,			// KErrGeneral(-2)
	E_FILE_CANCEL,		// KErrCancel(-3)
	E_GEN_NOMEMORY,		// KErrNoMemory(-4)
	E_GEN_NSUP,			// KErrNotSupported(-5)
	E_GEN_ARG,   		// KErrArgument(-6)
	E_GEN_RANGE,		// KErrTotalLossOfPrecision(-7)
	E_FILE_NXIST,		// KErrBadHandle(-8)
	E_GEN_OVER,			// KErrOverflow(-9)
	E_GEN_UNDER,		// KErrUnderflow(-10)
	E_FILE_EXIST,		// KErrAlreadyExists(-11)
	E_FILE_DIR,  		// KErrPathNotFound(-12)
	E_GEN_FAIL,			// KErrDied(-13)
	E_GEN_INUSE,		// KErrInUse(-14)
	E_GEN_FAIL,			// KErrServerTerminated(-15)
	E_GEN_TOOMANY,		// KErrServerBusy(-16)
	E_GEN_FAIL,			// KErrCompletion(-17)
	E_FILE_NOTREADY,	// KErrNotReady(-18)
	E_FILE_UNKNOWN,		// KErrUnknown(-19)
	E_FILE_CORRUPT,	 	// KErrCorrupt(-20)
	E_FILE_ACCESS,		// KErrAccessDenied(-21)
	E_FILE_LOCKED,		// KErrLocked(-22)
	E_FILE_WRITE,		// KErrWrite(-23)
	E_GEN_FSYS,			// KErrDismounted(-24)
	E_FILE_EOF,			// KErrEof(-25)
	E_FILE_FULL,		// KErrDiskFull(-26)
	E_GEN_DEVICE,		// KErrBadDriver(-27)
	E_FILE_NAME,		// KErrBadName(-28)
	E_FILE_LINE,		// KErrCommsLineFail(-29)
	E_FILE_FRAME,		// KErrCommsFrame(-30)
	E_FILE_OVERRUN,		// KErrCommsOverrun(-31)
	E_FILE_PARITY,		// KErrCommsParity(-32)
	E_FILE_RETRAN,		// KErrTimeout(-33)
	E_FILE_CONNECT,		// KErrCouldNotConnect(-34)
	E_FILE_DISC,		// KErrCouldNotDisconnect(-35)
	E_FILE_DISC,		// KErrDisconnected(-36)
	E_GEN_FAIL,			// KErrBadLibraryEntryPoint(-37)
	E_GEN_FAIL,			// KErrBadDescriptor(-38)
	E_FILE_ABORT,		// KErrAbort(-39)
	E_FILE_FULL,		// KErrTooBig(-40)
	E_GEN_DIVIDE,		// KErrDivideByZero(-41)
	E_GEN_FAIL,			// KErrBadPower(-42)
	E_FILE_DIRFULL		// KErrDirFull=(-43)
	};

TInt16 OplUtil::ErrorEraToOpl1993(TInt aE32Error)
	{
	if (aE32Error>0)
		return (TInt16) aE32Error;

	aE32Error=-aE32Error;
	if (aE32Error<KEraToOpl1993TableSize)
		return EraToOpl1993Table[aE32Error];
	return E_GEN_FAIL;
	}

TInt OplUtil::GetErrResource(TInt aError)
	// Return the resource id with R_OPLR_UNKNOWN_ERROR if unknown
	{
	TInt resourceNo=R_OPLR_UNKNOWN_ERROR;
 	if (aError<0&&aError>-26)
		resourceNo=R_OPLR_UNKNOWN_ERROR-aError; // NB aError is negative here
	else if (aError<-31&&aError>=KOplMaxErr) // may be more errors added !!!!!
		resourceNo-=(aError+6); // -32 is mapped to -26 etc.
	return resourceNo;
	}

TInt16 OplUtil::MultiplyL(TInt16 aArg1,TInt16 aArg2)
	{
	TInt res=aArg1*aArg2;
	if (res>KMaxTInt16 || res<KMinTInt16)
		User::Leave(KOplErrOverflow);
	return (TInt16)res;
	}

TInt32 OplUtil::MultiplyL(TInt32 aArg1,TInt32 aArg2)
	{
	TInt64 arg164=TInt64((TInt)aArg1);
	TInt64 arg264=TInt64((TInt)aArg2);
	arg164*=arg264;
	TInt32 low=arg164.Low();
	TInt32 high=arg164.High();	
	if ((low<0 && high!=-1) || (low>=0 && high!=0))
		User::Leave(KOplErrOverflow);
	return low;
	}

#if defined(__SERIES60__)
// Some stale font ids from EON14.GDR's fontid.h
#define KScreenFontUidCourierNormal8   268436065
#define KScreenFontUidTimesNormal8     268435965
#define KScreenFontUidTimesNormal11    268435966
#define KScreenFontUidTimesNormal13    268435967
#define KScreenFontUidTimesNormal15    268435968
#define KScreenFontUidArialNormal8     268435954
#define KScreenFontUidArialNormal11    268435955
#define KScreenFontUidArialNormal13    268435956
#define KScreenFontUidArialNormal15    268435957
#define KScreenFontUidTiny4     268435504

// OS61fonts.h
const TInt KOS61BaseFontId2		= 0x63330000 ;
const TInt KOS61NormalFontId	= KOS61BaseFontId2 + 0x00 ;
const TInt KOS61TitleFontId		= KOS61BaseFontId2 + 0x01 ;
const TInt KOS61AnnotationFontId = KOS61BaseFontId2 + 0x02 ;
const TInt KOS61LegendFontId	= KOS61BaseFontId2 + 0x03 ;
const TInt KOS61SymbolFontId	= KOS61BaseFontId2 + 0x04 ;
const TInt KOS61DenseFontId		= KOS61BaseFontId2 + 0x05 ;

const TInt KOS61BaseFontId	=0x10000000;
const TInt KOplLatinPlain12	=KOS61BaseFontId+ELatinPlain12;
const TInt KOplLatinBold12	=KOS61BaseFontId+ELatinBold12;
const TInt KOplKOplLatinBold13	=KOS61BaseFontId+ELatinBold13;
const TInt KOplLatinBold17	=KOS61BaseFontId+ELatinBold17;
const TInt KOplLatinBold19	=KOS61BaseFontId+ELatinBold19;
const TInt KOplNumberPlain5	=KOS61BaseFontId+ENumberPlain5;
const TInt KOplClockBold30	=KOS61BaseFontId+EClockBold30;
const TInt KOplLatinClock14	=KOS61BaseFontId+ELatinClock14;
const TInt KOplFontCustom	=KOS61BaseFontId+EFontCustom; //0x10000009
const TInt KOplPlain12		=KOS61BaseFontId+EPlain12;
const TInt KOplPlain16		=KOS61BaseFontId+EPlain16; 
const TInt KOplApacPlain12	=KOS61BaseFontId+EApacPlain12;
const TInt KOplApacPlain16	=KOS61BaseFontId+EApacPlain16;
const TInt KOplBold18		=KOS61BaseFontId+EBold18;
const TInt KOplBold24		=KOS61BaseFontId+EBold24; //0x1000000d
const TInt KOS61MaxFontId	=KOplBold24; // Always set to the last font id.

/*
 * TitleFont -- the heavyweight app title font
 * AnnotationFont -- used by the softkeys Option, Exit etc.
 * LegendFont -- ditto
 * SymbolFont -- ditto
 * DenseFont -- the app names in main "menu"
 */

TBool OplUtil::IsStandardFontUid(const TUid aFontUid)
	{
	if ((aFontUid.iUid==0x1000000a) || (aFontUid.iUid==0x1000000b)) // Don't use: gives LAFENV 8 panic
		return EFalse; 
	if ((aFontUid.iUid >= KOS61BaseFontId) && (aFontUid.iUid<=KOS61MaxFontId))
		return ETrue;
	return EFalse;
	}

/*
KOplKOplLatinBold13
KOplLatinBold17
KOplLatinBold19
KOplNumberPlain5
KOplClockBold30
KOplLatinClock14 
KOplFontCustom 
KOplPlain12
KOplPlain16
KOplApacPlain12
KOplApacPlain16
KOplBold18
KOplBold24
*/

//  ELatinPlain12, ELatinBold12, ELatinBold13, ELatinBold17, ELatinBold19, ENumberPlain5,
// ClockBold30, LatinClock14, ECustomFont


const CFont* OplUtil::MapFontUidToStandardFontL(const TUid aFontUid)
	{
	if (OplUtil::IsStandardFontUid(aFontUid))
		return (AknLayoutUtils::FontFromId(aFontUid.iUid-KOS61BaseFontId));
	User::Leave(KOplErrFontNotLoaded);
	return NULL; //Keep compiler happy.
	}

/*


	switch(aFontUid.iUid)
		{
		case KOplLatinPlain2:
			return LatinPlain12();
			break;
		case KOplLatinBold12:
			return LatinBold12();
			break;
		case KOplKOplLatinBold13:
			return LatinBold13();
			break;
		case KOplLatinBold17:
			return LatinBold17();
			break;
		case KOplLatinBold19:
			return LatinBold19();
			break;
		case KOplNumberPlain5:
			return NumberPlain5();
			break;
		case KOplClockBold30:
			return ClockBold30();
			break;
		case KOplLatinClock14:
			return LatinClock14();
			break;
		case KOplFontCustom:
			return FontCustom();
			break;
		case KOplPlain12:
			return Plain12();
			break;
		case KOplPlain16:
			return Plain16();
			break;
		case KOplApacPlain12:
			return ApacPlain12();
			break;
		case KOplApacPlain16:
			return ApacPlain16();
			break;
		case KOplBold18:
			return Bold18();
			break;
		case KOplBold24:
			return Bold24();
			break;
*/

/*
		case KOS61NormalFontId:
			return CEikonEnv::Static()->NormalFont();
			break;
		case KOS61TitleFontId:
			return CEikonEnv::Static()->TitleFont();
			break;
		case KOS61AnnotationFontId:
			return CEikonEnv::Static()->AnnotationFont();
			break;
		case KOS61LegendFontId:
			return CEikonEnv::Static()->LegendFont();
			break;
		case KOS61SymbolFontId:
			return CEikonEnv::Static()->SymbolFont();
			break;
		case KOS61DenseFontId:
			return CEikonEnv::Static()->DenseFont();
			break;
			
*/

/*		
		default:
			break;
		}
	User::Leave(KOplErrFontNotLoaded);
	return NULL; //Keep compiler happy.
*/

//AknLayoutUtils::FontFromId



/*

// If the gFONT id is low, it's one of the OS6.1 built-in ones,
// so don't try and find a UID. (or use some 'match' uid)
//

// Keeping with the spirit of early font usage,
// on-board fonts are mapped to low numbers,
// external fonts can be loaded and their ids are used as before.

 * TitleFont -- the heavyweight app title font
 * AnnotationFont -- used by the softkeys Option, Exit etc.
 * LegendFont -- ditto
 * SymbolFont -- ditto
 * DenseFont -- the app names in main "menu"

Fonts:

MapFontId

Do all uses then call SetFontByUid somewhere???
FONT does
CLOCK n/a
gstyle calls getfontbyuid and then setfont()
gfont calls getfontbyname!!!!This will have to change...

mono 8x8 = dense mono ?????
roman 8/swiss 8 = normal ???
11 normal ????
13 annot ???
16 = title ??? 

*/
#endif

#if !defined(__UIQ__)
TUid OplUtil::MapFontId(TInt aFontId,TAlgStyle& aStyle)
	{
	switch(aFontId)
		{
	case 4 :
		aFontId = KScreenFontUidCourierNormal8;
//		aStyle.SetIsMono(ETrue); // courier should already be mono !!!
		break;
	case 5 :
		aFontId = KScreenFontUidTimesNormal8;
		break;
	case 6 :
		aFontId = KScreenFontUidTimesNormal11;
		break;
	case 7 :
		aFontId = KScreenFontUidTimesNormal13;
		break;
	case 8 :
		aFontId = KScreenFontUidTimesNormal15;
		break;
	case 9 :
		aFontId = KScreenFontUidArialNormal8;
		break;
	case 10 :
		aFontId = KScreenFontUidArialNormal11;
		break;
	case 11 :
		aFontId = KScreenFontUidArialNormal13;
		break;
	case 13 :
		aFontId = KScreenFontUidTiny4;
		aStyle.SetIsMono(ETrue);
		break;
	case 12 :
		aFontId = KScreenFontUidArialNormal15;
		break;
	case 0x9A :
#if defined(__SERIES60__)
		aFontId = KOplLatinPlain12;
#else
		aFontId = KScreenFontUidArialNormal15;
#endif
	default :
		// treat aFontId as the fonts UID
		// e.g. LatinBold12 on Series 60
		break;
		}
	return TUid::Uid(aFontId);
	}
#else
TUid OplUtil::MapFontId(TInt aFontId,TAlgStyle& /*aStyle*/)
	{
	return TUid::Uid(aFontId);
	}
#endif

//
// Character conversion stuff
//
EXPORT_C HBufC16* COplRuntimeCharConv::ConvertFromNarrowToUnicodeL(const TDesC8& aNarrowString,const TUint aCharacterSet)
	{
	HBufC* wide=ConvertFromNarrowToUnicodeLC(aNarrowString,aCharacterSet);
	CleanupStack::Pop();
	return wide;
	}

EXPORT_C HBufC16* COplRuntimeCharConv::ConvertFromNarrowToUnicodeLC(const TDesC8& aNarrowString,const TUint aCharacterSet)
	{
	HBufC* wide = HBufC::NewLC(aNarrowString.Length());
	CCnvCharacterSetConverter* charconv = CCnvCharacterSetConverter::NewLC();
	TPtr widePtr = wide->Des();
	// Convert the buffer to unicode and decode it with the current charset
	if (charconv->PrepareToConvertToOrFromL(aCharacterSet, CEikonEnv::Static()->FsSession())!=CCnvCharacterSetConverter::EAvailable)
		User::Leave(KErrNotSupported);
	TInt state = CCnvCharacterSetConverter::KStateDefault;
#ifdef _DEBUG
	const TInt conversionResult=User::LeaveIfError(charconv->ConvertToUnicode(widePtr,aNarrowString,state));
	__ASSERT_DEBUG(conversionResult==0, User::Invariant());
#else
	User::LeaveIfError(charconv->ConvertToUnicode(widePtr,aNarrowString,state));
#endif
	CleanupStack::PopAndDestroy(); // charconv
	return wide;
	}

EXPORT_C HBufC8* COplRuntimeCharConv::ConvertFromUnicodeToNarrowL(const TDesC16& aUnicodeString,const TUint aCharacterSet)
	{
	HBufC8* narrow = ConvertFromUnicodeToNarrowLC(aUnicodeString,aCharacterSet);
	CleanupStack::Pop();
	return narrow;
	}

EXPORT_C HBufC8* COplRuntimeCharConv::ConvertFromUnicodeToNarrowLC(const TDesC16& aUnicodeString,const TUint aCharacterSet)
	{
	HBufC8* narrow = HBufC8::NewLC(aUnicodeString.Length());
	CCnvCharacterSetConverter* charconv = CCnvCharacterSetConverter::NewLC();
	TPtr8 narrowPtr = narrow->Des();
	// Convert the buffer and decode it with the current charset
	if (charconv->PrepareToConvertToOrFromL(aCharacterSet, CEikonEnv::Static()->FsSession())!=CCnvCharacterSetConverter::EAvailable)
		User::Leave(KErrNotSupported);
	charconv->SetDowngradeForExoticLineTerminatingCharacters(CCnvCharacterSetConverter::EDowngradeExoticLineTerminatingCharactersToJustLineFeed);
#ifdef _DEBUG
	const TInt conversionResult=User::LeaveIfError(charconv->ConvertFromUnicode(narrowPtr, aUnicodeString));
	__ASSERT_DEBUG(conversionResult==0, User::Invariant());
#else
	User::LeaveIfError(charconv->ConvertFromUnicode(narrowPtr, aUnicodeString));
#endif
	CleanupStack::PopAndDestroy(); // charconv
	return narrow;
	}

#if defined(__UIQ__)
const CFont* OplUtil::MapFontUidToStandardFontL(const TUid /*aFontUid*/)
	{
	return CEikonEnv::Static()->NormalFont();
	}

TBool OplUtil::IsStandardFontUid(const TUid /*aFontUid*/)
	{
	return ETrue;
	}
#endif