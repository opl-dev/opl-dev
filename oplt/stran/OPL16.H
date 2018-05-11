// OPL16.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// COpl16ModuleBackEnd - OPL16 backends
//


#ifndef __OPL16_H__
#define __OPL16_H__

#include <f32file.h>
// NOTE: A Back end should be possible only using the published headers
#include <opllex.h>
#include <opltran.h>
#include <oplbacke.h>
#include <s32file.h>

// Opl related UIDs
const TUid KUidOplObj=			{0x100055C0};
const TUid KUidOplApp=			{0x100055C1};
const TUid KUidOplDoc=			{0x100055C2}; //not used here.

const TUid KUidDebuggerObj= {0x10000124}; //268435748
const TUid KUidOplDebugger= {0x10000125}; //268435749

const TUid KUidOplInterpreter=	{0x10005D2E};

// Various version numbers 
const TUint KOpl16Opl1993TranVer=0x112f;
const TUint KOpl16Opl1993RuntimeVer=0x111f;
//const TUint KOpl16P1TranVer=0x200a;
//const TUint KOpl16P1RuntimeVer=0x200a;
const TUint KOpl16CrystalTranVer=0x3000;
const TUint KOpl16CrystalRuntimeVer=0x3000;

////////////////////////////////////////////////////////////////////////
//
// ROpoWriteStream
//
////////////////////////////////////////////////////////////////////////
class ROpoWriteStream : public RBufWriteStream
	{
public:
	inline TStreamPos AddPatch16L() { return AddPatchL(EFalse);}
	inline void Patch16L(TStreamPos aPos, TUint aValue) { PatchL(aPos, aValue, EFalse);}
	inline TStreamPos AddPatch32L() { return AddPatchL(ETrue);}
	inline void Patch32L(TStreamPos aPos, TUint aValue) { PatchL(aPos, aValue, ETrue);}
	void PatchCheckedUidL(TCheckedUid& aCheckedUid); // FOR NOW - while we still have the old file format madness
	TInt SizeL(); // The size of the stream so far
private:
	TStreamPos AddPatchL(TBool a32BitPatch);
	void PatchL(TStreamPos aPos, TUint aValue, TBool a32BitPatch);
	};

//////////////////////////////////////////////////////////////////////
//
// TOpl16ProcEntry
//
//////////////////////////////////////////////////////////////////////
class TOpl16ProcEntry
	{
	friend class COpl16ModuleBackEnd;
private:
	TOpl16ProcEntry(const TDesC& aName,TInt aLine, TInt aPosition);
	TBuf<KOplMaxIdentifierLength> iName;
	TInt iLineNumber;
	TInt iModulePos;
	};

/////////////////////////////////////////////////////////////////////
//
// COpl16BackEnd - 16 bit module Back End
//
////////////////////////////////////////////////////////////////////
class COpl16PcodeConverter;
class COpl16App;
const TInt KOpl16QcodeBufferGran=0x80;
const TInt KOpl16ProcsListGran=8;
const TInt KOplNoErrorLocation=KMaxTInt;

class COpl16ModuleBackEnd : public COplModuleBackEnd
	{
public:
	virtual void StartL(const TDesC& aSourceName,CTextTranslator::TDebugFlag aDebugFlag, TSourceTranslateError& anError);
	virtual COplParseApp* GetAppLC(TOplTranTarget aTarget,COplLexerBase& aLexer);
	virtual void AppL(COplParseApp* anApp);	// App Declaration
	virtual void StartProcL(const TDesC& aName, TInt aLine, TInt anErrorOffset);
	virtual void ProcL(COplProcHeader* aHeader, CBufSeg* aCodeBlock);
	virtual void EndOfFileL();
	inline const TFileName& OutputName() {return iOutputName;}
	// From COplModuleBackEnd
	// virtual void Reset();
	void OplTargetIsApp();
protected:
	void EnsurePathL();
	COpl16ModuleBackEnd(const TDesC& aTargetFile, MTranAppCB* anAppCB);
	void ConstructL();
	~COpl16ModuleBackEnd();
	virtual void DebugSymbolsL(COplProcHeader *aHeader);

	virtual void DoStartL(const TDesC& aSourceName)=0;
	virtual void DoAppL(COpl16App* anApp)=0;
	virtual void FirstProcL()=0;
	virtual TInt DoStartProcL()=0;
	virtual void DoProcL()=0;
	virtual RWriteStream& StartProcTableL()=0;
	virtual void DoEndOfFileL()=0;

private:
	enum TState
		{
		EReset,
		EStarted,
		EAppSeen,
		EAwaitingBody,
		EProcSeen
		};
	void ProcessProcHeaderL(COplProcHeader *aHeader,TStreamPos& aQcodeSizePos,TStreamPos& aStackDepthPos);
protected:	
	// State & stuff
	TState iState;
	TSourceTranslateError* iError;
	CTextTranslator::TDebugFlag iDebugFlag;

	// Target files
	RFs iFs;			// File server connection
	MTranAppCB* iAppCB;
	TFileName iOutputName;

	// Some internal buffering - unnecessary in the full streaming case
	// but a bit of a hang-over from OPL1993 file formats
	CBufSeg* iQcodeBuffer;
	ROpoWriteStream iOpo;

	CArrayFixFlat<TOpl16ProcEntry>* iProcs;
	COpl16PcodeConverter* iConverter;
	};


