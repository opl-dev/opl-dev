// TXTEDSRC.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __TEXTSRC_H__
#define __TEXTSRC_H__

#ifndef __TEXTTRAN_H__
#include <texttran.h>
#endif

class CTextEdDocument;
class CApaProcess;

class CTextSource : public CBase , public MTextSource
	{
public:
	CTextSource(CTextEdDocument* aDoc,CApaProcess* aProcess); // aProcess=NULL if main document
	~CTextSource();
	TInt Read(TDes& aBuf,TInt aPos);
	TInt Read(TDes& aBuf);
	void Close(); // will "delete this"
private:
	CTextEdDocument* iDoc;
	CApaProcess* iProcess;
	TInt iCurrentPos;
	};

#endif