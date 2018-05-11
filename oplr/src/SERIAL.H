// SERIAL.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef SERIAL_H
#define SERIAL_H

/* Serial port characteristics */
typedef struct
    {
    TUint8 tbaud; /* transmit Baud rate selector */
    TUint8 rbaud; /* receive Baud rate selector */
    TUint8 frame; /* number of data, parity and stop bits */
    TUint8 parity; /* parity selector */
    TUint8 hand; /* handshake flags */
    TUint8 xon; /* XON character */
    TUint8 xoff; /* XOFF character */
    TUint8 flags; /* ignore parity errors/dont drive DTR changing chars */
    TUint32 tmask; /* terminator mask */
    } P_SRCHAR;

/* Baud rates */
#define P_BAUD_50    0x01
#define P_BAUD_75    0x02
#define P_BAUD_110   0x03
#define P_BAUD_134   0x04
#define P_BAUD_150   0x05
#define P_BAUD_300   0x06
#define P_BAUD_600   0x07
#define P_BAUD_1200  0x08
#define P_BAUD_1800  0x09
#define P_BAUD_2000  0x0A
#define P_BAUD_2400  0x0B
#define P_BAUD_3600  0x0C
#define P_BAUD_4800  0x0D
#define P_BAUD_7200  0x0E
#define P_BAUD_9600  0x0F
#define P_BAUD_19200 0x10
#define P_BAUD_38400 0x11
#define P_BAUD_56000 0x12
#define P_BAUD_115000 0x13


/* allocation of frame bits */
#define P_DATA_MASK 0x03 /* number of data bits mask */
#define P_DATA_5 0  /* 5 data bits */
#define P_DATA_6 1  /* 6 data bits */
#define P_DATA_7 2  /* 7 data bits */
#define P_DATA_8 3  /* 8 data bits */
#define P_TWOSTOP 0x10 /* 2 stop bits if set, 1 if clear */
#define P_PARITY 0x20 /* 1 parity bit if set, 0 if clear */

/* parity - ignored unless P_PARITY is set */
#define P_PAR_EVEN 1 /* even parity */
#define P_PAR_ODD 2 /* odd parity */
#define P_PAR_MARK 3 /* mark parity */
#define P_PAR_SPACE 4 /* space parity */

/* handshaking control */
#define P_OBEY_XOFF 0x01 /* respond to received XOFF (and XON) if set */
#define P_SEND_XOFF 0x02 /* send XOFF/XON to control receive buffer if set */
#define P_IGN_CTS 0x04 /* ignore the state of CTS if set */
#define P_OBEY_DSR 0x08 /* obey the state of DSR if set */
#define P_FAIL_DSR 0x10 /* fail if DSR goes OFF if set */
#define P_OBEY_DCD 0x20 /* obey the state of DCD if set */
#define P_FAIL_DCD 0x40 /* fail if DCD goes OFF if set */

/* flags control */
#define P_IGNORE_PARITY 0x01    /* ignore parity errors */

/* For P_FCTRL function */
#define P_SRCTRL_CTS 0x01
#define P_SRCTRL_DSR 0x02
#define P_SRCTRL_DCD 0x04
#define P_SRCTRL_RTS 0x08
#define P_SRCTRL_DTR 0x10

#define P_SRDTR_ON 1 /* to set DTR to MARK */
#define P_SRDTR_OFF 2 /* to set DTR to SPACE */

/* Bit masks for P_FINQ function */
#define P_SRINQ_50    0x0001
#define P_SRINQ_75    0x0002
#define P_SRINQ_110   0x0004
#define P_SRINQ_134   0x0008
#define P_SRINQ_150   0x0010
#define P_SRINQ_300   0x0020
#define P_SRINQ_600   0x0040
#define P_SRINQ_1200  0x0080
#define P_SRINQ_1800  0x0100
#define P_SRINQ_2000  0x0200
#define P_SRINQ_2400  0x0400
#define P_SRINQ_3600  0x0800
#define P_SRINQ_4800  0x1000
#define P_SRINQ_7200  0x2000
#define P_SRINQ_9600  0x4000
#define P_SRINQ_19200 0x8000
/* second baud rate word */
#define P_SRINQ_38400 0x0001
#define P_SRINQ_56000 0x0002
/* 2nd set of info */

#define P_SRINQ_DATA5 0x0001 /* supports 5 data bits */
#define P_SRINQ_DATA6 0x0002 /* supports 6 data bits */
#define P_SRINQ_DATA7 0x0004 /* supports 7 data bits */
#define P_SRINQ_DATA8 0x0008 /* supports 8 data bits */
#define P_SRINQ_STOP2 0x0010 /* supports 2 stop bits (as well as 1) */
#define P_SRINQ_PAREVEN 0x0020 /* supports even parity */
#define P_SRINQ_PARODD 0x0040 /* supports odd parity */
#define P_SRINQ_PARMARK 0x0080 /* supports mark parity */
#define P_SRINQ_PARSPACE 0x0100 /* supports space parity */
#define P_SRINQ_SETDTR 0x0200 /* can set DTR */
#define P_SRINQ_SPLIT 0x400 /* supports split Baud rates */
#define P_SRINQ_XONXOFF 0x800 /* Supports soft xon/xoff characters */


// additional function numbers for eXtended serial
#define FXSET	107
#define FXSENSE	108
#define FXCTRL	111
#define FXINQ	112

#endif