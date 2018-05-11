// EN_APARC.CPP
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#include <e32std.h>
#include <eikdoc.h>
#include <opcodes.h>
#include <oplcmd.h>
#if defined(__SERIES60__)
#include <coeutils.h>
#endif

void FuncOpCode::CmdStr(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	TInt cmdNum=aStack.PopInt16();
	TFileName res;
	res=KNullDesC;
	COplCommandLine& cmd=aRuntime.CommandLine();
	switch (cmdNum)
		{
	case 1:
		res=cmd.ModuleName();
		break;
	case 2:
		res=cmd.DocumentName();
		break;
	case 3:
		{
		res.Append(cmd.Command());
		break;
		}
	default:
		User::Leave(KOplErrNotSupported);
		}
	aStack.Push(res);
	}

//
// Class COplCommandLine
//
COplCommandLine::~COplCommandLine()
	{
	delete iModuleName;
	delete iDocumentName;
	}

#define KDelim 32

void COplCommandLine::CrackCommandLineL(const TDesC8& aCmd)
	// Just the tailend
	{
	if (!aCmd.Length())
		{
		iTailEndCommand=KOplrCommandRunNoIPC;
		iModuleName=iDocumentName->AllocL();
		return;
		}
	iTailEndCommand=aCmd[0];
	TPtrC8 rest(aCmd.Mid(sizeof(TText)));
	TInt offsetDelim;
	HBufC8* moduleName8;
	if (rest[0]=='"')
		{ // quote delimited file name
		rest.Set(rest.Mid(sizeof(TText)));
		offsetDelim=rest.Locate('"');
		if (offsetDelim<=0) // must have some name
			User::Leave(KErrArgument);
		moduleName8=rest.Left(offsetDelim).AllocL();
		offsetDelim+=sizeof(TText);
		}
	else
		{ // unquoted
		offsetDelim=rest.Locate(KDelim);
		if (offsetDelim<0)
			{
			moduleName8=rest.AllocL();
			offsetDelim=rest.Length();
			}
		else if (offsetDelim>0)
			{
			moduleName8=rest.Left(offsetDelim).AllocL();
			}
		else
			{
			User::Leave(KErrArgument);
			moduleName8=_L8(" ").AllocL(); // keep compiler happy
			}
		}
	TInt len=moduleName8->Length();
	TInt halfLen=len>>1;
	iModuleName=HBufC::NewL(halfLen);
	iModuleName->Des().Append((TUint16*)moduleName8->Ptr(),halfLen);
	delete moduleName8;
	TPckgBuf<TRuntimeParams> parPckg;
	if (rest.Length()>offsetDelim)
		{ // is some more
		rest.Set(rest.Mid(offsetDelim));
		if (rest[0]!=KDelim)
			User::Leave(KErrArgument);
		TPtr8 rest8((TUint8*)rest.Ptr(),rest.Length()<<1);
		rest8.SetLength(rest.Length()<<1);
		TPtrC8 rest1=rest8;
		rest1.Set(rest8.Mid(2,sizeof(TRuntimeParams))); // !! Debug: why is OPLEDIT passing more?
		parPckg.Copy(rest1);
		}
	else
		parPckg.FillZ();

	iRuntimeParams=parPckg();
	// RDebug::Print(_L("Cracked moduleName=%S\n"),&iModuleName->Des());
	}

void COplCommandLine::SaveCommandLineParametersL(TApaCommand aCommand,TFileName& aDocumentName, const TDesC8& aTail)
	{
	_LIT(KOplParameters,"OPL parameters");
	switch (aCommand)
		{
	case EApaCommandOpen:
		iCommand=KApaCommandLetterOpen;
		break;
	case EApaCommandCreate:
		iCommand=KApaCommandLetterCreate;
		break;
	case EApaCommandRun:
		iCommand=KApaCommandLetterRun;
		break;
	case EApaCommandBackground:
		iCommand=KApaCommandLetterBackground;
		break;
	case EApaCommandViewActivate:
		iCommand=KApaCommandLetterViewActivate;
		break;
	case EApaCommandRunWithoutViews:
		iCommand=KApaCommandLetterRunWithoutViews;
		break;
	default:
		User::Panic(KOplParameters,1);
		}
	iDocumentName=aDocumentName.AllocL();
	CrackCommandLineL(aTail);
	}

