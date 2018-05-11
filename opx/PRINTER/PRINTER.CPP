// PRINTER.CPP
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#include "Printer.h"

//////////////////////////////////////////////////////////////////////////////
//																			//
//		The class CPRINTEROpx, which is in the format required by opx		//
//		will access COplPrintEngine which will do all the actual work		//
//																			//
//////////////////////////////////////////////////////////////////////////////
COplPrintEngine::~COplPrintEngine() 
	{
	delete iPrintSetup;
	delete iPrint;
	delete iParaFormatLayer;
	delete iCharFormatLayer;
	delete iMyRichText;
	delete iCharsPerPage;
	delete iParaFormat;
	}

COplPrintEngine* COplPrintEngine::NewL(OplAPI& aOplAPI)
	{
	COplPrintEngine* This=new(ELeave) COplPrintEngine();
	CleanupStack::PushL(This);
	This->ConstructL(aOplAPI);
	CleanupStack::Pop();
	return This;
	}

void COplPrintEngine::ConstructL(OplAPI& aOplAPI) 
	{
	iInitialised = EFalse;
	iDocumentChanged =ETrue;
	iPaginateDialogCalled = EFalse;
	iOplAPI = &aOplAPI;
	iFormatChanged = EFalse;
	_LIT(KArialName,"Arial"); // !! This probably shouldn't be hard-coded
	iFontName = KArialName;
	iFontHeight = 178;

	iParaFormat = CParaFormat::NewL();
	DefaultFormatting();

	iParaFormatLayer = CParaFormatLayer::NewL();
	iCharFormatLayer = CCharFormatLayer::NewL(iCharFormat,iCharFormatMask);
	iMyRichText=CRichText::NewL(iParaFormatLayer,iCharFormatLayer);
	// iRichText is the current doc, can change in SendRichTextToPrinter
	// Keep a reference to the original though, so we can delete it
	iRichText = iMyRichText;
	iPrintSetup = aOplAPI.EikonEnv().NewDefaultPrintSetupL();

	iPrintSetup->Header()->SetFileNameInfo(*this); 
	iPrintSetup->Header()->SetNumPagesInfo(*this); 
	iPrintSetup->Footer()->SetFileNameInfo(*this); 
	iPrintSetup->Footer()->SetNumPagesInfo(*this); 
	
	iPrint= CTextPageRegionPrinter::NewL(iRichText,iPrintSetup->PrinterDevice());
	iCharsPerPage=new(ELeave) CArrayFixFlat<TInt>(5);
	iPrint->SetPageList(iCharsPerPage);
	iPrintParameters.iFirstPage=0;
	iPrintParameters.iNumCopies=1;
	iPrint->SetFirstPageOfDoc(0);
	iPrint->SetPageMarginsInTwips(iPrintSetup->iPageMarginsInTwips.iMargins);
	iPrint->SetPageSpecInTwips(iPrintSetup->PrinterDevice()->CurrentPageSpecInTwips());
	iRichText->InsertL(0,KNullDesC);
	}

TInt COplPrintEngine::UpdateFieldFileName(TPtr& aValueText) const
// Set aValueText with the current document filename only. // (No path or extension).
	{
	TParse parser;

	User::LeaveIfError(parser.Set(iOplAPI->EikonEnv().Process()->MainDocFileName(),NULL,NULL));
	if (parser.Name().Length()>aValueText.MaxLength())
		return parser.Name().Length();
	aValueText=parser.Name();
	return 0;
	}

TInt COplPrintEngine::UpdateFieldNumPages(void)const
	{
	return (iCharsPerPage->Count());
	}

void COplPrintEngine::RunPageSetupDialogL() 
	{		
	TBool cancel=EFalse;
	if (iDocumentChanged)
		cancel=RunPaginationDialogL();
	if (cancel)
		return;
#if !defined(__UIQ__)
	if (CEikPageSetupDialog::RunDlgLD(iPrintSetup))	
		{
		iPrint->SetPageMarginsInTwips(iPrintSetup->iPageMarginsInTwips.iMargins);
		iPrint->SetPageSpecInTwips(iPrintSetup->PrinterDevice()->CurrentPageSpecInTwips());
		iDocumentChanged=ETrue;
		}
#endif
	}

void COplPrintEngine::RunPrintPreviewDialogL() 
	{
	TBool cancel=EFalse;
	if (iDocumentChanged)
		cancel=RunPaginationDialogL();
	if (cancel)
		return;
	
	iPrint->SetPrintPreview(ETrue);
#if !defined(__UIQ__)
	TInt totalNumPages=	iCharsPerPage->Count(); // hard coded this before
	if (CEikPrintPreviewDialog::RunDlgLD(*iPrintSetup,*iPrint,totalNumPages,this,4))
		{
		iPrint->SetPrintPreview(EFalse);
		CEikPrintProgressDialog::RunDlgLD(iPrintSetup,iPrint,iPrintParameters);
		}
#endif
	iPrintSetup->FreeModelList();
	}

