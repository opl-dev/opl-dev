The batch file "dorelease.cmd" in the 9200 and 9500 folders can be used to build a complete release.

Before building a release you MUST edit the dorelease.cmd file to with the settings for your system. The part before the :mkrelease label can be left untoched.

In the call to mkrelease specify:
- the location of your symbian SDK (including the \epoc32 folder)
- the location of the \opl folder where the OPL source code is located

The last part of the dorelease.cmd batch file creates zip files of the built releases. Currently the zip-files
are built with data 2006-06-10 (date of the 1.55 release). Before creating a new release, change the date of
the zip-files to the date of your release.

The following example, builds version 1.55 for the S80 SDK, where the OPL source code is located in \Projects\OPL1.55\opl\:
call mk9500release \Symbian\7.0s\S80_DP2_0_SDK\epoc32 \Projects\OPL1.55\opl %1 %2 >release.log 2>&1

Dorelease has one mandatory argument (the version number) and can have three optional arguments:
1. SIS      Recreates all SIS files
2. BUILD    Builds the OPL sources
3. CLEAN    Cleans the OPL sources

Possible syntaxes:
dorelease 1.55                   Creates release version 1.55 from the existing files
dorelease 1.55 SIS               Rebuilds all SIS files and creates a release
dorelease 1.55 SIS BUILD         Rebuilds the project, all SIS files and creates a release
dorelease 1.55 SIS BUILD CLEAN   Cleans and rebuilds the project, all SIS files and creates a release.

The release is always built in a new folder that has the same name as the specified version number.
