// OPLAPP.CPP
//

#include <akndoc.h>
#include <aknapp.h>
#include <aknenv.h>
#include <e32std.h>
#include <f32file.h>
#include <eikappui.h>
#include <eikapp.h>
#include <apgwgnam.h>
#include <apgicnfl.h>
#include <apfdef.h>
#include <eikmobs.h>
#include <oplr.h>
#include "debug.h"
#include "oplwrapper.h"

class COplDocument :public CAknDocument
	{
public:
	static COplDocument* NewL(CEikApplication& aApp);
private: // from CApaDocument
	CEikAppUi* CreateAppUiL();
	COplDocument(CEikApplication& aApp);
	void ConstructL();
	};


class COplApplication : public CAknApplication
	{
public:
	TUid AppDllUid() const;
private: // from CApaApplication
	CApaDocument* CreateDocumentL();
	COplDocument* iDocument;
	};

const TUid KUidOplWrapper=	{_OPLWRAPPERUID};

TUid COplApplication::AppDllUid() const
	{
	return KUidOplWrapper;
	}


// Document

COplDocument::COplDocument(CEikApplication& aApp)
: CAknDocument(aApp)
	{
	}


CApaDocument* COplApplication::CreateDocumentL()
	{
	iDocument=COplDocument::NewL(*this);
	return iDocument;
	}

void COplDocument::ConstructL()
	{
//	FLOGWRITE(_L("COplDocument::ConstructL()"));
	}

COplDocument* COplDocument::NewL(CEikApplication& aApp)
	{
	COplDocument* self = new (ELeave) COplDocument(aApp);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


EXPORT_C CApaApplication* NewApplication()
	{
	return(new COplApplication);
	}

GLDEF_C TInt E32Dll(TDllReason /* aReason */)
	{
	return(KErrNone);
	}

CEikAppUi* COplDocument::CreateAppUiL()
	{
//	FLOGWRITE(_L("COplDocument::CreateAppUiL()"));
	return new (ELeave) COplRuntime;
	}

