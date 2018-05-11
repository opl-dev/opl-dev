// TESTOPX.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include "testopx.h"
#include <oplerr.h>

//
// OPX Procedures in this OPX
//
void COpxRoot::AddFloatToInt()
	{

	TInt16 a2;
	TInt ret=Math::Int(a2,iOplAPI.PopReal64());
	if (ret==KErrUnderflow)
		ret=KErrOverflow;
	User::LeaveIfError(ret);
	TInt16 a1=iOplAPI.PopInt16();
	iOplAPI.Push(TInt16(a1+a2));
	}

void COpxRoot::Add4()
	{

	TPtrC8 p4=iOplAPI.PopString8();
	TLex8 lex(p4);
	TInt32 a4;
	TInt ret=lex.Val(a4);
	User::LeaveIfError(ret);
	TInt32 a3;
	ret=Math::Int(a3,iOplAPI.PopReal64());
	if (ret==KErrUnderflow)
		ret=KErrOverflow;
	User::LeaveIfError(ret);
	TInt32 a2=iOplAPI.PopInt32();
	TInt32 a1=iOplAPI.PopInt16();
	TInt32 res1=a1+a2;
	if ((a1^a2)>=0)
		{					// args same sign
		if ((a1^res1)<0)
			{				// result different sign
			User::Leave(KOplErrOverflow);
			}
		}
	TInt32 res2=res1+a3;
	if ((res1^a3)>=0)
		{					// args same sign
		if ((a3^res2)<0)
			{				// result different sign
			User::Leave(KOplErrOverflow);
			}
		}
	TInt32 res=res2+a4;
	if ((a4^res2)>=0)
		{					// args same sign
		if ((a4^res)<0)
			{				// result different sign
			User::Leave(KOplErrOverflow);
			}
		}
	iOplAPI.Push(res);
	}

void COpxRoot::StringToFloat()
	{

	TPtrC8 p1=iOplAPI.PopString8();
	TLex8 lex(p1);
	TReal64 a1;
	TInt ret=lex.Val(a1);
	User::LeaveIfError(ret);
	iOplAPI.Push(a1);
	}

void COpxRoot::FloatToString()
	{

	TReal64 a1=iOplAPI.PopReal64();
	TRealFormat format;
	TBuf <0x100> buf;
	TInt ret=buf.Num(a1,format);
	User::LeaveIfError(ret);
	iOplAPI.PushL(TDesC8(buf));	
	}

void COpxRoot::LowerToUpper()
	{
	TBuf8<256> buf1=_L8("\1");
	TBuf8<256> buf2=iOplAPI.PopString8();
	buf2.UpperCase();
	buf1.Append(buf2);
	iOplAPI.PushL(buf1.Ptr());	
	}

void COpxRoot::StackCheck()
	{

	TAny* sp=iOplAPI.StackPtr();
	TInt16 a1=1;
	iOplAPI.Push(a1);
	TInt32 a2=1;
	iOplAPI.Push(a2);
	TReal64 a3=1.0;
	iOplAPI.Push(a3);
	TBuf8<0x100> a4=_L("Hello");
	iOplAPI.PushL(a4);
	TPtrC8 p4=iOplAPI.PopString8();
	iOplAPI.SetStackPtr(sp);
	if (iOplAPI.StackPtr()!=sp)
		User::Leave(KOplErrorBase+TInt16(1));
	iOplAPI.Push(0.0);	
	}

void COpxRoot::FreeBytes()
	{

	TInt free=iOplAPI.FreeBytes();
	if (free<0)
		User::Leave(KOplErrorBase+TInt16(1));
	iOplAPI.Push(0.0);
	}

void COpxRoot::IncIntByAddr()
// passed addr(i%)
	{
	
	TInt32 addr=iOplAPI.PopInt32();
	TAny* p=iOplAPI.OffsetToAddrL(addr,sizeof(TInt16));
	TInt16 a=(*(TInt16*)p)++;
	p=(TAny*)&a;
	iOplAPI.Push(0.0);
	}

void COpxRoot::IncLongByAddr()
// passed addr(l&)
	{

	TInt32 addr=iOplAPI.PopInt32();
	TAny* p=iOplAPI.OffsetToAddrL(addr,sizeof(TInt32));
	iOplAPI.PutLong(p,iOplAPI.GetLong(p)+1);
	iOplAPI.Push(0.0);
	}

void COpxRoot::IncFloatByAddr()
// passed addr(f)
	{
	
	TInt32 addr=iOplAPI.PopInt32();
	TAny* p=iOplAPI.OffsetToAddrL(addr,sizeof(TReal64));
	iOplAPI.PutFloat(p,iOplAPI.GetFloat(p)+1.0);
	iOplAPI.Push(0.0);	
	}

