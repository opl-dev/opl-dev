// OPLREADR.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Reader classes for the various types of OPO.
// Used to test the files are the same.
//

#include <opltdef.h>
#include "oplreadr.h"

LOCAL_C void LeaveSubSystem()
	{
	User::Leave(KErrGeneral);
	}


LOCAL_C void TestL(TBool flag)
	{
	if (!flag)
		LeaveSubSystem();
	}

LOCAL_C HBufC* GetLbcLC(RReadStream& aStream)
//
// Reads in an Lbc
//
	{
	TInt length=aStream.ReadUint16L();
	if (length==0x2e00) // That's all
		length=0;
	HBufC *pH=HBufC::NewMaxLC(length);
	TPtr8 des((TUint8*)pH->Des().Ptr(),pH->Des().Size());
	aStream.ReadL(des,length*sizeof(TText)); //Includes LBC2
	return pH;
	}

LOCAL_C HBufC* GetLbcL(RReadStream& aStream)
//
// Reads in an Lbc
//
	{
	HBufC* string=GetLbcLC(aStream);
	CleanupStack::Pop();
	return string;
	}

LOCAL_C TInt ExtractInt16(const TDesC8& aDes,TUint anOffset)
    {
    return TInt(aDes[anOffset]|(TInt(aDes[anOffset+1])<<8));
    }

LOCAL_C TInt ExtractInt32(const TDesC8& aDes,TUint anOffset)
    {
    return ExtractInt16(aDes,anOffset)|(ExtractInt16(aDes,anOffset+sizeof(TUint16))<<16);
    }

////////////////////////////////////////////////////////////////
//
// COpoApp - Details about the App, as stored in the file
//
////////////////////////////////////////////////////////////////
TBool COpoApp::operator ==(COpoApp& anApp)
//
// Checks if a couple of these things are the same
//
	{
	return Equals(anApp);
	}

TBool COpoApp::operator !=(COpoApp& anApp)
//
// Checks out if they're different
//
	{
	return !Equals(anApp);
	}


////////////////////////////////////////////////////////////////
//
// COpl1993App - Details about the App, as stored in the Opl1993 file format
//
////////////////////////////////////////////////////////////////
COpl1993App *COpl1993App::NewL(const TDesC8& anAppData)
//
//
//
	{

	COpl1993App *pA=new(ELeave) COpl1993App;
	CleanupStack::PushL(pA);
	pA->iData=anAppData.AllocL();
	CleanupStack::Pop();
	return pA;
	}

COpl1993App::~COpl1993App()
	{
	delete iData;
	}

COpoApp::TType COpl1993App::Type() const
//
// Not inline as it's virtual
//
	{
	return EOpl1993;
	}

TBool COpl1993App::Equals(COpoApp& anApp) const
	{

	if (anApp.Type()!=EOpl1993)
		return EFalse;
	return *iData==*((COpl1993App&)anApp).iData;
	}


////////////////////////////////////////////////////////////////
//
// COpler1App - Details about the Awpp, as stored in the file
//
////////////////////////////////////////////////////////////////
COpler1App* COpler1App::NewL(const TUid& aUid)
//
//
//
	{

	return new(ELeave) COpler1App(aUid);
	}

COpoApp::TType COpler1App::Type() const
//
//
//
	{
	return EOpler1;
	}

TBool COpler1App::Equals(COpoApp& anApp) const
//
//
//	
	{
	if (anApp.Type()!=EOpler1)
		return EFalse;

	return iUid==((COpler1App&)anApp).iUid;
	}

COpler1App::COpler1App(const TUid& aUid)
	: iUid(aUid)
//
//
//
	{
	}

////////////////////////////////////////////////////////////////
//
// COpoOpx - Opx as put in an Opo file
//
///////////////////////////////////////////////////////////////
COpoOpx* COpoOpx::NewLC(RReadStream& aStream)
//
// Opx info
//
	{
	COpoOpx* pOpx=new(ELeave) COpoOpx();
	CleanupStack::PushL(pOpx);
	pOpx->ConstructL(aStream);
	return pOpx;
	}

