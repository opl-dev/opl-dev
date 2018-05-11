// OPLTOKEN.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.
//
// Various classes used to describe tokens returned by the OPL lexer.

#ifndef __OPLTOKEN_H__
#define __OPLTOKEN_H__

#include <e32std.h>
#include <s32strm.h>

#if defined(__VC32__) && _MSC_VER==1100
// Disable MSVC++ 5.0 aggressive warnings about non-expansion of inline functions. 
#pragma warning(disable : 4710)	// function '...' not expanded
#endif

// NOTE - The order of these enums is often significant. Do not rearrange
// the tokens unless you're sure you mean to..

/////////////////////////////////////////////////////////////////////////
//
// TOplToken - Basic token type returned by the lexer
//
/////////////////////////////////////////////////////////////////////////
const TUint KOplTokenMask=0x00000fff; // NOTE - designed to fit in 16 bits
const TUint KOplTokenShift=12;
class TOplToken
	{
public:
	enum TClass
		{
		EOperator,
		EIdentifier,
		EPunctuation,
		ECall,      // Function, procedure or 
		EKeyword,   // Reserved keywords that can be used in statements
		EStructure,      // DO, IF, WHILE - Start of loops/conditional
		EReserved   // All other reserved words 
		};
	enum TValue
		{
		EBadToken=-1,

		EStartOfExpression=(EOperator<<KOplTokenShift),
		ELessThan,
		ELessThanEq,
		EGreaterThan,
		EGreaterThanEq,
		EEqual,
		ENotEqual,
		EPlus,
		EMinus,
		EMultiply,
		EDivide,
		EPower,
		EAnd,
		EOr,
		ENot,
		EUnaryMinus,
		EPerc,
		EOpenBracket,
		ECloseBracket,
		EEndOfExpression,
		EPercLess,
		EPercGreater,
		EPercPlus,
		EPercMinus,
		EPercMultiply,													
		EPercDivide,

		EConstant=(EIdentifier<<KOplTokenShift),
		ESimple,
		EArray,
		EField,
		ELabel,

		EEos=(EPunctuation<<KOplTokenShift),
		EColon,
		EEof,
		ESemiColon,
		EComma,
		EHash,

		EFunction=(ECall<<KOplTokenShift),
		EProcId,
		EProcByName,
		EOpxFunction,

		EKeywordToken=(EKeyword<<KOplTokenShift), // PRINT, BEEP etc

		EDo=(EStructure<<KOplTokenShift), 
		EWhile,
		EIf,
		EVector,

		EAlias=(EReserved<<KOplTokenShift),
		EApp,
		EByRef,
		ECaption,
		EConst,
		EDeclare,
		EElse,
		EElseIf,
		EEnd,
		EEndA,
		EEndIf,
		EEndP,
		EEndV,
		EEndWh,
		EExt,
		EExternal,
		ELocal,
		EGlobal,
		EInclude,
		EIcon,
		EOn,
		EOpx,
		EPath,
		EProc,
		ERem,
		EType,
		EUntil,
		EFlags,
		EMime
		};
	enum TType
		{
		EBadType=(-1),
		EWord,
		ELong,
		EReal,
		EString,
		EUWord,
		EMaxNormalType,
		ENativeReturn // Used to indicate that a function returns the native type in the lookup tables - filterd out in expression evaluator
		};
	
	inline TOplToken() : iValue(EBadToken) {};
	inline TOplToken(TValue aValue) : iValue(aValue) {};
	inline operator TOplToken::TValue() const {return iValue;} 
	inline TOplToken::TClass Class() {return TClass(iValue>>KOplTokenShift);}
	inline TUint Number() {return iValue&KOplTokenMask;}
//	inline TOplToken& operator=(TValue aValue) {iValue=aValue; return *this;}
//	inline TOplToken& operator=(const TOplToken aToken) {iValue=aToken.iValue; return *this;}
	inline void ExternalizeL(RWriteStream& aStream) const {aStream.WriteUint16L(iValue);}
private:
	TValue iValue;
	};

