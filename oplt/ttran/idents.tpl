REM					IDENTS.OPL
REM
REM Opl test module containing all kinds of identifier references and
REM expressions
REM	This will not run.
	

PROC test1:(arg1%,arg2&,arg3,arg4$)
	LOCAL local1%,local2&,local3,local4$(255)
	LOCAL array1%(2),array2&(2),array3(2),array4$(2,255)
	GLOBAL global1%,global2&,global3,global4$(255)
	REM   ------- IDENTIFIERS AND ASSIGNMENTS --------
	REM left side references
	local1%=$1:local2&=&1:local3=1.0:local4$="ONE"
	array1%(1)=$2:array2&(1)=&2:array3(1)=2.0:array4$(1)="TWO"
	global1%=$3:global2&=&3:global3=3.0:global4$="THREE"
	extern1%=$4:extern2&=&4:extern3=4.0:extern4$="FOUR"
	a.a%=$5:a.a&=&5:a.a=5.0:a.a$="FIVE"

	REM right side references
	local1%=arg1% :local2&=arg2& :local3=arg3 :local4$=arg4$
	array1%(1)=local1% :array2&(1)=local2& :array3(1)=local3 :array4$(1)=local4$
	global1%=array1%(1):global2&=array2&(1):global3=array3(1):global4$=array4$(1)
	extern1%=global1% :extern2&=global2& :extern3=global3 :extern4$=global4$
	local1%=b.a% :local2&=b.a& :local3=b.a :local4$=b.a$
	local1%=local1% :local2&=local2& :local3=local3 :local4$=local4$
	alabel::
ENDP

