OPL developer package for Series 60 mobile phones.
v1.30 27 November 03
ricka@users.sourceforge.net

Contents
--------
This zip file contains a patch for the Series 60 SDK which allows OPL 
development on the emulator.
It contains:

readme.txt             This file
\Binaries\pc\*.*       The OPL binary files for the emulator
\Binaries\sisfile\*.*  OPL SIS files for Series60 mobile phone.

Installing onto SDK
-------------------
This assumes you have installed a Series 60 C++ SDK on your PC.
Unzip this zip over the SDK, so that the Symbian folders align.
  That is, the contents of \Binaries\PC\Symbian are copied into 
  your D:\Symbian folder (where D is your Series 60 SDK drive).

For more info, see http://opl-dev.sourceforge.net/

Recent OPL changes
------------------

v1.30 27 November 03 ricka@users.sourceforge.net

OPLR:
* Further changes to cascade menus.
* Fixed priority key halt for Series 60 emulator, as a workround for
  OnStopping() not being called.
* Fixed defect 817867 "reset the default window" so the default OPL console is 
  full screen, with the screen furniture behind the OPL console.
* Fixed defect 844681 "OPO filename too long" by making the length for chars, 
  not bytes.

OPX:
* Released an alpha version on Contact OPX for testing with Series 60.
  No major changes to the source code, but the test code modified as Series 60
  only supports one contact db, and the original test code was better suited to
  the 9210's support for many databases.

---
v1.26a 5 October 03 ricka@users.sourceforge.net
(Not released externally)

OPLR:
* The runtime now derived from MCoeFepAwareTextEditor for FEP interaction.
  This means a new export for S60, #167.
* Also calls OfferKeyEvent() on the FEP
* Test version of GET, INPUT$ etc. Numberic mode only at the moment.
* Fixes to MENU for FEP.
  And KCommandIdOffset is 0x8000 to prevent interaction with 0x4000 used by FEP.
* Cleaned up a WINS CFrame dtor problem.
* Added a makezip.bat file for oplwrapper0 and updated its readme.txt file.
* Added menu test opl source to opltest\Series60\

---
v1.25 27 July 03 ricka@users.sourceforce.net

OPLR:
* Added the Series60 font ids to Const.tph file - see KFontS60...
  Added oplr\opltest\series60\gfontdemo61.tpl test code to exercise the consts.
* Using the default font id &9A now gives you Latin Plain 12 on Series 60 runtime.
  This fixes defect 735621.
* Adjusted the console window width to better fit a non-proportional font.
  Goes a long way to fixing 740298.
* Added oplr\oplwrapper0 - a canonical Series60 wrapper. Start with this version
  if you want to roll your own C++ app wrapper. See oplr\oplwrapper0\readme.txt.
  As it stands, Oplwrapper0 builds a C++ wrapper for OplTestS60, a test harness 
  which can be used to test .OPO files located in C:\OplTest\.
  See the OPL source code oplr\oplwrapper0\opltests60.tpl for more info.

OPX:
* Released an alpha version of AppFrame OPX to switch the status pane on/off.
  This fixes defect 745071.
  To use, call AfSetStatusVisible%:(0) for off, 1 for on.
  Note that no other OPX APIs have been tested.
* Added some mediaserveropx test code progs missing from earlier releases.

Other:
* Added set of 6.1 logs giving examples of good Series 60 builds. See \logs6.1.

---
v1.24 15 June 03 ricka@users.sourceforge.net

Added scripts for building the 9200 public (runtime) and developer packages.
Added copies of the build logs in \opl\logs\ for comparision purposes.

---
v1.23 26 May 03 ricka@users.sourceforge.net

OPLT: Improved OPLT casting - changes from Keita 
  http://www.hi-ho.ne.jp/~ktkawabe/densha_e.html
  See oplt\stran\ot_utl.cpp for more details.
* Added a new test to the OPLT test suite in ttran\.

---
v1.22 26 May 03 ricka@users.sourceforge.net

All components:
* Fix THUMB builds for building with Nokia 9200 Communicator Series 
  SDK v1.2.
  Basically adding new librarys to ensure that THUMB builds are
  complete with this SDK. 
  - egcclib with edll.lib and edllstub.lib

OPLT: 
* Build from relative location.
* Fixed the test suite so that it runs for 9210.
* Got tmodtran test working again.
* Export the test suite data to \c\topt\.
  (It was building for z:\data\).
* Added a makefile 'preptest.mk' to build the suite code.
* Corrected a couple of typo's while debugging the suite.

OPLR: 
* Added thumb build libs to oplr and recopl.

OPLTOOLS:
* Fixed bug 743653 "OPLTRAN can't handle relative filenames" in opltran.cpp
* Upped the version number to 2.09 accordingly.

OPX: 
* Got the OPX code building with a relative location.
* And using EPOCROOT.

Other minor changes:
* Removed some distribution.policy files.
* Changed the name of this src zip from 'opl-dev' to 'opl', matching the 
  sourceForge.net CVS details.

---
v1.21 05 May 03 ricka@users.sourceforge.net

OPLR: Added support for Series 60 phones on Symbian OS 6.1 
* Added conditional framework in .mmp files for .def, src and libs
* Major mods to opl runtime startup.
* Disabled FEP interaction for now.

Plus other minor source changes:
* Made oplt use a relative build, no longer has to be \oplt.
* Ditto for oplr.
* Removed distribution.policy files - we don't need them any longer.
* Removed monochrome oplr\aif icons.
* Renamed colour oplr\aifc icon folder to oplr\aif.
* Removed some old stuff from oplr\group.
* Renamed oplr\aif\opl.rss to oplaif.rss - or we'd have two version of opl.rss.
* Moved Symbian OS 6.0-only versions into <name>60 subdirs:
    inc60 - dummy bldvariant.hrh file. Keeps 6.0 builds happy.
    opl60 - 6.0 version of opl.app. I was having major problems with __SERIES60_
      definitions in resource files (and this was panicking the S60 emulator
      at startup!), so I split the subdir in two. :-(
* Added a readme.txt file

Note: this is only a half-way house and not suitable for porting to new
versions of the OS. We need to do something like move the core OPL functions
to some new core library, and get each OPL UI to link to the core.

---
v1.20 23 April 03

Initial upload of OPL for SourceForge.net
<ends>