// IOMAIN.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <oplr.h>
#include "iodef.h"
#include "iodev.h"
#include "oplutil.h"
#include <coemain.h>
#if !defined(__S80_DP2_0__) && !defined(__SERIES90__)
#include <eikkeys.h>
#endif
#include <apgtask.h>
#include "clckupdt.h"
#include <fepbase.h>
#include <e32def.h>
#if defined(__UIQ__)
#include <quartzkeys.h>
#endif
#if defined(__SERIES60__)
#include <aknappui.h>
#include <coecntrl.h>
#endif

_LIT(KLitC, "C");
_LIT(KLitO, "O");
_LIT(KLitF, "F");
_LIT(KLitX, "X");
_LIT(KLitS, "S");
_LIT(KLitR, "R");
_LIT(KLitU, "U");
_LIT(KOplIOSystem,"Opl IO system");

const TInt KShelfLifeOfUnhandledUserEvents=10*1000000; // 10 seconds
const TInt KMaxNestedDialogDepth=16; // Max number of nested dialogs that the runtime can handle.
enum
	{
	EOplEventUnknown = 0x1400
	};

class TSingleEventSource : public MEventSource
	{
public:
	inline TSingleEventSource(TRequestStatus& aRequestStatus, TInt aPriority) :iRequestStatus(aRequestStatus), iPriority(aPriority) {}
private:
	// from MEventSource
	virtual void CancelRequest();
	virtual TRequestStatus& RequestStatus();
	virtual TInt Priority() const;
private:
	TRequestStatus& iRequestStatus;
	TInt iPriority;
	};

void TSingleEventSource::CancelRequest()
	// This function should never get called as objects of this class should 
	// only ever be the *only* event source being waited on (aUserEventsRequired must be 
	// EUserEventsNone, aArrayOfAdditionalEventSources must only contain this object)
	{
#ifdef _DEBUG
	User::Panic(KOplIOSystem,6);
#endif
	}

TRequestStatus& TSingleEventSource::RequestStatus()
	{
	return iRequestStatus;
	}

TInt TSingleEventSource::Priority() const
	{
	return iPriority;
	}

//
// Class TTimerEventSource
//
class TTimerEventSource : public MEventSource
	{
public:
	inline TTimerEventSource(RTimer& aActiveTimer, TRequestStatus& aRequestStatus, TInt aPriority) :iActiveTimer(aActiveTimer), iRequestStatus(aRequestStatus), iPriority(aPriority) {}
private:
	// from MEventSource
	virtual void CancelRequest();
	virtual TRequestStatus& RequestStatus();
	virtual TInt Priority() const;
private:
	RTimer& iActiveTimer;
	TRequestStatus& iRequestStatus;
	TInt iPriority;
	};

void TTimerEventSource::CancelRequest()
	{
	iActiveTimer.Cancel();
	}

TRequestStatus& TTimerEventSource::RequestStatus()
	{
	return iRequestStatus;
	}

TInt TTimerEventSource::Priority() const
	{
	return iPriority;
	}

//
// Class CEventQueue
//
class CEventQueue : public CBase, public CCoeFep::MKeyEventQueue
	{
public:
	static CEventQueue* NewL();
	virtual ~CEventQueue();
	inline TBool IsNotEmpty() const {return (iQueue.Count()>iNumberOfEventsInBackupRegion);}
	inline TBool BackupIsEmpty() const {return (iNumberOfEventsInBackupRegion==0);}
	inline void PeekNextEvent(TWsEvent& aEvent) const {aEvent=iQueue[iNumberOfEventsInBackupRegion];}
	inline void ConsumeNextEvent() {RemoveEventAdjustingNumberOfExpiringEvents(iNumberOfEventsInBackupRegion);}
	inline void PutNextEventInBackup() {++iNumberOfEventsInBackupRegion;}
	void RestoreAllEventsFromBackup();
	void PurgeAllNonKeyEvents();
	void PrepareToStartActiveScheduler();
	void NotifyActiveSchedulerStopped();
	TInt AppendEvent(const TWsEvent& aEvent);
	// from CCoeFep::MKeyEventQueue
	virtual void AppendKeyEventL(const TKeyEvent& aKeyEvent);
private:
	class CTimeOutTimer : private CTimer
		{
	public:
		static CTimeOutTimer* NewL(CEventQueue& aEventQueue);
		void Activate();
		void Deactivate();
	private:
		CTimeOutTimer(CEventQueue& aEventQueue);
		virtual void RunL();
	private:
		CEventQueue& iEventQueue;
		};
	friend class CTimeOutTimer;
private:
	CEventQueue();
	void ConstructL();
	void RemoveEventAdjustingNumberOfExpiringEvents(TInt aIndex);
	void PurgeAllExpiringEvents();
private:
	RArray<TWsEvent> iQueue;
	CTimeOutTimer* iTimeOutTimer;
	TInt iNumberOfEventsInBackupRegion;
	TInt iNumberOfExpiringEvents;
	};

CEventQueue* CEventQueue::NewL()
	{
	CEventQueue* eventQueue=new(ELeave) CEventQueue;
	CleanupStack::PushL(eventQueue);
	eventQueue->ConstructL();
	CleanupStack::Pop(); // eventQueue
	return eventQueue;
	}

CEventQueue::~CEventQueue()
	{
	iQueue.Close();
	delete iTimeOutTimer;
	}

void CEventQueue::RestoreAllEventsFromBackup()
	{
	CWsEventHandler::Signal(iNumberOfEventsInBackupRegion);
	iNumberOfEventsInBackupRegion=0;
	}

void CEventQueue::PurgeAllNonKeyEvents()
	{
	__ASSERT_DEBUG(BackupIsEmpty(), User::Panic(KOplIOSystem, 24));
	for (TInt i=iQueue.Count()-1; i>=0; --i)
		{
		if (iQueue[i].Type()!=EEventKey)
			{
			RemoveEventAdjustingNumberOfExpiringEvents(i);
			User::WaitForAnyRequest();	// returns straightaway as we're consuming the signal
										// associated with the event that has just been taken
										// out of iQueue
			}
		}
	}

void CEventQueue::PrepareToStartActiveScheduler()
	{
	// Empty the queue and stop the timer for purging old unhandled events
	iNumberOfExpiringEvents=iQueue.Count();
	PurgeAllExpiringEvents();
	iTimeOutTimer->Deactivate();
	}

void CEventQueue::NotifyActiveSchedulerStopped()
	{
	iTimeOutTimer->Activate();
	}

TInt CEventQueue::AppendEvent(const TWsEvent& aEvent)
	{
	const TInt error=iQueue.Append(aEvent);
	if (error==KErrNone)
		{
		CWsEventHandler::Signal(1);
		}
	return error;
	}

void CEventQueue::AppendKeyEventL(const TKeyEvent& aKeyEvent)
	{
	TWsEvent event;
	event.SetType(EEventKey);
	event.SetHandle(NULL);
	event.SetTimeNow();
	*event.Key()=aKeyEvent;
	User::LeaveIfError(AppendEvent(event));
	}

CEventQueue::CEventQueue()
	:iQueue(10),
	 iNumberOfEventsInBackupRegion(0),
	 iNumberOfExpiringEvents(0)
	{
	}

void CEventQueue::ConstructL()
	{
	iTimeOutTimer=CTimeOutTimer::NewL(*this);
	iTimeOutTimer->Activate();
	}

void CEventQueue::RemoveEventAdjustingNumberOfExpiringEvents(TInt aIndex)
	{
	iQueue.Remove(aIndex);
	if (aIndex<iNumberOfExpiringEvents)
		{
		--iNumberOfExpiringEvents;
		}
	}

void CEventQueue::PurgeAllExpiringEvents()
	{
	for (TInt i=iNumberOfExpiringEvents-1; i>=0; --i)
		{
		iQueue.Remove(i);
		if (i>=iNumberOfEventsInBackupRegion)
			{
			User::WaitForAnyRequest();	// returns straightaway as we're consuming the signal
										// associated with the event that has just been taken
										// out of iQueue
			}
		}
	iNumberOfExpiringEvents=iQueue.Count();
	}

//
// Class CEventQueue::CTimeOutTimer
//
CEventQueue::CTimeOutTimer* CEventQueue::CTimeOutTimer::NewL(CEventQueue& aEventQueue)
	{
	CTimeOutTimer* timeOutTimer=new(ELeave) CTimeOutTimer(aEventQueue);
	CleanupStack::PushL(timeOutTimer);
	timeOutTimer->ConstructL();
	CleanupStack::Pop(); // timeOutTimer
	return timeOutTimer;
	}

void CEventQueue::CTimeOutTimer::Activate()
	{
	After(TTimeIntervalMicroSeconds32(KShelfLifeOfUnhandledUserEvents));
	}

void CEventQueue::CTimeOutTimer::Deactivate()
	{
	Cancel();
	}

CEventQueue::CTimeOutTimer::CTimeOutTimer(CEventQueue& aEventQueue)
	:CTimer(EActivePriorityDefault),
	 iEventQueue(aEventQueue)
	{
	CActiveScheduler::Add(this);
	}

void CEventQueue::CTimeOutTimer::RunL()
	{
	iEventQueue.PurgeAllExpiringEvents();
	Activate();
	}

//
// Class CSignalBuffer
//
// This is a *very* non-standard active-derived class - it's used to temporarily
// buffer any signals from OPL asynchonous requests that complete whilst
// we're executing CActiveScheduler::Start (e.g. when displaying a dialog)
class CSignalBuffer : public CActive
	{
public:
	static CSignalBuffer* NewL();
	virtual ~CSignalBuffer();
	void StartBufferingAnyUnhandledSignals();
	void StopBufferingAndReplaceAnyBufferedSignals();
private:
	CSignalBuffer();
	virtual void DoCancel();
	virtual void RunL();
private:
	TInt iNumberOfBufferedSignals;
	};

CSignalBuffer* CSignalBuffer::NewL()
	{
	return new(ELeave) CSignalBuffer;
	}

CSignalBuffer::~CSignalBuffer()
	{
	Cancel();
	CWsEventHandler::Signal(iNumberOfBufferedSignals);
	}

void CSignalBuffer::StartBufferingAnyUnhandledSignals()
	{
	SetActive();
	}

void CSignalBuffer::StopBufferingAndReplaceAnyBufferedSignals()
	{
	// Stop buffering
	__ASSERT_ALWAYS(IsActive(), User::Panic(KOplIOSystem, 11));
	Cancel();
	// Restore any buffered signals
	CWsEventHandler::Signal(iNumberOfBufferedSignals);
	iNumberOfBufferedSignals=0;
	}

CSignalBuffer::CSignalBuffer()
	:CActive(KMinTInt),
	 iNumberOfBufferedSignals(0)
	{
	iStatus=KErrNone;
	CActiveScheduler::Add(this);
	}

void CSignalBuffer::DoCancel()
	{
	CWsEventHandler::Signal(1);	// Generate a signal for Cancel to consume to pretend that we're
								// a normal CActive-derived class (objects of this class actually
								// have *no* signal associated with them, even when IsActive(),
								// hence the need for this signal here)
	}

