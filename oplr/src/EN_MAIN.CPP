// EN_MAIN.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <bldvariant.hrh>
#if defined(__SERIES60__)
#include <e32uid.h>
#include <opcodes.h>
#include <oplr.h>
#include "graphics.h"
#include "oplutil.h"
#include "opldialg.h"
#include <aknenv.h>
#include <eikhkeyt.h>
#include <opldbg.h>
#include <bafindf.h>
#include "clckupdt.h"
#include "opldb.h"
#include "module.h"
#include "frame.h"
#include "debug.h"
#include <avkon.rsg>
#elif defined(__UIQ__)
#include <OplAPI.h>
#include <e32hal.h>
#include <e32uid.h>
#include <opcodes.h>
#include <module.h>
#include <oplcmd.h>
#include <opldb.h>
#include <eikon.hrh>
#include <apgwgnam.h>
#include "frame.h"
#include "graphics.h"
#include "oplutil.h"
#include <opldbg.h>
#include "opldialg.h"
#include "iodev.h"
#include "oplr.h"
#include "clckupdt.h"
#include <bafindf.h>
#else
#include <OplAPI.h>
#include <e32hal.h>
#include <e32uid.h>
#include <opcodes.h>
#include <module.h>
#include <oplcmd.h>
#include <opldb.h>
#include <eikon.hrh>
#include <apgwgnam.h>
#include "frame.h"
#include "graphics.h"
#include "oplutil.h"
#include <opldbg.h>
#include "opldialg.h"
#include "iodev.h"
#include "oplr.h"
#include "clckupdt.h"
#include <bafindf.h>
#include <cknctl.rsg>
#include <ckon.mbg>
#endif


#define KMin64HeapSize 0x1000
#define KMenuBarId 0x2000

#if defined(__SERIES60__)
#define KMenuPaneIdOffset R_AVKON_MENUPANE_FEP_DEFAULT
#elif defined(__UIQ__)
#define KMenuPaneIdOffset 0
#else
#define KMenuPaneIdOffset R_CKN_TASKS_MENUPANE_STUB
#endif

#if defined(__SERIES60__)
#define KCommandIdOffset 0x8000
#else
#define KCommandIdOffset 0x4000
#endif
#define KSizeMagicStatics 0x36

enum {ENumberOfOpcodesToRunBetweenEventChecks=150};
enum {EMaximumNumberOfCompletedEventsToHandlePerEventCheck=5};

GLREF_C void DebugPrintThreadHandles(const TDesC& aText);

_LIT(KOplRuntime, "OPL-runtime");
TInt COplRuntime::Execute()
	{
//	FLOGWRITE(_L("COplRuntime::Execute() starts."));
	__ASSERT_DEBUG(iIOCollection->WsEventHandler().IsStarted(), User::Panic(KOplRuntime, 1));
	TInt returnValue=0;
	FOREVER
		{
		TRAPD(err,SequencerL());
		if (!err)				// sequencer exited with 0
			break;				// so exit runtime with no error
		iErrorValue=err;		// set value returned by ERR function
		if (ErrorHandler())
			{
			returnValue=iErrorValue;
			break;
			}
		}
	__ASSERT_DEBUG(iIOCollection->WsEventHandler().IsStarted(), User::Panic(KOplRuntime, 2));

	// Fix the E32User-CBase 46 Panic.
	// Soak up any lingering event, it's too late for the sequencer to process it.
//	FLOGWRITE(_L("COplRuntime::Execute() about to DoConsumeEvent()."));
	IOCollection().WsEventHandler().DoConsumeEvent(*iCoeEnv);
//	FLOGWRITE(_L("COplRuntime::Execute() exits."));
	return returnValue;
	}

typedef CDebuggerBase* (*TDebuggerEntryPointL)(CDebuggerAPI* aDebuggerAPI);

