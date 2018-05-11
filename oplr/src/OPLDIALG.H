// OPLDIALG.H
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#ifndef OPLDIALG_H
#define OPLDIALG_H
#if defined(__UIQ__)
#include <qikstockcontrols.hrh>
#endif
#include <bldvariant.hrh>
#include <badesca.h>
#include <eikdialg.h>
#include <eikon.hrh>
#if !defined(__SERIES60__) && !defined(__SERIES90__)
#include <eikfsel.h>
#endif
#include <bamdesca.h>
#if !defined(__SERIES90__)
#include <eikspane.h>
#endif

#ifndef FRAME_H
class CFrame;
#endif

class CEikButtonGroupContainer;

class MValidatedItem
	{
public:
	virtual TBool IsStateValid()=0;
	};

#if !defined(__SERIES60__) && !defined(__SERIES90__)
class COplFileNameSelector : public CEikFileNameSelector , public MValidatedItem
	{
public:
	void ConstructL(CCoeControl* aParent,TInt aFlags);
	TBool IsStateValid();
private:
	TInt iFlags;
	};

class COplFileNameEditor : public CEikFileNameEditor , public MValidatedItem
	{
public:
	void ConstructL(TInt aFlags);
	TBool IsStateValid();
	TFileName& FullName() {return iReturnName;};
private:
	TInt iOplFlags;
	TFileName iReturnName;
	};

class COplFolderNameEditor : public CEikFileNameEditor , public MValidatedItem
	{
public:
	void ConstructL(TInt aFlags);
	TFileName Name();
	TBool IsStateValid();
private:
	TInt iOplFlags;
	TFileName iReturnName;
	};

class COplDriveNameSelector : public CEikDriveNameSelector
	{
	// Gives us access to BuildDriveNameArrayL and lets us return iFileSelectionObserver
	// for Crystal-specific hackery with FolderSelector popout dialog behaviour
public:
	void BuildDriveArrayL() {BuildDriveNameArrayL();};
	MEikFileSelectionObserver* FileSelectionObserver() const {return iFileSelectionObserver;};
	};

class COplFolderNameSelector : public CEikFolderNameSelector
	{
	// Gives us access to DoCreatePopoutL() and lets us associate drive selectors with
	// the folder browser. Crystal-specific hackery to cope with the fact drives can
	// be changed in the folder selector's popout
public:
	void ConstructL(CCoeControl* aParent, const TInt aFlags);
	void SetCorrespondingDriveSelector(COplDriveNameSelector* aCorrespondingDriveSelector) {iCorrespondingDriveSelector=aCorrespondingDriveSelector;};
protected: // from CEikFolderNameSelector
	virtual void DoCreatePopoutL();
private:
	TInt iOplFlags;
	COplDriveNameSelector* iCorrespondingDriveSelector;
	};
#endif

#define KMenuPopup		0x5000
#define KMaxButLength	20
#define KMaxLineLength	80

const TInt KOplDialogIdBase=0x64;

const TInt KOplDialogItem=0x1000000; // Check for conflict with item numbers!!!!

const TInt KOplMultiLineTextEditor=0x2000000; // Special case since we want a EEikCtEdwin here also

