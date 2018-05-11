// OT_TRAN.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// The top level interface to the OPL translator
//

#include "ot_std.h"
#include "opl16.h"

LOCAL_C TInt processTranError(TInt ret,TTranslateError& anError)
//
// checks the return value from a translator. Converts translator errors
// into KErrGeneral
//
	{
	
	anError.SetError(ret);
	return (ret>0 ? KErrGeneral : ret);
	}


// First export - the interface required by Program editors
EXPORT_C COplTranslatorBase *NewTranslatorL()
	{
	return COplModuleTranslator::NewL();
	}

//////////////////////////////////////////////////////////////
//
// COplModuleTranslator
//
/////////////////////////////////////////////////////////////

EXPORT_C COplModuleTranslator::~COplModuleTranslator()
//
// Delete all the components of the translator
//
   {

   delete iLexer;
   delete iParser;
   if (iOwnBackEnd)
	   delete iBackEnd;
   }


EXPORT_C COplModuleTranslator* COplModuleTranslator::NewL()
//
// Creates a new OPL translator and leaves if there is an error
//
   {

   COplModuleTranslator *pT=NewLC();
   CleanupStack::Pop();
   return pT;
   }

EXPORT_C COplModuleTranslator* COplModuleTranslator::NewLC()
//
// Creates a new opl translator leaving it on the cleanup stack
//
   {

   COplModuleTranslator *pT=new(ELeave) COplModuleTranslator();
   CleanupStack::PushL(pT);
   pT->ConstructL();
   return pT;
   }

EXPORT_C void COplModuleTranslator::SetAppCB(MTranAppCB *anAppCB)
	{
	iAppCB=anAppCB;
	}


LOCAL_C void DoCancel(TAny *This)
//
//
//
   {

   ((COplModuleTranslator *)This)->Cancel();
   }


EXPORT_C void COplModuleTranslator::StartTranslateL(
      MTextSourceSystem& aTextSystem,
      const TFileName& aSourceName,
      COplModuleBackEnd& aBackEnd,
	  CTextTranslator::TDebugFlag aDebugFlag,
      TSourceTranslateError& anError)
//
// Initializes everything for translating, using a passed back end to do the output.
//
   {


	// In case anything goes wrong
	CleanupStack::PushL(TCleanupItem(DoCancel,this));

	
	// record the fact that we're just borrowing the back end.
	iBackEnd=&aBackEnd;
	iOwnBackEnd=EFalse;

	CommonStartL(aTextSystem, aSourceName,aDebugFlag,anError);
	}

EXPORT_C void COplModuleTranslator::StartTranslateL(
      MTextSourceSystem& aTextSystem,
      const TFileName& aSourceName,
      const TFileName& aTargetName,
	  CTextTranslator::TDebugFlag aDebugFlag,
      TSourceTranslateError& anError)
//
// Initializes everything for translating
//
   {


   // In case anything goes wrong
   CleanupStack::PushL(TCleanupItem(DoCancel,this));

   // Make sure that we have got a translator BackEnd
   switch (iParser->Target())
      {
      case EOplTargetOpl1993:
		  iBackEnd=COpl16Opl1993BackEnd::NewL(aTargetName,iAppCB);
		  break;
      case EOplTargetER1:
		  iBackEnd=COpl16Opler1BackEnd::NewL(aTargetName,iAppCB);
		  break;
	  default:
		  Panic(EOpltUnknownTranTarget);
		}
	iOwnBackEnd=ETrue;

	CommonStartL(aTextSystem,aSourceName,aDebugFlag,anError);
	}


EXPORT_C void COplModuleTranslator::LocateErrorL(
		MTextSourceSystem& aTextSystem,
		const TFileName& aSourceName,
		const TFileName& aTargetName,
		CTextTranslator::TDebugFlag aDebugFlag,
		TSourceTranslateError& anError,
		TInt aLineNumber)
	{

	StartTranslateL(aTextSystem,aSourceName,aTargetName,aDebugFlag,anError);
	iParser->SetErrorLocation(aLineNumber, anError.Position());
	}


