// SYSTEM.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#if defined(USE_ETEL) && defined(__S80_DP2_0__)
#include <etel3rdparty.h>
#endif

const TInt KSystemOpxVersion = 0x650;

const TInt KOplMaxStringLength=255;

// Constant for obtaining default document folder
const TUid KUidGlobalDefaultDocumentDirectory = {0x10005127};

// Constants for running EXE with command line
const TInt KDefaultMinHeapSize=0x10000;
const TInt KDefaultMaxHeapSize=0x200000;

// Definitions for use with the Remote Link enabler functions
_LIT(KPortComm0,"COMM::0");
_LIT(KModuleEcuart,"ECUART");
_LIT(KPortIrcomm0,"IRCOMM::0");
_LIT(KModuleIrcomm,"IRCOMM");

enum TLinkTypes
	{
	ELinkTypeUnknown,	// 0
	ELinkTypeCable,		// 1
	ELinkTypeIrDA		// 2
	};

enum TLinkBauds
	{
	ELinkBpsUnknown,	// 0
	ELinkBps9600,		// 1
	ELinkBps19200,		// 2
	ELinkBps38400,		// 3
	ELinkBps57600,		// 4
	ELinkBps115200,		// 5
	ELinkBps230400,		// 6
	ELinkBps460800,		// 7
	ELinkBps576000,		// 8
	ELinkBps1152000,	// 9
	ELinkBps4000000		// 10
	};

enum TPhoneInfoQuery
	{
	TPhoneManufacturer,
	TPhoneModelId,
	TPhoneRevisionId,
	TPhoneSerialNumber
	};

#if defined(USE_ETEL) && defined(__S80_DP2_0__)
class CPhoneInfo : public CActive
	{
public:
	CPhoneInfo();
	void ConstructL(TPhoneInfoQuery aPhoneInfoQuery);
	~CPhoneInfo();
	static CPhoneInfo* NewL(TPhoneInfoQuery aPhoneInfoQuery);
	void GetPhoneInfo(TRequestStatus& iStatus);
	void DoCancel();
	void RunL();

private:
	CTelephony* iTelephony; // telephony object we own
	CTelephony::TPhoneIdV1 iV1;
	CTelephony::TPhoneIdV1Pckg* iPkg;

public:
	TRequestStatus* iPhoneInfoStatus;
	TBuf<50> retrievedValue;
	TPhoneInfoQuery iPhoneInfoQuery;
	};
#endif

class CTlsDataOPXSystem;
class CSystemOpx;
class RThreadHolder : public RThread
	{
public:
	void SetTReq(TRequestStatus& aTReq) {iTReq=&aTReq;};
	void SetTls(CSystemOpx*  aTls) {iTls=aTls;};
	CSystemOpx* TheTls() {return iTls;};
private:
	CSystemOpx* iTls;  // cached for deleting oneself
	TRequestStatus* iTReq; // optional for logon
	};

