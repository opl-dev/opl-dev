// IOKEYWRD.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <e32std.h>
#include <opcodes.h>
#include "iodef.h"
#include "oplutil.h"
#include "module.h"
#include "iomain.h"

enum
	{
	EMaximumNumberOfCompletedEventsToHandleInIoYield=10
	};

void OpCode::IoWaitStat32(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TAny* statusPtr=OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt32));
	aRuntime.IOCollection().WsEventHandler().WaitForOplAsynchronousRequestCompletion32(*aRuntime.ConEnv(),statusPtr);
 	}

void OpCode::IoWaitStat(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TOplReqStatus* statusPtr=(TOplReqStatus*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt16));
	aRuntime.IOCollection().WsEventHandler().WaitForOplAsynchronousRequestCompletion16(*aRuntime.ConEnv(),statusPtr);
	}
	
void OpCode::IoYield(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aRuntime.IOCollection().WsEventHandler().HandleAnyCompletedEvents(*aRuntime.ConEnv(),EMaximumNumberOfCompletedEventsToHandleInIoYield);
	}

_LIT(KFormatDecimal,"%d");

void OpCode::LPrintInt(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<16> buf;
	buf.Format(KFormatDecimal,aStack.PopInt16());
	TInt16 length=(TInt16)buf.Size();
	TInt16 ret=aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),-1,FWRITE,(TAny*)buf.Ptr(),&length);
	if (ret<0)
		User::Leave(OPLERR(ret));
	}
	
void OpCode::LPrintLong(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<16> buf;
	buf.Format(KFormatDecimal,aStack.PopInt32());
	TInt16 length=(TInt16)buf.Size();
	TInt16 ret=aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),-1,FWRITE,(TAny*)buf.Ptr(),&length);
	if (ret<0)
		User::Leave(OPLERR(ret));
	}
	
void OpCode::LPrintFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<16> outputBuf;
	TInt flags=aRuntime.UserFlags();
	TOplRealFormat format(flags,16);
	outputBuf.Num(aStack.PopReal64(),format);
	TInt16 length=(TInt16)outputBuf.Size();
	TInt16 ret=aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),-1,FWRITE,(TAny*)outputBuf.Ptr(),&length);
	if (ret<0)
		User::Leave(OPLERR(ret));
	}
	
void OpCode::LPrintString(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPtrC ptr=aStack.PopString();
	TInt16 length=(TInt16)ptr.Size();
	TInt16 ret=aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),-1,FWRITE,(TAny*)ptr.Ptr(),&length);
	if (ret<0)
		User::Leave(OPLERR(ret));
	}

void OpCode::LPrintSpace(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	_LIT(KSpace," ");
	TPtrC ptr=KSpace();
	TInt16 length=(TInt16)ptr.Size();
	TInt16 ret=aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),-1,FWRITE,(TAny*)ptr.Ptr(),&length);
	if (ret<0)
		User::Leave(OPLERR(ret));
	}
	
void OpCode::LPrintCarriageReturn(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	_LIT(KEOL,"\r\n");
	TPtrC ptr=KEOL();
	TInt16 length=(TInt16)ptr.Size();
	TInt16 ret=aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),-1,FWRITE,(TAny*)ptr.Ptr(),&length);
	if (ret<0)
		User::Leave(OPLERR(ret));
	}
	
void OpCode::LOpen(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16 mode=FREPLACE|FSTREAM_TEXT|FUPDATE|FRANDOM;
	TPtrC devname=aStack.PopString();
	TInt16 handle=0;
	CIOCollection& IOCollection=aRuntime.IOCollection();
	if (IOCollection.LHandle())
		User::Leave(KOplErrDevOpen);
	else
		{
		mode=IOCollection.NewObject(&handle,devname,mode);
		if (mode<0)
			User::Leave(OPLERR(mode));
		IOCollection.SetLHandle(handle);
		}
	aRuntime.ClearTrap();
	}
	
void OpCode::LClose(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	CIOCollection& IOCollection=aRuntime.IOCollection();
	TInt16 lHandle=IOCollection.LHandle();
	if (lHandle)
		{
		TInt16 ret=IOCollection.DoIow(*aRuntime.ConEnv(),lHandle,FCLOSE,NULL,NULL);
		if (ret<0)
			User::Leave(OPLERR(ret));
		IOCollection.SetLHandle(0);
		}
	aRuntime.ClearTrap();
	}

