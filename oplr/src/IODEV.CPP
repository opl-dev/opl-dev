// IODEV.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include "iodev.h"
#include "iodef.h"
#include "oplutil.h"
#include "serial.h"
#include "module.h"

/* 
 * Arm THUMB requires alignment on the 16bit boundary. 
 */
#if defined(__CW32__)
#define __LEAVE_IF_UNALIGNED(aPtr,aLeaveError)\
	if((TUint32)(aPtr)&1)\
		User::Leave((aLeaveError));
#else // Not CodeWarrior
#define __LEAVE_IF_UNALIGNED(aPtr,aLeaveError)\
	if((TUint16)(aPtr)&1)\
		User::Leave((aLeaveError));
#endif

_LIT(KOplrIODEV,"OplrIODEV");

//
// Class COplIOTimer
//
COplIOTimer::COplIOTimer(TInt16 aHandle)
	:COplIO(aHandle)
	{
	}

void COplIOTimer::ConstructL(TBuf<256>& /*aDevName*/, TInt16 /*aMode*/)
	{
	User::LeaveIfError(iTimer.CreateLocal());
	}

COplIOTimer::~COplIOTimer()
	{
	iTimer.Cancel(); // eb205: is any consumption of signals necessary here?
	iTimer.Close();
	}

void COplIOTimer::RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus,TOplReqStatus* aOplStatusPtr,TAny* aParam1,TAny* /*aParam2*/)
	// eb205: I've replaced calls to "User::Leave(KOplErrInvalidIO)" with calls
	// to "User::RequestComplete(statusPtr,KErrArgument)"
	{
	aIOStatus=new(ELeave) TOplIORequest(aOplStatusPtr,EActivePriorityWsEvents+3);
	TRequestStatus* statusPtr=&aIOStatus->Status();
	switch (aFuncNo)
		{
		case (FRELATIVE):
			{
			TInt32 time=OplUtil::GetLong(aParam1); // maybe should check range
			iTimer.After(*statusPtr,TTimeIntervalMicroSeconds32(time*100000));
			break;
			}
		case (FABSOLUTE):
			{
			TDateTime date(1970,TMonth(0),0,0,0,0,0);
			TTime time(date);
			TTimeIntervalSeconds secs=(TUint32)OplUtil::GetLong(aParam1);
			iTimer.At(*statusPtr,(time+secs));
			break;
			}
		case (FCLOSE):
			{
			(TheRuntime()->IOCollection()).RemoveObject(iHandle);
			User::RequestComplete(statusPtr,KErrNone);
			break;
			}
		case (FCANCEL):
			{
			iTimer.Cancel();
			User::RequestComplete(statusPtr,KErrNone);
			break;
			}
		default:
			{
			User::RequestComplete(statusPtr,KErrArgument);
			return; // return here as there's been an error
			}
		}
	}

//
// Class COplIOComm
//
_LIT(KCommPortName,"COMM::");
_LIT(KUartCsyName,"ECUART");

COplIOComm::COplIOComm(TInt16 aHandle)
	:COplIO(aHandle)
	{
	}

void COplIOComm::ConstructL(TBuf<256>& aDevName,TInt16 aMode)
	{
	if (aDevName.Length()==5)
		{
		TChar port=aDevName[4];
		port.UpperCase();
		if (port.IsAlpha())
			{
			port-='A'-'0'; // TTY:A is COMM0, TTY:B is COMM1 etc.
			User::LeaveIfError(iSession.Connect());
			User::LeaveIfError(iSession.LoadCommModule(KUartCsyName));
			TBuf<16> commPort=KCommPortName();
			commPort.Append(port);
			User::LeaveIfError(iPort.Open(iSession,commPort,ECommExclusive));
			if (aMode==-1) // then try to set OPL1993 defaults
				{
				TCommConfig cBuf;
				TCommConfigV01 &c=cBuf();
				iPort.Config(cBuf);
				c.iRate=EBps9600;
				c.iDataBits=EData8;
				c.iStopBits=EStop1;
				c.iParity=EParityNone;
				c.iHandshake=KConfigObeyCTS;
				c.iParityError=KConfigParityErrorFail;
				c.iFifo=EFifoDisable;
				c.iTerminatorCount=0;
				c.iXonChar=0x11;
				c.iXoffChar=0x13;
				c.iSIREnable=ESIRDisable;
				User::LeaveIfError(iPort.SetConfig(cBuf));
				}
			return;
			}
		}
	User::Leave(KOplErrInvalidIO);
	}

