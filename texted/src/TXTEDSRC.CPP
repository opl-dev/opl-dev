// TXTEDSRC.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <txtglobl.h>
#include <apparc.h>
#include "txtedsrc.h"
#include "texted.h"

CTextSource::CTextSource(CTextEdDocument* aDoc,CApaProcess* aProcess)
	{
	iDoc=aDoc;
	iProcess=aProcess;
	}

CTextSource::~CTextSource()
	{
	if (iProcess)
		iProcess->DestroyDocument(iDoc);
	}

TInt CTextSource::Read(TDes& aBuf,TInt aPos)
	{
	CGlobalText* globalText=iDoc->iGlobalText;
	TInt docLen=globalText->DocumentLength();
	if (aPos>=docLen)
		return KErrEof;
	globalText->Extract(aBuf,aPos,aBuf.MaxLength());
	if (aBuf.Length()<aBuf.MaxLength()) // buffer not full so must be at the end of the file
		aBuf.Append(KTextTranLineDelimiter); // so a CR on the last line isn't needed
	iCurrentPos=aPos+aBuf.Length();
	return KErrNone;
	}

TInt CTextSource::Read(TDes& aBuf)
	{
	return Read(aBuf,iCurrentPos);
	}

void CTextSource::Close()
	{
	delete this;
	}