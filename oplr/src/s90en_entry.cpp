// EN_ENTRY.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.
// Not used for Series 60 - see \oplr\src\s60en_entry.cpp instead.

#include <e32std.h>
#include <w32std.h>
#include <opcodes.h>
#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <eikon.hrh>
#include <apgwgnam.h>
#include <apparc.h>
#include <bautils.h>
#include <oplcmd.h>
#include "oplutil.h"
#include "graphics.h"
#include "frame.h"
#include <module.h>
#include <opldoc.h>
#include "opldialg.h"

#include <eikon.rsg>
#include <basched.h>
#include <apgtask.h>

#include "debug.h" // for Flogger.

#define EOplCmdSidebarEditMenu 0x1000
#define EOplCmdSidebarIrdaMenu 0x1001

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
COplStartUp::COplStartUp(COplRuntime* aRuntime)
	:CActive(EPriorityHigh) // may need to change this
	{
	FLOGWRITE(_L("COplStartUp:COplStartUp()"));
	iRuntime=aRuntime;
	CActiveScheduler::Add(this);
	SetActive();
	TRequestStatus* pS=&iStatus;
	User::RequestComplete(pS,KErrNone);
	}

COplStartUp::~COplStartUp()
	{
	Cancel();
	}

void COplStartUp::DoCancel()
	{
	}

void COplStartUp::RunL()
	{
	__UHEAP_MARK;
	iRuntime->ExecuteD();
	__UHEAP_MARKEND;
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
	FLOGWRITE(_L("COplRuntime::COplRuntime()"));
#ifdef _DEBUG
	DebugPrintThreadHandles(_L("COplRuntime::COplRuntime"));
#endif
	}

void COplRuntime::ConstructL()
	{
	FLOGWRITE(_L("COplRuntime::ConstructL()"));
	SetTheRuntime(this);
	
	iIOCollection=new(ELeave) CIOCollection;
	iIOCollection->ConstructL(this,ConEnv()->WsSession());

	iConsole=new(ELeave) COplConsole(ConEnv()->WsSession());

	TInt color=0;
	TInt gray=0;
	TDisplayMode dMode=ConEnv()->WsSession().GetDefModeMaxNumColors(color,gray);
	
	iConsole->ConstructL(ConEnv()->ScreenDevice(),&(iIOCollection->WsEventHandler()),ConEnv()->RootWin(),dMode);

	BaseConstructL(ENoAppResourceFile|ENoScreenFurniture);

	// Find and load the runtime resource file.
	TFileName resourceFile=RuntimeResourceFile();
	TFindFile FindResourceFile(iCoeEnv->FsSession());
	User::LeaveIfError(FindResourceFile.FindByDir(resourceFile,KNullDesC));
	resourceFile=FindResourceFile.File();
	BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(),resourceFile);
	iResourceFile=iCoeEnv->AddResourceFileL(resourceFile);

	iCommandLine=new(ELeave) COplCommandLine;
	iCurrentDocumentName=KNullDesC().AllocL();
	iStartUp=new(ELeave) COplStartUp(this);
	iHelpContextNamesArray=new(ELeave) TFixedArray<TCoeContextName,KOplLenContextNamesArray>;
	iHelpUid=KNullUid;
	}

void COplRuntime::ExecuteD()
	{
	FLOGWRITE(_L("COplRuntime::ExecuteD()"));
	TInt err;
	TBuf<256> buf;
/*
	_LIT(KStarting,"Starting OPL runtime\r\n\n");
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
			err=Execute();
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
	CBaActiveScheduler::Exit();
	}

void COplRuntime::HandleWsEventL(const TWsEvent& aEvent,CCoeControl* aDestination)
	{
	if (aEvent.Type()!=EEventPointer||((TInt)aDestination>KMaxDrawables))
		CEikAppUi::HandleWsEventL(aEvent,aDestination);
	}

void COplRuntime::HandleSystemEventL(const TWsEvent& aEvent)
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
		}
		break;
	case EApaSystemEventBroughtToForeground:
		{
		// Will leave with KErrNotSupported from LOAK21 (since ViewSrv was removed)
		TRAPD(ignore,ActivateTopViewL());
		}
		break;
	default:
		break;
		}
	}

MCoeMessageObserver::TMessageResponse COplRuntime::HandleMessageL(TUint32 /*aClientHandleOfTargetWindowGroup*/, TUid /*aMessageUid*/, const TDesC8& /*aMessageParameters*/)
	{
	// This is only called when we are displaying a menu or dialog. Ideally the app
	// should be locked (and OPOs shouldn't get the event) so we can throw away
	// any system events.
	//
	// Only one at the moment is switch files
	return EMessageNotHandled;
	}

void COplRuntime::HandleForegroundEventL(TBool aForeground)
	{
	// Have to override this so we can make it public not protected - but all we want to
	// do is base call
	CEikAppUi::HandleForegroundEventL(aForeground); // Handle fading, etc.
	}

void COplRuntime::HandleControlEventL(CCoeControl* /*aControl*/,MCoeControlObserver::TCoeEvent /*aEvent*/)
	{
	}

void COplRuntime::ProcessCommandL(TInt aSelection)
	{
	CEikMenuBar* menuBar=iEikonEnv->AppUiFactory()->MenuBar();
	if (menuBar)
		menuBar->StopDisplayingMenuBar();

	if (iEikonEnv->AppUiFactory()->Popup())
		ClosePopup();
	
	const TUint16 lastMenuItem=(TUint16)((((TUint8)(iEikonEnv->AppUiFactory()->MenuBar()->SelectedTitle()))<<8)+((TUint8)iEikonEnv->AppUiFactory()->MenuBar()->SelectedItem()));

// Fix from Arjen for [934352] MPOPUP crashes
	TUint16 lastMenuItem=0;
	if (menuBar)
		{
		lastMenuItem=(TUint16)((((TUint8)(menuBar->SelectedTitle()))<<8)+((TUint8)menuBar->SelectedItem()));
		if ((iMenuInitPtr) && !(UserFlags() & KOplStateOPL1993MenuCancelBehaviour))
			OplUtil::PutWord(iMenuInitPtr,lastMenuItem);
		}

	if (aSelection == EEikCmdCanceled)
		COplRuntime::iStack->Push((TInt16)0);
	else
		{
		for(TInt index=0;index<iMenuItemsArray->Count();index++)
			{
			if(aSelection == (*iMenuItemsArray)[index]->iData.iCommandId)
				{
				if (menuBar)
					{
					if ((iMenuInitPtr) && (UserFlags() & KOplStateOPL1993MenuCancelBehaviour))
					OplUtil::PutWord(iMenuInitPtr,lastMenuItem);
					}
				COplRuntime::iStack->Push(TInt16((*iMenuItemsArray)[index]->iHotKeyCode));
				break;
				}
			}
		}
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