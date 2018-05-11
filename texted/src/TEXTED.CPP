// TEXTED.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <txtrich.h>
#include <txtglobl.h>
#include <txtfmlyr.h>

#if defined(__UIQ__) 
 #include <qiknumbereditor.h> 
 #include <eikchlst.h> 
 #include <eikcfdlg.h> 
 #include <eikcore.rsg> 
 #define  R_EIK_TBUF_SAVED R_EIK_TBUF_FILE_SAVED 
#else 
 #include <sendui.h>
 #include <sendnorm.rsg>
 #include <eikfontd.h>
 #if !defined(__SERIES90__)
  #include <eikprtpv.h>
  #include <eikprtdg.h>
  #include <indicato.rsg>
 #endif

 #include <ckntitle.h>
 #include <cknconf.h>
 #include <ckndgopn.h>
 #include <ckndgsve.h>
 #if defined(USE_ADDTODESK)
  #if defined(__S80_DP2_0__)
   #include <addtodesk.h>
   #include <eikmenup.h>
  #else
   #include <linkbase.h>
   #include <linkdocument.h>
  #endif
 #endif

#if !defined(__SERIES90__)
 #include <linkutils.h>
#endif
 #include <eikfard.h>

#endif

#include <coeutils.h>

#include <s32file.h>
#include <e32uid.h>
#include <eikon.hrh>

#include <eikenv.h>
#include <eikproc.h>
#include <eikdll.h>
#include <gulutil.h>

#include <prnsetup.h>
#include <frmprint.h>
#include <eiktbar.h>
#include <eikbtgpc.h>
#include <eiklabel.h>
#include <eikmenub.h>
#include <eikmenup.h>
#include <eikfnlab.h>
#include <eikbtpan.h>
#include <eikcmbut.h>
#include <eikfutil.h>
#include <eikmsg.h>
#include <eiktxlbm.h>
#include <eiktxlbx.h>

#if !defined(__SERIES90__)
#include <eikspane.h>
#endif

#include <bautils.h>




#include <apgicnfl.h>

#include <eikon.rsg>
#include <texted.rsg>
#include <eikon.mbg>
#include "texted.hrh"

#include "txtedsrc.h"
#include "txtedpan.h"

#include <apadef.h>
#include <apacmdln.h>
#include <apfdef.h>
#include <bafindf.h>

#include <oplapi.h>

#include "opl.hlp.hrh" 
#include "texted.h"

//#define _USE_FILELOGGING
#include "debug.h"

// Comment this out to lose the app close panic due to messaging's open RHandle assert
// This should no longer be a problem, however: COAKL07, 09/09/2001

GLDEF_C void Panic(TTextedPanic aPanic)
	{
	_LIT(KAppName,"Program"); // external name is Program
	User::Panic(KAppName,aPanic);
	}

#define KTextedFontControlFlags EGulFontControlItalic|EGulFontControlUnderline|EGulFontControlStrikethrough|EGulFontControlPrintPos

_LIT(KOplExtension,".opl");
_LIT(KOpoExtension,".opo");
_LIT(KAppExtension,".app");
_LIT(KDefaultTranslatorName,"OPLT");
_LIT(KOplRuntimeLibraryName,"Opl.app");
_LIT(KDefaultAppPath,"\\System\\Apps\\.app");
_LIT(KDefaultOplIncludePath,"\\System\\OPL\\");
_LIT(KOplDefaultFilePath,"C:\\");

// For import/export of text
const TInt KFullFormattingUpperThreshold=2000; // from EIKEDWIN.CPP
_LIT(KTextExtension,".txt");
_LIT(KCarriageReturnLineFeed, "\x0d\x0a"); // the DOS/Windows line-terminating character-sequence
_LIT(KLineFeed, "\x0a"); // the Unix line-terminating character

const TInt KUidOplTranslatorProviderValue=0x100001D3;
const TUid KUidOplTranslatorProvider={KUidOplTranslatorProviderValue};

const TInt KUidOplDebuggerValue=0x10000269;
const TUid KUidOplDebugger={KUidOplDebuggerValue};

const TUid KUidInterpreterObj={0x100055C0}; 
const TUid KUidOplInterpreter={0x10005D2E};

// Restrictions on suitable MTMs for SendUi
const TInt KSendAsMaxBodySizeNotUsed=0;
const TInt KSendAsMaxMessageSizeNotUsed=0;

//
// Class CTextEdGlobalTextEditor
//
CTextEdGlobalTextEditor::CTextEdGlobalTextEditor(const TBool aFindHack)
	:iFindHack(aFindHack)
	{
	}

void CTextEdGlobalTextEditor::SetAutoIndent(TBool aAutoIndent)
	{
	iAutoIndent=aAutoIndent;
	}

void CTextEdGlobalTextEditor::SetupForFind()
	{
#if defined(__S80_DP2_0__)
	iFindStarted = ETrue;
#else
	iFindStarted = EFalse;
#endif
	iFindHack=0;
	}

TKeyResponse CTextEdGlobalTextEditor::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	TKeyEvent keyEvent=aKeyEvent;
	if (keyEvent.iCode==EKeyEnter || keyEvent.iCode==EKeySpace)	// remove shift to stop forced
																// line breaks and non-breaking
																// spaces
		keyEvent.iModifiers&=~(EModifierShift|EModifierLeftShift|EModifierRightShift);
	if ((keyEvent.iCode==EKeyTab || keyEvent.iCode==EKeyEnter) && SelectionLength())
		ClearSelectionL(); // is this needed when undo is added !!!!
	CEikGlobalTextEditor::OfferKeyEventL(keyEvent,aType);
	if (keyEvent.iCode==EKeyEnter&&!(keyEvent.iModifiers&EModifierCtrl)&&IsAutoIndenting())
		{
		CGlobalText* globalText=GlobalText();
		TInt curPos=CursorPos();
		__ASSERT_DEBUG(curPos>0,Panic(ETextedPanicInvalidCursorPosition));
		TInt pos=curPos-1;
		TInt maxLen=globalText->ToParagraphStart(pos);
		TBuf<256> buf;
		globalText->Extract(buf,pos,(maxLen<256)?maxLen:256);
		TInt len=0;
		const TText* ptr=buf.Ptr();
		for (;len<buf.Length();len++,ptr++)
			{
			if ((*ptr!=0x20)&&(*ptr!=CEditableText::ETabCharacter))
				break;
			}
		if (len>0)
			{
			buf.SetLength(len);
			const TInt oldLength=iText->DocumentLength();
			globalText->InsertL(curPos,buf);
			TCursorSelection sel(curPos,curPos + len);
			TextView()->HandleInsertDeleteL(sel,0);
			SetCursorPosL(curPos+len,EFalse);
			const TInt length=iText->DocumentLength();
			if (length>UpperFullFormattingLength() && oldLength<=UpperFullFormattingLength())
				SetAmountToFormatL();
			UpdateScrollBarsL();
			}
		}
	return EKeyWasConsumed;
	}

void CTextEdGlobalTextEditor::ResetWindow()
	{
	ActivateGc();
	SystemGc().Clear();
	DeactivateGc();
	}

// Fixed [1095527] Search and Replace doesn't work in TextEd (for 9200)
#if defined(__UIQ__)
TBool CTextEdGlobalTextEditor::CallBackFindL(SEdwinFindModel& /*aModel*/, TBool /*aResetStart*/)
	{
	return iFindHack;
	}

TBool CTextEdGlobalTextEditor::CallBackReplaceL(SEdwinFindModel& aModel, TBool /*aResetStart*/)
	{
	if (aModel.iReplaceOption==ENoReplace || (!iFindHack && aModel.iReplaceOption!=EReplaceAll))
		{
		if (iFindHack && aModel.iReplaceOption==ENoReplace)
			aModel.iReplaceOption=EReplaceOnce;
		}
	return iFindHack;
	}
#else // For 9200 __CRYSTAL__ and 9500 __S80_DP2_0__
TBool CTextEdGlobalTextEditor::CallBackFindL(SEdwinFindModel& aModel, TBool /*aResetStart*/)
	{
#if defined(__S80_DP2_0__)
	// Never restart from the beginning like on ER5, but use EFindStartAtCursor
	// at the first find
	if (iFindStarted)
		aModel.iFlags |= EFindStartAtCursor;
	else
		aModel.iFlags &= ~EFindStartAtCursor;
#endif
	iFindHack=CEikGlobalTextEditor::CallBackFindL(aModel,iFindStarted);

	iFindStarted=EFalse;
	return iFindHack;
	}

TBool CTextEdGlobalTextEditor::CallBackReplaceL(SEdwinFindModel& aModel, TBool /*aResetStart*/)
	{
#if defined(__S80_DP2_0__)
	if (iFindStarted)
		aModel.iFlags |= EFindStartAtCursor;
	else
		aModel.iFlags &= ~EFindStartAtCursor;

	iFindHack=CEikGlobalTextEditor::CallBackReplaceL(aModel,iFindStarted);
	iFindStarted=EFalse;
#else
	// Never restart from the beginning like on ER5 (always pass EFalse to callback). In
	// addition if we are in the replace callback but ENoReplace it set, we need to hack
	// things by setting it to EReplaceOnce, but only if something has already been found
	// (iFindHack==ETrue). If nothing has been found, do a find first and THEN change the
	// iReplaceOption for future callbacks. If we leave it set to EReplaceOnce when
	// nothing is found, the replace text is inserted immediately when the user presses
	// the Replace CBA button. On the other hand if we don't do EReplaceOnce if a valid find
	// has happened then the user has to bring up the Replace options dialog and then confirm
	// it before any replace settings take effect!

	if (aModel.iReplaceOption==ENoReplace || (!iFindHack && aModel.iReplaceOption!=EReplaceAll))
		{
		// arjenb: only do a find if nothing was found yet (otherwise it skips over the first 
		// found entry. Fixes [1103100] Find/Replace skips first find.
		if (!iFindHack)
		{
			iFindHack=CEikGlobalTextEditor::CallBackFindL(aModel,EFalse);
		}
		if (iFindHack && aModel.iReplaceOption==ENoReplace)
			{
			aModel.iReplaceOption=EReplaceOnce;
			iFindHack=CEikGlobalTextEditor::CallBackReplaceL(aModel,EFalse);
			}
		}
	else
		{
		iFindHack=CEikGlobalTextEditor::CallBackReplaceL(aModel,EFalse);
		}
#endif
	return iFindHack;
	}
#endif


//
// Class CTxtedAppUi
//
CTxtedAppUi::~CTxtedAppUi()
	{
	iCoeEnv->DeleteResourceFile(iOplrResourceFile);
	iTranslatorDll.Close();
	delete iFindFile;
#if defined(USE_PRINTING)
	delete iPageTable;
	delete iPrint;
#endif
	// Take the global text off the stack, to stop that CONE 44 panic.
	RemoveFromStack(iGlobalEd);
	delete iGlobalEd;
	delete iTranFileName;
	delete iOplRtLogon;
#ifdef USE_SENDAS
	delete iSendAppUi;
#endif
#ifdef USE_IRLISTEN
	delete iIrListenAppUi;
#endif
	}

TBool CTxtedAppUi::ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& /*aTail*/)
	{
	TBool ret=CEikAppUi::ProcessCommandParametersL(aCommand,aDocumentName);
	if (!ret) // create 
		ConeUtils::EnsurePathExistsL(aDocumentName); //!!!! Eikon should do this !!!!
	return ret;
	}

void CTxtedAppUi::HandleModelChangeL()
	{
	ResetViewL();
	}

void CTxtedAppUi::ResetViewL()
	{
	iGlobalEd->SetReadOnly(!(iDocument->AppFileMode()&EFileWrite));
	if (!(iDocument->AppFileMode()&EFileWrite))
		iEikonEnv->InfoMsg(R_TXED_FILE_IS_READ_ONLY);
#if defined(__S80_DP2_0__)
	else
		{
		// Fix bug [1103109] Replace missing from Texted editor
		// If the file is not Read Only, set the Find/Replace option, to force the Replace CBA button to appear.
		iFindModel.iReplaceOption=EReplaceOnce;
		}
#endif
	iGlobalEd->ClearUndo();
#if defined(USE_PRINTING)
	iPrintSetup=STATIC_CAST(CTextEdDocument*,iDocument)->PrintSetup();

	iPrintSetup->Header()->SetFileNameInfo(*this);
	iPrintSetup->Header()->SetNumPagesInfo(*this);
	iPrintSetup->Footer()->SetFileNameInfo(*this);
	iPrintSetup->Footer()->SetNumPagesInfo(*this); 

	iPrint->SetFirstPageOfDoc(0);
	iPrint->SetDocument((STATIC_CAST(CTextEdDocument*,iDocument)->iGlobalText));
	iPrint->SetPageList(iPageTable);
	iPrint->SetPrinterDevice(iPrintSetup->PrinterDevice());
	iPrint->SetPageMarginsInTwips(iPrintSetup->iPageMarginsInTwips.iMargins);
	iPrint->SetPageSpecInTwips(iPrintSetup->PrinterDevice()->CurrentPageSpecInTwips());
	iPaginateDocumentChanged=ETrue;
#endif

	iGlobalEd->SetDocumentContentL(*(STATIC_CAST(CTextEdDocument*,iDocument)->iGlobalText),CEikEdwin::EUseText);

	iGlobalEd->NotifyNewFormatL(); // doesn't cause the same flicker as NotifyNewDocumentL
	TInt pos=(STATIC_CAST(CTextEdDocument*,iDocument)->iIsNewDoc)?5:0;
	iGlobalEd->SetCursorPosL(pos,EFalse);

	SetTitleBarFileNameL(iEikonEnv->Process()->MainDocFileName());
	delete iTranFileName;
	iTranFileName=NULL;
	}

