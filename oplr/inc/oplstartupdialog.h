
#ifndef OPLRUNTIMEDIALOG_H
#define OPLRUNTIMEDIALOG_H

#include <eikdialg.h>
#if !defined(__UIQ__)
#include <aknlists.h>
#endif

#include "oplstartupdialog.hrh"

class COplRuntimeStartUpDialog : public CEikDialog
    {
    public: // Constructors and destructor
        /**
        * Destructor.
        */
        ~COplRuntimeStartUpDialog();

    public: // New functions.

    protected:  // Functions from base classes.
        void PreLayoutDynInitL();
        TBool OkToExitL(TInt aButtonId);

    private: // Data.
    };


#endif

// End of file.
