// EN_ERROR.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <e32std.h>
#include <opcodes.h>
#include <e32math.h>
#include <oplr.rsg>
#include <module.h>
#include "oplutil.h"
#include "frame.h"
#include <opldbg.h>
	
///
// CoplRuntime class
///

void COplRuntime::ClearError(CFrame* aFP)
	{
	COplRuntime::iStack->SetStackPtr(aFP->StackPtr()); // remove expressions on stack
	ClearTrap();
	ClearStateFlags(KErrBufferCurrent);
	}

TInt COplRuntime::ErrorHandler()
/*
	Look for TRAP set
	Walk frames looking for error handler
	Return 0 if error trapped

	NB. Should there be some untrappable errors, e.g.:
		- Undefined externals
		- Illegal opcode
		- etc.

*/
	{
#if defined(NO_SEQUENCER)
	while (iFrame!=NULL)
		delete iFrame;
	TBuf<256> buf;
	_LIT(KShowError,"*** Error occurred : %d ***\r\n");
	buf.Format(KShowError,iErrorValue);
	iConsole->Write(buf);
	return ETrue;
#else
//  FlushSignals(); // eb205: why do we need to reinstate cursors, etc here? commented out for now
	if (!(StateFlags()&KErrBufferCurrent))
		SetErrorBuffer(NULL);
//	if (iErrorValue==KOplErrIllegal || iErrorValue==KOplErrUndef)
//		return ETrue;			// untrappable errors
	if (iTrap)
		{	// TRAPped
		ClearError(iFrame);
		return EFalse;
		}
	// Set up resbuf in case untrapped
	CProcDef& procDef=iFrame->ProcDef();
	CProcedure* proc=(procDef.Procedure());
	// file offset=(offset proc start)+(datadef)+(offset of IP from start of proc's QCode)
#if 0
	// Until proper error location supported by tran, give file offset
	iRuntimeResBuf.iOffset=procDef.FileOffset()+proc.iDataSize+(IP()-proc.iQCodeCell);
#else
	iRuntimeResBuf.iOffset=(proc!=NULL)?(IP()-(proc->iQCodeCell)-1):-1;			// data size not included : -1 because QCode at IP has been read
	iRuntimeResBuf.iProcLineNumber=procDef.LineInSource();	// line number of proc start in source
#endif
	iRuntimeResBuf.iSrcFile=procDef.Module().OplName();
	iRuntimeResBuf.iProcName=procDef.ProcName();
	if (iDebuggerBase!=NULL)
		iDebuggerBase->ErrorOccurred(iErrorValue);
	while (iFrame!=NULL)
		{
		if (iFrame->EvalNestCount())
			break;
		if (iFrame->OnErrIP()!=NULL)
			{					// found error handler
			SetIP(iFrame->OnErrIP());
			ClearError(iFrame);
			return EFalse;
			}
		delete iFrame;	// sets iFrame and iIP to caller's
		}

	return ETrue;
#endif
	}

void COplRuntime::SetErrorBuffer(const TDesC* aDesPtr)
	{
	SetStateFlags(KErrBufferCurrent);
	if (iFrame)
		{
		ConEnv()->ReadResource(iErrBuf,R_OPLR_ERROR_IN);
		TFileName fname=iFrame->ProcDef().ModuleName();
		fname.UpperCase(); //should be uppercase throughout, really.
		TParsePtrC parse(fname);
		_LIT(KSlash,"\\");
		if (AppendErrorText(parse.Name())
			||AppendErrorText(KSlash)
			||AppendErrorText(iFrame->ProcDef().ProcName())
			)
			return;
		}
	else
		iErrBuf.Zero();
	if (aDesPtr!=NULL)
		{
		_LIT(KComma,",");
		if (iFrame)
			AppendErrorText(KComma);
		AppendErrorText(*aDesPtr);
		}
    }

TInt COplRuntime::AppendErrorText(const TDesC& aDes)
	{
	TInt room=iErrBuf.MaxLength()-iErrBuf.Length();
	if (room<aDes.Length())
		{
		iErrBuf.Append(aDes.Ptr(),room); //add as much as we can
		return KErrTooBig;
		}
	iErrBuf.Append(aDes);
	return KErrNone;
	}





void OpCode::Trap(CStack&, COplRuntime& aRuntime, CFrame*)
	{	// set trap flag and clear error value
	aRuntime.SetTrap();
	aRuntime.ClearError();
	}

void OpCode::OnErr(CStack&, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt16 offset=aRuntime.IP16();
	TUint8* newIP=NULL;
	if (offset)
		newIP=aRuntime.IP()+offset-3;
	aFramePtr->SetOnErrIP(newIP);	
	}

void OpCode::Raise(CStack& aStack, COplRuntime& , CFrame*)
	{
	User::Leave(KOplErrorBase+(TUint16)aStack.PopInt16());
	}

void FuncOpCode::Err(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	aStack.Push(OplUtil::MapError(aRuntime.ErrorValue()));
    }

void FuncOpCode::ErrStr(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	TInt16 err=aStack.PopInt16();
	TBuf<128> buf;
	aRuntime.GetErrStr(buf,err);
	aStack.Push(buf);
	}

void FuncOpCode::ErrxStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
    {
    aStack.Push(aRuntime.ErrBuf());
    }                                                                                  

void COplRuntime::GetErrStr(TDes& aDes,TInt16 aError)
	{
	TInt resourceNo=OplUtil::GetErrResource(aError);
	if (resourceNo==R_OPLR_UNKNOWN_ERROR)
		{
		ConEnv()->Format128(aDes,resourceNo,aError); //put in error number for unknown errors
		return;
		}
	ConEnv()->ReadResource(aDes,resourceNo);
	}


/* End of $Workfile:   EN_ERROR.CPP  $ */
