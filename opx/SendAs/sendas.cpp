// SendAs.cpp - for SendAs OPX
//
// Copyright (c) 1999-2002 Symbian Ltd. All rights reserved.
//
// NOTE:	This OPX is based on the functionality found in the file
//			//EPOC/development/crystal/6.0/sendui/normSRC/Sendnorm.cpp
//
//			Original versions kept in step until Revision 19 of the above.
//			This OPX was given a major re-write on 24/07/2001 to bring the
//			code/approach in line with newer versions of the above file. For
//			some reason CSendAs is no longer used, with most of the sending
//			functionality being re-implemented.

#include <eikon.hrh>
#include <eikenv.h>
#include <eikhkeyt.h>
#include <w32std.h>
#include <bautils.h>
#include <mtuireg.h>
#include <mtmuibas.h>
#include <mtudreg.h>
#if defined(__UIQ__)
#include <QMappExtInterface.h>
#include <eikappui.h>
#else
#include <mtmstore.h>
#include <muiuServ.h>
#include <MTMExtendedCapabilities.hrh>
#endif
#include <mtmuids.h>
#include <msvapi.h>
#include <mtudcbas.h>
//
#include <MTUD.HRH> // EMtudCommandSendAs
#include <MSVIDS.H> // KMsvGlobalOutBoxIndexEntryId, etc.
#include <coeutils.h>
#include <txtrich.h>
#include <s32mem.h>
//
#include <IrcMtm.h>
//
#include <eikfutil.h>
//
#include "opxutil.h"
#include "sendasopx.h"

const TInt KSendingMtmGranularity		= 5;
const TInt KHotkeysGranularity			= 5;
const TInt KFileNamesArrayGranularity	= 5;
const TInt KAddressArrayGranularity		= 5;
const TInt32 KHotkeyStartValue			= 1;

const TUid KUidMsgTypeIR = {0x100053A4}; // See #include <irobutil.h>
const TInt KRichTextStoreGranularity = 512;

COpxSendAsEngine* COpxSendAsEngine::NewL()
	{
	COpxSendAsEngine* This=new(ELeave) COpxSendAsEngine();
	CleanupStack::PushL(This);
	This->ConstructL();
	CleanupStack::Pop();
	return This;
	}

COpxSendAsEngine::COpxSendAsEngine()
	:
#if !defined(__UIQ__)
	iSendingMtms(KSendingMtmGranularity),
#endif
	iHotkeysArray(KHotkeysGranularity),
	iCurrentChosenMtmUid(KNullUid)
	{
	}

void COpxSendAsEngine::ConstructL()
	{
	iSession = CMsvSession::OpenSyncL(*this);
#if !defined(__UIQ__)
	iMtmStore = CMtmStore::NewL(*iSession);
#endif
	iUiDataRegistry = CMtmUiDataRegistry::NewL(*iSession);
	iClientRegistry = CClientMtmRegistry::NewL(*iSession);
	iScannedAlready=EFalse;
	iStore = CBufStore::NewL(KRichTextStoreGranularity);
	}

COpxSendAsEngine::~COpxSendAsEngine()
	{
#if !defined(__UIQ__)
	delete iMtmStore;
#else
	delete iUiMtmData;
#endif
	delete iUiDataRegistry;
	delete iStore;
	delete iClientRegistry;
	delete iSession; // delete Session last after all things which use it

	delete iBody;
	delete iAttachmentFileNamesArray;
	delete iRealAddressesArray;
	delete iSubject;
	}

