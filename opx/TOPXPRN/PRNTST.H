// PRNTST.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#ifndef __PRNTST_H__
#define __PRNTST_H__

#include <coemain.h>
#include <coeutils.h>
#include <coecntrl.h>
#include <eikenv.h>
#include <eikdll.h>        
#include <e32base.h>
#include <e32hal.h>
#include <eikappui.h>
#include <e32keys.h>
#include <eikedwin.h>
#include <eikedwob.h>
#include <eikrted.h>
#include <eiktbar.h>
#include <eikcfdlg.h>
#include <eikpprob.h>
#include <eikprtdg.h>
#include <eikprtpv.h>
#include <eikproc.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <eikchkbx.h>
#include <eikmfne.h>
#include <gulutil.h>
#include <eiklabel.h>
#include <fbs.h>   
#include <frmprint.h>
#include <frmvis.h>
#include <oplapi.h>
#include <opxapi.h>
#include <opldoc.h>
#include <oplerr.h>  
#include <prnsetup.h>       
#include <txtfmlyr.h>
#include <txtrich.h>
#include <w32std.h>
#include <eikon.rsg>
#include <eikon.mbg>
#include <eikon.hrh>

class CEikonEnv;

// The OPX's UID as used in the DECLARE OPX statement for your OPX
//
const TInt KUidOpxPrintTest=0x100002F1;

// The OPX's version number as used in the DECLARE OPX statement for your OPX
//
const TInt KOpxPrintTestVersion=0x100;

class COplPrintTestEngine;

const TInt KOplDontTruncate=-1;

enum TTextInsertPosition {EAppend, EInsert};

// CPRNTSTOpx uses OplAPI to get and return parameters and COplPrintTestEngine to do the work
// Public interface functions in this class correspond to those in the OPX header file, and
// thus represent the interface for the OPX
//
class CPRNTSTOpx :public COpxBase
    {
public:
    CPRNTSTOpx();
     ~CPRNTSTOpx();
	static CPRNTSTOpx* NewL(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
   
	// Public interface functions that are called from opl
	// These generally pop arguments from iOplAPI reference member, and then call 
	// a member of the iOplPrintTestEngine on those args.

	void GetRichText();


private:
	enum TExtensions
		{
		EGetRichText = 1
		};
	private:
		void ConstructL();
		CPRNTSTOpx(OplAPI& aOplAPI);
	private:
		COplPrintTestEngine* iOplPrintTestEngine;
  };

// COplPrintTestEngine: Providing printing capability (dialogs, print control) 
// that is accessed by CPRNTSTOpx. 
//
class COplPrintTestEngine 
 :	public CBase
	{
public:
	static COplPrintTestEngine* NewL(OplAPI& aOplAPI);
	COplPrintTestEngine();
	~COplPrintTestEngine();
	
	TInt32 GetRichText();
	void SendStringToPrinterL(TDesC& string);
	void InsertStringL(const TDesC& aString,const TInt aPos,TTextInsertPosition aTIP = EInsert);
	TInt DocumentLength();
	void InsertSpecialCharacterL(TInt16 aSpecialChar, const TInt aPos, TTextInsertPosition aTIP = EInsert);
	void SetFontNameL(const TDesC& aName);
	void SetFontHeight(const TInt16 aHeight);
	void SetFontPosition(const TInt16 aPosition);
	void SetFontWeight(const TInt16 aWeight);
	void SetFontPosture(const TInt16 aPosture);
	void SetFontStrikethrough(const TInt16 aStrikethrough);
	void SetFontUnderline(const TInt16 aUnderline);
	void ResetPrinting();		
	void SetAlignmentL(CParaFormat::TAlignment aAlignment);
	void InitialiseParaFormatL(
		TInt16 aRed,
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
		TUint32 aDefaultTabWidthInTwips);
void SetLocalParaFormatL(); 
void SetGlobalParaFormatL();
void RemoveSpecificParaFormatL();
void SetGlobalCharFormatL();
void RemoveSpecificCharFormatL();

private:
	void ConstructL(OplAPI& aOplAPI);
	void ResetFormatL(const TInt aPos);
	TInt SetInsertPositon(const TInt aPos, const TTextInsertPosition aTIP);
	void DefaultFormatting();
	TBool RemoveUnnessaryParaSettings(const CParaFormat *aParaFormat,TParaFormatMask *aParaMask,long pos);
	TBool RemoveUnnessaryCharSettings(const TCharFormat *pCharFormat,TCharFormatMask *pCharMask,long Pos);
	TBool RemoveUnnessaryParaSettings2(const CParaFormat *pBaseParaFormat,const CParaFormat *pParaFormat,TParaFormatMask *pParaMask);
	TBool RemoveUnnessaryCharSettings2(const TCharFormat *pBaseCharFormat,const TCharFormat *pCharFormat,TCharFormatMask *pCharMask);

private:
	CParaFormatLayer* iParaFormatLayer;
	CCharFormatLayer* iCharFormatLayer;
	CRichText* iRichText;
	TPrintParameters iPrintParameters;
	OplAPI* iOplAPI;
	TBool iInitialised ;
	TBool iDocumentChanged;
	TBool iFormatChanged;
	TInt iNumPages;
	TBool iPaginateDialogCalled;
	TBufC<KMaxTypefaceNameLength> iFontName;	
	TCharFormatMask iCharFormatMask;
    TCharFormat iCharFormat;
	CParaFormat* iParaFormat;
	TParaFormatMask iParaFormatMask;
	TInt16 iFontHeight;
	};

#endif  // __PRNTST_H__