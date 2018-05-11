// MediaServerOPX.cpp
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

//
// NOTE: Check that SoundDevice.mda and it's resource file are present on WINS
//

#include <coemain.h>
#include "mediaserveropx.h"

#define _DO_NOT_USE_FILELOGGING_
#ifdef _USE_FILELOGGING_
#include "flogger.h"
_LIT(KLoggingDir,"OPX");
_LIT(KLoggingFileName,"MediaServerOpx.log");

#define FLOGWRITE(a) RFileLogger::Write(KLoggingDir(),\
	KLoggingFileName(),\
	EFileLoggingModeAppend,\
	a)
#else
#define FLOGWRITE(a)
#endif

const TInt KStatusPriority=EActivePriorityWsEvents+1;

//
// COpxMediaAudioPlayer
//
COpxMediaAudioPlayer* COpxMediaAudioPlayer::NewL()
	{
	COpxMediaAudioPlayer* self=new(ELeave)COpxMediaAudioPlayer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

COpxMediaAudioPlayer::COpxMediaAudioPlayer()
	{
	}

COpxMediaAudioPlayer::~COpxMediaAudioPlayer()
	{
	delete iMdaAudioPlayerUtility;
	}

void COpxMediaAudioPlayer::ConstructL()
	{
	// Not needed yet
	}

void COpxMediaAudioPlayer::MapcPlayComplete(TInt aError)
	{
	User::RequestComplete(iRequestStatus,aError);
	iError=aError;
	}

void COpxMediaAudioPlayer::MapcInitComplete(TInt aError,const TTimeIntervalMicroSeconds& /*aDuration*/)
	{
	User::RequestComplete(iRequestStatus,aError);
	iError=aError;
	}

void COpxMediaAudioPlayer::CreateFilePlayerAL(OplAPI& aOplAPI)
	{
	// OPL call: CreateFilePlayerA:(aFileName$,aPriority&,aPreference&,BYREF aStatus&)
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TMdaPriorityPreference preference=(TMdaPriorityPreference)aOplAPI.PopInt32();
	TInt priority=aOplAPI.PopInt32();
	TPtrC filename=aOplAPI.PopString();

	if (iMdaAudioPlayerUtility)
		User::Leave(KOplErrAlreadyOpen);
	// No additional call back required here
	TCallBack callBack(NULL);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,KStatusPriority,callBack);
	tr=KRequestPending; // mark it as pending
	iRequestStatus=&tr;
	iMdaAudioPlayerUtility=CMdaAudioPlayerUtility::NewFilePlayerL(filename,
		*this,priority,preference);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioPlayer::CreateFilePlayerSimpleAL(OplAPI& aOplAPI)
	{
	// OPL call: CreateFilePlayerA:(aFileName$,BYREF aStatus&)
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TPtrC filename=aOplAPI.PopString();

	if (iMdaAudioPlayerUtility)
		User::Leave(KOplErrAlreadyOpen);
	// No additional call back required here
	TCallBack callBack(NULL);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,KStatusPriority,callBack);
	tr=KRequestPending; // mark it as pending
	iRequestStatus=&tr;
	iMdaAudioPlayerUtility=CMdaAudioPlayerUtility::NewFilePlayerL(filename,*this);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioPlayer::CloseFilePlayerL(OplAPI& aOplAPI)
	{
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	delete iMdaAudioPlayerUtility;
	iMdaAudioPlayerUtility=NULL;
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioPlayer::PlayFileAL(OplAPI& aOplAPI)
	{
	// OPL call: PlayFile:(BYREF aStatus&)
	TInt32* statusW=aOplAPI.PopPtrInt32();
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	TCallBack callBack(NULL);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,KStatusPriority,callBack);
	tr=KRequestPending; // mark it as pending
	iRequestStatus=&tr;
	iMdaAudioPlayerUtility->Play();
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioPlayer::FileMaxVolumeL(OplAPI& aOplAPI)
	{
	// OPL call: FileMaxVolume&:
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	aOplAPI.Push(TInt32(iMdaAudioPlayerUtility->MaxVolume()));
	}

void COpxMediaAudioPlayer::DurationL(OplAPI& aOplAPI)
	{
	// OPL call: Duration&:
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	TTimeIntervalMicroSeconds duration=iMdaAudioPlayerUtility->Duration();
	TInt32 lowDuration=duration.Int64().Low();
	aOplAPI.Push(lowDuration);
	}

void COpxMediaAudioPlayer::FileSetVolumeRampL(OplAPI& aOplAPI)
	{
	// OPL call: FileSetVolumeRamp:(aVolumeRampDuration&)
	TTimeIntervalMicroSeconds duration(TInt(aOplAPI.PopInt32()));
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	iMdaAudioPlayerUtility->SetVolumeRamp(duration);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioPlayer::FileSetRepeatsL(OplAPI& aOplAPI)
	{
	// OPL call: FileSetRepeats:(aRepeatNumberOfTimes&,aTrailingSilence&)
	TTimeIntervalMicroSeconds trailingSilence(TInt(aOplAPI.PopInt32()));
	TInt repeats=aOplAPI.PopInt32();
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	iMdaAudioPlayerUtility->SetRepeats(repeats,trailingSilence);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioPlayer::FileSetVolumeL(OplAPI& aOplAPI)
	{
	// OPL call: FileSetVolume:(aVolume&)
	TInt32 volume=aOplAPI.PopInt32();
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	iMdaAudioPlayerUtility->SetVolume(volume);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioPlayer::StopFileL(OplAPI& aOplAPI)
	{
	// OPL call: StopFile:
	if (!iMdaAudioPlayerUtility)
		User::Leave(KOplErrNotOpen);
	iMdaAudioPlayerUtility->Stop();
	aOplAPI.Push(0.0);
	}

//
// COpxMediaAudioTone
//
COpxMediaAudioTone* COpxMediaAudioTone::NewL()
	{
	COpxMediaAudioTone* self=new(ELeave)COpxMediaAudioTone();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

COpxMediaAudioTone::COpxMediaAudioTone()
	{
	}

COpxMediaAudioTone::~COpxMediaAudioTone()
	{
	delete iMdaAudioToneUtility;
	}

void COpxMediaAudioTone::ConstructL()
	{
	iMdaAudioToneUtility=CMdaAudioToneUtility::NewL(*this);
	}

void COpxMediaAudioTone::MatoPlayComplete(TInt aError)
	{
	User::RequestComplete(iRequestStatus,aError);
	iError=aError;
	}

void COpxMediaAudioTone::MatoPrepareComplete(TInt aError)
	{
	User::RequestComplete(iRequestStatus,aError);
	iError=aError;
	}

void COpxMediaAudioTone::PrepareToPlayToneAL(OplAPI& aOplAPI)
	{
	// OPL call: PrepareToPlayToneA:(aFrequency&,aDuration&,BYREF aStatus&)
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TTimeIntervalMicroSeconds duration(TInt(aOplAPI.PopInt32())); // thumb gcc needs a little help here
	TInt frequency=aOplAPI.PopInt32();

	// No additional call back required here
	TCallBack callBack(NULL);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,KStatusPriority,callBack);
	tr=KRequestPending; // mark it as pending
	iRequestStatus=&tr;
	iMdaAudioToneUtility->PrepareToPlayTone(frequency,duration);	
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::PrepareToPlayFileSequenceAL(OplAPI& aOplAPI)
	{
	// OPL call: PlaySeqFilename:(aFileName$,BYREF aStatus&)
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TPtrC filename=aOplAPI.PopString();
	TCallBack callBack(NULL);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,KStatusPriority,callBack);
	tr=KRequestPending; // mark it as pending
	iRequestStatus=&tr;
	iMdaAudioToneUtility->PrepareToPlayFileSequence(filename);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::ToneState(OplAPI& aOplAPI)
	{
	// OPL call: ToneState%:
	aOplAPI.Push(TInt16(iMdaAudioToneUtility->State()));
	}

void COpxMediaAudioTone::ToneMaxVolumeL(OplAPI& aOplAPI)
	{
	// OPL call: ToneMaxVolume&:
	aOplAPI.Push(TInt32(iMdaAudioToneUtility->MaxVolume()));
	}

void COpxMediaAudioTone::ToneSetVolumeL(OplAPI& aOplAPI)
	{
	// OPL call: ToneSetVolume:(aVolume&)
	TInt32 volume=aOplAPI.PopInt32();
	iMdaAudioToneUtility->SetVolume(volume);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::ToneVolumeL(OplAPI& aOplAPI)
	{
	// OPL call: ToneVolume&:
	aOplAPI.Push(TInt32(iMdaAudioToneUtility->Volume()));
	}

void COpxMediaAudioTone::ToneSetVolumeRampL(OplAPI& aOplAPI)
	{
	// OPL call: ToneSetVolumeRamp:(aVolumeRampDuration&)
	TTimeIntervalMicroSeconds duration(TInt(aOplAPI.PopInt32()));
	iMdaAudioToneUtility->SetVolumeRamp(duration);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::ToneSetRepeatsL(OplAPI& aOplAPI)
	{
	// OPL call: ToneSetRepeats:(aRepeatNumberOfTimes&,aTrailingSilence&)
	TTimeIntervalMicroSeconds trailingSilence(TInt(aOplAPI.PopInt32()));
	TInt repeats=aOplAPI.PopInt32();
	iMdaAudioToneUtility->SetRepeats(repeats,trailingSilence);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::ToneSetPriorityL(OplAPI& aOplAPI)
	{
	// OPL call: ToneSetPriority:(aPriority&,aPriorityPreference&)
	TMdaPriorityPreference preference=(TMdaPriorityPreference)aOplAPI.PopInt32();
	TInt priority=aOplAPI.PopInt32();
	iMdaAudioToneUtility->SetPriority(priority,preference);
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::CancelPlay(OplAPI& aOplAPI)
	{
	// OPL call: CancelPlay:
	iMdaAudioToneUtility->CancelPlay();
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::CancelPrepare(OplAPI& aOplAPI)
	{
	// OPL call: CancelPrepare:
	iMdaAudioToneUtility->CancelPrepare();
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::PlayToneAL(OplAPI& aOplAPI)
	{
	// OPL call: Play:(BYREF aStatus&)
	TInt32* statusW=aOplAPI.PopPtrInt32();
	TCallBack callBack(NULL);
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,KStatusPriority,callBack);
	tr=KRequestPending; // mark it as pending
	iRequestStatus=&tr;
	iMdaAudioToneUtility->Play();
	aOplAPI.Push(0.0);
	}

void COpxMediaAudioTone::Debug1(OplAPI& aOplAPI)
	{
	TInt32* statusW=aOplAPI.PopPtrInt32();
#if defined(_DEBUG)
	RDebug::Print(_L("TCallBack"));
#endif
	TCallBack callBack(NULL);
#if defined(_DEBUG)
	RDebug::Print(_L("NewRequestL()"));
#endif
	TRequestStatus& tr=aOplAPI.NewRequestL(statusW,KStatusPriority,callBack);
#if defined(_DEBUG)
	RDebug::Print(_L("Mark pending"));
#endif
	tr=KRequestPending; // mark it as pending
	iRequestStatus=&tr;
#if defined(_DEBUG)
	RDebug::Print(_L("User::RequestComplete()"));
#endif
	User::RequestComplete(iRequestStatus,KErrNone);
#if defined(_DEBUG)
	RDebug::Print(_L("Push()"));
#endif
	aOplAPI.Push(0.0);
	}

//
// COpxMediaServerInterface 
//
COpxMediaServerInterface::COpxMediaServerInterface(OplAPI& aOplAPI) 
	:COpxBase(aOplAPI)
	{
	}

COpxMediaServerInterface* COpxMediaServerInterface::NewLC(OplAPI& aOplAPI)
	{
	COpxMediaServerInterface* self=new(ELeave)COpxMediaServerInterface(aOplAPI);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void COpxMediaServerInterface::ConstructL()
	{
	iOpxMediaAudioTone=COpxMediaAudioTone::NewL();
	iOpxMediaAudioPlayer=COpxMediaAudioPlayer::NewL();
	}

COpxMediaServerInterface::~COpxMediaServerInterface()
	{
	delete iOpxMediaAudioPlayer;
	delete iOpxMediaAudioTone;
	Dll::FreeTls();
	}

void COpxMediaServerInterface::RunL(TInt aProcNum)
	{
	switch (aProcNum)
		{
	case EToneState:
		iOpxMediaAudioTone->ToneState(iOplAPI);
		break;
	case EToneMaxVolume:
		iOpxMediaAudioTone->ToneMaxVolumeL(iOplAPI);
		break;
	case EToneVolume:
		iOpxMediaAudioTone->ToneVolumeL(iOplAPI);
		break;
	case EToneSetVolume:
		iOpxMediaAudioTone->ToneSetVolumeL(iOplAPI);
		break;
	case EPrepareToPlayToneA:
		iOpxMediaAudioTone->PrepareToPlayToneAL(iOplAPI);
		break;
	case ESetPriority:
		iOpxMediaAudioTone->ToneSetPriorityL(iOplAPI);
		break;
	case EToneSetRepeats:
		iOpxMediaAudioTone->ToneSetRepeatsL(iOplAPI);
		break;
	case EToneSetVolumeRamp:
		iOpxMediaAudioTone->ToneSetVolumeRampL(iOplAPI);
		break;
	case ECancelPrepare:
		iOpxMediaAudioTone->CancelPrepare(iOplAPI);
		break;
	case ECancelPlay:
		iOpxMediaAudioTone->CancelPlay(iOplAPI);
		break;
	case EPrepareToPlayFileSequenceA:
		iOpxMediaAudioTone->PrepareToPlayFileSequenceAL(iOplAPI);
		break;
	case EPlayToneA:
		iOpxMediaAudioTone->PlayToneAL(iOplAPI);
		break;
	//
	// File player
	//
	case ECreateFilePlayerA:
		iOpxMediaAudioPlayer->CreateFilePlayerAL(iOplAPI);
		break;
	case ECloseFilePlayer:
		iOpxMediaAudioPlayer->CloseFilePlayerL(iOplAPI);
		break;
	case EPlayFileA:
		iOpxMediaAudioPlayer->PlayFileAL(iOplAPI);
		break;
	case EStopFile:
		iOpxMediaAudioPlayer->StopFileL(iOplAPI);
		break;
	case EFileSetVolume:
		iOpxMediaAudioPlayer->FileSetVolumeL(iOplAPI);
		break;
	case EFileSetRepeats:
		iOpxMediaAudioPlayer->FileSetRepeatsL(iOplAPI);
		break;
	case EFileSetVolumeRamp:
		iOpxMediaAudioPlayer->FileSetVolumeRampL(iOplAPI);
		break;
	case EFileDuration:
		iOpxMediaAudioPlayer->DurationL(iOplAPI);
		break;
	case EFileMaxVolume:
		iOpxMediaAudioPlayer->FileMaxVolumeL(iOplAPI);
		break;
	case ECreateFilePlayerSimpleA:
		iOpxMediaAudioPlayer->CreateFilePlayerSimpleAL(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxMediaServerInterface::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& iOplAPI)
	{
	COpxMediaServerInterface* tls=((COpxMediaServerInterface*)Dll::Tls());
	if (tls==NULL)
		{
		tls=COpxMediaServerInterface::NewLC(iOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop(tls);
		}
	return (COpxBase*)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}