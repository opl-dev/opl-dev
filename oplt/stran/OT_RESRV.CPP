// OT_RESRV.CPP
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
//

// Reserved word lookup for Opl Lexical scanner
//		COplLexBase::IsReserved()

#include "ot_std.h"

const TUint8 KOplAllTargets=KMaxTUint8;
const TUint8 KOplNotOpl1993=TUint8(~(TUint8(KOplTargetOpl1993)));
const TUint8 KOplOpl1993Only=TUint8(KOplTargetOpl1993);

/////////////////////////////////////////////////////////////////
//
// TLookupHeader. Reserved word lookup table entry header
//
////////////////////////////////////////////////////////////////
class TLookupHeader
	{
public:
	TBool Matches(TOplTranTarget aTarget,const TDesC& anEnd);
public:
	TText *iName;
	TUint8 iTargets;
	TUint8 byte1;
	TUint8 byte2;
	TUint8 byte3;
	};

TBool TLookupHeader::Matches(TOplTranTarget aTarget,const TDesC& aName)
	{

	return ((aTarget&iTargets) && (aName.Compare(TPtrC(iName))==0));
	}

/////////////////////////////////////////////////////////////////
//
// TFunctionLookup - Entry inn the function lookup tables
//
/////////////////////////////////////////////////////////////////

// A whole bunch of defined used in an attempt to make the function lookup table readable
#define TARGETS(t) TUint8(t)
#define RET_TYPE(t) TUint8(t)
#define NUMBER(n) TUint8(n)

#define AOK(n) TUint8(0x01<<n)
#define STATS TUint8(0)

#define ARGSTATS 0
#define ARG1(a) (TUint(a))
#define ARG2(a) (TUint(a)<<4)
#define ARG3(a) (TUint(a)<<8)
#define ARG4(a) (TUint(a)<<12)
#define ARG5(a) (TUint(a)<<16)
#define ARG6(a) (TUint(a)<<20)
#define ARG7(a) (TUint(a)<<24)
// NOTE don't allow ARG 8 so that the final argument can will always be ABAD

// Argument types. NOTE: The +1 is so that uninitialized arguments entries appear as bad ones.
#define ABAD TOplSignature::EArgBadType+1
#define AWORD TOplSignature::EArgWord+1
#define ALONG TOplSignature::EArgLong+1
#define AREAL TOplSignature::EArgReal+1
#define ASTRING TOplSignature::EArgString+1
#define AUWORD TOplSignature::EArgUword+1
#define AWREF TOplSignature::EArgWordRef+1
#define ALREF TOplSignature::EArgLongRef+1
#define ARREF TOplSignature::EArgRealRef+1
#define ASREF TOplSignature::EArgStringRef+1
#define AREF TOplSignature::EArgAnyRef+1
#define AOVER TOplSignature::EArgOverLoad+1
#define ANATIVE TOplSignature::EArgNative+1
#define AUNATIVE TOplSignature::EArgUNative+1

//
// This should be derived from TLookup Header, but I can't seem to find a way to stop the
// compiler whingeing on about can't find TOplFunction (TOplFunction& ) constructor.
// So I just put it straight in there and do some explicit casting in the lookup table.
// Note that another compiler could start rearranging the fields
// so that a TFunctionLookup no longer looks like a TLookupHeader. We'll find out soone enough if this does
// happen, so I'm not fretting too much
//
class TFunctionLookup
	{
public:
	inline TOplToken::TType ReturnType() const { return TOplToken::TType(byte1);}
	inline TOplFunction::TNumber Number() const { return TOplFunction::TNumber(byte2);}
	inline TBool IsStatistical() const { return byte3==ARGSTATS;}
	inline TBool IsAllowedCount(TUint anArgCount) const { return (1<<anArgCount)&byte3;}
	inline TOplSignature::TArgType Arg(TUint n) const {return TOplSignature::TArgType(((iArgs>>(n<<2))&0x0f)-1);}
public:
	const TText *iName;
	TUint8 iTargets;
	TUint8 byte1;
	TUint8 byte2;
	TUint8 byte3;
	TUint iArgs;		// Types of the arguments stored as nibbles
	};

//////////////////////////////////////////////////////////////////
//
// TKeywordLookup - Entry in the keyword lookup tables
//
///////////////////////////////////////////////////////////////////
class TKeywordLookup
	{
public:
	inline TOplKeyword::TCode Code() const { return TOplKeyword::TCode(byte1);}
	inline TBool Trappable() const { return byte2;}
public:
	const TText *iName;
	TUint8 iTargets;
	TUint8 byte1;
	TUint8 byte2;
	TUint8 byte3;
	};

////////////////////////////////////////////////////////////////////
//
// TReservedLookup - Entry in teh Reserved words lookup table
//
///////////////////////////////////////////////////////////////////
class TReservedLookup
	{
public:
	inline TOplToken::TValue Token() const { return TOplToken::TValue(TUint(byte2<<8)|byte1);}
public:
	const TText *iName;
	TUint8 iTargets;
	TUint8 byte1;
	TUint8 byte2;
	TUint8 byte3;
	};

////////////////////////////////////////////////////////////////////
//
// Function lookup tables
//
////////////////////////////////////////////////////////////////////;
static const TFunctionLookup functionATable[]=
	{
	{_S("BS"), KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EAbs),AOK(1), ARG1(AREAL)},
	{_S("COS"), KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EACos),AOK(1),ARG1(AREAL)},
	{_S("DDR"), KOplAllTargets,RET_TYPE(TOplToken::ENativeReturn),NUMBER(TOplFunction::EAddr),AOK(1),ARG1(AOVER)},
	{_S("DJUSTALLOC"), KOplAllTargets,RET_TYPE(TOplToken::ENativeReturn),NUMBER(TOplFunction::EAdjustAlloc),AOK(3),ARG1(ANATIVE)|ARG2(ANATIVE)|ARG3(ANATIVE)},
	{_S("LERT"), KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EAlert),AOK(1)|AOK(2)|AOK(3)|AOK(4)|AOK(5),ARG1(ASTRING)|ARG2(ASTRING)|ARG3(ASTRING)|ARG4(ASTRING)|ARG5(ASTRING)},
	{_S("LLOC"), KOplAllTargets,RET_TYPE(TOplToken::ENativeReturn),NUMBER(TOplFunction::EAlloc),AOK(1), ARG1(AUNATIVE)},
	{_S("SC"), KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EAsc),AOK(1),ARG1(ASTRING)},
	{_S("SIN"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EASin),AOK(1),ARG1(AREAL)},
	{_S("TAN"), KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EATan),AOK(1),ARG1(AREAL)}
	};
static const TFunctionLookup functionBTable[]=
	{
	{_S("OOKMARK"),KOplNotOpl1993,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EBookMark),AOK(0),0}
	};
static const TFunctionLookup functionCTable[]=
	{
	{_S("ALL"), KOplOpl1993Only, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::ECall), AOK(1)|AOK(2)|AOK(3)|AOK(4)|AOK(5)|AOK(6),ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)|ARG4(AWORD)|ARG5(AWORD)|ARG6(AWORD)},
	{_S("HR$"), KOplAllTargets, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EChrS),AOK(1),ARG1(AWORD)},
	{_S("MD$"), KOplAllTargets,RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::ECmdS),AOK(1),ARG1(AWORD)},
	{_S("OS"), KOplAllTargets,RET_TYPE(TOplToken::EReal), NUMBER(TOplFunction::ECos), AOK(1), ARG1(AREAL)},
	{_S("OUNT"), KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ECount),AOK(0),0},
	{_S("REATESPRITE"), KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ECreateSprite),AOK(0), 0}
	};
