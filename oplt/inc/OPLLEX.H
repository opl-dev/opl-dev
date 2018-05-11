// OPLLEX.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#ifndef __OPLLEX_H__
#define __OPLLEX_H__

#include <e32base.h>
#include <texttran.h>
#include <opltdef.h>
#include <opltoken.h>
#include <oplstack.h>

const TUint KOplMaxLogicalDevice=25; // A - Z
const TUint KOplMaxLogicalDeviceOpl1993=3;

/////////////////////////////////////////////////////////////
//
// COplLexerBase
//
/////////////////////////////////////////////////////////////

class COplSymbolTable;
class COplCallSymbol;
class COplLexerBase : public CBase
//
// Base OPL lexer class. Pure virtual.
//
	{
public:
	IMPORT_C virtual TOplToken LexL(); // Get next token
	IMPORT_C void LexL(TOplToken aToken); // Ensure that the next token is what is expected
	IMPORT_C virtual void UnLex(); // Put back last token
	IMPORT_C TChar NextChar();
	IMPORT_C TPtrC MarkedToken();
	IMPORT_C void Mark();
	IMPORT_C virtual void UnGetToMark();
	IMPORT_C virtual void Reset();
	IMPORT_C void SetTarget(TOplTranTarget aTarget);
	IMPORT_C virtual TInt TokenOffset() const;	
	inline void SetError(TTranslateError* anError);
	
	inline TOplToken Token() const;
	inline TOplToken::TType Type() const;	
	
	inline const TDesC& Name() const;

	inline TOplConstant& Constant();
	inline const TOplField& Field() const;
	inline const TOplFunction& Function() const;
	inline const TOplKeyword& Keyword() const;
	inline COplCallSymbol* OpxSymbol() const;
	void SetSymbolTable(COplSymbolTable *aTable);
	void CheckDeviceL(TUint aDeviceNo);
protected:
	COplLexerBase();
	virtual void DecNumberL(TChar start);  // Starts with digit or decimal separator
	void HexNumberL();          // Starts with $ or &
	void LexStringL();          // Starts with " - a string constant
	virtual void AlphaL();      // Starts with IsAplpha.
	void ProcByNameL();         // Starts with @ - @:(str_expr)() prodcedure call
	void CharacterL();          // Everything else;
	void GetRealL();			// Floating point constants
	void ConstantToken();			// Sets conatnt token & type;
	
	// Sorting out alphabetics
	TBool IsReserved(); 
protected:
	// Current line
	TLex iLex;
	TTranslateError* iError;

	COplSymbolTable *iSymbols;

	// Configuration
	TChar iDecimalSeparator;
	TOplTranTarget iTarget;

	// Last token seen
	TOplToken iToken;
	TOplToken::TType iType;

	// Used to mark position so we can have a sniff ahead;
	TLexMark iMark;

	TInt iMaxIdent;
	
	// Value of last token. 
	union
		{
		TUint8 iConstant[sizeof(TOplConstant)];
		TUint8 iFunction[sizeof(TOplFunction)];
		TUint8 iKeyword[sizeof(TOplKeyword)];
		TUint8 iField[sizeof(TOplField)];
		COplCallSymbol *iOpxSymbol;
		};
	TBuf<KOplMaxStringLength> iText;
	};


/////////////////////////////////////////////////////
//
// COplLineLexer - Some parts of the language are different when doing EVAL.
//
/////////////////////////////////////////////////////

class COplLineLexer : public COplLexerBase
//
// Deals with a line at a time.
// All numbers are floating point  ('cos that's the way the calculator used to work)
//
   {
public:
   IMPORT_C static COplLineLexer* NewL();
   inline void SetExpression(const TDesC& anExpr);
protected:
   COplLineLexer();
   virtual void DecNumberL(TChar start);
   virtual void AlphaL();
   };

//////////////////////////////////////////////////////
//
// COplModuleLexer
//
//////////////////////////////////////////////////////
const TInt KOplModLexBufSize=0x200; // Must be greater than max line length
const TInt KOplModLexSourceGran=0x04; // Nexted 4 deep include files

struct TOpenSource
	{
	HBufC *iName;
	MTextSource *iSource;
	TInt iPosition;
	TInt iLineNumber;
	};

class COplModuleLexer : public COplLexerBase
//
// Adds multiple line support for modules & multiple file support for many files.
//
	{
public:
	IMPORT_C static COplModuleLexer* NewL();
	IMPORT_C ~COplModuleLexer();
	IMPORT_C void OpenL(MTextSourceSystem& aSourceSystem, const TDesC& aFileName,TSourceTranslateError& anError);
	IMPORT_C void IncludeL(const TDesC& aFileName);
	IMPORT_C void Reset();
public:
	IMPORT_C virtual TOplToken LexL();
	IMPORT_C virtual void UnLex();
	IMPORT_C virtual void UnGetToMark();
	IMPORT_C void GotoPosition(TInt aPos);
	IMPORT_C virtual TInt TokenOffset() const;

	inline TInt LineNumber() const;
	inline void SetLineNumber(TInt aLineNumber);
	inline const TDesC& FileName() const; // Name of the currently open file
	TBool InRootSource() const;
	
protected:
	COplModuleLexer();
	void ConstructL();
	TBool CloseCurrentSource(TBool aRestoreName); // Flag is set if ErrorLocation file name should be restored
	friend void LexCleanupInclude(TAny *); // Cleanup item which needs to call CloseCurrentSource
private:
	TBool iNewLineNeeded;	// Need to work out where the next line starts
	
	TInt iLineOffset;		// Offset of current line in the read buffer
	TInt iNextLineOffset;	// Offset of the start of the next line in the buffer

	
	MTextSourceSystem *iTextSystem;
	TOpenSource iCurrentSource;
	CTranStackFlat<TOpenSource> *iSources;
	TBuf<KOplModLexBufSize> iReadBuf;
	};

#include <opllex.inl>
#endif
