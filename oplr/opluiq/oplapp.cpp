// OPLAPP.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <qikapplication.h>
#include <bldvariant.hrh>
#include <e32std.h>
#include <f32file.h>
#include <eikappui.h>
#include <eikapp.h>
#include <apgwgnam.h>
#include <apgicnfl.h>
#include <apfdef.h>
#include <oplr.h>

#include <qikdocument.h>
class COplDocument :public CQikDocument
	{
public:
	COplDocument(CEikApplication& aApp): CQikDocument(aApp) {}
	void UpdateTaskNameL(CApaWindowGroupName* aWgName);
	TUid CorrectAppUidAtThisTime() const;
private:
	void GetCaptionL(const TDesC& aAppModuleName);
private: // from CApaDocument
	CEikAppUi* CreateAppUiL();
	TApaAppCaption iCaption;
private:
	TBool iReturnOplAppUid;
	};

CEikAppUi* COplDocument::CreateAppUiL()
	{
	// RDebug::Print(_L("CreateAppUiL()\n"));
	return new(ELeave) COplRuntime;
	}

//
// This method is needed on Crystal 6.0 only. This allows
// us to work-around a problem with ViewSrv (prior to LOAK21)
// caused by OPL renaming the Application UID of the
// WindowGroup (see UpdateTaskNameL() below). This allows us
// to set a null UID in the ViewSrv until such time as our
// own UID has been set correctly. We then manually call
// ViewSrv through our AppUi (see UpdateTaskNameL()
// again) to create the views properly.
//
TUid COplDocument::CorrectAppUidAtThisTime() const
	{
	if (iReturnOplAppUid)
		return KUidOplInterpreter;
	else if (iAppUi)
		{
		COplCommandLine& comLine=STATIC_CAST(COplRuntime*,iAppUi)->CommandLine();
		if(comLine.AppUid()!=KNullUid)
			return TUid(comLine.AppUid());
		return KUidOplInterpreter;
		}
	return KNullUid;
	}

void COplDocument::UpdateTaskNameL(CApaWindowGroupName *aWgName)
	{
	if (iAppUi==NULL)
		{
		iReturnOplAppUid=ETrue;
		CEikDocument::UpdateTaskNameL(aWgName);
		iReturnOplAppUid=EFalse;
		}
	else
		{
		COplCommandLine& comLine=STATIC_CAST(COplRuntime*,iAppUi)->CommandLine();
		TUid uid(comLine.AppUid());
		if (uid!=KUidOplInterpreter)
			{ // is an app
			if (iCaption.Length()==0)
				{
				GetCaptionL(comLine.ModuleName());
				aWgName->SetCaptionL(iCaption);
				}

			aWgName->SetAppUid(comLine.AppUid());
			aWgName->SetDocNameL(STATIC_CAST(COplRuntime*,iAppUi)->CurrentDocumentName());
			}
		else
			{
			aWgName->SetDocNameL(comLine.ModuleName());
			aWgName->SetRespondsToShutdownEvent(EFalse);
			aWgName->SetRespondsToSwitchFilesEvent(EFalse);
			}
		aWgName->SetDocNameIsAFile(ETrue);

		TChar command=comLine.Command();
		if (command!=KApaCommandLetterRunWithoutViews)
			{
			// Will leave with KErrNotSupported from LOAK21 (since ViewSrv was removed)
			TRAPD(error,iAppUi->CheckInitializeViewsL(uid));
			if (error==KErrNone)
				{
				if (command!=KApaCommandLetterBackground && command!=KApaCommandLetterViewActivate)
					{
					// Will leave with KErrNotSupported from LOAK21 (since ViewSrv was removed)
					TRAPD(ignore,iAppUi->ActivateTopViewL());
					}
				}
			}
		}
	}


void COplDocument::GetCaptionL(const TDesC& aAppModuleName)
	{
	TParse parse;
	parse.Set(KAppInfoFileExtension,&aAppModuleName,NULL); // can the module name be invalid
	CApaAppInfoFileReader* infoFileReader=NULL;
	TRAPD(err,infoFileReader=CApaAppInfoFileReader::NewL(CEikonEnv::Static()->FsSession(),parse.FullName()));
	if (!err)
		{
		CleanupStack::PushL(infoFileReader);
		iCaption=infoFileReader->CaptionL(User::Language());
		CleanupStack::PopAndDestroy(); // infoFileReader
		}
	else
		iCaption=parse.Name();
	}

//
// COplApplication
//

class COplApplication : public CQikApplication
	{
public:
	TUid AppDllUid() const;
	TFileName ResourceFileName() const;
private: // from CApaApplication
	CApaDocument* CreateDocumentL();
	COplDocument* iDocument;
	};

TUid COplApplication::AppDllUid() const
	{
	if (iDocument)
		return iDocument->CorrectAppUidAtThisTime();
	return KUidOplInterpreter;
	}


TFileName COplApplication::ResourceFileName() const
	{
	return KNullDesC();
	}


CApaDocument* COplApplication::CreateDocumentL()
	{
	iDocument=new(ELeave) COplDocument(*this);
	return iDocument;
	}

EXPORT_C CApaApplication* NewApplication()
	{
	return(new COplApplication);
	}

GLDEF_C TInt E32Dll(TDllReason /* aReason */)
	{
	// RDebug::Print(_L("E32Dll()\n"));
	return(KErrNone);
	}

