// Agenda.cpp - Agenda OPX Source
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include <s32file.h>
#include <coeutils.h>
#include <txtfmlyr.h>
#include <eikenv.h>

#include <agmmodel.h>
#include <agmtodos.h>
#include <oplerr.h>
#include "Agenda.h"

/* ----------------------------------------------------------------------
	Constructor
   ---------------------------------------------------------------------- */
CAgendaOpx::CAgendaOpx(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

/* ----------------------------------------------------------------------
	EPOC standard method
   ---------------------------------------------------------------------- */
CAgendaOpx* CAgendaOpx::NewL(OplAPI& aOplAPI)
	{
	CAgendaOpx* self = new (ELeave) CAgendaOpx(aOplAPI);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/* ----------------------------------------------------------------------
	EPOC standard method - do any construction which may leave
   ---------------------------------------------------------------------- */
void CAgendaOpx::ConstructL()
	{
	iParaFormatLayer = CParaFormatLayer::NewL();
	iCharFormatLayer = CCharFormatLayer::NewL();
	iAgnServ=RAgendaServ::NewL();
	}

/* ----------------------------------------------------------------------
	Destructor
   ---------------------------------------------------------------------- */
CAgendaOpx::~CAgendaOpx()
	{
	delete iParaFormatLayer;
	delete iCharFormatLayer;
	if (iAgnServ)
		{
		iAgnServ->Close();
		delete iAgnServ;
		}
	delete iModel;
	delete iAgnSyncIter;
	Dll::FreeTls();
	}

/* ======================================================================
	OPX API functions start here
   ====================================================================== */

/* ----------------------------------------------------------------------
	Open an existing agenda file
	OPL parameters:
		Name of Agenda file
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::OpenL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	TPtrC16 aFileName = aAPI.PopString();
	// Check if already open & if so silently close old file
	if (aOpx.iModel!=NULL)
		DoCloseL(aOpx);

	// Check file exists
	if (!ConeUtils::FileExists(aFileName))
		User::Leave(KErrNotFound);

	aOpx.iModel = CAgnEntryModel::NewL();
	if (!aOpx.iAgnServ)
		{
		aOpx.iAgnServ=RAgendaServ::NewL();
		}
	User::LeaveIfError(aOpx.iAgnServ->Connect());
	
	aOpx.iModel->SetServer(aOpx.iAgnServ);
	aOpx.iModel->SetMode(CAgnEntryModel::EClient);
	aOpx.iModel->OpenL(aFileName);
	aOpx.iAgnServ->WaitUntilLoaded();

	// Construct object for iterating over agenda entries
	aOpx.iAgnSyncIter = CAgnSyncIter::NewL(aOpx.iAgnServ); 
	aOpx.iAgnSyncIter->First();

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Close the agenda file
	OPL parameters:
		None
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::CloseL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	// ignore if not open
	if (aOpx.iModel!=NULL)
		DoCloseL(aOpx);
	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Write an entry to the file
	OPL parameters:
		Handle of entry
	OPL return value:
		Entry id
   ---------------------------------------------------------------------- */
void CAgendaOpx::AddL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	TAgnEntryId id;
	TRAPD(err, id = aOpx.iModel->AddEntryL(entry));
	// retry if error
	if (err<0)
		id = aOpx.iModel->AddEntryL(entry);

	aAPI.Push((TInt32)id.Value());
	}

/* ----------------------------------------------------------------------
	Update an entry in the file
	OPL parameters:
		Handle of entry
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::ModifyL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	aOpx.iModel->UpdateEntryL(entry);

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Delete an entry in the file
	OPL parameters:
		Handle of entry
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::DeleteL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	aOpx.iModel->DeleteEntryL(entry);

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Fetch an entry in the file
	OPL parameters:
		ID of entry (see Add() and EnGetId())
	OPL return value:
		ID of entry
   ---------------------------------------------------------------------- */
void CAgendaOpx::FetchL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	TAgnId id(aAPI.PopInt32());

	CAgnEntry* entry = aOpx.iModel->FetchEntryL(id);

	aAPI.Push((TInt32)entry);
	}

/* ----------------------------------------------------------------------
	Return first entry in agenda file - used with NextEntry()
	OPL parameters:
		None
	OPL return value:
		Handle of entry or 0 if no more
   ---------------------------------------------------------------------- */
void CAgendaOpx::FirstEntryL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	aOpx.iAgnSyncIter->First();
	if (aOpx.iAgnSyncIter->Available())
		{
		TAgnEntryId id=aOpx.iAgnSyncIter->EntryId();
		CAgnEntry* entry = aOpx.iModel->FetchEntryL(id);
		aAPI.Push(CastEntryL(entry));
		}
	else
		aAPI.Push((TInt32)0);
	}

/* ----------------------------------------------------------------------
	Get	next entry in Agenda file - see also GetFirstEntry()
	OPL parameters:
		None
	OPL return value:
		Handle of entry or 0 if no more
   ---------------------------------------------------------------------- */
void CAgendaOpx::NextEntryL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	TAgnEntryId id;
	CAgnEntry* entry=NULL;

	do
		{
		if (entry)
			delete entry;
		aOpx.iAgnSyncIter->Next();
		if (aOpx.iAgnSyncIter->Available())
			{
			id=aOpx.iAgnSyncIter->EntryId();
			entry = aOpx.iModel->FetchEntryL(id);
			}
		else
			{
			aAPI.Push((TInt32)0);
			return;
			}
		}while (entry->ReplicationData().HasBeenDeleted());

	aAPI.Push(CastEntryL(entry));
	}