void CTxtedAppUi::CreateEdwinL()
	{
	CGraphicsDevice* device=iCoeEnv->ScreenDevice();
	iAppZoom.SetGraphicsDeviceMap(device);

	iGlobalEd=new(ELeave) CTextEdGlobalTextEditor();

	iGlobalEd->ConstructL(NULL,0,0,EEikEdwinAllowUndo|EEikEdwinUserSuppliedText|EEikEdwinInclusiveSizeFixed|EEikEdwinKeepDocument|EEikEdwinNoWrap|EEikEdwinLineCursor|EEikEdwinOwnsWindow|EEikEdwinAlwaysShowSelection,KTextedFontControlFlags,EGulNoSymbolFonts);
	iGlobalEd->SetObserver(this);
	iGlobalEd->SetZoomFactorL(&iAppZoom);
	iGlobalEd->CreateScrollBarFrameL();
	iGlobalEd->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff/*EAuto*/, CEikScrollBarFrame::EAuto);
	iGlobalEd->SetDocumentContentL(*(STATIC_CAST(CTextEdDocument*,iDocument)->iGlobalText),CEikEdwin::EUseText);
#if defined(USE_PRINTING)
	iPaginateDocumentChanged=ETrue;
#endif
	iGlobalEd->SetRect(ClientRect());
	iGlobalEd->ActivateL();

	iGlobalEd->SetWordDelimiters(EFalse,ETrue);
	iGlobalEd->SetCursorPosL(0,EFalse);
	iGlobalEd->SetFocus(ETrue);
	}

void CTxtedAppUi::ConstructL()
	{
	FLOGWRITE(_L("CTxtedAppUi::ConstructL() starts."));
	CEikAppUi::ConstructL();

	FLOGWRITE(_L("edwin."));
	CreateEdwinL();
	AddToStackL(iGlobalEd);

	FLOGWRITE(_L("Title bar."));
	InitTitleBar();
	InitFullScreenL();

	FLOGWRITE(_L("Translator."));
	iFindFile=new(ELeave) CFindFileByType(iEikonEnv->FsSession());
	// iFindFile is used by LoadTranslatorL()
	LoadTranslatorL();

	FLOGWRITE(_L("Prefs."));
	RestorePrefsL();
	SetTabWidthL(iTabWidthInChars);
	iGlobalEd->TextLayout()->SetNonPrintingCharsVisibility(iNonPrintingCharVisibility);
	iGlobalEd->NotifyNewFormatL();
#if defined(USE_PRINTING)
	iPrintSetup=STATIC_CAST(CTextEdDocument*,iDocument)->PrintSetup();
	iPrint=CTextPageRegionPrinter::NewL((STATIC_CAST(CTextEdDocument*,iDocument)->iGlobalText),iPrintSetup->PrinterDevice());
	iPageTable=new(ELeave) CArrayFixFlat<TInt>(5);
	iPrint->SetPageList(iPageTable);
	PrintSetupChanged(iPrintSetup);
#endif
	FLOGWRITE(_L("Runtime."));
	// Initialise file to be run
	iOplRtLogon=new(ELeave) CTxOplRtLogon(*this);
	iOplRtLogon->ConstructL();

	FLOGWRITE(_L("Resource file."));
	// Find and load the runtime resource file.
	TFileName resourceFile=RuntimeResourceFile();
	TFindFile FindResourceFile(iCoeEnv->FsSession());
	User::LeaveIfError(FindResourceFile.FindByDir(resourceFile,KNullDesC));
	resourceFile=FindResourceFile.File();
	BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(),resourceFile);
	iOplrResourceFile=iCoeEnv->AddResourceFileL(resourceFile);

#if defined(USE_SENDAS)
	iSendAppUi=CSendAppUi::NewL(ETextEdCmdSend,iEikonEnv->AppUiFactory()->MenuBar()->HotKeyTable());
#endif

#if defined(USE_IRLISTEN)
	iIrListenAppUi=CIrListenAppUi::NewL(ETextEdCmdIRReceive, iEikonEnv->AppUiFactory()->MenuBar()->HotKeyTable());
#endif

#if defined(USE_ADDTODESK)
// dp2 does not have a hotkey for add to desk.
#if !defined(__S80_DP2_0__)
	LinkUtils::AddLinkHotKeyL(*iEikonEnv->AppUiFactory()->MenuBar()->HotKeyTable(),ETextEdCmdAddToHome);
#endif
#endif
	FLOGWRITE(_L("CTxtedAppUi::ConstructL() ends."));
	}

void CTxtedAppUi::RestorePrefsL()
	{
	TInt scrollState=ETextEdCmdScrollBarVertical;
	TInt fullScreenState=EFalse;
	TInt titleBarState=ETrue;
	TInt zoomFactor=KZoomStateThree;
	TRAPD(err,DoRestorePrefsL(scrollState,fullScreenState,titleBarState,zoomFactor));
	if (err)
		{
		// failed to read so use defaults
		iIsMonoSpaced=ETrue;
		iTabWidthInChars=KDefaultTabWidthInCharacters;
		iGlobalEd->SetAutoIndent(ETrue);
		iNonPrintingCharVisibility.SetNoneVisible();
		}
	if (iExportFormatOption<EExportFormatUnicode || iExportFormatOption>=EExportFormatMaxOptions)
		iExportFormatOption=EExportFormatUnicode;

	STATIC_CAST(CTextEdDocument*,iDocument)->iGlobalText->SetGlobalCharFormat(STATIC_CAST(CTextEdApplication*,Application())->iCharFormat);
	SetFullScreenVisibilityL(fullScreenState);
	SetTitleBarVisibilityL(titleBarState);
	iAppZoom.SetZoomFactor(zoomFactor);
	CmdScrollBarL(scrollState);
	}

void CTxtedAppUi::DoRestorePrefsL(TInt& aScrollBarState,TInt& aFullScreenState,TInt& aTitleBarState,TInt& aZoomFactor)
	{
	CDictionaryStore* iniFile=Application()->OpenIniFileLC(iEikonEnv->FsSession());
//	if (!(iniFile->IsPresentL(KUidTextedPrefs)))
//		User::Leave(KErrNotFound); // read will leave if not there
	RDictionaryReadStream readStream;
	readStream.OpenLC(*iniFile,KUidTextedPrefs);
	iTabWidthInChars=readStream.ReadInt32L();
	readStream>>iNonPrintingCharVisibility;
	iIsMonoSpaced=(TBool)readStream.ReadInt8L();
	iGlobalEd->SetAutoIndent((TBool)readStream.ReadInt8L());
	aScrollBarState=readStream.ReadInt8L();
	aFullScreenState=readStream.ReadInt8L();
	aTitleBarState=readStream.ReadInt8L();
	aZoomFactor=readStream.ReadInt32L();

	CCharFormatLayer* charFormat=CCharFormatLayer::NewL(readStream);
	delete STATIC_CAST(CTextEdApplication*,Application())->iCharFormat;
	STATIC_CAST(CTextEdApplication*,Application())->iCharFormat=charFormat;

	iExportFormatOption=(TExportFormatOption)readStream.ReadInt8L();

	CleanupStack::PopAndDestroy(); // readStream
	CleanupStack::PopAndDestroy(); // iniFile
	}

TInt CTxtedAppUi::StorePrefs()
	{
	TRAPD(err,DoStorePrefsL());
	return err;
	}

void CTxtedAppUi::DoStorePrefsL()
	{
	CDictionaryStore* iniFile=Application()->OpenIniFileLC(iEikonEnv->FsSession());
	RDictionaryWriteStream writeStream;
	writeStream.AssignLC(*iniFile,KUidTextedPrefs);
	writeStream.WriteInt32L(iTabWidthInChars);
	writeStream<<iNonPrintingCharVisibility;
	writeStream.WriteInt8L(TInt8(iIsMonoSpaced));
	writeStream.WriteInt8L(TInt8(iGlobalEd->IsAutoIndenting()));
	writeStream.WriteInt8L((TInt8)(ScrollBarState()));
	writeStream.WriteInt8L((TInt8)iIsFullScreenVisible);
	writeStream.WriteInt8L((TInt8)iIsTitleBarVisible);
	writeStream.WriteInt32L(iAppZoom.ZoomFactor());
	writeStream<<*(STATIC_CAST(CTextEdApplication*,Application())->iCharFormat);
	writeStream.WriteInt8L(TInt8(iExportFormatOption));
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(); // writeStream
	iniFile->CommitL();
	CleanupStack::PopAndDestroy(); // iniFile
	}

TBool CTxtedAppUi::CheckHotKeyNotDimmedL(TInt aCommand)
	{
	const TInt documentLength=iGlobalEd->TextLength();
	if ((aCommand==EEikCmdEditCopy || aCommand==EEikCmdEditCut) && iGlobalEd->SelectionLength()==0)
		return EFalse;
	if ((aCommand==EEikCmdFileRevert || aCommand==EEikCmdFileSave) && !iDocument->HasChanged())
		return EFalse;
	if (aCommand==ETextEdCmdTranslateDebug && DebuggerNotPresent())
		return EFalse;
	if (aCommand==EEikCmdEditUndo && !iGlobalEd->CanUndo())
		return EFalse;
	if (aCommand==EEikCmdFileExport && documentLength==0)
		return EFalse;
	if ((aCommand==ETextEdCmdTranslate || aCommand==ETextEdCmdTranslateDebug) && documentLength==0)
		return EFalse;
	if (!(iDocument->AppFileMode()&EFileWrite))
		{
		switch (aCommand)
			{
		case EEikCmdFileSave:
		case EEikCmdEditCut:
		case EEikCmdEditPaste:
		case EEikCmdInsertSymbol:
		case EEikCmdSearchReplace:
		case EEikCmdFileImport:
			return EFalse;
		default:
			break;
			}
		}
	if (iOplRtLogon->IsActive()) // running
		{
		switch (aCommand)
			{
		case EEikCmdFileNew:
		case EEikCmdFileOpen:
		case EEikCmdFileSaveAs: // changes files
		case EEikCmdEditCut:
		case EEikCmdEditPaste:
		case EEikCmdInsertSymbol:
		case EEikCmdSearchReplace:
		case ETextEdCmdTranslate:
		case ETextEdCmdTranslateDebug:
		case ETextEdCmdRun:
		case EEikCmdFileImport:
		case EEikCmdFileRevert:
		case EEikCmdEditUndo:
			return EFalse;
		default:
			//return ETrue;
			break;
			}
		}
	return ETrue;
	}

void CTxtedAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EEikCmdExit:
		{
		StorePrefs();
		if (iDocument->HasChanged())
			STATIC_CAST(CTextEdDocument*,iDocument)->DoSaveToCurrentFileL();
#if !defined(__UIQ__)
		CEikProcess* process=iEikonEnv->Process();
		TRAPD(ignore,process->SetLastUsedFileL(CEikProcess::ELastOpenFile,process->MainDocFileName()));
		// ignore any error saving current file name
#endif
		Exit();
		break;
		}
	case EEikCmdFileNew:
		CmdNewFileL();
		break;
	case EEikCmdFileOpen:
		CmdOpenFileL();
		break;
#if defined(USE_PRINTING)
	case ETextEdCmdPageSetup:
		RunPageSetupDialogL();
		break;
	case EEikCmdPrintPaginate:
		RunPaginationDialogL();
		break;
	case EEikCmdPrintPrint:
	case EEikCmdPrintPreview:
		{
		TBool cancel=EFalse;
 		if (iPaginateDocumentChanged)
			cancel=RunPaginationDialogL();
		if (cancel)
			break;
		switch (aCommand)
			{
		case EEikCmdPrintPrint:
			RunPrintDialogL();
			break;
		case EEikCmdPrintPreview:
			RunPreviewDialogL();
			break;
			};
		break;
		}
#endif
	case EEikCmdFileSave:
		CmdFileSaveL();
		break;
	case EEikCmdFileSaveAs:
		CmdFileSaveAsL();
		break;
	case EEikCmdFileRevert:
		CmdFileRevertL();
		break;
	case EEikCmdFileImport:
		ImportTextL();
		break;
	case EEikCmdFileExport:
		ExportTextL();
		break;
	case ETextEdCmdDelete:
		DeleteL();
		break;
	case ETextEdCmdAddToHome:
		AddToHomeL();
		break;
	case EEikCmdEditUndo:
		iGlobalEd->UndoL();
		break;
	case EEikCmdEditCut:
		iGlobalEd->ClipboardL(CEikEdwin::ECut);
		break;
	case EEikCmdEditCopy:
		iGlobalEd->ClipboardL(CEikEdwin::ECopy);
		break;
	case EEikCmdEditPaste:
		iGlobalEd->ClipboardL(CEikEdwin::EPaste);
		break;
	case EEikCmdEditSelectAll:
		iGlobalEd->SelectAllL();
		break;
