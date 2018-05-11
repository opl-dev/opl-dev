// TESTMAIN.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include <e32std.h>						 
#include "tests.h"
#include <eikenv.h>
#include <eikappc.h>
#include <eikdoc.h>

class COplDocument :public CEikDocument
	{
private:
	CEikAppControl* CreateAppControl();
	};


EXPORT_C CEikDocument* EikMain()
	{
	return new COplDocument;
	}

CEikAppControl* COplDocument::CreateAppControl()
	{
	CEikAppControl* appControl=new COplRuntime;
	appControl->SetAppControlFlags(EEikAppControlNoAppResourceFile);
	return(appControl);
	}

void dummyOpcode(CStack&, COplRuntime&, CFrame*) {}

// dummy table for testing purposes only
OPCODEFUNC OpCode::OpCodeTable[] =
		{
		&dummyOpcode
		};

// another dummy!!
COplRuntime::COplRuntime()
	{
	}
	
void COplRuntime::SetUserRaisedError(TOplRaised aRaised)
	{
	iRaised = aRaised;
	}

//Dummy
void COplRuntime::SequencerL()
	{
	}

COplRuntime::~COplRuntime()
    {
    delete iConsole;
	delete iStack;
    }


TKeyResponse COplRuntime::OfferUnhandledKeyL(TKeyEvent*,TEventCode)
	{
   	return(EKeyWasNotConsumed);
	}

TKeyResponse COplRuntime::OfferKeyEventL(TKeyEvent* aKeyEvent,TEventCode aType)
	{
	if (aType==EEventKey && aKeyEvent->code==CTRL('q'))
		CmdQuitL();
   	return(EKeyWasConsumed);
	}

void COplRuntime::EikonEnvironmentReadyL()
	{
	SetTheRuntime(this);
	iConsole=new(ELeave) COplConsole;
	iConsole->ConstructL();
	iStack=CStack::NewL(2048);
	iConsole->DrawCursor();
	TBuf<80> buf;
	buf.Format(_L("Starting Tests\r\n"));
	iConsole->Write(buf);
	RunTests();
	buf.Format(_L("Tests Complete\r\n"));
	iConsole->Write(buf);
	}

//
// Main
//
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(ETrue);
	}