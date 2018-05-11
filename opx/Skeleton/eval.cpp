// Eval.cpp
//
// Copyright (c) 1999-2000 Symbian Ltd. All rights reserved.

#include "eval.h"

COpxEvalWrapper::~COpxEvalWrapper()
	{
	}

LOCAL_C EvaluateL(TDesC& aEvalString)
	{
	}


void COpxEvalWrapper::EvalL(OplAPI& aOplAPI)
	{
	// OPL call: returnValue=EvEval:(aEvaluatedString$)
	TPtrC string=aOplAPI.PopString();
	TReal64 ret=EvaluateL(string);
	aOplAPI.Push(ret);
	}

COpxEval::COpxEval(OplAPI& aOplAPI) 
	: COpxBase(aOplAPI)
	{
	}

COpxEval* COpxEval::NewLC(OplAPI& aOplAPI)
	{
	COpxEval* This=new(ELeave) COpxEval(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	return This;
	}

void COpxEval::ConstructL()
	{
	iOpxEvalWrapper=new(ELeave) COpxEvalWrapper;
	}

COpxEval::~COpxEval()
	{
	delete iOpxEvalWrapper;
	Dll::FreeTls();
	}

void COpxEval::RunL(TInt aProcNum)
	{
	switch (aProcNum)
		{
	case EEval:
		iOpxEvalWrapper->EvalL(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxEval::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	{
	COpxEval* tls=((COpxEval*)Dll::Tls());
	if (tls==NULL)    // tls is NULL on loading an OPX DLL (also after unloading it)
		{
		tls=COpxEval::NewLC(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop();    // tls
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}