// OPLFILE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __OPLFILE_H
#define __OPLFILE_H

#include <texttran.h>

const TInt KMaxProcName=32;
typedef TBuf<KMaxProcName> TOplProcName;

class TOplAPI;

class TOplName
	{
public:
	TUint16 iLength; // the first word of the name. Length in characters.
	TText  iModuleName[127];
	};

class TOpoStoreHeader
	{
public:
	TUid iFileUid[4];
	TUint32 iRootStreamId;
	};

// root stream
class TOpoRootStream
	{
public:
	TUid iInterpUid;				// OPL interpreter Uid
    TUint16 iTransVersion; 			// Translator version  
    TUint16 iMinRunVersion; 		// minimum runtime version needed to run qcode 
	TUint32 iSrcNameId;				// id of stream containing source name
	TUint32 iProcTableId;			// id of stream conatining procedure table
	TUint32 iOpxTableId;
	CTextTranslator::TDebugFlag iDebugFlag;
	};


class TOpoProcHeader
	{
public:
	TOplName *iOplProcName;
	TUint32 iProcOffset;		// offset of procedure code in file
	TUint16 iLineNumber;		// line number of start of proc in source file
	}; 

class TOpoOpxHeader
	{
public:
	TFileName iOpxFileName;
	TUid iUid;
	TUint16 iVersion;
	}; 				  		// followed by Opx procedure Names

#if defined(INTERNAL_RELEASE)
// for Opl1993 modules only
class TOpoFileHeader16
    {
public:
    TText 	iSignature[0x10]; 	// File signature 
    TUint16 iVersion;			// File type version 
    TUint16 iOffset; 			// Offset in file of data
    };

class TOpoModuleHeader16
    {
public:
    TInt32 	iTotalSize; 			// Total size of data in file/segement 
    TUint16 iTransVersion; 		// Translator version  
    TUint16 iMinRunVersion; 		// minimum runtime version needed to run qcode 
    TInt32	iProcTableOffset; 	// Offset in file to procedure table 
    };
#endif
	

class CModuleReader : public CBase
	{
public:
	RFile CreateL(RFs &aFsClient,TFileName &aFileName);
	TOplName  *ReadOplNameL();
	void ReadFileHeaderL();
	void SeekProcHeaderL();
	void SeekOpxHeaderL();
	TOpoOpxHeader *ReadOpxHeaderL();
	TInt ReadCountOfOpxsL();
	void ReadOpxProcNamesL(CArrayVarFlat<TOplProcName>& aProcNames,TInt aProcCount);
	const TOpoRootStream& RootStream();
	const TBool IsOpler1OPO();
	TUid Uid() const;
#ifdef INTERNAL_RELEASE
	TOpoModuleHeader16 *ReadModuleHeader16L();
	TBool  SignatureOK() const;
#endif
	void ReadProcTableL(HBufC8* &aProcTable);
	TOpoProcHeader* ReadProcHeaderL(TUint8* &aProcInfo);
	TUint32 iRootStreamId;
	TInt iProcTableLen;
private:
	RFs iFsClient;
	RFile iFileHandle;
	TOpoRootStream iRootStream;
	TOpoProcHeader iProcHeader;
	TOpoOpxHeader iOpxHeader;
	TOplName  iOplName;				 // any opl names to be passed to the caller
	TUid iUid;
	TBool iIsOpler1OPO;				// not Opl1993 OPO
#ifdef INTERNAL_RELEASE	
	TOpoModuleHeader16 iModuleHeader16;
	TOpoFileHeader16 iFileHeader16;	// for Opl1993 modules
#endif
	};

#endif