void CSignalBuffer::RunL()
	{
	++iNumberOfBufferedSignals;
	SetActive();
	}

//
// Class TArrayOfAdditionalEventSources
//
class TArrayOfAdditionalEventSources
	{
public:
	TArrayOfAdditionalEventSources(const TArray<MEventSource*>& aArrayOfAdditionalEventSources);
	inline TBool AtLeastOneHasCompletedAndBeenConsumed() const {return (iBitArrayOfCompletedAdditionalEventSourcesConsumed!=0);}
	const MEventSource* CompletedAdditionalEventSourceWithHighestPriority() const;
	void MarkAsConsumed(const MEventSource* aEventSource);	// to be passed what has been returned
															// by CompletedAdditionalEventSourceWithHighestPriority
	void CancelAll();
private:
	inline TBool HasNotBeenConsumed(TInt aIndex) const {return ((iBitArrayOfCompletedAdditionalEventSourcesConsumed&(1<<aIndex))==0);}
	inline void MarkAsConsumed(TInt aIndex) {__ASSERT_DEBUG((iArrayOfAdditionalEventSources[aIndex]->RequestStatus()!=KRequestPending) && HasNotBeenConsumed(aIndex), User::Panic(KOplIOSystem, 5)); iBitArrayOfCompletedAdditionalEventSourcesConsumed|=(1<<aIndex);}
private:
	const TArray<MEventSource*>& iArrayOfAdditionalEventSources;
	TUint iBitArrayOfCompletedAdditionalEventSourcesConsumed;
	};

TArrayOfAdditionalEventSources::TArrayOfAdditionalEventSources(const TArray<MEventSource*>& aArrayOfAdditionalEventSources)
	:iArrayOfAdditionalEventSources(aArrayOfAdditionalEventSources),
	 iBitArrayOfCompletedAdditionalEventSourcesConsumed(0)
	{
	__ASSERT_DEBUG(aArrayOfAdditionalEventSources.Count()<=(TInt)sizeof(TUint)*8, User::Panic(KOplIOSystem, 8));
	}

const MEventSource* TArrayOfAdditionalEventSources::CompletedAdditionalEventSourceWithHighestPriority() const
	{
	const MEventSource* completedAdditionalEventSourceWithHighestPriority=NULL;
	for (TInt i=iArrayOfAdditionalEventSources.Count()-1; i>=0; --i)
		{
		MEventSource& eventSource=*iArrayOfAdditionalEventSources[i];
		if ((eventSource.RequestStatus()!=KRequestPending) && HasNotBeenConsumed(i))
			{
			if ((completedAdditionalEventSourceWithHighestPriority==NULL) || (eventSource.Priority()>completedAdditionalEventSourceWithHighestPriority->Priority()))
				{
				completedAdditionalEventSourceWithHighestPriority=&eventSource;
				}
			}
		}
	return completedAdditionalEventSourceWithHighestPriority;
	}

void TArrayOfAdditionalEventSources::MarkAsConsumed(const MEventSource* aEventSource)
	{
	for (TInt i=iArrayOfAdditionalEventSources.Count()-1; ; --i)
		{
		__ASSERT_ALWAYS(i>=0, User::Panic(KOplIOSystem, 4));
		if (iArrayOfAdditionalEventSources[i]==aEventSource)
			{
			MarkAsConsumed(i);
			break;
			}
		}
	}

void TArrayOfAdditionalEventSources::CancelAll()
	{
	for (TInt i=iArrayOfAdditionalEventSources.Count()-1; i>=0; --i)
		{
		if (HasNotBeenConsumed(i))
			{
			MEventSource& eventSource=*iArrayOfAdditionalEventSources[i];
			eventSource.CancelRequest();
			User::WaitForRequest(eventSource.RequestStatus());	// returns straightaway as
																// eventSource's request has
																// been cancelled
			// Returns straightaway as we're consuming the signal associated with the
			// cancelled event (however, if aStatPtr!=iStatPtr and both are non-NULL,
			// this may hang indefinitely - this is the same behaviour as EPOC16 OPL,
			// it basically means there's a bug in the OPL program)
			}
		}
	}

const TInt KNoNestedDialogs=0;

class COplActiveScheduler : public CCoeScheduler
	{
public:
	enum TMode
		{
		EModeCone,
		EModeOpl
		};
public:
	static COplActiveScheduler* NewL(CCoeEnv& aCoeEnv, COplRuntime& aRuntime);
	virtual ~COplActiveScheduler();
	void InstallOverConeActiveScheduler();
	void ReinstallConeActiveScheduler();
#if defined(_DEBUG)
	inline TBool IsInstalledOverConeActiveScheduler() const {return iInstalled;}
#endif
	inline static void SetMode(TMode aMode) {OplActiveScheduler()->DoSetMode(aMode);}
	// Cone mode nested dialog count
	inline TInt IncConemodeNestCount(){return iConemodeNestCount++;}; //returns depth before push
	inline TInt DecConemodeNestCount(){return --iConemodeNestCount;}; //returns depth after pop
private:
	enum TSignature {ESignature=0x3ad6079e}; // a random number
private:
	COplActiveScheduler(CCoeEnv& aCoeEnv, COplRuntime& aRuntime);
	static COplActiveScheduler* OplActiveScheduler();
	void DoSetMode(TMode aMode);
	virtual void WaitForAnyRequest();
	virtual void OnStarting();
	virtual void OnStopping();
private:
	CCoeEnv& iCoeEnv;
	COplRuntime& iRuntime;
	TSignature iSignature;
	TInt iHeapCellLength;
	CActiveScheduler* iConeActiveScheduler;
	TMode iMode;
	TBool iInstalled;
	TInt iConemodeNestCount;
	};

COplActiveScheduler* COplActiveScheduler::NewL(CCoeEnv& aCoeEnv, COplRuntime& aRuntime)
	{
	return new(ELeave) COplActiveScheduler(aCoeEnv, aRuntime);
	}

COplActiveScheduler::~COplActiveScheduler()
	{
	__ASSERT_DEBUG(iConemodeNestCount==KNoNestedDialogs, User::Panic(KOplIOSystem, 40));
	__ASSERT_DEBUG(!iInstalled, User::Panic(KOplIOSystem, 25));
	}

void COplActiveScheduler::InstallOverConeActiveScheduler()
	{
	__ASSERT_DEBUG(!iInstalled, User::Panic(KOplIOSystem, 26));
	iConeActiveScheduler=CActiveScheduler::Replace(this);
	iInstalled=ETrue;
	}

void COplActiveScheduler::ReinstallConeActiveScheduler()
	{
	__ASSERT_DEBUG(iInstalled, User::Panic(KOplIOSystem, 27));
#if defined(_DEBUG)
	const CActiveScheduler* const activeScheduler=
#endif
	CActiveScheduler::Replace(iConeActiveScheduler);
	__ASSERT_DEBUG(activeScheduler==this, User::Panic(KOplIOSystem, 28));
	iInstalled=EFalse;
	}

#pragma warning (disable: 4355) // warning about "this" being used in base member initializer list
COplActiveScheduler::COplActiveScheduler(CCoeEnv& aCoeEnv, COplRuntime& aRuntime)
	:CCoeScheduler(&aCoeEnv),
	 iCoeEnv(aCoeEnv),
	 iRuntime(aRuntime),
	 iSignature(ESignature),
	 iHeapCellLength(User::AllocLen(this)),
	 iConeActiveScheduler(NULL),
	 iMode(EModeCone), // forces DoSetMode below to call CCoeScheduler::CoeEnv()->Cancel()
	 iInstalled(EFalse),
	 iConemodeNestCount(0)
	{
	DoSetMode(EModeOpl);
	}
#pragma warning (default: 4355)

COplActiveScheduler* COplActiveScheduler::OplActiveScheduler()
	{
	COplActiveScheduler* oplActiveScheduler=(COplActiveScheduler*)CActiveScheduler::Current();
	__ASSERT_ALWAYS((oplActiveScheduler->iSignature==ESignature) && (oplActiveScheduler->iHeapCellLength==User::AllocLen(oplActiveScheduler)), User::Panic(KOplIOSystem, 18));
	return oplActiveScheduler;
	}

void COplActiveScheduler::DoSetMode(TMode aMode)
	{
	if ((iMode==EModeCone) && (aMode!=EModeCone))
		{
		iCoeEnv.Cancel();
		}
	iMode=aMode;
	}

void COplActiveScheduler::WaitForAnyRequest()
	{
	switch (iMode)
		{
	case EModeOpl:
		User::WaitForAnyRequest();
		break;
	case EModeCone:
		CCoeScheduler::WaitForAnyRequest();
		break;
	default:
		User::Panic(KOplIOSystem, 19);
		break;
		}
	}

void COplActiveScheduler::OnStarting()
	{
	iRuntime.PrepareToStartActiveScheduler();
	}

void COplActiveScheduler::OnStopping()
	{
	iRuntime.NotifyActiveSchedulerStopped();
	}

//
// Class CPriorityKeyHandler
//
class CPriorityKeyHandler : CActive
	{
public:
	static CPriorityKeyHandler* NewL(RWsSession& aWindowServerSession, CWsEventHandler& aWsEventHandler);
	virtual ~CPriorityKeyHandler();
	inline TRequestStatus& Status() {return iStatus;}
	inline void HandleCompletion() {DoHandleCompletion(EFalse);}
private:
	CPriorityKeyHandler(RWsSession& aWindowServerSession, CWsEventHandler& aWsEventHandler);
	void DoHandleCompletion(TBool aSetActive);
	virtual void DoCancel();
	virtual void RunL();
private:
	RWsSession& iWindowServerSession;
	CWsEventHandler& iWsEventHandler;
	};

CPriorityKeyHandler* CPriorityKeyHandler::NewL(RWsSession& aWindowServerSession, CWsEventHandler& aWsEventHandler)
	{
	return new(ELeave) CPriorityKeyHandler(aWindowServerSession, aWsEventHandler);
	}

CPriorityKeyHandler::~CPriorityKeyHandler()
	{
	Cancel();
	}

CPriorityKeyHandler::CPriorityKeyHandler(RWsSession& aWindowServerSession, CWsEventHandler& aWsEventHandler)
	:CActive(KMaxTInt),
	 iWindowServerSession(aWindowServerSession),
	 iWsEventHandler(aWsEventHandler)
	{
	CActiveScheduler::Add(this);
	iWindowServerSession.PriorityKeyReady(&iStatus);
	SetActive();
	}

