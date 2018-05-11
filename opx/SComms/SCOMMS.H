// SCOMMS.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#ifndef __SComms_H__
#define __SComms_H__

#include <e32base.h>
#include <oplapi.h>
#include <opxapi.h>
#include <e32base.h>
#include <c32comm.h>
#include <oplerr.h>
#include <ES_SOCK.H>
#include <IR_SOCK.H>

// This version number also needs changing in scomms.pkg and scomms.txh
#define KOpxVersion 0x600

const TInt KUidOpxSerialComms=0x100038F2;

const TInt KOplMaxStringLength=255;
const TInt32 KDefaultHandle=1;

_LIT(KIrCOMM0,"IRCOMM::0");
	
class CTlsDataOPXSComms;
class CSCommsOpx;

enum
	{
	ESCommsOpxPortOpen = 0x01,
	ESCommsOpxReadPending = 0x02,
	ESCommsOpxWritePending = 0x04
	};

class CPortOpx : public CBase
	{
public:
	~CPortOpx();
	void ClosePort();
	TBuf<KOplMaxStringLength> iReadBuf;
	TBuf<KOplMaxStringLength> iWriteBuf;
	TBuf<KOplMaxStringLength> iPortName;
	TUint8* iReadString;
	TUint8* iWriteString;
	RCommServ iCommsServer;
	RComm iIrcomm;
	TInt32 iStatusBits;
	TInt iRequestedLength;
	};

class CIrDAOpx : public CBase
	{
public:
	~CIrDAOpx();
	RSocketServ iSocketServ;
	RHostResolver iHostResolver;
	RSocket iSocket;
	RSocket iAcceptor;
	TBuf8<KOplMaxStringLength> iBuf; //Have to use 8bit for comms.
	TBool iSendMode;
	TProtocolDesc iPInfo;
	TUint8* iAddr;
	};

class CSCommsOpx :public CBase
	{
public:
	CSCommsOpx();
	~CSCommsOpx();
   
	void SCConnect(OplAPI& aOplAPI);
	void SCWriteA(OplAPI& aOplAPI);
	void SCReadA(OplAPI& aOplAPI);
	void SCReadOneOrMoreA(OplAPI& aOplAPI);
	void SCQueryReceiveBuffer(OplAPI& aOplAPI);
	void SCDisconnect(OplAPI& aOplAPI);
	void SCReadCancel(OplAPI& aOplAPI);
	void SCWriteCancel(OplAPI& aOplAPI);
	void SCSetConfig(OplAPI& aOplAPI);
// Ir comms
	void IrDAConnect(OplAPI& aOplAPI);
	void IrDAWaitForConnect(OplAPI& aOplAPI);
	void IrDAWrite(OplAPI& aOplAPI) const;
	void IrDARead(OplAPI& aOplAPI) const;
	void IrDAReadA(OplAPI& aOplAPI) const;
	static TInt IrDAReadCallBack(TAny* aIrDA);
	void IrDAWaitForDisconnect(OplAPI& aOplAPI);
	void IrDADisconnect(OplAPI& aOplAPI);

private:
	static TInt SCReadCallBack(TAny* aPort);
	static TInt SCWriteCallBack(TAny* aPort);
	CPortOpx* iPort;
	CIrDAOpx* iIrDA;
	};

class CTlsDataOPXSComms : public COpxBase 
	{
public:
	static CTlsDataOPXSComms* NewL(OplAPI& aOplAPI);
	void ConstructL();
	CTlsDataOPXSComms(OplAPI& aOplAPI);
	~CTlsDataOPXSComms() ;
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
	CSCommsOpx* iSCommsHandle;
private:
	// the language extension procedures
	enum TExtensions
		{
		ESCConnect=1,
		ESCSetConfig,
		ESCWriteA,
		ESCReadA,
		ESCReadOneOrMoreA,
		ESCQueryReceiveBuffer,
		ESCDisconnect,
		ESCReadCancel,
		ESCWriteCancel,
		EIrDAConnect,
		EIrDAWaitForConnect,
		EIrDAWrite,
		EIrDARead,
		EIrDAReadA,
		EIrDAWaitForDisconnect,
		EIrDADisconnect,
		};
	};

inline CTlsDataOPXSComms* TheTls() { return((CTlsDataOPXSComms *)Dll::Tls()); }
inline void SetTheTls(CTlsDataOPXSComms *theTls) { Dll::SetTls(theTls); }

#endif