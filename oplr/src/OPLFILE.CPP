// OPLFILE.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#include <oplr.h>
#include <oplfile.h>
#include <module.h>
#include "oplutil.h"

// Fix defect 844681 "OPO filename too long" by using chars, not bytes.
#define KMaxOplModuleNameLength (127*sizeof(TText))

const TBool CModuleReader::IsOpler1OPO()
	{
	return iIsOpler1OPO;
	}

const TOpoRootStream& CModuleReader::RootStream()
	{
	return iRootStream;
	}

TUid CModuleReader::Uid() const
	{
	return iUid;
	}

RFile CModuleReader::CreateL(RFs &aFsClient,TFileName &aFileName)
	{
	TInt err=iFileHandle.Open(aFsClient,aFileName,EFileStream|EFileRead|EFileShareReadersOnly);
	CleanupClosePushL(iFileHandle);
	if (err)
		{
		COplRuntime* rt=TheRuntime();
		TParse parser;
		aFsClient.Parse(aFileName,parser);
		TPtrC name(parser.NameAndExt());
		rt->SetErrorBuffer(&name);
		User::Leave(KOplErrNoMod);
		}
	ReadFileHeaderL();
	iFsClient=aFsClient;
	CleanupStack::Pop(); // iFileHandle *MAD* code this is member data!!
	return iFileHandle;
	}
	 
#if !defined(INTERNAL_RELEASE)
//#error No signature in final module format
#else
TBool CModuleReader::SignatureOK() const 
	{
	TBuf<32> tmp(iFileHeader16.iSignature);
	_LIT(KOPOSig,"OPLObjectFile**");
	if (tmp.Compare(KOPOSig)!=0)
		return EFalse;
	else 
		return ETrue;
	}
#endif

