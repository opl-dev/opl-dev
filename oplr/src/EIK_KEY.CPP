// EIK_KEY.CPP
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#include "debug.h"
#include <opcodes.h>
#include "oplutil.h"
#include "opldialg.h"
#include <eikfctry.h>
#include <eikon.hrh>
#include <eikon.rsg>
#include <eikdialg.h>
#include <eiklabel.h>
#include <eikseced.h>
#if !defined(__SERIES60__)
#include <eikchlst.h>
#if !defined(__SERIES90__)
#include <eikfsel.h>
#endif
#endif
#if !defined(__UIQ__)
#include <eikfpne.h>
#include <eikmfne.h>
#elif defined(__UIQ__)
#include <qikon.hrh>
#include <QikDurationEditor.h>
#include <qiktimeeditor.h>
#include <QikTimeAndDateEditor.h>
#include <QikDateEditor.h>
#include <QIKNUMBEREDITOR.H>
#include <QIKFLOATINGPOINTEDITOR.H>
#endif
#include <coeutils.h>
#include <eikfutil.h>
#include <eikbtpan.h>
#include <eikbtgpc.h>
#include <eikcmbut.h>
#include <eiklbbut.h>
#include <barsread.h>
#if defined(__SERIES60__)

#elif defined(__UIQ__)
#include <eikchkbx.h>

#else
#include <eikchkbx.h>
#include <ckndgsve.h>
#include <ckndgtrg.h>
#endif
#include <eikcapc.h>
#include <oplr.rsg>
#include <oplr.hrh>
#include "graphics.h"
#include "oplr.h"
#include "oplapi.h"
#if defined(__SERIES60__)
#include <aknenv.h>
#elif defined(__UIQ__)
#else
#include <ckndgtrg.h>	// Needed on Crystal 6.0 to workaround the fact that folder selectors
						// provide no ROM access flags by default. This allows us to hack
						// things ourselves and use CCknTargetFolderDialog directly
#endif
#if defined(__S80_DP2_0__)
#include <eikmover.h>	// For 9500 dDATE fix.
#endif

LOCAL_C void doProcessItemsL(CStack& aStack, COplRuntime& aRuntime,TInt aNumOfItems)
	{
	TInt hotKeyCode;
	for(TInt index=0; index<aNumOfItems; index++)
		{
		hotKeyCode = aStack.PopInt16();
		TBuf<256> str=aStack.PopString();
		if (str.Length()>CEikMenuPaneItem::SData::ENominalTextLength)
			User::Leave(KOplErrTooWide);
		if (hotKeyCode) // Skip optional items whose hotkeycode is zero.
			aRuntime.AddMenuItemL(&str,hotKeyCode);
		}
	}

LOCAL_C void CloseMenu(TAny* aRuntime)
	{
	STATIC_CAST(COplRuntime*,aRuntime)->CloseMenu();
	}

void FuncOpCode::mPopup(CStack& aStack,COplRuntime& aRuntime,CFrame* /*aFrame*/)
	{
	aRuntime.CloseMenu();
	
	TCleanupItem menu(CloseMenu,&aRuntime);
	CleanupStack::PushL(menu);

	TInt numOfArgs=aRuntime.IP8();
	doProcessItemsL(aStack,aRuntime,(numOfArgs-3)/2);
	TInt targetType=aStack.PopInt16();
	if (targetType<0 || targetType>3)
		User::Leave(KErrArgument);
	TInt ypos=aStack.PopInt16();
	TInt xpos=aStack.PopInt16();
	CEikHotKeyTable* hotKeyTable=aRuntime.CreateHotKeyTableLC();
	aRuntime.LaunchPopupMenuL(KMenuPopup,TPoint(xpos,ypos),(TPopupTargetPosType)targetType,hotKeyTable);
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(2); // hotKeyTable and menu
	}

LOCAL_C void doCardOrCascL(CStack& aStack, COplRuntime& aRuntime, TBool aIsCasc)
	{
	TCleanupItem menu(CloseMenu,&aRuntime);
	CleanupStack::PushL(menu);
	doProcessItemsL(aStack,aRuntime,aRuntime.IP8());
	TBuf<256> str;
	str=aStack.PopString();
	_LIT(KCascChar,">");
	if (aIsCasc)
		str.Append(KCascChar);
	if (str.Length()>CEikMenuBarTitle::SData::ENominalTextLength)
		User::Leave(KOplErrTooWide);
	aRuntime.AddMenuTitleL(&str,aIsCasc);
	CleanupStack::Pop(); // menu
	}

void OpCode::mInit(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	aRuntime.CloseMenu();
	aRuntime.PrepareForMenuL();
	}

void OpCode::mCard(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	doCardOrCascL(aStack,aRuntime,EFalse);
	}

