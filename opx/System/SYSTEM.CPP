// SYSTEM.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <opxapi.h>
#include <ES_SOCK.H>
#include <coeutils.h>

#include "OPXUTIL.H"

#include <BARSREAD.H>
#include <coemain.h>
#include <APGTASK.H>
#include <apgwgnam.h>
#include <eikapp.h>

#if !defined(__SERIES60__)
#include <eikcfdlg.h>
#endif

#if defined(USE_SYSTIMER)
#include <systimer.h>
#endif

#if defined(USE_ETEL)
#if defined(__S80_DP2_0__)
#include <Etel3rdParty.h>
#else
#include <ETelBGSM.h> // for IMEI number access
#endif
#endif

#include <eikon.rsg>
#include <e32hal.h>
#include <hal.h>
#include <oplerr.h>
#include <f32file.h>
#include <OplDb.h>
#include <eikenv.h>
#include <eikdll.h>
#include <IR_SOCK.H>
#if defined(USE_PLP)
#include <plpsess.h>
#endif
#include <e32math.h>
//
#include <apgcli.h>
#include <apaid.h>
//
#include <s32file.h> // for CDictionaryFileStore
//
#include "System.h"

#if defined(USE_ETEL) && defined(__S80_DP2_0__)
// Class for getting phone information using active scheduler
CPhoneInfo::CPhoneInfo() : CActive(EPriorityStandard)
	{
	}

CPhoneInfo* CPhoneInfo::NewL(TPhoneInfoQuery aPhoneInfoQuery)
	{
	CPhoneInfo* self =new(ELeave) CPhoneInfo();
	CleanupStack::PushL(self);
	self->ConstructL(aPhoneInfoQuery);
	CleanupStack::Pop(self);
	return self;
	}

void CPhoneInfo::ConstructL(TPhoneInfoQuery aPhoneInfoQuery)
	{
	iPhoneInfoQuery=aPhoneInfoQuery;
	iPkg = new (ELeave) CTelephony::TPhoneIdV1Pckg(iV1);
	iTelephony = CTelephony::NewL();
	CActiveScheduler::Add(this);
	}

CPhoneInfo::~CPhoneInfo()
	{
	Cancel(); // if any request outstanding, calls DoCancel() to cleanup
	delete iTelephony;
	delete iPkg;
	}

void CPhoneInfo::GetPhoneInfo(TRequestStatus& aStatus)
	{
	aStatus = KRequestPending;
	iPhoneInfoStatus = &aStatus;

	iTelephony->GetPhoneId( iStatus, *iPkg );
	SetActive();
	CActiveScheduler::Start();
	}

void CPhoneInfo::RunL()
	{
	if ( (iStatus == KErrNone) )
		{ 
		if (iPhoneInfoQuery==TPhoneManufacturer)
			retrievedValue = (*iPkg)().iManufacturer;
		else if (iPhoneInfoQuery==TPhoneModelId)
			retrievedValue = (*iPkg)().iModel;
		else if (iPhoneInfoQuery==TPhoneSerialNumber)
			retrievedValue = (*iPkg)().iSerialNumber;
	}
	CActiveScheduler::Stop();

	User::RequestComplete(iPhoneInfoStatus,iStatus.Int());
	}

void CPhoneInfo::DoCancel()
	{
	Cancel();
	} 
#endif

CSystemOpx::CSystemOpx()
	:iThreadArray(4)
	{
	}

CSystemOpx::~CSystemOpx()
	{
	iThreadArray.ResetAndDestroy();
	if (iConnectedToSystemTimerServer)
		{
#if defined(USE_SYSTIMER)
		iSystemTimer.Close();
#endif
		}
	}

// System timers are only supported by Crystal.
// Other platforms will leave with Not Supported.

void CSystemOpx::ConnectToSystemTimerServerL()
	{
	if (iConnectedToSystemTimerServer)
		return;
#if defined(USE_SYSTIMER)
	User::LeaveIfError(iSystemTimer.Connect());
#else
	User::Leave(KOplErrNotSupported);
#endif
	iConnectedToSystemTimerServer=ETrue;
	}

void CSystemOpx::GetHAL(OplAPI& aOplAPI) const
	{ //SyHAL&:(aAttribute&,BYREF aValue&)
	TAny *pvalue=aOplAPI.PopPtrInt32();
	TInt value=aOplAPI.GetLong(pvalue);
	TInt32 attribute=aOplAPI.PopInt32();
	TInt error=HAL::Get((HALData::TAttribute)attribute,value);
	aOplAPI.PutLong(pvalue,value);
	aOplAPI.Push(TInt32(error));
	}

void CSystemOpx::SetHAL(OplAPI& aOplAPI) const
	{ //SySetHAL&:(aAttribute&,aValue&)
	TInt32 value=aOplAPI.PopInt32();
	TInt attribute=aOplAPI.PopInt32();
	TInt error=HAL::Set((HALData::TAttribute)attribute,value);
	aOplAPI.Push(TInt32(error));
	}

