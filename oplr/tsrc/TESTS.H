// TESTS.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
#ifndef __TESTS_H__
#define __TESTS_H__

class COplTest;


#include <opcodes.h>


class COplTest : public CBase
	{
public:
	virtual void DoTest()=0;
	static TInt CmpReal(TReal aReal1,TReal aReal2);
	};

#endif