void OpCode::mCardX(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{ // mCARDX bitmapId%,maskId%,item$,key%,
	TCleanupItem menu(CloseMenu,&aRuntime);
	CleanupStack::PushL(menu);
	doProcessItemsL(aStack,aRuntime,aRuntime.IP8());
	CDrawablesCollection& collection=aRuntime.DrawablesCollection();
	COplDrawable* maskDrawable=collection.DrawableL(aStack.PopInt16());
	COplDrawable* bitmapDrawable=collection.DrawableL(aStack.PopInt16());
	CFbsBitmap* mask=new(ELeave) CFbsBitmap();
	CleanupStack::PushL(mask);
	mask->Duplicate(((COplBitmap*)maskDrawable)->FbsBitmap().Handle());
	CFbsBitmap* bitmap=new(ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	bitmap->Duplicate(((COplBitmap*)bitmapDrawable)->FbsBitmap().Handle());
	aRuntime.AddMenuBitmapL(bitmap,mask);
	CleanupStack::Pop(3); // two bitmaps and a menu
	}

void OpCode::mCasc(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	doCardOrCascL(aStack,aRuntime,ETrue);
	}

void FuncOpCode::Menu(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{ // MENU
	TCleanupItem menu(CloseMenu,&aRuntime);
	CleanupStack::PushL(menu);
#if defined(__SERIES60__)
	aRuntime.DisplayMenuL(0,0); // No task list pane on Series60
#else
	aRuntime.DisplayMenuL(1,0); // Skip the zero'th task list pane by default
#endif
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(); // menu
	}

void FuncOpCode::MenuWithMemory(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	TUint8* pInit=OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt16));
	aRuntime.DisplayMenuL(*(pInit+1),*pInit,(TInt16*)pInit);
	CActiveScheduler::Start();
	aRuntime.CloseMenu();
	}

//
// End of menu stuff
//

//
// COplFileNameSelector
//
#if !defined(__SERIES60__) && !defined(__SERIES90__)
void COplFileNameSelector::ConstructL(CCoeControl* aParent,TInt aFlags)
	{
	CEikFileNameSelector::ConstructL(aParent,EEikChlistIncrementalMatching,KOplFileSelectorsMaxDisplayChars);
	iFlags=aFlags;
	}

TBool COplFileNameSelector::IsStateValid()
	{
	if ((iFileSelectorFlags&ESpecifiedFolderIsEmpty)&&!(iFlags&KFileSelectorFlagAllowNulls))
		{
		iEikonEnv->InfoMsg(R_EIK_TBUF_NO_FILENAME_SPECIFIED);
		return EFalse;
		}
	return ETrue;
	}
	
//
// COplFileNameEditor
//
void COplFileNameEditor::ConstructL(TInt aFlags)
	{
	CEikFileNameEditor::ConstructL(0,KOplFileSelectorsMaxDisplayChars,255,1); // check these values!!!!!
	iOplFlags=aFlags;
	}

TBool COplFileNameEditor::IsStateValid()
	{
	TInt infoMsg=0;
	TParse parse;
	GetText(iReturnName);
	if (parse.Set(iReturnName,NULL,NULL)!=KErrNone)
		{
		infoMsg=R_EIK_TBUF_INVALID_FILE_NAME;
		goto exit;
		}
	if (parse.DrivePresent())
		{
		TBuf<2> driveName = parse.Drive();
		driveName.UpperCase();
		TInt driveId = driveName[0] - 'A';
		TVolumeInfo volInfo;
		TInt retcode = iEikonEnv->FsSession().Volume(volInfo, driveId);
		if (retcode != KErrNone)
			{
			infoMsg=R_EIK_TBUF_DISK_NOT_PRESENT;
			goto exit;
			}
		}
	if (!parse.NamePresent())
		{
		if (parse.ExtPresent())
			infoMsg=R_EIK_TBUF_INVALID_FILE_NAME;
		else
			{
			if (parse.PathPresent()&&(iOplFlags&KFileSelectorFlagAllowDirs)) // allow dirs
				{
				parse.Set(iReturnName,&iRef,NULL);
				iReturnName=parse.DriveAndPath();
				if ((iOplFlags&KFileSelectorFlagDisallowExisting)&&EikFileUtils::PathExists(iReturnName)) // disallow existing
					infoMsg=R_EIK_TBUF_EXISTING_FOLDER_SPECIFIED;
				}
			else if (iOplFlags&KFileSelectorFlagAllowNulls) // allow null
				{// ensure drive and path in iReturnName
				parse.Set(iReturnName,&iRef,NULL);
				iReturnName=parse.DriveAndPath();
				}
			else
				infoMsg=R_EIK_TBUF_NO_FILENAME_SPECIFIED;
			}
		goto exit;
		}
	if (parse.Set(iReturnName,&iRef,NULL)!=KErrNone || !(iEikonEnv->FsSession().IsValidName(parse.FullName())))
		{
		infoMsg=R_EIK_TBUF_INVALID_FILE_NAME;
		goto exit;
		}
	if (ConeUtils::FileExists(parse.FullName()))
		{
		if (iOplFlags&KFileSelectorFlagDisallowExisting) // disallow existing
			{
			infoMsg=R_EIK_TBUF_EXISTING_FILE_SPECIFIED;
			goto exit;
			}
		else if (iOplFlags&KFileSelectorFlagQueryExisting) // query existing
			{
			TBuf<50> title;
			TBuf<35> displayFileName;
			TBuf<128> message;
			iEikonEnv->ReadResource(title,R_EIK_TBUF_DIALOG_TITLE_CONFIRM_FILE_REPLACE);
			EikFileUtils::AbbreviateFileName(parse.NameAndExt(),displayFileName);
			iEikonEnv->Format128(message,R_EIK_TBUF_FILE_REPLACE_CONFIRM1,&displayFileName);
			if (!iEikonEnv->QueryWinL(title,message))
				return EFalse;
			}
		}
	iReturnName=parse.FullName();
exit:
	if (infoMsg!=0)
		{
		iEikonEnv->InfoMsg(infoMsg);
		return EFalse;
		}
	return ETrue;
	}

//
// COplFolderNameEditor
//
void COplFolderNameEditor::ConstructL(TInt aFlags)
	{
	CEikFileNameEditor::ConstructL(0,KOplFileSelectorsMaxDisplayChars,253,1); // check these values !!!!!!!
	iOplFlags=aFlags;
	}

TFileName COplFolderNameEditor::Name()
	{
	return iReturnName;
	}

TBool COplFolderNameEditor::IsStateValid()
	{
	GetText(iReturnName);
	TParse parse;
	if (parse.Set(iReturnName,&iRef,NULL)!=KErrNone || !(iEikonEnv->FsSession().IsValidName(parse.FullName())))
		{
		iEikonEnv->InfoMsg(R_EIK_TBUF_INVALID_FOLDER_NAME);
		return EFalse;
		}
	iReturnName=parse.FullName();
	if (parse.NameOrExtPresent() && iReturnName.Length()<iReturnName.MaxLength())
		iReturnName.Append(KPathDelimiter); 
	if ((iOplFlags&KFileSelectorFlagDisallowExisting)&&EikFileUtils::PathExists(iReturnName)) // disallow existing
		{
		iEikonEnv->InfoMsg(R_EIK_TBUF_EXISTING_FOLDER_SPECIFIED);
		return EFalse;
		}
	return ETrue;
	}

//
// COplFolderNameSelector
//
void COplFolderNameSelector::DoCreatePopoutL()
	{
#if defined(__UIQ__)
#else
	TFullName fullName(FullName());
	TParse originalParse;
	originalParse.Set(fullName,NULL,NULL);
	TUint flags=CCknTargetFolderDialog::EShowAllDrives;
	if(iFolderSelectorFlags&EShowSystem)
		flags|=CCknTargetFolderDialog::EShowSystemFolders;
	if(iOplFlags&KFileSelectorFlagShowRom)
		flags|=CCknTargetFolderDialog::EShowRomDrive;
	if(iOplFlags&KFileSelectorFlagShowHidden)
		flags|=CCknTargetFolderDialog::EShowHiddenFolders;
	if(!(iOplFlags&KFileSelectorFlagAllowNewFolders)) // Note it's if the flag is NOT specified...
		flags|=CCknTargetFolderDialog::EDisallowNewFolder;

	if (CCknTargetFolderDialog::RunSelectFolderDlgLD(fullName,flags))
		{
		// Check to see if user changed drives in Crystal folder selector too. If they did
		// we need to update the drive selector (if stored) with the new drive...
		TParse newParse;
		newParse.Set(fullName,NULL,NULL);
		if ((newParse.Drive()!=originalParse.Drive()) && (iCorrespondingDriveSelector))
			{
			if (iCorrespondingDriveSelector->FileSelectionObserver()==this)
				{
				// If this control is set as the observer for the drive selector then
				// de-register it, set the drive name and then re-register it. This will
				// prevent the drive selector control from updating this control
				// which is what we want to do ourselves with SetFullNameL() below.
				iCorrespondingDriveSelector->SetFileSelectionObserver(NULL);
				iCorrespondingDriveSelector->SetFullNameL(fullName);
				iCorrespondingDriveSelector->SetFileSelectionObserver(this);
				}
			}
		SetFullNameL(fullName);
		}
#endif
	}

void COplFolderNameSelector::ConstructL(CCoeControl* aParent,const TInt aFlags)
	{
	CEikFolderNameSelector::ConstructL(aParent,EEikChlistIncrementalMatching,KOplFileSelectorsMaxDisplayChars);
	iOplFlags=aFlags;
	}
#endif

//
// COplDialog
//
COplDialog::COplDialog()
	{
	iNextId=KOplDialogIdBase;
	iCorner=EHCenterVCenter;
	iHasButtons=EFalse;
	}

#if defined(__UIQ__)
void COplDialog::ConstructLC(CFrame* aFramePtr)
	{
	PrepareLC(R_OPL_DIALOG_BASIC);
	iValidatedItems=new(ELeave) CArrayFixSeg<SValidatedItem>(4); // more than four is unlikely
	iFramePtr=aFramePtr;
	}
#else
void COplDialog::ConstructLC(CFrame* aFramePtr,TInt aFlags)
	{
	CEikDialog::ConstructAutoDialogLC(aFlags,0);
	iValidatedItems=new(ELeave) CArrayFixSeg<SValidatedItem>(4); // more than four is unlikely
	iFramePtr=aFramePtr;
	}
#endif

COplDialog::~COplDialog()
	{
	delete iValidatedItems; // the items have already been deleted
	}

void COplDialog::RunLD(TInt16* aReturnPtr)
	{ // were on the cleanup stack!
	if (iLastCreatedChoiceListId)
		User::Leave(KOplStructure);
	iReturnPtr=aReturnPtr;
	*iReturnPtr=(TInt16)0;
	CEikDialog::RunLD();
	}

void COplDialog::AddValidatedItemL(MValidatedItem* aItem,TInt aId)
	{
	SValidatedItem item;
	item.iItem=aItem;
	item.iId=aId;
	iValidatedItems->AppendL(item);
	}

TBool COplDialog::OkToExitL(TInt aButtonId)
	{
	if (aButtonId!=EEikBidCancel)
		{
		// validate any dFiles
		for (TInt ii=iValidatedItems->Count();--ii>=0;)
			{
			if (!((*iValidatedItems)[ii].iItem->IsStateValid()))
				{
				TryChangeFocusToL((*iValidatedItems)[ii].iId);
				return EFalse;
				}
			}
		CEikButtonGroupContainer& btGrp=ButtonGroupContainer();
		if (btGrp.ButtonCount())
			{
			TInt bid;
			switch(aButtonId)
				{
			case EEikBidDelete:
				bid=EKeyBackspace;
				break;
			case EEikBidTab:
				bid=EKeyTab;
				break;

			case EEikBidOk:
				{
				if (btGrp.PositionById(aButtonId)==KErrNotFound)
					return EFalse; // in case enter pressed and no enter button specified
				bid=EKeyEnter;
				}
				break;
// EEikBidCancel never gets here
//			case EEikBidCancel:
//				bid=EKeyEscape;
//				break;
			case EEikBidSpace:
				bid=EKeySpace;
				break;
			default:
				bid=aButtonId;
				break;
				}
			*iReturnPtr=(TInt16)bid;
			}
		else
			{ // Dialogs with no buttons
			TInt id=IdOfFocusControl();
			if (id)
				*iReturnPtr=(TInt16)(id-KOplDialogIdBase);
			else
				*iReturnPtr=TInt16(1);
			}
		}
#if defined(__UIQ__)
	for(TInt loop=KOplDialogIdBase;loop<iNextId;loop++) // save data in controls
	{
		CEikCaptionedControl* line=Line(loop+1);
		if(line->iReturnValue!=NULL)
			GetCustomAutoValue(line->iReturnValue,line->iControlType,line->iControl);
	}
#endif
	return ETrue;
	}

void COplDialog::AddTitleL(const TDesC& aTitle)
	{
	SetTitleL(aTitle);
//!!TODOUIQ Removed from UIQ, yet OPL needs nextId != KOplDialogIdBase to prevent structure fault.
// So this needs to be retested on UIQ and Crystal.
#if !defined(__UIQ__)
	++iNextId;
#endif
	}

void COplDialog::SetCorner(TInt aXPos,TInt aYPos)
	{
	// assume aXPos and aYPos are -1,0 or 1
	TInt corner=aXPos+1+(0x10*(aYPos+1));
	iCorner=(TGulAlignmentValue)corner;
	}

void COplDialog::SetSizeAndPosition(const TSize& aSize)
	{
	// Set dialog size and position as requested normally
	SetCornerAndSize(iCorner,aSize);

	// Get bounded size (by Status Pane and CBA) so we can prevent dialog
	// exceeding these boundaries
#if defined(__UIQ__)
	const TSize screenSize=iEikonEnv->EikAppUi()->ClientRect().Size();
#else
	const TSize screenSize=iCoeEnv->ScreenDevice()->SizeInPixels();
#endif
	TRect boundingRect(screenSize);
	CEikButtonGroupContainer& buttonGroup=ButtonGroupContainer();
	if (&buttonGroup)
		{
		// Must do this here else the cba is not sized
		buttonGroup.SetBoundingRect(boundingRect);
		buttonGroup.ReduceRect(boundingRect);
		}
#if !defined(__SERIES90__)
	CEikStatusPaneBase* statusPane = CEikStatusPaneBase::Current();
	if(statusPane)
		statusPane->ReduceRect(boundingRect);
#endif
	
	// Compare the current dialog size to maximum bounded size. SetCornerAndSize seems
	// to correctly set the size of the dialog but the position is not reflected in the
	// value returned by Rect() so we manually fix this using Position()
	TRect currentRect=Rect();
	TPoint currentPos=Position();
	currentRect.iTl.iX=currentPos.iX;
	currentRect.iBr.iX += currentPos.iX;
	currentRect.iTl.iY=currentPos.iY;
	currentRect.iBr.iY += currentPos.iY;

	// Slight hack for dialogs dPOSITIONed right up against the CBA, this prevents
	// their width from shrinking when the other settings are made below
	//
	// Currently assumes CBA is always vertical and on right!!
	if ((iCorner&EHRight) && (&buttonGroup))
		currentRect.iTl.iX=currentPos.iX-(screenSize.iWidth-boundingRect.iBr.iX);

#if !defined(__S80_DP2_0__)
	if (currentRect.iBr.iX>boundingRect.iBr.iX)
		currentRect.iBr.iX=boundingRect.iBr.iX;
	if (currentRect.iTl.iX<boundingRect.iTl.iX)
		currentRect.iTl.iX=boundingRect.iTl.iX;
	if (currentRect.iBr.iY>boundingRect.iBr.iY)
		currentRect.iBr.iY=boundingRect.iBr.iY;
	if (currentRect.iTl.iY<boundingRect.iTl.iY)
		currentRect.iTl.iY=boundingRect.iTl.iY;

	// Now reset the Rect area to ensure dialog fits within appropriate bounds
	SetRect(currentRect);
#else
	if (currentRect.iBr.iX>boundingRect.iBr.iX)
		currentRect.iTl.iX -= (currentRect.iBr.iX-boundingRect.iBr.iX);
	if (currentRect.iTl.iX<boundingRect.iTl.iX)
		currentRect.iTl.iX=boundingRect.iTl.iX;
	if (currentRect.iBr.iY>boundingRect.iBr.iY)
		currentRect.iTl.iY -= (currentRect.iBr.iY-boundingRect.iBr.iY);
	if (currentRect.iTl.iY<boundingRect.iTl.iY)
		currentRect.iTl.iY=boundingRect.iTl.iY;

	SetPosition(currentRect.iTl);
//	SetSize(TSize(currentRect.iBr.iX-currentRect.iTl.iX,currentRect.iBr.iY-currentRect.iTl.iY));
#endif
	}

SEikControlInfo COplDialog::CreateCustomControlL(TInt aType)
	{
	if (aType==KOplMultiLineTextEditor)
		aType=EEikCtEdwin;
	SEikControlInfo ret;
	ret.iControl = NULL;
	ret.iTrailerTextId = 0;
	ret.iFlags = 0;
#if !defined(__SERIES60__) && !defined(__SERIES90__)
	switch (aType)
		{
	case KOplDriveNameSel:
		ret.iControl=new(ELeave) COplDriveNameSelector();
		ret.iFlags|=EEikControlHasEars;
		break;
	case KOplFileSelector:
		ret.iControl=new(ELeave) COplFileNameSelector();
		ret.iFlags|=EEikControlHasEars;
		break;
	case KOplFileEditor:
		ret.iControl=new(ELeave) COplFileNameEditor();
		break;
	case KOplFolderNameEd:
		ret.iControl=new(ELeave) COplFolderNameEditor();
		break;
	case KOplFolderNameSel:
		ret.iControl=new(ELeave) COplFolderNameSelector();
		break;
	default:
		ret=EikControlFactory::CreateByTypeL(aType&~KOplDialogItem);
		break;
		}
#else
	ret=EikControlFactory::CreateByTypeL(aType&~KOplDialogItem);
#endif
	if (aType==KOplTextLabel)
		ret.iFlags&=~EEikControlIsNonFocusing;
	return ret;
	}

void COplDialog::GetCustomAutoValue(TAny* aReturn,TInt aType,const CCoeControl* aControl)
	{
	switch (aType)
		{
	case (KOplLongEditor):
		{
#if defined(__UIQ__)
		OplUtil::PutLong(aReturn,((CQikNumberEditor*)aControl)->Value());
#else
		OplUtil::PutLong(aReturn,((CEikNumberEditor*)aControl)->Number());
#endif
		break;
		}
	case (KOplFloatEditor):
		{
#if defined(__UIQ__)
		OplUtil::PutFloat(aReturn,((CQikFloatingPointEditor*)aControl)->Value());
#else
		OplUtil::PutFloat(aReturn,((CEikFloatingPointEditor*)aControl)->Value());
#endif
		break;
		}
	case (KOplTextEditor):
		{
		TPtr ptr((TText*)((TUint8*)aReturn+1+KOplAlignment),255);
		((CEikEdwin*)aControl)->GetText(ptr);
		*(TUint8*)aReturn=(TUint8)ptr.Length();
		break;
		}
	case (KOplMultiLineTextEditor):
		{
		TPtr ptr((TText*)((TInt32*)aReturn+1),KMaxTInt); // KOplAlignment NOT needed (or last 2 chars get lost)
		((CEikEdwin*)aControl)->GetText(ptr);
		OplUtil::PutLong((TInt32*)aReturn,ptr.Length());
		break;
		}
#if !defined(__SERIES60__)
	case (KOplChoiceList):
		{
		OplUtil::PutWord(aReturn,(TInt16)(((CEikChoiceList*)aControl)->CurrentItem()+1));
		break;
		}
#endif
	case (KOplSecretEditor):
		{
		TPtr ptr((TText*)((TUint8*)aReturn+1+KOplAlignment),255);
		((CEikSecretEditor*)aControl)->GetText(ptr);
		*(TUint8*)aReturn=(TUint8)ptr.Length();
		break;
		}
	case (KOplTimeEditor):
		{
#if defined(__UIQ__)
		TTime time=((CQikTimeEditor*)aControl)->Time();
#else
		TTime time=((CEikTimeEditor*)aControl)->Time();
#endif
		TInt64 timeInt=time.Int64();
		timeInt/=1000000;
		OplUtil::PutLong(aReturn,timeInt.Low());
		break;
		}
	case (KOplDurationEditor):
		{
#if defined(__UIQ__)
		TTimeIntervalSeconds duration=((CQikDurationEditor*)aControl)->Duration();
#else
		TTimeIntervalSeconds duration=((CEikDurationEditor*)aControl)->Duration();
#endif
		OplUtil::PutLong(aReturn,duration.Int());
		break;
		}
	case (KOplDateEditor):
		{
#if defined(__UIQ__)
		TTime time=((CQikDateEditor*)aControl)->Time();
#else
		TTime time=((CEikDateEditor*)aControl)->Date();
#endif
		TDateTime startDate(1900,TMonth(0),0,0,0,0,0);
		TTime sTime(startDate);
		OplUtil::PutLong(aReturn,time.DaysFrom(sTime).Int());
		break;
		}
#if !defined(__SERIES60__) && !defined(__SERIES90__)
	case (KOplFileSelector):
		{
		TPtr ptr((TText*)((TUint8*)aReturn+1+KOplAlignment),255);
		ptr=((COplFileNameSelector*)aControl)->FullName();
		*(TUint8*)aReturn=(TUint8)ptr.Length();
		break;
		}
	case (KOplFileEditor):
		{
		TPtr ptr((TText*)((TUint8*)aReturn+1+KOplAlignment),255);
		ptr=((COplFileNameEditor*)aControl)->FullName();
		*(TUint8*)aReturn=(TUint8)ptr.Length();
		break;
		}
	case (KOplFolderNameSel):
		{
		TPtr ptr((TText*)((TUint8*)aReturn+1+KOplAlignment),255);
		ptr=((COplFolderNameSelector*)aControl)->FullName();
		*(TUint8*)aReturn=(TUint8)ptr.Length();
		break;
		}
	case (KOplFolderNameEd):
		{
		TPtr ptr((TText*)((TUint8*)aReturn+KOplAlignment),255);
		ptr=((COplFolderNameEditor*)aControl)->Name();
		*(TUint8*)aReturn=(TUint8)ptr.Length();
		break;
		}
#endif
/*	case (KOplCheckBox):
		{
		OplUtil::PutWord(aReturn,(TInt16)((((CEikCheckBox*)aControl)->State()==CEikButtonBase::ESet)?KOplTrue:KOplFalse));
		break;
		}
*/	default:
		_LIT(KOplDialogs,"Opl Dialogs");
		User::Panic(KOplDialogs,1);
		}
	}

void COplDialog::AddLongEditorL(const TDesC& aPrompt,TInt aMin,TInt aMax,TAny* aReturn)
	{
#if defined(__UIQ__)
	CQikNumberEditor* numEd=(CQikNumberEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplLongEditor,aReturn);
#else
	CEikNumberEditor* numEd=(CEikNumberEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplLongEditor,aReturn);
#endif
	TInt init=OplUtil::GetLong(aReturn);
	if (init<aMin)
		init=aMin;
	else if (init>aMax)
		init=aMax;
	numEd->ConstructL(aMin,aMax,init);
	}

void COplDialog::AddFloatEditorL(const TDesC& aPrompt,TReal64 aMin,TReal64 aMax,TAny* aReturn)
	{
#if defined(__UIQ__)
	CQikFloatingPointEditor* flPtEd=(CQikFloatingPointEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplFloatEditor,aReturn);
#else
	CEikFloatingPointEditor* flPtEd=(CEikFloatingPointEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplFloatEditor,aReturn);
#endif
	flPtEd->ConstructL(aMin,aMax,18);
	TReal64 real(OplUtil::GetFloat(aReturn));
	if (real<aMin)
		real=aMin;
	else if (real>aMax)
		real=aMax;
#if defined(__UIQ__)
	flPtEd->SetValueL(real);
#else
	flPtEd->SetValueL(&real);
#endif
	}

void COplDialog::AddTextEditorL(const TDesC& aPrompt,TInt aWidthInChars,TInt aMaxLength,TText* aReturn)
	{ // aReturn is a pointer to the length byte
	CEikEdwin* edwin=(CEikEdwin*)CreateLineByTypeL(aPrompt,++iNextId,KOplTextEditor,aReturn);
	edwin->ConstructL(0,aWidthInChars,aMaxLength,1);
	TUint8* ptr8=(TUint8*)aReturn;
	TPtr ptr((TText*)(ptr8+1+KOplAlignment),*ptr8,aMaxLength);
	// Set the edwin from the des contents.
	edwin->SetTextL(&ptr);
	}

#if !defined(__SERIES60__)
CDesCArray* COplDialog::AddChoiceListL(const TDesC& aPrompt,TAny* aReturn,TBool aIncrementalMatching)
	{
	CEikChoiceList* list;
	if (iLastCreatedChoiceListId)
		{
		CEikCaptionedControl* line=Line(iLastCreatedChoiceListId);
		if (line->iReturnValue!=aReturn)
			User::Leave(KErrArgument);
		list=(CEikChoiceList*)(line->iControl);
		}
	else
		{
		list = (CEikChoiceList*)CreateLineByTypeL(aPrompt,++iNextId,KOplChoiceList,aReturn);
		list->ConstructL(list,0,0);
		CDesCArray* array=new(ELeave) CDesCArraySeg(4);
		list->SetArrayL(array); // takes ownership BEFORE it can leave!
		list->SetIncrementalMatching(aIncrementalMatching);
		iLastCreatedChoiceListId=iNextId;
		}
	return list->DesCArray();
	}
#else
CDesCArray* COplDialog::AddChoiceListL(const TDesC&,TAny*,TBool)
	{
	return (CDesCArray*)NULL;
	}
#endif

#if defined(__SERIES60__)
void COplDialog::ChoiceListFinishedL() {}
#else
void COplDialog::ChoiceListFinishedL()
	{
	if (!iLastCreatedChoiceListId)
		User::Leave(KOplStructure);
	CEikCaptionedControl* line=Line(iLastCreatedChoiceListId);
	CEikChoiceList* list=(CEikChoiceList*)(line->iControl);
	TInt current=(*(TUint16*)(line->iReturnValue));
	if (current==0)
		current=1;
	else 
		{
		TInt count=list->Array()->MdcaCount();
		if (current>count)
			current=count;
		}
	list->SetCurrentItem(current-1);
	iLastCreatedChoiceListId=0;
	}
#endif

void COplDialog::AddSecretEditorL(const TDesC& aPrompt,TInt aMaxLength,TText* aReturn, TBool aSeedTextContents)
	{
	CEikSecretEditor* secEd = (CEikSecretEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplSecretEditor,aReturn);
#if defined(__SERIES60__)
	TInt actualMaxLength=(aMaxLength<CEikSecretEditor::EMaxSecEdBufLength)?aMaxLength:CEikSecretEditor::EMaxSecEdBufLength;
#else
	TInt actualMaxLength=(aMaxLength<CEikSecretEditor::EMaxSecEdLength)?aMaxLength:CEikSecretEditor::EMaxSecEdLength;
#endif
	secEd->SetMaxLength(actualMaxLength);
	if (aSeedTextContents)
		{
		TUint8* ptr8=(TUint8*)aReturn;
		TPtr ptr((TText*)(ptr8+1+KOplAlignment),*ptr8,actualMaxLength);
		secEd->SetText(ptr);
		}
	}

void COplDialog::AddTextLabelL(const TDesC& aPrompt,const TDesC& aBody,TInt aFlags)
	{
	if (aPrompt.Length()==0 && aBody.Length()==0)
		{
		if (aFlags==0x800)
			{
			if (iNextId==KOplDialogIdBase) // no items added
				User::Leave(KOplStructure);
			Line(iNextId++)->SetDividerAfter(ETrue);
			}
		else
			User::Leave(KErrArgument);
		}
	else
		{
		TGulAlignment align=EHLeftVTop;
		if ((aFlags&0x3)==1)
			align=EHRightVTop;
		else if ((aFlags&0x3)==2)
			align=EHCenterVTop;
		TInt type=EEikCtLabel;
		if (aFlags&0x400)
			type=KOplTextLabel;
		CEikLabel* label=(CEikLabel*)CreateLineByTypeL(aPrompt,++iNextId,type,NULL);
		label->SetTextL(aBody);
		label->iAlignment=align;
		if (aFlags&0x200)
			Line(iNextId)->SetDividerAfter(ETrue);
		}
	}

void COplDialog::AddTimeEditorL(const TDesC& aPrompt,TUint aMinTime,TUint aMaxTime,TInt aFlags,TAny* aReturn)
	{// assumes aFlags and times are valid
#if defined(__UIQ__)
	TInt flags=0;
	if (aFlags&0x8)
		flags|=EQikTimeForce24HourFormat;
#else
	TInt flags=(aFlags&0x1)?0:EEikTimeWithoutSecondsField;
	if (aFlags&0x4)
		flags|=EEikTimeWithoutHoursField;
	if (aFlags&0x8)
		flags|=EEikTimeForce24HourFormat;
#endif
	if (~aFlags&0x2)
		{
#if defined(__UIQ__)
		CQikTimeEditor* timeEd = (CQikTimeEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplTimeEditor,aReturn);
#else
		CEikTimeEditor* timeEd = (CEikTimeEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplTimeEditor,aReturn);
#endif
		TInt64 maxInt=aMaxTime;
		TInt64 minInt=aMinTime;
		TInt64 initInt=TUint(OplUtil::GetLong(aReturn));
		if (initInt<minInt)
			initInt=minInt;
		else if (initInt>maxInt)
			initInt=maxInt;
		TTime max(maxInt*1000000);
		TTime min(minInt*1000000);
		TTime init(initInt*1000000);
		timeEd->ConstructL(min,max,init,flags);
		}
	else
		{
#if defined(__UIQ__)
		CQikDurationEditor* timeEd = (CQikDurationEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplDurationEditor,aReturn);
#else
		CEikDurationEditor* timeEd = (CEikDurationEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplDurationEditor,aReturn);
#endif
		TUint init=OplUtil::GetLong(aReturn);
		if (init<aMinTime)
			init=aMinTime;
		else if (init>aMaxTime)
			init=aMaxTime;
		timeEd->ConstructL(aMinTime,aMaxTime,init,flags);
		}
	}

void COplDialog::AddDateEditorL(const TDesC& aPrompt,TInt32 aMinDate,TInt32 aMaxDate,TAny* aReturn)
	{
	TDateTime baseDateTime(1900,TMonth(0),0,0,0,0,0);
	TTime baseTime(baseDateTime);
#if defined(__UIQ__)
	CQikDateEditor* dateEd=(CQikDateEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplDateEditor,aReturn);
#else
#if defined(__S80_DP2_0__)
	TBool realClipboardButtonSetting = Title().HasClipboardButton();
	Title().SetHasClipboardButton(ETrue);
	CEikDateEditor* dateEd=(CEikDateEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplDateEditor,aReturn);
	Title().SetHasClipboardButton(realClipboardButtonSetting);
#else
	CEikDateEditor* dateEd=(CEikDateEditor*)CreateLineByTypeL(aPrompt,++iNextId,KOplDateEditor,aReturn);
#endif
#endif
	TInt32 init=OplUtil::GetLong(aReturn);
	if (init<aMinDate)
		init=aMinDate;
	else if (init>aMaxDate)
		init=aMaxDate;
	dateEd->ConstructL(baseTime+TTimeIntervalDays(aMinDate),baseTime+TTimeIntervalDays(aMaxDate),baseTime+TTimeIntervalDays(init),EFalse);
										// with popout calendar
	}

#if !defined(__SERIES60__) && !defined(__SERIES90__)
void COplDialog::AddFileSelectorL(const TDesC& aPrompt1,const TDesC& aPrompt2,const TDesC& aPrompt3,TInt aFlags,const TUidType& aUidType,TText* aReturn)
	{
	COplFileNameSelector* fileNameSelector = (COplFileNameSelector*)CreateLineByTypeL(aPrompt1,++iNextId,KOplFileSelector,aReturn);
	fileNameSelector->ConstructL(fileNameSelector,aFlags);
	fileNameSelector->SetUidType(aUidType);
	AddValidatedItemL(fileNameSelector,iNextId);

	COplFolderNameSelector* folderNameSelector = (COplFolderNameSelector*)CreateLineByTypeL(aPrompt2,++iNextId,KOplFolderNameSel,NULL);
	folderNameSelector->ConstructL(folderNameSelector,aFlags);

	COplDriveNameSelector* driveNameSelector = (COplDriveNameSelector*)CreateLineByTypeL(aPrompt3,++iNextId,KOplDriveNameSel,NULL);
	driveNameSelector->ConstructL(driveNameSelector,EEikChlistIncrementalMatching,0);
	driveNameSelector->BuildDriveArrayL();

	folderNameSelector->SetCorrespondingDriveSelector(driveNameSelector);

	TParse parse;
	TUint8* ptr8=(TUint8*)aReturn;
	TPtrC ptr((TText*)(ptr8+1+KOplAlignment),*(ptr8));
	User::LeaveIfError(iCoeEnv->FsSession().Parse(ptr, parse));
	if (aFlags&KFileSelectorFlagShowSystem)
		folderNameSelector->SetShowSystem(ETrue);
	folderNameSelector->SetRootL(parse.Drive());
	if (parse.ExtPresent()&&(!parse.IsExtWild())&&(aFlags&KFileSelectorFlagDisallowExisting))
		fileNameSelector->SetExtensionL(parse.Ext());
	if (aFlags&KFileSelectorFlagAllowWildcards)
		fileNameSelector->SetMultipleSelectionAllowed(ETrue);
	if (aFlags&KFileSelectorFlagShowRom)
		driveNameSelector->ShowRom(ETrue);
	folderNameSelector->SetFileSelectionObserver(fileNameSelector);
	driveNameSelector->SetFileSelectionObserver(folderNameSelector);
	driveNameSelector->SetFullNameL(parse.FullName()); 
	}

void COplDialog::AddFileEditorL(const TDesC& aPrompt1,const TDesC& aPrompt2,const TDesC& aPrompt3,TInt aFlags,TText* aReturn)
	{
	COplFileNameEditor* fileNameEditor = (COplFileNameEditor*)CreateLineByTypeL(aPrompt1,++iNextId,KOplFileEditor,aReturn);
	fileNameEditor->ConstructL(aFlags);
	AddValidatedItemL(fileNameEditor,iNextId);

	COplFolderNameSelector* folderNameSelector = (COplFolderNameSelector*)CreateLineByTypeL(aPrompt2,++iNextId,KOplFolderNameSel,NULL);
	folderNameSelector->ConstructL(folderNameSelector,aFlags);

	COplDriveNameSelector* driveNameSelector = (COplDriveNameSelector*)CreateLineByTypeL(aPrompt3,++iNextId,KOplDriveNameSel,NULL);
	driveNameSelector->ConstructL(driveNameSelector,EEikChlistIncrementalMatching,0);
	driveNameSelector->BuildDriveArrayL();

	folderNameSelector->SetCorrespondingDriveSelector(driveNameSelector);

	TParse parse;
	TUint8* ptr8=(TUint8*)aReturn;
	TPtrC ptr((TText*)(ptr8+1+KOplAlignment),*(ptr8));
	User::LeaveIfError(iCoeEnv->FsSession().Parse(ptr, parse));
	if (aFlags&KFileSelectorFlagShowSystem)
		folderNameSelector->SetShowSystem(ETrue);
	folderNameSelector->SetRootL(parse.Drive());
	folderNameSelector->SetFileSelectionObserver(fileNameEditor);
	if (aFlags&KFileSelectorFlagShowRom)
		driveNameSelector->ShowRom(ETrue);
	driveNameSelector->SetFileSelectionObserver(folderNameSelector);
	driveNameSelector->SetFullNameL(parse.FullName()); 
	}

void COplDialog::AddFolderSelectorL(const TDesC& aPrompt1,const TDesC& aPrompt2,TInt aFlags,TText* aReturn)
	{
	COplFolderNameSelector* folderNameSelector = (COplFolderNameSelector*)CreateLineByTypeL(aPrompt1,++iNextId,KOplFolderNameSel,aReturn);
	folderNameSelector->ConstructL(folderNameSelector,aFlags);

	COplDriveNameSelector* driveNameSelector = (COplDriveNameSelector*)CreateLineByTypeL(aPrompt2,++iNextId,KOplDriveNameSel,NULL);
	driveNameSelector->ConstructL(driveNameSelector,EEikChlistIncrementalMatching,0);
	driveNameSelector->BuildDriveArrayL();

	folderNameSelector->SetCorrespondingDriveSelector(driveNameSelector);

	TParse parse;
	TUint8* ptr8=(TUint8*)aReturn;
	TPtrC ptr((TText*)(ptr8+1+KOplAlignment),*(ptr8));
	User::LeaveIfError(iCoeEnv->FsSession().Parse(ptr, parse));
	if (aFlags&KFileSelectorFlagShowSystem)
		folderNameSelector->SetShowSystem(ETrue);
	folderNameSelector->SetRootL(parse.Drive());
	if (aFlags&KFileSelectorFlagShowRom)
		driveNameSelector->ShowRom(ETrue);
	driveNameSelector->SetFileSelectionObserver(folderNameSelector);
	driveNameSelector->SetFullNameL(parse.DriveAndPath()); 
	}

void COplDialog::AddFolderEditorL(const TDesC& aPrompt1,const TDesC& aPrompt2,const TDesC& aPrompt3,TInt aFlags,TText* aReturn)
	{
	COplFolderNameEditor* folderNameEditor = (COplFolderNameEditor*)CreateLineByTypeL(aPrompt1,++iNextId,KOplFolderNameEd,aReturn);
	folderNameEditor->ConstructL(aFlags);
	AddValidatedItemL(folderNameEditor,iNextId);

	COplFolderNameSelector* folderNameSelector = (COplFolderNameSelector*)CreateLineByTypeL(aPrompt2,++iNextId,KOplFolderNameSel,NULL);
	folderNameSelector->ConstructL(folderNameSelector,aFlags);

	COplDriveNameSelector* driveNameSelector = (COplDriveNameSelector*)CreateLineByTypeL(aPrompt3,++iNextId,KOplDriveNameSel,NULL);
	driveNameSelector->ConstructL(driveNameSelector,EEikChlistIncrementalMatching,0);
	driveNameSelector->BuildDriveArrayL();

	folderNameSelector->SetCorrespondingDriveSelector(driveNameSelector);

	TParse parse;
	TUint8* ptr8=(TUint8*)aReturn;
	TPtrC ptr((TText*)(ptr8+1+KOplAlignment),*(ptr8));
	User::LeaveIfError(iCoeEnv->FsSession().Parse(ptr, parse));
	if (aFlags&KFileSelectorFlagShowSystem)
		folderNameSelector->SetShowSystem(ETrue);
	folderNameSelector->SetRootL(parse.Drive());
	folderNameSelector->SetFileSelectionObserver(folderNameEditor);
	if (aFlags&KFileSelectorFlagShowRom)
		driveNameSelector->ShowRom(ETrue);
	driveNameSelector->SetFileSelectionObserver(folderNameSelector);
	driveNameSelector->SetFullNameL(parse.DriveAndPath()); 
	}
#else
void COplDialog::AddFileSelectorL(const TDesC&, const TDesC&, const TDesC&, TInt, const TUidType&, TText*) {}
void COplDialog::AddFileEditorL(const TDesC&, const TDesC&, const TDesC&, TInt, TText*) {}
void COplDialog::AddFolderSelectorL(const TDesC&, const TDesC&, TInt, TText*) {}
void COplDialog::AddFolderEditorL(const TDesC&, const TDesC&, const TDesC&, TInt, TText*) {}
#endif

/*
void COplDialog::AddCheckBoxL(const TDesC& aPrompt,TInt16* aReturn)
	{
	CEikCheckBox* checkBox=(CEikCheckBox*)CreateLineByTypeL(aPrompt,++iNextId,KOplCheckBox,aReturn);
	TInt init=*(aReturn);
	checkBox->SetState(init?CEikButtonBase::ESet:CEikButtonBase::EClear);
	}
*/

void COplDialog::AddMultiLineEditorL(const TDesC& aPrompt,TInt aNumOfLines,TInt aWidthInChars,TInt aMaxLength,TInt32* aReturn, TBool aReadOnly)
	{
	TInt length=OplUtil::GetLong(aReturn);
	if (length<0 || length>aMaxLength)
		User::Leave(KErrArgument);
	CEikEdwin* edwin=(CEikEdwin*)CreateLineByTypeL(aPrompt,++iNextId,KOplMultiLineTextEditor,aReturn);
	Line(iNextId)->SetTakesEnterKey(ETrue);
	edwin->ConstructL(CEikEdwin::EInclusiveSizeFixed,aWidthInChars,aMaxLength,aNumOfLines);
	edwin->CreateScrollBarFrameL()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto);
	TPtr ptr((TText*)(aReturn+1),length,aMaxLength); // KOplAlignment not needed
	edwin->SetTextL(&ptr);
	edwin->SetReadOnly(aReadOnly);
	}

