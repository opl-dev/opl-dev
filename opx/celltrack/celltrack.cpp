// celltrack.cpp

#include <e32base.h>
#include <e32cons.h>
#include <e32std.h>

#include <etel.h>
#include <etelbgsm.h>
#include <cdbcols.h>
#include <commdb.h>

#include "CellTrack.h"
#include "profileengine.h"
#include "OPXUTIL.H"

#include <oplerr.h>
#include <OplDb.h>
#include <eikdll.h>

#include <badesca.h> //cptrcarray

#include <cbsserver.h>
#include <cbsserversession.h>
#include <cbsclientserver.h>

#include <saclient.h> // for getinfo

#include <bautils.h> // for cbsviewlistinit - check for file existance

///////////////////////////  cbs - class to access cbsserver ?/////////////////
RCBSServ::RCBSServ()
	{
	}

TInt RCBSServ::Connect()
	{
	TInt r=StartThread();
	if (r==KErrNone)
		r=CreateSession(CBS_SERVER_NAME,Version(),4);
	return r;
	}

TVersion RCBSServ::Version(void) const
	{
	return(TVersion(KCBSServerMajorVersionNumber,KCBSServerMinorVersionNumber,KCBSServerBuildVersionNumber));
	}

TInt RCBSServ::turnon()
	{
	TAny *p[KMaxMessageArguments];
	return Send(ECBSServerSetReceptionStatusOn,p);
	}

TInt RCBSServ::turnoff()
	{
	TAny *p[KMaxMessageArguments];
	return Send(ECBSServerSetReceptionStatusOff,p);
	}

TInt RCBSServ::cbsstat()
	{
	TInt res=0;
	TPckgBuf<TInt> pckg;
	TAny *p[KMaxMessageArguments];
	p[0]=(TAny *) &pckg;
	SendReceive(ECBSServerGetReceptionOnOffValue,p);
	res = pckg();
	return res;
	}

TInt RCBSServ::viewlistclose()
	{
	TAny *p[KMaxMessageArguments];
	return Send(ECBSServerSetMsgFileOpenStatusOff,p);
	}

TInt RCBSServ::viewlistopen()
	{
	TAny *p[KMaxMessageArguments];
	return Send(ECBSServerSetMsgFileOpenStatusOn,p);
	}

TInt RCBSServ::viewliststat()
	{
	TInt res=0;
	TPckgBuf<TInt> pckg;
	TAny *p[KMaxMessageArguments];
	p[0]=(TAny *) &pckg;
	SendReceive(ECBSServerGetMsgFileOpenValue,p);
	res = pckg();
	return res;
	}

////////////////////////// cbs opx ///////////////////////////////
void CCellTrackOPX::CBSOnOffStat(OplAPI& aOplAPI) const
	{

	TCBSStat choice = (TCBSStat) aOplAPI.PopInt16();

	RCBSServ cs;

	User::LeaveIfError(cs.Connect());
	CleanupClosePushL(cs);

	TInt stat = -1;
	if (choice == TCBSon)
		stat = cs.turnon();
	else if (choice == TCBSoff)
		stat = cs.turnoff();
	else if (choice == TCBSstat)
		stat = cs.cbsstat();
	else
	{ User::Leave(KOplErrInvalidArgs); }

	CleanupStack::PopAndDestroy(1); // cs
	aOplAPI.Push(TInt16(stat));
	}

void CCellTrackOPX::CBSListsOpenCloseStat(OplAPI& aOplAPI) const
	{

	TCBSListsStat choice = (TCBSListsStat) aOplAPI.PopInt16();
//	TCBSListsStat listchoice = (TCBSListsStat) aOplAPI.PopInt16();

	RCBSServ cs;

	User::LeaveIfError(cs.Connect());
	CleanupClosePushL(cs);

	TInt stat = -1;
	if (choice == TCBSListOpen)
		stat = cs.viewlistopen();
	else if (choice == TCBSListClose)
		stat = cs.viewlistclose();
	else if (choice == TCBSListStat)
		stat = cs.viewliststat();
	else
		User::Leave(KOplErrInvalidArgs); 
	CleanupStack::PopAndDestroy(1); // cs

	aOplAPI.Push(TInt16(stat));
	}

void CCellTrackOPX::CBSListsInit(OplAPI& aOplAPI) const
	{
    TPtrC viewlist = aOplAPI.PopString();

	RFs fsSession;
 	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL(fsSession);
	TParse	filestorename;

	TInt res = -1;
	TBool resfile;

	// viewlist
	User::LeaveIfError(fsSession.Parse(viewlist,filestorename));
   	resfile = User::LeaveIfError(BaflUtils::FileExists(fsSession,filestorename.FullName()));
	if (!resfile)
    {
		CBSMsgRoot::InitializeViewListFileL();
		res = 0;
	}
	else
	{
		res--;
	}

	CleanupStack::PopAndDestroy(); // fsession

	aOplAPI.Push(TInt16(res));

	}

