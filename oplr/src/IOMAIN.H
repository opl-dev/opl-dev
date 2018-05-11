// IOMAIN.H
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#ifndef IOMAIN_H
#define IOMAIN_H

#ifndef __FEPBASE_H__
#include <fepbase.h>
#endif

#ifndef __OPLAPI_H__
#include <oplapi.h>
#endif

typedef TInt16 TOplReqStatus;
const TInt16 KIOHandleBase=1;
const TInt KMaxIOObjects=32;

//
// Class TIORequest - this class is not derivable outside of this DLL
// as it's constructor is not exported
//
class TIORequest
	{
public:
	TIORequest(TAny* aPtr, TInt aPriority);
	inline TRequestStatus& Status() {return iStatus;}
	inline TAny* Ptr() {return iStatusPtr;}
	inline TInt Priority() const {return iPriority;}
	void HandleCompletionIfNecessary();
private:
	virtual void DoHandleCompletion()=0;
protected:
	TRequestStatus iStatus;
	TAny* iStatusPtr;
private:
	const TInt iPriority;
	TBool iHasHandledCompletion;
	};

class TOplIORequest : public TIORequest
	{
public:
	TOplIORequest(TOplReqStatus* aStatPtr, TInt aPriority);
	virtual void DoParamsUpdate();
private:
	virtual void DoHandleCompletion();
	};

class COplIO : public CBase
	{
public:
	COplIO(TInt16 aHandle) {iHandle=aHandle;};
	virtual void ConstructL(TBuf<256>& /*devName*/, TInt16 /*aMode*/) {};
	virtual void RunFunctionL(TInt aFuncNo, TIORequest*& aIOStatus, TOplReqStatus* aStatusPtr,  TAny* aParam1,  TAny* aParam2) =0;
protected:
	TInt16 iHandle;
	};

class CIOCollection;

#ifdef __WINS__
	#define KKillKeyModifier	EModifierShift
#else
	#define KKillKeyModifier	EModifierCtrl
#endif
#define KPauseKeyModifier	(EModifierCtrl|EModifierFunc)

class CCoeEnv;
class CEventQueue;
class CSignalBuffer;
class TArrayOfAdditionalEventSources;
class COplActiveScheduler;
class CPriorityKeyHandler;

