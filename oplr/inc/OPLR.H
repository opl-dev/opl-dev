// OPLR.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __OPLR_H__
#define __OPLR_H__

#include <bldvariant.hrh>
#include <oplflg.h>
#include <oplstack.h>
#include <oplpan.h>
#include <oplcmd.h>

#if defined(__UIQ__)
#include <qikappui.h>
#endif

#if defined(__SERIES60__)
#include <aknenv.h>
#include <aknappui.h>
#include <e32base.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <eikdialg.h>
#else
#include <coecntrl.h>
#include <eikenv.h>
#include <eikappui.h>
#include <eikhkeyt.h>
#include <eikmenup.h>
#include <eikmenub.h>
#include <eikproc.h>
#endif

#include "globtbl.h"
#include "iomain.h"
#include "console.h"
#include <oplerr.h>

#if defined(_HISTORY)
#include "hist.h"
#endif
const TInt KLeftSideField		= -1; // for ASSIGN and PushLeftSide...
const TInt KLeftSideNonField	= 0;

// runtime state flags
const TInt EDirOutstanding = 1;
const TInt KErrBufferCurrent = 2;
const TInt KHistoryEnabled = 4;
const TInt KInCallback = 8;

const TInt K64Kbytes=0x10000;
// 
// used by SETFLAGS params
const TInt KOplState64kLimit = 0x001;
const TInt KOplStateAutoCompact = 0x002;
const TInt KOplStateTwoDigitExp=0x004;
const TInt KOplStateOPL1993MenuCancelBehaviour=0x008;
const TInt KOplStateOutputAsciiTextFiles=0x016;
const TInt KOplStateUserFlagsNonInstantMask = KOplState64kLimit|KOplStateAutoCompact|KOplStateTwoDigitExp|KOplStateOPL1993MenuCancelBehaviour|KOplStateOutputAsciiTextFiles;

const TInt KOplStateInstantMask = 0xffff0000; // these bits require immediate action
const TInt KOplStateSendSwitchOnMessage=0x010000;
// edit the following if any added/removed to/from list above
const TInt KOplStateUserFlagsInstantMask = KOplStateSendSwitchOnMessage;
const TInt KOplStateUserFlagsMask =	KOplStateUserFlagsNonInstantMask|
									KOplStateUserFlagsInstantMask;
// Dynamic flags for setting history on and off
const TInt KSwitchHistoryOn = 10000;
const TInt KSwitchHistoryOff = 10001;

// Context-sensitive help control types
enum TOplHelpControl {EOplHelpView,EOplHelpDialog,EOplHelpMenu};
const TInt KOplLenContextNamesArray=3; // View, Dialog, Menu.
const TInt KOplHelpMaxControl=EOplHelpMenu; // always the largest value.
const TInt KOplHelpMaxContextNameLen=30; // 30 chars maximum name length
typedef union 
		{
		TUint8* uint8;
		TUint16* uint16;
		TInt16* int16;
		TUint32* uint32;
		TReal* real;
		OplString string;
		} TInstructionPtr;

typedef TBuf<256> TModuleName;

class COplRuntime;

#ifdef __EXE__
COplRuntime* TheRuntime();
void SetTheRuntime(COplRuntime* aRuntime);
#else
inline COplRuntime* TheRuntime() { return((COplRuntime *)Dll::Tls()); }
inline void SetTheRuntime(COplRuntime *theRuntime) { Dll::SetTls(theRuntime); }
#endif

struct SOplMenuData
	{
	CEikMenuBarTitle::SData iMenuTitleData;
	TInt iOffsetIntoItemsArray;
	TBool iIsCascade;
	CFbsBitmap* iBitmap[2];
	};

enum TOplBool {KOplTrue=-1,KOplFalse=0};

/*
 * Some forward declarations...
 */
class CModulesCollection;

class CDrawablesCollection;	//forward decl

class COplDbManager;

class OplAPI;

class CDebuggerAPI;
class CDebuggerBase;

class COplDialog;

class COplCommandLine;

class COplAlertWin;

#ifdef NO_SEQUENCER
class COplTest;
#endif

class COplClockUpdater;


#if defined(__SERIES60__)||defined(__UIQ__)

// Start the AO from a dialog.
// New in Symbian OS 6.1

class COplRuntimeStartUpDialog;