void COplModuleTranslator::CommonStartL(MTextSourceSystem& aTextSystem,
										const TFileName& aSourceName,
										CTextTranslator::TDebugFlag aDebugFlag,
										TSourceTranslateError& anError)
	{
	// Open up the source file
	iLexer->OpenL(aTextSystem,aSourceName,anError);

	// Set things up for error tracking
	iBackEnd->StartL(aSourceName,aDebugFlag,anError);

	// And finally start the parser
	iParser->Start(*iLexer,anError,*iBackEnd);

	// Where to put the error value
    iError=&anError;
	
	
	CleanupStack::Pop(); // Cancel code
	}

EXPORT_C TInt COplModuleTranslator::TranslateChunk()
//
// Next step in the module translation
//
	{

	TRAPD(ret,iParser->ParseChunkL());
	ret=processTranError(ret,*(TTranslateError*)iError);
	if (ret!=KErrNone)
		Cancel();
	
	return ret;
	}
   
EXPORT_C void COplModuleTranslator::Cancel()
//
// Resets everything
//
   {

   iParser->Reset();
   iLexer->Reset();
   if (iOwnBackEnd)
	   delete iBackEnd;
   iBackEnd=NULL;
   iError=NULL;

   CleanupStaleAIFFiles();
   }

EXPORT_C void COplModuleTranslator::SetTarget(TOplTranTarget aTarget)
//
// Sets the target for subsequent tarnslations.
//
   {

   iParser->SetTarget(aTarget); // This will PANIC is this is an inappriate time to SetTarget
   iLexer->SetTarget(aTarget);
   }

void COplModuleTranslator::ConstructL()
//
// Constructs all the bits
//
   {

   iLexer=COplModuleLexer::NewL();
   iParser=COplModuleParser::NewL();
   }


void COplModuleTranslator::CleanupStaleAIFFiles()
// 
// Deletes left over temporary AIF files.
// Fixes [911620] Stale .AIF files left in \System\Temp
//
	{
	RFs fsSession;
	User::LeaveIfError(fsSession.Connect()); 

	// Create file management object
	CFileMan* fileMan = CFileMan::NewL(fsSession);
	CleanupStack::PushL(fileMan); 

	// Delete stale AIF files in C:\System\Temp
	_LIT(KStaleAIF, "C:\\SYSTEM\\TEMP\\TMP*.AIF");
	fileMan->Delete(KStaleAIF);

	// Delete file management object
	CleanupStack::PopAndDestroy();

	fsSession.Close();
	}

//////////////////////////////////////////////////////////////////
//
// COplEvalTranslator
//
//////////////////////////////////////////////////////////////////
EXPORT_C COplEvalTranslator::~COplEvalTranslator()
//
// deletes all the constituent parts
//	
	{

	delete iParser;
	delete iBackEnd;
	delete iLexer;
	}

EXPORT_C COplEvalTranslator *COplEvalTranslator::NewL()
//
// Safely constructs one
//
	{	
	COplEvalTranslator *pT=NewLC();
	CleanupStack::Pop();
	return pT;
	}

EXPORT_C COplEvalTranslator *COplEvalTranslator::NewLC()
//
// constructs one leaving it on the cleaup stack
//
	{
	COplEvalTranslator *pT=new(ELeave) COplEvalTranslator;
	CleanupStack::PushL(pT);
	pT->ConstructL();
	return pT;
	}

EXPORT_C TInt COplEvalTranslator::Translate(const TDesC& anExpression,
						 TTranslateError& anError,
						 CBufBase& aQcode,TInt &aMaxStackDepth)
//
// Translates the expression
//
	{

	iLexer->SetExpression(anExpression);
	iLexer->SetError(&anError);

	iBackEnd->SetBuffers(anError,aQcode,aMaxStackDepth);

	TRAPD(ret,iParser->ParseExpressionL(*iLexer,anError,*iBackEnd));

	return processTranError(ret,anError);
	}

EXPORT_C TInt COplEvalTranslator::LocateError(const TDesC& /* anExpression*/, TTranslateError& anError)
//
// Tries to locate the positon of an error - FOR NOW 
//
	{

	anError.SetPosition(0);
	return KErrNone;
	}

void COplEvalTranslator::ConstructL()
//
// Gets all the constituent parts
//
	{
	
	iLexer=COplLineLexer::NewL();
	iParser=COplEvalParser::NewL();
	iBackEnd=COpl16EvalBackEnd::NewL();
	}



   
