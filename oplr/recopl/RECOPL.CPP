// RECOPL.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "recopl.h"
#include <apgaplst.h>
#include <apacmdln.h>
#include <s32file.h>
#include "program.h"

//
// Class CApaOplRecognizer
//

CApaFileRecognizerType::TRecognizedType CApaOplRecognizer::DoRecognizeFileL(RFs& /*aFs*/,TUidType aType)
// Recognises interpreter app (eg FLIGHT.APP) and document files (eg TEST1.FLT)
// iRecognizedType is set to ENotRecognised on entering this method
// Sets iRecognizedType to one of: EApp, EDoc, ENotRecognised (and returns this)
//		iRecognizedType==EApp for Interpreted apps
//      iRecognizedType==EDoc for documents and OPOs(these are OPLR.APP documents :-)
// Sets iFileType (eg KUidOplInterpreter) read as 1st word in OPLAPP, OVALAPP or DOC root stream
// Sets iAppUid to the Uid of the APP  (eg. of FLIGHT.APP) - needed by RunL())
//		for interpreted Apps or documents
	{
	if (aType[1]==KUidOplApp)
		iRecognizedType=EProgram;
	else if (aType[1]==KUidOplDoc || iFileType==KUidOplObj)
		{
		iRecognizedType=EDoc;
		}
	return iRecognizedType;
	}

const TUint KSpace=' ';
const TUint KQuote='"';
_LIT(KQuoteDes,"\"");

TThreadId CApaOplRecognizer::RunL(TApaCommand aCommand,const TDesC* aDocFileName,const TDesC8* /*aTailEnd*/) const
// Run the recognized file.
// First finds the interpreter App DLL to be run.
// If it's an app or a program, run the interpreter with the given command line, passing the app
// or program name in the tail end.
// If it's a doc, find the app and run with the "Open" command and the docName (aAppCommand will be NULL)
// iFullFileName is the interpreted program, the interpreted app
// Command-line to interpreter runtime is:
//		<commandByte><docName><space><tailEnd>
// where <tailEnd> is:
//      KOplrCommandLetter<interpretedProgramName>
// KOplrCommandRunNoIPC signifies that program runs from the Shell (and so has no other info in the tailend, like IPC info to OPLEDIT)
// Opl supports other command-letters in the tailend - 'E' if running from the Editor, 'D' if running from the debugger.
//
	{	
	CApaCommandLine* commandLine=CApaCommandLine::NewLC();
	commandLine->SetCommandL(aCommand);
	//
	// Get name of OPL interpreter DLL
	TApaAppEntry appEntry;
	User::LeaveIfError(iFileRecognizer->AppLocator()->GetAppEntryByUid(appEntry,KUidOplInterpreter));
	commandLine->SetLibraryNameL(appEntry.iFullName);
	//
	if (aDocFileName)
		commandLine->SetDocumentNameL(*aDocFileName);
	//
	// now build the tailend - just the app or program name when called from the shell
	TBuf<0x100> tailEnd;
	tailEnd.Append(KOplrCommandRunNoIPC);	// 'R' tells the runtime there is no IPC required on termination
	if (iRecognizedType==EProgram || iAppUid==KUidOplInterpreter) // app or OPO
		tailEnd.Append(*iFullFileName);
	else
		{ // get the app name
		TApaAppEntry appEntry;
		User::LeaveIfError(iFileRecognizer->AppLocator()->GetAppEntryByUid(appEntry,iAppUid));
		tailEnd.Append(appEntry.iFullName);
		}
	if (tailEnd.Locate(KSpace)!=KErrNotFound)
		{ // has spaces so add quotes
		tailEnd.Insert(1,KQuoteDes);
		tailEnd.Append(KQuote);
		}
	TPtrC8 pBuf(REINTERPRET_CAST(const TUint8*,tailEnd.Ptr()),tailEnd.Size());
	commandLine->SetTailEndL(pBuf);
	TThreadId id=AppRunL(*commandLine); 
	CleanupStack::PopAndDestroy(); // commandLine
	return id;
	};

//
// dll stuff...
//

EXPORT_C CApaFileRecognizerType* CreateRecognizer()
// The gate function - ordinal 1
//
	{
	CApaFileRecognizerType* thing=NULL;
	thing=new CApaOplRecognizer();
	return thing;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return KErrNone;
	}
