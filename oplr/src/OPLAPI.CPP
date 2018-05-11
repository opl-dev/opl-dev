// OPLAPI.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd. All rights reserved.

#include <oplapi.h>
#include <opcodes.h>
#include <oplcmd.h>
#include <opltdef.h>
#include <opldb.h>
#include <oplr.rsg>
#include "oplutil.h"
#include "frame.h"
#include "graphics.h"

const TInt16 KTypeInt16=0;
const TInt16 KTypeInt32=1;
const TInt16 KTypeReal64=2;
const TInt16 KTypeString=3;

//
// Class MEventSource
//
EXPORT_C void MEventSource::MEventSource_Reserved1()
	{
	}

EXPORT_C void MEventSource::MEventSource_Reserved2()
	{
	}

//
// Class OplAPI
//
OplAPI::OplAPI(COplRuntime* aRuntime,RHeap& aHeap)
	:iHeap(aHeap),iCallbackInfoArray(4)
	{
	iRuntime=aRuntime;
	iStack=&(iRuntime->Stack());
	}

// Functions forwarded to CStack
EXPORT_C void OplAPI::Push(TInt16 aInt16)
	{
	iStack->Push(aInt16);
	}

EXPORT_C void OplAPI::Push(TInt32 aInt32)
	{
	iStack->Push(aInt32);
	}

EXPORT_C void OplAPI::Push(const TReal64 aReal)
	{
	iStack->Push(aReal);
	}

EXPORT_C void OplAPI::PushL(const TText16* aLbc)
	{
	iStack->Push(aLbc);
	}

EXPORT_C void OplAPI::PushL(const TDesC16& aDes)
	{
	iStack->Push(aDes);
	}

EXPORT_C TInt16 OplAPI::PopInt16()
	{
	return iStack->PopInt16();
	}

EXPORT_C TInt32 OplAPI::PopInt32()
	{
	return iStack->PopInt32();
	}

EXPORT_C TReal64 OplAPI::PopReal64()
	{
	return iStack->PopReal64();
	}

EXPORT_C TPtrC OplAPI::PopString()
	{
	return iStack->PopString();
	}

EXPORT_C TInt16* OplAPI::PopPtrInt16()
	{
	TInt addr=iStack->PopInt32();
	return (TInt16*)OplUtil::OffsetToAddrL(iHeap,addr,sizeof(TInt16));
	}

EXPORT_C TInt32* OplAPI::PopPtrInt32()
	{
	TInt addr=iStack->PopInt32();
	return (TInt32*)OplUtil::OffsetToAddrL(iHeap,addr,sizeof(TInt32));
	}

EXPORT_C TReal64* OplAPI::PopPtrReal64()
	{
	TInt addr=iStack->PopInt32();
	return (TReal64*)OplUtil::OffsetToAddrL(iHeap,addr,sizeof(TReal64));
	}

EXPORT_C TAny* OplAPI::StackPtr() const
	{
	return iStack->StackPtr().Uint8;
	}

EXPORT_C void OplAPI::SetStackPtr(const TAny* aSP)
	{
	TStackPtr stackPtr;
	stackPtr.Uint8=(TUint8*)aSP;
	iStack->SetStackPtr(stackPtr);
	}

EXPORT_C TInt OplAPI::FreeBytes() const
	{
	return iStack->FreeBytes();
	}

// Functions forwarded to OplUtil
// - These are for getting and putting values to non-aligned addresses
EXPORT_C void OplAPI::PutLong(TAny* aPtr,TInt32 aLong)
	{
	OplUtil::PutLong(aPtr,aLong);
	}

EXPORT_C TInt32 OplAPI::GetLong(TAny* aPtr)
	{
	return OplUtil::GetLong(aPtr);
	}

EXPORT_C void OplAPI::PutFloat(TAny* aPtr,TReal64 aFloat)
	{
	OplUtil::PutFloat(aPtr,aFloat);
	}

EXPORT_C TReal64 OplAPI::GetFloat(TAny* aPtr)
	{
	return OplUtil::GetFloat(aPtr);
	}

