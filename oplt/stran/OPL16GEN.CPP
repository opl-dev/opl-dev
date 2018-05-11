// OPL16GEN.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.
//
// The 16 bit OPL Qcode Generator class. Converts Parser PCODE
// into the real McCoy Qcode.

#include "opl16.h"

#include <s32file.h>
#include <apparc.h>
#include <apgicnfl.h>
#include <apmstd.h>
#include <apfdef.h>
#include <charconv.h> // Unicode charconv

enum TOpl16BackEndPanic
	{
	// TOpl16QcodeGenerator
	EOpl16GenUnknownPCode,
	EOpl16GenUnknownConstantType,
	EOpl16StackError, // Error keeping track of the stack depth
	
	EOpl16BackEndReentrantStart,
	EOpl16BackEndAppState,
	EOpl16BackEndProcState,
	EOpl16BackEndBodyState,
	
	//
	EOpl16Opl1993NoOpx,

	// Eval back end
	EOpl16EvalBackEndBuffers
	};


////////////////////////////////////////////////////////////////
//
// TGlobalType - for putting out the type of a Global or external Symbol
//
////////////////////////////////////////////////////////////////
// Flag to indicate that a global or external variable (i.e. an indirected reference)
// is actually an array - or'd in with the variable type
const TInt KOpl16IndirectArrayFlag=0x80;

class TGlobalType
	{
public:
	TGlobalType(COplSymbol* aSymbol);
	inline void ExternalizeL(RWriteStream& aStream) const { aStream.WriteUint16L(iType);}
private:
	TInt iType;
	};

TGlobalType::TGlobalType(COplSymbol*aSymbol)
	{
	iType=aSymbol->Type();
	if (aSymbol->Token()==TOplToken::EArray)
		iType|=KOpl16IndirectArrayFlag;
	}
/////////////////////////////////////////////////////////////////
//
// Opo write stream - allows a RBufWriteStream to be patched
//
/////////////////////////////////////////////////////////////////
const TUint KOpl16PatchValue=KMaxTUint32; // To be patched with good value
TStreamPos ROpoWriteStream::AddPatchL(TBool a32BitPatch)
//
// Reserves space in a stream for a 32 bit value to be patched later
//
	{
	
	TStreamPos aPatchPos(SizeL());
	if (a32BitPatch)
		WriteUint32L(KOpl16PatchValue);
	else
		WriteUint16L(KOpl16PatchValue);
	return aPatchPos;
	}


void ROpoWriteStream::PatchL(TStreamPos aPos, TUint aValue,TBool a32BitPatch)
	{
// ????
	
	Sink()->SeekL(MStreamBuf::EWrite,aPos);

	if (a32BitPatch)
		WriteUint32L(aValue);
	else
		WriteUint16L(aValue);
	Sink()->SeekL(MStreamBuf::EWrite,EStreamEnd);
	}

void ROpoWriteStream::PatchCheckedUidL(TCheckedUid& aCheckedUid)
//
// FOR NOW - this is a bit of a horrible kludge that comes out of trying to
// embed the old file format in what looks to be a stream store.
//
	{
	
	Sink()->SeekL(MStreamBuf::EWrite,EStreamBeginning);
	ExternalizeL(aCheckedUid,*this);
	Sink()->SeekL(MStreamBuf::EWrite,EStreamEnd);
	}

TInt ROpoWriteStream::SizeL()
//
// Checks the size
//
	{
	
	return Sink()->SeekL(0,EStreamEnd).Offset();
	}


//////////////////////////////////////////////////////////////////
//
// TCode16 - Class for externalizing qcodes
//
/////////////////////////////////////////////////////////////////
const TInt KOpl16ExtendedQcodeFlag=0x100;

const TUint KOpl16LeftSideReference=0x04;
const TUint KOpl16ExternalReference=0x08;
const TUint KOpl16ArrayReference=0x10;

const TUint KOpl16EndOfFieldList=0xff;

class TQcode16
	{
public:
	TQcode16(TUint aCode) : iCode(aCode) {}
	enum TCode
		{
		EIdentifier=0x00,
		EField=0x20,
		EConstant=0x28,
		ESpecial=0x2c, 

		ELessThan=0x30,
		ELessThanEq=0x34,
		EGreaterThan=0x38,
		EGreaterThanEq=0x3c,
		EEqual=0x40,
		ENotEqual=0x44,
		EAddition=0x48,
		ESubtraction=0x4c,
		EStackByteAsWord=0x4f,
		EMultiply=0x50,
		ECallProcedure=0x53,
		EDivide=0x54,
		EFunctionCall=0x57,
		EPower=0x58,
		EBranchFalse=0x5b,
		EAnd=0x5c,
		EStackByteAsLong=0x5f,

		EOr=0x60,
		EStackWordAsLong=0x63,
		ENot=0x64,
		EDebugStatement16=0x67,
		EUnaryMinus=0x68,
		ECallProcByName=0x6b,
		EPercLess=0x6c,
		EPercGreater=0x6d,
		EPercPlus=0x6e,
		EPercMinus=0x6f,

		EPercMultiply=0x70,
		EPercDivide=0x71,
		EReturnNull=0x74,

		ELongToWord=0x78,
		EDoubleToWord=0x79,
		EDoubleToLong=0x7a,
		EWordToLong=0x7b,


		EWordToDouble=0x7c,
		ELongToDouble=0x7d,
		ELongToUword=0x7e,
		EDoubleToUword=0x7f,

		EDropValue=0x80,
		EAssign=0x84,
		EPrintValue=0x88,
		ELprintValue=0x8c,

		EPrintSpace=0x90,
		ELprintSpace=0x91,
		EPrintCrLf=0x92,
		ELprintCrLf=0x93,
		EInput=0x94,
		EPokeWord=0x98,
		EPokeLong=0x99,
		EPokeDouble=0x9a,
		EPokeString=0x9b,
		EPokeByte=0x9c,
		EAppend=0x9d,
		EAt=0x9e,
		EBack=0x9f,

		EBeep=0xa0,
		EClose=0xa1,
		ECls=0xa2,
		ECompress=0xa3,
		ECopy=0xa4,
		ECreate=0xa5,
		ECursor=0xa6,
		EDelete=0xa7,
		EErase=0xa8,
		EEscape=0xa9,
		EFirst=0xaa,
		EVector=0xab,
		ELast=0xac,
		ELclose=0xad,
		ELoadm=0xae,
		ELopen=0xaf,

		ENext=0xb0,
		EOnerr=0xb1,
		EOff=0xb2,
		EOffTime=0xb3,
		EOpen=0xb4,
		EPause=0xb5,
		EPosition=0xb6,
		EIoSignal=0xb7,
		ERaise=0xb8,
		ERandomize=0xb9,
		ERename=0xba,
		EStop=0xbb,
		ETrap=0xbc,
		EUpdate=0xbd,
		EUse=0xbe,
		EGoto=0xbf,

		EReturnValue=0xc0,
		EUnloadm=0xc1,
		EEdit=0xc2,
		EScreen=0xc3,
		EOpenr=0xc4,
		EgSaveBit=0xc5,
		EgClose=0xc6,
		EgUse=0xc7,
		EgSetWin=0xc8,
		EgVisible=0xc9,
		EgFont=0xca,
		EgUnloadFont=0xcb,
		EgGMode=0xcc,
		EgTMode=0xcd,
		EgStyle=0xce,
		EgOrder=0xcf,

		EgInfo=0xd0,
		EgCls=0xd1,
		EgAt=0xd2,
		EgMove=0xd3,
		EgPrint=0xd4,
		EgPrintSpace=0xd8,
		EgPrintB=0xd9,
		EgLineBy=0xda,
		EgBox=0xdb,
		EgCircle=0xdc,
		EgEllipse=0xdd,
		EgPoly=0xde,
		EgFill=0xdf,

		EgPatt=0xe0,
		EgCopy=0xe1,
		EgScroll=0xe2,
		EgUpdate=0xe3,
		EGetEvent=0xe4,
		EgLineTo=0xe5,
		EgPeekLine=0xe6,
		EgScreen=0xe7,
		EIoWaitStat=0xe8,
		EIoYield=0xe9,
		EmInit=0xea,
		EmCard=0xeb,
		EdInit=0xec,
		EdItem=0xed,
		ESetName=0xee,
		EStatusWin=0xef,

		EBusy=0xf0,
		ELock=0xf1,
		EgInvert=0xf2,
		EgXPrint=0xf3,
		EgBorder=0xf4,
		EgClock=0xf5,
		EMemoryRight=0xf6,
		EMemoryLeft=0xf7,
		EMkDir=0xf8,
		ERmDir=0xf9,
		ESetPath=0xfa,
		ESecsToDate=0xfb,
		EgIPrint=0xfc,
		EExtended=0xff,

		EgGrey = KOpl16ExtendedQcodeFlag,
		EDefaultWin,
		EDiamInit,
		EDiamPos,
		EFont,
		EStyle,
		EUseSprite,
		EAppendSprite,
		EDrawSprite,
		EChangeSprite,
		EPosSprite,
		ECloseSprite,
		EFreeAlloc,
		ELinkLib,
		ECache,
		EgButton,

		EgXBorder,
		EgDrawObject,
		EOdbInfo,
		ECacheTidy,
		EScreenInfo,
		ECacheHdr,
		ECacheRec,
		EdInitS,
		EOpxCall,
		EDebugStatement32,
		EModify,
		EInsert,
		ECancel,
		EPut,
		EDeleteTable,
		EGotoMark,
		
		EKillMark,	// 0x120
		EReturnFromEval,
		EGetEvent32,
		EGetEventa32,
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

		EmCasc,  // 0x130
		EEvalRightSideExternal,
		EEvalLeftSideExternal,
//		EdCheckBox,
		EgSetPenWidth,
		EdEditMulti,
		EGcolorinfo,
		EGcolorbackground,
		EmCardX,
		ESetHelp,
		EShowHelp,
		ESetHelpUid,
		EgXBorder32
		};

	enum TDialogItem
		{
		EDlgText,
		EDlgChoice,
		EDlgLong,
		EDlgFloat,
		EDlgTime,
		EDlgDate,
		EDlgEdit,
		EDlgSedit,
		EDlgIinput,
		EDlgFile,
		EDlgButtons,
		EDlgPosition
		};
	enum TFuncNumber
		{
		EAddr=0x0,
		EAsc=0x1,
		ECall=0x2,
		ECount=0x3,
		EDay=0x4,
		EDow=0x5,
		EEof=0x6,
		EErr=0x7,
		EExist=0x8,
		EFind=0x9,
		EGet=0xA,
		EIoA=0xB,
		EIoW=0xC,
		EIoOpen=0xD,
		EIoWrite=0xE,
		EIoRead=0xF,
		EIoClose=0x10,
		EIoWait=0x11,
		EHour=0x12,
		EKey=0x13,
		ELen=0x14,
		ELoc=0x15,
		EMinute=0x16,
		EMonth=0x17,
		EPeekB=0x18,
		EPeekW=0x19,
		EPos=0x1A,
		ERecSize=0x1B,
		ESecond=0x1C,
		EUsr=0x1D,
		EYear=0x1E,
		ESAddr=0x1F,
		EWeek=0x20,
		EIoSeek=0x21,
		EKMod=0x22,
		EKeyA=0x23,
		EKeyC=0x24,
		EIoOpenX=0x25,
		EgCreate=0x26,
		EgCreateBit=0x27,
		EgLoadBit=0x28,
		EgLoadFont=0x29,
		EgRank=0x2A,
		EgIdentity=0x2B,
		EgX=0x2C,
		EgY=0x2D,
		EgWidth=0x2E,
		EgHeight=0x2F,
		EgOriginX=0x30,
		EgOriginY=0x31,
		EgTWidth=0x32,
		EgPrintClip=0x33,
		ETestEvent=0x34,
		EOs=0x35,
		EMenu=0x36,
		EDialog=0x37,
		EAlert=0x38,
		EGreyCreate=0x39,
		EMenuX=0x3A,
		ECreateSprite=0x3B,
		ELoadLib=0x3C,
		EUnloadLib=0x3D,
		EFindLib=0x3E,
		EGetLibH=0x3F,
		EDays=0x40,
		EIAbs=0x41,
		EInt=0x42,
		EPeekL=0x43,
		ESpace=0x44,
		EDateToSecs=0x45,
		ENewObj=0x46,
		ENewObjH=0x47,
		ESend=0x48,
		EEnterSend=0x49,
		EEnterSend0=0x4A,
		EAlloc=0x4B,
		EReAlloc=0x4C,
		EAdjustAlloc=0x4D,
		ELenAlloc=0x4E,
		EIoc=0x4F,
		EUAdd=0x50,
		EUSub=0x51,
		EIoCancel=0x52,
		EStatWinInfo=0x53,
		EFindField=0x54,
		EBookMark=0x55,
		EGetEventC=0x56,
		EInTrans=0x57,
		EMPopup=0x58,
		EAbs=0x80,
		EACos=0x81,
		EASin=0x82,
		EATan=0x83,
		ECos=0x84,
		EDeg=0x85,
		EExp=0x86,
		EFlt=0x87,
		EIntF=0x88,
		ELn=0x89,
		ELog=0x8A,
		EPeekF=0x8B,
		EPi=0x8C,
		ERad=0x8D,
		ERnd=0x8E,
		ESin=0x8F,
		ESqr=0x90,
		ETan=0x91,
		EVal=0x92,
		EMax=0x93,
		EMean=0x94,
		EMin=0x95,
		EStd=0x96,
		ESum=0x97,
		EVar=0x98,
		EEval=0x99,
		EChrS=0xC0,
		EDatim=0xC1,
		EDayNameS=0xC2,
		EDirS=0xC3,
		EErrS=0xC4,
		EFixS=0xC5,
		EGenS=0xC6,
		EGetS=0xC7,
		EHexS=0xC8,
		EKeyS=0xC9,
		ELeftS=0xCA,
		ELowerS=0xCB,
		EMidS=0xCC,
		EMonthS=0xCD,
		ENumS=0xCE,
		EPeekS=0xCF,
		EReptS=0xD0,
		ERightS=0xD1,
		ESciS=0xD2,
		EUpperS=0xD3,
		EUserS=0xD4,
		EGetCmdS=0xD5,
		ECmdS=0xD6,
		EParseS=0xD7,
		EErrxS=0xD8,
		EGetDocS=0xD9,
		ESize=0xDA,
		ELocX=0xDB,
		EgPixel=0xDC
		};
	void ExternalizeL(RWriteStream& aStream) const;
private:
	TUint iCode;
	};

