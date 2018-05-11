// EN_ENTRY.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <oplr.h>
#include <oplr.rsg>
#include <basched.h>
#include <qikappui.h>
#include "OplStartUpDialog.h" 
#include <opldoc.h>
#include "oplutil.h"
#include "graphics.h"
#include <bautils.h>
#include "opldialg.h"
#include "frame.h"
#include "module.h"
#include "opldb.h"
#include "opldbg.h"
#include "debug.h"

//
// Main
//
GLDEF_C TInt E32Dll(TDllReason /* aReason */)
	{
	// RDebug::Print(_L("E32Dll()\n"));
	return(KErrNone);
	}

#if defined(_DEBUG)
GLREF_C void DebugPrintThreadHandles(const TDesC& aText);
GLDEF_C void DebugPrintThreadHandles(const TDesC& aText)
	{
	TInt processHandleCount=0;
	TInt threadHandleCount=0;
	RThread().HandleCount(processHandleCount, threadHandleCount);
	RDebug::Print(_L("!!!!! eb205: (%d) %d, %S"), processHandleCount, threadHandleCount, &aText);
	}
#endif

//
// Class TOplDocRootStream
//

EXPORT_C void TOplDocRootStream::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iAppUid; // usually KUidOplInterpreter (but may want to support other?)
	aStream	<< iStreamId;
	}

EXPORT_C void TOplDocRootStream::InternalizeL(RReadStream& aStream)
	{
	aStream >> iAppUid;
	aStream	>> iStreamId;
	}

//
// Main
//
COplStartUpAO::COplStartUpAO(COplRuntime* aRuntime) 
	:CActive(EPriorityHigh) // may need to change this
	{
	FLOGWRITE(_L("COplStartUpAO::COplStartUpAO()"));
	iRuntime=aRuntime;
	CActiveScheduler::Add(this);
	}

COplStartUpAO::~COplStartUpAO()
	{
	Cancel();
	}

void COplStartUpAO::DoCancel()
	{
	}

TInt COplStartUpAO::RunError(TInt /*aError*/)
	{
	// Just return KErrNone to stop the active scheduler panicking.
	return KErrNone;
	}

void COplStartUpAO::StartItUpL()
	{
	SetActive();
	TRequestStatus* pS=&iStatus;
	User::RequestComplete(pS,KErrNone);
	}

void COplStartUpAO::RunL()
	{
	iRuntime->ExecuteCheck();
	}

//
// COplRuntime class
//
#ifdef __EXE__
LOCAL_D COplRuntime* OplRuntime;
COplRuntime* TheRuntime()
	{
	return OplRuntime;
	}

void SetTheRuntime(COplRuntime* aRuntime)
	{
	OplRuntime=aRuntime;
	}
#endif

EXPORT_C COplRuntime::COplRuntime()
	{
#ifdef _DEBUG
	DebugPrintThreadHandles(_L("COplRuntime::COplRuntime"));
#endif
	}

EXPORT_C COplRuntime::~COplRuntime()
    {
	FLOGWRITE(_L("COplRuntime::~COplRuntime()"));
    if (iAppStartUpDialog)
        {
        RemoveFromStack(iAppStartUpDialog);
        delete iAppStartUpDialog;
		iAppStartUpDialog=NULL;
        }
	if (iStartUpAO)
	{
		iStartUpAO->Deque();
		delete iStartUpAO;
		iStartUpAO=NULL;
	}
//	delete iOplAlertWin;
	delete iCommandLine;
	delete iCurrentDocumentName;
	while (iFrame!=NULL)
		delete iFrame;
	delete this->iStack;
	delete iGlobTbl;
	DeleteMenu(); // Must be done before deleting the console (or any control that can gain focus)
 	delete iDrawablesCollection;
	delete iConsole;
	delete iModulesCollection;
	delete iDbManager;
	delete iIOCollection;
	delete iOplAPI;
	delete iOplDialog;
	delete iDebuggerBase;
	iDebuggerLib.Close();
	delete iDebuggerAPI;
	iDir.Close();
	iCoeEnv->DeleteResourceFile(iResourceFile);
//	delete iClockUpdater;
	if (iHelpContextNamesArray)
		{
		iHelpContextNamesArray->Reset();
		delete iHelpContextNamesArray;
		}
#ifdef _HISTORY
	iHistory.Close();
#endif
	if (iHeap64)
		iHeap64->Close();
#ifdef _DEBUG
	DebugPrintThreadHandles(_L("COplRuntime::~COplRuntime()"));
#endif
	}

