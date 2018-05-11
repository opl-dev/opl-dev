// TOPX.H
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#if !defined(__TOPX_H__)
#define __TOPX_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__OPLAPI_H__)
#include <oplapi.h>
#endif
#if !defined(__OPX_H__)
#include <opx.h>
#endif

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// The OPX's UID as used in the DECLARE OPX statement for your OPX
// NB. This UID must be reserved by contacting Symbian
//
const TInt KUidOpxTemplate=0x10000259;

// The OPX's version number as used in the DECLARE OPX statement for your OPX
const TInt KOpxVersion=0x100;

// The root class of the OPX - add any members and/or component classes
// that your particular OPX requires to this class
//
// An instance of this class is created immediately after the OPX is loaded (using LOADM)
// and the object pointer must be stored in the Tls (Thread local static).
// See the 1st EXPORT function NewOpxL(), which does this for you.
// The Tls contains the single global (32-bit) value available to any EPOC32 DLL.
// No other non-const static data may be used.
//
class COpxRoot : public COpxBase 
    {
public:
    static COpxRoot* NewL(OplAPI& aOplAPI);
	virtual void RunL(TInt aProcNum);
	virtual TInt CheckVersion(TInt aVersion);
	// Add any other members specific to your OPX
	// ...    
private:
	// the language extension procedures
	enum TExtensions
		{
		EAdd3=1,
		EIncNumericArgsPassedByref
		};
	void Add3() const;
	void IncNumericArgsPassedByref() const;
private:
	void ConstructL();
    COpxRoot(OplAPI& aOplAPI);
    ~COpxRoot() ;
    };

// OpxRoot() accesses the Tls which contains the COpxRoot*, created and stored there by NewOpxL()
//
// OpxRoot() is only required if you need to access your COpxRoot object
// in a deeply nested function which hasn't been passed this pointer directly
// Note that accessing the Tls in this way is relatively slow (about 4 microsecs) so
// it is often more efficient to pass the COpxRoot* down the chain of functions.
inline COpxRoot* OpxRoot() { return((COpxRoot *)Dll::Tls()); }

#endif __TOPX_H__