/* ----------------------------------------------------------------------
	Create a new appointment
	OPL parameters:
		None
	OPL return value:
		Handle to appointment
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnNewApptL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnAppt* appt = CAgnAppt::NewL(aOpx.iParaFormatLayer,aOpx.iCharFormatLayer);
	// default to today
	TTime today;
	today.HomeTime();
	appt->SetStartAndEndDateTime(today);

	aAPI.Push((TInt32)appt);
	}
	
/* ----------------------------------------------------------------------
	Create a new todo
	OPL parameters:
		None
	OPL return value:
		Handle to todo
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnNewTodoL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnTodo* todo = CAgnTodo::NewL(aOpx.iParaFormatLayer,aOpx.iCharFormatLayer);

	// default to first todo list
	CArrayFixFlat<TAgnTodoListId>* TodoListIds = aOpx.iModel->TodoListIdsL();

	if (TodoListIds->Count()>0)
		{
		TAgnId id=(*TodoListIds)[0].Value();
		todo->SetTodoListId(TAgnTodoListId(id));
		}

	delete TodoListIds;

	aAPI.Push((TInt32)todo);
	}
	
/* ----------------------------------------------------------------------
	Create a new event
	OPL parameters:
		None
	OPL return value:
		Handle to event
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnNewEventL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnEvent* event = CAgnEvent::NewL(aOpx.iParaFormatLayer,aOpx.iCharFormatLayer);
	// default to today
	TTime today;
	today.HomeTime();
	event->SetStartAndEndDate(today);

	aAPI.Push((TInt32)event);
	}
	
/* ----------------------------------------------------------------------
	Create a new anniversary
	OPL parameters:
		None
	OPL return value:
		Handle to anniversary
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnNewAnnivL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnAnniv* anniv = CAgnAnniv::NewL(aOpx.iParaFormatLayer,aOpx.iCharFormatLayer);
	// default to today
	TTime today;
	today.HomeTime();
	anniv->SetStartAndEndDate(today);

	aAPI.Push((TInt32)anniv);
	}

/* ----------------------------------------------------------------------
	Set the title of an entry
	OPL parameters:
		Handle of entry
		String text
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnSetTextL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TPtrC text = aAPI.PopString();
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	entry->RichTextL()->Reset();
	entry->RichTextL()->InsertL(0, text);

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Set the year symbol of an entry
	OPL parameters:
		Handle of entry
		Symbol
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnSetSymbolL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TPtrC symbol = aAPI.PopString();

	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (symbol.Length())
		{
		entry->SetHasEntrySymbol(ETrue);
		entry->SetEntrySymbol(symbol[0]);
		}
	else
		{
		entry->SetHasEntrySymbol(EFalse);
		}

	aAPI.Push(0.0);
	}	
	
/* ----------------------------------------------------------------------
	Set the alarm of an entry
	OPL parameters:
		Handle of entry
		Days warning
		Hour
		Minute
		Sound name (may be null string)
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnSetAlarmL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TPtrC sndname	= aAPI.PopString();
	TInt32 minute	= aAPI.PopInt32();
	TInt32 hour 	= aAPI.PopInt32();
	TInt32 days		= aAPI.PopInt32();

	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (sndname.Length()==0 && days==-1 && hour==-1 && minute==-1)
		{
		entry->SetHasAlarm(EFalse);
		}
	else
		{
		if (days<0 || days>31 || hour<0 || hour>24 || minute<0 || minute>60)
			User::Leave(KOplErrInvalidArgs);

		TTimeIntervalDays interDays(days);
		TTimeIntervalMinutes alarmTime((hour * 60) + minute);

		entry->SetAlarm(interDays, alarmTime);
		if (sndname.Length())
			entry->SetAlarmSoundNameL(sndname);
		entry->SetHasAlarm(ETrue);
		}
	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the crossout state of a entry
	NOTE: todo entries have a date associated with the cross-out and
	this date will be set to todays date
	OPL parameters:
		Handle of entry
		Flag
			1=crossed out
			0=not crossed out		
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnSetCrossOutL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt16 flag			= aAPI.PopInt16();
	CAgnEntry* entry	= (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (flag>0)
		{
		if (entry->Type()==CAgnEntry::ETodo)
			{
			TTime today;
			today.HomeTime();
			((CAgnTodo*)entry)->CrossOut(today.DateTime());
			}
		else
			entry->SetIsCrossedOut(ETrue);
		}
	else
		{
		if (entry->Type()==CAgnEntry::ETodo)
			((CAgnTodo*)entry)->UnCrossOut();
		else
			entry->SetIsCrossedOut(EFalse);
		}

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the tentative flag for an entry
	OPL parameters:
		Handle of entry
		flag, 1=set tentative, 0=clear		
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnSetTentativeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt16 flag			= aAPI.PopInt16();
	CAgnEntry* entry	= (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (flag)
		entry->SetIsTentative(ETrue);
	else
		entry->SetIsTentative(EFalse);

	aAPI.Push(0.0);
	}

#if 0	
// FIX ME: this need writing!!!
/* ----------------------------------------------------------------------
	Make an entry repeating
	OPL parameters:
		Handle of entry
		Repeat type
		Repeat interval
		Repeat count (0=forever)
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnSetRepeatL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 count		= aAPI.PopInt32();
	TInt32 interval		= aAPI.PopInt32();
	TInt32 type			= aAPI.PopInt32();
	CAgnEntry* entry	= (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	CAgnRptDef *rptDef = CAgnRptDef::NewL();
	CleanupStack::PushL(rptDef);

	switch (type)
		{
	case EDaily:			// i,e. repeat every x days 
		{
		TAgnDailyRpt& rpt;
		rpt.SetStartDate(date);
		rpt.SetInterval(interval);
		if (count==0)
			rpt.SetRepeatForever(ETrue);
		else
			rpt.();
		rptDef->SetDaily(rpt);
		}
		break;
	case EWeekly:			// i.e.	repeat every x weeks on days Monday and Thursday with Tuesday as the first day of the week
		{
		TAgnWeeklyRpt& rpt;
		rpt.SetStartDate(date);
		rpt.SetInterval(interval);
		rpt.SetRepeatForever(ETrue);
		rptDef->SetWeekly(rpt);
		}
		break;
	case EMonthlyByDates:	// i.e. repeat every x months on the 3rd, 5th and 9th
		{
		TAgnMonthlyByDatesRpt& rpt;
		rpt.SetStartDate(date);
		rpt.SetInterval(1);
		rpt.SetRepeatForever(ETrue);
		rptDef->SetMonthlyByDates(rpt);
		}
		break;
	case EMonthlyByDays:	// i.e. repeat every x months on Tuesday of the 1st week of the month and Wednesday of the 3rd week
		{
		TAgnMonthlyByDaysRpt& rpt;
		rpt.SetStartDate(date);
		rpt.SetInterval(1);
		rpt.SetRepeatForever(ETrue);
		rptDef->SetMonthlyByDays(rpt);
		}
		break;
	case EYearlyByDate:		// i.e. repeat every year on the 3rd of October
		{
		TAgnYearlyByDateRpt rpt;
		rpt.SetStartDate(date);
		rpt.SetInterval(1);
		rpt.SetRepeatForever(ETrue);
		rptDef->SetYearlyByDay(rpt);
		}
	case EYearlyByDay:		// i.e. repeat every year on Wednesday of the 4th week of November
		{
		TAgnYearlyByDayRpt& rpt;
		rpt.SetStartDate(date);
		rpt.SetInterval(1);
		rpt.SetRepeatForever(ETrue);
		rptDef->SetMonthlyByDays(rpt);
		}
	default:
		User::Leave(KOplErrInvalidArgs);
		break;
	}

	entry->SetRptDefL(rptDef);

	CleanupStack::PopAndDestroy();		// CAgnRptDef

	aAPI.Push(0.0);
	}
#endif
	
/* ----------------------------------------------------------------------
	Get the ID of an entry
	OPL parameters:
		Handle of entry
	OPL return value:
		Id of entry
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetIdL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	aAPI.Push((TInt32)entry->EntryId().Value());
	}

/* ----------------------------------------------------------------------
	Get the type of an entry
	OPL parameters:
		Handle of entry
	OPL return value:
		Type of entry:
			0=appt
			1=todo
			2=event
			3=anniv		
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetTypeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	aAPI.Push((TInt16)entry->Type());
	}

/* ----------------------------------------------------------------------
	Get the text for an entry
	OPL parameters:
		Handle of entry
		String text
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetTextL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();
	TBuf<256> text;
	CRichText *rText;
	TInt len;

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	rText = entry->RichTextL();
	len = rText->DocumentLength();
	if (len>255)
		len=255;
	if (rText) rText->Extract(text,0,rText->DocumentLength());

	aAPI.PushL(text);
	}

/* ----------------------------------------------------------------------
	Get the year symbol for an entry
	OPL parameters:
		Handle of entry
	OPL return value:
		Symbol
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetSymbolL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();
	
	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);
	TBuf<1> symbol;
	if (entry->HasEntrySymbol())
		symbol.Append(symbol);
	aAPI.PushL(symbol);
	}
	
/* ----------------------------------------------------------------------
	Get the alarm details of an entry
	OPL parameters:
		Handle of entry
		Days warning
		Hour
		Minute
	OPL return value:
		Sound name
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetAlarmL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* minute=aAPI.PopPtrInt32();
	TAny* hour=aAPI.PopPtrInt32();
	TAny* days=aAPI.PopPtrInt32();

	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (entry->HasAlarm())
		{
		TTimeIntervalDays d=entry->AlarmDaysWarning();
		TTimeIntervalMinutes m=entry->AlarmTime();
		TPtrC name=entry->AlarmSoundName();
		TInt hr, mn;

		hr = m.Int()/60;
		mn = m.Int()-hr*60;

		aAPI.PutLong(minute,mn);
		aAPI.PutLong(hour,hr);
		aAPI.PutLong(days,d.Int());

		aAPI.PushL(name);
		}
	else
		{
		aAPI.PutLong(minute,-1);
		aAPI.PutLong(hour,-1);
		aAPI.PutLong(days,-1);

		aAPI.PushL(KNullDesC);
		}
	}
	
/* ----------------------------------------------------------------------
	Return whether the entry is crossed out
	OPL parameters:
		Handle of entry
	OPL return value:
		1 if crossed out
		0 if not crossed out
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetCrossOutL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (entry->IsCrossedOut())
		aAPI.Push((TInt16)1);
	else
		aAPI.Push((TInt16)0);
	}
	
/* ----------------------------------------------------------------------
	Check whether entry is marked as tentative
	OPL parameters:
		Handle of entry
	OPL return value:
		1 if tentative
		0 if not tentative
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetTentativeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (entry->IsTentative())
		aAPI.Push((TInt16)1);
	else
		aAPI.Push((TInt16)0);
	}
	
/* ----------------------------------------------------------------------
	Check status of an entry
	OPL parameters:
		Handle of entry
	OPL return value:
		0 open
		1 private
		2 restricted
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnGetStatusL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);

	aAPI.Push((TInt16)entry->ReplicationData().Status());
	}
	
/* ----------------------------------------------------------------------
	Free an entry
	i.e. Delete the entry object
	OPL parameters:
		Handle of entry
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EnFreeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnEntry* entry = (CAgnEntry*)aAPI.PopInt32();

	if (entry==NULL)
		User::Leave(KOplErrInvalidArgs);
	
	delete entry;

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Set the start time of an appointment
	OPL parameters:
		Handle of appointment
		Start Year
		Start Month
		Start Day
		Start Hour
		Start Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::ApSetStartTimeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 minute	= aAPI.PopInt32();
	TInt32 hour 	= aAPI.PopInt32();
	TInt32 day		= aAPI.PopInt32();
	TInt32 month	= aAPI.PopInt32();
	TInt32 year		= aAPI.PopInt32();

	CAgnAppt* appt = (CAgnAppt*)aAPI.PopInt32();

	CheckEntryType(appt, CAgnEntry::EAppt);

	if (year<1980 || year>2100 || month<1 || month>12 || day<1 || day>31 || hour<0 || hour>24 || minute<0 || minute>60)
		User::Leave(KOplErrInvalidArgs);

	TDateTime date(year, (TMonth)(month-1), day-1, hour, minute, 0, 0);

	appt->SetStartAndEndDateTime(date);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the end time of an appointment
	OPL parameters:
		Handle of appointment
		End Year
		End Month
		End Day
		End Hour
		End Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::ApSetEndTimeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 minute	= aAPI.PopInt32();
	TInt32 hour 	= aAPI.PopInt32();
	TInt32 day		= aAPI.PopInt32();
	TInt32 month	= aAPI.PopInt32();
	TInt32 year		= aAPI.PopInt32();

	CAgnAppt* appt = (CAgnAppt*)aAPI.PopInt32();

	CheckEntryType(appt, CAgnEntry::EAppt);

	if (year<1980 || year>2100 || month<1 || month>12 || day<1 || day>31 || hour<0 || hour>24 || minute<0 || minute>60)
		User::Leave(KOplErrInvalidArgs);

	TDateTime date(year, (TMonth)(month-1), day-1, hour, minute, 0, 0);

	appt->SetStartAndEndDateTime(appt->StartDateTime(), date);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Get the start time of an appointment
	OPL parameters:
		Handle of appointment
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::ApGetStartTimeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* minute=aAPI.PopPtrInt32();
	TAny* hour=aAPI.PopPtrInt32();
	TAny* day=aAPI.PopPtrInt32();
	TAny* month=aAPI.PopPtrInt32();
	TAny* year=aAPI.PopPtrInt32();

	CAgnAppt* appt = (CAgnAppt*)aAPI.PopInt32();

	CheckEntryType(appt, CAgnEntry::EAppt);

	TDateTime d=appt->StartDateTime().DateTime();

	aAPI.PutLong(year,d.Year());
	aAPI.PutLong(month,d.Month()+1);
	aAPI.PutLong(day,d.Day()+1);
	aAPI.PutLong(hour,d.Hour());
	aAPI.PutLong(minute,d.Minute());

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Get the end time of an appointment
	OPL parameters:
		Handle of appointment
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::ApGetEndTimeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* minute=aAPI.PopPtrInt32();
	TAny* hour=aAPI.PopPtrInt32();
	TAny* day=aAPI.PopPtrInt32();
	TAny* month=aAPI.PopPtrInt32();
	TAny* year=aAPI.PopPtrInt32();

	CAgnAppt* appt = (CAgnAppt*)aAPI.PopInt32();

	CheckEntryType(appt, CAgnEntry::EAppt);

	TDateTime d=appt->EndDateTime().DateTime();

	aAPI.PutLong(year,d.Year());
	aAPI.PutLong(month,d.Month()+1);
	aAPI.PutLong(day,d.Day()+1);
	aAPI.PutLong(hour,d.Hour());
	aAPI.PutLong(minute,d.Minute());

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Find existing todo in given todo list
	OPL parameters:
		handle of todo list
		index of todo entry, first is 0
	OPL return value:
		handle of todo
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdAtL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	TInt32 index = aAPI.PopInt32();
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (index>=0 && index<list->Count())
		{
		TAgnEntryId todoId = list->At(index);
		CAgnEntry* entry = aOpx.iModel->FetchEntryL(todoId);
		aAPI.Push(CastEntryL(entry));
		}
	else
		aAPI.Push((TInt32)0);
	}


/* ----------------------------------------------------------------------
	Set the list to which a todo belongs
	OPL parameters:
		Handle of todo
		todo list id
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdSetListL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 id = aAPI.PopInt32();

	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	todo->SetTodoListId(TAgnTodoListId(id));

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the priority of a todo
	OPL parameters:
		Handle of todo
		todo priority
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdSetPriorityL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 pri = aAPI.PopInt32();

	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	if (pri<1 || pri>9)
		User::Leave(KOplErrInvalidArgs);

	todo->SetPriority(pri);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the due date and time of a todo
	OPL parameters:
		Handle of todo
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdSetDueDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 day		= aAPI.PopInt32();
	TInt32 month	= aAPI.PopInt32();
	TInt32 year		= aAPI.PopInt32();

	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	if (year==0 && month==0 && day==0)
		{
		// undated todo
		todo->SetDueDate(Time::NullTTime());
		}
	else
		{
		if (year<1980 || year>2100 || month<1 || month>12 || day<1 || day>31)
			User::Leave(KOplErrInvalidArgs);

		todo->SetDueDate(TDateTime(year, (TMonth)(month-1), day-1, 0, 0, 0, 0));
		}
	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the duration of a todo
	OPL parameters:
		Handle of todo
		days
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdSetDurationL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 days		= aAPI.PopInt32();

	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	if (days<0)
		User::Leave(KOplErrInvalidArgs);

	todo->SetDuration(TTimeIntervalDays(days));

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Get the list to which a todo belongs
	OPL parameters:
		Handle of todo
	OPL return value:
		todo list id
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdGetListL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	aAPI.Push((TInt32)todo->TodoListId().Value());
	}
	
/* ----------------------------------------------------------------------
	Set the priority of a todo
	OPL parameters:
		Handle of todo
	OPL return value:
		todo priority
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdGetPriorityL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	aAPI.Push((TInt32)todo->Priority());
	}
	
/* ----------------------------------------------------------------------
	Get the due date and time of a todo
	OPL parameters:
		Handle of todo
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdGetDueDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* day=aAPI.PopPtrInt32();
	TAny* month=aAPI.PopPtrInt32();
	TAny* year=aAPI.PopPtrInt32();

	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	if (todo->DueDate()!=Time::NullTTime())
		{
		TDateTime due = todo->DueDate().DateTime();

		aAPI.PutLong(day,due.Day()+1); 
		aAPI.PutLong(month,due.Month()+1); 
		aAPI.PutLong(year,due.Year());
		}
	else
		{
		aAPI.PutLong(day,0); 
		aAPI.PutLong(month,0); 
		aAPI.PutLong(year,0); 
		}

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Get the duration of a todo
	OPL parameters:
		Handle of todo
	OPL return value:
		days
   ---------------------------------------------------------------------- */