void COpxSendAsEngine::ScanMtmsL()
	{
#if !defined(__UIQ__)
	iSendingMtms.Reset();
#endif
	iHotkeysArray.Reset();
	iNextAvailableHotkey=KHotkeyStartValue;

	const TInt count = iUiDataRegistry->NumRegisteredMtmDlls();
	for (TInt cc = 0; cc < count; cc++)
		{
		TUid mtmTypeUid = iUiDataRegistry->MtmTypeUid(cc);
#if defined(__UIQ__)
		CBaseMtmUiData& uiData = *iUiDataRegistry->NewMtmUiDataLayerL(mtmTypeUid);
#else
		CBaseMtmUiData& uiData = iMtmStore->MtmUiDataL(mtmTypeUid);
#endif
		TInt response;
		TUid canSend = {KUidMtmQueryCanSendMsgValue};
		TInt err = uiData.QueryCapability(canSend, response);
		if (err != KErrNotSupported)
			User::LeaveIfError(err);
		if (err == KErrNone)
			{
			// Can send messages of this type
			TBool found = EFalse;
			for (TInt dd = 0; dd < iSendingMtms.Count(); ++dd)
				{
				if (iSendingMtms[dd].iUid == mtmTypeUid)
					{
					// Already have this MTM
					found = ETrue;
					break;
					}
				}
			if (!found)
				{
				const CArrayFix<CBaseMtmUiData::TMtmUiFunction>& funcs = uiData.MtmSpecificFunctions();
				for (TInt i = funcs.Count() - 1; i >= 0; i--)
					{
#if defined(__UIQ__)					
					if (funcs.At(i).iFlags & EMtudCommandSendAs)	//***** change iFlags to iFunctionType when the MTMs are corrected
						{
						found = ETrue;
						break;
						}
#else
					CBaseMtmUiData::TMtmUiFunction func = funcs.At(i);
					if (func.iFlags & EMtudCommandSendAs)	//***** change iFlags to iFunctionType when the MTMs are corrected
						{
						iSendingMtms.AppendL(TUidNameInfo(mtmTypeUid, func.iCaption));
						found = ETrue;
						break;
						}
#endif
					}
				if (!found)
					{
					// no info available for caption - use the human readable name
#if defined(__UIQ__)
					TPtrC name(iUiDataRegistry->RegisteredMtmDllInfo(mtmTypeUid).HumanReadableName());
					iSendingMtms.Append(TUidNameInfo(mtmTypeUid, name));
#else
					iSendingMtms.AppendL(TUidNameInfo(mtmTypeUid, iUiDataRegistry->RegisteredMtmDllInfo(mtmTypeUid).HumanReadableName()));
#endif
					}
				// Now set the hotkey for this MTM
#if defined(__UIQ__)
				iHotkeysArray.AppendL(iNextAvailableHotkey++);
#else
				iHotkeysArray.AppendL(iNextAvailableHotkey++);
#endif
				}
			}
		}
#if !defined(__UIQ__)
	iSendingMtms.Sort(ECmpFolded);
#endif
	iScannedAlready=ETrue;
	}

TInt COpxSendAsEngine::NumberOfMtmsL()
	{
	ScanIfNeededL();
	return iSendingMtms.Count();
	}

