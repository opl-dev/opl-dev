// OPCODES.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include <opcodes.h>

const OPCODEFUNC OpCode::OpCodeTable[] =
		{
		&SimpleDirectRightSideInt,
		&SimpleDirectRightSideLong,
		&SimpleDirectRightSideFloat,
		&SimpleDirectRightSideString,
		&SimpleDirectLeftSideInt,
		&SimpleDirectLeftSideLong,
		&SimpleDirectLeftSideFloat,
		&SimpleDirectLeftSideString,
		&SimpleInDirectRightSideInt,
		&SimpleInDirectRightSideLong,
		&SimpleInDirectRightSideFloat,
		&SimpleInDirectRightSideString,
		&SimpleInDirectLeftSideInt,
		&SimpleInDirectLeftSideLong,
		&SimpleInDirectLeftSideFloat,
		&SimpleInDirectLeftSideString,
		// 10h
		&ArrayDirectRightSideInt,
		&ArrayDirectRightSideLong,
		&ArrayDirectRightSideFloat,
		&ArrayDirectRightSideString,
		&ArrayDirectLeftSideInt,
		&ArrayDirectLeftSideLong,
		&ArrayDirectLeftSideFloat,
		&ArrayDirectLeftSideString,
		&ArrayInDirectRightSideInt,
		&ArrayInDirectRightSideLong,
		&ArrayInDirectRightSideFloat,
		&ArrayInDirectRightSideString,
		&ArrayInDirectLeftSideInt,
		&ArrayInDirectLeftSideLong,
		&ArrayInDirectLeftSideFloat,
		&ArrayInDirectLeftSideString,
		// 20h
		&FieldRightSideInt,
		&FieldRightSideLong,
		&FieldRightSideFloat,
		&FieldRightSideString,
		&FieldLeftSide,
		&FieldLeftSide,
		&FieldLeftSide,
		&FieldLeftSide,
		&ConstantInt,
		&ConstantLong,
		&ConstantFloat,
		&ConstantString,
		&IllegalOpCode,	// word - QI_SPECIAL
		&IllegalOpCode,	// long - QI_SPECIAL
		&IllegalOpCode,	// float - QI_SPECIAL
		&IllegalOpCode,	// string - QI_SPECIAL
		// 30h
		&CompareLessThanInt,
		&CompareLessThanLong,
		&CompareLessThanFloat,
		&CompareLessThanString,
		&CompareLessOrEqualInt,
		&CompareLessOrEqualLong,
		&CompareLessOrEqualFloat,
		&CompareLessOrEqualString,
		&CompareGreaterThanInt,
		&CompareGreaterThanLong,
		&CompareGreaterThanFloat,
		&CompareGreaterThanString,
		&CompareGreaterOrEqualInt,
		&CompareGreaterOrEqualLong,
		&CompareGreaterOrEqualFloat,
		&CompareGreaterOrEqualString,
		//40h
		&CompareEqualInt,
		&CompareEqualLong,
		&CompareEqualFloat,
		&CompareEqualString,
		&CompareNotEqualInt,
		&CompareNotEqualLong,
		&CompareNotEqualFloat,
		&CompareNotEqualString,
		&AddInt,
		&AddLong,
		&AddFloat,
		&AddString,
		&SubtractInt,
		&SubtractLong,
		&SubtractFloat,
		&StackByteAsWord,
		// 50h
		&MultiplyInt,
		&MultiplyLong,
		&MultiplyFloat,
		&RunProcedure,
		&DivideInt,
		&DivideLong,
		&DivideFloat,
		&CallFunction,
		&PowerOfInt,
		&PowerOfLong,
		&PowerOfFloat,
		&BranchIfFalse,
		&AndInt,
		&AndLong,
		&AndFloat,
		&StackByteAsLong,
		// 60h
		&OrInt,
		&OrLong,
		&OrFloat,
		&StackWordAsLong,
		&NotInt,
		&NotLong,
		&NotFloat,
		&Statement16,
		&UnaryMinusInt,
		&UnaryMinusLong,
		&UnaryMinusFloat,
		&CallProcByStringExpr,
		&PercentLessThan,		// <%
		&PercentGreaterThan,		// >%
		&PercentAdd,				// +%
		&PercentSubtract,		// -%
		// 70h
		&PercentMultiply,		// *%
		&PercentDivide,			// /%
		&IllegalOpCode,
		&IllegalOpCode,
		&ZeroReturnInt,
		&ZeroReturnLong,
		&ZeroReturnFloat,
		&NullReturnString,
		&LongToInt,
		&FloatToInt,
		&FloatToLong,
		&IntToLong,
		&IntToFloat,
		&LongToFloat,
		&LongToUInt,
		&FloatToUInt,
		// 80h
		&DropInt,
		&DropLong,
		&DropFloat,
		&DropString,
		&AssignInt,
		&AssignLong,
		&AssignFloat,
		&AssignString,
		&PrintInt,
		&PrintLong,
		&PrintFloat,
		&PrintString,
		&LPrintInt,
		&LPrintLong,
		&LPrintFloat,
		&LPrintString,
		// 90h
		&PrintSpace,
		&LPrintSpace,
		&PrintCarriageReturn,
		&LPrintCarriageReturn,
		&InputInt,
		&InputLong,
		&InputFloat,
		&InputString,
		&PokeW,		// POKEW
		&PokeL,		// POKEL
		&PokeD,		// POKED
		&PokeStr,	// POKE$
		&PokeB,		// POKEB
		&Append,		// APPEND
		&At,	// AT
		&Back,		// BACK
		// A0h
		&Beep,
		&Close,
		&Cls,
		&IllegalOpCode, // was COMPRESS
		&Copy,
		&Create,
		&Cursor,
		&Delete,
		&Erase,
		&Escape,
		&First,
		&Vector,
		&Last,
		&LClose,
		&LoadM,
		&LOpen,
		// B0h
		&Next,
		&OnErr,
		&Off,
		&OffFor,
		&Open,
		&Pause,
		&Position,
		&IoSignal, 	
		&Raise,
		&Randomize,
		&Rename,
		&Stop,
		&Trap,
		&Update,
		&Use,
		&GoTo,
		// C0h
		&Return,
		&UnLoadM,
		&Edit,
		&Screen2,
		&OpenR,
		&gSaveBit,
		&gClose,
		&gUse,
		&gSetWin,
		&gVisible,
		&gFont,
		&gUnloadFont,
		&gGMode,
		&gTMode,
		&gStyle,
		&gOrder,
		// D0h
		&IllegalOpCode,		// gInfo in Opl1993 replaced by gInfo32 in Era
		&gCls,			
		&gAt,
		&gMove,
		&gPrintWord,
		&gPrintLong,
		&gPrintDbl,
		&gPrintStr,
		&gPrintSpace,
		&gPrintBoxText,
		&gLineBy,
		&gBox,
		&gCircle,
		&gEllipse,
		&gPoly,
		&gFill,
		// E0h
		&gPatt,
		&gCopy,
		&gScroll,
		&gUpdate,
		&GetEvent,
		&gLineTo,
		&gPeekLine,
		&Screen4,
		&IoWaitStat,
		&IoYield,
		&mInit,
		&mCard,
		&dInit,
		&dItem,
		&IllegalOpCode,		// used to be SETNAME
		&IllegalOpCode,		// used to be StatusWin
		// F0h
		&Busy,
		&Lock,
		&gInvert,
		&gXPrint,
		&gBorder,
		&gClock,
		&IllegalOpCode,    // used to be memoryRight
		&IllegalOpCode, 	  // used to be memoryLeft
		&MkDir,
		&RmDir,
		&SetPath,
		&SecsToDate,
		&gIPrint,
		// FDh
		&IllegalOpCode,		
		&IllegalOpCode,		
		&NextOpcodeTable,
		// 100h
		&gGrey,
		&DefaultWin,
		&IllegalOpCode,	// used to be diamInit
#if defined(INTERNAL_RELEASE)
		&__Dummy,       // called by diamPos i% !!
#else
		&IllegalOpCode,	// used to be diamPos
#endif
		&Font,
		&Style,
		&IllegalOpCode,	// used to be useSprite
		&IllegalOpCode,	// used to be appendSprite 
		&IllegalOpCode,	// used to be drawSprite
		&IllegalOpCode,	// used to be changeSprite
		&IllegalOpCode,	// used to be posSprite
		&IllegalOpCode,	// used to be closeSprite
		&FreeAlloc,
		&IllegalOpCode,	// used to be LinkLib
		&IllegalOpCode,	// used to be rCache
		&gButton,
		// 110h
		&gXBorder,
		&IllegalOpCode,	// used to be gDrawObject
		&IllegalOpCode,	// used to be OdbInfo
		&IllegalOpCode,	// used to be CacheTidy
		&ScreenInfo,		
		&IllegalOpCode,	// used to be rCacheHdr
		&IllegalOpCode,	// used to be rCacheRec
		&IllegalOpCode, // used to be dInitS
		&CallOpxFunc,	// NEW functions for Opler1
		&Statement32,
		&Modify,
		&Insert,
		&Cancel,
		&Put,
		&DeleteTable,
		&GotoMark,
		// 120h
		&KillMark,
		&ReturnFromEval,
		&GetEvent32,
		&GetEventA32,
		&gColor,
		&SetFlags,
		&SetDoc,
		&DaysToDate,	// new function for converting result of dDATE
		&gInfo32,
		&IoWaitStat32,
		&Compact,
		&BeginTrans,
		&CommitTrans,
		&Rollback,
		&ClearFlags,
		&PointerFilter,
		// 130h
		&mCasc,
		&EvalExternalRightSideRef,	// eval right side (eg. EVAL("myvar%*2"))
		&EvalExternalLeftSideRef,	// eval left side (eg. EVAL("ADDR(myVar%)"))
//		&dCheckBox,
		&gSetPenWidth,
		&dEditMulti,
		&gColorInfo,
		&gColorBackground,
		&mCardX,
		&SetHelp,
		&ShowHelp,
		&SetHelpUid,
		&gXBorder32,
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 140h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 150h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 160h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 170h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 180h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 190h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 1A0h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 1B0h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 1C0h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 1D0h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 1E0h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		// 1F0h
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode,	// filler
		&IllegalOpCode	// filler
		// No more allowed
		};

