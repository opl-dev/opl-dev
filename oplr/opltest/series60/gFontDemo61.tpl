REM gFontDemo61.tpl
REM 27 July 03
REM ricka@users.sourceforge.net

REM Demo code showing the built-in fonts on Series 60 phones.
REM Uses these S60 font ids from const.oph:

REM CONST KFontS60LatinPlain12&=&10000001
REM CONST KFontS60LatinBold12&=&10000002
REM CONST KFontS60LatinBold13&=&10000003
REM CONST KFontS60LatinBold17&=&10000004
REM CONST KFontS60LatinBold19&=&10000005
REM CONST KFontS60NumberPlain5&=&10000006
REM CONST KFontS60ClockBold30&=&10000007
REM CONST KFontS60LatinClock14&=&10000008
REM CONST KFontS60Custom&=&10000009
REM CONST KFontS60ApacPlain12&=&1000000c
REM CONST KFontS60ApacPlain16&=&1000000d


DECLARE EXTERNAL

INCLUDE "const.oph" 
EXTERNAL style:
EXTERNAL Show:(aFont&,aName$)

PROC gFontDemo61:
	LOCAL id%
	LOCAL font&
	id%=gCREATE(0,44, 176,164, 1,1)
	Show:(KFontS60LatinPlain12&,"LatinPlain12")
	Show:(KFontS60LatinBold12&,"LatinBold12")
	Show:(KFontS60LatinBold13&,"LatinBold13")
	Show:(KFontS60LatinBold17&,"LatinBold17")
	Show:(KFontS60LatinBold19&,"LatinBold19")
	Show:(KFontS60NumberPlain5&,"NumberPlain5")
	Show:(KFontS60ClockBold30&,"ClockBold30")
	Show:(KFontS60LatinClock14&,"LatinClock14")
	Show:(KFontS60Custom&,"Custom")
	Show:(KFontS60ApacPlain12&,"ApacPlain12")
	Show:(KFontS60ApacPlain16&,"ApacPlain16")
	gCLOSE id%
	AT 1,5
	PRINT "Demo over."
	PRINT "Press any key."
	GET
ENDP


PROC Show:(aFont&,aName$)
	gCLS
	gBORDER $100
	gFONT aFont&
	Style:
	gFONT KFontS60LatinPlain12&
	gAT 10,20 :gPRINT aName$
	gAT 10,159 :gPRINT "Press any key."
	IF GET=8
		STOP
	ENDIF
ENDP


PROC Style:
	gAT 80,40 :gPRINT "'0123 ABCD abcd'"
	gSTYLE KgStyleBold%
	gAT 20,60 :gPRINT "Bold"
	gSTYLE KgStyleUnder%
	gAT 100,60 :gPRINT "Underline"
	gSTYLE KgStyleInverse%
	gAT 20,80 :gPRINT "Inverse"
	gSTYLE KgStyleDoubleHeight%
	gAT 100,100 :gPRINT "Double h."
	gSTYLE KgStyleMonoFont%
	gAT 20,100 :gPRINT "Mono font"
	gSTYLE KgStyleItalic%
	gAT 20,120 :gPRINT "Italic"
ENDP

REM Ends.