void TQcode16::ExternalizeL(RWriteStream& aStream) const
//
// Adds the Qcode to the stream
//
	{

	TInt code=iCode;
	if (code&KOpl16ExtendedQcodeFlag)
		{
		code&=~KOpl16ExtendedQcodeFlag;
		aStream.WriteUint8L(EExtended);
		}
	aStream.WriteUint8L(code);
	}

/////////////////////////////////////////////////////////////////
//
// TLbc - class for externalizing descriptors to E16 Lbcs
//
/////////////////////////////////////////////////////////////////
class TLbc
	{
public:
	TLbc(const TDesC& aDes) : iDes(aDes) {}
	void ExternalizeL(RWriteStream& aStream) const;
private:
	const TDesC& iDes;
	};

void TLbc::ExternalizeL(RWriteStream& aStream) const
//
// Adds the Lbc to the stream
//
	{
    TUint len=iDes.Length();
	if (len>TInt(KMaxTUint8))
        User::Leave(EErrBadStringLength);
	// Write the len as 16-bit, even though only 8 bits are used for the length.
	aStream.WriteUint16L(len);
	// Just to avoid 256 bytes on the stack
    TBuf8<KMaxTUint8*sizeof(TText)> *tempBuf=new(ELeave) TBuf8<KMaxTUint8*sizeof(TText)>;
	CleanupStack::PushL(tempBuf);
	TUint8* d=(TUint8*)tempBuf->Ptr();
	TUint16* s=(TUint16*)iDes.Ptr();
	for(TUint i=0;i<len;++i)
		{
		TUint16 c=*s++;
		*d++=(TUint8)(c&0xff);
		*d++=(TUint8)(c>>8);
		}
    aStream.WriteL(tempBuf->Ptr(),len<<1);
	CleanupStack::PopAndDestroy();
	// format: len pad lo0 hi0 lo1 hi1 ... LenInBytes
    }

GLDEF_C void Panic(TOpl16BackEndPanic aPanicNumber)
//
// Panic which identifies that it came from OPL16 back end
//
	{
	_LIT(KOpltPanic,"OPL16 Backend");
	User::Panic(KOpltPanic,aPanicNumber);
	}


///////////////////////////////////////////////////////////////////////
//
// COpl16App : App parsing
//
///////////////////////////////////////////////////////////////////////
// Real maximum sizes 
const TInt KOpl16AppMaxExtLen=3;
const TInt KOpl16AppMaxNameLen=8;
const TInt KOpl16AppMaxPathLen=19;

// Size of the data in the file - includes zero terminators
const TInt KOpl16AppNameExtBufLen=14;
const TInt KOpl16AppPathBufLen=20; 
const TInt KOpl16AppDataLen=KOpl16AppNameExtBufLen+KOpl16AppPathBufLen+sizeof(TUint16); // Total size of data in the file.
const TInt KOpl16AppMaxIconSize=KMaxTInt16;

const TUint KOpl16AppSupportsNewFile=0x0001;   // App not in Shell's new file list
const TUint KOpl16AppIsHidden=0x0002;          // App not in Extras bar etc
const TUint KOpl16AppFlagsMask=KOpl16AppSupportsNewFile|KOpl16AppIsHidden;

const TUint KOpl16AppNameFlag=0x00000001;
const TUint KOpl16AppTypeFlag=0x00000002;
const TUint KOpl16AppExtFlag =0x00000004;
const TUint KOpl16AppPathFlag=0x00000008;
const TUint KOpl16AppIconFlag=0x00000010;
const TUint KOpl16AppCaptionFlag=0x00000020;

const TUint KOpl16MaxIconReadBufSize=0x400; // 1K - plenty big enough for most ICON files
class COpl16App : public COplParseApp
	{
public:
	void ParseL(COpl16ModuleBackEnd& aBackEnd,TBool aLocatingError);
private:
	friend class COpl16ModuleBackEnd;
	friend class COpl16Opl1993BackEnd;
	friend class COpl16Opler1BackEnd;
	COpl16App(TOplTranTarget aTarget,COplLexerBase& aLexer);
	inline TUint Flags() const  {return iFlags;}
	inline TUint Type() const { return iType;}
	inline TInt32 Id() const {return iD;}
	inline TFileName& IconFile() { return iIconName;}
	inline TDes& NameAndExt() { return iNameAndExt;}
	inline TInt MaxNameAndExtLength() { return iMaxNameAndExtLength;}
	inline TDes& Path() { return iPath;}
	void TestFlagTokenAndTypeL(TUint aFlag,TOplToken aToken,TOplToken::TType aType);
	const CCnvCharacterSetConverter* LoadUnicodeConverterLC();
	void FromUnicodeL(TPtrC16& aUnicode,TPtr8& aNarrow);
private:
	TOplTranTarget iTarget;
	TInt32 iD;
	TUint iType;
	TUint iFlags;			// flag what's been done already
	COplLexerBase& iLexer;
	TApaAppCaption iNameAndExt;
	TInt iMaxNameAndExtLength;	// more allowed on Opler1
	TBuf<KOpl16AppPathBufLen> iPath;
	TFileName iIconName;
	TBool iNormalFlag;		// opler1 only
	};

void COpl16App::ParseL(COpl16ModuleBackEnd& aBackEnd,TBool aLocatingError)
//
// Only public method - does the parsing and generates AIF file for opler1 apps
//
// AppDefinition:= APP appName opler1-stuff EOS
//					[app-entry EOS]*
//                 	ENDA EOS
//
// opler1-stuff :=    // NULL for OPL1993 machines
//				| , long-integer // , APP UID on Opler1
//
// app-entry:= [TYPE number] |
//			   [EXT string-constant] |
//			   [ICON icon file ] |
//             [PATH pathdetails] | 
//             [CAPTION captioninfo] |
//             [MIME priority,datatype]
//
// There is a semantic rule that says  TYPE, EXT, PATH may only appear once.
// But ICON can appear more than once on Opler1, where the filename is an MBM
// The MIME entry can also occur more than once.
//
	{
	TInt maxNameLength=KApaMaxAppCaption;	// assume Opler1
	if (iTarget==EOplTargetER1)
		iMaxNameAndExtLength=KApaMaxAppCaption;
	else
		{
		maxNameLength=KOpl16AppMaxNameLen;
		iMaxNameAndExtLength=KOpl16AppNameExtBufLen;
		}

	//  APP appName [opler1-stuff] EOS // 
	TestFlagTokenAndTypeL(KOpl16AppNameFlag,TOplToken::ESimple,TOplToken::EReal);
	TPtrC name=iLexer.MarkedToken(); // Get it raw as case is significant
	if (name.Length()>maxNameLength)
		User::Leave(EErrBadStringLength);
	iNameAndExt.Insert(0,name);

	// opler1-stuff
	CApaAppInfoFileWriter* appInfoFileWriter=NULL;
	TApaAppCapability capability;
	capability.iSupportsNewFile=EFalse;		// doesn't support new files by default
	capability.iEmbeddability=TApaAppCapability::ENotEmbeddable;
	capability.iAppIsHidden=EFalse;			// not hidden by default
	TParse aifName;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	if (iTarget==EOplTargetER1)
		{
		iLexer.LexL(TOplToken::EComma);
		iLexer.LexL(TOplToken::EConstant);
		iD=iLexer.Constant().AsLongL();

		aifName.SetNoWild(KAppInfoFileExtension,&(aBackEnd.OutputName()),NULL);	// path same as APP
		appInfoFileWriter=CApaAppInfoFileWriter::NewLC(fs,aifName.FullName(),TUid::Uid(iD));
		}

	// EOS
	iLexer.LexL(TOplToken::EEos);

	// [app-entry EOS]*
	// ENDA EOS
	TBool iconSeen=EFalse;
	CApaMaskedBitmap* maskedBitmap=NULL;
	TBool bitmapOnCleanup=EFalse;
	for (TBool endSeen=EFalse;!endSeen;)
		{
		// app-entry | ENDA
		TOplToken next;
		do
			{
			next=iLexer.LexL();
			} while (next==TOplToken::EEos);
	    	switch (next)
			{
			case TOplToken::EFlags:     // same as TYPE but for Opler1
				TestFlagTokenAndTypeL(KOpl16AppTypeFlag,TOplToken::EConstant,TOplToken::EWord);
				iType=iLexer.Constant().AsWordL();
				if (!iType || (iType & (~KOpl16AppFlagsMask)))
					User::Leave(EErrSyntax);
				capability.iSupportsNewFile=iType&KOpl16AppSupportsNewFile;
				capability.iAppIsHidden=iType&KOpl16AppIsHidden;
				break;
			case TOplToken::EType:
				TestFlagTokenAndTypeL(KOpl16AppTypeFlag,TOplToken::EConstant,TOplToken::EWord);
				iType=iLexer.Constant().AsWordL();
				break;
			case TOplToken::EExt:
				{ // because of declaration of 'ext'
				TestFlagTokenAndTypeL(KOpl16AppExtFlag,TOplToken::EConstant,TOplToken::EString);
				TPtrC ext=iLexer.Constant().AsStringL();
				if (ext.Length()>KOpl16AppMaxExtLen)
					User::Leave(EErrBadStringLength);
				iNameAndExt.Append('.');
				iNameAndExt.Append(ext); // !! - AppendF ??
				iNameAndExt.UpperCase();
				}
				break;
			case TOplToken::ECaption:
				{
				TestFlagTokenAndTypeL(0,TOplToken::EConstant,TOplToken::EString);
				iFlags|=KOpl16AppCaptionFlag;
				TPtrC capt=iLexer.Constant().AsStringL();
				if (capt.Length()>maxNameLength)
					User::Leave(EErrBadStringLength);
				TApaAppCaption captBuf=capt;
				iLexer.LexL(TOplToken::EComma);
				iLexer.LexL(TOplToken::EConstant);
				if (iLexer.Type()!=TOplToken::EWord)
					User::Leave(EErrSyntax);
				TLanguage lang=TLanguage(iLexer.Constant().AsWordL());
				if (aLocatingError)
					break;
				appInfoFileWriter->AddCaptionL(lang,captBuf);
				}
				break;
				
			case TOplToken::EMime:
				{ // MIME priority, Type
				TestFlagTokenAndTypeL(0,TOplToken::EConstant,TOplToken::EWord);
                TDataTypePriority priority=iLexer.Constant().AsWordL();
				iLexer.LexL(TOplToken::EComma);
				iLexer.LexL(TOplToken::EConstant);

				// Convert to 8-bit for TDataType
				TPtrC mime16=iLexer.Constant().AsStringL();
				TInt len=mime16.Length(); // in characters.
				TPtr8 mime((TText8*)User::AllocLC(len),len);
				FromUnicodeL(mime16,mime);

				if (mime.Length()==0) // length of narrow string
					User::Leave(EErrSyntax);
				if (mime.Length()>KMaxDataTypeLength)
					User::Leave(EErrBadStringLength);
                TDataTypeWithPriority dataType(mime,priority);
                appInfoFileWriter->AddDataTypeL(dataType);
				CleanupStack::PopAndDestroy(); // mime string
                }
				break;
			
			case TOplToken::EIcon:
				{
				iconSeen=ETrue;
				// hp: don't restrict to one icon file for Opler1
				TUint flag=0;
				if (iTarget!=EOplTargetER1)
					flag=KOpl16AppIconFlag;
				TestFlagTokenAndTypeL(flag,TOplToken::EConstant,TOplToken::EString);
				TFileName fileName=iLexer.Constant().AsStringL();
				if (fileName.Length()>KMaxFileName)
					User::Leave(EErrBadStringLength);
				if (flag) // if Opl1993
					{
					iIconName.Copy(fileName);
					break;
					}

				// definitely Opler1 if reaches here				
				if (aLocatingError)
					break;
				TInt err;
				TInt index=0;
				COpl16Opler1BackEnd& opler1BackEnd=STATIC_CAST(COpl16Opler1BackEnd&,aBackEnd);
				TParse parser;
				parser.Set(fileName,opler1BackEnd.SourceName(),NULL);
				fileName=parser.FullName();
				iIconName.Copy(fileName);

				FOREVER
					{
					if (iNormalFlag)	// doing normal
						{
						maskedBitmap=CApaMaskedBitmap::NewLC();
						err=maskedBitmap->Load(fileName,index++);

						if (err!=KErrEof)
							bitmapOnCleanup=ETrue;
						else
							{
							// failed to load bitmap from this file
							CleanupStack::PopAndDestroy();
							bitmapOnCleanup=EFalse;
							break;
							}
						if (err)
							User::Leave(err);
						iNormalFlag=EFalse;
						}
					else
						{
						err=(maskedBitmap->Mask())->Load(fileName,index++);
						if (err==KErrEof)
							break;				// failed to load mask from this file
						if (err)
							User::Leave(err);
						iNormalFlag=ETrue;
						appInfoFileWriter->AddIconL(*maskedBitmap);	// appInfoFileWriter now owns icon
						CleanupStack::Pop();			// maskedBitmap (now owned by writer)
						bitmapOnCleanup=EFalse;
						}
					}
				}
				break;
			case TOplToken::EPath:
				{
				TestFlagTokenAndTypeL(KOpl16AppPathFlag,TOplToken::EConstant,TOplToken::EString);
				TPtrC path=iLexer.Constant().AsStringL();
				if (path.Length()>KOpl16AppMaxPathLen)
					User::Leave(EErrBadStringLength);
				iPath.CopyF(path);
				if (iPath.Length()==0 || iPath[iPath.Length()-1]!='\\') // Need to add trailing
					{
					if (path.Length()==KOpl16AppMaxPathLen)
						User::Leave(EErrBadStringLength);
					iPath.Append('\\');
					}
				}
				break;

			case TOplToken::EEndA:
				endSeen=ETrue;
				break;
			default:
				User::Leave(EErrSyntax);
			}
		// EOS
		iLexer.LexL(TOplToken::EEos);
		}
	if (iTarget==EOplTargetER1)
		{
		if (bitmapOnCleanup)
			CleanupStack::PopAndDestroy();	// pop and destroy the bitmap that hasn't been added
		if (iNormalFlag==EFalse)	// if mask still required
			User::Leave(EErrIconMaskMissing);

		TApaAppCapabilityBuf buf(capability);
		User::LeaveIfError(appInfoFileWriter->SetCapability(buf));
		if ((iFlags&KOpl16AppCaptionFlag)==0)
			appInfoFileWriter->AddCaptionL(User::Language(),name); // Use APP <name> as caption

		appInfoFileWriter->StoreL();
		CleanupStack::PopAndDestroy();	// appInfoFileWriter
		}
	CleanupStack::PopAndDestroy(); // fs
	}