/*
  This method is required by MEikEdwinOberver
*/
TBool COplPrintEngine::RunPrintRangeDialogL(CPrintSetup* aPrintSetup, TInt& aNumPagesInDoc)
	{
	TUid olduid = aPrintSetup->PrinterDevice()->Model().iUid;
	TUid uid = olduid;
	iPrintParameters.iFirstPage=0;
	iPrintParameters.iLastPage=aNumPagesInDoc-1;
#if !defined(__UIQ__)
	if (CEikPrintRangeDialog::RunDlgLD(iPrintParameters,aPrintSetup,uid))
		{
		if (olduid!=uid)	
			{
			aPrintSetup->CreatePrinterDeviceL(uid,iOplAPI->EikonEnv().FsSession());
			iPrint->SetPrinterDevice(aPrintSetup->PrinterDevice());
			iDocumentChanged = ETrue;
			if (RunPaginationDialogL())
				return ETrue;
			}
		}
#endif
	aNumPagesInDoc = iCharsPerPage->Count();
	iPrintSetup->FreeModelList();
	return EFalse; //no cancellation
	}

//
// From CEikGlobalTextEditor
//
TBool COplPrintEngine::RunPaginationDialogL()
	{
	iPaginateDialogCalled = ETrue;
	TBool cancel=EFalse;
#if !defined(__UIQ__)
	CTextPaginator* paginator=CTextPaginator::NewL(iPrintSetup->PrinterDevice(),iCharsPerPage,0); // Priority 0
	CleanupStack::PushL(paginator);
	paginator->SetPageMarginsInTwips(iPrintSetup->iPageMarginsInTwips.iMargins);
	paginator->SetPageSpecInTwips(iPrintSetup->PrinterDevice()->CurrentPageSpecInTwips());
	paginator->SetDocumentL(iRichText); // takes an MLayDoc
	if (!CEikPaginateDialog::RunDlgLD(paginator))
		cancel=ETrue;
#endif
	CleanupStack::PopAndDestroy(); //paginator;
	iDocumentChanged =EFalse; // Prevent repagination
	iPrintParameters.iFirstPage = 0;
	iPrintParameters.iLastPage = iCharsPerPage->Count()-1;
	return cancel;
	}

void COplPrintEngine::RunPrintDialogL()
	{
	TBool cancel=EFalse;
	if (iDocumentChanged)
		cancel=RunPaginationDialogL();
	if (cancel)
		return;
	TUid olduid=iPrintSetup->PrinterDevice()->Model().iUid; 	
	TUid uid=olduid;
#pragma message ("*** the following two lines produce an alloc heaven on UIKON 308. It's demonstrable with the test code")	
#if !defined(__UIQ__)
	CEikDialog* rangedialog=new(ELeave) CEikPrintRangeDialog(iPrintParameters,iPrintSetup,uid,ETrue);
	if (rangedialog->ExecuteLD(R_EIK_DIALOG_PRINT_RANGE_SETUP))
		{
		if (olduid!=uid)
			{
			iPrintSetup->CreatePrinterDeviceL(uid,iOplAPI->EikonEnv().FsSession());
			iPrint->SetPrinterDevice(iPrintSetup->PrinterDevice());
			iDocumentChanged=ETrue;
			}
		}
	else
#endif
		return;
	if (iDocumentChanged)
		cancel=RunPaginationDialogL();
	if (cancel)
		return;
	//progress
	iPrint->SetPrintPreview(EFalse);
#if !defined(__UIQ__)
 	CEikPrintProgressDialog::RunDlgLD(iPrintSetup,iPrint,iPrintParameters);
#endif
	iPrintSetup->FreeModelList();
	}

TBool COplPrintEngine::PageSetupChangedL(CPrintSetup* aPrintSetup,TInt& aNumPagesInDoc)
	{
	iPrint->SetPageMarginsInTwips(aPrintSetup->iPageMarginsInTwips.iMargins);
	iPrint->SetPageSpecInTwips(aPrintSetup->PrinterDevice()->CurrentPageSpecInTwips());
	iDocumentChanged=ETrue;
	if (RunPaginationDialogL())
		return ETrue;
	aNumPagesInDoc=iCharsPerPage->Count();
	return EFalse;
	}

void COplPrintEngine::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
	{
	if (aEventType==EEventStateChanged)
		iDocumentChanged=ETrue;
	if (iPaginateDialogCalled && aEventType==EEventRequestExit)
		{ // !! possibly better - the dialog should define its own observer class
		delete aControl;
		iDocumentChanged=EFalse;
		iPaginateDialogCalled=EFalse;
		}
	}

