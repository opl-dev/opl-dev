// LOCALE.H
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#ifndef __LOCALE_H__
#define __LOCALE_H__

#include <oplapi.h>
#include <opxapi.h>
#include <eikenv.h>
#include <oplerr.h>
#include <BAUtils.h>

#define KOpxVersion 0x600

const TInt KUidOpxLocale=0x100052B0;

class CTlsDataOpxLocale;

//
// Procedures for providing locale information
//
class COpxLocale :public CBase
	{
public:
	COpxLocale();
	~COpxLocale();
	//
	// Generic get/set functions by value type
	//
	void TInt16LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const;
	void SetTInt16LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const;
	void TInt32LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const;
	void SetTInt32LocaleFunction(OplAPI& aOplAPI,const TInt aProcNum) const;
	void SetLocalePositionFunction(OplAPI& aOplAPI,const TInt aProcNum) const;
	void SeparatorCharacter(OplAPI& aOplAPI,const TInt aProcNum) const;
	void SetSeparatorCharacter(OplAPI& aOplAPI,const TInt aProcNum) const;
	//
	// Specific functions begin here
	//
	void NearestLanguageFile(OplAPI& aOplAPI) const;
	void HomeCountry(OplAPI& aOplAPI) const;

	void CurrencySymbol(OplAPI& aOplAPI) const;
	void CurrencySymbolSet(OplAPI& aOplAPI) const;

	void Units(OplAPI& aOplAPI) const;
	void SetUnits(OplAPI& aOplAPI) const;

	void DaylightSaving(OplAPI& aOplAPI) const;

	void SetDateFormat(OplAPI& aOplAPI) const;
	void DateSeparator(OplAPI& aOplAPI) const;
	void SetTimeFormat(OplAPI& aOplAPI) const;
	void TimeSeparator(OplAPI& aOplAPI) const;

	void SetClockFormat(OplAPI& aOplAPI) const;

	void DayNameFull(OplAPI& aOplAPI) const;
	void MonthNameFull(OplAPI& aOplAPI) const;

	void SetStartOfWeek(OplAPI& aOplAPI) const;
	void Workday(OplAPI& aOplAPI) const;
	};

class CTlsDataOpxLocale : public COpxBase 
	{
public:
	static CTlsDataOpxLocale* NewL(OplAPI& aOplAPI);
	void ConstructL();
	CTlsDataOpxLocale(OplAPI& aOplAPI);
	~CTlsDataOpxLocale();
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
	COpxLocale* iLocaleHandle;
	};

// The language extension procedures (global)
enum TExtensions
		{
		ENearestLanguageFile = 1,
		ELanguage,

		ECountryCode,
		EHomeCountry,

		ECurrencySymbol,
		ECurrencySetSymbol,
		ECurrencySymbolPosition,
		ECurrencySetSymbolPosition,
		ECurrencyDecimalPlaces,
		ECurrencySetDecimalPlaces,
		ECurrencyNegativeInBrackets,
		ECurrencySetNegativeInBrackets,
		ECurrencySpaceBetween,
		ECurrencySetSpaceBetween,
		ECurrencyTriadsAllowed,
		ECurrencySetTriadsAllowed,

		EUnits,
		EUnitsSet,

		EUTCOffset,
		EDaylightSaving,
		
		EDecimalSeparator,
		EDecimalSeparatorSet,
		EThousandsSeparator,
		EThousandsSeparatorSet,
		EDateFormat,
		EDateFormatSet,
		EDateSeparator,
		ETimeFormat,
		ETimeFormatSet,
		ETimeSeparator,
		EAmPmSymbolPosition,
		EAmPmSetSymbolPosition,
		EAmPmSpaceBetween,
		EAmPmSetSpaceBetween,

		EClockFormat,
		ESetClockFormat,

		EDayNameFull,
		EMonthNameFull,

		EStartOfWeek,
		EStartOfWeekSet,
		EWorkday
		};

inline CTlsDataOpxLocale* TheTls() { return((CTlsDataOpxLocale *)Dll::Tls()); }
inline void SetTheTls(CTlsDataOpxLocale *theTls) { Dll::SetTls(theTls); }

#endif // __LOCALE_H__