void CAgendaOpx::TdGetDurationL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodo* todo = (CAgnTodo*)aAPI.PopInt32();

	CheckEntryType(todo, CAgnEntry::ETodo);

	aAPI.Push((TInt32)todo->Duration().Int());
	}
	

/* ----------------------------------------------------------------------
	Set the start time of an event
	OPL parameters:
		Handle of event
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EvSetStartDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 day		= aAPI.PopInt32();
	TInt32 month	= aAPI.PopInt32();
	TInt32 year		= aAPI.PopInt32();

	CAgnEvent* event = (CAgnEvent*)aAPI.PopInt32();

	CheckEntryType(event, CAgnEntry::EEvent);

	if (year<1980 || year>2100 || month<1 || month>12 || day<1 || day>31)
		User::Leave(KOplErrInvalidArgs);

	TDateTime date(year, (TMonth)(month-1), day-1, 0, 0, 0 ,0);

	event->SetStartAndEndDate(date);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the end time of an event
	OPL parameters:
		Handle of event
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EvSetEndDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 day		= aAPI.PopInt32();
	TInt32 month	= aAPI.PopInt32();
	TInt32 year		= aAPI.PopInt32();

	CAgnEvent* event = (CAgnEvent*)aAPI.PopInt32();

	CheckEntryType(event, CAgnEntry::EEvent);

	if (year<1980 || year>2100 || month<1 || month>12 || day<1 || day>31)
		User::Leave(KOplErrInvalidArgs);

	TDateTime date(year, (TMonth)(month-1), day-1, 0, 0, 0, 0);

	if (AgnDateTime::IsLessThan(date, event->StartDate().DateTime()))
		event->SetStartAndEndDate(date, date);
	else
		event->SetStartAndEndDate(event->StartDate(), date);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Get the start time of an event
	OPL parameters:
		Handle of event
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EvGetStartDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* day=aAPI.PopPtrInt32();
	TAny* month=aAPI.PopPtrInt32();
	TAny* year=aAPI.PopPtrInt32();

	CAgnEvent* event = (CAgnEvent*)aAPI.PopInt32();

	CheckEntryType(event, CAgnEntry::EEvent);

	TDateTime d=event->StartDate().DateTime();

	aAPI.PutLong(year,d.Year());
	aAPI.PutLong(month,d.Month()+1);
	aAPI.PutLong(day,d.Day()+1);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Get the end time of an event
	OPL parameters:
		Handle of event
		Year
		Month
		Day
		Hour
		Minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::EvGetEndDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* day=aAPI.PopPtrInt32();
	TAny* month=aAPI.PopPtrInt32();
	TAny* year=aAPI.PopPtrInt32();

	CAgnEvent* event = (CAgnEvent*)aAPI.PopInt32();

	CheckEntryType(event, CAgnEntry::EEvent);

	TDateTime d=event->EndDate().DateTime();

	aAPI.PutLong(year,d.Year());
	aAPI.PutLong(month,d.Month()+1);
	aAPI.PutLong(day,d.Day()+1);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the date of an anniversary
	OPL parameters:
		Handle of anniversary
		Start year
		Month
		Day
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::AnSetDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 day		= aAPI.PopInt32();
	TInt32 month	= aAPI.PopInt32();
	TInt32 year		= aAPI.PopInt32();

	CAgnAnniv* anniv = (CAgnAnniv*)aAPI.PopInt32();

	CheckEntryType(anniv, CAgnEntry::EAnniv);

	if (year<-30000 || year>2100 || month<1 || month>12 || day<1 || day>31)
		User::Leave(KOplErrInvalidArgs);

	TDateTime date(1980, (TMonth)(month-1), day-1, 0, 0, 0, 0);

	//anniv->SetStartAndEndDate(date);

	CAgnRptDef *rptDef = CAgnRptDef::NewL();
	CleanupStack::PushL(rptDef);

	TAgnYearlyByDateRpt rpt;
	rpt.SetStartDate(date);
	rpt.SetInterval(1);
	rpt.SetRepeatForever(ETrue);

	rptDef->SetYearlyByDate(rpt);

	anniv->SetRptDefL(rptDef);

	CleanupStack::PopAndDestroy();		// CAgnRptDef

	anniv->SetBaseYear(year);
	anniv->SetHasBaseYear(ETrue);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set the show type of an anniversary
	OPL parameters:
		Handle of anniversary
		Type:
			0=none
			1=base year
			2=elapsed years
			3=both
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::AnSetShowL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt16 type	= aAPI.PopInt16();

	CAgnAnniv* anniv = (CAgnAnniv*)aAPI.PopInt32();

	CheckEntryType(anniv, CAgnEntry::EAnniv);

	if (type<0 || type>3)
		User::Leave(KOplErrInvalidArgs);

	anniv->SetDisplayAs((CAgnAnniv::TDisplayAs)type);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Get the date of an anniversary
	OPL parameters:
		Handle of anniversary
		Year
		Month
		Day
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::AnGetDateL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* day=aAPI.PopPtrInt32();
	TAny* month=aAPI.PopPtrInt32();
	TAny* year=aAPI.PopPtrInt32();

	CAgnAnniv* anniv = (CAgnAnniv*)aAPI.PopInt32();

	CheckEntryType(anniv, CAgnEntry::EAnniv);

	TDateTime d=anniv->StartDate().DateTime();

	if (anniv->HasBaseYear())
		aAPI.PutLong(year,anniv->BaseYear().Int());
	else
		aAPI.PutLong(year,d.Year());
	aAPI.PutLong(month,d.Month()+1);
	aAPI.PutLong(day,d.Day()+1);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Get the show type of an anniversary
	OPL parameters:
		Handle of anniversary
	OPL return value:
		Type:
			0=none
			1=base year
			2=elapsed years
			3=both
   ---------------------------------------------------------------------- */