/////////////////////////////////////////////////////////////
//
// TOplConstant
//
/////////////////////////////////////////////////////////////
class TOplConstant
	{
public:
	inline TOplConstant() {}
	inline TOplConstant(TInt16 aWord) {SetWord(aWord);}
	inline TOplConstant(TInt32 aLong) {SetLong(aLong);}
	inline TOplConstant(TReal64 aReal) {SetReal(aReal);}
	inline TOplConstant(const TDesC& aDes) {SetString(aDes);}
	inline TOplConstant& operator=(TInt16 aWord) {SetWord(aWord); return *this;}
	inline TOplConstant& operator=(TInt32 aLong) {SetLong(aLong); return *this;}
	inline TOplConstant& operator=(TReal64 aReal) {SetReal(aReal); return *this;}
	inline TOplConstant& operator=(const TDesC& aDes) {SetString(aDes); return *this;}

	inline TOplToken::TType Type() const { return iType;}
	IMPORT_C TInt16 AsWordL() const;
	IMPORT_C TInt32 AsLongL() const;
	IMPORT_C TReal64 AsRealL() const;
	IMPORT_C const TDesC& AsStringL() const;
	IMPORT_C void NegateL();
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
private:
	void SetWord(TInt aWord);
	void SetLong(TInt aLong);
	void SetReal(TReal64 aReal);
	void SetString(const TDesC& aDes);
private:
	TOplToken::TType iType;
	union
		{
		TInt iInt;
		TReal64 iReal;
		const TDesC* iString;
		};
	};

class TOplField
	{
public:
	enum TDevice
		{
		EBadDevice=-1,
		EDeviceA,EDeviceB,EDeviceC,EDeviceD,EDeviceE,EDeviceF,EDeviceG,
		EDeviceH,EDeviceI,EDeviceJ,EDeviceK,EDeviceL,EDeviceM,EDeviceN,
		EDeviceO,EDeviceP,EDeviceQ,EDeviceR,EDeviceS,EDeviceT,EDeviceU,
		EDeviceV,EDeviceW,EDeviceX,EDeviceY,EDeviceZ
		};
	inline TOplField::TDevice LogicalDevice() const { return iDevice;}
	inline const TDesC& Name() const { return iName;}	
	void ExternalizeL(RWriteStream& aStream) const;
private:
	friend class COplLexerBase;
	TOplField(TDevice aDevice,const TDesC& aName); // In ot_util.cpp
	TOplField& operator=(const TOplField& aField);
private:
	TDevice iDevice;
	TPtrC iName;
	};

const TInt KOplMaxArgCount=32;
class TOplSignature // Used to keep the signature for functions, external references (and procedures?)
	{
public:
	enum TArgType
		{
		EArgBadType=(-1),
		EArgWord=TOplToken::EWord,
		EArgLong,
		EArgReal,
		EArgString,
		EArgUword,
		EArgWordRef,
		EArgLongRef,
		EArgRealRef,
		EArgStringRef,
		EArgAnyRef,
		EArgOverLoad, // Special case for functions where the argument type changes the function number
		EArgNative,
		EArgUNative
		};

	void Initialize(TOplToken::TType aReturnType);
	void SetArgumentCount(TUint anArgumentCount);
	void AllowArgumentCount(TUint anArgCount);
	void AllowArgumentCountByPass(TUint anArgCount); // Horriblekludge to deal with overloaded function madness.
	void AddArgument(TUint anArgument,TArgType aType);
	TInt operator==(const TOplSignature& aSignature) const;

	inline TOplToken::TType ReturnType() const { return TOplToken::TType(iReturnType);}
	inline TBool NeedsArgumentCount() const {return iNeedsCount;}
	inline TBool IsStatistical() const { return !iPermittedArgumentCounts;}
	inline TBool IsPermittedArgumentCount(TInt anArgumentCount) const  { return (1<<anArgumentCount)&iPermittedArgumentCounts;}
	inline TUint ArgumentCount() const { return iArgumentCount;}
	inline TInt operator!=(const TOplSignature& aSignature) const {return !(*this==aSignature);}
	TOplSignature::TArgType operator[](TInt argNumber) const;
private:
	TUint8 iReturnType;	
	TUint8 iNeedsCount;
	TUint16 iArgumentCount; // Used for procedure refeerences for putting the archaic argument count out in the procedure look-up table
	TUint32 iPermittedArgumentCounts;
	TUint8 iArgTypes[KOplMaxArgCount];
	};