#if !defined(__UIQ__)
	case EEikCmdSearchFind:
		iGlobalEd->SetupForFind();
		CEikFindAndReplaceDialog::RunDlgLD(*iGlobalEd, iFindModel, ETrue, iGlobalEd->Rect()); // Try EFalse
		break;
#endif
	case ETextEdCmdMenuGoTo:
	case ETextEdCmdToolBarGoTo:
		CmdGotoL(aCommand);
		break;
	case EEikCmdZoomIn:
	case EEikCmdZoomOut:
		ZoomL(aCommand);
		break;
	case ETextEdCmdShowTitleBar:
		SetTitleBarVisibilityL(!iIsTitleBarVisible);
		break;
	case ETextEdCmdShowHorizScrollBar:
		SetHorizScrollBarVisibilityL(!iIsHorizScrollBarVisible);
		break;
	case ETextEdCmdFullScreen:
		SetFullScreenVisibilityL(!iIsFullScreenVisible);
		break;
	case EEikCmdFontChange:
		{
		TInt flag=EGulNoSymbolFonts;
		if (iIsMonoSpaced)
			flag=EGulMonospaceFontsOnly;
		if (iGlobalEd->RunFontDialogL(flag,NULL,KTextedFontControlFlags))
			SetTabWidthL(iTabWidthInChars);
		break;
		}
	case ETextEdCmdIndentation:
		TabSettingL();
		break;
	case ETextEdCmdSetPreferences:
		SetPreferencesL();
		break;
	case EEikCmdInsertSymbol:
		iGlobalEd->RunCharMapDialogL();
		break;
	case ETextEdCmdTranslate:
		DoTranslateL(CTextTranslator::ERelease);
		break;
	case ETextEdCmdTranslateDebug:
		DoTranslateL(CTextTranslator::EDebug);
		break;
	case ETextEdCmdLog:
		DoLogL();
		break;
	case ETextEdCmdRun:
		DoRunFileL();
		break;
	case ETextEdCmdShowLastError:
		CmdShowLastError();
		break;
	case ETextEdCmdIRReceive:
#ifdef USE_IRLISTEN
		iIrListenAppUi->ToggleListeningL();
#endif
		break;
	case ETextEdCmdScrollBarNone:
	case ETextEdCmdScrollBarVertical:
	case ETextEdCmdScrollBarHorizontal:
	case ETextEdCmdScrollBarBoth:
	case ETextEdCmdCycleScrollBars:
		CmdScrollBarL(aCommand);
		break;
	default:
		HandleSendAsMenuL(aCommand);
		break;
		}
	}

void DeleteSendAsTempFile(TAny* aFileName)
	{
	CEikonEnv::Static()->FsSession().Delete(*(STATIC_CAST(TDesC*,aFileName)));
	}

#ifdef USE_SENDAS
void CTxtedAppUi::HandleSendAsMenuL(const TInt aCommand)
	{
/**
 * Sends OPL source via the SendUi mechanism.
 *
 * If there is a highlighted selection, it is sent as text via the message body
 * otherwise, the whole file is sent as an attachment to an empty body.
 */ 
	if (iSendAppUi && iSendAppUi->CommandIsValidL(aCommand, TSendingCapabilities(KSendAsMaxBodySizeNotUsed,KSendAsMaxMessageSizeNotUsed,NULL), NULL))
		{
		TBool sendAsAttachment=EFalse;
		TCursorSelection selection=iGlobalEd->Selection();
		TSendingCapabilities capabilities=iSendAppUi->MtmCapabilitiesL(iSendAppUi->MtmForCommand(aCommand));
		const TBool supportsBody=(capabilities.iFlags & TSendingCapabilities::ESupportsBodyText);
		const TInt selectionLength=selection.Length();
		// Setup how we should try to send the message by checking if we need to
		// send it as an attachment
		if ((capabilities.iFlags & TSendingCapabilities::ESupportsAttachments) && (!supportsBody))
			sendAsAttachment=ETrue; // Can't use body text, so use attachement instead.
		if (supportsBody && !sendAsAttachment)
			{
			CParaFormatLayer* defaultPFL=CParaFormatLayer::NewL();
			CleanupStack::PushL(defaultPFL);
			CCharFormatLayer* defaultCFL=CCharFormatLayer::NewL();
			CleanupStack::PushL(defaultCFL);
			CRichText* richToSend=CRichText::NewL(defaultPFL,defaultCFL);
			CleanupStack::PushL(richToSend);
			HBufC* buffer=NULL;
			if (selectionLength)
				{
				// Send selection as body text
				buffer=HBufC::NewLC(selectionLength);
				TPtr selectionPtr=buffer->Des();
				iGlobalEd->GlobalText()->Extract(selectionPtr,selection.LowerPos(),selectionLength);
				}
			else
				{
				// Send entire file as body text
				buffer=HBufC::NewLC(iGlobalEd->GlobalText()->DocumentLength());
				TPtr bodyPtr=buffer->Des();
				iGlobalEd->GlobalText()->Extract(bodyPtr,0);
				}
			richToSend->InsertL(0,*buffer);
			iSendAppUi->CreateAndSendMessageL(aCommand,richToSend);
			CleanupStack::PopAndDestroy(4); // buffer,richToSend,defaultCFL,defaultPFL
			return;
			}
		else if (!supportsBody && !sendAsAttachment)
			User::Leave(KErrNotSupported);

		// MTM doesn't support body text, so it must support attachments.
		TFileName fileName(iEikonEnv->Process()->MainDocFileName());
		TParse path;
		CApaProcess* process=iDocument->Process();
		path.Set(process->TempFilePath(),&fileName,NULL); 
		fileName=path.FullName();
		ConeUtils::EnsurePathExistsL(fileName);

		// Create a TCleanupItem so we can delete temp file if anything below leaves. Note
		// that we add it to the stack here (BEFORE it's been created) so we can PopAndDestroy
		// right at the very end regardless. The cleanup function calls RFs::Delete which will
		// not leave - it simply returns an error if the file does not exist (which we ignore).
		CleanupStack::PushL(TCleanupItem(DeleteSendAsTempFile,&fileName));

		CFileStore* fileStore=CDirectFileStore::ReplaceLC(process->FsSession(),fileName, EFileStream|EFileWrite|EFileShareExclusive);
		fileStore->SetTypeL(REINTERPRET_CAST(CEikProcess*,process)->MainStore()->Type());
		CStreamDictionary *streamDic=CStreamDictionary::NewL();
		CleanupStack::PushL(streamDic);

		// Create buffer of text to send
		HBufC* bufferFromFile=NULL;
		if (selectionLength)
			{
			// Send selection contents only
			bufferFromFile=HBufC::NewLC(selectionLength);
			TPtr fileSelectionPtr=bufferFromFile->Des();
			iGlobalEd->GlobalText()->Extract(fileSelectionPtr,selection.LowerPos(),selectionLength);
			}
		else
			{
			// Send entire file contents
			bufferFromFile=HBufC::NewLC(iGlobalEd->GlobalText()->DocumentLength());
			TPtr fileBodyPtr=bufferFromFile->Des();
			iGlobalEd->GlobalText()->Extract(fileBodyPtr,0);
			}
		StorePortionL(*fileStore,*streamDic,*bufferFromFile);
		CleanupStack::PopAndDestroy(); // bufferFromFile
			
		// Write root stream
		process->WriteRootStreamL(*fileStore,*streamDic,*(this->Application()));
		CleanupStack::PopAndDestroy(); // streamDic
		fileStore->CommitL();
		CleanupStack::PopAndDestroy(); // fileStore

		CDesCArrayFlat* attachments=new(ELeave)CDesCArrayFlat(1);
		CleanupStack::PushL(attachments);
		attachments->AppendL(fileName);
		iSendAppUi->CreateAndSendMessageL(aCommand,NULL,attachments);
		CleanupStack::PopAndDestroy();	//attachments
		CleanupStack::PopAndDestroy(); // TCleanupItem() - ensure any temp file is deleted
		}
	}
#else
void CTxtedAppUi::HandleSendAsMenuL(const TInt /*aCommand*/)
	{
	}
#endif

void CTxtedAppUi::StorePortionL(CStreamStore& aStore,CStreamDictionary& aStreamDic,const TDesC& aPortion) const
	{
	RStoreWriteStream stream;
	TStreamId streamId=stream.CreateLC(aStore); // pushes stream
	CParaFormatLayer* paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer=CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CGlobalText* textModel=CGlobalText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(textModel);
	textModel->InsertL(0,aPortion);
	stream<<*textModel;
	stream.CommitL();
	CleanupStack::PopAndDestroy(4); // textModel,2xlayers,stream
	aStreamDic.AssignL(KUidTextEdApp,streamId);
	}
		
void CTxtedAppUi::HandleAttemptDimmedSelectionL(TInt aCommand)
	{
	TInt resourceId=0;
	if (aCommand==ETextEdCmdTranslateDebug && DebuggerNotPresent())
		return;
	const TInt documentLength=iGlobalEd->TextLength();
	const TInt selectionLength=iGlobalEd->SelectionLength();
	if (aCommand==EEikCmdEditUndo && !iGlobalEd->CanUndo())
		resourceId=R_EIK_TBUF_NOTHING_TO_UNDO;
	else if (aCommand==EEikCmdEditCut && selectionLength==0)
		resourceId=R_EIK_TBUF_NOTHING_TO_CUT;
	else if (aCommand==EEikCmdEditCopy && selectionLength==0)
		resourceId=R_EIK_TBUF_NOTHING_TO_COPY;
	if ((aCommand==EEikCmdFileSave || aCommand==EEikCmdFileRevert) && (!iDocument->HasChanged()))
		resourceId=R_TXED_FILE_UNCHANGED;
	if (aCommand==EEikCmdFileExport && documentLength==0)
		resourceId=R_TXED_NOTHING_TO_EXPORT;
	if ((aCommand==ETextEdCmdTranslate || aCommand==ETextEdCmdTranslateDebug) && documentLength==0)
		resourceId=R_TXED_NOTHING_TO_TRANSLATE;
	if (!resourceId && iOplRtLogon->IsActive())
		resourceId=R_TXED_NOT_WHILE_RUNNING;
	if (!resourceId && !(iDocument->AppFileMode()&EFileWrite))
		resourceId=R_TXED_FILE_IS_READ_ONLY;
	
	if (resourceId)
		iEikonEnv->InfoMsg(resourceId);
	else
		CEikAppUi::HandleAttemptDimmedSelectionL(aCommand);
	}

void CTxtedAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
	{
	TBool readOnly=(!(iDocument->AppFileMode()&EFileWrite));
	TBool running=iOplRtLogon->IsActive();
	TBool noSelection=iGlobalEd->SelectionLength()==0;
	TInt documentLength=iGlobalEd->TextLength();
	if (aResourceId==R_TXED_FILE_MENU)
		{
#if defined(USE_ADDTODESK)
#if defined(__S80_DP2_0__)
		// Fix bug [1103547] Texted doesn't use AddToDesk on 9500
		CEikMenuPaneItem::SData dp2AddToDesk;
		dp2AddToDesk.iCommandId=ETextEdCmdAddToHome;
		dp2AddToDesk.iCascadeId=0;
		dp2AddToDesk.iFlags=0;
		iCoeEnv->ReadResource(dp2AddToDesk.iText, R_TXED_ADD_TO_DESK);
		aMenuPane->AddMenuItemL(dp2AddToDesk);
#else
		LinkUtils::AddLinkMenuItemL(*aMenuPane,ETextEdCmdAddToHome);
#endif
#endif
#ifdef USE_SENDAS
		iSendAppUi->DisplaySendMenuItemL(*aMenuPane, aMenuPane->MenuItemIndex(ETextEdCmdPrintingCascade),
			TSendingCapabilities(KSendAsMaxBodySizeNotUsed,KSendAsMaxMessageSizeNotUsed,NULL));
#endif
		if (running)
			{
			aMenuPane->SetItemDimmed(EEikCmdFileNew,ETrue);
			aMenuPane->SetItemDimmed(EEikCmdFileOpen,ETrue);
			}
		else if (running || (!iDocument->HasChanged()))
			aMenuPane->SetItemDimmed(EEikCmdFileRevert,ETrue);
		}
	else if (aResourceId==R_TXED_SAVE_MENU)
		{
//		if (running)
//			aMenuPane->SetItemDimmed(EEikCmdFileSaveAs,ETrue);
		if (!(iDocument->AppFileMode()&EFileWrite) || (!iDocument->HasChanged()))
			aMenuPane->SetItemDimmed(EEikCmdFileSave,ETrue);
		}
	else if (aResourceId==R_TXED_FILEOPTIONS_MENU)
		{
		if (readOnly || running)
			aMenuPane->SetItemDimmed(EEikCmdFileImport,ETrue);
		if (documentLength==0)
			aMenuPane->SetItemDimmed(EEikCmdFileExport,ETrue);
		} 
#if !defined(__UIQ__)
	else if (aResourceId==R_SENDUI_MENU)
		{
#ifdef USE_SENDAS
		iSendAppUi->DisplaySendCascadeMenuL(*aMenuPane);
#endif
		}
	else if (aResourceId==R_TXED_VIEW_MENU)
		{
		if (iIsFullScreenVisible)
			aMenuPane->SetItemButtonState(ETextEdCmdFullScreen,EEikMenuItemSymbolOn);
		if (iIsTitleBarVisible)
			aMenuPane->SetItemButtonState(ETextEdCmdShowTitleBar,EEikMenuItemSymbolOn);
		}
#endif
	else if (aResourceId==R_TXED_EDIT_MENU)
		{
#if defined(__UIQ__)
		if (iIsHorizScrollBarVisible)
			aMenuPane->SetItemButtonState(ETextEdCmdShowHorizScrollBar,EEikMenuItemSymbolOn);
#endif
		if (!iGlobalEd->CanUndo() || running)
			aMenuPane->SetItemDimmed(EEikCmdEditUndo,ETrue);
		if (noSelection)
			aMenuPane->SetItemDimmed(EEikCmdEditCopy,ETrue);
		if (noSelection || readOnly || running)
			aMenuPane->SetItemDimmed(EEikCmdEditCut,ETrue);
		if (readOnly || running)
			{
			aMenuPane->SetItemDimmed(EEikCmdEditPaste,ETrue);
			}
		}
	else if (aResourceId==R_TXED_BUILD_MENU)
		{
		if (documentLength==0)
			{
			aMenuPane->SetItemDimmed(ETextEdCmdTranslate,ETrue);
			aMenuPane->SetItemDimmed(ETextEdCmdTranslateDebug,ETrue);
			}
		if (running)
			{
			aMenuPane->SetItemDimmed(ETextEdCmdTranslate,ETrue);
			aMenuPane->SetItemDimmed(ETextEdCmdTranslateDebug,ETrue);
			aMenuPane->SetItemDimmed(ETextEdCmdRun,ETrue);
			}
		if (DebuggerNotPresent())
			aMenuPane->DeleteMenuItem(ETextEdCmdTranslateDebug);
		}
#if !defined(__UIQ__)
	else if (aResourceId==R_TXED_TOOLS_MENU)
		{
#ifdef USE_IRLISTEN
		iIrListenAppUi->DisplayIrListenMenuItemL(*aMenuPane, aMenuPane->MenuItemIndex(ETextEdCmdLog)+1);
#endif
		}
#endif
	}