TBool COpxSendAsEngine::DoesMtmHaveCapabilityL(const TInt aIndex, TSendingCapabilities aRequiredCapabilities)
	{
	ScanIfNeededL();
	if ((aIndex<0) || (aIndex>=iSendingMtms.Count()))
		{
		User::Leave(KOplErrOutOfRange);
		}

	TUid mtmTypeUid = iSendingMtms[aIndex].iUid;
#if defined(__UIQ__)
	delete iUiMtmData;
	iUiMtmData =NULL;
	iUiMtmData= iUiDataRegistry->NewMtmUiDataLayerL(mtmTypeUid);//=iMtmStore->MtmUiDataL(mtmTypeUid);
	CBaseMtmUiData& mtmUiData=*iUiMtmData;
#else
	CBaseMtmUiData& mtmUiData = iMtmStore->MtmUiDataL(mtmTypeUid);
#endif
	TInt response = 0;

	// Check to see first if any valid accounts (if required) are setup
	TMsvEntry serviceEntry;
	serviceEntry.iType.iUid = KUidMsvServiceEntryValue;
	serviceEntry.iMtm = mtmTypeUid;
	CMsvEntry* rootEntry = iSession->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(rootEntry);

	TInt validAccounts = ETrue;
#if defined(__UIQ__)
	const TUid KUidQueryNeedsAccountButCannotCreate={0};
#else
	const TUid KUidQueryNeedsAccountButCannotCreate={KUidQueryNeedsAccountButCannotCreateValue};
#endif
	if ((mtmUiData.CanCreateEntryL(rootEntry->Entry(), serviceEntry, response)) ||
		(mtmUiData.QueryCapability(KUidQueryNeedsAccountButCannotCreate, response) == KErrNone))
		{
		CMsvEntrySelection* accounts = NULL;
#if !defined(__UIQ__)
		accounts = MsvUiServiceUtilities::GetListOfAccountsWithMTML(*iSession, mtmTypeUid, ETrue); 
#endif
		if (!accounts || accounts->Count() == 0)
			validAccounts = EFalse;
		delete accounts;
		}
	CleanupStack::PopAndDestroy(rootEntry);
	
	if (!validAccounts)
		return EFalse; // If no valid accounts, can't use MTM hence capability check is redundant

	// If we get to here, accounts are valid so there is the possibility to send messages - 
	// carry on and check the capabilities as passed.
	if (aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsAttachments)
		{
		if (mtmUiData.QueryCapability(KUidMtmQuerySupportAttachments, response) != KErrNone)
			return EFalse;
		}
	
	if (aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsBodyText)
		{
		if (mtmUiData.QueryCapability(KUidMtmQuerySupportedBody, response) != KErrNone)
			return EFalse;
		}

	if (aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsBioSending)
		{
		if (mtmUiData.QueryCapability(KUidMsvQuerySupportsBioMsg, response) != KErrNone)
			return EFalse;
		}
	
	if (aRequiredCapabilities.iFlags & TSendingCapabilities::ESupportsAttachmentsOrBodyText)
		{
		if ((mtmUiData.QueryCapability(KUidMtmQuerySupportAttachments, response) != KErrNone) &&
			(mtmUiData.QueryCapability(KUidMtmQuerySupportedBody, response) != KErrNone))
			return EFalse;
		}

	return ETrue;
	}

TPtrC COpxSendAsEngine::CascNameForMtmL(const TInt aIndex)
	{
	ScanIfNeededL();
	if ((aIndex<0) || (aIndex>=iSendingMtms.Count()))
		{
		User::Leave(KOplErrOutOfRange);
		}
	return iSendingMtms[aIndex].iName;
	}

TInt32 COpxSendAsEngine::HotkeyForMtmL(const TInt aIndex)
	{
	ScanIfNeededL();
	if ((aIndex<0) || (aIndex>=iSendingMtms.Count()))
		{
		User::Leave(KOplErrOutOfRange);
		}
	return iHotkeysArray[aIndex];
	}

TInt32 COpxSendAsEngine::NextAvailableHotkeyL()
	{
	ScanIfNeededL();
	return iNextAvailableHotkey;
	}

TInt32 COpxSendAsEngine::UidForMtmL(const TInt aIndex)
	{
	ScanIfNeededL();
	if ((aIndex<0) || (aIndex>=iSendingMtms.Count()))
		{
		User::Leave(KOplErrOutOfRange);
		}
	return iSendingMtms[aIndex].iUid.iUid;
	}

void COpxSendAsEngine::PrepareMessageL(const TInt aIndex)
	{
	ScanIfNeededL();
	if ((aIndex<0) || (aIndex>=iSendingMtms.Count()))
		{
		User::Leave(KOplErrOutOfRange);
		}
	iCurrentChosenMtmUid=iSendingMtms[aIndex].iUid;
	}

