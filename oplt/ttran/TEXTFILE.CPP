// TEXTFILE.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

// Classes to read files into memory as MTextSources
//

#include <f32file.h>
#include "textfile.h"
#include <s32file.h> // file store
#include <txtglobl.h> // global text
#include <txtfmlyr.h> // paraformatlevel
#include <apparc.h> // streamdictionary


/////////////////////////////////////////////////////////////////////////////////
//
// ROplTextSystem
//
/////////////////////////////////////////////////////////////////////////////////
#define TESTDIR _L("c:\\TOPLT\\")

TInt ROplTextSystem::OpenSource(TDes& aFileName, MTextSource*& aSource)
//
// 
//
	{
	TParse name;
	const TFileName path=TESTDIR;
	name.Set(aFileName,&path,NULL);

	CEpocTextSource *pT=new CEpocTextSource();
	TInt ret=KErrNoMemory;
	if (pT!=NULL)
		{	
		TRAP(ret,pT->OpenL(*this,name.FullName()));
		if (ret==KErrNone)
			aSource=pT;
		else
			delete pT;
		}
	return ret;
	}

//////////////////////////////////////////////////////////////////////////////////
//
// CFileSource
//
//////////////////////////////////////////////////////////////////////////////////
TInt CFileSource::Read(TDes& aBuf,TInt aPos)
//
// Reads as much as will fit (or remains) from aPos
//
	{
	
	TInt ret=KErrEof;
	TInt len=iContents.Length()-aPos;
	if (len>0)
		{
		if (len>aBuf.MaxLength())
			len=aBuf.MaxLength();
		aBuf.Copy(iContents.Mid(aPos,len));
		iPos=aPos+len;
		ret=KErrNone;
		}
	return ret;
	}

TInt CFileSource::Read(TDes& aBuf)
//
// Reads as much as will fit (or remains) from immediately after the last read.
//
	{
	return Read(aBuf,iPos);
	}

void CFileSource::Close()
//
//
//
	{
	delete this;
	}

CFileSource::CFileSource() : iContents(NULL,0)
//
//
//
	{
	}

CFileSource::~CFileSource()
//
// Delets the buffer
// 
	{
	User::Free((TAny *)iContents.Ptr());
	}

/////////////////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////


void CCrLfTextSource::OpenL(RFs& aFs,const TDesC& aFileName)
//
// Loads up the contents of the file, making it into TText and replacing CrLfs with 0's
//
	{

	// Open the file and make room for it
	TAutoClose<RFile> file;
	User::LeaveIfError(file.iObj.Open(aFs,aFileName,EFileRead|EFileShareExclusive|EFileStreamText));
	TInt size=0;
	User::LeaveIfError(file.iObj.Size(size));
	size+=sizeof(TChar); // space for belts and braces terminator on the last line
	iContents.Set((TText *)User::AllocL(sizeof(TText)*size),0,size);
	

	// Read it in reasonable chunks replacing Cr with a KTextTranLineDelimiter and Lf with a space
	// We add in the space so that the line breaks stay in roughly the same places.
	TPtr8 readBuf((TUint8 *)User::AllocLC(0x1000*sizeof(TUint8)),0x1000);
	FOREVER
		{
		TInt ret=file.iObj.Read(readBuf);
		TInt readLen=readBuf.Length();
		if (ret==KErrEof || readLen==0) // Cos read on an empty file is a bit odd.
			{
			iContents.Append(KTextTranLineDelimiter);
			break;
			}
		User::LeaveIfError(ret);
		for (TInt index=0;index<readLen;index++)
			{
			TChar c=readBuf[index++]+(readBuf[index]<<8);
			if (c=='\r')
				c=KTextTranLineDelimiter;
			else if (c=='\n')
				c=' ';
			iContents.Append(c);
			}
		}
	iContents.Append(KTextTranLineDelimiter);
	CleanupStack::PopAndDestroy();
	}


/////////////////////////////////////////////////////////////////////////////////////////
//
// Symbian OS text methods
//
/////////////////////////////////////////////////////////////////////////////////////////
const TUid KUidTextEdApp={0x10003A63};

void CEpocTextSource::OpenL(RFs& aFs,const TDesC& aFileName)
	{
	CFileStore* store=NULL;
	// Pushes stream dictionary automatically.
	CStreamDictionary* dic=CApaProcess::ReadRootStreamLC(aFs,store,aFileName,EFileShareReadersOnly|EFileRead);
	CleanupStack::PushL(store);
	TStreamId streamId=dic->At(KUidTextEdApp);

	RStoreReadStream instream;
	instream.OpenLC(*store, streamId); // Pushes instream

	// Create format layers for global text
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CGlobalText* textModel = CGlobalText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(textModel);
	instream >> *textModel;

	TInt docSize=textModel->DocumentLength();

	//Ensure the last char is a CR.
	TBool needToAddDelim=EFalse;
	TBuf<2> end;
	textModel->Extract(end,docSize-1,1);
	if (end[0]!=KTextTranLineDelimiter)
		{
		docSize++; // =(sizeof(TText));
		needToAddDelim=ETrue;
		}

	if (iContents.Ptr())
		User::Free((TAny *)iContents.Ptr());

	iContents.Set((TText *)User::AllocL(sizeof(TText)*docSize),0,docSize);
	textModel->Extract(iContents,0,docSize);

	// Ensure there's a CR on the last line.
	if (needToAddDelim)
		iContents.Append(KTextTranLineDelimiter);
	CleanupStack::PopAndDestroy(6); // textModel & 2xformatLayers, instream,store,streamDictionary
	}

// <ends>