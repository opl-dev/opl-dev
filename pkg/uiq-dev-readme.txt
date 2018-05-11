OPL developer package v1.54 for UIQ
For more info, including porting documents, see http://opl-dev.sourceforge.net/

Contents
--------
This package contains the following files and folders:

readme.txt         This file
\Binaries\*.*      The OPL binary files for both the emulator and target device
\Source\*.*        OPL example source code

Note: the target files should be installed on a UIQ phone. 

PC/SDK Installation
-------------------
1) Ensure you have a Symbian OS v7.0 UIQ 2.1 SDK 
   (available from http://www.sonyericsson.com/developer/site/global/home/p_home.jsp)
   installed on your PC.
2) Unzip the epoc32 folder onto the \Symbian\UIQ_21\epoc32 folder.

Phone installation
------------------
To develop on a UIQ phone itself:

1) Install the OPL-UIQ-xxx.SIS, OPX-UIQ-xxx.SIS and Texted-UIQ-xxx.SIS onto your UIQ phone
   (where xxx is the current release of OPL e.g. 050).

Known Issues
============
1. There's no OPLTRAN for UIQ yet. (OPLTRAN is a WINC tool, and the UIQ SDK doesn't support
   WINC). One solution is to use OPLTRAN from another SDK installation e.g. Nokia 9200. 
   Lack of OPLTRAN impacts the automatic building of DemoOPL, hence this example app isn't
   part of the UIQ developer package right now.
2. The Jog Dial push key is used to activate OPL menus.

<ends>