//
// Func opcodes
//	
void FuncOpCode::Ioa(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	RHeap& heap=aRuntime.Heap64();
	TAny* param2=OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint8));
	TAny* param1=OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint8));
	TOplReqStatus* statusPtr=(TOplReqStatus*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TInt16));
	TInt16 funcNo=aStack.PopInt16();
	TInt16 handle=aStack.PopInt16();
	aStack.Push(aRuntime.IOCollection().DoIOA(handle,funcNo,statusPtr,param1,param2));
	}

void FuncOpCode::Iow(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	RHeap& heap=aRuntime.Heap64();
	TAny* param2=OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint8));
	TAny* param1=OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint8));
	TInt16 funcNo=aStack.PopInt16();
	TInt16 handle=aStack.PopInt16();
	aStack.Push(aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),handle,funcNo,param1,param2));
	}

void FuncOpCode::IoOpen(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16 mode=aStack.PopInt16();
	TPtrC devname=aStack.PopString();
	TInt16* handlePtr=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt16));
	aStack.Push(aRuntime.IOCollection().NewObject(handlePtr,devname,mode));
	}

void FuncOpCode::IoOpenUnique(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16 mode=aStack.PopInt16();
	RHeap& heap=aRuntime.Heap64();
	TUint8* addr=OplUtil::PopOffsetAsAddrL(aStack,heap,KMaxFileName);
	TInt16* handlePtr=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TInt16));
	TPtr devname((TText*)(addr+1+KOplAlignment),*addr,KMaxFileName-1);	// KMaxFileName is 256, max string length in opl is 255!
	aStack.Push(aRuntime.IOCollection().NewObject(handlePtr,devname,mode));
	OplUtil::PutWord(addr,(TUint16)devname.Length());
	}

void FuncOpCode::IoWrite(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	// Supports both 8- and 16-bit writing, so we cannot ASSERT addr is aligned, i.e.
	// it is valid to write an 8-bit binary value from an odd address.
	TInt16 length=aStack.PopInt16();
	TAny* addr=OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),length);
	TInt16 handle=aStack.PopInt16();
	if (!handle)
		User::Leave(KOplErrInvalidArgs);
	aStack.Push(aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),handle,FWRITE,addr,&length));
	}

void FuncOpCode::IoRead(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16 maxLength=aStack.PopInt16();
	TAny* addr=OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),maxLength);
	TInt16 handle=aStack.PopInt16();
	if (!handle)
		User::Leave(KOplErrInvalidArgs);
	TInt16 ret=aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),handle,FREAD,addr,&maxLength);
	if (!ret)
		aStack.Push(maxLength); //maxLength is set to be actual length read
	else
		aStack.Push(ret);
	}

void OpCode::IoSignal(CStack& /*aStack*/, COplRuntime& /*aRuntime*/, CFrame* /*aFramePtr*/)
	{
	CWsEventHandler::Signal(1);
	}
	
void FuncOpCode::IoSeek(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt32* offsetPtr=(TInt32*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt32));
	TInt16 mode=aStack.PopInt16();
	TInt16 handle=aStack.PopInt16();
	aStack.Push(aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),handle,FSEEK,&mode,offsetPtr));
	}

void FuncOpCode::IoClose(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16 handle=aStack.PopInt16();
	if (handle)
		aStack.Push(aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),handle,FCLOSE,NULL,NULL));
	else
		aStack.Push((TInt16)0);
	}

void FuncOpCode::IoWait(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aRuntime.IOCollection().WsEventHandler().WaitForAnyOplAsynchronousRequestCompletion(*aRuntime.ConEnv());
	aStack.Push((TInt16)0);
	}

void FuncOpCode::Ioc(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt noOfParams=aRuntime.IP8();
	TAny* params[2]={NULL};
	RHeap& heap=aRuntime.Heap64();
	for (TInt count=noOfParams-4;count>=0;--count)
		{
		params[count]=OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint8));
		}
	TOplReqStatus* statusPtr=(TOplReqStatus*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TInt16));
	TInt16 funcNo=aStack.PopInt16();
	TInt16 handle=aStack.PopInt16();
	TInt16 err=aRuntime.IOCollection().DoIOA(handle,funcNo,statusPtr,params[0],params[1]);
	if (err)
		{
		// eb205: is this correct?, what about the 32-bit status word?
		OplUtil::PutWord(statusPtr,err);
		CWsEventHandler::Signal(1);
		}
	aStack.Push((TInt16)0);
	}

void FuncOpCode::IoCancel(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push(aRuntime.IOCollection().DoIow(*aRuntime.ConEnv(),aStack.PopInt16(),FCANCEL,NULL,NULL)); // eb205: does this also work if there's nothing to cancel?
	}