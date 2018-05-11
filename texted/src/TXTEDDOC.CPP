// TXTEDDOC.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <txtglobl.h>
#include <s32file.h>
#include <eikenv.h>
#include <eikproc.h>
#include <eikapp.h>
#include <prnsetup.h>
#include "texted.h"
#include "txtedpan.h"

//#define _USE_FILELOGGING
#include "debug.h"

//
// CTextEdDocument
//
CTextEdDocument::CTextEdDocument(CEikApplication& aApp)
	:CEikDocument(aApp),iProcNameArray(4),iProcPosArray(4)
	{
	FLOGWRITE(_L("CTextEdDocument ctor."));
	}

void CTextEdDocument::ConstructL(CParaFormatLayer* aParaFormat,CCharFormatLayer* aCharFormat,CPrintSetup* aDefaultPrintSetup)
	{
	FLOGWRITE(_L("CTextEdDocument::ConstructL() starts."));
	iPrintSetup=aDefaultPrintSetup; // must take ownership before we can leave
	iGlobalText=CGlobalText::NewL(aParaFormat,aCharFormat);
	FLOGWRITE(_L("CTextEdDocument::ConstructL() ends."));
	}

void CTextEdDocument::StoreL(CStreamStore& aStore,CStreamDictionary& aStreamDic) const
	{
	RStoreWriteStream stream;
	TStreamId streamId=stream.CreateLC(aStore);
	stream<<*iGlobalText;
	stream.CommitL();
	CleanupStack::PopAndDestroy(); // stream
	aStreamDic.AssignL(KUidTextEdApp,streamId);

	if (iPrintSetup)
		{
		TStreamId printSetupStreamId=iPrintSetup->StoreL(aStore);
		aStreamDic.AssignL(KUidPrintSetupStream,printSetupStreamId);
		}
	}

void CTextEdDocument::RestoreL(const CStreamStore& aStore,const CStreamDictionary& aStreamDic)
	{
	DoRestoreL(aStore,aStreamDic,iGlobalText,iPrintSetup);
	}

void CTextEdDocument::DoRestoreL(const CStreamStore& aStore,const CStreamDictionary& aStreamDic,CGlobalText* aGlobalText,CPrintSetup* aPrintSetup)
	{
	TStreamId streamId=aStreamDic.At(KUidTextEdApp);
	RStoreReadStream stream;
	stream.OpenLC(aStore,streamId);
	stream>>*aGlobalText;
	CleanupStack::PopAndDestroy(); // stream

	if (iPrintSetup)
		{
		TStreamId printSetupStreamId=aStreamDic.At(KUidPrintSetupStream);
		if (printSetupStreamId!=KNullStreamId)
			{
			CTxtedAppUi* appUi=STATIC_CAST(CTxtedAppUi*,iAppUi);
			aPrintSetup->RestoreL(aStore,printSetupStreamId,appUi,appUi,CEikonEnv::Static()->PictureFactory());
			}
		}
	SetDocChanged(EFalse);
	}

void CTextEdDocument::DoNewFileL(const TFileName& aFileName)
	{
	DoSaveToCurrentFileL();
	CTextEdDocument* doc = (CTextEdDocument*)(Process()->AddNewDocumentL(iAppUi->Application()->AppFullName()));
	CFileStore* store=NULL;
	TRAPD(err,
		{
		store=doc->CreateFileStoreLC(Process()->FsSession(),aFileName);
		doc->NewDocumentL();
		CleanupStack::Pop(); // store
		});
	if (err!=KErrNone)
		{
		Process()->DestroyDocument(doc);
		User::Leave(err);
		}
	// cannot leave after this
	doc->SetAppFileMode(EFileWrite);
	Process()->SetMainDocument(doc);
	Process()->SetMainDocFileName(aFileName);
	delete iEikProcess->MainStore();
	iEikProcess->SetMainStore(store);
	//doc->SetDocChanged(EFalse);
	//doc->iProcArrayValid=EFalse;
	doc->iAppUi=iAppUi;
	iAppUi->SetDocument(doc);
	Process()->DestroyDocument(this); // deletes this
	}

void CTextEdDocument::DoOpenFileL(const TFileName& aFileName)
	{
	DoSaveToCurrentFileL();
	CFileStore* store=NULL;
	CStreamDictionary* dict=NULL;	//hp: Now needed for OpenNewDocumentL(), and use mode EFileShareExclusive|EFileWrite
	CTextEdDocument* doc=NULL;
	TInt fileMode=EFileWrite;
	TRAPD(err,doc=(CTextEdDocument*)(Process()->OpenNewDocumentL(store,dict,aFileName,EFileShareExclusive|fileMode)));
	if (err==KErrAccessDenied || err==KErrInUse)
		{
		fileMode=EFileRead;
		doc=(CTextEdDocument*)(Process()->OpenNewDocumentL(store,dict,aFileName,EFileShareReadersOnly|fileMode));
		}
	else
		User::LeaveIfError(err);
	delete dict; //hp
	doc->SetAppFileMode(fileMode);
	Process()->SetMainDocument(doc);
	Process()->SetMainDocFileName(aFileName);
	delete iEikProcess->MainStore();
	iEikProcess->SetMainStore(store);
	//doc->SetDocChanged(EFalse);
	//doc->iProcArrayValid=EFalse;
	doc->iAppUi=iAppUi;
	iAppUi->SetDocument(doc);
	Process()->DestroyDocument(this); // deletes this
	}

void CTextEdDocument::DoSaveToCurrentFileL()
	{
	if (HasChanged())
		SaveL();
	}

