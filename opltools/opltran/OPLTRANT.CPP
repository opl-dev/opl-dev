// OPLTRANT.CPP
//
// Copyright (c) 1998-2002 Symbian Ltd. All rights reserved.

// Based on TEXTFILE.CPP
// Classes to read ASCII and Symbian OS native files into memory as MTextSources

#include <f32file.h>
#include <s32file.h> // file store
#include <txtglobl.h> // global text
#include <txtfmlyr.h> // paraformatlevel
#include <apparc.h> // streamdictionary

#include "winutil.h"
#include "opltrant.h"

GLREF_D TFileName IncludePath; // For INCLUDE'd files.
GLREF_D TFileName InputPath;
GLREF_D RFs theFileServer;
GLREF_D TFileName OutputPath;
GLREF_D TConvertFlag ConvertFlag;
GLREF_D TFileName EpocRoot;

const TUid KUidTextEdApp={0x10003A63};
_LIT(KSystemIncDir,"epoc32\\wins\\c\\System\\OPL\\");
_LIT(KTextIncDir,"epoc32\\winc\\OPL\\"); 

_LIT(KFileExtensionDot,".");
_LIT(KFileExtensionTpl,".tpl");
_LIT(KFileExtensionTph,".tph");
_LIT(KFileExtensionTxh,".txh");
_LIT(KFileExtensionTsg,".tsg");
_LIT(KFileExtensionOpl,".opl");
_LIT(KFileExtensionOph,".oph");
_LIT(KFileExtensionOxh,".oxh");
_LIT(KFileExtensionOsg,".osg");

/////////////////////////////////////////////////////////////////////////////////
//
// ROplTextSystem
//
/////////////////////////////////////////////////////////////////////////////////

//!!TODO demo.tpl comes up Error in StartTranslate: "not found"

TInt ROplTextSystem::OpenSource(TDes& aFileName, MTextSource*& aSource)
	{
	TParse name;
	TInt ret=KErrNoMemory;
	// Try the programs own folder
	name.Set(aFileName,&InputPath,NULL);
	ret=DoOpenSource(name.FullName(),aSource);
	if (ret!=KErrNone)
		{
		// Include folder from command line.
		name.Set(aFileName,&IncludePath,NULL);
		ret=DoOpenSource(name.FullName(),aSource);
		if (ret!=KErrNone)
			{
			// Text WINC folder.
			TFileName textIncPath(EpocRoot);
			textIncPath.Append(KTextIncDir);
			name.Set(aFileName,&textIncPath,&IncludePath); // Use IncludePath to get drive name.
			ret=DoOpenSource(name.FullName(),aSource);
			if (ret!=KErrNone)
				{
				// 'ROM'
				// Was Z: ROM, now it's C: System
				TFileName systemPath(EpocRoot);
				systemPath.Append(KSystemIncDir);
				name.Set(aFileName,&systemPath,&IncludePath);
				ret=DoOpenSource(name.FullName(),aSource);
				}
			}
		}
	aFileName=name.FullName();
	return ret;
	}

TBool ROplTextSystem::IsOplSourceFileL(const TDesC& aFileName)
//
// Check for Symbian OS native OplSourceUidType file.
//
	{
	CFileStore *store;
	const TInt fileMode=EFileStream|EFileRead|EFileShareReadersOnly;
	store=CDirectFileStore::OpenLC(theFileServer,aFileName,fileMode);
	const TUidType CheckUidType=store->Type();
	// Destroy the permanent file store object (closes the file) 
	CleanupStack::PopAndDestroy();//store
	TBool ret;
	//!!TODO is this TUidType ctored from the triple ctor?
	if (CheckUidType==TUidType(KDirectFileStoreLayoutUid,KUidAppDllDoc,KUidTextEdApp))
		ret=ETrue;
	else
		ret=EFalse;
	return ret;
	}