class COplStartUpAO : public CActive
	{
public:
	COplStartUpAO(COplRuntime* aRuntime);
	~COplStartUpAO();
	void StartItUpL();
private:
	virtual void DoCancel();
	virtual void RunL();
	TInt RunError(TInt aError);

	COplRuntime* iRuntime;
	};
#else
class COplStartUp : public CActive
	{
public:
	COplStartUp(COplRuntime* aRuntime);
	~COplStartUp();
private:
	virtual void DoCancel();
	virtual void RunL();
	COplRuntime* iRuntime;
	};
#endif

#if defined(__SERIES60__)
class COplRuntime : public CAknAppUi, public MCoeFepAwareTextEditor
#elif defined(__UIQ__)
class COplRuntime:public CQikAppUi, public MCoeControlObserver
#else
class COplRuntime : public CEikAppUi, public MCoeControlObserver
#endif
	{
public:
#if !defined(NO_SEQUENCER)
	void ExecuteD(); //called by startup active object
#if defined(__SERIES60__)||defined(__UIQ__)
	void ExecuteCheck(); //called by startup active object
#endif
	TInt Execute();
	void InitializeL(const TInt aFlags, const TModuleName aModuleName);
#else
	void RunTests(TInt aTestNo=0);
#endif
	IMPORT_C COplRuntime();
#if defined(__SERIES60__)||defined(__UIQ__)
	IMPORT_C void ConstructL();
	IMPORT_C ~COplRuntime();
	IMPORT_C virtual TBool ProcessCommandParametersL(TApaCommand aCommand, TFileName& aDocumentName, const TDesC8& aTail);
	IMPORT_C void HandleForegroundEventL(TBool aForeground);
	IMPORT_C virtual void HandleWsEventL(const TWsEvent& aEvent,CCoeControl* aDestination);
	IMPORT_C virtual void HandleSystemEventL(const TWsEvent& aEvent);
	IMPORT_C virtual MCoeMessageObserver::TMessageResponse HandleMessageL(TUint32 aClientHandleOfTargetWindowGroup, TUid aMessageUid, const TDesC8& aMessageParameters);

	virtual void StartFepInlineEditL(const TDesC& aInitialInlineText, TInt aPositionOfInsertionPointInInlineText, TBool aCursorVisibility, const MFormCustomDraw* aCustomDraw, MFepInlineTextFormatRetriever& aInlineTextFormatRetriever, MFepPointerEventHandlerDuringInlineEdit& aPointerEventHandlerDuringInlineEdit);
	virtual void UpdateFepInlineTextL(const TDesC& aNewInlineText, TInt aPositionOfInsertionPointInInlineText);
	virtual void SetInlineEditingCursorVisibilityL(TBool aCursorVisibility);
	IMPORT_C void CommitFepInlineEditL(CCoeEnv& aConeEnvironment);
	virtual void CancelFepInlineEdit();
	// with regard to the behaviour of the following functions (except GetScreenCoordinatesForFepL), note that when inline editing, the contents of the editor will be such that the text constituting the selection immediately prior to inline editing will be replaced by the inline text (CancelFepInlineEdit reinstates the previous selection)
	virtual TInt DocumentLengthForFep() const;
	virtual TInt DocumentMaximumLengthForFep() const;
	virtual void SetCursorSelectionForFepL(const TCursorSelection& aCursorSelection);
	virtual void GetCursorSelectionForFep(TCursorSelection& aCursorSelection) const;
	virtual void GetEditorContentForFep(TDes& aEditorContent, TInt aDocumentPosition, TInt aLengthToRetrieve) const;
	virtual void GetFormatForFep(TCharFormat& aFormat, TInt aDocumentPosition) const;
	virtual void GetScreenCoordinatesForFepL(TPoint& aLeftSideOfBaseLine, TInt& aHeight, TInt& aAscent, TInt aDocumentPosition) const;
	IMPORT_C MCoeFepAwareTextEditor_Extension1* Extension1();
	virtual void DoCommitFepInlineEditL();
#else
	virtual void ConstructL();
	~COplRuntime();
	virtual TBool ProcessCommandParametersL(TApaCommand aCommand, TFileName& aDocumentName, const TDesC8& aTail);
	void HandleForegroundEventL(TBool aForeground);
	virtual void HandleWsEventL(const TWsEvent& aEvent,CCoeControl* aDestination);
	virtual void HandleSystemEventL(const TWsEvent& aEvent);
	virtual MCoeMessageObserver::TMessageResponse HandleMessageL(TUint32 aClientHandleOfTargetWindowGroup, TUid aMessageUid, const TDesC8& aMessageParameters);
#endif
	COplCommandLine& CommandLine() {return *iCommandLine;}
	IMPORT_C TDesC& CurrentDocumentName();
	void SetCurrentDocumentName(const TDesC& aName);
	CCoeEnv* ConEnv() {return iCoeEnv;}
	void SetFrame(CFrame* aFrame);
	void ClearTrap() {iTrap=EFalse;}
	void SetTrap();
	TBool GetTrap() {return iTrap;}
	void ClearError() {iErrorValue=KErrNone;}
	void SetError(TInt aError) {iErrorValue=aError;}
	void DoGoto();
	TUint8* IP() {return iIP.uint8;}
	void SetIP(TUint8* aNewIP) {iIP.uint8=aNewIP;}
	void IncIP(TInt aAdd=1) {iIP.uint8+=aAdd;}
	TUint8 IP8() {return *iIP.uint8++;}	
	TUint16 IP16();
	TUint32 IP32();
	TReal IPReal();
	CFrame* Frame() {return iFrame;}
	CDrawablesCollection& DrawablesCollection() {return *iDrawablesCollection;}
	TInt ErrorHandler();
	TPtrC ErrBuf() const {return TPtrC(iErrBuf);}
	void SetHeap64(RHeap* aHeap64) {iHeap64 = aHeap64;}
	RHeap& Heap64() {return *iHeap64;};	
	void SequencerL();
	COplConsole& Console() {return *iConsole;};
	CStack& Stack() const {return *iStack;}
	CGlobalsTbl& GlobalTbl() const {return *iGlobTbl;};
	CIOCollection& IOCollection() {return *iIOCollection;};
	TInt ErrorValue() {return iErrorValue;};
	CModulesCollection& ModulesCollection() const {return *iModulesCollection;};
	TInt64& Seed() {return iSeed;}
	COplDbManager* DbManager() {return iDbManager;}
	void GetErrStr(TDes& aDes,TInt16 aError);
	void SetErrorBuffer(const TDesC* aDes); //aDes points to the extended error text, otherwise NULL
	TInt AppendErrorText(const TDesC& aDes);
	void PrepareForMenuL();
	void CloseMenu();
	void DeleteMenu();
	void AddMenuTitleL(const TDesC* aTitleText,TBool aIsCascade);
	void AddMenuBitmapL(CFbsBitmap* aBitmap,CFbsBitmap* aMask);
	TInt CascadeId(const TDesC& aTitleText);
	void AddMenuItemL(const TDesC* aItemText,TInt aHotKeyCode);
	void DisplayMenuL(TInt aTitle,TInt aItem,TInt16* aInitPtr=NULL);
	void LaunchPopupMenuL(TInt aResourceId,const TPoint& aTargetPos,TPopupTargetPosType aTargetType,const CEikHotKeyTable* aHotKeyTable = NULL);
#if defined(__SERIES60__)||defined(__UIQ__)||defined(__SERIES90__)
#define USE_IMPORT IMPORT_C
#else
#define USE_IMPORT // Not used.
	void HandleSideBarMenuL(TInt aResourceId,const TPoint& aPos,TInt aModifiers,const CEikHotKeyTable* aTable);
#endif
	USE_IMPORT void RestoreMenuL(CCoeControl* aCommandWindow,TInt aWindowId,MEikMenuObserver::TMenuType aType);
	USE_IMPORT void ProcessCommandL(TInt aCommandId);
	void HandleControlEventL(CCoeControl* aControl,MCoeControlObserver::TCoeEvent aEvent);
	void SetStateFlags(TInt aFlags) {iStateFlags|=aFlags;};
	void ClearStateFlags(TInt aFlags) {iStateFlags&=~aFlags;};
	void SetUserFlags(TInt aUserFlags);
	void ClearUserFlags(TInt aUserFlags);
	TInt UserFlags() {return iUserFlags;};
	TInt StateFlags() {return iStateFlags;};
	RDir& Dir() {return iDir;};
	TFileName& DirPath() {return iDirPath;};
	void SetDirPath(const TDesC& aPath) {iDirPath=aPath;};
	OplAPI* TheOplAPI() {return iOplAPI;};
	CDebuggerAPI* iDebuggerAPI;
	CDebuggerBase* Debugger() {return iDebuggerBase;};
	COplDialog* Dialog() {return iOplDialog;};
	void SetDialog(COplDialog* aDialog) {iOplDialog=aDialog;};
	TInt AlertWin(const TDesC& aLine1,const TDesC& aLine2,const TDesC& aBut1,const TDesC& aBut2,const TDesC& aBut3);
	CEikHotKeyTable* CreateHotKeyTableLC();
#if !defined(__CRYSTAL__)
	IMPORT_C CArrayFix<TCoeHelpContext>* HelpContextL() const; // overridden from CCoeAppUi
#else
	CArrayFix<TCoeHelpContext>* HelpContextL() const; // overridden from CCoeAppUi
#endif
#if !defined(__SERIES60__)
	void PrepareToExit();
	void ActivateNextAppTopViewL();
#endif
	TFixedArray<TCoeContextName,KOplLenContextNamesArray>* HelpContextNamesArray() {return iHelpContextNamesArray;};
	void PrepareToStartActiveScheduler();
	void NotifyActiveSchedulerStopped();
	TUid HelpUid() {return iHelpUid;};
	void SetHelpUid(TUid aUid) {iHelpUid=aUid;};
	void CancelMenu();
private:
#ifdef _HISTORY
	RHist iHistory;
#endif
private:
#if !defined(__SERIES60__)
	void CreateHotKeyTableL();
#endif
	void ClearError(CFrame* aFrame);
	void AddProfileMenuPaneL();
private:
#if !defined(NO_SEQUENCER)
	TInt iFlags;
#else 
	COplTest* iTest;
#endif
	OplAPI* iOplAPI;	// For use by OPXs
	TInt iStateFlags;	// EDirOutstanding etc
	TInt iUserFlags;
	TFileName iDirPath;
	RDir iDir;
	CFrame* iFrame;
	TInstructionPtr iIP;
	// Variable heap
	RHeap* iHeap64;
	TUint8* iBase64k;
	CStack* iStack;
	COplConsole* iConsole;
	CModulesCollection* iModulesCollection;
	CDrawablesCollection* iDrawablesCollection;
	CGlobalsTbl* iGlobTbl;
	TInt64 iSeed;
	CIOCollection* iIOCollection;
	COplDbManager* iDbManager;
	// Error data
	TBool iTrap;
	TInt iErrorValue;
	TBuf<256> iErrBuf;
	TRuntimeResBuf iRuntimeResBuf;
	// For menu
	CArrayFixFlat<SOplMenuData>* iMenuTitleArray;
	CArrayPtrSeg<CEikMenuPaneItem>* iMenuItemsArray;

	CEikMenuBar* iOplMenuBar;
	TInt16* iMenuInitPtr;
	TBool iMenuIsActive;
	// For dialogs
	COplDialog* iOplDialog;
#if defined(__SERIES60__)||defined(__UIQ__)
	COplRuntimeStartUpDialog* iAppStartUpDialog;
	COplStartUpAO* iStartUpAO;
#else
	// Startup active object
	COplStartUp* iStartUp;
#endif
	// Debugger
	CDebuggerBase* iDebuggerBase;
	RLibrary iDebuggerLib;
	// Commandline info
	COplCommandLine* iCommandLine;
	// Document info
	HBufC* iCurrentDocumentName;
	COplAlertWin* iOplAlertWin;
	TInt iResourceFile; // Oplr.rsc
	COplClockUpdater* iClockUpdater;
	TFixedArray<TCoeContextName,KOplLenContextNamesArray>* iHelpContextNamesArray;
	TUid iHelpUid;
#if defined(__SERIES60__)||defined(__UIQ__)
private: // From MEikMenuObserver
	IMPORT_C void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
private: // From CEikAppUi
	IMPORT_C void HandleCommandL(TInt aCommand);
	IMPORT_C virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	TBool iGoneThroughMenu;
#endif
	};

//
// Paths used by OPL
//
_LIT(KOpxPath,"\\System\\OPX\\");
_LIT(KSystemLibsPath,"\\System\\Libs\\");

//
// Uids for OPL
//
const TUid KUidOplInterpreter=	{0x10005D2E}; // Opl.app

const TUid KUidOplObj=			{0x100055C0};
const TUid KUidOplApp=			{0x100055C1};
const TUid KUidOplDoc=			{0x100055C2};

const TUid KUidExternalOplFile= {0x1000008A};

#endif
