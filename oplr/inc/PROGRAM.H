// PROGRAM.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <e32std.h>
#include <texttran.h>
#include <charconv.h>

IMPORT_C TPtrC DebuggerDllFilename();
IMPORT_C TFileName RuntimeResourceFile();

class COplRuntimeCharConv: public CBase
	{
public:
	IMPORT_C static HBufC16* ConvertFromNarrowToUnicodeL(const TDesC8& aNarrowString,const TUint aCharacterSet=KCharacterSetIdentifierAscii);
	IMPORT_C static HBufC16* ConvertFromNarrowToUnicodeLC(const TDesC8& aNarrowString,const TUint aCharacterSet=KCharacterSetIdentifierAscii);
	IMPORT_C static HBufC8* ConvertFromUnicodeToNarrowL(const TDesC16& aUnicodeString,const TUint aCharacterSet=KCharacterSetIdentifierAscii);
	IMPORT_C static HBufC8* ConvertFromUnicodeToNarrowLC(const TDesC16& aUnicodeString,const TUint aCharacterSet=KCharacterSetIdentifierAscii);
	};

class TRuntimeResBuf
	{
public:
	TInt  iError;           // runtime error or KErrNone if no error
	TInt  iProcLineNumber;  // line number of the procedure in the source module ( from OPO Module)
	TUint iOffset;          // Q code offset to runtime error within proc
	TBuf<80> iErrMsg;       // runtime error string
	TFileName iSrcFile;		// to take source file name causing error
	TBuf<32> iProcName;		// to take name of procedure causing error
	CTextTranslator::TDebugFlag iDebugFlag;
	};

// for TRuntimeParams::iFlags
const TInt KRuntimeFlagsSignal=0x1; /* signal on completion */
const TInt KRuntimeFlagsNotify=0x2; /* call notifier on error */

// Runtime commands for command-line
const TInt KOplrCommandRunFromEditor='E';
const TInt KOplrCommandRunFromDebugger='D';
const TInt KOplrCommandRunNoIPC='R';

class TRuntimeParams
	{       /* used to pass extra command-line parameters to OPLR.APP */
public:
	TInt  iFlags;                  /* mode information */
	TThreadId  iOwnerThreadId;     /* requestor process id */
	TDes* iResultOffset;			/* result buffer offset */
	TInt iOwnerWindowGroup;			/* requestor window group identifier */
	};

#endif