void COplIOComm::RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus,TOplReqStatus* aOplStatusPtr, TAny* aParam1, TAny* aParam2)
	// eb205: I've replaced calls to "User::Leave(KOplErrInvalidIO)" with calls
	// to "User::RequestComplete(statusPtr,KErrArgument)"
	{
	if (aFuncNo==FREAD)
		{
		__LEAVE_IF_UNALIGNED(aParam1,KOplErrBadAlignment); // No statusPtr to RequestComplete against, at this point.
		aIOStatus=new(ELeave) TOplIOReadRequest(aOplStatusPtr,EActivePriorityWsEvents+5,(TUint8*)aParam1,aParam2); 
		iPort.Read(aIOStatus->Status(),((TOplIOReadRequest*)aIOStatus)->iPtr8);
		}
	else if (aFuncNo==FWRITE)
		{
		__LEAVE_IF_UNALIGNED(aParam1,KOplErrBadAlignment); // No statusPtr to RequestComplete against, at this point.
		aIOStatus=new(ELeave) TOplIOPtrRequest(aOplStatusPtr,EActivePriorityWsEvents+5,(TUint8*)aParam1,aParam2);
		iPort.Write(aIOStatus->Status(),((TOplIOPtrRequest*)aIOStatus)->iPtr8);
		}
	else
		{
		aIOStatus=new(ELeave) TOplIORequest(aOplStatusPtr,EActivePriorityWsEvents+5);
		TRequestStatus* statusPtr=&aIOStatus->Status();
		switch (aFuncNo)
			{
			case (FCLOSE):
				(TheRuntime()->IOCollection()).RemoveObject(iHandle);
				User::RequestComplete(statusPtr,KErrNone);
				break;
			case (FCANCEL):
				iPort.ReadCancel();
				iPort.WriteCancel();
				iPort.Cancel();
				User::RequestComplete(statusPtr,KErrNone);
				break;
			case (FFLUSH):
				iPort.ResetBuffers();
				User::RequestComplete(statusPtr,KErrNone); // eb205: I've added this
				break;
			case (FSET):
				{
				TCommConfig configBuf;
				TCommConfigV01& config=configBuf();
				iPort.Config(configBuf);
				P_SRCHAR& newConfig=*(P_SRCHAR*)aParam1;
				config.iRate=(TBps)(Max(newConfig.tbaud,newConfig.rbaud)-1);
				config.iDataBits=(TDataBits)(P_DATA_MASK&newConfig.frame);
				config.iStopBits=(newConfig.frame&P_TWOSTOP)?EStop2:EStop1;
				config.iParity=(newConfig.frame&P_PARITY)?((TParity)newConfig.parity):EParityNone;
				config.iHandshake=Handshake(newConfig.hand);
				config.iParityError=(newConfig.flags&P_IGNORE_PARITY)?KConfigParityErrorIgnore:KConfigParityErrorFail;
				config.iXonChar=newConfig.xon;
				config.iXoffChar=newConfig.xoff;
				const TInt returnValue=Terminators(OplUtil::GetLong(&newConfig.tmask),config.iTerminator);
				if (returnValue<0)
					{
					User::RequestComplete(statusPtr,returnValue);
					return; // return here as there's been an error
					}
				config.iTerminatorCount=returnValue;
				User::RequestComplete(statusPtr,iPort.SetConfig(configBuf));
				break;
				}
			case (FSENSE):
				{
				TCommConfig configBuf;
				TCommConfigV01& config=configBuf();
				iPort.Config(configBuf);
				P_SRCHAR& out=*(P_SRCHAR*)aParam1;
				out.tbaud=out.rbaud=(TUint8)((config.iRate==EBpsSpecial)?0:(config.iRate+1)); // EBpsSpecial returned as zero
				out.frame=(TUint8)(config.iDataBits|((config.iStopBits==EStop2)?P_TWOSTOP:0)|((config.iParity==0)?0:P_PARITY));
				out.parity=(TUint8)config.iParity;
				out.hand=Handshake(config.iHandshake);
				out.xon=config.iXonChar;
				out.xoff=config.iXoffChar;
				out.flags=(TUint8)((config.iParityError==KConfigParityErrorIgnore)?P_IGNORE_PARITY:0);
				TUint32 terminatorMask=0;
				for (TInt i=0; i<config.iTerminatorCount; ++i)
					{
					TInt pos=config.iTerminator[i];
					if (pos<0 || pos>31)
						{
						User::RequestComplete(statusPtr,KErrGeneral); // eb205: this was User::Leave(KOplErrInvalidIO)
						return; // return here as there's been an error
						}
					terminatorMask|=(0x1<<pos);
					}
				OplUtil::PutLong(&out.tmask,terminatorMask);
				User::RequestComplete(statusPtr,KErrNone);
				break;
				}
			case (FTEST):
				OplUtil::PutWord((TUint16*)aParam1,(TUint16)iPort.QueryReceiveBuffer());
				User::RequestComplete(statusPtr,KErrNone);
				break;
			case (FINQ):
				{
				TCommCaps capsBuf;
				TCommCapsV01& caps=capsBuf();
				iPort.Caps(capsBuf);
				OplUtil::PutLong(aParam1,(TInt32)caps.iRate);
				TUint flags=caps.iDataBits;
				if (caps.iStopBits&KCapsStop2)
					flags|=P_SRINQ_STOP2;
				flags|=((caps.iParity&~KCapsParityNone)<<4);
				if (caps.iSignals&KCapsSignalDTRSupported)
					flags|=P_SRINQ_SETDTR;
				if ((caps.iHandshake&KCapsObeyXoffSupported) && (caps.iHandshake&KCapsSendXoffSupported))
					flags|=P_SRINQ_XONXOFF;
				OplUtil::PutWord((TUint16*)aParam1+2,(TUint16)flags);
				User::RequestComplete(statusPtr,KErrNone);
				break;
				}
			case (FCTRL):
				{
				TUint signals=iPort.Signals(KSignalCTS|KSignalDSR|KSignalDCD);
				*(TUint8*)aParam1=(TUint8)signals;
				TUint8 setDTR=*((TUint8*)aParam1+1);
				if (setDTR==1)
					iPort.SetSignalsToMark(KSignalDTR);
				else if (setDTR==2)
					iPort.SetSignalsToSpace(KSignalDTR);
				User::RequestComplete(statusPtr,KErrNone);
				break;
				}
			case (FXSET):
				{
				TCommConfig cBuf;
				Mem::Copy(&cBuf(),aParam1,sizeof(TCommConfigV01));
				User::RequestComplete(statusPtr,iPort.SetConfig(cBuf));
				break;
				}
			case (FXSENSE):
				{
				TCommConfig cBuf;
				iPort.Config(cBuf);
				Mem::Copy(aParam1,&cBuf(),sizeof(TCommConfigV01));
				User::RequestComplete(statusPtr,KErrNone);
				break;
				}
			case (FXINQ):
				{
				TCommCaps caps;
				iPort.Caps(caps);
				Mem::Copy(aParam1,&caps(),sizeof(TCommCapsV01));
				User::RequestComplete(statusPtr,KErrNone);
				break;
				}
			case (FXCTRL):
				{
				OplUtil::PutWord((TUint16*)aParam1,(TUint16)iPort.Signals(KSignalCTS|KSignalDSR|KSignalDCD|KSignalRNG|KSignalRTS|KSignalDTR));
				TUint set=OplUtil::GetWord((TUint16*)aParam1+1);
				TUint clear=OplUtil::GetWord((TUint16*)aParam1+2);
				iPort.SetSignalsToMark(set&~clear);
				iPort.SetSignalsToSpace(clear&~set);
				User::RequestComplete(statusPtr,KErrNone);
				break;
				}
			default:
				User::RequestComplete(statusPtr,KErrArgument);
				return; // return here as there's been an error
			}
		}
	}