const CCnvCharacterSetConverter* COpl16App::LoadUnicodeConverterLC()
//
// Locate and load the convchar converter
//
	{
	CCnvCharacterSetConverter* characterSetConverter=CCnvCharacterSetConverter::NewLC();//push convertor.

	RFs fsSession; 
	User::LeaveIfError(fsSession.Connect());
	CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* arrayOfCharacterSetsAvailable=characterSetConverter->CreateArrayOfCharacterSetsAvailableL(fsSession);
	CleanupStack::PushL(arrayOfCharacterSetsAvailable);//push array of.

	if (arrayOfCharacterSetsAvailable->Count()==0)
		User::Leave(KErrNotFound);

	//!!TODO Ask David B about which charSetConvertors are going to be most popular...
	//Or has this changed anyway? See ConvertToUnicodeFromNative8BitBuild().
	TInt gotIt=EFalse;
	for (TInt i=arrayOfCharacterSetsAvailable->Count()-1; i>=0; --i)
		{
		const CCnvCharacterSetConverter::SCharacterSet& charactersSet=(*arrayOfCharacterSetsAvailable)[i];
		characterSetConverter->PrepareToConvertToOrFromL(charactersSet.Identifier(), *arrayOfCharacterSetsAvailable, fsSession);
		TPtrC charactersSetName;
		charactersSetName.Set(charactersSet.Name());
		_LIT(KCharSetName,"Code Page 1252");
		if (charactersSetName.Find(KCharSetName)==0)
			{
			gotIt=ETrue;
			break;
			}
		}
	if (gotIt==EFalse)
		User::Leave(KErrNotFound);

	TInt KUidCodePage1252=0x100012b6;

	characterSetConverter->PrepareToConvertToOrFromL(KUidCodePage1252, *arrayOfCharacterSetsAvailable, fsSession);
	fsSession.Close();
	return characterSetConverter;
	}


void COpl16App::FromUnicodeL(TPtrC16& aUnicode,TPtr8& aNarrow)
	{
	const CCnvCharacterSetConverter* converter=LoadUnicodeConverterLC();
	TInt state=CCnvCharacterSetConverter::KStateDefault;
	converter->ConvertFromUnicode(aNarrow,aUnicode,state);
	if (state!=CCnvCharacterSetConverter::KStateDefault)
		User::Leave(KErrNotSupported);
	CleanupStack::PopAndDestroy(2); // arrayOfCharacterSetsAvailable and characterSetConverter
	}

					 
COpl16App::COpl16App(TOplTranTarget aTarget,COplLexerBase& aLexer)
	: iTarget(aTarget), iType(0), iFlags(0), iLexer(aLexer),iNormalFlag(ETrue)
//
// C'tor
//
	{

	}


void COpl16App::TestFlagTokenAndTypeL(TUint aFlag,TOplToken aToken,TOplToken::TType aType)
//
// Called when trying to add component
// If we've seen it already that's a syntax error
//
	{
	
	if (iFlags&aFlag)
		User::Leave(EErrSyntax);
	iFlags|=aFlag;
	iLexer.LexL(aToken);
	if (iLexer.Type()!=aType)
		User::Leave(EErrSyntax);
	}

////////////////////////////////////////////////////////////////
// 
// COpl16PcodeConverter
//
////////////////////////////////////////////////////////////////
const TInt KOpl16PcodeConvStackGran=8;
const TInt KOpl16PcodeConvLabelsGran=16;
class TTranslateError;

class COpl16PcodeConverter : public CBase
	{
public:
	static COpl16PcodeConverter* NewL();
	static COpl16PcodeConverter* NewLC();
	~COpl16PcodeConverter();
	inline void SetErrorOffset(TInt aQcodeOffset) { iErrorPos=aQcodeOffset;}
	TInt RunL(CBufBase& aPcodeStream,
				ROpoWriteStream& aQcodeStream,
				TTranslateError& anError,
				CTextTranslator::TDebugFlag aFlag);
	inline TInt MaxStackDepth() { return iMaxStackDepth;}
private:
	void ConvertPcodeL(RReadStream& aPcodeStream,ROpoWriteStream& aQcodeStream,
			TTranslateError& anError,CTextTranslator::TDebugFlag aFlag);
	void LabelReferenceL(RReadStream& aPcodeStream, ROpoWriteStream& aQcodeStream,TInt aBase);
	
	// Stack monitoring
	void AddToStackL(TOplToken::TType aType);
	void AddToStackL(TUint byteCount);
	TUint AddToStackL(TPcode::TIdentifierSide aSide ,TOplToken::TType aType);
	void AdjustStackForCallL(TOplToken::TType aReturnType);
	void RemoveFromStack(TOplToken::TType aType);
	TUint SizeOnStack(TOplToken::TType aType);
	inline void ZeroStack() {iStackDepth=0;}
	void ConstructL();
	COpl16PcodeConverter();
private:
	TUint iStackDepth;
	TUint iMaxStackDepth;
	TInt iErrorPos;
	CTranStackFlat<TUint>* iStackDepths;
	CArrayFixSeg<COplSymbol*>* iLabels;
	};

	
static const TUint8 procByNameTypeBytes[]={'%','&',0,'$'};
static const TUint8 casts[TOplToken::EMaxNormalType][TOplToken::EMaxNormalType]= // TQcode16::EExtended is type mismatch error 
	{
	// WORD, LONG, DOUBLE, STRING, UWORD 
	{ 0, TQcode16::EWordToLong, TQcode16::EWordToDouble,	TQcode16::EExtended,0 }, // WORD
	{TQcode16::ELongToWord, 0, TQcode16::ELongToDouble,TQcode16::EExtended, TQcode16::ELongToUword},  // LONG
	{TQcode16::EDoubleToWord,TQcode16::EDoubleToLong, 0,TQcode16::EExtended, TQcode16::EDoubleToUword}, // Double 
	{TQcode16::EExtended,	TQcode16::EExtended,		TQcode16::EExtended, 	0,	TQcode16::EExtended}, // String 
	{ 0,	TQcode16::EWordToLong, TQcode16::EWordToDouble,	TQcode16::EExtended,0},  // UWORD - dummy row really
	};

static const TUint8 operators[]=
	{
	0,
	TQcode16::ELessThan,
	TQcode16::ELessThanEq,
	TQcode16::EGreaterThan,
	TQcode16::EGreaterThanEq,
	TQcode16::EEqual,
	TQcode16::ENotEqual,
	TQcode16::EAddition,
	TQcode16::ESubtraction,
	TQcode16::EMultiply,
	TQcode16::EDivide,
	TQcode16::EPower,
	TQcode16::EAnd,
	TQcode16::EOr,
	TQcode16::ENot,
	TQcode16::EUnaryMinus,
	0,
	0,
	0,
	0,
	TQcode16::EPercLess,
	TQcode16::EPercGreater,
	TQcode16::EPercPlus,
	TQcode16::EPercMinus,
	TQcode16::EPercMultiply,													
	TQcode16::EPercDivide
	};

