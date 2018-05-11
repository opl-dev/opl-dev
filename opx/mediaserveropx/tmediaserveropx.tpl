rem TMediaServerOpx.TPL
rem Test code for Crystal MediaServer OPX
rem

DECLARE EXTERNAL
INCLUDE "MediaServerOpx.oxh"

EXTERNAL tToneSimple:
EXTERNAL tToneAll:
EXTERNAL tToneCancelPrepare:
EXTERNAL tToneCancelPlay:
EXTERNAL tFileSequence:

EXTERNAL tFilePlayerSimple:
EXTERNAL tFilePlayerAll:
EXTERNAL tFilePlayerStop:

CONST KSeqFilename$="C:\Documents\tMediaServer\Sequence"
CONST KWAVFilename$="C:\Documents\tMediaServer\tPlay.wav"
CONST KDuration&=2000000 rem 2.0 seconds
CONST KFreq&=3000 rem 3KHz

CONST KPriority&=KMdaPriorityNormal&
CONST KPreference&=KMediaSMdaPriorityPrefTimeQual&

PROC Main:
	GLOBAL gStatus&
	tToneSimple:
	tToneAll:
	tToneCancelPrepare:
	tToneCancelPlay:
	tFilePlayerSimple:
	tFilePlayerAll:
	tFilePlayerStop:
	PRINT "Test code complete." :PRINT "Press any key" :GET
ENDP


rem
rem Audio tone
rem 

PROC tToneSimple:
	LOCAL status1&,status2&
	PRINT "tToneSimple:"
	PRINT "Simple audio tone demo"
	PRINT "	Calling PrepareToPlayToneA:()"
	PrepareToPlayToneA:(KFreq&,KDuration&,status1&) rem 3KHz for 2.0 sec.
	IOWAITSTAT32 status1&
	ToneSetVolume:(ToneMaxVolume&:)
	PRINT "	PlayToneA:()"
	PlayToneA:(status2&)
	IOWAITSTAT32 status2&
	PRINT
ENDP


PROC tToneAll:
	EXTERNAL gStatus&
	LOCAL freq&,duration&,trailingSilence&
	LOCAL volRampDuration&
	LOCAL vol&,maxVol&
	LOCAL toneState%

	PRINT "tToneAll:"
	PRINT "Exercise all tone APIs"
	toneState%=ToneState%:
	IF (toneState%<>KMdaAudioToneUtilityNotReady%) AND 	(toneState%<>KMdaAudioToneUtilityPrepared%)
		PRINT ToneState%:
		RAISE 1
	ENDIF

	freq&=KFreq&
	duration&=KDuration&
	PRINT "	PrepareToPlayToneA:"
	PrepareToPlayToneA:(freq&, duration&, gStatus&)
	IOWAITSTAT32 gStatus&

	IF ToneState%:<>KMdaAudioToneUtilityPrepared% :RAISE 2 :ENDIF

	vol&=ToneVolume&:
	maxVol&=ToneMaxVolume&:
	ToneSetVolume:(maxVol&)
	IF ToneVolume&:<>maxVol& :RAISE 3 :ENDIF rem Should be max.

	SetPriority:(0,KMediaSMdaPriorityPrefQual&)

	trailingSilence&=1000000
	SetRepeats:(2, trailingSilence&) 

	volRampDuration&=4000000
	SetVolumeRamp:(volRampDuration&)

	PRINT "	PlayToneA: (2 repeats with volume ramping)"
	PlayToneA:(gStatus&)
	IOWAITSTAT32 gStatus&
	REM Reset the repeat value for the subsequent tests.
rem	SetRepeats:(1, trailingSilence&) 
	PRINT
ENDP


PROC tToneCancelPrepare:
	EXTERNAL gStatus&
	PRINT "tToneCancelPrepare:"
	PRINT "Exercice the cancel API"
	PrepareToPlayToneA:(KFreq&,KDuration&,gStatus&)
	IOWAITSTAT32 gStatus&
	PRINT "	CancelPrepare:"
	CancelPrepare:
rem	PRINT "	IOWAITSTAT32"
rem	IOWAITSTAT32 gStatus&
	PRINT
ENDP


PROC tToneCancelPlay:
	EXTERNAL gStatus&
	PRINT "tToneCancelPlay:"
	PrepareToPlayToneA:(KFreq&,KDuration&,gStatus&)
	IOWAITSTAT32 gStatus&
	PRINT "	PlayToneA:"
	PlayToneA:(gStatus&)
	PRINT "	CancelPlay:"
	CancelPlay:
rem	PRINT "	IOWAITSTAT32"
rem	IOWAITSTAT32 gStatus&
	PRINT
ENDP


rem
rem FilePlayer
rem 


PROC tFilePlayerSimple:
	EXTERNAL gStatus&
	LOCAL priority&,preference&
	IF NOT EXIST(KWAVFilename$)
		ALERT(KWAVFilename$,"not found, test abandoned.")
		RETURN
	ENDIF
	priority&=0
	preference&=KPreference&
	CreateFilePlayerSimpleA:(KWAVFilename$,gStatus&)
	IOWAITSTAT32 gStatus&
	PRINT "	PlayFileA:"
	PlayFileA:(gStatus&)
	IOWAITSTAT32 gStatus&
	CloseFilePlayer:
	PRINT
ENDP


PROC tFilePlayerAll:
	EXTERNAL gStatus&
	LOCAL vol&,repeat&,trailingSilence&,rampDuration&
	LOCAL duration&
	CreateFilePlayerA:(KWAVFilename$,KPriority&,KPreference&,gStatus&)
	IOWAITSTAT32 gStatus&
	vol&=FileMaxVolume&:
	FileSetVolume:(vol&)
	repeat&=2
	trailingSilence&=1000000 rem 1 sec
	FileSetRepeats:(repeat&,trailingSilence&)
	rampDuration&=3000000 rem 3 sec
	FileSetVolumeRamp:(rampDuration&)
	duration&=FileDuration&:
	PRINT "	FileDuration&:=",duration&
	PRINT "	PlayFileA:"
	PlayFileA:(gStatus&)
	IOWAITSTAT32 gStatus&
	CloseFilePlayer:
	PRINT
ENDP


PROC tFilePlayerStop:
	EXTERNAL gStatus&
	CreateFilePlayerA:(KWAVFilename$,KPriority&,KPreference&,gStatus&)
	IOWAITSTAT32 gStatus&
	PlayFileA:(gStatus&)
	StopFile:
	CloseFilePlayer:
ENDP

REM End.
