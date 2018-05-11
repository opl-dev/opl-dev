// CONKYWD.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include <bldvariant.hrh>
#include <opcodes.h>
#include <opldb.h>
#include "oplutil.h"
#include "graphics.h"
#include "console.h"
#include "module.h"
#include "oplstack.h"
		
void OpCode::ScreenInfo(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)		
	{
	TUint16* pArray=(TUint16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),
					sizeof(SScreenInfo));
	SScreenInfo screenInfo;
	aRuntime.Console().ScreenInfo(screenInfo);
	Mem::Copy(pArray,&screenInfo,sizeof(SScreenInfo));
	}

void OpCode::PrintSpace(CStack&, COplRuntime& aRuntime, CFrame* )
	{
	_LIT(KSpace," ");
    aRuntime.Console().Print(KSpace);
	}	

void OpCode::PrintInt(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<KOplIntLen> buf;
	_LIT(KFormatDecimal,"%d");
	buf.Format(KFormatDecimal,aStack.PopInt16());
	aRuntime.Console().Print(buf);
	aRuntime.DrawablesCollection().FlushIfOn();
	}
	
void OpCode::PrintLong(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<KOplLongLen> buf;
	_LIT(KFormatDecimal,"%d");
	buf.Format(KFormatDecimal,aStack.PopInt32());
	aRuntime.Console().Print(buf);
	aRuntime.DrawablesCollection().FlushIfOn();
	}

void OpCode::PrintFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TReal value = aStack.PopReal64();
	TReal* pValue;
	pValue = &value;
	TBuf<KOplFloatLen> buf;
	TInt flags=aRuntime.UserFlags();
	TOplRealFormat format(flags,KOplFloatLen);	//changed to TOplRealFormat by Ann
	User::LeaveIfError(buf.Num(value,format));
	aRuntime.Console().Print(buf);
	aRuntime.DrawablesCollection().FlushIfOn();
	}
	
void OpCode::PrintString(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aRuntime.Console().Print(aStack.PopString());
	aRuntime.DrawablesCollection().FlushIfOn();
	}

void OpCode::PrintCarriageReturn(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aRuntime.Console().Cr(); // must be Cr() then Lf()
	aRuntime.Console().Lf();   
	aRuntime.DrawablesCollection().FlushIfOn();
	}

void OpCode::InputInt(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt16 val;
	aRuntime.Console().InputL(*aRuntime.ConEnv(),val,aRuntime.GetTrap());
	aStack.Push(val);
	aRuntime.ClearTrap();
	AssignInt(aStack,aRuntime,aFramePtr);
	}
	
void OpCode::InputLong(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt32 val;
	aRuntime.Console().InputL(*aRuntime.ConEnv(),val,aRuntime.GetTrap());
	aStack.Push(val);
	aRuntime.ClearTrap();
	AssignLong(aStack,aRuntime,aFramePtr);
	}
	
void OpCode::InputFloat(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TReal64 val;
	aRuntime.Console().InputL(*aRuntime.ConEnv(),val,aRuntime.GetTrap());
	aStack.Push(val);
	aRuntime.ClearTrap();
	AssignFloat(aStack,aRuntime,aFramePtr);
	}

LOCAL_C void doInputEditL(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr,TBool aIsInput)
	{
	TInt16 fieldFlag=aStack.PopInt16();
	TText *strRef=NULL;
	TInt16 maxLen=0;
    TText buf[255];
	TPtr ptr(buf,255);
	TUint8* p=NULL;
    if (fieldFlag!=KLeftSideField)
		{
		strRef=aStack.PopRefString();
		__ASSERT_ISALIGNED(strRef);
		maxLen=aStack.PopInt16();
		p=(TUint8*)strRef;
		ptr.Set((TText*)(p+1+KOplAlignment),*p,maxLen);
		}
	else
		{
		if (!aIsInput)
            aRuntime.DbManager()->StringFieldValueL(ptr);
		}
	if (aIsInput)
		ptr.SetLength(0);
	TInt ret=aRuntime.Console().Edit(*aRuntime.ConEnv(),ptr,aRuntime.GetTrap());
	if (ret!=KErrNone) // only esc returns with error
		User::Leave(KOplErrEsc);
	if (fieldFlag!=KLeftSideField)
		{
		aStack.Push(maxLen);
		aStack.PushRef(strRef);
		}
	aStack.Push(fieldFlag);
	aStack.Push(ptr);
	aRuntime.ClearTrap();
	OpCode::AssignString(aStack, aRuntime, aFramePtr);
	}
	
void OpCode::InputString(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	doInputEditL(aStack,aRuntime,aFramePtr,ETrue);
	}

void OpCode::Edit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	doInputEditL(aStack,aRuntime,aFramePtr,EFalse);
	}


void FuncOpCode::Get(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push(aRuntime.IOCollection().WsEventHandler().DoGet(*aRuntime.ConEnv()));
	}

void FuncOpCode::Key(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push(aRuntime.IOCollection().WsEventHandler().DoKey(*aRuntime.ConEnv()));
	}

void FuncOpCode::Kmod(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aStack.Push(aRuntime.IOCollection().WsEventHandler().DoKmod());
	}

void FuncOpCode::GetStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<1> str;
	str.Append(aRuntime.IOCollection().WsEventHandler().DoGet(*aRuntime.ConEnv()));
	aStack.Push(str);
	}

