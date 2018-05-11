// OT_LEX.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

// Lexer for OPL
//
// Classes: COplLexerBase - The base class for OPL lexers
//          COplCalcLex - The lexer for the OPL based calculator   
//          COplModuleLexer - The lexer for the OPL module translator

#include "ot_std.h"
#include <f32file.h> // for TParse

////////////////////////////////////////////////////////
//
// TOplField
//
///////////////////////////////////////////////////////
TOplField::TOplField(TDevice aDevice, const TDesC& aName)
	: iDevice(aDevice), iName(aName)
//
// Sets up the members.
//
	{
	
	__ASSERT_ALWAYS(aDevice!=EBadDevice,Panic(EOpltBadLogicalDevice));
	}

/////////////////////////////////////////////////////////////////////////
//
// COplLexerBase - The base class for OPL lexers
//
/////////////////////////////////////////////////////////////////////////

EXPORT_C TOplToken COplLexerBase::LexL()
//
// Returns the next token from the input - the heart of it 
//
   {

   iToken=TOplToken::EBadToken;
   iType=TOplToken::EBadType;

   iLex.SkipSpaceAndMark(); // mark the start of the current token
   if (iError!=NULL) // Keep track of error position
	   iError->SetPosition(TokenOffset());

   if (iLex.Eos())
      iToken=TOplToken::EEos;
   else
      {
      TChar next=iLex.Peek();
      if (next.IsDigit() || next==iDecimalSeparator)
         DecNumberL(next);
      else if (next=='&' || next=='$')
         HexNumberL();
      else if (next=='\"')
         LexStringL();
      else if (next.IsAlpha() || next=='_')
         AlphaL();
      else if (next=='@')
         ProcByNameL();
      else
         CharacterL();
      }
   return iToken;
   }

EXPORT_C void COplLexerBase::LexL(TOplToken aToken)
//
// Does a lex ensureing that the next token is as expected
//
	{
	if (LexL()!=aToken)
		SyntaxErrorL();
	}

EXPORT_C void COplLexerBase::UnLex()
//
// Puts back the last token returned.
//
   {

   iLex.UnGetToMark();
   }


EXPORT_C TChar COplLexerBase::NextChar()
//
// Needed by the % operator
//
	{
	if (iLex.Eos())
		User::Leave(EErrBadCharacter);
	return iLex.Get();
	}

EXPORT_C TPtrC COplLexerBase::MarkedToken()
//
// Returns the iLex marked token for cases where you 
// want the raw unadulterated input.
//
	{
	return iLex.MarkedToken();
	}

EXPORT_C void COplLexerBase::Mark()
//
// Marks the current position. Used to sniff a couple of 
// tokens ahead when the parser isn't sure what to do.
//
	{
	iLex.Mark(iMark);
	}

EXPORT_C void COplLexerBase::UnGetToMark()
//
// Restores where we were in the line
//
	{
	iLex.UnGetToMark(iMark);
	}


EXPORT_C void COplLexerBase::Reset()
//
//
//
	{
	}


EXPORT_C void  COplLexerBase::SetTarget(TOplTranTarget aTarget)
//
//
//
	{
	iTarget=aTarget;
	iMaxIdent=(aTarget==EOplTargetER1 ? KOplMaxIdentifierLength : KOplOpl1993MaxIdentLength);
	}

void COplLexerBase::SetSymbolTable(COplSymbolTable *aTable)
//
// Sets the symbol table. NOTE this is deliberately not inline & not EXPORT_C.
// It's public:, but only within the DLL.
//
	{
	
	iSymbols=aTable;
	}
						

#pragma warning(disable : 4705) // "Code has no effect". Due to some un-initialized members 
								// See "strnge warning from Compiler - Graham Darnell
							    // In programing (Harcourt) notes database
COplLexerBase::COplLexerBase()
	: iDecimalSeparator('.'),iTarget(KOplDefaultTarget),iMaxIdent(KDefaultIdentifierLength)
//
// Set default target and decimal separator
// 
	{
	}
#pragma warning(default : 4705)


void COplLexerBase::GetRealL()
//
// Where all numbers end up - the default for a decimal number
//
	{
	TReal64 realVal;
	if (iLex.Val(realVal,iDecimalSeparator)!=KErrNone)
		User::Leave(EErrBadNumber);
	new(iConstant) TOplConstant(realVal);
	ConstantToken();
	}

