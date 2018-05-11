// TERRLOC.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Translator error location testing


#include <e32test.h>
#include <f32file.h>
#include <opltran.h>

LOCAL_D RTest test(_L("Translator Error Location"));
struct TErrorText
	{
	TOplTranslateError iError;
	const TText *iText; // Uses ^ as a marker for where the error should be - removed before translating 
	};

class CErrorTextSystem : public CBase, public MTextSourceSystem, public MTextSource
	{
public:
	static CErrorTextSystem* CErrorTextSystem::NewLC();
	~CErrorTextSystem();
	TInt SetTextFindErrorPosL(const TText* aProc); // Returns the position of the error characterr
	TInt OpenSource(TDes& aFileName, MTextSource*& aTextSource);
	TInt Read(TDes& aBuf,TInt aPos);
	TInt Read(TDes& aBuf);
	void Close();
private:
	void ConstructL();
private:
	TInt iPos;
	CBufFlat *iBuf;
	};

TInt CErrorTextSystem::SetTextFindErrorPosL(const TText *aProc)
//
// Takes a zero terminated bit of text, finds the ^ character which
// marks where the error should be
//
//	
	{

	Close();
	TPtrC input(aProc);
	TInt offset=input.Locate(TChar('^'));
	TPtr8 ptr((TUint8*)input.Ptr(),input.Size());
	ptr.SetLength(input.Size());
	iBuf->InsertL(0,ptr.Left(offset<<1));
	iBuf->InsertL(offset<<1,ptr.Mid((offset+1)<<1));
	return offset;
	}


CErrorTextSystem* CErrorTextSystem::NewLC()
	{

	CErrorTextSystem *pTs=new(ELeave) CErrorTextSystem;
	CleanupStack::PushL(pTs);
	pTs->ConstructL();
	return pTs;
	}

CErrorTextSystem::~CErrorTextSystem()
	{
	delete iBuf;
	}

TInt CErrorTextSystem::OpenSource(TDes& /*aFileName*/,MTextSource*& aTextSource)
	{
	aTextSource=this;
	return KErrNone;
	}

TInt CErrorTextSystem::Read(TDes& aBuf, TInt aPos)
	{
	iPos=aPos*sizeof(TText);
	return Read(aBuf);
	}

TInt CErrorTextSystem::Read(TDes& aBuf)
	{
	
	TInt length=iBuf->Size()-iPos;
	if (length<=0)
		return KErrEof;
	if (length>aBuf.MaxLength())
		length=aBuf.MaxLength();
	TPtr8 ptr((TUint8*)aBuf.Ptr(),aBuf.MaxLength()<<1);
	iBuf->Read(iPos,ptr,length);
	aBuf.SetLength(length>>1);
	iPos+=length;
	return KErrNone;
	}

void CErrorTextSystem::Close()
	{
	iPos=0;
	iBuf->Reset();
	}

void CErrorTextSystem::ConstructL()
	{
	
	iBuf=CBufFlat::NewL(0x20);
	}

#ifdef ___OPL1993
LOCAL_D TErrorText opl1993Tests[]=
	{
	{ EErrBadLogicalDevice,_S("PROC junk:\x2029 ^i.c=5\x2029 ENDP\x2029 ")}, // bad field va
	{ EErrIdentifierTooLong,_S("PROC junk:\x2029 abcdefgh=3\x2029 ^abcdefghi=4\x2029 ENDP\x2029 ")}, // name too long
	{ EErrIdentifierTooLong,_S("PROC junk:\x2029 abcdefg$=\"a\"\x2029 ^abcdefgh$=\"a\"\x2029 abcdefghi$=\"b\"\x2029 ENDP\x2029 ")}, // name too long
#if defined(_UNICODE) // K.K Japanese Visual C++ expect correct character code that exists in JIS code table!!!!
	{ EErrIdentifierTooLong,_S("PROC junk:\x2029 CCCCCCCC=3\x2029 ^AAAAAAAAEE4\x2029 ENDP\x2029 ")},
#else
 	{ EErrIdentifierTooLong,_S("PROC junk:\x2029 çççççççç=3\x2029 ^æææææææææ=4\x2029 ENDP\x2029 ")}, // name too long error, make sure isReserved doesn't disappear on funny characters
#endif
	{ EErrSyntax,_S("PROC junk:\x2029 call(^)\x2029 ENDP\x2029 ")}, // check for no arguments to variable count fn
	{ EErrTypeMismatch,_S("PROC junk:\x2029 call(2,\"fred\"^,2)\x2029 ENDP\x2029 ")}, // Check for argument types in optional parameter lists
	{ EErrSubscript,_S("PROC oplha18:\x2029 PRINT sum(a%(^))\x2029 ENDP\x2029 ")},
	{ EErrDataTooLarge,_S("PROC junk:\x2029 global a$(1639,40)\x2029 ^ENDP\x2029 ")}, // oversize allocation
	{ EErrDataTooLarge,_S("PROC junk:\x2029 global a(4000),b$(136,250)\x2029 ^ENDP\x2029 ")}, // combined oversize allocation
	{ EErrDataTooLarge,_S("PROC junk:\x2029 local a(5),b(8191),b%\x2029 a(1)=b(1)\x2029 ^ENDP\x2029 ")}, // too large numeric array
	{ EErrFound,NULL}
	};
