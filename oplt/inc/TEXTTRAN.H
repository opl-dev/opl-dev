// TEXTTRAN.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Base classes & mixins which comprise the interfaces
// between Text Translators and the associated Text sources.

#ifndef __TEXTTRAN_H__
#define __TEXTTRAN_H__

#include <e32base.h>

const TInt KNarrowTextTranLineDelimiter=0x06;
#include <txtetext.h>
const TInt KUnicodeTextTranLineDelimiter=CEditableText::EParagraphDelimiter;
const TInt KTextTranLineDelimiter=KUnicodeTextTranLineDelimiter;



///////////////////////////////////////////////////////////////
//
// MTextSource
//
///////////////////////////////////////////////////////////////
class MTextSource
	{
public:
	virtual TInt Read(TDes& aBuf,TInt aPos)=0;
	virtual TInt Read(TDes& aBuf)=0;
	virtual void Close()=0;
	};

///////////////////////////////////////////////////////////////
//
// MTextSourceSystem -  machanism for gaining pointers to text sources.
//
//////////////////////////////////////////////////////////////
class MTextSourceSystem
	{
public:
	virtual TInt OpenSource(TDes& aFileName,MTextSource*& aTextSource)=0;
	};

///////////////////////////////////////////////////////////////
//
// TTranslateError - Error and Offset
//
////////////////////////////////////////////////////////////////
class TTranslateError
	{
public:
	inline void SetPosition(TInt aPosition);
	inline void SetError(TInt anError);
	
	inline TInt Position() const;
	inline TInt Error() const;	

//	virtual void ErrorText(TDes& aBuf)=0;
private:
	TInt iError;
	TInt iPosition;
	};

class TSourceTranslateError : public TTranslateError
	{
public:	
	inline void SetSource(const TDesC& aSourceName);
	inline const TDesC& Source() const;
private:
	TFileName iSource;
	};

	///////////////////////////////////////////////////////////////
//
// CTextTranslator
//
///////////////////////////////////////////////////////////////
class CTextTranslator : public CBase
	{
public:
	enum TDebugFlag
		{
		ERelease,
		EDebug
		};
	virtual void StartTranslateL(MTextSourceSystem& aSourceSystem,
							const TFileName& aSourceName,
							const TFileName& aTargetName,
							TDebugFlag aDebugMode,
							TSourceTranslateError& anError)=0; 
	virtual TInt TranslateChunk()=0;
	virtual void Cancel()=0;
	};

#include <texttran.inl>
#endif