void COplLexerBase::ConstantToken()
//
// Sets
//
	{
	iType=Constant().Type();
	iToken=TOplToken::EConstant;
	}

	
void COplLexerBase::DecNumberL(TChar aStart)
//
// Gets a number starting with a decimal separator or a digit
// Always try for the smallest possible size.
//
   {

	TInt err=KErrGeneral;
	if (aStart!=iDecimalSeparator) // Can try for an integer
		{
	  
		TInt16 wordVal;
		err=iLex.Val(wordVal);
		new (iConstant) TOplConstant(wordVal);
		if (err!=KErrNone)
			{
			TInt32 longVal;
			err=iLex.Val(longVal);
			new (iConstant) TOplConstant(longVal);
			}

		ConstantToken(); // Can do no harm if there was an error 
	
		if (err==KErrNone) // Looked like an integer, but ...
			{
			TCharUC next=iLex.Peek(); // .. check if it's really a TReal
			if (next=='.' || next=='E') // e.g. 1.2 or 1E2
				{
				err=KErrGeneral; // force using float
				iLex.UnGetToMark();
				}
			}
		}

	if (err!=KErrNone) // Not an integer - or looks like a TReal
		GetRealL();
	}

void COplLexerBase::HexNumberL()
//
// Gets something that starts with either $ or &
//
	{

	TInt err;
	if (iLex.Get()=='&')
		{
		TUint32 longVal;
		err=iLex.Val(longVal,EHex);
		new (iConstant) TOplConstant(TInt32(longVal));
		}
	else
		{
		TUint16 wordVal;
		err=iLex.Val(wordVal,EHex);
		new (iConstant) TOplConstant(TInt16(wordVal));
		}					
	if (err!=KErrNone)
		User::Leave(EErrBadNumber);	
	ConstantToken();
	}


void COplLexerBase::LexStringL()
//
// Deals with a string constant
//
   {

   iText.Zero();
   TChar next=iLex.Get();
   FOREVER
      {
      if (iLex.Eos())
         User::Leave(EErrMissingQuote);
      next=iLex.Get();
      if (next=='\"') // end or embedded quote
         {
         if (iLex.Eos()) // Must have been the end
		 	break;
         if (iLex.Get()!='\"') // also the end
            {
            iLex.UnGet(); // so put it back
            break;
            }
         }
      if (iText.Length()==KOplMaxStringLength)
         User::Leave(EErrBadStringLength);
      iText.Append(next);
      }
   new (iConstant) TOplConstant(iText);
   ConstantToken();
   }

//
// Used to map suffices onto tokens and token types.
//
struct TOplSuffixMap
   {
   TUint32 iSuffices;
   TOplToken::TType iType;
   TOplToken::TValue iToken;
   };

LOCAL_D const TOplSuffixMap suffixMap[]=
   {
   {0,            TOplToken::EReal,TOplToken::ESimple},
   {':',          TOplToken::EReal,TOplToken::EProcId},
   {'%',          TOplToken::EWord,TOplToken::ESimple},
   {('%'<<16)+':',TOplToken::EWord,TOplToken::EProcId},
   {'&',          TOplToken::ELong,TOplToken::ESimple},
   {('&'<<16)+':',TOplToken::ELong,TOplToken::EProcId},
   {'$',          TOplToken::EString,TOplToken::ESimple},
   {('$'<<16)+':',TOplToken::EString,TOplToken::EProcId},
   {(':'<<16)+':',TOplToken::EWord,TOplToken::ELabel},
   {'.',          TOplToken::EWord,TOplToken::EField}
   };


