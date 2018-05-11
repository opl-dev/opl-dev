// OPXBMP.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <coemain.h>
#include "opxbmp.h"
#include <oplerr.h>

//
// COplSprite
//
COplSprite::COplSprite(RWsSession& aWsSession)
	:iSprite(aWsSession)
	{
	iDrawn=EFalse;
	}

COplSprite::~COplSprite()
	{
	iSprite.Close();
	}

//
// COpxData
//
COpxData::COpxData(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

COpxData::~COpxData()
	{
	if (iBitmapArray)
		{
		iBitmapArray->ResetAndDestroy();
		delete iBitmapArray;
		}
	if (iSpriteArray)
		{
		iSpriteArray->ResetAndDestroy();
		delete iSpriteArray;
		}
	Dll::FreeTls();
	}

void COpxData::ConstructL()
	{
	iBitmapArray=new(ELeave) CArrayPtrSeg<CFbsBitmap>(4);
	iSpriteArray=new(ELeave) CArrayPtrSeg<COplSprite>(4);
	}

TInt COpxData::FindIndexL(CFbsBitmap* aHandle)
	{
	TInt index;
	TKeyArrayFix key(0,ECmpTUint32);
	TInt ret=iBitmapArray->Find(aHandle,key,index);
	if (ret!=KErrNone)
		User::Leave(KErrBadHandle);
	return index;
	}

TInt COpxData::FindIndexL(COplSprite* aHandle)
	{
	TInt index;
	TKeyArrayFix key(0,ECmpTUint32);
	TInt ret=iSpriteArray->Find(aHandle,key,index);
	if (ret!=KErrNone)
		User::Leave(KErrBadHandle);
	return index;
	}

//
// OPX bitmap functions
//
void COpxData::LoadBitmapL()
	{
	CFbsBitmap* bmp=new(ELeave) CFbsBitmap;
	TInt index=iOplAPI.PopInt32();
	CleanupStack::PushL(bmp);
	User::LeaveIfError(bmp->Load(iOplAPI.PopString(),index));
	iBitmapArray->AppendL(bmp);
	CleanupStack::Pop();
	iOplAPI.Push((TInt32)bmp);
	}

void COpxData::UnloadBitmapL()
	{
	CFbsBitmap* bmp=(CFbsBitmap*)(iOplAPI.PopInt32());
	TInt index=FindIndexL(bmp);
	delete bmp;
	iBitmapArray->Delete(index);
	iOplAPI.Push(0.0);
	}

void COpxData::BitmapDisplayModeL()
	{
	CFbsBitmap* bmp=(CFbsBitmap*)(iOplAPI.PopInt32());
	FindIndexL(bmp);
	iOplAPI.Push((TInt32)(bmp->DisplayMode()-1));
	}

//
// OPX sprite functions
//
void COpxData::SpriteCreateL()
	{
	COplSprite* sprite=new(ELeave) COplSprite(iOplAPI.WsSession());
	TInt flags=iOplAPI.PopInt32();
	TInt y=iOplAPI.PopInt32();
	TPoint pos(iOplAPI.PopInt32(),y);
	RWindowTreeNode* win;
	TInt16 id=iOplAPI.PopInt16();
	if (id==0)
		win=&iOplAPI.RootWindow();
	else
		win=&iOplAPI.WindowFromIdL(id);
	sprite->Sprite().Construct(*win,pos,flags);
	iSpriteArray->AppendL(sprite);
	iCurrentSprite=sprite;
	iOplAPI.Push((TInt32)sprite);
	}

void COpxData::GetMemberL(TSpriteMember& aMem)
	{
	TInt y=iOplAPI.PopInt32();
	aMem.iOffset=TPoint(iOplAPI.PopInt32(),y);
	aMem.iInvertMask=((iOplAPI.PopInt32())?ETrue:0);
	aMem.iMaskBitmap=(CFbsBitmap*)(iOplAPI.PopInt32());
	aMem.iBitmap=(CFbsBitmap*)(iOplAPI.PopInt32());
	if (aMem.iBitmap==NULL)
		aMem.iMaskBitmap=NULL;
	else
		{
		// ensure bitmaps exist
		FindIndexL(aMem.iBitmap);
		FindIndexL(aMem.iMaskBitmap);
		// check size is the same
		if (aMem.iBitmap->SizeInPixels()!=aMem.iMaskBitmap->SizeInPixels())
			User::Leave(KErrArgument);
		}
	aMem.iInterval=iOplAPI.PopInt32();
	}

void COpxData::SpriteAppendL()
	{
	if (iCurrentSprite==NULL)
		User::Leave(KErrNotFound);
	TSpriteMember sMem;
	GetMemberL(sMem);
	iCurrentSprite->Sprite().AppendMember(sMem);
	iOplAPI.Push(0.0); // will be void
	}

void COpxData::SpriteChangeL()
	{
	COplSprite* sprite=iCurrentSprite;
	if (sprite==NULL)
		User::Leave(KErrNotFound);
	if (!sprite->iDrawn)
		User::Leave(KOplStructure);
	TSpriteMember sMem;
	GetMemberL(sMem);
	User::LeaveIfError(iCurrentSprite->Sprite().UpdateMember(iOplAPI.PopInt32(),sMem));
	iOplAPI.Push(0.0); // will be void
	}

void COpxData::SpriteDrawL()
	{
	if (iCurrentSprite==NULL)
		User::Leave(KErrNotFound);
	COplSprite* sprite=iCurrentSprite;
	if (!sprite->iDrawn)
		User::LeaveIfError(sprite->Sprite().Activate());
	sprite->iDrawn=ETrue;
	iOplAPI.Push(0.0); // will be void
	}

void COpxData::SpritePosL()
	{
	if (iCurrentSprite==NULL)
		User::Leave(KErrNotFound);
	TInt y=iOplAPI.PopInt32();
	TPoint pos(iOplAPI.PopInt32(),y);
	iCurrentSprite->Sprite().SetPosition(pos);
	iOplAPI.WsSession().Flush();
	iOplAPI.Push(0.0); // will be void
	}

void COpxData::SpriteDeleteL()
	{
	COplSprite* sprite=(COplSprite*)(iOplAPI.PopInt32());
	TInt index=FindIndexL(sprite);
	delete sprite;
	if (iCurrentSprite==sprite)
		iCurrentSprite=NULL;
	iSpriteArray->Delete(index);
	iOplAPI.WsSession().Flush();
	iOplAPI.Push(0.0); // will be void
	}

void COpxData::SpriteUseL()
	{
	COplSprite* sprite=(COplSprite*)(iOplAPI.PopInt32());
	FindIndexL(sprite);
	iCurrentSprite=sprite;
	iOplAPI.Push(0.0); // will be void
	}

void COpxData::TestCallbackL()
	{
	// takes a string as the name to callback
	TInt16 num=iOplAPI.PopInt16();
	TPtrC ptr=iOplAPI.PopString();
	iOplAPI.InitCallbackL(ptr);
	iOplAPI.PushParamL(num);
	TInt32 ret=iOplAPI.CallProcedure(EReturnLong);
	if (ret)
		iOplAPI.Push(ret);
	}

void DoCloseTimer(TAny* aTimer)
	{
	((RTimer*)aTimer)->Close();
	delete aTimer;
	}

TInt CloseTimer(TAny* aTimer)
	{
	DoCloseTimer(aTimer);
	return KErrNone;
	}

void COpxData::TestAsyncL()
	{
	RTimer* timer=new(ELeave)RTimer;
	TCleanupItem cleanup(DoCloseTimer,timer);
	CleanupStack::PushL(cleanup);
	User::LeaveIfError(timer->CreateLocal());
	TInt32 delay=iOplAPI.PopInt32();
	TInt32* statPtr=iOplAPI.PopPtrInt32();
	TCallBack callback(CloseTimer,timer);
	TRequestStatus& status=iOplAPI.NewRequestL(statPtr,EActivePriorityWsEvents,callback); // eb205: is EActivePriorityWsEvents the right priority?
	CleanupStack::Pop(); // timer
	timer->After(status,delay*1000);
	iOplAPI.Push(0.0);
	}

//
// OPX loading interface
//
enum 
	{
	ELoadBitmapL=1,		// 1
	EUnloadBitmapL,		// 2
	EBitmapDisplayModeL,// 3
	ESpriteCreateL,		// 4
	ESpriteAppendL,		// 5
	ESpriteChangeL,		// 6
	ESpriteDrawL,		// 7
	ESpritePosL,		// 8
	ESpriteDeleteL,		// 9
	ESpriteUseL,		// 10
	ETestCallbackL,		// 11
	ETestAsyncL			// 12
	};

void COpxData::RunL(TInt aProcNum)
	{
	switch(aProcNum)
		{
	case ELoadBitmapL:
		{
		LoadBitmapL();
		break;
		}
	case EUnloadBitmapL:
		{
		UnloadBitmapL();
		break;
		}
	case EBitmapDisplayModeL:
		{
		BitmapDisplayModeL();
		break;
		}
	case ESpriteCreateL:
		{
		SpriteCreateL();
		break;
		}
	case ESpriteAppendL:
		{
		SpriteAppendL();
		break;
		}
	case ESpriteChangeL:
		{
		SpriteChangeL();
		break;
		}
	case ESpriteDrawL:
		{
		SpriteDrawL();
		break;
		}
	case ESpritePosL:
		{
		SpritePosL();
		break;
		}
	case ESpriteDeleteL:
		{
		SpriteDeleteL();
		break;
		}
	case ESpriteUseL:
		{
		SpriteUseL();
		break;
		}
	case ETestCallbackL:
		{
		TestCallbackL();
		break;
		}
	case ETestAsyncL:
		{
		TestAsyncL();
		break;
		}
	default:
		User::Leave(KOplErrOpxProcNotFound);
		};
	}

TBool COpxData::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse; // bad version
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	{
	COpxData* theData=((COpxData*)Dll::Tls());
	if (theData==NULL)
		{
		theData=new(ELeave) COpxData(aOplAPI);
		CleanupStack::PushL(theData);
		theData->ConstructL();
		User::LeaveIfError(Dll::SetTls(theData));
		CleanupStack::Pop();
		}
	return theData;
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