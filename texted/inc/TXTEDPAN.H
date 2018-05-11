// TXTEDPAN.H
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#ifndef __TXTEDPAN_H__
#define __TXTEDPAN_H__

enum TTextedPanic
	{
	ETextedPanicInvalidCursorPosition,
	ETextedPanicGotoListBoxAlreadyExists,
	ETextedPanicUnexpectedTranslatorError,
	ETextedPanicProcedureArrayInvalid,
	ETextedPanicTitleBarInvalid
	};

GLREF_C void Panic(TTextedPanic aPanic);

#endif