EXPORT_C void COplRuntime::ConstructL()
    {
	FLOGWRITE(_L("COplRuntime::ConstructL()"));
    BaseConstructL(ENoAppResourceFile);
	SetTheRuntime(this);
	iIOCollection=new(ELeave) CIOCollection;
	iIOCollection->ConstructL(this,ConEnv()->WsSession());

	iConsole=new(ELeave) COplConsole(ConEnv()->WsSession());
	TInt color=0;
	TInt gray=0;
	TDisplayMode dMode=ConEnv()->WsSession().GetDefModeMaxNumColors(color,gray);
	iConsole->ConstructL(ConEnv()->ScreenDevice(),&(iIOCollection->WsEventHandler()),ConEnv()->RootWin(),dMode);

	// Find and load the runtime resource file.
	TFileName resourceFile=RuntimeResourceFile();
	TFindFile FindResourceFile(iCoeEnv->FsSession());
	User::LeaveIfError(FindResourceFile.FindByDir(resourceFile,KNullDesC));
	resourceFile=FindResourceFile.File();

	BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(),resourceFile);
	iResourceFile=iCoeEnv->AddResourceFileL(resourceFile);

	iCommandLine=new(ELeave) COplCommandLine;
	iCurrentDocumentName=KNullDesC().AllocL();
	iStartUpAO=new(ELeave) COplStartUpAO(this);
	iHelpContextNamesArray=new(ELeave) TFixedArray<TCoeContextName,KOplLenContextNamesArray>;
	iHelpUid=KNullUid;

#ifdef __USE_DIALOG
    iAppStartUpDialog = new (ELeave) COplRuntimeStartUpDialog;
	iAppStartUpDialog->SetMopParent(this);
    iAppStartUpDialog->ExecuteLD(R_OPLRUNTIMESTARTUP_DIALOG);
    AddToStackL(iAppStartUpDialog);
#else
	iStartUpAO->StartItUpL();
#endif
	CEikMenuBar* menubar=new (ELeave)CEikMenuBar;
	CleanupStack::PushL(menubar);
	menubar->ConstructL(this);
	TResourceReader reader;
	iEikonEnv->CreateResourceReaderLC(reader,R_OPLRSTARTUPDIALOG_MENUBAR);
	menubar->ConstructFromResourceL(reader);
	CleanupStack::PopAndDestroy();//reader buffer
	iEikonEnv->AppUiFactory()->SwapMenuBar(menubar);
	CleanupStack::Pop(menubar);
	}

void COplRuntime::ExecuteCheck()
	{
	// reserve some space on the cleanup stack 
	//DEBUG ONLY!!!
//	for (TInt ii=0;ii<1000;++ii)
//		CleanupStack::PushL(&ii);
//	CleanupStack::Pop(1000);
#pragma message("Disabling ExecuteCheck() __UHEAP_ checking...")
//	__UHEAP_MARK;
	ExecuteD();
//	__UHEAP_MARKEND;
	// Having completed, close everything down...
	iStartUpAO->Deque();
	delete iStartUpAO;
	iStartUpAO=NULL;
	CQikAppUi::PrepareToExit();
	delete TheRuntime(); // closes files etc.
	User::Exit(0);
	}

void COplRuntime::ExecuteD()
	{
	FLOGWRITE(_L("COplRuntime::ExecuteD()"));
	TInt err;
	TBuf<256> buf;
/*
	_LIT(KStarting,"Starting the OS v6.1 OPL runtime!\r\n\n");
	buf.Format(KStarting);
	iConsole->Print(buf);
*/
#ifdef _HISTORY 
	if ((err=iHistory.Open(iCoeEnv->FsSession()))==0)
		{
#endif
		TRAP(err,InitializeL(0,iCommandLine->ModuleName()));
		if (!err)
			{
			CWsEventHandler& wsEventHandler=iIOCollection->WsEventHandler();
			wsEventHandler.Start();
//			__UHEAP_MARK;
			err=Execute();
//			__UHEAP_MARKEND;
			wsEventHandler.Stop();
			}
		else
			{
			iErrorValue=err;
			if (iFrame) // iFrame is set if we managed to load the module
				ErrorHandler(); // sets up error info returned to texted
			}
#ifdef _HISTORY 
		}
#endif
	TRuntimeParams params=iCommandLine->RuntimeParams();

	if (err)
		{
		GetErrStr(buf,OplUtil::MapError(err));
		if (iCommandLine->Command()!=KOplrCommandRunFromEditor || params.iFlags&KRuntimeFlagsNotify)
			{
			((CEikonEnv*)this->ConEnv())->AlertWin(iErrBuf,buf);
			}
		}
	if (params.iFlags&KRuntimeFlagsSignal)
		{
		// other iRuntimeResBuf info was set up in error handler
		iRuntimeResBuf.iError=err; // if KErrNone rest of error info to be ignored
		iRuntimeResBuf.iErrMsg=buf;
		// signal owner if exists (offset in owner is iRuntimeParams.iResultOffset);
		RThread owner;
		if (owner.Open(params.iOwnerThreadId)==KErrNone)
			// ?? what if owner exits now before writing?
			{
			TPckgBuf<TRuntimeResBuf> resBufPckg(iRuntimeResBuf);
			owner.WriteL(params.iResultOffset,resBufPckg,0);
			}
		}
//	CBaActiveScheduler::Exit();
	}

