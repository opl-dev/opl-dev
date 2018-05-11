proc wMain:
   OplWT:(1)
endp

proc OplWT:(doGet%)
  global chrW%,chrH%,screenX%,screenY%
  global marginX%,marginY%,initw%,inith%,font&
  local info%(10)

  initW%=gWidth :initH%=gHeight
  trap loadm "g3bplus"
  if err=0
		scrInfoX:(addr(info%()))
		unloadm "g3bplus"
  endif
  marginX%=info%(1) :marginY%=info%(2)
  screenX%=info%(3) :screenY%=info%(4)
  font&=int(info%(9))+(&2**&10)*int(info%(10))
  chrW%=info%(7) :chrH%=info%(8)
  initW%=gWidth :initH%=gHeight

	do:("wDLG")
	do:("wDPOS")
	do:("wCMD")
	do:("wMENU")
	do:("wALERT")
	do:("wDFLOAT")
	setDoc "wMain"
	do:("wDTEXT")
	do:("wDLONG")
	do:("wBUSY")
	do:("wDLGERR")
	do:("wDCHOICE")
	do:("wDFILE")
	cls
	do:("wDTIME")
	do:("wDDATE")
	rem Rick. New in here.
	do:("wDIR")
	print "Finished WMAIN"
	beep 5,100
	pause -100 :key
  gSetWin 0,0,initW%,initH%
endp

proc do:(test$)
	local i%

	i%=2
	while i%<=8
		trap gClose i%
		i%=i%+1
	endwh
	gSetwin 0,0,initW%,initH%
	screen screenX%,screenY%,1,1
	gFont font&
	gStyle 0
	gGMode 0
	gTMode 0
	cls
	gCls
	gUpdate on
	gUpdate
	loadm test$
	@(test$):
	unloadm test$
endp