TUint COplIOComm::Handshake(TUint8 aOpl1993Handshake)
	{
	TUint opler1Handshake;
	if (aOpl1993Handshake&P_IGN_CTS)
		opler1Handshake=0;
	else
		opler1Handshake=KConfigObeyCTS;
	opler1Handshake|=(aOpl1993Handshake&0x3); // Obey/Send Xoff
	return opler1Handshake|((aOpl1993Handshake&0x78)<<1);
	}

TUint8 COplIOComm::Handshake(TUint aOpler1Handshake)
	{
	TUint opl1993Handshake=0;
	if (!(aOpler1Handshake&KConfigObeyCTS))
		opl1993Handshake=P_IGN_CTS;
	opl1993Handshake|=(aOpler1Handshake&0x3);
	return (TUint8)(opl1993Handshake|((aOpler1Handshake>>1)&0x78));
	}

TInt COplIOComm::Terminators(TUint32 aMask,TText8* charPtr)
	{
	TInt count=0;
	for (TInt ii=0;ii<32;ii++)
		{
		if ((aMask>>ii)&0x1)
			{
			count++;
			if (count>KConfigMaxTerminators)
				{
				return KErrNotSupported;
				}
			*charPtr++=(TUint8)ii;
			}
		}
	return count;
	}

COplIOComm::~COplIOComm()
	{
	if (iPort.IsOpen())
		{
		iPort.ReadCancel();
		iPort.WriteCancel();
		iPort.Cancel();
		// eb205: is any consumption of signals necessary here?
		}
	iPort.Close();
	iSession.Close();
	}

