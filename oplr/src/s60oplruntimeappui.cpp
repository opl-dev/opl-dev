#include <eikmenup.h>

#include "Oplr.h"
#include "OplStartUpDialog.h" 
//#include "Opl.rsg"
#include "OplStartUpDialog.hrh"

#include <avkon.hrh>


EXPORT_C void COplRuntime::DynInitMenuPaneL(TInt /*aResourceId*/, CEikMenuPane* /*aMenuPane*/)
    {
    }

EXPORT_C TKeyResponse COplRuntime::HandleKeyEventL(const TKeyEvent& /*aKeyEvent*/,
											TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

EXPORT_C void COplRuntime::HandleCommandL(TInt aCommand)
    {
    if (aCommand == EEikCmdExit)
        {
        Exit();
        }
	else if (aCommand == EOplStartUpDialogCmdIdStart)
		{
		iStartUpAO->StartItUpL();
		}
	else if (aCommand == EOplStartUpDialogCmdIdDebug)
		{
		_LIT(KDebugMsg,"Debug start N/A.");
		User::InfoPrint(KDebugMsg);
		}
	}

// End of file