TOplName *CModuleReader::ReadOplNameL()
	{
	TInt pos(iRootStream.iSrcNameId);
#ifdef INTERNAL_RELEASE
	if (iIsOpler1OPO) // only seek if opler1 OPO
#endif
		User::LeaveIfError(iFileHandle.Seek(ESeekStart,pos));
	TPtr8 pLen((TText8 *)&iOplName.iLength,2);
	TPtr pBuf(iOplName.iModuleName,KMaxOplModuleNameLength);
	User::LeaveIfError(iFileHandle.Read(pLen,2));	       // read the length of the name first
	__ASSERT_DEBUG(iOplName.iLength<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
	if ((iOplName.iLength=*pLen.Ptr())==0)
		return NULL;
	User::LeaveIfError(iFileHandle.Read((TDes8&)pBuf,iOplName.iLength*sizeof(TText)));
	return &iOplName;
	}

void CModuleReader::ReadFileHeaderL()
// Rewrite this when Opl1993 modules no longer supported
	{
	// read 4 Uids, root stream id, OPL interpreter Uid
	TOpoStoreHeader storeHdr;
	TPtr8 pStoreBuf((TText8 *)&storeHdr,sizeof(TOpoStoreHeader));
	User::LeaveIfError(iFileHandle.Read(pStoreBuf,sizeof(TOpoStoreHeader)));
	iRootStreamId=storeHdr.iRootStreamId;
	TPtr8 rootPtr((TText8 *)&iRootStream,sizeof(TOpoRootStream));
	User::LeaveIfError(iFileHandle.Read(storeHdr.iRootStreamId,rootPtr,sizeof(TOpoRootStream)));
	if (storeHdr.iFileUid[2]==KUidOplInterpreter || storeHdr.iFileUid[1]==KUidOplApp)
		{
		iIsOpler1OPO=ETrue;
		iUid=storeHdr.iFileUid[2];	// the app's uid (unless not top-level)
		}
	else
		// Opl1993 module
#if !defined(INTERNAL_RELEASE)
		{
		User::Leave(KOplErrBadFileType);
		}
#else
		{
		// Scaffolding for B4-day
		// Seek to start to try S3a module header
		iIsOpler1OPO=EFalse;
		TInt pos(0);
		iFileHandle.Seek(ESeekStart,pos);
		TPtr8 pBuf((TText8 *)&iFileHeader16,sizeof(TOpoFileHeader16));
		User::LeaveIfError(iFileHandle.Read(pBuf,sizeof(TOpoFileHeader16)));
		}
#endif
	}

#if defined(INTERNAL_RELEASE)
// Opl1993 modules only
TOpoModuleHeader16 *CModuleReader::ReadModuleHeader16L()
	{
	TPtr8 pBuf((TText8 *)&iModuleHeader16,sizeof(TOpoModuleHeader16));
	User::LeaveIfError(iFileHandle.Read(iFileHeader16.iOffset,pBuf,sizeof(TOpoModuleHeader16)));
	return &iModuleHeader16;
	}
#endif

void CModuleReader::SeekProcHeaderL()
	{
#if defined(INTERNAL_RELEASE)
	TInt offset;
	if (iIsOpler1OPO)
		offset=iRootStream.iProcTableId;
	else
		offset=iModuleHeader16.iProcTableOffset;
#else
	TInt offset=iRootStream.iProcTableId;
#endif
	User::LeaveIfError(iFileHandle.Seek(ESeekStart,offset));
	}

void CModuleReader::ReadProcTableL(HBufC8* &aProcTable)
	{
	iProcTableLen=iRootStreamId-iRootStream.iProcTableId;
	aProcTable=HBufC8::NewMaxL(iProcTableLen);
	TPtr8 pTable(aProcTable->Des());
	User::LeaveIfError(iFileHandle.Read(pTable,iProcTableLen)); // read whole proc table in one go
	}

TOpoProcHeader* CModuleReader::ReadProcHeaderL(TUint8* &aProcInfo)
	{
	if (!*aProcInfo)
		return NULL;
	iProcHeader.iOplProcName=(TOplName*)aProcInfo;
	aProcInfo+=(*aProcInfo)*sizeof(TText)+1+KOplAlignment;
	iProcHeader.iProcOffset=(TUint32)OplUtil::GetLong(aProcInfo);
	aProcInfo+=sizeof(TUint32);
	iProcHeader.iLineNumber=(TUint16)OplUtil::GetWord(aProcInfo);
	aProcInfo+=sizeof(TUint16);
	return &iProcHeader;
	}


void CModuleReader::SeekOpxHeaderL()
    {
	TInt offset=iRootStream.iOpxTableId;
	User::LeaveIfError(iFileHandle.Seek(ESeekStart,offset));
    }

TInt CModuleReader::ReadCountOfOpxsL()
	{
	TUint16 len;
	TPtr8 pLen((TText8 *)&len,sizeof(TInt));
	User::LeaveIfError(iFileHandle.Read(pLen,sizeof(TUint16)));
	return TInt(len);
	}

TOpoOpxHeader *CModuleReader::ReadOpxHeaderL()
	{
	TPtr8 pLen((TText8 *)&iOplName.iLength,2);
	TPtr pBuf(iOplName.iModuleName,KMaxOplModuleNameLength);	
	User::LeaveIfError(iFileHandle.Read(pLen,2));				// read the length of OpxName first
	__ASSERT_DEBUG(iOplName.iLength<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
	if ((iOplName.iLength=*pLen.Ptr())==0)
		return NULL;
	User::LeaveIfError(iFileHandle.Read((TDes8&)pBuf,iOplName.iLength*sizeof(TText)));
	TPtrC str((TText*)iOplName.iModuleName,iOplName.iLength);
	TFileName opxName=str;
	_LIT(KOpx,".OPX");
	opxName+=KOpx;
	TFindFile opxFile(iFsClient);
	TInt ret=opxFile.FindByDir(opxName,KOpxPath);
	if (ret<0)
		{
		TheRuntime()->SetErrorBuffer(&opxName);
		User::Leave(KOplErrOpxNotFound);
		}
	iOpxHeader.iOpxFileName=opxFile.File();
	TPtr8 pUid((TText8 *)&iOpxHeader.iUid,sizeof(TUid));
	User::LeaveIfError(iFileHandle.Read(pUid,sizeof(TUid)));
	TEntry entry;
	iFsClient.Entry(opxFile.File(),entry); // must exist

	if (entry[1]!=KUidOpx || entry[2]!=iOpxHeader.iUid)
		{
		TheRuntime()->SetErrorBuffer(&opxName);
		User::Leave(KErrNotSupported); // OPX is not of the right type
		}
	TPtr8 pVersion((TText8 *)&iOpxHeader.iVersion,sizeof(TUint16));
	User::LeaveIfError(iFileHandle.Read(pVersion,sizeof(TUint16)));
	return &iOpxHeader;
	}

void CModuleReader::ReadOpxProcNamesL(CArrayVarFlat<TOplProcName>& aProcNames,TInt aProcCount)
	{
	TInt i; 
	for (i=0;i<aProcCount;i++)
		{
		TPtr8 pLen((TText8 *)&iOplName.iLength,2); 
		TPtr pBuf(iOplName.iModuleName,KMaxOplModuleNameLength);
		User::LeaveIfError(iFileHandle.Read(pLen,2));				// read the length of OpxName first
		__ASSERT_DEBUG(iOplName.iLength<=KOplMaxStrLen,AlignPanic(KErrOplAlignBadLength));
		User::LeaveIfError(iFileHandle.Read((TDes8&)pBuf,iOplName.iLength*sizeof(TText)));
		TBuf<32> str;
		str.Copy((TText*)iOplName.iModuleName,iOplName.iLength);
		aProcNames.AppendL(str,(TInt)(sizeof(TDes)+str.Length()));
		}
	}