void COplLexerBase::AlphaL()
//
// Something that starts with an IsAlpha()
// FOR NOW - needs fixing so error occurs in the right place ??
// 
	{

	TOplField::TDevice logicalDevice=TOplField::EBadDevice;
	FOREVER // Loop - done if doing logical device/field e.g. A.B$ is done as 
		{	  // two trips round the loop 
		// Use up all alpha-numerics
		TLexMark start;
		iLex.Mark(start);
		FOREVER
			{
			TChar peek=iLex.Peek();
			if (!peek.IsAlphaDigit() && peek!='_')
				break;
			iLex.Inc();
			}

		// Gather up the suffices
		TLexMark suffices;
		iLex.Mark(suffices);
		TUint32 suffVal=0;
		FOREVER
			{
			TChar next=iLex.Peek();
			if (next!='%' && next!='&' && next!='$' && next != ':' && next!='.')
				break;
			suffVal=(suffVal<<16)+next;
			iLex.Inc();
			}

		// Check that we've got no more than two, then include them in the token
		TUint suffixCount=iLex.TokenLength(suffices);
		if (suffixCount>2)
			User::Leave(EErrBadSuffices);

		// Quick check to make sure that the token will fit in the string buffer
		// Just in case we make the line length much bigger than max length
		// for a string constant. Assumes that string length
		// is bigger than ident length. Can't just check against KOplMaxIdentLen
		// as some function names are bigger than short identifiers 

		if (iLex.TokenLength(start)>KOplMaxStringLength) 
			User::Leave(EErrIdentifierTooLong);

		// Store the token in upper case.
		iText.CopyUC(iLex.MarkedToken(start));

		// Now - and only now, have a quick sniff and see if it's a reserved
		// word like PROC.
		if (!IsReserved())
			{
			
			// Check the suffices to work out what it is we might have.
			const TOplSuffixMap *pMap=suffixMap;
			const TOplSuffixMap *pEnd=pMap+sizeof(suffixMap)/sizeof(TOplSuffixMap);
			while (pMap<pEnd)
				{
				if (pMap->iSuffices==suffVal)
					break;
				pMap++;
				}
			if (pMap==pEnd) // It's not a legal combination of suffices.
				User::Leave(EErrBadSuffices);

			// Looks good so far;
			iToken=pMap->iToken;
			iType=pMap->iType;

			// Now drop any ':'s as they have served their purpose.
			TUint dropLen=0;
			if (iToken==TOplToken::EProcId)
				dropLen=1;
			else if (iToken==TOplToken::ELabel)
				dropLen=2;
			iText.SetLength(iText.Length()-dropLen); //?? No better way to do this iText::DecLen(2);

			// Finally we can check the length
			if (iText.Length()>iMaxIdent)
				User::Leave(EErrIdentifierTooLong);


			if (iToken==TOplToken::ESimple) // Check now for Arrays and memories
				{
				if (iLex.Peek()=='(')
					{
					iLex.Inc();
					iToken=TOplToken::EArray;
					}
				}
			}
		
		// Things that don't look like a logical device
		if (iToken!=TOplToken::EField)
			{
			if (logicalDevice!=TOplField::EBadDevice) // Already seen one so we're now looking for the field name
				{
				if (iToken!=TOplToken::ESimple) // Field names must be simple variables
					User::Leave(EErrBadFieldName);
				new (&iField[0]) TOplField(logicalDevice,iText);
				iToken=TOplToken::EField;
				}
			if (iSymbols!=NULL) // Might be an Opx Call or Constant
				{
				COplSymbol *pSym=iSymbols->Find(iText,iToken);
				if (pSym!=NULL)
					{
					if (pSym->Class()==COplSymbol::EOpxFunction) // Looks that war
						{
						iToken=TOplToken::EOpxFunction;
						iOpxSymbol=STATIC_CAST(COplCallSymbol *,pSym);
						}
					else if (pSym->Class()==COplSymbol::EConstant) // Actually a constant
						{
						new (iConstant) TOplConstant(STATIC_CAST(COplConstantSymbol *,pSym)->Value());
						ConstantToken();
						}
					}
				}
			break; // All done
			}

		// Just seen what purports to be a logical device i.e A.
		if (logicalDevice!=TOplField::EBadDevice) // For the second time e.g. A.B.
			User::Leave(EErrBadFieldName);

		// Logical devices must be a single letter (A-D on OPL1993, A-Z on Opler1)
		if (iText.Length()!=2)
			User::Leave(EErrBadLogicalDevice);
		CheckDeviceL(iText[0]-'A');
		logicalDevice=TOplField::TDevice(iText[0]-'A');

		// ..it must be followed by an identifier
		if (!iLex.Peek().IsAlpha())
			User::Leave(EErrBadFieldName);
		}
	
	}

