REM DSUBST.OPL
REM
REM Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
REM
REM Demo of Subst OPX

INCLUDE "DATE.OXH"
INCLUDE "SUBST.OXH"

PROC Main:
	local k$(1), path$(255)
	print "This program allows you to query the drives of"
	print "your EPOC Computer."
	print "You can also SUBST a drive (like the DOS command)"
	DisplayState:
	Print "If you would like to substitute a drive,"
	print "press a drive letter from J to Y."
	print "press any other key to exit."
	k$=UPPER$(get$)
	if k$ < "J" or k$ > "Y"
		return
	endif
	if DriveAttributes&:(k$) and KDriveAttSubsted&
		print "Drive",k$,"is currently substituted."
		print "I'm undoing the substitution."
		SetSubst:(k$, "")
		print "Done. Press any key to exit."
		get$
		return
	endif
	print
	print "***********************************************"
	print
	print "We are about to substitute Drive """;k$;":"""
	print "You need to know:"
	print "The local file system always reserves drive letters A through I."
  print "The internal RAM always uses drive letter C. Drive letter Z is"
  print "always used for the ROM which means that letters J through Y are"
  print "available to be used by SetSubst() or for redirecting."
	print
	print "By entering a path now, you assign that path to drive"
	print "letter """;k$;""".    Whenever that"
  print "drive letter is used, it will be translated into a reference to the"
  print "path specified here. To clear a drive substitution, specify an empty"
  print "substitution path.  If a drive letter is specified in the path, it"
	print "must not itself be substituted or redirected, or the function will"
	print "return an error."
	print
  print "If a filename or extension is included in the path, the function"
	print "will return an error. Therefore, the final component in the path"
	print "must have a trailing backslash to indicate that it is a directory."
	print 
	print "Enter path for drive ";k$;": ";
	input path$
	SetSubst:(k$, path$)
	DisplayState:
	print "Press any key"
	get$
ENDP

PROC DisplayState:
	local i%, state&, subst$(250), k$(1)
	i%=0
	while i% < 26
		k$=chr$(%a + i%)
		state&=DriveAttributes&:(k$)
		if state&<>0
			print "Drive",k$;":",
			if state& and KDriveAttLocal&
				print "is local,",
			endif
			if state& and KDriveAttRom&
				print "is in ROM,",
			endif
			if state& and KDriveAttRedirected&
				print "is redirected,",
			endif
			if state& and KDriveAttSubsted&
				print "is substituted to """;
				subst$=Subst$:(k$)
				print subst$;""",",
			endif
			if state& and KDriveAttInternal&
				print "is internal,",
			endif
			if state& and KDriveAttRemovable&
				print "is removable,",
			endif
			print
		endif
		i%=i% + 1
	endwh
ENDP