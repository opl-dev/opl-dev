REM pOpxDbas.tpl
REM EPOC OPL automatic test code for dbase opx.
REM Copyright (c) 1991-2000 Symbian Ltd. All rights reserved.

INCLUDE "Const.oph"
INCLUDE "hUtils.oph"
INCLUDE "dbase.oxh"


PROC Standalone:
	REM Main procedure called when not running inside test harness.
	LOADM KhUtils$
	hLink:( "pOpxDbas", hThreadIdFromOplDoc&:, KhUserFull%)
	REM After standalone completion, control returns here.
	dINIT "Tests complete" :DIALOG
ENDP


proc pOpxdbas:
	rem hInitTestHarness:(KhInitLocalErrorHandling%, KhInitNotUsed%)
	hRunTest%:("tOpxDbas")
	hRunTest%:("tOpxDbasNum")
	hRunTest%:("tOpxDbasStr")
	hCleanUp%:("CleanUp")
endp

const kpath$="c:\Opl1993\"

PROC CleanUp:
	trap delete kpath$+"*.*"
	trap rmdir kpath$
ENDP


proc topxdbas:
	global exp%,val%,te%,te$(32)
	local c%,d%,k&,l&,co&

	trap mkdir kpath$
	print"Testing dbase OPX"
	print datim$
	trap delete kpath$+"DbOPX.dbf"

	create kpath$+"DbOPX.dbf FIELDS name, age TO table2",A, nam$, age%

	te%=1
	val%=pos
	exp%=1
	test:

	c%=1
	while c%<10
		A.nam$=chr$(c%+64)
		A.age%=c%
		append
		c%=c%+1
	ENDWH

	A.age%=500
	append

	te%=4
	val%=0
	exp%=0
	test:

	close

	k&=dbnewkey&:
	dbaddfieldtrunc:(k&,"name",0,100)
	dbCreateIndex:("indexa",k&,kpath$+"DbOPX.dbf","table2")

	l&=dbnewkey&:
	dbaddfield:(l&,"age",0)
	if DbIsUnique&:(l&)
		raise 1
	endif
	DbMakeUnique:(l&)
	if DbIsUnique&:(l&) <> -1
		raise 2
	endif
	DbSetcomparison:(l&,2)
	dbCreateIndex:("indexb",l&,kpath$+"DbOPX.dbf","table2")

	open kpath$+"DbOPX.dbf SELECT age FROM table2 ORDER BY age DESC",A,age%
	open kpath$+"DbOPX.dbf SELECT name FROM table2 ORDER BY name DESC",B,nam$

	use a

	te%=3
	val%=a.age%
	exp%=500
	test:

	next
	c%=1
	while c%<10
		te%=4+c%
		val%=a.age%
		exp%=10-c%
		test:
		c%=c%+1
		next
	ENDWH

	use b

	c%=1
	next
	while c%<10
		te%=12+c%
		val%=asc(b.nam$)
		exp%=74-c%
		test:
		c%=c%+1
		next
	ENDWH

	close
	close
	dbDropIndex:("indexa",kpath$+"DbOPX.dbf","table2")
	dbDropIndex:("indexb",kpath$+"DbOPX.dbf","table2")
	DbDeletekey:(l&)
	print"damaged: ";DbIsDamaged&:(kpath$+"DbOPX.dbf")
	DbRecover:(kpath$+"DbOPX.dbf")
	
	co&=0
	co&=dbgetfieldcount&:(kpath$+"DbOPX.dbf","table2")
	print"the database used has",co&,"fields"

	while co&<>0
	print"field",co&,"Name=", dbgetfieldname$:(kpath$+"DbOPX.dbf","table2",co&),"type=", dbgetfieldtype&:(kpath$+"DbOPX.dbf","table2",co&)
	co&=co&-1
	endwh
	print"the database used has",co&,"fields"

	print"complete"
	rem get
endp


proc test:
	if exp%<>val%
		raise 1000
		print "error found in test:", te%
		print val%," instead of ",exp%
		print " press a key to continue"
		rem get
	endif
endp


proc tOpxDbasNum:
	trap close
	trap delete kpath$+"DbOPXnum.dbf"
	create kpath$+"DbOPXnum.dbf FIELDS n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32 TO table2", A, n1%,n2%,n3%,n4%,n5%,n6%,n7%,n8%,n9%,n10%,n11%,n12%,n13%,n14%,n15%,n16%,n17%,n18%,n19%,n20%,n21%,n22%,n23%,n24%,n25%,n26%,n27%,n28%,n29%,n30%,n31%,n32%
	a.n32%=32
	append
	first
	print a.n32%
	close
endp


proc tOpxDbasStr:
	trap close
	trap delete kpath$+"DbOPXstr.dbf"
rem	create kpath$+"DbOPXstr.dbf FIELDS s1(1),s2(1),s3(1),s4(1),s5(1),s6(1),s7(1),s8(1),s9(1),s10(1),s11(1),s12(1),s13(1),s14(1),s15(1),s16(1) TO table2", A, s1$,s2$,s3$,s4$,s5$,s6$,s7$,s8$,s9$,s10$,s11$,s12$,s13$,s14$,s15$,s16$
	create kpath$+"DbOPXstr.dbf FIELDS s1(1),s2(1),s3(1),s4(1),s5(1),s6(1),s7(1),s8(1),s9(1),s10(1),s11(1),s12(1),s13(1),s14(1),s15(1),s16(1),s17(1),s18(1),s19(1),s20(1),s21(1),s22(1),s23(1),s24(1),s25(1),s26(1),s27(1),s28(1),s29(1),s30(1),s31(1),s32(1) TO table2", A, s1$,s2$,s3$,s4$,s5$,s6$,s7$,s8$,s9$,s10$,s11$,s12$,s13$,s14$,s15$,s16$,s17$,s18$,s19$,s20$,s21$,s22$,s23$,s24$,s25$,s26$,s27$,s28$,s29$,s30$,s31$,s32$
	a.s16$="s"
	append
	first
	print a.s16$
	close
endp


REM End of pOpxDbas.tpl
