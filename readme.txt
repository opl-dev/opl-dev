opl-dev Project Readme
======================
29 February 2004 - valid for opl-dev v1.51 onwards.
ricka@users.sourceforge.net

This release contains the opl-dev project source code. For more information, 
see http://sourceforge.net/projects/opl-dev/.

This assumes you have installed a relevant SDK for Symbian OS development:
* Nokia 9200 Communicator series 1.1 for the Symbian OS 6.0 version of OPL
* Series 60 SDK for the Symbian OS 6.1 version of OPL
* UIQ SDK v2.0 for the Symbian OS 7.0 version of OPL
* Nokia 7710 SDK (27-Jan-05) for the Symbian OS 7.0s version of OPL
* Series 80 Developer Platform 2.0 SDK (12-Jan-05) for 9500/9300.

NB Opl for Series60 won't build on later versions of S60 SDKs. See 
OPL bug [ 1161204 ] OPL runtime WSERV 3 on S60_2nd_fp2 SDK

Note that you can switch between SDKs by controlling the system environment 
variables.

Another assumption is that you want to build the S60 target. If not, see
the section "building for other targets" below.

Steps to build the open source OPL binaries
-------------------------------------------
1. Unzip the opl-dev source code.
2. Build the OPL components in the following order:
  oplt
  opltools (*this may also require a manual stage)
  oplr (**don't forget the test components for Series 60)
  opx
  texted
  demoopl

Note: to build a component, use the following commands:
  cd <component>\group
  bldmake bldfiles
  abld build 
  Note: For a quick emulator-only version: abld build wins udeb
        To verify your build output, see "Checking the build" below.

*the manual stage after opltools has completed is as follows:
copy opltools\opltran\opltran.bat to the epoc32\tools folder in your path.
e.g. \Symbian\6.1\Shared\epoc32\tools\
Note that this is a different location to that in EPOCROOT.

**the test components for the Series 60 version of OPLR are the 
opltests60 launcher etc. Build them with:
 abld test build
 cd oplwrapper0
 trancopy

Checking the build
------------------
If you wish to compare your build results with a known build, replace the line:
  abld build
with:
  abld build > \<component>.log 2>&1
And then use your favourite file comparison tool (beyond compare2 is recommended)
to diff your log with the corresponding one in \opl\logs\ or \opl\logs6.1.

Building for other targets
--------------------------
The default configuration is for S60 target. To switch to another target, modify
the \opl\opl-target.mmpi file and ensure a single target is uncommented. For
example, to build for UIQ, ensure the 
	#define __UIQ_MMP__
line is uncommented, and the other targets are commented.

Making a release
----------------
Information about building the release .SIS files can be found in the \pkg
folder - see \pkg\readme.txt

Thanks
------
This weekend's release was brought to you by the artist "Beck" and the 
albums "Mellow Gold" and "Odelay".
 
<ends>
