// OPLTRANT.H
//
// Copyright (c) 1998-2001 Symbian Ltd. All rights reserved.

#ifndef __OPLTRANT_H__
#define __OPLTRANT_H__

#include <texttran.h>

// Convertor flag values.
enum TConvertFlag {ENoConvert=0,EConvert};

////////////////////////////////////////////////////////////
//
// ROplTextSystem
//
////////////////////////////////////////////////////////////
class ROplTextSystem : public RFs, public MTextSourceSystem
	{
public:
	TInt OpenSource(TDes& aFileName, MTextSource*& aSource);
	TBool IsOplSourceFileL(const TDesC& aFilename);
private:
	TInt DoOpenSource(const TDesC& aFileName, MTextSource*& aSource);
	};

////////////////////////////////////////////////////////////
//
// CFileSource - Base class for all ROplTextSystem files
//
////////////////////////////////////////////////////////////
class CFileSource : public CBase, public MTextSource
	{
	friend class COplTextSystem;
	friend class ROplTextSystem;
public:
	TInt Read(TDes& aDes, TInt aPos);
	TInt Read(TDes& aDes);
	void Close();
	virtual void MapPosToLineAndOffsetL(RFs& aFs,const TDesC& aFilename,const TInt aPos, TInt& aLine, TInt& aOffset)=0;
	virtual TInt ConvertL(RFs& aFs,const TDesC& aSourceName)=0;

	 CFileSource(TConvertFlag aFlag);
protected:
	~CFileSource();
	virtual void OpenL(RFs& aFS,const TDesC& aFile)=0;
protected:
	TConvertFlag iConvert; 
	TInt iPos;
	TPtr iContents;
	};

/////////////////////////////////////////////////////////////////
//
// CCrLfTextSource
//
/////////////////////////////////////////////////////////////////
class CCrLfTextSource : public CFileSource
	{
	friend class ROplTextSystem;
public:
	inline CCrLfTextSource(TConvertFlag aFlag) : CFileSource(aFlag) {};
	void MapPosToLineAndOffsetL(RFs& aFs,const TDesC& aFilename,const TInt aPos, TInt& aLine, TInt& aOffset);
	virtual TInt ConvertL(RFs& aFs,const TDesC& aSourceName);
	void SaveL(RFs& aFs,const TDesC& aFile);
	void StoreL(CStreamStore& aStore,CStreamDictionary& aStreamDic);
	void ConvertChars(TDes16& aUnicode,const TChar aPrevChar, const TChar aNewChar);
protected:
	void OpenL(RFs& aFs,const TDesC& aFile);
	};

/////////////////////////////////////////////////////////////////
//
// CEpocTextSource
//
/////////////////////////////////////////////////////////////////
class CEpocTextSource : public CFileSource
	{
	friend class ROplTextSystem;
public:
	inline CEpocTextSource(TConvertFlag aFlag) : CFileSource(aFlag) {};
	void MapPosToLineAndOffsetL(RFs& aFs,const TDesC& aFilename,const TInt aPos, TInt& aLine, TInt& aOffset);
	virtual TInt ConvertL(RFs& aFs,const TDesC& aSourceName);
	TInt SizeL(RFs& aFs,const TDesC& aFileName);
	void ConvertChars(TDes16& aUnicode, const TChar aPrevChar, const TChar aNewChar);
protected:
	void OpenL(RFs& aFs,const TDesC& aFile);
	};

//
// Filename mangling -- Windows uses a non-Unicode file system
//
typedef TBuf8<KMaxFileName> TFileName8;

#endif