void COplRuntime::InitializeL(const TInt aFlags, const TModuleName aModuleName)
	// sets the environment for execution
	{
#if defined(__SERIES60__)
#pragma message("Need to fix the iOplAlertWin!!!")
// It's still using three-button alert, prolly not suitable for OSv6.1!
#else
	iOplAlertWin=new(ELeave) COplAlertWin;
	iOplAlertWin->ConstructL(iCoeEnv);
#endif
	iDebuggerAPI=new(ELeave) CDebuggerAPI(this);

	if (iCommandLine->TailEndCommand()==KOplrCommandRunFromDebugger)
		{
		CFindFileByType* findFile=new(ELeave)CFindFileByType(ConEnv()->FsSession());
		CleanupStack::PushL(findFile);
		if (findFile->FindFirst(DebuggerDllFilename(),KNullDesC,TUidType(KDynamicLibraryUid,KUidOplDebugger,KNullUid))==KErrNone)
			{
			User::LeaveIfError(iDebuggerLib.Load(findFile->File().FullName()));
			TDebuggerEntryPointL func=(TDebuggerEntryPointL)iDebuggerLib.Lookup(1);
			if (!func)
				User::Leave(KErrBadLibraryEntryPoint);
			iDebuggerBase=(*func)(iDebuggerAPI);
			}
		CleanupStack::PopAndDestroy(); // findFile
		}
		
	COplRuntime::iFlags = aFlags;
	COplRuntime::iStack=CStack::NewL(4096,this);

	iSeed=0x12345678;

	iGlobTbl=new(ELeave) CGlobalsTbl;

	TMemoryInfoV1Buf membuf;
	UserHal::MemoryInfo(membuf);
	TInt maxmem=membuf().iTotalRamInBytes;

	SetHeap64(UserHeap::ChunkHeap(NULL,KMin64HeapSize,maxmem));
#ifdef _DEBUG
	if (!iHeap64)
		{
		RDebug::Print(_L("*** Module '%S' is unable to alloc the ChunkHeap."),&aModuleName);
		// Looking at defect EDNRANS-4J3K86
		// Dump free RAM info.
		RDebug::Print(_L("iTotalRamInBytes=0x%x"),maxmem);
		RDebug::Print(_L("iMaxFreeRamInBytes=0x%x"),membuf().iMaxFreeRamInBytes);
		RDebug::Print(_L("iFreeRamInBytes=0x%x"),membuf().iFreeRamInBytes);
		}
#endif
	User::LeaveIfNull(iHeap64);
	iHeap64->AllocL(KSizeMagicStatics);

	iDrawablesCollection = new(ELeave) CDrawablesCollection(ConEnv()->WsSession());
	iDrawablesCollection->SetConsoleWinL(*this,iConsole);

#if !defined(__SERIES60__)
	iClockUpdater=new(ELeave) COplClockUpdater(*iDrawablesCollection);
	iClockUpdater->ConstructL();
#endif

	iDbManager=COplDbManager::NewL(*this,*COplRuntime::iStack);

	iOplAPI = new(ELeave) OplAPI(this,*iHeap64);

	iModulesCollection = new(ELeave) CModulesCollection;
	iModulesCollection->Init(ConEnv()->FsSession());
	
	iModulesCollection->LoadModuleL(aModuleName);
	TUid uid(iModulesCollection->Uid());
	iCommandLine->SetAppUid(uid);

	iEikonEnv->UpdateTaskNameL();

	iFrame = new(ELeave) CFrame;
	COplRuntime::iStack->Push(TInt16(0));
	iFrame->ConstructL(*COplRuntime::iStack,*this,iModulesCollection->FindFirstProcedure());
	iFrame->FinishConstructionL();

	iCoeEnv->Cancel(); // cancel outstanding Ws Event
	User::LeaveIfError(iCoeEnv->RootWin().AddPriorityKey(EKeyEscape,EAllStdModifiers,KKillKeyModifier));
	User::LeaveIfError(iCoeEnv->RootWin().AddPriorityKey(CTRL('s'),EAllStdModifiers,KPauseKeyModifier));
	User::LeaveIfError(iCoeEnv->RootWin().AddPriorityKey(CTRL('q'),EAllStdModifiers,KPauseKeyModifier));
	ConEnv()->WsSession().ComputeMode(RWsSession::EPriorityControlComputeOn);
	}