EXPORT_C void OplAPI::PutWord(TAny* aPtr,TInt16 aWord)
	{
	OplUtil::PutWord(aPtr,aWord);
	}

EXPORT_C TInt16 OplAPI::GetWord(TAny* aPtr)
	{
	return OplUtil::GetWord(aPtr);
	}

EXPORT_C TUint8* OplAPI::OffsetToAddrL(TInt aOffset,TInt aSize)
	{
	return OplUtil::OffsetToAddrL(iHeap,aOffset,aSize);
	}

EXPORT_C TInt OplAPI::MapTranslatorError(TInt aErr,TInt& aResId)
// Return the mapped error and set aResId to OPLR's resource number or R_OPLR_UNKNOWN_ERROR
// If an OPL-specific error the high word has KOplErrorMask ORed in to avoid clashes with E32 errors
// If not mapped the passed error code is returned unchanged
	{
	const TInt KOpltErrBase=-70+EErrMissingQuote; // Translator errors in same order as for OPL1993 but starting at 2
	TBool oplError=ETrue;
	TInt map;
	switch (aErr)
		{
	case EErrMissingQuote:
	case EErrBadStringLength:
	case EErrBadIdentifier:
	case EErrIdentifierTooLong:
	case EErrBadLogicalDevice:
	case EErrBadFieldName:
	case EErrBadNumber:
	case EErrBadSuffices:
	case EErrBadCharacter:
	case EErrFnArgument:
	case EErrTypeMismatch:
	case EErrMissingLabel:
	case EErrDuplicateName:
	case EErrBadDeclaration:
	case EErrBadArraySize:
	case EErrStructure:
	case EErrUnexpectedEof:
	case EErrSyntax:
	case EErrMismatchedBracket:
	case EErrBadFieldList:
	case EErrTooComplex:
	case EErrMissingComma:
	case EErrDataTooLarge:
	case EErrArgNotReferencedByValue:
	case EErrSubscript:
	case EErrInconsistentArgs:
		map=(KOpltErrBase-aErr);
        	break;
	case EErrLineTooLong:
		map=KOplErrRecord;
	        break;
	case EErrAssignToConstant:
		map=KOplErrBadAssignment;
		break;
	case EErrIncludeProcedure:
		map=KOplErrIncludeProcedure;
		break;
	case EErrTooManyOPXs:
		map=KOplErrTooManyOPXs;
		break;
	case EErrTooManyOpxFunctions:
		map=KOplErrTooManyOpxFunctions;
		break;
	case EErrUndefinedVariable:
		map=KOplErrUndefinedVariable;
		break;
	case EErrUndefinedProcedure:
		map=KOplErrUndefinedProcedure;
		break;
	case EErrIconMaskMissing:
		map=KOplErrIconMaskMissing;
		break;
	case EErrIncompatibleDeclaration:
		map=KOplErrIncompatibleDeclaration;
		break;
	default:
		// must be E32 error
		oplError=EFalse;
		map=aErr;
		// map=OplUtil::MapError(aErr);
		}
	if (oplError)
		{
		aResId=OplUtil::GetErrResource(TInt16(map));
		map=OPLERR(map);
		}
	else
		aResId=R_OPLR_UNKNOWN_ERROR;
	return map;
	}

EXPORT_C TInt16 OplAPI::MapError(TInt aErr)
	{
	return OplUtil::MapError(aErr);
	}

// Arithmetic with overflow checking
EXPORT_C TInt16 OplAPI::MultiplyL(TInt16 aArg1,TInt16 aArg2)
	{
	return OplUtil::MultiplyL(aArg1,aArg2);
	}

EXPORT_C TInt32 OplAPI::MultiplyL(TInt32 aArg1,TInt32 aArg2)
	{
	return OplUtil::MultiplyL(aArg1,aArg2);
	}

EXPORT_C RWindowGroup& OplAPI::RootWindow()
	{
	return iRuntime->ConEnv()->RootWin();
	}