const OPCODEFUNC FuncOpCode::FuncOpCodeTable[] =
		{
		&Addr,
		&Asc,
		&IllegalFuncOpCode,
		&Count,
		&Day,
		&Dow,
		&Eof,
		&Err,
		&Exist,
		&Find,
		&Get,
		&Ioa,
		&Iow,
		&IoOpen,
		&IoWrite,
		&IoRead,
		// 10h
		&IoClose,
		&IoWait,
		&Hour,
		&Key,
		&Len,
		&Loc,
		&Minute,
		&Month,
		&PeekB,
		&PeekW,
		&Pos,
		&IllegalFuncOpCode, // was RecSize
		&Second,
		&IllegalFuncOpCode,
		&Year,
		&SAddr,
		// 20h
		&Week,
		&IoSeek,
		&Kmod,
		&KeyA,
		&KeyC,
		&IoOpenUnique,
		&gCreate,
		&gCreateBit,
		&gLoadBit,
		&gLoadFont,
		&gRank,
		&gIdentity,
		&gX,
		&gY,
		&gWidth,
		&gHeight,
		// 30h
		&gOriginX,
		&gOriginY,
		&gTWidth,
		&gPrintClip,
		&TestEvent,
		&IllegalFuncOpCode,
		&Menu,
		&Dialog,
		&Alert,
		&gCreateEnhanced,
		&MenuWithMemory,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		// 40h
		&Days,
		&IAbs,
		&IntLong,
		&PeekL,
		&Space,
		&DateToSecs,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&Alloc,
		&ReAlloc,
		&AdjustAlloc,
		&LenAlloc,
		&Ioc,
		// 50h
		&Uadd,
		&Usub,
		&IoCancel,
		&IllegalFuncOpCode,
		&FindField,
		&Bookmark,
		&GetEventC,
		&InTrans,
		&mPopup,
		&IllegalFuncOpCode, // was going to be ModInt32
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		// 60h
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		// 70h
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		// 80h
		&Abs,
		&ACos,
		&ASin,
		&ATan,
		&Cos,
		&Deg,
		&Exp,
		&Flt,
		&Intf,
		&Ln,
		&Log,
		&PeekF,
		&Pi,
		&Rad,
		&Rnd,
		&Sin,
		// 90h
		&Sqr,
		&Tan,
		&Val,
		&Max,
		&Mean,
		&Min,
		&Std,
		&Sum,
		&Var,
		&Eval,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		// a0h
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		// b0h
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		// c0h
		&ChrStr,
		&DatimStr,
		&DayNameStr,
		&DirStr,
		&ErrStr,
		&FixStr,
		&GenStr,
		&GetStr,
		&HexStr,
		&KeyStr,
		&LeftStr,
		&LowerStr,
		&MidStr,
		&MonthStr,
		&NumStr,
		&PeekStr,
		// d0h
		&ReptStr,
		&RightStr,
		&SciStr,
		&UpperStr,
		&IllegalFuncOpCode,
		&WCmd,
		&CmdStr,
		&ParseStr,
		&ErrxStr,
		&GetDocStr,
		&Size,
		&LocWithCase,
		&gPixel,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		&IllegalFuncOpCode,
		};

	