//
// Class COplIOFile
//
COplIOFile::COplIOFile(TInt16 aHandle)
	:COplIO(aHandle)
	{
	}

void COplIOFile::DoOpenFileL(RFs& aRFs,TBuf<256>& aFileName,TUint aMode)
	{
	User::LeaveIfError(iFile.Open(aRFs,aFileName,aMode));
	// If we're in text mode, check whether we have opened a Unicode text file or not
	if (aMode&EFileStreamText)
		{
		// Read the first two bytes of the file
		HBufC8* tempBuf=HBufC8::NewLC(2);
		TPtr8 ptr(tempBuf->Des());
		User::LeaveIfError(iFile.Read(ptr,2));
		if (!(ptr[0]==0xFF && ptr[1]==0xFE) && !(ptr[0]==0xFE && ptr[1]==0xFF)) // Not Unicode
			iUnicodeTextFile=EFalse;
		CleanupStack::PopAndDestroy(); //tempBuf
		// Reset the file to the start for any future use
		TInt currentPos=0;
		User::LeaveIfError(iFile.Seek(ESeekStart,currentPos));
		}
	}

void COplIOFile::ConstructL(TBuf<256>& aFileName,TInt16 aMode)
	{
	RFs& fs=TheRuntime()->ConEnv()->FsSession();
	iMode=aMode;
	const TUint mode=GetModeL();
	iUnicodeTextFile=ETrue; // Assume Unicode by default
	if (mode&EFileWrite && mode&EFileStreamText && (TheRuntime()->UserFlags() & KOplStateOutputAsciiTextFiles))
		iUnicodeTextFile=EFalse;
	switch (iMode&FSER_MASK)
		{
		case (FOPEN):
			{
			DoOpenFileL(fs,aFileName,mode);
			break;
			}
		case (FCREATE):
			{
			User::LeaveIfError(iFile.Create(fs,aFileName,mode));
			break;
			}
		case (FREPLACE):
			{
			User::LeaveIfError(iFile.Replace(fs,aFileName,mode));
			break;
			}
		case (FAPPEND):
			{
			DoOpenFileL(fs,aFileName,mode);
			TInt pos=0;
			User::LeaveIfError(iFile.Seek(ESeekEnd,pos));
			break;
			}
		case (FUNIQUE):
			{
			TFileName fileName;
			TParse parse;
			User::LeaveIfError(fs.Parse(aFileName,parse));
			User::LeaveIfError(iFile.Temp(fs,parse.DriveAndPath(),fileName,mode));
			if (fileName.Length()>aFileName.MaxLength())
				{
				iFile.Close();
				User::Leave(KErrBadName);
				}
			aFileName=fileName;
			break;
			}
		default:
			{
			User::Leave(KOplErrInvalidIO);
			break;
			}
		}
	// If we are in text file mode, additional action is needed. If we are WRITING a file
	// and it's non-Unicode we need to dump out 0xFEFF to the start of it to signify it's
	// Unicode not ASCII. If we are trying to READ a non-Unicode file, we must leave
	// with KErrNotSupported since it's not possible to do this at the moment (i.e.
	// the developer can trap for this error and fall back to opening the ASCII text file
	// in binary mode instead).
	if (mode&EFileStreamText)
		{
		if (mode&EFileWrite && iUnicodeTextFile)
			{
			TInt currentPos=0;
			User::LeaveIfError(iFile.Seek(ESeekCurrent,currentPos));
			if (currentPos==0)
				{
				TBuf8<2> buf(2);
				buf[0]=0xFF;
				buf[1]=0xFE;
				iFile.Write(buf);
				}
			}
		else if (!(mode&EFileWrite) && !iUnicodeTextFile)
			{
			iFile.Close();
			User::Leave(KErrNotSupported);
			}
		}
	}