EXPORT_C RBackedUpWindow& OplAPI::WindowFromIdL(TInt aId)
	{
	COplDrawable* drawable=iRuntime->DrawablesCollection().DrawableL(aId);
	TDrawableFlag drawFlag=drawable->DrawableFlag();
	if (drawFlag!=EIsOplWindow && drawFlag!=EIsOplConsole)
		User::Leave(KOplErrInvalidWindow);
	return ((COplWindow*)drawable)->Window();
	}

EXPORT_C RWsSession& OplAPI::WsSession()
	{
	return iRuntime->ConEnv()->WsSession();
	}

EXPORT_C CWsScreenDevice* OplAPI::ScreenDevice()
	{
	return iRuntime->ConEnv()->ScreenDevice();
	}

EXPORT_C TInt OplAPI::BitmapHandleFromIdL(TInt aId)
	{
	COplDrawable* drawable=iRuntime->DrawablesCollection().DrawableL(aId);
	return drawable->BitmapHandle();
	}

EXPORT_C CEikonEnv& OplAPI::EikonEnv() const
	{
	return (*STATIC_CAST(CEikonEnv*,iRuntime->ConEnv()));
	}

EXPORT_C COplDbManager* OplAPI::DbManager()
	{
	return iRuntime->DbManager();
	}
	
EXPORT_C void OplAPI::OpenCheckL()
	{
	iRuntime->DbManager()->OpenCheckL();
	}

EXPORT_C void OplAPI::InitCallbackL(TDesC16& aProcName)
	{
	SCallbackInfo callbackInfo;
	callbackInfo.iStackPtr=StackPtr();
	callbackInfo.iParamCount=0;
	iCallbackInfoArray.AppendL(callbackInfo);
	iStack->Push(aProcName);
	}

_LIT(KOplAPI,"OplAPI");

EXPORT_C void OplAPI::PushParamL(TInt16 aInt)
	{
	TInt last=iCallbackInfoArray.Count()-1;
	__ASSERT_DEBUG(last>=0,User::Panic(KOplAPI,1));
	iCallbackInfoArray[last].iParamCount++;
	if (iStack->FreeBytes()<(TInt)(3*sizeof(TInt32))) // int,type,count
		{
		ResetStack();
		User::LeaveNoMemory();
		}
	iStack->Push(aInt);
	iStack->Push(KTypeInt16);
	}

EXPORT_C void OplAPI::PushParamL(TInt32 aLong)
	{
	TInt last=iCallbackInfoArray.Count()-1;
	__ASSERT_DEBUG(last>=0,User::Panic(KOplAPI,1));
	iCallbackInfoArray[last].iParamCount++;
	if (iStack->FreeBytes()<(TInt)(3*sizeof(TInt32))) // int,type,count
		{
		ResetStack();
		User::LeaveNoMemory();
		}
	iStack->Push(aLong);
	iStack->Push(KTypeInt32);
	}

EXPORT_C void OplAPI::PushParamL(TReal64 aFloat)
	{
	TInt last=iCallbackInfoArray.Count()-1;
	__ASSERT_DEBUG(last>=0,User::Panic(KOplAPI,1));
	iCallbackInfoArray[last].iParamCount++;
	if (iStack->FreeBytes()<(TInt)(sizeof(TReal64)+2*sizeof(TInt32))) // real,type,count
		{
		ResetStack();
		User::LeaveNoMemory();
		}
	iStack->Push(aFloat);
	iStack->Push(KTypeReal64);
	}

EXPORT_C void OplAPI::PushParamL(const TDesC16& aString)
	{
	TInt last=iCallbackInfoArray.Count()-1;
	__ASSERT_DEBUG(last>=0,User::Panic(KOplAPI,1));
	iCallbackInfoArray[last].iParamCount++;
	TRAPD(err,iStack->Push(aString)); // checks room for string
	if ((err!=KErrNone) || (iStack->FreeBytes()<(TInt)(2*sizeof(TInt32))))	// type,count
		{
		ResetStack();
		User::LeaveNoMemory();
		}
	iStack->Push(KTypeString);
	}

