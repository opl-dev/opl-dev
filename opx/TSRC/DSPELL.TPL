REM DSPELL.OPL
REM
REM Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.
REM
REM Demo code for Spell OPX

INCLUDE "SPELL.OXH"

PROC Main:
	local string$(255), word$(100)
	print "Do you want to use the user dictionary, too?"
	print "The system setting is:  User dictionary ";
	if SpellUserDict&:<>0
		print "ON"
	else
		print "OFF"
	endif
	print "Y/N ? ";
	if upper$(get$)="Y"
		rem You have to open the speller before you use it
		rem the argument determines whether to use the user dictionary
		OpenSpeller:(&1)
	else
		OpenSpeller:(&0)
	endif
	print
	SpellExtras:
	do 
	  print "Enter a string: ";
	  input string$
	  word$=Spell$:(string$)
	  if word$=""
	  	  print "Correct"
	  	else
	  	  print "Incorrect:",word$
	  	  print "Alternatives are:"
	  	  SpellAlternatives:(word$, &10, "PrintAlternative")
	  	  print
	  	endif
	until string$=""
	rem And now try Spellbuffer
	TrySpellBuffer:
	rem you should close the speller when it's no longer used
	rem the user dictionary cannot be shared with another application
	rem (like spell)
	CloseSpeller:
ENDP

PROC PrintAlternative&:(alter$)
	print alter$;", ";
	rem return &1 if you want to stop after some alternative
	return &0
ENDP

CONST KLenBuffer%=1024

PROC TrySpellBuffer:
	local pLen&, pText&, len&, p&
	local word$(255)
	pLen&=ALLOC(KLenBuffer% + 4)
	POKEL pLen&, &2
	pText&=pLen& + 4
	POKEL pText&, &00006948
	dINIT "Now try SpellBuffer"
	dEDITMULTI pLen&, "Prompt", 10, 3, KLenBuffer%
	dBUTTONS "Done",%d
	DIALOG
	p&=pText&
	len&=PEEKL(pLen&)
	do 
		word$=SpellBuffer$:(p&, len&)
		if word$<>""
			len&=len& - SpellWhere&: - LEN(word$)
			p&=p& + SpellWhere&: + LEN(word$)
			print "Incorrect:",word$
			print "Alternatives are:"
			SpellAlternatives:(word$, &10, "PrintAlternative")
			print
		endif
	until word$=""
	print "Press a key"
	get$
ENDP

PROC SpellExtras:
	local str$(255), len&
	do
		print "Enter a word with wildcards:",
		input str$
		if str$<>""
			print "Expansions are: ";
			SpellWildcards:(str$, &10, "PrintAlternative")
			print
		endif
	until str$=""
	print "And now for some anagram finding:"
	print "Word:",
	input str$
	print "Minimal anagram length:",
	input len&
	print "Anagrams are: ";
	SpellAnagrams:(str$, len&, &100, "PrintAlternative")
	print
ENDP