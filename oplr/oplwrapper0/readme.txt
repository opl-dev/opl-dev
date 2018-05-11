oplwrapper0 readme.txt
v0.05 March 04
ricka@users.sourceforge.net

This is an example of a C++ Wrapper application for Series 60 OPL.

You can create your own C++ wrapper by modifying this example, but note that 
you'll need the Series 60 OPL Developers Pack installed on top of your C++ 
installation. See the 'OPL Developers' page on http://opl-dev.sourceforge.net for 
more info.

To create your own C++ wrapper app:

0. Ensure you have the following:
	appname e.g. WebServer
	app UID e.g. 0x102FABC4 (your UID should be obtained from Symbian)
	app icon in four files:
		44x44 pixel 256 colour icon
		44x44 pixel monochrome mask
		42x29 pixel 256 colour icon  
		42x29 pixel monochrome mask
		The 256 colour icons should be about 3KB in size, and the
		monochrome masks about 400B.  

1. Replace all occurances of "OplTestS60" with your app name e.g. "WebServer".
   (Note that this include filenames and directories e.g. contents of the .pkg file)
2. Replace _OPLWRAPPERUID in oplwrapper.h with your app UID.
3. Replace the icon files with your four icon files.
4. cd ..\group
5. Run 'bldmake bldfiles'
6. Run 'abld build oplwrapper0' to build the oplwrapper0 target.
7. cd ..\oplwrapper0
8. Run 'makezip' to create a .ZIP file containing the four files that you'll need.
9. Run 'trancopy' to translate the stub .opo file, for testing the wrapper.
10. Run 'makesis _oplwrapper.pkg' to create a .SIS file for testing on phone.
11.Test the wrapper by installing the .SIS file onto your phone.
   After installing, you should see a new app in the menu app, with the correct
   captions, and when opened, the wrapper should run the stub .opo file.

The .ZIP file will contain the following files:
	appname.app
	appname.aif
	appname.rsc
	appname_caption.rsc

You should package these files, together with your .OPO file and any other files 
that your app requires, into your final .SIS file.

<ends>
