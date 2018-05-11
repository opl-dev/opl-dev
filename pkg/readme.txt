opl-dev \opl\pkg\readme.txt
===========================
v1.56 16 June 06
Replaced by batch files in the rel folder, which clean, build, generate sis files and create a complete release
in one step

v1.50 22 February 04
ricka@users.sourceforge.net

Building the UIQ release of OPL
-------------------------------
1. Build the UIQ binaries - see \opl\readme.txt
2. Make the runtime package
     cd \opl\pkg
     makeUiqRuntimePkg
3. Make the UIQ developer patch
     cd \opl\pkg
     makeUiqDevPkg
     Zip up \rel\dev into OPL-UIQ-WINS-999.zip
       where 999 is the current release number.


Building the Series 60 release of OPL
-------------------------------------
Build the user runtime package, and developer patch.
1. Build the Series60 binaries - see \opl\readme.txt.
2. Make the runtime package
     cd \opl\pkg
     makeruntimepkg
3. Make the developer S60 patch
     cd \opl\pkg
     makedevpkg
     zip up \rel\dev into OPL-OS61-WINS-999.zip
       where 999 is the current release number.

Building a Nokia 9200 Series Communicator release of OPL
--------------------------------------------------------
Build the runtime and developer packages for 9200 OPL:
1. Build the binaries - see \opl\readme.txt.
2. Make OPX sis files:
     cd \opl\opx\
     opxsis
3. Make the dev SIS file:
     cd \opl\texted\rom\
     buildsis
4. Build runtime package:
     cd \opl\pkg
     make9210runtimepkg
5. Build developer package:
     cd \opl\pkg
     make9210devpkg
6. Manually zip the developer files into developer.zip:
     Use WinZip32 to zip up \rel\dev\*.* into \rel\dev\developer.zip
7. Rename the following files:
     \rel\dev\developer.zip to \rel\9200-OPL-DevPack-2003-05-26.zip (or whatever the date is)
     \rel\public\public.zip to \rel\9200-OPL-Public-2003-05-26.zip 

<ends>