TInt CTxtedAppUi::ScrollBarState()
	{
	// only works because the command ids are sequential
	CEikScrollBarFrame* scrollFrame=iGlobalEd->ScrollBarFrame();
	TInt commandId=ETextEdCmdScrollBarNone;
	if (scrollFrame)
		{
		if (scrollFrame->VScrollBarVisibility()!=CEikScrollBarFrame::EOff)
			commandId++; // Vertical
		if (scrollFrame->HScrollBarVisibility()!=CEikScrollBarFrame::EOff)
			commandId+=2; // Horizontal, or Both, depending on Vertical.
		}
	return commandId;
	}

TBool CTxtedAppUi::DebuggerNotPresent()
	{
	return iFindFile->FindFirst(DebuggerDllFilename(),KNullDesC,TUidType(KDynamicLibraryUid,KUidOplDebugger,KNullUid))!=KErrNone;
	}

void CTxtedAppUi::ZoomL(TInt aZoomCmd)
	{
	TInt zoomFactor=iAppZoom.ZoomFactor();
	switch(zoomFactor)
		{
	case KZoomStateOne:
		zoomFactor=(aZoomCmd==EEikCmdZoomIn)? KZoomStateTwo : KZoomStateFour;
		break;
	case KZoomStateTwo:
		zoomFactor=(aZoomCmd==EEikCmdZoomIn)? KZoomStateThree : KZoomStateOne;
		break;
	case KZoomStateThree:
		zoomFactor=(aZoomCmd==EEikCmdZoomIn)? KZoomStateFour : KZoomStateTwo;
		break;
	case KZoomStateFour:
		zoomFactor=(aZoomCmd==EEikCmdZoomIn)? KZoomStateOne : KZoomStateThree;
		break;
	default:
		zoomFactor=KZoomStateTwo;
		}
	iAppZoom.SetZoomFactor(zoomFactor);
	iGlobalEd->NotifyNewFormatL();
	iGlobalEd->UpdateScrollBarsL();
	}

void CTxtedAppUi::InitTitleBar()
	{
#if !defined(__UIQ__)
	CEikToolBar* toolBand=iEikonEnv->AppUiFactory()->ToolBand();
	iTitleBar=STATIC_CAST(CCknAppTitle*,toolBand->ControlById(ETextEdTitleBarId));
	__ASSERT_DEBUG(iTitleBar,Panic(ETextedPanicTitleBarInvalid));
	TSize toolBandSize=toolBand->Size();
	toolBandSize.iHeight=iTitleBar->MinimumSize().iHeight;
	toolBand->SetSize(toolBandSize);
#endif
	}

void CTxtedAppUi::InitFullScreenL()
	{
#if !defined(__UIQ__) && !defined(__SERIES90__)
	CEikStatusPane* statuspane = iEikonEnv->AppUiFactory()->StatusPane();
	if (statuspane)
		{
		statuspane->SwitchLayoutL(R_INDICATOR_LAYOUT_WIDE);
		}	
#endif
	}

void CTxtedAppUi::SetTitleBarVisibilityL(const TBool aVisibility)
	{
	iIsTitleBarVisible=aVisibility;
#if !defined(__UIQ__)
	CEikToolBar* band = iEikonEnv->AppUiFactory()->ToolBand();
	band->MakeVisible(iIsTitleBarVisible);
	TRect bandRect=band->Rect();
	bandRect.iTl.iX=ClientRect().iTl.iX;
	bandRect.iBr.iX=ClientRect().iBr.iX;
	band->SetBoundingRect(bandRect);
#endif
	iGlobalEd->SetRect(ClientRect());
	iGlobalEd->DrawDeferred();
	}

void CTxtedAppUi::SetFullScreenVisibilityL(const TBool aVisibility)
	{
	iIsFullScreenVisible=aVisibility;
	if(iEikonEnv->AppUiFactory()->ToolBar())
		iEikonEnv->AppUiFactory()->ToolBar()->MakeVisible(!iIsFullScreenVisible); // cba
#if !defined(__SERIES90__)
	CEikStatusPane* statusPane=iEikonEnv->AppUiFactory()->StatusPane();
	if (statusPane)
		{
		if (iIsFullScreenVisible)
			statusPane->MakeVisible(EFalse);
		else
			statusPane->MakeVisible(ETrue);
		}
#endif
	SetTitleBarVisibilityL(iIsTitleBarVisible);
	}

void CTxtedAppUi::SetHorizScrollBarVisibilityL(const TBool aVisibility)
	{
	iIsHorizScrollBarVisible=aVisibility;
	if (iIsHorizScrollBarVisible)
		CmdScrollBarL(ETextEdCmdScrollBarBoth);
	else
		CmdScrollBarL(ETextEdCmdScrollBarVertical);
	}

void CTxtedAppUi::CmdScrollBarL(TInt aCommand)
	{
	CEikScrollBarFrame::TScrollBarVisibility vertical=CEikScrollBarFrame::EAuto;
	CEikScrollBarFrame::TScrollBarVisibility horizontal=CEikScrollBarFrame::EOff;
	CEikScrollBarFrame* scrollFrame=iGlobalEd->ScrollBarFrame();
	switch (aCommand)
		{
	case ETextEdCmdScrollBarNone:
		vertical=CEikScrollBarFrame::EOff;
		break;
	case ETextEdCmdScrollBarVertical:
		break;
	case ETextEdCmdScrollBarHorizontal:
		vertical=CEikScrollBarFrame::EOff;
		horizontal=CEikScrollBarFrame::EOn;
		break;
	case ETextEdCmdScrollBarBoth:
		horizontal=CEikScrollBarFrame::EAuto;
		break;
	case ETextEdCmdCycleScrollBars:
		// Removed. Not used since ER5?
		break;
		}
	if (!scrollFrame && (vertical!=CEikScrollBarFrame::EOff || horizontal!=CEikScrollBarFrame::EOff))
		scrollFrame=iGlobalEd->CreateScrollBarFrameL();
	if (scrollFrame)
		{
		scrollFrame->SetScrollBarVisibilityL(horizontal,vertical);
		iIsHorizScrollBarVisible=horizontal;
		iGlobalEd->UpdateScrollBarsL();
		}
	}

const TInt KVertMargin=6;

void CTxtedAppUi::CmdGotoL(TInt /*aCommand*/)
	{
	MDesCArray* array=STATIC_CAST(CTextEdDocument*,iDocument)->ProcNameArrayL();
	if (!array)
		{
		TBuf<40> resNoProcsText;
		iCoeEnv->ReadResource(resNoProcsText,R_TXED_TBUF_NO_PROCS);
		_LIT(KProc,"PROC"); // Constant text, even for non-English localizations.
		TBuf<40> msg;
		msg.Format(resNoProcsText,&KProc);
		iEikonEnv->InfoMsg(msg);
		return;
		}

	TInt selectedItem=STATIC_CAST(CTextEdDocument*,iDocument)->IndexOfPrevious(iGlobalEd->CursorPos());
	CEikDialog* dialog=new(ELeave) CTextEdProcListDialog(array,selectedItem);
	if (dialog->ExecuteLD(R_TXED_DIALOG_LIST_PROC))
		{
		// Set the cursor position to the currently selected procedure in the GotoListBox.
		iGlobalEd->SetCursorPosL(STATIC_CAST(CTextEdDocument*,iDocument)->ProcPosition(selectedItem),EFalse);
		}
	}

void CTxtedAppUi::TabSettingL()
	{
	TInt tabWidthInChars=iTabWidthInChars;
	TBool autoIndent=iGlobalEd->IsAutoIndenting();
	CEikDialog* dialog=new(ELeave) CTextEdTabDialog(&tabWidthInChars,&autoIndent);
	if (dialog->ExecuteLD(R_TXED_DIALOG_INDENTATION))
		{
		if (iTabWidthInChars!=tabWidthInChars)
			SetTabWidthL(tabWidthInChars);
		iGlobalEd->SetAutoIndent(autoIndent);
		StorePrefs();
		}
	}

void CTxtedAppUi::SetTabWidthL(TInt aTabWidthInChars)
	{
	const CCharFormatLayer* charFormatLayer=(STATIC_CAST(CTextEdDocument*,iDocument)->iGlobalText)->GlobalCharFormatLayer();
	TCharFormat charFormat;
	charFormatLayer->SenseEffective(charFormat);

	TInt height=charFormat.iFontSpec.iHeight;
	TInt charWidth=height*3/5; // a default value
	CFont *font;
	CWsScreenDevice* device=iEikonEnv->ScreenDevice(); // only worried about the screen
	if (device->GetNearestFontInTwips(font,charFormat.iFontSpec)==KErrNone)
		{
		TFontSpec spec=font->FontSpecInTwips();
		TInt width=font->MaxNormalCharWidthInPixels();
		if (!width)
			width=font->MaxCharWidthInPixels();
		charWidth=width*spec.iHeight/font->HeightInPixels();
		device->ReleaseFont(font);
		}
	
	TUint32 tabStop=aTabWidthInChars*charWidth;
	CParaFormat* paraFormat=CParaFormat::NewL();
	CleanupStack::PushL(paraFormat);
	
	CParaFormatLayer* paraFormatLayer=CONST_CAST(CParaFormatLayer*,(STATIC_CAST(CTextEdDocument*,iDocument)->iGlobalText)->GlobalParaFormatLayer());

	paraFormatLayer->SenseEffectiveL(paraFormat);
	if(tabStop!=paraFormat->iDefaultTabWidthInTwips)
		{
		TParaFormatMask attribChangedMask;
		attribChangedMask.SetAttrib(EAttDefaultTabWidth);
		paraFormat->iDefaultTabWidthInTwips=tabStop;
		paraFormatLayer->SetL(paraFormat,attribChangedMask);
		iGlobalEd->ApplyParaFormatL(paraFormat,attribChangedMask);
		}
	iTabWidthInChars=aTabWidthInChars;
	CleanupStack::PopAndDestroy();
	}

void CTxtedAppUi::SetPreferencesL()
	{
	CEikDialog* dialog=new(ELeave) CTextEdSetPreferencesDialog(&iIsMonoSpaced,&iNonPrintingCharVisibility,iExportFormatOption);
	if (dialog->ExecuteLD(R_TXED_DIALOG_SETPREFERENCES))
		{
		iGlobalEd->TextLayout()->SetNonPrintingCharsVisibility(iNonPrintingCharVisibility);
		iGlobalEd->NotifyNewFormatL();
		StorePrefs();
		}
	}

#if defined(USE_PRINTING)
void CTxtedAppUi::RunPageSetupDialogL()
	{
	if (CEikPageSetupDialog::RunDlgLD(iPrintSetup))
		PrintSetupChanged(iPrintSetup);
	}

