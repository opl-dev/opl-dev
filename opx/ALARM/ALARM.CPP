// ALARM.CPP - Source file for ALARM.OPX OPL Language Extension
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include "Alarm.h"

////////////////////////////////////////////////////////////
// The language extension procedures provided by this OPX //
////////////////////////////////////////////////////////////
void COpxAlarm::SetClockAlarm() const
//
// AlmSetClockAlarm:(anAlarmNumber&,aDateTime&,aMessage$,aSound$,aRepeat&) : 1
//
	{
#if !defined(__UIQ__)
	TAlarmClockRepeat aRepeat= TAlarmClockRepeat(iOplAPI.PopInt32());
	TAlarmSoundName aSound = iOplAPI.PopString();
	TAlarmMessage aMessage = iOplAPI.PopString();
	TAlarmSoundName tempSound = aSound;
	tempSound.UpperCase();	// Fix for 'Silent' alarms appearing as 'Silent'
							// in TimeW but actually playing as Chimes
	if (tempSound==KAlarmSilentText)
		aSound=KNullDesC;
	// Note: If you specify an external alarm (e.g. D:\System\Alarms\New Sound)
	// then that sound will play fine, but in TimeW the sound is listed
	// as 'Chimes' - long standing problem.
	TDateTime *dtime= (TDateTime *) iOplAPI.PopInt32();
	TInt aAlarmNumber = iOplAPI.PopInt32();
	
	if (aAlarmNumber < KMinimumAlarms || KMaximumAlarms < aAlarmNumber)
		User::Leave(KOplErrOutOfRange);
	
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	TTime time(*dtime);
	almServer.SetClockAlarm(aAlarmNumber,time,aMessage,aSound,aRepeat);
	almServer.Close();
#else
	iOplAPI.PopInt32();
	iOplAPI.PopString();
	iOplAPI.PopString();
	iOplAPI.PopInt32();
	iOplAPI.PopInt32();
#endif
	iOplAPI.Push(0.0);
	}

void COpxAlarm::AlarmState() const
//
// AlmAlarmState&:(anAlarmNumber&) : 2
//
	{
	TInt aAlarmNumber = iOplAPI.PopInt32();
	
	if (aAlarmNumber < KMinimumAlarms || KMaximumAlarms < aAlarmNumber)
		User::Leave(KOplErrOutOfRange);
#if !defined(__UIQ__)

	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	TAlarmSetState state = almServer.AlarmState(aAlarmNumber);
	almServer.Close();
#else // UIQ
	TInt32 state=0;
#endif
	iOplAPI.Push(TInt32(state));
	}

void COpxAlarm::AlarmEnable() const
//
// AlmAlarmEnable:(aEnable&, anAlarmNumber&) : 3
//
	{
#if !defined(__UIQ__)
	TInt aAlarmNumber = iOplAPI.PopInt32();
	TBool aEnable = (iOplAPI.PopInt32() != 0);
	
	if (aAlarmNumber < KMinimumAlarms || KMaximumAlarms < aAlarmNumber)
		User::Leave(KOplErrOutOfRange);
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	almServer.AlarmEnable(aEnable, aAlarmNumber);
	almServer.Close();
#else // UIQ
	iOplAPI.PopInt32();
	iOplAPI.PopInt32();
#endif
	iOplAPI.Push(0.0);
	}

void COpxAlarm::AlarmDelete() const
//
// AlmAlarmDelete:(anAlarmNumber&) : 4
//
	{
	TInt aAlarmNumber = iOplAPI.PopInt32();
	
	if (aAlarmNumber < KMinimumAlarms || KMaximumAlarms < aAlarmNumber)
		User::Leave(KOplErrOutOfRange);
#if !defined(__UIQ__)
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	almServer.AlarmDelete(aAlarmNumber);
	almServer.Close();
#else // UIQ
#endif
	iOplAPI.Push(0.0);
	}

void COpxAlarm::QuietPeriodCancel() const
//
// AlmQuietPeriodCancel: : 5
//
	{
#if !defined(__UIQ__)
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	almServer.QuietPeriodCancel();
	almServer.Close();
#else // UIQ
#endif
	iOplAPI.Push(0.0);
	}

