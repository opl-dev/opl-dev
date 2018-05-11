// HISTORY.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#include <oplr.h>
#if defined(_HISTORY)
#include "hist.h"
const TText* OpCodeNames[] =
		{
		_S("SimpleDirectRightSideInt"),
		_S("SimpleDirectRightSideLong"),
		_S("SimpleDirectRightSideFloat"),
		_S("SimpleDirectRightSideString"),
		_S("SimpleDirectLeftSideInt"),
		_S("SimpleDirectLeftSideLong"),
		_S("SimpleDirectLeftSideFloat"),
		_S("SimpleDirectLeftSideString"),
		_S("SimpleInDirectRightSideInt"),
		_S("SimpleInDirectRightSideLong"),
		_S("SimpleInDirectRightSideFloat"),
		_S("SimpleInDirectRightSideString"),
		_S("SimpleInDirectLeftSideInt"),
		_S("SimpleInDirectLeftSideLong"),
		_S("SimpleInDirectLeftSideFloat"),
		_S("SimpleInDirectLeftSideString"),
		// 10h
		_S("ArrayDirectRightSideInt"),
		_S("ArrayDirectRightSideLong"),
		_S("ArrayDirectRightSideFloat"),
		_S("ArrayDirectRightSideString"),
		_S("ArrayDirectLeftSideInt"),
		_S("ArrayDirectLeftSideLong"),
		_S("ArrayDirectLeftSideFloat"),
		_S("ArrayDirectLeftSideString"),
		_S("ArrayInDirectRightSideInt"),
		_S("ArrayInDirectRightSideLong"),
		_S("ArrayInDirectRightSideFloat"),
		_S("ArrayInDirectRightSideString"),
		_S("ArrayInDirectLeftSideInt"),
		_S("ArrayInDirectLeftSideLong"),
		_S("ArrayInDirectLeftSideFloat"),
		_S("ArrayInDirectLeftSideString"),
		// 20h
		_S("FieldRightSideInt"),
		_S("FieldRightSideLong"),
		_S("FieldRightSideFloat"),
		_S("FieldRightSideString"),
		_S("FieldLeftSide"),
		_S("FieldLeftSide"),
		_S("FieldLeftSide"),
		_S("FieldLeftSide"),
		_S("ConstantInt"),
		_S("ConstantLong"),
		_S("ConstantFloat"),
		_S("ConstantString"),
		_S("IllegalOpCode"),	// word - QI_SPECIAL
		_S("IllegalOpCode"),	// long - QI_SPECIAL
		_S("IllegalOpCode"),	// float - QI_SPECIAL
		_S("IllegalOpCode"),	// string - QI_SPECIAL
		// 30h
		_S("CompareLessThanInt"),
		_S("CompareLessThanLong"),
		_S("CompareLessThanFloat"),
		_S("CompareLessThanString"),
		_S("CompareLessOrEqualInt"),
		_S("CompareLessOrEqualLong"),
		_S("CompareLessOrEqualFloat"),
		_S("CompareLessOrEqualString"),
		_S("CompareGreaterThanInt"),
		_S("CompareGreaterThanLong"),
		_S("CompareGreaterThanFloat"),
		_S("CompareGreaterThanString"),
		_S("CompareGreaterOrEqualInt"),
		_S("CompareGreaterOrEqualLong"),
		_S("CompareGreaterOrEqualFloat"),
		_S("CompareGreaterOrEqualString"),
		//40h
		_S("CompareEqualInt"),
		_S("CompareEqualLong"),
		_S("CompareEqualFloat"),
		_S("CompareEqualString"),
		_S("CompareNotEqualInt"),
		_S("CompareNotEqualLong"),
		_S("CompareNotEqualFloat"),
		_S("CompareNotEqualString"),
		_S("AddInt"),
		_S("AddLong"),
		_S("AddFloat"),
		_S("AddString"),
		_S("SubtractInt"),
		_S("SubtractLong"),
		_S("SubtractFloat"),
		_S("StackByteAsWord"),
		// 50h
		_S("MultiplyInt"),
		_S("MultiplyLong"),
		_S("MultiplyFloat"),
		_S("RunProcedure"),
		_S("DivideInt"),
		_S("DivideLong"),
		_S("DivideFloat"),
		_S("CallFunction"),
		_S("PowerOfInt"),
		_S("PowerOfLong"),
		_S("PowerOfFloat"),
		_S("BranchIfFalse"),
		_S("AndInt"),
		_S("AndLong"),
		_S("AndFloat"),
		_S("StackByteAsLong"),
		// 60h
		_S("OrInt"),
		_S("OrLong"),
		_S("OrFloat"),
		_S("StackWordAsLong"),
		_S("NotInt"),
		_S("NotLong"),
		_S("NotFloat"),
		_S("IllegalOpCode"),
		_S("UnarySubtractInt"),
		_S("UnarySubtractLong"),
		_S("UnarySubtractFloat"),
		_S("CallProcByStringExpr"),
		_S("PercentLessThan"),		// <%
		_S("PercentGreaterThan"),		// >%
		_S("PercentAdd"),				// +%
		_S("PercentSubtract"),		// -%
		// 70h
		_S("PercentMultiply"),		// *%
		_S("PercentDivide"),			// /%
		_S("IllegalOpCode"),
		_S("IllegalOpCode"),
		_S("ZeroReturnInt"),
		_S("ZeroReturnLong"),
		_S("ZeroReturnFloat"),
		_S("NullReturnString"),
		_S("LongToInt"),
		_S("FloatToInt"),
		_S("FloatToLong"),
		_S("IntToLong"),
		_S("IntToFloat"),
		_S("LongToFloat"),
		_S("LongToUInt"),
		_S("FloatToUInt"),
		// 80h
		_S("DropInt"),
		_S("DropLong"),
		_S("DropFloat"),
		_S("DropString"),
		_S("AssignInt"),
		_S("AssignLong"),
		_S("AssignFloat"),
		_S("AssignString"),
		_S("PrintInt"),
		_S("PrintLong"),
		_S("PrintFloat"),
		_S("PrintString"),
		_S("LPrintInt"),
		_S("LPrintLong"),
		_S("LPrintFloat"),
		_S("LPrintString"),
		// 90h
		_S("PrintSpace"),
		_S("LPrintSpace"),
		_S("PrintCarriageReturn"),
		_S("LPrintCarriageReturn"),
		_S("InputInt"),
		_S("InputLong"),
		_S("InputFloat"),
		_S("InputString"),
		_S("PokeW"),		// POKEW
		_S("PokeL"),		// POKEL
		_S("PokeD"),		// POKED
		_S("PokeStr"),	// POKE$
		_S("PokeB"),		// POKEB
		_S("Append"),		// APPEND
		_S("At"),	// AT
		_S("Back"),		// BACK
		// A0h
		_S("Beep"),
		_S("Close"),
		_S("Cls"),
		_S("Compress"),
		_S("Copy"),
		_S("Create"),
		_S("Cursor"),
		_S("Delete"),
		_S("Erase"),
		_S("Escape"),
		_S("First"),
		_S("Vector"),
		_S("Last"),
		_S("LClose"),
		_S("LoadM"),
		_S("LOpen"),
		// B0h
		_S("Next"),
		_S("OnErr"),
		_S("Off"),
		_S("OffFor"),
		_S("Open"),
		_S("Pause"),
		_S("Position"),
		_S("IoSignal"), 	
		_S("Raise"),
		_S("Randomize"),
		_S("Rename"),
		_S("Stop"),
		_S("Trap"),
		_S("Update"),
		_S("Use"),
		_S("GoTo"),
		// C0h
		_S("Return"),
		_S("UnLoadM"),
		_S("Edit"),
		_S("Screen2"),
		_S("OpenR"),
		_S("gSaveBit"),
		_S("gClose"),
		_S("gUse"),
		_S("gSetWin"),
		_S("gVisible"),
		_S("gFont"),
		_S("gUnloadFont"),
		_S("gGMode"),
		_S("gTMode"),
		_S("gStyle"),
		_S("gOrder"),
		// D0h
		_S("gInfo32"),		// known as gInfo in Opl1993
		_S("gCls"),			
		_S("gAt"),
		_S("gMove"),
		_S("gPrintWord"),
		_S("gPrintLong"),
		_S("gPrintDbl"),
		_S("gPrintStr"),
		_S("gPrintSpace"),
		_S("gPrintBoxText"),
		_S("gLineBy"),
		_S("gBox"),
		_S("gCircle"),
		_S("gEllipse"),
		_S("gPoly"),
		_S("gFill"),
		// E0h
		_S("gPatt"),
		_S("gCopy"),
		_S("gScroll"),
		_S("gUpdate"),
		_S("getEvent"),
		_S("gLineTo"),
		_S("gPeekLine"),
		_S("Screen4"),
		_S("IoWaitStat"),
		_S("IoYield"),
		_S("mInit"),
		_S("mCard"),
		_S("dInit"),
		_S("dItem"),
		_S("SetName"),
		_S("IllegalOpCode"),		// used to be StatusWin
		// F0h
		_S("Busy"),
		_S("Lock"),
		_S("gInvert"),
		_S("gXPrint"),
		_S("gBorder"),
		_S("gClock"),
		_S("IllegalOpCode"),    // used to be memoryRight
		_S("IllegalOpCode"), 	  // used to be memoryLeft
		_S("MkDir"),
		_S("RmDir"),
		_S("SetPath"),
		_S("SecsToDate"),
		_S("gIPrint"),
		// FDh
		_S("IllegalOpCode"),		
		_S("IllegalOpCode"),		
		_S("NextOpcodeTable"),
		// 100h
		_S("gGrey"),
		_S("DefaultWin"),
        _S("IllegalOpCode"),	// used to be diamInit
#if defined(INTERNAL_RELEASE)
        _S("__Dummy"),       // called by diamPos i% !!
#else
		_S("IllegalOpCode"),	// used to be diamPos
#endif
		_S("Font"),
		_S("Style"),
		_S("IllegalOpCode"),	// used to be useSprite
		_S("IllegalOpCode"),	// used to be appendSprite 
		_S("IllegalOpCode"),	// used to be drawSprite
		_S("IllegalOpCode"),	// used to be changeSprite
		_S("IllegalOpCode"),	// used to be posSprite
		_S("IllegalOpCode"),	// used to be closeSprite
		_S("FreeAlloc"),
		_S("IllegalOpCode"),	// used to be LinkLib
		_S("IllegalOpCode"),	// used to be rCache
		_S("gButton"),
		_S("gXBorder"),
		// 110h
		_S("gDrawObject"),
		_S("IllegalOpCode"),	// used to be OdbInfo
		_S("IllegalOpCode"),	// used to be CacheTidy
		_S("ScreenInfo"),		
		_S("IllegalOpCode"),	// used to be rCacheHdr
		_S("IllegalOpCode"),	// used to be rCacheRec
		_S("dInitS"),
		_S("IllegalOpCode"),
		_S("DaysToDate")		// new function for converting result of dDATE
		};