void COplRuntime::SequencerL()
	{
	CWsEventHandler& eventHandler=iIOCollection->WsEventHandler();

	TRequestStatus& priorityKeyStatus=eventHandler.PriorityKeyStatus();

	// Force HandleAnyCompletedEvents() to be called at least once, even for tiny .OPO files.
	TInt opcodeCount=ENumberOfOpcodesToRunBetweenEventChecks;

	FOREVER
		{
		TUint opcode=IP8();
#ifdef _HISTORY
		if (iStateFlags&KHistoryEnabled)
			{
			if (opcode==0xff)						///// extended opcode
				{
				iHistory.WriteLog(256+IP8());
				iIP.uint8--;
				}
			else
				{
				if (opcode==0x57)
					{
					iHistory.FuncWriteLog(IP8());
					iIP.uint8--;
					}
				else
					iHistory.WriteLog(opcode);
				}
			}
#endif // #define _HISTORY
		// just to test escape
		if (priorityKeyStatus!=KRequestPending)
			{
			User::WaitForRequest(priorityKeyStatus); // returns straightaway as priorityKeyStatus has already completed
			eventHandler.HandlePriorityKeyCompletion();
			}

		if (opcodeCount<ENumberOfOpcodesToRunBetweenEventChecks)
			{
			++opcodeCount;
			}
		else
			{
			opcodeCount=0;
			eventHandler.HandleAnyCompletedEvents(*iCoeEnv, EMaximumNumberOfCompletedEventsToHandlePerEventCheck);
			}

		if (eventHandler.IsSuspended()) // an line function, so very cheap to call
			{
			eventHandler.WaitWhileSuspended(*iCoeEnv);
			opcodeCount=0;
			}

		// Main sequencer - good place for breakpoints
		OpCode::OpCodeTable[opcode](*COplRuntime::iStack,*this,iFrame); // can leave
		}
	}

#if defined(__UIQ__)
void COplRuntime::PrepareToExit()
	{
	CQikAppUi::PrepareToExit();
	}
#endif
#if !defined(__SERIES60__)
#if !defined(__UIQ__)
void COplRuntime::PrepareToExit()
	{
	CEikAppUi::PrepareToExit();
	}

void COplRuntime::ActivateNextAppTopViewL()
	{
	TRuntimeParams params=iCommandLine->RuntimeParams();
	TThreadId thisThread=params.iOwnerThreadId;
	TInt prev=0;
	RWsSession& ws=iEikonEnv->WsSession();
	TInt windowGroup=ws.FindWindowGroupIdentifier(prev,thisThread);
	CApaWindowGroupName* wgName=CApaWindowGroupName::NewLC(ws,windowGroup);
	TVwsViewId viewId(wgName->AppUid(),KNullUid);
	CleanupStack::PopAndDestroy(wgName);
	// Will leave with KErrNotSupported from LOAK21 (since ViewSrv was removed)
	TRAPD(ignore,ActivateViewL(viewId));
	}

COplRuntime::~COplRuntime()
	{
	delete iStartUp;
	delete iOplAlertWin;
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
	delete iClockUpdater;
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
#endif
#endif
TUint32 COplRuntime::IP32()
	{
	return OplUtil::GetLong(iIP.uint32++);
	}

TUint16 COplRuntime::IP16()
	{
	return OplUtil::GetWord(iIP.uint16++);
	}

TReal COplRuntime::IPReal()
	{
// GCC compiler flag
#if defined(__DOUBLE_WORDS_SWAPPED__)
	union
		{
		TUint32 uint32[2];
		TReal flt;
		};

	uint32[1]=OplUtil::GetLong(iIP.uint32++);
	uint32[0]=OplUtil::GetLong(iIP.uint32++);
	return flt;
#else
	return OplUtil::GetFloat(iIP.real++);
#endif
	}

void COplRuntime::DoGoto()
	{
	iIP.uint8+=OplUtil::GetWord(iIP.int16);
	iIP.uint8--;	// avoid -32768-1 overflow
	}

void COplRuntime::SetTrap()
	{
	iTrap=ETrue;
	iErrorValue=KErrNone;
	}

//void COplRuntime::FlushSignals()
//	{
//	iIOSponge->Flush();
//
//	// redraw cursor if enabled
//	Console().DrawCursor(Console().CurrentPos());
//	DrawablesCollection().DrawCursorIfOn();
//
//	ConEnv()->WsSession().Flush();
//	// flush the wserv buffer(removes menu/dialog windows)
//	}

class COplMenuBar : public CEikMenuBar
	{
public:
	void GetHelpContext(TCoeHelpContext& aContext) const;
	};

void COplMenuBar::GetHelpContext(TCoeHelpContext& aContext) const
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
		TCoeContextName context=helpContext->At(EOplHelpMenu);
		TUid helpUid=rt->HelpUid();
		if (context.Length() && helpUid!=KNullUid)
			{
			aContext=TCoeHelpContext(helpUid,context);
			}
		}
	}