// Command-line interface with CEikAppUi
#if defined(__SERIES60__)
EXPORT_C TBool COplRuntime::ProcessCommandParametersL(TApaCommand aCommand, TFileName& aDocumentName, const TDesC8& aTail)
	{
/*
 * Problems with the Series 60 version:
 * The menu app uses EikDll::StartAppL() which expects .app files to be executable apps.
 * Other file launchers use apparc's ApaLsSession.StartApp() which uses recogs :-(
 * So we have to jump thru hoops to:
 *  a. Provide a C++ wrapper for each opl app. The main content of the OPL app now lives
 *     in a .opo file in \System\Apps\<MyAppName>\
 *     For example:
 *       C:\System\Apps\HelloWorld\HelloWorld.app -- the C++ wrapper.
 *       C:\System\Apps\HelloWorld\HelloWorld.opo -- the main OPL opo file.
 *  b. Crack the app name to find the .opo file name.
 *  c. Set the doc name to the .opo file for loading.
 */

	// Either someone has launched the hidden OPL.APP,
	// or more likely, we're running on Series 60, so try to find the .OPO file.

	if (aDocumentName.Length()==0)
		{
		TParse opoFile;
		_LIT(KOpo,".opo");
		const TFileName wrapper(Application()->AppFullName());
		opoFile.Set(KOpo,&wrapper,NULL);

// If we're running debug, assume that's the emulator,
// so for ease of testing, change Z: to C: and smile.
// If you're easily offended by hacky code, look away now...
#ifdef _DEBUG
		_LIT(KZDrive,"Z:");
		if (opoFile.Drive().CompareF(KZDrive)==0)
			{
			TParse cDrive;
			_LIT(KCDrive,"C:");
			cDrive.Set(KCDrive,&opoFile.FullName(),NULL);
			opoFile.Set(cDrive.FullName(),NULL,NULL);
			}
#endif
		aDocumentName=opoFile.FullName();

// Another major hack :-(
// Workround for my inability to get opl.app KAppIsHidden attribute to work.
// If the target is opl.opo, and if it doesn't exist, quietly exit.
// This stops the bug #729865 "opl.app reports an error when run"

		_LIT(KOplOpoName,"opl.opo");
		if (opoFile.NameAndExt().CompareF(KOplOpoName)==0)
			{
			if (!ConeUtils::FileExists(opoFile.FullName()))
				{
				// Terminate here - bail out!
				TheRuntime()->PrepareToExit();
				delete TheRuntime(); // closes files etc.
				User::Exit(0);
				}
			}
		}
	iCommandLine->SaveCommandLineParametersL(aCommand,aDocumentName,aTail);

	TInt winGroup=iCommandLine->RuntimeParams().iOwnerWindowGroup;
	if (winGroup>=0)
		ConEnv()->RootWin().SetOwningWindowGroup(winGroup);

	aDocumentName=KNullDesC; // Prevent further processing of document by APPARC
	return EFalse;
	}
#else
#if defined(__UIQ__)
EXPORT_C
#endif
TBool COplRuntime::ProcessCommandParametersL(TApaCommand aCommand, TFileName& aDocumentName, const TDesC8& aTail)
	{
	iCommandLine->SaveCommandLineParametersL(aCommand,aDocumentName,aTail);

	TInt winGroup=iCommandLine->RuntimeParams().iOwnerWindowGroup;
	if (winGroup>=0)
		ConEnv()->RootWin().SetOwningWindowGroup(winGroup);

	aDocumentName=KNullDesC; // Prevent further processing of document by APPARC
	return EFalse;
	}
#endif

void OpCode::SetDoc(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	TFileName name(aStack.PopString());
	aRuntime.SetCurrentDocumentName(name); 
	}
		
void FuncOpCode::GetDocStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	aStack.Push(aRuntime.CurrentDocumentName());
	}

void OpCode::Lock(CStack& /* aStack */, COplRuntime& aRuntime, CFrame* /*  */)
	{
	STATIC_CAST(CEikonEnv*,aRuntime.ConEnv())->SetBusy(aRuntime.IP8());
	}

void FuncOpCode::WCmd(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
	{
	CWsEventHandler& eventHandler=aRuntime.IOCollection().WsEventHandler();
	aStack.Push(eventHandler.Command());
	eventHandler.ClearCommand();
	}