TBool CTxtedAppUi::RunPaginationDialogL(CPrintSetup& aPrintSetup, TCknPrintParameters& aPrintParameters)
	{
	TBool paginationCancelled = EFalse;
	if (iPaginateDocumentChanged)
		{
		paginationCancelled = iGlobalEd->RunPaginateDialogL(&aPrintSetup,iPageTable);
		if (!paginationCancelled)
			{
			aPrintParameters.iParams.iFirstPage=0;
			aPrintParameters.iParams.iLastPage=iPageTable->Count()-1;
			iGlobalEd->GlobalText()->SetPageTable(iPageTable);
			iPaginateDocumentChanged=EFalse;
			}
		}
	return paginationCancelled;
	}

TBool CTxtedAppUi::RunPaginationDialogL()
	{
	return RunPaginationDialogL(*iPrintSetup,iPrintParameters);
	}

TBool CTxtedAppUi::RunPrintPreviewDialogL(CPrintSetup& aPrintSetup, MPageRegionPrinter& aPrinter)
	{
	TInt pageCount=iPageTable->Count();
	return CEikPrintPreviewDialog::RunDlgLD(aPrintSetup,
												aPrinter,
												pageCount,
												this,
												KPrintPreviewBandsPerPage);
	}

void CTxtedAppUi::RunPreviewDialogL()
	{
	iPrint->SetPrintPreview(ETrue);
	if (RunPrintPreviewDialogL(*iPrintSetup,*iPrint))
		RunPrintDialogL();
	iGlobalEd->DrawDeferred();
	}

TBool CTxtedAppUi::PageSetupChangedL(CPrintSetup* aPrintSetup,TInt& aNumPagesInDoc)
	// Return ETrue if pagination is cancelled, otherwise return EFalse.
	{
	iPrint->SetPageMarginsInTwips(aPrintSetup->iPageMarginsInTwips.iMargins);
	iPrint->SetPageSpecInTwips(aPrintSetup->PrinterDevice()->CurrentPageSpecInTwips());
	iPaginateDocumentChanged=ETrue; // need to paginate if page spec changed
	if (RunPaginationDialogL()) // returns ETrue if pagination cancelled
		return ETrue;
	aNumPagesInDoc=iPageTable->Count();
	return EFalse;
	}

TBool CTxtedAppUi::RunPrintRangeDialogL(CPrintSetup* aPrintSetup,TInt& aNumPagesInDoc)
	// Return ETrue if pagination is cancelled, otherwise return EFalse.
	{
	aPrintSetup->CreatePrinterDeviceL(aPrintSetup->PrinterDevice()->Model().iUid,iCoeEnv->FsSession());
	return PageSetupChangedL(aPrintSetup,aNumPagesInDoc);
	}

void CTxtedAppUi::RunPrintDialogL()
	{
	TUid oldUid = iPrintSetup->PrinterDevice()->Model().iUid;
	TUid newUid = oldUid;
	TBool printRequested = CCknPrintDialog::RunDlgLD(*iPrint,this,iPrintParameters,*iPrintSetup,newUid);
	if (newUid != oldUid)
		{
		iPrintSetup->CreatePrinterDeviceL(newUid, iEikonEnv->FsSession());
		iPrint->SetPrinterDevice(iPrintSetup->PrinterDevice());
		}
	if (printRequested)
		DoPrintL();
	}

void CTxtedAppUi::DoPrintL()
	// Run the print progress dialog
	{
	iPrint->SetPrintPreview(EFalse);
	CEikPrintProgressDialog::RunDlgLD(iPrintSetup,iPrint,iPrintParameters.iParams);
	iPrintSetup->FreeModelList();
	}

void CTxtedAppUi::PrintSetupChanged(CPrintSetup* aPrintSetup)
	{
	iPrint->SetPrinterDevice(aPrintSetup->PrinterDevice());
	iPrint->SetPageMarginsInTwips(aPrintSetup->iPageMarginsInTwips.iMargins);
	iPrint->SetPageSpecInTwips(aPrintSetup->PrinterDevice()->CurrentPageSpecInTwips());
	STATIC_CAST(CTextEdDocument*,iDocument)->SetDocChanged(ETrue);
	iPaginateDocumentChanged=ETrue; // need to paginate if page spec changed
	}
#endif

TInt CTxtedAppUi::UpdateFieldFileName(TPtr& aValueText) const
	{
	TPtrC fileName=iEikonEnv->Process()->MainDocFileName();
	if (fileName.Length()>aValueText.MaxLength())
		aValueText=fileName.Left(aValueText.MaxLength());
	else
		aValueText=fileName;
	return KErrNone;
	}

TInt CTxtedAppUi::UpdateFieldNumPages() const
	{
#if defined(USE_PRINTING)
	return iPageTable->Count();
#else
	return 0;
#endif
	}

void CTxtedAppUi::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
	{
	if (aControl==iGlobalEd&&aEventType==EEventStateChanged)
		{
#if defined(USE_PRINTING)
		iPaginateDocumentChanged=ETrue;
#endif
		STATIC_CAST(CTextEdDocument*,iDocument)->SetDocChanged(ETrue);
		}
	}

