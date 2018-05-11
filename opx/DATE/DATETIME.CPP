// DATETIME.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <OPLAPI.H>
#include <F32FILE.H>
#include <OPLDB.H>
#include "datetime.h"

CDateTimeOpx::CDateTimeOpx() : iDTArray(4)
    {
    }


CDateTimeOpx::~CDateTimeOpx() 
    {
    iDTArray.ResetAndDestroy();
    }


void CDateTimeOpx::DTNewDateTime(OplAPI& aOplAPI)
    {
    TInt micro=aOplAPI.PopInt32();
	TInt second=aOplAPI.PopInt32();
	TInt minute=aOplAPI.PopInt32();
	TInt hour=aOplAPI.PopInt32();
   	TInt day=aOplAPI.PopInt32();
	TInt month=aOplAPI.PopInt32();
	TInt year=aOplAPI.PopInt32();

    RangeCheckL(micro,0,999999);
    RangeCheckL(second,0,59);
    RangeCheckL(minute,0,59);
    RangeCheckL(hour,0,23);
    RangeCheckL(month,1,12);
    RangeCheckL(day,1,Time::DaysInMonth(year,TMonth(month-1)));
	
    TDateTime* dtptr = new(ELeave) TDateTime(year,TMonth(month-1), day-1, hour, minute, second, micro);
    TRAPD(err,iDTArray.AppendL(dtptr));
    if (err)
        {
        delete dtptr;
        User::Leave(err);
        }
    aOplAPI.Push(TInt32(dtptr));
    }


void CDateTimeOpx::DTDeleteDateTime(OplAPI& aOplAPI)
    {
    TDateTime* dtptr=(TDateTime *)aOplAPI.PopInt32();
    TInt pos=CheckPointerL(dtptr);
    delete dtptr;
    iDTArray.Delete(pos);
    aOplAPI.Push(TReal64(0.0));

    }


TInt CDateTimeOpx::CheckPointerL(TDateTime* adtptr)
    {
    TInt count=iDTArray.Count();
    TInt ii;
    for(ii=0;ii<count;ii++)
        {
        if (iDTArray[ii]==adtptr)
            return ii;
        }
    User::Leave(KOplErrInvalidArgs);
    return ii;    
    }


void CDateTimeOpx::RangeCheckL(TInt aVal, TInt aMin, TInt aMax)
    {
    if(aVal<aMin || aVal>aMax)
        User::Leave(KOplErrOutOfRange);
    }


void CDateTimeOpx::DTYear(OplAPI& aOplAPI)      // Check pointer
    {	
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    aOplAPI.Push(TInt32(pDate->Year()));
    }


void CDateTimeOpx::DTMonth(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    aOplAPI.Push(TInt32(pDate->Month()+1));
    }


void CDateTimeOpx::DTDay(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    aOplAPI.Push(TInt32(pDate->Day()+1));
    }


void CDateTimeOpx::DTHour(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    aOplAPI.Push(TInt32(pDate->Hour()));
    }


void CDateTimeOpx::DTMinute(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    aOplAPI.Push(TInt32(pDate->Minute()));
    }


void CDateTimeOpx::DTSecond(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    aOplAPI.Push(TInt32(pDate->Second()));
    }


void CDateTimeOpx::DTMicro(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    aOplAPI.Push(TInt32(pDate->MicroSecond()));
    }