void CPriorityKeyHandler::DoHandleCompletion(TBool aSetActive)
	{
	__ASSERT_DEBUG(iStatus!=KRequestPending, User::Panic(KOplIOSystem, 7));
	TWsPriorityKeyEvent event;
	iWindowServerSession.GetPriorityKey(event);
	iWindowServerSession.PriorityKeyReady(&iStatus);
	// Next ASSERT: "logical-not" both sides before comparing for inequality
	// to fold non-zero values
	__ASSERT_DEBUG(!IsActive()!=!aSetActive, User::Panic(KOplIOSystem, 29));
	if (aSetActive)
		{
		SetActive();
		}
	__ASSERT_DEBUG(IsActive(), User::Panic(KOplIOSystem, 30));
	const TKeyEvent& key=*event.Key();
	const TUint code=key.iCode;
	const TUint mods=(key.iModifiers&EAllStdModifiers);
	if ((code==EKeyEscape) && (mods==KKillKeyModifier))
		{
		// Terminate here - bail out!
#if defined(__SERIES60__)
		CActiveScheduler::Stop();
		// Not sure why OnStopping() isn't getting called...
		// so do it manually...
		TheRuntime()->NotifyActiveSchedulerStopped();
#endif
		iWsEventHandler.Stop();
		TheRuntime()->PrepareToExit();
		delete TheRuntime(); // closes files etc.
		User::Exit(0);
		}
	if (mods==KPauseKeyModifier)
		{
		switch (code)
			{
		case CTRL('s'):
			iWsEventHandler.SetSuspended();
			break;
		case CTRL('q'):
			iWsEventHandler.SetNotSuspended();
			break;
			}
		}
	}

void CPriorityKeyHandler::DoCancel()
	{
	iWindowServerSession.PriorityKeyReadyCancel();
	}

void CPriorityKeyHandler::RunL()
	{
	DoHandleCompletion(ETrue);
	}

//
// Class TIORequest
//
TIORequest::TIORequest(TAny* aPtr, TInt aPriority)
	:iStatusPtr(aPtr),
	 iPriority(aPriority),
	 iHasHandledCompletion(EFalse)
	{
	}

void TIORequest::HandleCompletionIfNecessary()
	{
	if (!iHasHandledCompletion)
		{
		DoHandleCompletion();
		iHasHandledCompletion=ETrue;
		}
	}

//
// Class TOplIORequest
//
#pragma warning (disable: 4310) // cast truncates constant
TOplIORequest::TOplIORequest(TOplReqStatus* aStatPtr, TInt aPriority)
	:TIORequest(aStatPtr, aPriority)
	{
	OplUtil::PutWord((TOplReqStatus*)iStatusPtr,(TInt16)KOplErrFilePending);
	}
#pragma warning (default: 4310)

void TOplIORequest::DoHandleCompletion()
	{
	DoParamsUpdate();
	OplUtil::PutWord((TOplReqStatus*)iStatusPtr,OplUtil::MapError(iStatus.Int()));
	}

void TOplIORequest::DoParamsUpdate()
	{
	}

//
// Class CIOCollection
//
void CIOCollection::ConstructL(COplRuntime* aRuntime,RWsSession& aWsSession)
	{
	iWsEventHandler=new(ELeave) CWsEventHandler(aWsSession,this);
	iWsEventHandler->ConstructL(*aRuntime->ConEnv());
	iRuntime=aRuntime;
	}

TInt16 CIOCollection::NewObject(TInt16* aHandlePtr,TPtrC aPtr, TInt16 aMode)
	{
	TBuf<256> devName=aPtr;
	return DoNewObject(aHandlePtr,devName,aMode);
	}

TInt16 CIOCollection::NewObject(TInt16* aHandlePtr,TPtr& aPtr, TInt16 aMode)
	//
	// Specifically for use when creating a unique file name
	//
	{
	TBuf<256> devName=aPtr;
	TInt16 ret=DoNewObject(aHandlePtr,devName,aMode);
	if (!ret)
		aPtr=devName;
	return ret;
	}

TInt16 CIOCollection::DoNewObject(TInt16* aHandlePtr,TBuf<256>& aDevName, TInt16 aMode)
	{
	TOplIOType type=GetType(aDevName,aMode);
	TInt index=FindSlot();
	if (index==KErrGeneral)
		return (TInt16)KErrNoMemory;
	COplIO* obj;
	switch (type)
		{
		case (ETimer):
			{
			obj=new(ELeave) COplIOTimer((TInt16)(index+KIOHandleBase));
			break;
			}
		case (EComm):
			{
			obj=new(ELeave) COplIOComm((TInt16)(index+KIOHandleBase));
			break;
			}
		case (EFileText):
			{
			obj=new(ELeave) COplIOFileText((TInt16)(index+KIOHandleBase));
			break;
			}
		default:
			{
			obj=new(ELeave) COplIOFile((TInt16)(index+KIOHandleBase));
			break;
			}
		}
	TRAPD(err,obj->ConstructL(aDevName,aMode));
	if (err)
		delete obj;
	else
		{
		iObjArray[index]=obj;
		OplUtil::PutWord(aHandlePtr,(TInt16)(index+KIOHandleBase));
		}
	return OplUtil::MapError(err);
	}

TInt CIOCollection::FindSlot()
	{
	for (TInt count=0;count<KMaxIOObjects;++count)
		{
		if (iObjArray[count]==NULL)
			return count;
		}
	return KErrGeneral;
	}

CIOCollection::TOplIOType CIOCollection::GetType(TBuf<256>& aDevName,TInt16 aMode)
	{
	if (aDevName.Locate(':')==3)
		{
		_LIT(KTIM,"TIM");
		_LIT(KTTY,"TTY");
		_LIT(KFIL,"FIL");
		if (aDevName.Left(3).CompareF(KTIM)==KErrNone)
			return ETimer;
		if (aDevName.Left(3).CompareF(KTTY)==KErrNone)
			return EComm;
		// others in here

		if (aDevName.Left(3).CompareF(KFIL)==KErrNone) // files don't need FIL:
			aDevName.Delete(0,4);
		}
	if ((aMode&FMT_MASK)==FTEXT)
		return EFileText;
	return EFile;
	}

void CIOCollection::RemoveObject(TInt16 aHandle)
	{
	aHandle-=KIOHandleBase;
	if (aHandle<0 || aHandle>KMaxIOObjects)
		User::Panic(KOplIOSystem,1);
	delete iObjArray[aHandle];
	iObjArray[aHandle]=NULL;
	}


COplIO* CIOCollection::FindObjectL(TInt16 aHandle)
	{
	aHandle-=KIOHandleBase;
	COplIO* obj=NULL;
	if ((aHandle<0 || aHandle>KMaxIOObjects)||((obj=iObjArray[aHandle])==NULL))
		User::Leave(KOplErrInvalidArgs);
	return obj;
	}

const TIORequest* CIOCollection::HandleAnyCompletedOplAsynchronousRequests()
	// N.B. this function does *not* handle completed KEYA or GETEVENTA32 requests
	{
	const TIORequest* completedRequestWithHighestPriority=NULL;
	TInt highestPriority=KMinTInt;
	for (TInt i=iRequestArray.Count()-1; i>=0; --i)
		{
		TIORequest* const request=iRequestArray[i];
		__ASSERT_DEBUG(request!=NULL,User::Panic(KOplIOSystem,100));
		if (request->Status()!=KRequestPending)
			{
			request->HandleCompletionIfNecessary();
			const TInt priority=request->Priority();
			if (highestPriority<=priority)	// <= rather than < just in case there actually
											// is a request with priority "KMinTInt"
				{
				completedRequestWithHighestPriority=request;
				highestPriority=priority;
				}
			}
		}
	return completedRequestWithHighestPriority;
	}

void CIOCollection::RemoveCompletedOplAsynchronousRequest(const TIORequest* aRequest)
	// Can only be passed a pointer that was returned by
	// HandleAnyCompletedOplAsynchronousRequests, and it must *not* be NULL
	{
	__ASSERT_DEBUG(aRequest!=NULL, User::Panic(KOplIOSystem, 12));
	iRequestArray.Remove(iRequestArray.Find(aRequest));
	delete (TIORequest*)aRequest;
	}

#include "consIO.h"

LOCAL_C void TPointFromP_POINT(TPoint& aPoint,P_POINT* aP_Point)
	{
	aPoint.iX=OplUtil::GetWord(&aP_Point->x+1);
	aPoint.iY=OplUtil::GetWord(&aP_Point->y+1);
	}

LOCAL_C void TRectFromP_RECT(TRect& aRect,P_RECT* aP_Rect)
	{
	TPointFromP_POINT(aRect.iTl,&(aP_Rect->tl));
	TPointFromP_POINT(aRect.iBr,&(aP_Rect->br));
	}

void CIOCollection::HandleConsoleRequest(TInt16 aFuncNo,TRequestStatus& aRequestStatus,TAny* aParam1,TAny* aParam2)
	// eb205: I've replaced calls to "User::Leave(KOplErrInvalidIO)" with calls to
	// "User::RequestComplete(requestStatus,KErrArgument)"
	{
	TRequestStatus* requestStatus=&aRequestStatus;
	COplConsole& console=iRuntime->Console();
	switch (aFuncNo)
		{
	case (FSENSE):
		{
		P_RECTP* rectp=(P_RECTP*) aParam1;
		TPoint curPos=console.CurrentPos();

		OplUtil::PutWord(&(rectp->p.x),(TInt16)(curPos.iX-1));
		OplUtil::PutWord(&(rectp->p.y),(TInt16)(curPos.iY-1));
		TRect rect;
		console.ScreenRectInChars(rect);
		OplUtil::PutWord(&(rectp->r.tl.x),(TInt16)(rect.iTl.iX-1));
		OplUtil::PutWord(&(rectp->r.tl.y),(TInt16)(rect.iTl.iY-1));
		OplUtil::PutWord(&(rectp->r.br.x),(TInt16)(rect.iBr.iX-1));
		OplUtil::PutWord(&(rectp->r.br.y),(TInt16)(rect.iBr.iY-1));
		break;
		}
	case (FINQ):
		{
		TSize charSize=console.CharSize();
		CONSOLE_INFO* info=(CONSOLE_INFO*)aParam1;
		OplUtil::PutWord(&info->window_handle,1);
		OplUtil::PutWord(&info->font_handle,0);
		OplUtil::PutWord(&info->line_height,(TUint16)charSize.iHeight);
		OplUtil::PutWord(&info->char_width,(TUint16)charSize.iWidth);
		break;
		}
	case (FSET):
		{
		switch (OplUtil::GetWord(aParam1))
			{
			case P_SCR_CLR:
				{
				P_RECT* rect=(P_RECT*)aParam2;
				TRect clearRect;
				TRectFromP_RECT(clearRect,rect);
				console.ClearRect(clearRect);
				break;
				}
			case P_SCR_SCROLL:
				{
				P_RECTP* rectp=(P_RECTP*)aParam2;
				TRect area;
				TRectFromP_RECT(area,&(rectp->r));
				// don't use local function as it is an offset
				TPoint offset(OplUtil::GetWord(&rectp->p.x),OplUtil::GetWord(&rectp->p.y));
				console.ScrollRect(offset,area);
				break;
				}
			case P_SCR_NEL:
				console.Cr();
				console.Lf();
				break;
			case P_SCR_CURSOR:
				if (*(TUint8*)aParam2)
					console.DrawCursorIfOn(console.CurrentPos());
				else
					console.HideCursor();
				break;
			case P_SCR_SLOCK:
				console.SetScrollLock(*(TUint8*)aParam2);
				break;
			case P_SCR_WLOCK:
				console.SetWrapLock(!(*(TUint8*)aParam2));
				break;
			case P_SCR_LAST_LINE_WRAP:
				console.SetLastLineWrap(*(TUint8*)aParam2);
				break;
			// !!! Any more??
			default:
				User::RequestComplete(requestStatus,KErrArgument);
				return; // return here as there's been an error
			}
		break;
		}
	default:
		User::RequestComplete(requestStatus,KErrArgument);
		return; // return here as there's been an error
		}
	User::RequestComplete(requestStatus,KErrNone);
	}