#endif

//		EParagraphDelimiter = 0x2029,	// paragraph separator

#define Keol 0x2029


LOCAL_D TErrorText opler1Tests[]=
	{
	{ EErrTypeMismatch,_S("PROC jim:(arg1)\x2029 PRINT arg1\x2029 ENDP\x2029 PROC ext$:\x2029  PRINT jim(\"fred\"^)\x2029 ENDP\x2029 ")}, // Check that definition acts as declaration

	{ EErrBadCharacter,_S("PROC junk:\x2029 ^\001 23\x2029 ENDP\x2029 ")}, // Check for Edeath in lexer
    { EErrTypeMismatch,_S("PROC junk:\x2029 hello=\"fred\"^+1\x2029 ENDP\x2029 ")}, // Check for error location during operator output
	{ EErrArgNotReferencedByValue,_S("PROC junk:(hello)\x2029 ^hello=3\x2029 ENDP\x2029 ")}, // assignment to parameters
	{ EErrBadDeclaration,_S("PROC junk:\x2029 global ^glstr$\x2029 ENDP\x2029 ")}, // illegal string declaration - no size
	{ EErrBadDeclaration,_S("PROC junk:\x2029 global glnum(3^,4)\x2029 ENDP\x2029 ")}, // illegal 2-d array declaration
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a(^0)\x2029 ENDP\x2029 ")}, // illegal zero array size
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a(^40000)\x2029 ENDP\x2029 ")}, // oversize array
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a$(^0)\x2029 ENDP\x2029 ")}, // illegal zero length string declaration
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a$(^0,4)\x2029 ENDP\x2029 ")}, // illegal zero dimension string array
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a$(4,^0)\x2029 ENDP\x2029 ")}, //illegal zero size for string array elements
	{ EErrBadDeclaration,_S("PROC junk:\x2029 global^\x2029 ENDP\x2029 ")}, // empty declaration
	{ EErrIdentifierTooLong,_S("PROC junk:\x2029 abcdefgh=3\x2029 ^abcdefghijklmnopqrstuvwxyz0123456=4\x2029 ENDP\x2029 ")}, // name too long
	{ EErrIdentifierTooLong,_S("PROC junk:\x2029 abcdefg$=\"a\"\x2029 abcdefgh$=\"a\"\x2029 ^abcdefghijklmnopqrstuvwxyz012345$=\"b\"\x2029 ENDP\x2029 ")}, // name too long
	{ EErrSyntax,_S("PROC junk:\x2029 a=1.2^q9\x2029 ENDP\x2029 ")}, // bad char in number
	{ EErrBadCharacter,_S("PROC junk:\x2029 a=%A\x2029 b=^%\x2029 ENDP\x2029 ")}, // bad char in number
	{ EErrSyntax,_S("PROC junk:\x2029 a%=$f12f\x2029 a=$ffff\x2029 a%=$12f\x2029 a%=$12^g\x2029 ENDP\x2029 ")}, // bad hex
    { EErrBadNumber,_S("PROC junk:\x2029 a=^$\x2029 a=$+5\x2029 ENDP\x2029 ")}, // empty hex
	{ EErrBadFieldName,_S("PROC junk:\x2029 ^a.1c=0\x2029 ENDP\x2029 ")}, // bad field variable name
	{ EErrBadFieldName,_S("PROC junk:\x2029 ^a.ab(3)=9\x2029 ENDP\x2029 ")}, // illegal field array
	{ EErrSyntax,_S("PROC junk:\x2029 cls:^print a\x2029 ENDP\x2029 ")}, // missing space (unexpected procedure!!)
	{ EErrSyntax,_S("PROC junk:\x2029 print a,^input\x2029 ENDP\x2029 ")}, // keyword where expression expected
	{ EErrSyntax,_S("PROC junk:\x2029 a=b+^print\x2029 ENDP\x2029 ")}, // keyword in expression
    { EErrMismatchedBracket,_S("PROC junk:\x2029 a=a+b^)\x2029 ENDP\x2029 ")}, // missing open bracket
	{ EErrMismatchedBracket,_S("PROC junk:\x2029 a=(a+b^\x2029 ENDP\x2029 ")}, // missing closing bracket
	{ EErrMissingQuote,_S("PROC junk:\x2029 a$=abc^\"\x2029 ENDP\x2029 ")}, // missing open quote
	{ EErrMissingQuote,_S("PROC junk:\x2029 a$=^\"abc\x2029 ENDP\x2029 ")}, // missing open quote
	{ EErrSyntax,_S("PROC junk:\x2029 global a ^b\x2029 ENDP\x2029 ")}, // missing comma
	{ EErrDuplicateName,_S("PROC junk:\x2029 global a,b,c\x2029 local ^b\x2029 ENDP\x2029 ")}, // duplicate name
	{ EErrMissingLabel,_S("PROC junk:\x2029 ^goto a::\x2029 ENDP\x2029 ")}, // missing label
	{ EErrMissingLabel,_S("PROC junk:\x2029 a::\x2029 goto ^a:037ENDP\x2029 ")}, // missing label on goto
	{ EErrTooComplex,_S("PROC junk:\x2029 if a=4\x2029 if a=4\x2029 if a=4\x2029 if a=4\x2029 if a=4\x2029 if a=4\x2029 if a=4\x2029 if a=4\x2029 ^if a=4\x2029 endif\x2029 endif\x2029 endif\x2029 endif\x2029 endif\x2029 endif\x2029 endif\x2029 endif\x2029 endif\x2029 ENDP\x2029 ")}, // nested ifs
	{ EErrStructure,_S("PROC junk:\x2029 while a\x2029 print a\x2029 ^ENDP\x2029 ")}, // missing endwh
	{ EErrSyntax,_S("PROC junk:\x2029 a=a+1\x2029 ^until a>5\x2029 print a\x2029 ENDP\x2029 ")}, // missing DO
	{ EErrStructure,_S("PROC junk:\x2029 a=0\x2029 do a=a+1\x2029 until a>5\x2029 if a>5 :^break :endif\x2029 print a\x2029 ENDP\x2029 ")}, // break outside loop structure
	{ EErrFnArgument,_S("PROC junk:\x2029 a=sin(3^,4)\x2029 ENDP\x2029 ")}, // too many arguments to fn
	{ EErrFnArgument,_S("PROC junk:\x2029 a=sin^\x2029 ENDP\x2029 ")}, // too few arguments to fn
	{ EErrSubscript,_S("PROC junk:\x2029 a(1^,2)=3\x2029 ENDP\x2029 ")}, // array index error
	{ EErrSubscript,_S("PROC junk:\x2029 a=b(^)\x2029 ENDP\x2029 ")}, // no index to array
	{ EErrTypeMismatch,_S("PROC junk:\x2029 pause \"hello\"^\x2029 ENDP\x2029 ")}, // string where num expected
	{ EErrTypeMismatch,_S("PROC junk:\x2029 delete 3^\x2029 ENDP\x2029 ")}, // num where string expected
	{ EErrArgNotReferencedByValue,_S("PROC junk:(b)\x2029 a=addr(^b)\x2029 ENDP\x2029 ")}, // parameter to addr fn
	{ EErrBadLogicalDevice,_S("PROC junk:\x2029 a=^bb.c\x2029 ENDP\x2029 ")}, // bad field var
	{ EErrDuplicateName,_S("PROC junk:\x2029 global a,b\x2029 global ^a\x2029 ENDP\x2029 ")}, // duplicate global declare
	{ EErrDuplicateName,_S("PROC junk:\x2029 local a,b\x2029 local ^a\x2029 ENDP\x2029 ")}, // duplicate local declare
	{ EErrDuplicateName,_S("PROC junk:\x2029 local a,b\x2029 global ^b\x2029 local c\x2029 ENDP\x2029 ")}, // duplicate declare
	{ EErrDuplicateName,_S("PROC junk:(a)\x2029 local ^a\x2029 ENDP\x2029 ")}, // al b,c
	{ EErrDuplicateName,_S("PROC junk:(a)\x2029 global ^a\x2029 ENDP\x2029 ")}, // l b,c
	{ EErrBadFieldName,_S("PROC junk:\x2029 create \"a:fred\",a,^\x2029 ENDP\x2029 ")}, // no fields
	{ EErrBadFieldList,_S("PROC junk:\x2029 open \"a:fred\",a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee,ff,gg^,hh\x2029 ENDP\x2029 ")}, // one too many fields
	{ EErrMismatchedBracket,_S("PROC junk:\x2029 if (a+3)\x2029 endif\x2029 if a+3^)\x2029 endif\x2029 ENDP\x2029 ")}, // missing opening bracket in IF condition
	{ EErrMismatchedBracket,_S("PROC junk:\x2029 if (a+3^\x2029 endif\x2029 ENDP\x2029 ")}, // missing closing bracket in IF condition
	{ EErrSyntax,_S("PROC junk:\x2029 print (3*2+5)+9^)*7\x2029 ENDP\x2029 ")}, // extra close bracket in print
	{ EErrMismatchedBracket,_S("PROC junk:\x2029 a(5*(2+7))=a(3*9+5)+7^)+8\x2029 ENDP\x2029 ")}, // mismatched brackets in right side array
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a(^.999999999999e99),b(3)\x2029 ENDP\x2029 ")}, // big float in declaration
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a(^-1),b(3)\x2029 ENDP\x2029 ")}, // negative declaration
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a(3),b(^--1),c(3)\x2029 ENDP\x2029 ")}, // illegal double unary minus
	{ EErrBadArraySize,_S("PROC junk:\x2029 global a($ff),b(^$8000),c($ffff)\x2029 ENDP\x2029 ")}, // illegal hex integer in declaration
	{ EErrBadDeclaration,_S("PROC junk:\x2029 global a(3),^,,,,,,,,,,,,,,,,,,,,,,b(7),,,,,,,,\x2029 ENDP\x2029 ")}, // commas in declare list
	{ EErrBadDeclaration,_S("PROC junk:\x2029 global ^,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\x2029 ENDP\x2029 ")}, // only commas in declare list
	{ EErrSyntax,_S("PROC junk:\x2029 pause 3^,5\x2029 ENDP\x2029 ")}, // too many values for pause
	{ EErrSyntax,_S("PROC junk:\x2029 pause ^,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\x2029 ENDP\x2029 ")}, // only commas for pause
	{ EErrSyntax,_S("PROC junk:\x2029 pause ^;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\x2029 ENDP\x2029 ")}, // only semicolons for pause
	{ EErrSyntax,_S("PROC junk:\x2029 pause 3 ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\x2029 pause ^::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\x2029 ENDP\x2029 ")}, // only colons for pause
	{ EErrSubscript,_S("PROC junk:\x2029 a(3^,4)=5\x2029 ENDP\x2029 ")}, // 2 subscripts for ls numeric array 
	{ EErrSubscript,_S("PROC junk:\x2029 a(3)=a(4^,5)\x2029 ENDP\x2029 ")}, // 2 subscripts for rs numeric array 
	{ EErrSubscript,_S("PROC junk:\x2029 a$(3^,4	)=\"\"\x2029 ENDP\x2029 ")}, // 2 subscripts for ls string array 
	{ EErrSubscript,_S("PROC junk:\x2029 a$=a$(3^,4)\x2029 ENDP\x2029 ")}, // 2 subscripts for rs string array 
	{ EErrSyntax,_S("PROC junk:\x2029 global a,b ^c,d\x2029 ENDP\x2029 ")}, // missing comma in declare list
	{ EErrSyntax,_S("PROC junk:\x2029 pokew 5 ^5\x2029 ENDP\x2029 ")}, // missing comma in command
	{ EErrSyntax,_S("PROC junk:\x2029 mid$(\"hello\" ^2,1)\x2029 ENDP\x2029 ")}, // missing comma in function call
	{ EErrMissingComma,_S("PROC junk:\x2029 open \"fred\",a ^b\x2029 ENDP\x2029 ")}, // missing comma in open
	{ EErrBadFieldList,_S("PROC junk:\x2029 open \"a:fred\",a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,a%,b%,c%,d%,e%,f%,g%^,h%,i%,j%,k%,l%,m%,n%,o%,p%\x2029 ENDP\x2029 ")}, // lots too many fields
	{ EErrSyntax,_S("PROC junk:\x2029 input ^3\x2029 ENDP\x2029 ")}, // illegal input to literal
	{ EErrSyntax,_S("PROC junk:\x2029 input ^a$:\x2029 ENDP\x2029 ")}, // illegal input to procedure
	{ EErrSyntax,_S("PROC junk:\x2029 input ^a::\x2029 ENDP\x2029 ")}, // illegal input to label
	{ EErrSyntax,_S("PROC junk:\x2029 print (1) ^get\x2029 ENDP\x2029 ")}, // illegal function after close bracket - original error was \"print not(1)get\"
	{ EErrSyntax,_S("PROC junk:\x2029 print 1*1^sin\x2029 ENDP\x2029 ")}, // tests for \"1*1sin\" bug
	{ EErrMismatchedBracket,_S("PROC junk:\x2029 print (1^,2,3)\x2029 ENDP\x2029 ")}, // tests for \"print (1,2,3)\" bug
	{ EErrMismatchedBracket,_S("PROC junk:\x2029 a$=num$(1,3)\x2029 a=fred:(1,3)\x2029 a=(1^,2,3)\x2029 ENDP\x2029 ")}, // illegal commas in brackets
	{ EErrBadStringLength,_S("PROC junk:\x2029 local a$(255),b$(256^),c$(257)\x2029 a$=b$\x2029 ENDP\x2029 ")}, // illegal declaration of a simple string variable
	{ EErrBadStringLength,_S("PROC junk:\x2029 local a$(5,255),b$(5,^256),c$(5,257)\x2029 a$=b$\x2029 ENDP\x2029 ")}, // illegal declaration of a string array variable
	{ EErrBadNumber,_S("PROC junk:\x2029 a=^.\x2029 ENDP\x2029 ")}, // test for . being number
	{ EErrSyntax,_S("PROC junk:\x2029 a=%%<%%+%%*%%%%%^%\x2029 ENDP\x2029 ")}, // check extra %
	{ EErrSubscript,_S("PROC junk:\x2029 a=addr(b(1^,2))\x2029 ENDP\x2029 ")}, // check for array index in addr
	{ EErrArgNotReferencedByValue,_S("PROC junk:(a%)\x2029 ioopen(^a%,\"\",0)\x2029 ENDP\x2029 ")}, // check for illegal use of argument reference
	{ EErrFnArgument,_S("PROC junk:\x2029 ioopen(^a,\"\",0)\x2029 ENDP\x2029 ")}, // check for incorrect type in reference
	{ EErrFnArgument,_S("PROC junk:\x2029 fred:(1,2,3)\x2029 fred:(1,2^)\x2029 ENDP\x2029 ")}, // Inconsistant function calls
	{ EErrSyntax,_S("PROC junk:\x2029 edit ^\"hello\"\x2029 ENDP\x2029 ")}, // illegal edit of literal
	{ EErrSyntax,_S("PROC junk:\x2029 edit ^a$:\x2029 ENDP\x2029 ")}, // illegal edit of string procedure
	{ EErrSyntax,_S("PROC junk:\x2029 edit a$\x2029 edit a.b$\x2029 edit a$(3)\x2029 edit ^a\x2029 ENDP\x2029 ")}, // illegal edit of numeric variable
	{ EErrSyntax,_S("PROC junk:\x2029 edit ^a::\x2029 ENDP\x2029 ")}, // illegal edit of label
	{ EErrBadNumber,_S("PROC junk:\x2029 a&=&ffffffff\x2029 a&=^&\x2029 ENDP\x2029 ")}, // check for empty long hex
	{ EErrFnArgument,_S("PROC junk:\x2029 ioopen(^1,\"fred\",2)\x2029 ENDP\x2029 ")}, // check for first argument to ioopen
	{ EErrUndefinedProcedure,_S("DECLARE EXTERNAL\x2029 PROC junk:\x2029 ^proc3:\x2029 ENDP\x2029 ")}, // Trying to call undeclared procedure
	{ EErrUndefinedProcedure,_S("DECLARE EXTERNAL\x2029 PROC junk:\x2029 ^p:(16)\x2029 ENDP\x2029 ")}, // Trying to call undeclared procedure
	{ EErrUndefinedVariable,_S("DECLARE EXTERNAL\x2029 PROC junk:\x2029 print ^a$\x2029 ENDP\x2029 ")}, // Trying to use an undeclared external
	{ EErrUndefinedVariable,_S("DECLARE EXTERNAL\x2029 PROC junk:\x2029 print ^c%(10)\x2029 ENDP\x2029 ")}, // Trying to call undeclared external
	{ EErrIncompatibleDeclaration,_S("EXTERNAL ext$:(arg1%,arg2&)\x2029 PROC jim:(arg1)\x2029 PRINT arg1\x2029 ENDP\x2029 PROC ext$:(arg1%^)\x2029  PRINT arg1%\x2029 ENDP\x2029 ")}, // Check that definition matches declaration
	{ EErrIncompatibleDeclaration,_S("EXTERNAL ext$:(arg1%,arg2&)\x2029 PROC jim:(arg1)\x2029 PRINT arg1\x2029 ENDP\x2029 PROC ext$:(arg1%,arg2^)\x2029  PRINT arg1%\x2029 ENDP\x2029 ")}, // Check that definition matches declaration
#pragma message("terrloc.cpp: EErrLineTooLong (reported as 'Missing ENDP' in Texted) appears 1 char beyond EOF.")
//	{ EErrLineTooLong,_S("PROC junk:\x2029 print \"96\"\x2029 ^")}, // check for missing ENDP at end of file
	{ EErrFound,NULL}
	};

