// PRNTST.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include "PRNTST.h"

////////////////////////////////////////////////////////////////////////////////
// The class CPRNTSTOpx, which is in the format required by opx
// will access COplPrintTestEngine which will do all the actual work
//////////////////////////////////////////////////////////////////////////////

COplPrintTestEngine::COplPrintTestEngine() 
	{
	}

COplPrintTestEngine::~COplPrintTestEngine() 
	{
	delete iParaFormatLayer;
	delete iCharFormatLayer;
	delete iRichText;
	delete iParaFormat;
	}

COplPrintTestEngine* COplPrintTestEngine::NewL(OplAPI& aOplAPI)
    {
    COplPrintTestEngine* This=new(ELeave) COplPrintTestEngine();
    CleanupStack::PushL(This);
    This->ConstructL(aOplAPI);				
    CleanupStack::Pop();
    return This;
    }

void COplPrintTestEngine::ConstructL(OplAPI& aOplAPI) 
	{
	iInitialised = EFalse;
	iDocumentChanged =ETrue;
	iPaginateDialogCalled = EFalse;
	iOplAPI = &aOplAPI;
	iFormatChanged = EFalse;
	_LIT(KArial,"Arial");
	iFontName = KArial;
	iFontHeight = 178;

	iParaFormat = CParaFormat::NewL();
	DefaultFormatting();

	iParaFormatLayer = CParaFormatLayer::NewL();
	iCharFormatLayer = CCharFormatLayer::NewL(iCharFormat,iCharFormatMask);
	iRichText=CRichText::NewL(iParaFormatLayer,iCharFormatLayer);
	}

void COplPrintTestEngine::ResetPrinting()
// Remove all content and formatting
	{
	iRichText->Reset();
	iFormatChanged = ETrue;
	DefaultFormatting();	
	}

void COplPrintTestEngine::DefaultFormatting()
// Reset the formatting
	{
	TCharFormat defaultCharFormat(iFontName,iFontHeight);
	iCharFormat = defaultCharFormat;
	iCharFormatMask.ClearAll();
	iCharFormatMask.SetAttrib(EAttFontTypeface);
	iCharFormatMask.SetAttrib(EAttFontHeight);
	CFbsScreenDevice *device = CFbsScreenDevice::NewL(_L("scdv.dll"),EGray4);
	iCharFormat.iFontSpec.iTypeface.SetAttributes(FontUtils::TypefaceAttributes(*device,iFontName));
	delete device;
	}

void COplPrintTestEngine::InsertStringL(const TDesC& aString, const TInt aPos, TTextInsertPosition aTIP)
// Insert a string at the given position  if aTIP == EInsert. Append otherwise
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

void COplPrintTestEngine::SetFontNameL(const TDesC& aName)
	{
	iDocumentChanged=ETrue;
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontTypeface);
	CFbsScreenDevice *device = CFbsScreenDevice::NewL(_L("scdv.dll"),EGray4);
	iCharFormat.iFontSpec.iTypeface.SetAttributes(FontUtils::TypefaceAttributes(*device,aName));
	iCharFormat.iFontSpec.iTypeface.iName = aName;
	delete device;
	}


void COplPrintTestEngine::SetFontHeight(const TInt16 aHeight)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontHeight);
	iCharFormat.iFontSpec.iHeight = aHeight;
	}

void COplPrintTestEngine::SetFontPosition(const TInt16 aPosition)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontPrintPos);
	iCharFormat.iFontSpec.iFontStyle.SetPrintPosition((TFontPrintPosition)aPosition);
	}

void COplPrintTestEngine::SetFontWeight(const TInt16 aWeight)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontStrokeWeight);
	iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight((TFontStrokeWeight)aWeight);
	}

void COplPrintTestEngine::SetFontPosture(const TInt16 aPosture)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontPosture);
	iCharFormat.iFontSpec.iFontStyle.SetPosture((TFontPosture)aPosture);
	}

