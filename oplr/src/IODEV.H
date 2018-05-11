// IODEV.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef IODEV_H
#define IODEV_H

#include <oplr.h>
#include <c32comm.h>

class COplIOTimer : public COplIO
	{
public:
	COplIOTimer(TInt16 aHandle);
	virtual void ConstructL(TBuf<256>& /*aDevName*/, TInt16 /*aMode*/);
	virtual void RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus, TOplReqStatus* aStatusPtr, TAny* aParam1, TAny* aParam2);
	~COplIOTimer();
private:
	RTimer iTimer;
	};

class ROplComm : public RComm
	{
public:
	inline TBool IsOpen();
	};

class COplIOComm : public COplIO
	{
public:
	COplIOComm(TInt16 aHandle);
	virtual void ConstructL(TBuf<256>& aDevName,TInt16 aMode);
	virtual void RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus,TOplReqStatus* aStatusPtr, TAny* aParam1, TAny* aParam2);
	~COplIOComm();
private:
	TUint8 Handshake(TUint aOpler1Handshake);
	TUint Handshake(TUint8 aOpl1993Handshake);
	TInt Terminators(TUint32 aMask,TText8* charPtr);
	RCommServ iSession;
	ROplComm iPort;
	};

class COplIOFile : public COplIO
	{
public:
	COplIOFile(TInt16 aHandle);
	virtual void ConstructL(TBuf<256>& aDevName,TInt16 aMode);
	virtual void RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus,TOplReqStatus* aStatusPtr, TAny* aParam1, TAny* aParam2);
	virtual void DoOpenFileL(RFs& aRFs,TBuf<256>& aDevName,TUint aMode);
	~COplIOFile();
protected:
	TUint GetModeL();
	RFile iFile;
	TInt16 iMode;
	TBool iUnicodeTextFile;
	HBufC8* iBufferToWrite;
	};

class COplIOFileText : public COplIOFile
	{
public:
	COplIOFileText(TInt16 aHandle);
	virtual void ConstructL(TBuf<256>& aDevName,TInt16 aMode);
	virtual void RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus,TOplReqStatus* aStatusPtr, TAny* aParam1, TAny* aParam2);
private:
	TFileText iTextFile;
	};

class TOplIOPtrRequest : public TOplIORequest
	{
public:
	TOplIOPtrRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TAny* aParam1,TAny* aParam2);
	TPtr8 iPtr8;
	};

class TOplIOReadRequest : public TOplIOPtrRequest
	{
public:
	TOplIOReadRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TAny* aParam1,TAny* aParam2);
protected:
	virtual void DoParamsUpdate();
	TUint16* iLenPtr;
	};

class TOplIOTextReadRequest : public TOplIOReadRequest
	{
public:
	TOplIOTextReadRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TAny* aParam1,TAny* aParam2);
protected:
	virtual void DoParamsUpdate();
	};

class TOplIOCallBackRequest : public TOplIORequest
	{
public:
	TOplIOCallBackRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TCallBack aCallBack);
protected:
	virtual void DoParamsUpdate();
	TCallBack iCallBack;
	};

class TEraIOCallBackRequest : public TIORequest
	{
public:
	TEraIOCallBackRequest(TInt32* aStatusPtr,TInt aPriority,TCallBack aCallBack);
private:
	virtual void DoHandleCompletion();
protected:
	TCallBack iCallBack;
	};

//
// inlines
//

//
// Class ROplComm
//
inline TBool ROplComm::IsOpen()
	{
	return SubSessionHandle()!=0;
	}

#endif