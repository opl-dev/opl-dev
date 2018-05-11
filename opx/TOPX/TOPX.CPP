// TOPX.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include "topx.h"
#include <oplerr.h>
                                                         
////////////////////////////////////////////////////////////////////////////////////////////
// COpxRoot class : derives from COpxBase
//

// The language extension procedures provided by this OPX
//
void COpxRoot::Add3() const
//
	{
	TInt32 i3=iOplAPI.PopInt32();
	TInt32 i2=iOplAPI.PopInt32();
	TInt32 i1=iOplAPI.PopInt32();
	iOplAPI.Push(i1+i2+i3);
	}

void COpxRoot::IncNumericArgsPassedByref() const
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

// The members of COpxRoot which are not language extension procedures
//
COpxRoot::COpxRoot(OplAPI& aOplAPI) : COpxBase(aOplAPI)
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
	case EAdd3:
		Add3();
		break;
	case EIncNumericArgsPassedByref:
		IncNumericArgsPassedByref();
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