void COplDialog::GetHelpContext(TCoeHelpContext& aContext) const
	{
/* If this dialog is being displayed by Uikon and the user hits the Help key then
 * Cone's CCoeAppUi calls GetHelpContext() for each control on the control stack.
 * So, have to return the help context for this control (if the OPL author has 
 * set one using SetHelp control%,context$) or null.
 *
 * If the author has set one, it'll be in the dialog'th element of the help context
 * array iHelpContextNamesArray.
 */
	COplRuntime* rt=TheRuntime(); // expensive call, so cache it here.
	TFixedArray<TCoeContextName,KOplLenContextNamesArray>* helpContext=rt->HelpContextNamesArray();
	if (helpContext)
		{
		TCoeContextName dialogContext=helpContext->At(EOplHelpDialog);
		TUid helpUid=rt->HelpUid();
		if (dialogContext.Length() && helpUid!=KNullUid)
			{
			aContext=TCoeHelpContext(helpUid,dialogContext);
			}
		}
	}

//
// Dialog OpCodes
//
void FuncOpCode::Dialog(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	COplDialog* dialog=aRuntime.Dialog();
	if ((dialog==NULL)||(dialog->InitFrame()!=aFramePtr))
		User::Leave(KOplStructure);
	aRuntime.SetDialog(NULL);
	CleanupStack::PushL(dialog);
	TInt16 ret;
#if defined(__S80_DP2_0__)
	dialog->ButtonGroupContainer().MakeVisible(dialog->HasButtons());
#endif
	dialog->RunLD(&ret);
	aStack.Push(ret);
	}

