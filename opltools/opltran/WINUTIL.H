// WINUTIL.H
//
// Copyright (c) 1998-2001 Symbian Ltd. All rights reserved.

#ifndef __WINUTIL_H__
#define __WINUTIL_H__

GLREF_C void _WingetcwdL();
GLREF_C void ConvertToUnicodeL(TDes16& aUnicode,const TDesC8& aForeign);
GLREF_C void ConvertFromUnicodeL(TDes8& aForeign,const TDesC16& aUnicode);

#endif