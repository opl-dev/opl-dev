// OPLSTACK.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


#ifndef __OPLSTACK_H
#define __OPLSTACK_H

#include <e32base.h>
#include <oplflg.h>

#define OplString TText*

_LIT(KOplAlignPanic,"OPL Alignment");
#define AlignPanic(a) User::Panic(KOplAlignPanic,a)

_LIT(KOplStackError,"OPL Stack Error");
#define StackPanic(a) User::Panic(KOplStackError,a)

#define __ASSERT_ISALIGNED(p) __ASSERT_DEBUG(!(TUint(p)&1),AlignPanic(KErrOplAlignBadAlignment))

const TInt KErrStackEmpty=-100;
const TInt KErrStackFull=-101;
const TInt KErrStackTypeMismatch=-102;
const TInt KErrInvalidStackPtr=-103;
const TInt KErrBadType=-104;

//Alignment problems
const TInt KErrOplAlignBadLength=-200;
const TInt KErrOplAlignPostStackCheck=-201;
const TInt KErrOplAlignBadAlignment=-202;

typedef union
{
TUint8* Uint8;
TInt32* Int;
TReal64* Real;
TPtr8* Ptr;
TAny** Any;
TUint16* Uint16;
// need to add more for strings etc.
} TStackPtr;

#ifdef _DEBUG_STACK
//EString already used.
enum Ttype {EInt16,EInt32,EReal64,EStringType,ERefInt16,ERefInt32,ERefReal64,ERefStringType};
#endif

class CFrame;
class OpCode;
class COplRuntime;

class CStack	: public CBase
{
public:
	CStack();
	static CStack* NewL(TUint aSize,COplRuntime* aRuntime);
	void ConstructL(TUint aSize,COplRuntime* aRuntime);
	~CStack();
	void Push(const TInt16 aInt16);
	void Push(const TInt32 aInt32);
	void Push(const TReal64 aReal);
	void Push(const OplString aLbc);
	void Push(const TDesC& aDes);
	TInt16 PopInt16();
	TInt32 PopInt32();
	TReal64 PopReal64();
	TPtrC PopString();
	void PushRef(TInt16& aInt16Ptr);
	void PushRef(TInt32& aInt32Ptr);
	void PushRef(TReal64& aRealPtr);
	void PushRef(OplString aLbc);
	TInt16& PopRefInt16();
	TInt32& PopRefInt32();
	TReal64& PopRefReal64();
	TUint16* PopRefString();
	TStackPtr StackPtr() const;
	void SetStackPtr(const TStackPtr &aSP);
	TInt FreeBytes() const;
	void UnwindParams();
	TAny* PopPtr();

private:
	TUint8* iCell;
	TUint8* iBase;
	TUint8* iRoot;
	TStackPtr iSP;
	COplRuntime* iRuntime;

	void Push(TAny* aPtr);
	void CheckParamCountL(TInt aCount);
	TAny* NextParamL(TInt aType);
	void JumpParam(TInt aType);
friend class CFrame;

#ifdef _DEBUG_STACK
friend class OpCode;
	TUint8 Type() {return *(iTypes-iCell+iSP.Uint8);};
	void SetType(TUint8 aType) {*(iTypes-iCell+iSP.Uint8)=aType;};
	TUint8* iTypes;
#endif

};

#endif