class CSystemOpx :public CBase
	{
public:
	CSystemOpx();
	~CSystemOpx();
	TInt CheckThreadPointerL(RThreadHolder* aThread);
	void ConnectToSystemTimerServerL();
	static TInt LogonToThreadCallBack(TAny* aThread);

// HAL
	void GetHAL(OplAPI& aOplAPI) const;
	void SetHAL(OplAPI& aOplAPI) const;
//	Timers
	void GetTimer(OplAPI& aOplAPI);
	void SetTimer(OplAPI& aOplAPI);
	void ResetTimer(OplAPI& aOPlAPI);
// File system
	void IsReadOnly(OplAPI& aOplAPI) const;
	void IsHidden(OplAPI& aOplAPI) const;
	void IsSystem(OplAPI& aOplAPI) const;
	void SetReadOnly(OplAPI& aOplAPI) const;
	void SetHiddenFile(OplAPI& aOplAPI) const;
	void SetSystemFile(OplAPI& aOplAPI) const;
	void VolumeSize(OplAPI& aOplAPI) const;
	void VolumeSpaceFree(OplAPI& aOplAPI) const;
	void VolumeUniqueID(OplAPI& aOplAPI) const;
	void MediaType(OplAPI& aOplAPI) const;
	void FileSize(OplAPI& aOplAPI) const;

	void IsDirectory(OplAPI& aOplAPI) const;
	void VolumeName(OplAPI& aOplAPI) const;
	void UniqueFilename(OplAPI& aOplAPI) const;
	void IsPathVisible(OplAPI& aOplAPI) const;
// Task
	void LogonToThread(OplAPI& aOplAPI);
	void RunApp(OplAPI& aOplAPI) const;
	void RunExe(OplAPI& aOplAPI) const;
	void RunExeWithCommandLine(OplAPI& aOplAPI) const;
	void RunDocument(OplAPI& aOplAPI) const;
	void TerminateCurrentProcess(OplAPI& aOplAPI) const;
	void TerminateProcess(OplAPI& aOplAPI) const;
	void KillCurrentProcess(OplAPI& aOplAPI) const;
	void KillProcess(OplAPI& aOplAPI) const;
	void EndTask(OplAPI& aOplAPI) const;
	void KillTask(OplAPI& aOplAPI) const;
	void SetComputeMode(OplAPI& aOplAPI) const;
	void ThreadIdFromOpenDoc(OplAPI& aOplAPI) const;
	void ThreadIdFromAppUid(OplAPI& aOplAPI) const;
	void ThreadIdFromCaption(OplAPI& aOplAPI) const;
	void SetBackground(OplAPI& aOplAPI) const;
	void SetForeground(OplAPI& aOplAPI) const;
	void SetForegroundByThread(OplAPI& aOplAPI) const;
	void SetBackgroundByThread(OplAPI& aOplAPI) const;
	void NextWindowGroupName(OplAPI& aOplAPI) const;
	void NextWindowId(OplAPI& aOplAPI) const;
	void SendSwitchFilesMessageToApp(OplAPI& aOplAPI) const;
	void SendKeyEventToApp(OplAPI& aOplAPI) const;
	void CaptureKey(OplAPI& aOplAPI) const;
	void CancelCaptureKey(OplAPI& aOplAPI) const;
	void ClaimPointerGrab(OplAPI& aOplAPI) const;
	void SetPointerGrabOn(OplAPI& aOplAPI) const;
	void SetPointerCapture(OplAPI& aOplAPI) const;
// Rsc
	void LoadRsc(OplAPI& aOplAPI) const;
	void UnLoadRsc(OplAPI& aOplAPI) const;
	void ReadRsc(OplAPI& aOplAPI) const;
	void ReadRscLong(OplAPI& aOplAPI) const;
// Dialogs
	void FileOpenDialog(OplAPI& aOplAPI) const;
	void FileCreateDialog(OplAPI& aOplAPI) const;
	void FileSaveAsDialog(OplAPI& aOplAPI) const;
// Version
	void ROMVersionMajor(OplAPI& aOplAPI) const;
	void ROMVersionMinor(OplAPI& aOplAPI) const;
	void ROMVersionBuild(OplAPI& aOplAPI) const;
	void OSVersionMajor(OplAPI& aOplAPI) const;
	void OSVersionMinor(OplAPI& aOplAPI) const;
	void OSVersionBuild(OplAPI& aOplAPI) const;
// Password
	void PWIsEnabled(OplAPI& aOplAPI) const;
	void PWSetEnabled(OplAPI& aOplAPI) const;
	void PWIsValid(OplAPI& aOplAPI) const;
	void PWSet(OplAPI& aOplAPI) const;
// Remote link
	void RemoteLinkStatus(OplAPI& aOplAPI) const;
	void RemoteLinkDisable(OplAPI& aOplAPI) const;
	void RemoteLinkEnable(OplAPI& aOplAPI) const;
	void RemoteLinkEnableWithOptions(OplAPI& aOplAPI) const;
	void RemoteLinkConfig(OplAPI& aOplAPI) const;
// Misc
	void DisplayTaskList(OplAPI& aOplAPI) const;
	void UidCheckSum(OplAPI& aOplAPI) const;
	void Mod(OplAPI& aOplAPI) const;
	void XOR(OplAPI& aOplAPI) const;
	void LanguageIndex(OplAPI& aOplAPI) const;
	void GetOPXVersion(OplAPI& aOplAPI) const;

	void Random(OplAPI& aOplAPI) const;

	void InactivityTime(OplAPI& aOplAPI) const;
	void ResetInactivityTime(OplAPI& aOplAPI) const;
// Sound
	void SetSoundEnabled(OplAPI& aOplAPI) const;
	void SetSoundDriverEnabled(OplAPI& aOplAPI) const;
// Additional functions after first release was made
	void MemoryInformation(OplAPI& aOplAPI) const;
	void DefaultDocumentFolder(OplAPI& aOplAPI) const;
	void GetPhoneInfo(OplAPI& aOplAPI) const;
public:
	CArrayPtrFlat<RThreadHolder> iThreadArray;
private:
#if defined(USE_SYSTIMER)
	RSystemTimer iSystemTimer;
#endif
	TBool iConnectedToSystemTimerServer;
	};

