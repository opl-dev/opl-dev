// LB_MATHS.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <opcodes.h>
#include <e32math.h>
#include <oplapi.h>
#include <opltran.h>
#include "frame.h"
#include "oplutil.h"

// TOplRealFormat class

TOplRealFormat::TOplRealFormat(TInt aUserFlags)
	{

	iType|=KDoNotUseTriads;
	if (!(aUserFlags&KOplStateTwoDigitExp)) 
		iType|=KAllowThreeDigitExp;
	}

TOplRealFormat::TOplRealFormat(TInt aUserFlags,TInt aWidth)
	{
	
	iWidth=aWidth;
	iType|=KDoNotUseTriads;
	if (!(aUserFlags&KOplStateTwoDigitExp)) 
		iType|=KAllowThreeDigitExp;
	}

TOplRealFormat::TOplRealFormat(TInt aUserFlags,TInt aWidth,TInt aPlaces)
	{
	
	iWidth=aWidth;
	iPlaces=aPlaces;
	iType|=KDoNotUseTriads;
	if (!(aUserFlags&KOplStateTwoDigitExp)) 
		iType|=KAllowThreeDigitExp;
	}

void OpCode::AddFloat(CStack& aStack, COplRuntime&, CFrame*)
	{
    
	TRealX right(aStack.PopReal64());
	TRealX left(aStack.PopReal64());
	User::LeaveIfError(left.AddEq(right));
	aStack.Push(TReal64(left));
	}
	
void OpCode::SubtractFloat(CStack& aStack, COplRuntime&, CFrame*)
	{

	TRealX right(aStack.PopReal64());
	TRealX left(aStack.PopReal64());
	User::LeaveIfError(left.SubEq(right));
	aStack.Push(TReal64(left));
	}
	
// 50h

void OpCode::MultiplyFloat(CStack& aStack, COplRuntime& , CFrame* )
	{

	TRealX right(aStack.PopReal64());
	TRealX left(aStack.PopReal64());
	User::LeaveIfError(left.MultEq(right));
	aStack.Push(TReal64(left));
	}

void OpCode::DivideFloat(CStack& aStack, COplRuntime& , CFrame* )
	{

	TRealX denominator(aStack.PopReal64());
	TRealX numerator(aStack.PopReal64());
	User::LeaveIfError(numerator.DivEq(denominator));
	aStack.Push(TReal64(numerator));
	}
	
void OpCode::PowerOfInt(CStack& aStack, COplRuntime& , CFrame* )
	{

	TInt16 powerOf=aStack.PopInt16();
	TInt16 number=aStack.PopInt16();
	TInt16 result=1;

	if (powerOf<=0 && number==0)
		User::Leave(KOplErrInvalidArgs);
	if (number==0)
		{
		aStack.Push(number);
		return;
		}
	if (number!=1)
		{		
		for (;powerOf>0;powerOf--)
			result=OplUtil::MultiplyL(result,number);
		for (;powerOf<0;powerOf++)
			result=(TInt16)(result/number);
		}
	aStack.Push((TInt16)result);
	}
	
void OpCode::PowerOfLong(CStack& aStack, COplRuntime& , CFrame* )
	{

	TInt32 powerOf=aStack.PopInt32();
	TInt32 number=aStack.PopInt32();
	TInt32 result=1;
	if (powerOf<=0 && number==0)
		User::Leave(KOplErrInvalidArgs);
	if (number==0)
		{
		aStack.Push(number);
		return;
		}
	if (number!=1)
		{		
		for (;powerOf>0;powerOf--)
			result=OplUtil::MultiplyL(result,number);
		for (;powerOf<0;powerOf++)
			result/=number;
		}
	aStack.Push(result);
	}

void OpCode::PowerOfFloat(CStack& aStack, COplRuntime& , CFrame* )
	{

	TReal powerOf=aStack.PopReal64();
	TReal number=aStack.PopReal64();
	TReal result;
	User::LeaveIfError(Math::Pow(result,number,powerOf));
	aStack.Push(result);
	}

