rem t_MIME.tpl
rem Testing ER5 MIME functionality.
rem v0.01 25 Jan 99.

include "Const.oph"

declare external

rem Some random UID here:
const KUidtmime&=&00face0ff

external main:
external printcmd:
external handlecmd:
external create:(f$)
external open:(f$)
external run:(f$)

APP pr5mime,KUidtmime&
	icon "\Opl\Oplr\Opltest\Data\t_mime.mbm"
	icon "\Opl\Oplr\Opltest\Data\t_mimem.mbm"
	FLAGS 1
	Caption "MimeTester",1
	MIME KDatatypePriorityHigh%,"text/plain"
REM	MIME KDatatypePriorityHigh%,"text/html"
ENDA


proc main:
	print "t_MIME running"
	printcmd:
	get
endp


proc printcmd:
	local c$(255)
	print
	c$=cmd$(1)
	print "OPA:  "+c$
	c$=cmd$(2)
	print "Path: "+c$
	c$=cmd$(3)
	print "Type: "+c$
	print
endp


proc testMIME:
	print
	print "Testing the MIME recognition properties"
	print
	print "The test procedure is:"
	print " 1. Ensure the t_MIME (MimeTester) program is visible in the Extras bar"
	print "    If not, locate and translate the t_MIME OPL program."
	print " 2. From the shell, observe the first target file: testnote.txt - a text file"
	print " 3. Confirm the target uses the t_MIME icon"
	print " 4. Select and tap the target. Confirm t_MIME runs"
	print " 5. Check the Path: displayed in t_MIME is the target filename"
	print " 6. Edit the t_MIME program so it associates with 'text/html' files."
	print " 7. Repeat steps 3-5 for the second target file: testHTML.htm - an HTML file."
	print "End of MIME test"
	print
endp

REM End of tMIME.tpl