/*
 * Menu stuff
 *

Menu's under OPL:

	OpCode::mInit // mINIT
	CloseMenu() Delete any currently existing menu vars.
	PrepareForMenuL() Creates a new COplMenuBar and constructs it.
	Create the Task list pane for Crystal.
	Creates the Profiles pane for Crystal.
	
	OpCode::mCasc // mCASC
	...

	OpCode::mCard // mCARD
	Add the menu item pairs of hotkey, string to the menu items array, using AddMenuItemL()
	Adds the pane title (+icons if mCARDX) using AddMenuTitleL()

	MENU
	Execute the menu by calling DisplayMenuL(0,0) then kick off the active scheduler.
	
	For the task pane, a dummy card with null string is added. The title card id offset
	used is R_CKN_TASKS_MENUPANE_STUB.
*/

void COplRuntime::CloseMenu()
	{
	RemoveFromStack(iEikonEnv->AppUiFactory()->MenuBar());
	DeleteMenu();
	}

void COplRuntime::DeleteMenu()
	{ // Can be safely called from UIKON dtor, as iAppUiFactoryArray killed before CEikonEnv::DestroyEnvironment() called.
	if (iOplMenuBar)
		{
		delete iOplMenuBar;
		iOplMenuBar=NULL;
		}
	if (iMenuTitleArray)
		{
		const TInt count=iMenuTitleArray->Count();
		for (TInt ii=0;ii<count;++ii)
			{
			SOplMenuData& menuData=(*iMenuTitleArray)[ii];
			delete menuData.iBitmap[0];
			delete menuData.iBitmap[1];
			}
		delete iMenuTitleArray;
		iMenuTitleArray = NULL;
		}
	if (iMenuItemsArray)
		{
		iMenuItemsArray->ResetAndDestroy();
		delete iMenuItemsArray;
		iMenuItemsArray = NULL;
		}
	iMenuInitPtr = NULL;
	}

const TInt KMenuDimmed=0x1000;
const TInt KMenuSymbolOn=0x2000;
const TInt KMenuSymbolIndeterminate=0x4000;
const TInt KMenuCheckBox=0x0800;
const TInt KMenuRadioStart=0x0900;
const TInt KMenuRadioMiddle=0x0A00;
const TInt KMenuRadioEnd=0x0B00;
const TInt KMenuRadioMask=KMenuRadioEnd;
const TInt KMenuProfile=0x8000; // Used internally by OPLR only.
const TInt KMenuIrListener=0x9000; 	// Defined here for reference only: used in AppFrame OPX
									// as dummy for CIrListenAppUi::NewL() method.
const TInt KMenuFlagsMask=KMenuRadioMask|KMenuCheckBox|KMenuSymbolIndeterminate|KMenuSymbolOn|KMenuDimmed|KMenuProfile;

_LIT(KDummyMenuItem," ");

void COplRuntime::PrepareForMenuL()
	{
	iOplMenuBar = new(ELeave) COplMenuBar;
	iOplMenuBar->ConstructL(this,0,KMenuBarId);

	// Now add the TaskList and Profile items, in reverse order.
#if !defined(__SERIES60__)
	// Add dummy menu item. This also ensures iMenuItemsArray is populated.
#if !defined(__UIQ__)
	AddMenuItemL(&KDummyMenuItem,KMenuProfile);
#else
	if (!iMenuItemsArray)
		iMenuItemsArray = new(ELeave) CArrayPtrSeg<CEikMenuPaneItem>(1); 
#endif
	// Add the task menu pane.
	if (!iMenuTitleArray)
		iMenuTitleArray = new(ELeave) CArrayFixFlat<SOplMenuData> (16); // in line with UIKON
	SOplMenuData data;
	data.iOffsetIntoItemsArray=iMenuItemsArray->Count();
	data.iMenuTitleData.iMenuPaneResourceId=iMenuTitleArray->Count()+KMenuPaneIdOffset;
	data.iIsCascade=EFalse;
#if !defined(__UIQ__)
	_LIT(KCKONBitmap,"z:\\System\\Data\\ckon.mbm");
	data.iBitmap[0]=iEikonEnv->CreateBitmapL(KCKONBitmap, EMbmCkonTasklist);
	data.iBitmap[1]=iEikonEnv->CreateBitmapL(KCKONBitmap, EMbmCkonTasklism);
#else
	data.iBitmap[0]=NULL;
	data.iBitmap[1]=NULL;
#endif
	iMenuTitleArray->AppendL(data);
#endif
	}