EXPORT_C TInt OplAPI::CallProcedure(TReturnType aReturnType)
	{
	iRuntime->SetStateFlags(KInCallback);
	TInt last=iCallbackInfoArray.Count()-1;
	__ASSERT_DEBUG(last>=0,User::Panic(KOplAPI,1));
	iStack->Push(iCallbackInfoArray[last].iParamCount);
	CFrame* iCurrentFrame=iRuntime->Frame();
	iRuntime->SetFrame(NULL); // makes errors return here
	TInt err;
	TRAP(err,OpCode::DoCallProcByStringExpr(*iStack,*iRuntime,NULL,(TUint8)aReturnType));
	if (err!=KErrNone || (err=iRuntime->Execute())!=KErrNone)
		{
		ResetStack();
		}
	else
		iCallbackInfoArray.Delete(iCallbackInfoArray.Count()-1);
	iRuntime->SetFrame(iCurrentFrame);
	iRuntime->ClearStateFlags(KInCallback);
	return err;
	}

void OplAPI::ResetStack()
	{
	TInt last=iCallbackInfoArray.Count()-1;
	SetStackPtr(iCallbackInfoArray[last].iStackPtr);
	iCallbackInfoArray.Delete(last);
	}

EXPORT_C TUid OplAPI::AppUid()
	{
	return iRuntime->CommandLine().AppUid();
	}

EXPORT_C TRequestStatus& OplAPI::NewRequestL(TInt16* aUserStatusPtr,TInt aPriority,TCallBack aCallBack)
	{
	return iRuntime->IOCollection().NewRequestL(aUserStatusPtr,aPriority,aCallBack);
	}

EXPORT_C TRequestStatus& OplAPI::NewRequestL(TInt32* aUserStatusPtr,TInt aPriority,TCallBack aCallBack)
	{
	return iRuntime->IOCollection().NewRequestL(aUserStatusPtr,aPriority,aCallBack);
	}

EXPORT_C void OplAPI::WaitForEvent(TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired)
	{
	iRuntime->IOCollection().WsEventHandler().WaitForEvent(*iRuntime->ConEnv(), aEvent, aUserEventsRequired);
	}

EXPORT_C void OplAPI::WaitForEvent(TRequestStatus& aRequestStatus, TInt aPriority)
	{
	iRuntime->IOCollection().WsEventHandler().WaitForEvent(*iRuntime->ConEnv(), aRequestStatus, aPriority);
	}

EXPORT_C void OplAPI::WaitForEvent(TWsEvent& aEvent, TUserEventsRequired aUserEventsRequired, const TArray<MEventSource*>& aArrayOfAdditionalEventSources)
	{
	iRuntime->IOCollection().WsEventHandler().WaitForEvent(*iRuntime->ConEnv(), aEvent, aUserEventsRequired, aArrayOfAdditionalEventSources);
	}

EXPORT_C TPtrC OplAPI::AppCurrentDocument()
	{
	return iRuntime->CurrentDocumentName();
	}

EXPORT_C TPtrC OplAPI::AppLocation()
	{
	return iRuntime->CommandLine().ModuleName();
	}

EXPORT_C HBufC16* OplAPI::ConvertFromNarrowToUnicodeL(const TDesC8& aNarrowString,const TUint aCharacterSet) const
	{
	return COplRuntimeCharConv::ConvertFromNarrowToUnicodeL(aNarrowString,aCharacterSet);
	}

EXPORT_C HBufC16* OplAPI::ConvertFromNarrowToUnicodeLC(const TDesC8& aNarrowString,const TUint aCharacterSet) const
	{
	return COplRuntimeCharConv::ConvertFromNarrowToUnicodeLC(aNarrowString,aCharacterSet);
	}

EXPORT_C HBufC8* OplAPI::ConvertFromUnicodeToNarrowL(const TDesC16& aUnicodeString,const TUint aCharacterSet) const
	{
	return COplRuntimeCharConv::ConvertFromUnicodeToNarrowL(aUnicodeString,aCharacterSet);
	}

EXPORT_C HBufC8* OplAPI::ConvertFromUnicodeToNarrowLC(const TDesC16& aUnicodeString,const TUint aCharacterSet) const
	{
	return COplRuntimeCharConv::ConvertFromUnicodeToNarrowLC(aUnicodeString,aCharacterSet);
	}