void COpxRoot::IncByRef3()
// passed "byref" 
	{

	TAny* p3=iOplAPI.PopPtrReal64();
	iOplAPI.PutFloat(p3,iOplAPI.GetFloat(p3)+1.0);
	TAny* p2=iOplAPI.PopPtrInt32();
	iOplAPI.PutLong(p2,iOplAPI.GetLong(p2)+1);
	TAny* p1=iOplAPI.PopPtrInt16();
	TInt16 a1=(*(TInt16*)p1)++;
	p1=(TAny*)&a1;
	iOplAPI.Push(0.0);
	}

void COpxRoot::MapE32Err()
	{

	TInt32 e32Err=iOplAPI.PopInt32();
	TInt16 oplErr=iOplAPI.MapError(e32Err);
	iOplAPI.Push(oplErr);
	}

void COpxRoot::MapSyntaxErr()
	{

	TInt32 err=EErrSyntax;
	TInt rid;
	TInt ret=iOplAPI.MapTranslatorError(err,rid);
	if (rid==R_OPLR_SYNTAX_ERROR)
		User::Leave(KOplErrorBase+TInt16(1));
	User::LeaveIfError(ret);
	iOplAPI.Push(0.0);
	}

void COpxRoot::MapTooManyOPXErr()
	{

	TInt32 err=EErrTooManyOPXs;
	TInt rid;
	TInt ret=iOplAPI.MapTranslatorError(err,rid);
	if (rid==R_OPLR_TOO_MANY_OPXS)
		User::Leave(KOplErrorBase+TInt16(1));
	User::LeaveIfError(ret);
	iOplAPI.Push(0.0);
	}

void COpxRoot::Mult316()
	{

	TInt16 a3=iOplAPI.PopInt16();
	TInt16 a2=iOplAPI.PopInt16();
	TInt16 a1=iOplAPI.PopInt16();
	TInt16 res=iOplAPI.MultiplyL(a1,a2);
	iOplAPI.Push(iOplAPI.MultiplyL(res,a3));
	}

void COpxRoot::Mult332()
	{

	TInt32 a3=iOplAPI.PopInt32();
	TInt32 a2=iOplAPI.PopInt32();
	TInt32 a1=iOplAPI.PopInt32();
	TInt32 res=iOplAPI.MultiplyL(a1,a2);
	iOplAPI.Push(iOplAPI.MultiplyL(res,a3));	
	}

void COpxRoot::DBaseManCount()
	{

	COplDbManager* dbmp=iOplAPI.DbManager();
	// dbmp->OpenCheckL();
	TInt32 n=dbmp->Count();
	iOplAPI.Push(n);
	}

void COpxRoot::SimpleProcCall()
	{

	// pop a procedure name of proc which takes no args and returns nothing and call
	TBuf8<0x100> proc=iOplAPI.PopString8();
	iOplAPI.InitCallbackL(proc);
	TInt ret=iOplAPI.CallProcedure(EReturnFloat);
	User::LeaveIfError(ret);
	TReal64 retval=iOplAPI.PopReal64();
	if (retval!=0.0)
		User::Leave(KOplErrorBase+TInt16(1));
	iOplAPI.Push(0.0);
	}

void COpxRoot::CallProcRetInt()
	{
	
	// call a proc taking 4 params and returning int 1
	TBuf8<256> a5=iOplAPI.PopString8();
	TReal64 a4=iOplAPI.PopReal64();
	TInt32 a3=iOplAPI.PopInt32();
	TInt16 a2=iOplAPI.PopInt16();
	TBuf8<256> proc=iOplAPI.PopString8();
	iOplAPI.InitCallbackL(proc);
	iOplAPI.PushParamL(a2);
	iOplAPI.PushParamL(a3);
	iOplAPI.PushParamL(a4);
	iOplAPI.PushParamL(a5);
	TInt ret=iOplAPI.CallProcedure(EReturnInt);
	if (ret)
		User::Leave(ret);
	TInt16 retval=iOplAPI.PopInt16();
	if (retval!=TInt16(1))
		User::Leave(KOplErrorBase+TInt16(1));
	iOplAPI.Push(0.0);
	}

void COpxRoot::CallProcRetLong()
	{
	
	// call a proc taking 4 params and returning long 1
	TBuf8<256> a5=iOplAPI.PopString8();
	TReal64 a4=iOplAPI.PopReal64();
	TInt32 a3=iOplAPI.PopInt32();
	TInt16 a2=iOplAPI.PopInt16();
	TBuf8<256> proc=iOplAPI.PopString8();
	iOplAPI.InitCallbackL(proc);
	iOplAPI.PushParamL(a2);
	iOplAPI.PushParamL(a3);
	iOplAPI.PushParamL(a4);
	iOplAPI.PushParamL(a5);
	TInt ret=iOplAPI.CallProcedure(EReturnLong);
	if (ret)
		User::Leave(ret);
	TInt32 retval=iOplAPI.PopInt32();
	if (retval!=1)
		User::Leave(KOplErrorBase+TInt16(1));	
	iOplAPI.Push(0.0);
	}

