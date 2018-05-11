// FRAME.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef FRAME_H
#define FRAME_H

#include <oplr.h>

// if these constants are equal reading a string needs only one function
const TInt KMaxGlobalNameLen=32; // also defined in GlobTbl.h
const TInt KMaxProcNameLen=32; // also defined in Module.h

// flags used in frame
const TInt KCalledByString=0x01;
const TInt KGlobalsAdded=0x02;
const TInt KDebuggerSignalled=0x04;

struct SOplGlobal
	{
	TBufC<KMaxGlobalNameLen> name;
	TInt type;
	TInt offset;
	};

struct SOplSubProc
	{
	TInt offset; // offset to name in q-code
	TBufC<KMaxProcNameLen> name;
	TInt noOfParam;
	};

struct SStringFixUp
	{
	TInt offset;
	TInt maxLen;
	};

struct SArrayFixUp
	{
	TInt offset;
	TInt maxIndex;
	};

class CProcDef;
class COplRuntime;
class CProcedure;

class CFrame : public CBase
	{
public:
	CFrame();
	void ConstructL(CStack& aStack,COplRuntime& aRuntime,CProcDef* aProcDef,TUint8* aReturnIP=NULL,CFrame* aPrevFrame=NULL,TInt aFlags=0);
	void FinishConstructionL(); // errors here are reported as in this frame
	TAny* VarAddrRel(TInt aOffset);
	TAny* VarAddrInd(TInt aIndex);
	SOplSubProc GetSubProc(TInt aIndex);
	TStackPtr StackPtr() {return iSP;};
	TInt Flags() {return iFlags;};
	~CFrame();
    TUint8* OnErrIP() {return iOnErrIP;}
    void SetOnErrIP(TUint8* aNewIP) {iOnErrIP=aNewIP;}
	CProcDef& ProcDef() {return *iProcDef;}
	CFrame* iPrevFrame;
	TInt MaxStack() {return iMaxStack;}
	TInt EvalNestCount() {return iEvalNestCount;} const
	void IncEvalNestCount() {++iEvalNestCount;}
	void DecEvalNestCount() {--iEvalNestCount;}
private:
	void GetParamsL(CStack& aStack,CProcedure* aProcedure);
	void GetExternsL(CProcedure* aProcedure);
	void DoStringFixUps();
	void DoArrayFixUps();
private:
	COplRuntime* iRuntime;
	TUint8* iFrameCell;
	TUint8* iReturnIP;
	CArrayFixFlat<TAny*>* iIndirectTbl;
	CProcDef* iProcDef;
	TUint8* iOnErrIP;
	TStackPtr iSP;
	TInt iFlags;
	TInt iMaxStack;
	TInt iStartOfIndTbl;
	TInt iEvalNestCount;
	};


class CProcedure : public CBase
	{
public:
	CProcedure();
	~CProcedure();
	void ConstructL(CProcDef* aProcDef,const RFile& aFile);
	TUint8* iQCodeCell;
	TInt iMaxStack;
	//all that is needed for "Hello World"
	TInt iDataSize;
	CArrayFixFlat<TInt>* iParamList;
	CArrayFixFlat<SOplGlobal>* iGlobals;
	CArrayFixFlat<SOplSubProc>* iSubProcs;
	CArrayFixFlat<SOplGlobal>* iExternals;
	CArrayFixFlat<SStringFixUp>* iStringFixUps;
	CArrayFixFlat<SArrayFixUp>* iArrayFixUps;
	void AddGlobalsL(COplRuntime& aRuntime,TUint8* aCell);
	TInt iTotalTableSize;

private:
	TInt ReadWordL(TInt& aPos,const TUint8* aDataPtr);
	TBuf<KMaxGlobalNameLen> ReadStringL(TInt& aPos,const TUint8* aDataPtr);
	void LoadQCodeL(TInt aPos,TInt aLength,const RFile& aFile);
	};

#endif