static const TFunctionLookup functionDTable[]=
	{
	{_S("ATETOSECS"), KOplAllTargets, RET_TYPE(TOplToken::ELong), NUMBER(TOplFunction::EDateToSecs),AOK(6), ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)|ARG4(AWORD)|ARG5(AWORD)|ARG6(AWORD)},
	{_S("ATIM$"), KOplAllTargets, RET_TYPE(TOplToken::EString),	NUMBER(TOplFunction::EDatim),AOK(0), 0},
	{_S("AY"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EDay),AOK(0), 0},
	{_S("AYNAME$"), KOplAllTargets, RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EDayNameS),AOK(1), ARG1(AWORD)},
	{_S("AYS"), KOplAllTargets, RET_TYPE(TOplToken::ELong), NUMBER(TOplFunction::EDays),AOK(3), ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)},
	{_S("EG"), KOplAllTargets, RET_TYPE(TOplToken::EReal), NUMBER(TOplFunction::EDeg),AOK(1), ARG1(AREAL)},
	{_S("IALOG"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EDialog),AOK(0), 0},
	{_S("IR$"), KOplAllTargets, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EDirS),AOK(1), ARG1(ASTRING)},
	{_S("OW"), KOplAllTargets,	RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EDow),AOK(3), ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)}
	};
static const TFunctionLookup functionETable[]=
	{
	{_S("NTERSEND"), KOplOpl1993Only, RET_TYPE(TOplToken::EWord),  NUMBER(TOplFunction::EEnterSend),AOK(2)|AOK(3)|AOK(4)|AOK(5),ARG1(AWORD)|ARG2(AWORD)|ARG3(AREF)|ARG4(AREF)|ARG5(AREF)},
	{_S("NTERSEND0"), KOplOpl1993Only, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EEnterSend0),AOK(2)|AOK(3)|AOK(4)|AOK(5),ARG1(AWORD)|ARG2(AWORD)|ARG3(AREF)|ARG4(AREF)|ARG5(AREF)},
	{_S("OF"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EEof),AOK(0), 0},
	{_S("RR"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EErr),AOK(0), 0},
	{_S("RR$"), KOplAllTargets, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EErrS),AOK(1), ARG1(AWORD)},
	{_S("RRX$"), KOplNotOpl1993, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EErrxS),AOK(0), 0},
	{_S("VAL"), KOplAllTargets, RET_TYPE(TOplToken::EReal), NUMBER(TOplFunction::EEval),AOK(1), ARG1(ASTRING)},
	{_S("XIST"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EExist),AOK(1), ARG1(ASTRING)},
	{_S("XP"), KOplAllTargets, RET_TYPE(TOplToken::EReal), NUMBER(TOplFunction::EExp),AOK(1), ARG1(AREAL)}
	};
static const TFunctionLookup functionFTable[]=
	{
	{_S("IND"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EFind),AOK(1), ARG1(ASTRING)},
	{_S("INDFIELD"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EFindField),AOK(4), ARG1(ASTRING)|ARG2(AWORD)|ARG3(AWORD)|ARG4(AWORD)},
	{_S("INDLIB"), KOplOpl1993Only, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EFindLib),AOK(2), ARG1(AWREF)|ARG2(ASTRING)},
	{_S("IX$"), KOplAllTargets, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EFixS),AOK(3), ARG1(AREAL)|ARG2(AWORD)|ARG3(AWORD)},
	{_S("LT"), KOplAllTargets, RET_TYPE(TOplToken::EReal), NUMBER(TOplFunction::EFlt),AOK(1), ARG1(ALONG)}
	};
static const TFunctionLookup functionGTable[]=
	{
	{_S("CREATE"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgCreate), AOK(5), ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)|ARG4(AWORD)|ARG5(AWORD)|ARG6(AOVER)},
	{_S("CREATEBIT"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgCreateBit),AOK(2), ARG1(AWORD)|ARG2(AOVER)|ARG3(AWORD)}, //overload: gCREATEBIT has extra optional mode% arg for opler1
	{_S("EN$"), KOplAllTargets, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EGen),AOK(2), ARG1(AREAL)|ARG2(AWORD)},
	{_S("ET"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EGet),AOK(0), 0},
	{_S("ET$"), KOplAllTargets, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EGetS),AOK(0),0},
	{_S("ETCMD$"), KOplAllTargets,RET_TYPE(TOplToken::EString),	NUMBER(TOplFunction::EGetCmdS),AOK(0), 0},
	{_S("ETDOC$"), KOplNotOpl1993,RET_TYPE(TOplToken::EString),	NUMBER(TOplFunction::EGetDocS),AOK(0), 0},
	{_S("ETEVENTC"),KOplNotOpl1993,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EGeteventc),AOK(1), ARG1(AWREF)},
	{_S("ETLIBH"), KOplOpl1993Only,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EGetLibH),AOK(1), ARG1(AWORD)},
	{_S("HEIGHT"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgHeight), AOK(0), 0},
	{_S("IDENTITY"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgIdentity),AOK(0), 0},
	{_S("LOADBIT"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgLoadBit),	AOK(1)|AOK(2)|AOK(3),ARG1(ASTRING)|ARG2(AWORD)|ARG3(AWORD)},
	{_S("LOADFONT"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgLoadFont),AOK(1), ARG1(ASTRING)},
	{_S("ORIGINX"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgOriginX),AOK(0), 0},
	{_S("ORIGINY"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgOriginY),	AOK(0), 0},
	{_S("PIXEL"), KOplNotOpl1993,RET_TYPE(TOplToken::ELong), NUMBER(TOplFunction::EgPixel),AOK(2), ARG1(AWORD)|ARG2(AWORD)},
	{_S("PRINTCLIP"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgPrintClip),	AOK(2), ARG1(ASTRING)|ARG2(AWORD)},
	{_S("RANK"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgRank),AOK(0), 0},
	{_S("TWIDTH"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgTWidth),AOK(1), ARG1(ASTRING)},
	{_S("WIDTH"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgWidth),AOK(0), 0},
	{_S("X"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgX),AOK(0), 0},
	{_S("Y"), KOplAllTargets,RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EgY),AOK(0), 0}
	};
static const TFunctionLookup functionHTable[]=
	{
	{_S("EX$"), KOplAllTargets, RET_TYPE(TOplToken::EString), NUMBER(TOplFunction::EHexS),AOK(1), ARG1(ALONG)},
	{_S("OUR"), KOplAllTargets,	RET_TYPE(TOplToken::EWord),  NUMBER(TOplFunction::EHour),AOK(0), 0}
	};