#pragma warning ( disable: 4310) // cast truncates constant
#pragma warning ( disable: 4706) // assignment withing conditional expression
TInt16 CIOCollection::DoIOA(TInt16 aHandle,TInt16 aFuncNo,TOplReqStatus* aOplStatPtr, TAny* aParam1, TAny* aParam2)
	{
	TInt err=iRequestArray.Append(NULL);
	if (err==KErrNone)
		{
		const TInt indexOfRequest=iRequestArray.Count()-1;
		TIORequest*& request=iRequestArray[indexOfRequest];
		__ASSERT_DEBUG(request==NULL, User::Panic(KOplIOSystem, 14));
		if (aHandle==-2)
			{
			request=new TOplIORequest(aOplStatPtr,EActivePriorityWsEvents);
			if (request==NULL)
				{
				err=KErrNoMemory;
				}
			else
				{
				HandleConsoleRequest(aFuncNo,request->Status(),aParam1,aParam2);
				}
			}
		else if (aHandle==-1 && !(aHandle=iLHandle)) // trying Lxxx without calling LOPEN
			{
			err=KOplErrClosed;
			}
		else
			{
			TRAP(err,FindObjectL(aHandle)->RunFunctionL(aFuncNo,request,aOplStatPtr,aParam1,aParam2));
			}
		if (err)
			{
			__ASSERT_DEBUG(request==NULL, User::Panic(KOplIOSystem, 15));
			iRequestArray.Remove(indexOfRequest);
			}
		}
	return OplUtil::MapError(err);
	}
#pragma warning (default: 4706)

TInt16 CIOCollection::DoIow(CCoeEnv& aCoeEnv,TInt16 aHandle,TInt16 aFuncNo, TAny* aParam1, TAny* aParam2)
	{
	TOplReqStatus status=(TInt8)KOplErrFilePending;
	TInt16 err=DoIOA(aHandle,aFuncNo,&status,aParam1,aParam2);
	if (err)
		return err;
	iWsEventHandler->WaitForOplAsynchronousRequestCompletion16(aCoeEnv,&status);
	return status;
	}
#pragma warning (default: 4310)

TRequestStatus& CIOCollection::NewRequestL(TOplReqStatus* aStatusPtr,TInt aPriority,TCallBack aCallBack)
	{
	TOplIOCallBackRequest* request=(TOplIOCallBackRequest*)User::AllocLC(sizeof(TOplIOCallBackRequest));
	User::LeaveIfError(iRequestArray.Append(request));
	CleanupStack::Pop(); // request
	new(request) TOplIOCallBackRequest(aStatusPtr,aPriority,aCallBack);
	return request->Status();
	}

TRequestStatus& CIOCollection::NewRequestL(TInt32* aStatusPtr,TInt aPriority,TCallBack aCallBack)
	{
	TEraIOCallBackRequest* request=(TEraIOCallBackRequest*)User::AllocLC(sizeof(TEraIOCallBackRequest));
	User::LeaveIfError(iRequestArray.Append(request));
	CleanupStack::Pop(); // request
	new(request) TEraIOCallBackRequest(aStatusPtr,aPriority,aCallBack);
	return request->Status();
	}

CIOCollection::~CIOCollection()
	{
	TInt count;
	for (count=0;count<KMaxIOObjects;++count)
		{
		delete iObjArray[count];
		}
	iRequestArray.ResetAndDestroy();
	iRequestArray.Close();
	delete iWsEventHandler;
	}

//
// Class CWsEventHandler
//
CWsEventHandler::CWsEventHandler(RWsSession& aWsSession,CIOCollection* aIOCollection)
	:iWs(aWsSession),
	 iIOCollection(aIOCollection),
	 iIsSuspended(EFalse)
	{
	}

void CWsEventHandler::ConstructL(CCoeEnv& aCoeEnv)
	{
	iPriorityKeyHandler=CPriorityKeyHandler::NewL(iWs, *this);
	iOplActiveScheduler=COplActiveScheduler::NewL(aCoeEnv, *TheRuntime());
	iEventQueue=CEventQueue::NewL();
	iSignalBuffer=CSignalBuffer::NewL();
	iCommand=HBufC::NewL(1);
	iFepControl=NULL;
	}

CWsEventHandler::~CWsEventHandler()
	{
	delete iPriorityKeyHandler;
	delete iEventQueue;
	delete iSignalBuffer;
	delete iCommand;
	delete iOplActiveScheduler;
	}

void CWsEventHandler::Start()
	{
	iOplActiveScheduler->InstallOverConeActiveScheduler();
	iWs.EventReady(&iStatus);
	}

void CWsEventHandler::Stop()
	{
	iOplActiveScheduler->ReinstallConeActiveScheduler();
	iWs.EventReadyCancel();
	User::WaitForRequest(iStatus);	// returns straightaway as the iWs.EventReady request
									// has been canceled
	}

#if defined(_DEBUG)
TBool CWsEventHandler::IsStarted() const
	{
	return iOplActiveScheduler->IsInstalledOverConeActiveScheduler();
	}
#endif

TInt CWsEventHandler::HandleSpecialWsEvent(const TWsEvent& aWsEvent)
	{
	// Since this method cannot leave, TRAP the HandleWsEventL() call and return any error
	TInt error=KErrNone;
	const TEventCode type=(TEventCode)aWsEvent.Type();
	const TKeyEvent* key=(TKeyEvent*)aWsEvent.Key();
	if (type==EEventFocusGained || type==EEventFocusLost || key->iCode==EKeyIncBrightness)
		{
		// For Focus Gained/Lost events this ensures we handle both the application
		// screen fading and also notifications to foreground observers (e.g. Status Panes)
		//
		// For the Brightness key it ensures OPL apps properly let CKON cycle the screen
		// brightness with, for example, Chr+Space on Crystal.
		TRAP(error,TheRuntime()->HandleWsEventL(aWsEvent,NULL));
		}
	return error;
	}

void CWsEventHandler::PrepareToStartActiveScheduler(CCoeEnv&)
	{
	if (iOplActiveScheduler->IncConemodeNestCount()==KNoNestedDialogs) //just starting
		{
		COplActiveScheduler::SetMode(COplActiveScheduler::EModeCone);
		iEventQueue->PrepareToStartActiveScheduler();
		iWs.EventReadyCancel();
		User::WaitForRequest(iStatus);	// returns straightaway as the iWs.EventReady request
										// has been canceled
		iSignalBuffer->StartBufferingAnyUnhandledSignals();
		}
	}

void CWsEventHandler::NotifyActiveSchedulerStopped()
	{
	if (iOplActiveScheduler->DecConemodeNestCount()==KNoNestedDialogs) // now ending
		{
		COplActiveScheduler::SetMode(COplActiveScheduler::EModeOpl);
		iEventQueue->NotifyActiveSchedulerStopped();
		iWs.EventReady(&iStatus);
		iSignalBuffer->StopBufferingAndReplaceAnyBufferedSignals();
		iWs.Flush();
		}
	}

TUint CWsEventHandler::DoGetUnmapped(CCoeEnv& aCoeEnv)
	{
	TWsEvent event;
	WaitForEvent(aCoeEnv, event, EUserEventsKeysOnly);
	const TKeyEvent& key=*event.Key();
	iKMod=key.iModifiers;
	return key.iCode;
	}

TInt16 CWsEventHandler::DoGet(CCoeEnv& aCoeEnv)
	{
	return MapKeys(DoGetUnmapped(aCoeEnv));
	}

void CWsEventHandler::DoGetEvent(CCoeEnv& aCoeEnv, TInt16* aArray)
	{
	TBool done=EFalse;
	while (!done)
		{
		done=ETrue;
		TWsEvent event;
		WaitForEvent(aCoeEnv, event, EUserEventsAny);

		switch (event.Type())
			{
		case EEventKey:
			{
			const TKeyEvent& key=*event.Key();
			TInt mod=MapToOplMod(key.iModifiers);
			OplUtil::PutWord(aArray,MapKeys(key.iCode));
			OplUtil::PutWord(aArray+1,(TInt16)(mod | (key.iRepeats<<8)));
			break;
			}
		case EEventFocusGained:
			OplUtil::PutWord(aArray,0x401);
			break;
		case EEventFocusLost:
			OplUtil::PutWord(aArray,0x402);
			break;
		case EEventSwitchOn:
			OplUtil::PutWord(aArray,0x403);
			break;
//		case EEventSwitchOff:
//			OplUtil::PutWord(aArray,0x40B);
//			break;
		case EEventKeyDown:
			OplUtil::PutWord(aArray,0x406);
			break;
		case EEventKeyUp:
			OplUtil::PutWord(aArray,0x407);
			break;
		case EEventPointerEnter:
			OplUtil::PutWord(aArray,0x409);
			break;
		case EEventPointerExit:
			OplUtil::PutWord(aArray,0x40A);
			break;
		case EEventMessageReady:
			// same as EEventUser
			if (IsCreateOrOpenMessage(aCoeEnv, event))
				{
				OplUtil::PutWord(aArray,0x404);
				}
			else
				{
				done=EFalse;
				}
			break;
		case EEventUser: 
			// same as EEventMessageReady
			OplUtil::PutWord(aArray,0x404);
			SetCommand(event);
			break;
		case EEventPointer:
			OplUtil::PutWord(aArray,0x408);
			break;
		default:
			// unknown event type
			OplUtil::PutWord(aArray,0x400);
			break;
			}
		}
	}

void CWsEventHandler::DoGetEvent(CCoeEnv& aCoeEnv, TInt32* aArray)
	{
	TWsEvent event;
	TBool done=EFalse;
	while (!done)
		{
		WaitForEvent(aCoeEnv, event, EUserEventsAny);
		done=DoGetEventArrayWrite(aCoeEnv, event,aArray);
		}
	}