void CAgendaOpx::AnGetShowL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnAnniv* anniv = (CAgnAnniv*)aAPI.PopInt32();

	CheckEntryType(anniv, CAgnEntry::EAnniv);

	aAPI.Push((TInt16)anniv->DisplayAs());
	}
	
/* ----------------------------------------------------------------------
	Add a new todo list to the file
	OPL parameters:
		Handle of list
	OPL return value:
		Entry id
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiAddL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	TAgnEntryId id = aOpx.iModel->AddTodoListL(list);

	aAPI.Push((TInt32)id.Value());
	}

/* ----------------------------------------------------------------------
	Change the position of a todo list
	OPL parameters:
		Old position of todo list
		New position of todo list
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiChangePositionL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	TInt newPos = aAPI.PopInt32();
	TInt oldPos = aAPI.PopInt32();

	if (oldPos<0 || newPos<0 ||
		oldPos>aOpx.iModel->TodoListCount() || newPos>aOpx.iModel->TodoListCount())
		User::Leave(KOplErrInvalidArgs);

	aOpx.iModel->ChangeTodoListOrderL(oldPos, newPos);

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Update a todo list
	OPL parameters:
		Handle of todo list
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiModifyL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	aOpx.iModel->UpdateTodoListL(list);

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Delete a todo list
	OPL parameters:
		Handle of todo list
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiDeleteL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	aOpx.iModel->DeleteTodoListL(list);

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Fetch an agenda todo list by ID
	OPL parameters:
		ID of list
	OPL return value:
		Handle of list
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiFetchL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	TAgnId id(aAPI.PopInt32());

	CAgnTodoList* list = aOpx.iModel->FetchTodoListL(id);

	aAPI.Push((TInt32)list);
	}
	
/* ----------------------------------------------------------------------
	Create a new todo list
	OPL parameters:
		None
	OPL return value:
		Handle to todo list
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiNewL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodoList* list = CAgnTodoList::NewL();

	// default to display crossed out entries
	list->SetDisplayCrossedOutEntries(ETrue);

	aAPI.Push((TInt32)list);
	}
	
/* ----------------------------------------------------------------------
	Find an existing todo list in file by list index number
	OPL parameters:
		index number of todo list, first is 0
	OPL return value:
		Handle to todo list, or 0 if no more
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiAtL(OplAPI& aAPI,CAgendaOpx& aOpx)
	{
	//TAgnId i(aAPI.PopInt32());
	TInt32 i=aAPI.PopInt32();
	TInt32 h=0;

	if (i<0)
		User::Leave(KOplErrInvalidArgs);

	CArrayFixFlat<TAgnTodoListId>* TodoListIds = aOpx.iModel->TodoListIdsL();

	if (i<TodoListIds->Count())
		{
		TAgnId id=(*TodoListIds)[i].Value();
		h=(TInt32)aOpx.iModel->FetchTodoListL(id);
		}
	delete TodoListIds;
	aAPI.Push((TInt32)h);
	}

/* ----------------------------------------------------------------------
	Set the name of a todo list
	OPL parameters:
		Handle of todo list
		String text
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiSetTitleL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TPtrC name = aAPI.PopString();
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	// FIX ME: should really check for duplicate todo list name here

	if (list==NULL || name.Length()==0)
		User::Leave(KOplErrInvalidArgs);

	list->SetName(name);

	aAPI.Push(0.0);
	}

/* ----------------------------------------------------------------------
	Set the list order
	OPL parameters:
		Handle of todo
		order:
			0=manual
			1=by date
			2=by priority
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiSetOrderL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt16 order		= aAPI.PopInt16();

	CAgnTodoList* list	= (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL || order<0 || order>2)
		User::Leave(KOplErrInvalidArgs);

	list->SetSortOrder((CAgnTodoList::TSortOrder)order);

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Set whether to display todo's in other views (with time)
	OPL parameters:
		Handle of todo list
		Whether to display, 1=true, 0=false
		Display hour
		Display minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiSetViewDisplayL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TInt32 minute	= aAPI.PopInt32();
	TInt32 hour		= aAPI.PopInt32();
	TInt16 display	= aAPI.PopInt16();

	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL || hour<0 || hour>24 || minute<0 || minute>60)
		User::Leave(KOplErrInvalidArgs);

	if (display)
		{
		list->SetDisplayEntriesInOtherViews(ETrue);
		list->SetDefaultDisplayTimeInOtherViews(hour*60+minute);
		}
	else
		{
		list->SetDisplayEntriesInOtherViews(EFalse);
		}

	aAPI.Push(0.0);
	}
	
/* ----------------------------------------------------------------------
	Returns the todo list id
	OPL parameters:
		Handle of todo list
	OPL return value:
		Id
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiGetIdL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	aAPI.Push((TInt32)list->Id().Value());
	}
	
/* ----------------------------------------------------------------------
	Get the title of a todo list
	OPL parameters:
		Handle of todo
	OPL return value:
		name of todo list
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiGetTitleL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	aAPI.PushL(list->Name());
	}

/* ----------------------------------------------------------------------
	Get the order type of a todo list
	OPL parameters:
		Handle of todo list
	OPL return value:
		order:
			0=manual
			1=by date
			2=by priority		
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiGetOrderL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	aAPI.Push((TInt16)list->SortOrder());
	}
	
/* ----------------------------------------------------------------------
	Get whether displays todo's in other views (with time)
	OPL parameters:
		Handle of todo list
		Display hour
		Display minute
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiGetViewDisplayL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	TAny* minute=aAPI.PopPtrInt32();
	TAny* hour=aAPI.PopPtrInt32();

	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	if (list->DisplayEntriesInOtherViews())
		{
		TInt m=list->DefaultDisplayTimeInOtherViews().Int();
		TInt h=m/60;
		aAPI.PutLong(hour,h);
		aAPI.PutLong(minute,m-h*60);
		aAPI.Push((TInt16)1);
		}
	else
		{
		aAPI.Push((TInt16)0);
		}
	}
	
/* ----------------------------------------------------------------------
	Free a todo list
	OPL parameters:
		Handle of todo list
	OPL return value:
		None
   ---------------------------------------------------------------------- */
