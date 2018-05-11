// TEXTED.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __TEXTED_H__
#define __TEXTED_H__

#include <eikappui.h>
#include <eikdoc.h>
#include <opltbas.h>
#include <program.h>
#include <eikgted.h>
#include <eikapp.h>

#if defined(__UIQ__)||defined(__SERIES90__)
#include <eikdialg.h>
#else
#include <sendui.h>
#include <irlistenui.h>
#include <cknprtdg.h>
#endif

#define KUidTextEdApp KUidTextEdApp16

const TUid KUidTextEdApp16={0x10003A63};

const TUid KUidTextedPrefs={0x10000102};
const TUid KUidPrintSetupStream={0x10000105};

const TInt KZoomStateOne=600; // 60%
const TInt KZoomStateTwo=800; // 80%
const TInt KZoomStateThree=TZoomFactor::EZoomOneToOne; // 100%
const TInt KZoomStateFour=1500; // 150%

const TInt KDefaultTabWidthInCharacters=2;

const TInt KExportLineWrap=512;

const TInt KTextEdResourceLen=64;

const TInt KPrintPreviewBandsPerPage=6;

class CGlobalText;
class CPrintSetup;
class CTextPageRegionPrinter;
class CEikonEnv;
class CFindFileByType;
class CCknAppTitle;

class CTextEdGlobalTextEditor : public CEikGlobalTextEditor
	{
public:
	CTextEdGlobalTextEditor(const TBool aFindHack=EFalse);
public:
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	void SetAutoIndent(TBool aAutoIndent);
	void SetupForFind();
	TBool IsAutoIndenting() {return iAutoIndent;};
	void ResetWindow();
public: // from MEikFindAndReplaceDlgObserver
	TBool CallBackFindL(SEdwinFindModel& aModel, TBool aResetStart);
	TBool CallBackReplaceL(SEdwinFindModel& aModel, TBool aResetStart);
public: // from CEikEdwin
	void DrawContents() {CEikEdwin::DrawContents();};
public: // from CCoeControl
	void ReportEventL(MCoeControlObserver::TCoeEvent aEvent) {CEikEdwin::ReportEventL(aEvent);};
private:
	TBool iAutoIndent;
	TBool iFindHack;
	TBool iFindStarted;
	};

class CTextEdDocument: public CEikDocument
	{
public:
	CTextEdDocument(CEikApplication& aApp);
	void ConstructL(CParaFormatLayer* aParaFormat,CCharFormatLayer* aCharFormat,CPrintSetup* aDefaultPrintSetup);
	~CTextEdDocument();
	// APPARC stuff
	void StoreL(CStreamStore& aStore,CStreamDictionary& aStreamDic) const;
	void RestoreL(const CStreamStore& aStore,const CStreamDictionary& aStreamDic);
	void NewDocumentL();
	//
	void DoNewFileL(const TFileName& aFileName);
	void DoOpenFileL(const TFileName& aFileName);
	void DoSaveToCurrentFileL();
	void DoSaveToNewFileL(const TFileName& aFileName);
	void DoFileRevertL(CGlobalText* aTempGlobalText,CPrintSetup* aTempPrintSetup);

	void SetDocChanged(TBool aState);
	MDesCArray* ProcNameArrayL(); // NULL if no procs present
	TInt ProcPosition(TInt aIndex);
	TInt IndexOfPrevious(TInt aPos);
	CPrintSetup* PrintSetup() {return iPrintSetup;};
public:
	CGlobalText* iGlobalText;
	TBool iIsNewDoc;
private: // from CApaDocument
	void DoRestoreL(const CStreamStore& aStore,const CStreamDictionary& aStreamDic,CGlobalText* aGlobalText,CPrintSetup* aPrintSetup);
	CPrintSetup* iPrintSetup;
	CEikAppUi* CreateAppUiL();
	CDesCArraySeg iProcNameArray;
	CArrayFixSeg<TInt> iProcPosArray;
	TBool iProcArrayValid;
	};

typedef COplTranslatorBase* (*TNewOplTranslatorL)();

enum TExportFormatOption
	{
	EExportFormatUnicode,
	EExportFormatUnixASCII,
	EExportFormatDosASCII,

	EExportFormatMaxOptions
	};