class COpl16Opl1993BackEnd : public COpl16ModuleBackEnd
	{
public:
	static COpl16Opl1993BackEnd *NewL(const TDesC& aTargetName, MTranAppCB* anAppCB);
	virtual void OpxL(COplParseOpx* anOpx);  // Opx Declaration
	TInt OpxCount();
protected:
	void DoStartL(const TDesC& aSourceName);
	void DoAppL(COpl16App* anApp);
	void FirstProcL();
	TInt DoStartProcL();
	void DoProcL();
	RWriteStream& StartProcTableL();
	void DoEndOfFileL();
private:
	COpl16Opl1993BackEnd(const TDesC& aTargetFile, MTranAppCB* anAppCB);
	void TidyFile(RFile& aFile);
	friend void FileCleanup(void *aFileBackEnd);
private:
	TStreamPos iDataPatchPos;		// Where to patch the offset to 'real' header
	TStreamPos iTotalSizePatchPos; // Total size of the stream
	TStreamPos iProcTablePatchPos; // Offset of teh Procedure lokup table in the stream
	};

const TInt KOpl16OpxListGran=8;
const TInt KOpl16ProcSymbolsGran=8;
class COpl16Opler1BackEnd : public COpl16ModuleBackEnd
	{
public:
	~COpl16Opler1BackEnd();
	static COpl16Opler1BackEnd *NewL(const TDesC& aTargetName, MTranAppCB* anAppCB);
	virtual void OpxL(COplParseOpx* anOpx);  // Opx Declaration
	TInt OpxCount();
	inline const TDesC* SourceName() {return iSourceName;}
protected:
	void DoStartL(const TDesC& aSourceName);
	void DoAppL(COpl16App* anApp);
	void FirstProcL();
	TInt DoStartProcL();
	void DoProcL();
	RWriteStream& StartProcTableL();
	void DoEndOfFileL();
	void DebugSymbolsL(COplProcHeader *aHeader);
private:
	COpl16Opler1BackEnd(const TDesC& aTargetFile, MTranAppCB* anAppCB);
	TStreamId NewStreamL();
	void DeleteOpxs();
private:
	// OPO Store
	TUidType iUid;
	CFileStore *iStore;
	RStoreWriteStream iStream;

	// Source info
	TStreamId iSourceId;
	HBufC *iSourceName;
	
	// Tables
	TStreamId iProcTableId;
	CArrayFixFlat<COplParseOpx*> iOpxList;

	// Stuff for putting out the debugger symbol table
	HBufC* iSymbolFileName;					     // Symbol Table file name
	CFileStore *iSymbolStore;					 // Symbol table file
	CArrayFixFlat<TStreamId> iProcSymbolStreams; // Stream Ids of the procedure tables
	};

class COpl16EvalBackEnd
	{
public:
	static COpl16EvalBackEnd *NewL();
	void SetBuffers(TTranslateError& anError,CBufBase& aQcodeBuffer,TInt& aMaxStackDepth);
	void ProcessExpressionL(CBufBase& aPcodeBuffer);
private:
	TTranslateError* iError;
	CBufBase* iQcodeBuffer;
	TInt* iMaxStackDepth;
	};


#endif