EXPORT_C void COplRuntime::HandleWsEventL(const TWsEvent& aEvent,CCoeControl* aDestination)
	{
	if (aEvent.Type()!=EEventPointer||((TInt)aDestination>KMaxDrawables))
		CEikAppUi::HandleWsEventL(aEvent,aDestination);
	}

EXPORT_C void COplRuntime::HandleSystemEventL(const TWsEvent& aEvent)
	{
	// This is only called when we are displaying a menu or dialog. Ideally the app
	// should be locked (and OPOs shouldn't get the event) so we can throw away
	// any system events.
	//
	// Only one USED to be Shutdown but in Crystal 6.0 there is also one for
	// activating the top view when we come to the foreground. We now support that
	// BUT also for Crystal 6.0 we hack the fact apps won't close down if the user
	// tries to use our own menu pane task list to close this app.
	switch (*(TApaSystemEvent*)(aEvent.EventData()))
		{
	case EApaSystemEventShutdown:
		{
#ifdef THERE_IS_NO_SPOON
		CEikMenuBar* menuBar=iEikonEnv->AppUiFactory()->MenuBar();
		if (menuBar)
			if (menuBar->SelectedTitle()==0) // only for the first menu pane
				{
				menuBar->StopDisplayingMenuBar();
				COplRuntime::iStack->Push((TInt16)0);
				if (iEikonEnv->IsBusy()) // Code has correctly used LOCK ON before menu
					iEikonEnv->SetBusy(EFalse);
				RWsSession& ws=iEikonEnv->WsSession();
				RWindowGroup& rw = iEikonEnv->RootWin();
				TInt winId = rw.Identifier();
				TApaTask tApatsk(ws);
				tApatsk.SetWgId(winId);
				if (!iEikonEnv->RespondsToShutdownEvent())
					tApatsk.KillTask();					
				else
					tApatsk.SendSystemEvent(EApaSystemEventShutdown);
				CActiveScheduler::Stop();
				}
#endif
		}
		break;
	case EApaSystemEventBroughtToForeground:
		{
#ifdef _USE_VIEWSRV_CRYSTAL
		// Will leave with KErrNotSupported from LOAK21 (since ViewSrv was removed)
		TRAPD(ignore,ActivateTopViewL());
#endif
		}
		break;
	default:
		break;
		}
	}

EXPORT_C MCoeMessageObserver::TMessageResponse COplRuntime::HandleMessageL(TUint32 /*aClientHandleOfTargetWindowGroup*/, TUid /*aMessageUid*/, const TDesC8& /*aMessageParameters*/)
	{
	// This is only called when we are displaying a menu or dialog. Ideally the app
	// should be locked (and OPOs shouldn't get the event) so we can throw away
	// any system events.
	//
	// Only one at the moment is switch files
	return EMessageNotHandled;
	}

EXPORT_C void COplRuntime::HandleForegroundEventL(TBool aForeground)
	{
	// Have to override this so we can make it public not protected - but all we want to
	// do is base call
	CEikAppUi::HandleForegroundEventL(aForeground); // Handle fading, etc.
	}

void COplRuntime::HandleControlEventL(CCoeControl* /*aControl*/,MCoeControlObserver::TCoeEvent /*aEvent*/)
	{
	}