//////////////////
// File dialogs //
//////////////////
void CTxtedAppUi::CmdOpenFileL()
	{
	TFileName fileName=iEikonEnv->Process()->MainDocFolder();
	TParsePtrC parser(fileName);
	fileName = parser.DriveAndPath();
#if defined(__UIQ__)
	CEikFileOpenDialog* fileDialog=new (ELeave)CEikFileOpenDialog(&fileName);
	if(fileDialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
#else
	if (CCknOpenFileDialog::RunDlgLD(fileName,KUidTextEdApp,CCknFileListDialogBase::EShowAllDrives,NULL))
#endif
		OpenFileL(fileName);
	}

void CTxtedAppUi::OpenFileL(const TDesC& aFileName)
	{
	((CTextEdDocument*)iDocument)->DoOpenFileL(aFileName); // Saves current file if changed
	iEikonEnv->UpdateTaskNameL();
	ResetViewL();
	}

void CTxtedAppUi::CmdNewFileL()
	{ 
	TFileName fileName;
	TFileName defaultFileName;
	iEikonEnv->ReadResource(defaultFileName,R_TXED_DEFAULT_FILE_NAME);
	CEikProcess* process=iEikonEnv->Process();
	TFileName tempFileName=process->MainDocFileName(); 
	TParsePtrC parser(tempFileName);
	fileName = parser.DriveAndPath();
	TRAPD(ret,ConeUtils::EnsurePathExistsL(fileName));
	if (ret!=KErrNone)
		{ // If the path is e.g. read-only default to C:
		fileName=KOplDefaultFilePath;
		ConeUtils::EnsurePathExistsL(fileName);
		}
	fileName.Append(defaultFileName);
	User::LeaveIfError(CEikApplication::GenerateFileName(iEikonEnv->Process()->FsSession(),fileName));
#if defined(__UIQ__)
	CEikFileSaveAsDialog* saveAsDlg = new (ELeave) CEikFileSaveAsDialog(&fileName);
	if(saveAsDlg->ExecuteLD(R_EIK_DIALOG_FILE_SAVEAS))
#else
	if (CCknSaveAsFileDialog::RunDlgLD(fileName,R_EIK_TBUF_CREATE_NEW_FILE,CCknFileListDialogBase::EShowAllDrives,KUidTextEdApp))
#endif
		CreateFileL(fileName);
	}

void CTxtedAppUi::CreateFileL(const TDesC& aFileName)
	{
	ConeUtils::EnsurePathExistsL(aFileName);
	((CTextEdDocument*)iDocument)->DoNewFileL(aFileName); // Saves current file if changed
	iEikonEnv->UpdateTaskNameL();
	ResetViewL();
	}

void CTxtedAppUi::CmdFileSaveL()
	{
	((CTextEdDocument*)iDocument)->SaveL();
	iEikonEnv->InfoMsg(R_EIK_TBUF_SAVED);
	}

void CTxtedAppUi::CmdFileSaveAsL()
	{
	CEikProcess* process=iEikonEnv->Process();
	TFileName fileName=process->MainDocFileName(); 
#if defined(__UIQ__)
	CEikFileSaveAsDialog* saveAsDlg = new (ELeave) CEikFileSaveAsDialog(&fileName);
	if(saveAsDlg->ExecuteLD(R_EIK_DIALOG_FILE_SAVEAS))
#else
	if (CCknSaveAsFileDialog::RunDlgLD(fileName,CCknFileListDialogBase::EShowAllDrives,KUidTextEdApp))
#endif
		{
		ConeUtils::EnsurePathExistsL(fileName);
		if (fileName.CompareF(process->MainDocFileName())==0)
			{
			CmdFileSaveL();
			iEikonEnv->InfoMsg(R_EIK_TBUF_SAVED);
			return;
			}
		((CTextEdDocument*)iDocument)->DoSaveToNewFileL(fileName);
		iEikonEnv->UpdateTaskNameL();
		iGlobalEd->SetReadOnly(EFalse); // Must be able to write as we've just saved it
		SetTitleBarFileNameL(fileName);
		iEikonEnv->InfoMsg(R_EIK_TBUF_SAVED);
		}
	}

void CTxtedAppUi::CmdFileRevertL()
	{
	if (!iDocument->HasChanged())
		{
		iEikonEnv->InfoMsg(R_TXED_FILE_UNCHANGED);
		return;
		}
#if defined(__UIQ__)
	if(iEikonEnv->QueryWinL(R_TXED_REVERT_TO_SAVED_TITLE,R_TXED_REVERT_TO_SAVED_TEXT))
#else
	if (CCknConfirmationDialog::RunDlgLD(R_TXED_REVERT_TO_SAVED_TITLE,
										 R_TXED_REVERT_TO_SAVED_TEXT,
										 NULL,
										 R_TXED_BTEXT_REVERT_TO_SAVED))
#endif
		{
		CPrintSetup* pPS=iEikonEnv->NewDefaultPrintSetupL();
		CleanupStack::PushL(pPS);
		CTextEdApplication* app=STATIC_CAST(CTextEdApplication*,iDocument->Application());
		CGlobalText* pGT=CGlobalText::NewL(app->iParaFormat,app->iCharFormat);
		CleanupStack::Pop();
		STATIC_CAST(CTextEdDocument*,iDocument)->DoFileRevertL(pGT,pPS);
		iGlobalEd->ResetWindow();
		TRAPD(err,ResetViewL());
		if (err)
			iEikonEnv->HandleErrorWithContext(err,R_EIK_TBUF_ERROR_WHILE_REDRAWING);
		}
	}

TFileName CTxtedAppUi::CurrentFilePath() const
	{
	TParsePtrC parser(iEikonEnv->Process()->MainDocFileName());
	return parser.DriveAndPath();
	}

void CTxtedAppUi::ImportTextL()
	{ // Import a text file.
	TFileName fileName=CurrentFilePath();
	TInt err=KErrNone;
	// Call the import dialog. There's a nasty hack going on here too - use
	// KUidOplTranslatorProvider to prevent the import dialog also
	// listing TextEd files for import as it does if you just use KNullUid
#if defined(__UIQ__)
	CEikFileOpenDialog* fileDialog=new (ELeave)CEikFileOpenDialog(&fileName);
	if(fileDialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
#else
	if (CCknOpenFileDialog::RunImportDlgLD(fileName,R_TXED_IMPORT_DIALOG_SHOW_TEXT,CCknFileListDialogBase::EShowAllDrives,KUidOplTranslatorProvider,&KTextExtension))
#endif
		{
		RFs fs=iEikonEnv->FsSession();
		// First check this is not a standard Symbian file with UIDs. If it is, report
		// the error and return
		CFileStore* store=NULL;
		TRAP(err,store=CDirectFileStore::OpenL(fs,fileName,EFileStream|EFileRead|EFileShareReadersOnly));
		if (err==KErrNone) // If it was a direct store we tried to open
			{
			CleanupStack::PushL(store);
			const TUidType fileUidType=store->Type();
			if (fileUidType[0]==KDirectFileStoreLayoutUid || fileUidType[1]==KUidAppDllDoc)
				err=KErrNotSupported;
			CleanupStack::PopAndDestroy(); // store
			}
		// Show an error and abort for KErrNotSupported or KErrInUse errors. For all other
		// errors try to import as text. For example KErrEof is sometimes raised for
		// empty/small files (e.g. single EOLN test) and KErrCorrupt can be raised for much
		// larger files.
		if (err==KErrInUse)
			{
			iEikonEnv->InfoMsg(R_TXED_FILE_IN_USE);
			return;
			}
		else if (err==KErrNotSupported)
			{
			iEikonEnv->InfoMsg(R_TXED_CANNOT_IMPORT_FILE);
			return;
			}
		// Next, see if the file is Unicode or ASCII text. If it's Unicode it *probably* has
		// 0xFFFE or 0xFEFF at the start. So open the file and check for this - if it's found
		// then we can use the normal CEikEdwin import method. Otherwise we have to do a
		// 'messy' import from ASCII.
		RFile file;
		User::LeaveIfError(file.Open(fs, fileName, EFileStreamText | EFileShareAny));
		CleanupClosePushL(file);
		TInt size=0;
		User::LeaveIfError(file.Size(size));
		if (size==0)
			{
			iEikonEnv->InfoMsg(R_TXED_NOTHING_TO_IMPORT);
			CleanupStack::PopAndDestroy(); // file (calls Close() automatically)
			return;
			}
		// Read the first two bytes
		HBufC8* tempBuf=HBufC8::NewLC(2);
		TPtr8 ptr(tempBuf->Des());
		User::LeaveIfError(file.Read(ptr,2));
		if ((ptr[0]==0xFF && ptr[1]==0xFE) || (ptr[0]==0xFE && ptr[1]==0xFF)) // Unicode
			{
			CleanupStack::PopAndDestroy(2); // tempBuf,file (calls Close() automatically)
			iGlobalEd->InsertFromTextFileL(fileName,CPlainText::EOrganiseByParagraph); 
			}
		else // Assume ASCII
			{
			CleanupStack::PopAndDestroy(); // tempBuf
			TInt seekPos=0;
			User::LeaveIfError(file.Seek(ESeekStart,seekPos)); // reset file reading to beginning
			iEikonEnv->BusyMsgL(R_TXED_IMPORTING,500000); // after 0.5 seconds
			TRAP(err,DoAsciiImportL(file,size));
			CleanupStack::PopAndDestroy(); // file (calls Close() automatically)
			iEikonEnv->BusyMsgCancel();
			User::LeaveIfError(err);
			}
		}
	}

void CTxtedAppUi::DoAsciiImportL(const RFile& aFile, const TInt aSize)
	{
	iGlobalEd->CancelFepTransaction();
	const TInt oldLength=iGlobalEd->TextLength();
	const TInt oldCursorPos=iGlobalEd->CursorPos();

	HBufC8* narrowBuf=HBufC8::NewLC(aSize);
	TPtr8 ptr(narrowBuf->Des());
	User::LeaveIfError(aFile.Read(ptr));
	HBufC* wideBuf=COplRuntimeCharConv::ConvertFromNarrowToUnicodeLC(*narrowBuf);

	// Now replace ASCII EOL characters with the Unicode one. We need to replace
	// KCarriageReturnLineFeed *before* KLineFeed since KCarriageReturnLineFeed
	// also contains a KLineFeed character. This allows us to cope with both DOS
	// and Unix format text files
	TPtr ptrW(wideBuf->Des());
	TBuf<1> unicodeLineDelimeter;
	unicodeLineDelimeter.Append(CEditableText::EParagraphDelimiter); // Works better than ELineBreak
	ReplaceWithLineDelimiter(ptrW, KCarriageReturnLineFeed, unicodeLineDelimeter);
	ReplaceWithLineDelimiter(ptrW, KLineFeed, unicodeLineDelimeter);

	// With all the conversions done, insert the text and then mirror the behaviour of
	// InsertFromTextFileL()'s so that the user sees consistent display results with
	// a Unicode import
	iGlobalEd->Text()->InsertL(oldCursorPos,*wideBuf);
	const TInt newLength=iGlobalEd->TextLength();
	const TInt newCursorPos=oldCursorPos+newLength-oldLength;
	CTextView* textView=iGlobalEd->TextView();
	textView->SetPendingSelection(TCursorSelection(newCursorPos,newCursorPos));
	if (newLength>KFullFormattingUpperThreshold &&
		oldLength<=KFullFormattingUpperThreshold)
		iGlobalEd->SetAmountToFormatL();
	else
		textView->HandleInsertDeleteL(TCursorSelection(newCursorPos,oldCursorPos),0,ETrue);
	iGlobalEd->DrawContents();
	iGlobalEd->UpdateScrollBarsL();
	iGlobalEd->ReportEventL(MCoeControlObserver::EEventStateChanged);

	CleanupStack::PopAndDestroy(2); // wideBuf,narrowBuf
	}

void CTxtedAppUi::ReplaceWithLineDelimiter(TDes& aBuffer, const TDesC& aTextToReplace, const TDesC& aReplaceWith) const
	{
	TInt location=KErrNotFound;
	while ( (location = aBuffer.Find(aTextToReplace)) != KErrNotFound ) 
		aBuffer.Replace(location, aTextToReplace.Length(), aReplaceWith);
	}

void CTxtedAppUi::ExportTextL()
	{
	TFileName fileName=CurrentFilePath();
	fileName.Append(KTextExtension);
#if defined(__UIQ__)
	CEikFileSaveAsDialog* saveAsDlg = new (ELeave) CEikFileSaveAsDialog(&fileName);
	if(saveAsDlg->ExecuteLD(R_EIK_DIALOG_FILE_SAVEAS))
#else
	if (CCknSaveAsFileDialog::RunDlgLD(fileName,R_TXED_EXPORT_DIALOG_TITLE,CCknFileListDialogBase::EShowAllDrives,KNullUid))
#endif
		{
		if (iExportFormatOption==EExportFormatUnixASCII || iExportFormatOption==EExportFormatDosASCII)
			{
			iEikonEnv->BusyMsgL(R_TXED_EXPORTING,500000); // after 0.5 seconds
			TRAPD(err,DoAsciiExportL(fileName));
			iEikonEnv->BusyMsgCancel();
			User::LeaveIfError(err);
			}
		else
			iGlobalEd->Text()->ExportAsTextL(fileName,CPlainText::EOrganiseByParagraph,KExportLineWrap);
		iEikonEnv->InfoMsg(R_EIK_TBUF_SAVED);
		}
	}

void CTxtedAppUi::DoAsciiExportL(const TFileName& aFileName)
	{
	HBufC* wideBuf=iGlobalEd->GetTextInHBufL();
	CleanupStack::PushL(wideBuf);
	// If we need to export as MS-DOS ASCII, parse the text for CEditableText::EParagraphDelimiter
	// and total the occurrences so we can increase the buffer size, allowing us to insert
	// the 2-character ASCII line delimeter. Once we have the new buffer size, re-allocate
	// it and then re-parse, replacing any Unicode line delimeters
	// (CEditableText::EParagraphDelimiter) with KCarriageReturnLineFeed.
	if (iExportFormatOption==EExportFormatDosASCII)
		{
		TPtr widePtr(wideBuf->Des());
		const TInt oldSize=widePtr.Length();
		TInt bufferSize=oldSize;
		TBuf<1> unicodeLineDelimeter;
		unicodeLineDelimeter.Append(CEditableText::EParagraphDelimiter);
		TInt location=0;
		TPtrC substring(widePtr);
		while ( (location = substring.Find(unicodeLineDelimeter)) != KErrNotFound ) 
			{
			substring.Set(widePtr.Mid(location+1,substring.Length()-location-1));
			bufferSize++;
			}
		// Only re-allocate if the new buffer size is actually different
		if (bufferSize!=oldSize)
			{
			wideBuf=wideBuf->ReAllocL(bufferSize);
			CleanupStack::Pop(); // wideBuf - in case we realloacted to a different start cell
			CleanupStack::PushL(wideBuf);
			}

		widePtr.Set(wideBuf->Des());
		ReplaceWithLineDelimiter(widePtr, unicodeLineDelimeter, KCarriageReturnLineFeed);
		}
	//
	// Now do the Unicode to Narrow conversion and output to file
	//
	ConeUtils::EnsurePathExistsL(aFileName);
	HBufC8* narrowBuf=COplRuntimeCharConv::ConvertFromUnicodeToNarrowLC(*wideBuf);
	RFile file;
	User::LeaveIfError(file.Replace(iEikonEnv->FsSession(), aFileName, EFileStreamText | EFileWrite));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Write(*narrowBuf));
	User::LeaveIfError(file.Flush());
	CleanupStack::PopAndDestroy(3); // file (calls Close() for us), narrowBuf,wideBuf
	}

void CTxtedAppUi::DeleteL()
	{
	TBuf<KTextEdResourceLen> title;
	TBuf<KTextEdResourceLen> buttonText;
	iCoeEnv->ReadResource(title,R_TXED_DELETE_CURRENT_FILE);
	iCoeEnv->ReadResource(buttonText,R_TXED_BTEXT_DELETE_FILE);
#if defined(__UIQ__)
	if(!iEikonEnv->QueryWinL(title,KNullDesC))
#else
	if (!CCknConfirmationDialog::RunDlgLD(title,KNullDesC,NULL,&buttonText))
#endif
		return;
	delete iEikonEnv->Process()->MainStore();
	iEikonEnv->Process()->SetMainStore(NULL);
	User::LeaveIfError(CEikonEnv::Static()->FsSession().Delete(iEikonEnv->Process()->MainDocFileName()));
	Exit();
	}

void CTxtedAppUi::AddToHomeL()
	{
#if defined(USE_ADDTODESK)
#if defined(__S80_DP2_0__)
	AddToDesk::AddDocToDeskL(CEikonEnv::Static()->FsSession(),iEikonEnv->Process()->MainDocFileName());
#else
	CLinkDocument* link=CLinkDocument::NewL();
	CleanupStack::PushL(link);
	link->SetDocumentL(iEikonEnv->Process()->MainDocFileName());
	LinkUtils::CreateLinkDocumentL(*link);
	CleanupStack::PopAndDestroy(); // link
#endif
#endif
	}

void CTxtedAppUi::DoLogL()
	{
	_LIT(KLogViewerAppFileName, "\\System\\Apps\\Logview\\Logview.app");
	TFileName dll;
	Dll::FileName(dll);
	TParse parse;
	parse.Set(KLogViewerAppFileName,&dll,NULL);
	CApaCommandLine* cmdLine=CApaCommandLine::NewLC();
	cmdLine->SetCommandL(EApaCommandRun);
	cmdLine->SetLibraryNameL(parse.FullName());
	EikDll::StartAppL(*cmdLine);
	CleanupStack::PopAndDestroy(cmdLine);
	}

void CTxtedAppUi::SetTitleBarFileNameL(const TDesC& aFileName)
	{
	TFileName fileName(aFileName);
	if (fileName.Length()==0)
		fileName=iEikonEnv->EikAppUi()->Application()->AppCaption();
	User::LeaveIfError(EikFileUtils::Parse(fileName));
	TParsePtrC parse(aFileName);
	if (parse.NamePresent())
		fileName=parse.NameAndExt();
	else
		fileName=TPtrC();
#if !defined(__UIQ__)
	iTitleBar->SetTextL(fileName, CCknAppTitle::EMainTitle);
	iTitleBar->DrawNow();
#endif
	}

//
/////////////////////OPL stuff//////////////////////////////
//
TInt CTxtedAppUi::OpenSource(TDes& aFileName,MTextSource*& aTextSource)
	{
	if (aFileName==iDocument->Process()->MainDocFileName()) // currently open file
		{
		aTextSource=new CTextSource(STATIC_CAST(CTextEdDocument*,iDocument),NULL);
		return (aTextSource)?KErrNone:KErrNoMemory;
		}
	TRAPD(err,DoOpenSourceL(aFileName,aTextSource));
	return err;
	}

void CTxtedAppUi::DoOpenSourceL(TDes& aFileName,MTextSource*& aTextSource)
	{
	RFs& fsSession=iEikonEnv->FsSession();
	TParse parse;
	TFileName mainDoc=iDocument->Process()->MainDocFileName();
	User::LeaveIfError(parse.Set(aFileName,&mainDoc,NULL));
	if (!parse.NameOrExtPresent())
		User::Leave(KErrBadName);
	TEntry entry;
	if (fsSession.Entry(parse.FullName(),entry))
		{
		parse.Set(aFileName,NULL,NULL);
		TFindFile find(fsSession);
		if (!parse.DrivePresent() && !parse.PathPresent() && find.FindByDir(aFileName,KDefaultOplIncludePath)==KErrNone)
			aFileName=find.File();
		else
			User::Leave(KErrNotFound);
		}
	else
		aFileName=parse.FullName();

	CApaProcess* process=iDocument->Process();
	CFileStore* store;
	CStreamDictionary* dict;
	CApaDocument* doc=process->OpenNewDocumentL(store,dict,aFileName,EFileShareReadersOnly|EFileRead);
	delete dict;
	delete store; // close the store
	TAny* cell=User::Alloc(sizeof(CTextSource));
	if (cell)
		aTextSource=new(cell) CTextSource(STATIC_CAST(CTextEdDocument*,doc),process);
	else
		{
		process->DestroyDocument(doc);
		User::LeaveNoMemory();
		}
	}

void CTxtedAppUi::LoadTranslatorL()
	{
	User::LeaveIfError(iTranslatorDll.Load(KDefaultTranslatorName));
	if ((iTranslatorDll.Type())[1]!=KUidOplTranslatorProvider)
		User::Leave(KErrNotSupported);
	iNewTranFuncL=(TNewOplTranslatorL)iTranslatorDll.Lookup(1);
	if (iNewTranFuncL==NULL)
		User::Leave(KErrBadLibraryEntryPoint);
	}

COplTranslatorBase* CTxtedAppUi::NewTranslatorLC()
	{
	COplTranslatorBase* translator=(*iNewTranFuncL)();
	CleanupStack::PushL(translator);
	translator->SetTarget(EOplTargetER1);
	translator->SetAppCB(this);
	return translator;
	}

void CTxtedAppUi::DoTranslateL(CTextTranslator::TDebugFlag aDebugFlag)
	{
	RFs& fsSession=iEikonEnv->FsSession();
	TPtrC fileName=iDocument->Process()->MainDocFileName();
	fsSession.SetSessionPath(fileName);
	TFileName output(fileName);
	TInt len=output.Length();
	if (len>4 && output.Right(4).CompareF(KOplExtension)==KErrNone)
		output.SetLength(len-4); // remove .OPL extension if present
	else if (len+4>output.MaxLength())
		User::Leave(KErrBadName);
	output.Append(KOpoExtension);
	TSourceTranslateError anErr;
	COplTranslatorBase* translator=NewTranslatorLC();
	translator->StartTranslateL(*this,fileName,output,aDebugFlag,anErr);
	// Save default output name
	// This name is changed elsewhere if the program is an app
	delete iTranFileName;
	iTranFileName=NULL;
	iTranFileName=output.AllocL();

	TInt r=KErrCancel; // Need to initialise r to avoid compiler warning

	CEikDialog* dialog=new(ELeave) CTextEdTranslateDialog(translator,r);
	dialog->ExecuteLD(R_TXED_DIALOG_TRANSLATE);
	CleanupStack::PopAndDestroy(); // the translator

	if (r!=KErrCancel)
		{
		if (r!=KErrGeneral || anErr.Error()!=EErrSuccess)
			{
			GetTranslateErrorText(iLastErrorBuf,anErr.Error());
			if (anErr.Source().CompareF(fileName)==0 || OpenIncludeFileWithErrorL(anErr.Source()))
				{
				TInt pos=anErr.Position();
				if (pos<=iGlobalEd->TextLength())
					{
					TRAPD(ignore,iGlobalEd->SetCursorPosL(pos,EFalse));
					}
				}
			iEikonEnv->InfoMsg(iLastErrorBuf);
			}
		else
			{
#if defined(__UIQ__)
			if(iEikonEnv->QueryWinL(R_TXED_TRANSLATE_COMPLETE,R_TXED_TRANS_RUN_QUESTION))
#else
			if (CCknConfirmationDialog::RunDlgLD(R_TXED_TRANSLATE_COMPLETE,
												 R_TXED_TRANS_RUN_QUESTION,
												 NULL,
												 R_TXED_BTEXT_RUN_TRANSLATED))
#endif
				DoRunL(*iTranFileName);
			}
		}
	}

TBool CTxtedAppUi::OpenIncludeFileWithErrorL(const TDesC& aFilename)
	{
	TBuf<KTextEdResourceLen> errInInclude;
	TBuf<KTextEdResourceLen> resErrorText;
	TBuf<KTextEdResourceLen> resOpenText;
	iCoeEnv->ReadResource(resErrorText,R_TXED_ERROR_IN_INCLUDE_FILE);
	iCoeEnv->ReadResource(resOpenText,R_TXED_OPEN_THE_FILE);
	_LIT(KInclude,"INCLUDE"); // Constant text, even for non-English localizations.
	errInInclude.Format(resErrorText,&KInclude);
	TBuf<KTextEdResourceLen> buttonText;
	iCoeEnv->ReadResource(buttonText, R_TXED_BTEXT_INCLUDE_OPEN_FILE);
#if defined(__UIQ__)
	if(iEikonEnv->QueryWinL(errInInclude,resOpenText))
#else
	if (CCknConfirmationDialog::RunDlgLD(errInInclude,resOpenText,NULL,&buttonText))
#endif
		{
		OpenFileL(aFilename);
		return ETrue;
		}
	return EFalse;
	}

void CTxtedAppUi::DoLocateRuntimeErrorL(CTextTranslator::TDebugFlag aDebugFlag,
										TSourceTranslateError& anErr,
										TInt aProcLine)
	{
	TInt qCodeOffset=anErr.Position();
	RFs& fsSession=iEikonEnv->FsSession();
	TPtrC fileName=iDocument->Process()->MainDocFileName();
	fsSession.SetSessionPath(fileName);
	TFileName output;
	RFile tempFile;
#if defined(__S80_DP2_0__) || defined(__SERIES90__)
	User::LeaveIfError(tempFile.Temp(fsSession,iDocument->Process()->TempFilePath(),output,EFileWrite));
#else
	User::LeaveIfError(tempFile.Temp(fsSession,Apfile::TempPath(),output,EFileWrite));
#endif
	tempFile.Close();
	COplTranslatorBase* translator=NewTranslatorLC();
	translator->LocateErrorL(*this,fileName,output,aDebugFlag,anErr,aProcLine);
	TInt ret=KErrCancel;
	CEikDialog* dialog=new(ELeave) CTextEdTranslateDialog(translator,ret,R_TXED_LOCATE_ERROR_DIALOG_TITLE);
	dialog->ExecuteLD(R_TXED_DIALOG_TRANSLATE);
	CleanupStack::PopAndDestroy(); // the translator

	if (ret!=KErrCancel)
		{
		if (ret==KErrGeneral && anErr.Error()==EErrFound)
			{
			TInt pos=anErr.Position();
			if (pos<=iGlobalEd->TextLength())
				{
				if (qCodeOffset<0)
					{
					TUint scanMask=CPlainText::EScanBackwards|CPlainText::EScanStayIfBoundary|CPlainText::EScanToUnitStart;
					iGlobalEd->GlobalText()->ScanParas(pos,scanMask); // guaranteed to find para start
					}
				TRAPD(ignore,iGlobalEd->SetCursorPosL(pos,EFalse));
				}
			}
		// else
		// error not found - Info message?
		}
	fsSession.Delete(output); // ignore any error
	}

void CTxtedAppUi::OplTargetIsAppL(TFileName& aName)
	{
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(KDefaultAppPath,&aName,NULL));
	User::LeaveIfError(parse.AddDir(parse.Name()));
	aName=parse.FullName();
	// save the output name
	delete iTranFileName;
	iTranFileName=NULL;
	iTranFileName=aName.AllocL();
	}