COpoOpx::~COpoOpx()
//
//
//
	{
	delete iName;
	}

TBool COpoOpx::operator==(COpoOpx& anOpx)
//
// Compares all the bits
//
	{
	return ((*iName)==(*anOpx.iName) &&	iUid==anOpx.iUid && iVersion==anOpx.iVersion);
	}

COpoOpx::COpoOpx()
//
//
//
	{
	}

void COpoOpx::ConstructL(RReadStream& aReadStream)
	{
	
	iName=GetLbcL(aReadStream);
	iUid=TUid::Uid(aReadStream.ReadUint32L());
	iVersion=aReadStream.ReadUint16L();
	}

////////////////////////////////////////////////////////////////
//
// COpoProc - A procedure from the file
//
////////////////////////////////////////////////////////////////
COpoProc* COpoProc::NewLC(RReadStream& aStream)
//
// Extracts the name from the stream and constructs the object
//
	{
	
	COpoProc* pProc=new(ELeave) COpoProc;
	CleanupStack::PushL(pProc);
	pProc->ConstructL(aStream);
	if (!pProc->iName) // End of the list
		{
		CleanupStack::PopAndDestroy();
		pProc=NULL;
		}
	return pProc;
	}

COpoProc::~COpoProc()
//
// Deletes all that good stuff
//
	{
	
	delete iName;
	delete iHeader;
	delete iQcode;
	}

TBool COpoProc::operator==(COpoProc& aProc)
//
// Do that comparision thing
//
	{
	
	return (*iName==*aProc.iName &&
		iLineNumber==aProc.iLineNumber &&
		*iHeader==*aProc.iHeader &&
		*iQcode==*aProc.iQcode);
	}

void COpoProc::ReadCodeL(RReadStream& aStream)
//
// Stream constains all the code for the procedure
//
	{

	// We take a couple of words off the top of the header (hence -2*sizeof...)
	TUint headerSize=aStream.ReadUint16L()-2*sizeof(TUint16);
	iTotalDataSize=aStream.ReadUint16L(); // Only really do this to get at code size
	TUint codeSize=aStream.ReadUint16L();
	iHeader=HBufC8::NewMaxL(headerSize);
	TPtr8 hdrDes=iHeader->Des();
	aStream.ReadL(hdrDes,headerSize);
	iQcode=HBufC8::NewMaxL(codeSize);
	TPtr8 qcodeDes=iQcode->Des();
	aStream.ReadL(qcodeDes,codeSize);

	}

void COpoProc::ConstructL(RReadStream& aStream)
	{

	iName=GetLbcL(aStream);
	}

////////////////////////////////////////////////////////////////
//
// COpo - an Opo Module
//
////////////////////////////////////////////////////////////////
COpo *COpo::NewLC(RFs& aFileSession, const TDesC& aFileName)
//
// Has a quick sniff at the file to decide what type
// it is and then creates a reader of the appropriate class
//
	{

	TAutoClose<RFile> file;
	file.PushL();
	User::LeaveIfError(file.iObj.Open(aFileSession,aFileName,EFileShareExclusive|EFileStream|EFileRead));
	TBuf8<0x20> signature8;
	User::LeaveIfError(file.iObj.Read(signature8));
	COpo *pOpo=NULL;
	TPtr signature((TUint16*)signature8.Ptr(),0x10);
	signature.SetLength(signature8.Length()>>1);
	if (signature==TPtrC(_S("OPLObjectFile**"),16))
		pOpo=new(ELeave) COpl1993Opo;
	else // This is a bit tricky since it'll all look likes streams
		pOpo=new(ELeave) COpler1Opl;
	CleanupStack::PopAndDestroy(); // close up the file
	CleanupStack::PushL(pOpo);// Push the Opo
	
	// The construct is where the real work happens
	pOpo->ConstructL(aFileSession,aFileName); // and get started
	return pOpo;
	}

