// OPLTRAN.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// The public interface classes to the OPL translator objects.

#ifndef __OPLTRAN_H__
#define __OPLTRAN_H__

#include <e32base.h>
#include <texttran.h>
#include <opltdef.h>
#include <opltbas.h>
#include <s32mem.h>

class COplModuleParser;
class COplModuleLexer;
class COplModuleBackEnd;


////////////////////////////////////////////////////////////
//
// COplModuleTranslator
//
////////////////////////////////////////////////////////////
class COplModuleTranslator : public COplTranslatorBase
//
// Translator object
//
   {
public:	
	// Construction & tear down
	IMPORT_C ~COplModuleTranslator();
	IMPORT_C static COplModuleTranslator *NewL();
	IMPORT_C static COplModuleTranslator *NewLC();

	// Translator interface - from CTextTranslator
	IMPORT_C void StartTranslateL(MTextSourceSystem& aTextSystem,const TFileName& aSourceName, const TFileName& aTargetName,CTextTranslator::TDebugFlag aDebugFlag,TSourceTranslateError& anError);
	IMPORT_C void StartTranslateL(MTextSourceSystem& aTextSystem,const TFileName& aSourceName,COplModuleBackEnd& aBackEnd,CTextTranslator::TDebugFlag aDebugFlag,TSourceTranslateError& anError);
	IMPORT_C void LocateErrorL(MTextSourceSystem& aTextSystem,const TFileName& aSourceName,const TFileName& aTargetName,CTextTranslator::TDebugFlag aDebugFlag,TSourceTranslateError& anError,TInt aLineNumber);
	IMPORT_C TInt TranslateChunk();
	IMPORT_C void Cancel();  // Abandon the currently active translation

	// Stuff specific to the OPL translator
	IMPORT_C void SetAppCB(MTranAppCB *anAppCB);
	IMPORT_C void SetTarget(TOplTranTarget aTarget);
private:
	COplModuleTranslator() {;}
	void ConstructL();
	void CommonStartL(MTextSourceSystem& aTextSystem, const TFileName& aSourceName,CTextTranslator::TDebugFlag,TSourceTranslateError& anError);
	void CleanupStaleAIFFiles(); // arjenb - cleans up any left over AIF files in C:\System\Temp, called by Cancel()
private:
	MTranAppCB *iAppCB;
	COplModuleLexer *iLexer;
	COplModuleParser *iParser;
	TBool iOwnBackEnd;
	COplModuleBackEnd *iBackEnd;
	TTranslateError* iError;   
    };

////////////////////////////////////////////////////////
//
// COplEvalTranslator
//
/////////////////////////////////////////////////////////
class COplEvalParser;
class COplLineLexer;
class COpl16EvalBackEnd;

class COplEvalTranslator : public CBase
//
// Line translator object for EVAL
//
	{
public:
	IMPORT_C ~COplEvalTranslator();
	IMPORT_C static COplEvalTranslator *NewL();
	IMPORT_C static COplEvalTranslator *NewLC();
	IMPORT_C TInt Translate(const TDesC& anExpression,TTranslateError& anError,CBufBase& aQcode,TInt& aMaxStackDepth);
	IMPORT_C TInt LocateError(const TDesC& anExpression,TTranslateError& anError);
private:
	COplEvalTranslator() {;}
	void ConstructL();
private:
	COplLineLexer *iLexer;
	COplEvalParser *iParser;
	COpl16EvalBackEnd *iBackEnd;
	};

#endif
