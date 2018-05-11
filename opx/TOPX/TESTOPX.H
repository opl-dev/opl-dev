// TESTOPX.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#if !defined(__TESTOPX_H__)
#define __TESTOPX_H__

#if !defined(__E32MATH_H__)
#include <e32math.h>
#endif
#if !defined(__F32FILE_H__)
#include <f32file.h>
#endif
#if !defined(__OPLTDEF_H__)
#include <opltdef.h>
#endif
#if !defined(__OPLDB_H__)
#include <opldb.h>
#endif
#if !defined(__OPLR_RSG__)
#include <oplr.rsg>
#endif
#if !defined(__OPLAPI_H__)
#include <oplapi.h>
#endif
#if !defined(__OPX_H__)
#include <opx.h>
#endif

const TInt KUidOpxTest=0x1;
const TInt KOpxVersion=0x110;

class COpxRoot : public COpxBase 
    {
public:
    static COpxRoot* NewL(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
	enum TExtensions
		{
		EAddFloatToInt=1,
		EAdd4,
		EStrToFlt,
		EFltToStr,
		ELowerToUpper,
		EStackCheck,
		EFreeBytes,
		EIncIntByAddr,
		EIncLngByAddr,
		EIncFltByAddr,
		EIncByRef3,
		EMapE32Err,
		EMapSyntaxErr,
		EMapTooManyOPXErr,
		EMult316,
		EMult332,
		EDBManCount,
		ECallProc,
		ECallProcRetInt,
		ECallProcRetLng,
		ECallProcRetFlt,
		ECallProcRetStr,
		EAppUid
		};		
		
		void AddFloatToInt();
		void Add4();
		void StringToFloat();
		void FloatToString();
		void LowerToUpper();
		void StackCheck();
		void FreeBytes();
		void IncIntByAddr();
		void IncLongByAddr();
		void IncFloatByAddr();
		void IncByRef3();
		void MapE32Err();
		void MapSyntaxErr();		
		void MapTooManyOPXErr();
		void Mult316();
		void Mult332();
		void DBaseManCount();
		void SimpleProcCall();
		void CallProcRetInt();
		void CallProcRetLong();
		void CallProcRetFlt();
		void CallProcRetStr();
		void GetAppUid();
private:
	void ConstructL();
    COpxRoot(OplAPI& aOplAPI);
    ~COpxRoot() ;
    };

inline COpxRoot* OpxRoot() { return((COpxRoot *)Dll::Tls()); }

#endif __TESTOPX_H__