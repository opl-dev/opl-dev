// DEBUG.H

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef _USE_FILELOGGING
#include "flogger.h"
_LIT(KFileLoggingDir,"texted");
_LIT(KFileLoggingFileName,"texted.log");
#define FLOGWRITE(a) RFileLogger::Write(KFileLoggingDir(),\
	KFileLoggingFileName(),\
	EFileLoggingModeAppend,\
	a)
#else
#define FLOGWRITE(a)
#endif
#endif