class CTxOplRtLogon;
class CTxtedAppUi : public CEikAppUi, public MCoeControlObserver
, public MTextSourceSystem, public MTranAppCB, 
#ifdef USE_PRINTING
public MPrintPreviewDialogObserver, public MPrintDialogObserver, 
#endif
public MFieldNumPagesInfo, public MFieldFileNameInfo
	{
public:
	~CTxtedAppUi();
	void ConstructL();
	TInt OpenSource(TDes& aFileName,MTextSource*& aTextSource);
	void OplTargetIsAppL(TFileName& aName);
	void HandleRuntimeErrorL();
private: // framework
	void HandleCommandL(TInt aCommand);
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
	TBool CheckHotKeyNotDimmedL(TInt aCommand);
	void HandleAttemptDimmedSelectionL(TInt aCommand);
	void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
private:
	void HandleSendAsMenuL(const TInt aCommand);
private:
	//
#ifdef USE_PRINTING
	// from MPrintPreviewDialogObserver
	TBool PageSetupChangedL(CPrintSetup* aPrintSetup,TInt& aNumPagesInDoc);
	TBool RunPrintRangeDialogL(CPrintSetup* aPrintSetup,TInt& aNumPagesInDoc);
	// from MPrintDialogObserver
	TBool RunPaginationDialogL(CPrintSetup& aPrintSetup, TCknPrintParameters& aPrintParameters);
	TBool RunPrintPreviewDialogL(CPrintSetup& aPrintSetup, MPageRegionPrinter& aPrinter);
	//
	void RunPrintDialogL();
	void DoPrintL();
	//
	// paginate/print/preview stuff
	void RunPageSetupDialogL();
	//
	TBool RunPaginationDialogL();
	void RunPreviewDialogL();
	//
	void PrintSetupChanged(CPrintSetup* aPrintSetup);
#endif
private: // help support from CCoeAppUi
	CArrayFix<TCoeHelpContext>* HelpContextL() const;
private:
	void SetTitleBarVisibilityL(const TBool aVisibility);
	void SetFullScreenVisibilityL(const TBool aVisibility);
	void SetHorizScrollBarVisibilityL(const TBool aVisibility);
	void CreateEdwinL();
	void CmdNewFileL();
	void CreateFileL(const TDesC& aFileName);
	void CmdOpenFileL();
	void OpenFileL(const TDesC& aFileName);
	void CmdFileSaveL();
	void CmdFileSaveAsL();
	void CmdFileRevertL();
	void ImportTextL();
	void DoAsciiImportL(const RFile& aFile, const TInt aSize);
	void ReplaceWithLineDelimiter(TDes& aBuffer, const TDesC& aTextToReplace, const TDesC& aReplaceWith) const;
	void ExportTextL();
	void DoAsciiExportL(const TFileName& aFileName);
	void DeleteL();
	void AddToHomeL(); // aka 'Add to favourites'
	void DoLogL();
	void DoCreateEdwinL(CGlobalText* aText);
	void TabSettingL();
	void SetTabWidthL(TInt aTabWidthInChars);
	void SetPreferencesL();
	void CmdScrollBarL(TInt aCommand);
	void ZoomL(TInt aCommand);
	void InitTitleBar();
	void InitFullScreenL();
	void SetTitleBarFileNameL(const TDesC& aFileName);
	void GetLastClosedFileL(TDes& aFileName);
	void SaveLastClosedFileL(const TDesC& aFileName);
	void RestorePrefsL();
	void DoRestorePrefsL(TInt& aScrollBarState,TInt& aFullScreenState,TInt& aTitleBarState,TInt& aZoomFactor);
	TInt StorePrefs(); // returns any error
	void DoStorePrefsL();
	void CmdGotoL(TInt aCommand);
	void CmdShowLastError();
	TInt GetTranslatorErrorResourceNumber(TInt aError);
	void GetTranslateErrorText(TDes& aDes,TInt aError);
	TBool OpenIncludeFileWithErrorL(const TDesC& aFilename);
	TInt UpdateFieldFileName(TPtr& aValueText) const;
	TInt UpdateFieldNumPages() const;
	TInt ScrollBarState();
	TFileName CurrentFilePath() const;
	void StorePortionL(CStreamStore& aStore,CStreamDictionary& aStreamDic,const TDesC& aPortion) const;
private:
	TBool DebuggerNotPresent();
	void DoOpenSourceL(TDes& aFilename,MTextSource*& aTextSource); //opl stuff
	void DoTranslateL(CTextTranslator::TDebugFlag aDebugFlag);
	void DoLocateRuntimeErrorL(CTextTranslator::TDebugFlag aDebugFlag,TSourceTranslateError& anErr,TInt aProcLine);
	void DoRunL(const TDesC& aFileName);
	void DoRunFileL();
	void LoadTranslatorL();
	void SendL(const TInt aCommandId) const;
	COplTranslatorBase* NewTranslatorLC();
	RLibrary iTranslatorDll;
	TNewOplTranslatorL iNewTranFuncL;
	CFindFileByType* iFindFile;
private:
	void ResetViewL();
	void HandleModelChangeL();
	TBool ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& /*aTail*/);