typedef TUint16 TUintCode; // Size of values to store in the table
static const TUintCode QcodeTable[TOplKeyword::EMaxCode]=
	{
	TUintCode(TQcode16::EAppend),TUintCode(TQcode16::EAppendSprite),TUintCode(TQcode16::EAt),TUintCode(TQcode16::EBack),TUintCode(TQcode16::EBeep),
	TUintCode(TQcode16::EExtended),TUintCode(TQcode16::EBusy),TUintCode(TQcode16::ECache),TUintCode(TQcode16::ECacheHdr),TUintCode(TQcode16::ECacheRec),
	TUintCode(TQcode16::ECacheTidy),TUintCode(TQcode16::EChangeSprite),TUintCode(TQcode16::EClose),TUintCode(TQcode16::ECloseSprite),TUintCode(TQcode16::ECls),
	TUintCode(TQcode16::ECompress),TUintCode(TQcode16::EExtended),TUintCode(TQcode16::ECopy),TUintCode(TQcode16::ECreate),TUintCode(TQcode16::ECursor),
	TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),
	TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),
	TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EdItem),TUintCode(TQcode16::EDefaultWin),TUintCode(TQcode16::EDelete),TUintCode(TQcode16::EDiamInit),
	TUintCode(TQcode16::EDiamPos),TUintCode(TQcode16::EdInit),TUintCode(TQcode16::EdInitS),TUintCode(TQcode16::EDrawSprite),TUintCode(TQcode16::EEdit),
	TUintCode(TQcode16::EErase),TUintCode(TQcode16::EEscape),TUintCode(TQcode16::EFirst),TUintCode(TQcode16::EFont),TUintCode(TQcode16::EFreeAlloc),
	TUintCode(TQcode16::EgAt),TUintCode(TQcode16::EgBorder),TUintCode(TQcode16::EgBox),TUintCode(TQcode16::EgButton),TUintCode(TQcode16::EgClock),
	TUintCode(TQcode16::EgClose),TUintCode(TQcode16::EgCls),TUintCode(TQcode16::EgCopy),TUintCode(TQcode16::EgDrawObject), TUintCode(TQcode16::EGetEvent),
	TUintCode(TQcode16::EgFill),TUintCode(TQcode16::EgFont),TUintCode(TQcode16::EgGMode),TUintCode(TQcode16::EgGrey),TUintCode(TQcode16::EgInfo),
	TUintCode(TQcode16::EgInvert),TUintCode(TQcode16::EgIPrint),TUintCode(TQcode16::EgLineBy),TUintCode(TQcode16::EgLineTo),TUintCode(TQcode16::EgMove),
	TUintCode(TQcode16::EgOrder),TUintCode(TQcode16::EExtended),TUintCode(TQcode16::EgPatt),TUintCode(TQcode16::EgPeekLine),TUintCode(TQcode16::EgPoly),
	TUintCode(TQcode16::EgPrint),TUintCode(TQcode16::EgPrintSpace),TUintCode(TQcode16::EgPrintB),TUintCode(TQcode16::EgSaveBit),TUintCode(TQcode16::EgScroll),
	TUintCode(TQcode16::EgSetWin),TUintCode(TQcode16::EgStyle),TUintCode(TQcode16::EgTMode),TUintCode(TQcode16::EgUnloadFont),TUintCode(TQcode16::EgUpdate),
	TUintCode(TQcode16::EgUse),TUintCode(TQcode16::EgVisible),TUintCode(TQcode16::EgXBorder),TUintCode(TQcode16::EgXPrint),TUintCode(TQcode16::EInput),
	TUintCode(TQcode16::EIoSignal),TUintCode(TQcode16::EIoWaitStat),TUintCode(TQcode16::EIoYield),TUintCode(TQcode16::ELast),TUintCode(TQcode16::ELclose),
	TUintCode(TQcode16::ELinkLib),TUintCode(TQcode16::ELoadm),TUintCode(TQcode16::ELock),TUintCode(TQcode16::ELopen),TUintCode(TQcode16::ELprintValue),
	TUintCode(TQcode16::ELprintSpace),TUintCode(TQcode16::ELprintCrLf),TUintCode(TQcode16::EmCard),TUintCode(TQcode16::EmInit),TUintCode(TQcode16::EMkDir),
	TUintCode(TQcode16::ENext),TUintCode(TQcode16::EOdbInfo),TUintCode(TQcode16::EOff),TUintCode(TQcode16::EOffTime),TUintCode(TQcode16::EExtended),		
	TUintCode(TQcode16::EOpen),TUintCode(TQcode16::EOpenr),TUintCode(TQcode16::EPause),TUintCode(TQcode16::EPokeByte),TUintCode(TQcode16::EPokeDouble),
	TUintCode(TQcode16::EPokeLong),TUintCode(TQcode16::EPokeString),TUintCode(TQcode16::EPokeWord),TUintCode(TQcode16::EPosition),TUintCode(TQcode16::EPosSprite),
	TUintCode(TQcode16::EPrintValue),TUintCode(TQcode16::EPrintSpace),TUintCode(TQcode16::EPrintCrLf),TUintCode(TQcode16::ERaise),TUintCode(TQcode16::ERandomize),
	TUintCode(TQcode16::ERename),TUintCode(TQcode16::EReturnValue),TUintCode(TQcode16::EReturnNull),TUintCode(TQcode16::ERmDir),TUintCode(TQcode16::EScreen),
	TUintCode(TQcode16::EgScreen),TUintCode(TQcode16::EScreenInfo),TUintCode(TQcode16::ESecsToDate),TUintCode(TQcode16::ESetName),TUintCode(TQcode16::ESetPath),
	TUintCode(TQcode16::EStatusWin),TUintCode(TQcode16::EStop),TUintCode(TQcode16::EStyle),TUintCode(TQcode16::ETrap),TUintCode(TQcode16::EUnloadm),
	TUintCode(TQcode16::EUpdate),TUintCode(TQcode16::EUse),TUintCode(TQcode16::EUseSprite),TUintCode(TQcode16::EModify),TUintCode(TQcode16::EInsert),
	TUintCode(TQcode16::ECancel), TUintCode(TQcode16::EPut),TUintCode(TQcode16::EDeleteTable),TUintCode(TQcode16::EgCircle),TUintCode(TQcode16::EgEllipse),
	TUintCode(TQcode16::EGotoMark),TUintCode(TQcode16::EKillMark),TUintCode(TQcode16::EReturnFromEval),TUintCode(TQcode16::EGetEvent32),TUintCode(TQcode16::EGetEventa32),
	TUintCode(TQcode16::EGcolor),TUintCode(TQcode16::ESetflags),TUintCode(TQcode16::ESetdoc),TUintCode(TQcode16::EDaysToDate),TUintCode(TQcode16::EGinfo32),
	TUintCode(TQcode16::EIowaitstat32),TUintCode(TQcode16::ECompact),TUintCode(TQcode16::EBeginTrans),TUintCode(TQcode16::ECommitTrans),TUintCode(TQcode16::ERollBack),
	TUintCode(TQcode16::EClearflags),TUintCode(TQcode16::EPointerFilter),TUintCode(TQcode16::EmCasc),/*TUintCode(TQcode16::EdCheckBox),*/TUintCode(TQcode16::EgSetPenWidth),
	TUintCode(TQcode16::EdEditMulti),TUintCode(TQcode16::EGcolorinfo),TUintCode(TQcode16::EGcolorbackground),TUintCode(TQcode16::EmCardX),TUintCode(TQcode16::ESetHelp),
	TUintCode(TQcode16::EShowHelp),TUintCode(TQcode16::ESetHelpUid),TUintCode(TQcode16::EgXBorder32),
	};

typedef TUint8 TUintFunc; // Type used for entries in this table
static const TUintFunc FunctionTable[TOplFunction::EMaxNumber]=
	{
	TUintFunc(TQcode16::EAddr), TUintFunc(TQcode16::EAsc), TUintFunc(TQcode16::ECall), TUintFunc(TQcode16::ECount), TUintFunc(TQcode16::EDay),
	TUintFunc(TQcode16::EDow), TUintFunc(TQcode16::EEof), TUintFunc(TQcode16::EErr), TUintFunc(TQcode16::EExist), TUintFunc(TQcode16::EFind),
	TUintFunc(TQcode16::EGet), TUintFunc(TQcode16::EIoA), TUintFunc(TQcode16::EIoW), TUintFunc(TQcode16::EIoOpen), TUintFunc(TQcode16::EIoWrite),
	TUintFunc(TQcode16::EIoRead), TUintFunc(TQcode16::EIoClose), TUintFunc(TQcode16::EIoWait), TUintFunc(TQcode16::EHour), TUintFunc(TQcode16::EKey),
	TUintFunc(TQcode16::ELen), TUintFunc(TQcode16::ELoc), TUintFunc(TQcode16::EMinute), TUintFunc(TQcode16::EMonth), TUintFunc(TQcode16::EPeekB),
	TUintFunc(TQcode16::EPeekW), TUintFunc(TQcode16::EPos), TUintFunc(TQcode16::ERecSize), TUintFunc(TQcode16::ESecond), TUintFunc(TQcode16::EUsr),
	TUintFunc(TQcode16::EYear), TUintFunc(TQcode16::EWeek), TUintFunc(TQcode16::EIoSeek), TUintFunc(TQcode16::EKMod), TUintFunc(TQcode16::EKeyA),
	TUintFunc(TQcode16::EKeyC), TUintFunc(TQcode16::EgCreate), TUintFunc(TQcode16::EgCreateBit), TUintFunc(TQcode16::EgLoadBit), TUintFunc(TQcode16::EgLoadFont),
	TUintFunc(TQcode16::EgRank), TUintFunc(TQcode16::EgIdentity), TUintFunc(TQcode16::EgX), TUintFunc(TQcode16::EgY), TUintFunc(TQcode16::EgWidth),
	TUintFunc(TQcode16::EgHeight), TUintFunc(TQcode16::EgOriginX), TUintFunc(TQcode16::EgOriginY), TUintFunc(TQcode16::EgTWidth), TUintFunc(TQcode16::EgPrintClip),
	TUintFunc(TQcode16::ETestEvent), TUintFunc(TQcode16::EOs), TUintFunc(TQcode16::EMenu), TUintFunc(TQcode16::EDialog), TUintFunc(TQcode16::EAlert),
	TUintFunc(TQcode16::ECreateSprite), TUintFunc(TQcode16::ELoadLib), TUintFunc(TQcode16::EUnloadLib), TUintFunc(TQcode16::EFindLib), TUintFunc(TQcode16::EGetLibH),
	TUintFunc(TQcode16::ENewObj), TUintFunc(TQcode16::ENewObjH), TUintFunc(TQcode16::ESend), TUintFunc(TQcode16::EEnterSend), TUintFunc(TQcode16::EEnterSend0),
	TUintFunc(TQcode16::EAlloc), TUintFunc(TQcode16::EReAlloc), TUintFunc(TQcode16::EAdjustAlloc), TUintFunc(TQcode16::ELenAlloc), TUintFunc(TQcode16::EIoc),
	TUintFunc(TQcode16::EUAdd), TUintFunc(TQcode16::EUSub), TUintFunc(TQcode16::EIoCancel), TUintFunc(TQcode16::EStatWinInfo), TUintFunc(TQcode16::EFindField),
	TUintFunc(TQcode16::EDays), TUintFunc(TQcode16::EIAbs), TUintFunc(TQcode16::EInt), TUintFunc(TQcode16::EPeekL), TUintFunc(TQcode16::ESpace),
	TUintFunc(TQcode16::EDateToSecs), TUintFunc(TQcode16::EAbs), TUintFunc(TQcode16::EACos), TUintFunc(TQcode16::EASin), TUintFunc(TQcode16::EATan),
	TUintFunc(TQcode16::ECos), TUintFunc(TQcode16::EDeg), TUintFunc(TQcode16::EExp), TUintFunc(TQcode16::EFlt), TUintFunc(TQcode16::EIntF),
	TUintFunc(TQcode16::ELn), TUintFunc(TQcode16::ELog), TUintFunc(TQcode16::EPeekF), TUintFunc(TQcode16::EPi), TUintFunc(TQcode16::ERad),
	TUintFunc(TQcode16::ERnd), TUintFunc(TQcode16::ESin), TUintFunc(TQcode16::ESqr), TUintFunc(TQcode16::ETan), TUintFunc(TQcode16::EVal),
	TUintFunc(TQcode16::EMax), TUintFunc(TQcode16::EMean), TUintFunc(TQcode16::EMin), TUintFunc(TQcode16::EStd), TUintFunc(TQcode16::ESum),
	TUintFunc(TQcode16::EVar), TUintFunc(TQcode16::EEval), TUintFunc(TQcode16::EChrS), TUintFunc(TQcode16::EDatim), TUintFunc(TQcode16::EDayNameS),
	TUintFunc(TQcode16::EDirS), TUintFunc(TQcode16::EErrS), TUintFunc(TQcode16::EFixS), TUintFunc(TQcode16::EGenS), TUintFunc(TQcode16::EGetS),
	TUintFunc(TQcode16::EHexS), TUintFunc(TQcode16::EKeyS), TUintFunc(TQcode16::ELeftS), TUintFunc(TQcode16::ELowerS), TUintFunc(TQcode16::EMidS),
	TUintFunc(TQcode16::EMonthS), TUintFunc(TQcode16::ENumS), TUintFunc(TQcode16::EPeekS), TUintFunc(TQcode16::EReptS), TUintFunc(TQcode16::ERightS),
	TUintFunc(TQcode16::ESciS), TUintFunc(TQcode16::EUpperS), TUintFunc(TQcode16::EUserS), TUintFunc(TQcode16::EGetCmdS), TUintFunc(TQcode16::ECmdS),
	TUintFunc(TQcode16::EParseS), TUintFunc(TQcode16::ESAddr), TUintFunc(TQcode16::EGreyCreate),TUintFunc(TQcode16::EIoOpenX), TUintFunc(TQcode16::EMenuX),
	TUintFunc(TQcode16::EBookMark), TUintFunc(TQcode16::EGetEventC), TUintFunc(TQcode16::EErrxS), TUintFunc(TQcode16::EInTrans), TUintFunc(TQcode16::EGetDocS),
	TUintFunc(TQcode16::EMPopup), TUintFunc(TQcode16::ESize), TUintFunc(TQcode16::ELocX), TUintFunc(TQcode16::EgPixel)
	};
// Sizes of the variable types for the purposes of accounting for runtime stack usage.
const TUint KOpl16SizeOfLeftSideReference=6;
const TInt KOpl16SizeOfWord=2;
const TInt KOpl16SizeOfLong=4;
const TInt KOpl16SizeOfReal=8;
const TInt KOpl16SizeOfString=0; // Don't keep track of stack for strings.
const TInt KOpl16SizeOfUword=2;

static const TUint8 stackSizes[TOplToken::EMaxNormalType]=
	{
	KOpl16SizeOfWord,KOpl16SizeOfLong,KOpl16SizeOfReal,KOpl16SizeOfString,KOpl16SizeOfUword
	};


static TInt StatementQcodeL(RWriteStream& aStream,TPcodeCommand& aCommand)
//
// Puts out the qcode for a command, checking first to see if it's one of those completely mental
// dialog command which have an index after them
//	
	{
	TUint code=QcodeTable[aCommand.Code()];

    if (code==TQcode16::EdItem)
		{
		aStream<<TQcode16(code); // relies on the fact that they are not trappable
		code=aCommand.Code()-TOplKeyword::EDtext; // Relies on knowledge  that these are contiguous
		}
	return code;
	}

COpl16PcodeConverter* COpl16PcodeConverter::NewLC()
	{
	COpl16PcodeConverter *pC=new(ELeave) COpl16PcodeConverter();
	CleanupStack::PushL(pC);
	pC->ConstructL();
	return pC;
	}

COpl16PcodeConverter* COpl16PcodeConverter::NewL()
	{
	COpl16PcodeConverter *pC=NewLC();
	CleanupStack::Pop();
	return pC;
	}

COpl16PcodeConverter::COpl16PcodeConverter()
	{
	iErrorPos=KOplNoErrorLocation;
	}

COpl16PcodeConverter::~COpl16PcodeConverter()
	{
	delete iStackDepths;
	delete iLabels;
	}

void COpl16PcodeConverter::ConstructL()
	{
	
	iStackDepths=new(ELeave) CTranStackFlat<TUint>(KOpl16PcodeConvStackGran);
	iLabels=new(ELeave) CArrayFixSeg<COplSymbol *>(KOpl16PcodeConvLabelsGran);
	}

