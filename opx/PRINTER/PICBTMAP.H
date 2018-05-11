// PICBTMAP.H
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#ifndef __PICBTMAP_H_
#define __PICBTMAP_H_

#include <e32std.h>
#include <s32mem.h>
#include <gdi.h>
#include <e32hal.h>
#include <s32strm.h>
#include <apgicnfl.h>

class CPictureSimpleBitmap: public CPicture
	{
public:
	static CPictureSimpleBitmap* NewL(const TInt aFbsBitmapHandle, const TDisplayMode aDisplayMode);
	//
	void Draw(CGraphicsContext& aGc,const TPoint& aTopLeft,const TRect& aClipRect,MGraphicsDeviceMap* aMap=NULL) const;
	TPictureCapability Capability() const;
	TInt ScaleFactorWidth() const;
	TInt ScaleFactorHeight() const;
	void SetScaleFactor(TInt aScaleFactorWidth,TInt aScaleFactorHeight);
	void GetOriginalSizeInTwips(TSize& aOriginalSize) const;
	//
	void InternalizeL(RReadStream& /*aStream*/) {};
	void ExternalizeL(RWriteStream& /*aStream*/)const {};
	CPictureSimpleBitmap();
	~CPictureSimpleBitmap();
private: 
//	CPictureSimpleBitmap(TUid aIconFileUid,const TSize& aIconSizeInTwips);
	void ConstructL(const TInt aHandle, const TDisplayMode aDisplayMode);
//	void CreateIconL();
private:
//	CApaMaskedBitmap* iIcon; // null unless the current format is EIcon
//	HBufC* iAppInfoFileName;
//	TUid iAppInfoFileUid;
	CFbsBitmap* iFbsBitmap;
protected:
	TSize iOriginalSizeInTwips;
	TInt iScaleFactorWidth;		// Stored as percent * 10
	TInt iScaleFactorHeight;	// Stored as percent * 10
	TSize iDisplaySizeInPixels;
	TSize iDisplaySizeInTwips;
	};

#endif // __PICBTMAP_H_