void COpxSendAsEngine::SetSubjectL(const TDesC& aSubject)
	{
	ScanIfNeededL();
	if (iCurrentChosenMtmUid==KNullUid)
		User::Leave(KOplErrNotExists);

#if defined(__UIQ__)
	delete iUiMtmData;
	iUiMtmData =NULL;
	iUiMtmData= iUiDataRegistry->NewMtmUiDataLayerL(iCurrentChosenMtmUid);//=iMtmStore->MtmUiDataL(mtmTypeUid);
	CBaseMtmUiData& mtmUiData=*iUiMtmData;
#else
	CBaseMtmUiData& mtmUiData = iMtmStore->MtmUiDataL(iCurrentChosenMtmUid);
#endif
	TInt response = 0;
	if (mtmUiData.QueryCapability(KUidMtmQuerySupportSubject, response) == KErrNone)
		{
		if(iSubject)
			{
			delete iSubject;
			iSubject=NULL;
			}
		iSubject=HBufC::NewL(aSubject.Length());
		*iSubject=aSubject;
		}
	else
		User::Leave(KOplErrNotSupported);
	}

void COpxSendAsEngine::AddFileL(const TFileName& aFileName)
	{
	ScanIfNeededL();
	if (iCurrentChosenMtmUid==KNullUid)
		User::Leave(KOplErrNotExists);

#if defined(__UIQ__)
	delete iUiMtmData;
	iUiMtmData =NULL;
	iUiMtmData= iUiDataRegistry->NewMtmUiDataLayerL(iCurrentChosenMtmUid);//=iMtmStore->MtmUiDataL(mtmTypeUid);
	CBaseMtmUiData& mtmUiData=*iUiMtmData;
#else
	CBaseMtmUiData& mtmUiData = iMtmStore->MtmUiDataL(iCurrentChosenMtmUid);
#endif
	TInt response = 0;
	if (mtmUiData.QueryCapability(KUidMtmQuerySupportAttachments, response) == KErrNone)
		{
		if(!iAttachmentFileNamesArray)
			iAttachmentFileNamesArray = new(ELeave) CDesCArrayFlat(KFileNamesArrayGranularity);
		iAttachmentFileNamesArray->AppendL(aFileName);
		}
	else
		User::Leave(KOplErrNotSupported);
	}

void COpxSendAsEngine::AddRecipientL(const TDesC& aAddress)
	{
	ScanIfNeededL();
	if (iCurrentChosenMtmUid==KNullUid)
		User::Leave(KOplErrNotExists);

	if(!iRealAddressesArray)
		iRealAddressesArray = new(ELeave) CDesCArrayFlat(KAddressArrayGranularity);
	iRealAddressesArray->AppendL(aAddress);
	}