TUint COplIOFile::GetModeL()
	{
	TUint mode=0;
	switch (iMode&FMT_MASK)
		{
	case (FSTREAM):
		mode=EFileStream;
		break;
	case (FSTREAM_TEXT):
		mode=EFileStreamText;
		break;
	default:
		User::Leave(KErrArgument);
		}
	mode|=(iMode&FUPDATE)?EFileWrite:EFileRead;
	mode|=(iMode&FSHARE)?EFileShareReadersOnly:EFileShareExclusive;
	return mode;
	}

void COplIOFile::RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus,TOplReqStatus* aOplStatusPtr, TAny* aParam1, TAny* aParam2)
	// eb205: I've replaced calls to "User::Leave(KOplErrInvalidIO)" with calls
	// to "User::RequestComplete(statusPtr,KErrArgument)"
	{
	if (aFuncNo==FREAD)
		{
		__LEAVE_IF_UNALIGNED(aParam1,KOplErrBadAlignment); // No statusPtr to RequestComplete against, at this point.
		aIOStatus=new(ELeave) TOplIOReadRequest(aOplStatusPtr,EActivePriorityWsEvents+4,(TUint8*)aParam1,aParam2);
		iFile.Read(((TOplIOReadRequest*)aIOStatus)->iPtr8,aIOStatus->Status());
		}
	else if (aFuncNo==FWRITE)
		{
		__LEAVE_IF_UNALIGNED(aParam1,KOplErrBadAlignment); // No statusPtr to RequestComplete against, at this point.
		aIOStatus=new(ELeave) TOplIOPtrRequest(aOplStatusPtr,EActivePriorityWsEvents+4,(TUint8*)aParam1,aParam2);
		// NOTE: 09/03/2002 (PhilS). We now use iBufferToWrite because the asynchronous
		// Write() method may not complete before this function exits. Previously we just
		// used an automatic, but that could potentially be discarded before the write
		// had finished (leading to a 'General Failure' error in OPL).
		TPtr8 ptr(((TOplIOPtrRequest*)aIOStatus)->iPtr8);
		const TInt length=ptr.Length();
		if (iBufferToWrite)
			{
			delete iBufferToWrite;
			iBufferToWrite=NULL;
			}
		if ((!iUnicodeTextFile) && (length))
			{
			// First get the raw 8-bit data which represents a wide string into an actual
			// 16-bit descriptor so it can be converted properly.
			const TInt halfLength=length>>1;
			HBufC* wideBuf=HBufC::NewLC(halfLength);
			wideBuf->Des().Append((TUint16*)(ptr.Ptr()),halfLength);
			// Convert Unicode buffer into ASCII, then write it to the text file.
			iBufferToWrite=COplRuntimeCharConv::ConvertFromUnicodeToNarrowL(*wideBuf);
			CleanupStack::PopAndDestroy(1); // wideBuf
			}
		else
			{
			iBufferToWrite=ptr.AllocL();
			}
		iFile.Write(*iBufferToWrite,aIOStatus->Status());
		}
	else
		{
		aIOStatus=new(ELeave) TOplIORequest(aOplStatusPtr,EActivePriorityWsEvents+4);
		TRequestStatus* statusPtr=&(aIOStatus->Status());
		switch (aFuncNo)
			{
			case (FCLOSE):
				(TheRuntime()->IOCollection()).RemoveObject(iHandle);
				User::RequestComplete(statusPtr,KErrNone);
				break;
			case (FCANCEL):
				User::RequestComplete(statusPtr,KErrNone);
				break;
			case (FFLUSH):
				User::RequestComplete(statusPtr,iFile.Flush());
				break;
			case (FSEEK):
				{
				if (!(iMode&FRANDOM))
					{
					User::RequestComplete(statusPtr,KErrArgument);
					return; // return here as there's been an error
					}
				TSeek seekMode=ESeekStart;
				switch (OplUtil::GetWord(aParam1))
					{
					case (FABS):
						//seekMode=ESeekStart;
						break;
					case (FEND):
						seekMode=ESeekEnd;
						break;
					case (FCUR):
						seekMode=ESeekCurrent;
						break;
					default:
						User::RequestComplete(statusPtr,KErrArgument);
						return; // return here as there's been an error
					}
				TInt pos=OplUtil::GetLong(aParam2);
				User::RequestComplete(statusPtr,iFile.Seek(seekMode,pos));
				OplUtil::PutLong(aParam2,pos);
				break;
				}
			case (FSETEOF):
				User::RequestComplete(statusPtr,iFile.SetSize(OplUtil::GetLong(aParam1))); // eb205: is this really correct?
				break;
			default:
				User::RequestComplete(statusPtr,KErrArgument);
				return; // return here as there's been an error
			}
		}
	}