void COpl16PcodeConverter::ConvertPcodeL(
//
// Converts a Pcode stream into a Qcode stream
//
//	
	RReadStream& aPcode,
	ROpoWriteStream& aQcode,
	TTranslateError& anError,
	CTextTranslator::TDebugFlag aFlag)
	{

	TUint vectorLabelsCount=0;
	TInt vectorBase=0;
	if (iErrorPos!=KOplNoErrorLocation)
		iErrorPos+=aQcode.SizeL(); // Account for anything that's in the Qcode buffer already

	TPcode next;
	TStreamPos vectorPatchPos=KStreamBeginning;

	FOREVER
		{

		aPcode>>next;
		anError.SetPosition(next.Offset()); // So that we report errors in the right place.

		switch (next.Code())
			{
			case TPcode::EStatement: // If in Debig mode we put out the statement command
				{
				if (aFlag!=CTextTranslator::EDebug)
					continue; // Skip all the size checking etc.
				if (next.Offset()>KMaxTUint16)
					aQcode<<TQcode16(TQcode16::EDebugStatement32)<<TUint32(next.Offset());
				else
					aQcode<<TQcode16(TQcode16::EDebugStatement16)<<TUint16(next.Offset());
				break;
				}
			case TPcode::EConstant:
				{
				CPcodeConstant *constant=CPcodeConstant::NewLC(aPcode);
				AddToStackL(constant->Type());

				switch (constant->Type())
					{
					default:
						Panic(EOpl16GenUnknownConstantType);
					case TOplToken::EWord:
						{
						TInt val=constant->Int();
						if (val<KMinTInt8 || val>KMaxTInt8)
							(aQcode<<TQcode16(TQcode16::EConstant|TOplToken::EWord)).WriteInt16L(val);
						else
							(aQcode<<TQcode16(TQcode16::EStackByteAsWord)).WriteInt8L(val);
						break;
						}
					case TOplToken::ELong:
						{
						TInt32 val=constant->Int();
						if (val>=KMinTInt8 && val<=KMaxTInt8)
							(aQcode<<TQcode16(TQcode16::EStackByteAsLong)).WriteInt8L(val);
						else if (val>=KMinTInt16 && val<=KMaxTInt16)
							(aQcode<<TQcode16(TQcode16::EStackWordAsLong)).WriteInt16L(val);
						else
							aQcode<<TQcode16(TQcode16::EConstant|TOplToken::ELong)<<val;
						break;
						}
					case TOplToken::EReal:
						aQcode<<TQcode16(TQcode16::EConstant|TOplToken::EReal)<<constant->Real();
						break;
					case TOplToken::EString:
						aQcode<<TQcode16(TQcode16::EConstant|TOplToken::EString)<<TLbc(constant->String());
						break;
					}	
				CleanupStack::PopAndDestroy();
				break;
				}
			case TPcode::EEvalIdentifier:
			case TPcode::EIdentifier:
				{
				TPcodeSymbolReference aRef;
				aPcode>>aRef;

				TUint code=TQcode16::EIdentifier;

				// Left or Right side				
				if (aRef.Symbol()->Token()==TOplToken::EArray)
					RemoveFromStack(TOplToken::EWord);
				code|=AddToStackL(aRef.Side(),aRef.Symbol()->Type());

				// Direct or Indirect
				COplSymbol::TClass symClass=aRef.Symbol()->Class();
				if (symClass==COplSymbol::EExternalRef || symClass==COplSymbol::EArgument)
					code|=KOpl16ExternalReference;

				// Simple or array 
				if (aRef.Symbol()->Token()==TOplToken::EArray)
					code|=KOpl16ArrayReference;

				// Put out the Qcode
				if (next.Code()==TPcode::EIdentifier) // 'Normal' qcode module
					aQcode<<TQcode16(code|aRef.Symbol()->Type())<<TUint16(aRef.Symbol()->Address());
				else // Eval identifier reference.
					{
					TUint code=(aRef.Side()==TPcode::ERight ? TQcode16::EEvalRightSideExternal : TQcode16::EEvalLeftSideExternal);
					aQcode<<TQcode16(code)<<TLbc(aRef.Symbol()->Name())<<TGlobalType(aRef.Symbol());
					}
                break;
				}
			case TPcode::EField:
				{
				CPcodeFieldReference *field=CPcodeFieldReference::NewLC(aPcode);
				TUint code=TQcode16::EField|AddToStackL(field->Side(),field->Type());

				// Stack field name, then Field code and logical device
				aQcode<<TQcode16(TQcode16::EConstant|TOplToken::EString)<<TLbc(field->Name())<<TQcode16(code|field->Type())<<TUint8(field->Device());
				CleanupStack::PopAndDestroy();
				}
				break;
			case TPcode::EProcCallName: // Ignore these, they exist for the benefit of the Munger
				break;
			case TPcode::EArgList:
				iStackDepths->PushL(&iStackDepth); // Save the stack depth
				break;

			case TPcode::EProcArg:
			case TPcode::EOpxArg:
			case TPcode::EFunctionArg:
				{
				TPcodeCallArgument arg;
				aPcode>>arg;

				if (next.Code()==TPcode::EProcArg) // For procedures we also stack the argument type
					{
					aQcode<<TQcode16(TQcode16::EStackByteAsWord)<<TUint8(arg.Type());
					AddToStackL(TOplToken::EWord);
					}
				break;
				}			
			
			case TPcode::EOpxCall:
			case TPcode::EProcCall: // Procedure call
				{
				TPcodeCall call;
				aPcode>>call;
				AdjustStackForCallL(call.Symbol()->Type());

				TUint addr=call.Symbol()->Address();
				if (next.Code()==TPcode::EProcCall)
					aQcode<<TQcode16(TQcode16::ECallProcedure)<<TUint16(addr);
				else
					{
					TOpxAddress opxFunc(addr);
					aQcode<<TQcode16(TQcode16::EOpxCall)<<TUint8(opxFunc.Module())<<TUint16(opxFunc.Function());
					}
				break;
				}
			case TPcode::EProcByNameCall:
				{
				TPcodeProcByName proc;
				aPcode>>proc;
				AdjustStackForCallL(proc.Type());

				aQcode<<TQcode16(TQcode16::ECallProcByName)<<TUint8(proc.ArgumentCount())<<TUint8(procByNameTypeBytes[proc.Type()]);
				break;
				}
			case TPcode::EFixedFunctionCall:
			case TPcode::ECountedFunctionCall:
				{
				TPcodeFunctionCall function;
				aPcode>>function;
				AdjustStackForCallL(function.Type());

				RWriteStream& code=aQcode<<TQcode16(TQcode16::EFunctionCall)<<FunctionTable[function.Number()];
				if (next.Code()==TPcode::ECountedFunctionCall)
					code<<TUint8(function.ArgumentCount());
				break;
				}
			case TPcode::EDrop:
				{
				TPcodeDrop drop;
				aPcode>>drop;

				RemoveFromStack(drop.Type());
				__ASSERT_ALWAYS(iStackDepth==0,Panic(EOpl16StackError));

				aQcode<<TQcode16(TQcode16::EDropValue|drop.Type());
				break;
				}
			case TPcode::EAssign:
				{
				TPcodeAssign assign;
				aPcode>>assign;

				aQcode<<TQcode16(TQcode16::EAssign|assign.Type());
				ZeroStack();
				break;
				}
			case TPcode::ECast:
				{
				TPcodeCast cast;
				aPcode>>cast;

				TUint castCode=casts[cast.From()][cast.To()];
				if (castCode==TQcode16::EExtended) // type mismatch
					User::Leave(EErrTypeMismatch);
				if (castCode!=0) // Really do need a cast
					{
					RemoveFromStack(cast.From());
					AddToStackL(cast.To());
					aQcode<<TQcode16(castCode);
					}
				break;
				}
			case TPcode::EOperator:
				{
				TPcodeOperator oper;
				aPcode>>oper;

				RemoveFromStack(oper.OperandType());
				if (oper.Arity()==TPcodeOperator::EBinary)
					RemoveFromStack(oper.OperandType());
				AddToStackL(oper.ResultingType());
				
				TUint code=operators[oper.Oper()-TOplToken::EStartOfExpression];
				if (code!=0) // Some operators are only there for the Qcode cracker
					{
					if (code<TQcode16::EPercLess) // Not a % operator
						code|=oper.OperandType();
					aQcode<<TQcode16(code);
					}
				break;
				}
			case TPcode::ECommand:
				{
				TPcodeCommand command;
				aPcode>>command;
				
				aQcode<<TQcode16(StatementQcodeL(aQcode,command));
				ZeroStack();
				break;
				}
			case TPcode::ETypedCommand:
				{
				TPcodeTypedCommand command;
				aPcode>>command;

				aQcode<<TQcode16(StatementQcodeL(aQcode,command)|command.Type());
				ZeroStack();
				break;
				}
			case TPcode::EQualifiedCommand:
				{
				TPcodeQualifiedCommand command;
				aPcode>>command;

				aQcode<<TQcode16(StatementQcodeL(aQcode,command))<<TUint8(command.Qualifier());
				ZeroStack();
				break;
				}
			case TPcode::ELabel: // Label declaration
				{
				TPcodeLabel label;

				aPcode>>label;
				label.Symbol()->SetAddress(aQcode.Sink()->SeekL(0,EStreamEnd).Offset());
				iLabels->AppendL(label.Symbol());
				break;
				}
			case TPcode::EBranchFalse:
			case TPcode::EBranchAlways:
			case TPcode::EOnErr:
				{
				TInt base=aQcode.SizeL(); // Calculate label offset from before branch instruction
				if (next.Code()==TPcode::EBranchFalse)
					{
					RemoveFromStack(TOplToken::EWord); // Takes the conditional expression off the stack
					aQcode<<TQcode16(TQcode16::EBranchFalse);
					}
				else if (next.Code()==TPcode::EBranchAlways)
					aQcode<<TQcode16(TQcode16::EGoto);
				else
					aQcode<<TQcode16(TQcode16::EOnerr);
				LabelReferenceL(aPcode,aQcode,base);
				break;
				}
			case TPcode::EFieldName:
				{
				CPcodeFieldName *field=CPcodeFieldName::NewLC(aPcode);
				aQcode<<TUint8(field->Type())<<TLbc(field->Name());
				CleanupStack::PopAndDestroy();
				break;
				}
			case TPcode::EEndFieldList:
				{
				aQcode<<TUint8(KOpl16EndOfFieldList);
				break;
				}
			case TPcode::EVector:
				{
				vectorBase=aQcode.SizeL();
				aQcode<<TQcode16(TQcode16::EVector);
				vectorPatchPos=aQcode.AddPatch16L();
				vectorLabelsCount=0;
				ZeroStack();
				break;
				}
			case TPcode::EVectorLabel:
				LabelReferenceL(aPcode,aQcode,vectorBase);
				vectorLabelsCount++;
				break;
			case TPcode::EEndVector:
				aQcode.Patch16L(vectorPatchPos,vectorLabelsCount);
				break;
			default:
				Panic(EOpl16GenUnknownPCode);
			}
		if (aQcode.SizeL()>iErrorPos)
			User::Leave(EErrFound);
		}
	}

void COpl16PcodeConverter::LabelReferenceL(RReadStream& aPcode, ROpoWriteStream& aQcode,TInt aBase)
/*
Does everytyhing needed to put out a lebel refernce
*/	
	{
	COplLabelSymbol* pLabel;
	pLabel=(COplLabelSymbol*)aPcode.ReadUint32L();
	
	if (pLabel==NULL)
		aQcode<<TUint16(0);
	else
		{
		if (!pLabel->IsDefined())
			User::Leave(EErrMissingLabel);
		pLabel->AddReferenceL(TLabelReference(aQcode.AddPatch16L(),aBase));
		}
	}


void COpl16PcodeConverter::AddToStackL(TOplToken::TType aType)
	{
	AddToStackL(SizeOnStack(aType));
	}

void COpl16PcodeConverter::AddToStackL(TUint byteCount)
	{
	iStackDepth+=byteCount;
	if (iStackDepth>iMaxStackDepth)
		iMaxStackDepth=iStackDepth;
	}

TUint COpl16PcodeConverter::AddToStackL(TPcode::TIdentifierSide aSide ,TOplToken::TType aType)
/*
Adds the appropriate side to the stack	
*/
	{
	TUint sideBits=0;
	if (aSide==TPcode::ERight) // It's a rightside reference
		AddToStackL(aType);
	else
		{
		AddToStackL(KOpl16SizeOfLeftSideReference);
		sideBits=KOpl16LeftSideReference;
		}
	return sideBits;
	}


void COpl16PcodeConverter::RemoveFromStack(TOplToken::TType aType)
	{
	
	iStackDepth-=SizeOnStack(aType);
	}


TUint COpl16PcodeConverter::SizeOnStack(TOplToken::TType aType)
	{
	return stackSizes[aType];
	}


void COpl16PcodeConverter::AdjustStackForCallL(TOplToken::TType aReturnType)
	{
	
	iStackDepths->Pop(&iStackDepth); // Pop the arguments off the stack
	AddToStackL(aReturnType); // Replace them with the return type
	}