TBool COpo::operator==(COpo& anOpo)
	{
	
	
	// TTypes of file - makes allowance for the existence of
	// Psuedo Opler1 stream files
	if (Type()!=anOpo.Type())
		return EFalse;


	// Check App info - assuming that they're both Apps
	if (iApp || anOpo.iApp) // One or other has an app
		{
		if (!(iApp && anOpo.iApp) ||
			!(*iApp==*anOpo.iApp))
			return EFalse;
		}
	
	// Check the Opx list
	TInt index;
	if (iOpxList.Count()!=anOpo.iOpxList.Count())
		return EFalse;
	for (index=iOpxList.Count()-1;index>=0;index--)
		{
		if (!(*iOpxList[index]==*anOpo.iOpxList[index]))
			return EFalse;
		}


	// And the procedures
	if (iProcList.Count()!=anOpo.iProcList.Count())
		return EFalse;
	for (index=iProcList.Count()-1;index>=0;index--)
		{
		if (!(*iProcList[index]==*anOpo.iProcList[index]))
 			return EFalse;
		}
	
	return ETrue;
	}

COpo::COpo()
	: iOpxList(KOpoOpxGranularity), iProcList(KOpoProcGranularity)
	{

	}

	
	
void COpo::ProceduresL(RReadStream& aStream)
//
// Stream contains all the procedures
//
	{

	FOREVER
		{
		COpoProc *proc=COpoProc::NewLC(aStream); // Gets the name
		if (!proc->Name().Length()) // End of the list
			{
			CleanupStack::PopAndDestroy(); // Empty proc
			break;
			}

		RReadStream* procStream=OpenProcStreamLC2(aStream);
		proc->SetLineNumber(aStream.ReadUint16L()); // Line number in file
		proc->ReadCodeL(*procStream);
		CleanupStack::PopAndDestroy(2); // Object and stream

		iProcList.AppendL(proc);
		CleanupStack::Pop(); // COpoProc 
		}
	}	

COpo::~COpo()
	{
	delete iSourceName;
	delete iApp;
	TInt index=0;
	TInt count=iOpxList.Count();
	for (;index<count;index++)
		delete iOpxList[index];
	for (index=0,count=iProcList.Count();index<count;index++)
		delete iProcList[index];

	}
/////////////////////////////////////////////////////////////////
// time flat file format
//
/////////////////////////////////////////////////////////////////
const TUint KSizeofSignature=16;
const TUint KSizeofHeader1=20;
const TUint KSizeofHeader2=12;
const TUint KQcodeSizeOffset=2;

void COpl1993Opo::ConstructL(RFs& aFileSession, const TDesC& aFileName)
	{
	
	//  Read the file into a big old descriptor
	RFile file;
	User::LeaveIfError(file.Open(aFileSession,aFileName,EFileShareExclusive|EFileStream|EFileRead));
	TInt size=0;
	User::LeaveIfError(file.Size(size));
	
	iBuf=HBufC8::NewMaxLC(size);
	TPtr8 readPtr=iBuf->Des();
	User::LeaveIfError(file.Read(readPtr));
	file.Close();	


	TPtrC8 ptr=iBuf->Left(KSizeofSignature);
	TPtrC ptr16((TText*)iBuf->Left(KSizeofSignature).Ptr(),KSizeofSignature);
	TInt dataOffset=0;
	// File header
		{
		TestL(ptr16.Compare(TPtrC(_S("OPLObjectFile**"),KSizeofSignature))==0);
		TestL(ExtractInt16(*iBuf,16*sizeof(TText))==1); // Global file format
		dataOffset=ExtractInt16(*iBuf,17*sizeof(TText));
		}
	
	TInt nameLength=0;
	// Source name
		{
		nameLength=(*iBuf)[36]<<1;
		ptr.Set(iBuf->Mid(37,nameLength));
		TPtr ptr1((TUint16*)ptr.Ptr(),ptr.Size()>>1);
		iSourceName=ptr1.AllocL();
		}
		
	// Any App info
		{
		TInt appStart=37+nameLength; // Just past the source name
		ptr.Set(iBuf->Mid(appStart,dataOffset-appStart));
		if (ptr.Length()!=0) // There's an app
			iApp=COpl1993App::NewL(ptr);
		}

	// Now we move on to Header 2 - Total file size: Translator Ver: Min runtime & offset to procedure table
	TInt procTableOffset=0;
		{
		ptr.Set(iBuf->Mid(dataOffset,KSizeofHeader2));
		TestL(ExtractInt32(ptr,0)==iBuf->Length()); // Total file size
		iTranslatorVersion=ExtractInt16(ptr,4);
		iRuntimeVersion=ExtractInt16(ptr,6);
		procTableOffset=ExtractInt32(ptr,8); // Offset to procedure table
		}
	
	// Now the procedure table
		{
		ptr.Set(iBuf->Mid(procTableOffset,iBuf->Length()-procTableOffset));
		RDesReadStream procList(ptr);
		ProceduresL(procList);
		}

	CleanupStack::PopAndDestroy(); // ibuf read buffer
	}