// this file is stale now?


const TText* FuncOpCodeNames[] =
		{
		_S("Addr"),
		_S("Asc"),
		_S("IllegalFuncOpCode"),
		_S("Count"),
		_S("Day"),
		_S("Dow"),
		_S("Eof"),
		_S("Err"),
		_S("Exist"),
		_S("Find"),
		_S("Get"),
		_S("Ioa"),
		_S("Iow"),
		_S("IoOpen"),
		_S("IoWrite"),
		_S("IoRead"),
		// 10h
		_S("IoClose"),
		_S("IoWait"),
		_S("Hour"),
		_S("Key"),
		_S("Len"),
		_S("Loc"),
		_S("Minute"),
		_S("Month"),
		_S("PeekB"),
		_S("PeekW"),
		_S("Pos"),
		_S("RecSize"),
		_S("Second"),
		_S("IllegalFuncOpCode"),
		_S("Year"),
		_S("SAddr"),
		// 20h
		_S("Week"),
		_S("IoSeek"),
		_S("Kmod"),
		_S("KeyA"),
		_S("KeyC"),
		_S("IoOpenUnique"),
		_S("gCreate"),
		_S("gCreateBit"),
		_S("gLoadBit"),
		_S("gLoadFont"),
		_S("gRank"),
		_S("gIdentity"),
		_S("gX"),
		_S("gY"),
		_S("gWidth"),
		_S("gHeight"),
		// 30h
		_S("gOriginX"),
		_S("gOriginY"),
		_S("gTWidth"),
		_S("gPrintClip"),
		_S("TestEvent"),
		_S("IllegalFuncOpCode"),
		_S("Menu"),
		_S("Dialog"),
		_S("Alert"),
		_S("gCreateEnhanced"),
		_S("MenuWithMemory"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
        // 40h
        _S("Days"),
		_S("IAbs"),
		_S("IntLong"),
		_S("PeekL"),
		_S("Space"),
		_S("DateToSecs"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("Alloc"),
		_S("ReAlloc"),
		_S("AdjustAlloc"),
		_S("LenAlloc"),
		_S("Ioc"),
		// 50h
        _S("Uadd"),
		_S("Usub"),
		_S("IoCancel"),
		_S("IllegalFuncOpCode"),
		_S("FindField"),
		_S("BookMark"),
		_S("GetEventC"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
        // 60h
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
        // 70h
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
        // 80h
        _S("Abs"),
		_S("ACos"),
		_S("ASin"),
		_S("ATan"),
		_S("Cos"),
		_S("Deg"),
		_S("Exp"),
		_S("Flt"),
		_S("Intf"),
		_S("Ln"),
		_S("Log"),
		_S("PeekF"),
		_S("Pi"),
		_S("Rad"),
		_S("Rnd"),
		_S("Sin"),
        // 90h
        _S("Sqr"),
		_S("Tan"),
		_S("Val"),
		_S("Max"),
		_S("Mean"),
		_S("Min"),
		_S("Std"),
		_S("Sum"),
		_S("Var"),
		_S("Eval"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
        // a0h
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
        // b0h
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
        // c0h
		_S("ChrStr"),
		_S("DatimStr"),
		_S("DayNameStr"),
		_S("DirStr"),
		_S("ErrStr"),
		_S("FixStr"),
		_S("GenStr"),
		_S("GetStr"),
		_S("HexStr"),
		_S("KeyStr"),
		_S("LeftStr"),
		_S("LowerStr"),
		_S("MidStr"),
		_S("MonthStr"),
		_S("NumStr"),
		_S("PeekStr"),
        // d0h
		_S("ReptStr"),
		_S("RightStr"),
		_S("SciStr"),
		_S("UpperStr"),
		_S("IllegalFuncOpCode"),
		_S("WCmd"),
		_S("CmdStr"),
		_S("ParseStr"),
		_S("ErrX$"),
		_S("IllegalFuncOpCode"),
        _S("Size"),
        _S("LocWithCase"),
		_S("gPixel"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode"),
		_S("IllegalFuncOpCode")
		};

TInt RHist::Open(RFs& afs)
     {
     if (iCount!=1)
         {
         iCount=1;
         ifilenum=1;
         }
     iFile.Close();
     TInt err;
	 _LIT(KHistA,"c:\\HistLogA.log");
	 _LIT(KHistB,"c:\\HistLogB.log");
     if (ifilenum%2)
          err=iFile.Replace(afs,KHistA,EFileRead|EFileWrite);
     else
          err=iFile.Replace(afs,KHistB,EFileRead|EFileWrite);

     if (err<0)
         {
		 _LIT(KFailed,"History: failed to create a log");
         const TPtrC buf(KFailed);
         ((CEikonEnv*)TheRuntime()->ConEnv())->AlertWin(buf);
		 return err;
         };

     TBuf<64> buf;
     buf.Num(ifilenum,EDecimal);
     iFile.Write(buf);
     _LIT(KcarRet,"\r\n");
     iFile.Write(KcarRet);
	 return KErrNone;
     }            
       
void RHist::Close()
     {
     iFile.Close();
     }                   

void RHist::WriteLog(TUint aOpcode)
    {
    if (iCount==1000)
        {
        ifilenum++;
        iCount=1;
        Open(TheRuntime()->ConEnv()->FsSession());
        }
    TBuf<256> buf;
    buf=OpCodeNames[aOpcode];
    iFile.Write(buf);
    _LIT(KcarRet,"\r\n");
    iFile.Write(KcarRet);

    iCount++;
    }

void RHist::FuncWriteLog(TUint aOpcode)
    {
    
    if (iCount==1000)
        {
        ifilenum++;
        iCount=1;
        Open(TheRuntime()->ConEnv()->FsSession());
        }
    TBuf<256> buf(FuncOpCodeNames[aOpcode]);
    iFile.Write(buf);
    _LIT(KcarRet,"\r\n");
    iFile.Write(KcarRet);

    iCount++;
    }

#endif










