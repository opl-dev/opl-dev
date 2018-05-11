// OPLAPI.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __OPLAPI_H__
#define __OPLAPI_H__

#include <e32base.h>
#include <charconv.h>

enum TReturnType
	{
	EReturnFloat=0,
	EReturnInt='%',
	EReturnLong='&',
	EReturnString='$'
	};

class CStack;
class CIOCollection;
class RHeap;
class RWsSession;
class RBackedUpWindow;
class RWindowGroup;
class COplRuntime;
class COplDbManager;
class CWsScreenDevice;

struct SCallbackInfo
	{
	TAny* iStackPtr;
	TInt16 iParamCount;
	};

enum TUserEventsRequired
	{
	EUserEventsNone,
	EUserEventsKeysOnly,
	EUserEventsAny
	};

class MEventSource
	{
public:
	virtual void CancelRequest()=0;
	virtual TRequestStatus& RequestStatus()=0;
	virtual TInt Priority() const=0;
private:
	IMPORT_C virtual void MEventSource_Reserved1();
	IMPORT_C virtual void MEventSource_Reserved2();
	};

class TWsEvent;
class CEikonEnv;
class OplAPI : public CBase
	{
public:
	OplAPI(COplRuntime* aRuntime,RHeap& aHeap);
	// Functions forwarded to CStack
	IMPORT_C void Push(TInt16 aInt16);
	IMPORT_C void Push(TInt32 aInt32);
	IMPORT_C void Push(const TReal64 aReal);
	IMPORT_C void PushL(const TText16* aLbc);
	IMPORT_C void PushL(const TDesC16& aDes);
	IMPORT_C TInt16 PopInt16();
	IMPORT_C TInt32 PopInt32();
	IMPORT_C TReal64 PopReal64();
	IMPORT_C TPtrC PopString();
	IMPORT_C TInt16* PopPtrInt16();
	IMPORT_C TInt32* PopPtrInt32();
	IMPORT_C TReal64* PopPtrReal64();
	IMPORT_C TAny* StackPtr() const;
	IMPORT_C void SetStackPtr(const TAny* aSP);
	IMPORT_C TInt FreeBytes() const;
	// Functions forwarded to OplUtil
	// - these are for getting and putting values to non-aligned addresses
	IMPORT_C TUint8* OffsetToAddrL(TInt aOffset,TInt aSize);
	IMPORT_C static void PutLong(TAny* aPtr,TInt32 aLong);
	IMPORT_C static TInt32 GetLong(TAny* aPtr);
	IMPORT_C static void PutFloat(TAny* aPtr,TReal64 aFloat);
	IMPORT_C static TReal64 GetFloat(TAny* aPtr);
	IMPORT_C static void PutWord(TAny* aPtr,TInt16 aWord);
	IMPORT_C static TInt16 GetWord(TAny* aPtr);
	IMPORT_C static TInt16 MapError(TInt aE32Error);
	IMPORT_C static TInt MapTranslatorError(TInt aErr,TInt& aResId);
	// - arithmetic with overflow checking
	IMPORT_C static TInt16 MultiplyL(TInt16 aArg1,TInt16 aArg2);
	IMPORT_C static TInt32 MultiplyL(TInt32 aArg1,TInt32 aArg2);
	// - Database interface
	IMPORT_C COplDbManager* DbManager();
	IMPORT_C void OpenCheckL();
	// - WSERV interface
	IMPORT_C RWsSession& WsSession();
	IMPORT_C RWindowGroup& RootWindow();
	IMPORT_C RBackedUpWindow& WindowFromIdL(TInt aId);
	IMPORT_C CWsScreenDevice* ScreenDevice();
	//
	IMPORT_C TInt BitmapHandleFromIdL(TInt aId);
	// - Eikon environment
	IMPORT_C CEikonEnv& EikonEnv() const;
	// - for doing callbacks
	IMPORT_C void InitCallbackL(TDesC16& aProcName);
	IMPORT_C void PushParamL(TInt16 aInt);
	IMPORT_C void PushParamL(TInt32 aLong);
	IMPORT_C void PushParamL(TReal64 aFloat);
	IMPORT_C void PushParamL(const TDesC16& aString);
	IMPORT_C TInt CallProcedure(TReturnType aReturnType);
	// AppArch
	IMPORT_C TUid AppUid();
	// Asynchronous programming support
	IMPORT_C TRequestStatus& NewRequestL(TInt16* aUserStatusPtr,TInt aPriority,TCallBack aCallBack);
	IMPORT_C TRequestStatus& NewRequestL(TInt32* aUserStatusPtr,TInt aPriority,TCallBack aCallBack);
	// Waiting
	IMPORT_C void WaitForEvent(TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired);
	IMPORT_C void WaitForEvent(TRequestStatus& aRequestStatus, TInt aPriority);
	IMPORT_C void WaitForEvent(TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired, const TArray<MEventSource*>& aArrayOfAdditionalEventSources);
	// Character conversion access
	IMPORT_C HBufC16* ConvertFromNarrowToUnicodeL(const TDesC8& aNarrowString,const TUint aCharacterSet=KCharacterSetIdentifierAscii) const;
	IMPORT_C HBufC16* ConvertFromNarrowToUnicodeLC(const TDesC8& aNarrowString,const TUint aCharacterSet=KCharacterSetIdentifierAscii) const;
	IMPORT_C HBufC8* ConvertFromUnicodeToNarrowL(const TDesC16& aUnicodeString,const TUint aCharacterSet=KCharacterSetIdentifierAscii) const;
	IMPORT_C HBufC8* ConvertFromUnicodeToNarrowLC(const TDesC16& aUnicodeString,const TUint aCharacterSet=KCharacterSetIdentifierAscii) const;
	// Additional Info
	IMPORT_C TPtrC AppCurrentDocument();
	IMPORT_C TPtrC AppLocation();
private:
	void ResetStack();
	COplRuntime* iRuntime;
	CStack* iStack;
	RHeap& iHeap;
	CArrayFixSeg<SCallbackInfo> iCallbackInfoArray;
	};

#endif