#if !defined(__SERIES60__)
/** Support licensee-specific dynamic menu plugins.
 *
 */
void COplRuntime::AddProfileMenuPaneL()
	{
	__ASSERT_DEBUG(iOplMenuBar,User::Invariant());

	// Add dummy menu item
	AddMenuItemL(&KDummyMenuItem,KMenuProfile);

	// Add the profile menu pane.
	__ASSERT_DEBUG(iMenuTitleArray,User::Invariant());

	SOplMenuData data;
	data.iOffsetIntoItemsArray=iMenuItemsArray->Count();
	data.iMenuTitleData.iMenuPaneResourceId=KMenuPaneIdOffset;
	data.iIsCascade=EFalse;
	_LIT(KProfileBitmap,"z:\\System\\Data\\ProfileAutoMenu.mbm");
	data.iBitmap[0]=iEikonEnv->CreateBitmapL(KProfileBitmap,0);
	data.iBitmap[1]=iEikonEnv->CreateBitmapL(KProfileBitmap,1);
	iMenuTitleArray->AppendL(data);
	}
#else
void COplRuntime::AddProfileMenuPaneL() {}
#endif

/*
 * CascadeId(). Return the id of the menu pane whose text matches the parameter,
 * or zero if not found.
 */
TInt COplRuntime::CascadeId(const TDesC& aMatchItemText)
	{
	_LIT(KCascChar,">");
	if (iMenuTitleArray && aMatchItemText.Right(1)==KCascChar) // Can't be a cascade if no titles yet
		{
		const TInt count=iMenuTitleArray->Count();
		for(TInt index=0;index<count; index++)
			{
			SOplMenuData& oplData=(*iMenuTitleArray)[index];
			if (oplData.iIsCascade && oplData.iMenuTitleData.iText==aMatchItemText)
				return index+KMenuPaneIdOffset;
			}
		}
	return 0;
	}

void COplRuntime::AddMenuTitleL(const TDesC* aTitleText,TBool aIsCasc)
	{
	if (!iOplMenuBar) // mInit not called
		User::Leave(KOplStructure);
	if (!iMenuTitleArray)
		iMenuTitleArray = new(ELeave) CArrayFixFlat<SOplMenuData> (16); // in line with UIKON
	SOplMenuData data;
	data.iOffsetIntoItemsArray = iMenuItemsArray->Count();
	data.iMenuTitleData.iText = *aTitleText;
	data.iMenuTitleData.iMenuPaneResourceId = iMenuTitleArray->Count()+KMenuPaneIdOffset;
	data.iIsCascade=aIsCasc;
	data.iBitmap[0]=NULL;
	data.iBitmap[1]=NULL;
	iMenuTitleArray->AppendL(data);
	}

void COplRuntime::AddMenuBitmapL(CFbsBitmap* aBitmap,CFbsBitmap* aMask)
	{
	if (!iOplMenuBar) // mInit not called
		User::Leave(KOplStructure);
	if (!iMenuTitleArray)
		iMenuTitleArray = new(ELeave) CArrayFixFlat<SOplMenuData> (16); // in line with UIKON
	SOplMenuData data;
	data.iOffsetIntoItemsArray = iMenuItemsArray->Count();
	data.iMenuTitleData.iMenuPaneResourceId = iMenuTitleArray->Count()+KMenuPaneIdOffset;
	data.iIsCascade=EFalse; // Can't cascade with a bitmap
	data.iBitmap[0]=aBitmap;
	data.iBitmap[1]=aMask;
	iMenuTitleArray->AppendL(data);
	}