void COplLexerBase::ProcByNameL()
//
// Checks out the proc by name @ construct
//
   {

   iLex.Get(); // absorb that @
   if (iLex.Eos())
      SyntaxErrorL();
   switch (iLex.Get())
      {
      case '%':
         iType=TOplToken::EWord;
         break;
      case '$':
         iType=TOplToken::EString;
         break;
      case '&':
         iType=TOplToken::ELong;
         break;
      default:
         iType=TOplToken::EReal;
         iLex.UnGet();
         break;
      }
   if (iLex.Eos() || iLex.Get()!='(')
      SyntaxErrorL();
   iToken=TOplToken::EProcByName;
   }

//
// Couple of structures used for character checking
// 

struct TOplCharMap
   {
   TUint iChar;
   TOplToken::TValue iToken;
   };

struct TOplDoubleMap
   {
   TOplToken::TValue iOldToken;
   TUint iChar;
   TOplToken::TValue iNewToken;
   };
   
LOCAL_D const TOplCharMap characterMaps[]=
   {
   {'<',TOplToken::ELessThan},
   {'>',TOplToken::EGreaterThan},
   {'=',TOplToken::EEqual},
   {'+',TOplToken::EPlus},
   {'-',TOplToken::EMinus},
   {'*',TOplToken::EMultiply},
   {'/',TOplToken::EDivide},
   {'(',TOplToken::EOpenBracket},
   {')',TOplToken::ECloseBracket},
   {'%',TOplToken::EPerc},
   {',',TOplToken::EComma},
   {';',TOplToken::ESemiColon},
   {':',TOplToken::EColon},
   {'#',TOplToken::EHash},
   {0,TOplToken::EStartOfExpression} // Null list terminator
   };

LOCAL_D const TOplDoubleMap doubleMaps[]=
   {
   {TOplToken::ELessThan,'=',TOplToken::ELessThanEq},
   {TOplToken::ELessThan,'>',TOplToken::ENotEqual},
   {TOplToken::EGreaterThan,'=',TOplToken::EGreaterThanEq},
   {TOplToken::EMultiply,'*',TOplToken::EPower},
   {TOplToken::EStartOfExpression,0,TOplToken::EStartOfExpression} // null list terminator
   };

void COplLexerBase::CharacterL()
//
// The operators && punctuation - but generally all the left over stuff.
// These are just checked in lookup tables.
//
   {

	TChar next=iLex.Get();
	// Look up list of acceptable characters
	const TOplCharMap *charMap=characterMaps;
	while (charMap->iChar!=0)
		{
		if (charMap->iChar==next)
			break;
		charMap++;
		}

	// If it's not in that list, it's no good.
	if (charMap->iChar==0)
		User::Leave(EErrBadCharacter);

	// OK so far
	iToken=charMap->iToken;

	// Now check for double character pairs e.g. '<='
	next=iLex.Peek();
	const TOplDoubleMap *dblMap=doubleMaps;
	while (dblMap->iChar!=0)
		{
		if (iToken==dblMap->iOldToken && dblMap->iChar==next)
			{
			iLex.Get();
			iToken=dblMap->iNewToken;
				break;
			}
		dblMap++;
		}
	}

EXPORT_C TInt COplLexerBase::TokenOffset() const
	{
	return iLex.MarkedOffset();
	}

void COplLexerBase::CheckDeviceL(TUint aDevice)
	{
	
	TUint maxDevice=KOplMaxLogicalDevice;
	if (iTarget==EOplTargetOpl1993)
		maxDevice=KOplMaxLogicalDeviceOpl1993;
	if (aDevice>maxDevice)
		User::Leave(EErrBadLogicalDevice);
	}


/////////////////////////////////////////////////////////////////////////
//
// COplCalcLexer - Lexical scanner for EVAL
//
/////////////////////////////////////////////////////////////////////////

EXPORT_C COplLineLexer* COplLineLexer::NewL()
//
// Forces the lexer to be allocated from the heap. 
//
	{
	return new(ELeave) COplLineLexer();
	}


COplLineLexer::COplLineLexer()
	{
	TLocale iLocale;
	iDecimalSeparator=iLocale.DecimalSeparator();	
	}