TInt COpl16PcodeConverter::RunL(CBufBase& aPcodeBuffer, ROpoWriteStream& aQcodeStream,
								TTranslateError& anError, CTextTranslator::TDebugFlag aFlag)
	
//
// Streams Qcode out to aQcodeBuffer according to Pcode in aPCodeBuffer
//
	{
	RBufReadStream pcodeStream(aPcodeBuffer);
	iStackDepth=0;
	iMaxStackDepth=0;

	TRAPD(ret,ConvertPcodeL(pcodeStream,aQcodeStream,anError,aFlag));

	if (ret!=KErrEof) // Run out of stream.
		User::Leave(ret);

	// Patch the label references now that we know where the labels actually are
	for (TUint index=0,max=iLabels->Count();index<max;index++)
		{
		COplLabelSymbol* pLabel=(COplLabelSymbol*)((*iLabels)[index]);
		const CArrayFixFlat<TLabelReference>& references=pLabel->References();
		for (TUint refIndex=0,refMax=references.Count();refIndex<refMax;refIndex++)
			{
			const TLabelReference& ref=references[refIndex];
			aQcodeStream.Patch16L(ref.Position(),pLabel->Address()-ref.Base()); // Plus one is becase references are off 
			}
		}
	iLabels->Reset();
	iStackDepths->Reset();
	return iMaxStackDepth;
	}

//////////////////////////////////////////////////////////////////////////////////
//
// TOpl16ProcEntry
//
//////////////////////////////////////////////////////////////////////////////////
TOpl16ProcEntry::TOpl16ProcEntry(const TDesC& aName,TInt aLine, TInt aPosition)
	: iName(aName),iLineNumber(aLine), iModulePos(aPosition)
	{
	}


//////////////////////////////////////////////////////////////////////////////////
//
// TSymbolArrayWalkL(s) - classes for walking the symbol arrays
//
//////////////////////////////////////////////////////////////////////////////////
class TSymbolArrayWalk
	{
public:
	void WalkL(const CArrayFix<COplSymbol*>& aSymbolArray, TInt &aTotal, TInt *anArg=NULL);
private:
	virtual void ProcessSymbolL(COplSymbol *aSymbol, TInt& aTotal, TInt* anArg)=0;
	};

void TSymbolArrayWalk::WalkL(const CArrayFix<COplSymbol*>&aSymbolArray,TInt& aTotal,TInt *anArg)
//
// Walks an array of symbols
//
	{
	
	TInt index=0;
	TInt count=aSymbolArray.Count();
	while (index<count)
		{
		ProcessSymbolL(aSymbolArray[index],aTotal,anArg);
		index++;
		}
	}

//////////////////////////////////////////////////////////////////////////////////
//
// TGlobalTableWalk - Works out the size of the global table
//
//////////////////////////////////////////////////////////////////////////////////
class TGlobalTableWalk : public TSymbolArrayWalk
	{
private:
	void ProcessSymbolL(COplSymbol* aSymbol,TInt& aTotal, TInt * anArg);
	};

// Extra space used by an entry in the global lookup table
// above and beyond the klength of the name.
// 2 for the lbc for the name length
// 2 for the type
// 2 are for the address of the variable.
const TInt KOpl16GlobalNameOverhead=6;

void TGlobalTableWalk::ProcessSymbolL(COplSymbol * aSymbol,TInt& aTotal, TInt* anArg)
//
// Works out the size of the entry.
//	
	{
	TInt entrySize=aSymbol->Name().Length()*sizeof(TText)+KOpl16GlobalNameOverhead;
	aTotal+=entrySize; // Total data size
	(*anArg)+=entrySize; // Size of globals table
	}

//////////////////////////////////////////////////////////////////////////////////
//
// TProcTableWalk - Works out details of the procedure table
//
//////////////////////////////////////////////////////////////////////////////////
class TProcTableWalk : public TSymbolArrayWalk
	{
private:
	void ProcessSymbolL(COplSymbol* aSymbol,TInt& aTotal, TInt *);
	};

// Extra space used in addition to the name in the 
// table containing thh names of all the procedures called
// 2 is for the name lbc
// 2 for the number of paramters passed
const TInt KOpl16ProcedureNameOverhead=4;

void TProcTableWalk::ProcessSymbolL(COplSymbol * aSymbol,TInt& aTotal, TInt*anArg)
//
// Works out the size of teh entry.
//	
	{
	aSymbol->SetAddress(aTotal);
	TInt entrySize=aSymbol->Name().Length()*sizeof(TText)+KOpl16ProcedureNameOverhead;
	aTotal+=entrySize;
	(*anArg)+=entrySize; // Procedure table size
	}

//////////////////////////////////////////////////////////////////////////////////
//
// TArgumentIndirectionsWalk - Works out teh addresses of an entry in the indirections table
//
//////////////////////////////////////////////////////////////////////////////////
class TIndirectionsWalk : public TSymbolArrayWalk
	{
private:
	void ProcessSymbolL(COplSymbol* aSymbol,TInt& aTotal, TInt *);
	};

void TIndirectionsWalk::ProcessSymbolL(COplSymbol * aSymbol,TInt& aTotal, TInt* /*anArg*/)
//
// Works out the size of the entry.
//	
	{
	aSymbol->SetAddress(aTotal);
	aTotal+=sizeof(TUint16);
	}

//////////////////////////////////////////////////////////////////////////////////
//
// TVariableAddresesWalk - Works out the addresses of the variables in the frame (LOCAL, GLOBAL)
//
//////////////////////////////////////////////////////////////////////////////////
class TVariableAddressesWalk : public TSymbolArrayWalk
	{
private:
	void ProcessSymbolL(COplSymbol* aSymbol,TInt& aTotal, TInt *);
	};
const TInt KOpl16StringLenOverhead=2; // Two lbc bytes.
const TInt KOpl16StringMaxLenOverhead=2; // Max length
const TInt KOpl16ArraySizeOverhead=2;   // Array size

void TVariableAddressesWalk::ProcessSymbolL(COplSymbol * aSymbol,TInt& aTotal, TInt* /*anArg*/)
//
// Works out the address of a global or Local symbol
//	
	{
	aSymbol->SetAddress(aTotal);
	COplAutomaticSymbol *automatic=(COplAutomaticSymbol *)aSymbol;
	TInt size=stackSizes[aSymbol->Type()];
	if (!size) // It's a string
		{
		size=automatic->StringLength()*sizeof(TText)+KOpl16StringLenOverhead;
		aSymbol->SetAddress(aTotal+KOpl16StringLenOverhead);
		}
	// It's an array
	if (aSymbol->Token()==TOplToken::EArray)
		{
		size*=automatic->ArraySize();
		size+=KOpl16ArraySizeOverhead;
		aSymbol->SetAddress(aSymbol->Address()+KOpl16ArraySizeOverhead);
		}
	// Now add in overhead for max string length if needed
	if (aSymbol->Type()==TOplToken::EString)
		size+=KOpl16StringMaxLenOverhead;
	aTotal+=size;
	}

//////////////////////////////////////////////////////////////////////////////////
//
// TOutputWalk
//
//////////////////////////////////////////////////////////////////////////////////
LOCAL_C void NameTypeAndAddressL(COplSymbol *aSymbol, RWriteStream& aStream)
	{
	aStream<<TLbc(aSymbol->Name())<<TGlobalType(aSymbol)<<TUint16(aSymbol->Address());
	}

class TOutputWalk
	{
public:
	void WalkL(const CArrayFix<COplSymbol*>& anArray,RWriteStream& aStream);
private:
	virtual void OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream)=0;
	};

void TOutputWalk::WalkL(const CArrayFix<COplSymbol*>& anArray, RWriteStream& aStream)
	{
	TInt count=anArray.Count();
	TInt index=0;
	while (index<count)
		{
		OutputSymbolL(anArray[index], aStream);
		index++;
		}
	}

///////////////////////////////////////////////////////////////////////////////////
//
// TGlobalOutputWalk
//
///////////////////////////////////////////////////////////////////////////////////
class TGlobalOutputWalk : public TOutputWalk
	{
private:
	void OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream);
	};

void TGlobalOutputWalk::OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream)
	{
	NameTypeAndAddressL(aSymbol,aStream);
	}

///////////////////////////////////////////////////////////////////////////////////
//
// TProcOutputWalk
//
///////////////////////////////////////////////////////////////////////////////////
class TProcOutputWalk : public TOutputWalk
	{
private:
	void OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream);
	};

void TProcOutputWalk::OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream)
//
// Getting back to the original call is a bit horrendous Most of the time we don't need to do this
//
	{
	COplCallSymbol *call=(COplCallSymbol *)(&((COplReferenceSymbol *)aSymbol)->Declaration());
	(aStream<<TLbc(aSymbol->Name())).WriteUint16L(call->Signature().ArgumentCount());
	}


///////////////////////////////////////////////////////////////////////////////////
//
// TStringsOutputWalk
//
///////////////////////////////////////////////////////////////////////////////////
class TStringsOutputWalk : public TOutputWalk
	{
private:
	void OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream);
	};

void TStringsOutputWalk::OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream)
	{
   if (aSymbol->Type()==TOplToken::EString)
      {
      COplAutomaticSymbol *automatic=(COplAutomaticSymbol *)aSymbol;
	  aStream<<TUint16(automatic->Address()-KOpl16StringLenOverhead)<<TUint16(automatic->StringLength());
      }
	}

///////////////////////////////////////////////////////////////////////////////////
//
// TArraysOutputWalk
//
///////////////////////////////////////////////////////////////////////////////////
class TArraysOutputWalk : public TOutputWalk
	{
private:
	void OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream);
	};

void TArraysOutputWalk::OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream)
	{
	if (aSymbol->Token()==TOplToken::EArray)
		{
		COplAutomaticSymbol *automatic=(COplAutomaticSymbol *)aSymbol;
		TUint addr=automatic->Address()-sizeof(TUint16);
		if (automatic->Type()==TOplToken::EString)
			addr-=sizeof(TUint16);
		aStream<<TUint16(addr)<<TUint16(automatic->ArraySize());
		}
	}
///////////////////////////////////////////////////////////////////////////////////
//
// TExternalsOutputWalk
//
///////////////////////////////////////////////////////////////////////////////////
class TExternalsOutputWalk : public TOutputWalk
	{
private:
	void OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream);
	};

void TExternalsOutputWalk::OutputSymbolL(COplSymbol *aSymbol, RWriteStream& aStream)
	{
	aStream<<TLbc(aSymbol->Name())<<TGlobalType(aSymbol);
	}

///////////////////////////////////////////////////////////////////////////////////
//
// TSymbolWalk
//
///////////////////////////////////////////////////////////////////////////////////
class TSymbolWalk
	{
public:
	TStreamId WalkL(CFileStore *aFileStore, const CArrayFix<COplSymbol*>& anArray);
private:
	virtual void OutputSymbolDetailsL(COplSymbol *aSymbol, RWriteStream& aStream);
	};

TStreamId TSymbolWalk::WalkL(CFileStore *aFileStore, const CArrayFix<COplSymbol*>& anArray)
//
// Creates a stream and does the walk. Returns the stream id2
//
	{
	RStoreWriteStream stream;
	TStreamId streamId=stream.CreateLC(*aFileStore);

	TInt count=anArray.Count();
	TInt index=0;
	while (index<count)
		{
		COplSymbol *symbol=anArray[index];
		NameTypeAndAddressL(symbol, stream);
		OutputSymbolDetailsL(symbol, stream);
		index++;
		}
	stream<<TUint8(0); // Terminate the list
	
	stream.CommitL();
	CleanupStack::PopAndDestroy();
	return streamId;
	} 

void TSymbolWalk::OutputSymbolDetailsL(COplSymbol* /* aSymbol*/, RWriteStream& /*aStream*/)
//
// Just a place marker
//
	{
	}


///////////////////////////////////////////////////////////////////////////////////
//
// TAutomaticSymbolWalk
//
///////////////////////////////////////////////////////////////////////////////////
class TAutomaticSymbolWalk : public TSymbolWalk
	{
private:
	virtual void OutputSymbolDetailsL(COplSymbol *aSymbol, RWriteStream& aStream);
	};

void TAutomaticSymbolWalk::OutputSymbolDetailsL(COplSymbol* aSymbol, RWriteStream& aStream)
//
// Puts out the maximum string and array sizes - may be 0
//
	{
	COplAutomaticSymbol *automatic=(COplAutomaticSymbol *)aSymbol;
	//!!TODO Debugger alignment: need to force alignment here?
	aStream<<TUint16(automatic->ArraySize())<<TUint16(automatic->StringLength()*sizeof(TText));
	}

/////////////////////////////////////////////////////////////////////////
//
//	TBackEndFile - Used to cleanup writing the output file if anything goes wrong
//
/////////////////////////////////////////////////////////////////////////
struct TBackEndFile
	{
	inline TBackEndFile(COpl16Opl1993BackEnd& aBackEnd,RFile& aFile) : iBackEnd(aBackEnd), iFile(aFile) {}
	COpl16Opl1993BackEnd& iBackEnd;
	RFile& iFile;
	};


void FileCleanup(void *aBackEndFile)
// Closes and deletes the output file if anything goes wrong
//
	{
	TBackEndFile *backEndFile=(TBackEndFile *)aBackEndFile;
	backEndFile->iBackEnd.TidyFile(backEndFile->iFile);	
	}