void CAgendaOpx::LiFreeL(OplAPI& aAPI,CAgendaOpx& /*aOpx*/)
	{
	CAgnTodoList* list = (CAgnTodoList*)aAPI.PopInt32();

	if (list==NULL)
		User::Leave(KOplErrInvalidArgs);

	delete list;

	aAPI.Push(0.0);
	}

/* ======================================================================
	Internal utility methods start here
   ====================================================================== */

/* ----------------------------------------------------------------------
	Close down agenda file - see OpenL() & CloseL() methods
   ---------------------------------------------------------------------- */
void CAgendaOpx::DoCloseL(CAgendaOpx& aOpx)
	{
	delete aOpx.iModel;
	aOpx.iModel = NULL;				// use as flag to indicate closed

	if (aOpx.iAgnServ)
		{
		aOpx.iAgnServ->Close();
		delete aOpx.iAgnServ;
		aOpx.iAgnServ=NULL;
		}

	delete aOpx.iAgnSyncIter;
	aOpx.iAgnSyncIter=NULL;
	}

/* ----------------------------------------------------------------------
	Convert an entry to the appropriate entry subclass
   ---------------------------------------------------------------------- */
TInt32 CAgendaOpx::CastEntryL(CAgnEntry *entry)
	{
	switch (entry->Type())
		{
	case CAgnEntry::EAppt:
		return (TInt32)entry->CastToAppt();
	case CAgnEntry::ETodo:
		return (TInt32)entry->CastToTodo();
	case CAgnEntry::EEvent:
		return (TInt32)entry->CastToEvent();
	case CAgnEntry::EAnniv:
		return (TInt32)entry->CastToAnniv();
	default:
		User::Leave(KOplErrRecord);
		}
	return 0; // to keep compiler happy
	}