void COplLineLexer::DecNumberL(TChar)
//
// Lexes out a number - i.e. something starting with either 
// a digit or a decimal separator.
// All calculator decimal numbers are TReal's
// and use the locale's decimal separator.
//
	{

	GetRealL();
	}

void COplLineLexer::AlphaL()
//
// Filters out all the identifiers
//
	{
	COplLexerBase::AlphaL(); // Something to work with
	
	// Only interested in filtering identifiers and keywords - e.g. AND, OR & NOT & the functions go straight through
/*	if ((iToken.Class()==TOplToken::EOperator) && (iToken.Class()!=TOplToken::ECall))
		User::Leave(EErrBadIdentifier);
*/	}


/////////////////////////////////////////////////////////////////////////
//
// COplModuleLexer - Lexical scanner for OPL modules
//
/////////////////////////////////////////////////////////////////////////

EXPORT_C COplModuleLexer *COplModuleLexer::NewL()
	{

	COplModuleLexer *pL=new(ELeave) COplModuleLexer();
	CleanupStack::PushL(pL);
	pL->ConstructL();
	CleanupStack::Pop();
	return pL;
	}

EXPORT_C COplModuleLexer::~COplModuleLexer()
//
// Destroy any include list
//
	{
	Reset();
	delete iSources;
	}


EXPORT_C void COplModuleLexer::OpenL(MTextSourceSystem& aSourceSystem,
									 const TDesC& aFileName,
									 TSourceTranslateError& anError)
//
// Resets things and opens up the root file.
//
	{
	
	__ASSERT_ALWAYS(iTextSystem==NULL,Panic(EOpltLexerReentrantOpen));
	iTextSystem=&aSourceSystem;
	iError=&anError;
	IncludeL(aFileName);
	}

void LexCleanupInclude(TAny *lexer)
	{
	((COplModuleLexer *)lexer)->CloseCurrentSource(ETrue);
	}

EXPORT_C void COplModuleLexer::IncludeL(const TDesC& aFileName)
//
// Opens a new file as the source
//
	{
	// Fix OPL HA27: Parse filename to be opened with the current file name
//	TParse includeName;
//	User::LeaveIfError(includeName.Set(aFileName,iCurrentSource.iName,NULL));
	TFileName includeName=aFileName;

	iCurrentSource.iPosition=TokenOffset(); // Record where we need to resume
	iSources->PushL(&iCurrentSource);
	Mem::FillZ(&iCurrentSource,sizeof(iCurrentSource));
	CleanupStack::PushL(TCleanupItem(LexCleanupInclude,this));
	
	User::LeaveIfError(iTextSystem->OpenSource(includeName,iCurrentSource.iSource));
	// Copy the file name
	iCurrentSource.iName=includeName/*.FullName()*/.AllocL();
	// Open the new source
	//hp:was 
	//User::LeaveIfError(iTextSystem->OpenSource(aFileName,iCurrentSource.iSource));
//	User::LeaveIfError(iTextSystem->OpenSource(includeName.FullName(),iCurrentSource.iSource));
	// Seed the read buffer
	User::LeaveIfError(iCurrentSource.iSource->Read(iReadBuf));

	iNewLineNeeded=ETrue;			// So we start by finding where the next line is
	iCurrentSource.iLineNumber=-1;	// And that will be line 0 in the file.
	iNextLineOffset=0;				// And it starts at the beginning of the buffer
	((TSourceTranslateError *)iError)->SetSource(*iCurrentSource.iName);    // Keep track of where any error occurs
	CleanupStack::Pop();
	}

EXPORT_C void COplModuleLexer::Reset()
//
// Closes all the open file(s)
//
	{
	if (iSources)
		{
		if (iSources->Depth()) // Got as far as opening 
			while (!CloseCurrentSource(EFalse))
				; // Close all the open files
		iSources->Reset();
		}
	iTextSystem=NULL;
	}

