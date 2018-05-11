// CLCKUPDT.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef CLCKUPDT_H
#define CLCKUPDT_H

#include <e32std.h>

#ifndef __GRAPHICS_H__
class CDrawablesCollection;
#endif

class COplClockUpdater : public CActive
	{
public:
	COplClockUpdater(CDrawablesCollection& aDrawablesCollection);
	void ConstructL(); // connects to notifier
	~COplClockUpdater();
	void RunL(); // updates the clocks and requeues the notifier
private:
	void DoCancel();
private:
	RChangeNotifier iChangeNotifier;
	CDrawablesCollection& iDrawablesCollection;
	};

#endif