void COplPrintEngine::ResetPrinting()
	// Remove all content and formatting
	{
	iRichText->Reset();
	iFormatChanged = ETrue;
	iDocumentChanged = ETrue;
	DefaultFormatting();
	iRichText->InsertL(0,KNullDesC);
	}

void COplPrintEngine::DefaultFormatting()
	// Reset the formatting
	{
	TCharFormat defaultCharFormat(iFontName,iFontHeight);
	iCharFormat = defaultCharFormat;
	iCharFormatMask.ClearAll();
	iCharFormatMask.SetAttrib(EAttFontTypeface);
	iCharFormatMask.SetAttrib(EAttFontHeight);
	_LIT(KScreenDeviceName,"scdv.dll");
	CFbsScreenDevice *device = CFbsScreenDevice::NewL(KScreenDeviceName,DefaultDisplayMode());
	iCharFormat.iFontSpec.iTypeface.SetAttributes(FontUtils::TypefaceAttributes(*device,iFontName));
	delete device;
	}

TDisplayMode COplPrintEngine::DefaultDisplayMode() const
	{
	TInt numGrays=0;
	TInt numColors=0;
	TDisplayMode defaultMode=iOplAPI->WsSession().GetDefModeMaxNumColors(numColors,numGrays);
	if (defaultMode != EColor16M)
		{
		return defaultMode;
		}
	else
		{
		return EColor4K;
		}
	}

void COplPrintEngine::InsertStringL(TDesC& aString, const TInt aPos, TTextInsertPosition aTIP)
	// Insert a string at the given position if aTIP == EInsert. Append otherwise
	{
	TInt pos;
	pos = SetInsertPositon(aPos, aTIP);
	iDocumentChanged=ETrue;
	if(iFormatChanged)
		{
		ResetFormatL(pos);
		}
	iRichText->InsertL(pos, aString);
	
	}

void COplPrintEngine::SetFontNameL(TDesC& aName)
	{
	iDocumentChanged=ETrue;
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontTypeface);
	_LIT(KScreenDeviceName,"scdv.dll");
	CFbsScreenDevice *device = CFbsScreenDevice::NewL(KScreenDeviceName,DefaultDisplayMode());
	iCharFormat.iFontSpec.iTypeface.SetAttributes(FontUtils::TypefaceAttributes(*device,aName));
	iCharFormat.iFontSpec.iTypeface.iName = aName;
	delete device;
	}

void COplPrintEngine::SetFontHeight(const TInt16 aHeight)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontHeight);
	iCharFormat.iFontSpec.iHeight = aHeight;
	}

void COplPrintEngine::SetFontPosition(const TInt16 aPosition)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontPrintPos);
	iCharFormat.iFontSpec.iFontStyle.SetPrintPosition((TFontPrintPosition)aPosition);
	}

void COplPrintEngine::SetFontWeight(const TInt16 aWeight)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontStrokeWeight);
	iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight((TFontStrokeWeight)aWeight);
	}

void COplPrintEngine::SetFontPosture(const TInt16 aPosture)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontPosture);
	iCharFormat.iFontSpec.iFontStyle.SetPosture((TFontPosture)aPosture);
	}

void COplPrintEngine::SetFontStrikethrough(const TInt16 aStrikethrough)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontStrikethrough);
	iCharFormat.iFontPresentation.iStrikethrough = (TFontStrikethrough)aStrikethrough;
	}

void COplPrintEngine::SetFontUnderline(const TInt16 aUnderline)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontUnderline);
	iCharFormat.iFontPresentation.iUnderline = (TFontUnderline)aUnderline;
	}

void COplPrintEngine::ResetFormatL(const TInt aPos)
	// If the format has been changed since insert was last called
	// the this member is called to enter the new format at the insert position.
	{
	iRichText->CancelInsertCharFormat();
	if (RemoveUnnessaryCharSettings(&iCharFormat,&iCharFormatMask,aPos))
		iRichText->SetInsertCharFormatL(iCharFormat, iCharFormatMask, aPos);
	iFormatChanged = EFalse;
	}

void COplPrintEngine::InsertBitmapL(const TInt aPos,const TInt aHandle, TTextInsertPosition aTIP, TInt aXScale, TInt aYScale)
	// Insert at the position if aTIP == EInsert, append otherwise
	{
	TInt pos;
	pos = SetInsertPositon(aPos, aTIP);
	iDocumentChanged=ETrue;
	CPictureSimpleBitmap* p = CPictureSimpleBitmap::NewL(aHandle,DefaultDisplayMode());
	p->SetScaleFactor(aXScale, aYScale);
	TPictureHeader h;
	h.iPicture = p;
	iRichText->InsertL(pos, h);
	}