void COpxAlarm::QuietPeriodUntil() const
//
// AlmQuietPeriodUntil:(aDateTime&) : 6
//
	{
#if !defined(__UIQ__)
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	TTime time = almServer.QuietPeriodUntil();
	almServer.Close();
	TDateTime *dtime = (TDateTime *) iOplAPI.PopInt32();
	*dtime = time.DateTime();
#else
	iOplAPI.PopInt32();
#endif
	iOplAPI.Push(0.0);
	}

void COpxAlarm::QuietPeriodSet() const
//
// AlmQuietPeriodSet:(aMinutes&) : 7
//
	{
#if !defined(__UIQ__)
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	TTimeIntervalMinutes aMinutesInterval(iOplAPI.PopInt32());
	almServer.QuietPeriodSet(aMinutesInterval);
	almServer.Close();
#else
	iOplAPI.PopInt32();
#endif
	iOplAPI.Push(0.0);
	}

void COpxAlarm::SetAlarmSoundOn() const
//
// AlmSetAlarmSound:(aState&) : 8
//
	{
#if !defined(__UIQ__)
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	TBool aSetSoundOn = (iOplAPI.PopInt32() != 0);
	almServer.SetAlarmSoundOn(aSetSoundOn);
	almServer.Close();
#else // UIQ
	iOplAPI.PopInt32();
#endif
	iOplAPI.Push(0.0);
	}

void COpxAlarm::AlarmSoundState() const
//
// AlmAlarmSoundState&: : 9
//
	{
#if !defined(__UIQ__)
	RAlarmServer almServer;
	User::LeaveIfError(almServer.Connect());
	iOplAPI.Push(TInt32(almServer.AlarmSoundState()));
	almServer.Close();
#else
	iOplAPI.Push(TInt32(0));
#endif
	}

void COpxAlarm::FindCity() const
//
// WldFindCity:(aSearchCity$, aCallback$) : 10
//
	{
	TBufC<100> procedure(iOplAPI.PopString());
	TPtrC cityName = iOplAPI.PopString();

	RWorldServer worldServer;
	TInt err = worldServer.Connect();

	if (err)
		User::Leave(err);

	TWorldId worldId;
	TCityData aCity;
	TCountryData aCountry;
	err = worldServer.FindCity(worldId, cityName);

	while (!err)
		{
		User::LeaveIfError(worldServer.CityData(aCity,worldId));
		User::LeaveIfError(worldServer.CountryData(aCountry,worldId));
		iOplAPI.InitCallbackL(procedure);
		iOplAPI.PushParamL(aCity.iCity);
		iOplAPI.PushParamL(aCity.iCountry);
		iOplAPI.PushParamL(TInt32(aCity.iLatLong.MinutesLat()));
		iOplAPI.PushParamL(TInt32(aCity.iLatLong.MinutesLong()));
		iOplAPI.PushParamL(aCity.iAreaCode);
		iOplAPI.PushParamL(TInt32(aCity.iUniversalTimeOffset));
		iOplAPI.PushParamL(TInt32(aCity.iDaylightSavingZone));
		iOplAPI.PushParamL(aCountry.iNatCode);
		iOplAPI.PushParamL(aCountry.iCapital);
		iOplAPI.PushParamL(aCountry.iNatPref);
		iOplAPI.PushParamL(aCountry.iIntlPref);
		User::LeaveIfError(iOplAPI.CallProcedure(EReturnLong));
		err = iOplAPI.PopInt32();
		if (!err)
			{
			err = worldServer.NextCity(worldId);
			}
		}
	worldServer.Close();
	iOplAPI.Push(0.0);
}

