DECLARE EXTERNAL
CONST KErrInvalidArgs%=-2
EXTERNAL tLOCNearestLanguageFile:(aDrive$)
EXTERNAL tLOCLanguage:
EXTERNAL tLOCMisc:
EXTERNAL tLOCDayNames:(aSpecific&)
EXTERNAL tLOCMonthNames:(aSpecific&)
EXTERNAL tLOCPositionFunction:(aCurrency%)
EXTERNAL tLOCSetPositionFunction:(aCurrency%,aPos&)
INCLUDE "Locale.oxh"

PROC Main:
	PRINT "Test Locale OPX"
	PRINT "---------------"
	PRINT
	PRINT "Note that this code assumes the language of the Symbian OS device is"
	PRINT "English -- that is, KLangEnglish% is used."
	PRINT
	PRINT "This may result in false failures on other machines."
	PRINT
	tLOCNearestLanguageFile:("Z")
GET
	tLOCLanguage:
GET
	tLOCMisc:
GET
	tLOCDayNames:(INT(-1))
GET
	tLOCMonthNames:(INT(-1))
GET
	tLOCPositionFunction:(1)
	tLOCPositionFunction:(2)
GET
	tLOCSetPositionFunction:(1,6)
	tLOCSetPositionFunction:(1,KLocalePosBefore&)
	tLOCPositionFunction:(1)
	tLOCSetPositionFunction:(2,9)
	tLOCSetPositionFunction:(2,KLocalePosAfter&)
	tLOCPositionFunction:(2)
GET
	PRINT
	PRINT "All tests passed. Press any key to finish..."
	GET
ENDP

PROC tLOCNearestLanguageFile:(aDrive$)
LOCAL Rsc$(255)
	Rsc$=LCNearestLanguageFile$:(aDrive$+":\system\data\oplr.rsc")
	PRINT "Nearest language file:",Rsc$
	IF Rsc$<>aDrive$+":\system\data\oplr.r01" AND Rsc$<>aDrive$+":\system\data\oplr.rsc"
		IF aDrive$="Z"
			tLOCNearestLanguageFile:("C")
		ELSE
			RAISE 10
		ENDIF
	ENDIF
ENDP

PROC tLOCLanguage:
LOCAL Lang&
	Lang&=LCLanguage&:
	PRINT "Language:",Lang&
	IF Lang&<1 OR Lang&>24
		RAISE 20
	ENDIF
ENDP

PROC tLOCMisc:
LOCAL ClockFormat$(2,10),i%
	ClockFormat$(1)="Analog"
	ClockFormat$(2)="Digital"

	PRINT "Country code: ";LCCountryCode&:
	PRINT "Decimal seperator: ";LCDecimalSeparator$:
	PRINT "Thousands seperator: ";LCThousandsSeparator$:
	i%=0
	DO
		PRINT "Date seperator "+NUM$(i%,9)+": ";LCDateSeparator$:(i%)
		i%=i%+1
	UNTIL i%=KMaxDateSeparators&+1
	i%=0
	DO
		PRINT "Time seperator "+NUM$(i%,9)+": ";LCTimeSeparator$:(i%)
		i%=i%+1
	UNTIL i%=KMaxTimeSeparators&+1
	IF (LCAmPmSpaceBetween%:)
		PRINT "There IS a space between AM/PM"
	ELSE
		PRINT "There is NOT a space between AM/PM"
	ENDIF
	PRINT "Clock format: ";ClockFormat$(LCClockFormat&:+1)
	PRINT "Set clock format not tested"
	PRINT "Start of week: ";tLOCDayNames:(LCStartOfWeek&:)
ENDP

PROC tLOCDayNames:(aSpecific&)
LOCAL i%
	IF aSpecific&<>-1
		PRINT LCDayNameFull$:(aSpecific&)
		RETURN
	ENDIF
	i%=1
	DO
		PRINT LCDayNameFull$:(i%)
		i%=i%+1
	UNTIL i%=8
ENDP

PROC tLOCMonthNames:(aSpecific&)
LOCAL i%
	IF aSpecific&<>-1
		PRINT LCMonthNameFull$:(aSpecific&)
		RETURN
	ENDIF
	i%=1
	DO
		PRINT LCMonthNameFull$:(i%)
		i%=i%+1
	UNTIL i%=13
ENDP

PROC tLOCPositionFunction:(aCurrency%)
LOCAL Pos&
	IF (aCurrency%=1)
		Pos&=LcCurrencySymbolPosition&:
		PRINT "Currency pos:";
	ELSE
		Pos&=LcAmPmSymbolPosition&:
		PRINT "Am/Pm pos:";
	ENDIF
	IF Pos& = KLocalePosBefore&
		PRINT "Before"
	ELSEIF Pos& = KLocalePosAfter&
	 	PRINT "After"
	ELSE
		RAISE -90
	ENDIF
ENDP

PROC tLOCSetPositionFunction:(aCurrency%,aPos&)
LOCAL Pos&
	ONERR Err::
	IF (aCurrency%=1)
		PRINT "Setting currency pos to:";aPos&
		LcCurrencySetSymbolPosition:(aPos&)
	ELSE
		PRINT "Setting Am/Pm pos to:";aPos&
		LcAmPmSetSymbolPosition:(aPos&)
	ENDIF
	
	ONERR OFF
	RETURN
	Err::
	ONERR OFF
	IF ERR=KErrInvalidArgs%
		PRINT "Trapped attempt to set out of bounds Pos&"
	ELSE
		RAISE ERR
	ENDIF
ENDP