void COplRuntime::AddMenuItemL(const TDesC* aItemText, TInt aHotKeyCode)
	{
	if (!iMenuItemsArray)
		iMenuItemsArray = new(ELeave) CArrayPtrSeg<CEikMenuPaneItem>(1); 
	CEikMenuPaneItem* item = new(ELeave) CEikMenuPaneItem();
	CleanupStack::PushL(item);
	item->iData.iText = *aItemText;
	item->iData.iCommandId = iMenuItemsArray->Count()+KCommandIdOffset;
	item->iData.iFlags=0;
	if ((item->iData.iCascadeId = CascadeId(*aItemText))!=0)
		{
		item->iData.iText = (*aItemText).Left((*aItemText).Length()-1); // drop trailing ">" from cascade item
		}
	if (aHotKeyCode < 0)
		{
		item->iData.iFlags = EEikMenuItemSeparatorAfter;
		aHotKeyCode=-aHotKeyCode;
		}

#if defined(__SERIES60__)||defined(__UIQ__)
#else
	// Handle profile menu
	if (aHotKeyCode==KMenuProfile)
		item->iData.iCommandId=ECknTasksAutoMenuCommand1;
	else
#endif
		{
		item->iHotKeyCode=TInt16(aHotKeyCode&(~KMenuFlagsMask));
		if (item->iHotKeyCode<1 || (item->iHotKeyCode>32 && !TChar(item->iHotKeyCode).IsAlpha()))
			User::Leave(KErrArgument);
		aHotKeyCode&=KMenuFlagsMask;
		if (aHotKeyCode&KMenuDimmed)
			item->iData.iFlags|=EEikMenuItemDimmed;
		if (aHotKeyCode&KMenuSymbolOn)
			item->iData.iFlags|=EEikMenuItemSymbolOn;
		if (aHotKeyCode&KMenuSymbolIndeterminate)
			item->iData.iFlags|=EEikMenuItemSymbolIndeterminate;
		aHotKeyCode&=(~(KMenuDimmed|KMenuSymbolOn|KMenuSymbolIndeterminate));
		// the rest are mutually exclusive
		if (aHotKeyCode==KMenuCheckBox)
			item->iData.iFlags|=EEikMenuItemCheckBox;
		else if (aHotKeyCode==KMenuRadioStart)
			item->iData.iFlags|=EEikMenuItemRadioStart;
		else if (aHotKeyCode==KMenuRadioMiddle)
			item->iData.iFlags|=EEikMenuItemRadioMiddle;
		else if (aHotKeyCode==KMenuRadioEnd)
			item->iData.iFlags|=EEikMenuItemRadioEnd;
		}

	TInt offset=0;
	if (iMenuTitleArray)
		{	
		offset=(*iMenuTitleArray)[iMenuTitleArray->Count()-1].iOffsetIntoItemsArray;
		iMenuItemsArray->InsertL(offset,item);
		}
	else
		iMenuItemsArray->InsertL(0,item);
	CleanupStack::Pop(); //(menu) item
	}

CEikHotKeyTable* COplRuntime::CreateHotKeyTableLC()
	{ // does it need to be on the cleanup stack
	if (!iMenuItemsArray)
		User::Leave(KOplStructure);
	CEikHotKeyTable* hotKeyTable = new(ELeave) CEikHotKeyTable;
	CleanupStack::PushL(hotKeyTable);
	const TInt count=iMenuItemsArray->Count();
	for(TInt index=0;index<count; index++)
		{
		const CEikMenuPaneItem& item = *(*iMenuItemsArray)[index];
		TChar character(item.iHotKeyCode);
		if (character<=32)
			continue; // <=32 means no Hot-key
		if (character.IsUpper())
			hotKeyTable->AddItemL(item.iData.iCommandId,item.iHotKeyCode+32/*-('A'-1)*/,EModifierCtrl|EModifierShift);
		else
			hotKeyTable->AddItemL(item.iData.iCommandId,item.iHotKeyCode/*-('a'-1)*/,EModifierCtrl);
		}
	return hotKeyTable;
	}

_LIT(KProfileDllName,"ProfileAutoMenu.dll");
_LIT(KSystemLibsPath2,"\\System\\Libs\\Uikon_Init\\");

void COplRuntime::LaunchPopupMenuL(TInt aResourceId,const TPoint& aTargetPos,TPopupTargetPosType aTargetType,const CEikHotKeyTable* aHotKeyTable)
	{ // Called by mPOPUP keyword
	iMenuIsActive = ETrue;
	CEikAppUi::LaunchPopupMenuL(aResourceId,aTargetPos,aTargetType,aHotKeyTable);
	}