void COpxAlarm::Sunlight() const
//
// WldSunlight:(aSearchCity$, theTimeOfSunrise&, theTimeOfSunset&, aDate&) : 11
//
	{
	TInt32 aDay = iOplAPI.PopInt32();
	TTime theDay = (aDay != 0) ? TTime(*((TDateTime *)aDay)) : Time::NullTTime();
	TDateTime *sunSet = (TDateTime *) iOplAPI.PopInt32();
	TDateTime *sunRise = (TDateTime *) iOplAPI.PopInt32();
	TPtrC cityName = iOplAPI.PopString();

	RWorldServer worldServer;
	TWorldId worldId;
	TTime aSunRise, aSunSet;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.FindCity(worldId, cityName));
	User::LeaveIfError(worldServer.CalculateSunlight(aSunRise, aSunSet, worldId, theDay));
	worldServer.Close();
	*sunSet = aSunSet.DateTime();
	*sunRise = aSunRise.DateTime();
	iOplAPI.Push(0.0);
	}

void COpxAlarm::Home() const
//
// WldHome$: : 12
//
	{
	RWorldServer worldServer;
	TWorldId worldId;
	TCityData aCity;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.Home(worldId));
	User::LeaveIfError(worldServer.CityData(aCity, worldId));
	worldServer.Close();
	iOplAPI.PushL(aCity.iCity);
	}

void COpxAlarm::MoveCity(TInt aProcNum) const
//
// WldPreviousCity$:(aSearchCity$) : 13
// WldNextCity$:(aSearchCity$) : 15
//
	{
	TPtrC aCityName = iOplAPI.PopString();

	RWorldServer worldServer;
	TWorldId worldId;
	TCityData aCity;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.FindCity(worldId, aCityName));
	switch (aProcNum)
		{
	case EPreviousCity:
		User::LeaveIfError(worldServer.PrevCity(worldId));
		break;
	case ENextCity:
		User::LeaveIfError(worldServer.NextCity(worldId));
		break;
		}
	User::LeaveIfError(worldServer.CityData(aCity, worldId));
	worldServer.Close();
	iOplAPI.PushL(aCity.iCity);
	}

void COpxAlarm::MoveCountry(TInt aProcNum) const
//
// WldPreviousCountry$:(aSearchCountry$) : 14
// WldNextCountry$:(aSearchCountry$) : 16
//
	{
	TPtrC aCountryName = iOplAPI.PopString();

	RWorldServer worldServer;
	TWorldId worldId;
	TCountryData aCountry;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.FindCountry(worldId, aCountryName));
	switch (aProcNum)
		{
	case EPreviousCountry:
		User::LeaveIfError(worldServer.PrevCountry(worldId));
		break;
	case ENextCountry:
		User::LeaveIfError(worldServer.NextCountry(worldId));
		break;
		}
	User::LeaveIfError(worldServer.CountryData(aCountry, worldId));
	worldServer.Close();
	iOplAPI.PushL(aCountry.iCountry);
	}

void COpxAlarm::AddCity() const
//
// WldAddCity:(aCityName$, aCountryName$, aMinsLat&, aMinsLong&, aAreaCode$, aDaylightZone%,
//																aGMTOffset&, aMapX%, aMapY%) : 17
//
	{
	// Get city info
	TInt32 aMapY = iOplAPI.PopInt16();
	TInt32 aMapX = iOplAPI.PopInt16();
	TInt32 aGMTOffset = iOplAPI.PopInt32();
	TDaylightSavingZone aDaylightZone = STATIC_CAST(TDaylightSavingZone,iOplAPI.PopInt16());
	TAreaCode aAreaCode = iOplAPI.PopString();
	TInt32 aLong = iOplAPI.PopInt32();
	TInt32 aLat = iOplAPI.PopInt32();
	TCountryName aCountryName = iOplAPI.PopString();
	TCityName aCityName = iOplAPI.PopString();
	
	// Setup city info
	TLatLong latLong;
	latLong.SetMinutesL(aLat,aLong);

	TCityData cityData;
	cityData.iCity = aCityName;
	cityData.iMapPos = TPoint(aMapX,aMapY);
	cityData.iCountry = aCountryName;
	cityData.iLatLong = latLong;
	cityData.iAreaCode = aAreaCode;
	cityData.iDaylightSavingZone = aDaylightZone;
	cityData.iUniversalTimeOffset = aGMTOffset;
	
	// Add city to database
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.AddCity(cityData));
	worldServer.Close();
	iOplAPI.Push(0.0);
	}