void OpCode::dInit(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	// MUST for real code have one dialog ptr in aRuntime
	// Must save frame pointer in aRuntime
	// Must save item ID in aRuntime

	delete aRuntime.Dialog();
	aRuntime.SetDialog(NULL); //set to NULL incase the new leaves
	TInt noOfParams=aRuntime.IP8();
	TInt flags=EEikDialogFlagWait|EEikDialogFlagAllKeysToButtons /*|EEikDialogFlagNoBackup*/;
	TPtrC title;
	if (noOfParams==2)
		{
		TInt f=aStack.PopInt16();
		flags|=((f&EOplDialogFlagsButtonsRight)?EEikDialogFlagButtonsRight:EEikDialogFlagButtonsBelow);
		if (f&EOplDialogFlagsNoTitleBar)
			flags|=EEikDialogFlagNoTitleBar;
		if (f&EOplDialogFlagsFillScreen)
			flags|=EEikDialogFlagFillScreen;
		if (f&EOplDialogFlagsNoDrag)
			flags|=EEikDialogFlagNoDrag;
		if (f&EOplDialogFlagsDensePacking)
			flags|=EEikDialogFlagDensePacking;
		}
	if (noOfParams>0)
		{
		title.Set(aStack.PopString());
		}
	COplDialog* dialog=new(ELeave) COplDialog();
#if defined(__UIQ__)
	dialog->ConstructLC(aFramePtr);
#else
	dialog->ConstructLC(aFramePtr,flags);
#endif
	if (title.Length()!=0)
		dialog->AddTitleL(title);
	aRuntime.SetDialog(dialog);
	CleanupStack::Pop();
	}