RReadStream * COpl1993Opo::OpenProcStreamLC2(RReadStream& aProcList)
//
// Next thing in the proc stream is the file offset of the proc data
//
	{

	TInt offset=aProcList.ReadInt32L();
	RDesReadStream *pS=new(ELeave) RDesReadStream(iBuf->Mid(offset,iBuf->Length()-offset)); // Just take the rest
	CleanupStack::PushL(pS);
	CleanupStack::PushL((TAny *)NULL);
	return pS;
	}

COpo::TType COpl1993Opo::Type() const
	{
	return EOpl1993;
	}

/////////////////////////////////////////////////////////////////
//
// COpler1Opl - the real McCoy
//
/////////////////////////////////////////////////////////////////
void COpler1Opl::ConstructL(RFs& aFileSession, const TDesC& aFileName )
	{

	iStore=CFileStore::OpenLC(aFileSession,aFileName,EFileRead);

	// First off check the UID thing
	const TUidType& type=iStore->Type();
	if (type[1]==KUidOplObj) // 'Straight' interpreter object
		TestL(type[2]==KUidOplInterpreter); // Check Uid is Ok
	else
		{
		TestL(type[1]==KUidOplAppNoFile || type[1]==KUidOplApp); // It's actually an APP
		iApp=COpler1App::NewL(type[2]);
		}

	// Now check out the Root stream
	RStoreReadStream root;
	root.OpenLC(*iStore,iStore->Root());
	TUid rootUid;
	root>>rootUid;
	TestL(rootUid==KUidOplInterpreter);
	iTranslatorVersion=root.ReadUint16L();
	iRuntimeVersion=root.ReadUint16L();

	// Now read it all in
	TStreamId id;
	RStoreReadStream stream;

	// First off we get Source name stream
	root>>id;
	TestL(id!=KNullStreamId);
	stream.OpenLC(*iStore,id);		 // Source stream
	iSourceName=(HBufC16*)GetLbcL(stream);
	CleanupStack::PopAndDestroy(); // Source stream

	// Now we get in the procedure table
	root>>id;
	TestL(id!=KNullStreamId);
	stream.OpenLC(*iStore,id);
	ProceduresL(stream);
	CleanupStack::PopAndDestroy(); // Proc stream

	// Finally the OPX stream
	root>>id; // Id of the Opx stream
	if (id!=KNullStreamId) // Actually have a stream there
		{
		stream.OpenLC(*iStore,id);
		for(TUint opxCount=stream.ReadUint16L();opxCount>0;opxCount--)
			{
			COpoOpx *opx=NULL;
			opx=COpoOpx::NewLC(stream);
			iOpxList.AppendL(opx);
			CleanupStack::Pop();
			}
		CleanupStack::PopAndDestroy(); // Opx stream
		}

	CleanupStack::PopAndDestroy(2); // Root stream & file store
	}

COpo::TType COpler1Opl::Type() const
	{
	return COpo::EOpler1;
	}


RReadStream *COpler1Opl::OpenProcStreamLC2(RReadStream& aReadStream)
	{
	
	TStreamId id;
	aReadStream>>id;
	RStoreReadStream *pS=new(ELeave) RStoreReadStream;
	CleanupStack::PushL(pS);
	pS->OpenLC(*iStore,id);
	return pS;
	}

