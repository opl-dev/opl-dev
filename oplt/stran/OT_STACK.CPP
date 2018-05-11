// OT_STACK.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "ot_std.h"


////////////////////////////////////////////////////////////////
//
// CTranStackBase  
//
////////////////////////////////////////////////////////////////
EXPORT_C void CTranStackBase::Drop()
//
// Drops the last item off the stack
//
	{
	
	Delete(Index(0),1);
	}



EXPORT_C const TAny* CTranStackBase::Peek(TInt aDepth) const 
//
//Pointer to item down the stack
//
	{
	
	return At(Index(aDepth));
	}


EXPORT_C void CTranStackBase::Pick(TAny* aPtr,TInt aDepth)
//
// Temove an itenm from the middle of the stack
//
	{
	
	Mem::Copy(aPtr,Peek(aDepth),Length());
	Delete(Index(aDepth),1);
	}

TUint CTranStackBase::Index(TInt aDepth) const 
//
// Converst a stack depth into an array index.
// This would be where to have an underflow panic, if separate from the
// EArray.. bounds checking panic
//
	{

	return Count()-(aDepth+1);
	}

