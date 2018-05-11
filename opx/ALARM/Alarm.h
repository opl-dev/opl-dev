// ALARM.H - Header file for ALARM.OPX OPL Language Extension
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#ifndef __ALARM_H__
#define __ALARM_H__

#include <e32base.h>
#include <oplapi.h>
#include <opx.h>
#if !defined(__UIQ__)
#include <t32alm.h>
#include <t32wld.h>
#else
#include <asclisession.h>
#include <t32wld.h>
#endif
#include <oplerr.h>

const TInt KOpxVersion=0x600;
const TInt KUidOpxAlarm=0x10004EC8;
const TInt KMinimumAlarms=0;
const TInt KMaximumAlarms=7;
_LIT(KAlarmSilentText,"SILENT");

class COpxAlarm : public COpxBase 
	{
public:
	static COpxAlarm* NewLC(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
private:
	// The language extension procedure ordinals
	enum TExtensions
		{
		ESetClockAlarm=1,
		EAlarmState,
		EAlarmEnable,
		EAlarmDelete,
		EQuietPeriodCancel,
		EQuietPeriodUntil,
		EQuietPeriodSet,
		ESetAlarmSoundOn,
		EAlarmSoundState,
		EFindCity,
		ESunlight,
		EHome,
		EPreviousCity,
		EPreviousCountry,
		ENextCity,
		ENextCountry,
		EAddCity,
		EAddCountry,
		EEditCity,
		EEditCountry,
		EDeleteCity,
		EDeleteCountry,
		ENumberOfCities,
		ENumberOfCountries,
		ESaveDataFile,
		ERevertDataFile /*,
		EDataFileLocation,
		EResetDataFile
*/		};
	void SetClockAlarm() const;
	void AlarmState() const;
	void AlarmEnable() const;
	void AlarmDelete() const;
	void QuietPeriodCancel() const;
	void QuietPeriodUntil() const;
	void QuietPeriodSet() const;
	void SetAlarmSoundOn() const;
	void AlarmSoundState() const;
	void FindCity() const;
	void Sunlight() const;
	void Home() const;
	void MoveCity(TInt aProcNum) const;
	void MoveCountry(TInt aProcNum) const;
	void AddCity() const;
	void AddCountry() const;
	void EditCity() const;
	void EditCountry() const;
	void DeleteCity() const;
	void DeleteCountry() const;
	void NumberOfCities() const;
	void NumberOfCountries() const;
	void SaveDataFile() const;
	void RevertDataFile() const;
/*	void DataFileLocation() const;
	void ResetAllData() const; */
private:
	void ConstructL();
	COpxAlarm(OplAPI& aOplAPI);
	~COpxAlarm();
};

#endif
