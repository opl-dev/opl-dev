// WINUTIL.CPP
//
// Copyright (c) 1998-2001 Symbian Ltd. All rights reserved.

// Windows utilities
#pragma warning(disable: 4201)  // winnt.h uses nameless structs
#pragma warning(disable: 4514)	// unreferenced inline/local function has been removed
#if !defined(__UIQ__)
#include <stdlib.h>
#include <direct.h>
#include <locale.h>
#endif
#include <e32std.h>
#include <e32base.h>

GLREF_D TText8 CurrentWorkingDirectory8[];

GLDEF_C void main()
	{
//	printf("Error: wrong \"main\" function invoked (should be executing E32Main)!\n"); 
//  this should never get called - it is only there to stop a stupid linker error which 
// 	demands it (commenting out the "setlocale" call below gets rid of this linker error)
//	exit(1);
	}

#if defined(__UIQ__)
GLDEF_C void _WingetcwdL() // Get the current working directory.
	{
	User::Leave(KErrNotSupported);
	}
#else
GLDEF_C void _WingetcwdL() // Get the current working directory.
	{
	if (_getcwd((char*)CurrentWorkingDirectory8,KMaxFileName)==NULL)
		{
		User::Leave(KErrGeneral);
		}
	}
#endif

LOCAL_C void UseDefaultMultiByteCharacterSetForCurrentLocaleL()
	{
/* AAARGH! can't use setlocale as it tries to use Windows malloc :-(
	if (setlocale(LC_ALL, "")==NULL) // trying to set the multi-byte character set to the Windows locale's default character set
		{
		User::Leave(KErrGeneral);
		}
*/
	}

#if defined(__UIQ__)
GLDEF_C void ConvertToUnicodeL(TDes16& /*aUnicode*/,const TDesC8& /*aForeign*/)
	{
	User::Leave(KErrNotSupported);
	}

GLDEF_C void ConvertFromUnicodeL(TDes8& /*aForeign*/,const TDesC16& /*aUnicode*/)
	{
	User::Leave(KErrNotSupported);
	}
#else
GLDEF_C void ConvertToUnicodeL(TDes16& aUnicode,const TDesC8& aForeign)
	{
	UseDefaultMultiByteCharacterSetForCurrentLocaleL();
	HBufC8* foreignBuffer=HBufC8::NewLC(aForeign.Length()+1);
	TPtr8 foreign(foreignBuffer->Des());
	foreign=aForeign;
	foreign.ZeroTerminate();
	const char* foreignPointer=(const char*)foreign.Ptr();
	const TInt requiredSize=mbstowcs(NULL, foreignPointer, KMaxTInt)+1;
	if ((requiredSize<=0) || (requiredSize>aUnicode.MaxLength()))
		{
		User::Leave(KErrGeneral);
		}
	if ((TInt)mbstowcs((wchar_t*)aUnicode.Ptr(), foreignPointer, KMaxTInt)!=requiredSize-1)
		{
		User::Leave(KErrGeneral);
		}
	aUnicode.SetLength(User::StringLength(aUnicode.Ptr()));
	CleanupStack::PopAndDestroy(); // foreignBuffer
	}

GLDEF_C void ConvertFromUnicodeL(TDes8& aForeign,const TDesC16& aUnicode)
	{
	UseDefaultMultiByteCharacterSetForCurrentLocaleL();
	HBufC16* unicodeBuffer=HBufC16::NewLC(aUnicode.Length()+1);
	TPtr16 unicode(unicodeBuffer->Des());
	unicode=aUnicode;
	unicode.ZeroTerminate();
	const wchar_t* unicodePointer=unicode.Ptr();
	const TInt requiredSize=wcstombs(NULL, unicodePointer, KMaxTInt)+1;

	// wcstombs bug information 
	// the method returns -1 if the method fails (contains unicode) it returns (-1)cast to a t_size 
	// which is a unsigned int, so its never going to be less than zero 
	
	if ((requiredSize<=0) || (requiredSize>aForeign.MaxLength()))
		{
		User::Leave(KErrGeneral);
		}
	TInt writtenBytes=wcstombs((char*)aForeign.Ptr(), unicodePointer, KMaxTInt);
	if (writtenBytes <0)
		{
		User::Leave(KErrNotSupported);
		}
	
	aForeign.SetLength(requiredSize -1);	// -1 removing the null terminator 
	//aForeign.SetLength(User::StringLength(aForeign.Ptr()));
	CleanupStack::PopAndDestroy(unicodeBuffer);
	}
#endif

