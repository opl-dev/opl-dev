// MediaServerOPX.h
//
// Copyright (c) 2000-2002 Symbian Ltd. All rights reserved.

#ifndef __MEDIASERVEROPX_H__
#define __MEDIASERVEROPX_H__

#include <MdaAudioTonePlayer.h>
#include <MdaAudioSamplePlayer.h>
#include <opxapi.h>
#include <oplerr.h>

// This version number also needs changing in .pkg and .txh files.
const TInt KOpxVersion=0x100;
const TInt KUidOpxMediaServer=0x100057DA;

//
// COpxMediaAudioPlayer
//
class COpxMediaAudioPlayer : public CBase, public MMdaAudioPlayerCallback
	{
public:
	COpxMediaAudioPlayer();
	~COpxMediaAudioPlayer();
	static COpxMediaAudioPlayer* NewL();
	void ConstructL();
	void PlayFileAL(OplAPI& aOplAPI);
	void StopFileL(OplAPI& aOplAPI);
	void CreateFilePlayerAL(OplAPI& aOplAPI);
	void CreateFilePlayerSimpleAL(OplAPI& aOplAPI);
	void CloseFilePlayerL(OplAPI& aOplAPI);
	void FileSetVolumeL(OplAPI& aOplAPI);
	void FileSetRepeatsL(OplAPI& aOplAPI);
	void FileSetVolumeRampL(OplAPI& aOplAPI);
	void DurationL(OplAPI& aOplAPI);
	void FileMaxVolumeL(OplAPI& aOplAPI);
private:
	void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
	void MapcPlayComplete(TInt aError);
	CMdaAudioPlayerUtility* iMdaAudioPlayerUtility;
	TInt iError;
	TRequestStatus* iRequestStatus;
	};

//
// COpxMediaAudioTone
//
class COpxMediaAudioTone : public CBase, public MMdaAudioToneObserver
	{
public:
	COpxMediaAudioTone();
	~COpxMediaAudioTone();
	static COpxMediaAudioTone* NewL();
	void ConstructL();
	void PrepareToPlayToneAL(OplAPI& aOplAPI);
	void PrepareToPlayFileSequenceAL(OplAPI& aOplAPI);
	void PlayToneAL(OplAPI& aOplAPI);
	void ToneState(OplAPI& aOPlAPI);
	void ToneMaxVolumeL(OplAPI& aOplAPI);
	void ToneVolumeL(OplAPI& aOplAPI);
	void ToneSetVolumeL(OplAPI& aOplAPI);
	void ToneSetPriorityL(OplAPI& aOplAPI);
	void ToneSetRepeatsL(OplAPI& aOplAPI);
	void ToneSetVolumeRampL(OplAPI& aOplAPI);
	void CancelPrepare(OplAPI& aOplAPI);
	void CancelPlay(OplAPI& aOplAPI);
	void Debug1(OplAPI& aOplAPI);
private:
	void MatoPrepareComplete(TInt aError);
	void MatoPlayComplete(TInt aError);
	CMdaAudioToneUtility* iMdaAudioToneUtility;
	TInt iError;
	TRequestStatus* iRequestStatus;
	};

//
// COpxMediaServerInterface
//
class COpxMediaServerInterface : public COpxBase
	{
public:
	static COpxMediaServerInterface* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
private:
	COpxMediaServerInterface(OplAPI& aOplAPI);
	void ConstructL();
	~COpxMediaServerInterface();
	COpxMediaAudioTone* iOpxMediaAudioTone;
	COpxMediaAudioPlayer* iOpxMediaAudioPlayer;
private:
	enum TExtensions
		{
		//
		// Tone player
		//
		EToneState=1,
		EToneMaxVolume,
		EToneVolume,
		EToneSetVolume,
		ESetPriority,
		ESetDTMFLengths,
		EToneSetRepeats,
		EToneSetVolumeRamp,
		EFixedSequenceCount,
		EFixedSequenceName, // 10

		EPrepareToPlayToneA,
		EPrepareToPlayDTMFStringA,
		EPrepareToPlayDesSequenceA,
		EPrepareToPlayFileSequenceA,
		EPrepareToPlayFixedSequenceA,
		ECancelPrepare,

		EPlayToneA,
		ECancelPlay,
		//
		// Audio player
		//
		ECreateFilePlayerA,
		ECloseFilePlayer,
		EPlayFileA,
		EStopFile,
	
		EFileSetVolume,
		EFileSetRepeats,
		EFileSetVolumeRamp,

		EFileDuration,
		EFileMaxVolume,

		ECreateFilePlayerSimpleA
		};
	};

#endif