static const TFunctionLookup functionITable[]=
	{
	{_S("ABS"), KOplAllTargets, RET_TYPE(TOplToken::ELong), NUMBER(TOplFunction::EIAbs),AOK(1), ARG1(ALONG)},
	{_S("NT"), KOplAllTargets, RET_TYPE(TOplToken::ELong), NUMBER(TOplFunction::EInt),AOK(1), ARG1(AREAL)},
	{_S("NTF"), KOplAllTargets, RET_TYPE(TOplToken::EReal), NUMBER(TOplFunction::EIntF),AOK(1), ARG1(AREAL)},
	{_S("NTRANS"), KOplNotOpl1993, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EInTrans),AOK(0), 0},
	{_S("OA"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoa),AOK(5), ARG1(AWORD)|ARG2(AWORD)|ARG3(AWREF)|ARG4(AREF)|ARG5(AREF)},
	{_S("OC"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoc),AOK(3)|AOK(4)|AOK(5),ARG1(AWORD)|ARG2(AWORD)|ARG3(AWREF)|ARG4(AREF)|ARG5(AREF)},
	{_S("OCANCEL"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoCancel),AOK(1), ARG1(AWORD)},
	{_S("OCLOSE"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoClose),AOK(1), ARG1(AWORD)},
	{_S("OOPEN"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoOpen),AOK(3), ARG1(AWREF)|ARG2(AOVER)|ARG3(AWORD)},
	{_S("OREAD"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoRead),AOK(3), ARG1(AWORD)|ARG2(AUNATIVE)|ARG3(AWORD)},
	{_S("OSEEK"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoSeek),AOK(3), ARG1(AWORD)|ARG2(AWORD)|ARG3(ALREF)},
	{_S("OW"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIow),AOK(4), ARG1(AWORD)|ARG2(AWORD)|ARG3(AREF)|ARG4(AREF)},
	{_S("OWAIT"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoWait),AOK(0), 0},
	{_S("OWRITE"), KOplAllTargets, RET_TYPE(TOplToken::EWord), NUMBER(TOplFunction::EIoWrite),AOK(3), ARG1(AWORD)|ARG2(AUNATIVE)|ARG3(AWORD)}
	};
static const TFunctionLookup functionKTable[]=
	{
	{_S("EY"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EKey),AOK(0), 0},
	{_S("EY$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EKeyS),AOK(0), 0},
	{_S("EYA"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EKeyA),AOK(2), ARG1(AWREF)|ARG2(AWREF)},
	{_S("EYC"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EKeyC),AOK(1), ARG1(AWREF)},
	{_S("MOD"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EKMod),AOK(0), 0}
	};
static const TFunctionLookup functionLTable[]=
	{
	{_S("EFT$"), KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::ELeftS),AOK(2), ARG1(ASTRING)|ARG2(AWORD)},
	{_S("EN"), KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ELen),AOK(1), ARG1(ASTRING)},
	{_S("ENALLOC"), KOplAllTargets,RET_TYPE(TOplToken::ENativeReturn),NUMBER(TOplFunction::ELenAlloc),AOK(1), ARG1(ANATIVE)},
	{_S("N"), KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ELn),AOK(1), ARG1(AREAL)},
	{_S("OADLIB"), KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ELoadLib),AOK(3), ARG1(AWREF)|ARG2(ASTRING)|ARG3(AWORD)},
	{_S("OC"), KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ELoc),AOK(2), ARG1(ASTRING)|ARG2(ASTRING)|ARG3(AOVER)},
	{_S("OG"), KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ELog),AOK(1), ARG1(AREAL)},
	{_S("OWER$"), KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::ELowerS),AOK(1), ARG1(ASTRING)}
	};
static const TFunctionLookup functionMTable[]=
	{
	{_S("AX"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EMax),ARGSTATS, 0},
	{_S("EAN"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EMean),ARGSTATS, 0},
	{_S("ENU"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EMenu),AOK(0),ARG1(AOVER)},
	{_S("ID$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EMidS),AOK(3), ARG1(ASTRING)|ARG2(AWORD)|ARG3(AWORD)},
	{_S("IN"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EMin),ARGSTATS, 0},
	{_S("INUTE"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EMinute),AOK(0), 0},
	{_S("ONTH"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EMonth),AOK(0), 0},
	{_S("ONTH$"), KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EMonthS),AOK(1), ARG1(AWORD)},
	{_S("POPUP"),KOplNotOpl1993,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EMPopup),AOK(5),ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)|ARG4(ASTRING)|ARG5(AWORD)|ARG6(AOVER)}
	};
static const TFunctionLookup functionNTable[]=
	{
	{_S("EWOBJ"),KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ENewObj),AOK(2), ARG1(AWORD)|ARG2(AWORD)},
	{_S("EWOBJH"),KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ENewObjH),AOK(2), ARG1(AWORD)|ARG2(AWORD)},
	{_S("UM$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::ENumS),AOK(2), ARG1(AREAL)|ARG2(AWORD)}
	};
static const TFunctionLookup functionOTable[]=
	{
	{_S("S"), KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EOs),AOK(1)|AOK(2)|AOK(3), ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)}
	};