void COpxSendAsEngine::LaunchSendL(OplAPI& /*aOplAPI*/)
	{
	ScanIfNeededL();
	if (iCurrentChosenMtmUid==KNullUid)
		User::Leave(KOplErrNotExists);

	// Create message
	CBaseMtm* mtm = iClientRegistry->NewMtmL(iCurrentChosenMtmUid);
	CleanupStack::PushL(mtm);

	CMsvEntry* draftEntry = iSession->GetEntryL(KMsvDraftEntryId);
	mtm->SetCurrentEntryL(draftEntry); // mtm takes ownership (and handles CleanupStack)
#if defined(__UIQ__)
	CMsvDefaultServices* defServices=new (ELeave) CMsvDefaultServices;
	CleanupStack::PushL(defServices);
	CMsvEntry* rootEntry=mtm->Session().GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(rootEntry);
	CMsvStore* rootStore= rootEntry->ReadStoreL();
	CleanupStack::PushL(rootStore);
	defServices->RestoreL(*rootStore);
	TMsvId service =0;
	defServices->DefaultService(iCurrentChosenMtmUid,service);
	CleanupStack::PopAndDestroy(2,rootEntry),
	CleanupStack::PopAndDestroy(defServices);
#else
	TMsvId service = MsvUiServiceUtilities::DefaultServiceForMTML(*iSession, iCurrentChosenMtmUid, ETrue);
#endif
	mtm->CreateMessageL(service);

	// Begin setting the message data
	TInt loop;
	if (iRealAddressesArray)
		{
		TInt count = iRealAddressesArray->MdcaCount();
		for (loop = 0; loop < count; loop++)
			{
			if (iRealAddressesArray->MdcaPoint(loop).Length() > 0) 
				mtm->AddAddresseeL(iRealAddressesArray->MdcaPoint(loop));
			}
		}

//	if (aBioTypeUid != KNullUid)
//		mtm->BioTypeChangedL(aBioTypeUid);

	if (iBody)
		SetBodyDuringSendingL(mtm, *iBody);

	if (iSubject)
		mtm->SetSubjectL(*iSubject);

	if (iAttachmentFileNamesArray)
		{
		if (iCurrentChosenMtmUid == KUidMsgTypeIR)
			AddIrAttachmentsL(mtm, iAttachmentFileNamesArray);
		else
			{
			TFileName directory;
			TMsvId attachId;
			TParse dest;
			for (loop = iAttachmentFileNamesArray->MdcaCount() - 1; loop >= 0; loop--)
				{
				mtm->CreateAttachmentL(attachId, directory);
				const TDesC& attachment = iAttachmentFileNamesArray->MdcaPoint(loop);
				dest.Set(directory, &attachment, NULL);
				User::LeaveIfError(EikFileUtils::CopyFile(attachment, dest.FullName()));

				CMsvEntry* newEntry = iSession->GetEntryL(attachId);
				CleanupStack::PushL(newEntry);

				// Store the attachment name in iDetails of the attachment entry
				TMsvEntry attachmentEntry = newEntry->Entry();
				const TDesC& attachmentName = dest.NameAndExt();
				attachmentEntry.iDetails.Set(attachmentName);

				TEntry attEntry;
				if (newEntry->Session().FileSession().Entry(dest.FullName(), attEntry) == KErrNone)
						attachmentEntry.iSize = attEntry.iSize;

				newEntry->ChangeL(attachmentEntry);
				CleanupStack::PopAndDestroy(newEntry);
				}
			}
		}

	// Save message
	mtm->SaveMessageL();

	// Set the time and make visible
	TMsvEntry entry = mtm->Entry().Entry();
	entry.iDate.HomeTime();
	entry.SetVisible(ETrue);
	entry.SetAttachment(iAttachmentFileNamesArray != NULL);
	entry.SetInPreparation(EFalse);
//	entry.iBioType = aBioTypeUid.iUid;

	mtm->Entry().ChangeL(entry);

	const TMsvId messageId = entry.Id();
	draftEntry = iSession->GetEntryL(KMsvDraftEntryId);
	CleanupStack::PushL(draftEntry);

#if defined(__UIQ__)
	TVwsViewId id(KUidMsgAppUid,KUidMsgListViewUid);
	TQMappDnlViewBuf viewMessage;
	viewMessage().iMessageId = messageId;
	CEikonEnv::Static()->EikAppUi()->ActivateViewL(id,KUidQMappViewMessage ,viewMessage );
#else
	TMsvEntry messageEntry = draftEntry->ChildDataL(messageId);
	// Edit the message
	CMsvOperationWait* waiter = CMsvOperationWait::NewLC();		
	CBaseMtmUi& mtmUi = iMtmStore->GetMtmUiAndSetContextLC(messageEntry);
	CMsvOperation* op = mtmUi.EditL(waiter->iStatus);
	CleanupStack::PushL(op);
	waiter->iStatus = KRequestPending;
	waiter->Start();
	CActiveScheduler::Start();

	const TDesC8& progressBuf = op->ProgressL();
	mtmUi.DisplayProgressSummary(progressBuf);

	CleanupStack::PopAndDestroy(op);
	CleanupStack::PopAndDestroy();		//mtmui releaser
	CleanupStack::PopAndDestroy(waiter);
#endif
	CleanupStack::PopAndDestroy(draftEntry);
	CleanupStack::PopAndDestroy(mtm);

	// Tidy up after sending - reset member data in preparation for the next send
	delete iAttachmentFileNamesArray;
	iAttachmentFileNamesArray=NULL;
	delete iRealAddressesArray;
	iRealAddressesArray=NULL;
	delete iSubject;
	iSubject=NULL;
	iCurrentChosenMtmUid=KNullUid;
	}
	
