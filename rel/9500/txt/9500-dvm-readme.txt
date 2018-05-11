OPL mini developer package v1.56 for Nokia 9300/9500 Communicator Series.
For more info, including porting documents,
see http://opl-dev.sourceforge.net/

Contents
--------
This package contains the following files and folders:

readme.txt         This file
\SisFiles\*.*      The OPL installation files for the target device
\ExampleSrc\*.*    OPL example source code

Note: this package only contains the necessary files to do OPL development directly on the device.
If you wish to do OPL development on the PC, you need the complete developer package.

Nokia 9300/9500 Series Communicator installation
------------------------------------------------
If your Communicator already contains an earlier release of OPL (before v1.55), you will need to 
remove it first. Because of issues with the font file supplied, this requires some extra steps. 
Before using the Application manager applet in Control Panel, you need to make sure the font file used by OPL 
(\System\Fonts\Eon14.gdr) is not in use anymore. For this reason, you need to close all OPL applications
before attempting to uninstall the previous version of the OPL runtime.
If you previously installed the Alpha release of the OPL runtime, you need to perform an additional step. If 
you installed the alpha release on your MMC card (D:\ drive) as recommended, you should remove the MMC from 
your Communicator and then reboot the phone WITHOUT the MMC in it. When fully booted, re-insert the MMC and 
use File Manager to delete D:\System\Fonts\Eon14.gdr. Once this is done, you can proceed with an uninstall 
in the normal way.

To install the OPL runtime in order to allow you to run OPL applications you should connect your Nokia 9300/9500 
Series Communicator to your PC and install \SisFiles\Other\OPL_S80.sis in the normal way as described in the 
manual. If you wish to verify your installation you can optionally install 
\SisFiles\Other\DemoOPL_S80.sis which will allow you to test OPL is properly installed.

To develop on the Nokia 9300/9500 Series Communicator itself, additionally:

1) Install \SisFiles\Other\OPLDev_S80.sis
2) Install any of the OPX files in \SisFiles\OPX\ which you require
3) Copy the files in \ExampleSrc\System\OPL\ to a \System\OPL\ folder on either drive of your Communicator
4) You can also optionally copy \ExampleSrc\DemoOPL\ to any folder and \ExampleSrc\System\Apps\DemoOPL\
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