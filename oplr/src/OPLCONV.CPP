// OPLCONV.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include <e32math.h>
#include <oplstack.h>
#include <opcodes.h>

void OpCode::LongToInt(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	TInt32 arg=aStack.PopInt32();
	if (arg<KMinTInt16||arg>KMaxTInt16)
		User::Leave(KErrOverflow);
	aStack.Push((TInt16)arg);
	}

	
void OpCode::FloatToInt(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	TReal arg=aStack.PopReal64();
	TInt16 ret;
	TInt err=Math::Int(ret,arg);
	User::LeaveIfError(err==KErrUnderflow ? KErrOverflow : err);
	aStack.Push(ret);
	}
	
void OpCode::FloatToLong(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	TReal arg=aStack.PopReal64();
	TInt32 ret;
	TInt err=Math::Int(ret,arg);
	User::LeaveIfError(err==KErrUnderflow ? KErrOverflow : err);
	aStack.Push(ret);
	}
	
void OpCode::IntToLong(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TInt32)aStack.PopInt16());
	}
	
void OpCode::IntToFloat(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TReal)aStack.PopInt16());
	}
	
void OpCode::LongToFloat(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	aStack.Push((TReal)aStack.PopInt32());
	}
	
void OpCode::LongToUInt(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	TInt32 arg=aStack.PopInt32();
	if (arg&0xffff0000L)
		User::Leave(KErrOverflow);
	aStack.Push((TInt16)arg);
	}
	
void OpCode::FloatToUInt(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	TReal arg=aStack.PopReal64();
	TInt32 ret;
	if (Math::Int(ret,arg)!=KErrNone || ret&0xffff0000L)
		User::Leave(KErrOverflow);
	aStack.Push((TInt16)ret);
	}

//TInt TCoerce::ToDouble(TReal &aDouble,const TDesC &aStr)
//	{
//	TLex lex(aStr);
//	return(lex.Val(aDouble));  // check whether the whole string is consumed??
//	} 

//TInt TCoerce::HexToLong(TInt32 &aLong,const TDesC &aStr)
//	{
//	TLex lex(aStr);
//	return(lex.Val(aLong),EHex);     // check whether the whole string is consumed??
//	} 

/*
void FuncOpCode::HexStr(CStack& aStack, COplRuntime& , CFrame* )
	{
	TBuf8<0x100> ret;
	ret.Num(aStack.PopInt32(),EHex);
	aStack.Push(ret);
	}

void FuncOpCode::FixStr(CStack& aStack, COplRuntime& , CFrame*)
	{
	TRealFormat format(12);	
	format.iType=EFixed;
	format.iWidth=12;
	format.iDecimalPlaces=0;
	format.iPoint='.';
	format.iTriad=',';
	format.iTriLen=3;
	TBuf<0x100> ret;
	ret.Num(aStack.PopReal64(),format);
	aStack.Push(ret);
	}

void FuncOpCode::GenStr(CStack& aStack, COplRuntime&, CFrame* )
	{
	TRealFormat format(12);	
	format.iType=EGeneral;
	format.iWidth=12;
	format.iDecimalPlaces=0;
	format.iPoint='.';
	format.iTriad=',';
	format.iTriLen=3;
	TBuf8<0x100> ret;
	ret.Num(aStack.PopReal64(),format);
	aStack.Push(ret);
	}
*/