#if !defined(__UIQ__)
const TInt KOplLongEditor=KOplDialogItem|EEikCtNumberEditor;
const TInt KOplFloatEditor=KOplDialogItem|EEikCtFlPtEd;
#else
const TInt KOplLongEditor=KOplDialogItem|EQikCtNumberEditor;
const TInt KOplFloatEditor=KOplDialogItem|EQikCtFloatingPointEditor;
#endif
const TInt KOplTextEditor=KOplDialogItem|EEikCtEdwin;
const TInt KOplChoiceList=KOplDialogItem|EEikCtChoiceList;
const TInt KOplSecretEditor=KOplDialogItem|EEikCtSecretEd;
const TInt KOplTextLabel=KOplDialogItem|EEikCtLabel; // this is a focusing label for non-focusing label use EEikCtLabel
#if !defined(__UIQ__)
const TInt KOplTimeEditor=KOplDialogItem|EEikCtTimeEditor;
const TInt KOplDurationEditor=KOplDialogItem|EEikCtDurationEditor;
const TInt KOplDateEditor=KOplDialogItem|EEikCtDateEditor;
#else
const TInt KOplTimeEditor=KOplDialogItem|EQikCtTimeEditor;
const TInt KOplDurationEditor=KOplDialogItem|EQikCtDurationEditor;
const TInt KOplDateEditor=KOplDialogItem|EQikCtDateEditor;
#endif
const TInt KOplFileSelector=KOplDialogItem|EEikCtFileNameSel;
const TInt KOplFileEditor=KOplDialogItem|EEikCtFileNameEd;
const TInt KOplDriveNameSel=KOplDialogItem|EEikCtDriveNameSel;
const TInt KOplFolderNameSel=KOplDialogItem|EEikCtFolderNameSel;
const TInt KOplFolderNameEd=KOplDialogItem|EEikCtFolderNameEd;
// const TInt KOplCheckBox=KOplDialogItem|EEikCtCheckBox;
// Add any new values here

const TInt KOplFileSelectorsMaxDisplayChars = 15;
#define KFileSelectorFlagShowHidden			0x800	// 2048
#define KFileSelectorFlagAllowNewFolders	0x400	// 1024
#define KFileSelectorFlagShowSystem			0x200	// 512
#define KFileSelectorFlagShowRom			0x100	// 256
#define KFileSelectorFlagAllowWildcards		0x80	// 128
// 0x40 (64) is used on OPL1993 only for KFileSelectorFlagHideExtension
#define KFileSelectorFlagAllowNulls			0x20	// 32
#define KFileSelectorFlagQueryExisting		0x10	// 16
#define KFileSelectorFlagDisallowExisting	0x08	// 8
#define KFileSelectorFlagAllowDirsOnly		0x04	// 4
#define KFileSelectorFlagAllowDirs			0x02	// 2
#define KFileSelectorFlagEditor				0x01	// 1

struct SValidatedItem
	{
	MValidatedItem* iItem;
	TInt iId;
	};

enum
	{
	EOplDialogFlagsButtonsRight=0x01,
	EOplDialogFlagsNoTitleBar=0x02,
	EOplDialogFlagsFillScreen=0x04,
	EOplDialogFlagsNoDrag=0x08,
	EOplDialogFlagsDensePacking=0x10
	};

enum
	{
	EOplDialogButtonHideHotKey=0x100,
	EOplDialogButtonPlainHotKey=0x200
	};