private:
	friend class CTextEdDocument;
	CTextEdGlobalTextEditor* iGlobalEd;
	TZoomFactor iAppZoom;
	TExportFormatOption iExportFormatOption;
	TBool iIsMonoSpaced;
	TInt32 iTabWidthInChars;
	TNonPrintingCharVisibility iNonPrintingCharVisibility;
#if defined(USE_IRLISTEN)
	CIrListenAppUi* iIrListenAppUi;
#endif
#if defined(USE_PRINTING)
private:	 //print stuff
	CPrintSetup* iPrintSetup;
	CTextPageRegionPrinter* iPrint;
	CArrayFix<TInt>* iPageTable;
	TBool iPaginateDocumentChanged;
	TCknPrintParameters iPrintParameters;
#endif
private:	// OPL stuff
	CTxOplRtLogon* iOplRtLogon;
	TPckgBuf<TRuntimeResBuf> iRuntimeResultPckg;
	TBuf<128> iLastErrorBuf;
	HBufC* iTranFileName;
	TInt iOplrResourceFile;
#if defined(USE_SENDAS)
	CSendAppUi* iSendAppUi;
#endif
#if !defined(__UIQ__)
	CCknAppTitle* iTitleBar;
#endif
	TBool iIsTitleBarVisible;
	TBool iIsFullScreenVisible; // cba and status pane.
	TBool iIsHorizScrollBarVisible;
	SEdwinFindModel iFindModel;
	};

//
// class CTxOplRtLogon
//
class CEikBusyMsgWin;
class CTxOplRtLogon : public CActive
	{
public:
	CTxOplRtLogon(CTxtedAppUi& aAppUi);
	~CTxOplRtLogon();
	void ConstructL();
	void Request(const TThreadId& aId);
private:
	void DoCancel();
	void RunL();
private:
	RThread iOplRtThread;
	CTxtedAppUi& iAppUi;
	CEikBusyMsgWin* iBusyMsg;
	HBufC* iBusyText;
	};

//
// class CTextEdTabDialog
//
class CTextEdTabDialog : public CEikDialog
	{
public:
	CTextEdTabDialog(TInt* aTabWidthInCharacters,TBool* aAutoIndent);
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
	// from CCoeControl
	void GetHelpContext(TCoeHelpContext& aContext) const;
private:
	TInt* iTabWidthInCharacters;
	TBool* iAutoIndent;
	enum TChoiceListItems {EPrefNo, EPrefYes};
	};

//
// class CTextEdSetPreferencesDialog
//
class CTextEdSetPreferencesDialog : public CEikDialog
	{
public:
	CTextEdSetPreferencesDialog(TBool* aIsMonoSpaced,TNonPrintingCharVisibility* aNonPrintingCharVisibility,TExportFormatOption& aExportFormatOption);
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
	// from CCoeControl
	void GetHelpContext(TCoeHelpContext& aContext) const;
private:
	TBool* iIsWrapped;
	TBool* iIsMonoSpaced;
	TNonPrintingCharVisibility* iNonPrintingCharVisibility;
	enum TChoiceListItems {EPrefNo, EPrefYes};
	TExportFormatOption& iExportFormatOption;
	};

//
// class CTextEdTranslateDialog
//
class CTextEdTranslateDialog : public CEikDialog
	{
public:
	CTextEdTranslateDialog(CTextTranslator* aTranslator,TInt& aReturnValue,TInt aTitleResource=0);
	~CTextEdTranslateDialog();
	void PreLayoutDynInitL();
	void PostLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
	static TInt TranslateChunk(TAny* aPtr);
	// from CCoeControl
	void GetHelpContext(TCoeHelpContext& aContext) const;
private:
	TInt DoTranslateChunk();
	CTextTranslator* iTranslator;
	TInt& iReturnValue;
	CIdle* iIdle;
	TInt iTitleResource;
	};

//
// class CTextEdProcListDialog
//
class CTextEdProcListDialog : public CEikDialog
	{
public:
	CTextEdProcListDialog(MDesCArray* aArray,TInt &selectedItem);
	// from CCoeControl
	void GetHelpContext(TCoeHelpContext& aContext) const;
private:
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aButtonId);
	MDesCArray* iArray;
	TInt &iSelectedItem;
	};

//
// CTextEdApplication
//
class CTextEdApplication : public CEikApplication
	{
public:
	CCharFormatLayer* iCharFormat;
	CParaFormatLayer* iParaFormat;
	~CTextEdApplication();
private: // from CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};

#endif