void CCellTrackOPX::CBSReadMsg(OplAPI& aOplAPI) const
	{

	TInt index = (TInt) aOplAPI.PopInt16();
	TUint cellid = (TUint) aOplAPI.PopInt16();
    TPtrC viewlist = aOplAPI.PopString();

	RFs fsSession;
	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL(fsSession);
	TParse	filestorename;
	User::LeaveIfError(fsSession.Parse(viewlist,filestorename));

    CFileStore* store = CPermanentFileStore::OpenLC(fsSession,filestorename.FullName(),EFileWrite);
	store->SetTypeL(store->Layout());

	CBSMsgRoot* cmroot = CBSMsgRoot::NewLC(*store,store->Root());

	CBSTMessage* msg = CBSTMessage::NewL();
	CleanupStack::PushL( msg );

	STMsgRoot a = cmroot->MsgRootItem(index);
	cmroot->GetMsgL(a.iTSid,msg);

	TBuf<KViewMessageSize> msgtext = msg->iViewMessage;

	CleanupStack::PopAndDestroy(4); // store, cmroot, msg, fsession

	if (cellid == a.iCellId )
	{ aOplAPI.PushL(msgtext); }
	else
	{ aOplAPI.PushL(_L("")); }

	}

void CCellTrackOPX::CBSMsgInfo(OplAPI& aOplAPI) const
	{
	TInt index = (TInt) aOplAPI.PopInt16();
	TCBSchoice choice = (TCBSchoice) aOplAPI.PopInt16();
    TPtrC viewlist = aOplAPI.PopString();

	RFs fsSession;
 	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL(fsSession);
	TParse	filestorename;
	User::LeaveIfError(fsSession.Parse(viewlist,filestorename));

	TInt res = -1;

    CFileStore* store = CPermanentFileStore::OpenLC(fsSession,filestorename.FullName(),EFileWrite);
	store->SetTypeL(store->Layout());

	CBSMsgRoot* cmroot = CBSMsgRoot::NewLC(*store,store->Root());

	if (choice == TCBSCount)
		{
		res=cmroot->Count();
		}
	else if (choice == TCBSCellId)
		{
		CBSTMessage* msg = CBSTMessage::NewL();
		CleanupStack::PushL( msg );
		STMsgRoot a = cmroot->MsgRootItem(index);
		CleanupStack::PopAndDestroy(1); //msg
		res = a.iCellId;
		}

	CleanupStack::PopAndDestroy(3); // store, cmroot, fsession
	aOplAPI.Push(TInt16(res));
	}

////////////////////////// profile opx //////////////////////////

void CCellTrackOPX::SwitchProfile(OplAPI& aOplAPI) const
	{
    TPtrC Profile = aOplAPI.PopString();

	CProfileAPI* myProfile = CProfileAPI::NewLC(0);

	TUint8 a = 0;
	TUint8 b = 0;
	TPtrC16 profile(Profile);

	CProfileAPI::TProErrorCode err;
	err = myProfile->SetProfileNameL(profile, a , b);

	CleanupStack::PopAndDestroy(1); // myprofile

	aOplAPI.Push(TInt16(err));
	}


void CCellTrackOPX::ProfileCount(OplAPI& aOplAPI) const
	{

	CProfileAPI* myProfile = CProfileAPI::NewLC(0);

	TInt c = myProfile->GetProfileCountL();

	CleanupStack::PopAndDestroy(1); // myprofile

	aOplAPI.Push(TInt16(c));
	}

void CCellTrackOPX::ActiveProfile(OplAPI& aOplAPI) const
	{
	int i=0;
#if defined(__WINS__)
	User::Leave(KOplErrNotSupported);
#else
	int* a = &i;
	CProfileDb* myProfile = CProfileDb::NewLC();
	myProfile->GetActiveProfileL(a);
#endif
	CleanupStack::PopAndDestroy(1); // myprofile

	aOplAPI.Push(TInt16(i));
	}

void CCellTrackOPX::ActiveProfileName(OplAPI& aOplAPI) const
	{
	int i;
	int* a = &i;

	_LIT(KText,"               ");
	TBufC<15> buf1(KText);
	TPtr16 c = buf1.Des();

	CProfileAPI* myProfile = CProfileAPI::NewLC(0);

	myProfile->GetProfileActiveNameL(c,a);

	CleanupStack::PopAndDestroy(1); // myprofile

	aOplAPI.PushL(c);

	}