static const TFunctionLookup functionPTable[]=
	{
	{_S("ARSE$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EParseS),AOK(3), ARG1(ASTRING)|ARG2(ASTRING)|ARG3(AWREF)},
	{_S("EEK$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EPeekS),AOK(1), ARG1(AUNATIVE)},
	{_S("EEKB"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EPeekB),AOK(1), ARG1(AUNATIVE)},
	{_S("EEKF"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EPeekF),AOK(1), ARG1(AUNATIVE)},
	{_S("EEKL"),KOplAllTargets,RET_TYPE(TOplToken::ELong),NUMBER(TOplFunction::EpeekL),AOK(1), ARG1(AUNATIVE)},
	{_S("EEKW"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EPeekW),AOK(1), ARG1(AUNATIVE)},
	{_S("I"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EPi),AOK(0), 0},
	{_S("OS"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::Epos),AOK(0), 0}
	};
static const TFunctionLookup functionRTable[]=
	{
	{_S("AD"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ERad),AOK(1), ARG1(AREAL)},
	{_S("EALLOC"),KOplAllTargets,RET_TYPE(TOplToken::ENativeReturn),NUMBER(TOplFunction::EReAlloc),AOK(2), ARG1(ANATIVE)|ARG2(AUNATIVE)},
	{_S("ECSIZE"),KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ERecSize),AOK(0), 0}, // hp: dropped from Opler1
	{_S("EPT$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EReptS),AOK(2), ARG1(ASTRING)|ARG2(AWORD)},
	{_S("IGHT$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::ERightS),AOK(2), ARG1(ASTRING)|ARG2(AWORD)},
	{_S("ND"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ERnd),AOK(0), 0}
	};
static const TFunctionLookup functionSTable[]=
	{
	{_S("CI$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::ESciS),AOK(3), ARG1(AREAL)|ARG2(AWORD)|ARG3(AWORD)},
	{_S("ECOND"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ESecond),AOK(0), 0},
	{_S("END"),KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ESend),AOK(2)|AOK(3)|AOK(4)|AOK(5), ARG1(AWORD)|ARG2(AWORD)|ARG3(AREF)|ARG4(AREF)|ARG5(AREF)},
	{_S("IN"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ESin),AOK(1),ARG1(AREAL)},
	{_S("IZE"), KOplNotOpl1993,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ESize),AOK(1), ARG1(ASTRING)},
	{_S("PACE"),KOplAllTargets,RET_TYPE(TOplToken::ELong),NUMBER(TOplFunction::ESpace),AOK(0), 0},
	{_S("QR"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ESqr),AOK(1), ARG1(AREAL)},
	{_S("TATWININFO"),KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EStatWinInfo),AOK(2), ARG1(AWORD)|ARG2(AWREF)},
	{_S("TD"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EStd),ARGSTATS, 0},
	{_S("UM"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ESum),ARGSTATS, 0}
	};
static const TFunctionLookup functionTTable[]=
	{
	{_S("AN"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::ETan),AOK(1), ARG1(AREAL)},
	{_S("ESTEVENT"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::ETestEvent),AOK(0), 0}
	};
static const TFunctionLookup functionUTable[]=
	{
	{_S("ADD"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EUAdd),AOK(2), ARG1(AUWORD)|ARG2(AWORD)},
	{_S("NLOADLIB"),KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EUnloadLib),AOK(1), ARG1(AWORD)},
	{_S("PPER$"),KOplAllTargets,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EUpperS),AOK(1), ARG1(ASTRING)},
	{_S("SR"),KOplOpl1993Only,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EUser),AOK(5), ARG1(AUWORD)|ARG2(AUWORD)|ARG3(AUWORD)|ARG4(AUWORD)|ARG5(AUWORD)},
	{_S("SR$"),KOplOpl1993Only,RET_TYPE(TOplToken::EString),NUMBER(TOplFunction::EUserS),AOK(5), ARG1(AUWORD)|ARG2(AUWORD)|ARG3(AUWORD)|ARG4(AUWORD)|ARG5(AUWORD)},
	{_S("SUB"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EUSub),AOK(2), ARG1(AUWORD)|ARG2(AWORD)}
	};
static const TFunctionLookup functionVTable[]=
	{
	{_S("AL"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EVal),AOK(1), ARG1(ASTRING)},
	{_S("AR"),KOplAllTargets,RET_TYPE(TOplToken::EReal),NUMBER(TOplFunction::EVar),ARGSTATS, 0}
	};
static const TFunctionLookup functionWTable[]=
	{
	{_S("EEK"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EWeek),AOK(3), ARG1(AWORD)|ARG2(AWORD)|ARG3(AWORD)}
	};
static const TFunctionLookup functionYTable[]=
	{
	{_S("EAR"),KOplAllTargets,RET_TYPE(TOplToken::EWord),NUMBER(TOplFunction::EYear),AOK(0), 0}
	};

//////////////////////////////////////////////////////////////////
//
// Keyword lookup tables
//
//////////////////////////////////////////////////////////////////
static const TKeywordLookup keywordATable[]=
	{
	{_S("PPEND"),KOplAllTargets,TUint8(TOplKeyword::EAppend),ETrue,0},
	{_S("PPENDSPRITE"),KOplOpl1993Only,TUint8(TOplKeyword::EAppendsprite),EFalse,0},
	{_S("T"),KOplAllTargets,TUint8(TOplKeyword::EAt),EFalse,0}
	};
static const TKeywordLookup keywordBTable[]=
	{
	{_S("ACK"),KOplAllTargets,TUint8(TOplKeyword::EBack),ETrue,0},
	{_S("EEP"),KOplAllTargets,TUint8(TOplKeyword::EBeep),EFalse,0},
	{_S("EGINTRANS"),KOplNotOpl1993,TUint8(TOplKeyword::EBeginTrans),EFalse,0},	// hp:
	{_S("REAK"),KOplAllTargets,TUint8(TOplKeyword::EBreak),EFalse,0},
	{_S("USY"),KOplAllTargets,TUint8(TOplKeyword::EBusy),EFalse,0}
	};
static const TKeywordLookup keywordCTable[]=
	{
	{_S("ACHE"),KOplOpl1993Only,TUint8(TOplKeyword::ECache),ETrue,0},
	{_S("ACHEHDR"),KOplOpl1993Only,TUint8(TOplKeyword::ECachehdr),EFalse,0},
	{_S("ACHEREC"),KOplOpl1993Only,TUint8(TOplKeyword::ECacherec),EFalse,0},
	{_S("ACHETIDY"),KOplOpl1993Only,TUint8(TOplKeyword::ECachetidy),EFalse,0},
	{_S("ANCEL"),KOplNotOpl1993,TUint8(TOplKeyword::ECancel),ETrue,0},
	{_S("HANGESPRITE"),KOplOpl1993Only,TUint8(TOplKeyword::EChangesprite),EFalse,0},
	{_S("LEARFLAGS"),KOplNotOpl1993,TUint8(TOplKeyword::EClearflags),EFalse,0},
	{_S("LOSE"),KOplAllTargets,TUint8(TOplKeyword::EClose),ETrue,0},
	{_S("LOSESPRITE"),KOplOpl1993Only,TUint8(TOplKeyword::EClosesprite),EFalse,0},
	{_S("LS"),KOplAllTargets,TUint8(TOplKeyword::ECls),EFalse,0},
	{_S("OMMITTRANS"),KOplNotOpl1993,TUint8(TOplKeyword::ECommitTrans),EFalse,0},	// hp:
	{_S("OMPACT"),KOplNotOpl1993,TUint8(TOplKeyword::ECompact),EFalse,0},	// hp: no TRAP
	{_S("OMPRESS"),KOplOpl1993Only,TUint8(TOplKeyword::ECompress),ETrue,0},
	{_S("ONTINUE"),KOplAllTargets,TUint8(TOplKeyword::EContinue),EFalse,0},
	{_S("OPY"),KOplAllTargets,TUint8(TOplKeyword::ECopy),ETrue,0},
	{_S("REATE"),KOplAllTargets,TUint8(TOplKeyword::ECreate),ETrue,0},
	{_S("URSOR"),KOplAllTargets,TUint8(TOplKeyword::ECursor),EFalse,0}
	};

static const TKeywordLookup keywordDTable[]=
	{
	{_S("AYSTODATE"),KOplNotOpl1993,TUint8(TOplKeyword::EDaysToDate),EFalse,0},
	{_S("BUTTONS"),KOplAllTargets,TUint8(TOplKeyword::EDbuttons),EFalse,0},
//	{_S("CHECKBOX"),KOplNotOpl1993,TUint8(TOplKeyword::EDCheckBox),EFalse,0},
	{_S("CHOICE"),KOplAllTargets,TUint8(TOplKeyword::EDchoice),EFalse,0},
	{_S("DATE"),KOplAllTargets,TUint8(TOplKeyword::EDdate),EFalse,0},
	{_S("EDIT"),KOplAllTargets,TUint8(TOplKeyword::EDedit),EFalse,0},
	{_S("EDITMULTI"),KOplNotOpl1993,TUint8(TOplKeyword::EDeditMulti),EFalse,0},
	{_S("EFAULTWIN"),KOplAllTargets,TUint8(TOplKeyword::EDefaultwin),EFalse,0},
	{_S("ELETE"),KOplAllTargets,TUint8(TOplKeyword::EDelete),ETrue,0},
	{_S("FILE"),KOplAllTargets,TUint8(TOplKeyword::EDfile),EFalse,0},
	{_S("FLOAT"),KOplAllTargets,TUint8(TOplKeyword::EDfloat),EFalse,0},
	{_S("IAMINIT"),KOplOpl1993Only,TUint8(TOplKeyword::EDiaminit),EFalse,0},
	{_S("IAMPOS"),KOplOpl1993Only,TUint8(TOplKeyword::EDiampos),EFalse,0},
	{_S("INIT"),KOplAllTargets,TUint8(TOplKeyword::EDinit),EFalse,0},
	{_S("INITS"),KOplOpl1993Only,TUint8(TOplKeyword::EDinits),EFalse,0}, // ONLY ON OPL1994
	{_S("LONG"),KOplAllTargets,TUint8(TOplKeyword::EDlong),EFalse,0},
	{_S("POSITION"),KOplAllTargets,TUint8(TOplKeyword::EDposition),EFalse,0},
	{_S("RAWSPRITE"),KOplOpl1993Only,TUint8(TOplKeyword::EDrawsprite),EFalse,0},
	{_S("TEXT"),KOplAllTargets,TUint8(TOplKeyword::EDtext),EFalse,0},
	{_S("TIME"),KOplAllTargets,TUint8(TOplKeyword::EDtime),EFalse,0},
	{_S("XINPUT"),KOplAllTargets,TUint8(TOplKeyword::EDxinput),EFalse,0}
	};

static const TKeywordLookup keywordETable[]=
	{
	{_S("DIT"),KOplAllTargets,TUint8(TOplKeyword::EEdit),ETrue,0},
	{_S("RASE"),KOplAllTargets,TUint8(TOplKeyword::EErase),ETrue,0},
	{_S("SCAPE"),KOplAllTargets,TUint8(TOplKeyword::EEscape),EFalse,0}
	};
static const TKeywordLookup keywordFTable[]=
	{
	{_S("IRST"),KOplAllTargets,TUint8(TOplKeyword::EFirst),ETrue,0},
	{_S("ONT"),KOplAllTargets,TUint8(TOplKeyword::EFont),EFalse,0},
	{_S("REEALLOC"),KOplAllTargets,TUint8(TOplKeyword::EFreealloc),EFalse,0}
	};
static const TKeywordLookup keywordGTable[]=
	{
	{_S("AT"),KOplAllTargets,TUint8(TOplKeyword::EGat),EFalse,0},
	{_S("BORDER"),KOplAllTargets,TUint8(TOplKeyword::EGborder),EFalse,0},
	{_S("BOX"),KOplAllTargets,TUint8(TOplKeyword::EGbox),EFalse,0},
	{_S("BUTTON"),KOplAllTargets,TUint8(TOplKeyword::EGbutton),EFalse,0},
	{_S("CIRCLE"),KOplNotOpl1993,TUint8(TOplKeyword::EGcircle),EFalse,0},
	{_S("CLOCK"),KOplAllTargets,TUint8(TOplKeyword::EGclock),EFalse,0},
	{_S("CLOSE"),KOplAllTargets,TUint8(TOplKeyword::EGclose),ETrue,0},
	{_S("CLS"),KOplAllTargets,TUint8(TOplKeyword::EGcls),EFalse,0},
	{_S("COLOR"),KOplNotOpl1993,TUint8(TOplKeyword::EGcolor),EFalse,0},
	{_S("COLORBACKGROUND"),KOplNotOpl1993,TUint8(TOplKeyword::EGcolorbackground),EFalse,0},
	{_S("COLORINFO"),KOplNotOpl1993,TUint8(TOplKeyword::EGcolorinfo),EFalse,0},
	{_S("COPY"),KOplAllTargets,TUint8(TOplKeyword::EGcopy),ETrue,0},
	{_S("DRAWOBJECT"),KOplOpl1993Only,TUint8(TOplKeyword::EGdrawobject),EFalse,0},
	{_S("ELLIPSE"),KOplNotOpl1993,TUint8(TOplKeyword::EGellipse),EFalse,0},
	{_S("ETEVENT"),KOplAllTargets,TUint8(TOplKeyword::EGetevent),EFalse,0},
	{_S("ETEVENT32"),KOplNotOpl1993,TUint8(TOplKeyword::EGetevent32),EFalse,0},
	{_S("ETEVENTA32"),KOplNotOpl1993,TUint8(TOplKeyword::EGeteventa32),EFalse,0},
	{_S("FILL"),KOplAllTargets,TUint8(TOplKeyword::EGfill),EFalse,0},
	{_S("FONT"),KOplAllTargets,TUint8(TOplKeyword::EGfont),ETrue,0},
	{_S("GMODE"),KOplAllTargets,TUint8(TOplKeyword::EGgmode),EFalse,0},
	{_S("GREY"),KOplAllTargets,TUint8(TOplKeyword::EGgrey),EFalse,0},
	{_S("INFO"),KOplOpl1993Only,TUint8(TOplKeyword::EGinfo),EFalse,0},
	{_S("INFO32"),KOplNotOpl1993,TUint8(TOplKeyword::EGinfo32),EFalse,0},
	{_S("INVERT"),KOplAllTargets,TUint8(TOplKeyword::EGinvert),EFalse,0},
	{_S("IPRINT"),KOplAllTargets,TUint8(TOplKeyword::EGiprint),EFalse,0},
	{_S("LINEBY"),KOplAllTargets,TUint8(TOplKeyword::EGlineby),EFalse,0},
	{_S("LINETO"),KOplAllTargets,TUint8(TOplKeyword::EGlineto),EFalse,0},
	{_S("MOVE"),KOplAllTargets,TUint8(TOplKeyword::EGmove),EFalse,0},
	{_S("ORDER"),KOplAllTargets,TUint8(TOplKeyword::EGorder),EFalse,0},
	{_S("OTO"),KOplAllTargets,TUint8(TOplKeyword::EGoto),EFalse,0},
	{_S("OTOMARK"),KOplNotOpl1993,TUint8(TOplKeyword::EGotoMark),EFalse,0},
	{_S("PATT"),KOplAllTargets,TUint8(TOplKeyword::EGpatt),ETrue,0},
	{_S("PEEKLINE"),KOplAllTargets,TUint8(TOplKeyword::EGpeekline),EFalse,0},
	{_S("POLY"),KOplAllTargets,TUint8(TOplKeyword::EGpoly),EFalse,0},
	{_S("PRINT"),KOplAllTargets,TUint8(TOplKeyword::EGprint),EFalse,0},
	{_S("PRINTB"),KOplAllTargets,TUint8(TOplKeyword::EGprintb),EFalse,0},
	{_S("SAVEBIT"),KOplAllTargets,TUint8(TOplKeyword::EGsavebit),ETrue,0},
	{_S("SCROLL"),KOplAllTargets,TUint8(TOplKeyword::EGscroll),EFalse,0},
	{_S("SETPENWIDTH"),KOplNotOpl1993,TUint8(TOplKeyword::EgSetPenWidth),EFalse,0},
	{_S("SETWIN"),KOplAllTargets,TUint8(TOplKeyword::EGsetwin),EFalse,0},
	{_S("STYLE"),KOplAllTargets,TUint8(TOplKeyword::EGstyle),EFalse,0},
	{_S("TMODE"),KOplAllTargets,TUint8(TOplKeyword::EGtmode),EFalse,0},
	{_S("UNLOADFONT"),KOplAllTargets,TUint8(TOplKeyword::EGunloadfont),ETrue,0},
	{_S("UPDATE"),KOplAllTargets,TUint8(TOplKeyword::EGupdate),EFalse,0},
	{_S("USE"),KOplAllTargets,TUint8(TOplKeyword::EGuse),ETrue,0},
	{_S("VISIBLE"),KOplAllTargets,TUint8(TOplKeyword::EGvisible),EFalse,0},
	{_S("XBORDER"),KOplAllTargets,TUint8(TOplKeyword::EGxborder),EFalse,0},
	{_S("XPRINT"),KOplAllTargets,TUint8(TOplKeyword::EGxprint),EFalse,0},
	{_S("XBORDER32"),KOplNotOpl1993,TUint8(TOplKeyword::EGxborder32),EFalse,0},
	};
static const TKeywordLookup keywordITable[]=
	{
	{_S("NPUT"),KOplAllTargets,TUint8(TOplKeyword::EInput),ETrue,0},
	{_S("NSERT"),KOplNotOpl1993,TUint8(TOplKeyword::EInsert),ETrue,0},
	{_S("OSIGNAL"),KOplAllTargets,TUint8(TOplKeyword::EIosignal),ETrue,0},
	{_S("OWAITSTAT"),KOplAllTargets,TUint8(TOplKeyword::EIowaitstat),EFalse,0},
	{_S("OWAITSTAT32"),KOplNotOpl1993,TUint8(TOplKeyword::EIowaitstat32),EFalse,0},
	{_S("OYIELD"),KOplAllTargets,TUint8(TOplKeyword::EIoyield),EFalse,0}
	};
static const TKeywordLookup keywordKTable[]=
	{
	{_S("ILLMARK"),KOplNotOpl1993,TUint8(TOplKeyword::EKillMark),EFalse,0}
	};
static const TKeywordLookup keywordLTable[]=
	{
	{_S("AST"),KOplAllTargets,TUint8(TOplKeyword::ELast),ETrue,0},
	{_S("CLOSE"),KOplAllTargets,TUint8(TOplKeyword::ELclose),ETrue,0},
	{_S("INKLIB"),KOplOpl1993Only,TUint8(TOplKeyword::ELinklib),EFalse,0},
	{_S("OADM"),KOplAllTargets,TUint8(TOplKeyword::ELoadm),ETrue,0},
	{_S("OCK"),KOplAllTargets,TUint8(TOplKeyword::ELock),EFalse,0},
	{_S("OPEN"),KOplAllTargets,TUint8(TOplKeyword::ELopen),ETrue,0},
	{_S("PRINT"),KOplAllTargets,TUint8(TOplKeyword::ELprint),EFalse,0}
	};
static const TKeywordLookup keywordMTable[]=
	{
	{_S("CARD"),KOplAllTargets,TUint8(TOplKeyword::EMcard),EFalse,0},
	{_S("CARDX"),KOplNotOpl1993,TUint8(TOplKeyword::EMcardX),EFalse,0},
	{_S("CASC"),KOplNotOpl1993,TUint8(TOplKeyword::EMcasc),EFalse,0},
	{_S("INIT"),KOplAllTargets,TUint8(TOplKeyword::EMinit),EFalse,0},
	{_S("KDIR"),KOplAllTargets,TUint8(TOplKeyword::EMkdir),ETrue,0},
	{_S("ODIFY"),KOplNotOpl1993,TUint8(TOplKeyword::EModify),ETrue,0}
	};
static const TKeywordLookup keywordNTable[]=
	{
	{_S("EXT"),KOplAllTargets,TUint8(TOplKeyword::ENext),ETrue,0}
	};
static const TKeywordLookup keywordOTable[]=
	{
	{_S("DBINFO"),KOplOpl1993Only,TUint8(TOplKeyword::EOdbinfo),EFalse,0},
	{_S("FF"),KOplAllTargets,TUint8(TOplKeyword::EOff),EFalse,0},
	{_S("NERR"),KOplAllTargets,TUint8(TOplKeyword::EOnerr),EFalse,0},
	{_S("PEN"),KOplAllTargets,TUint8(TOplKeyword::EOpen),ETrue,0},
	{_S("PENR"),KOplAllTargets,TUint8(TOplKeyword::EOpenr),ETrue,0},
	};
static const TKeywordLookup keywordPTable[]=
	{
	{_S("AUSE"),KOplAllTargets,TUint8(TOplKeyword::EPause),EFalse,0},
	{_S("OKE$"),KOplAllTargets,TUint8(TOplKeyword::EPokes),EFalse,0},
	{_S("OKEB"),KOplAllTargets,TUint8(TOplKeyword::EPokeb),EFalse,0},
	{_S("OKEF"),KOplAllTargets,TUint8(TOplKeyword::EPokef),EFalse,0},
	{_S("OKEL"),KOplAllTargets,TUint8(TOplKeyword::EPokel),EFalse,0},
	{_S("OKEW"),KOplAllTargets,TUint8(TOplKeyword::EPokew),EFalse,0},
	{_S("OINTERFILTER"),KOplNotOpl1993,TUint8(TOplKeyword::EPointerFilter),EFalse,0},
	{_S("OSITION"),KOplAllTargets,TUint8(TOplKeyword::EPosition),ETrue,0},
	{_S("OSSPRITE"),KOplOpl1993Only,TUint8(TOplKeyword::EPossprite),EFalse,0},
	{_S("RINT"),KOplAllTargets,TUint8(TOplKeyword::EPrint),EFalse,0},
	{_S("UT"),KOplNotOpl1993,TUint8(TOplKeyword::EPut),ETrue,0}
	};
static const TKeywordLookup keywordRTable[]=
	{
	{_S("AISE"),KOplAllTargets,TUint8(TOplKeyword::ERaise),ETrue,0},	// TRAP RAISE err% allowed
	{_S("ANDOMIZE"),KOplAllTargets,TUint8(TOplKeyword::ERandomize),EFalse,0},
	{_S("ENAME"),KOplAllTargets,TUint8(TOplKeyword::ERename),ETrue,0},
	{_S("ETURN"),KOplAllTargets,TUint8(TOplKeyword::EReturnValue),EFalse,0},
	{_S("MDIR"),KOplAllTargets,TUint8(TOplKeyword::ERmdir),ETrue,0},
	{_S("OLLBACK"),KOplNotOpl1993,TUint8(TOplKeyword::ERollBack),EFalse,0}		// hp:
	};
static const TKeywordLookup keywordSTable[]=
	{
	{_S("CREEN"),KOplAllTargets,TUint8(TOplKeyword::EScreen),EFalse,0},
	{_S("CREENINFO"),KOplAllTargets,TUint8(TOplKeyword::EScreeninfo),EFalse,0},
	{_S("ECSTODATE"),KOplAllTargets,TUint8(TOplKeyword::ESecstodate),EFalse,0},
	{_S("ETDOC"),KOplNotOpl1993,TUint8(TOplKeyword::ESetdoc),EFalse,0},
	{_S("ETFLAGS"),KOplNotOpl1993,TUint8(TOplKeyword::ESetflags),EFalse,0},
	{_S("ETHELP"),KOplNotOpl1993,TUint8(TOplKeyword::ESetHelp),EFalse,0},
	{_S("ETHELPUID"),KOplNotOpl1993,TUint8(TOplKeyword::ESetHelpUid),EFalse,0},
	{_S("ETNAME"),KOplOpl1993Only,TUint8(TOplKeyword::ESetname),EFalse,0},
	{_S("ETPATH"),KOplAllTargets,TUint8(TOplKeyword::ESetpath),EFalse,0},
	{_S("HOWHELP"),KOplNotOpl1993,TUint8(TOplKeyword::EShowHelp),EFalse,0},
	{_S("TATUSWIN"),KOplOpl1993Only,TUint8(TOplKeyword::EStatuswin),EFalse,0},
	{_S("TOP"),KOplAllTargets,TUint8(TOplKeyword::EStop),EFalse,0},
	{_S("TYLE"),KOplAllTargets,TUint8(TOplKeyword::EStyle),EFalse,0}
	};
static const TKeywordLookup keywordTTable[]=
	{
	{_S("RAP"),KOplAllTargets,TUint8(TOplKeyword::ETrap),EFalse,0}
	};
static const TKeywordLookup keywordUTable[]=
	{
	{_S("NLOADM"),KOplAllTargets,TUint8(TOplKeyword::EUnloadm),ETrue,0},
	{_S("PDATE"),KOplAllTargets,TUint8(TOplKeyword::EUpdate),ETrue,0},
	{_S("SE"),KOplAllTargets,TUint8(TOplKeyword::EUse),ETrue,0},
	{_S("SESPRITE"),KOplOpl1993Only,TUint8(TOplKeyword::EUsesprite),EFalse,0}
	};

////////////////////////////////////////////////////////////////////////
//
// Reserved word lookup tables
//
////////////////////////////////////////////////////////////////////////
static const TReservedLookup reservedATable[]=
	{
	{_S("LIAS"),KOplNotOpl1993,TUint8(TOplToken::EAlias&0xff),TUint8(TOplToken::EAlias>>8),0},
	{_S("ND"),KOplAllTargets,TUint8(TOplToken::EAnd&0xff),TUint8(TOplToken::EAnd>>8),0},
	{_S("PP"),KOplAllTargets,TUint8(TOplToken::EApp&0xff),TUint8(TOplToken::EApp>>8),0}
	};
static const TReservedLookup reservedBTable[]=
	{
	{_S("YREF"),KOplNotOpl1993,TUint8(TOplToken::EByRef&0xff),TUint8(TOplToken::EByRef>>8),0}
	};
static const TReservedLookup reservedCTable[]=
	{
	{_S("APTION"),KOplNotOpl1993,TUint8(TOplToken::ECaption&0xff),TUint8(TOplToken::ECaption>>8),0},
	{_S("ONST"),KOplNotOpl1993,TUint8(TOplToken::EConst&0xff),TUint8(TOplToken::EConst>>8),0}
	};
static const TReservedLookup reservedDTable[]=
	{
	{_S("O"),KOplAllTargets,TUint8(TOplToken::EDo&0xff),TUint8(TOplToken::EDo>>8),0},
	{_S("ECLARE"),KOplNotOpl1993,TUint8(TOplToken::EDeclare&0xff),TUint8(TOplToken::EDeclare>>8),0}
	};
static const TReservedLookup reservedETable[]=
	{
	{_S("ND"),KOplNotOpl1993,TUint8(TOplToken::EEnd&0xff),TUint8(TOplToken::EEnd>>8),0},
	{_S("LSE"),KOplAllTargets,TUint8(TOplToken::EElse&0xff),TUint8(TOplToken::EElse>>8),0},
	{_S("LSEIF"),KOplAllTargets,TUint8(TOplToken::EElseIf&0xff),TUint8(TOplToken::EElseIf>>8),0},
	{_S("NDA"),KOplAllTargets,TUint8(TOplToken::EEndA&0xff),TUint8(TOplToken::EEndA>>8),0},
	{_S("NDIF"),KOplAllTargets,TUint8(TOplToken::EEndIf&0xff),TUint8(TOplToken::EEndIf>>8),0},
	{_S("NDP"),KOplAllTargets,TUint8(TOplToken::EEndP&0xff),TUint8(TOplToken::EEndP>>8),0},
	{_S("NDV"),KOplAllTargets,TUint8(TOplToken::EEndV&0xff),TUint8(TOplToken::EEndV>>8),0},
	{_S("NDWH"),KOplAllTargets,TUint8(TOplToken::EEndWh&0xff),TUint8(TOplToken::EEndWh>>8),0},
	{_S("XT"),KOplOpl1993Only,TUint8(TOplToken::EExt&0xff),TUint8(TOplToken::EExt>>8),0},
	{_S("XTERNAL"),KOplNotOpl1993,TUint8(TOplToken::EExternal&0xff),TUint8(TOplToken::EExternal>>8),0}
	};
static const TReservedLookup reservedFTable[]=
	{
	{_S("LAGS"),KOplNotOpl1993,TUint8(TOplToken::EFlags&0xff),TUint8(TOplToken::EFlags>>8),0}
	};
static const TReservedLookup reservedGTable[]=
	{
	{_S("LOBAL"),KOplAllTargets,TUint8(TOplToken::EGlobal&0xff),TUint8(TOplToken::EGlobal>>8),0}
	};
static const TReservedLookup reservedITable[]=
	{
	{_S("CON"),KOplAllTargets,TUint8(TOplToken::EIcon&0xff),TUint8(TOplToken::EIcon>>8),0},
	{_S("F"),KOplAllTargets,TUint8(TOplToken::EIf&0xff),TUint8(TOplToken::EIf>>8),0},
	{_S("NCLUDE"),KOplNotOpl1993,TUint8(TOplToken::EInclude&0xff),TUint8(TOplToken::EInclude>>8),0}
	};
static const TReservedLookup reservedLTable[]=
	{
	{_S("OCAL"),KOplAllTargets,TUint8(TOplToken::ELocal&0xff),TUint8(TOplToken::ELocal>>8),0}
	};
static const TReservedLookup reservedMTable[]=
	{
	{_S("IME"),KOplNotOpl1993,TUint8(TOplToken::EMime&0xff),TUint8(TOplToken::EMime>>8),0}
	};
static const TReservedLookup reservedNTable[]=
	{
	{_S("OT"),KOplAllTargets,TUint8(TOplToken::ENot&0xff),TUint8(TOplToken::ENot>>8),0}
	};
static const TReservedLookup reservedOTable[]=
	{
	{_S("R"),KOplAllTargets,TUint8(TOplToken::EOr&0xff),TUint8(TOplToken::EOr>>8),0},
	{_S("N"),KOplAllTargets,TUint8(TOplToken::EOn&0xff),TUint8(TOplToken::EOn>>8),0},
	{_S("PX"),KOplNotOpl1993,TUint8(TOplToken::EOpx&0xff),TUint8(TOplToken::EOpx>>8),0}
	};
static const TReservedLookup reservedPTable[]=
	{
	{_S("ATH"),KOplOpl1993Only,TUint8(TOplToken::EPath&0xff),TUint8(TOplToken::EPath>>8),0},
	{_S("ROC"),KOplAllTargets,TUint8(TOplToken::EProc&0xff),TUint8(TOplToken::EProc>>8),0}
	};
static const TReservedLookup reservedRTable[]=
	{
	{_S("EM"),KOplAllTargets,TUint8(TOplToken::EEos&0xff),TUint8(TOplToken::EEos>>8),0}
	};
static const TReservedLookup reservedTTable[]=
	{
	{_S("YPE"),KOplOpl1993Only,TUint8(TOplToken::EType&0xff),TUint8(TOplToken::EType>>8),0}
	};
static const TReservedLookup reservedUTable[]=
	{
	{_S("NTIL"),KOplAllTargets,TUint8(TOplToken::EUntil&0xff),TUint8(TOplToken::EUntil>>8),0}
	};
static const TReservedLookup reservedVTable[]=
	{
	{_S("ECTOR"),KOplAllTargets,TUint8(TOplToken::EVector&0xff),TUint8(TOplToken::EVector>>8),0}
	};
static const TReservedLookup reservedWTable[]=
	{
	{_S("HILE"),KOplAllTargets,TUint8(TOplToken::EWhile&0xff),TUint8(TOplToken::EWhile>>8),0}
	};
/////////////////////////////////////////////////////////////////////////
//
// The root table. One entry per latter of the latin alphabet.
//
/////////////////////////////////////////////////////////////////////////
struct TInitialLetterTableEntry
	{
	const TFunctionLookup *iFunctions;
	TInt iFunctionsCount;
	const TKeywordLookup *iKeywords;
	TInt iKeywordsCount;
	const TReservedLookup *iReserved;
	TInt iReservedCount;
	};

const TInt KAlphabetTableSize=25; // One for each letter of the latin alphabet, except Z for there are no Z's
static const TInitialLetterTableEntry reservedLookupTable[KAlphabetTableSize]=
	{
	{functionATable, sizeof(functionATable)/sizeof(TFunctionLookup),keywordATable, sizeof(keywordATable)/sizeof(TKeywordLookup),reservedATable, sizeof(reservedATable)/sizeof(TReservedLookup)},
	{functionBTable, sizeof(functionBTable)/sizeof(TFunctionLookup),keywordBTable, sizeof(keywordBTable)/sizeof(TKeywordLookup),reservedBTable, sizeof(reservedBTable)/sizeof(TReservedLookup)},
	{functionCTable, sizeof(functionCTable)/sizeof(TFunctionLookup),keywordCTable, sizeof(keywordCTable)/sizeof(TKeywordLookup),reservedCTable, sizeof(reservedCTable)/sizeof(TReservedLookup)},
	{functionDTable, sizeof(functionDTable)/sizeof(TFunctionLookup),keywordDTable, sizeof(keywordDTable)/sizeof(TKeywordLookup),reservedDTable, sizeof(reservedDTable)/sizeof(TReservedLookup)},
	{functionETable, sizeof(functionETable)/sizeof(TFunctionLookup),keywordETable, sizeof(keywordETable)/sizeof(TKeywordLookup),reservedETable, sizeof(reservedETable)/sizeof(TReservedLookup)},
	{functionFTable, sizeof(functionFTable)/sizeof(TFunctionLookup),keywordFTable, sizeof(keywordFTable)/sizeof(TKeywordLookup),reservedFTable, sizeof(reservedFTable)/sizeof(TReservedLookup)},
	{functionGTable, sizeof(functionGTable)/sizeof(TFunctionLookup),keywordGTable, sizeof(keywordGTable)/sizeof(TKeywordLookup),reservedGTable, sizeof(reservedGTable)/sizeof(TReservedLookup)},
	{functionHTable, sizeof(functionHTable)/sizeof(TFunctionLookup),         NULL,                                            0,          NULL,                                              0},
	{functionITable, sizeof(functionITable)/sizeof(TFunctionLookup),keywordITable, sizeof(keywordITable)/sizeof(TKeywordLookup),reservedITable, sizeof(reservedITable)/sizeof(TReservedLookup)},
	{          NULL,                                              0,         NULL,                                            0,          NULL,                                              0},
	{functionKTable, sizeof(functionKTable)/sizeof(TFunctionLookup),keywordKTable, sizeof(keywordKTable)/sizeof(TKeywordLookup),          NULL,                                              0},
	{functionLTable, sizeof(functionLTable)/sizeof(TFunctionLookup),keywordLTable, sizeof(keywordLTable)/sizeof(TKeywordLookup),reservedLTable, sizeof(reservedLTable)/sizeof(TReservedLookup)},
	{functionMTable, sizeof(functionMTable)/sizeof(TFunctionLookup),keywordMTable, sizeof(keywordMTable)/sizeof(TKeywordLookup),reservedMTable, sizeof(reservedMTable)/sizeof(TReservedLookup)},
	{functionNTable, sizeof(functionNTable)/sizeof(TFunctionLookup),keywordNTable, sizeof(keywordNTable)/sizeof(TKeywordLookup),reservedNTable, sizeof(reservedNTable)/sizeof(TReservedLookup)},
	{functionOTable, sizeof(functionOTable)/sizeof(TFunctionLookup),keywordOTable, sizeof(keywordOTable)/sizeof(TKeywordLookup),reservedOTable, sizeof(reservedOTable)/sizeof(TReservedLookup)},
	{functionPTable, sizeof(functionPTable)/sizeof(TFunctionLookup),keywordPTable, sizeof(keywordPTable)/sizeof(TKeywordLookup),reservedPTable, sizeof(reservedPTable)/sizeof(TReservedLookup)},
	{          NULL,                                              0,         NULL,                                            0,          NULL,                                              0},
	{functionRTable, sizeof(functionRTable)/sizeof(TFunctionLookup),keywordRTable, sizeof(keywordRTable)/sizeof(TKeywordLookup),reservedRTable, sizeof(reservedRTable)/sizeof(TReservedLookup)},
	{functionSTable, sizeof(functionSTable)/sizeof(TFunctionLookup),keywordSTable, sizeof(keywordSTable)/sizeof(TKeywordLookup),          NULL,                                              0},
	{functionTTable, sizeof(functionTTable)/sizeof(TFunctionLookup),keywordTTable, sizeof(keywordTTable)/sizeof(TKeywordLookup),reservedTTable, sizeof(reservedTTable)/sizeof(TReservedLookup)},
	{functionUTable, sizeof(functionUTable)/sizeof(TFunctionLookup),keywordUTable, sizeof(keywordUTable)/sizeof(TKeywordLookup),reservedUTable, sizeof(reservedUTable)/sizeof(TReservedLookup)},
	{functionVTable, sizeof(functionVTable)/sizeof(TFunctionLookup),         NULL,                                            0,reservedVTable, sizeof(reservedVTable)/sizeof(TReservedLookup)},
	{functionWTable, sizeof(functionWTable)/sizeof(TFunctionLookup),         NULL,                                            0,reservedWTable, sizeof(reservedWTable)/sizeof(TReservedLookup)},
	{          NULL,                                              0,         NULL,                                            0,          NULL,                                              0},
	{functionYTable, sizeof(functionYTable)/sizeof(TFunctionLookup),         NULL,                                            0,          NULL,                                              0}
    };


/////////////////////////////////////////////////////////////////////////
//
// COplLexBase - Code to lookup reserved words.
//
/////////////////////////////////////////////////////////////////////////


TBool COplLexerBase::IsReserved()
//
// Checks to see if the parsed string is reserved
// FOR NOW 
	{

	TUint index=iText[0]-'A';
	if (index>=(TUint)KAlphabetTableSize)
		return EFalse;

	TPtrC end=iText.Mid(1); // Take the remains after the first letter
	const TInitialLetterTableEntry& tables=reservedLookupTable[index];

    TBool isReserved=EFalse;
	// Functions first
		{
		const TFunctionLookup *pF=tables.iFunctions;
		const TFunctionLookup *pFe=pF+tables.iFunctionsCount;
		while (pF<pFe)
			{
			if (((TLookupHeader *)pF)->Matches(iTarget,end)) // See comment by TFunctionHeader
				{
				isReserved=ETrue;
				iToken=TOplToken::EFunction;
				iType=pF->ReturnType();
				new(iFunction) TOplFunction(pF->Number());
				TOplSignature& signature=((TOplFunction *)iFunction)->Signature();
				signature.Initialize(iType);
				if (!pF->IsStatistical()) // Need to add the argument types
					{
					for (TInt argCount=0;;argCount++)
						{
						if (pF->IsAllowedCount(argCount))
							signature.AllowArgumentCount(argCount);

						TOplSignature::TArgType type=pF->Arg(argCount);
						if (type==TOplSignature::EArgBadType)
							break;
						signature.AddArgument(argCount,type);
						}
					}
				break;
				}
			pF++;
			}
		}
	if (!isReserved)
		{
		const TKeywordLookup *pK=tables.iKeywords;
		const TKeywordLookup *pKe=pK+tables.iKeywordsCount;
		while (pK<pKe)
			{
			if (((TLookupHeader *)pK)->Matches(iTarget,end))
				{
				isReserved=ETrue;
				iToken=TOplToken::EKeywordToken;
				new(iKeyword) TOplKeyword(pK->Trappable(),pK->Code());
				break;
				}
			pK++;
			}
		}
	if (!isReserved)
		{
		const TReservedLookup *pR=tables.iReserved;
		const TReservedLookup *pRe=pR+tables.iReservedCount;
		while (pR<pRe)
			{
			if (((TLookupHeader *)pR)->Matches(iTarget,end))
				{
				isReserved=ETrue;
				iToken=pR->Token();
				break;
				}
			pR++;
			}
		}
	return isReserved;
	}