void COplRuntime::DisplayMenuL(TInt aTitle, TInt aItem, TInt16* aInitPtr)
	{ // Called by MENU keyword.
	if (!iOplMenuBar)
		User::Leave(KOplStructure);

	// Check that profile plugin is present...
	RFs& fileSystem=ConEnv()->FsSession();
	TFindFile findFile(fileSystem);
	if (findFile.FindByDir(KProfileDllName,KSystemLibsPath)==KErrNone)
		{
		AddProfileMenuPaneL();
		}
	// It has moved on the Nokia 9500 Communicator, so try under System\Libs\Uikon_init\ too ...
	else if (findFile.FindByDir(KProfileDllName,KSystemLibsPath2)==KErrNone)
		{
		AddProfileMenuPaneL();
		}

#if defined(__SERIES60__)
	iGoneThroughMenu=EFalse; // We've not been through the menu once yet.
#endif
	CEikMenuBar* oldMenu=iEikonEnv->AppUiFactory()->SwapMenuBar(iOplMenuBar);
	if (oldMenu!=iOplMenuBar)
		delete oldMenu;
	iOplMenuBar=NULL;
	if (aInitPtr)
		iMenuInitPtr = aInitPtr;

	iEikonEnv->AppUiFactory()->MenuBar()->SetHotKeyTable(CreateHotKeyTableLC());
	CleanupStack::Pop(); // hotkeyTable
	AddToStackL(iEikonEnv->AppUiFactory()->MenuBar(),ECoeStackPriorityMenu);
	iEikonEnv->AppUiFactory()->MenuBar()->TryDisplayMenuBarL();

	// EDNPSPR-4SGNDX: If aNewSelectedTitle is invalid, a EIKCOCTL 25 panic happens.
	// This was deferred for Crystal 6.0 so we work-around it directly here using
	// the proposed fix as posted in the defect.
	const TInt max=iMenuTitleArray->Count()-1;
	iEikonEnv->AppUiFactory()->MenuBar()->MoveHighlightToL(((aTitle>max) ? max : aTitle),aItem);

	iMenuIsActive=ETrue;
	}

#if !defined(__CRYSTAL__)
EXPORT_C
#endif
void COplRuntime::RestoreMenuL(CCoeControl* aCommandWindow,TInt aWindowId,MEikMenuObserver::TMenuType /*aType*/)
	{
	if (aWindowId == KMenuBarId)
		{// restore the menu bar
		CEikMenuBar::CTitleArray* titleArray = new(ELeave) CEikMenuBar::CTitleArray;
		CleanupStack::PushL(titleArray);
		const TInt count=iMenuTitleArray->Count();
		for (TInt ii=0;ii<count;++ii)
			{
			SOplMenuData& oplData=(*iMenuTitleArray)[ii];
			if (oplData.iIsCascade)
				continue;	// only a cascade, so don't add
			CEikMenuBarTitle* title=new(ELeave) CEikMenuBarTitle;
			CleanupStack::PushL(title);
			title->iData=oplData.iMenuTitleData;
			if (oplData.iBitmap[0])
				{
				title->CreateIconL(oplData.iBitmap[0],oplData.iBitmap[1]);
				title->SetBitmapsOwnedExternally(ETrue);
				}
			titleArray->AppendL(title);
			CleanupStack::Pop(); // title
			title=NULL;
			}
		STATIC_CAST(CEikMenuBar*,aCommandWindow)->SetMenuTitleArray(titleArray);		// in this function which
		CleanupStack::Pop();		// titleArray
		}
	else
		{
		// restore a menu tile

		// Prepare the correct menu pane, using entries from the iMenuItemsArray,
		// based on the WindowId parameter.
		// Use the iMenuTitleArray to work out the offsets into the iMenuItemsArray.
		//
		// On Series 60, there's a problem with the WindowId incorrectly picking up
		// a cascade pane, but this only happens for the main menu pane, which is
		// the first to be displayed. So, if this is the first time through, it cannot
		// be a cascade pane, and must be the main pane.

		TInt offset=0;
		TInt start=0;
		if (aWindowId==KMenuPopup)
			{
			offset=iMenuItemsArray->Count();
			}
		else
			{
#if defined(__SERIES60__)
			TInt arrayPos=aWindowId-KMenuPaneIdOffset;
			if (iGoneThroughMenu==EFalse) //not been through yet... 
				{
				iGoneThroughMenu=ETrue;
				// Skip until a non-cascade pane is found,
				// adjusting the start accordingly. 
				//	 start=prev iOffset
				const TInt count=iMenuTitleArray->Count();
				__ASSERT_ALWAYS(arrayPos<count,User::Leave(KOplStructure)); 
				while ((*iMenuTitleArray)[arrayPos].iIsCascade)
					{
					start=(*iMenuTitleArray)[arrayPos].iOffsetIntoItemsArray;
					arrayPos++;
					if (arrayPos>=count)
						User::Leave(KOplStructure); //No valid panes in menu.
					}
				}
			else
				{
				// Maybe this is a real cascade here.
				//so don't skip any...
				const TInt count=iMenuTitleArray->Count();
				__ASSERT_ALWAYS(arrayPos<count,User::Leave(KOplStructure));
				if (arrayPos)
					start=(*iMenuTitleArray)[arrayPos-1].iOffsetIntoItemsArray;
				}
			offset=(*iMenuTitleArray)[arrayPos].iOffsetIntoItemsArray;
#else
			const TInt arrayPos = aWindowId-KMenuPaneIdOffset;
			offset = (*iMenuTitleArray)[arrayPos].iOffsetIntoItemsArray;
			if (arrayPos)
				start=(*iMenuTitleArray)[arrayPos-1].iOffsetIntoItemsArray;
#endif
			}
		STATIC_CAST(CEikMenuPane*,aCommandWindow)->DrawableWindow()->SetOrdinalPosition(0);
		for (TInt ii=start;ii<offset;++ii)
			{
			STATIC_CAST(CEikMenuPane*,aCommandWindow)->AddMenuItemL((*iMenuItemsArray)[ii]->iData);
			}
		}
	}