LOCAL_C void ParseForPromptsL(const TDesC& aString,TPtrC& aP1,TPtrC& aP2,TPtrC& aP3)
	{
	TInt pos=aString.Locate(',');
	if (pos==KErrNotFound)
		{
		aP1.Set(aString);
		return;
		}
	aP1.Set(aString.Ptr(),pos);
	if (pos>=aString.Length())
		return;
	TPtrC rest=aString.Mid(pos+1);
	pos=rest.Locate(',');
	if (pos==KErrNotFound)
		{
		aP2.Set(rest);
		return;
		}
	aP2.Set(rest.Ptr(),pos);
	if (pos<rest.Length())
		{
		aP3.Set(rest.Mid(pos+1));
		if (aP3.Locate(',')!=KErrNotFound)
			User::Leave(KErrArgument);
		}
	}

struct SOplButton
	{
	TInt iId;
	TBool iDefaultCommand;
	TPtrC iButtonText;
	CEikButtonGroupContainer::THotKeyFlags iHotkeyFlags;
	};

const TInt KMaxSupportedButtons=4;

void OpCode::dItem(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	// Handle a control item in a dialog
	{
	COplDialog* dialog=aRuntime.Dialog();
	
	aRuntime.SetDialog(NULL); // temporarily take ownership
	CleanupStack::PushL(dialog);

	if ((dialog==NULL)||(dialog->InitFrame()!=aFramePtr))
		User::Leave(KOplStructure);
	TInt itemType=aRuntime.IP8();
	switch (itemType)
		{
	case 0:
		{
		// dTEXT
		TInt flags=(aRuntime.IP8()?aStack.PopInt16():0);
		TPtrC body=aStack.PopString();
		TPtrC prompt=aStack.PopString();
		dialog->AddTextLabelL(prompt,body,flags);
		break;
		}
	case 1:
		{
		// dCHOICE
		TInt numOptionalArgs = aRuntime.IP8();
		TBool incrementalMatching=EFalse;
		if (numOptionalArgs==1)
			incrementalMatching = (TBool)(aStack.PopInt16());
		TPtrC list=aStack.PopString();
		TPtrC prompt=aStack.PopString();
		aStack.PopInt16(); // field flag
		TInt16* ptr=&aStack.PopRefInt16();
		CDesCArray* array=dialog->AddChoiceListL(prompt,ptr,incrementalMatching);
		TChar comma(',');
		TInt remainder=list.Length();
		TInt pos=list.Locate(comma);
		while (pos>0)
			{
			array->AppendL(list.Left(pos));
			remainder-=(pos+1);
			list.Set(list.Right(remainder));
			pos=list.Locate(comma);
			}
		if (pos==0||list.Length()==0)
			User::Leave(KErrArgument);
		_LIT(KEllipses,"...");
		if (list!=KEllipses)
			{
			array->AppendL(list);
			dialog->ChoiceListFinishedL();
			}
		break;
		}
	case 2:
		{
		// dLONG
		// Stack (tos 1st)= {max&,min&,prompt$,leftSideVar}
		TInt max=aStack.PopInt32();
		TInt min=aStack.PopInt32();
		if (min>max)
			User::Leave(KErrArgument);
		TPtrC prompt=aStack.PopString();
		aStack.PopInt16(); // field flag
		TInt32* pLive=&(aStack.PopRefInt32());
		dialog->AddLongEditorL(prompt,min,max,pLive);
		break;
		}
	case 3:
		{
		// dFLOAT
		TReal64 max=aStack.PopReal64();
		TReal64 min=aStack.PopReal64();
		if (min>max)
			User::Leave(KErrArgument);
		TPtrC prompt=aStack.PopString();
		aStack.PopInt16(); // field flag
		dialog->AddFloatEditorL(prompt,min,max,&aStack.PopRefReal64());
		break;
		}
	case 4:
		{
		// dTIME
		TUint max=aStack.PopInt32();
		TUint min=Abs(aStack.PopInt32());
		TInt16 flags=aStack.PopInt16();
		if (flags<0||flags>0xf||min>max||max>=86400)
			User::Leave(KErrArgument);
		TPtrC prompt=aStack.PopString();
		aStack.PopInt16(); // field flag
		dialog->AddTimeEditorL(prompt,min,max,flags,&aStack.PopRefInt32());
		break;
		}
	case 5:
		{
		// dDATE
		TInt32 max=aStack.PopInt32();
		TInt32 min=Abs(aStack.PopInt32());
		if (min>max)
			User::Leave(KErrArgument);
		TPtrC prompt=aStack.PopString();
		aStack.PopInt16(); // field flag
		dialog->AddDateEditorL(prompt,min,max,&aStack.PopRefInt32());
		break;
		}
	case 6:
	case 7:
		{
		// case 7 is dEDIT with len%
		// case 6 is dEDIT without len%
		TInt widthInChars=((itemType==6)?0:aStack.PopInt16());
		TPtrC prompt=aStack.PopString();
		aStack.PopInt16(); // field flag
		TText* ptr=aStack.PopRefString();
		TInt maxLen=aStack.PopInt16();
		if (itemType==6)
			widthInChars=maxLen;
		dialog->AddTextEditorL(prompt,widthInChars,maxLen,ptr);
		break;
		}
	case 8:
		{
		// dXINPUT
		TInt numOptionalArgs = aRuntime.IP8();
		TBool seedTextContents=EFalse;
		if (numOptionalArgs==1)
			seedTextContents = (TBool)(aStack.PopInt16());
		TPtrC prompt=aStack.PopString();
		aStack.PopInt16(); // field flag
		TText* ptr=aStack.PopRefString();
		TInt maxLen=aStack.PopInt16();
		dialog->AddSecretEditorL(prompt,maxLen,ptr,seedTextContents);
		break;
		}
	case 9:
		{
		// dFILE
		TUid uid3=TUid::Uid(aStack.PopInt32());
		TUid uid2=TUid::Uid(aStack.PopInt32());
		TUid uid1=TUid::Uid(aStack.PopInt32());
		TUidType uidType(uid1,uid2,uid3);
		TInt16 flags=aStack.PopInt16();
		TPtrC prompt=aStack.PopString();

		TPtrC p1;
		TPtrC p2;
		TPtrC p3;
		ParseForPromptsL(prompt,p1,p2,p3);

		aStack.PopInt16(); // field flag
		TText* ptr=aStack.PopRefString();
		if (aStack.PopInt16()<255) // used to be 128
			User::Leave(KErrArgument);
		if (flags&KFileSelectorFlagAllowDirsOnly) // folders only
			{
			if (flags&KFileSelectorFlagEditor) //editor
				dialog->AddFolderEditorL(p1,p2,p3,flags,ptr);
			else //selector
				{
				if (p3.Length()!=0)
					User::Leave(KErrArgument);
				dialog->AddFolderSelectorL(p1,p2,flags,ptr);
				}
			}
		else
			{
			if (flags&KFileSelectorFlagEditor) //editor
				dialog->AddFileEditorL(p1,p2,p3,flags,ptr);
			else //selector
				dialog->AddFileSelectorL(p1,p2,p3,flags,uidType,ptr);
			}
		break;
		}
	case 10:
		{
		// Can't call dBUTTONs more than once in the same dialog
		if (dialog->HasButtons())
			User::Leave(KOplStructure);
		const TInt butCount=aRuntime.IP8();// Number of buttons in container
		if (butCount>KMaxSupportedButtons)
			User::Leave(KOplStructure);

		// Create the empty container.
#if !defined(__UIQ__)
#if !defined(__S80_DP2_0__)
		CEikButtonGroupContainer* buttonPanel=
			CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::EDialog,
				CEikButtonGroupContainer::EVertical, dialog->ButtonCommandObserver(), 
				NULL, *dialog);
#else
		CEikButtonGroupContainer* buttonPanel = &(dialog->ButtonGroupContainer());
#endif
#else
//!!TODOUIQ Where are the uiq classes here?
		CEikButtonGroupContainer* buttonPanel=
			CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::EDialog,
				CEikButtonGroupContainer::EHorizontal, dialog->ButtonCommandObserver(), 
				NULL, *dialog);
#endif
#if !defined(__S80_DP2_0__)
		CleanupStack::PushL(buttonPanel);
#endif
		// Create some buttons
		CArrayFixSeg<SOplButton>* buttons=new(ELeave) CArrayFixSeg<SOplButton>(KMaxSupportedButtons);
		CleanupStack::PushL(buttons);
		SOplButton buttonAndId;
		SOplButton unusedButton;
		unusedButton.iId=0;

		for (TInt ii=butCount;--ii>=0;)
			{
			TInt id=aStack.PopInt16();
			TBool isCancel=EFalse;
			if (id<0)
				{
				id=(-id);
				isCancel=ETrue;
				}
			CEikButtonGroupContainer::THotKeyFlags hotKeyFlags=CEikButtonGroupContainer::EShowHotKey;
			if (id&EOplDialogButtonPlainHotKey)
				hotKeyFlags=STATIC_CAST(CEikButtonGroupContainer::THotKeyFlags,hotKeyFlags|CEikButtonGroupContainer::EPlainHotKey);
			if (id&EOplDialogButtonHideHotKey)
				hotKeyFlags=STATIC_CAST(CEikButtonGroupContainer::THotKeyFlags,hotKeyFlags&~CEikButtonGroupContainer::EShowHotKey);
			id&=~(EOplDialogButtonHideHotKey|EOplDialogButtonPlainHotKey); // Remove flags
			switch(id)
				{
			case 0:
				id=EEikBidBlank;
				break;
			case EKeyBackspace:
				id=EEikBidDelete;
				break;
			case EKeyTab:
				id=EEikBidTab;
				break;
			case EKeyEnter:
				id=EEikBidOk;
				break;
			case EKeyEscape:
				id=EEikBidCancel;
				break;
			case EKeySpace:
				id=EEikBidSpace;
				break;
			default:
				id=User::LowerCase(id);
				if (!(id>='a' && id<='z'))
					User::Leave(KErrArgument);
				break;
				}
			buttonAndId.iDefaultCommand=(id==EEikBidOk);
			buttonAndId.iId=(isCancel)?EEikBidCancel:id;
			if (id==EEikBidBlank)
				{
				aStack.PopString(); // Is this needed anyway just to keep stack in order?
				buttonAndId.iButtonText.Set(KNullDesC); // Ensure text is blank
				}
			else
				buttonAndId.iButtonText.Set(aStack.PopString());
			buttonAndId.iHotkeyFlags=hotKeyFlags;

// By default, populate the buttons array according to the Series 80 style guide.
// For a single button OR a button of value EEikBidOk, move it to the last CBA button
// for cancel (Close in the latter case). Otherwise it stays at the top (BUT note it may get
// 'hijacked' by the current control if this is a dCHOICE - this is Series 80 style)

			switch(butCount)
				{
			case 1:
				if (buttonAndId.iId==EEikBidCancel || buttonAndId.iId==EEikBidOk)
					{
					buttons->AppendL(buttonAndId); // Cancel or Close on CBA4.
					buttons->AppendL(unusedButton);
					buttons->AppendL(unusedButton);
					buttons->AppendL(unusedButton);
					}
				else
					{
					buttons->AppendL(unusedButton);
					buttons->AppendL(unusedButton);
					buttons->AppendL(unusedButton);
					buttons->AppendL(buttonAndId); // CBA1
					}
				break;
			case 2:
				buttons->AppendL(buttonAndId);
				if (ii==1)
					{
					buttons->AppendL(unusedButton);
					buttons->AppendL(unusedButton);
					}
				break;
			case 3:
				buttons->AppendL(buttonAndId);
				if (ii==2)
					{
					buttons->AppendL(unusedButton);
					}
				break;
			default:
				buttons->AppendL(buttonAndId);
				break;
				}
			}

		TInt buttonPosition=0;
		for (TInt i=KMaxSupportedButtons;--i>=0;)
			{
			if ((*buttons)[i].iId) // only a real button has an id.
				{
#if !defined(__UIQ__)
				buttonPanel->SetCommandL(buttonPosition,(*buttons)[i].iId,(*buttons)[i].iButtonText);
#else
				buttonPanel->AddCommandL(buttonPosition,(*buttons)[i].iId,(*buttons)[i].iButtonText);
				buttonPosition++;
#endif
				if ((*buttons)[i].iDefaultCommand)
					buttonPanel->SetDefaultCommand((*buttons)[i].iId);
				}
			else
				{
#if !defined(__UIQ__)
				buttonPanel->DimCommand(buttonPosition,ETrue);
#endif
				}
#if !defined(__UIQ__)
			buttonPosition++;
#endif
			}

		// HotKey only supported for dialogs.
		// buttonPanel->UpdateHotKey(but.iId,but.iHotkeyFlags,User::UpperCase(but.iId));

#if !defined(__UIQ__)
		// Mark any remaining CBAs as dimmed so they can't be pressed.
		const TInt max=buttonPanel->MaxCommands();
		while(buttonPosition<max)
			{
			buttonPanel->DimCommand(buttonPosition++,ETrue);
			}
#endif

#if !defined(__S80_DP2_0__)
		// Finally, having built the new buttonGroupContainer, swap it in.
		CEikButtonGroupContainer* oldButtons=dialog->SwapButtonGroupContainer(buttonPanel);
		delete oldButtons;
#endif
		dialog->HasButtons(ETrue);
		CleanupStack::PopAndDestroy(); // buttons
#if !defined(__S80_DP2_0__)
		CleanupStack::Pop(); // buttonPanel
#endif
		break;
		}
	case 11:
		{
		// dPOSITION
		TInt yPos=aStack.PopInt16();
		TInt xPos=aStack.PopInt16();
		if (Abs(xPos)>1||Abs(yPos)>1)
			User::Leave(KErrArgument);
		dialog->SetCorner(xPos,yPos);
		break;
		}
	default:
		User::Leave(KErrGeneral);
		}

	aRuntime.SetDialog(dialog);
	CleanupStack::Pop(); // runtime owns the dialog again
	}