COplIOFile::~COplIOFile()
	{
	iFile.Close();
	delete iBufferToWrite;
	}

COplIOFileText::COplIOFileText(TInt16 aHandle)
	:COplIOFile(aHandle)
	{
	}

void COplIOFileText::ConstructL(TBuf<256>& aFileName,TInt16 aMode)
	{
	COplIOFile::ConstructL(aFileName,(TInt16)((aMode&~FMT_MASK)|FSTREAM_TEXT)); // open or create as stream-text
	iTextFile.Set(iFile);
	iMode=aMode;
	}

void COplIOFileText::RunFunctionL(TInt aFuncNo,TIORequest*& aIOStatus,TOplReqStatus* aOplStatusPtr, TAny* aParam1, TAny* aParam2)
	// eb205: I've replaced calls to "User::Leave(KOplErrInvalidIO)" with calls
	// to "User::RequestComplete(statusPtr,KErrArgument)"
	{
	if (aFuncNo==FREAD)
		{
		__LEAVE_IF_UNALIGNED(aParam1,KOplErrBadAlignment); // No statusPtr to RequestComplete against, at this point.
		TOplIOTextReadRequest* request=new(ELeave) TOplIOTextReadRequest(aOplStatusPtr,EActivePriorityWsEvents+4,(TUint8*)aParam1,aParam2);
		aIOStatus=request;
		TRequestStatus* statusPtr=&request->Status();
		TPtr ptr((TText*)request->iPtr8.Ptr(),request->iPtr8.Length(),request->iPtr8.MaxLength());
		User::RequestComplete(statusPtr,iTextFile.Read(ptr));
		request->iPtr8.SetLength(ptr.Length());
		}
	else if (aFuncNo==FWRITE)
		{
		__LEAVE_IF_UNALIGNED(aParam1,KOplErrBadAlignment); // No statusPtr to RequestComplete against, at this point.
		TOplIOPtrRequest* request=new(ELeave) TOplIOPtrRequest(aOplStatusPtr,EActivePriorityWsEvents+4,(TUint8*)aParam1,aParam2);
		aIOStatus=request;
		TRequestStatus* statusPtr=&request->Status();
		const TPtrC ptr((TText*)request->iPtr8.Ptr(),request->iPtr8.Length()); // Text IO takes number of Unicode chars to write, not bytes.
		if (!iUnicodeTextFile)
			{
			// Convert Unicode buffer into ASCII, then write it to the text file.
			// Note that we write to iFile and NOT iTextFile - iTextFile expects
			// a wide buffer on Unicode builds, but all it does is flush it through
			// to an RFile anyway (see COplIOFileText::ConstructL() where this is set)
			HBufC8* buf=COplRuntimeCharConv::ConvertFromUnicodeToNarrowLC(ptr);
			User::RequestComplete(statusPtr,iFile.Write(*buf));
			iTextFile.Set(iFile);
			CleanupStack::PopAndDestroy(); // buf
			}
		else
			{
			User::RequestComplete(statusPtr,iTextFile.Write(ptr));
			}
		}
	else
		{
		aIOStatus=new(ELeave) TOplIORequest(aOplStatusPtr,EActivePriorityWsEvents+4);
		TRequestStatus* statusPtr=&(aIOStatus->Status());
		switch (aFuncNo)
			{
			case (FCLOSE):
				(TheRuntime()->IOCollection()).RemoveObject(iHandle);
				User::RequestComplete(statusPtr,KErrNone);
				break;
			case (FCANCEL):
				User::RequestComplete(statusPtr,KErrNone);
				break;
			case (FFLUSH):
				User::RequestComplete(statusPtr,iFile.Flush());
				break;
			case (FSEEK):
				{
				if (!(iMode&FRANDOM))
					{
					User::RequestComplete(statusPtr,KErrArgument);
					return; // return here as there's been an error
					}
				TSeek seekMode=ESeekStart;
				switch (OplUtil::GetWord(aParam1))
					{
					case (FREWIND):
						//seekMode=ESeekStart;
						break;
					case (FRSENSE):
					case (FRSET):
					default:
						User::RequestComplete(statusPtr,KErrArgument);
						return; // return here as there's been an error
					}
				//TInt pos=OplUtil::GetLong(aParam2);
				User::RequestComplete(statusPtr,iTextFile.Seek(seekMode));
				//OplUtil::PutLong(aParam2,0);
				break;
				}
			case (FSETEOF):
				{
				const TInt error=iFile.SetSize(OplUtil::GetLong(aParam1)); // eb205: is this really correct?
				if (error!=KErrNone)
					{
					User::RequestComplete(statusPtr,error);
					return; // return here as there's been an error
					}
				iTextFile.Set(iFile);
				User::RequestComplete(statusPtr,iTextFile.Seek(ESeekEnd));
				}
				break;
			default:
				User::RequestComplete(statusPtr,KErrArgument);
				return; // return here as there's been an error
			}
		}
	}