void COpxAlarm::AddCountry() const
//
// WldAddCountry:(aCountryName$, aNationalCode$, aNationalPrefix$, aInternationalPrefix$,
//							aCapitalCity$, aMinsLat&, aMinsLong&, aAreaCode$, aDaylightZone%,
//																aGMTOffset&, aMapX%, aMapY%) : 18
//
	{
	// Get Capital City Info
	TInt32 aMapY = iOplAPI.PopInt16();
	TInt32 aMapX = iOplAPI.PopInt16();
	TInt32 aGMTOffset = iOplAPI.PopInt32();
	TDaylightSavingZone aDaylightZone = STATIC_CAST(TDaylightSavingZone,iOplAPI.PopInt16());
	TAreaCode aAreaCode = iOplAPI.PopString();
	TInt32 aLong = iOplAPI.PopInt32();
	TInt32 aLat = iOplAPI.PopInt32();
	TCityName aCityName = iOplAPI.PopString();
	// Get Country Info
	TIntlPrefCode aInternationalPrefixCode = iOplAPI.PopString();
	TNatPrefCode aNationalPrefixCode = iOplAPI.PopString();
	TNatCode aNationalCode = iOplAPI.PopString();
	TCountryName aCountryName = iOplAPI.PopString();

	// Setup country info
	TCountryData countryData;
	countryData.iCountry = aCountryName;
	countryData.iNatCode = aNationalCode;
	countryData.iCapital = aCityName;
	countryData.iNatPref = aNationalPrefixCode;
	countryData.iIntlPref = aInternationalPrefixCode;

	// Setup capital city info
	TLatLong latLong;
	latLong.SetMinutesL(aLat,aLong);
	
	TCityData cityData;
	cityData.iCity = aCityName;
	cityData.iMapPos = TPoint(aMapX,aMapY);
	cityData.iCountry = aCountryName;
	cityData.iLatLong = latLong;
	cityData.iAreaCode = aAreaCode;
	cityData.iDaylightSavingZone = aDaylightZone;
	cityData.iUniversalTimeOffset = aGMTOffset;
	
	// Add country (and new capital city) to database
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.AddCountry(countryData, cityData));
	worldServer.Close();
	iOplAPI.Push(0.0);
	}

void COpxAlarm::EditCity() const
//
// WldEditCity:(aCityToEdit$, aCountryToEdit$, aCityName$, aCountryName$, aMinsLat&,
//						aMinsLong&, aAreaCode$, aDaylightZone%, aGMTOffset&, aMapX%, aMapY%) : 19
//
	{
	// Get city info
	TInt32 aMapY = iOplAPI.PopInt16();
	TInt32 aMapX = iOplAPI.PopInt16();
	TInt32 aGMTOffset = iOplAPI.PopInt32();
	TDaylightSavingZone aDaylightZone = STATIC_CAST(TDaylightSavingZone,iOplAPI.PopInt16());
	TAreaCode aAreaCode = iOplAPI.PopString();
	TInt32 aLong = iOplAPI.PopInt32();
	TInt32 aLat = iOplAPI.PopInt32();
	TCountryName aCountryName = iOplAPI.PopString();
	TCityName aCityName = iOplAPI.PopString();
	TCountryName aCountryToEdit = iOplAPI.PopString();
	TCityName aCityToEdit = iOplAPI.PopString();

	// Setup city info
	TLatLong latLong;
	latLong.SetMinutesL(aLat,aLong);

	TCityData cityData;
	cityData.iCity = aCityName;
	cityData.iMapPos = TPoint(aMapX,aMapY);
	cityData.iCountry = aCountryName;
	cityData.iLatLong = latLong;
	cityData.iAreaCode = aAreaCode;
	cityData.iDaylightSavingZone = aDaylightZone;
	cityData.iUniversalTimeOffset = aGMTOffset;

	// Update city
	RWorldServer worldServer;
	TWorldId worldId;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.Find(worldId, aCityToEdit, aCountryToEdit));
	User::LeaveIfError(worldServer.UpdateCity(worldId, cityData));
	worldServer.Close();
	iOplAPI.Push(0.0); // returns nothing
	}