/*
void OpCode::dCheckBox(CStack& aStack,COplRuntime& aRuntime,CFrame* aFramePtr)
	{
	COplDialog* dialog=aRuntime.Dialog();

	aRuntime.SetDialog(NULL); // temporarily take ownership
	CleanupStack::PushL(dialog);
	
	if ((dialog==NULL)||(dialog->InitFrame()!=aFramePtr))
		User::Leave(KOplStructure);
	TPtrC prompt=aStack.PopString();
	aStack.PopInt16(); // field flag
	TInt16* pLive=&(aStack.PopRefInt16());
	dialog->AddCheckBoxL(prompt,pLive);

	aRuntime.SetDialog(dialog);
	CleanupStack::Pop(); // runtime owns the dialog again
	}
*/

void OpCode::dEditMulti(CStack& aStack,COplRuntime& aRuntime,CFrame* aFramePtr)
	{
	TInt numOptionalArgs = aRuntime.IP8();
	COplDialog* dialog=aRuntime.Dialog();

	aRuntime.SetDialog(NULL); // temporarily take ownership
	CleanupStack::PushL(dialog);
	
	if ((dialog==NULL)||(dialog->InitFrame()!=aFramePtr))
		User::Leave(KOplStructure);
	TBool readOnly=EFalse;
	if (numOptionalArgs==1)
		readOnly = (TBool)(aStack.PopInt16());
	TInt maxLen=aStack.PopInt32();
	TInt numOfLines=aStack.PopInt16();
	TInt widthInChars=aStack.PopInt16();
	TPtrC prompt=aStack.PopString();
	TInt32* pLive=(TInt32*)OplUtil::PopOffsetAsAddrL(aStack,aRuntime.Heap64(),sizeof(TInt32)*2);
	dialog->AddMultiLineEditorL(prompt,numOfLines,widthInChars,maxLen,pLive,readOnly);

	aRuntime.SetDialog(dialog);
	CleanupStack::Pop(); // runtime owns the dialog again
	}