void OpCode::CallFunction(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TUint funcCode=aRuntime.IP8();
	FuncOpCode::FuncOpCodeTable[funcCode](aStack,aRuntime,aFramePtr);
	}


void OpCode::IllegalOpCode(CStack&, COplRuntime&, CFrame*)
	{
	User::Leave(KOplErrIllegal);
	}

void FuncOpCode::IllegalFuncOpCode(CStack&, COplRuntime&, CFrame* )
	{
	// Worth keeping this separate from IllegalOpcode() for debugging?
	User::Leave(KOplErrIllegal);
	}

#if defined(INTERNAL_RELEASE)
void OpCode::__Dummy(CStack& aStack, COplRuntime& aRuntime, CFrame*)
// Called by DiamPos i% in internal releases
	{
	TInt16 val=aStack.PopInt16();
#ifdef _HISTORY
	if (val==KSwitchHistoryOn)
		{
		aRuntime.SetStateFlags(KHistoryEnabled);
		return;
		}
	else if (val==KSwitchHistoryOff)
		{
		aRuntime.ClearStateFlags(KHistoryEnabled);
		return;
		}
#endif
	TBuf<16> buf;
	_LIT(KDummy,"__Dummy: %d");
	buf.Format(KDummy,val);
	((CEikonEnv*)aRuntime.ConEnv())->AlertWin(buf);
	}
#endif