class TOplFunction
	{
public:
	enum TNumber // Function numbers - Add new ones only at the end and 
		{        // update the backends accordingly
		EAddr,
		EAsc,
		ECall,
		ECount,
		EDay,
		EDow,
		EEof,
		EErr,
		EExist,
		EFind,
		EGet,
		EIoa,
		EIow,
		EIoOpen,
		EIoWrite,
		EIoRead,
		EIoClose,
		EIoWait,
		EHour,
		EKey,
		ELen,
		ELoc,
		EMinute,
		EMonth,
		EPeekB,
		EPeekW,
		Epos,
		ERecSize,
		ESecond,
		EUser,
		EYear,
		EWeek,
		EIoSeek,
		EKMod,
		EKeyA,
		EKeyC,
		EgCreate,
		EgCreateBit,
		EgLoadBit,
		EgLoadFont,
		EgRank,
		EgIdentity,
		EgX,
		EgY,
		EgWidth,
		EgHeight,
		EgOriginX,
		EgOriginY,
		EgTWidth,
		EgPrintClip,
		ETestEvent,
		EOs,
		EMenu,
		EDialog,
		EAlert,
		ECreateSprite,
		ELoadLib,
		EUnloadLib,
		EFindLib,
		EGetLibH,
		ENewObj,
		ENewObjH,
		ESend,
		EEnterSend,
		EEnterSend0,
		EAlloc,
		EReAlloc,
		EAdjustAlloc,
		ELenAlloc,
		EIoc,
		EUAdd,
		EUSub,
		EIoCancel,
		EStatWinInfo,
		EFindField,
		EDays,
		EIAbs,
		EInt,
		EpeekL,
		ESpace,
		EDateToSecs,
		EAbs,
		EACos,
		EASin,
		EATan,
		ECos,
		EDeg,
		EExp,
		EFlt,
		EIntF,
		ELn,
		ELog,
		EPeekF,
		EPi,
		ERad,
		ERnd,
		ESin,
		ESqr,
		ETan,
		EVal,
		EMax,
		EMean,
		EMin,
		EStd,
		ESum,
		EVar,
		EEval,
		EChrS,
		EDatim,
		EDayNameS,
		EDirS,
		EErrS,
		EFixS,
		EGen,
		EGetS,
		EHexS,
		EKeyS,
		ELeftS,
		ELowerS,
		EMidS,
		EMonthS,
		ENumS,
		EPeekS,
		EReptS,
		ERightS,
		ESciS,
		EUpperS,
		EUserS,
		EGetCmdS,
		ECmdS,
		EParseS,
		ESAddr, // The string variant of addr.
		EgCreateGrey,
		EIoOpenX,
		EMenuX,
		EBookMark,
		EGeteventc,
		EErrxS,		// hp: ERRX$
		EInTrans,
		EGetDocS,
		EMPopup,
		ESize,
		ELocX,
		EgPixel,
		EMaxNumber // This must be left at the end. Insert new functions immediately before this.
		};
	inline TOplFunction(TNumber aNumber) : iNumber(aNumber) {}
	inline TOplFunction::TNumber Number() const { return iNumber;}
	inline void SetNumber(TNumber aNumber) {iNumber=aNumber;}
	inline TOplSignature &Signature() { return iSignature;}
private:
	TNumber iNumber;
	TOplSignature iSignature;
	};

