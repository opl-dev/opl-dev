// OPCODES.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include <oplr.h>

typedef void (*OPCODEFUNC)(CStack&, COplRuntime&, CFrame*);

class TOplRealFormat : public TRealFormat
	{
public:
	TOplRealFormat(TInt aUserFlags); /*{iRealFormat.iType|=KAllowThreeDigitExp;}*/ // implemented in NOT_YET.CPP
	TOplRealFormat(TInt aUserFlags,TInt aWidth);
	TOplRealFormat(TInt aUserFlags,TInt aWidth,TInt aPlaces);
	inline void SetType(TInt aType) {iType=(iType&KRealFormatTypeFlagsMask)|aType;};	// added by AnnW
//	inline TRealFormat RealFormat() const {return iRealFormat;}
//private:
//	TRealFormat iRealFormat;
	};

class OpCode
	{
private:
	static const OPCODEFUNC OpCodeTable[];

public:
	friend void COplRuntime::SequencerL();
	friend TInt COplRuntime::ErrorHandler();
#ifndef NO_SEQUENCER
	static void SimpleDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SimpleInDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	//10h
	static void ArrayDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectLeftSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectLeftSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectLeftSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ArrayInDirectLeftSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
#endif
	// 20h
	static void FieldRightSideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FieldRightSideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FieldRightSideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FieldRightSideString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FieldLeftSide(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ConstantInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ConstantLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ConstantFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ConstantString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IllegalOpCode(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);			// don't delete!
	static void CompareLessThanInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareLessThanLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareLessThanFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareLessThanString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareLessOrEqualInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareLessOrEqualLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareLessOrEqualFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareLessOrEqualString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterThanInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterThanLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterThanFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterThanString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterOrEqualInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterOrEqualLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterOrEqualFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareGreaterOrEqualString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareEqualInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareEqualLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareEqualFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareEqualString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareNotEqualInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareNotEqualLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareNotEqualFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CompareNotEqualString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AddInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AddLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AddFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AddString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SubtractInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SubtractLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SubtractFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void StackByteAsWord(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void MultiplyInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void MultiplyLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void MultiplyFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void RunProcedure(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DivideInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DivideLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DivideFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CallFunction(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PowerOfInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PowerOfLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PowerOfFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void BranchIfFalse(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AndInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AndLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AndFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void StackByteAsLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void OrInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void OrLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void OrFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void StackWordAsLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void NotInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void NotLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void NotFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Statement16(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void UnaryMinusInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void UnaryMinusLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void UnaryMinusFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CallProcByStringExpr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PercentLessThan(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// <%
	static void PercentGreaterThan(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// >%
	static void PercentAdd(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);				// +%
	static void PercentSubtract(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// -%
	// 70h
	static void PercentMultiply(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// *%
	static void PercentDivide(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);			// /%
	static void ZeroReturnInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ZeroReturnLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ZeroReturnFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void NullReturnString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LongToInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FloatToInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FloatToLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IntToLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IntToFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LongToFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LongToUInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FloatToUInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 80h
	static void DropInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DropLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DropFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DropString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AssignInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AssignLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AssignFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AssignString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PrintInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PrintLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PrintFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PrintString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LPrintInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LPrintLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LPrintFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LPrintString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 90h
	static void PrintSpace(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LPrintSpace(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PrintCarriageReturn(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LPrintCarriageReturn(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void InputInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void InputLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void InputFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void InputString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PokeW(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// POKEW
	static void PokeL(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// POKEL
	static void PokeD(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// POKED
	static void PokeStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);	// POKE$
	static void PokeB(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// POKEB
	static void Append(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// APPEND
	static void At(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);	// AT
	static void Back(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		// BACK
	// a0h
	static void Beep(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Close(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Cls(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
//	static void Compress(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Copy(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Create(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Cursor(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Delete(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Erase(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Escape(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void First(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Vector(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Last(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LClose(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LoadM(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LOpen(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// B0h
	static void Next(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void OnErr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Off(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void OffFor(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Open(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Pause(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Position(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoSignal(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr); 	
	static void Raise(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Randomize(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Rename(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Stop(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Trap(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Update(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Use(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GoTo(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// C0h
	static void Return(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void UnLoadM(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Edit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Screen2(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void OpenR(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gSaveBit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gClose(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gUse(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gSetWin(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gVisible(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gFont(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gUnloadFont(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gGMode(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gTMode(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gStyle(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gOrder(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// D0h
	//static void gInfo(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gCls(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gAt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gMove(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPrintWord(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPrintLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPrintDbl(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPrintStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPrintSpace(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPrintBoxText(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gLineBy(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gBox(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gCircle(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gEllipse(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPoly(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gFill(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// E0h
	static void gPatt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gCopy(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gScroll(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gUpdate(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GetEvent(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gLineTo(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPeekLine(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Screen4(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoWaitStat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoYield(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void mInit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void mCard(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void dInit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void dItem(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SetName(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// F0h
	static void Busy(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Lock(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gInvert(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gXPrint(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gBorder(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gClock(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void MkDir(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void RmDir(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SetPath(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SecsToDate(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gIPrint(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// FDh
	static void gGrey(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void NextOpcodeTable(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 100h
	static void DefaultWin(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Font(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Style(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FreeAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gButton(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gXBorder(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 110h
	//static void gDrawObject(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ScreenInfo(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);		
	//static void dInitS(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CallOpxFunc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Statement32(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Modify(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Insert(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Cancel(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Put(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DeleteTable(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GotoMark(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 120h
	static void KillMark(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ReturnFromEval(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GetEvent32(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GetEventA32(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gColor(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SetFlags(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SetDoc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DaysToDate(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gInfo32(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoWaitStat32(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Compact(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void BeginTrans(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CommitTrans(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Rollback(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ClearFlags(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PointerFilter(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 130h
	static void mCasc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void EvalExternalRightSideRef(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void EvalExternalLeftSideRef(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
//	static void dCheckBox(CStack& aStack,COplRuntime& aRuntime,CFrame* aFramePtr);
	static void gSetPenWidth(CStack& aStack,COplRuntime& aRuntime,CFrame* aFramePtr);
	static void dEditMulti(CStack& aStack,COplRuntime& aRuntime,CFrame* aFramePtr);
	static void gColorInfo(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gColorBackground(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void mCardX(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SetHelp(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ShowHelp(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SetHelpUid(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gXBorder32(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);

#if defined(INTERNAL_RELEASE)
	static void __Dummy(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
#endif
	static void DoCallProcByStringExpr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr,TUint8 aType);
	static TAny* GetElementL(TAny* aArray,TInt aIndex,TInt aElementSize);
	static TAny* JumpToIndex(TAny* aArray,TInt aIndex,TInt aElementSize);
private:
	static void RemoveFrame(CStack& aStack, CFrame* aFramePtr);
	static void CheckExitL(COplRuntime& aRuntime);
	};


class FuncOpCode
	{
public :
	friend /*static*/ void OpCode::CallFunction(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Addr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Asc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Call(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Count(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Day(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Dow(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Eof(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Err(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Exist(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Find(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Get(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Ioa(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Iow(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoOpen(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoWrite(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoRead(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 10h
	static void IoClose(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoWait(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Hour(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Key(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Len(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Loc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LocWithCase(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Minute(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Month(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PeekB(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PeekW(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Pos(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
//	static void RecSize(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Second(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Usr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Year(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SAddr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	// 20h
	static void Week(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoSeek(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Kmod(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void KeyA(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void KeyC(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoOpenUnique(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gCreate(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gCreateBit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gLoadBit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gLoadFont(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gRank(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gIdentity(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gX(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gY(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gWidth(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gHeight(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gOriginX(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gOriginY(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gTWidth(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPrintClip(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void TestEvent(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IllegalFuncOpCode(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Menu(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Dialog(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Alert(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gCreateEnhanced(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void MenuWithMemory(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Days(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IAbs(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IntLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PeekL(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Space(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DateToSecs(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Alloc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ReAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void AdjustAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LenAlloc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Ioc(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Uadd(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Usub(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void IoCancel(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FindField(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Bookmark(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GetEventC(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void InTrans(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void mPopup(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
//	static void ModInt32(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Abs(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ACos(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ASin(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ATan(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Cos(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Deg(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Exp(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Flt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Intf(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Ln(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Log(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PeekF(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Pi(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Rad(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Rnd(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Sin(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Sqr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Tan(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Val(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Max(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Mean(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Min(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Std(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Sum(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Var(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Eval(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ChrStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DatimStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DayNameStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void DirStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ErrStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void FixStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GenStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GetStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void HexStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void KeyStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LeftStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void LowerStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void MidStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void MonthStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void NumStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void PeekStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ReptStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void RightStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void SciStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void UpperStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void WCmd(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void CmdStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ParseStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void ErrxStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void GetDocStr(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void Size(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
	static void gPixel(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr);
private:
	static const OPCODEFUNC FuncOpCodeTable[];
	};