//
// Class TOplIOReadRequest
//
TOplIOReadRequest::TOplIOReadRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TAny* aParam1,TAny* aParam2)
	:TOplIOPtrRequest(aStatusPtr,aPriority,aParam1,aParam2)
	{
	iLenPtr=(TUint16*)aParam2;
	}
	
void TOplIOReadRequest::DoParamsUpdate()
	{
	if (OplUtil::GetWord(iLenPtr)!=0 && iPtr8.Length()==0) //end of file reached!?!
		iStatus=KErrEof;
	else
		OplUtil::PutWord(iLenPtr,(TUint16)iPtr8.Length());
	}

//
// Class TOplIOTextReadRequest
//
TOplIOTextReadRequest::TOplIOTextReadRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TAny* aParam1,TAny* aParam2)
	:TOplIOReadRequest(aStatusPtr,aPriority,aParam1,aParam2)
	{
	}

void TOplIOTextReadRequest::DoParamsUpdate()
	{
	if (iStatus==KErrTooBig)
		iStatus=KOplErrRecord; // OPL1993 also positioned to the start of the next record
	OplUtil::PutWord(iLenPtr,(TUint16)iPtr8.Length());
	}

//
// Class TOplIOPtrRequest
//
TOplIOPtrRequest::TOplIOPtrRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TAny* aParam1,TAny* aParam2)
	:TOplIORequest(aStatusPtr,aPriority), iPtr8((TUint8*)aParam1,OplUtil::GetWord(aParam2),OplUtil::GetWord(aParam2))
	{
	}
	
//
// Class TOplIOCallBackRequest
//
#pragma warning ( disable: 4310) // cast truncates constant
TOplIOCallBackRequest::TOplIOCallBackRequest(TOplReqStatus* aStatusPtr,TInt aPriority,TCallBack aCallBack)
	:TOplIORequest(aStatusPtr,aPriority)
	{
	OplUtil::PutWord(aStatusPtr,(TInt16)KOplErrFilePending);
	iCallBack=aCallBack;
	}
#pragma warning ( default: 4310)

void TOplIOCallBackRequest::DoParamsUpdate()
	{
	TInt ret=iCallBack.CallBack();
	__ASSERT_ALWAYS(ret!=KRequestPending, User::Panic(KOplrIODEV, 1));
	if (ret)
		iStatus=ret;
	}

//
// Class TEraIOCallBackRequest
//
TEraIOCallBackRequest::TEraIOCallBackRequest(TInt32* aStatusPtr,TInt aPriority,TCallBack aCallBack)
	:TIORequest(aStatusPtr, aPriority)
	{
	OplUtil::PutLong(aStatusPtr,KRequestPending);
	iCallBack=aCallBack;
	}

void TEraIOCallBackRequest::DoHandleCompletion()
	{
	TInt ret=iCallBack.CallBack();
	__ASSERT_ALWAYS(ret!=KRequestPending, User::Panic(KOplrIODEV, 2));
	OplUtil::PutLong(iStatusPtr,((ret)?ret:iStatus.Int()));
	}