void FuncOpCode::LeftStr(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// str=Left(aStr,aLen)
	{
	TInt16 len=aStack.PopInt16();
	TPtrC tmpStr=aStack.PopString();
	if (len<0)
		User::Leave(KErrArgument);
	if (len>tmpStr.Length())
		len=(TInt16)tmpStr.Length();
	TPtrC pRetStr=tmpStr.Left(len);
	aStack.Push(pRetStr);
	}

void FuncOpCode::LowerStr(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// str=Lower(aStr)
	{
	TPtrC ptr=aStack.PopString();
	TInt len=ptr.Length();
	TPtr tmpStr((TText*)ptr.Ptr(),len,len);			//setting maxLen to Len means we won't corrupt the stack
	tmpStr.LowerCase();
	aStack.Push(tmpStr);
	}

void FuncOpCode::MidStr(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// str=Mid(aStr,aOff,aLen) 
	{
	TInt16 len=aStack.PopInt16();
	TInt16 off=aStack.PopInt16();
	TPtrC tmpStr=aStack.PopString();
	if (off<1)
		User::Leave(KErrArgument);
	TInt16 tmpLen=(TInt16)(tmpStr.Length()-off+1);
	if (len>tmpLen)
		len=tmpLen;
	if (len<0)
		User::Leave(KErrArgument);
	TPtrC pRetStr=tmpStr.Mid(off-1,len);	// off-1, for the 1st counts 1 in opl and 0 in E32 
	aStack.Push(pRetStr);
	}

void FuncOpCode::ReptStr(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// str=Rept(aStr,aInt)
	{
	TInt16 len=aStack.PopInt16();
	TPtrC repStr=aStack.PopString();
	if (len<0)
		User::Leave(KErrArgument);    // OPL leaves on E_RUN_STRTL
	if ((len*repStr.Length())>255)
		User::Leave(KOplErrStrTooLong);
	TBuf<0x100> tmpStr;
	tmpStr.SetLength(len*repStr.Length());
	tmpStr.Repeat(repStr);
	aStack.Push(tmpStr);
	}

void FuncOpCode::RightStr(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// str=Right(aStr,aLen) 
	{
	TInt16 len=aStack.PopInt16();
	TPtrC tmpStr=aStack.PopString();
	if (len<0)
		User::Leave(KErrArgument);
	if (len>tmpStr.Length())
		len=(TInt16)tmpStr.Length();
	TPtrC pRetStr=tmpStr.Right(len);
	aStack.Push(pRetStr);
	}

void FuncOpCode::UpperStr(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// str=Upper(aStr)
	{
	TPtrC ptr=aStack.PopString();
	TInt len=ptr.Length();
	TPtr tmpStr((TText*)ptr.Ptr(),len,len);	//setting maxLen to Len means we won't corrupt the stack
	tmpStr.UpperCase();
	aStack.Push(tmpStr);
	}

void FuncOpCode::Len(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// int=Len(aStr) 
	{
	TPtrC tmpStr=aStack.PopString();
	aStack.Push((TInt16)tmpStr.Length());
	}

void FuncOpCode::Size(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// int=Size(aStr) 
	{
	TPtrC tmpStr=aStack.PopString();
	aStack.Push((TInt16)tmpStr.Size());
	}

void FuncOpCode::Asc(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// int=Asc(aStr)
	{
	TPtrC tmpStr=aStack.PopString();
	if (tmpStr.Length()==0)
		aStack.Push((TInt16)NULL);
	else
		aStack.Push((TInt16)*tmpStr.Ptr());
	}

void FuncOpCode::ChrStr(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	// str=Chr(aInt)
	{
	TInt tempInt=((aStack.PopInt16()<<16)|0X00000001);
	aStack.Push((TUint16*)&tempInt);
	}

void FuncOpCode::Loc(CStack& aStack, COplRuntime& /* aRuntime*/, CFrame* /*aFramePtr*/)
	{
	TPtrC subStr=aStack.PopString();
	TPtrC tmpStr=aStack.PopString();
	if (subStr.Length()==0)
		aStack.Push((TInt16)1);
	else
		// Fix for bug that LOC is partially broken.
		// Replace FindC since this does language dependent collating, which doesn't work
		// for non-printable characters and can potentially return different results 
		// in different languages
		aStack.Push((TInt16)(tmpStr.FindF(subStr)+1));

	}

void FuncOpCode::LocWithCase(CStack& aStack, COplRuntime& /* aRuntime */, CFrame* /*aFramePtr*/)
	// int=Loc(aStr1,aStr2,aMode)
	{
	TInt mode=aStack.PopInt16(); // pop compare mode, 0=normal, 1=collated, 2=folded
	TPtrC subStr=aStack.PopString();
	TPtrC tmpStr=aStack.PopString();
	if (subStr.Length()==0)
		aStack.Push((TInt16)1);
	else
		{
		if (mode==0)
			aStack.Push((TInt16)(tmpStr.Find(subStr)+1));
		else if (mode==1)
			aStack.Push((TInt16)(tmpStr.FindC(subStr)+1));
		else if (mode==2)
			aStack.Push((TInt16)(tmpStr.FindF(subStr)+1));
		else
			User::Leave(KErrArgument);
		}
	}
