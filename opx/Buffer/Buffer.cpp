// Buffer.cpp
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <e32base.h>

#include <oplerr.h>
#include <oplapi.h>
#include <opx.h>

const TInt KUidOpxBuffer=0x10004EC9;
const TInt KOpxBufferVersion=0x600;

//
// The class of the OPX 
//
class COpxBuffer : public COpxBase 
	{
public:
	static COpxBuffer* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
	// the language extension procedures
	enum TExtensions
		{
		EBufferCopy=1,
		EBufferFill,
		EBufferAsString,
		EBufferFromString,
		EBufferMatch,
		EBufferFind,
		EBufferLocate,
		EBufferLocateReverse
		};
	void BufferCopy() const;
	void BufferFill() const;
	void BufferAsString() const;
	void BufferFromString() const;
	void BufferMatch() const;
	void BufferFind() const;
	void BufferLocate() const;
	void BufferLocateReverse() const;
private:
	void ConstructL();
	COpxBuffer(OplAPI& aOplAPI);
	~COpxBuffer();
	};

//
// The language extension procedures provided by this OPX
//
void COpxBuffer::BufferCopy() const
	{
	TInt length=iOplAPI.PopInt32();
	TUint8* source=iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	TUint8* target=iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	Mem::Copy(target, source, length);
	iOplAPI.Push(0.0);
	}

void COpxBuffer::BufferFill() const
	{
	TChar aChar=iOplAPI.PopInt16();
	TInt length=iOplAPI.PopInt32();
	TUint16* buffer=(TUint16*)iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	Mem::Fill(buffer, length, aChar);
	iOplAPI.Push(0.0);
	}

void COpxBuffer::BufferAsString() const
	{
	TInt length=iOplAPI.PopInt32();
	TUint16* buffer=(TUint16*)iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	iOplAPI.PushL(TPtrC(buffer, length));
	}

void COpxBuffer::BufferFromString() const
	{
	TPtrC string=iOplAPI.PopString();
	TInt length=iOplAPI.PopInt32();
	TUint16* buffer=(TUint16*)iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	TPtr theBuffer(buffer, length);
	theBuffer=string;
	iOplAPI.Push(TInt32(theBuffer.Length()));
	}

void COpxBuffer::BufferMatch() const
	{
	TInt foldMode=iOplAPI.PopInt32();
	TPtrC string=iOplAPI.PopString();
	TInt length=iOplAPI.PopInt32();
	TUint16* buffer=(TUint16*)iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	TPtrC theBuffer(buffer, length);
	TInt32 result=0;
	switch (foldMode)
		{
	case 0:
		result=theBuffer.Match(string);
		break;
	case 1:
		result=theBuffer.MatchF(string);
		break;
	case 2:
		result=theBuffer.MatchC(string);
		break;
	default:
		User::Leave(KErrNotSupported);
		}
	iOplAPI.Push(result);
	}

void COpxBuffer::BufferFind() const
	{
	TInt foldMode=iOplAPI.PopInt32();
	TPtrC string=iOplAPI.PopString();
	TInt length=iOplAPI.PopInt32();
	TUint16* buffer=(TUint16*)iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	TPtrC theBuffer(buffer, length);
	TInt32 result=0;
	switch (foldMode)
		{
	case 0:
		result=theBuffer.Find(string);
		break;
	case 1:
		result=theBuffer.FindF(string);
		break;
	case 2:
		result=theBuffer.FindC(string);
		break;
	default:
		User::Leave(KErrNotSupported);
		}
	iOplAPI.Push(result);
	}

void COpxBuffer::BufferLocate() const
	{
	TInt foldMode=iOplAPI.PopInt32();
	TChar aChar=iOplAPI.PopInt16();
	TInt length=iOplAPI.PopInt32();
	TUint16* buffer=(TUint16*)iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	TPtrC theBuffer(buffer, length);
	TInt32 result=foldMode ? theBuffer.LocateF(aChar) : theBuffer.Locate(aChar);
	iOplAPI.Push(result);
	}

void COpxBuffer::BufferLocateReverse() const
	{
	TInt foldMode=iOplAPI.PopInt32();
	TChar aChar=iOplAPI.PopInt16();
	TInt length=iOplAPI.PopInt32();
	TUint16* buffer=(TUint16*)iOplAPI.OffsetToAddrL(iOplAPI.PopInt32(), 4);
	TPtrC theBuffer(buffer, length);
	TInt32 result=foldMode ? theBuffer.LocateReverseF(aChar) :theBuffer.LocateReverse(aChar);
	iOplAPI.Push(result);
	}

//
// The members of COpxBuffer which are not language extension procedures
//
COpxBuffer::COpxBuffer(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

COpxBuffer* COpxBuffer::NewLC(OplAPI& aOplAPI)
	{
	COpxBuffer* This=new(ELeave) COpxBuffer(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	return This;
	}

void COpxBuffer::ConstructL()
	{
	// Do whatever is required to construct component members in COpxBuffer
	}

COpxBuffer::~COpxBuffer()
	{
	// Required so that Tls is set to zero on unloading the OPX in UNLOADM
	Dll::FreeTls();
	}

//
// COpxBase implementation
//
void COpxBuffer::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case EBufferCopy:
		BufferCopy();
		break;
	case EBufferMatch:
		BufferMatch();
		break;
	case EBufferFill:
		BufferFill();
		break;
	case EBufferLocate:
		BufferLocate();
		break;
	case EBufferLocateReverse:
		BufferLocateReverse();
		break;
	case EBufferAsString:
		BufferAsString();
		break;
	case EBufferFromString:
		BufferFromString();
		break;
	case EBufferFind:
		BufferFind();
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxBuffer::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xff00)>(KOpxBufferVersion & 0xff00)) // Major version must be <= OPX's version
		return EFalse; // Bad version
	else
		return ETrue; // OK
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	COpxBuffer* tls=((COpxBuffer*)Dll::Tls());
	// tls is NULL on loading an OPX DLL (also after unloading it)
	if (tls==NULL)
		{
		tls=COpxBuffer::NewLC(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop(); // tls
		}
	return (COpxBase*)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxBufferVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	//
	// DLL entry point
	//
	{
	return(KErrNone);
	}