// SCOMMS.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include "scomms.h"
#include <APGTASK.H>
#include <ES_SOCK.H>
#include <coemain.h>

CSCommsOpx::CSCommsOpx() 
	{
	}

CSCommsOpx::~CSCommsOpx() 
	{
	if (iPort)
		{
		iPort->ClosePort();
		delete iPort;
		}
	delete iIrDA;
	}

void CSCommsOpx::SCConnect(OplAPI& aOplAPI)
	{
	// The OPL call is: handle&=SCConnect&:(svrName$)
	TPtrC portName=aOplAPI.PopString();

	// Check if already connected
	if (iPort)
		User::Leave(KOplErrInvalidIO);

	iPort=new(ELeave) CPortOpx();
	iPort->iStatusBits=0;
	iPort->iPortName=portName; // must be IRCOMM or COMM
	if (iPort->iCommsServer.Connect()!=KErrNone)
		{
		delete iPort;
		iPort=NULL;
		User::Leave(KOplErrNoFileSystem);
		}
	if (iPort->iCommsServer.LoadCommModule(portName)!=KErrNone)
		{
		iPort->iCommsServer.Close();
		delete iPort;
		iPort=NULL;
		User::Leave(KOplErrNoDevices);
		}
	TInt ret=iPort->iIrcomm.Open(iPort->iCommsServer,KIrCOMM0,ECommExclusive);
	if (ret!=KErrNone && ret!=KErrAlreadyExists)
		{
		iPort->iCommsServer.Close();
		delete iPort;
		iPort=NULL;
		User::Leave(KOplErrExists);
		}
	iPort->iStatusBits |= ESCommsOpxPortOpen;

	aOplAPI.Push(TInt32(KDefaultHandle));
	}

void CSCommsOpx::SCWriteA(OplAPI& aOplAPI)
	{
	// The OPL call is: SCWriteA:(handle&,buffer$,timeout&,BYREF statusW&)
#ifdef working
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TTimeIntervalMicroSeconds32 timeout=aOplAPI.PopInt32();
	TPtrC send=aOplAPI.PopString();
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (!iPort)
		User::Leave(KOplErrInvalidIO);
	if (iPort->iStatusBits & ESCommsOpxWritePending)
		User::Leave(KOplErrFilePending);
	iPort->iStatusBits |= ESCommsOpxWritePending;
	iPort->iWriteBuf=send;
	TCallBack callBack(&(CSCommsOpx::SCWriteCallBack),iPort);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,EActivePriorityWsEvents+1,callBack);
	iPort->iIrcomm.Write(tr,timeout,iPort->iWriteBuf);
#else
#pragma message("scomms.cpp: Need to Write() here")
#endif
	aOplAPI.Push(0.0);
	}

TInt CSCommsOpx::SCWriteCallBack(TAny* aPort)
	{
	// Called when a SCWrite completes.
	CPortOpx* Port=(CPortOpx*)aPort;
	Port->iStatusBits &= ~ESCommsOpxWritePending;
	return KErrNone;
	}

void CSCommsOpx::SCReadA(OplAPI& aOplAPI)
	{
	// The OPL call is: SCReadA:(handle&,stringAddr&,length&,timeout&,BYREF statusW&)
#ifdef working
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TTimeIntervalMicroSeconds32 timeout=aOplAPI.PopInt32();
	iPort->iRequestedLength=aOplAPI.PopInt32();
	iPort->iReadString=aOplAPI.OffsetToAddrL(aOplAPI.PopInt32(),256);
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (!iPort)
		User::Leave(KOplErrInvalidIO);
	if (iPort->iStatusBits & ESCommsOpxReadPending)
		User::Leave(KOplErrFilePending);
	iPort->iStatusBits |= ESCommsOpxReadPending;
	TCallBack callBack(&(CSCommsOpx::SCReadCallBack),iPort);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,EActivePriorityWsEvents+1,callBack);

	iPort->iIrcomm.Read(tr,timeout,iPort->iReadBuf,iPort->iRequestedLength);