TBool CWsEventHandler::DoGetEventArrayWrite(CCoeEnv& aCoeEnv, const TWsEvent& aEvent,TInt32* aArray)
	{
	TOplEventBuf eventBuf;
	TOplPointerEventBuf pointerEventBuf;
	TInt id=(TInt)aEvent.Handle();
	if (id==0)
		id=1;
	TInt64 time64=aEvent.Time().Int64();
	eventBuf.iTimeStamp=time64.GetTInt();
	const TKeyEvent& key=*aEvent.Key();
	TInt mod=MapToOplMod(key.iModifiers);

	const TEventCode type=(TEventCode)aEvent.Type();
	switch (type)
		{
		// Not yet dealt with/ignored:
		// EEventModifiersChanged, EEventDragDrop, EEventPassword
		case EEventKeyDown:
			eventBuf.iCode=0x406;
			eventBuf.iScanOrWinId=key.iScanCode;
			eventBuf.iModifiers=mod;
#if defined(__UIQ__)
			if(key.iScanCode==EStdQuartzKeyConfirm)
				eventBuf.iScanOrWinId=EStdKeyMenu; 
#endif
			break;
		case EEventKeyUp:
			eventBuf.iCode=0x407;
			eventBuf.iScanOrWinId=key.iScanCode;
			eventBuf.iModifiers=mod;
#if defined(__UIQ__)
			if(key.iScanCode==EStdQuartzKeyConfirm)
				eventBuf.iScanOrWinId=EStdKeyMenu; 
#endif
			break;
		case EEventKey:
			eventBuf.iCode=key.iCode;
			eventBuf.iScanOrWinId=key.iScanCode;
			eventBuf.iModifiers=mod;
			eventBuf.iRepeat=key.iRepeats;
#if defined(__UIQ__)
			if(key.iCode==EQuartzKeyConfirm)
				{
				eventBuf.iCode=EKeyMenu;
				eventBuf.iScanOrWinId=EStdKeyMenu; 
				}
#endif
			break;
		case EEventPointerEnter:
			eventBuf.iCode=0x409;
			eventBuf.iScanOrWinId=id;
			break;
		case EEventPointerExit:
			eventBuf.iCode=0x40A;
			eventBuf.iScanOrWinId=id;
			break;
		case EEventFocusGained:
			eventBuf.iCode=0x401;
			break;
		case EEventFocusLost:
			eventBuf.iCode=0x402;
			break;
		case EEventSwitchOn:
			eventBuf.iCode=0x403;
			break;
//		case EEventSwitchOff:
//			eventBuf.iCode=0x40B;
//			break;
		case EEventMessageReady:
			// same as EEventUser
			if (IsCreateOrOpenMessage(aCoeEnv, aEvent))
				{
				eventBuf.iCode=0x404;
				}
			else
				{
				return EFalse;
				}
			break;
		case EEventUser:
			// same as EEventMessageReady
			eventBuf.iCode=0x404;
			SetCommand(aEvent);
			break;
		case EEventPointer:
			pointerEventBuf.iCode=0x408;
		default:
			if (type!=EEventPointer)
				pointerEventBuf.iCode=type | EOplEventUnknown; // unknown event type (EEventUser), pass it on
			pointerEventBuf.iTimeStamp=time64.GetTInt();
			pointerEventBuf.iWinId=id;
			pointerEventBuf.iPointer=*aEvent.Pointer();
			Mem::Copy(aArray,&pointerEventBuf,sizeof(TOplPointerEventBuf));
			return ETrue;
		}
	Mem::Copy(aArray,&eventBuf,sizeof(TOplEventBuf));
	return ETrue;
	}

void CWsEventHandler::SetCommand(const TWsEvent& aEvent)
	{
	switch (*(TInt*)aEvent.EventData())
		{
	case EApaSystemEventShutdown:
		*iCommand=KLitX;
		break;
	case EApaSystemEventBackupStarting:
		*iCommand=KLitS;
		break;
	case EApaSystemEventBackupComplete:
		*iCommand=KLitR;
		break;
	case EApaSystemEventBroughtToForeground:
		*iCommand=KLitF;
		break;
	default:
		*iCommand=KLitU;
		}
	}

TBool CWsEventHandler::IsCreateOrOpenMessage(CCoeEnv& aCoeEnv, const TWsEvent& aEvent)
	{
	TBool returnValue=EFalse;
	TRAPD(error, returnValue=IsCreateOrOpenMessageL(aCoeEnv, aEvent));
	HandleError(error);
	return returnValue;
	}

TBool CWsEventHandler::IsCreateOrOpenMessageL(CCoeEnv& aCoeEnv, const TWsEvent& aEvent)
	{
	TUid messageUid=KNullUid;
	TPtr8 messageParameters(NULL, 0, 0);
	aCoeEnv.GetMessageNotifyingObserversLC(aEvent.Handle(), messageUid, messageParameters, aEvent);
	const TPtrC commandParameters((TText*)messageParameters.Ptr(), messageParameters.Length()/sizeof(TText));
	const TInt commandLength=1+commandParameters.Length();
	if (iCommand->Des().MaxLength()<commandLength)
		{
		iCommand=iCommand->ReAllocL(commandLength);
		}
	TPtr command(iCommand->Des());
	TBool returnValue=EFalse;
	switch (messageUid.iUid)
		{
	case KUidApaMessageSwitchCreateFileValue:
		command=KLitC;
		command.Append(commandParameters);
		returnValue=ETrue;
		break;
	case KUidApaMessageSwitchOpenFileValue:
		command=KLitO;
		command.Append(commandParameters);
		returnValue=ETrue;
		break;
		}
	CleanupStack::PopAndDestroy();	// stuff left on cleanup-stack
									// by GetMessageNotifyingObserversLC
	return returnValue;
	}

TInt16 CWsEventHandler::DoKey(CCoeEnv& aCoeEnv)
	{
	iKMod=0;
	TInt16 code=0;
	TWsEvent event;
	if (IsEventReady(aCoeEnv, &event, EUserEventsKeysOnly))
		{
		__ASSERT_DEBUG(event.Type()==EEventKey, User::Panic(KOplIOSystem, 13));
		const TKeyEvent& key=*event.Key();
		iKMod=key.iModifiers;
		code=MapKeys(key.iCode);
		}
	return code;
	}


TInt16 CWsEventHandler::DoKmod()
	{
	return MapToOplMod(iKMod);
	}

#pragma warning ( disable: 4310) // cast truncates constant
void CWsEventHandler::DoCancelOplAsynchronousWservRequest(TInt16* aStatPtr)
	{
	DoCancelOplAsynchronousWservRequestWithoutConsumingSignal(aStatPtr);
	User::WaitForAnyRequest();	// returns straightaway as we're consuming the signal
								// associated with the canceled event (however, if
								// aStatPtr!=iStatPtr and both are non-NULL, this may hang
								// indefinitely - this is the same behaviour as EPOC16 OPL,
								// it basically means there's a bug in the OPL program)
	}

void CWsEventHandler::DoCancelOplAsynchronousWservRequestWithoutConsumingSignal(TInt16* aStatPtr)
	{
	if ((iStatPtr!=NULL) && (iStatPtr==aStatPtr))
		{
		OplUtil::PutWord(iStatPtr,(TInt16)KOplErrIOCancelled);
		iStatPtr=NULL;
		iArrayPtr=NULL;
		Signal(1);
		}
	}

void CWsEventHandler::DoKeyA(CCoeEnv& aCoeEnv,TInt16* aStatPtr,TInt16* aArrayPtr)
	{
	if (iStatPtr!=NULL)
		{
		DoCancelOplAsynchronousWservRequest(iStatPtr);
		}
	iKeysOnly=ETrue;
	iStatPtr=aStatPtr;
	OplUtil::PutWord(iStatPtr,(TInt16)KOplErrFilePending);
	iArrayPtr=aArrayPtr;
	IsEventReady(aCoeEnv, NULL, EUserEventsAny);	// this completes the request immediately
													// if possible
	}

void CWsEventHandler::DoGetEventA(CCoeEnv& aCoeEnv,TInt16* aStatPtr,TInt32* aArrayPtr)
	{
	if (iStatPtr!=NULL)
		{
		DoCancelOplAsynchronousWservRequest(iStatPtr);
		}
	iKeysOnly=EFalse;
	iStatPtr=aStatPtr;
	OplUtil::PutWord(iStatPtr,(TInt16)KOplErrFilePending);
	iArrayPtr=aArrayPtr;
	IsEventReady(aCoeEnv, NULL, EUserEventsAny);	// this completes the request immediately
													// if possible
	}
#pragma warning (default: 4310)

TBool CWsEventHandler::DoTestEvent(CCoeEnv& aCoeEnv)
	{
	return IsEventReady(aCoeEnv, NULL, EUserEventsAny);
	}

void CWsEventHandler::DoConsumeEvent(CCoeEnv& aCoeEnv)
	{
	TWsEvent ignoredEvent;
	IsEventReady(aCoeEnv, &ignoredEvent, EUserEventsKeysOnly);
	}

TBool CWsEventHandler::IsEventReady(CCoeEnv& aCoeEnv, TWsEvent* aEvent, TUserEventsRequired aUserEventsRequired)
	// Consumes the event (and its signal) if there is one *and* if aEvent!=NULL
	// KEY/TESTEVENT (both of which call this function) have lower priority than
	// KEYA (which is handled by the EventCompletesAnyOplAsynchronousWservRequest calls
	// in this function)
	//
	// For Crystal we need to acknowledge focus changes because application views
	// get dimmed in the background. This method was altered by PhilS on
	// 24/10/2001 to include the HandleSpecialWsEvent() check below to
	// allow for this. Doing this check here ensures KEY/TESTEVENT still get
	// Focus gained/lost events actioned. GET/GETEVENT functions are taken care of
	// with an identical check in CWsEventHandler::OfferEventToUserEventHandler()
	{
	__ASSERT_DEBUG(iEventQueue->BackupIsEmpty(), User::Panic(KOplIOSystem, 20));
	if ((aUserEventsRequired==EUserEventsKeysOnly) && (aEvent!=NULL))
		{
		iEventQueue->PurgeAllNonKeyEvents();
		}
	iIOCollection->HandleAnyCompletedOplAsynchronousRequests(); // a good opportunity to call this
	for (TInt i=0; i<10; ++i)	// only allow a finite number of iterations of this loop to
								// prevent this function from taking too long to execute
		{
		__ASSERT_DEBUG(iEventQueue->BackupIsEmpty(), User::Panic(KOplIOSystem, 23));
		while (iEventQueue->IsNotEmpty())
			{
			TWsEvent event;
			iEventQueue->PeekNextEvent(event);
			if (EventCompletesAnyOplAsynchronousWservRequest(aCoeEnv, event))
				{
				iEventQueue->ConsumeNextEvent();
				User::WaitForAnyRequest();	// returns straightaway as we're consuming the signal
											// associated with the event that has just been taken
											// out of iEventQueue
				}
			else if ((aUserEventsRequired==EUserEventsAny) ||
					((aUserEventsRequired==EUserEventsKeysOnly) && (event.Type()==EEventKey)))
				{
				if (aEvent!=NULL)
					{
					iEventQueue->ConsumeNextEvent();
					User::WaitForAnyRequest();	// returns straightaway as we're consuming the
												// signal associated with the event that has just
												// been taken out of iEventQueue
					*aEvent=event;
					}
				iEventQueue->RestoreAllEventsFromBackup();
				return ETrue;
				}
			else
				{
				iEventQueue->PutNextEventInBackup();
				User::WaitForAnyRequest();	// returns straightaway as we're consuming the signal
											// associated with the event that has just been put
											// in iEventQueue's backup
				}
			}
		iEventQueue->RestoreAllEventsFromBackup();
		if (iStatus==KRequestPending)
			{
			return EFalse;
			}
		User::WaitForRequest(iStatus); // returns straightaway as iStatus has already completed
		TWsEvent event;
		iWs.GetEvent(event);
		iWs.EventReady(&iStatus);
		// get this inside the loop in case it changes between iterations
		CCoeFep* const fep=aCoeEnv.Fep();
		if ((fep==NULL) || (OfferEventToFep(*fep, event)==CCoeFep::EEventWasNotConsumed))
			{
			// Here we can do checks for special events/keys, calling HandleSpecialWsEvent()
			// if appropriate. Currently we check for:
			//
			// 1) EEventFocusGained
			// 2) EEventFocusLost
			// 3) The EKeyIncBrightness key
			const TEventCode type=(TEventCode)event.Type();
			const TKeyEvent* key=(TKeyEvent*)event.Key();
			if (type==EEventFocusGained || type==EEventFocusLost || key->iCode==EKeyIncBrightness)
				HandleSpecialWsEvent(event);
			// Now continue with the rest of the processing as normal
			if (!EventCompletesAnyOplAsynchronousWservRequest(aCoeEnv, event))
				{
				if ((aUserEventsRequired==EUserEventsAny) ||
					((aUserEventsRequired==EUserEventsKeysOnly) && (type==EEventKey)))
					{
					if (aEvent!=NULL)
						{
						*aEvent=event;
						return ETrue;
						}
					const TInt error=iEventQueue->AppendEvent(event);
					HandleError(error);
					return (error==KErrNone);
					}
				const TInt error=iEventQueue->AppendEvent(event);
				HandleError(error);
				if (error!=KErrNone)
					{
					return EFalse;
					}
				}
			}
		}
	iWs.Flush();
	return EFalse;
	}

