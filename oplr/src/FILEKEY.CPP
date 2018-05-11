// FILEKEY.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include <coeutils.h>
#include <oplr.h>
#include <opcodes.h>
#include <f32file.h>
#include "oplutil.h"

#define NUM_PARSE_ELEMS 6

class TOplParseOffset
	{
public:
	TInt16 iFilSysOff;
	TInt16 iDeviceOff;
	TInt16 iPathOff;
	TInt16 iNameOff;
	TInt16 iExtOff;
	TInt16 iWildCardFlags;
	};

enum TOplDirOpFlags
	{
	EOplDirMake,
	EOplDirRemove
	};

GLREF_C void ut_parse(COplRuntime& aRuntime,TPtrC aBase,TPtrC aRel,TDes& aParsedName,TOplParseOffset& aOffset);	
 
LOCAL_C void popAndParseFileNameL(CStack& aStack, COplRuntime& aRuntime,TParse& aParse)
	{
	RFs& fsClient=aRuntime.ConEnv()->FsSession();
	User::LeaveIfError(fsClient.Parse(aStack.PopString(),aParse));
	}

void FuncOpCode::ParseStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*  */)
/*
Parse filename
In:	stack0	address of array (*not* left-side ref)
	stack1  related$
	stack2	baseFile$
	Out:	parsed$
*/
	{
	TOplParseOffset* pOffset=(TOplParseOffset*)OplUtil::PopOffsetAsAddrL(
            aStack,aRuntime.Heap64(),sizeof(TInt16) * NUM_PARSE_ELEMS);
	const TPtrC related=aStack.PopString();
	const TPtrC baseFile=aStack.PopString();
 	TFileName parsedName;
	ut_parse(aRuntime,baseFile,related,parsedName,*pOffset);
	aStack.Push(parsedName);
    }

void FuncOpCode::DirStr(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TPtrC name=aStack.PopString();
	TInt err=KErrNone;
	if (name.Length()!=0)
		{
		if (aRuntime.StateFlags() & EDirOutstanding)
			{
			aRuntime.Dir().Close();
			aRuntime.ClearStateFlags(EDirOutstanding);
			}
		RFs&	fs=aRuntime.ConEnv()->FsSession();
		TParse parser;
		fs.Parse(name,parser);
		if ((err=aRuntime.Dir().Open(fs,parser.FullName(),KEntryAttMatchMask))==KErrNone)
			{
			aRuntime.SetStateFlags(EDirOutstanding);
			aRuntime.SetDirPath(parser.DriveAndPath());
			}
		}
	else if (!(aRuntime.StateFlags() & EDirOutstanding))
		{
		aStack.Push(KNullDesC);	// As in Opl1993 DIR$("") returns "" when no DIR$() outstanding
		return;
		}
	TEntry* entry=new(ELeave) TEntry;
	CleanupStack::PushL(entry);
	TFileName* fileName=new(ELeave) TFileName;
	CleanupStack::PushL(fileName);
	if (err==KErrNone)
		{
		if (aRuntime.Dir().Read(*entry)==KErrNone)
			{
			fileName->Copy(aRuntime.DirPath());
			fileName->Append(entry->iName);
			}
		else
			{
			aRuntime.Dir().Close();
			aRuntime.ClearStateFlags(EDirOutstanding);
			}
		}
	else if (err!=KErrNotFound)
		User::Leave(err);
	aStack.Push(*fileName);
	CleanupStack::PopAndDestroy(2); // entry and fileName
    }

void OpCode::Copy(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	RFs& fsClient=aRuntime.ConEnv()->FsSession();
	TPtrC des=aStack.PopString();
	CFileMan *fm=CFileMan::NewL(fsClient);
	TInt err=fm->Copy(aStack.PopString(),des);
	delete fm;
	User::LeaveIfError(err);
	aRuntime.ClearTrap();
	}

void OpCode::Delete(CStack& aStack, COplRuntime& aRuntime, CFrame*)
	{
	RFs fsClient=aRuntime.ConEnv()->FsSession();
	TParse parser;
	TPtrC desFile=aStack.PopString();
	if (desFile.Length()==0)
		User::Leave(KOplErrName);
	User::LeaveIfError(fsClient.Parse(desFile,parser));
	CFileMan *fm=CFileMan::NewL(fsClient);
	TInt err=fm->Delete(parser.FullName());
	delete fm;
	User::LeaveIfError(err);
	aRuntime.ClearTrap();
	}

void OpCode::Rename(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TParse desFile,srcFile;
	popAndParseFileNameL(aStack,aRuntime,desFile);
	popAndParseFileNameL(aStack,aRuntime,srcFile);
	User::LeaveIfError(aRuntime.ConEnv()->FsSession().Rename(srcFile.FullName(),desFile.FullName()));
	aRuntime.ClearTrap();
	}
	
void doDirL(CStack& aStack, COplRuntime& aRuntime, TOplDirOpFlags aFlag)
	{
	TParse parser;
	RFs& fsClient=aRuntime.ConEnv()->FsSession();
	User::LeaveIfError(fsClient.Parse(aStack.PopString(),parser));
	if (parser.NameOrExtPresent()>0)
		User::LeaveIfError(parser.AddDir(parser.NameAndExt()));
	if (aFlag==EOplDirMake)
		User::LeaveIfError(fsClient.MkDirAll(parser.DriveAndPath()));
	else	/*aFlag==EOplDirRemove */
		User::LeaveIfError(fsClient.RmDir(parser.DriveAndPath()));
	aRuntime.ClearTrap();
	}

void OpCode::MkDir(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	doDirL(aStack,aRuntime,EOplDirMake);
	}
	
void OpCode::RmDir(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	doDirL(aStack,aRuntime,EOplDirRemove);
	}
	
void OpCode::SetPath(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	TParse parser;
	RFs fsClient=aRuntime.ConEnv()->FsSession();
	User::LeaveIfError(fsClient.Parse(aStack.PopString(),parser));
	User::LeaveIfError(fsClient.SetSessionPath(parser.DriveAndPath()));
	}

void FuncOpCode::Exist(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFramePtr*/)
	{
	RFs fsClient=aRuntime.ConEnv()->FsSession();
	// As in Opl1993, directories with no extension can be tested for by using
	// terminating "."
	TParse parser;
	fsClient.Parse(aStack.PopString(),parser);
	if (ConeUtils::FileExists(parser.FullName()))
		aStack.Push((TInt16)KOplTrue); 
	else
		aStack.Push((TInt16)KOplFalse);
	}



