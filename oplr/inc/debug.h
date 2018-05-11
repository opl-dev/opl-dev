// DEBUG.H

#ifndef DEBUG_H
#define	DEBUG_H

/*
 * To use:
 * #define _USE_FILELOGGING (in this file perhaps)
 * then create the following folder on the device:
 *  c:\logs\opl\
 * Run your test, and check that folder for the opl.log text file.
 */

// #define _USE_FILELOGGING
#ifdef _USE_FILELOGGING
#include "flogger.h"
_LIT(KOplFileLoggingDir,"opl");
_LIT(KOplFileLoggingFileName,"opl.log");
#define FLOGWRITE(a) RFileLogger::Write(KOplFileLoggingDir(),\
	KOplFileLoggingFileName(),\
	EFileLoggingModeAppend,\
	a)
#else
#define FLOGWRITE(a)
#endif
#endif