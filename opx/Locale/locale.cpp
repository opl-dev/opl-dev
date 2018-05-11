// LOCALE.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <apadef.h>
#include "locale.h"
#include <BARSREAD.H>
#include <APGTASK.H>
#include <apgwgnam.h>
#include <T32WLD.H>

COpxLocale::COpxLocale()
	{
	}

COpxLocale::~COpxLocale()
	{
	}

void COpxLocale::TInt16LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const
	{
	TLocale loc;
	TInt16 res=0;
	switch (aProcNum)
		{
	case ECurrencyNegativeInBrackets:
		res=(TInt16)(loc.CurrencyNegativeInBrackets());
		break;
	case ECurrencySpaceBetween:
		res=(TInt16)(loc.CurrencySpaceBetween());
		break;
	case ECurrencyTriadsAllowed:
		res=(TInt16)(loc.CurrencyTriadsAllowed());
		break;
	case EAmPmSpaceBetween:
		res=(TInt16)(loc.AmPmSpaceBetween());
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	aOplAPI.Push(res);
	}

void COpxLocale::SetTInt16LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const
	{
	TLocale loc;
	TInt16 set=aOplAPI.PopInt16();
	switch (aProcNum)
		{
	case ECurrencySetNegativeInBrackets:
		loc.SetCurrencyNegativeInBrackets(set);
		break;
	case ECurrencySetSpaceBetween:
		loc.SetCurrencySpaceBetween(set);
		break;
	case ECurrencySetTriadsAllowed:
		loc.SetCurrencyTriadsAllowed(set);
		break;
	case EAmPmSetSpaceBetween:
		loc.SetAmPmSpaceBetween(set);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::TInt32LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const
	{
	TLocale loc;
	TInt32 res=0;
	switch (aProcNum)
		{
	case ELanguage:
		res=(TInt32)(User::Language());
		break;
	case ECountryCode:
		res=(TInt32)(loc.CountryCode());
		break;
	case ECurrencySymbolPosition:
		res=(TInt32)(loc.CurrencySymbolPosition());
		break;
	case ECurrencyDecimalPlaces:
		res=(TInt32)(loc.CurrencyDecimalPlaces());
		break;
	case EUTCOffset:
		res=(TInt32)(loc.UniversalTimeOffset().Int());
		break;
	case EDateFormat:
		res=(TInt32)(loc.DateFormat());
		break;
	case ETimeFormat:
		res=(TInt32)(loc.TimeFormat());
		break;
	case EAmPmSymbolPosition:
		res=(TInt32)(loc.AmPmSymbolPosition());
		break;
	case EClockFormat:
		res=(TInt32)(loc.ClockFormat());
		break;
	case EStartOfWeek:
		res=(TInt32)(loc.StartOfWeek()+1);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	aOplAPI.Push(res);
	}

void COpxLocale::SetTInt32LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const
	{
	TLocale loc;
	TInt32 set=aOplAPI.PopInt32();
	switch (aProcNum)
		{
	case ECurrencySetDecimalPlaces:
		loc.SetCurrencyDecimalPlaces(set);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	loc.Set();	
	aOplAPI.Push(0.0);
	}

void COpxLocale::SetLocalePositionFunction(OplAPI& aOplAPI, const TInt aProcNum) const
	{
	TLocalePos pos=(TLocalePos)(aOplAPI.PopInt32());
	if (pos!=ELocaleBefore && pos!=ELocaleAfter)
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	switch (aProcNum)
		{
	case ECurrencySetSymbolPosition:
		loc.SetCurrencySymbolPosition(pos);
		break;
	case EAmPmSetSymbolPosition:
		loc.SetAmPmSymbolPosition(pos);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::SeparatorCharacter(OplAPI& aOplAPI, const TInt aProcNum) const
	{
	TLocale loc;
	TChar c=0; // prevents strict compiler warning about it not being initialised
	switch (aProcNum)
		{
	case EDecimalSeparator: 
		c=loc.DecimalSeparator();
		break;
	case EThousandsSeparator: 
		c=loc.ThousandsSeparator();
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	TBuf<1> des;
	des.Append(c);
	aOplAPI.PushL(des);
	}

void COpxLocale::SetSeparatorCharacter(OplAPI& aOplAPI, const TInt aProcNum) const
	{
	TPtrC string=aOplAPI.PopString();
	if (string.Length()!=1)
		User::Leave(KOplErrInvalidArgs);
	TChar c=string[0];
	TLocale loc;
	switch (aProcNum)
		{
	case EDecimalSeparator: 
		loc.SetDecimalSeparator(c);
		break;
	case EThousandsSeparator: 
		loc.SetThousandsSeparator(c);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::NearestLanguageFile(OplAPI& aOplAPI) const
	{
	TFileName ret(aOplAPI.PopString());
	BaflUtils::NearestLanguageFile(aOplAPI.EikonEnv().FsSession(),ret);
	aOplAPI.PushL(ret);
	}

void COpxLocale::HomeCountry(OplAPI& aOplAPI) const
	{
	RWorldServer worldServer;
	TWorldId worldId;
	TCountryData country;
	User::LeaveIfError(worldServer.Connect());
	CleanupClosePushL(worldServer);
	User::LeaveIfError(worldServer.Home(worldId));
	User::LeaveIfError(worldServer.CountryData(country, worldId));
	CleanupStack::PopAndDestroy(); //worldServer
	aOplAPI.PushL(country.iCountry);
	}

void COpxLocale::CurrencySymbol(OplAPI& aOplAPI) const
	{
	TCurrencySymbol symbol;
	symbol.Set();
	aOplAPI.PushL(symbol);
	}

void COpxLocale::CurrencySymbolSet(OplAPI& aOplAPI) const
	{
	User::SetCurrencySymbol(aOplAPI.PopString());
	aOplAPI.Push(0.0);
	}

void COpxLocale::Units(OplAPI& aOplAPI) const
	{
	TLocale loc;
	TAny *pLongDist = aOplAPI.PopPtrInt32();
	TAny *pShortDist = aOplAPI.PopPtrInt32();
	TAny *pGeneral = aOplAPI.PopPtrInt32();
	aOplAPI.PutLong(pGeneral, TInt(loc.UnitsGeneral()));
	aOplAPI.PutLong(pShortDist, TInt(loc.UnitsDistanceShort()));
	aOplAPI.PutLong(pLongDist, TInt(loc.UnitsDistanceLong()));
	aOplAPI.Push(0.0);
	}

void COpxLocale::SetUnits(OplAPI& aOplAPI) const
	{
	TUnitsFormat longDist	= (TUnitsFormat)(aOplAPI.PopInt32());
	TUnitsFormat shortDist	= (TUnitsFormat)(aOplAPI.PopInt32());
	TUnitsFormat general	= (TUnitsFormat)(aOplAPI.PopInt32());
	if ((general!=EUnitsImperial && general!=EUnitsMetric) || 
		(shortDist!=EUnitsImperial && shortDist!=EUnitsMetric) ||
			(longDist!=EUnitsImperial && longDist!=EUnitsMetric))
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	loc.SetUnitsGeneral(general);
	loc.SetUnitsDistanceShort(shortDist);
	loc.SetUnitsDistanceLong(longDist);
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::DaylightSaving(OplAPI& aOplAPI) const
	{
	TInt where = aOplAPI.PopInt32();
	TLocale loc;
	TInt32 homeDst = loc.QueryHomeHasDaylightSavingOn();
	TInt32 allDst = loc.DaylightSaving();
	if (!where)
		aOplAPI.Push(homeDst ? TInt16(-1) : TInt16(0));
	else
		aOplAPI.Push((allDst & where) ? TInt16(-1) : TInt16(0));
	}

void COpxLocale::SetDateFormat(OplAPI& aOplAPI) const
	{
	TDateFormat format = (TDateFormat)(aOplAPI.PopInt32());
	if (format!=EDateAmerican && format!=EDateEuropean && format!=EDateJapanese)
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	loc.SetDateFormat(format);
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::DateSeparator(OplAPI& aOplAPI) const 
	{
	TInt index=aOplAPI.PopInt32();
	if (index<0 || index > KMaxDateSeparators)
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	TChar c=loc.DateSeparator(index);
	TBuf<1> des;
	des.Append(c);
	aOplAPI.PushL(des);
	}

void COpxLocale::SetTimeFormat(OplAPI& aOplAPI) const
	{
	TTimeFormat format = (TTimeFormat)(aOplAPI.PopInt32());
	if (format!=ETime12 && format!=ETime24)
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	loc.SetTimeFormat(format);
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::TimeSeparator(OplAPI& aOplAPI) const
	{
	TInt index=aOplAPI.PopInt32();
	if (index<0 || index > KMaxTimeSeparators)
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	TChar c=loc.TimeSeparator(index);
	TBuf<1> des;
	des.Append(c);
	aOplAPI.PushL(des);
	}

void COpxLocale::SetClockFormat(OplAPI& aOplAPI) const
	{
	TClockFormat format=(TClockFormat)(aOplAPI.PopInt32());
	if (format!=EClockAnalog && format!=EClockDigital)
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	loc.SetClockFormat(format);
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::DayNameFull(OplAPI& aOplAPI) const
	{
	TInt day=aOplAPI.PopInt32()-1;
	if (day<0 || day>6)
		User::Leave(KOplErrInvalidArgs);
	aOplAPI.PushL(TDayName(TDay(day)));
	}

void COpxLocale::MonthNameFull(OplAPI& aOplAPI) const
	{
	TInt month=aOplAPI.PopInt32()-1;
	if (month<0 || month>11)
		User::Leave(KOplErrInvalidArgs);
	aOplAPI.PushL(TMonthName(TMonth(month)));
	}

void COpxLocale::SetStartOfWeek(OplAPI& aOplAPI) const
	{
	TInt day=aOplAPI.PopInt32()-1;
	if (day<0 || day>6)
		User::Leave(KOplErrInvalidArgs);
	TLocale loc;
	loc.SetStartOfWeek(TDay(day));
	loc.Set();
	aOplAPI.Push(0.0);
	}

void COpxLocale::Workday(OplAPI& aOplAPI) const
	{
	TLocale loc;
	TInt dayNo = aOplAPI.PopInt32();
	if (dayNo<0 || dayNo>6)
		User::Leave(KOplErrInvalidArgs);
	TInt workdays = loc.WorkDays();
	TInt flag = workdays & (1 << dayNo);
	aOplAPI.Push(TInt16(flag ? -1 : 0));
	}

CTlsDataOpxLocale::CTlsDataOpxLocale(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

CTlsDataOpxLocale* CTlsDataOpxLocale::NewL(OplAPI& aOplAPI)
	{
	CTlsDataOpxLocale* This=new(ELeave) CTlsDataOpxLocale(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	CleanupStack::Pop();
	return This;
	}

void CTlsDataOpxLocale::ConstructL()
	{
	iLocaleHandle=new(ELeave) COpxLocale;
	}

CTlsDataOpxLocale::~CTlsDataOpxLocale()
	{
	delete iLocaleHandle;
	Dll::FreeTls();
	}

void CTlsDataOpxLocale::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case ECurrencyNegativeInBrackets:
	case ECurrencySpaceBetween:
	case ECurrencyTriadsAllowed:
	case EAmPmSpaceBetween:
		iLocaleHandle->TInt16LocaleFunction(iOplAPI,aProcNum);
		break;
	case ECurrencySetNegativeInBrackets:
	case ECurrencySetSpaceBetween:
	case ECurrencySetTriadsAllowed:
	case EAmPmSetSpaceBetween:
		iLocaleHandle->SetTInt16LocaleFunction(iOplAPI,aProcNum);
		break;
	case ELanguage:
	case ECountryCode:
	case ECurrencySymbolPosition:
	case ECurrencyDecimalPlaces:
	case EUTCOffset:
	case EDateFormat:
	case ETimeFormat:
	case EClockFormat:
	case EAmPmSymbolPosition:
	case EStartOfWeek:
		iLocaleHandle->TInt32LocaleFunction(iOplAPI,aProcNum);
		break;
	case ECurrencySetDecimalPlaces:
		iLocaleHandle->SetTInt32LocaleFunction(iOplAPI,aProcNum);
		break;
	case ECurrencySetSymbolPosition:
	case EAmPmSetSymbolPosition:
		iLocaleHandle->SetLocalePositionFunction(iOplAPI,aProcNum);
		break;
	case EDecimalSeparator:
	case EThousandsSeparator:
		iLocaleHandle->SeparatorCharacter(iOplAPI,aProcNum);
		break;
	case EDecimalSeparatorSet:
	case EThousandsSeparatorSet:
		iLocaleHandle->SetSeparatorCharacter(iOplAPI,aProcNum);
		break;
	case ENearestLanguageFile:
		iLocaleHandle->NearestLanguageFile(iOplAPI);
		break;
	case EHomeCountry:
		iLocaleHandle->HomeCountry(iOplAPI);
		break;
	case ECurrencySymbol:
		iLocaleHandle->CurrencySymbol(iOplAPI);
		break;
	case ECurrencySetSymbol:
		iLocaleHandle->CurrencySymbolSet(iOplAPI);
		break;
	case EUnits:
		iLocaleHandle->Units(iOplAPI);
		break;
	case EUnitsSet:
		iLocaleHandle->SetUnits(iOplAPI);
		break;
	case EDaylightSaving:
		iLocaleHandle->DaylightSaving(iOplAPI);
		break;
	case EDateFormatSet:
		iLocaleHandle->SetDateFormat(iOplAPI);
		break;
	case EDateSeparator:
		iLocaleHandle->DateSeparator(iOplAPI);
		break;
	case ETimeFormatSet:
		iLocaleHandle->SetTimeFormat(iOplAPI);
		break;
	case ETimeSeparator:
		iLocaleHandle->TimeSeparator(iOplAPI);
		break;
	case ESetClockFormat:
		iLocaleHandle->SetClockFormat(iOplAPI);
		break;
	case EDayNameFull:
		iLocaleHandle->DayNameFull(iOplAPI);
		break;
	case EMonthNameFull:
		iLocaleHandle->MonthNameFull(iOplAPI);
		break;
	case EStartOfWeekSet:
		iLocaleHandle->SetStartOfWeek(iOplAPI);
		break;
	case EWorkday:
		iLocaleHandle->Workday(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

//
// OPX loading interface
//
TBool CTlsDataOpxLocale::CheckVersion(TInt aVersion)
	{
	if ((aVersion & 0xFF00) > (KOpxVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	CTlsDataOpxLocale* tls=((CTlsDataOpxLocale*)Dll::Tls());
	if (tls==NULL)
		{
		tls=CTlsDataOpxLocale::NewL(aOplAPI);
		Dll::SetTls(tls);
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