#pragma warning ( disable: 4310) // cast truncates constant
void CWsEventHandler::DoPauseL(CCoeEnv& aCoeEnv, TInt16 aDelay)
	{
	TWsEvent event;
	const TInt KDummyEventType=-1;
	event.SetType(KDummyEventType);
	if (aDelay==0)
		{
		WaitForEvent(aCoeEnv, event, EUserEventsKeysOnly);
		}
	else
		{
		RTimer timer;
		CleanupClosePushL(timer);
		User::LeaveIfError(timer.CreateLocal());
		TRequestStatus timStat;
		timer.After(timStat, Abs(aDelay*(1000000/20)));
		TTimerEventSource timerEventSource(timer, timStat, EActivePriorityWsEvents-1);
		TFixedArray<MEventSource*, 1> timerEventSourceArray;
		timerEventSourceArray[0]=&timerEventSource;
		WaitForEvent(aCoeEnv, event, (aDelay>0)? EUserEventsNone: EUserEventsKeysOnly, timerEventSourceArray.Array());
		CleanupStack::PopAndDestroy(); // timer
		}
	if (event.Type()!=KDummyEventType)
		{
		User::LeaveIfError(iEventQueue->AppendEvent(event));
		}
	}
#pragma warning (default: 4310)

void CWsEventHandler::Signal(TInt aNumberOfSignals)
	{
	__ASSERT_DEBUG(aNumberOfSignals>=0, User::Panic(KOplIOSystem, 10));
	TRequestStatus requestStatus;
	for (TInt i=0; i<aNumberOfSignals; ++i)
		{
		TRequestStatus* requestStatusPointer=&requestStatus;
		User::RequestComplete(requestStatusPointer, KErrNone);
		}
	}

TBool CWsEventHandler::EventCompletesAnyOplAsynchronousWservRequest(CCoeEnv& aCoeEnv, const TWsEvent& aEvent)
	{
	if (iStatPtr==NULL)
		{
		return EFalse;
		}
	if (!iKeysOnly)
		{
		if (!DoGetEventArrayWrite(aCoeEnv, aEvent,(TInt32*)iArrayPtr))
			{
			return EFalse;
			}
		}
	else
		{
		if (aEvent.Type()!=EEventKey)
			{
			return EFalse;
			}
		const TKeyEvent& key=*aEvent.Key();
		OplUtil::PutWord(iArrayPtr,MapKeys(key.iCode));
		OplUtil::PutWord((TInt16*)iArrayPtr+1,(TInt16)(256*(key.iRepeats)+(TUint8)MapToOplMod(key.iModifiers)));
		}
	OplUtil::PutWord(iStatPtr,0);
	iStatPtr=NULL;
	iArrayPtr=NULL;
	Signal(1);
	return ETrue;
	}

LOCAL_C TInt ZeroReturner(const CBase*)
	{
	return 0;
	}

void CWsEventHandler::HandleAnyCompletedEvents(CCoeEnv& aCoeEnv, TInt aMaximumNumberOfCompletedEventsToHandle)
	{
	TWsEvent notUsed;
	const TArray<MEventSource*> emptyArray(ZeroReturner, NULL, NULL);
	WaitForEvent(aCoeEnv, notUsed, EUserEventsNone, emptyArray, &aMaximumNumberOfCompletedEventsToHandle);
	}

void CWsEventHandler::WaitWhileSuspended(CCoeEnv& aCoeEnv)
	{
	__ASSERT_DEBUG(iIsSuspended, User::Panic(KOplIOSystem, 22));
	TWsEvent notUsed;
	const TArray<MEventSource*> emptyArray(ZeroReturner, NULL, NULL);
	WaitForEvent(aCoeEnv, notUsed, EUserEventsNone|EPriorityKeyEventsTerminatingSuspendedState, emptyArray, NULL);
	__ASSERT_DEBUG(!iIsSuspended, User::Panic(KOplIOSystem, 16));
	}

void CWsEventHandler::WaitForAnyOplAsynchronousRequestCompletion(CCoeEnv& aCoeEnv)
	{
	TWsEvent notUsed;
	const TArray<MEventSource*> emptyArray(ZeroReturner, NULL, NULL);
	WaitForEvent(aCoeEnv, notUsed, EUserEventsNone|EOplAsynchronousRequestCompletion, emptyArray, NULL);
	}

#pragma warning (disable: 4310) // cast truncates constant
void CWsEventHandler::WaitForOplAsynchronousRequestCompletion16(CCoeEnv& aCoeEnv, TAny* aRequestStatus16)
	{
	TInt numberOfSignalsToReplace=0;
	FOREVER
		{
		WaitForAnyOplAsynchronousRequestCompletion(aCoeEnv);
		if (OplUtil::GetWord(aRequestStatus16)!=(TInt16)KOplErrFilePending)
			{
			break;
			}
		++numberOfSignalsToReplace;
		}
	Signal(numberOfSignalsToReplace);
	}
#pragma warning (default: 4310)

void CWsEventHandler::WaitForOplAsynchronousRequestCompletion32(CCoeEnv& aCoeEnv, TAny* aRequestStatus32)
	{
	TInt numberOfSignalsToReplace=0;
	FOREVER
		{
		WaitForAnyOplAsynchronousRequestCompletion(aCoeEnv);
		if (OplUtil::GetLong(aRequestStatus32)!=KRequestPending)
			{
			break;
			}
		++numberOfSignalsToReplace;
		}
	Signal(numberOfSignalsToReplace);
	}

void CWsEventHandler::WaitForEvent(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired)
	{
	__ASSERT_DEBUG(aUserEventsRequired!=EUserEventsNone, User::Panic(KOplIOSystem, 3));
	const TArray<MEventSource*> emptyArray(ZeroReturner, NULL, NULL);
	WaitForEvent(aCoeEnv, aEvent, aUserEventsRequired, emptyArray);
	}

void CWsEventHandler::WaitForEvent(CCoeEnv& aCoeEnv, TRequestStatus& aRequestStatus, TInt aPriority)
	{
	TSingleEventSource singleEventSource(aRequestStatus, aPriority);
	TFixedArray<MEventSource*, 1> singleEventSourceArray;
	singleEventSourceArray[0]=&singleEventSource;
	TWsEvent event;
	WaitForEvent(aCoeEnv, event, EUserEventsNone, singleEventSourceArray.Array());
	}

void CWsEventHandler::WaitForEvent(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired, const TArray<MEventSource*>& aArrayOfAdditionalEventSources)
	{
	WaitForEvent(aCoeEnv, aEvent, aUserEventsRequired, aArrayOfAdditionalEventSources, NULL);
	}

void CWsEventHandler::WaitForEvent(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUint aEventsRequired, const TArray<MEventSource*>& aArrayOfAdditionalEventSources, const TInt* aMaximumNumberOfCompletedEventsToHandle)
	// If aMaximumNumberOfCompletedEventsToHandle!=NULL, only events that have completed are to
	// be handled, and only *aMaximumNumberOfCompletedEventsToHandle of them (at most) are to be
	// handled. This function does not return whilst iIsSuspended is ETrue, with one exception:
	// if aMaximumNumberOfCompletedEventsToHandle!=NULL.
	{
	__ASSERT_DEBUG(iEventQueue->BackupIsEmpty(), User::Panic(KOplIOSystem, 21));
	if ((aEventsRequired&EUserEvents)==EUserEventsKeysOnly)
		{
		iEventQueue->PurgeAllNonKeyEvents();
		}
	TArrayOfAdditionalEventSources arrayOfAdditionalEventSources(aArrayOfAdditionalEventSources);
	SEventHandler::SParameters eventHandlerParameters(aCoeEnv, aEvent, aEventsRequired, arrayOfAdditionalEventSources, aMaximumNumberOfCompletedEventsToHandle);
	if (aMaximumNumberOfCompletedEventsToHandle!=NULL)
		{
		__ASSERT_DEBUG(((aEventsRequired&EUserEvents)==EUserEventsNone) && (aArrayOfAdditionalEventSources.Count()==0), User::Panic(KOplIOSystem, 9));
		Signal(1);	// signal a dummy event, so that we do not have to wait for non-completed
					// events if aMaximumNumberOfCompletedEventsToHandle!=NULL
		}
	for (; !eventHandlerParameters.iDone; ++eventHandlerParameters.iNumberOfCompletedEventsHandled)
		{
		if ((aMaximumNumberOfCompletedEventsToHandle!=NULL) && (eventHandlerParameters.iNumberOfCompletedEventsHandled>=*aMaximumNumberOfCompletedEventsToHandle))
			{
			User::WaitForAnyRequest(); // returns straightaway as we're consuming the dummy signal
			break;
			}
		User::WaitForAnyRequest();
		// Set up the members of eventHandlerParameters that may change between
		// iterations of the loop
		eventHandlerParameters.iFep=aCoeEnv.Fep();
		eventHandlerParameters.iCompletedAdditionalEventSourceWithHighestPriority=arrayOfAdditionalEventSources.CompletedAdditionalEventSourceWithHighestPriority();
		eventHandlerParameters.iCompletedOplAsynchronousRequestWithHighestPriority=iIOCollection->HandleAnyCompletedOplAsynchronousRequests();
		if (OfferEventToPriorityKeyHandler(eventHandlerParameters)==CCoeFep::EEventWasNotConsumed)
			{
			TFixedArray<SEventHandler, 4> arrayOfEventHandlers;
			SetUpArrayOfEventHandlers(arrayOfEventHandlers, eventHandlerParameters.iCompletedAdditionalEventSourceWithHighestPriority, eventHandlerParameters.iCompletedOplAsynchronousRequestWithHighestPriority);
			for (TInt i=0; ; ++i) // must iterate *forwards* through arrayOfEventHandlers
				{
				if (OfferEventToActiveScheduler(eventHandlerParameters.iFep, arrayOfEventHandlers[i].iPriority)==CCoeFep::EEventWasConsumed)
					{
					break; // wait for another event
					}
				if ((*arrayOfEventHandlers[i].iOfferEventFunction)(*this, eventHandlerParameters)==CCoeFep::EEventWasConsumed)
					{
					break; // wait for another event
					}
				}
			}
		iWs.Flush();
		}
	arrayOfAdditionalEventSources.CancelAll();
	Signal(eventHandlerParameters.iNumberOfSignalsToReplace);
	iEventQueue->RestoreAllEventsFromBackup();
	if ((aEventsRequired&EUserEvents)==EUserEventsKeysOnly)
		{
		iEventQueue->PurgeAllNonKeyEvents();
		}
	}