EXPORT_C void COplRuntime::ProcessCommandL(TInt aSelection)
	{
	CEikMenuBar* menuBar=iEikonEnv->AppUiFactory()->MenuBar();
	if (menuBar)
		menuBar->StopDisplayingMenuBar();

	if (iEikonEnv->AppUiFactory()->Popup())
		ClosePopup();

	TInt key=0;
/*	switch (aSelection)
		{
	case EEikCmdZoomIn:
		key=EEikSidebarZoomInKey;
		break;
	case EEikCmdZoomOut:
		key=EEikSidebarZoomOutKey;
		break;
	case EOplCmdSidebarEditMenu:
		key=EEikSidebarClipKey;
		break;
	case EOplCmdSidebarIrdaMenu:
		key=EEikSidebarIrdaKey;
		break;
		}
*/
	if (key)
		{
		__DEBUGGER(); // eb205: I wouldn't mind looking at the call stack when we get here - is there a TRAP[D] anywhere down there or not?
		IOCollection().WsEventHandler().AppendKeyEventToQueueL(key,0);
		aSelection=EEikCmdCanceled;
		}

// Fix from Arjen for [934352] MPOPUP crashes
	TUint16 lastMenuItem=0;
	if (menuBar)
		{
		lastMenuItem=(TUint16)((((TUint8)(menuBar->SelectedTitle()))<<8)+((TUint8)menuBar->SelectedItem()));
		}
	if (aSelection != EEikCmdCanceled)
	{
	if ((iMenuInitPtr) && !(UserFlags() & KOplStateOPL1993MenuCancelBehaviour))
		OplUtil::PutWord(iMenuInitPtr,lastMenuItem);
		for(TInt index=0;index<iMenuItemsArray->Count();index++)
			{
			if(aSelection == (*iMenuItemsArray)[index]->iData.iCommandId)
				{
				if ((iMenuInitPtr) && (UserFlags() & KOplStateOPL1993MenuCancelBehaviour))
					OplUtil::PutWord(iMenuInitPtr,lastMenuItem);
				COplRuntime::iStack->Push(TInt16((*iMenuItemsArray)[index]->iHotKeyCode));
				break;
				}
			}
		}
	if (aSelection != EEikCmdCanceled)
		CActiveScheduler::Stop();
	}


EXPORT_C TDesC& COplRuntime::CurrentDocumentName()
	// Unfortunately this can leave if no current doc and new one can't be allocated
	// But App should have exited in this case anyway
	{ 
	if (iCurrentDocumentName==NULL) // may be null if SETDOC ran out of memory
		iCurrentDocumentName=KNullDesC().AllocL();
	return *iCurrentDocumentName;
	}


void COplRuntime::SetCurrentDocumentName(const TDesC& aName)
	{
	delete iCurrentDocumentName;
	iCurrentDocumentName=NULL; // in case alloc fails
	iCurrentDocumentName=aName.AllocL();
	iEikonEnv->UpdateTaskNameL();

	// Ignore any Not Found error if device doesn't support Recent Files API.
	if (aName.Length()!=0)
		{
		TRAPD(ignore,CApaRecentFile::AddLastUsedEntryL(TheRuntime()->ConEnv()->FsSession(), aName, iCommandLine->AppUid()));
		}
	}



EXPORT_C MCoeFepAwareTextEditor_Extension1* COplRuntime::Extension1()
	{
	return NULL;
	}


void COplRuntime::StartFepInlineEditL(const TDesC& /*aInitialInlineText*/, TInt /*aPositionOfInsertionPointInInlineText*/, TBool /*aCursorVisibility*/, 
									  const MFormCustomDraw* /*aCustomDraw*/, 
									  MFepInlineTextFormatRetriever& /*aInlineTextFormatRetriever*/, 
									  MFepPointerEventHandlerDuringInlineEdit& /*aPointerEventHandlerDuringInlineEdit*/)
	{
	}

void COplRuntime::UpdateFepInlineTextL(const TDesC& /*aNewInlineText*/, TInt /*aPositionOfInsertionPointInInlineText*/)
	{
	}


void COplRuntime::DoCommitFepInlineEditL()
	{
	}

void COplRuntime::SetInlineEditingCursorVisibilityL(TBool /*aCursorVisibility*/)
	{
	}


void COplRuntime::CancelFepInlineEdit() {}
TInt COplRuntime::DocumentLengthForFep() const {return NULL;}
TInt COplRuntime::DocumentMaximumLengthForFep() const {return NULL;}
void COplRuntime::SetCursorSelectionForFepL(const TCursorSelection& /*aCursorSelection*/) {}
void COplRuntime::GetCursorSelectionForFep(TCursorSelection& /*aCursorSelection*/) const {}
void COplRuntime::GetEditorContentForFep(TDes& /*aEditorContent*/, TInt /*aDocumentPosition*/, TInt /*aLengthToRetrieve*/) const {}
void COplRuntime::GetFormatForFep(TCharFormat& /*aFormat*/, TInt /*aDocumentPosition*/) const {}
void COplRuntime::GetScreenCoordinatesForFepL(TPoint& /*aLeftSideOfBaseLine*/, TInt& /*aHeight*/, TInt& /*aAscent*/, TInt /*aDocumentPosition*/) const {}

// End of file
