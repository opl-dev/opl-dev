DECLARE EXTERNAL
EXTERNAL TrueOrFalse$:(aBool%,_Avail&)

INCLUDE "Const.oph"
INCLUDE "SendAs.oxh"

PROC Main:
LOCAL _Body&,Chosen&,Max&,i%,Type$(1),Ret%,Available%(KMaxSendAsTypes%)
	Max&=SaMaximumTypes&:
	Top::
	PRINT "Maximum SendAs types =",Max&
	PRINT "Capability order = All/Attach/Body/BioSend/AttachOrBody"
	PRINT "(0) Exit"
	i%=1
	DO
		Available%(i%)=KFalse%
		PRINT "(";i%;")",SaCascName$:(i%)," -",
		PRINT TrueOrFalse$:(SaCapabilitySupported%:(i%,KCapabilityAllMTMs&),ADDR(Available%(i%)));"/";
		PRINT TrueOrFalse$:(SaCapabilitySupported%:(i%,KCapabilityAttachments&),ADDR(Available%(i%)));"/";
		PRINT TrueOrFalse$:(SaCapabilitySupported%:(i%,KCapabilityBodyText&),ADDR(Available%(i%)));"/";
		PRINT TrueOrFalse$:(SaCapabilitySupported%:(i%,KCapabilityBioSending&),ADDR(Available%(i%)));"/";
		PRINT TrueOrFalse$:(SaCapabilitySupported%:(i%,KCapabilityAttachmentOrBody&),ADDR(Available%(i%)))
		i%=i%+1
	UNTIL i%=Max&+1
	PRINT "Type to use:",
	INPUT Type$
	Chosen&=INT(VAL(Type$))
	IF Chosen&<0 OR Chosen&>Max&
		PRINT "Enter a value between 1 and",Max&,"please! Press any key..."
		GET
		CLS
		GOTO Top::
	ELSEIF Chosen&=0
		STOP
	ELSEIF Available%(Chosen&)<>KTrue%
		PRINT "Choice not available (all capabilities are False). Press any key..."
		GET
		CLS
		GOTO Top::
	ENDIF
	
	PRINT "Preparing body..."
	_Body&=SaNewBody&:
	PRINT "Body prepared. Preparing message..."
	SaPrepareMessage:(Chosen&)
	PRINT "SaPrepareMessage called. Setting subject..."
	Ret%=SaSetSubject%:("Subject set via OPX!")
	IF Ret%
		PRINT "SaSetSubject failed ("+ERR$(Ret%)+"). Appending to body..."
	ELSE
		PRINT "SaSetSubject called. Appending to body..."
	ENDIF
	SaAppendToBody:("Test string 1")
	SaAppendToBody:("Test string 2")
	PRINT "Some text added to the body. Setting body..."
	SaSetBody:(_Body&)
	PRINT "Body set. Launching to send..."
	SaLaunchSend:
	PRINT "Launch called! Deleting body..."
	SaDeleteBody:
	PRINT "Body deleted."
	PRINT
	PRINT "Done!"
	GET
ENDP

PROC TrueOrFalse$:(aBool%,_Avail&)
	IF aBool%=KTrue%
		POKEL _Avail&,KTrue%
		RETURN "True"
	ENDIF
	RETURN "False"
ENDP