TInt COplPrintEngine::NumberOfPagesInDoc() 
	{
	if (iDocumentChanged)
		RunPaginationDialogL();
	return (iCharsPerPage->Count());
	}

//
// This is from MPrintPreviewObserve
//
void COplPrintEngine::HandleEdwinEventL(CEikEdwin* ,TEdwinEvent) 
	{
	}

TInt COplPrintEngine::SetInsertPositon(const TInt aPos, const TTextInsertPosition aTIP)
	// Set the insert position, called by the InsertXXX members.
	{
	TInt last = iRichText->DocumentLength();
	if(aTIP == EInsert)
		{
		if(aPos >= 0 && aPos <= last)
			return aPos;
		User::Leave(KOplErrInvalidArgs); // OPL error code
		}
	return(last);
	}

void COplPrintEngine::SendRichTextToPrinterL(CRichText * aRichText)
	// This replaces the content and formatting.
	{
	iFormatChanged = ETrue;
	iRichText = aRichText;
	iPrint->SetDocument(iRichText);
	DefaultFormatting();
	}

void COplPrintEngine::SetAlignmentL(CParaFormat::TAlignment aAlignment)
	// Alignment can also be set with InitialiseParaFormatL. 
	{
	iFormatChanged = ETrue;
	iParaFormat->iHorizontalAlignment = aAlignment; 
	iParaFormatMask.SetAttrib(EAttAlignment); // interested in alignment
	}

void COplPrintEngine::InitialiseParaFormatL(TInt16 aRed,
											TInt16 aGreen,
											TInt16 aBlue,
											TInt32 aLeftMarginInTwips,
											TInt32 aRightMarginInTwips,
											TInt32 aIndentInTwips,
											CParaFormat::TAlignment aHorizontalAlignment,
											CParaFormat::TAlignment aVerticalAlignment,
											TInt32 aLineSpacingInTwips,
											CParaFormat::TLineSpacingControl aLineSpacingControl,
											TInt32 aSpaceBeforeInTwips,
											TInt32 aSpaceAfterInTwips,
											TBool aKeepTogether,
											TBool aKeepWithNext,
											TBool aStartNewPage,
											TBool aWidowOrphan,
											TBool aWrap,
											TInt32 aBorderMarginInTwips,
											TUint32 aDefaultTabWidthInTwips
											)
	// Set the pargraph format. Has no immediate effect.
	// Call Set[Local|Global]ParaFormat to apply formatting
	{
	iFormatChanged = ETrue;
	iParaFormat->iFillColor = TRgb(aRed, aGreen, aBlue);
	iParaFormatMask.SetAttrib(EAttFillColor);
	iParaFormat->iLeftMarginInTwips = aLeftMarginInTwips;
	iParaFormatMask.SetAttrib(EAttLeftMargin);
	iParaFormat->iRightMarginInTwips = aRightMarginInTwips;
	iParaFormatMask.SetAttrib(EAttRightMargin);
	iParaFormat->iIndentInTwips = aIndentInTwips;
	iParaFormatMask.SetAttrib(EAttIndent);
	iParaFormat->iHorizontalAlignment = aHorizontalAlignment;
	iParaFormatMask.SetAttrib(EAttAlignment);
	iParaFormat->iVerticalAlignment = aVerticalAlignment;
	iParaFormatMask.SetAttrib(EAttVerticalAlignment);
	iParaFormat->iLineSpacingInTwips = aLineSpacingInTwips;
	iParaFormatMask.SetAttrib(EAttLineSpacing);
	iParaFormat->iLineSpacingControl = aLineSpacingControl;
	iParaFormatMask.SetAttrib(EAttLineSpacingControl);
	iParaFormat->iSpaceBeforeInTwips = aSpaceBeforeInTwips;
	iParaFormatMask.SetAttrib(EAttSpaceBefore);
	iParaFormat->iSpaceAfterInTwips = aSpaceAfterInTwips;
	iParaFormatMask.SetAttrib(EAttSpaceAfter);
	iParaFormat->iKeepTogether = aKeepTogether;
	iParaFormatMask.SetAttrib(EAttKeepTogether);
	iParaFormat->iKeepWithNext = aKeepWithNext;
	iParaFormatMask.SetAttrib(EAttKeepWithNext);
	iParaFormat->iStartNewPage = aStartNewPage;
	iParaFormatMask.SetAttrib(EAttStartNewPage);
	iParaFormat->iWidowOrphan = aWidowOrphan;
	iParaFormatMask.SetAttrib(EAttWidowOrphan);
	iParaFormat->iWrap = aWrap;
	iParaFormatMask.SetAttrib(EAttWrap);
	iParaFormat->iBorderMarginInTwips = aBorderMarginInTwips;
	iParaFormatMask.SetAttrib(EAttBorderMargin);
	iParaFormat->iDefaultTabWidthInTwips = aDefaultTabWidthInTwips;
	iParaFormatMask.SetAttrib(EAttDefaultTabWidth);
	}

