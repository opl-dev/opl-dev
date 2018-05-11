// OPLERR.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __OPLERR_H__
#define __OPLERR_H__

#include <e32std.h>

const TInt KOplErrorBase    (0xabcd0000);	// must have a zero lower word
const TInt KOplErrorMask	(0xffff0000);
#define OPLERR(a) (KOplErrorBase|(0x0000ffff&(a)))
// standard error codes - same as OPL16
// other errors should be added when needed

const TInt KOplErrGenFail (OPLERR(-1));
const TInt KOplErrInvalidArgs   (OPLERR(-2));		// Invalid Arguments
const TInt KOplErrOs (OPLERR(-3));
const TInt KOplErrNotSupported (OPLERR(-4));
const TInt KOplErrUnderflow (OPLERR(-5));
const TInt KOplErrOverflow (OPLERR(-6));
const TInt KOplErrOutOfRange	(OPLERR(-7));		// Value out of range
const TInt KOplErrDivideByZero	(OPLERR(-8));		// Divide by Zero
const TInt KOplErrInUse (OPLERR(-9));
const TInt KOplErrNoMemory    (OPLERR(-10));		// No memory
const TInt KOplErrNoSegments (OPLERR(-11));
const TInt KOplErrNoSemaphore (OPLERR(-12));
const TInt KOplErrNoProcess (OPLERR(-13));
const TInt KOplErrAlreadyOpen (OPLERR(-14));
const TInt KOplErrNotOpen (OPLERR(-15));
const TInt KOplErrImage (OPLERR(-16));
const TInt KOplErrNoReceiver (OPLERR(-17));
const TInt KOplErrNoDevices (OPLERR(-18));
const TInt KOplErrNoFileSystem (OPLERR(-19));
const TInt KOplErrFailedToStart (OPLERR(-20));
const TInt KOplErrFontNotLoaded (OPLERR(-21));
const TInt KOplErrTooWide (OPLERR(-22));
const TInt KOplErrTooManyItems (OPLERR(-23));
const TInt KOplErrBatLowSound (OPLERR(-24));
const TInt KOplErrBatLowFlash (OPLERR(-25));

/* file errors */
const TInt KOplErrExists      (OPLERR(-32));
const TInt KOplErrNotExists   (OPLERR(-33));
const TInt KOplErrWrite       (OPLERR(-34));
const TInt KOplErrRead        (OPLERR(-35));
const TInt KOplErrEof         (OPLERR(-36));
const TInt KOplErrFull        (OPLERR(-37));
const TInt KOplErrName        (OPLERR(-38));
const TInt KOplErrAccess      (OPLERR(-39));
const TInt KOplErrLocked      (OPLERR(-40));
const TInt KOplErrDevNotExist (OPLERR(-41));		// Device does not exist
const TInt KOplErrDir         (OPLERR(-42));
const TInt KOplErrRecord      (OPLERR(-43));
const TInt KOplErrReadOnly    (OPLERR(-44));
const TInt KOplErrInvalidIO   (OPLERR(-45));		// Invalid IO request
const TInt KOplErrFilePending (OPLERR(-46));		// IO operation pending
const TInt KOplErrVolume      (OPLERR(-47));
const TInt KOplErrIOCancelled (OPLERR(-48));		// IO operation cancelled

// OPL translator error codes
const TInt KOplErrSyntax      (OPLERR(-77));		// Syntax Error
const TInt KOplStructure		(OPLERR(-85));		// Structure fault
const TInt KOplErrBadAssignment (OPLERR(-93));		// Bad assignment
// OPL error codes
const TInt KOplErrIllegal 		(OPLERR(-96));		// Illegal Opcode
const TInt KOplErrNumArg 	 	(OPLERR(-97));		// Wrong number of parameters to procedure	
const TInt KOplErrUndef   		(OPLERR(-98));		// Undefined externals
const TInt KOplErrNoProc  		(OPLERR(-99));		// Procedure not found
const TInt KOplErrNoFld   		(OPLERR(-100));	// Database field not found
const TInt KOplErrOpen    		(OPLERR(-101));	// File already open
const TInt KOplErrClosed  		(OPLERR(-102));	// File not open
const TInt KOplErrRecSize  		(OPLERR(-103));	// Database file record to big
const TInt KOplErrModLoad 		(OPLERR(-104));	// Module already loaded (LOADM)
const TInt KOplErrMaxLoad 		(OPLERR(-105));	// Maximum modules loaded (LOADM)
const TInt KOplErrNoMod   		(OPLERR(-106));	// Module does not exist (LOADM)
const TInt KOplErrNewVer  		(OPLERR(-107));	// Incompatible translator version (LOADM)
const TInt KOplErrModNotLoaded  (OPLERR(-108));	// Module not loaded (UNLOADM)
const TInt KOplErrBadFileType	(OPLERR(-109));	// ODB file header wrong or corrupt
const TInt KOplErrTypeViol 		(OPLERR(-110));	// Type violation when passing parameters to procedure
const TInt KOplErrSubs 			(OPLERR(-111));	// Subscript error (array out of range)
const TInt KOplErrStrTooLong	(OPLERR(-112));	// String too long (REPT$ and when adding strings)
const TInt KOplErrDevOpen 		(OPLERR(-113));	// Device already open (for LOPEN)
const TInt KOplErrEsc 			(OPLERR(-114));	// Esc key presses in TRAP EDIT or TRAP INPUT
// const TInt KOplErrOdbVer 	(OPLERR(-115));	// Obsolete (not even used in Opl1993)
// const TInt KOplErrOdbOpen 	(OPLERR(-116));	// "
const TInt KOplErrMaxDraw 		(OPLERR(-117));	// Maximum number of drawables open
const TInt KOplErrDrawNotOpen   (OPLERR(-118));	// Drawable not open
const TInt KOplErrInvalidWindow (OPLERR(-119));	// Window operation attempted on a bitmap
const TInt KOplErrScreenDenied  (OPLERR(-120));	// Screen access denied (when run from EVAL?)
const TInt KOplErrOpxNotFound	(OPLERR(-121)); // Opx file not found
const TInt KOplErrOpxVersion	         (OPLERR(-122)); // Wrong Opx version 
const TInt KOplErrOpxProcNotFound	     (OPLERR(-123)); // Opx procedure not found 
const TInt KOplErrStopInCallback	     (OPLERR(-124)); // Stop used in a callback
const TInt KOplErrIncompatibleUpdateMode (OPLERR(-125)); // Incompatible update mode
const TInt KOplErrInTransaction	         (OPLERR(-126)); // In transaction
// translator errors
const TInt KOplErrIncludeProcedure       (OPLERR(-127)); // INCLUDE file cannot contain procedures - Not yet used
const TInt KOplErrTooManyOPXs            (OPLERR(-128)); // Too many OPXs
const TInt KOplErrTooManyOpxFunctions    (OPLERR(-129)); // Too many OPX functions
const TInt KOplErrUndefinedVariable      (OPLERR(-130)); // Undefined variable
const TInt KOplErrUndefinedProcedure     (OPLERR(-131)); // Undefined procedure
const TInt KOplErrIconMaskMissing        (OPLERR(-132)); // Icon mask missing
const TInt KOplErrIncompatibleDeclaration (OPLERR(-133)); // Incompatible declaration
// ARM thumb Unicode error
const TInt KOplErrBadAlignment           (OPLERR(-134)); // Misaligned string
const TInt KOplMaxErr (-134);   // the maximum error code (so far)

#endif