/* ----------------------------------------------------------------------
	Check type of an entry matches expected type
   ---------------------------------------------------------------------- */
void CAgendaOpx::CheckEntryType(CAgnEntry *entry, CAgnEntry::TType type)
	{
	if (entry==NULL || entry->Type()!=type)
		User::Leave(KOplErrInvalidArgs);
	}

/* ======================================================================
	OPX framework methods start here
   ====================================================================== */
void CAgendaOpx::RunL(TInt aProcNum)
	{
	switch (aProcNum)
		{
	case EAgnOpen:
		OpenL(iOplAPI,*this);
		break;
	case EAgnClose:
		CloseL(iOplAPI,*this);
		break;
	case EAgnAdd:
		AddL(iOplAPI,*this);
		break;
	case EAgnModify:
		ModifyL(iOplAPI,*this);
		break;
	case EAgnDelete:
		DeleteL(iOplAPI,*this);
		break;
	case EAgnFetch:
		FetchL(iOplAPI,*this);
		break;
	case EAgnFirstEntry:
		FirstEntryL(iOplAPI,*this);
		break;
	case EAgnNextEntry:
		NextEntryL(iOplAPI,*this);
		break;
	case EAgnEnNewAppt:
		EnNewApptL(iOplAPI,*this);
		break;
	case EAgnEnNewTodo:
		EnNewTodoL(iOplAPI,*this);
		break;
	case EAgnEnNewEvent:
		EnNewEventL(iOplAPI,*this);
		break;
	case EAgnEnNewAnniv:
		EnNewAnnivL(iOplAPI,*this);
		break;
	case EAgnEnSetText:
		EnSetTextL(iOplAPI,*this);
		break;
	case EAgnEnSetSymbol:
		EnSetSymbolL(iOplAPI,*this);
		break;
	case EAgnEnSetAlarm:
		EnSetAlarmL(iOplAPI,*this);
		break;
	case EAgnEnSetCrossOut:
		EnSetCrossOutL(iOplAPI,*this);
		break;
	case EAgnEnSetTentative:
		EnSetTentativeL(iOplAPI,*this);
		break;
	case EAgnEnGetId:
		EnGetIdL(iOplAPI,*this);
		break;
	case EAgnEnGetType:
		EnGetTypeL(iOplAPI,*this);
		break;
	case EAgnEnGetText:
		EnGetTextL(iOplAPI,*this);
		break;
	case EAgnEnGetSymbol:
		EnGetSymbolL(iOplAPI,*this);
		break;
	case EAgnEnGetAlarm:
		EnGetAlarmL(iOplAPI,*this);
		break;
	case EAgnEnGetCrossOut:
		EnGetCrossOutL(iOplAPI,*this);
		break;
	case EAgnEnGetTentative:
		EnGetTentativeL(iOplAPI,*this);
		break;
	case EAgnEnFree:
		EnFreeL(iOplAPI,*this);
		break;
	case EAgnApSetStartTime:
		ApSetStartTimeL(iOplAPI,*this);
		break;
	case EAgnApSetEndTime:
		ApSetEndTimeL(iOplAPI,*this);
		break;
	case EAgnApGetStartTime:
		ApGetStartTimeL(iOplAPI,*this);
		break;
	case EAgnApGetEndTime:
		ApGetEndTimeL(iOplAPI,*this);
		break;
	case EAgnTdAt:
		TdAtL(iOplAPI,*this);
		break;
	case EAgnTdSetList:
		TdSetListL(iOplAPI,*this);
		break;
	case EAgnTdSetPriority:
		TdSetPriorityL(iOplAPI,*this);
		break;
	case EAgnTdSetDueDate:
		TdSetDueDateL(iOplAPI,*this);
		break;
	case EAgnTdSetDuration:
		TdSetDurationL(iOplAPI,*this);
		break;
	case EAgnTdGetList:
		TdGetListL(iOplAPI,*this);
		break;
	case EAgnTdGetPriority:
		TdGetPriorityL(iOplAPI,*this);
		break;
	case EAgnTdGetDueDate:
		TdGetDueDateL(iOplAPI,*this);
		break;
	case EAgnTdGetDuration:
		TdGetDurationL(iOplAPI,*this);
		break;
	case EAgnEvSetStartDate:
		EvSetStartDateL(iOplAPI,*this);
		break;
	case EAgnEvSetEndDate:
		EvSetEndDateL(iOplAPI,*this);
		break;
	case EAgnEvGetStartDate:
		EvGetStartDateL(iOplAPI,*this);
		break;
	case EAgnEvGetEndDate:
		EvGetEndDateL(iOplAPI,*this);
		break;
	case EAgnAnSetDate:
		AnSetDateL(iOplAPI,*this);
		break;
	case EAgnAnSetShow:
		AnSetShowL(iOplAPI,*this);
		break;
	case EAgnAnGetDate:
		AnGetDateL(iOplAPI,*this);
		break;
	case EAgnAnGetShow:
		AnGetShowL(iOplAPI,*this);
		break;
	case EAgnLiAdd:
		LiAddL(iOplAPI,*this);
		break;
	case EAgnLiModify:
		LiModifyL(iOplAPI,*this);
		break;
	case EAgnLiDelete:
		LiDeleteL(iOplAPI,*this);
		break;
	case EAgnLiFetch:
		LiFetchL(iOplAPI,*this);
		break;
	case EAgnLiNew:
		LiNewL(iOplAPI,*this);
		break;
	case EAgnLiAt:
		LiAtL(iOplAPI,*this);
		break;
	case EAgnLiSetTitle:
		LiSetTitleL(iOplAPI,*this);
		break;
	case EAgnLiSetOrder:
		LiSetOrderL(iOplAPI,*this);
		break;
	case EAgnLiSetViewDisplay:
		LiSetViewDisplayL(iOplAPI,*this);
		break;
	case EAgnLiGetId:
		LiGetIdL(iOplAPI,*this);
		break;
	case EAgnLiGetTitle:
		LiGetTitleL(iOplAPI,*this);
		break;
	case EAgnLiGetOrder:
		LiGetOrderL(iOplAPI,*this);
		break;
	case EAgnLiGetViewDisplay:
		LiGetViewDisplayL(iOplAPI,*this);
		break;
	case EAgnLiFree:
		LiFreeL(iOplAPI,*this);
		break;
	case EAgnLiChangePosition:
		LiChangePositionL(iOplAPI,*this);
		break;
	case EAgnEnGetStatus:
		EnGetStatusL(iOplAPI,*this);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	};

TBool CAgendaOpx::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KAgendaVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	{
	CAgendaOpx* tls=((CAgendaOpx*)Dll::Tls());
	if (tls==NULL)
		{
		tls=CAgendaOpx::NewL(aOplAPI);
		Dll::SetTls(tls);
		}
	return (COpxBase *)tls;
	}

EXPORT_C TUint Version()
	{
	return KAgendaVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return KErrNone;
	}