class CTlsDataOPXSystem : public COpxBase 
	{
public:
	static CTlsDataOPXSystem* NewL(OplAPI& aOplAPI);
	void ConstructL();
	CTlsDataOPXSystem(OplAPI& aOplAPI);
	~CTlsDataOPXSystem() ;
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
	CSystemOpx* iSystemHandle;
	// the language extension procedures
	enum TExtensions
		{
		EGetHAL=1,
		ESetHAL,

		EGetTimer,
		ESetTimer,
		EResetTimer,

		EIsReadOnly,
		EIsHidden,
		EIsSystem,
		ESetReadOnly,
		ESetHiddenFile,
		ESetSystemFile,
		EVolumeSize,
		EVolumeSpaceFree,
		EVolumeUniqueID,
		EMediaType,
		EFileSize,

		EIsDirectory,
		EVolumeName,
		EUniqueFilename,
		EIsPathVisible,

		ELogonToThread,
		ERunApp,
		ERunExe,
		ERunExeWithCommandLine,
		ERunDocument,
		ETerminateCurrentProcess,
		ETerminateProcess,
		EKillCurrentProcess,
		EKillProcess,
		EEndTask,
		EKillTask,
		ESetComputeMode,
		EThreadIdFromOpenDoc,
		EThreadIdFromAppUid,
		EThreadIdFromCaption,
		ESetBackground,
		ESetForeground,
		ESetForegroundByThread,
		ESetBackgroundByThread,
		ENextWindowGroupName,
		ENextWindowId,
		ESendSwitchFilesMessageToApp,
		ESendKeyEventToApp,
		ECaptureKey,
		ECancelCaptureKey,
		EClaimPointerGrab,
		ESetPointerGrabOn,
		ESetPointerCapture,

		ELoadRsc,
		EUnLoadRsc,
		EReadRsc,
		EReadRscLong,

		EFileOpenDialog,
		EFileCreateDialog,
		EFileSaveAsDialog,

		EROMVersionMajor,
		EROMVersionMinor,
		EROMVersionBuild,
		EOSVersionMajor,
		EOSVersionMinor,
		EOSVersionBuild,

		EPWIsEnabled,
		EPWSetEnabled,
		EPWIsValid,
		EPWSet,

		ERemoteLinkStatus,
		ERemoteLinkDisable,
		ERemoteLinkEnable,
		ERemoteLinkEnableWithOptions,
		ERemoteLinkConfig,

		EDisplayTaskList,
		EUidCheckSum,
		EMod,
		EXOR,
		ELanguageIndex,
		EGetOPXVersion,

		ERandom,

		EInactivityTime,
		EResetInactivityTime,

		ESetSoundEnabled,
		ESetSoundDriverEnabled,

		EMemoryInformation,
		EDefaultDocumentFolder,
		EGetPhoneInfo
		};
	};

inline CTlsDataOPXSystem* TheTls() { return((CTlsDataOPXSystem *)Dll::Tls()); }
inline void SetTheTls(CTlsDataOPXSystem *theTls) { Dll::SetTls(theTls); }

#endif