class TOplKeyword
	{
public:
	// DO NOT REORDER THESE (in many places the local order is significant)
enum TCode
	{
	EAppend,
	EAppendsprite,
	EAt,
	EBack,
	EBeep,
	EBreak,
	EBusy,
	ECache,
	ECachehdr,
	ECacherec,
	ECachetidy,
	EChangesprite,
	EClose,
	EClosesprite,
	ECls,
	ECompress,
	EContinue,
	ECopy,
	ECreate,
	ECursor,

	EDtext, // Order is significant here 
	EDchoice,
	EDlong,
	EDfloat,
	EDtime,
	EDdate,
	EDedit,
	EDSedit,
	EDxinput,
	EDfile,
	EDbuttons,
	EDposition, // Down to
	EDefaultwin,
	EDelete,
	EDiaminit,
	EDiampos,
	EDinit,
	EDinits,
	EDrawsprite,
	EEdit,
	EErase,
	EEscape,
	EFirst,
	EFont,
	EFreealloc,
	EGat,
	EGborder,
	EGbox,
	EGbutton,
	EGclock,
	EGclose,
	EGcls,
	EGcopy,
	EGdrawobject,
	EGetevent,
	EGfill,
	EGfont,
	EGgmode,
	EGgrey,
	EGinfo,
	EGinvert,
	EGiprint,
	EGlineby,
	EGlineto,
	EGmove,
	EGorder,
	EGoto,
	EGpatt,
	EGpeekline,
	EGpoly,
	EGprint,
	EGprintspace,
	EGprintb,
	EGsavebit,
	EGscroll,
	EGsetwin,
	EGstyle,
	EGtmode,
	EGunloadfont,
	EGupdate,
	EGuse,
	EGvisible,
	EGxborder,
	EGxprint,
	EInput,
	EIosignal,
	EIowaitstat,
	EIoyield,
	ELast,
	ELclose,
	ELinklib,
	ELoadm,
	ELock,
	ELopen,
	ELprint,
	ELprintspace,
	ELprintcrlf,
	EMcard,
	EMinit,
	EMkdir,
	ENext,
	EOdbinfo,
	EOff,
	EOffTime,
	EOnerr,
	EOpen,
	EOpenr,
	EPause,
	EPokeb,
	EPokef,
	EPokel,
	EPokes,
	EPokew,
	EPosition,
	EPossprite,
	EPrint,
	EPrintspace, 
	EPrintcrlf,
	ERaise,
	ERandomize,
	ERename,
	EReturnValue,
	EReturnNull,
	ERmdir,
	EScreen, // With 2 arguments
	EGscreen,// With 4 arguments
	EScreeninfo,
	ESecstodate,
	ESetname,
	ESetpath,
	EStatuswin,
	EStop,
	EStyle,
	ETrap,
	EUnloadm,
	EUpdate,
	EUse,
	EUsesprite,

	// New stuff here
	EModify,
	EInsert,
	ECancel,
	EPut,
	EDeleteTable,
	EGcircle,
	EGellipse,
	EGotoMark,
	EKillMark,
	EReturnFromEval,
	EGetevent32,
	EGeteventa32,
	EGcolor,
	ESetflags,
	ESetdoc,
	EDaysToDate,
	EGinfo32,
	EIowaitstat32,
	ECompact,
	EBeginTrans,
	ECommitTrans, 
	ERollBack,
	EClearflags,
	EPointerFilter,
	EMcasc,
//	EDCheckBox,
	EgSetPenWidth,
	EDeditMulti,
	EGcolorinfo,
	EGcolorbackground,
	EMcardX,
	ESetHelp,
	EShowHelp,
	ESetHelpUid,
	EGxborder32,
	EMaxCode // MUST BE AT THE END - Add new codes immediately before this
				  // And update back ends accordingly.
	}; // End of keywords

	inline TBool IsTrappable() const {return iTrapFlag;}
	inline TOplKeyword::TCode Code() const {return iCode;}
	inline void SetCode(TCode aCode) {iCode=aCode;} // So parser can modify a TOplKeyword.
private:
	friend class COplLexerBase;
	inline TOplKeyword(TBool aTrapFlag, TCode aCode) : iTrapFlag(aTrapFlag), iCode(aCode) {}
private:
	TBool iTrapFlag;
	TCode iCode;
	};

#endif