void COpxSendAsEngine::AddIrAttachmentsL(CBaseMtm* aMtm, MDesC16Array* aAttachments)
	{
	if (aMtm->Type() != KUidMsgTypeIR)
		User::Leave(KErrNotReady);

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection();
	CleanupStack::PushL(selection);
	selection->AppendL(aMtm->Entry().EntryId());

	TBuf8<1> nullParamater;
	aMtm->InvokeSyncFunctionL(CIrClientMtm::ESendasCmdStartExternalise, *selection, nullParamater);

	for (TInt i = 0; i < aAttachments->MdcaCount(); ++i)
		{
		TFileName name(aAttachments->MdcaPoint(i));
		TPtr8 ptr8((TUint8*)name.Ptr(), name.Size());
		ptr8.SetLength(name.Size());

		aMtm->InvokeSyncFunctionL(CIrClientMtm::ESendasCmdExternaliseFileName, *selection, ptr8);
		}
	
	aMtm->InvokeSyncFunctionL(CIrClientMtm::ESendasCmdCommitExternalise, *selection, nullParamater);
	CleanupStack::PopAndDestroy(selection);
	}

void COpxSendAsEngine::SetBodyDuringSendingL(CBaseMtm* aMtm, const CRichText& aMessageBody)
	{
	// Set the picture factory !!!
	MPictureFactory* pictureFactory = aMessageBody.PictureFactory();
	if (pictureFactory)
		aMtm->Body().SetPictureFactory(pictureFactory, this);

	// Save and restore the rich text
	TStreamId id = aMessageBody.StoreL(*iStore);
	aMtm->Body().RestoreL(*iStore, id);

	// Get access to protected data
	CSendAsMtm* mtm = (CSendAsMtm*)aMtm;

	mtm->SetCharFormatL(*aMessageBody.GlobalCharFormatLayer());
	mtm->SetParaFormatL(*aMessageBody.GlobalParaFormatLayer());
	}

const CStreamStore& COpxSendAsEngine::StreamStoreL(TInt) const
	{
	return *iStore;
	}

void COpxSendAsEngine::SetBodyL(CRichText* aMessageBody)
	{
	ScanIfNeededL();
	if ((iCurrentChosenMtmUid==KNullUid) || (!&aMessageBody))
		User::Leave(KOplErrNotExists);

#if defined(__UIQ__)
	delete iUiMtmData;
	iUiMtmData =NULL;
	iUiMtmData= iUiDataRegistry->NewMtmUiDataLayerL(iCurrentChosenMtmUid);//=iMtmStore->MtmUiDataL(mtmTypeUid);
	CBaseMtmUiData& mtmUiData=*iUiMtmData;
#else
	CBaseMtmUiData& mtmUiData = iMtmStore->MtmUiDataL(iCurrentChosenMtmUid);
#endif
	TInt response = 0;
	if (mtmUiData.QueryCapability(KUidMtmQuerySupportedBody, response) == KErrNone)
		{
		if (iBody!=aMessageBody)
			{
			delete iBody;
			iBody=NULL;
			iBody=aMessageBody;
			}
		}
	else
		User::Leave(KOplErrNotSupported);
	}

CRichText* COpxSendAsEngine::NewBodyL(const OplAPI& aOplAPI)
	{
	if (iBody)
		User::Leave(KOplErrExists);
	CEikonEnv& env=aOplAPI.EikonEnv();
	iBody=CRichText::NewL(env.SystemParaFormatLayerL(),env.SystemCharFormatLayerL());
	iBody->InsertL(0,KNullDesC);
	return iBody;
	}

void COpxSendAsEngine::DeleteBodyL()
	{
	if (!iBody)
		User::Leave(KOplErrNotExists);
	delete iBody;
	iBody=NULL;
	}

void COpxSendAsEngine::AppendToMessageBodyL(const TDesC& aString)
	{
	if (!iBody)
		User::Leave(KOplErrNotExists);
	iBody->InsertL(iBody->DocumentLength(),aString);
	}

void COpxSendAsEngine::ResetMessageBodyL()
	{
	if (!iBody)
		User::Leave(KOplErrNotExists);
	iBody->Reset();
	}