//
// Alert Support
//
void FuncOpCode::Alert(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TInt numArgs = aRuntime.IP8();
	TPtrC txt1(KNullDesC);
	TPtrC txt2(KNullDesC);

	TPtrC but1(KNullDesC);
	TPtrC but2(KNullDesC);
	TPtrC but3(KNullDesC);

	switch (numArgs)
		{
	case 5:
		but3.Set(aStack.PopString());
	case 4:
		but2.Set(aStack.PopString());
	case 3:
		but1.Set(aStack.PopString());
	case 2:
		txt2.Set(aStack.PopString());
	case 1:
		txt1.Set(aStack.PopString());
		}
	aStack.Push((TInt16)(aRuntime.AlertWin(txt1,txt2,but1,but2,but3)+1));
	}

TInt COplRuntime::AlertWin(const TDesC& aLine1,const TDesC& aLine2,const TDesC& aBut1,const TDesC& aBut2,const TDesC& aBut3)
	{
	TInt ret=0;
	__ASSERT_DEBUG(iOplAlertWin,User::Invariant());
	iOplAlertWin->RunAlert(aLine1,aLine2,aBut1,aBut2,aBut3,ret);
	return ret;
	}

//
// Class COplAlertWin
//
COplAlertWin::COplAlertWin()
	{
	iExitOnEnter=EFalse;
	iButtonGroupContainerFocus=ENoButtonGroupContainer;
	}

