PROC Main:
	local ev&(16)
	while 1
		getevent32 ev&()
		print HEX$(ev&(1)), HEX$(ev&(2)),ev&(3),ev&(4),
		IF ev&(1)=27
			break
		ENDIF
		
		IF ev&(1)=&406 :print "Keydown"
		ELSEIF ev&(1)=&407 :print "Keyup"
		ELSE PRINT
		ENDIF
	endwh	
	dinit "Reader has completed" :dialog
ENDP
