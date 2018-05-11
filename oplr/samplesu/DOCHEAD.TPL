REM Displays the UIDs in a document's header
REM Copyright (c) 1997-1998 Symbian Ltd. All rights reserved.

DECLARE EXTERNAL
CONST KIoOpenModeOpen%=$0000
CONST KIoOpenFormatBinary%=$0000

EXTERNAL readUids:(file$)

PROC main:
    LOCAL f$(255)
    WHILE 1
        dINIT "Show UIDs in document header"
            dPOSITION 1,0
            dFile f$,"Document,Folder,Drive",0
        IF DIALOG=0
            RETURN
        ENDIF
        readUids:(f$)
    ENDWH
ENDP

PROC readUids:(file$)
    LOCAL ret%,h%
    LOCAL uid&(4),i%

    ret%=IOOPEN(h%,file$, KIoOpenModeOpen% OR KIoOpenFormatBinary%)
    IF ret%>=0
        ret%=IOREAD(h%,ADDR(uid&()),16)
        PRINT "Reading ";file$
        IF ret%=16
            WHILE i%<4
                i%=i%+1
                print "  Uid"+num$(i%,1)+"=",hex$(uid&(i%))
            ENDWH
        ELSE
            PRINT "  Error reading: ";
            IF ret%<0
                PRINT err$(ret%)
            ELSE
                PRINT "Read ";ret%;" bytes only (4 long integers required)"
            ENDIF
        ENDIF
        IOCLOSE(h%)
    ELSE
        PRINT "Error opening: ";ERR$(ret%)
    ENDIF
ENDP