void COplPrintTestEngine::SetFontStrikethrough(const TInt16 aStrikethrough)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontStrikethrough);
	iCharFormat.iFontPresentation.iStrikethrough = (TFontStrikethrough)aStrikethrough;
	}


void COplPrintTestEngine::SetFontUnderline(const TInt16 aUnderline)
	{
	iFormatChanged = ETrue;
	iCharFormatMask.SetAttrib(EAttFontUnderline);
	iCharFormat.iFontPresentation.iUnderline = (TFontUnderline)aUnderline;
	}


void COplPrintTestEngine::ResetFormatL(const TInt aPos)
// If the format has been changed since insert was last called
// the this member is called to enter the new format at the insert position.
	{
	iRichText->CancelInsertCharFormat();
	if (RemoveUnnessaryCharSettings(&iCharFormat,&iCharFormatMask,aPos))
		iRichText->SetInsertCharFormatL(iCharFormat, iCharFormatMask, aPos);
	iFormatChanged = EFalse;
	}



TInt COplPrintTestEngine::SetInsertPositon(const TInt aPos, const TTextInsertPosition aTIP)
// Set the insert position, called by the InsertXXX members.
	{
	TInt last = iRichText->DocumentLength();
	if(aTIP == EInsert)
		{
		if(aPos >= 0 && aPos <= last)
			return  aPos;
		User::Leave(KOplErrInvalidArgs);			// OPL error code
		}
	return(last);
	}

void COplPrintTestEngine::SetAlignmentL(CParaFormat::TAlignment aAlignment)
// Alignment can also be set with InitialiseParaFormatL. 
	{
	iFormatChanged = ETrue;
	iParaFormat->iHorizontalAlignment = aAlignment; 
	iParaFormatMask.SetAttrib(EAttAlignment); // interested in alignment
	}

