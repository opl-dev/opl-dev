// TEXTFILE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Some classes for turning files into MTextSource objects.
//
#ifndef __TEXTFILE_H__
#define __TEXTFILE_H__

#include <texttran.h>


////////////////////////////////////////////////////////////
//
// ROplTextSystem
//
////////////////////////////////////////////////////////////
class ROplTextSystem : public RFs, public MTextSourceSystem
	{
public:
	TInt OpenSource(TDes& aFileName, MTextSource*& aSource);	
	};

////////////////////////////////////////////////////////////
//
// CFileSource - Base class for all ROplTextSystem files
//
////////////////////////////////////////////////////////////
class CFileSource : public CBase, public MTextSource
	{
	friend class COplTextSystem;
public:
	TInt Read(TDes& aDes, TInt aPos);
	TInt Read(TDes& aDes);
	void Close();
protected:
	CFileSource();
	~CFileSource();
	virtual void OpenL(RFs& aFS,const TDesC& aFile)=0;
protected:
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
protected:
	void OpenL(RFs& aFs,const TDesC& aFile);
	};




#endif
