OPL developer package v1.56 for Nokia 9200 Communicator Series.
For more info, including porting documents,
see http://opl-dev.sourceforge.net/

Contents
--------
This package contains the following files and folders:

readme.txt         This file
\Binaries\*.*      The OPL binary files for both the emulator and target device
\ExampleSrc\*.*    OPL example source code

Note: the target files should be installed on a Nokia 9200 Series Communicator. 

PC/SDK Installation
-------------------
1) Ensure you have a Nokia 9200 Series C++ SDK (available from http://www.forum.nokia.com/) installed on your PC.
2) From this C++ SDK installation, delete the following folders and their contents (if present) 
     from the \NokiaCPP\ folder tree (not the \Shared\ one):

\epoc32\release\wins\udeb\z\system\opx\
\epoc32\release\wins\urel\z\system\opx\

3) On top of the \epoc32\Tools\ tree of your SDK installation inside \Shared\, copy the contents of 
     \Binaries\PC\Tools\
4) On top of the \epoc32\ tree of your SDK installation inside \NokiaCPP\, copy the contents of 
     \Binaries\PC\epoc32\

The above steps ensure the OPL runtime, tools and OPX files are installed over your SDK. These files do not come
inside SIS packages for installation under the emulator (there are, however, 'stub' SIS files included in the 
emulated Z:\System\Install\ folder to allow proper version control to take place).

5) Although the OPXs are installed from the \epoc32\ tree in step 4 above, SIS files for all WINS variants of 
the OPXs are also included here for the sake of completeness and to allow developers to produce SIS files of 
their own applications for WINS if they wish to. It is recommended that you therefore copy the files located in 
\Binaries\SisFiles\PC\OPX\udeb\ into \NokiaCPP\epoc32\release\wins\udeb\ and the files located in 
\Binaries\SisFiles\PC\OPX\urel\ into \NokiaCPP\epoc32\release\wins\urel\ on your SDK installation.

This will ensure the WINS OPX SIS files are in the recommended location on your SDK. You can then code your 
PKG file to look in \NokiaCPP\epoc32\release\wins\<variant>\ for the OPX SIS files you need to include in your 
own WINS distribution.

6) The DemoOPL application can also be installed under WINS. Decide which variant of WINS you wish to use 
(release or debug) and then copy the appropriate SIS file from \Binaries\SisFiles\PC\Other\<variant>\ onto 
the emulated C:\ drive of the emulator (usually <EPOCROOT>\epoc32\wins\c\). You can then install DemoOPL the 
next time you run WINS.
7) You can also optionally copy \ExampleSrc\Target\DemoOPL\ to the emulated C:\ drive if you wish to play 
with/re-translate the DemoOPL program under WINS (the 'Target' in the folder name refers to the fact the 
contents are in the correct format for the target device being emulated by WINS, i.e. all code files can be 
opened by the OPL Editor application rather than being in plain text format for viewing under Windows on a PC).
8) Create a \NokiaCPP\epoc32\release\thumb\urel\ folder on your SDK installation. In to this copy the files in 
\Binaries\SisFiles\Target\OPX\. This is the recommended location for these files (for example, it's where the 
PKG file for DemoOPL expects the OPX SIS files it depends on to be). When building your own SIS files for 
target devices, this is the location you specify when including OPX SIS files in your own package.
9) You can also optionally copy the contents of \ExampleSrc\PC\ onto your SDK installation tree. This is source 
and example code in plain text format for easy reading under Windows and, in the case of DemoOPL, translation 
using OPLTRAN if you so desire.

NOTE 1: If you are ONLY using the SDK installation for OPL development then you can remove some redundant 
files needed for C++ developers in order to speed up the emulator in the following OPTIONAL steps:

From the location your SDK is installed in ONLY, delete (or in the case of DLL files move to another location):

\epoc32\*.pdb;*.ilk;*.idb;*.bsc;*.org;*.log;*.bak;*.lib
\epoc32\release\wins\udeb\z\system\libs\lindastart.dll
\epoc32\release\wins\urel\z\system\libs\lindastart.dll

You can also delete all the files in \epoc32\include\ EXCEPT for the following (which are needed for resource 
compilation):

ckon.hrh
EIKCDLG.RSG
EIKCOCTL.RSG
eikcolor.hrh
EIKCORE.RSG
EIKCTL.RSG
EIKDLG.RSG
EIKFILE.RSG
EIKMISC.RSG
eikon.hrh
EIKON.mbg
eikon.rh
eikon.rsg
EIKPRINT.RSG
gulftflg.hrh
uikon.hrh
uikon.rh

Finally, if you wish to save more disk space, you can remove \epoc32\release\armi\ and \epoc32\release\arm4\

NOTE 2: If you run OPL on early versions of the Nokia SDK and see a crash/panic/error, you may need to patch 
one of your DLLs. On the http://opl-dev.sourceforge.net website, you can download a zip file called 
"CONE Patch (WINS).zip". Extract the contents of this file (with the restore folders option enabled) on top 
of your SDK installation. This will overwrite CONE.DLL and related files with a patched version which should 
cure these errors.

Nokia 9200 Series Communicator installation
-------------------------------------------
If your Communicator already contains an earlier release of OPL (before v1.55), you will need to 
remove it first. Because of issues with the font file supplied, this requires some extra steps. 
Before using the Add/Remove applet in Control Panel, you need to make sure the font file used by OPL 
(\System\Fonts\Eon14.gdr) is not in use anymore. For this reason, you need to close all OPL applications
before attempting to uninstall the previous version of the OPL runtime.
If you previously installed the Alpha release of the OPL runtime, you need to perform an additional step. If 
you installed the alpha release on your MMC card (D:\ drive) as recommended, you should remove the MMC from 
your Communicator and then reboot the phone WITHOUT the MMC in it. When fully booted, re-insert the MMC and 
use File Manager to delete D:\System\Fonts\Eon14.gdr. Once this is done, you can proceed with an uninstall 
in the normal way.

To install the OPL runtime in order to allow you to run OPL applications you should connect your Nokia 9200 
Series Communicator to your PC and install \Binaries\SisFiles\Target\Other\OPL.sis in the normal way as described in 
the manual. If you wish to verify your installation you can optionally install 
\Binaries\SisFiles\Target\Other\DemoOPL.sis which will allow you to test OPL is properly installed.

To develop on the Nokia 9200 Series Communicator itself, additionally:

1) Install \Binaries\SisFiles\Target\Other\OPLDev.sis
2) Install any of the OPX files in \Binaries\SisFiles\Target\OPX\ which you require
3) Copy the files in \ExampleSrc\Target\System\OPL\ to a \System\OPL\ folder on either drive of your Communicator
4) You can also optionally copy \ExampleSrc\Target\DemoOPL\ to any folder and \ExampleSrc\Target\System\Apps\DemoOPL\
 to \System\Apps\DemoOPL\ on your Communicator if you wish to play with/re-translate the DemoOPL program.

==============
=Known Issues=
==============
The following issues are currently known to affect OPL. This is the complete list of notable issues at the 
time of release, some of this information will only be of interest to OPL Developers:

1) When OPL applications are running, the loaded font file (EON14.GDR) may cause compatibility problems 
    with some C++/Java applications which use checkboxes.
2) Agenda OPX will cause a KERN-EXEC 3 crash on the target device if your program INCLUDEs it and the default 
    Calendar file is open when your application starts.
3) SyLogonToThread: in System OPX does not work properly - it never returns the completion value, leading to 
    an infinite loop.
4) The MediaServerOPX OPX is not complete - some functionality is missing in this release.

<ends>