class TTestTranErrors
	{
public:
	void RunTests();
private:
	void TestErrors(const TDesC& aTestName,COplModuleTranslator& aTranslator,TErrorText *aTestArray);
	};


void TTestTranErrors::TestErrors(const TDesC& aTestName,COplModuleTranslator& aTranslator,TErrorText *aTestArray)
	{

	test.Start(aTestName);
	CErrorTextSystem* textSystem=CErrorTextSystem::NewLC();
	TInt testNo=0;
	while (aTestArray->iText!=NULL)
		{
		test.Printf(_L("\r%04d"),testNo);
		TInt errPos=0;
		TRAPD(err,errPos=textSystem->SetTextFindErrorPosL(aTestArray->iText));
		test(err==KErrNone);
		TSourceTranslateError error;
		TRAP(err,aTranslator.StartTranslateL(*textSystem,_L("C:\\TEMP\\JUNK.OPL"),_L("C:\\TEMP\\JUNK.OPO"),CTextTranslator::ERelease,error));
		while (aTranslator.TranslateChunk()==KErrNone);
		test(error.Error()==aTestArray->iError);
		test(error.Position()==errPos);
		aTestArray++;
		testNo++;
		}
	test.Printf(_L("\r\n"));
	CleanupStack::PopAndDestroy();
	test.End();
	}

