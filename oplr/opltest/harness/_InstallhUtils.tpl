REM _InstallhUtils.tpl
REM Simple harness installer.

INCLUDE "Const.oph"

CONST KHarnOpo$="\Opltest\harness\hUtils.opo"
CONST KHarnOph$="\Opltest\harness\hUtils.oph"

CONST KSysOpo$="\System\Opl\hUtils.opo"
CONST KSysOph$="\System\Opl\hUtils.oph"

PROC Main:
	LOCAL diskName$(2)
	REM Current disk name taken from current OPL doc (.opo)
	diskName$=CurrentDisk$:
	
	TRAP MKDIR diskName$+"\System\Opl"	
	TRAP DELETE diskName$+KSysOpo$
	TRAP DELETE diskName$+KSysOph$

	IF NOT EXIST (diskName$+KHarnOph$)
		dINIT "Error"
		dTEXT "",diskName$+KHarnOph$+" does not exist"
	
		REM Have a quick peek for t'other.
		IF NOT EXIST (diskName$+KHarnOpo$)
			dTEXT "",diskName$+KHarnOpo$+" doesn't exist either."
		ENDIF
		dTEXT "","Installer will stop."
		DIALOG :STOP
	ENDIF

	COPY diskName$+KHarnOph$, diskName$+KSysOph$
	
	IF NOT EXIST (diskName$+KHarnOpo$)
		dINIT "Error"
		dTEXT "",diskName$+KHarnOpo$+" does not exist"
		dTEXT "","Installer will stop."
		DIALOG :STOP
	ENDIF
	COPY diskName$+KHarnOpo$, diskName$+KSysOpo$
	GIPRINT "Opl test harness installed."
	PAUSE 10
ENDP


PROC CurrentDisk$:
	LOCAL off%(6),p$(255)
	p$=PARSE$(CMD$(1),"",off%())
	RETURN LEFT$(p$,2)
ENDP
