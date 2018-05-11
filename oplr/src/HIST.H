// HIST.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __HIST_H__
#define __HIST_H__

#include <e32Std.h>

class RHist 
     {
 public:
     TInt Open(RFs& aFs);
	 void Close();

     void WriteLog(TUint aOpcode);
     void FuncWriteLog(TUint aOpcode);
 private:
	 TInt iCount;
     TInt ifilenum;
	 RFile iFile;
    };
#endif