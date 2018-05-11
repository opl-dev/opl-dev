rem tCatz.tpl
rem Crystal MediaServer OPX defect reported by FatCatz.
rem

DECLARE EXTERNAL
INCLUDE "MediaServerOpx.oxh"

proc sound:
	GLOBAL gBgStatus&,sound$(255)
	sound$="c:\documents\tmediaserver\tplay.wav"
	CreateFilePlayerSimpleA:(Sound$,gbgstatus&)
	iowaitstat32 gbgstatus&
	filesetvolume:(200)
	playfileA:(gbgstatus&)
	print "sound playing"
	print "Don't press a key until you see the word END"
	pause -100 rem this is the killer
	print "END"
	get
	print "You never see this. The runtime hangs on the GET"
endp