void CCellTrackOPX::ExtraProfileName(OplAPI& aOplAPI) const
	{
	TProfileExtra choice = (TProfileExtra) aOplAPI.PopInt16();

	_LIT(KText,"               ");
	TBufC<15> buf1(KText);
	TPtr16 c = buf1.Des();

	CProfileAPI* myProfile = CProfileAPI::NewLC(0);

	if (choice == THead)
	{ myProfile->GetHDSDefProfileL(c); }
	else if (choice == TCar)
	{ myProfile->GetCarDefProfileL(c); }

	CleanupStack::PopAndDestroy(1); // myprofile

	aOplAPI.PushL(c);
	}

void CCellTrackOPX::ProfileNameLC(OplAPI& aOplAPI) const
	{
	TInt index = aOplAPI.PopInt16();

	CProfileAPI* myProfile = CProfileAPI::NewLC(0);

	CDesC16ArrayFlat* b = new (ELeave) CDesC16ArrayFlat(5);

	myProfile->GetProfileNameListL(b,(CProfileDb::TDbSortType) 0);
	CleanupStack::PopAndDestroy(1); // myprofile

	TBufC<15> buf1(b->MdcaPoint(index));
	TPtr16 c = buf1.Des();

	b->~CDesC16ArrayFlat();
	delete b;

	aOplAPI.PushL(c);
	}

///////////////////////////// phoneinfo opx
void CCellTrackOPX::CellInfo(OplAPI& aOplAPI) const
	{
	TPhoneInfoQuery aQuery=(TPhoneInfoQuery)(aOplAPI.PopInt16());

	RTelServer server;
	User::LeaveIfError( server.Connect() );
	CleanupClosePushL(server);
	// load a phone profile
	_LIT(KGsmModuleName, "phonetsy.tsy");
	User::LeaveIfError( server.LoadPhoneModule( KGsmModuleName ) );
	// initialize the phone object
	RTelServer::TPhoneInfo info;
	User::LeaveIfError( server.GetPhoneInfo( 0, info ) );
	RBasicGsmPhone phone;
	User::LeaveIfError( phone.Open( server, info.iName ) );
	CleanupClosePushL(phone);

	TInt res = -1;
	if (aQuery != TNetStat)
		{
		MBasicGsmPhoneNetwork::TCurrentNetworkInfo ni;
		User::LeaveIfError( phone.GetCurrentNetworkInfo( ni ) );
		switch (aQuery)
			{
			case TMCC:
				res = ni.iNetworkInfo.iId.iMCC;
				break;
			case TMNC:
				res = ni.iNetworkInfo.iId.iMNC;
				break;
			case TLocalACode:
				res = ni.iLocationAreaCode;
				break;
			case TCellId:
		        res = ni.iCellId;
				break;
			default:
				break;
			}
		}	
	else if (aQuery == TNetStat)
		{
		MBasicGsmPhoneNetwork::TRegistrationStatus ri=MBasicGsmPhoneNetwork::EUnknown;
		// Perhaps GetNetworkRegistrationStatus can leave as well as return error???
		TRAPD( error , User::LeaveIfError( phone.GetNetworkRegistrationStatus( ri )) );
		if (!error)
			res = ri;
		}

	CleanupStack::PopAndDestroy(1); // phone
	server.UnloadPhoneModule( KGsmModuleName );
	CleanupStack::PopAndDestroy(1); // server
    server.Close();

    aOplAPI.Push(TInt32(res));

	}

void CCellTrackOPX::Serial(OplAPI& aOplAPI) const
	{
	TPhoneInfoQuery aQuery=(TPhoneInfoQuery)(aOplAPI.PopInt16());

	RTelServer server;
	CleanupClosePushL(server);
	User::LeaveIfError( server.Connect() );
	// load a phone profile
	_LIT(KGsmModuleName, "phonetsy.tsy");
	User::LeaveIfError( server.LoadPhoneModule( KGsmModuleName ) );
	// initialize the phone object
	RTelServer::TPhoneInfo info;
	User::LeaveIfError( server.GetPhoneInfo( 0, info ) );
	RBasicGsmPhone phone;
	User::LeaveIfError( phone.Open( server, info.iName ) );
	CleanupClosePushL(phone);

	TBuf<50> res;
	MBasicGsmPhoneId::TId id;
	MBasicGsmPhoneNetwork::TCurrentNetworkInfo ni;
	switch (aQuery)
	{
	case TSerialNum:
		User::LeaveIfError(phone.GetGsmPhoneId(id) );
		res = id.iSerialNumber;
		break;
	case TProviderName:
		User::LeaveIfError( phone.GetCurrentNetworkInfo(ni) );
		res = ni.iNetworkInfo.iLongName;
		break;
	default:
		break;
	}

	CleanupStack::PopAndDestroy(1); // phone
	server.UnloadPhoneModule( KGsmModuleName );
	CleanupStack::PopAndDestroy(1); // server
    server.Close();

	aOplAPI.PushL(res);

	}