#else
#pragma message("scomms.cpp: Need to support Read() here.")
#endif
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::SCReadOneOrMoreA(OplAPI& aOplAPI)
	{
	// The OPL call is: SCReadOneOrMoreA:(handle&,stringAddr&,BYREF statusW&)
#ifdef working
	TInt32* statusW=aOplAPI.PopPtrInt32();
	iPort->iReadString=aOplAPI.OffsetToAddrL(aOplAPI.PopInt32(),256);
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (!iPort)
		User::Leave(KOplErrInvalidIO);
	if (iPort->iStatusBits & ESCommsOpxReadPending)
		User::Leave(KOplErrFilePending);
	iPort->iStatusBits|=ESCommsOpxReadPending;
	iPort->iRequestedLength=KOplMaxStringLength;
	TCallBack callBack(&(CSCommsOpx::SCReadCallBack),iPort);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,EActivePriorityWsEvents+1,callBack);
	iPort->iIrcomm.ReadOneOrMore(tr,iPort->iReadBuf);
#else
#pragma message("scomms.cpp: Need to support ReadOneOrMore() here.")
#endif
	aOplAPI.Push(0.0);
	}

TInt CSCommsOpx::SCReadCallBack(TAny* aPort)
	{
	CPortOpx* Port=(CPortOpx*)aPort;
	TInt safeLength=Min(Port->iReadBuf.Length(),Port->iRequestedLength);
	Mem::Copy(Port->iReadString+1,Port->iReadBuf.Ptr(),safeLength);
	*(Port->iReadString)=(TUint8)safeLength;
	Port->iStatusBits &= ~ESCommsOpxReadPending;
	return KErrNone;
	}

void CSCommsOpx::SCQueryReceiveBuffer(OplAPI& aOplAPI)
	{
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (!iPort)
		User::Leave(KOplErrInvalidIO);
	TInt count = iPort->iIrcomm.QueryReceiveBuffer();

	aOplAPI.Push(TInt32(count));
	}

