// RECOPL.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <apmrec.h>
#include <apmstd.h>
#include <f32file.h>
#include "recopl.h"
//#include <apgaplst.h>
//#include <apacmdln.h>
#include <s32file.h>
#include "program.h"

#ifdef _USE_OLD_RECOGNISER_

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

#endif

const TInt KMimeOplRecognizerValue=0x10000148;
const TUid KUidMimeOplRecognizer={KMimeOplRecognizerValue};
const TInt KOplNumMimeTypes=1;
//_LIT8(KOpoMimeType,"x-epoc/x-app268459310"); //0x10005D2E
_LIT8(KOpoMimeType,"opl/opo"); //0x10005D2E



CApaOplRecognizer::CApaOplRecognizer()
	:CApaDataRecognizerType(KUidMimeOplRecognizer,CApaDataRecognizerType::ENormal)
	// All these mime types have reasonable recognition
	{
	iCountDataTypes=KOplNumMimeTypes;
	}

TUint CApaOplRecognizer::PreferredBufSize()
	{
	// no buffer recognition yet
	return 0;
	}

TDataType CApaOplRecognizer::SupportedDataTypeL(TInt aIndex) const
	{
	__ASSERT_DEBUG(aIndex>=0 && aIndex<KOplNumMimeTypes,User::Invariant());
	switch (aIndex)
		{
//	case 0:
//		return TDataType(KHtmlMimeType);
	default:
		return TDataType(KOpoMimeType);
		}
	}

void CApaOplRecognizer::DoRecognizeL(const TDesC& aName, const TDesC8& /*aBuffer*/)
	{
	TParse parse;
	parse.Set(aName,NULL,NULL);
	TPtrC ext=parse.Ext();
	_LIT(KDotOpo,".opo");
	if (ext.CompareF(KDotOpo)==0)
		{
		iDataType=TDataType(KOpoMimeType);
		iConfidence=EHigh; // Shouldn't be anything else on the system that picks these up.
		}
//	else if (ext.CompareF(KDotGif)==0)
//		{
//		iDataType=TDataType(KGifMimeType);
//		iConfidence=EProbable;
//		}
	}

//
// dll stuff...
//

EXPORT_C CApaDataRecognizerType* CreateRecognizer()
// The gate function - ordinal 1
	{
	CApaDataRecognizerType* thing=new CApaOplRecognizer();
	return thing; // NULL if new failed
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return KErrNone;
	}

//End of file