void CCellTrackOPX::Info(OplAPI& aOplAPI) const
	{
	TPhoneInfoStatusUID choice = (TPhoneInfoStatusUID) aOplAPI.PopInt32();

	TInt res = -1;
	TUid uid(TUid::Null());
	if (choice == Tphoneoffon)
		{ uid.iUid = 0x100052C5; }
	else if (choice == Tnetwork)
		{ uid.iUid = 0x100052C7; }
	else if (choice == Tprofile)
		{ uid.iUid = 0x100052D2; }

	RSystemAgent sa;
	User::LeaveIfError(sa.Connect());
	CleanupClosePushL(sa);
	res = sa.GetState(uid);
	CleanupStack::PopAndDestroy(1); // sa

	aOplAPI.Push(TInt32(res));
	}

void CCellTrackOPX::InfoUid(OplAPI& aOplAPI) const
	{
	TInt32 auid = (TInt32) aOplAPI.PopInt32();

	TInt32 res = -1;
	TUid uid;
	uid.iUid = auid;

	RSystemAgent sa;
	User::LeaveIfError(sa.Connect());
	CleanupClosePushL(sa);
	res = sa.GetState(uid);
	CleanupStack::PopAndDestroy(1); // sa

	aOplAPI.Push(TInt32(res));
	}

///////////////////////// opx stuff /////////////////////////////

CCellTrackOPX::CCellTrackOPX()
	:iThreadArray(4)
	{
	}

CCellTrackOPX::~CCellTrackOPX()
	{
	iThreadArray.ResetAndDestroy();
	}

CTlsDataOPXCellTrack::CTlsDataOPXCellTrack(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

CTlsDataOPXCellTrack* CTlsDataOPXCellTrack::NewL(OplAPI& aOplAPI)
	{
	CTlsDataOPXCellTrack* This=new(ELeave) CTlsDataOPXCellTrack(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	CleanupStack::Pop();
	return This;
	}

void CTlsDataOPXCellTrack::ConstructL()
	{
	iSystemHandle= new(ELeave) CCellTrackOPX;
	}

CTlsDataOPXCellTrack::~CTlsDataOPXCellTrack()
	{
	delete iSystemHandle;
	Dll::FreeTls();
	}

void CTlsDataOPXCellTrack::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	// profile
	case ESwitchProfile:
	   	iSystemHandle->SwitchProfile(iOplAPI);
		break;
	case EProfileName:
	   	iSystemHandle->ProfileNameLC(iOplAPI);
		break;
	case EProfileCount:
	   	iSystemHandle->ProfileCount(iOplAPI);
		break;
	case EActiveProfile:
	   	iSystemHandle->ActiveProfile(iOplAPI);
		break;
	case EActiveProfileName:
	   	iSystemHandle->ActiveProfileName(iOplAPI);
		break;
	case EExtraProfileName:
		iSystemHandle->ExtraProfileName(iOplAPI);
		break;
	// cbs
	case ECBSOnOffStat:
	   	iSystemHandle->CBSOnOffStat(iOplAPI);
		break;
	case ECBSMessageRead:
		iSystemHandle->CBSReadMsg(iOplAPI);
		break;
	case ECBSMessageInfo:
		iSystemHandle->CBSMsgInfo(iOplAPI);
		break;
	case ECBSListsOpenCloseStat:
	   	iSystemHandle->CBSListsOpenCloseStat(iOplAPI);
		break;
	case ECBSListsInit:
	   	iSystemHandle->CBSListsInit(iOplAPI);
		break;
	// phoneinfo
	case ECellInfo:
	   	iSystemHandle->CellInfo(iOplAPI);
		break;
	case ESerial:
		iSystemHandle->Serial(iOplAPI);
		break;
	case EInfo:
		iSystemHandle->Info(iOplAPI);
		break;
	case EInfoUid:
		iSystemHandle->InfoUid(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

//
// OPX loading interface
//
TBool CTlsDataOPXCellTrack::CheckVersion(TInt aVersion)
	// To check whether the opx is a compatible version
	// *** Change as required ***
	{
	if ((aVersion & 0xff00)>(KCellTrackOPXVersion & 0xff00)) // Major version must be <= OPX's version
		return EFalse;	// Bad version
	else
		return ETrue;	// OK
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	CTlsDataOPXCellTrack* tls=((CTlsDataOPXCellTrack*)Dll::Tls());
	if (tls==NULL)
		{
		tls=CTlsDataOPXCellTrack::NewL(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	//
	// DLL entry point
	//
	{
	return(KErrNone);
	}