//////////////////////////////////////////////////////////////////////////////////
//
// COpl16ModuleBackEnd - Common 16 bit opcode back end
//
/////////////////////////////////////////////////////////////////////////////////
void COpl16ModuleBackEnd::StartL(const TDesC& aSourceName,CTextTranslator::TDebugFlag aDebugFlag, TSourceTranslateError& anError)
//
// Sets things up ready to go.
//
	{

	__ASSERT_DEBUG(iState==EReset,Panic(EOpl16BackEndReentrantStart));
	iError=&anError;
	iDebugFlag=aDebugFlag;
	iOpo.Open(*iQcodeBuffer); // Gives a Patchable in memory stream to play with

	DoStartL(aSourceName);
	iState=EStarted;
	}

COplParseApp* COpl16ModuleBackEnd::GetAppLC(TOplTranTarget aTarget,COplLexerBase& aLexer)
//
// Returns 16 bit APP ENDA parser
//
	{
	
	COpl16App *pA=new(ELeave) COpl16App(aTarget,aLexer);
	CleanupStack::PushL(pA);
	return pA;
	}

void COpl16ModuleBackEnd::OplTargetIsApp()
	{
	if (iAppCB!=NULL)
		iAppCB->OplTargetIsAppL(iOutputName);
	}

void COpl16ModuleBackEnd::AppL(COplParseApp* anApp)
//
// Just parsed an APP...ENDA construct and need to add it to file
// Takes ownership of the passed object IF it succeeds
//
	{

	__ASSERT_DEBUG(iState==EStarted,Panic(EOpl16BackEndAppState));

	DoAppL((COpl16App *)anApp);
	delete anApp;
	iState=EAppSeen;
	}

void COpl16ModuleBackEnd::StartProcL(const TDesC& aName,TInt aLineNumber, TInt anErrorOffset )
//
// Starts a new procedure
//
	{
	__ASSERT_DEBUG(iState!=EReset && iState!=EAwaitingBody,Panic(EOpl16BackEndProcState));
	if (iState!=EProcSeen)
		FirstProcL();
	
	iConverter->SetErrorOffset(anErrorOffset);
	TInt pos=DoStartProcL();
	TOpl16ProcEntry tableEntry(aName, aLineNumber, pos);
	//!!TODO fix this to be upper case in Unicode???
	TKeyArrayFix aKey(_FOFF(TOpl16ProcEntry,iName),ECmpFolded);
	TInt dummy;
	
	if (iProcs->Find(tableEntry,aKey,dummy)==0)
		User::Leave(EErrDuplicateName);
	iProcs->AppendL(tableEntry);
	
	iState=EAwaitingBody;
	}

void COpl16ModuleBackEnd::ProcL(COplProcHeader *aHeader, CBufSeg *aPcodeBuffer)
//
// We construct the stream fragment for the Procedure in the iOpo
// stream and then have the particular back end do with it what it will
//
	{	
	 
	__ASSERT_DEBUG(iState==EAwaitingBody,Panic(EOpl16BackEndBodyState));
	
	// First we process the header
	TStreamPos qcodeSizePatchPos;
	TStreamPos stackDepthPatchPos;
	ProcessProcHeaderL(aHeader,qcodeSizePatchPos,stackDepthPatchPos);
	
	// And now generate the main Qcode
	TInt qcodeStart=iOpo.SizeL();
	iConverter->RunL(*aPcodeBuffer,iOpo,*iError,iDebugFlag);
	
	// Go back and make patches in header based on what we now know.
	iOpo.Patch16L(qcodeSizePatchPos,iOpo.SizeL()-qcodeStart);
	iOpo.Patch16L(stackDepthPatchPos,iConverter->MaxStackDepth());

	// Now do whatever's needed to add this
	// to the particular file type
	DoProcL();
	
	// And finally tidy up a bit
	delete aHeader;
	delete aPcodeBuffer;
	iState=EProcSeen;
	}


void COpl16ModuleBackEnd::EndOfFileL()
//
// Reached the end of the source. Sort out all the bits
// and pieces at the end of the file.
//
	{
	// Procedure table
	RWriteStream& procTable=StartProcTableL();
	for (TInt index=0;index<iProcs->Count();index++)
		{
		TOpl16ProcEntry& proc=(*iProcs)[index];
		procTable<<TLbc(proc.iName)<<TUint32(proc.iModulePos)<<TUint16(proc.iLineNumber);
		}
	// This is still 8bit -- see corresponding OPLR CModuleReader::ReadProcHeaderL()
	procTable.WriteUint8L(0); // Terminate the list
	DoEndOfFileL();
	}


void COpl16ModuleBackEnd::EnsurePathL()
//
// Ensures that the path for the output file exists
//
	{
	TInt ret=iFs.MkDirAll(iOutputName);
	if (ret!=KErrAlreadyExists)
		User::LeaveIfError(ret);
	}


COpl16ModuleBackEnd::COpl16ModuleBackEnd(const TDesC& aTargetFile, MTranAppCB *anAppCB)
	: iState(COpl16ModuleBackEnd::EReset), iAppCB(anAppCB), iOutputName(aTargetFile)
//
//
//
	{
	}

void COpl16ModuleBackEnd::ConstructL()
//
// Second stage risky construction
//
	{

	User::LeaveIfError(iFs.Connect()); // Connect to the file server
	iQcodeBuffer=CBufSeg::NewL(KOpl16QcodeBufferGran);
	iConverter=COpl16PcodeConverter::NewL();
	iProcs=new(ELeave) CArrayFixFlat<TOpl16ProcEntry>(KOpl16ProcsListGran);
	}
	
COpl16ModuleBackEnd::~COpl16ModuleBackEnd()
//
//
//
	{
	delete iProcs;
	delete iConverter;
	delete iQcodeBuffer;
	iFs.Close();
	}

void COpl16ModuleBackEnd::DebugSymbolsL(COplProcHeader * /*aHeader*/)
//
// Default does nothing
//
	{
	}

// Size of the fixed stuff at the stat of the frame,to whit. 9 16-bit words
// previous frame pointer	previous pC		previous Sp
// error recover address	pModuleCell		pProcName
// pQcode					max stack		size of globals table
const TInt KOpl16FrameFixedSize=18;
void COpl16ModuleBackEnd::ProcessProcHeaderL(COplProcHeader *aHeader,TStreamPos& aQcodeSizePos, TStreamPos& aStackPatchPos)
//
// Does two things:
//		Works out the addresses of all the variables
//		Writes out the information needed to generate the header
	{
	
	// 1) Total data size and address calculations

	// Theres some fixed stuff at the start of the frame.
	TInt totalDataSize=KOpl16FrameFixedSize;
	
	// ... Work out the size of the global table
	TInt globalTableSize=0;
		{
		TGlobalTableWalk globalWalk;
		globalWalk.WalkL(aHeader->Globals(),totalDataSize,&globalTableSize);
		}
	
	// ... the procedure call table
	TInt procTableSize=0;
		{
		TProcTableWalk procWalk;
		procWalk.WalkL(aHeader->ProcCalls(),totalDataSize,&procTableSize);
		}
	
	// ... indirections addresses
		{
		TIndirectionsWalk indirectionsWalk;
		indirectionsWalk.WalkL(aHeader->Arguments(),totalDataSize);
		indirectionsWalk.WalkL(aHeader->Externals(),totalDataSize);
		}

	// ... variable addresses
		{
		TVariableAddressesWalk addrWalk;
		addrWalk.WalkL(aHeader->Globals(),totalDataSize);
		addrWalk.WalkL(aHeader->Locals(),totalDataSize);
		}

	// By now we've set the addresses and got (almost) all the data we
	// need to write the header out to the file
	// Note: for Thumb, the contents of the header must always be
	// even-word aligned. e.g. TStringsOutputWalk would need to force
	// alignment if the header alignment changed.
	TStreamPos headerSizePatchPos=iOpo.AddPatch16L();
	if (((TUint32)totalDataSize)>KMaxTUint16)
		User::Leave(EErrDataTooLarge);	//rick -- this doesn't quite work, it's possible to get .OPOs with
										// PROCs slightly larger than 64K. The runtime then panics.
	iOpo.WriteUint16L(totalDataSize);
	aQcodeSizePos=iOpo.AddPatch16L();
	aStackPatchPos=iOpo.AddPatch16L();
	
	// Put out the bytes for the proc argument types
	TInt count=aHeader->Arguments().Count();

	iOpo.WriteUint16L(count--);
	for (;count>=0;count--)
		iOpo<<TUint16(aHeader->Arguments()[count]->Type());
	// Global look-up table
	iOpo.WriteUint16L(globalTableSize);
		{
		TGlobalOutputWalk globalWalk;
		globalWalk.WalkL(aHeader->Globals(),iOpo);
		}

	// Procedures name table
	iOpo.WriteUint16L(procTableSize);
		{
		TProcOutputWalk procWalk;
		procWalk.WalkL(aHeader->ProcCalls(),iOpo);
		}

	// External references table - write out the look-up table for the external references
		{
		TExternalsOutputWalk externalsWalk;
		externalsWalk.WalkL(aHeader->Externals(),iOpo);
		}
	iOpo.WriteUint16L(0); // Terminate the list with an two byte (lbc) sized value.

	// String Fix-ups
		{
		TStringsOutputWalk stringsWalk;
		stringsWalk.WalkL(aHeader->Globals(),iOpo);
		stringsWalk.WalkL(aHeader->Locals(),iOpo);
		}
	iOpo.WriteUint16L(0);

	// Array fix-ups
		{
		TArraysOutputWalk arraysWalk;
		arraysWalk.WalkL(aHeader->Globals(),iOpo);
		arraysWalk.WalkL(aHeader->Locals(),iOpo);
		}
	iOpo.WriteUint16L(0);

	// Don't include the header size word in the size of the header
	iOpo.Patch16L(headerSizePatchPos,iOpo.SizeL()-(headerSizePatchPos.Offset()+sizeof(TUint16)));

	// And put out any debug information
	if (iDebugFlag==CTextTranslator::EDebug)
		DebugSymbolsL(aHeader);
	}

///////////////////////////////////////////////////////////////////////////////////
// 
// COpl16Opl1993BackEnd  - Back end for OPL1993 compatible modules
//
//////////////////////////////////////////////////////////////////////////////////

COpl16Opl1993BackEnd* COpl16Opl1993BackEnd::NewL(const TDesC& aTargetFile, MTranAppCB* anAppCB)
//
//
//
	{
	COpl16Opl1993BackEnd* pBe=new(ELeave) COpl16Opl1993BackEnd(aTargetFile,anAppCB);
	CleanupStack::PushL(pBe);
	pBe->ConstructL();
	CleanupStack::Pop();
	return pBe;
	}

void COpl16Opl1993BackEnd::OpxL(COplParseOpx* /*anOpx*/)
	{

	Panic(EOpl16Opl1993NoOpx); // Opl1993 doesn't support OPX's
	}

TInt COpl16Opl1993BackEnd::OpxCount()
	{

	return 0;
	}

static const TText Opl16ModuleHeader1[]=
	{
	'O','P','L','O','b','j','e','c','t','F','i','l','e','*','*','\0', // File signature
	0x0001				// File structure version
	};

void COpl16Opl1993BackEnd::DoStartL(const TDesC& aSourceName)
//
// Opens up that scratch buffer and starts a writing
//
	{
	iOpo.WriteL((TUint8*)Opl16ModuleHeader1,sizeof(Opl16ModuleHeader1)/sizeof(TUint8));
	iDataPatchPos=iOpo.AddPatch16L(); // Will be set to point to the start of the 'real' data
	iOpo<<TLbc(aSourceName);
	}


void COpl16Opl1993BackEnd::DoAppL(COpl16App* anApp)
//
// Put anApp out to the file
//
	{

	COpl16App& app=*(COpl16App*)anApp;

	// ICON
	if (!(app.Flags()&KOpl16AppIconFlag)) // No Icon
		iOpo.WriteUint16L(0);
	else
		{
		TAutoClose<RFile> iconFile;
		iconFile.PushL();
		User::LeaveIfError(iconFile.iObj.Open(iFs,app.IconFile(),EFileStream|EFileShareReadersOnly));

		TInt iconLen;
		User::LeaveIfError(iconFile.iObj.Size(iconLen));
		if (iconLen>KOpl16AppMaxIconSize)
			User::Leave(EErrDataTooLarge);
		iOpo.WriteUint16L(iconLen);

		// Get some space in the qcode buffer to read in the buffer
		TUint reserve=iconLen;
		if (reserve>KOpl16MaxIconReadBufSize)
			reserve=KOpl16MaxIconReadBufSize;
			
		// And read the icon in reasonable sized chunks
		TAny *pReadBuf=User::AllocLC(reserve);
		TPtr8 readBuf((TUint8 *)pReadBuf,reserve);
		FOREVER
			{
			User::LeaveIfError(iconFile.iObj.Read(readBuf));
			if (readBuf.Length()==0)
				break;
			iOpo.WriteL(readBuf.Ptr(),readBuf.Length());
			}
		CleanupStack::PopAndDestroy(2); // Read buffer.& icon file
		}
	
	// Fixed length Stuff
	iOpo.WriteUint16L(KOpl16AppDataLen);

	// Default name and extension
	_LIT(KOpltOdb,".ODB");
	if (!(app.Flags()&KOpl16AppExtFlag)) 
		app.NameAndExt().Append(KOpltOdb);
	app.NameAndExt().AppendFill(TChar(0),app.MaxNameAndExtLength()-app.NameAndExt().Length());
	iOpo.WriteL(app.NameAndExt().Ptr(),app.NameAndExt().Length());

	// Default patch
	_LIT(KOpltOpd,"\\OPD\\");
	if (!(app.Flags()&KOpl16AppPathFlag))
		app.Path().Copy(KOpltOpd);

	app.Path().AppendFill(TChar(0),app.Path().MaxLength()-app.Path().Length());
	iOpo.WriteL(app.Path().Ptr(),app.Path().Length());
	iOpo.WriteUint16L(app.Type());
	}
	