void TTestTranErrors::RunTests()
//
// Constructs a translator, and does the tests
//
	{
	
	TInt i;
	for (i=0;i<100;i++)
		CleanupStack::PushL((TAny *)i);
	CleanupStack::Pop(100);
	
	__UHEAP_MARK;

	COplModuleTranslator *translator=COplModuleTranslator::NewLC();

#ifdef ___OPL1993
	translator->SetTarget(EOplTargetOpl1993);
	TestErrors(_L("Opl1993 only"),*translator,opl1993Tests);
#endif
	translator->SetTarget(EOplTargetER1);
	TestErrors(_L("Opler1 & common"),*translator,opler1Tests);

	CleanupStack::PopAndDestroy();
	__UHEAP_MARKEND;	
	}


GLDEF_C TInt E32Main()
//
// Test the file server.
//
	{
	__UHEAP_MARK;
	CTrapCleanup *trapCleanup=CTrapCleanup::New();

	test.Title();
	TTestTranErrors errorTester;
	test.Start(_L("Opl Module Translator Errors"));
	TRAPD(err,errorTester.RunTests()); // Just to give us a framework to work in
	test (err==KErrNone);
	test.End();
	test.Close();

	delete trapCleanup;
	__UHEAP_MARKEND;
	return(KErrNone);
	}

