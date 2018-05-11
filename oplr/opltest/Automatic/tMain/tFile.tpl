REM tFile.tpl
REM EPOC OPL automatic test code for database file handling.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"

PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:("tFile", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc tFile:
  global path$(9),drv$(2),patha$(9)
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	
  drv$="c:"
  path$=drv$
  patha$=drv$+"\Opl1993"
  trap mkdir patha$
	
  onerr gotpath::
  setpath path$+"\"  : rem as for Opl1989
	gotpath::
  onerr off
rem  Strtest:("Database File Handling - supplements T_BPFILE.OPL")
	
	hRunTest%:("tCorpBug")
  hRunTest%:("tFind")
  hRunTest%:("tTrapDel")
  hRunTest%:("tFilGen")
  hRunTest%:("tCreate")
  hRunTest%:("tCopy")
  hRunTest%:("tBackNxt")
	rem tWrite: rem ON opl1993 used Data app files - not yet needed on Opler1 but
	rem tRead:  rem code left as template in case OPL later supports Data app files
	hCleanUp%:("CleanUp")
	rem	KLog%:(KhLogHigh%,"Some sample text")
endp


proc CleanUp:
	trap delete "c:\Opl1993\*.*"
	trap rmdir "c:\Opl1993"
	trap delete "copy?.jnk"
	trap delete "tFile.odb"
	trap delete "t_erased.odb"
endp


proc tBackNxt:
	rem	SubTest:("Testing NEXT,BACK at start of file and eof")
  trap delete "j.odb"
	create "j.odb",a,a$
	close
	open "j.odb",a,a$
	back
	
	if a.a$<>""
		rem print a.a$
		rem get
		raise 1
	endif
	if pos<>1 :raise 2 :endif
	next
	if a.a$<>"" :raise 3 :print a.a$ :get :endif
	if pos<>1 :raise 4 :endif
	
	a.a$="1st"
	append
	a.a$="2nd"
	append
	next
	if a.a$<>"" :raise 5 :print a.a$ :get :raise 5 :endif
	if not eof :raise 6 :endif
	first
	back
	if a.a$<>"1st" :raise 7 :print a.a$ :raise 7 :endif
	last
	next
	if a.a$<>"" :print a.a$ :raise 8 : get :raise 8 :endif
	close
	delete "j.odb"
	rem pause pause% :key
endp


proc tCreate:
	rem	SubTest:("Create an existing file")
  trap create "tCreate.odb",a,a$
	trap close
	trap create "tCreate.odb",a,a$
	if err<>-32 : raise 1 :endif
	trap close
	delete "tCreate.odb"
	return
endp


proc tTrapDel:
	rem	SubTest:("Trap delete test")
	trap delete "tFile.odb"     :rem delete tFile.odb
  if err and err<>-33         :rem NotExistsErr
    raise 1
  endif
	
	create "tFile.odb",a,a$
	close
  delete "tFile.odb"
endp


proc tFilGen:
  local rec%,loop%
	
	rem  SubTest:("General but basic test of Dbf related keywords")
  rem print "trap delete"
  trap delete "tfile.odb"
  rem print "create"
  create "tFile.odb",a,a$,i%,l&,d
  rem print "assign string"
  a.a$="tFile,a,a$ - record 1"
  rem print "assign integer field"
  a.i%=$7f
  rem print "assign long field"
  a.l&=&7fffffff
  rem print "assign double field"
  a.d=3.0
  rem print "append 3 records"
  append
  a.a$="r2f1"
  append
  a.a$="r3f1"
  append
  rem print "close"
  close
  rem print "open"
  open "tFile.odb",a,a$,i%,l&,d
  rem print "first"
  first
  rem print a.a$
  rem print "next"
  next
  rem print a.a$
  rem print "back"
  back
  rem print a.a$
  rem print "last"
  last
  rem print a.a$
  first
  rem print "find r2 in record 2"
  rec%=find("*r2*")
  close
  rem print "copy"
  copy "tfile.odb","j.j"
  rem print "delete"
  delete "j.j"
  trap delete "t_erased.odb"
  if rec%<>2 :raise 1 :endif
  rem print "Creating t_erased..."
  create "t_erased.odb",a,rec%,a$
  a.a$="hello"
  rem print "Appending 'recNum",a.a$;"' 10 times"
  loop%=0
  while loop%<10
    loop%=loop%+1
    a.rec%=loop%
    append
  endwh
  rem print "position 2"
  position 2
  rem print "erase records 2,4,6,8,10..."
  while not eof
    erase
    trap next
  endwh
  first
  loop%=1
  while not eof
    rem print "record",loop%,"is now",a.rec%,a.a$
    loop%=loop%+1
    next
  endwh
  rem print "Count=";count
  rem print "position 2"
  position 2
  rem print pos;":",a.a$
  a.a$="updated r2f1"
  rem print "update record 2 to",a.a$
  update
  rem print "first"
  first
  rem print "position 1"
  position 1
  rem print "erase"
  erase
  first
  rem print "Display all records"
  while not eof
    rem print pos;":",a.a$
    next
  endwh
  close
endp


proc tCopy:
	trap delete "copy1.jnk"
	trap delete "copy2.jnk"
	create "copy1.jnk",a,a$
	a.a$="copy1.jnk"
	append
	close
	if not exist("copy1.jnk")
		raise 1
	endif
	
  rem print "COPY ONE FILE TO ANOTHER FILE"
  copy "copy1.jnk","copy2.jnk" :rem <file>,<file>
	if not exist("copy2.jnk")
		raise 2
	endif
  rem print "Ok"
	
  rem print "COPY ONE FILE to ROOT DIRECTORY"
  rem print "copy  copy2.jnk a:\"
  copy "copy2.jnk",patha$+"\"
	if not exist(patha$+"\copy2.jnk")
		raise 3
	endif
  rem print "Ok"
	
  rem print "COPY WILDCARD to ROOT DIRECTORY"
  rem print "copy *.jnk a:\"
  copy "*.jnk",patha$+"\"
	if not exist("copy1.jnk")
		raise 4
	endif
	if not exist("copy2.jnk")
		raise 5
	endif
  rem print "ok"
	
  rem print "COPY WILDCARD to WILDCARD IN ROOT DIRECTORY"
  rem print "copy *.jnk, ";patha$;"*.dat"
  copy "*.jnk",patha$+"\*.dat"
	if not exist("copy1.jnk")
		raise 4
	endif
	if not exist("copy2.jnk")
		raise 5
	endif
  rem print "ok"
  
  rem print
  rem print "TRY ERRORS"
	rem print
  rem print "Trap copy : bad <from> name"
  trap copy "a1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.odb",patha$+"\"
  if err<>KErrName%
  	raise 7
  else
  	rem print "Ok"
  endif
	
  rem print "Trap copy : bad <to> name"
  trap copy "copy1.jnk","a1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.odb"
  if err<>KErrName%
  	raise 8
  else
  	rem print "Ok"
  endif
	
  rem print "Trap copy nexist.jnk \ - non-existent <from>"
  trap copy "nexist.jnk","\"
  if err<>KErrNotExists%
  	raise 10
  else
  	rem print "Ok"
  endif
	
  rem print "Trap copy copy1.jnk \nexist\ - no such <to> directory"
  trap copy "copy1.jnk","\nexist\"
  if err<>KErrDir%
  	raise 11
  else
  	rem print "Ok"
  endif
	
  rem print "Trap copy *.jnk \nexist\ - wild to non-existent <to> directory"
  trap copy "*.jnk","\nexist\"
  if err<>KErrDir%
		raise 12
	else
		rem print "Ok"
	endif
	rem pause -30
endp


proc tWrite:
  global flname$(3,128)
  local pos%,count%
  local org$(32,255),j%,i%
	
	rem  SubTest:("Writing to files generated by Personal Database System")
  copy "find1.dbf","tochange.dbf"
  flName$(1)="tochange.dbf"
  while i%<1
    i%=i%+1
		rem	SubTest:("Opening "+flName$(i%))
    trap open flName$(i%),a,f1$,f2$,f3$,f4$,f5$,f6$,f7$,f8$,f9$,f10$,f11$,f12$,f13$,f14$,f15$,f16$,f17$,f18$,f19$,f20$,f21$,f22$,f23$,f24$,f25$,f26$,f27$,f28$,f29$,f30$,f31$,f32$
		count%=count
		pos%=1
    do
			position pos%
			pos%=pos%+1
			j%=1
			while j%<33
				org$(j%)=getFld$:(j%)
				j%=j%+1
			endwh
			a.f32$=a.f32$+"new"
			if a.f32$<>org$(32)+"new"
				raise 1
			endif
			j%=1
			while j%<32
				if getFld$:(j%)<>org$(j%)
					raise 2
				else
					rem print "a.f";j%;"$ =",org$(j%)
					rem pause pause% :key
				endif
				j%=j%+1
			endwh
			rem print "a.f32$ =",a.f32$
			append
			next
    until pos%=count%
    close
  endwh
endp


proc tRead:
  global flname$(3,128)
	local i%,j%
	
  rem SubTest:("Reading from files generated by Personal Database System")
  flName$(1)="bigrec.dbf"  : rem record 2 too big
  flName$(2)="find1.dbf"
  flName$(3)="find2.dbf"
  while i%<3
    i%=i%+1
		rem SubTest:("Opening "+flName$(i%))
    trap open flName$(i%),a,f1$,f2$,f3$,f4$,f5$,f6$,f7$,f8$,f9$,f10$,f11$,f12$,f13$,f14$,f15$,f16$,f17$,f18$,f19$,f20$,f21$,f22$,f23$,f24$,f25$,f26$,f27$,f28$,f29$,f30$,f31$,f32$
    if err
      if i%<>1 :raise 1 :endif
      if err<>KErrRecord% : raise 2 :endif
			continue			: rem record too big in bigrec.dbf so failed to open
    elseif i%=1
      raise 3  : rem should get error for record 2 too big
    endif
    first
    do
			j%=1
			while j%<33
				rem print "a.f";j%;"$ =",getFld$:(j%)
				j%=j%+1
			endwh
      next
    until eof
    close
  endwh
endp


proc getFld$:(f%)
  if f%=1
    return(a.f1$)
	endif
  if f%=2
    return(a.f2$)
	endif
  if f%=3
    return(a.f3$)
	endif
  if f%=4
    return(a.f4$)
	endif
  if f%=5
    return(a.f5$)
	endif
  if f%=6
    return(a.f6$)
	endif
  if f%=7
    return(a.f7$)
	endif
  if f%=8
    return(a.f8$)
	endif
  if f%=9
    return(a.f9$)
	endif
  if f%=10
    return(a.f10$)
	endif
  if f%=11
    return(a.f11$)
	endif
  if f%=12
    return(a.f12$)
	endif
  if f%=13
    return(a.f13$)
	endif
  if f%=14
    return(a.f14$)
	endif
  if f%=15
    return(a.f15$)
	endif
  if f%=16
    return(a.f16$)
	endif
  if f%=17
    return(a.f17$)
	endif
  if f%=18
    return(a.f18$)
	endif
  if f%=19
    return(a.f19$)
	endif
  if f%=20
    return(a.f20$)
	endif
  if f%=21
    return(a.f21$)
	endif
  if f%=22
    return(a.f22$)
	endif
  if f%=23
    return(a.f23$)
	endif
  if f%=24
    return(a.f24$)
	endif
  if f%=25
    return(a.f25$)
	endif
  if f%=26
    return(a.f26$)
	endif
  if f%=27
    return(a.f27$)
	endif
  if f%=28
    return(a.f28$)
	endif
  if f%=29
    return(a.f29$)
	endif
  if f%=30
    return(a.f30$)
	endif
  if f%=31
    return(a.f31$)
	endif
  if f%=32
    return(a.f32$)
	endif
	raise 1
endp


proc tFind:
	rem SubTest:("Test only fields specified for CREATE/OPEN are used for find")
	rem trap delete "t_find.odb"
	create "t_find.odb",a,a$,b$
	a.a$="r1f1"
	a.b$="r1f2"
	append
	a.a$="r2f1"
	a.b$="r2f2"
	append
	first
	if find("r1f2")<>1 :raise 1 :endif
	next
	if find("r2f2")<>2 :raise 2 :endif
	close
	open "t_find.odb",a,a$ :rem OPEN with 1 less field
	if find("r1f2")<>0 :raise 3 :endif
	next
	if find("r2f2")<>0 :raise 4 :endif
	close
	delete "t_find.odb"
endp


proc tCorpbug:
	rem SubTest:("Test non all-text file with all text sublist")
  trap delete "corpbug.odb"
	rem print "creating.."
	create "corpbug.odb",a,a$,b$,c$,d$,e$,f$,g$,h$,i$,j$,k$,i%,l$,m$,n$,o$
	rem print "closing.."
	close	
	
	rem print "opening.."
	open "corpbug.odb",a,a$,b$,c$,d$,e$,f$,g$,h$,i$,j$,k$
	a.a$="1"
	a.b$="2"
	a.c$="3"
	a.d$="4"
	a.e$="5"
	a.f$="6"
	a.g$="7"
	a.h$="8"
	a.i$="9"
	a.j$="10"
	a.k$="11"
	rem print "appending (not beyond integer).."
	append
	position 1
	do
		if a.a$<>"1" :raise 1 :endif
		if a.b$<>"2" :raise 2 :endif
		if a.c$<>"3" :raise 3 :endif
		if a.d$<>"4" :raise 4 :endif
		if a.e$<>"5" :raise 5 :endif
		if a.f$<>"6" :raise 6 :endif
		if a.g$<>"7" :raise 7 :endif
		if a.h$<>"8" :raise 8 :endif
		if a.i$<>"9" :raise 9 :endif
		if a.j$<>"10" :raise 10 :endif
		if a.k$<>"11" :raise 11 :endif
		next
	until eof
	close
	
	open "corpbug.odb",a,a$,b$,c$,d$,e$,f$,g$,h$,i$,j$,k$,i%,l$,m$,n$,o$
	a.a$="1"
	a.b$="2"
	a.c$="3"
	a.d$="4"
	a.e$="5"
	a.f$="6"
	a.g$="7"
	a.h$="8"
	a.i$="9"
	a.j$="10"
	a.k$="11"
	a.i%=32767
	a.l$="12"
	a.m$="13"
	a.n$="14"
	a.o$="15"
	rem print "appending (beyond integer).."
	append
	position 1
	do
		if a.a$<>"1" :raise 1 :endif
		if a.b$<>"2" :raise 2 :endif
		if a.c$<>"3" :raise 3 :endif
		if a.d$<>"4" :raise 4 :endif
		if a.e$<>"5" :raise 5 :endif
		if a.f$<>"6" :raise 6 :endif
		if a.g$<>"7" :raise 7 :endif
		if a.h$<>"8" :raise 8 :endif
		if a.i$<>"9" :raise 9 :endif
		if a.j$<>"10" :raise 10 :endif
		if a.k$<>"11" :raise 11 :endif
		if pos=1
			if a.i%<>0 :raise 12 :endif
			if a.l$<>"" :raise 13 :endif
			if a.m$<>"" :raise 14 :endif
			if a.n$<>"" :raise 15 :endif
			if a.o$<>"" :raise 16 :endif
		elseif pos=2
			if a.i%<>32767 :raise 12 :endif
			if a.l$<>"12" :raise 13 :endif
			if a.m$<>"13" :raise 14 :endif
			if a.n$<>"14" :raise 15 :endif
			if a.o$<>"15" :raise 16 :endif
		endif
		next
	until eof
	close
	open "corpbug.odb",a,a$,b$,c$,d$,e$,f$,g$,h$,i$,j$,k$
	a.a$="1"
	a.b$="2"
	a.c$="3"
	a.d$="4"
	a.e$="5"
	a.f$="6"
	a.g$="7"
	a.h$="8"
	a.i$="9"
	a.j$="10"
	a.k$="11"
	rem print "appending (not beyond integer).."
	append
	position 1
	do
		if a.a$<>"1" :raise 1 :endif
		if a.b$<>"2" :raise 2 :endif
		if a.c$<>"3" :raise 3 :endif
		if a.d$<>"4" :raise 4 :endif
		if a.e$<>"5" :raise 5 :endif
		if a.f$<>"6" :raise 6 :endif
		if a.g$<>"7" :raise 7 :endif
		if a.h$<>"8" :raise 8 :endif
		if a.i$<>"9" :raise 9 :endif
		if a.j$<>"10" :raise 10 :endif
		if a.k$<>"11" :raise 11 :endif
		next
	until eof
	close
	open "corpbug.odb",a,a$,b$,c$,d$,e$,f$,g$,h$,i$,j$,k$,i%,l$,m$,n$,o$
	do
		if a.a$<>"1" :raise 1 :endif
		if a.b$<>"2" :raise 2 :endif
		if a.c$<>"3" :raise 3 :endif
		if a.d$<>"4" :raise 4 :endif
		if a.e$<>"5" :raise 5 :endif
		if a.f$<>"6" :raise 6 :endif
		if a.g$<>"7" :raise 7 :endif
		if a.h$<>"8" :raise 8 :endif
		if a.i$<>"9" :raise 9 :endif
		if a.j$<>"10" :raise 10 :endif
		if a.k$<>"11" :raise 11 :endif
		if pos=1 or pos=3
			if a.i%<>0 :raise 12 :endif
			if a.l$<>"" :raise 13 :endif
			if a.m$<>"" :raise 14 :endif
			if a.n$<>"" :raise 15 :endif
			if a.o$<>"" :raise 16 :endif
		elseif pos=2
			if a.i%<>32767 :raise 12 :endif
			if a.l$<>"12" :raise 13 :endif
			if a.m$<>"13" :raise 14 :endif
			if a.n$<>"14" :raise 15 :endif
			if a.o$<>"15" :raise 16 :endif
		endif
		next
	until eof
	close
	
	delete "corpbug.odb"
	
  create "corpbug.odb",a,a$,b$,c$,d$
  if a.a$<>"" :raise 1 :print  a.a$ :get :raise 1 :endif
	if a.b$<>"" :raise 2 :print  a.b$ :get :raise 2 :endif
  a.a$="1"
	if a.a$<>"1" :raise 3 :print  a.a$ :get :raise 3 :endif
	if a.b$<>"" :raise 4 :print  a.b$ :get :raise 4 :endif
	if a.c$<>"" :raise 5 :print  a.c$ :get :raise 5 :endif
	if a.d$<>"" :raise 6 :print  a.d$ :get :raise 6 :endif
	if pos<>1 :raise 2 :endif
	back
	if a.a$<>"" :raise 7 :print  a.a$ :get :raise 7 :endif
	if a.b$<>"" :raise 8 :print  a.b$ :get :raise 8 :endif
	if pos<>1 :raise 9 :endif
	
	a.d$="2"
	if a.a$<>"" :raise 10 :print  a.a$ :get : raise 10  :endif
  if a.b$<>"" :raise 11 :print  a.b$ :get :raise 11 :endif
	if a.c$<>"" :raise 12 :print  a.c$ :get :raise 12 :endif
	if a.d$<>"2" :raise 13 :print  a.d$ :get :raise 13 :endif
	append
	first
	if a.a$<>"" :raise 14 :print  a.a$ :get :raise 14 :endif
	if a.b$<>"" :raise 15 :print  a.b$ :get :raise 15 :endif
	if a.c$<>"" :raise 16 :print  a.c$ :get :raise 16 :endif
	if a.d$<>"2" :raise 17 :print  a.d$ :get :raise 17 :endif
	last
	next
	if a.a$<>"" :raise 18 :print  a.a$ :get :raise 18 :endif
	if a.b$<>"" :raise 19 :print  a.b$ :get :raise 19 :endif
	if a.c$<>"" :raise 20 :print  a.c$ :get :raise 20 :endif
	if a.d$<>"" :raise 21 :print  a.d$ :get :raise 21 :endif
	close
	delete "corpbug.odb"
endp


REM End of tFile.tpl
