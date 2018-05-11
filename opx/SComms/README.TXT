// README.TXT
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

SCOMMS.OPX
==========
This OPX allows EPOC OPL developers to make use of the IrDA IRCOMM protocol.  
This is the protocol used by IrDA equipped mobile phones with data modems.

The previous IrDA functions in SYSTEM.OPX gave TinyTP functionality, which
limited it's use to other EPOC devices and IrDA printers.  

This release includes an update of the IrDA stack for increased reliability.

Copyright
---------
SComms.opx is Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.  It 
forms part of the OPL SDK and is subject to the License contained therein.

Distribution 
------------ 
SComms OPX should only be distributed to end users in one of the SIS files 
included in this release.  This ensures that older versions will not be 
installed over newer versions without warning.

The best way to do this is to include the SComms SIS within your applications 
SIS.  This means you can distribute your application as a single SIS file, and 
be sure that SComms and the IrDA update will only be upgraded if necessary.

For MARM distributions use this line in your PKG file:  
@"SComms.SIS",(0x100038F2)

For WINS distributions use this line: 
@"SCommsW.SIS",(0x100038F2)

TSCOMMS.OPL
-----------
This program tests and demos much of the functionality of SCOMMS OPX.  Use it 
against another ER5 device running Comms (or the emulator if you have an IrDA adapter 
for the PC which runs from COM1.)

Ensure the remote link is switched off on both machines, and set-up Comms to 
run with "InfraRed" rather than "Serial port 0".  Make sure you have the IrDA 
update on both machines.

Contents of ZIP
---------------
The ZIP file contains the following files

readme.txt  - This file
tscomms.tpl - OPL test and demo program (text format)
e32err.tph  - Lists the error codes that E32 sets status words to when using
	          asynchronous functions (text format)
scomms.txh  - OPX header file (text format)
scomms.sis  - SIS file containing scomms.opx and IrDA update (MARM) 
scommsw.sis - SIS file containing scomms.opx and IrDA update (WINS)
