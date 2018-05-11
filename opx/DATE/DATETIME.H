// DATETIME.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#ifndef __DATETIME_H__
#define __DATETIME_H__

#include <e32base.h>
#if !defined(__OPLAPI_H__)
#include <oplapi.h>
#endif
#if !defined(__OPXAPI_H__)
#include <opxapi.h>
#endif
#include <fbs.h>
#include <w32std.h>
#include <oplerr.h>

const TInt KOpxDateVersion=0x600;
class CTlsDataOPXTime;

/// procedures for emulating tdatetime 
class CDateTimeOpx : public CBase
	{
public:
	CDateTimeOpx();
	~CDateTimeOpx();

	TInt CheckPointerL(TDateTime* adtptr);
	void RangeCheckL(TInt aVal, TInt aMin, TInt aMax);

	void DTNewDateTime(OplAPI& aOplAPI);
	void DTDeleteDateTime(OplAPI& aOplAPI);

	void DTYear(OplAPI& aOplAPI);
	void DTMonth(OplAPI& aOplAPI);
	void DTDay(OplAPI& aOplAPI);
	void DTHour(OplAPI& aOplAPI);
	void DTMinute(OplAPI& aOplAPI);
	void DTSecond(OplAPI& aOplAPI);
	void DTMicro(OplAPI& aOplAPI);

	void DTSetYearL(OplAPI& aOplAPI);	   // returns error if not possible
	void DTSetMonthL(OplAPI& aOplAPI);
	void DTSetDayL(OplAPI& aOplAPI);
	void DTSetHourL(OplAPI& aOplAPI);
	void DTSetMinuteL(OplAPI& aOplAPI);
	void DTSetSecondL(OplAPI& aOplAPI);
	void DTSetMicroL(OplAPI& aOplAPI);

	void DTNow(OplAPI& aOplAPI);
	void DTDateTimeDiff(OplAPI& aOplAPI);
	void DTYearsDiff(OplAPI& aOplAPI);
	void DTMonthsDiff(OplAPI& aOplAPI);
	void DTDaysDiff(OplAPI& aOplAPI);
	void DTHoursDiff(OplAPI& aOplAPI);
	void DTMinutesDiff(OplAPI& aOplAPI);
	void DTSecsDiff(OplAPI& aOplAPI);
	void DTMicrosDiff(OplAPI& aOplAPI);

	void DTWeekNoInYear(OplAPI& aOplAPI);
	void DTDayNoInYear(OplAPI& aOplAPI);
	void DTDayNoInWeek(OplAPI& aOplAPI);
	void DTDaysInMonth(OplAPI& aOplAPI);
	void DTSetHomeTime(OplAPI& aOplAPI);
	void DTFileTime(OplAPI& aOplAPI);
	void DTSetFileTime(OplAPI& aOplAPI);
	void DTIsLeapYear(OplAPI& aOplAPI);

	CArrayPtrFlat<TDateTime> iDTArray;
	};

class CTlsDataOPXTime : public COpxBase 
	{
public:
	static CTlsDataOPXTime* NewL(OplAPI& aOplAPI);
	void ConstructL();
	CTlsDataOPXTime(OplAPI& aOplAPI);
	~CTlsDataOPXTime() ;
	virtual void RunL(TInt aProcNum);
	virtual TBool CheckVersion(TInt aVersion);
	CDateTimeOpx* iDateHandle;
private:
	// the language extension procedures
	enum TExtensions
		{
		EDTNewDateTime=1,
		EDTDeleteDateTime,

		EDTYear,
		EDTMonth,
		EDTDay,
		EDTHour,
		EDTMinute,
		EDTSecond,
		EDTMicro,

		EDTSetYearL,
		EDTSetMonthL,
		EDTSetDayL,
		EDTSetHourL,
		EDTSetMinuteL,
		EDTSetSecondL,
		EDTSetMicroL,

		EDTNow,
		EDTDateTimeDiff,
		EDTYearsDiff,
		EDTMonthsDiff,
		EDTDaysDiff,
		EDTHoursDiff,
		EDTMinutesDiff,
		EDTSecsDiff,
		EDTMicrosDiff,

		EDTWeekNoInYear,
		EDTDayNoInYear,
		EDTDayNoInWeek,
		EDTDaysInMonth,
		EDTSetHomeTime,
		EDTFileTime,
		EDTSetFileTime,
		EDTIsLeapYear
		};
	};

inline CTlsDataOPXTime* TheTls() { return((CTlsDataOPXTime *)Dll::Tls()); }
inline void SetTheTls(CTlsDataOPXTime *theTls) { Dll::SetTls(theTls); }

#endif __DATETIME_H__