CWsEventHandler::SEventHandler::SParameters::SParameters(CCoeEnv& aCoeEnv, TWsEvent& aEvent, TUint aEventsRequired, TArrayOfAdditionalEventSources& aArrayOfAdditionalEventSources, const TInt* aMaximumNumberOfCompletedEventsToHandle)
	:iCoeEnv(aCoeEnv),
	 iDone(EFalse),
	 iFep(NULL),
	 iEvent(aEvent),
	 iEventsRequired(aEventsRequired),
	 iArrayOfAdditionalEventSources(aArrayOfAdditionalEventSources),
	 iMaximumNumberOfCompletedEventsToHandle(aMaximumNumberOfCompletedEventsToHandle),
	 iNumberOfCompletedEventsHandled(0),
	 iNumberOfSignalsToReplace(0),
	 iCompletedAdditionalEventSourceWithHighestPriority(NULL),
	 iCompletedOplAsynchronousRequestWithHighestPriority(NULL)
	{
	}

CCoeFep::TEventResponse CWsEventHandler::OfferEventToPriorityKeyHandler(SEventHandler::SParameters& aParameters)
	{
	if (PriorityKeyStatus()!=KRequestPending)
		{
		const TBool wasSuspended=iIsSuspended;
		HandlePriorityKeyCompletion();
		if (!iIsSuspended)
			{
			if (aParameters.iEventsRequired&EPriorityKeyEventsTerminatingSuspendedState)
				{
				__ASSERT_DEBUG(wasSuspended, User::Panic(KOplIOSystem, 17));
				aParameters.iDone=ETrue;
				return CCoeFep::EEventWasConsumed;
				}
			if (wasSuspended)
				{
				// Check if an additional event source completed while iIsSuspended was true
				if (aParameters.iArrayOfAdditionalEventSources.AtLeastOneHasCompletedAndBeenConsumed())
					{
					aParameters.iDone=ETrue;
					return CCoeFep::EEventWasConsumed;
					}
				Signal(aParameters.iNumberOfSignalsToReplace);	// replace the signals of any OPL
																// asynchronous requests that
																// completed while iIsSuspended
																// was true
				aParameters.iNumberOfSignalsToReplace=0;
				iEventQueue->RestoreAllEventsFromBackup();
				}
			}
		return CCoeFep::EEventWasConsumed;
		}
	return CCoeFep::EEventWasNotConsumed;
	}

void CWsEventHandler::SetUpArrayOfEventHandlers(TFixedArray<SEventHandler, 4>& aArrayOfEventHandlers, const MEventSource* aCompletedAdditionalEventSourceWithHighestPriority, const TIORequest* aCompletedOplAsynchronousRequestWithHighestPriority)
	{
	aArrayOfEventHandlers[0].iPriority=KMinTInt;
	aArrayOfEventHandlers[0].iOfferEventFunction=FinalOfferEventFunction;
	aArrayOfEventHandlers[1].iPriority=KMinTInt;
	aArrayOfEventHandlers[1].iOfferEventFunction=FinalOfferEventFunction;
	aArrayOfEventHandlers[2].iPriority=KMinTInt;
	aArrayOfEventHandlers[2].iOfferEventFunction=FinalOfferEventFunction;
	aArrayOfEventHandlers[3].iPriority=KMinTInt;
	aArrayOfEventHandlers[3].iOfferEventFunction=FinalOfferEventFunction;
	if (aCompletedAdditionalEventSourceWithHighestPriority==NULL)
		{
		if (aCompletedOplAsynchronousRequestWithHighestPriority==NULL)
			{
			aArrayOfEventHandlers[0].iPriority=EActivePriorityWsEvents;
			aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToUserEventHandler;
			}
		else
			{
			const TInt priorityOfCompletedOplAsynchronousRequest=aCompletedOplAsynchronousRequestWithHighestPriority->Priority();
			if (priorityOfCompletedOplAsynchronousRequest>EActivePriorityWsEvents)
				{
				aArrayOfEventHandlers[0].iPriority=priorityOfCompletedOplAsynchronousRequest;
				aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToOplAsynchronousRequest;
				aArrayOfEventHandlers[1].iPriority=EActivePriorityWsEvents;
				aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToUserEventHandler;
				}
			else
				{
				aArrayOfEventHandlers[0].iPriority=EActivePriorityWsEvents;
				aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToUserEventHandler;
				aArrayOfEventHandlers[1].iPriority=priorityOfCompletedOplAsynchronousRequest;
				aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToOplAsynchronousRequest;
				}
			}
		}
	else
		{
		if (aCompletedOplAsynchronousRequestWithHighestPriority==NULL)
			{
			const TInt priorityOfCompletedAdditionalEventSource=aCompletedAdditionalEventSourceWithHighestPriority->Priority();
			if (priorityOfCompletedAdditionalEventSource>EActivePriorityWsEvents)
				{
				aArrayOfEventHandlers[0].iPriority=priorityOfCompletedAdditionalEventSource;
				aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToArrayOfAdditionalEventSources;
				aArrayOfEventHandlers[1].iPriority=EActivePriorityWsEvents;
				aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToUserEventHandler;
				}
			else
				{
				aArrayOfEventHandlers[0].iPriority=EActivePriorityWsEvents;
				aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToUserEventHandler;
				aArrayOfEventHandlers[1].iPriority=priorityOfCompletedAdditionalEventSource;
				aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToArrayOfAdditionalEventSources;
				}
			}
		else
			{
			const TInt priorityOfCompletedAdditionalEventSource=aCompletedAdditionalEventSourceWithHighestPriority->Priority();
			const TInt priorityOfCompletedOplAsynchronousRequest=aCompletedOplAsynchronousRequestWithHighestPriority->Priority();
			if (priorityOfCompletedAdditionalEventSource>priorityOfCompletedOplAsynchronousRequest)
				{
				if (EActivePriorityWsEvents>priorityOfCompletedAdditionalEventSource)
					{
					aArrayOfEventHandlers[0].iPriority=EActivePriorityWsEvents;
					aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToUserEventHandler;
					aArrayOfEventHandlers[1].iPriority=priorityOfCompletedAdditionalEventSource;
					aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToArrayOfAdditionalEventSources;
					aArrayOfEventHandlers[2].iPriority=priorityOfCompletedOplAsynchronousRequest;
					aArrayOfEventHandlers[2].iOfferEventFunction=OfferEventToOplAsynchronousRequest;
					}
				else
					{
					aArrayOfEventHandlers[0].iPriority=priorityOfCompletedAdditionalEventSource;
					aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToArrayOfAdditionalEventSources;
					if (priorityOfCompletedOplAsynchronousRequest>EActivePriorityWsEvents)
						{
						aArrayOfEventHandlers[1].iPriority=priorityOfCompletedOplAsynchronousRequest;
						aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToOplAsynchronousRequest;
						aArrayOfEventHandlers[2].iPriority=EActivePriorityWsEvents;
						aArrayOfEventHandlers[2].iOfferEventFunction=OfferEventToUserEventHandler;
						}
					else
						{
						aArrayOfEventHandlers[1].iPriority=EActivePriorityWsEvents;
						aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToUserEventHandler;
						aArrayOfEventHandlers[2].iPriority=priorityOfCompletedOplAsynchronousRequest;
						aArrayOfEventHandlers[2].iOfferEventFunction=OfferEventToOplAsynchronousRequest;
						}
					}
				}
			else
				{
				if (EActivePriorityWsEvents>priorityOfCompletedOplAsynchronousRequest)
					{
					aArrayOfEventHandlers[0].iPriority=EActivePriorityWsEvents;
					aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToUserEventHandler;
					aArrayOfEventHandlers[1].iPriority=priorityOfCompletedOplAsynchronousRequest;
					aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToOplAsynchronousRequest;
					aArrayOfEventHandlers[2].iPriority=priorityOfCompletedAdditionalEventSource;
					aArrayOfEventHandlers[2].iOfferEventFunction=OfferEventToArrayOfAdditionalEventSources;
					}
				else
					{
					aArrayOfEventHandlers[0].iPriority=priorityOfCompletedOplAsynchronousRequest;
					aArrayOfEventHandlers[0].iOfferEventFunction=OfferEventToOplAsynchronousRequest;
					if (priorityOfCompletedAdditionalEventSource>EActivePriorityWsEvents)
						{
						aArrayOfEventHandlers[1].iPriority=priorityOfCompletedAdditionalEventSource;
						aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToArrayOfAdditionalEventSources;
						aArrayOfEventHandlers[2].iPriority=EActivePriorityWsEvents;
						aArrayOfEventHandlers[2].iOfferEventFunction=OfferEventToUserEventHandler;
						}
					else
						{
						aArrayOfEventHandlers[1].iPriority=EActivePriorityWsEvents;
						aArrayOfEventHandlers[1].iOfferEventFunction=OfferEventToUserEventHandler;
						aArrayOfEventHandlers[2].iPriority=priorityOfCompletedAdditionalEventSource;
						aArrayOfEventHandlers[2].iOfferEventFunction=OfferEventToArrayOfAdditionalEventSources;
						}
					}
				}
			}
		}
	}

CCoeFep::TEventResponse CWsEventHandler::FinalOfferEventFunction(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters)
	{
	if (aParameters.iMaximumNumberOfCompletedEventsToHandle!=NULL)
		{
		aParameters.iDone=ETrue;
		}
	else
		{
		// Must corrsepond to an explicit IOSIGNAL OPL-call
		if (aThis.iIsSuspended || !(aParameters.iEventsRequired&EOplAsynchronousRequestCompletion))
			{
			++aParameters.iNumberOfSignalsToReplace;
			}
		else
			{
			aParameters.iDone=ETrue;
			}
		}
	return CCoeFep::EEventWasConsumed;
	}