void COplPrintEngine::SetLocalParaFormatL() 
	// Set the paragraph format created by InitialiseParaFormat or SetAlignment as the local paragraph format
	{
	iFormatChanged = ETrue;
	TInt pos = iRichText->DocumentLength();
	CParaFormat* paraFormat = CParaFormat::NewL();
	TParaFormatMask paraFormatMask;
	iRichText->GetParaFormatL(paraFormat, paraFormatMask, pos,0);
	if(!(iParaFormat->IsEqual(*paraFormat, paraFormatMask) && paraFormatMask == iParaFormatMask))
		{
		if (RemoveUnnessaryParaSettings(iParaFormat,&iParaFormatMask,pos))
			iRichText->ApplyParaFormatL(iParaFormat, iParaFormatMask, pos, 0);
		}
	delete paraFormat;
	}

void COplPrintEngine::SetGlobalParaFormatL() 
	// Set the paragraph format created by InitialiseParaFormat or SetAlignment as the global paragraph format
	{
	iFormatChanged = ETrue;
	CParaFormat* paraFormat = CParaFormat::NewL();
	TParaFormatMask paraFormatMask;
	iRichText->CGlobalText::GetParaFormatL(paraFormat, paraFormatMask, 0,0);
	if(!(iParaFormat->IsEqual(*paraFormat, paraFormatMask) && paraFormatMask == iParaFormatMask))
		iRichText->CGlobalText::ApplyParaFormatL(iParaFormat, iParaFormatMask, 0,0);
	delete paraFormat;
	}

void COplPrintEngine::RemoveSpecificParaFormatL()
	// Get rid of the local paragraph format, so the global takes effect
	{
	iRichText->RemoveSpecificParaFormatL(iRichText->DocumentLength(),0);
	}

void COplPrintEngine::SetGlobalCharFormatL() 
	// Set the current character format to be the global default
	{
	TCharFormatMask mask;
	mask.SetAll();
	iRichText->CGlobalText::ApplyCharFormatL(iCharFormat, mask, 0,0);
	}

void COplPrintEngine::RemoveSpecificCharFormatL()
	// Make the character format take on global attributes
	{
	const CCharFormatLayer *pLay = iRichText->GlobalCharFormatLayer();
	iCharFormatMask.ClearAll();
	pLay->Sense(iCharFormat,iCharFormatMask);
	iFormatChanged = ETrue;
	}

void COplPrintEngine::InsertSpecialCharacterL(TInt16 aSpecialChar, const TInt aPos, TTextInsertPosition aTIP)
	// Insert a special character (as defined by CEditableText) at aPos if aTIP == EInsert. Append otherwise
	{
	iFormatChanged = ETrue;
	TInt pos;
	pos = SetInsertPositon(aPos, aTIP);
	iRichText->InsertL(pos, aSpecialChar);
	}
	
TInt COplPrintEngine::DocumentLength()
	// Return number of chars (including special chars, bitmaps count as 1).
	// This is called when appending characters for example
	{
	return iRichText->DocumentLength();
	}

//
// The following 4 functions are based on ones by Gareth Richards. 
// Some changes had to be made
//
TBool COplPrintEngine::RemoveUnnessaryParaSettings(const CParaFormat *pParaFormat,TParaFormatMask *pParaMask,long pos)
	{
	CParaFormat ParaFormat;
	TParaFormatMask Mask;
	if(pos != 0 )
		{	// Make sure we get the Format for the previous position, but don't go -ve.
		pos--;
		}
	TRAPD(res,iRichText->GetParaFormatL(&ParaFormat,Mask,pos,0));
	if (res!=0)
		return 0;
	return RemoveUnnessaryParaSettings2(&ParaFormat,pParaFormat,pParaMask);
	}

TBool COplPrintEngine::RemoveUnnessaryCharSettings(const TCharFormat *pCharFormat,TCharFormatMask *pCharMask,long Pos)
	{
	TCharFormat CharFormat;
	TCharFormatMask Mask;
	iRichText->GetCharFormat(CharFormat,Mask, Pos, 0);
	return RemoveUnnessaryCharSettings2(&CharFormat,pCharFormat,pCharMask);
	}

