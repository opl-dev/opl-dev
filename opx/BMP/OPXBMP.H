// OPXBMP.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include <e32base.h>
#include <opxapi.h>
#include <fbs.h>
#include <w32std.h>

#define KOpxVersion 0x600

class COplSprite : public CBase
	{
public:
	COplSprite(RWsSession& aWsSession);
	~COplSprite();
	RWsSprite& Sprite() {return iSprite;};
	TBool iDrawn;
private:
	RWsSprite iSprite;
	};

class COpxData : public COpxBase
	{
public:
	COpxData(OplAPI& aOplAPI);
	~COpxData();
	void ConstructL();
	TBool CheckVersion(TInt aVersion);
	void RunL(TInt aProcNum);
private:
	void LoadBitmapL();
	void UnloadBitmapL();
	void BitmapDisplayModeL();
	void SpriteCreateL();
	void SpriteAppendL();
	void SpriteChangeL();
	void SpriteDrawL();
	void SpritePosL();
	void SpriteDeleteL();
	void SpriteUseL();
	void TestCallbackL();
	void TestAsyncL();
private:
	void GetMemberL(TSpriteMember& aMem);
	TInt FindIndexL(CFbsBitmap* aHandle);
	TInt FindIndexL(COplSprite* aHandle);
private:
	CArrayPtrSeg<CFbsBitmap>* iBitmapArray;
	CArrayPtrSeg<COplSprite>* iSpriteArray;
	COplSprite* iCurrentSprite;
	};