void COpxAlarm::EditCountry() const
//
// WldEditCountry:(aCountryToEdit$, aCountryName$, aNationalCode$, aNationalPrefix$,
//														aInternationalPrefix$, aCapitalCity$) : 20
//
	{
	// Get Country Info
	TCityName aCityName = iOplAPI.PopString();
	TIntlPrefCode aInternationalPrefixCode = iOplAPI.PopString();
	TNatPrefCode aNationalPrefixCode = iOplAPI.PopString();
	TNatCode aNationalCode = iOplAPI.PopString();
	TCountryName aCountryName = iOplAPI.PopString();
	TCountryName aCountryToEdit = iOplAPI.PopString();

	// Setup country info
	TCountryData countryData;
	countryData.iCountry = aCountryName;
	countryData.iNatCode = aNationalCode;
	countryData.iCapital = aCityName;
	countryData.iNatPref = aNationalPrefixCode;
	countryData.iIntlPref = aInternationalPrefixCode;

	// Update city
	RWorldServer worldServer;
	TWorldId worldId;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.FindCountry(worldId, aCountryToEdit));
	User::LeaveIfError(worldServer.UpdateCountry(worldId, countryData));
	worldServer.Close();
	iOplAPI.Push(0.0); // returns nothing
	}

void COpxAlarm::DeleteCity() const
//
// WldDeleteCity:(aCityName$, aCountryName$) : 21
//
	{
	TCountryName aCountryName = iOplAPI.PopString();
	TCityName aCityName = iOplAPI.PopString();

	RWorldServer worldServer;
	TWorldId worldId;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.Find(worldId, aCityName, aCountryName));
	User::LeaveIfError(worldServer.DeleteCity(worldId));
	worldServer.Close();
	iOplAPI.Push(0.0); // returns nothing
	}

void COpxAlarm::DeleteCountry() const
//
// WldDeleteCountry:(aCountryName$) : 22
//
	{
	TCountryName aCountryName = iOplAPI.PopString();
	
	RWorldServer worldServer;
	TWorldId worldId;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.FindCountry(worldId, aCountryName));
	User::LeaveIfError(worldServer.DeleteCountry(worldId));
	worldServer.Close();
	iOplAPI.Push(0.0); // returns nothing
	}

void COpxAlarm::NumberOfCities() const
//
// WldNumberOfCities&: : 23
//
	{
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	TInt32 noCities = worldServer.NumberCities();
//	User::LeaveIfError(noCities);
	worldServer.Close();
	iOplAPI.Push(noCities); // returns result of the NumberCities() call
	}

void COpxAlarm::NumberOfCountries() const
//
// WldNumberOfCountries&: : 24
//
	{
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	TInt32 noCountries = worldServer.NumberCountries();
//	User::LeaveIfError(noCountries);
	worldServer.Close();
	iOplAPI.Push(noCountries); // returns result of the NumberCountries() call
	}

void COpxAlarm::SaveDataFile() const
//
// WldDataFileSave: : 25
//
	{
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.DataFileSave());
	worldServer.Close();
	iOplAPI.Push(0.0); // returns nothing
	}

void COpxAlarm::RevertDataFile() const
//
// WldDataFileRevert: : 26
//
	{
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	// This should leave with 'Does not exist' in OPL if no data RAM file exists
	User::LeaveIfError(worldServer.DataFileRevertToSaved());
	User::LeaveIfError(worldServer.DataFileSave()); // does it need saving after revert? Leave in just to be safe :-)
	worldServer.Close();
	iOplAPI.Push(0.0); // returns nothing
	}