void COpxSendAsEngine::ScanIfNeededL()
	{
	if (!iScannedAlready)
		ScanMtmsL();
	}

void COpxSendAsEngine::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	switch (aEvent)
		{
	case EMsvServerReady:
		{
#if !defined(__UIQ__)
		iMtmStore = CMtmStore::NewL(*iSession);
#endif
		iUiDataRegistry = CMtmUiDataRegistry::NewL(*iSession);
		iClientRegistry = CClientMtmRegistry::NewL(*iSession);
		iScannedAlready=EFalse;
		ScanMtmsL();
		break;
		}
	case EMsvCorruptedIndexRebuilt:
	case EMsvMtmGroupInstalled:
	case EMsvMtmGroupDeInstalled:
		{
		ScanMtmsL();
		break;
		}
	case EMsvCloseSession:
	case EMsvServerTerminated:
		{
		iScannedAlready=EFalse;
#if !defined(__UIQ__)
		delete iMtmStore;
		iMtmStore=NULL;
#endif
		delete iUiDataRegistry;
		iUiDataRegistry=NULL;
		delete iClientRegistry;
		iClientRegistry=NULL;
		delete iSession;  // delete Session last after all things which use it
		iSession=NULL;

		delete iBody;
		iBody=NULL;
		delete iAttachmentFileNamesArray;
		iAttachmentFileNamesArray=NULL;
		delete iRealAddressesArray;
		iRealAddressesArray=NULL;
		delete iSubject;
		iSubject=NULL;
		//
		// delete iStore too? SendNorm.cpp doesn't
		//
		break;
		}
	default:
		break;
		}
	}

//////////////////////////////////////////////////////////////////////////////
//
// COpxSendAs - the OPX 'wrapper' and function handler, etc.
//
//////////////////////////////////////////////////////////////////////////////
COpxSendAs::COpxSendAs(OplAPI& aOplAPI) 
	:COpxBase(aOplAPI)
	{
	}