void CSCommsOpx::SCDisconnect(OplAPI& aOplAPI)
	{
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (iPort)
		{
		iPort->iIrcomm.ReadCancel();
		iPort->iIrcomm.WriteCancel();
		iPort->ClosePort();
		delete iPort;
		iPort=NULL;
		}
	
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::SCReadCancel(OplAPI& aOplAPI)
	{
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (!iPort)
		User::Leave(KOplErrInvalidIO);
	iPort->iIrcomm.ReadCancel();
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::SCWriteCancel(OplAPI& aOplAPI)
	{
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (!iPort)
		User::Leave(KOplErrInvalidIO);
	iPort->iIrcomm.WriteCancel();
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::SCSetConfig(OplAPI& aOplAPI)
	{ 
	//Not implemented at present
#if defined(_NOT_YET_IMPLEMENTED_)
	TInt32 endOfLineChar=
#endif
	aOplAPI.PopInt32();
	TInt32 handle=aOplAPI.PopInt32();

	if (handle!=KDefaultHandle)
		User::Leave(KOplErrDevNotExist);
	if (!iPort)
		User::Leave(KOplErrInvalidIO);

#if defined(_NOT_YET_IMPLEMENTED_) 
	TCommConfig config;
	iPort->iIrcomm.Config(config);
	TInt32 currentEOL=((config().iTerminatorCount>0) ? config().iTerminator[0] : 0);
	config().iTerminatorCount=1;
	config().iTerminator[0]=TInt8(endOfLineChar);
	User::LeaveIfError(iPort->iIrcomm.SetConfig(config));
#endif

	TInt32 currentEOL=0;	

	aOplAPI.Push(currentEOL);
	}

void CSCommsOpx::IrDAConnect(OplAPI& aOplAPI)
	{ // synchronous returns error value
	TInt32 port=aOplAPI.PopInt32();
	TPtrC tp=aOplAPI.PopString();
	if (iIrDA==NULL)
		iIrDA=new(ELeave) CIrDAOpx();
	TNameEntry log;
	// K.K	THostName name;
	TBuf<64> name;	// K.K
	User::LeaveIfError(iIrDA->iSocketServ.Connect());
	//tp = _L("IrTinyTP") case sensitive
	User::LeaveIfError(iIrDA->iSocketServ.FindProtocol(tp,iIrDA->iPInfo));
	User::LeaveIfError(iIrDA->iHostResolver.Open(iIrDA->iSocketServ,iIrDA->iPInfo.iAddrFamily,iIrDA->iPInfo.iProtocol));
	User::LeaveIfError(iIrDA->iHostResolver.GetByName(name,log));
	User::LeaveIfError(iIrDA->iSocket.Open(iIrDA->iSocketServ,iIrDA->iPInfo.iAddrFamily,iIrDA->iPInfo.iSockType,iIrDA->iPInfo.iProtocol));
	log().iAddr.SetPort(port);
	TRequestStatus stat;
	iIrDA->iSocket.Connect(log().iAddr,stat);
	aOplAPI.WaitForEvent(stat,EActivePriorityWsEvents+1);
	iIrDA->iSendMode=ETrue;
	aOplAPI.Push((TInt32)stat.Int());
	}

void CSCommsOpx::IrDAWaitForConnect(OplAPI& aOplAPI)
	{ // is asynchronous 
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TInt32 port=aOplAPI.PopInt32();
	TPtrC tp=aOplAPI.PopString();
	if (iIrDA==NULL)
		iIrDA=new(ELeave) CIrDAOpx();
	TSockAddr addr;
	User::LeaveIfError(iIrDA->iSocketServ.Connect());
	User::LeaveIfError(iIrDA->iSocketServ.FindProtocol(tp,iIrDA->iPInfo));
	User::LeaveIfError(iIrDA->iAcceptor.Open(iIrDA->iSocketServ,iIrDA->iPInfo.iAddrFamily,iIrDA->iPInfo.iSockType,iIrDA->iPInfo.iProtocol));
	addr.SetPort(port);
	iIrDA->iAcceptor.Bind(addr);
	iIrDA->iAcceptor.Listen(1);
	iIrDA->iSocket.Open(iIrDA->iSocketServ); // Create a null socket to hold the connection
	TCallBack callBack(NULL);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,EActivePriorityWsEvents+1,callBack);
	iIrDA->iAcceptor.Accept(iIrDA->iSocket,tr);
	iIrDA->iSendMode=EFalse;
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::IrDAWrite(OplAPI& aOplAPI) const
	{ // ScIrDAWrite:(aPtr&, aLen&, BYREF aGlobalStatusW&)
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TInt32 length=aOplAPI.PopInt32();
	TInt32 ptr=aOplAPI.PopInt32(); // Ptr to 8-bit buffer.
	TPtrC8 send((const unsigned char*)ptr,length);
	if (!iIrDA)
		User::Leave(KOplErrInvalidIO);
	TCallBack callBack(NULL);
	TRequestStatus * tr=&(aOplAPI.NewRequestL(statusW,EActivePriorityWsEvents+1,callBack));
	if (send.Length())
		iIrDA->iSocket.Write(send,*tr); // Reading from accepted socket.
	else 
		User::RequestComplete(tr,KErrNone);
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::IrDARead(OplAPI& aOplAPI) const
	{ // ScIrDARead&:(aPtr&, BYREF aLen&)
	if (!iIrDA)
		User::Leave(KOplErrInvalidIO);
	TRequestStatus tr;
	iIrDA->iSocket.Read(iIrDA->iBuf,tr); // Reading from accepted socket.
	aOplAPI.WaitForEvent(tr,EActivePriorityWsEvents+1);
	User::LeaveIfError(tr.Int());
#pragma message("scomms.cpp: IrDaRead should store data in user buffer")
	// iIrDA->iBuf
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::IrDAReadA(OplAPI& aOplAPI) const
	{
	TInt32* statusW=aOplAPI.PopPtrInt32();
	iIrDA->iAddr=aOplAPI.OffsetToAddrL(aOplAPI.PopInt32(),256);

	if (!iIrDA)
		User::Leave(KOplErrInvalidIO);

	TCallBack callBack(&(CSCommsOpx::IrDAReadCallBack),iIrDA);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,EActivePriorityWsEvents+1,callBack);
	iIrDA->iSocket.Read(iIrDA->iBuf,tr); // Reading from accepted socket.
	aOplAPI.Push(0.0);
	}

TInt CSCommsOpx::IrDAReadCallBack(TAny* aIrDA)
	{
	CIrDAOpx* irDA=(CIrDAOpx*)aIrDA;
	Mem::Copy(irDA->iAddr+1,irDA->iBuf.Ptr(),irDA->iBuf.Length());
	*(irDA->iAddr)=(TUint8)irDA->iBuf.Length();
	return KErrNone;
	}

void CSCommsOpx::IrDAWaitForDisconnect(OplAPI& aOplAPI)
	{
	if (iIrDA!=NULL)
		{
		TRequestStatus tr;
		iIrDA->iSocket.Ioctl(KDiscoveryIndicationIoctl,tr,&iIrDA->iBuf,iIrDA->iPInfo.iAddrFamily);
		aOplAPI.WaitForEvent(tr,EActivePriorityWsEvents+1);
		delete iIrDA;
		iIrDA=NULL;
		}	
	aOplAPI.Push(0.0);
	}

void CSCommsOpx::IrDADisconnect(OplAPI& aOplAPI)
	{
	delete iIrDA;
	iIrDA=NULL;
	aOplAPI.Push(0.0);
	}

CIrDAOpx::~CIrDAOpx()
	{
	// When we try to Open() these we Leave if there's an error, therefore if we get here
	// they must be Open OK, hence we don't need to do if (iHostResolver)...
	iHostResolver.Close();
	iSocket.Close();
	iAcceptor.Close();
	iSocketServ.Close();
	}

CPortOpx::~CPortOpx()
	{
	ClosePort();
	}

void CPortOpx::ClosePort()
	{
	if (iStatusBits & ESCommsOpxPortOpen)
		{
		iIrcomm.Close();
		iCommsServer.Close();
		iStatusBits &= ~ESCommsOpxPortOpen;
		}
	}

CTlsDataOPXSComms::CTlsDataOPXSComms(OplAPI& aOplAPI) : COpxBase(aOplAPI)
	{
	}

CTlsDataOPXSComms* CTlsDataOPXSComms::NewL(OplAPI& aOplAPI)
	{
	CTlsDataOPXSComms* This=new(ELeave) CTlsDataOPXSComms(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	CleanupStack::Pop();
	return This;
	}

void CTlsDataOPXSComms::ConstructL()
	{
	iSCommsHandle= new(ELeave) CSCommsOpx;
	}

CTlsDataOPXSComms::~CTlsDataOPXSComms()
	{
	delete iSCommsHandle;
	Dll::FreeTls();
	}

void CTlsDataOPXSComms::RunL(TInt aProcNum)
// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case ESCConnect:
	 iSCommsHandle->SCConnect(iOplAPI);
	 break;
	case ESCSetConfig:
		 iSCommsHandle->SCSetConfig(iOplAPI);
		 break;
	case ESCWriteA:
		 iSCommsHandle->SCWriteA(iOplAPI);
		 break;
	case ESCReadA:
		 iSCommsHandle->SCReadA(iOplAPI);
		 break;
	case ESCReadOneOrMoreA:
		 iSCommsHandle->SCReadOneOrMoreA(iOplAPI);
		 break;
	case ESCQueryReceiveBuffer:
		 iSCommsHandle->SCQueryReceiveBuffer(iOplAPI);
		 break;
	case ESCDisconnect:
		 iSCommsHandle->SCDisconnect(iOplAPI);
		 break;
	case ESCReadCancel:
		 iSCommsHandle->SCReadCancel(iOplAPI);
		 break;
	case ESCWriteCancel:
		 iSCommsHandle->SCWriteCancel(iOplAPI);
		 break;
	case EIrDAConnect:
		iSCommsHandle->IrDAConnect(iOplAPI);
		break;
	case EIrDAWaitForConnect:
		iSCommsHandle->IrDAWaitForConnect(iOplAPI);
		break;
	case EIrDAWrite:
		iSCommsHandle->IrDAWrite(iOplAPI);
		break;
	case EIrDARead:
		iSCommsHandle->IrDARead(iOplAPI);
		break;
	case EIrDAReadA:
		iSCommsHandle->IrDAReadA(iOplAPI);
		break;
	case EIrDAWaitForDisconnect:
		iSCommsHandle->IrDAWaitForDisconnect(iOplAPI);
		break;
	case EIrDADisconnect:
		iSCommsHandle->IrDADisconnect(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool CTlsDataOPXSComms::CheckVersion( TInt aVersion )
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
// Creates a COpxBase instance as required by the OPL runtime
// This object is to be stored in the OPX's TLS as shown below
	{
	CTlsDataOPXSComms* tls=((CTlsDataOPXSComms*)Dll::Tls());
	if (tls==NULL)
		{
	tls=CTlsDataOPXSComms::NewL(aOplAPI);
		Dll::SetTls(tls);
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