void CTxtedAppUi::HandleRuntimeErrorL()
	{
	iEikonEnv->SetBusy(EFalse);
	if (iDocument->AppFileMode()&EFileWrite)
		iGlobalEd->SetReadOnly(EFalse);
	TRuntimeResBuf res=iRuntimeResultPckg();
	if (res.iError)
		{
		iLastErrorBuf=res.iErrMsg;
		TSourceTranslateError tranErr;
		tranErr.SetPosition(res.iOffset);
		if (res.iSrcFile.CompareF(iDocument->Process()->MainDocFileName())==0)
			{
			TRAPD(err,DoLocateRuntimeErrorL(res.iDebugFlag,tranErr,res.iProcLineNumber));	// ignore error
			iEikonEnv->InfoMsg(iLastErrorBuf);
			}
		}
	}

void CTxtedAppUi::DoRunFileL()
	{
	TFileName fileName;
	if (iTranFileName)
		fileName=*iTranFileName;
	else
		{
		fileName=iDocument->Process()->MainDocFileName();
		TInt len=fileName.Length();
		if (len>4 && fileName.Right(4).CompareF(KOplExtension)==KErrNone)
			fileName.SetLength(len-4); // remove .OPL extension if present
		if (fileName.Length()+4<=fileName.MaxLength())
			fileName.Append(KOpoExtension);
		}
#if defined(__UIQ__)
#else
	if (CCknOpenFileDialog::RunSourceDlgLD(fileName,R_TXED_RUN_PROGRAM,R_TXED_RUN_SHOW_PROGRAMS,CCknFileListDialogBase::EShowAllDrives,KUidOplInterpreter,&KAppExtension))
#endif
		{
		DoRunL(fileName);
		}
	}

#define KDelim 32

void CTxtedAppUi::DoRunL(const TDesC& aFileName)
	{
	const TChar delim(KDelim);
	const TChar quotes('"');
	CApaCommandLine* apaCommandLine=CApaCommandLine::NewLC();
	apaCommandLine->SetLibraryNameL(KOplRuntimeLibraryName);
	apaCommandLine->SetCommandL(EApaCommandRun);

	TBuf<276>* fName=new(ELeave) TBuf<276>; // 256 filename plus quotes and TRuntimeParams
	CleanupStack::PushL(fName);

	// get default doc name
	TParsePtrC parse(aFileName);
	fName->Append(parse.DriveAndPath());
	fName->Append(parse.Name());
	fName->Append(KAppInfoFileExtension);
	CApaAppInfoFileReader* infoFileReader=NULL;
	TRAPD(err,infoFileReader=CApaAppInfoFileReader::NewL(iEikonEnv->FsSession(),*fName));
	iCoeEnv->ReadResource(*fName,R_EIK_TBUF_DEFAULT_FOLDER);
	TApaAppCaption caption=parse.Name();
	if (!err)
		TRAP(err,caption=infoFileReader->CaptionL(User::Language()));
	delete infoFileReader;
	TInt fNameLen=fName->Length();
	if (caption.Length()>KMaxFileName-fNameLen-1) // -1 because Opl runtime only supports 255
		caption.SetLength(KMaxFileName-fNameLen-1);
	fName->Append(caption);
	apaCommandLine->SetDocumentNameL(*fName);
	// tailend starts here
	TBool fileNameHasSpaces=(aFileName.Locate(delim)!=KErrNotFound);
	fName->Zero();
//#define _TEST_DEBUGGER
#ifdef _TEST_DEBUGGER
#pragma message("texted.cpp: Testing OPL debugger")
	fName->Append(TChar(KOplrCommandRunFromDebugger));	// special command-code for Debugger
#else
	fName->Append(TChar(KOplrCommandRunFromEditor));	// special command-code for Editor
#endif
	if (fileNameHasSpaces)
		fName->Append(quotes);
	fName->Append(aFileName);				// OPL program name
	if (fileNameHasSpaces)
		fName->Append(quotes);
	fName->Append(delim);	
	
	TRuntimeParams params;
	params.iFlags=KRuntimeFlagsSignal|KRuntimeFlagsNotify;
	params.iResultOffset=(TDes*)&iRuntimeResultPckg;
	params.iOwnerThreadId=RThread().Id();
	params.iOwnerWindowGroup=iCoeEnv->RootWin().Identifier();
	TPckgBuf<TRuntimeParams> paramsPckg(params);

	TInt len=fName->Size();
	TPtr8 ptr((TUint8*)fName->Ptr()+len,(fName->MaxLength()<<1)-len);
	ptr.Append(paramsPckg);
	fName->SetLength((len+ptr.Length())>>1);

	TPtrC8 fname8bit(REINTERPRET_CAST(const TUint8*,fName->Ptr()),fName->Size());
	apaCommandLine->SetTailEndL(fname8bit);
	CleanupStack::PopAndDestroy(); // fName

	// start the app
	iRuntimeResultPckg().iError=KErrNone; // reset to no error
	iOplRtLogon->Request(EikDll::StartAppL(*apaCommandLine));
	CleanupStack::PopAndDestroy(); // apaCommandLine
	iEikonEnv->SetBusy(ETrue);
	iGlobalEd->SetReadOnly(ETrue);
	}

void CTxtedAppUi::CmdShowLastError()
	{
	if (iLastErrorBuf.Length())
		iEikonEnv->InfoMsg(iLastErrorBuf);
	else
		iEikonEnv->InfoMsg(R_TXED_NO_ERROR_TO_SHOW);
	}

TInt CTxtedAppUi::GetTranslatorErrorResourceNumber(TInt aErr)
	{
	TInt resNo=0;
	OplAPI::MapTranslatorError(aErr,resNo);
	return resNo;
	}

void CTxtedAppUi::GetTranslateErrorText(TDes& aDes,TInt aError)
	{
	if (aError<0)
		{
		iEikonEnv->GetErrorText(aDes,aError);
		}
	else
		{ // must be a translator internal error
		iCoeEnv->ReadResource(aDes,GetTranslatorErrorResourceNumber(aError));
		}
	}

CArrayFix<TCoeHelpContext>* CTxtedAppUi::HelpContextL() const
	{
	// build the TCoeHelpContext object, and pass it back to Cone.
	CArrayFix<TCoeHelpContext>* viewHelpContext=new(ELeave) CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(viewHelpContext);
	viewHelpContext->AppendL(TCoeHelpContext(KUidTextEdApp,KOPL_HLP_VIEW_MAIN));
	CleanupStack::Pop(viewHelpContext);
	return viewHelpContext;
	}

//
// Clase CTxOplRtLogon
//
CTxOplRtLogon::CTxOplRtLogon(CTxtedAppUi& aAppUi) 
	:CActive(EActivePriorityIpcEventsHigh), iAppUi(aAppUi)
	{
	}

CTxOplRtLogon::~CTxOplRtLogon()
	{
	Cancel();
	delete iBusyMsg;
	User::Free(iBusyText);
	}

void CTxOplRtLogon::ConstructL()
	{
	CEikonEnv* eikonEnv=CEikonEnv::Static();
	iBusyMsg=new(ELeave) CEikBusyMsgWin(*eikonEnv);
	iBusyMsg->ConstructL(eikonEnv->RootWin());
	iBusyText=eikonEnv->AllocReadResourceL(R_TXED_RUNNING);
	CActiveScheduler::Add(this);
	}

void CTxOplRtLogon::DoCancel()
	{
	iBusyMsg->CancelDisplay();
	iOplRtThread.LogonCancel(iStatus);
	iOplRtThread.Close();
	}

void CTxOplRtLogon::RunL()
	{
	iBusyMsg->CancelDisplay();
	iOplRtThread.Close();
	iAppUi.HandleRuntimeErrorL();
	}