COpxSendAs* COpxSendAs::NewLC(OplAPI& aOplAPI)
	{
	COpxSendAs* This=new(ELeave) COpxSendAs(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	return This;
	}

void COpxSendAs::ConstructL()
	{
	iEngine = COpxSendAsEngine::NewL();
	}

COpxSendAs::~COpxSendAs()
	{
	delete iEngine;
	Dll::FreeTls();
	}

void COpxSendAs::ScanTypesL() const
	{
	iEngine->ScanMtmsL();
	iOplAPI.Push(0.0);
	}

void COpxSendAs::MaximumTypesL() const
	{
	iOplAPI.Push(TInt32(iEngine->NumberOfMtmsL()));
	}

void COpxSendAs::CapabilityCheckL() const
	{
	TSendingCapabilities aCapability(KSendAsMaxBodySizeNotUsed,KSendAsMaxMessageSizeNotUsed,iOplAPI.PopInt32());
	TInt32 aIndex = OpxUtil::CppIndex(iOplAPI.PopInt32());
	iOplAPI.Push(OpxUtil::OplBool16(iEngine->DoesMtmHaveCapabilityL(aIndex,aCapability)));
	}

void COpxSendAs::CascNameL() const
	{
	TInt32 aIndex = OpxUtil::CppIndex(iOplAPI.PopInt32());
	TPtrC text;
	TRAPD(err,text.Set(iEngine->CascNameForMtmL(aIndex)));
	if (err!=KErrNone)
		{
		iOplAPI.PushL(KNullDesC);
		}
	else
		{
		iOplAPI.PushL(text);
		}
	}

void COpxSendAs::HotkeyValueL() const
	{
	TInt32 aIndex = OpxUtil::CppIndex(iOplAPI.PopInt32());
	iOplAPI.Push(iEngine->HotkeyForMtmL(aIndex));
	}

void COpxSendAs::NextAvailableHotkeyL() const
	{
	iOplAPI.Push(iEngine->NextAvailableHotkeyL());
	}

void COpxSendAs::UidOfMtmL() const
	{
	TInt32 aIndex = OpxUtil::CppIndex(iOplAPI.PopInt32());
	iOplAPI.Push(iEngine->UidForMtmL(aIndex));
	}

void COpxSendAs::PrepareMessageL() const
	{
	TInt32 aIndex = OpxUtil::CppIndex(iOplAPI.PopInt32());
	iEngine->PrepareMessageL(aIndex);
	iOplAPI.Push(0.0);
	}

void COpxSendAs::SetSubject() const
	{
	TPtrC aSubject = iOplAPI.PopString();
	TRAPD(ret,iEngine->SetSubjectL(aSubject));
	iOplAPI.Push((TInt16)(ret));
	}

void COpxSendAs::AddFileL() const
	{
	TFileName aFileName=iOplAPI.PopString();
	if (!ConeUtils::FileExists(aFileName))
		User::Leave(KOplErrNotExists);
	iEngine->AddFileL(aFileName);
	iOplAPI.Push(0.0);
	}

void COpxSendAs::AddRecipientL() const
	{
	TPtrC aRecipient=iOplAPI.PopString();
	iEngine->AddRecipientL(aRecipient);
	iOplAPI.Push(0.0);
	}

void COpxSendAs::NewBodyL() const
	{
	CRichText* text=iEngine->NewBodyL(iOplAPI);
	iOplAPI.Push(TInt32(text));
	}

void COpxSendAs::DeleteBodyL() const
	{
	iEngine->DeleteBodyL();
	iOplAPI.Push(0.0);
	}

void COpxSendAs::AppendToBodyL() const
	{
	TPtrC aString = iOplAPI.PopString();
	iEngine->AppendToMessageBodyL(aString);
	iOplAPI.Push(0.0);
	}

void COpxSendAs::ResetBodyL() const
	{
	iEngine->ResetMessageBodyL();
	iOplAPI.Push(0.0);
	}

void COpxSendAs::SetBodyL() const
	{
	TInt32 aRichTextPointer = iOplAPI.PopInt32();
	if((aRichTextPointer & 0x3) !=0) // check its on a word boundary
		{
		User::Leave(KOplErrInvalidArgs);
		}
	iEngine->SetBodyL((CRichText*)aRichTextPointer); //*(CRichText*)aRichTextPointer);
	iOplAPI.Push(0.0);
	}

void COpxSendAs::LaunchSendL() const
	{
	iEngine->LaunchSendL(iOplAPI);
	iOplAPI.Push(0.0);
	}

void COpxSendAs::RunL(const TInt aProcNum)
	{
	switch (aProcNum)
		{
	case EMaximumTypes:
		MaximumTypesL();
		break;
	case ECapabilityCheck:
		CapabilityCheckL();
		break;
	case ECascName:
		CascNameL();
		break;
	case EHotkeyValue:
		HotkeyValueL();
		break;
	case ENextAvailableHotkey:
		NextAvailableHotkeyL();
		break;
	case EUidOfMtm:
		UidOfMtmL();
		break;
	case EPrepareMessage:
		PrepareMessageL();
		break;
	case ESetSubject:
		SetSubject();
		break;
	case EAddFile:
		AddFileL();
		break;
	case EAddRecipient:
		AddRecipientL();
		break;
	case ENewBody:
		NewBodyL();
		break;
	case EDeleteBody:
		DeleteBodyL();
		break;
	case EAppendToBody:
		AppendToBodyL();
		break;
	case EResetBody:
		ResetBodyL();
		break;
	case ESetBody:
		SetBodyL();
		break;
	case ELaunchSend:
		LaunchSendL();
		break;
	case EScanTypes:
		ScanTypesL();
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxSendAs::CheckVersion(const TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& iOplAPI)
	{
	COpxSendAs* tls=((COpxSendAs*)Dll::Tls());
	if (tls==NULL) // tls is NULL on loading an OPX DLL (also after unloading it)
		{
		tls=COpxSendAs::NewLC(iOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop(); // tls
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}
