OPL runtime v1.56 for Nokia 9300/9500 Communicator Series
See http://opl-dev.sourceforge.net/ for more info.

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
Series Communicator to your PC and install OPL_S80.sis in the normal way as described in the manual. If you wish 
to verify your installation you can optionally install DemoOPL_S80.sis which will allow you to test OPL is 
properly installed.