CCoeFep::TEventResponse CWsEventHandler::OfferEventToArrayOfAdditionalEventSources(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters)
	{
	aParameters.iArrayOfAdditionalEventSources.MarkAsConsumed(aParameters.iCompletedAdditionalEventSourceWithHighestPriority);
	if (!aThis.iIsSuspended)
		{
		aParameters.iDone=ETrue;
		}
	return CCoeFep::EEventWasConsumed;
	}

CCoeFep::TEventResponse CWsEventHandler::OfferEventToOplAsynchronousRequest(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters)
	{
	if (aThis.iIsSuspended || !(aParameters.iEventsRequired&EOplAsynchronousRequestCompletion))
		{
		++aParameters.iNumberOfSignalsToReplace;
		}
	else
		{
		aThis.iIOCollection->RemoveCompletedOplAsynchronousRequest(aParameters.iCompletedOplAsynchronousRequestWithHighestPriority);
		aParameters.iDone=ETrue;
		}
	return CCoeFep::EEventWasConsumed;
	}

CCoeFep::TEventResponse CWsEventHandler::OfferEventToUserEventHandler(CWsEventHandler& aThis, SEventHandler::SParameters& aParameters)
	{
	// For Crystal we need to acknowledge focus changes because application views
	// get dimmed in the background. This method was altered by PhilS on
	// 24/10/2001 to include the HandleSpecialWsEvent() check below to
	// allow for this. Doing this check here ensures all event GET functions from
	// GET to GETEVENT still get Focus gained/lost events actioned before, for example
	// GET discards them (since it only stores key presses).
	const TUserEventsRequired userEventsRequired=(TUserEventsRequired)(aParameters.iEventsRequired&EUserEvents);
	if (aThis.iEventQueue->IsNotEmpty())
		{
		if (!aThis.iIsSuspended)
			{
			aThis.iEventQueue->PeekNextEvent(aParameters.iEvent);
			if ((userEventsRequired==EUserEventsAny) ||
				((userEventsRequired==EUserEventsKeysOnly) && (aParameters.iEvent.Type()==EEventKey)))
				{
				aThis.iEventQueue->ConsumeNextEvent();
				aParameters.iDone=ETrue;
				return CCoeFep::EEventWasConsumed;
				}
			if (aThis.EventCompletesAnyOplAsynchronousWservRequest(aParameters.iCoeEnv, aParameters.iEvent))
				{
				aThis.iEventQueue->ConsumeNextEvent();
				if (aParameters.iEventsRequired&EOplAsynchronousRequestCompletion)
					{
					aParameters.iDone=ETrue;
					}
				return CCoeFep::EEventWasConsumed;
				}
			}
		aThis.iEventQueue->PutNextEventInBackup();
		--aParameters.iNumberOfCompletedEventsHandled;	// we don't want the event queue swamping
														// *aParameters.iMaximumNumberOfCompletedEventsToHandle
														// (which it would do if there are
														// >*aParameters.iMaximumNumberOfCompletedEventsToHandle
														// events of the "wrong" type in the event
														// queue). Therefore we need to decrement
														// it to cancel the effect of it being
														// incremented at the end of the iteration
														// of the top-level loop.
		return CCoeFep::EEventWasConsumed;
		}
	if (aThis.iStatus!=KRequestPending)
		{
		//!!TODOUIQ Test this additional test.
		if(aThis.iStatus!=KErrCancel)
			aThis.iWs.GetEvent(aParameters.iEvent);
		aThis.iWs.EventReady(&aThis.iStatus);
		if ((aParameters.iFep==NULL) || (aThis.OfferEventToFep(*aParameters.iFep, aParameters.iEvent)==CCoeFep::EEventWasNotConsumed))
			{
			if (!aThis.iIsSuspended)
				{
				// Here we can do checks for special events/keys, calling HandleSpecialWsEvent()
				// if appropriate. Currently we check for:
				//
				// 1) EEventFocusGained
				// 2) EEventFocusLost
				// 3) The EKeyIncBrightness key
				const TEventCode type=(TEventCode)aParameters.iEvent.Type();
				const TKeyEvent* key=(TKeyEvent*)aParameters.iEvent.Key();
				if (type==EEventFocusGained || type==EEventFocusLost || key->iCode==EKeyIncBrightness)
					HandleSpecialWsEvent(aParameters.iEvent);
				// Now continue with the rest of the processing as normal			
				if ((userEventsRequired==EUserEventsAny) ||
					((userEventsRequired==EUserEventsKeysOnly) && (type==EEventKey)))
					{
					aParameters.iDone=ETrue;
					return CCoeFep::EEventWasConsumed;
					}
				if (aThis.EventCompletesAnyOplAsynchronousWservRequest(aParameters.iCoeEnv, aParameters.iEvent))
					{
					if (aParameters.iEventsRequired&EOplAsynchronousRequestCompletion)
						{
						aParameters.iDone=ETrue;
						}
					return CCoeFep::EEventWasConsumed;
					}
				}
			aThis.HandleError(aThis.iEventQueue->AppendEvent(aParameters.iEvent));
			}
		return CCoeFep::EEventWasConsumed;
		}
	return CCoeFep::EEventWasNotConsumed;
	}

CCoeFep::TEventResponse CWsEventHandler::OfferEventToActiveScheduler(CCoeFep* aFep, TInt aMinimumPriority)
	{
	TInt error=KErrNone;
	const CCoeFep::TEventResponse eventResponse=CCoeFep::OfferEventToActiveScheduler(aFep, error, *iEventQueue, aMinimumPriority);
	HandleError(error);
	return eventResponse;
	}

/*
 * Lots of problems with the FEP.
 */

const TInt KFepControlOffset=0x5C;

LOCAL_C CCoeControl* GetFepControl(const CCoeFep& aFep)
	{
	// Get a ptr to the current loaded fep. Look away from the code now...
	CCoeControl *theFepControl=(CCoeControl*)*((TInt*)((TInt8*)&aFep+KFepControlOffset));
	return theFepControl; 
	}

CCoeFep::TEventResponse CWsEventHandler::OfferEventToFep(CCoeFep& aFep, const TWsEvent& aEvent)
	// Note: may change iEventQueue
	{
	CCoeFep::TEventResponse eventResponse=CCoeFep::EEventWasNotConsumed;
	TInt error=KErrNone;
	const TEventCode eventCode=(TEventCode)aEvent.Type();

	// Work out the FEP CoeControl to use, if we've not cached it already.
	if (iFepControl==NULL)
		iFepControl=GetFepControl(aFep);

	switch (eventCode)
		{
	case EEventKey:
	case EEventKeyUp:
	case EEventKeyDown:
#if defined(__SERIES60__)
		// But we haven't set the iEventQueue ....
		TKeyResponse ignoreKeyResponse;
		TRAP(error,ignoreKeyResponse=iFepControl->OfferKeyEventL(*aEvent.Key(), eventCode));
#elif defined(__UIQ__)
		eventResponse = CCoeFep::EEventWasNotConsumed;
#else
		eventResponse=aFep.OfferKeyEvent(error, *iEventQueue, *aEvent.Key(), eventCode);
#endif
		break;
	case EEventPointer:
#if !defined(__UIQ__)
		eventResponse=aFep.OfferPointerEvent(error, *iEventQueue, *aEvent.Pointer(), (const CCoeControl*)aEvent.Handle());
#else
		eventResponse = CCoeFep::EEventWasNotConsumed;
#endif
		break;
	case EEventPointerBufferReady:
#if !defined(__UIQ__)
		eventResponse=aFep.OfferPointerBufferReadyEvent(error, *iEventQueue, (const CCoeControl*)aEvent.Handle());
#else
		eventResponse = CCoeFep::EEventWasNotConsumed;
#endif
		break;
	case EEventFocusGained:
		aFep.HandleGainingForeground();
		break;
	case EEventFocusLost:
		aFep.HandleLosingForeground();
		break;
	default:
		break;
		}
	HandleError(error);
	return eventResponse;
	}

void CWsEventHandler::HandleError(TInt aError)
	{
	if (aError!=KErrNone)
		{
		CActiveScheduler::Current()->Error(aError);
		}
	}

TInt16 CWsEventHandler::MapToOplMod(TUint aMod)
	{
	TInt16 oplMod=0;
	if (aMod==0)
		return oplMod;
	if (aMod&(EModifierLeftAlt|EModifierRightAlt|EModifierAlt))
		oplMod|=EOplModAlt;
	if (aMod&(EModifierLeftCtrl|EModifierRightCtrl|EModifierCtrl))
		oplMod|=EOplModCtrl;
	if (aMod&(EModifierLeftShift|EModifierRightShift|EModifierShift))
		oplMod|=EOplModShift;
	if (aMod&EModifierCapsLock)
		oplMod|=EOplModCapsLock;
	if (aMod&(EModifierLeftFunc|EModifierRightFunc|EModifierFunc))
		oplMod|=EOplModFunc;
	return oplMod;
	}

TRequestStatus& CWsEventHandler::PriorityKeyStatus()
	{
	return iPriorityKeyHandler->Status();
	}

void CWsEventHandler::HandlePriorityKeyCompletion()
	{
	iPriorityKeyHandler->HandleCompletion();
	}

TInt16 CWsEventHandler::MapKeys(TUint aKeyCode)
	{
	switch (aKeyCode)
		{
	case EKeyHome:
		aKeyCode=262;
		break;
	case EKeyEnd:
		aKeyCode=263;
		break;
	case EKeyPageUp:
		aKeyCode=260;
		break;
	case EKeyPageDown:
		aKeyCode=261;
		break;
	case EKeyLeftArrow:
		aKeyCode=259;
		break;
	case EKeyRightArrow:
		aKeyCode=258;
		break;
	case EKeyUpArrow:
		aKeyCode=256;
		break;
	case EKeyDownArrow:
		aKeyCode=257;
		break;
#if defined(__UIQ__)
	case EQuartzKeyConfirm:
#endif
	case EKeyMenu:
#if !defined(__S80_DP2_0__) && !defined(__SERIES90__)
// EEik...Key not available with 9500 SDK.
	case EEikSidebarMenuKey:
#endif
		aKeyCode=290;
		break;
	case EKeyHelp:
		aKeyCode=291;
		break;
	default:
		break;
		}
	return (TInt16)aKeyCode;
	}

void CWsEventHandler::AppendKeyEventToQueueL(TUint aKeyCode, TUint aModifiers)
	{
	TKeyEvent keyEvent;
	keyEvent.iCode=aKeyCode;
	keyEvent.iScanCode=0;
	keyEvent.iModifiers=aModifiers;
	keyEvent.iRepeats=0;
	iEventQueue->AppendKeyEventL(keyEvent);
	}

TPtrC CWsEventHandler::Command() const
	{
	return *iCommand;
	}

void CWsEventHandler::ClearCommand()
	{
	*iCommand=KNullDesC;
	}