void COplPrintTestEngine::InitialiseParaFormatL(TInt16 aRed,
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



void COplPrintTestEngine::SetLocalParaFormatL() 
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


void COplPrintTestEngine::SetGlobalParaFormatL() 
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


void COplPrintTestEngine::RemoveSpecificParaFormatL()
// Get rid of the local paragraph format, so the global takes effect
	{
	iRichText->RemoveSpecificParaFormatL(iRichText->DocumentLength(),0);
	}

void COplPrintTestEngine::SetGlobalCharFormatL() 
// Set the current character format to be the global default
	{
	TCharFormatMask mask;	
	mask.SetAll();			
	iRichText->CGlobalText::ApplyCharFormatL(iCharFormat, mask, 0,0);
	}

void COplPrintTestEngine::RemoveSpecificCharFormatL()
// Make the character format take on global attributes
	{
	const CCharFormatLayer *pLay = iRichText->GlobalCharFormatLayer();
	iCharFormatMask.ClearAll();	
	pLay->Sense(iCharFormat,iCharFormatMask  );
	iFormatChanged = ETrue;
	}

void COplPrintTestEngine::InsertSpecialCharacterL(TInt16 aSpecialChar, const TInt aPos, TTextInsertPosition aTIP)
// Insert a special character (as defined by CEditableText) at aPos if aTIP == EInsert. Append otherwise
	{
	iFormatChanged = ETrue;
	TInt pos;
	pos = SetInsertPositon(aPos, aTIP);
	iRichText->InsertL(pos, aSpecialChar);
	}
	

TInt COplPrintTestEngine::DocumentLength()
// Return number of chars (including special chars, bitmaps count as 1).
// This is called when appending characters for example
	{
		return iRichText->DocumentLength();
	}


// The following 4 functions are based on ones by Gareth Richards. 
//Some changes had to be made

TBool COplPrintTestEngine::RemoveUnnessaryParaSettings(const CParaFormat *pParaFormat,TParaFormatMask *pParaMask,long pos)
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

TBool COplPrintTestEngine::RemoveUnnessaryCharSettings(const TCharFormat *pCharFormat,TCharFormatMask *pCharMask,long Pos)
{

	TCharFormat CharFormat;
	TCharFormatMask Mask;
	iRichText->GetCharFormat(CharFormat,Mask, Pos, 0);
	
	return RemoveUnnessaryCharSettings2(&CharFormat,pCharFormat,pCharMask);
}


TBool COplPrintTestEngine::RemoveUnnessaryParaSettings2(const CParaFormat *pBaseParaFormat,const CParaFormat *pParaFormat,TParaFormatMask *pParaMask)
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

TBool COplPrintTestEngine::RemoveUnnessaryCharSettings2(const TCharFormat *pBaseCharFormat,const TCharFormat *pCharFormat,TCharFormatMask *pCharMask)
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


TInt32 COplPrintTestEngine::GetRichText()
	{
	return (TInt32) iRichText;
	}


//////////////////////////////////////////////////////////////////////////////////////////
//																						//
//		CPRNTSTOpx, a proxy class to provide a opx interface to COplPrintTestEngine		//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

CPRNTSTOpx::CPRNTSTOpx(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

CPRNTSTOpx* CPRNTSTOpx::NewL(OplAPI& aOplAPI)
    {
    CPRNTSTOpx* This=new(ELeave) CPRNTSTOpx(aOplAPI);
    CleanupStack::PushL(This);
    This->ConstructL();
    CleanupStack::Pop();
    return This;
    }


void CPRNTSTOpx::ConstructL()
    {
	iOplPrintTestEngine  = COplPrintTestEngine::NewL(iOplAPI);
    } 

CPRNTSTOpx::~CPRNTSTOpx()
    {
	delete iOplPrintTestEngine;
    Dll::FreeTls();		// Required so that Tls is set to zero on unloading the OPX in UNLOADM
    }


//		Get the function by ordinal.
//
void CPRNTSTOpx::RunL(TInt aProcNum)
	{
	switch(aProcNum)
		{
		case EGetRichText:
			GetRichText();
			break;
		default:
		User::Leave(KOplErrOpxProcNotFound);	
		}
	}



//////////////////////////////////////////////////////////////////////////////
//																			//
//				User functions start here									//
//																			//
//																			//
//																			//
//////////////////////////////////////////////////////////////////////////////

void CPRNTSTOpx::GetRichText() 
	{
	TInt32 pointer;
	_LIT(KRichText,"This is some hard-coded rich text.");
	iOplPrintTestEngine->InsertStringL(KRichText, 0, EAppend);
	iOplPrintTestEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplPrintTestEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplPrintTestEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplPrintTestEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplPrintTestEngine->SetAlignmentL(CParaFormat::ECenterAlign);
	iOplPrintTestEngine->SetFontHeight(1000);
	iOplPrintTestEngine->SetLocalParaFormatL();
	iOplPrintTestEngine->InsertStringL(_L("This paragraph is centered!"), 0, EAppend);
	iOplPrintTestEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplPrintTestEngine->RemoveSpecificParaFormatL();
	iOplPrintTestEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplPrintTestEngine->InsertSpecialCharacterL(CEditableText::EParagraphDelimiter,0,EAppend);
	iOplPrintTestEngine->SetFontNameL(_L("Times New Roman"));
	iOplPrintTestEngine->InsertStringL(_L("This paragraph is in Times font."), 0, EAppend);


	pointer = iOplPrintTestEngine->GetRichText();
	iOplAPI.Push(pointer);
	}

TBool CPRNTSTOpx::CheckVersion(TInt aVersion)
// To check whether the opx is a compatible version
	{
	if ((aVersion & 0x0f00)>(KOpxPrintTestVersion & 0xf00))	// major version must be <= OPX's version
		return EFalse;
	else
		return ETrue;
	}


EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
// Creates a COpxBase instance as required by the OPL runtime
// This object is to be stored in the OPX's TLS as shown below
	{
	CPRNTSTOpx* tls=((CPRNTSTOpx*)Dll::Tls());
	if (tls==NULL)		// tls is NULL on loading an OPX DLL (also after unloading it)
		{
        tls=CPRNTSTOpx::NewL(aOplAPI);
	    Dll::SetTls(tls);
        }
    return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxPrintTestVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return(KErrNone);
	}