void COplAlertWin::ConstructL(CCoeEnv* aCoeEnv)
	{
	ConstructSleepingDialogL(R_OPL_DIALOG_ALERT); // includes three button panel
	Window().SetOrdinalPosition(0,ECoeWinPriorityAlwaysAtFront-1);
#if !defined(__UIQ__)
	SetLabelReserveLengthL(EOplCidAlertText1,KMaxLineLength);
	SetLabelReserveLengthL(EOplCidAlertText2,KMaxLineLength);
#endif
	iTwoButtonGroupContainer=
		CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::EDialog,
			CEikButtonGroupContainer::EVertical,
			this->ButtonCommandObserver(), R_OPL_BUTTONS_CANCEL_OK, *this);
	iTwoButtonGroupContainer->MakeVisible(EFalse);

	iThreeButtonGroupContainer=SwapButtonGroupContainer(iTwoButtonGroupContainer);
	iButtonGroupContainerFocus=ETwoButtonGroupContainer;

	iOneButtonGroupContainer=
		CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::EDialog,
			CEikButtonGroupContainer::EVertical,
			this->ButtonCommandObserver(), R_OPL_BUTTONS_CANCEL, *this);
	iOneButtonGroupContainer->MakeVisible(EFalse);
	iTwoButtonGroupContainer=SwapButtonGroupContainer(iOneButtonGroupContainer);
	iButtonGroupContainerFocus=EOneButtonGroupContainer;

	SetButtonTextReserveLengthL(iThreeButtonGroupContainer,EEikBidCancel,KMaxButLength);
	SetButtonTextReserveLengthL(iThreeButtonGroupContainer,EEikBidSpace,KMaxButLength);
	SetButtonTextReserveLengthL(iThreeButtonGroupContainer,EEikBidOk,KMaxButLength);
	SetButtonTextReserveLengthL(iTwoButtonGroupContainer,EEikBidCancel,KMaxButLength);
	SetButtonTextReserveLengthL(iTwoButtonGroupContainer,EEikBidOk,KMaxButLength);
	SetButtonTextReserveLengthL(iOneButtonGroupContainer,EEikBidCancel,KMaxButLength);
	iContinueText=aCoeEnv->AllocReadResourceL(R_OPLR_TBUF_CONTINUE_TEXT);
	}

void COplAlertWin::SetButtonTextReserveLengthL(const CEikButtonGroupContainer* aButtonPanel,const TInt aButtonId,const TInt aReserveLength)
	{
	aButtonPanel->ButtonById(aButtonId)->Label()->SetBufferReserveLengthL(aReserveLength);
	}

void COplAlertWin::RunAlert(const TDesC& aLine1,const TDesC& aLine2,const TDesC& aBut1,const TDesC& aBut2,const TDesC& aBut3,TInt& aReturn)
	{
	iReturnPtr=&aReturn;
	Set(iLine1,aLine1,KMaxLineLength);
	Set(iLine2,aLine2,KMaxLineLength);
	Set(iBut1,aBut1,KMaxButLength);
	Set(iBut2,aBut2,KMaxButLength);
	Set(iBut3,aBut3,KMaxButLength);
	RouseSleepingDialog();
	}

void COplAlertWin::Set(TPtrC& aTrg,const TDesC& aSrc,TInt aMaxLen)
	{
	TInt len=aSrc.Length();
	aTrg.Set(aSrc.Ptr(),(len>aMaxLen)?aMaxLen:len);
	}

void COplAlertWin::ExchangeButtonGroupContainer(TButtonGroupContainerFocus aNewFocus) 
	{
	if (aNewFocus==iButtonGroupContainerFocus)
		return;
	switch(iButtonGroupContainerFocus)
		{
	case EOneButtonGroupContainer:
		if (aNewFocus==ETwoButtonGroupContainer)
			iOneButtonGroupContainer=SwapButtonGroupContainer(iTwoButtonGroupContainer);
		else
			iOneButtonGroupContainer=SwapButtonGroupContainer(iThreeButtonGroupContainer);
		break;
	case ETwoButtonGroupContainer:
		if (aNewFocus==EOneButtonGroupContainer)
			iTwoButtonGroupContainer=SwapButtonGroupContainer(iOneButtonGroupContainer);
		else
			iTwoButtonGroupContainer=SwapButtonGroupContainer(iThreeButtonGroupContainer);
		break;
	case EThreeButtonGroupContainer:
	default:
		if (aNewFocus==EOneButtonGroupContainer)
			iThreeButtonGroupContainer=SwapButtonGroupContainer(iOneButtonGroupContainer);
		else
			iThreeButtonGroupContainer=SwapButtonGroupContainer(iTwoButtonGroupContainer);
		break;
		}
	iButtonGroupContainerFocus=aNewFocus;
	}

void COplAlertWin::PreLayoutDynInitL()
	{
	TInt butCount=1;
#if defined(__UIQ__)
	CEikLabel* label = static_cast<CEikLabel*>(Control(EOplCidAlertText1));
	label->SetTextL(iLine1);
	label = static_cast<CEikLabel*>(Control(EOplCidAlertText2));
	label->SetTextL(iLine2);
#else
	SetLabelL(EOplCidAlertText1,iLine1);
	SetLabelL(EOplCidAlertText2,iLine2);
#endif
	if (iBut1.Length() && iBut2.Length())
		{
		if (iBut3.Length())
			{
			ExchangeButtonGroupContainer(EThreeButtonGroupContainer);
			butCount=3;
			}
		else
			{
			ExchangeButtonGroupContainer(ETwoButtonGroupContainer);
			butCount=2;
			}
		}
	else
		ExchangeButtonGroupContainer(EOneButtonGroupContainer);

//	CEikButtonGroupContainer* butPanel=ButtonPanel();
//	butPanel->ResetMinimumSize();
	CEikButtonGroupContainer& btGrp=ButtonGroupContainer();
	iExitOnEnter=EFalse;
	if (iBut1.Length())
		{
		btGrp.SetCommandL(EEikBidCancel,iBut1);
		if (butCount>=2)
			{
			btGrp.SetCommandL(EEikBidOk,iBut2);
			iExitOnEnter=ETrue;
			if (butCount==3)
				{
				btGrp.SetCommandL(EEikBidSpace,iBut3);
				}
			}
		}
	else
		{
		btGrp.SetCommandL(EEikBidCancel,*iContinueText);
		}
	}

TBool COplAlertWin::OkToExitL(TInt aButtonId)
	{
	switch (aButtonId)
		{
	case EEikBidSpace:
		*iReturnPtr=2;
		break;
	case EEikBidOk:
		if (!iExitOnEnter)
			return EFalse;
		*iReturnPtr=1;
		break;
		}	
	return ETrue;
	}

COplAlertWin::~COplAlertWin()
	{
	ExchangeButtonGroupContainer(EOneButtonGroupContainer); // dialog deletes this
	delete iTwoButtonGroupContainer;
	delete iThreeButtonGroupContainer;
	delete iContinueText;
	}