class COplDialog : public CEikDialog
	{
public:
	COplDialog();
	~COplDialog();
#if defined(__UIQ__)
	void ConstructLC(CFrame* aFramePtr);
#else
	void ConstructLC(CFrame* aFramePtr,TInt aFlags);
#endif
	void RunLD(TInt16* aReturnPtr);

	void AddLongEditorL(const TDesC& aPrompt,TInt aMin,TInt aMax,TAny* aReturn);
	void AddFloatEditorL(const TDesC& aPrompt,TReal64 aMin,TReal64 aMax,TAny* aReturn);
	void AddTextEditorL(const TDesC& aPrompt,TInt aWidthInChars,TInt aMaxLength,TText* aReturn);	//dEdit
	void AddSecretEditorL(const TDesC& aPrompt,TInt aMaxLength,TText* aReturn,TBool aSeedTextContents);
	void AddTextLabelL(const TDesC& aPrompt,const TDesC& aBody,TInt aFlags);
	void AddTimeEditorL(const TDesC& aPrompt,TUint aMinTime,TUint aMaxTime,TInt aFlags,TAny* aReturn);
	void AddDateEditorL(const TDesC& aPrompt,TInt32 aMinDate,TInt32 aMaxDate,TAny* aReturn);
	void AddFileSelectorL(const TDesC& aPrompt1,const TDesC& aPrompt2,const TDesC& aPrompt3,TInt aFlags,const TUidType& aUidType,TText* aReturn);
	void AddFileEditorL(const TDesC& aPrompt1,const TDesC& aPrompt2,const TDesC& aPrompt3,TInt aFlags,TText* aReturn);
	void AddFolderSelectorL(const TDesC& aPrompt1,const TDesC& aPrompt2,TInt aFlags,TText* aReturn);
	void AddFolderEditorL(const TDesC& aPrompt1,const TDesC& aPrompt2,const TDesC& aPrompt3,TInt aFlags,TText* aReturn);
//	void AddCheckBoxL(const TDesC& aPrompt,TInt16* aReturn);
	void AddMultiLineEditorL(const TDesC& aPrompt,TInt aNumOfLines,TInt aWidthInChars,TInt aMaxLength,TInt32* aReturn,TBool aReadOnly);

	CDesCArray*  AddChoiceListL(const TDesC& aPrompt,TAny* aReturn,TBool aIncrementalMatching);
	void ChoiceListFinishedL();

	SEikControlInfo CreateCustomControlL(TInt aControlType);
	void GetCustomAutoValue(TAny* aReturn,TInt aControlType,const CCoeControl* aControl);
	TBool OkToExitL(TInt aButtonId);
	CFrame* InitFrame() {return iFramePtr;};
	void SetCorner(TInt aXPos,TInt aYPos);
	void AddTitleL(const TDesC& aTitle);
	void AddValidatedItemL(MValidatedItem* aItem,TInt aId);
	inline CEikButtonGroupContainer* SwapButtonGroupContainer(CEikButtonGroupContainer* aContainer) 
		{return CEikDialog::SwapButtonGroupContainer(aContainer);};
	inline MEikCommandObserver* ButtonCommandObserver() const
		{return CEikDialog::ButtonCommandObserver();};
	void GetHelpContext(TCoeHelpContext& aContext) const;
	inline TBool HasButtons() const {return iHasButtons;};
	inline void HasButtons(const TBool aFlag) {iHasButtons=aFlag;};
private:
	void SetSizeAndPosition(const TSize& aSize);
	TInt iNextId;
	CFrame* iFramePtr;
	TGulAlignmentValue iCorner;
	TInt16* iReturnPtr;
	CArrayFixSeg<SValidatedItem>* iValidatedItems;
	TInt iLastCreatedChoiceListId; // zero if none outstanding
	TBool iHasButtons; //Because ButtonCount() returns 4 immediately after dialog is created.
	};

enum TButtonGroupContainerFocus
	{
	ENoButtonGroupContainer=0,EOneButtonGroupContainer,ETwoButtonGroupContainer,EThreeButtonGroupContainer
	};

class COplAlertWin : public CEikDialog
	{
public:
	COplAlertWin();
	~COplAlertWin();
	void ConstructL(CCoeEnv* aCoeEnv);
	void RunAlert(const TDesC& aLine1,const TDesC& aLine2,const TDesC& aBut1,const TDesC& aBut2,const TDesC& aBut3,TInt& aReturn);
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
private:
	CEikButtonGroupContainer* CreateButtonGroupL(TInt aResourceId);
	void SetButtonTextReserveLengthL(const CEikButtonGroupContainer* aButtonPanel,const TInt aButtonId,const TInt aReserveLength);
	void ExchangeButtonGroupContainer(TButtonGroupContainerFocus aFocus);
	void Set(TPtrC& aTrg,const TDesC& aSrc,TInt aMaxLen);
	TPtrC iLine1;
	TPtrC iLine2;
	TPtrC iBut1;
	TPtrC iBut2;
	TPtrC iBut3;

	CEikButtonGroupContainer* iThreeButtonGroupContainer;
	CEikButtonGroupContainer* iTwoButtonGroupContainer;
	CEikButtonGroupContainer* iOneButtonGroupContainer;
	HBufC* iContinueText;
	TInt* iReturnPtr;
	TBool iExitOnEnter;
	TButtonGroupContainerFocus  iButtonGroupContainerFocus;
	};

#endif
