// IODEF.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef IODEF_H
#define IODEF_H

/* open service mode flags */
#define FOPEN 0x0000 /* Open i/o service */
#define FCREATE 0x0001 /* Create file */
#define FREPLACE 0x0002 /* Replace file */
#define FAPPEND 0x0003 /* Append records */
#define FUNIQUE 0x0004 /* Unique file open */
#define FSER_MASK 0x000f /* File service mask */

#define FSTREAM 0x0000 /* Stream access to a binary file */
#define FSTREAM_TEXT 0x0010 /* Stream access to a text file */
#define FTEXT 0x0020 /* Record access to a text file */
#define FDIR 0x0030 /* Record access to a directory file */
#define FFORMAT 0x0040 /* Format a device */
#define FDEVICE 0x0050 /* Record access to device name list */
#define FNODE 0x0060 /* Record access to node name list */
#define FMT_MASK 0x00f0 /* File format mask */

#define FUPDATE 0x0100 /* Read and write access */
#define FRANDOM 0x0200 /* Random access */
#define FSHARE 0x0400 /* File can be shared */
#define FACC_MASK 0x0f00 /* mask for access types */

/* io/iow function constants */
/* Generic I/O functions */
#define FPANIC 0 /* Panic the device */
#define FREAD 1 /* Read */
#define FWRITE 2 /* Write */
#define FCLOSE 3 /* Close */
#define FCANCEL 4 /* Cancel io */
#define FATTACH 5 /* attach this pseudo device */
#define FDETACH 6 /* detach this pseudo device */
#define FSET 7 /* set channel characteristics */
#define FSENSE 8 /* sense channel characteristics */
#define FFLUSH 9 /* Flush buffers */

/* Now follows the device specific functions */
/* Files */
#define FSEEK 10 /* Seek */
#define FSETEOF 11 /* Set end of file */

/* Timer */
#define FRELATIVE 1 /* Relative timer */
#define FABSOLUTE 2 /* Absolute timer */

/* Data Link I/O + wserv */
#define FCONNECT 10 /* Queue a link connect request */
#define FDISCONNECT 11 /* Disconnect the link */

/* wserv functions */
#define FWRITE_REPLY 12 /* FWRITE + read a reply from the server */

/* Serial/llmac */
#define FTEST 10 /* return no. of bytes in serial receieve buffer */
#define FCTRL 11 /* serial port control lines */
#define FINQ 12 /* inquire of device capabilities */
#define FRSUPER 13 /* read a supervisory frame (llmac) */
#define FSTOP 14 /* stop LLMAC mode */
#define FSTART 15 /* start LLMAC mode */

/* Serial LDD to Serial PDD I/O defines */
#define FENABLE 16 /* Enable transmit interrupts */
#define FSETINT 17 /* set the serial Interrupt code ptrs */

/* modem functions */
#define FMINIT 16 /* modem initialization function */
#define FMDIAL 17 /* modem dial function */
#define FMWAITCALL 18 /* wait for an incoming call */
#define FMSENSE 19 /* Sense Modem characteristics */
#define FMSET 20 /* Set Modem characteristics */
#define FMCANCEL 21 /* Cancel outstanding mode request */

/* ioseek() mode constants */
#define FABS 1 /* Position absolute in the file */
#define FEND 2 /* Position relative from the end of file */
#define FCUR 3 /* Position relative to the current pos */
#define FRSENSE 4 /* Sense the record position */
#define FRSET 5 /* Set the record position */
#define FREWIND 6 /* Rewind a text file */


/* defines for changing directories */
#define CD_ROOT 0     /* goto root directory */
#define CD_PARENT 1   /* goto parent directory */
#define CD_SUBDIR 2   /* goto subdirectory */

/* masks for status flag of INFO */
#define FAWRITE  0x01 /* can the file be written to? */
#define FAHIDDEN 0x02 /* set if file is hidden */
#define FASYSTEM 0x04 /* set if  file is a system file */
#define FAVOLUME 0x08 /* set if the name is a volume name */
#define FADIR    0x10 /* set if file is a directory file */
#define FAMOD    0x20 /* has the file been modified? */
#define FAREAD   0x100 /* can the file be read? */
#define FAEXEC   0x200 /* is the file executable? */
#define FASTREAM 0x400 /* is the file a byte stream file? */
#define FATEXT   0x800 /* is it a text file? */

#define FMEDIA_COMPRESSIBLE 0x8000 /* Worth compressing deleted records */
#define FMEDIA_DYNAMIC 0x4000 /* Media size is dynamic */
#define FMEDIA_INTERNAL 0x2000 /* Media is internal */
#define FMEDIA_DUAL_DENSITY 0x1000 /* media has dual density */
#define FMEDIA_FORMATTABLE 0x800 /* media is formattable  */

#define VOLUMENAME    32

/* Media Constants */

#define FMEDIA_UNKNOWN 0
#define FMEDIA_FLOPPY 1
#define FMEDIA_HARDDISK 2
#define FMEDIA_FLASH 3
#define FMEDIA_RAM 4
#define FMEDIA_ROM 5
#define FMEDIA_WRITEPROTECTED 6


#define FSYSTYPE_FLAT 0
#define FSYSTYPE_HIER 1

#endif

/* End of $Workfile:   IODEF.H  $ */
