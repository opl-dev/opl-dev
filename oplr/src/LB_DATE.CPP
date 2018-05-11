// LB_DATE.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//



#include <opcodes.h>
#include "oplutil.h"

void FuncOpCode::Day(CStack& aStack, COplRuntime& , CFrame* )
	{
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime();
	aStack.Push(TInt16(dateTime.Day()+1));
	}


void FuncOpCode::Hour(CStack& aStack, COplRuntime& , CFrame* )
	{
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime();
	aStack.Push(TInt16(dateTime.Hour()));
	}
void FuncOpCode::Minute(CStack& aStack, COplRuntime& , CFrame* )
	{
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime();
	aStack.Push(TInt16(dateTime.Minute()));
	}

void FuncOpCode::Month(CStack& aStack, COplRuntime& , CFrame* )
	{
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime();
	aStack.Push(TInt16(dateTime.Month()+1));
	}
void FuncOpCode::Second(CStack& aStack, COplRuntime& , CFrame* )
	{
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime();
	aStack.Push(TInt16(dateTime.Second()));
	}
void FuncOpCode::Year(CStack& aStack, COplRuntime& , CFrame* )
	{
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime();
	aStack.Push(TInt16(dateTime.Year()));
	}

LOCAL_C TDateTime LocalDateTime(TInt aYear,TMonth aMonth,TInt aDay,TInt aHour,TInt aMinute,TInt aSecond,TInt aMicroSecond)
    {
    TDateTime dateTime;
    if (dateTime.Set(aYear,aMonth,aDay,aHour,aMinute,aSecond,aMicroSecond))
        User::Leave(KOplErrInvalidArgs);
    return dateTime;
    }
void FuncOpCode::Dow(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt16 year=aStack.PopInt16();
	TInt16 month=aStack.PopInt16();
	TInt16 day=aStack.PopInt16();
	TDateTime dateTime=LocalDateTime(year,TMonth(month-1),day-1,0,0,0,0);
	TTime aTime(dateTime);
	aStack.Push(TInt16(aTime.DayNoInWeek()+1));
	}

void FuncOpCode::Week(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt16 year=aStack.PopInt16();
	TInt16 month=aStack.PopInt16();
	TInt16 day=aStack.PopInt16();
	TDateTime dateTime=LocalDateTime(year,TMonth(month-1),day-1,0,0,0,0);
	TTime aTime(dateTime);
	aStack.Push(TInt16(aTime.WeekNoInYear(EFirstFourDayWeek)));
	}

void FuncOpCode::Days(CStack& aStack, COplRuntime& , CFrame* )
	{            // dateTodays
	TInt16 year=aStack.PopInt16();
	TInt16 month=aStack.PopInt16();
	TInt16 day=aStack.PopInt16();
	
	TDateTime dateTime=LocalDateTime(year,TMonth(month-1),day-1,0,0,0,0);
	TTime aTime(dateTime);								  // input date
 	TDateTime offSetDate(1900,TMonth(0),0,0,0,0,0);		 
	TTime offSet(offSetDate);                               // 1/1/1900
	TTimeIntervalDays  days=aTime.DaysFrom(offSet);
	aStack.Push(TInt32(days.Int()));
	}

void OpCode::DaysToDate(CStack& aStack, COplRuntime& aRuntime, CFrame* )
    {
    RHeap& heap=aRuntime.Heap64();
	TInt16* dy=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt16* mo=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt16* yr=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt  days=aStack.PopInt32();	                     	// days from 1/1/1900

	TDateTime offSetDate(1900,TMonth(0),0,0,0,0,0);
	TTime offSet(offSetDate);                               // micros to 1900
	TInt64 totalMicroSecs=(offSet.Int64())+ (TInt64(days)*86400*1000000);
	TTime  retTime(totalMicroSecs);
	TDateTime retDate=retTime.DateTime();

	OplUtil::PutWord(dy,TInt16(retDate.Day()+1));
	OplUtil::PutWord(mo,TInt16(retDate.Month()+1));
	OplUtil::PutWord(yr,TInt16(retDate.Year()));
    }

void FuncOpCode::DateToSecs(CStack& aStack, COplRuntime& , CFrame* )
	{
	TInt16 seconds=aStack.PopInt16();
	TInt16 minutes=aStack.PopInt16();
	TInt16 hours=aStack.PopInt16();
   	TInt16 day=aStack.PopInt16();
	TInt16 month=aStack.PopInt16();
	TInt16 year=aStack.PopInt16();
    TDateTime dateTime=LocalDateTime(year,TMonth(month-1),day-1,hours,minutes,seconds,0);
	TDateTime startDate(1970,TMonth(0),0,0,0,0,0);
	TTime aTime(dateTime);
	TTime sTime(startDate);
    if (aTime<sTime)
        User::Leave(KOplErrInvalidArgs);
	TTimeIntervalMicroSeconds microsFrom=aTime.MicroSecondsFrom(sTime);
	TInt64 int64=(microsFrom.Int64())/1000000;
	if (int64>KMaxTUint32)
		User::Leave(KOplErrInvalidArgs);
	aStack.Push(TInt32(int64.Low()));
	}

void OpCode::SecsToDate(CStack& aStack, COplRuntime& aRuntime, CFrame* )
	{
    RHeap& heap=aRuntime.Heap64();
	TInt16* yrDay=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt16* sc=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt16* mn=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
  	TInt16* hr=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt16* dy=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt16* mo=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TInt16* yr=(TInt16*)OplUtil::PopOffsetAsAddrL(aStack,heap,sizeof(TUint16));
	TUint secs=aStack.PopInt32();	                     	// secs from 1/1/1970

	TDateTime offSetDate(1970,TMonth(0),0,0,0,0,0);
	TTime offSet(offSetDate);								// microsecs since 0000-1970
	TInt64 totalMicroSecs=(TInt64(secs)*1000000)+(offSet.Int64()); // micros since 0000
	TTime  retTime(totalMicroSecs);
	TDateTime retDate=retTime.DateTime();			   // required date

	OplUtil::PutWord(yrDay,TInt16(retTime.DayNoInYear()));
	OplUtil::PutWord(sc,TInt16(retDate.Second()));
	OplUtil::PutWord(mn,TInt16(retDate.Minute()));
	OplUtil::PutWord(hr,TInt16(retDate.Hour()));
	OplUtil::PutWord(dy,TInt16(retDate.Day()+1));
	OplUtil::PutWord(mo,TInt16(retDate.Month()+1));
	OplUtil::PutWord(yr,TInt16(retDate.Year()));
	}


void FuncOpCode::DatimStr(CStack& aStack, COplRuntime& , CFrame* )
	{// system time ->  Fri 16 oct 1992 16:25:30
	TTime now;
	now.HomeTime();
	TBuf<256> datim;
	_LIT(KDatim,"%F%*E %D %*N %Y %H:%T:%S");
	now.FormatL(datim,KDatim);
	aStack.Push(datim);
	}
   

void FuncOpCode::DayNameStr(CStack& aStack, COplRuntime& , CFrame* )
	{// 1-7 -> mon-sun
    TInt day=(aStack.PopInt16()-1);
    if(day<0 || day>6)
        User::Leave(KOplErrInvalidArgs);	
    aStack.Push(TDayNameAbb(TDay(day)));
	}

void FuncOpCode::MonthStr(CStack& aStack, COplRuntime& , CFrame* )
	{// 1-12 -> jan - dec
    TInt month=(aStack.PopInt16()-1);
    if(month<0 || month>11)
        User::Leave(KOplErrInvalidArgs);
    aStack.Push(TMonthNameAbb(TMonth(month)));	
	}