TInt ROplTextSystem::DoOpenSource(const TDesC& aFileName, MTextSource*& aSource)
	{
	CFileSource *pT;
	TBool nativeFormat=ETrue;
	TRAPD(err,(nativeFormat=IsOplSourceFileL(aFileName)));

	//test for missing file
	if (err==KErrNotFound || err==KErrPathNotFound)
		return err;

	// err may set for ASCII file.
	if ((err==KErrNone) && (nativeFormat))
		pT=new CEpocTextSource(ConvertFlag);
	else
		pT=new CCrLfTextSource(ConvertFlag);

	TInt ret=KErrNone;
	if (pT!=NULL)
		{	
		TRAP(ret,pT->OpenL(*this,aFileName));
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
	{
	delete this;
	}

CFileSource::CFileSource(TConvertFlag aFlag) : iContents(NULL,0),iConvert(aFlag)
	{
	}

CFileSource::~CFileSource()
//
// Deletes the buffer
// 
	{
	User::Free((TAny *)iContents.Ptr());
	}

/////////////////////////////////////////////////////////////////////////////////////////
//
// ASCII Text methods
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

	TPtr8 contents8((TText8 *)User::AllocL(size),0,size);

	// Read it in reasonable chunks replacing Cr with a KTextTranLineDelimiter
	// but skipping the Lf.
	TPtr8 readBuf((TUint8 *)User::AllocLC(0x1000*sizeof(TUint8)),0x1000); // push
	TChar c=NULL;
	FOREVER
		{
		TInt ret=file.iObj.Read(readBuf);
		TInt readLen=readBuf.Length();
		if (ret==KErrEof || readLen==0) // Cos read on an empty file is a bit odd.
			break;
		User::LeaveIfError(ret);
		for (TInt index=0;index<readLen;index++)
			{
			c=readBuf[index];

			if (c=='\n'); // skip it
			else
				{
				if (c=='\r')
					c=KNarrowTextTranLineDelimiter;
				contents8.Append(c);
				}
			}
		}
	// Make sure the last line ends correctly, for translation only!
	if (iConvert==ENoConvert && c!=KNarrowTextTranLineDelimiter)
		contents8.Append(KNarrowTextTranLineDelimiter);

	User::Free((TAny *)iContents.Ptr());
	iContents.Set((TText *)User::AllocL(sizeof(TText)*size),0,size);
	ConvertToUnicodeL(iContents,contents8);
	ConvertChars(iContents,KNarrowTextTranLineDelimiter,KUnicodeTextTranLineDelimiter);
	CleanupStack::PopAndDestroy(); // the alloc'd
	}

void CCrLfTextSource::MapPosToLineAndOffsetL(RFs& aFs,const TDesC& aFileName,
							 const TInt aErrorPos,TInt& aLine, TInt& aColOffset)
	{
	// aErrorPos uses normal indexing -- 0 for the first char etc.
	TAutoClose<RFile> file;
	User::LeaveIfError(file.iObj.Open(aFs,aFileName,EFileRead|EFileShareExclusive|EFileStreamText));
	TInt size=0;
	User::LeaveIfError(file.iObj.Size(size));

	// Allow for the CR we append 
	//!!TODO Support aErrorPos still being one char beyond the end of file!
	TInt errorPos=aErrorPos;
	if (errorPos>size)
		{
		if (errorPos==size+1)
			errorPos=size; // Hit error on last char in file.
		else
			User::Leave(KErrEof);
		}

	size=errorPos;
	// Read it in reasonable chunks replacing Cr with a KTextTranLineDelimiter
	// and remembering to fudge the lost Lf chars.
	TPtr8 readBuf((TUint8 *)User::AllocLC(0x1000*sizeof(TUint8)),0x1000);//push

	TChar c;
	TInt bufferRemaining=0;
	TInt index=0;
	TInt checkPos=0;
	aColOffset=0;
	aLine=1;
	FOREVER
		{
		// buffer check.
		if (bufferRemaining==0)
			{
			// buffer read, test for eof and set index to 0.
			index=0;
			TInt ret=file.iObj.Read(readBuf);
			bufferRemaining=readBuf.Length();
			if (ret==KErrEof||bufferRemaining==0)
				User::Leave(KErrEof);
			}
		c=readBuf[index++]; bufferRemaining--;
		aColOffset++;

		if (checkPos++==errorPos)
			break; // found it

		if (c=='\r') // CR
			{
			aLine++;
			aColOffset=0;
			}
		else if (c=='\n') //LF
			{ // Skip it.
			aColOffset--;
			checkPos--;
			}
		}
	CleanupStack::PopAndDestroy(); // the alloc'ed above.
	}

TInt CCrLfTextSource::ConvertL(RFs& aFs,const TDesC& aSourceName)
//
// Export iContents as Symbian OS document - weird form of export I know... 
//
	{
	// Determine the outputfile name, based on the source extension.
	//		.txh	-> .oxh
	//		.tph	-> .oph
	//		.tsg	-> .osg
	//		.tpl	->
	//		.opl	->
	TParse inputname;
	// Ensure the name has an extension.
	TFileName dot(KFileExtensionDot);
	inputname.Set(aSourceName,NULL,&dot);
	TBuf<4> sourceExtn,outputExtn;

	sourceExtn=inputname.Ext().Left(4);
	sourceExtn.LowerCase();
	if (sourceExtn.Find(KFileExtensionTxh)==0)
		outputExtn=KFileExtensionOxh;
	else if (sourceExtn.Find(KFileExtensionTph)==0)
		outputExtn=KFileExtensionOph;
	else if (sourceExtn.Find(KFileExtensionTsg)==0)
		outputExtn=KFileExtensionOsg;
	else if (sourceExtn.Find(KFileExtensionTpl)==0)
		outputExtn=KNullDesC;
	else if (sourceExtn.Find(KFileExtensionOpl)==0)
		outputExtn=KNullDesC;
	else
		outputExtn=KNullDesC; // default no extension

	TParse output;
	TInt r=theFileServer.Parse(outputExtn,OutputPath,output);
	if (r!=KErrNone)
		return r;

	aFs.Delete(output.FullName());
	TRAP(r,SaveL(aFs,output.FullName()));
	return r;
	}

void CCrLfTextSource::SaveL(RFs& aFs,const TDesC& aFileName)
	{
	CDirectFileStore* store=NULL;

	store=CDirectFileStore::CreateLC(aFs,aFileName,EFileShareExclusive|EFileWrite);//pushes store
	store->SetTypeL(TUidType(KDirectFileStoreLayoutUid,KUidAppDllDoc,KUidTextEdApp));
	// store main in temp
	CStreamDictionary* streamDic=CStreamDictionary::NewL();
	CleanupStack::PushL(streamDic);
	StoreL(*store,*streamDic);
	// write root stream
	TApaAppIdentifier fred;
	fred.iAppUid=KUidTextEdApp;
	CApaProcess::WriteRootStreamL(*store,*streamDic,fred);
	CleanupStack::PopAndDestroy(); // streamDic
	// close the new store
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store
	}

void CCrLfTextSource::StoreL(CStreamStore& aStore,CStreamDictionary& aStreamDic) 
	{
	RStoreWriteStream stream;
	TStreamId streamId=stream.CreateLC(aStore); // pushes stream
	// convert iContents to CGlobalText
	CParaFormatLayer* paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer=CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CGlobalText* textModel=CGlobalText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(textModel);
	textModel->InsertL(0,iContents);
	stream<<*textModel;
	stream.CommitL();
	CleanupStack::PopAndDestroy(4); // textModel,2xlayers,stream
	aStreamDic.AssignL(KUidTextEdApp,streamId);
	}

void CCrLfTextSource::ConvertChars(TDes16& aUnicode, const TChar aPrevChar, const TChar aNewChar)
	{ // convert one Unicode char to another.
	const TChar previousChar=aPrevChar;
	TBuf<1> newChar;
	newChar.Append(aNewChar);
	TInt location=KErrNotFound;
	while ((location=aUnicode.Locate(previousChar))!=KErrNotFound)
		{
		aUnicode.Replace(location,1,newChar);
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Symbian OS text methods
//
/////////////////////////////////////////////////////////////////////////////////////////

void CEpocTextSource::OpenL(RFs& aFs,const TDesC& aFileName)
	{
	TInt filesize=0;
	TRAPD(err,(filesize=SizeL(aFs,aFileName)));
	if (err!=KErrNone)
		return;

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

void CEpocTextSource::MapPosToLineAndOffsetL(RFs& aFs,const TDesC& aFileName,const TInt aErrorPos,
											 TInt& aLine, TInt& aOffset)
	{
	OpenL(aFs,aFileName);
	// Having filled iContents, parse it for the correct l+c
	if (aErrorPos>iContents.Length())
		User::Leave(KErrEof);
	
	aLine=1;
	aOffset=1;
	TInt readPos=0;
	TChar c;
	while (readPos<aErrorPos)
		{
		c=iContents[readPos++];
		aOffset++;
		if (c==KTextTranLineDelimiter)
			{
			aLine++;
			aOffset=1;
			}
		}
	}

TInt CEpocTextSource::ConvertL(RFs& aFs,const TDesC& aSourceName)
//
// Export the Symbian OS text as ASCII.
//
	{
	// Determine the outputfile name, based on the source extension.
	//		.oxh	-> .txh
	//		.oph	-> .tph
	//		.osg	-> .tsg
	//		.opl	-> .tpl
	//				-> .tpl
	TParse inputname;
	// Ensure the name has an extension.
	TFileName dot(KFileExtensionDot);
	inputname.Set(aSourceName,NULL,&dot);
	TBuf<4> sourceExtn,outputExtn;

	sourceExtn=inputname.Ext().Left(4);
	sourceExtn.LowerCase();
	if (sourceExtn.Find(KFileExtensionOxh)==0)
		outputExtn=KFileExtensionTxh;
	else if (sourceExtn.Find(KFileExtensionOph)==0)
		outputExtn=KFileExtensionTph;
	else if (sourceExtn.Find(KFileExtensionOsg)==0)
		outputExtn=KFileExtensionTsg;
	else if (sourceExtn.Find(KFileExtensionOpl)==0)
		outputExtn=KFileExtensionTpl;
	else // default
		outputExtn=KFileExtensionTpl;

	TParse output;
	TInt r=theFileServer.Parse(outputExtn,OutputPath,output);	
	User::LeaveIfError(r);

	// open the output file
	RFile TextFile;
	TUint mode=EFileShareExclusive|EFileStreamText|EFileWrite;
	r=TextFile.Replace(aFs,output.FullName(),mode);
	User::LeaveIfError(r);
	
	const TInt maximumLength=iContents.Length()*3; // the Unicode characters may be converted to multi-byte characters, hence the multiplication by 3 (which should be plenty)
	TPtr8 contents8((TText8 *)User::AllocL(maximumLength),0,maximumLength);
	// Lose the Unicode delimters.
	ConvertChars(iContents,KUnicodeTextTranLineDelimiter,KNarrowTextTranLineDelimiter);

	ConvertFromUnicodeL(contents8,iContents);
	
	const TInt len=contents8.Length();

	TInt pos=0;
	TInt count=0;
	TBuf8<0x3FF> writebuf(0x3FF);
	writebuf.FillZ(0x3FF);
	writebuf.SetMax();

	TText c;
	while (pos<len)
		{
		c=contents8[pos++];
		if (c==KNarrowTextTranLineDelimiter)
			{
			writebuf[count++]='\r';
			writebuf[count++]='\n';
			TextFile.Write(writebuf,count);
			count=0;
			}
		else
			writebuf[count++]=(char)c;
		}
	if (count)
		TextFile.Write(writebuf,count);

	TextFile.Flush();
	TextFile.Close();
	return r;
	}

void CEpocTextSource::ConvertChars(TDes16& aUnicode, const TChar aPrevChar, const TChar aNewChar)
	{ // convert one Unicode char to another.
	const TChar previousChar=aPrevChar;
	TBuf<1> newChar;
	newChar.Append(aNewChar);
	TInt location=KErrNotFound;
	while ((location=aUnicode.Locate(previousChar))!=KErrNotFound)
		{
		aUnicode.Replace(location,1,newChar);
		}
	}

TInt CEpocTextSource::SizeL(RFs& aFs,const TDesC& aFileName)
	{
	TAutoClose<RFile> file;
	User::LeaveIfError(file.iObj.Open(aFs,aFileName,EFileRead|EFileShareExclusive|EFileStreamText));
	TInt size=0;
	User::LeaveIfError(file.iObj.Size(size));
	size+=sizeof(TChar); // space for belts and braces terminator on the last line
	return size;
	}