// SendAsOpx.H
//
// Copyright (c) 1999-2002 Symbian Ltd. All rights reserved.

#ifndef __SENDASOPX_H__
#define __SENDASOPX_H__

#include <opxapi.h>
#include <oplerr.h>
#include <eikmenup.h>
#if !defined(__UIQ__)
#include <msgarrays.h> //for CUidNameArray
#include <sendui.h>
#endif
#include <txtmrtsr.h>

const TInt KUidSendAsOpx=0x100055BF;
const TInt KOpxVersion=0x100;

const TInt KSendAsMaxBodySizeNotUsed	=0;
const TInt KSendAsMaxMessageSizeNotUsed	=0;
#if defined(__UIQ__)
class TSendingCapabilities
	{
public:
	TSendingCapabilities(){};
	TSendingCapabilities(TInt aCap1,TInt aCap2,TInt aCap3){iFlags=aCap1|aCap2|aCap3;};
	enum TSendCapabilities
		{
		ESupportsAttachments=1,
		ESupportsBodyText=2,
		ESupportsBioSending=4,
		ESupportsAttachmentsOrBodyText=8
		};
	TInt iFlags;
	};
#endif

//
// Our engine class
//
class COpxSendAsEngine : public CBase, public MMsvSessionObserver, MRichTextStoreResolver
	{
public:
	static COpxSendAsEngine* NewL();
	~COpxSendAsEngine();
	//
	void ScanMtmsL();
	//
	TInt NumberOfMtmsL();
	TBool DoesMtmHaveCapabilityL(const TInt aIndex, TSendingCapabilities aRequiredCapabilities);
	TPtrC CascNameForMtmL(const TInt aIndex);
	TInt32 HotkeyForMtmL(const TInt aIndex);
	TInt32 NextAvailableHotkeyL();
	TInt32 UidForMtmL(const TInt aIndex);
	//
	void PrepareMessageL(const TInt aIndex);
	void SetSubjectL(const TDesC& aString);
	void AddFileL(const TFileName& aFileName);
	void AddRecipientL(const TDesC& aAddress);
	void LaunchSendL(OplAPI& aOplAPI);
	//
	void SetBodyL(CRichText* aMessageBody);
	CRichText* NewBodyL(const OplAPI& aOplAPI);
	void DeleteBodyL();
	void AppendToMessageBodyL(const TDesC& aString);
	void ResetMessageBodyL();
	//
	void ScanIfNeededL();
protected:
	// from MMsvSessionObserver
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
private:
	// from MRichTextStoreResolver
	const CStreamStore& StreamStoreL(TInt) const;
private:
	void SelectMtmL(const TInt aIndex);
	void AddIrAttachmentsL(CBaseMtm* aMtm, MDesC16Array* aAttachments);
	void SetBodyDuringSendingL(CBaseMtm* aMtm, const CRichText& aMessageBody);
private:
	COpxSendAsEngine();
	void ConstructL();
private:
	CMsvSession*			iSession;
#if !defined(__UIQ__)
	CMtmStore*				iMtmStore;
	CUidNameArray			iSendingMtms;
#else
	class  TUidNameInfo
		{
	public:
		TUidNameInfo(TUid aUid,TDesC& aName){iUid=aUid;iName=aName;};
		TUid iUid;
		TBuf<32> iName;
		};
	RArray<TUidNameInfo>     iSendingMtms;
#endif
	CMtmUiDataRegistry*		iUiDataRegistry;
	CClientMtmRegistry*		iClientRegistry;
	CRichText*				iBody;
	CArrayFixFlat<TInt32>	iHotkeysArray;
	TInt32					iNextAvailableHotkey;
	TBool					iScannedAlready;
	CDesCArray*				iAttachmentFileNamesArray;
	CDesCArray*				iRealAddressesArray;
	TUid					iCurrentChosenMtmUid;
	HBufC*					iSubject;
	CStreamStore*			iStore;
#if defined(__UIQ__)
	CBaseMtmUiData* iUiMtmData;
#endif
	};

//
// CSendAsMtm
//
class CSendAsMtm : public CBaseMtm
	{
public:
	CSendAsMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession) : CBaseMtm(aRegisteredMtmDll, aSession) {};
	//
public:
	inline CCharFormatLayer*& CharFormat() { return iCharFormatLayer; }
	inline CParaFormatLayer*& ParaFormat() { return iParaFormatLayer; }
	inline void SetCharFormatL(const CCharFormatLayer& aCharFormatLayer)
		{
		delete iCharFormatLayer;
		iCharFormatLayer = NULL;
		iCharFormatLayer = aCharFormatLayer.CloneL();
		}
	inline void SetParaFormatL(const CParaFormatLayer& aParaFormatLayer)
		{
		delete iParaFormatLayer;
		iParaFormatLayer = NULL;
		iParaFormatLayer = aParaFormatLayer.CloneL();
		}
	};

//
// The class COpxSendAs dispatches OPX procedures called by ordinal to the service routines
//
class COpxSendAs: public COpxBase
	{
public:
	static COpxSendAs* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
private:
	void ScanTypesL() const;
	void MaximumTypesL() const;
	void CapabilityCheckL() const;
	void CascNameL() const;
	void HotkeyValueL() const;
	void NextAvailableHotkeyL() const;
	void UidOfMtmL() const;
	//
	void PrepareMessageL() const;
	//
	void SetSubject() const;
	void AddFileL() const;
	void AddRecipientL() const;
	//
	void NewBodyL() const;
	void DeleteBodyL() const;
	void AppendToBodyL() const;
	void ResetBodyL() const;
	void SetBodyL() const;
	//
	void LaunchSendL() const;
private:
	COpxSendAs(OplAPI& aOplAPI);
	void ConstructL();
	~COpxSendAs();
	enum
		{
		EScanTypes=1,
		EMaximumTypes,
		ECapabilityCheck,
		ECascName,
		EHotkeyValue,
		ENextAvailableHotkey,
		EUidOfMtm,
		//
		EPrepareMessage,
		//
		ESetSubject,
		EAddFile,
		EAddRecipient,
		//
		ENewBody,
		EDeleteBody,
		EAppendToBody,
		EResetBody,
		ESetBody,
		//
		ELaunchSend
		};
	COpxSendAsEngine* iEngine;
	};

#endif