void CDateTimeOpx::DTSetYearL(OplAPI& aOplAPI)      // returns error if not possible
    {
    TInt year=aOplAPI.PopInt32();    
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    pDate->SetYear(year);
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTSetMonthL(OplAPI& aOplAPI)       // returns error if not possible
    {
    TInt month=aOplAPI.PopInt32();
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    RangeCheckL(month,1,12);
    pDate->SetMonth(TMonth(month-1));
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTSetDayL(OplAPI& aOplAPI)       // returns error if not possible
    {
    TInt day=aOplAPI.PopInt32();
	TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    TInt year=pDate->Year();
    TMonth month=pDate->Month();
    RangeCheckL(day,1,Time::DaysInMonth(year,month));
    pDate->SetDay(day-1);
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTSetHourL(OplAPI& aOplAPI)       // returns error if not possible
    {
    TInt hour=aOplAPI.PopInt32();    
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    RangeCheckL(hour,0,23);
    pDate->SetHour(hour);
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTSetMinuteL(OplAPI& aOplAPI)       // returns error if not possible
    {
    TInt minute=aOplAPI.PopInt32();
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    RangeCheckL(minute,0,59);
    pDate->SetMinute(minute);
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTSetSecondL(OplAPI& aOplAPI)       // returns error if not possible
    {
    TInt second=aOplAPI.PopInt32();
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    RangeCheckL(second,0,59);
    pDate->SetSecond(second);
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTSetMicroL(OplAPI& aOplAPI)       // returns error if not possible
    {
    TInt micro=aOplAPI.PopInt32();
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    RangeCheckL(micro,0,999999);
    pDate->SetMicroSecond(micro);
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTSetHomeTime(OplAPI& aOplAPI)       // returns error if not possible
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(pDate);
    TTime set(*pDate);
    User::SetHomeTime(set);
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTNow(OplAPI& aOplAPI)
    {
    TDateTime* dtptr = new(ELeave) TDateTime();

    TTime now ;
    now.HomeTime();
    *dtptr=now.DateTime();
    TRAPD(err,iDTArray.AppendL(dtptr));
    if (err)
        {
        delete dtptr;
        User::Leave(err);
        }    
    aOplAPI.Push(TInt32(dtptr));
    }


void CDateTimeOpx::DTDateTimeDiff(OplAPI& aOplAPI)
    {
    TInt32* micro=aOplAPI.PopPtrInt32();
	TInt32* second=aOplAPI.PopPtrInt32();
	TInt32* minute=aOplAPI.PopPtrInt32();
	TInt32* hour=aOplAPI.PopPtrInt32();
   	TInt32* day=aOplAPI.PopPtrInt32();
	TInt32* month=aOplAPI.PopPtrInt32();
	TInt32* year=aOplAPI.PopPtrInt32();
    TDateTime* aEnd=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* TheStart=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(aEnd);
    CheckPointerL(TheStart);

    TDateTime toReturn;     
    TDateTime aStart(*TheStart);
    TTime start(aStart);
    TTime end(*aEnd);
    TTimeIntervalYears years(start.YearsFrom(end));
    aStart.SetYear(aStart.Year()+ years.Int());
    start=aStart;
    TTimeIntervalMonths months(start.MonthsFrom(end));
    aStart.SetMonth(TMonth(aStart.Month()+months.Int()));
    start=aStart;
    TTimeIntervalDays days(start.DaysFrom(end));
    aStart.SetDay(aStart.Day()+days.Int());
    start=aStart;
    TTime diff=(end.Int64()-start.Int64());
    toReturn=diff.DateTime();
    aOplAPI.PutLong(year,years.Int());
    aOplAPI.PutLong(month,months.Int());
    aOplAPI.PutLong(day,days.Int());
    aOplAPI.PutLong(hour,toReturn.Hour());
    aOplAPI.PutLong(minute,toReturn.Minute());
    aOplAPI.PutLong(second,toReturn.Second());
    aOplAPI.PutLong(micro,toReturn.MicroSecond());
    aOplAPI.Push(TReal64(0.0));
    }


void CDateTimeOpx::DTYearsDiff(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* endPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(endPtr);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime end(*endPtr);
    TTimeIntervalYears years(end.YearsFrom(theStart));
    aOplAPI.Push(TInt32(years.Int()));
    }


void CDateTimeOpx::DTMonthsDiff(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* endPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(endPtr);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime end(*endPtr);
    TTimeIntervalMonths months(end.MonthsFrom(theStart));
    aOplAPI.Push(TInt32(months.Int()));
    }


void CDateTimeOpx::DTDaysDiff(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* endPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(endPtr);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime end(*endPtr);
    TTimeIntervalDays days(end.DaysFrom(theStart));
    aOplAPI.Push(TInt32(days.Int()));
    }


void CDateTimeOpx::DTHoursDiff(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* endPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(endPtr);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime end(*endPtr);
    TTimeIntervalHours hours;
    end.HoursFrom(theStart,hours);
    aOplAPI.Push(TInt32(hours.Int()));
    }


void CDateTimeOpx::DTMinutesDiff(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* endPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(endPtr);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime end(*endPtr);
    TTimeIntervalMinutes minutes;
    end.MinutesFrom(theStart,minutes);
    aOplAPI.Push(TInt32(minutes.Int()));
    }


void CDateTimeOpx::DTSecsDiff(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* endPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(endPtr);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime end(*endPtr);
    TTimeIntervalSeconds seconds;
    end.SecondsFrom(theStart,seconds);
    aOplAPI.Push(TInt32(seconds.Int()));
    }


void CDateTimeOpx::DTMicrosDiff(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* endPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(endPtr);
    CheckPointerL(startPtr);

    TTime theStart(*startPtr);
    TTime end(*endPtr);
    TInt64 microSeconds(end.MicroSecondsFrom(theStart).Int64());
    if(microSeconds<KMinTInt32 || microSeconds>KMaxTInt32)
       User::Leave(KOplErrOverflow);                // error if in any mode already
    aOplAPI.Push(TInt32(microSeconds.GetTInt()));
    }


void CDateTimeOpx::DTWeekNoInYear(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TFirstWeekRule rule=(TFirstWeekRule)aOplAPI.PopInt32();
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* Now=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(Now);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime now(*Now);
    switch(rule)
	    {
	    case EFirstWeek:
        case EFirstFourDayWeek:
        case EFirstFullWeek:
     		break;				
        default:
             User::Leave(KOplErrInvalidArgs);                // error if in any mode already
             break;
	         } 

    aOplAPI.Push(TInt32(now.WeekNoInYear(theStart,rule)));
    }


void CDateTimeOpx::DTDayNoInYear (OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    TDateTime* Now=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(Now);
    CheckPointerL(startPtr);
    TTime theStart(*startPtr);
    TTime now(*Now);

    aOplAPI.Push(TInt32(now.DayNoInYear(theStart)));
    }

                
void CDateTimeOpx::DTDayNoInWeek(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(startPtr);
    TTime now(*startPtr);
    aOplAPI.Push(TInt32(now.DayNoInWeek()+1));
    }


void CDateTimeOpx::DTDaysInMonth(OplAPI& aOplAPI)
    {                                                           // Check ptr s
    TDateTime* startPtr=(TDateTime*)aOplAPI.PopInt32();
    CheckPointerL(startPtr);
    TTime now(*startPtr);
    aOplAPI.Push(TInt32(now.DaysInMonth()));
    }

// Filetime setters and getters are located here, and not System,
// because these use an entry in the TDateTime array.
    
void CDateTimeOpx::DTFileTime(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
	CheckPointerL(pDate);
    TPtrC file=aOplAPI.PopString();
    RFs& fs=aOplAPI.DbManager()->FsSession();
    TTime tim(*pDate);
    fs.Modified(file,tim);
    *pDate=tim.DateTime();
    aOplAPI.Push(TReal64(0.0));
    }

void CDateTimeOpx::DTSetFileTime(OplAPI& aOplAPI)
    {
    TDateTime* pDate=(TDateTime*)aOplAPI.PopInt32();
	CheckPointerL(pDate);
    TPtrC file=aOplAPI.PopString();
    RFs& fs=aOplAPI.DbManager()->FsSession();
    TTime tim(*pDate);
    fs.SetModified(file,tim);
    aOplAPI.Push(TReal64(0.0));
    }

void CDateTimeOpx::DTIsLeapYear(OplAPI& aOplAPI)
	{
	TInt year=aOplAPI.PopInt32();
	aOplAPI.Push(TInt32(Time::IsLeapYear(year) ? -1 : 0));
	}


CTlsDataOPXTime::CTlsDataOPXTime(OplAPI& aOplAPI) : COpxBase(aOplAPI)
    {
    }


CTlsDataOPXTime* CTlsDataOPXTime::NewL(OplAPI& aOplAPI)
    {
    CTlsDataOPXTime* This=new(ELeave) CTlsDataOPXTime(aOplAPI);
    CleanupStack::PushL(This);
    This->ConstructL();
    CleanupStack::Pop();
    return This;
    }


void CTlsDataOPXTime::ConstructL()
    {
    iDateHandle= new(ELeave) CDateTimeOpx;
    } 


CTlsDataOPXTime::~CTlsDataOPXTime()
    {
    delete iDateHandle;
	Dll::FreeTls();
    }


//
// OPX loading interface
//

void CTlsDataOPXTime::RunL(TInt aProcNum)
// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case EDTNewDateTime:
		 iDateHandle->DTNewDateTime(iOplAPI);
		break;
	case EDTDeleteDateTime:
		 iDateHandle->DTDeleteDateTime(iOplAPI);
		break;

	case EDTYear:
		 iDateHandle->DTYear(iOplAPI);
		break;
	case EDTMonth:
		 iDateHandle->DTMonth(iOplAPI);
		break;
	case EDTDay:
		 iDateHandle->DTDay(iOplAPI);
		break;
	case EDTHour:
		 iDateHandle->DTHour(iOplAPI);
		break;
	case EDTMinute:
		 iDateHandle->DTMinute(iOplAPI);
		break;
	case EDTSecond:
		 iDateHandle->DTSecond(iOplAPI);
		break;
	case EDTMicro:
		 iDateHandle->DTMicro(iOplAPI);
		break;

	case EDTSetYearL:
		 iDateHandle->DTSetYearL(iOplAPI);
		break;
	case EDTSetMonthL:
		 iDateHandle->DTSetMonthL(iOplAPI);
		break;
	case EDTSetDayL:
		 iDateHandle->DTSetDayL(iOplAPI);
		break;
	case EDTSetHourL:
		 iDateHandle->DTSetHourL(iOplAPI);
		break;
	case EDTSetMinuteL:
		 iDateHandle->DTSetMinuteL(iOplAPI);
		break;
	case EDTSetSecondL:
		 iDateHandle->DTSetSecondL(iOplAPI);
		break;
	case EDTSetMicroL:
		iDateHandle->DTSetMicroL(iOplAPI);
		break;

    case EDTSetHomeTime:
		iDateHandle->DTSetHomeTime(iOplAPI);
		break; 
	case EDTNow: 
		iDateHandle->DTNow(iOplAPI);
		break; 

    case EDTDateTimeDiff:
		iDateHandle->DTDateTimeDiff(iOplAPI);
		break; 
    case EDTYearsDiff:
		iDateHandle->DTYearsDiff(iOplAPI); 
		break; 
    case EDTMonthsDiff: 
		iDateHandle->DTMonthsDiff(iOplAPI);
		break; 
    case EDTDaysDiff:
		iDateHandle->DTDaysDiff(iOplAPI); 
		break; 
    case EDTHoursDiff:
		iDateHandle->DTHoursDiff(iOplAPI);
		break; 
    case EDTMinutesDiff:
		iDateHandle->DTMinutesDiff(iOplAPI); 
		break; 
    case EDTSecsDiff:
		iDateHandle->DTSecsDiff(iOplAPI);
		break; 
    case EDTMicrosDiff: 
		iDateHandle->DTMicrosDiff(iOplAPI); 
		break; 

    case EDTWeekNoInYear:
		iDateHandle->DTWeekNoInYear(iOplAPI);
		break; 
    case EDTDayNoInYear:
		iDateHandle->DTDayNoInYear(iOplAPI); 
		break; 
    case EDTDayNoInWeek:
		iDateHandle->DTDayNoInWeek(iOplAPI);
		break; 
    case EDTDaysInMonth: 
		iDateHandle->DTDaysInMonth(iOplAPI);
		break; 
	case EDTFileTime:
		iDateHandle->DTFileTime(iOplAPI);
		break;
	case EDTSetFileTime:
		iDateHandle->DTSetFileTime(iOplAPI);
		break;
	case EDTIsLeapYear:
		iDateHandle->DTIsLeapYear(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}


TBool CTlsDataOPXTime::CheckVersion(TInt aVersion)
// To check whether the opx is a compatible version
// *** Change as required ***
	{
	if ((aVersion & 0xff00)>(KOpxDateVersion & 0xff00))	// major version must be <= OPX's version
		return EFalse; // bad version
	else
		return ETrue; // ok
	}


EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
// Creates a COpxBase instance as required by the OPL runtime
// This object is to be stored in the OPX's TLS as shown below
	{
	CTlsDataOPXTime* tls=((CTlsDataOPXTime*)Dll::Tls());
	if (tls==NULL)
		{
        tls=CTlsDataOPXTime::NewL(aOplAPI);
	    Dll::SetTls(tls);
        }
    return (COpxBase *)tls;
	}


EXPORT_C TUint Version()
	{
	return KOpxDateVersion;
	}


GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return(KErrNone);
	}

/* endPtr of $Workfile:   datetime.cpp  $ */