void COpl16Opl1993BackEnd::FirstProcL()
//
// Called just before we do the first procedure. Gives us a chance
// to sort out all the headers and that
//
	{

	iOpo.Patch16L(iDataPatchPos,iOpo.SizeL());
	iTotalSizePatchPos=iOpo.AddPatch32L();
	iOpo.WriteUint16L(KOpl16Opl1993TranVer);
	iOpo.WriteUint16L(KOpl16Opl1993RuntimeVer);
	iProcTablePatchPos=iOpo.AddPatch32L();
	}

TInt COpl16Opl1993BackEnd::DoStartProcL()
//
// About to start off for a new procedure. How will it be referred to 
// internally
//
	{
	
	return iOpo.SizeL();
	}

void COpl16Opl1993BackEnd::DoProcL()
//
// Does nothing - we leave the proc in the buffered stream
//
	{
	}

RWriteStream& COpl16Opl1993BackEnd::StartProcTableL()
//
// Sets things up for doing the start of the procedure table
//
	{

	iOpo.Patch32L(iProcTablePatchPos,iOpo.SizeL());
	return iOpo;
	}

void COpl16Opl1993BackEnd::DoEndOfFileL()
//
// Puts out the end of the file
//
	{

	iOpo.Patch32L(iTotalSizePatchPos,iOpo.SizeL());
	iOpo.CommitL();
	
	EnsurePathL();
	
	RFile outputFile;
	User::LeaveIfError(outputFile.Replace(iFs,iOutputName, EFileStream|EFileWrite|EFileShareExclusive));
	TBackEndFile cleanupObject(*this,outputFile);
	CleanupStack::PushL(TCleanupItem(FileCleanup,&cleanupObject));

	
	TInt pos=0;
	TInt qcodeSize=iQcodeBuffer->Size();
	while (pos<qcodeSize)
		{
		TPtrC8 chunk=iQcodeBuffer->Ptr(pos);
		User::LeaveIfError(outputFile.Write(chunk));
		pos+=chunk.Length();
		}
	outputFile.Close();

	CleanupStack::Pop();
	}

COpl16Opl1993BackEnd::COpl16Opl1993BackEnd(const TDesC& aTarget, MTranAppCB * anAppCB)
	: COpl16ModuleBackEnd(aTarget, anAppCB)
	{
	}

void COpl16Opl1993BackEnd::TidyFile(RFile& aFile)
//
// Closes and delete the output file if anything goes wrong
//
	{

	aFile.Close();
	iFs.Delete(iOutputName);
	}

//////////////////////////////////////////////////////////////////////////////////
// 
// COpl16Opler1BackEnd  - Back end for OPL1993 compatible modules
//
//////////////////////////////////////////////////////////////////////////////////
COpl16Opler1BackEnd::~COpl16Opler1BackEnd()
//
//
//
	{
	
	delete iSourceName;
	iStream.Close();
	
	if (iStore!=NULL) // Something went wrong
		{
		delete iStore;
		iFs.Delete(iOutputName);
		}

	if (iSymbolStore!=NULL) /// Still got the store open - something went wrong
		{
		delete iSymbolStore;
		iFs.Delete(*iSymbolFileName);
		}
	delete iSymbolFileName;

	DeleteOpxs();
	}

COpl16Opler1BackEnd* COpl16Opler1BackEnd::NewL(const TDesC& aTargetName, MTranAppCB* anAppCB)
//
// 
//
	{
	COpl16Opler1BackEnd* pBe=new(ELeave) COpl16Opler1BackEnd(aTargetName, anAppCB);
	CleanupStack::PushL(pBe);
	pBe->ConstructL();
	CleanupStack::Pop();
	return pBe;
	}

void COpl16Opler1BackEnd::OpxL(COplParseOpx* anOpx)
//
// Adds the Opx to the list
//
	{
	
	iOpxList.AppendL(anOpx);
	}

TInt COpl16Opler1BackEnd::OpxCount()
	{
	
	return iOpxList.Count();
	}

void COpl16Opler1BackEnd::DoStartL(const TDesC& aSourceName)
//
// Target specific body
//
	{
		
	iUid=TUidType(KDirectFileStoreLayoutUid,KUidOplObj,KUidOplInterpreter);
	iSourceName=aSourceName.AllocL();
	}
	

void COpl16Opler1BackEnd::DoAppL(COpl16App* anApp)
//
// Change the UID appropriately
//
	{
	iUid=TUidType(KDirectFileStoreLayoutUid,KUidOplApp,TUid::Uid(anApp->Id()));
	}

const TInt KSymbolExtensionLength=4;
void COpl16Opler1BackEnd::FirstProcL()
//
// gives us a chance to open up that file and really get going
//
	{
	
	// Ensures that the path exists	
	EnsurePathL();
	
	// Create the Store
	iStore=CDirectFileStore::ReplaceL(iFs,iOutputName, EFileStream|EFileWrite|EFileShareExclusive);
	iStore->SetTypeL(iUid);

	if (iDebugFlag==CTextTranslator::EDebug) // Doing debug translation - need symbol table
		{
		// Just stick .SYM on the end of the filename
		TParse symbolName;
		_LIT(KOpltSym,".SYM");
		symbolName.Set(KOpltSym,&iOutputName,NULL);
		iSymbolFileName=symbolName.FullName().AllocL();
		
		// And create the stream store
		iSymbolStore=CDirectFileStore::ReplaceL(iFs,*iSymbolFileName,EFileStream|EFileWrite|EFileShareExclusive);
		iSymbolStore->SetTypeL(TUidType(KDirectFileStoreLayoutUid,KUidDebuggerObj,KUidOplDebugger));
		}
	
	// And save the Source stream
	iSourceId=NewStreamL();
	iStream<<TLbc(*iSourceName);	// hp: was missing TLbc
	iStream.CommitL();
	iStream.Close();
	}


TInt COpl16Opler1BackEnd::DoStartProcL()
//
// Creates a stream in the store, into which teh details of the proc
// will be extruded.
// Returns the stream ID.
//
	{
	return NewStreamL().Value();
	}

void COpl16Opler1BackEnd::DoProcL()
//
// Takes the contents of the in-memory stream and streams them out as the current procedure
//
	{
	
	// Copy the stream out to the file
	RBufReadStream proc;
	proc.Open(*iQcodeBuffer);
	iStream.WriteL(proc);
	iStream.CommitL();
	iStream.Close();

	// And Reset the buffer
	iQcodeBuffer->Reset();
	iOpo.Open(*iQcodeBuffer);
	}

RWriteStream& COpl16Opler1BackEnd::StartProcTableL()
//
// Creates a new stream for the proc table
//
	{
	iProcTableId=NewStreamL();
	return iStream;
	}

void COpl16Opler1BackEnd::DoEndOfFileL()
//
// Puts out the Opx table and the root stream;
//
	{
	// End of the proc table stream
	iStream.CommitL();
	iStream.Close();

	// Stuff out the Opx table
	TInt count=OpxCount();
	TStreamId opxStreamId=KNullStreamId;
	if (count)
		{
		opxStreamId=NewStreamL();
		iStream<<TUint16(count);				// the count of OPXs
		for (TInt index=0;index<count;index++)
			{
			COplParseOpx& opx=*iOpxList[index];
			iStream<<TLbc(opx.Name())<<opx.Uid()<<TUint16(opx.Version());
			}
		DeleteOpxs(); // Done with the Opxs for now
		iStream.CommitL();
		iStream.Close();
		}

	// Now the root stream
	TStreamId root=NewStreamL();
	iStream<<KUidOplInterpreter<<TUint16(KOpl16CrystalTranVer)<<TUint16(KOpl16CrystalRuntimeVer);
	iStream<<iSourceId<<iProcTableId<<opxStreamId;
	if (iDebugFlag==CTextTranslator::EDebug)
		iStream<<TUint16(CTextTranslator::EDebug);
	else
		iStream<<TUint16(CTextTranslator::ERelease);
	iStream.CommitL();
	iStream.Close();

	iStore->SetRootL(root);
	iStore->CommitL();
	delete iStore;
	iStore=NULL;
	// And tidy up the debugger symbols if needed
	if (iDebugFlag==CTextTranslator::EDebug)
		{
		// Proc table stream
		RStoreWriteStream stream;
		TStreamId procStreamId=stream.CreateLC(*iSymbolStore);
		count=iProcSymbolStreams.Count();
		stream<<TUint16(count);
		for (TInt index=0;index<count;index++)
			stream<<iProcSymbolStreams[index];
		stream.CommitL();
		CleanupStack::PopAndDestroy();

		// Root stream
		root=stream.CreateLC(*iSymbolStore);
		stream<<KUidOplDebugger<<TUint16(KOpl16CrystalTranVer)<<TUint16(KOpl16CrystalRuntimeVer)<<procStreamId;
		stream.CommitL();
		CleanupStack::PopAndDestroy();

		iSymbolStore->SetRootL(root);
		iSymbolStore->CommitL();
		delete iSymbolStore;
		iSymbolStore=NULL;
		}
	}


void COpl16Opler1BackEnd::DebugSymbolsL(COplProcHeader *aHeader)
//
// Puts out the debug symbols for the current procedure
//
	{
	// Argument & Externals
	TSymbolWalk simpleWalk;
	TStreamId argsId=simpleWalk.WalkL(iSymbolStore,aHeader->Arguments());
	TStreamId externalsId=simpleWalk.WalkL(iSymbolStore,aHeader->Externals());

	// Automatics - globals & Locals
	TAutomaticSymbolWalk autoWalk;
	TStreamId globalsId=autoWalk.WalkL(iSymbolStore,aHeader->Globals());
	TStreamId localsId=autoWalk.WalkL(iSymbolStore,aHeader->Locals());

	// Stream for the procedure
	RStoreWriteStream procStream;
	TStreamId procStreamId=procStream.CreateLC(*iSymbolStore);
	procStream<<argsId<<externalsId<<globalsId<<localsId;
	procStream.CommitL();
	CleanupStack::PopAndDestroy();
	iProcSymbolStreams.AppendL(procStreamId);
	}


COpl16Opler1BackEnd::COpl16Opler1BackEnd(const TDesC& aTargetFile, MTranAppCB* anAppCB)
// 
//
//
	: COpl16ModuleBackEnd(aTargetFile, anAppCB), iOpxList(KOpl16OpxListGran), iProcSymbolStreams(KOpl16ProcSymbolsGran)
	{
	}

TStreamId COpl16Opler1BackEnd::NewStreamL()
//
// Creates a stream
//
	{
	return iStream.CreateL(*iStore);
	}


void COpl16Opler1BackEnd::DeleteOpxs()
	{
	for (TInt index=OpxCount()-1;index>=0;index--)
		delete iOpxList[index];
	iOpxList.Reset();
	}

//////////////////////////////////////////////////////////////////////
//
// COpl16EvalBackEnd
//
//////////////////////////////////////////////////////////////////////
COpl16EvalBackEnd *COpl16EvalBackEnd::NewL()
//
// Creates this
//
	{

	return new(ELeave) COpl16EvalBackEnd;
	}



void COpl16EvalBackEnd::SetBuffers(TTranslateError& anError,CBufBase& aQcodeBuffer,TInt& aMaxStackDepth)
//
// Sets up the buffers
//
	{
	
	iError=&anError;
	iQcodeBuffer=&aQcodeBuffer;
	iMaxStackDepth=&aMaxStackDepth;
	}

void COpl16EvalBackEnd::ProcessExpressionL(CBufBase& aPcodeBuffer)
//
// Sets the addresses of all the called procedures
// and generates the appropriate qcode 
// 
	{

	// Check and reset buffers
	__ASSERT_ALWAYS(iError!=NULL, Panic(EOpl16EvalBackEndBuffers)); // Check that we have set the buffers
	iQcodeBuffer->Reset();

	// Get stream onto qcode buffer
	ROpoWriteStream qcodeStream;
	qcodeStream.Insert(*iQcodeBuffer,0);

	// Get and run a Pcode converter
	COpl16PcodeConverter* pPc=COpl16PcodeConverter::NewLC();
	*iMaxStackDepth=pPc->RunL(aPcodeBuffer,qcodeStream,*iError,CTextTranslator::ERelease);
	CleanupStack::PopAndDestroy();
	}