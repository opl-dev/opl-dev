include "printer.oxh"
include "prntst.oxh"
include "const.oph"

PROC main:
LOCAL x&

SendStringToPrinter:("This string was sent first. ")
InsertString:("This string was inserted at the front.  ", 0)

SendNewParaToPrinter:
SetAlignment:(KPrintCenterAlign%)
SetLocalParaFormat:
SendStringToPrinter:("This text is centered. ")
SendSpecialCharToPrinter:(KLineBreak%)
x& = PrinterDocLength&:

InsertSpecialChar:(KParagraphDelimiter%, x&)
SetALignment:(KPrintRightAlign%)
SetLocalParaFormat:
SendStringToPrinter:("This text is right aligned. ")
SendNewParaToPrinter:
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KLineBreak%)
SetAlignment:(KPrintLeftAlign%)
SetLocalParaFormat:
SendStringToPrinter:("This paragraph, and the next few following are left aligned")
SetFontUnderline:(KUnderlineOn%)
SendNewParaToPrinter:
SendSpecialCharToPrinter:(KLineBreak%)
SendStringToPrinter:("It must be emphasised that this paragraph has all the text underlined.")
SendNewParaToPrinter:
SendSpecialCharToPrinter:(KLineBreak%)
SetFontUnderline:(KUnderlineOff%)
SetFontStrikethrough:(KStrikethroughOn%)
SendStringToPrinter:("This paragraph has strikethrough of all text. ")
SendStringToPrinter:("It may be that although text is printed as strikethrough, that this font mode is not supported by the current printer. ")
SendStringToPrinter:("The same goes for many other forms of formatting of course.")
SendNewParaToPrinter:
SendSpecialCharToPrinter:(KLineBreak%)
SetFontStrikethrough:(KStrikethroughOff%)
SendStringToPrinter:("This paragraph is in the standard global text for this document. ")
SendStringToPrinter:("This global formatting is actually set later after this text is inserted, but nonethless takes effect on all globally formatted text. ")
SendNewParaToPrinter:
SendSpecialCharToPrinter:(KLineBreak%)
SendStringToPrinter:("This is another globally formatted paragraph. Its only here as a way of filling up the page to make later paragraphs start on a new page. ")
SendNewParaToPrinter:


InitialiseParaFormat:(255,255,255,100,100,100,KPrintJustifiedAlign%, KPrintTopAlign%,300, KLineSpacingExactlyInTwips%, 100, 200, KTrue%, KTrue%, KFalse%, KTrue%, KTrue%, 200, 2880 )
SetLocalParaFormat:
SendStringToPrinter:("This paragraph has been formatted using the InitialiseParaFormat proc. ")
SendStringToPrinter:("It contains a number of line breaks and tabs, and has been given the following set of attributes:")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("White background")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Left margin = 100 twips")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Right Margin = 100 twips")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Indent = 100 twips")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Text is justified")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Text is top aligned")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("The line spacing is 300 twips")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Line spacing control is exactly in twips")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Space before the paragraph is 100 twips, and space after is 200")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Lines are kept together on a page")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("This paragraph is kept with the next")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("No new page is started")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Widow orphan protection is being used")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Word wrap is turned on")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Border margin is 200 twips")
SendSpecialCharToPrinter:(KLineBreak%)
SendSpecialCharToPrinter:(KTabCharacter%)
SendStringToPrinter:("Tab width is set at 2 inches")

SendNewParaToPrinter:
SetLocalParaFormat:
SendStringToPrinter:("This is a new paragraph with the same paragraph format as the previous one. ")
SendStringToPrinter:("It should be easier to determine whether the paragraph settings are operating correctly with two paragraphs having the same format. ")
SendStringToPrinter:("Of course ther will need to be enough text in each of the paragraphs for the format settings to be correctly tested. ")
SendStringToPrinter:("For instance, there needs to be enough text to fill the width of the page, in order to test that it is justified. ")
SendStringToPrinter:("Ideally there should be enough  text to make these two paragraphs and the next one start a new page.")

SendNewParaToPrinter:
RemoveSpecificParaFormat:
SendStringToPrinter:("This new paragraph is in the default paragraph formatting. ")
SetFontName:("Times New Roman")
SetFontHeight:(280)
SetGlobalCharFormat:
SendStringToPrinter:("The default text format for the whole document is 14pt Times New Roman. ")
SetFontName:("Arial")
SendStringToPrinter:("But text could be printed in Arial")
SetFontHeight:(240)
SendStringToPrinter:(" at 12 point. ")
SetFontWeight:(KStrokeWeightBold%)
SendStringToPrinter:("Text could be printed in bold type. ")
SendStringToPrinter:("And text can be printed as either ")
SetFontPosition:(KPrintPosSuperscript%)
SendStringToPrinter:("superscript")
SetFontPosition:(KPrintPosNormal%)
SendStringToPrinter:("or ")
SetFontPosition:(KPrintPosSubscript%)
SendStringToPrinter:("subscript. ")
RemoveSpecificCharFormat:
SendStringToPrinter:("Any local character formatting can be removed if desired, by reinstating the global format. ")
SetFontPosture:(KPostureItalic%)
SendStringToPrinter:("Then new settings can be layered on top, such as italics. ")
SetFontPosture:(KPostureUpright%)
PRINT "Some formatted text has been printed. Now press enter to bring up the page setup dialog"
get
PageSetupDialog:
PRINT "Press Enter to bring up the print range dialog"
get
PrintRangeDialog:
PRINT "Press Enter to bring up the print preview dialog"
get
PrintPreviewDialog:
PRINT "Press enter to bring up the print dialog"
get
PrintDialog:
COPY "c:\temp.prn", "c:\text.tst"
ResetPrinting:
SendStringToPrinter:("First we'll send this text, then we'll insert a bitmap before, and then a scaled bitmap before that. ")
SendStringToPrinter:("Then well send a bitmap and finally a scaled bitmap after that. ")
x& = gLOADBIT("MINES.MBM",0,0)
			gUSE 2
			gAT 200,10
			gCOPY x&, 0, 0, 100, 80, 3
InsertBitmap:( x&,0)
InsertScaledBitmap:(x&, 0, 2000, 1500)
SendBitmapToPrinter:(x&)
SendScaledBitmapToPrinter:(x&, 800, 800)
CLS
PRINT "The formatted text has been deleted and replaced with some bitmaps and some text."
PRINT "Press enter to bring up the print preview dialog"
get
PrintPreviewDialog:
COPY "c:\temp.prn", "c:\bmp.tst"
CLS
ResetPrinting:
PRINT "Press Enter to replace the text with some generated by a different OPX"
x& = GetRichText&:
SendRichTextToPrinter:(x&)
PrintPreviewDialog:
COPY "c:\temp.prn", "c:\rtext.tst"
CLS
ResetPrinting:
PRINT "Press enter to bring up a text entry dialog"
GET
dEditM:
PrintPreviewDialog:
ENDP

CONST KLenBuffer%=399
PROC dEditM:
LOCAL buffer&(101)	REM 101=1+(399+3)/4 in integer arithmetic
LOCAL pLen&,pText&
LOCAL i%
LOCAL c%

pLen&=ADDR(buffer&(1))
pText&=ADDR(buffer&(2))
WHILE 1
	dINIT "Try dEditMulti"
	  dEditMulti pLen&,"Prompt",10,3,KLenBuffer%
	  dBUTTONS "Done",%d	REM button needed to exit dialog
  IF DIALOG=0 :BREAK :ENDIF
SendBufferToPrinter:(ADDR(buffer&(1)))

ENDWH
ENDP