#define _EVAL_IN_RUNTIME
#ifdef _EVAL_IN_RUNTIME	
void FuncOpCode::Eval(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
/*
Evaluate an expression
In:		String expression
Out:	Float result
Leaves on error
*/
	{
	TPtrC string=aStack.PopString();
    // translate expression
    TTranslateError anError;
    COplEvalTranslator *pT=COplEvalTranslator::NewLC();
	CBufFlat *pQc=CBufFlat::NewL(32);
	CleanupStack::PushL(pQc);
	TInt maxStackDepth;
	if (pT->Translate(string,anError,*pQc,maxStackDepth)!=KErrNone) // **** CHANGED TO MAP TRAN ERRORS
		{
		TInt resId;
		User::Leave(OplAPI::MapTranslatorError(anError.Error(),resId));
		}
	if (aStack.FreeBytes()<maxStackDepth)
	    User::LeaveIfError(KOplErrNoMemory);    // not enough stack
    // set IP and run the Qcode
    TUint8* ip=aRuntime.IP();
    TUint8* pQcode=(TUint8*) (pQc->Ptr(0)).Ptr();
    aRuntime.SetIP(pQcode);
	aFramePtr->IncEvalNestCount();
    TInt err=aRuntime.Execute();
	aFramePtr->DecEvalNestCount();
    aRuntime.SetIP(ip);
    // propagate any error
    User::LeaveIfError(err);
    CleanupStack::PopAndDestroy(2);
	}
#endif

void OpCode::PercentGreaterThan(CStack& aStack, COplRuntime& , CFrame* )		
	{
    TRealX a(aStack.PopReal64());
    TRealX b(aStack.PopReal64());
    User::LeaveIfError(a.DivEq(100.0));
    a.AddEq(1.0);
    User::LeaveIfError(b.DivEq(a));
    aStack.Push(TReal64(b));
    }
	
void OpCode::PercentLessThan(CStack& aStack, COplRuntime& , CFrame* )		
    {
    TRealX a(aStack.PopReal64());
    TRealX b(aStack.PopReal64());
    TRealX temp(100.0);
    User::LeaveIfError(temp.DivEq(a));
    temp.AddEq(1.0);
    User::LeaveIfError(b.DivEq(temp));
    aStack.Push(TReal64(b));
    }
	
void OpCode::PercentSubtract(CStack& aStack, COplRuntime& , CFrame* )		
	{
    TRealX a(aStack.PopReal64());
    TRealX b(aStack.PopReal64());
    User::LeaveIfError(a.MultEq(b));
    User::LeaveIfError(a.DivEq(100.0));
    User::LeaveIfError(b.SubEq(a));
    aStack.Push(TReal64(b));
    }
	
void OpCode::PercentMultiply(CStack& aStack, COplRuntime& , CFrame* )		
	{
    TRealX a(aStack.PopReal64());
    TRealX b(aStack.PopReal64());
    User::LeaveIfError(a.MultEq(b));
    User::LeaveIfError(a.DivEq(100.0));
    aStack.Push(TReal64(a));
    }
	
void OpCode::PercentDivide(CStack& aStack, COplRuntime& , CFrame* )			
	{
    TRealX a(aStack.PopReal64());
    TRealX b(aStack.PopReal64());
    User::LeaveIfError(b.MultEq(100.0));
    User::LeaveIfError(b.DivEq(a));
    aStack.Push(TReal64(b));
    }

void OpCode::PercentAdd(CStack& aStack, COplRuntime& , CFrame* )				
	{
    TRealX a(aStack.PopReal64());
    TRealX b(aStack.PopReal64());
    User::LeaveIfError(a.MultEq(b));
    User::LeaveIfError(a.DivEq(100.0));
    User::LeaveIfError(b.AddEq(a));
    aStack.Push(TReal64(b));
    }

void FuncOpCode::Abs(CStack& aStack, COplRuntime& , CFrame* )
	{
	TReal arg=aStack.PopReal64();
	aStack.Push(arg>=0.0 ? arg : -arg);
	}

void FuncOpCode::IAbs(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt32 arg=aStack.PopInt32();
	aStack.Push(arg>=0 ? arg : -arg);
	}

void FuncOpCode::ACos( CStack& aStack, COplRuntime& , CFrame* /*  */)
	{
	User::LeaveIfError(Math::ACos(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::ASin(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::ASin(*aStack.StackPtr().Real,*aStack.StackPtr().Real));	
	}

void FuncOpCode::ATan(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::ATan(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::Cos(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::Cos(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::Deg(CStack& aStack, COplRuntime& , CFrame* )
	{
    TRealX arg(aStack.PopReal64());
    User::LeaveIfError(arg.MultEq(KRadToDeg));
    aStack.Push(TReal64(arg));
	}

void FuncOpCode::Exp(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::Exp(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::Flt(CStack& aStack, COplRuntime& , CFrame* )
    // long to double	
	{
	TInt32 arg=aStack.PopInt32();
	aStack.Push(TReal(arg));
	}

void FuncOpCode::Intf(CStack& aStack, COplRuntime& , CFrame* )
	// double to integerised double
	{
    User::LeaveIfError(Math::Int(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::IntLong(CStack& aStack,COplRuntime&, CFrame*)
   // Double to long integer
    {
	TInt32 res;
	if (Math::Int(res,aStack.PopReal64())<0)
		User::Leave(KErrOverflow);		
	aStack.Push(res);
	}

void FuncOpCode::Ln(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::Ln(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::Log(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::Log(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::Pi(CStack& aStack, COplRuntime& , CFrame* )
	{
	aStack.Push(KPi);
	}

void FuncOpCode::Rad(CStack& aStack, COplRuntime& , CFrame* )
	{
	TRealX arg(aStack.PopReal64());
	User::LeaveIfError(arg.DivEq(TRealX(KRadToDeg)));
	aStack.Push(TReal64(arg));
	}

void FuncOpCode::Rnd(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	aStack.Push(Math::FRand(aRuntime.Seed()));
	}


void FuncOpCode::Sin(CStack& aStack, COplRuntime& , CFrame* )
	{									 
	User::LeaveIfError(Math::Sin(*aStack.StackPtr().Real,*aStack.StackPtr().Real));		
	}

void FuncOpCode::Sqr(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::Sqrt(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::Tan(CStack& aStack, COplRuntime& , CFrame* )
	{
	User::LeaveIfError(Math::Tan(*aStack.StackPtr().Real,*aStack.StackPtr().Real));
	}

void FuncOpCode::Val(CStack& aStack, COplRuntime& , CFrame* )
	{
	TLex lex(aStack.PopString());
    lex.SkipSpaceAndMark();
	TReal result;
	if (lex.Val(result)<0)
		User::Leave(KErrArgument);
	aStack.Push(result);
	}

#pragma warning ( disable : 4706) // assignment within conditional expression 
TReal* ptrList(TInt& count,CStack& aStack,COplRuntime& aRuntime) 
/*
Point to a list of doubles, whether on the stack or in an array.
Byte at IP is 0 for an array or else the count of arguments on the stack for a list.
If an array is passed, the stack contains an 'OPL left-side reference' which, for a variable
or array is
{
TUint8 flag;	// zero for a normal variable
TAny *ptr;		// address of the variable or array
}
*/
	{

	if (!(count=aRuntime.IP8()))
		{			// it's an array
		count=aStack.PopInt16();		    // count on stack for array
		aStack.PopInt16();					// throw away field flag
        TInt16* ret=(TInt16*)aStack.PopPtr();	    // ptr to array
        if (count<=0 || count>*(ret-1))      // array count 16-bits before ptr
            User::Leave(KOplErrSubs);
        return (TReal *)ret;
		}
	else
		{
		TReal* ret=aStack.StackPtr().Real;
		TStackPtr stackPtr;
		stackPtr.Real =  ret + count;
		aStack.SetStackPtr(stackPtr);
		return ret;
		}
	}
#pragma warning ( default: 4706)

void FuncOpCode::Max(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	TInt count;
	TReal* list=ptrList(count,aStack,aRuntime);  
    TReal result=OplUtil::GetFloat(list++);
	for (TInt ii=(count-1);ii;ii--)
		{ 
        TReal val=OplUtil::GetFloat(list++);       
        if (result<val)
			result=val;
		}
	aStack.Push(result);
	}

void FuncOpCode::Mean(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{ 
	TInt count;
	TReal* list=ptrList(count, aStack, aRuntime);
	TRealX result=0.0;
	for (TInt ii=count;ii;ii--)
		{
        User::LeaveIfError(result.AddEq(TRealX(OplUtil::GetFloat(list++))));
		}
    
    User::LeaveIfError(result.DivEq(count));
    aStack.Push(TReal64(result));
	}

void FuncOpCode::Min(CStack& aStack, COplRuntime& aRuntime , CFrame* )
	{
	TInt count;
	TReal* list=ptrList(count,aStack,aRuntime);  
	TReal result=OplUtil::GetFloat(list++);
	for (TInt ii=(count-1);ii;ii--)
		{ 
								// list is return value
		TReal val=OplUtil::GetFloat(list++);
        if (val<result)
			result=val;
		}
	aStack.Push(result);
	}

void FuncOpCode::Std(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	FuncOpCode::Var(aStack,aRuntime,aFramePtr);
	FuncOpCode::Sqr(aStack,aRuntime,aFramePtr);
    }

void FuncOpCode::Sum(CStack& aStack, COplRuntime& aRuntime, CFrame* )
/* 
SUM(array(), count%) has *IP8==0
			 stack[0] == array ref
			 stack[1] == count

SUM(real1,real2,...) has *IP8==count
			 stack[0] == real1
			 stack[1] == real2
			 etc.
 */
	{
	TInt count;
	TReal* list=ptrList(count,aStack,aRuntime);
	TRealX result = 0.0;
	TInt ii = 0;
	for (ii=count; ii; ii--)
		{
        User::LeaveIfError(result.AddEq(TRealX(OplUtil::GetFloat(list++))));
		}
	aStack.Push(TReal64(result));
	}

void FuncOpCode::Var(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
		TInt count;
	TReal* list=ptrList(count,aStack,aRuntime);
	TRealX result = 0.0;
	TRealX var =0.0;
   	for (TInt ii=count;ii;ii--)
		{
        User::LeaveIfError(result.AddEq(TRealX(OplUtil::GetFloat(list++))));
		}
	    TRealX mean=TReal64(result);
        User::LeaveIfError(mean.DivEq(count));
    list-=count;
	for (TInt iii=count; iii; iii--)
		{
        TRealX val=OplUtil::GetFloat(list++);		
        TRealX temp(val);
        User::LeaveIfError(temp.SubEq(mean));
        User::LeaveIfError(temp.MultEq(temp));
        User::LeaveIfError(var.AddEq(temp));
		}
    User::LeaveIfError(var.DivEq(TRealX(count-1)));
    aStack.Push(TReal64(var));
	}

void PushJustified(CStack& aStack, TBuf<256>& buffer, TInt width,TInt err)
	{
	TInt absWidth=width;
	if (width<0)
		absWidth=-width;
	if (err<0)
	   { 
	   buffer.SetLength(absWidth);
	   buffer.Fill('*');
	   }
	else if (width<0)
    	{
		TInt length=buffer.Length();
		TInt pad= absWidth - length;
		if (pad<0)
			{
		    buffer.SetLength(absWidth);
			buffer.Fill('*');
			}
		else
			{
  			TBuf<256> spaces(pad);
			spaces.Fill(' ');
			buffer.Insert(0,spaces);
			}
		}
	aStack.Push(buffer);
	}

LOCAL_C TInt PushRealString(CStack& aStack, COplRuntime& aRuntime, TInt aWidth,TInt aDecimals,TInt aEType)
	{
	TReal arg=aStack.PopReal64();	  
	if (aWidth<-255||aWidth>255)
		return KOplErrInvalidArgs;
	TBuf<256> buffer;
	TInt flags=aRuntime.UserFlags();
	TOplRealFormat format(flags,aWidth >= 0 ? aWidth : -aWidth,aDecimals);// object(width)  object (width, no.of d.p.'s)
	format.SetType(aEType);
	// format.iTriLen=0;
	TInt err=buffer.Num(arg,format); 				 // fills buffer with arg in format
    buffer.UpperCase();		
    PushJustified(aStack,buffer,aWidth,err);
	return KErrNone;
	}

void FuncOpCode::FixStr(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	// double to fixed decimal places string  Fix$(x,d.p's,length of string)
	TInt16 width=aStack.PopInt16();
	TInt16 decimals=aStack.PopInt16();
	User::LeaveIfError(PushRealString(aStack,aRuntime,width,decimals,KRealFormatFixed));					
	}

void FuncOpCode::GenStr(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	// double to general format string	  Gen$(x,length of string)
	TInt16 width=aStack.PopInt16();
	User::LeaveIfError(PushRealString(aStack,aRuntime,width,0,KRealFormatGeneral));					
	}

void FuncOpCode::HexStr(CStack& aStack, COplRuntime& , CFrame* )
	{
    // long to hex string				  Hex$(x&)
	TBuf<256> buffer;
	buffer.Num(aStack.PopInt32(),EHex);
 	buffer.UpperCase();		
	aStack.Push(buffer);
	}   

void FuncOpCode::NumStr(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	TInt16 width=aStack.PopInt16();
	User::LeaveIfError(PushRealString(aStack,aRuntime,width,0,KRealFormatFixed));					
	}
 
void FuncOpCode::SciStr(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	// floating point to string in scientific format
	TInt16 width=aStack.PopInt16();
	TInt16 decimals=aStack.PopInt16();
	User::LeaveIfError(PushRealString(aStack,aRuntime,width,decimals,KRealFormatExponent));					
	}

// OpCode class
void OpCode::Randomize(CStack& aStack, COplRuntime& aRuntime, CFrame* /* aFramePtr */)
	{
	TInt32 s=aStack.PopInt32();
	aRuntime.Seed().Set(s,s);
	}

/* End of $Workfile:   LB_MATHS.CPP  $ */
  
  
