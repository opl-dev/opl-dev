// Agenda.h - Agenda OPX Header
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#ifndef __AGENDAOPX_H__
#define __AGENDAOPX_H__

#ifndef __E32BASE_H__
#include <e32base.h>
#endif

#ifndef __OPLAPI_H__
#include <oplapi.h>
#endif

#ifndef __OPX_H__
#include <opx.h>
#endif

#include <f32file.h>
#include <oplerr.h>   
#include <agmmiter.h>

#define KAgendaVersion 0x600

const TUid KUidAgnModel		= {0x100000F1};
const TInt KUidOpxAgenda	= 0x10000547;

class CAgnEntryModel;

//
// The root class of the OPX
//
class CAgendaOpx : public COpxBase 
	{
public:
	static CAgendaOpx* NewL(OplAPI& aOplAPI);
	void ConstructL();
	CAgendaOpx(OplAPI& aOplAPI);
	~CAgendaOpx() ;
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);

	// Following functions map to the OPL API
	static void OpenL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void CloseL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void AddL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void ModifyL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void DeleteL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void FetchL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void FirstEntryL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void NextEntryL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void EnNewApptL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnNewTodoL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnNewEventL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnNewAnnivL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void EnSetTextL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnSetSymbolL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnSetAlarmL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnSetCrossOutL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnSetTentativeL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void EnGetIdL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnGetTypeL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnGetTextL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnGetSymbolL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnGetAlarmL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnGetCrossOutL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnGetTentativeL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EnGetStatusL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void EnFreeL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void ApSetStartTimeL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void ApSetEndTimeL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void ApGetStartTimeL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void ApGetEndTimeL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void TdAtL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdSetListL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdSetPriorityL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdSetDueDateL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdSetDurationL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdGetListL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdGetPriorityL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdGetDueDateL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void TdGetDurationL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void EvSetStartDateL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EvSetEndDateL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EvGetStartDateL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void EvGetEndDateL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void AnSetDateL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void AnSetShowL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void AnGetDateL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void AnGetShowL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void LiAddL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiChangePositionL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiModifyL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiDeleteL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiFetchL(OplAPI& aAPI, CAgendaOpx& aOpx);

	static void LiNewL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiAtL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiSetTitleL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiSetOrderL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiSetViewDisplayL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiGetIdL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiGetTitleL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiGetOrderL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiGetViewDisplayL(OplAPI& aAPI, CAgendaOpx& aOpx);
	static void LiFreeL(OplAPI& aAPI, CAgendaOpx& aOpx);
private:
	static void DoCloseL(CAgendaOpx& aOpx);
	static TInt32 CastEntryL(CAgnEntry *entry);
	static void CheckEntryType(CAgnEntry *entry, CAgnEntry::TType type);
	enum TExtensions
		{
		EAgnOpen=1,
		EAgnClose,

		EAgnAdd,
		EAgnModify,
		EAgnDelete,
		EAgnFetch,

		EAgnFirstEntry,
		EAgnNextEntry,

		EAgnEnNewAppt,
		EAgnEnNewTodo,
		EAgnEnNewEvent,
		EAgnEnNewAnniv,
		EAgnEnSetText,
		EAgnEnSetSymbol,
		EAgnEnSetAlarm,
		EAgnEnSetCrossOut,
		EAgnEnSetTentative,
		EAgnEnGetId,
		EAgnEnGetType,
		EAgnEnGetText,
		EAgnEnGetSymbol,
		EAgnEnGetAlarm,
		EAgnEnGetCrossOut,
		EAgnEnGetTentative,
		EAgnEnFree,

		EAgnApSetStartTime,
		EAgnApSetEndTime,
		EAgnApGetStartTime,
		EAgnApGetEndTime,

		EAgnTdAt,
		EAgnTdSetList,
		EAgnTdSetPriority,
		EAgnTdSetDueDate,
		EAgnTdSetDuration,
		EAgnTdGetList,
		EAgnTdGetPriority,
		EAgnTdGetDueDate,
		EAgnTdGetDuration,

		EAgnEvSetStartDate,
		EAgnEvSetEndDate,
		EAgnEvGetStartDate,
		EAgnEvGetEndDate,

		EAgnAnSetDate,
		EAgnAnSetShow,
		EAgnAnGetDate,
		EAgnAnGetShow,

		EAgnLiAdd,
		EAgnLiModify,
		EAgnLiDelete,
		EAgnLiFetch,

		EAgnLiNew,
		EAgnLiAt,
		EAgnLiSetTitle,
		EAgnLiSetOrder,
		EAgnLiSetViewDisplay,
		EAgnLiGetId,
		EAgnLiGetTitle,
		EAgnLiGetOrder,
		EAgnLiGetViewDisplay,
		EAgnLiFree,
		EAgnLiChangePosition,
		EAgnEnGetStatus,
		};

	CAgnEntryModel*		iModel;				// for handling agenda editing
	CAgnSyncIter*		iAgnSyncIter;		// for iterating over agenda entries
	CParaFormatLayer*	iParaFormatLayer;	// default text formating
	CCharFormatLayer*	iCharFormatLayer;	// default text formating
	RAgendaServ*		iAgnServ;
	};

inline CAgendaOpx* TheTls() { return((CAgendaOpx *)Dll::Tls()); }
inline void SetTheTls(CAgendaOpx *theTls) { Dll::SetTls(theTls); }
  
#endif __AGENDAOPX_H__