void CTextEdDocument::DoSaveToNewFileL(const TFileName& aNewFileName)
	{
	TInt ret=CEikonEnv::Static()->FsSession().Delete(aNewFileName); // incase it exists already
	if (ret!=KErrNone && ret!=KErrNotFound)
		User::Leave(ret);
	iEikProcess->SaveToDirectFileStoreL(this,&aNewFileName); // writes root stream
	SetAppFileMode(EFileWrite);
	SetDocChanged(EFalse);
	}

void CTextEdDocument::DoFileRevertL(CGlobalText* aTempGlobalText,CPrintSetup* aTempPrintSetup)
	{
	CleanupStack::PushL(aTempGlobalText);
	CleanupStack::PushL(aTempPrintSetup);
	RStoreReadStream root;
	CStreamDictionary* streamDic=CStreamDictionary::NewL();
	CleanupStack::PushL(streamDic);
	CFileStore* aMainStore=iEikProcess->MainStore();
	root.OpenLC(*aMainStore,aMainStore->Root());
	root>> *streamDic;
	root.Close();
	CleanupStack::PopAndDestroy(); // root
	DoRestoreL(*aMainStore,*streamDic,aTempGlobalText,aTempPrintSetup);
	CleanupStack::PopAndDestroy(); // streamDic
	delete iGlobalText;
	delete iPrintSetup;
	iGlobalText=aTempGlobalText;
	iPrintSetup=aTempPrintSetup;
	CleanupStack::Pop(2); // aTempGlobalText,aTempPrintSetup
	SetDocChanged(EFalse);
	iProcArrayValid=EFalse;
	}

void CTextEdDocument::SetDocChanged(TBool aState)
	{
	iIsNewDoc=EFalse;
	SetChanged(aState);
	if (aState)
		iProcArrayValid=EFalse;
	}

void CTextEdDocument::NewDocumentL()
	{
	iGlobalText->Reset();
	// Inserting elements at top of document in reverse order.
	_LIT(KProcText2,"ENDP");
	iGlobalText->InsertL(0,KProcText2);
	iGlobalText->InsertL(0,KTextTranLineDelimiter);
	iGlobalText->InsertL(0,CEditableText::ETabCharacter);
	iGlobalText->InsertL(0,KTextTranLineDelimiter);
	_LIT(KProcText1,"PROC :");
	iGlobalText->InsertL(0,KProcText1);
	SetDocChanged(ETrue);
	iIsNewDoc=ETrue;
	}

const TInt KProcLength=4;
const TInt KMaxLineLength=256;
_LIT(KProc,"PROC");

MDesCArray* CTextEdDocument::ProcNameArrayL()
	{
	if (iProcArrayValid)
		return &iProcNameArray;

	TInt pos=0;
	TInt pParaEnd=0;
	TInt pWordEnd=0;
	TInt endPos=iGlobalText->DocumentLength();
	TBool found=EFalse;
	TPtrC ptr;
	TBuf<KMaxLineLength> line;
	iProcNameArray.Reset();
	iProcPosArray.Reset();
	TUint KStayStart=CPlainText::EScanStayIfBoundary|CPlainText::EScanToUnitStart|CPlainText::EScanJoinDelimiters;
	TUint KEnd=CPlainText::EScanToUnitEnd|CPlainText::EScanJoinDelimiters;
	while (pos<endPos)
		{
		iGlobalText->ScanWords(pos,KStayStart);
		if (pos==-1)
			break; // no more words
		pParaEnd=pos;
		iGlobalText->ScanParas(pParaEnd,KEnd);
//		if (pParaEnd==-1) // never fails to find a paragraph end
//			pParaEnd=endPos;
		pWordEnd=pos;
		iGlobalText->ScanWords(pWordEnd,KEnd);
		if ((pWordEnd-pos)==KProcLength)
			{
			ptr.Set(iGlobalText->Read(pos,KProcLength));
			if (ptr.Length()<KProcLength) // may be at segment boundry
				{
				iGlobalText->Extract(line,pos,KProcLength);
				ptr.Set(line);
				}
			if (ptr.CompareF(KProc)==0)
				{
				found=ETrue;
				iGlobalText->Extract(line,pos,Min(KMaxLineLength,(pParaEnd-pos)));
				// Lose the PROC<white-space> from the start of the line.
				TLex buf(line);
				buf.Inc(4); // skip over 'PROC'
				buf.SkipSpace(); // lose the space(s) and tabs.
				iProcNameArray.AppendL(buf.Remainder());
				iProcPosArray.AppendL(pos);
				}
			}
		pos=pParaEnd+1;
		}
	if (found)
		{
		iProcArrayValid=ETrue;
		return &iProcNameArray;
		}
	return NULL;
	}

TInt CTextEdDocument::ProcPosition(TInt aIndex)
	{
	__ASSERT_DEBUG(iProcArrayValid,Panic(ETextedPanicProcedureArrayInvalid));
	return iProcPosArray[aIndex];
	}

TInt CTextEdDocument::IndexOfPrevious(TInt aPos)
	{// NB returns zero if no previous proc pos
	__ASSERT_DEBUG(iProcArrayValid,Panic(ETextedPanicProcedureArrayInvalid));
	TInt ii=iProcPosArray.Count();
	while (ii>0)
		{
		if (iProcPosArray[--ii]<=aPos)
			break;
		}
	return ii;
	}

CEikAppUi* CTextEdDocument::CreateAppUiL()
	{
	return new(ELeave) CTxtedAppUi;
	}

CTextEdDocument::~CTextEdDocument()
	{
	delete iGlobalText;
	delete iPrintSetup;
	}
