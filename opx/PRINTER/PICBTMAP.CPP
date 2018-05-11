// PICBTMAP.CPP
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

//
// Class CPIctureSimpleBitmap
//
#include "picbtmap.h"
#define KDefaultIconSizeInPixels TSize(64,50)

CPictureSimpleBitmap* CPictureSimpleBitmap::NewL(const TInt aFbsBitmapHandle, const TDisplayMode aDisplayMode)
	{
	CPictureSimpleBitmap* self=new(ELeave) CPictureSimpleBitmap();
	CleanupStack::PushL(self);
	self->ConstructL(aFbsBitmapHandle,aDisplayMode);
	CleanupStack::Pop(); // self
	return self;
	}

CPictureSimpleBitmap::CPictureSimpleBitmap()
	:iScaleFactorWidth(1000), iScaleFactorHeight(1000)
	{
	}

void CPictureSimpleBitmap::ConstructL(const TInt aHandle, const TDisplayMode aDisplayMode)
	{
	iFbsBitmap = new (ELeave) CFbsBitmap();
	iFbsBitmap->Duplicate(aHandle);
	_LIT(KScreenDeviceName,"scdv.dll");
	CFbsScreenDevice *device = CFbsScreenDevice::NewL(KScreenDeviceName,aDisplayMode);
	iOriginalSizeInTwips = device->SizeInTwips();
	delete device;
	}

CPictureSimpleBitmap::~CPictureSimpleBitmap()
	{
	delete iFbsBitmap;
	}

void CPictureSimpleBitmap::Draw(CGraphicsContext& aGc,const TPoint& aTopLeft,const TRect& aClipRect,
						   MGraphicsDeviceMap* aMap) const
	{
	TSize sizeInPixels(0,0);
	sizeInPixels.iWidth = aMap->HorizontalTwipsToPixels(iOriginalSizeInTwips.iWidth);
	sizeInPixels.iHeight = aMap->VerticalTwipsToPixels(iOriginalSizeInTwips.iHeight);
	sizeInPixels.iWidth = (ScaleFactorWidth()*sizeInPixels.iWidth)/1000;
	sizeInPixels.iHeight = (ScaleFactorHeight()*sizeInPixels.iHeight)/1000;
	//
	TRect originalRectInPixels(aTopLeft,sizeInPixels);
	aGc.SetClippingRect(aClipRect);
	aGc.DrawBitmap(originalRectInPixels,iFbsBitmap);
	aGc.CancelClippingRect();
	}

void CPictureSimpleBitmap::GetOriginalSizeInTwips(TSize& aOriginalSize) const
	{
	aOriginalSize = iOriginalSizeInTwips;
	}

void CPictureSimpleBitmap::SetScaleFactor(TInt aScaleFactorWidth,TInt aScaleFactorHeight)
	{
	iScaleFactorWidth = aScaleFactorWidth;
	iScaleFactorHeight = aScaleFactorHeight;
	}

TInt CPictureSimpleBitmap::ScaleFactorWidth() const
	{
	return iScaleFactorWidth;
	}

TInt CPictureSimpleBitmap::ScaleFactorHeight() const
	{
	return iScaleFactorHeight;
	}

TPictureCapability CPictureSimpleBitmap::Capability() const
	{
	return TPictureCapability(TPictureCapability::EFullyScaleable,EFalse); // scalable, not cropable
	}