class CWsEventHandler : public CBase
	{
public:
	CWsEventHandler(RWsSession& aWsSession,CIOCollection* aIOCollection);
	void ConstructL(CCoeEnv& aCoeEnv);
	~CWsEventHandler();
	void Start();
	void Stop();
#if defined(_DEBUG)
	TBool IsStarted() const;
#endif
	static TInt HandleSpecialWsEvent(const TWsEvent& aWsEvent);
	inline TBool IsSuspended() const {return iIsSuspended;}
	inline void SetSuspended() {iIsSuspended=ETrue;}
	inline void SetNotSuspended() {iIsSuspended=EFalse;}
	void PrepareToStartActiveScheduler(CCoeEnv& aCoeEnv);
	void NotifyActiveSchedulerStopped();
	TInt16 DoGet(CCoeEnv& aCoeEnv);
	TUint DoGetUnmapped(CCoeEnv& aCoeEnv);
	TInt16 DoKey(CCoeEnv& aCoeEnv);
	TInt16 DoKmod();
	void DoCancelOplAsynchronousWservRequest(TInt16* aStatPtr);
	void DoCancelOplAsynchronousWservRequestWithoutConsumingSignal(TInt16* aStatPtr);
	void DoKeyA(CCoeEnv& aCoeEnv,TInt16* aStatPtr,TInt16* aArrayPtr);
	void DoGetEvent(CCoeEnv& aCoeEnv,TInt16* aArrayPtr); // opl1993 style
	void DoGetEvent(CCoeEnv& aCoeEnv,TInt32* aArrayPtr); // opler1 style
	void DoGetEventA(CCoeEnv& aCoeEnv,TInt16* aStatPtr,TInt32* aArrayPtr);
	TBool DoTestEvent(CCoeEnv& aCoeEnv);
	void DoConsumeEvent(CCoeEnv& aCoeEnv);
	void DoPauseL(CCoeEnv& aCoeEnv, TInt16 aDelay);
	void DoIoSignal();
	static void Signal(TInt aNumberOfSignals);
	TRequestStatus& PriorityKeyStatus();
	void HandlePriorityKeyCompletion();
	void AppendKeyEventToQueueL(TUint aKeyCode,TUint aModifiers);
	TPtrC Command() const;
	void ClearCommand();
	void HandleAnyCompletedEvents(CCoeEnv& aCoeEnv, TInt aMaximumNumberOfCompletedEventsToHandle);
	void WaitWhileSuspended(CCoeEnv& aCoeEnv);
	void WaitForAnyOplAsynchronousRequestCompletion(CCoeEnv& aCoeEnv);
	void WaitForOplAsynchronousRequestCompletion16(CCoeEnv& aCoeEnv, TAny* aRequestStatus16);
	void WaitForOplAsynchronousRequestCompletion32(CCoeEnv& aCoeEnv, TAny* aRequestStatus32);
	void WaitForEvent(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired);
	void WaitForEvent(CCoeEnv& aCoeEnv, TRequestStatus& aRequestStatus, TInt aPriority);
	void WaitForEvent(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired, const TArray<MEventSource*>& aArrayOfAdditionalEventSources);
private:
	enum
		{
		EUserEvents			=0x000000ff,
		// The values below are for bitwise OR-ing together and with any *one*
		// TUserEventsRequired value, and passed into the aEventsRequired parameter of
		// the low-level WaitForEvent function (below). The value above is for obtaining
		// the TUserEventsRequired part by masking out the bits below.
		EPriorityKeyEventsTerminatingSuspendedState	=0x00000100,
		EOplAsynchronousRequestCompletion			=0x00000200
		};
	struct SEventHandler
		{
		struct SParameters
			{
			SParameters(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUint aEventsRequired, TArrayOfAdditionalEventSources& aArrayOfAdditionalEventSources, const TInt* aMaximumNumberOfCompletedEventsToHandle);
			CCoeEnv& iCoeEnv;
			TBool iDone;
			CCoeFep* iFep;
			TWsEvent& iEvent;
			const TUint iEventsRequired;
			TArrayOfAdditionalEventSources& iArrayOfAdditionalEventSources;
			const TInt* const iMaximumNumberOfCompletedEventsToHandle;
			TInt iNumberOfCompletedEventsHandled;
			TInt iNumberOfSignalsToReplace;
			const MEventSource* iCompletedAdditionalEventSourceWithHighestPriority;
			const TIORequest* iCompletedOplAsynchronousRequestWithHighestPriority;
			};
		typedef CCoeFep::TEventResponse (*FOfferEventFunction)(CWsEventHandler& aThis, SParameters& aParameters);
		TInt iPriority;
		FOfferEventFunction iOfferEventFunction;
		};
private:
	void WaitForEvent(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUint aEventsRequired, const TArray<MEventSource*>& aArrayOfAdditionalEventSources, const TInt* aMaximumNumberOfCompletedEventsToHandle);
	CCoeFep::TEventResponse OfferEventToPriorityKeyHandler(SEventHandler::SParameters& aParameters);
	static void SetUpArrayOfEventHandlers(TFixedArray<SEventHandler, 4>& aArrayOfEventHandlers, const MEventSource* aCompletedAdditionalEventSourceWithHighestPriority, const TIORequest* aCompletedOplAsynchronousRequestWithHighestPriority);
	static CCoeFep::TEventResponse FinalOfferEventFunction(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters);
	static CCoeFep::TEventResponse OfferEventToArrayOfAdditionalEventSources(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters);
	static CCoeFep::TEventResponse OfferEventToOplAsynchronousRequest(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters);
	static CCoeFep::TEventResponse OfferEventToUserEventHandler(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters);
	CCoeFep::TEventResponse OfferEventToActiveScheduler(CCoeFep* aFep, TInt aMinimumPriority);
	CCoeFep::TEventResponse OfferEventToFep(CCoeFep& aFep, const TWsEvent& aEvent);
	void HandleError(TInt aError);
	TBool EventCompletesAnyOplAsynchronousWservRequest(CCoeEnv& aCoeEnv, const TWsEvent& aEvent);
	TBool IsEventReady(CCoeEnv& aCoeEnv, TWsEvent* aEvent, TUserEventsRequired aUserEventsRequired);
	TInt16 MapToOplMod(TUint aMod);
	TBool DoGetEventArrayWrite(CCoeEnv& aCoeEnv, const TWsEvent& aEvent,TInt32* aArray); //opler1
	TInt16 MapKeys(TUint aKeyCode);
	void SetCommand(const TWsEvent& aEvent);
	TBool IsCreateOrOpenMessage(CCoeEnv& aCoeEnv, const TWsEvent& aEvent);
	TBool IsCreateOrOpenMessageL(CCoeEnv& aCoeEnv, const TWsEvent& aEvent);
private:
	COplActiveScheduler* iOplActiveScheduler;
	RWsSession& iWs;
	CIOCollection* iIOCollection;
	TInt iConsoleId;
	TRequestStatus iStatus;
	TBool iKeysOnly;
	TInt16* iStatPtr;
	TAny* iArrayPtr;
	TUint iKMod;
	CEventQueue* iEventQueue;
	CSignalBuffer* iSignalBuffer;
	CPriorityKeyHandler* iPriorityKeyHandler;
	TBool iIsSuspended;
	HBufC* iCommand;
	CCoeControl *iFepControl; // Cache the Series 60 FEP. 
	};