PROC consts%:
	LOCAL local1%,local2&,local3,local4$(255)

	REM  ---------- CONSTANTS ----------
	local1%=0:local1%=1:local1%=127:local1%=128:local1%=32767
	local1%=$0:local1%=$1:local1%=$7f:local1%=$80:local1%=$7fff
	local1%=$ffff:local1%=$ff81:local1%=$ff80:local1%=$8000

	local2&=32768:local2&=2147483647
	local2&=&0:local2&=&1:local2&=&7f:local2&=&80:local2&=&7fff
	local2&=&8000:local2&=&7fffffff
	local2&=&ffffffff:local2&=&ffffff81:local2&=&ffffff80:local2&=&ffff8001
	local2&=&ffff8000

	local3=1.0:local3=.1:local3=32767.0:local3=32768.0:local3=2147483647.0
	local3=2147483648
	local3=1e1:local3=1e+1:local3=1e-1

	local4$=""""
	ENDP

PROC casts%:
	LOCAL local1%,local2&,local3,local4$(255)

	REM	---------- CASTS IN ASSIGNMENTS --------------
	local1%=1:local2&=1:local3=1
	local1%=&1:local2&=&1:local3=&1
	local1%=1.0:local2&=1.0:local3=1.0
ENDP

PROC ops%:
	LOCAL local1%,local2&,local3,local4$(255)

	REM All the operators - as words
	local1%=1<2:local1%=1<=2:local1%=1>2:local1%=1>=2:local1%=1=2
	local1%=1<>2:local1%=1+2:local1%=1-2:local1%=1*2:local1%=1/2
	local1%=1**1:local1%=1AND 2:local1%=1OR 2:local1%=NOT 1:local1%=-1

	REM All the operators - as longs
	local2&=&1<&2:local2&=&1<=&2:local2&=&1>&2:local2&=&1>=&2:local2&=&1=&2
	local2&=&1<>&2:local2&=&1+&2:local2&=&1-&2:local2&=&1*&2:local2&=&1/&2
	local2&=&1**&1:local2&=&1 AND &2:local2&=&1OR &2:local2&=NOT &1:local2&=-&1

	REM All the operators - as doubles
	local3=1.<2.:local3=1.<=2.:local3=1.>2.:local3=1.>=2.:local3=1.=2.
	local3=1.<>2.:local3=1.+2.:local3=1.-2.:local3=1.*2.:local3=1./2.
	local3=1.**1.:local3=1.AND 2.:local3=1.OR 2.:local3=NOT 1.:local3=-1.
	local3=1.+2.%:local3=1.-2.%:local3=1.*2.%:local3=1./2.%:local3=1.<2.%
	local3=1.>2.%

	REM String Operators
	local4$="ONE"+"TWO"
	local1%="ONE">"TWO":local1%="ONE"<"TWO":local1%="ONE">="TWO"
	local1%="ONE"="TWO":local1%="ONE">="TWO":local1%="ONE"<>"TWO"
		
	REM Casts and backpatches
	local2&=&1+1:local3=1.+1:local3=1.+&1
	local2&=1+&1:local3=1+1.:local3=&1+1.
ENDP


PROC binary%:
	LOCAL local1%,local2&,local3,local4$(255)
	
		
	REM ORDINARY BINARY PRECEDENCE
	local1%=1<2<3:local1%=1<2<=3:local1%=1<2>3:local1%=1<2>=3
	local1%=1<2=3:local1%=1<2<>3:local1%=1<2+3:local1%=1<2-3
	local1%=1<2*3:local1%=1<2/3:local1%=1<2**3:local1%=1<2 AND 3
	local1%=1<2 OR 3

	local1%=1<=2<3:local1%=1<=2<=3:local1%=1<=2>3:local1%=1<=2>=3
	local1%=1<=2=3:local1%=1<=2<>3:local1%=1<=2+3:local1%=1<=2-3
	local1%=1<=2*3:local1%=1<=2/3:local1%=1<=2**3:local1%=1<=2 AND 3
	local1%=1<=2 OR 3

	local1%=1>2<3:local1%=1>2<=3:local1%=1>2>3:local1%=1>2>=3
	local1%=1>2=3:local1%=1>2<>3:local1%=1>2+3:local1%=1>2-3
	local1%=1>2*3:local1%=1>2/3:local1%=1>2**3:local1%=1>2 AND 3
	local1%=1>2 OR 3

	local1%=1>=2<3:local1%=1>=2<=3:local1%=1>=2>3:local1%=1>=2>=3
	local1%=1>=2=3:local1%=1>=2<>3:local1%=1>=2+3:local1%=1>=2-3
	local1%=1>=2*3:local1%=1>=2/3:local1%=1>=2**3:local1%=1>=2 AND 3
	local1%=1>=2 OR 3

	local1%=1=2<3:local1%=1=2<=3:local1%=1=2>3:local1%=1=2>=3
	local1%=1=2=3:local1%=1=2<>3:local1%=1=2+3:local1%=1=2-3
	local1%=1=2*3:local1%=1=2/3:local1%=1=2**3:local1%=1=2 AND 3
	local1%=1=2 OR 3

	local1%=1<>2<3:local1%=1<>2<=3:local1%=1<>2>3:local1%=1<>2>=3
	local1%=1<>2=3:local1%=1<>2<>3:local1%=1<>2+3:local1%=1<>2-3
	local1%=1<>2*3:local1%=1<>2/3:local1%=1<>2**3:local1%=1<>2 AND 3
	local1%=1<>2 OR 3

	local1%=1+2<3:local1%=1+2<=3:local1%=1+2>3:local1%=1+2>=3
	local1%=1+2=3:local1%=1+2<>3:local1%=1+2+3:local1%=1+2-3
	local1%=1+2*3:local1%=1+2/3:local1%=1+2**3:local1%=1+2 AND 3
	local1%=1+2 OR 3

	local1%=1-2<3:local1%=1-2<=3:local1%=1-2>3:local1%=1-2>=3
	local1%=1-2=3:local1%=1-2<>3:local1%=1-2+3:local1%=1-2-3
	local1%=1-2*3:local1%=1-2/3:local1%=1-2**3:local1%=1-2 AND 3
	local1%=1-2 OR 3

	local1%=1*2<3:local1%=1*2<=3:local1%=1*2>3:local1%=1*2>=3
	local1%=1*2=3:local1%=1*2<>3:local1%=1*2+3:local1%=1*2-3
	local1%=1*2*3:local1%=1*2/3:local1%=1*2**3:local1%=1*2 AND 3
	local1%=1*2 OR 3

	local1%=1/2<3:local1%=1/2<=3:local1%=1/2>3:local1%=1/2>=3
	local1%=1/2=3:local1%=1/2<>3:local1%=1/2+3:local1%=1/2-3
	local1%=1/2*3:local1%=1/2/3:local1%=1/2**3:local1%=1/2 AND 3
	local1%=1/2 OR 3

	local1%=1**2<3:local1%=1**2<=3:local1%=1**2>3:local1%=1**2>=3
	local1%=1**2=3:local1%=1**2<>3:local1%=1**2+3:local1%=1**2-3
	local1%=1**2*3:local1%=1**2/3:local1%=1**2**3:local1%=1**2 AND 3
	local1%=1**2 OR 3

	local1%=1 AND 2<3:local1%=1 AND 2<=3:local1%=1 AND 2>3:local1%=1 AND 2>=3
	local1%=1 AND 2=3:local1%=1 AND 2<>3:local1%=1 AND 2+3:local1%=1 AND 2-3
	local1%=1 AND 2*3:local1%=1 AND 2/3:local1%=1 AND 2**3:local1%=1 AND 2 AND 3
	local1%=1 AND 2 OR 3

	local1%=1 OR 2<3:local1%=1 OR 2<=3:local1%=1 OR 2>3:local1%=1 OR 2>=3
	local1%=1 OR 2=3:local1%=1 OR 2<>3:local1%=1 OR 2+3:local1%=1 OR 2-3
	local1%=1 OR 2*3:local1%=1 OR 2/3:local1%=1 OR 2**3:local1%=1 OR 2 AND 3
	local1%=1 OR 2 OR 3
ENDP

PROC rest%:
	LOCAL local1%,local2&,local3,local4$(255)

	REM PRECEDENCE ON THE REST
	local1%=1<-2:local1%=1<NOT 2:local1%=1+-2:local1%=1+NOT 2
	local1%=1*-2:local1%=1*NOT 2:local1%=1**-2:local1%=1**NOT 2
	local1%=-1<2:local1%=NOT 1<2:local1%=-1+2:local1%=NOT 1+2
	local1%=-1*2:local1%=NOT 1*2:local1%=-1**2:local1%=-1**2
	local1%=-NOT 1:local1%=NOT-1

	local3=1 AND 2<3%:local3=1<2<3%:local3=1+2<3%:local3=1*2<3%
	local3=1**2<3%
	local3=1<2%AND 3:local3=1<2%<3:local3=1<2%+3:local3=1<2%*3
	local3=1<2%**3
ENDP

PROC parenth%:
	LOCAL local1%,local2&,local3,local4$(255)

	REM NOW TRY OVERIDING THE PRECENDENCE

	local1%=(1<2)+3:local1%=(1+2)*3:local1%=(1*2)**3
	local1%=1+(2<3)%
ENDP

PROC mad%:
	LOCAL local1%,local2&,local3,local4$(255)

	REM AND FINALLY JUST FOR A LAUGH
	local3=%%<%%+%%*%%%%%
	local3=%%>%%-%%/%%%%%
ENDP

PROC procs%:
REM Procedure Calls

proc1%:
proc2&:
proc3:
proc4$:

print proc5%:(1,&2,3.0,"fred")+proc6&:(1,2)
print proc6&:(1,2)+proc7:(23)
print proc7:(proc5%:(proc1%:,proc2&:,proc3:,proc4$:)+proc1%:)
ENDP

REM That's all folks!