/*
void COpxAlarm::DataFileLocation() const
//
// WldDataFileLocation$: : 27
//
	{
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	TFileName dataFile;
	User::LeaveIfError(worldServer.DataFileLocation(dataFile));
	worldServer.Close();
	iOplAPI.PushL(dataFile); // returns data file location
	}

void COpxAlarm::ResetAllData() const
//
// WldDataFileReset: : 28
//
	{
	RWorldServer worldServer;
	User::LeaveIfError(worldServer.Connect());
	User::LeaveIfError(worldServer.ResetAllData());
	worldServer.Close();
	iOplAPI.Push(0.0); // returns nothing
	}
*/

//////////////////////////////////////////////////////////////////////////
// The members of COpxAlarm which are not language extension procedures //
//////////////////////////////////////////////////////////////////////////
COpxAlarm::COpxAlarm(OplAPI& aOplAPI)
	:COpxBase(aOplAPI)
	{
	}

COpxAlarm* COpxAlarm::NewLC(OplAPI& aOplAPI)
	{
	COpxAlarm* This=new(ELeave) COpxAlarm(aOplAPI);
	CleanupStack::PushL(This);
	This->ConstructL();
	return This;
	}

void COpxAlarm::ConstructL()
	{
	}

COpxAlarm::~COpxAlarm()
	// Required so that Tls is set to zero on unloading the OPX
	{
	Dll::FreeTls();
	}

////////////////////////////////
// Implementation of COpxBase //
////////////////////////////////
void COpxAlarm::RunL(TInt aProcNum)
	// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case ESetClockAlarm:
		SetClockAlarm();
		break;
	case EAlarmState:
		AlarmState();
		break;
	case EAlarmEnable:
		AlarmEnable();
		break;
	case EAlarmDelete:
		AlarmDelete();
		break;
	case EQuietPeriodCancel:
		QuietPeriodCancel();
		break;
	case EQuietPeriodUntil:
		QuietPeriodUntil();
		break;
	case EQuietPeriodSet:
		QuietPeriodSet();
		break;
	case ESetAlarmSoundOn:
		SetAlarmSoundOn();
		break;
	case EAlarmSoundState:
		AlarmSoundState();
		break;
	case EFindCity:
		FindCity();
		break;
	case ESunlight:
		Sunlight();
		break;
	case EHome:
		Home();
		break;
	case EPreviousCity:
	case ENextCity:
		MoveCity(aProcNum);
		break;
	case EPreviousCountry:
	case ENextCountry:
		MoveCountry(aProcNum);
		break;
	case EAddCity:
		AddCity();
		break;
	case EAddCountry:
		AddCountry();
		break;
	case EEditCity:
		EditCity();
		break;
	case EEditCountry:
		EditCountry();
		break;
	case EDeleteCity:
		DeleteCity();
		break;
	case EDeleteCountry:
		DeleteCountry();
		break;
	case ENumberOfCities:
		NumberOfCities();
		break;
	case ENumberOfCountries:
		NumberOfCountries();
		break;
	case ESaveDataFile:
		SaveDataFile();
		break;
	case ERevertDataFile:
		RevertDataFile();
		break;
/*	case EDataFileLocation:
		DataFileLocation();
		break;
	case EResetDataFile:
		ResetAllData();
		break;
*/	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

TBool COpxAlarm::CheckVersion(TInt aVersion)
	// To check whether the opx is a compatible version
	{
	if ((aVersion & 0xFF00)>(KOpxVersion & 0xFF00))
		return EFalse; // Bad version
	else
		return ETrue; // OK
	}

EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
	// Creates a COpxBase instance as required by the OPL runtime
	// This object is to be stored in the OPX's TLS as shown below
	{
	COpxAlarm* tls=((COpxAlarm*)Dll::Tls());
	// tls is NULL on loading an OPX DLL (also after unloading it)
	if (tls==NULL)
		{
		tls=COpxAlarm::NewLC(aOplAPI);
		User::LeaveIfError(Dll::SetTls(tls));
		CleanupStack::Pop(); // tls
		}
	return (COpxBase*)tls;
	}

EXPORT_C TUint Version()
	{
	return KOpxVersion;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	// DLL entry point
	{
	return(KErrNone);
	}