#ifndef __OPLR_H__
class COplRuntime;
#endif

class CIOCollection : public CBase
	{
public:
	void ConstructL(COplRuntime* aRuntime,RWsSession& aWsSession);
	TInt16 NewObject(TInt16* aHandlePtr,TPtrC aPtr, TInt16 aMode);
	TInt16 NewObject(TInt16* aHandlePtr,TPtr& aPtr, TInt16 aMode);
	void RemoveObject(TInt16 aHandle);
	TInt16 DoIOA(TInt16 aHandle,TInt16 aFuncNo,TOplReqStatus* aOplStatPtr, TAny* aParam1, TAny* aParam2);
	TInt16 DoIow(CCoeEnv& aCoeEnv,TInt16 aHandle,TInt16 aFuncNo, TAny* aParam1, TAny* aParam2);
	const TIORequest* HandleAnyCompletedOplAsynchronousRequests(); // returns the completed-request with the highest priority (if there are no completed requests, it returns NULL) // N.B. this function does *not* handle completed KEYA or GETEVENTA32 requests
	void RemoveCompletedOplAsynchronousRequest(const TIORequest* aRequest);
	inline TInt16 LHandle() {return iLHandle;}
	inline void SetLHandle(TInt16 aHandle) {iLHandle=aHandle;}
	TRequestStatus& NewRequestL(TOplReqStatus* aStatusPtr,TInt aPriority,TCallBack aCallBack);
	TRequestStatus& NewRequestL(TInt32* aStatusPtr,TInt aPriority,TCallBack aCallBack);
	inline CWsEventHandler& WsEventHandler() {return *iWsEventHandler;}
	~CIOCollection();
private:
	enum TOplIOType {EFile,EFileText,EComm,ETimer};
private:
	TInt16 DoNewObject(TInt16* aHandlePtr,TBuf<256>& aDevName, TInt16 aMode);
	COplIO* FindObjectL(TInt16 aHandle);
	TOplIOType GetType(TBuf<256>& devName,TInt16 aMode);
	TInt FindSlot();
	void HandleConsoleRequest(TInt16 aFuncNo,TRequestStatus& aRequestStatus,TAny* aParam1,TAny* aParam2);
private:
	COplIO* iObjArray[KMaxIOObjects];
	RPointerArray<TIORequest> iRequestArray;
	TInt16 iLHandle;
	CWsEventHandler* iWsEventHandler;
	COplRuntime* iRuntime;
	};

#endif