void COpxRoot::CallProcRetFlt()
	{
	
	// call a proc taking 4 params and returning float 1.0
	TBuf8<256> a5=iOplAPI.PopString8();
	TReal64 a4=iOplAPI.PopReal64();
	TInt32 a3=iOplAPI.PopInt32();
	TInt16 a2=iOplAPI.PopInt16();
	TBuf8<256> proc=iOplAPI.PopString8();
	iOplAPI.InitCallbackL(proc);
	iOplAPI.PushParamL(a2);
	iOplAPI.PushParamL(a3);
	iOplAPI.PushParamL(a4);
	iOplAPI.PushParamL(a5);
	TInt ret=iOplAPI.CallProcedure(EReturnFloat);
	if (ret)
		User::Leave(ret);
	TReal64 retval=iOplAPI.PopReal64();
	if (retval!=1.0)
		User::Leave(KOplErrorBase+TInt16(1));		
	iOplAPI.Push(0.0);
	}

void COpxRoot::CallProcRetStr()
	{
	
	// call a proc taking 4 params and returning string "Hello"
	TBuf8<256> a5=iOplAPI.PopString8();
	TReal64 a4=iOplAPI.PopReal64();
	TInt32 a3=iOplAPI.PopInt32();
	TInt16 a2=iOplAPI.PopInt16();
	TBuf8<256> proc=iOplAPI.PopString8();
	iOplAPI.InitCallbackL(proc);
	iOplAPI.PushParamL(a2);
	iOplAPI.PushParamL(a3);
	iOplAPI.PushParamL(a4);
	iOplAPI.PushParamL(a5);
	TInt ret=iOplAPI.CallProcedure(EReturnString);
	if (ret)
		User::Leave(ret);
	TBuf8<0x100> retval=iOplAPI.PopString8();
	TBuf8<0x100> comp=_L("Hello");
	if (retval.Compare(TPtrC(comp))!=0)
		User::Leave(KOplErrorBase+TInt16(1));		
	iOplAPI.Push(0.0);
	}

void COpxRoot::GetAppUid()
	{

	TUid uid=iOplAPI.AppUid();
	TUidName uidname=uid.Name();
	iOplAPI.PushL(TDesC8(uidname));
	}

/////////////////////

COpxRoot::COpxRoot(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

COpxRoot* COpxRoot::NewL(OplAPI& aOplAPI)
    {
    COpxRoot* This=new(ELeave) COpxRoot(aOplAPI);
    CleanupStack::PushL(This);
    This->ConstructL();
    CleanupStack::Pop();
    return This;
    }

void COpxRoot::ConstructL()
	{
	// Do whatever is required to constuct any component members you have in COpxRoot
	// ...
	} 

COpxRoot::~COpxRoot()
	{
	Dll::FreeTls();		// Required so that Tls is set to zero on unloading the OPX in UNLOADM
	}

// COpxBase implementation
//
void COpxRoot::RunL(TInt aProcNum)
// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case EAddFloatToInt:
		AddFloatToInt();
		break;
	case EAdd4:
		Add4();
		break;
	case EStrToFlt:
		StringToFloat();
		break;
	case EFltToStr:
		FloatToString();
		break;
	case ELowerToUpper:
		LowerToUpper();
		break;
	case EStackCheck:
		StackCheck();
		break;
	case EFreeBytes:
		FreeBytes();
		break;
	case EIncIntByAddr:
		IncIntByAddr();
		break;
	case EIncLngByAddr:
		IncLongByAddr();
		break;
	case EIncFltByAddr:
		IncFloatByAddr();
		break;
	case EIncByRef3:
		IncByRef3();
		break;
	case EMapE32Err:
		MapE32Err();
		break;
	case EMapSyntaxErr:
		MapSyntaxErr();
		break;
	case EMapTooManyOPXErr:
		MapTooManyOPXErr();
	case EMult316:
		Mult316();
		break;
	case EMult332:	
		Mult332();
		break;
	case EDBManCount:
		DBaseManCount();
		break;
	case ECallProc:
		SimpleProcCall();
		break;
	case ECallProcRetInt:
		CallProcRetInt();
		break;
	case ECallProcRetLng:
		CallProcRetLong();
		break;
	case ECallProcRetFlt:
		CallProcRetFlt();
		break;
	case ECallProcRetStr:
		CallProcRetStr();
		break;
	case EAppUid:
		GetAppUid();
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxRoot::CheckVersion(TInt aVersion)
// To check whether the opx is a compatible version
// *** Change as required ***
	{
	if ((aVersion & 0x0f00)>(KOpxVersion & 0xf00))	// major version must be <= OPX's version
		return EFalse; // bad version
	else
		return ETrue; // ok
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
// Creates a COpxBase instance as required by the OPL runtime
// This object is to be stored in the OPX's TLS as shown below
	{
	COpxRoot* tls=((COpxRoot*)Dll::Tls());
	if (tls==NULL)		// tls is NULL on loading an OPX DLL (also after unloading it)
		{
		tls=COpxRoot::NewL(aOplAPI);
		CleanupStack::PushL(tls);
		TInt ret=Dll::SetTls(tls);
		User::LeaveIfError(ret);
		CleanupStack::Pop();	// tls
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return(KErrNone);
	}