void COplRuntime::SetUserFlags(TInt aUserFlags)
	{
	if (aUserFlags&KOplState64kLimit)
		{
		iHeap64->Compress();
		if (iHeap64->Size()>K64Kbytes)
			User::LeaveNoMemory();
		}
	if (aUserFlags&KOplStateSendSwitchOnMessage)
		User::LeaveIfError(iCoeEnv->RootWin().EnableOnEvents(EEventControlAlways));
	iUserFlags|=aUserFlags;
	}

void COplRuntime::ClearUserFlags(TInt aUserFlags)
	{
	iUserFlags&=~aUserFlags;
	if (aUserFlags&KOplStateSendSwitchOnMessage)
		iCoeEnv->RootWin().DisableOnEvents();
	}

void COplRuntime::PrepareToStartActiveScheduler()
	{
	iIOCollection->WsEventHandler().PrepareToStartActiveScheduler(*iCoeEnv);
	iCoeEnv->RootWin().CancelTextCursor();
	}

void COplRuntime::NotifyActiveSchedulerStopped()
	{
	iIOCollection->WsEventHandler().NotifyActiveSchedulerStopped();
	Console().DrawCursorIfOn(Console().CurrentPos());
	DrawablesCollection().DrawCursorIfOn();
	iCoeEnv->WsSession().Flush(); // flush the wserv buffer(removes menu/dialog windows)
	}

#if !defined(__CRYSTAL__)
EXPORT_C
#endif
CArrayFix<TCoeHelpContext>* COplRuntime::HelpContextL() const
	{
	// Build the TCoeHelpContext object, and pass it back to Cone.
	CArrayFix<TCoeHelpContext>* viewHelpContext=new(ELeave) CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(viewHelpContext);

	COplRuntime* rt=TheRuntime(); // expensive call, so cache it here.
	TFixedArray<TCoeContextName,KOplLenContextNamesArray>* helpContext=rt->HelpContextNamesArray();
	if (helpContext)
		{
		TCoeContextName context=helpContext->At(EOplHelpView);
		TUid helpUid=rt->HelpUid();
		if (context.Length() && helpUid!=KNullUid)
			{
			viewHelpContext->AppendL(TCoeHelpContext(helpUid,context));
			}
		}
	CleanupStack::Pop(); // viewHelpContext
	return viewHelpContext;
	}

#if defined(__SERIES60__)
//hack hack hack

void CEikMenuBar::CloseState()
	{
	 __ASSERT_DEBUG(EFalse,User::Invariant());
// ????	    StopDisplayingMenuBar();

	}


void CEikMenuBar::ProcessCommandL(TInt aCommandId)
	{
	TheRuntime()->ProcessCommandL(aCommandId);
	}


// object provider - used by cone for menu ???
class TTypeUid::Ptr  CEikMenuBar::MopSupplyObject(class TTypeUid /*thing*/) 
	{
#pragma message("en_main.cpp: MopSupplyObject() should be supplying what exactly?")
	return TTypeUid::Null();
	}

#endif