EXPORT_C TOplToken COplModuleLexer::LexL()
//
// Puts a preamble on Lex(), to deal with the new line stuff
//
 	{
	if (iNewLineNeeded) // finished with the last line
		{
		// Find out where the next line runs to in the buffer
		TPtrC remainder=iReadBuf.Mid(iNextLineOffset);
		TInt lineEnd=remainder.Locate(TChar(KTextTranLineDelimiter)); 
		
		// If the end of the next line isn't in the buffer - read in the whole of the line
		if (lineEnd==KErrNotFound)
			{
			// Cannot disturb iLex or iFilePos or iLine pos until we're safe
			// so that any errors here get reported in the right place
			TInt bufStart;
			TInt ret;
			FOREVER // Grab the next chunk from the file
				{							
				bufStart=iCurrentSource.iPosition+iNextLineOffset;		// Where the next line starts in the file
				ret=iCurrentSource.iSource->Read(iReadBuf,bufStart);
				if (ret!=KErrEof || InRootSource())
					break;

				CloseCurrentSource(ETrue);
				} 

			if (ret==KErrEof)
				return iToken=TOplToken::EEof;

			User::LeaveIfError(ret);
			lineEnd=iReadBuf.Locate(TChar(KTextTranLineDelimiter));
			if (lineEnd==KErrNotFound)
				User::Leave(EErrLineTooLong);
			iCurrentSource.iPosition=bufStart;
			iNextLineOffset=0;
			}
		iLex=iReadBuf.Mid(iNextLineOffset,lineEnd);
		iLineOffset=iNextLineOffset;
		iNextLineOffset+=lineEnd+1; // skip over the zero terminator
		iNewLineNeeded=EFalse;
		iCurrentSource.iLineNumber++;
		}

	COplLexerBase::LexL();
	if (iToken==TOplToken::EEos)
		iNewLineNeeded=ETrue;
	return iToken;
	}

EXPORT_C void COplModuleLexer::UnLex()
//
// Adds the necessary to UnLex() to deal with multiple lines
// 
	{

	if (iToken!=TOplToken::EEof)
		{
		COplLexerBase::UnLex();
		iNewLineNeeded=EFalse; // In case last was EOS
		}
	}

EXPORT_C void COplModuleLexer::UnGetToMark()
//
// Restores where we were in the line
//
	{
	COplLexerBase::UnGetToMark();
	iNewLineNeeded=EFalse;
	}

EXPORT_C void COplModuleLexer::GotoPosition(TInt aPos)
//
// Sets up the lexer to resume continue from the passed position
//
	{
	iNewLineNeeded=ETrue; // So we will force a read
	iReadBuf.SetLength(0);
	
	iCurrentSource.iPosition=aPos; // from aPos
	iNextLineOffset=0;
	}

EXPORT_C TInt COplModuleLexer::TokenOffset() const
//
// Returns the position relative to the file of the current lex position
//
	{
//	TInt pos=iCurrentSource.iPosition+iLineOffset+COplLexerBase::TokenOffset();
//	if (iNewLineNeeded && pos!=0)
//		pos++;
//	return pos;

	return iCurrentSource.iPosition+iLineOffset+COplLexerBase::TokenOffset();
	}

COplModuleLexer::COplModuleLexer()
//
// Safe first stage construction
//
	{
	}

TBool COplModuleLexer::InRootSource() const
//
// Can tell we're in the root source if the top of the source stack is NULL
//
	{
	return iSources->Top().iSource==NULL;
	}

void COplModuleLexer::ConstructL()
//
//  Dangerous second stage construction
//
	{
	iSources=new(ELeave) CTranStackFlat<TOpenSource>(KOplModLexSourceGran);
	}

TBool COplModuleLexer::CloseCurrentSource(TBool aRestoreName)
//
// Closes down the current source file and pops the old one off the stack
//
	{
	TBool wasLast=ETrue;
	delete iCurrentSource.iName;
	if (iCurrentSource.iSource)
		iCurrentSource.iSource->Close();
	
	// And recover the previous (possibly NULL) source	
	iSources->Pop(&iCurrentSource);
	
	// If it wasn't the last we need to resume where we were
	if (iCurrentSource.iSource)
		{
		iCurrentSource.iLineNumber--; // So the reasd doesn't increment the line count.
		GotoPosition(iCurrentSource.iPosition); // Get back to where we left off
		if (aRestoreName)
			((TSourceTranslateError *)iError)->SetSource(*iCurrentSource.iName);    // Keep track of where any error occurs
		wasLast=EFalse;
		}
	return wasLast;
	}