TBool COplPrintEngine::RemoveUnnessaryParaSettings2(const CParaFormat *pBaseParaFormat,const CParaFormat *pParaFormat,TParaFormatMask *pParaMask)
	{
	TParaFormatMask Mask;
	TBool SetSomething=EFalse;
	for (int ii=EAttParaLanguage; ii<EAttTabStop; ii++)
		{
		if (pParaMask->AttribIsSet((TTextFormatAttribute)ii))
			{
			Mask.ClearAll();
			Mask.SetAttrib((TTextFormatAttribute)ii);
			if (pParaFormat->IsEqual(*pBaseParaFormat,Mask))
				pParaMask->ClearAttrib((TTextFormatAttribute)ii);
			else
				SetSomething=ETrue;
			}
		}
	return SetSomething;
	}

TBool COplPrintEngine::RemoveUnnessaryCharSettings2(const TCharFormat *pBaseCharFormat,const TCharFormat *pCharFormat,TCharFormatMask *pCharMask)
	{
	TCharFormatMask Mask;
	TBool SetSomething=EFalse;
	for (int ii=EAttCharLanguage; ii<ETextFormatAttributeCount; ii++)
		{
		if (pCharMask->AttribIsSet((TTextFormatAttribute)ii))
			{
			Mask.ClearAll();
			Mask.SetAttrib((TTextFormatAttribute)ii);
			if (pCharFormat->IsEqual(*pBaseCharFormat,Mask))
				pCharMask->ClearAttrib((TTextFormatAttribute)ii);
			else
				SetSomething=ETrue;
			}
		}
	return SetSomething;
	}

//////////////////////////////////////////////////////////////////////////////////////////
//																						//
//		CPRINTEROpx, a proxy class to provide a opx interface to COplPrintEngine		//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
CPRINTEROpx::CPRINTEROpx(OplAPI& aOplAPI) 
	:COpxBase(aOplAPI)
	{
	}

