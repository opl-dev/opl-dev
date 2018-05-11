// SYSTEM.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <opxapi.h>

const TInt KOpxVersion=0x600;
const TInt KCellTrackOPXVersion=0x600;

const TInt KOplMaxStringLength=255;

enum TCBSListsStat
	{
	TCBSViewList=1,
//	TCBSTopicList=2,
	TCBSListOpen=10,
	TCBSListClose=11,
	TCBSListStat=12
	};

enum TCBSStat
	{
	TCBSon=1,
	TCBSoff=2,
	TCBSstat=3
	};

enum TCBSchoice
	{
	TCBSCount=1,
	TCBSCellId=2
	};

enum TPhoneInfoQuery
	{
	TMCC,
	TMNC,
	TLocalACode,
	TCellId,
	TNetStat,
	TSerialNum,
	TProviderName
	};

enum TProfileExtra
	{
	THead=1,
	TCar=2
	};

enum TPhoneInfoStatusUID
	{
	Tphoneoffon=1, // 0=off, 1=on
	Tnetwork=2, // 0=available, 1=not
	Tprofile=3  // something i dont know
	};

class CTlsDataOPXCellTrack;
class CCellTrackOPX;
class RCBSServ;

class RThreadHolder : public RThread
	{
public:
	void SetTReq(TRequestStatus& aTReq) {iTReq=&aTReq;};
	void SetTls(CCellTrackOPX*  aTls) {iTls=aTls;};
	CCellTrackOPX* TheTls() {return iTls;};
private:
	CCellTrackOPX* iTls;  // cached for deleting oneself
	TRequestStatus* iTReq; // optional for logon
	};

class RCBSServ : public RSessionBase
	{
public:
	RCBSServ();
	TInt Connect();
	TVersion Version() const;

	TInt turnon();
	TInt turnoff();
	TInt cbsstat();

	TInt viewlistclose();
	TInt viewlistopen();
	TInt viewliststat();
	};

class CCellTrackOPX :public CBase
	{
public:
	CCellTrackOPX();
	~CCellTrackOPX();
	TInt CheckThreadPointerL(RThreadHolder* aThread);
	static TInt LogonToThreadCallBack(TAny* aThread);

// mine
    // profile
    void SwitchProfile(OplAPI& aOplAPI) const;
    void ProfileNameLC(OplAPI& aOplAPI) const;
	void ActiveProfile(OplAPI& aOplAPI) const;
	void ActiveProfileName(OplAPI& aOplAPI) const;
    void ProfileCount(OplAPI& aOplAPI) const;
    void ExtraProfileName(OplAPI& aOplAPI) const;
	// cbs
	void CBSOnOffStat(OplAPI& aOplAPI) const;

	void CBSReadMsg(OplAPI& aOplAPI) const;
	void CBSMsgInfo(OplAPI& aOplAPI) const;

	void CBSListsOpenCloseStat(OplAPI& aOplAPI) const;
	void CBSListsInit(OplAPI& aOplAPI) const;

	// phoneinfo
	void CellInfo(OplAPI& aOplAPI) const;
	void Serial(OplAPI& aOplAPI) const;
	void Info(OplAPI& aOplAPI) const;
	void InfoUid(OplAPI& aOplAPI) const;

public:
	CArrayPtrFlat<RThreadHolder> iThreadArray;
	};

class CTlsDataOPXCellTrack : public COpxBase
	{
public:
	static CTlsDataOPXCellTrack* NewL(OplAPI& aOplAPI);
	void ConstructL();
	CTlsDataOPXCellTrack(OplAPI& aOplAPI);
	~CTlsDataOPXCellTrack() ;
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
	CCellTrackOPX* iSystemHandle;

	// the language extension procedures
	enum TExtensions
		{
		// profile
		ESwitchProfile=1,
		EProfileName=2,
		EProfileCount=3,
		EActiveProfile=4,
		EActiveProfileName=5,
		EExtraProfileName=6,
		// CBS
		ECBSOnOffStat=100,

		ECBSMessageRead=103, // parameter viewlist file and cellid
		ECBSMessageInfo=104, // parameter viewlist

		ECBSListsOpenCloseStat=106,
		ECBSListsInit=109,

		// phoneinfo
		ECellInfo=200,
		ESerial=201,
		EInfo=202,
		EInfoUid=203,
		};
	};

inline CTlsDataOPXCellTrack* TheTls() { return((CTlsDataOPXCellTrack *)Dll::Tls()); }
inline void SetTheTls(CTlsDataOPXCellTrack *theTls) { Dll::SetTls(theTls); }

#endif