// OPLREADR.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Classes for reading OPL files.
//

#ifndef __OPLREADR_H__
#define __OPLREADR_H__

#include <s32file.h>
#include <s32mem.h>


// EPOC Application Architecture 
// OPL Uids

const TUid KUidOplAppNoFile=	{0x10000072};
const TUid KUidOplObj=			{0x100055C0};
const TUid KUidOplApp=			{0x100055C1};
const TUid KUidOplDoc=			{0x100055C2};
const TUid KUidOplInterpreter=	{0x10005D2E};


class COpoApp : public CBase
	{
public:
	TBool operator ==(COpoApp& anApp);
	TBool operator !=(COpoApp& anApp);
	enum TType
		{
		EOpl1993,
		EOpler1
		};
	virtual TType Type() const=0;
protected:
	virtual TBool Equals(COpoApp& anApp) const=0;
	};

class COpl1993App : public COpoApp
	{
public:
	static COpl1993App *NewL(const TDesC8& anAppData);
	~COpl1993App();
	TType Type() const;
protected:
	TBool Equals(COpoApp& anApp) const;
private:
	HBufC8 *iData;
	};


class COpler1App : public COpoApp
	{
public:
	static COpler1App *NewL(const TUid& aUid);
	TType Type() const;
protected:
	TBool Equals(COpoApp& anApp) const;
private:
	COpler1App(const TUid& aUid);
	TUid iUid;
	};

class COpoOpx : public CBase
	{
public:
	static COpoOpx *NewLC(RReadStream& aStream);
	~COpoOpx();
	TBool operator ==(COpoOpx& anOpx);
private:
	COpoOpx();
	void ConstructL(RReadStream& aStream);
private:
	HBufC *iName;
	TUid  iUid;
	TUint iVersion;
	};

class COpoProc : public CBase
	{
public:
	static COpoProc *NewLC(RReadStream& aStream); // Gets the name
	~COpoProc();
	TBool operator ==(COpoProc& aProc);
	inline void SetLineNumber(TUint aLineNumber) { iLineNumber=aLineNumber;}
	inline const TDesC& Name() const { return *iName;}
	void ReadCodeL(RReadStream& aStream);
private:
	void ConstructL(RReadStream& aStream);
private:
	HBufC* iName;
	TInt iLineNumber;
	TUint iTotalDataSize;
	HBufC8* iHeader;
	HBufC8* iQcode;
	};

const TUint KOpoOpxGranularity=16;
const TUint KOpoProcGranularity=16;
class COpo : public CBase
	{
public:
	static COpo *NewLC(RFs& aFileSession, const TDesC& afileName);
	~COpo();
	TBool operator ==(COpo& anOpo);
	TBool operator !=(COpo& anOpo);
	enum TType
		{
		EOpl1993,
		EOpler1
		};
protected:
	COpo();
	virtual void ConstructL(RFs& aFileSession, const TDesC& aFileName)=0;
	virtual COpo::TType Type() const=0;
	void ProceduresL(RReadStream& aProcList);
	virtual RReadStream* OpenProcStreamLC2(RReadStream& aProcList)=0;
protected:
	TUint iTranslatorVersion;
	TUint iRuntimeVersion;

	HBufC *iSourceName;
	COpoApp *iApp;
	CArrayFixFlat<COpoOpx *> iOpxList;
	CArrayFixFlat<COpoProc *> iProcList;
	};

class COpl1993Opo : public COpo
	{
protected:
	void ConstructL(RFs& aFileSession, const TDesC& aFileName);
	COpo::TType Type() const;
	RReadStream* OpenProcStreamLC2(RReadStream& aProcList);
private:
	HBufC8* iBuf;
	};

class COpler1Opl : public COpo
	{
protected:
	void ConstructL(RFs& aFileSession, const TDesC& aFileName);
	COpo::TType Type() const;
	RReadStream* OpenProcStreamLC2(RReadStream& aProcList);
private:
	CFileStore *iStore;
	};

#endif