CPRINTEROpx* CPRINTEROpx::NewL(OplAPI& aOplAPI)
	{
	CPRINTEROpx* This=new(ELeave) CPRINTEROpx(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	CleanupStack::Pop();
	return This;
	}

void CPRINTEROpx::ConstructL()
	{
	iOplPrintEngine = COplPrintEngine::NewL(iOplAPI);
	} 

CPRINTEROpx::~CPRINTEROpx()
	{
	delete iOplPrintEngine;
	Dll::FreeTls(); // Required so that Tls is set to zero on unloading the OPX in UNLOADM
	}

//
// Get the function by ordinal.
//
void CPRINTEROpx::RunL(TInt aProcNum)
	{
	switch(aProcNum)
		{
	case ESendStringToPrinter:
		SendStringToPrinterL();
		break;
	case EInsertString:
		InsertStringL();
		break;
	case ESendNewParaToPrinter: 
		SendNewParaToPrinterL();
		break;
	case EInsertNewPara: 
		InsertNewParaL();
		break;
	case ESendSpecialCharToPrinter: 
		SendSpecialCharToPrinterL();
		break;
	case EInsertSpecialChar: 
		InsertSpecialCharL();
		break;
	case ESetAlignment: 
		SetAlignmentL();
		break;
	case EInitialiseParaFormat: 
		InitialiseParaFormatL();
		break;
	case ESetLocalParaFormat: 
		SetLocalParaFormatL();
		break;
	case ESetGlobalParaFormat: 
		SetGlobalParaFormatL();
		break;
	case ERemoveSpecificParaFormat: 
		RemoveSpecificParaFormatL();
		break;
	case ESetFontName: 
		SetFontNameL();
		break;
	case ESetFontHeight: 
		SetFontHeight();
		break;
	case ESetFontPosition: 
		SetFontPosition();
		break;
	case ESetFontWeight: 
		SetFontWeight();
		break;
	case ESetFontPosture: 
		SetFontPosture();
		break;
	case ESetFontStrikethrough: 
		SetFontStrikethrough();
		break;
	case ESetFontUnderline: 
		SetFontUnderline();
		break;
	case ESetGlobalCharFormat: 
		SetGlobalCharFormatL();
		break;
	case ERemoveSpecificCharFormat: 
		RemoveSpecificCharFormatL();
		break;
	case ESendBitmapToPrinter: 
		SendBitmapToPrinterL();
		break;
	case EInsertBitmap: 
		InsertBitmapL();
		break;
	case ESendScaledBitmapToPrinter: 
		SendScaledBitmapToPrinterL();
		break;
	case EInsertScaledBitmap: 
		InsertScaledBitmapL();
		break;
	case EPrinterDocLength: 
		PrinterDocLength();
		break;
	case ESendRichTextToPrinter: 
		SendRichTextToPrinterL();
		break;
	case EResetPrinting: 
		ResetPrinting();
		break;
	case EPageSetupDialog: 
		PageSetupDialogL();
		break;
	case EPrintPreviewDialog: 
		PrintPreviewDialogL();
		break;
	case EPrintRangeDialog: 
		PrintRangeDialogL();
		break;
	case EPrintDialog:
		PrintDialogL();
		break;
	case ESendBufferToPrinter:
		SendBufferToPrinterL();
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}


//////////////////////////////////////////////////////////////////////////////
//																			//
//				User functions start here									//
//																			//
//////////////////////////////////////////////////////////////////////////////
void CPRINTEROpx::SendStringToPrinterL() 
	{
	TPtrC string = iOplAPI.PopString();
	iOplPrintEngine->InsertStringL(string,0, EAppend);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::InsertStringL()
	{
	TInt pos = iOplAPI.PopInt32();
	TPtrC string = iOplAPI.PopString();
	iOplPrintEngine->InsertStringL(string, pos);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SendNewParaToPrinterL()
	{
	iOplPrintEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::InsertNewParaL()
	{
	TInt pos = iOplAPI.PopInt32();
	iOplPrintEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter, pos);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SendSpecialCharToPrinterL()
	{
	TInt16 c = iOplAPI.PopInt16();
	iOplPrintEngine->InsertSpecialCharacterL(c,0,EAppend);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::InsertSpecialCharL()
	{
	TInt pos = iOplAPI.PopInt32();
	TInt16 c = iOplAPI.PopInt16();
	iOplPrintEngine->InsertSpecialCharacterL(c, pos);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetAlignmentL()
	{
	TInt16 alignment = iOplAPI.PopInt16();
	iOplPrintEngine->SetAlignmentL((CParaFormat::TAlignment)alignment);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::InitialiseParaFormatL()
	{	
	TUint32 aDefaultTabWidthInTwips = iOplAPI.PopInt32();
	TInt32 aBorderMarginInTwips = iOplAPI.PopInt32();
	TBool aWrap = (iOplAPI.PopInt16() != 0);
	TBool aWidowOrphan = (iOplAPI.PopInt16() != 0);
	TBool aStartNewPage = (iOplAPI.PopInt16() != 0);
	TBool aKeepWithNext = (iOplAPI.PopInt16() != 0);
	TBool aKeepTogether = (iOplAPI.PopInt16() != 0);
	TInt32 aSpaceAfterInTwips = iOplAPI.PopInt32();
	TInt32 aSpaceBeforeInTwips = iOplAPI.PopInt32();
	CParaFormat::TLineSpacingControl aLineSpacingControl= (CParaFormat::TLineSpacingControl)iOplAPI.PopInt16();
	TInt32 aLineSpacingInTwips = iOplAPI.PopInt32();
	CParaFormat::TAlignment aVerticalAlignment = (CParaFormat::TAlignment)iOplAPI.PopInt16();
	CParaFormat::TAlignment aHorizontalAlignment = (CParaFormat::TAlignment)iOplAPI.PopInt16();
	TInt32 aIndentInTwips = iOplAPI.PopInt32();
	TInt32 aRightMarginInTwips = iOplAPI.PopInt32();
	TInt32 aLeftMarginInTwips = iOplAPI.PopInt32();
	TInt16 aBlue = iOplAPI.PopInt16();
	TInt16 aGreen = iOplAPI.PopInt16();
	TInt16 aRed = iOplAPI.PopInt16();
	iOplPrintEngine->InitialiseParaFormatL
		(
		aRed,
		aGreen,
		aBlue,
		aLeftMarginInTwips,
		aRightMarginInTwips,
		aIndentInTwips,
		aHorizontalAlignment,
		aVerticalAlignment,
		aLineSpacingInTwips,
		aLineSpacingControl,
		aSpaceBeforeInTwips,
		aSpaceAfterInTwips,
		aKeepTogether,
		aKeepWithNext,
		aStartNewPage,
		aWidowOrphan,
		aWrap,
		aBorderMarginInTwips,
		aDefaultTabWidthInTwips
		);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetLocalParaFormatL()
	{
	iOplPrintEngine->SetLocalParaFormatL();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetGlobalParaFormatL()
	{
	iOplPrintEngine->SetGlobalParaFormatL();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::RemoveSpecificParaFormatL()
	{
	iOplPrintEngine->RemoveSpecificParaFormatL();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetFontNameL()
	{
	TPtrC name = iOplAPI.PopString();
	iOplPrintEngine->SetFontNameL(name);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetFontHeight()
	{
	TInt16 theHeight = iOplAPI.PopInt16();
	iOplPrintEngine->SetFontHeight(theHeight);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetFontPosition()
	{
	TInt16 thePosition = iOplAPI.PopInt16();
	iOplPrintEngine->SetFontPosition(thePosition);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetFontWeight()
	{
	TInt16 theWeight = iOplAPI.PopInt16();
	iOplPrintEngine->SetFontWeight(theWeight);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetFontPosture()
	{
	TInt16 thePosture = iOplAPI.PopInt16();
	iOplPrintEngine->SetFontPosture(thePosture);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetFontStrikethrough()
	{
	TInt16 theStrikethrough = iOplAPI.PopInt16();
	iOplPrintEngine->SetFontStrikethrough(theStrikethrough);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetFontUnderline()
	{
	TInt16 theUnderline = iOplAPI.PopInt16();
	iOplPrintEngine->SetFontUnderline(theUnderline);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SetGlobalCharFormatL()
	{
	iOplPrintEngine->SetGlobalCharFormatL();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::RemoveSpecificCharFormatL()
	{
	iOplPrintEngine->RemoveSpecificCharFormatL();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SendBitmapToPrinterL() 
	{
	TInt32 OplHandle = iOplAPI.PopInt32();
	TInt32 PictureHandle = iOplAPI.BitmapHandleFromIdL(OplHandle);
	iOplPrintEngine->InsertBitmapL(0,PictureHandle,EAppend);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::InsertBitmapL() 
	{
	TInt pos = iOplAPI.PopInt32();
	TInt32 OplHandle = iOplAPI.PopInt32();
	TInt32 PictureHandle = iOplAPI.BitmapHandleFromIdL(OplHandle);
	iOplPrintEngine->InsertBitmapL(pos,PictureHandle);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SendScaledBitmapToPrinterL() 
	{
	TInt y = iOplAPI.PopInt32();
	TInt x = iOplAPI.PopInt32();
	TInt32 OplHandle = iOplAPI.PopInt32();
	TInt32 PictureHandle = iOplAPI.BitmapHandleFromIdL(OplHandle);
	iOplPrintEngine->InsertBitmapL(0,PictureHandle,EAppend, x, y);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::InsertScaledBitmapL() 
	{
	TInt y = iOplAPI.PopInt32();
	TInt x = iOplAPI.PopInt32();
	TInt pos = iOplAPI.PopInt32();
	TInt32 OplHandle = iOplAPI.PopInt32();
	TInt32 PictureHandle = iOplAPI.BitmapHandleFromIdL(OplHandle);
	iOplPrintEngine->InsertBitmapL(pos,PictureHandle, EInsert, x, y);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::PrinterDocLength()
	{
	TInt32 length = iOplPrintEngine->DocumentLength();
	iOplAPI.Push(length);
	}

void CPRINTEROpx::SendRichTextToPrinterL() 
	{
	TInt32 aRichTextPointer = iOplAPI.PopInt32();
	if((aRichTextPointer & 0x3) !=0) //	check its on a word boundary
		{
		User::Leave(KOplErrInvalidArgs);
		}
	iOplPrintEngine->SendRichTextToPrinterL((CRichText*)aRichTextPointer);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::SendBufferToPrinterL()
	{
	TInt32 offset = iOplAPI.PopInt32();
	TUint8* uintPtr = iOplAPI.OffsetToAddrL(offset, 1);
	TInt32* address = (TInt32*) uintPtr;
	TInt32 length = *address++;
	TPtr16 string((TUint16*)address, length, length);
	iOplPrintEngine->InsertStringL(string,0, EAppend);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::ResetPrinting()
	{
	iOplPrintEngine->ResetPrinting();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::PageSetupDialogL()
	// Call standard Eikon page setup dialog. 
	{
	iOplPrintEngine->RunPageSetupDialogL();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::PrintPreviewDialogL()
	{
	iOplPrintEngine->RunPrintPreviewDialogL();
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::PrintRangeDialogL()
	{
	TInt i = iOplPrintEngine->NumberOfPagesInDoc();
	iOplPrintEngine->RunPrintRangeDialogL(iOplPrintEngine->ReturnPrintSetup(), i);
	iOplAPI.Push(0.0);
	}

void CPRINTEROpx::PrintDialogL() 
	{
	iOplPrintEngine->RunPrintDialogL();
	iOplAPI.Push(0.0);
	}

TBool CPRINTEROpx::CheckVersion(TInt aVersion)
	// To check whether the OPX is a compatible version
	{
	if ((aVersion & 0xff00)>(KOpxPrinterVersion & 0xff00))	// major version must be <= OPX's version
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	CPRINTEROpx* tls=((CPRINTEROpx*)Dll::Tls());
	if (tls==NULL)		// tls is NULL on loading an OPX DLL (also after unloading it)
		{
		tls=CPRINTEROpx::NewL(aOplAPI);
		Dll::SetTls(tls);
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxPrinterVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	//
	// DLL entry point
	//
	{
	return(KErrNone);
	}