void FuncOpCode::KeyStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TBuf<1> str;
	str.Append(aRuntime.IOCollection().WsEventHandler().DoKey(*aRuntime.ConEnv()));
	aStack.Push(str);
	}

void OpCode::At(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)	 
	{
	TInt16 y=aStack.PopInt16();
	TInt16 x=aStack.PopInt16();
	User::LeaveIfError(aRuntime.Console().At(TPoint(x,y)));
	aRuntime.DrawablesCollection().FlushIfOn();
	}

void OpCode::Cls(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	aRuntime.Console().Cls();
	CDrawablesCollection& coll=aRuntime.DrawablesCollection();
	coll.DrawAllClocks();
	coll.FlushIfOn();
	}

void OpCode::Screen2(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16 y=aStack.PopInt16();
	TSize  size=TSize(aStack.PopInt16(),y); 
	User::LeaveIfError(aRuntime.Console().SetScreen(size,TPoint(0,0)));
	aRuntime.DrawablesCollection().FlushIfOn();
	}
	
void OpCode::Screen4(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt16  y=aStack.PopInt16();
	TPoint  pos=TPoint(aStack.PopInt16(),y); 
	y=aStack.PopInt16();
	TSize  size=TSize(aStack.PopInt16(),y); 
	User::LeaveIfError(aRuntime.Console().SetScreen(size,pos));
	aRuntime.DrawablesCollection().FlushIfOn();
	}

LOCAL_C void PopStyle(TAlgStyle& aStyle,TBool& aInverse,TBool& aUnderline,CStack& aStack)
	{
	TInt style=aStack.PopInt16();
	aStyle.SetIsBold(style&1);
	aStyle.SetIsMono(style&16);
	aStyle.SetIsItalic(style&32);
	aInverse=(style&4);
	aUnderline=(style&2);
	if (style&8)
		aStyle.SetHeightFactor(2);
	}


void OpCode::Font(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	TAlgStyle style;
	style.SetWidthFactor(1);
	style.SetHeightFactor(1);
	TBool inverse;
	TBool underline;
	PopStyle(style,inverse,underline,aStack);
#if defined(__SERIES60__)||defined(__UIQ__)
	TUid fontUid=OplUtil::MapFontId(aStack.PopInt32(),style);
	if (OplUtil::IsStandardFontUid(fontUid))
		{
		const CFont *font=OplUtil::MapFontUidToStandardFontL(fontUid);
		User::LeaveIfError(aRuntime.Console().SetStandardFont(font,style,inverse,underline));
		}
	else
		{
		User::LeaveIfError(aRuntime.Console().SetFontByUidL(fontUid,style,inverse,underline));
		}
#else
	TUid fontId=OplUtil::MapFontId(aStack.PopInt32(),style);
	User::LeaveIfError(aRuntime.Console().SetFontByUid(fontId,style,inverse,underline));
#endif
	aRuntime.DrawablesCollection().FlushIfOn();
	}

void OpCode::Style(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	TAlgStyle style;
	style.SetWidthFactor(1);
	style.SetHeightFactor(1);
	TBool inverse;
	TBool underline;
	PopStyle(style,inverse,underline,aStack);
	User::LeaveIfError(aRuntime.Console().SetStyle(style,inverse,underline));
    }

void OpCode::GetEvent(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	TInt* pArray=(TInt*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),
					sizeof(TInt16)*KOplEvent16ArrayElems);
	aRuntime.IOCollection().WsEventHandler().DoGetEvent(*aRuntime.ConEnv(), (TInt16*)pArray);
    }

void OpCode::GetEvent32(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	TInt* pArray=(TInt*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),
					sizeof(TInt)*KOplEvent32ArrayElems);
	aRuntime.IOCollection().WsEventHandler().DoGetEvent(*aRuntime.ConEnv(), (TInt32*)pArray);
	}

void OpCode::GetEventA32(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	// GetEventA stat%,ev&()
	TInt* pArray=(TInt*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),
					sizeof(TInt)*KOplEvent32ArrayElems);
	TOplReqStatus* pStatus=(TOplReqStatus*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt16));
	aRuntime.IOCollection().WsEventHandler().DoGetEventA(*aRuntime.ConEnv(),pStatus,(TInt32*)pArray);
	}

void OpCode::Pause(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
	aRuntime.IOCollection().WsEventHandler().DoPauseL(*aRuntime.ConEnv(), aStack.PopInt16());
    }

void FuncOpCode::KeyA(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TInt16* pArray=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TUint16)*2);
	TInt16* pStatus=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt16));
	aRuntime.IOCollection().WsEventHandler().DoKeyA(*aRuntime.ConEnv(),pStatus,pArray);
	aStack.Push(TInt16(0));
    }

void FuncOpCode::KeyC(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TInt16* pStatus=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt16));
	aRuntime.IOCollection().WsEventHandler().DoCancelOplAsynchronousWservRequest(pStatus);
	aStack.Push(TInt16(0));
    }

void FuncOpCode::TestEvent(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	if (aRuntime.IOCollection().WsEventHandler().DoTestEvent(*aRuntime.ConEnv()))
		aStack.Push(TInt16(KOplTrue));
	else
		aStack.Push(TInt16(KOplFalse));
    }

void FuncOpCode::GetEventC(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TInt16* pStatus=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt16));
	aRuntime.IOCollection().WsEventHandler().DoCancelOplAsynchronousWservRequest(pStatus);
	aStack.Push(TInt16(0));
   	}