void CSystemOpx::GetTimer(OplAPI& aOplAPI)
	{ //SyGetTimer:(aTimerId&,BYREF aDuration&)
	TAny *pduration=aOplAPI.PopPtrInt32();
	TTimeIntervalSeconds durationSeconds(aOplAPI.GetLong(pduration));
#if defined(USE_SYSTIMER)
	TInt32 timerId=aOplAPI.PopInt32();
	ConnectToSystemTimerServerL();
	iSystemTimer.GetTimerL(STATIC_CAST(TSysTimer,timerId),durationSeconds);
#else
	aOplAPI.PopInt32();
	ConnectToSystemTimerServerL();
#endif
	aOplAPI.PutLong(pduration,durationSeconds.Int());
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetTimer(OplAPI& aOplAPI)
	{ //SySetTimer:(aTimerId&,aDuration&)
#if defined(USE_SYSTIMER)
	TInt32 duration=aOplAPI.PopInt32();
	TInt timerId=aOplAPI.PopInt32();
	ConnectToSystemTimerServerL();
	iSystemTimer.SetTimerL(STATIC_CAST(TSysTimer,timerId),duration);
#else
	aOplAPI.PopInt32();
	aOplAPI.PopInt32();
	ConnectToSystemTimerServerL();
#endif
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::ResetTimer(OplAPI& aOplAPI)
	{ //SyResetTimer:(aTimerId&)
#if defined(USE_SYSTIMER)
	TInt timerId=aOplAPI.PopInt32();
	ConnectToSystemTimerServerL();
	iSystemTimer.ResetTimerL(STATIC_CAST(TSysTimer,timerId));
#else
	aOplAPI.PopInt32();
	ConnectToSystemTimerServerL();
#endif
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetSoundEnabled(OplAPI& aOplAPI) const
	{
//	TInt32 state=
	aOplAPI.PopInt32();
#pragma message("system.cpp: !!TODO Support SetSoundEnabled()")
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetSoundDriverEnabled(OplAPI& aOplAPI) const
	{
//	TInt32 state=
	aOplAPI.PopInt32();
#pragma message("system.cpp: !!TODO Support SetSoundDriverEnabled()")
	aOplAPI.Push(TReal64(0.0));
	}

LOCAL_C void GetVol(TVolumeInfo& aVol, OplAPI& aOplAPI)
	{
	TInt32 drive=aOplAPI.PopInt32();
	if (drive<0 || drive>25)
		User::Leave(KOplErrInvalidArgs);
	RFs& fs=aOplAPI.DbManager()->FsSession();
	User::LeaveIfError(fs.Volume(aVol,drive));
	}

void CSystemOpx::VolumeSize(OplAPI& aOplAPI) const
	{
	TVolumeInfo vol;
	GetVol(vol, aOplAPI);
	TInt64 ret(vol.iSize);
	ret/=1024;
	aOplAPI.Push(TInt32(ret.GetTInt()));
	}

void CSystemOpx::VolumeSpaceFree(OplAPI& aOplAPI) const
	{
	TVolumeInfo vol;
	GetVol(vol, aOplAPI);
	TInt64 ret(vol.iFree);
	ret/=1024;
	aOplAPI.Push(TInt32(ret.GetTInt()));
	}

void CSystemOpx::VolumeUniqueID(OplAPI& aOplAPI) const
	{
	TVolumeInfo vol;
	GetVol(vol, aOplAPI);
	aOplAPI.Push(TInt32(vol.iUniqueID));
	}

void CSystemOpx::MediaType(OplAPI& aOplAPI) const
	{
	const TInt32 drive=aOplAPI.PopInt32();
	if (drive<0 || drive>25)
		User::Leave(KOplErrInvalidArgs);
	TDriveInfo info;
	RFs& fs=aOplAPI.DbManager()->FsSession();
	User::LeaveIfError(fs.Drive(info,drive));
	aOplAPI.Push(TInt32(info.iType));
	}

void CSystemOpx::IsReadOnly(OplAPI& aOplAPI) const
	{
	TPtrC file=aOplAPI.PopString();
	RFs& fs=aOplAPI.DbManager()->FsSession();
	TUint attVal;
	fs.Att(file,attVal);
	aOplAPI.Push(TInt32(attVal & KEntryAttReadOnly ? -1 : 0));
	}

void CSystemOpx::IsHidden(OplAPI& aOplAPI) const
	{
	TPtrC file=aOplAPI.PopString();
	RFs& fs=aOplAPI.DbManager()->FsSession();
	TUint attVal;
	fs.Att(file,attVal);
	aOplAPI.Push(TInt32(attVal & KEntryAttHidden ? -1 : 0));
	}

void CSystemOpx::IsSystem(OplAPI& aOplAPI) const
	{
	TPtrC file=aOplAPI.PopString();
	RFs& fs=aOplAPI.DbManager()->FsSession();
	TUint attVal;
	fs.Att(file,attVal);
	aOplAPI.Push(TInt32(attVal & KEntryAttSystem ? -1 : 0));
	}

void CSystemOpx::SetReadOnly(OplAPI& aOplAPI) const
	{ // 	SySetReadOnly:(file$,state&)
	TInt32 state=aOplAPI.PopInt32();
	TPtrC file=aOplAPI.PopString();
	RFs& fs=aOplAPI.DbManager()->FsSession();
	if (state)
		fs.SetAtt(file,KEntryAttReadOnly,0);
	else
		fs.SetAtt(file,0,KEntryAttReadOnly);
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetHiddenFile(OplAPI& aOplAPI) const
	{
	TInt32 state=aOplAPI.PopInt32();
	TPtrC file=aOplAPI.PopString();
	RFs& fs=aOplAPI.DbManager()->FsSession();
	if (state)
		fs.SetAtt(file,KEntryAttHidden,0);
	else
		fs.SetAtt(file,0,KEntryAttHidden);
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetSystemFile(OplAPI& aOplAPI) const
	{
	TInt32 state=aOplAPI.PopInt32();
	TPtrC file=aOplAPI.PopString();
	RFs& fs=aOplAPI.DbManager()->FsSession();
	if (state)
		fs.SetAtt(file,KEntryAttSystem,0);
	else
		fs.SetAtt(file,0,KEntryAttSystem);
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::DisplayTaskList(OplAPI& aOplAPI) const
	{
	aOplAPI.EikonEnv().DisplayTaskList();
	aOplAPI.Push(TReal64(0.0));
	}
	
void CSystemOpx::SetComputeMode(OplAPI& aOplAPI) const
	{
	TInt32 state=aOplAPI.PopInt32();
	if (state!=RWsSession::EPriorityControlDisabled && state!=RWsSession::EPriorityControlComputeOn&& state!=RWsSession::EPriorityControlComputeOff)
		User::Leave(KOplErrInvalidArgs);
	aOplAPI.WsSession().ComputeMode(RWsSession::TComputeMode(state));
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::LogonToThread(OplAPI& aOplAPI)
	{
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TInt tempId =aOplAPI.PopInt32();
	TThreadId id= *((TThreadId*)&tempId);
	RThreadHolder* thread=new(ELeave) RThreadHolder();
	TRAPD(err,iThreadArray.AppendL(thread));
	if (err)
		{
		delete thread;
		User::Leave(err);
		}
	User::LeaveIfError(thread->Open(id));
	TCallBack callBack(&(CSystemOpx::LogonToThreadCallBack),thread);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,EActivePriorityWsEvents+1,callBack);
	thread->SetTReq(tr);
	thread->SetTls(this);
	thread->Logon(tr);
 	thread->Resume(); 
	aOplAPI.Push(TReal64(0.0));
	}

TInt CSystemOpx::LogonToThreadCallBack(TAny* aThread)
	{
	RThreadHolder* thread=(RThreadHolder*)aThread;
	thread->Close();
	TInt pos=thread->TheTls()->CheckThreadPointerL(thread);
	thread->TheTls()->iThreadArray.Delete(pos);
	delete thread;
	return KErrNone; 
	}

void CSystemOpx::RunApp(OplAPI& aOplAPI) const
	{
	TInt32 cmd=aOplAPI.PopInt32();
	TPtrC tail=aOplAPI.PopString();
	TPtrC doc=aOplAPI.PopString();
	TPtrC lib=aOplAPI.PopString();
	if (cmd < EApaCommandOpen || cmd > EApaCommandRunWithoutViews) // min and max values.
		User::Leave(KOplErrInvalidArgs);
	CApaCommandLine* apaCommandLine=CApaCommandLine::NewLC();
	apaCommandLine->SetLibraryNameL(lib);
	apaCommandLine->SetDocumentNameL(doc);
	TPtrC8 tail8bit((TText8*)tail.Ptr(),tail.Size());
	apaCommandLine->SetTailEndL(tail8bit);
	apaCommandLine->SetCommandL(TApaCommand(cmd));
	TThreadId tId=EikDll::StartAppL(*apaCommandLine);
	aOplAPI.Push(*((TInt32*)&tId));
	CleanupStack::PopAndDestroy(); // apaCommandLine
	}

void CSystemOpx::RunExe(OplAPI& aOplAPI) const
	{
	TPtrC name=aOplAPI.PopString();
	TThreadId tId=EikDll::StartExeL(name);
	aOplAPI.Push(*((TInt32*)&tId));
	}

#ifdef __WINS__
typedef TInt (*ExeEntryPoint)();

LOCAL_C TInt ExeThreadStartFunction(TAny* aParam)
	{
	const TDesC& libraryName=(*(const TDesC*)aParam);
	RLibrary lib;
	TInt err=lib.Load(libraryName);
	User::Free(aParam);
	if (!err)
		{
		ExeEntryPoint exeFunc=(ExeEntryPoint)lib.Lookup(1);
		if (!exeFunc)
			err=KErrBadLibraryEntryPoint;
		else
			err=(*exeFunc)();
		}
		return(err);
	}
#endif

void CSystemOpx::RunExeWithCommandLine(OplAPI& aOplAPI) const
	{
	TPtrC cmdLine=aOplAPI.PopString();
	TPtrC exeName=aOplAPI.PopString();
	// Leave if the EXE doesn't exist in the first place!
	if (!ConeUtils::FileExists(exeName))
		User::Leave(KOplErrNotExists);
	TParse fileName;
	TThreadId threadId;
#ifdef __WINS__
#pragma warning(disable:4238)
		User::LeaveIfError(fileName.Set(exeName,&_L("Z:\\SYSTEM\\PROGRAMS\\.EXE"),NULL));
#pragma warning(default:4238)
		RThread process;
		
		TBuf<KMaxFileName> threadName;
		TPtrC baseName=fileName.Name();
		TInt err;
		TInt num=0;
		do
			{
			threadName.Format(_L("%S%02d"),&baseName,num++);
			err=process.Create(threadName,
								ExeThreadStartFunction,
								KDefaultStackSize,
								KDefaultMinHeapSize,
								KDefaultMaxHeapSize,
								NULL);
			}
		while(err==KErrAlreadyExists);
		User::LeaveIfError(err);
		RHeap* heap=process.Heap();
		RHeap* originalHeap=User::SwitchHeap(heap);
		HBufC* commandLine=fileName.FullName().Alloc();
		User::SwitchHeap(originalHeap);
		if (!commandLine)
			{
			process.Close();
			User::LeaveNoMemory();
			}
		process.SetInitialParameter(commandLine);
		threadId = process.Id();
#else
		TPtrC tempDes=_L("Z:\\SYSTEM\\PROGRAMS\\.EXE");
		User::LeaveIfError(fileName.Set(exeName,&tempDes,NULL));
		RProcess process;

		TInt err=process.Create(fileName.FullName(),cmdLine);
		User::LeaveIfError(err);
		// Get the threadId of the main thread in the exe's process
		TFullName fullName(process.Name());
		fullName.Append(_L("::Main"));
		TFindThread fT(fullName);
		fT.Next(fullName);
		RThread thread;
		thread.Open(fT);
		threadId = thread.Id();
		thread.Close();
#endif
	process.Resume();
	process.Close();
	aOplAPI.Push(*((TInt32*)&threadId));
	}

void CSystemOpx::RunDocument(OplAPI& aOplAPI) const
	{
//	It's illegal in GCC, but I'd like to do TBool aSwitchToIfRunning=((aOplAPI.PopInt16()==0) ? EFalse : ETrue);
	TBool aSwitchToIfRunning=aOplAPI.PopInt16();
	TFileName aDocFileName=aOplAPI.PopString();
	if (!ConeUtils::FileExists(aDocFileName))
		User::Leave(KOplErrNotExists);
	//
	// Code Compatible With ER5....
	//
	TThreadId threadId;
	if (aSwitchToIfRunning)
		{
		TApaTaskList taskList(aOplAPI.WsSession());
		TApaTask task=taskList.FindDoc(aDocFileName); // Check if doc is running
		if (task.Exists())
			{
			threadId=task.ThreadId();
			task.BringToForeground();
			aOplAPI.Push(*((TInt32*)&threadId));
			return;
			}
		}
	RApaLsSession appList;
	User::LeaveIfError(appList.Connect());
	appList.GetAllApps();

	TInt32 err = appList.StartDocument(aDocFileName,threadId);
	appList.Close();
	// -1 (KErrNotFound) maps to 'General Error' in OPL so
	// this is hopefully more meaningful
	if (err != KErrNone)
		{
		if (err == KErrNotFound)
			User::Leave(KOplErrNotSupported);
		else
			User::Leave(err);
		}
	aOplAPI.Push(*((TInt32*)&threadId));
	}

void CSystemOpx::KillCurrentProcess(OplAPI& aOplAPI) const
	{
	TInt32 reason=aOplAPI.PopInt32();
	RProcess proc;
	proc.Kill(reason);
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::KillProcess(OplAPI& aOplAPI) const
	{
	TInt32 reason=aOplAPI.PopInt32();
	TPtrC name=aOplAPI.PopString();
	RProcess proc;
	proc.Open(name);
	proc.Kill(reason);
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::TerminateCurrentProcess(OplAPI& aOplAPI) const
	{
	TInt32 reason=aOplAPI.PopInt32();
#ifdef __WINS__
	// Terminating the current process on stops the emulator
	// so kill the thread instead
	RThread thread;
	thread.Terminate(reason);
#else
	RProcess proc;
	proc.Terminate(reason);
#endif
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::TerminateProcess(OplAPI& aOplAPI) const
	{ //SyTerminateProcess&:(processName$,reasonCode&)
	TInt32 reason=aOplAPI.PopInt32();
	TPtrC name=aOplAPI.PopString();
	RProcess proc;
	TInt error=proc.Open(name);
	if (error==KErrNone)
		{
		proc.Terminate(reason);
		}
	aOplAPI.Push(TInt32(error));
	}

void CSystemOpx::Mod(OplAPI& aOplAPI) const
	{
	TInt32 right=aOplAPI.PopInt32();
	TInt32 left=aOplAPI.PopInt32();
	if (right==0)
		User::Leave(KErrDivideByZero); 
	aOplAPI.Push(left%right);
	}

void CSystemOpx::XOR(OplAPI& aOplAPI) const
	{
	TInt32 right=aOplAPI.PopInt32();
	TInt32 left=aOplAPI.PopInt32();
	aOplAPI.Push(left^right);
	}

void CSystemOpx::LoadRsc(OplAPI& aOplAPI) const
	{
	TPtrC file=aOplAPI.PopString();
	aOplAPI.Push(TInt32(aOplAPI.EikonEnv().AddResourceFileL(file)));
	}

void CSystemOpx::UnLoadRsc(OplAPI& aOplAPI) const
	{
	TInt file=aOplAPI.PopInt32();
	aOplAPI.EikonEnv().DeleteResourceFile(file);
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::ReadRsc(OplAPI& aOplAPI) const
	{
	TInt id=aOplAPI.PopInt32();
	TBuf<256> des;
	aOplAPI.EikonEnv().ReadResource(des,id);
	aOplAPI.PushL(des);
	}

void CSystemOpx::ReadRscLong(OplAPI& aOplAPI) const
	{
	TInt id=aOplAPI.PopInt32();
	TResourceReader reader;
	aOplAPI.EikonEnv().CreateResourceReaderLC(reader,id);
	aOplAPI.Push(TInt32(reader.ReadInt32()));
	}

void CSystemOpx::UidCheckSum(OplAPI& aOplAPI) const
	{
	TUid id3;
	id3.iUid=aOplAPI.PopInt32();
	TUid id2;
	id2.iUid=aOplAPI.PopInt32();
	TUid id1;
	id1.iUid=aOplAPI.PopInt32();
	TUidType uidType(id1,id2,id3);
	TCheckedUid check(uidType);
	TPtrC16 check16bit((TUint16*)check.Des().Ptr(),16);
	aOplAPI.PushL(check16bit);
	}

void CSystemOpx::SetPointerGrabOn(OplAPI& aOplAPI) const
	{
	TInt32 state=aOplAPI.PopInt32();
	TInt32 winId=aOplAPI.PopInt32();
	RBackedUpWindow win=aOplAPI.WindowFromIdL(winId);
	win.SetPointerGrab(TBool(state));
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::ClaimPointerGrab(OplAPI& aOplAPI) const
	{
	TInt32 state=aOplAPI.PopInt32();
	TInt32 winId=aOplAPI.PopInt32();
	RBackedUpWindow win=aOplAPI.WindowFromIdL(winId);
	win.ClaimPointerGrab(TBool(state));
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetPointerCapture(OplAPI& aOplAPI) const
//#pragma warning (disable:4710) // function not expanded.
	{
	TInt32 flags=aOplAPI.PopInt32();
	TInt32 winId=aOplAPI.PopInt32();
	RBackedUpWindow win=aOplAPI.WindowFromIdL(winId);
	win.SetPointerCapture(flags);
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::EndTask(OplAPI& aOplAPI) const
// Send Shut-down message. 
	{
	TInt prev =aOplAPI.PopInt32();
	TInt tempId =aOplAPI.PopInt32();
	TThreadId id= *((TThreadId*)&tempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(prev,id);
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winGid);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	CApaWindowGroupName* wgName=CApaWindowGroupName::NewLC(ws,winGid);
	if (wgName->IsSystem() || wgName->IsBusy() || !wgName->RespondsToShutdownEvent())
		User::Leave(KErrAccessDenied);
	CleanupStack::PopAndDestroy(); // wgName
	tApatsk.SendSystemEvent(EApaSystemEventShutdown);
	aOplAPI.Push(TInt32(winGid));
	}

void CSystemOpx::KillTask(OplAPI& aOplAPI) const
// Send Shut-down message Ignore apps.
	{
	TInt prev =aOplAPI.PopInt32();
	TInt tempId =aOplAPI.PopInt32();
	TThreadId id= *((TThreadId*)&tempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(prev,id);
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winGid);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	tApatsk.KillTask();
	aOplAPI.Push(TInt32(winGid));
	}

void CSystemOpx::ThreadIdFromOpenDoc(OplAPI& aOplAPI) const
	{
	TInt32* prev=aOplAPI.PopPtrInt32();
	TPtrC doc=aOplAPI.PopString();
	RWsSession& ws=aOplAPI.WsSession();
	TInt prevLive=aOplAPI.GetLong(prev);
	CApaWindowGroupName::FindByDocName(doc,ws,prevLive);
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(prevLive);
	aOplAPI.PutLong(prev,prevLive);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	TThreadId tId= tApatsk.ThreadId();
	aOplAPI.Push(*((TInt32*)&tId));
	}

void CSystemOpx::ThreadIdFromAppUid(OplAPI& aOplAPI) const
	{
	TInt32* prev=aOplAPI.PopPtrInt32();
	TUid uid;
	uid.iUid=(aOplAPI.PopInt32());
	RWsSession& ws=aOplAPI.WsSession();
	TInt prevLive=aOplAPI.GetLong(prev);
	CApaWindowGroupName::FindByAppUid(uid,ws,prevLive); 
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(prevLive);
	aOplAPI.PutLong(prev,prevLive);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	TThreadId tId= tApatsk.ThreadId();
	aOplAPI.Push(*((TInt32*)&tId));
	}

void CSystemOpx::SetForeground(OplAPI& aOplAPI) const
	{
	RWsSession& ws=aOplAPI.WsSession();
	RWindowGroup& rw = aOplAPI.RootWindow();
	TInt winId = rw.Identifier();
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winId);
	tApatsk.BringToForeground();
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetBackground(OplAPI& aOplAPI) const
	{
	RWsSession& ws=aOplAPI.WsSession();
	RWindowGroup& rw = aOplAPI.RootWindow();
	TInt winId = rw.Identifier();
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winId);
	tApatsk.SendToBackground();
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::SetBackgroundByThread(OplAPI& aOplAPI) const
	{
	TInt prev =aOplAPI.PopInt32();
	TInt tempId =aOplAPI.PopInt32();
	TThreadId id= *((TThreadId*)&tempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(prev,id);
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winGid);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	tApatsk.SendToBackground();
	aOplAPI.Push(TInt32(winGid));
	}

void CSystemOpx::SetForegroundByThread(OplAPI& aOplAPI) const
	{
	TInt prev =aOplAPI.PopInt32();
	TInt tempId =aOplAPI.PopInt32();
	TThreadId id= *((TThreadId*)&tempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(prev,id);
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winGid);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	tApatsk.BringToForeground();
	aOplAPI.Push(TInt32(winGid));
	}

void CSystemOpx::NextWindowGroupName(OplAPI& aOplAPI) const
	{
	TInt32* prev=aOplAPI.PopPtrInt32();
	TInt32 prevLive=aOplAPI.GetLong(prev);
	TInt tempId =aOplAPI.PopInt32();
	TThreadId id= *((TThreadId*)&tempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(prevLive,id);
	CApaWindowGroupName* wgName=CApaWindowGroupName::NewLC(ws,winGid);
	TBuf<255> name=wgName->WindowGroupName();
	CleanupStack::PopAndDestroy(); // wgName
	aOplAPI.PutLong(prev,winGid);
	aOplAPI.PushL(name);
	}

void CSystemOpx::NextWindowId(OplAPI& aOplAPI) const
	{
	TInt prev=aOplAPI.PopInt32();
	TInt tempId =aOplAPI.PopInt32();
	TThreadId id= *((TThreadId*)&tempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(prev,id);
	aOplAPI.Push(TInt32(winGid));
	}

void CSystemOpx::SendSwitchFilesMessageToApp(OplAPI& aOplAPI) const
	{
	TBool aCreateRatherThanOpen=aOplAPI.PopInt16();
	TPtrC aFileName = aOplAPI.PopString();
	TInt aPrevious = aOplAPI.PopInt32();
	TInt aTempId = aOplAPI.PopInt32();

	// Find our task
	TThreadId threadId= *((TThreadId*)&aTempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(aPrevious,threadId);
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winGid);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	// Now switch the file
	if (aCreateRatherThanOpen)
		tApatsk.SwitchCreateFile(aFileName);
	else
		tApatsk.SwitchOpenFile(aFileName);
	aOplAPI.Push(TInt32(winGid));
	}

void CSystemOpx::SendKeyEventToApp(OplAPI& aOplAPI) const
	{
	TKeyEvent event;
	event.iRepeats=aOplAPI.PopInt32();
	event.iModifiers=aOplAPI.PopInt32();
	event.iScanCode=aOplAPI.PopInt32();
	event.iCode=aOplAPI.PopInt32();
	TInt prev =aOplAPI.PopInt32();
	TInt tempId =aOplAPI.PopInt32();

	TThreadId id= *((TThreadId*)&tempId);
	RWsSession& ws=aOplAPI.WsSession();
	TInt winGid=ws.FindWindowGroupIdentifier(prev,id);
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(winGid);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	tApatsk.SendKey(event);
	aOplAPI.Push(TInt32(winGid));
	}

void CSystemOpx::CaptureKey(OplAPI& aOplAPI) const
	{
	TUint32 mod=aOplAPI.PopInt32();
	TUint32 msk=aOplAPI.PopInt32();
	TUint32 key=aOplAPI.PopInt32();
	RWindowGroup& rw = aOplAPI.RootWindow();
	TInt32 ret=rw.CaptureKey(key,msk,mod);
	User::LeaveIfError(ret);
	aOplAPI.Push(ret);
	}

void CSystemOpx::CancelCaptureKey(OplAPI& aOplAPI) const
	{
	TInt32 canc=aOplAPI.PopInt32();
	RWindowGroup& rw = aOplAPI.RootWindow();
	rw.CancelCaptureKey(canc);
	aOplAPI.Push(0.0);
	}

LOCAL_C void ParseNameL(RFs& aFs,TFileName& aName)
	{
	TParse parse;
	User::LeaveIfError(aFs.Parse(aName,parse));
	aName=parse.FullName();
	}

void CSystemOpx::FileOpenDialog(OplAPI& aOplAPI) const
	{ // SyOpenFileDialog$:(seedFile$,uid1&,uid2&,uid3&)
#if !defined(__SERIES60__)
	TInt uid3=aOplAPI.PopInt32();
	TInt uid2=aOplAPI.PopInt32();
	TUidType uidType(TUid::Uid(aOplAPI.PopInt32()),TUid::Uid(uid2),TUid::Uid(uid3));
	TFileName fName=aOplAPI.PopString();
	ParseNameL(aOplAPI.EikonEnv().FsSession(),fName);
	CEikFileOpenDialog* dialog=new(ELeave) CEikFileOpenDialog(&fName);
	dialog->SetRequiredUid(uidType);
	if (dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		{
		if (fName.Length()>KOplMaxStringLength)
			User::Leave(KOplErrStrTooLong);
		}
	else
		fName.Zero();
#else
	aOplAPI.PopInt32();
	aOplAPI.PopInt32();
	aOplAPI.PopInt32();
	TFileName fName=aOplAPI.PopString();
	User::Leave(KOplErrNotSupported);
#endif
	aOplAPI.PushL(fName);
	}

void CSystemOpx::FileCreateDialog(OplAPI& aOplAPI) const
	{ // SyCreateFileDialog$:(seedPath$)
	TFileName fName=aOplAPI.PopString();
	ParseNameL(aOplAPI.EikonEnv().FsSession(),fName);
#if !defined(__SERIES60__)
	if (CEikFileSaveAsDialog::RunDlgLD(&fName,NULL,R_EIK_TBUF_CREATE_NEW_FILE))
		{
		if (fName.Length()>KOplMaxStringLength)
			User::Leave(KOplErrStrTooLong);
		}
	else
		fName.Zero();
#else
	User::Leave(KOplErrNotSupported);
#endif
	aOplAPI.PushL(fName);
	}

void CSystemOpx::FileSaveAsDialog(OplAPI& aOplAPI) const
	{ // SySaveAsFileDialog$:(seedPath$,BYREF useNewFile%)
#if !defined(__SERIES60__)
	TInt16* useNewPtr=aOplAPI.PopPtrInt16();
	TFileName fName=aOplAPI.PopString();
	ParseNameL(aOplAPI.EikonEnv().FsSession(),fName);
	TBool useNewFile=(useNewPtr!=NULL)?(*useNewPtr!=0):EFalse;
	if (CEikFileSaveAsDialog::RunDlgLD(&fName,(useNewPtr!=NULL)?&useNewFile:NULL))
		{
		if (fName.Length()>KOplMaxStringLength)
			User::Leave(KOplErrStrTooLong);
		if (useNewPtr!=NULL)
			*useNewPtr = OpxUtil::OplBool16(useNewFile);
		}
	else
		fName.Zero();
#else
	aOplAPI.PopPtrInt16();
	TFileName fName=aOplAPI.PopString();
	User::Leave(KOplErrNotSupported);
#endif
	aOplAPI.PushL(fName);
	}

TInt CSystemOpx::CheckThreadPointerL(RThreadHolder* aKeyPtr)
	{
	TInt count=iThreadArray.Count();
	TInt ii;
	for(ii=0;ii<count;ii++)
		{
		if (iThreadArray[ii]==aKeyPtr)
			return ii;
		}
	User::Leave(KOplErrInvalidArgs);
	return ii;
	}

void CSystemOpx::ThreadIdFromCaption(OplAPI& aOplAPI) const
	{
	TInt32* prev=aOplAPI.PopPtrInt32();
	TPtrC doc=aOplAPI.PopString();
	RWsSession& ws=aOplAPI.WsSession();
	TInt prevLive=aOplAPI.GetLong(prev);
	CApaWindowGroupName::FindByCaption(doc,ws,prevLive); 
	TApaTask tApatsk(ws);
	tApatsk.SetWgId(prevLive);
	aOplAPI.PutLong(prev,prevLive);
	if (!tApatsk.Exists())
		User::Leave(KErrNotFound);
	TThreadId tId= tApatsk.ThreadId();
	aOplAPI.Push(*((TInt32*)&tId));
	}

void CSystemOpx::OSVersionMajor(OplAPI& aOplAPI) const
	{
	aOplAPI.Push(TInt32(User::Version().iMajor));
	}

void CSystemOpx::OSVersionMinor(OplAPI& aOplAPI) const
	{
	aOplAPI.Push(TInt32(User::Version().iMinor));
	}

void CSystemOpx::OSVersionBuild(OplAPI& aOplAPI) const
	{
	aOplAPI.Push(TInt32(User::Version().iBuild));
	}

void CSystemOpx::ROMVersionMajor(OplAPI& aOplAPI) const
	{
	TMachineInfoV1Buf buf;
	UserHal::MachineInfo(buf);
	TMachineInfoV1& t=buf();
	aOplAPI.Push(TInt32(t.iRomVersion.iMajor));
	}

void CSystemOpx::ROMVersionMinor(OplAPI& aOplAPI) const
	{
	TMachineInfoV1Buf buf;
	UserHal::MachineInfo(buf);
	TMachineInfoV1& t=buf();
	aOplAPI.Push(TInt32(t.iRomVersion.iMinor));
	}

void CSystemOpx::ROMVersionBuild(OplAPI& aOplAPI) const
	{
	TMachineInfoV1Buf buf;
	UserHal::MachineInfo(buf);
	TMachineInfoV1& t=buf();
	aOplAPI.Push(TInt32(t.iRomVersion.iBuild));
	}

void CSystemOpx::FileSize(OplAPI& aOplAPI) const
	{
	TFileName FileName(aOplAPI.PopString());
	RFs& TheFs=aOplAPI.DbManager()->FsSession();
	TEntry entry;
	User::LeaveIfError(TheFs.Entry(FileName,entry));
	aOplAPI.Push(TInt32(entry.iSize));
	}

// From SystInfo
void CSystemOpx::IsDirectory(OplAPI& aOplAPI) const
	{
	TPtrC fname = aOplAPI.PopString();
	TUint attValue;
	User::LeaveIfError(aOplAPI.EikonEnv().FsSession().Att(fname, attValue));
	aOplAPI.Push(TInt32((attValue & KEntryAttDir) ? 1 : 0));
	}

void CSystemOpx::IsPathVisible(OplAPI& aOplAPI) const
	{
	TPtrC fname = aOplAPI.PopString();
	TUint attValue;
	RFs &aFs = aOplAPI.EikonEnv().FsSession();
	TParse aParse;
	aParse.Set(fname, NULL, NULL);
	while (!aParse.IsRoot())
		{
		User::LeaveIfError(aFs.Att(aParse.DriveAndPath(), attValue));
		if (attValue & (KEntryAttHidden | KEntryAttSystem))
			{
			aOplAPI.Push(TInt32(0));
			return;
			}
		aParse.PopDir();
		}
	aOplAPI.Push(TInt32(1));
	}

void CSystemOpx::VolumeName(OplAPI& aOplAPI) const
	{
	TVolumeInfo vol;
	TInt32 drive= aOplAPI.PopInt32();
	if (drive<0 || drive>25)
		User::Leave(KOplErrInvalidArgs);
	aOplAPI.DbManager()->FsSession().Volume(vol, drive);
	aOplAPI.PushL(vol.iName);
	}

void CSystemOpx::UniqueFilename(OplAPI& aOplAPI) const
	{
	TBuf<256> fname = aOplAPI.PopString();
	User::LeaveIfError(CEikApplication::GenerateFileName(aOplAPI.EikonEnv().FsSession(), fname));
	aOplAPI.PushL(fname);
	}

void CSystemOpx::LanguageIndex(OplAPI& aOplAPI) const
	{
	TMachineInfoV2Buf buf;
	User::LeaveIfError(UserHal::MachineInfo(buf));
	TMachineInfoV2 info=buf();
	aOplAPI.Push(TInt32(info.iLanguageIndex));
	}

void CSystemOpx::GetOPXVersion(OplAPI& aOplAPI) const
	{
	TFileName aOPXName=aOplAPI.PopString();
	if (!ConeUtils::FileExists(aOPXName))
		User::Leave(KOplErrNotExists);
	TParse parse;
	_LIT(KOPXExtension,".OPX");
	parse.Set(aOPXName,NULL,NULL);
	if (parse.Ext().CompareF(KOPXExtension)!=KErrNone)
		User::Leave(KOplErrBadFileType);

	RLibrary opx;
	User::LeaveIfError(opx.Load(aOPXName));
	TLibraryFunction versionExport=opx.Lookup(2);
	if (versionExport==NULL) // Ordinal not found
		{
		opx.Close();
		User::Leave(KOplErrNotSupported);
		}
	TInt32 version=(TInt32)(*(versionExport))();
	opx.Close();
	aOplAPI.Push(version);
	}

void CSystemOpx::RemoteLinkStatus(OplAPI& 
#if defined(USE_PLP)
                                  aOplAPI
#endif
                                  ) const
	{
#if !defined(USE_PLP)
	User::Leave(KErrNotSupported);
#else
	RRemoteLink link;
	TRemoteLinkStatus status;
	User::LeaveIfError(link.Open());
	CleanupClosePushL(link);
	User::LeaveIfError(link.Status(status));
	CleanupStack::PopAndDestroy(); // link
	aOplAPI.Push(TInt32(status.iStatus));
#endif
	}

void CSystemOpx::RemoteLinkDisable(OplAPI& aOplAPI) const
	{
#if !defined(USE_PLP)
	aOplAPI.Push(0.0);
	User::Leave(KErrNotSupported);
#else
	RRemoteLink link;
	User::LeaveIfError(link.Open());
	CleanupClosePushL(link);
	User::LeaveIfError(link.Disable());
	CleanupStack::PopAndDestroy(); // link
	aOplAPI.Push(0.0);
#endif
	}

void CSystemOpx::RemoteLinkEnable(OplAPI& aOplAPI) const
	{
#if !defined(USE_PLP)
	aOplAPI.Push(0.0);
	User::Leave(KErrNotSupported);
#else
	RRemoteLink link;
	User::LeaveIfError(link.Open());
	CleanupClosePushL(link);
	User::LeaveIfError(link.Enable(EBps115200,KNullDesC,KNullDesC));
	CleanupStack::PopAndDestroy(); // link
	aOplAPI.Push(0.0);
#endif
	}

void CSystemOpx::RemoteLinkEnableWithOptions(OplAPI& aOplAPI) const
	{
#if !defined(USE_PLP)
	aOplAPI.PopInt16();
	aOplAPI.PopInt16();
	User::Leave(KOplErrNotSupported);
#else
	// Note: In each of the below two checks the ExxxxUnknown value is invalid
	TInt16 aLinkBaud = aOplAPI.PopInt16();
	if ((aLinkBaud < ELinkBps9600) || (aLinkBaud > ELinkBps4000000))
		User::Leave(KOplErrInvalidArgs);
	TInt16 aLinkType = aOplAPI.PopInt16();
	if ((aLinkType < ELinkTypeCable) || (aLinkType > ELinkTypeIrDA)) 
		User::Leave(KOplErrInvalidArgs);

	RRemoteLink link;
	User::LeaveIfError(link.Open());
	CleanupClosePushL(link);

	TBps linkBaud;
	switch (aLinkBaud)
		{
	case ELinkBps9600:
		linkBaud = EBps9600;
		break;
	case ELinkBps19200:
		linkBaud = EBps19200;
		break;
	case ELinkBps38400:
		linkBaud = EBps38400;
		break;
	case ELinkBps57600:
		linkBaud = EBps57600;
		break;
	case ELinkBps115200:
	default:
		linkBaud = EBps115200;
		break;
	case ELinkBps230400:
		linkBaud = EBps230400;
		break;
	case ELinkBps460800:
		linkBaud = EBps460800;
		break;
	case ELinkBps576000:
		linkBaud = EBps576000;
		break;
	case ELinkBps1152000:
		linkBaud = EBps1152000;
		break;
	case ELinkBps4000000:
		linkBaud = EBps4000000;
		break;
		}
	if (aLinkType == ELinkTypeCable)
		User::LeaveIfError(link.Enable(linkBaud, KPortComm0, KModuleEcuart));
	else
		User::LeaveIfError(link.Enable(linkBaud, KPortIrcomm0, KModuleIrcomm));
	CleanupStack::PopAndDestroy(); //link
	aOplAPI.Push(0.0);
#endif
	}

void CSystemOpx::RemoteLinkConfig(OplAPI& aOplAPI) const
	{
#if !defined(USE_PLP)
	aOplAPI.PopInt16();
	aOplAPI.PopInt16();
	User::Leave(KOplErrNotSupported);
#else
	TInt16 *aLinkBaud = aOplAPI.PopPtrInt16();
	TInt16 *aLinkType = aOplAPI.PopPtrInt16();

	RRemoteLink link;
	User::LeaveIfError(link.Open());
	CleanupClosePushL(link);
	TPortName port;
	TBps baud;
	User::LeaveIfError(link.Config(&baud,&port));

	if (!port.CompareF(KPortIrcomm0))
		*aLinkType=ELinkTypeIrDA;
	else if (!port.CompareF(KPortComm0))
		*aLinkType=ELinkTypeCable;
	else
		*aLinkType=ELinkTypeUnknown;
	switch (baud)
		{
	case EBps9600:
		*aLinkBaud = ELinkBps9600;
		break;
	case EBps19200:
		*aLinkBaud = ELinkBps19200;
		break;
	case EBps38400:
		*aLinkBaud = ELinkBps38400;
		break;
	case EBps57600:
		*aLinkBaud = ELinkBps57600;
		break;
	case EBps115200:
		*aLinkBaud = ELinkBps115200;
		break;
	case EBps230400:
		*aLinkBaud = ELinkBps230400;
		break;
	case EBps460800:
		*aLinkBaud = ELinkBps460800;
		break;
	case EBps576000:
		*aLinkBaud = ELinkBps576000;
		break;
	case EBps1152000:
		*aLinkBaud = ELinkBps1152000;
		break;
	case EBps4000000:
		*aLinkBaud = ELinkBps4000000;
		break;
	default:
		*aLinkBaud = ELinkBpsUnknown;
		break;
		}
	CleanupStack::PopAndDestroy(); // link
	aOplAPI.Push(0.0);
#endif
	}

void CSystemOpx::PWIsEnabled(OplAPI& aOplAPI) const
	{
	aOplAPI.Push(OpxUtil::OplBool16( Password::IsEnabled() ));
	}

void CSystemOpx::PWSetEnabled(OplAPI& aOplAPI) const
	{
	TBool enable = (aOplAPI.PopInt16() != 0);
	TPtrC passw = aOplAPI.PopString();
	Password::SetEnabled(passw, enable);
	aOplAPI.Push(0.0);
	}

void CSystemOpx::PWIsValid(OplAPI& aOplAPI) const
	{
	TPtrC passw = aOplAPI.PopString();
	aOplAPI.Push(OpxUtil::OplBool16( Password::IsValid(passw) ));
	}

void CSystemOpx::PWSet(OplAPI& aOplAPI) const
	{
	TPtrC npassw = aOplAPI.PopString();
	TPtrC opassw = aOplAPI.PopString();
	Password::Set(opassw, npassw);
	aOplAPI.Push(0.0);
	}

void CSystemOpx::Random(OplAPI& aOplAPI) const
	{ //SyRandom&:
	aOplAPI.Push(TInt32(Math::Random()));
	}

void CSystemOpx::InactivityTime(OplAPI& aOplAPI) const
	{ //SyInactivityTime&:
	TTimeIntervalSeconds inactiveTime=User::InactivityTime();
	aOplAPI.Push(TInt32(inactiveTime.Int()));
	}

void CSystemOpx::ResetInactivityTime(OplAPI& aOplAPI) const
	{ //SyResetInactivityTime:
	User::ResetInactivityTime();
	aOplAPI.Push(TReal64(0.0));
	}

void CSystemOpx::MemoryInformation(OplAPI& aOplAPI) const
	{ //SyMemoryInfo:
	TMemoryInfoV1Buf buf;
	User::LeaveIfError(UserHal::MemoryInfo(buf));
	TMemoryInfoV1 info=buf();
	TAny *pFreeRam = aOplAPI.PopPtrInt32();
	TAny *pMaxFreeRam = aOplAPI.PopPtrInt32();
	TAny *pRom = aOplAPI.PopPtrInt32();
	TAny *pRam = aOplAPI.PopPtrInt32();
	aOplAPI.PutLong(pRam, info.iTotalRamInBytes);
	aOplAPI.PutLong(pRom, info.iTotalRomInBytes);
	aOplAPI.PutLong(pMaxFreeRam, info.iMaxFreeRamInBytes);
	aOplAPI.PutLong(pFreeRam, info.iFreeRamInBytes);
	aOplAPI.Push(0.0);
	}

void CSystemOpx::DefaultDocumentFolder(OplAPI& aOplAPI) const
	{ //SyDefaultDocumentFolder$:
	TFileName defaultFolder;
	CDictionaryStore* store=CDictionaryFileStore::SystemLC(aOplAPI.DbManager()->FsSession());
	if (store->IsPresentL(KUidGlobalDefaultDocumentDirectory))
		{
		RDictionaryReadStream stream;
		stream.OpenLC(*store,KUidGlobalDefaultDocumentDirectory);
		stream >> defaultFolder;
		CleanupStack::PopAndDestroy(); //stream
		}
	else
		defaultFolder.Zero();
	CleanupStack::PopAndDestroy(); //store

	if (defaultFolder.Length()>KOplMaxStringLength)
		User::Leave(KOplErrStrTooLong);
	aOplAPI.PushL(defaultFolder);
	}


void CSystemOpx::GetPhoneInfo(OplAPI& aOplAPI) const
	{ // SyGetPhoneInfo$:(aInfoId%)
#if !defined(USE_ETEL)
	aOplAPI.PopInt16();
	User::Leave(KOplErrNotSupported);
#else
	// ETel required for Crystal
#if !defined(__S80_DP2_0__)
	TPhoneInfoQuery aQuery=(TPhoneInfoQuery)(aOplAPI.PopInt16());

	RTelServer server;
	User::LeaveIfError(server.Connect());
	CleanupClosePushL(server);

	_LIT(KModuleName,"PHONETSY");
   	User::LeaveIfError(server.LoadPhoneModule(KModuleName));

	RBasicGsmPhone gsmPhone;
	_LIT(KPhoneName,"Linda");
	User::LeaveIfError(gsmPhone.Open(server,KPhoneName));
	CleanupClosePushL(gsmPhone);
	RBasicGsmPhone::TId id;
	User::LeaveIfError(gsmPhone.GetGsmPhoneId(id));
	CleanupStack::PopAndDestroy(); // gsmPhone
	
	User::LeaveIfError(server.UnloadPhoneModule(KModuleName));
	CleanupStack::PopAndDestroy(); // server
	
	switch (aQuery)
		{
	case TPhoneManufacturer:
		aOplAPI.PushL(id.iManufacturerId);
		break;
	case TPhoneModelId:
		aOplAPI.PushL(id.iModelId);
		break;
	case TPhoneRevisionId:
		aOplAPI.PushL(id.iRevisionId);
		break;
	case TPhoneSerialNumber:
		aOplAPI.PushL(id.iSerialNumber);
		break;
	default:
		User::Leave(KOplErrInvalidArgs);
		}
#else
	// Etel for S80 DP 2.0
	TPhoneInfoQuery aQuery=(TPhoneInfoQuery)(aOplAPI.PopInt16());
	if ((aQuery==TPhoneRevisionId) || (aQuery>TPhoneSerialNumber)) // not available on S80
		User::Leave(KOplErrNotSupported);

	CPhoneInfo* phoneInfo = CPhoneInfo::NewL(aQuery);
	CleanupStack::PushL(phoneInfo);

	TRequestStatus status;
	phoneInfo->GetPhoneInfo(status);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());

	aOplAPI.PushL(phoneInfo->retrievedValue);
	CleanupStack::PopAndDestroy();
#endif 
#endif
	}

CTlsDataOPXSystem::CTlsDataOPXSystem(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

CTlsDataOPXSystem* CTlsDataOPXSystem::NewL(OplAPI& aOplAPI)
	{
	CTlsDataOPXSystem* This=new(ELeave) CTlsDataOPXSystem(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	CleanupStack::Pop();
	return This;
	}

void CTlsDataOPXSystem::ConstructL()
	{
	iSystemHandle= new(ELeave) CSystemOpx;
	} 

CTlsDataOPXSystem::~CTlsDataOPXSystem()
	{
	delete iSystemHandle;
	Dll::FreeTls();
	}

void CTlsDataOPXSystem::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case EGetHAL:
		iSystemHandle->GetHAL(iOplAPI);
		break;
	case ESetHAL:
		iSystemHandle->SetHAL(iOplAPI);
		break;

	case EGetTimer:
		iSystemHandle->GetTimer(iOplAPI);
		break;
	case ESetTimer:
		iSystemHandle->SetTimer(iOplAPI);
		break;
	case EResetTimer:
		iSystemHandle->ResetTimer(iOplAPI);
		break;

	case EIsReadOnly:
		iSystemHandle->IsReadOnly(iOplAPI);
		break;
	case EIsHidden:
		iSystemHandle->IsHidden(iOplAPI);
		break;
	case EIsSystem:
		iSystemHandle->IsSystem(iOplAPI);
		break;
	case ESetReadOnly:
		iSystemHandle->SetReadOnly(iOplAPI);
		break;
	case ESetHiddenFile:
		iSystemHandle->SetHiddenFile(iOplAPI);
		break;
	case ESetSystemFile:
		iSystemHandle->SetSystemFile(iOplAPI);
		break;
	case EVolumeSize:
		iSystemHandle->VolumeSize(iOplAPI);
		break;
	case EVolumeSpaceFree:
		iSystemHandle->VolumeSpaceFree(iOplAPI);
		break;
	case EVolumeUniqueID:
		iSystemHandle->VolumeUniqueID(iOplAPI);
		break;
	case EMediaType:
		iSystemHandle->MediaType(iOplAPI);
		break;
	case EFileSize:
		iSystemHandle->FileSize(iOplAPI);
		break;
	case EIsDirectory:
		iSystemHandle->IsDirectory(iOplAPI);
		break;
	case EVolumeName:
		iSystemHandle->VolumeName(iOplAPI);
		break;
	case EUniqueFilename:
		iSystemHandle->UniqueFilename(iOplAPI);
		break;
	case EIsPathVisible:
		iSystemHandle->IsPathVisible(iOplAPI);
		break;

	case ELogonToThread:
		iSystemHandle->LogonToThread(iOplAPI);
		break;
	case ERunApp:
		iSystemHandle->RunApp(iOplAPI);
		break;
	case ERunExe:
		iSystemHandle->RunExe(iOplAPI);
		break;
	case ERunExeWithCommandLine:
		iSystemHandle->RunExeWithCommandLine(iOplAPI);
		break;
	case ERunDocument:
		iSystemHandle->RunDocument(iOplAPI);
		break;
	case ETerminateCurrentProcess:
		iSystemHandle->TerminateCurrentProcess(iOplAPI);
		break;
	case ETerminateProcess:
		iSystemHandle->TerminateProcess(iOplAPI);
		break;
	case EKillCurrentProcess:
		iSystemHandle->KillCurrentProcess(iOplAPI);
		break;
	case EKillProcess:
		iSystemHandle->KillProcess(iOplAPI);
		break;
	case EEndTask:
		iSystemHandle->EndTask(iOplAPI);
		break;
	case EKillTask:
		iSystemHandle->KillTask(iOplAPI);
		break;
	case ESetComputeMode:
		iSystemHandle->SetComputeMode(iOplAPI);
		break;
	case EThreadIdFromOpenDoc:
		iSystemHandle->ThreadIdFromOpenDoc(iOplAPI);
		break;
	case EThreadIdFromAppUid:
		iSystemHandle->ThreadIdFromAppUid(iOplAPI);
		break;
	case EThreadIdFromCaption:
		iSystemHandle->ThreadIdFromCaption(iOplAPI);
		break;
	case ESetBackground:
		iSystemHandle->SetBackground(iOplAPI);
		break;
	case ESetForeground:
		iSystemHandle->SetForeground(iOplAPI);
		break;
	case ESetForegroundByThread:
		iSystemHandle->SetForegroundByThread(iOplAPI);
		break;
	case ESetBackgroundByThread:
		iSystemHandle->SetBackgroundByThread(iOplAPI);
		break;
	case ENextWindowGroupName:
		iSystemHandle->NextWindowGroupName(iOplAPI);
		break;
	case ENextWindowId:
		iSystemHandle->NextWindowId(iOplAPI);
		break;
	case ESendSwitchFilesMessageToApp:
		iSystemHandle->SendSwitchFilesMessageToApp(iOplAPI);
		break;
	case ESendKeyEventToApp:
		iSystemHandle->SendKeyEventToApp(iOplAPI);
		break;
	case ECaptureKey:
		iSystemHandle->CaptureKey(iOplAPI);
		break;
	case ECancelCaptureKey:
		iSystemHandle->CancelCaptureKey(iOplAPI);
		break;
	case EClaimPointerGrab:
		iSystemHandle->ClaimPointerGrab(iOplAPI);
		break;
	case ESetPointerGrabOn:
		iSystemHandle->SetPointerGrabOn(iOplAPI);
		break;
	case ESetPointerCapture:
		iSystemHandle->SetPointerCapture(iOplAPI);
		break;

	case ELoadRsc:
		iSystemHandle->LoadRsc(iOplAPI);
		break;
	case EUnLoadRsc:
		iSystemHandle->UnLoadRsc(iOplAPI);
		break;
	case EReadRsc:
		iSystemHandle->ReadRsc(iOplAPI);
		break;
	case EReadRscLong:
		iSystemHandle->ReadRscLong(iOplAPI);
		break;

	case EFileOpenDialog:
		iSystemHandle->FileOpenDialog(iOplAPI);
		break;
	case EFileCreateDialog:
		iSystemHandle->FileCreateDialog(iOplAPI);
		break;
	case EFileSaveAsDialog:
		iSystemHandle->FileSaveAsDialog(iOplAPI);
		break;

	case EROMVersionMajor:
		iSystemHandle->ROMVersionMajor(iOplAPI);
		break;
	case EROMVersionMinor:
		iSystemHandle->ROMVersionMinor(iOplAPI);
		break;
	case EROMVersionBuild:
		iSystemHandle->ROMVersionBuild(iOplAPI);
		break;
	case EOSVersionMajor:
		iSystemHandle->OSVersionMajor(iOplAPI);
		break;
	case EOSVersionMinor:
		iSystemHandle->OSVersionMinor(iOplAPI);
		break;
	case EOSVersionBuild:
		iSystemHandle->OSVersionBuild(iOplAPI);
		break;

	case EPWIsEnabled:
		iSystemHandle->PWIsEnabled(iOplAPI);
		break;
	case EPWSetEnabled:
		iSystemHandle->PWSetEnabled(iOplAPI);
		break;
	case EPWIsValid:
		iSystemHandle->PWIsValid(iOplAPI);
		break;
	case EPWSet:
		iSystemHandle->PWSet(iOplAPI);
		break;

	case ERemoteLinkStatus:
		iSystemHandle->RemoteLinkStatus(iOplAPI);
		break;
	case ERemoteLinkDisable:
		iSystemHandle->RemoteLinkDisable(iOplAPI);
		break;
	case ERemoteLinkEnable:
		iSystemHandle->RemoteLinkEnable(iOplAPI);
		break;
	case ERemoteLinkEnableWithOptions:
		iSystemHandle->RemoteLinkEnableWithOptions(iOplAPI);
		break;
	case ERemoteLinkConfig:
		iSystemHandle->RemoteLinkConfig(iOplAPI);
		break;

	case EDisplayTaskList:
		iSystemHandle->DisplayTaskList(iOplAPI);
		break;
	case EUidCheckSum:
		iSystemHandle->UidCheckSum(iOplAPI);
		break;
	case EMod:
		iSystemHandle->Mod(iOplAPI);
		break;
	case EXOR:
		iSystemHandle->XOR(iOplAPI);
		break;
	case ELanguageIndex:
		iSystemHandle->LanguageIndex(iOplAPI);
		break;
	case EGetOPXVersion:
		iSystemHandle->GetOPXVersion(iOplAPI);
		break;

	case ERandom:
		iSystemHandle->Random(iOplAPI);
		break;

	case EInactivityTime:
		iSystemHandle->InactivityTime(iOplAPI);
		break;
	case EResetInactivityTime:
		iSystemHandle->ResetInactivityTime(iOplAPI);
		break;

	case ESetSoundEnabled:
		iSystemHandle->SetSoundEnabled(iOplAPI);
		break;
	case ESetSoundDriverEnabled:
		iSystemHandle->SetSoundDriverEnabled(iOplAPI);
		break;

	case EMemoryInformation:
		iSystemHandle->MemoryInformation(iOplAPI);
		break;
	case EDefaultDocumentFolder:
		iSystemHandle->DefaultDocumentFolder(iOplAPI);
		break;
	case EGetPhoneInfo:
		iSystemHandle->GetPhoneInfo(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

//
// OPX loading interface
//
TBool CTlsDataOPXSystem::CheckVersion(TInt aVersion)
	// To check whether the opx is a compatible version
	// *** Change as required ***
	{
	if ((aVersion & 0xff00)>(KSystemOpxVersion & 0xff00)) // Major version must be <= OPX's version
		return EFalse;	// Bad version
	else
		return ETrue;	// OK
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	CTlsDataOPXSystem* tls=((CTlsDataOPXSystem*)Dll::Tls());
	if (tls==NULL)
		{
		tls=CTlsDataOPXSystem::NewL(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KSystemOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	//
	// DLL entry point
	//
	{
	return(KErrNone);
	}