void CTxOplRtLogon::Request(const TThreadId& aId)
	{
	TInt err=iOplRtThread.Open(aId);
	if (err)
		{
		TRequestStatus* pS=&iStatus;
		User::RequestComplete(pS,err);
		}
	else
		iOplRtThread.Logon(iStatus);
	SetActive();
	iBusyMsg->StartDisplay(iBusyText->Des(),EHRightVBottom);
	}

//
// Clase CTextEdProcListDialog
//
CTextEdProcListDialog::CTextEdProcListDialog(MDesCArray* aArray,TInt &aItem)
	:iArray(aArray), iSelectedItem(aItem)
	{
	}

void CTextEdProcListDialog::PreLayoutDynInitL()
	{
	CEikTextListBox* listBox=(CEikTextListBox*)Control(ETextEdShowProcListId);
	CDesCArray* listBoxArray=((CDesCArray*)listBox->Model()->ItemTextArray());
	if (iArray!=NULL)
		{
		for (TInt i=0;i<iArray->MdcaCount();i++)
			listBoxArray->AppendL(iArray->MdcaPoint(i));
		}
	listBox->Model()->SetItemTextArray(iArray);
	listBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);
	listBox->SetCurrentItemIndex(iSelectedItem);

	listBox->CreateScrollBarFrameL();
	listBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto,CEikScrollBarFrame::EAuto);

	ButtonGroupContainer().SetDefaultCommand(EEikBidOk);
	}

TBool CTextEdProcListDialog::OkToExitL(TInt aButtonId)
	{
	switch (aButtonId)
		{
	case EEikBidOk: // Close
#if defined(__UIQ__)
		iSelectedItem=static_cast<CEikListBox*>(Control(ETextEdShowProcListId))->CurrentItemIndex();
#else
 		iSelectedItem=ListBoxCurrentItem(ETextEdShowProcListId);
#endif
		return(ETrue);
		break;
	default: // Escape
		return(EFalse);
		break;
		}
	}

void CTextEdProcListDialog::GetHelpContext(TCoeHelpContext& aContext) const
	{
	aContext.iMajor=KUidTextEdApp;
	aContext.iContext=KOPL_HLP_DIAL_SHOW_PROCEDURES;
	}

//
// Clase CTexEdTabDialog
//
CTextEdTabDialog::CTextEdTabDialog(TInt* aTabWidthInCharacters,TBool* aAutoIndent)
	{
	iTabWidthInCharacters=aTabWidthInCharacters;
	iAutoIndent=aAutoIndent;
	}

void CTextEdTabDialog::PreLayoutDynInitL()
	{
#if defined(__UIQ__)
	static_cast<CQikNumberEditor*>(Control(ETxedCidTabSetting))->SetValueL(*iTabWidthInCharacters);
#else
	SetNumberEditorValue(ETxedCidTabSetting,*iTabWidthInCharacters);
#endif
	TInt state=EPrefYes;
	state=(*iAutoIndent) ? EPrefYes : EPrefNo;
#if defined(__UIQ__)
	static_cast<CEikChoiceList*>(Control(ETxedCidAutoIndent))->SetCurrentItem(state);
#else
	SetChoiceListCurrentItem(ETxedCidAutoIndent,state);
#endif
	}

TBool CTextEdTabDialog::OkToExitL(TInt aButtonId)
	{
	TInt state=EPrefYes;
	TBool setting=ETrue;
	switch (aButtonId)
		{
	case EEikBidOk:
#if defined(__UIQ__)
		*iTabWidthInCharacters=static_cast<CQikNumberEditor*>(Control(ETxedCidTabSetting))->Value();
		state=static_cast<CEikChoiceList*>(Control(ETxedCidAutoIndent))->CurrentItem();
#else
		*iTabWidthInCharacters=NumberEditorValue(ETxedCidTabSetting);
		state=ChoiceListCurrentItem(ETxedCidAutoIndent);
#endif
		setting=(state==EPrefYes);
		*iAutoIndent=setting;
		return(ETrue);
		break;
		}
		return(EFalse);
	}

void CTextEdTabDialog::GetHelpContext(TCoeHelpContext& aContext) const
	{
	aContext.iMajor=KUidTextEdApp;
	// This tab dialog controls tab settings as well as auto indentation.
	aContext.iContext=KOPL_HLP_DIAL_INDENTATION;
	}

//
// Clase CTextEdSetPreferencesDialog
//
CTextEdSetPreferencesDialog::CTextEdSetPreferencesDialog(TBool* aIsMonoSpaced,TNonPrintingCharVisibility* aNonPrintingCharVisibility, TExportFormatOption& aExportFormatOption)
	:iExportFormatOption(aExportFormatOption)
	{
	iIsMonoSpaced=aIsMonoSpaced;
	iNonPrintingCharVisibility=aNonPrintingCharVisibility;
	}

void CTextEdSetPreferencesDialog::PreLayoutDynInitL()
	{
	TInt state=EPrefYes;
	
	state=(*iIsMonoSpaced) ? EPrefYes : EPrefNo;
#if defined(__UIQ__)
	static_cast<CEikChoiceList*>(Control(ETxedCidSetPrefsAllowPropFonts))->SetCurrentItem(state);
#else
	SetChoiceListCurrentItem(ETxedCidSetPrefsAllowPropFonts,state);
#endif
	
	state=(iNonPrintingCharVisibility->TabsVisible()) ? EPrefYes : EPrefNo;
#if defined(__UIQ__)
	static_cast<CEikChoiceList*>(Control(ETxedCidShowTab))->SetCurrentItem(state);
#else
	SetChoiceListCurrentItem(ETxedCidShowTab,state);
#endif

	state=(iNonPrintingCharVisibility->SpacesVisible()) ? EPrefYes : EPrefNo;
#if defined(__UIQ__)
	static_cast<CEikChoiceList*>(Control(ETxedCidShowSpaces))->SetCurrentItem(state);
#else
	SetChoiceListCurrentItem(ETxedCidShowSpaces,state);
#endif
	
	state=(iNonPrintingCharVisibility->ParagraphDelimitersVisible()) ? EPrefYes : EPrefNo;
#if defined(__UIQ__)
	static_cast<CEikChoiceList*>(Control(ETxedCidShowParaEnds))->SetCurrentItem(state);
#else
	SetChoiceListCurrentItem(ETxedCidShowParaEnds,state);
#endif

#if defined(__UIQ__)
	static_cast<CEikChoiceList*>(Control(ETxedCidSetPrefsExportOptions))->SetCurrentItem(iExportFormatOption);
#else
	SetChoiceListCurrentItem(ETxedCidSetPrefsExportOptions,iExportFormatOption);
#endif
	}

TBool CTextEdSetPreferencesDialog::OkToExitL(TInt aButtonId)
	{
	TBool visible=ETrue;
	TInt state=EPrefYes;
	switch (aButtonId)
		{
#if !defined(__UIQ__)
	case EEikBidOk: // Change
		return(EFalse);
		break;
	case ETextEdBidClose: // Close
#endif
#if defined(__UIQ__)
	case EEikBidOk: // OK
	case ETextEdBidClose: // Just for compatibility
		state=static_cast<CEikChoiceList*>(Control(ETxedCidSetPrefsAllowPropFonts))->CurrentItem();
#else
		state=ChoiceListCurrentItem(ETxedCidSetPrefsAllowPropFonts);
#endif
		visible=(state==EPrefYes);

		*iIsMonoSpaced=visible;
#if defined(__UIQ__)
		state=static_cast<CEikChoiceList*>(Control(ETxedCidShowTab))->CurrentItem();
#else
		state=ChoiceListCurrentItem(ETxedCidShowTab);
#endif
		visible=(state==EPrefYes);

		iNonPrintingCharVisibility->SetTabsVisible(visible);
#if defined(__UIQ__)
		state=static_cast<CEikChoiceList*>(Control(ETxedCidShowSpaces))->CurrentItem();
#else
		state=ChoiceListCurrentItem(ETxedCidShowSpaces);
#endif
		visible=(state==EPrefYes);

		iNonPrintingCharVisibility->SetSpacesVisible(visible);
#if defined(__UIQ__)
		state=static_cast<CEikChoiceList*>(Control(ETxedCidShowParaEnds))->CurrentItem();
#else
		state=ChoiceListCurrentItem(ETxedCidShowParaEnds);
#endif
		visible=(state==EPrefYes);

		iNonPrintingCharVisibility->SetParagraphDelimitersVisible(visible);
#if defined(__UIQ__)
		iExportFormatOption=(TExportFormatOption)static_cast<CEikChoiceList*>(Control(ETxedCidSetPrefsExportOptions))->CurrentItem();
#else
		iExportFormatOption=(TExportFormatOption)ChoiceListCurrentItem(ETxedCidSetPrefsExportOptions);
#endif
		return(ETrue);
		break;
	default: // Escape
		return(EFalse);
		break;
		}
	}

void CTextEdSetPreferencesDialog::GetHelpContext(TCoeHelpContext& aContext) const
	{
	aContext.iMajor=KUidTextEdApp;
	aContext.iContext=KOPL_HLP_DIAL_VIEW_PREFERENCES;
	}

//
// CTextEdTranslateDialog
//
CTextEdTranslateDialog::CTextEdTranslateDialog(CTextTranslator* aTranslator,TInt& aReturnValue,TInt aTitleResource)
	:iReturnValue(aReturnValue)
	{
	iTranslator=aTranslator;
	iTitleResource=aTitleResource;
	}

CTextEdTranslateDialog::~CTextEdTranslateDialog()
	{
	delete iIdle;
	}

void CTextEdTranslateDialog::PreLayoutDynInitL()
	{
	if (iTitleResource)
		SetTitleL(iTitleResource);
	}

void CTextEdTranslateDialog::PostLayoutDynInitL()
	{
	iIdle=CIdle::NewL(CActive::EPriorityLow);
	iIdle->Start(TCallBack(&TranslateChunk,this));
	}

TInt CTextEdTranslateDialog::OkToExitL(TInt /*aButtonId*/)
	{
	iTranslator->Cancel();
	iReturnValue=KErrCancel;
	return ETrue;
	}

TInt CTextEdTranslateDialog::TranslateChunk(TAny* aPtr)
	{ // static
	return (STATIC_CAST(CTextEdTranslateDialog*,aPtr)->DoTranslateChunk()==KErrNone); // repeat if KErrNone
	}

TInt CTextEdTranslateDialog::DoTranslateChunk()
	{
	TInt ret=iTranslator->TranslateChunk();
	iReturnValue=ret;
	if (ret!=KErrNone) // translate complete or error
		delete this;
	return ret;
	}

void CTextEdTranslateDialog::GetHelpContext(TCoeHelpContext& aContext) const
	{
	aContext.iMajor=KUidTextEdApp;
	aContext.iContext=KOPL_HLP_DIAL_TRANSLATION;
	}

CTextEdApplication::~CTextEdApplication()
	{
	FLOGWRITE(_L("CTextEdApplication dtor."));
	delete iCharFormat;
	delete iParaFormat;
	}

CApaDocument* CTextEdApplication::CreateDocumentL()
	{
	FLOGWRITE(_L("CTextEdApplication::CreateDocumentL() starts."));
	if (!iParaFormat)
		{
		FLOGWRITE(_L("No paraformat."));
		iParaFormat=CEikonEnv::NewDefaultParaFormatLayerL();
		}
		
	if (!iCharFormat)
		{
		FLOGWRITE(_L("No charformat."));
		// This is based on the way CWord (25/01/2002) does things
		TCharFormat charFormat;
		TCharFormatMask charFormatMask;
		TInt typefaceAttributes=0;
		charFormat.iFontSpec.iTypeface.SetAttributes(typefaceAttributes);
		charFormat.iFontSpec.iHeight=200;
		charFormatMask.SetAttrib(EAttFontTypeface);
		charFormatMask.SetAttrib(EAttFontHeight);
		iCharFormat=CCharFormatLayer::NewL(charFormat,charFormatMask);
		}
	FLOGWRITE(_L("New texted doc."));
	CTextEdDocument* doc=new(ELeave) CTextEdDocument(*this);
	CleanupStack::PushL(doc);

	// Print setup not used on UIQ thumb.
	FLOGWRITE(_L("Getting print setup..."));
	CPrintSetup *defaultPrintSetup=NULL;
	TRAPD(err,defaultPrintSetup=iEikonEnv->NewDefaultPrintSetupL());

	if (err)
		{
		defaultPrintSetup=NULL; // Ensure that it's null.
		TBuf<0x7f> flogMsg;
		flogMsg.Format(_L("Error from print setup: %d"),err);
		FLOGWRITE(flogMsg);
		}
	else
		{
		FLOGWRITE(_L("no error from print setup."));
		}
	
	FLOGWRITE(_L("doc ctor."));
	doc->ConstructL(iParaFormat,iCharFormat,defaultPrintSetup);
	CleanupStack::Pop();
	FLOGWRITE(_L("CTextEdApplication::CreateDocumentL() ends."));
	return doc;
	}

TUid CTextEdApplication::AppDllUid() const
	{
	FLOGWRITE(_L("CTextEdApplication::AppDllUid()."));
	return KUidTextEdApp;
	}

EXPORT_C CApaApplication* NewApplication()
	{
	FLOGWRITE(_L("Creating new CTextEdApplication."));
	return(new CTextEdApplication);
	}

GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
