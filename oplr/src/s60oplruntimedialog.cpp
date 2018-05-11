
#include "OplStartUpDialog.h"

#include <avkon.hrh>
#include <aknappui.h>
#include <EikFUtil.h>
#include <MsvIds.h>

COplRuntimeStartUpDialog::~COplRuntimeStartUpDialog()
    {
    }

// This function is called when either of the two softkeys is pressed.

TBool COplRuntimeStartUpDialog::OkToExitL(TInt aButtonId)
	{
    if (aButtonId == EAknSoftkeyOptions)
        iAvkonAppUi->ProcessCommandL(EAknSoftkeyOptions);
    else if (aButtonId == EAknSoftkeyExit)
        iAvkonAppUi->ProcessCommandL(EEikCmdExit);
    
    return EFalse;
	}

void COplRuntimeStartUpDialog::PreLayoutDynInitL()
    {
    }



