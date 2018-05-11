// LB_MISC.CPP
//
// Copyright (c) 1997-2002 Symbian Ltd.All rights reserved.

#include <e32std.h>
#include <opcodes.h>
#include <e32hal.h>
#include <w32std.h>
#include "graphics.h"
#include <MdaAudioTonePlayer.h>
#if !defined(__SERIES60__)
#include <hlplch.h>
#endif
#if defined(__UIQ__)
#include <QikHelpLauncher.h>
#endif
const TInt KStateEscapeOff=16;

void OpCode::Escape(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	TInt newState=(aRuntime.IP8()==0)?0:1;
	TInt state=(aRuntime.StateFlags()&KStateEscapeOff)?0:1;
	if (state!=newState)
		{
		if (newState)
			{
			User::LeaveIfError(aRuntime.ConEnv()->RootWin().AddPriorityKey(EKeyEscape,EAllStdModifiers,KKillKeyModifier));
			User::LeaveIfError(aRuntime.ConEnv()->RootWin().AddPriorityKey(CTRL('s'),EAllStdModifiers,KPauseKeyModifier));
			User::LeaveIfError(aRuntime.ConEnv()->RootWin().AddPriorityKey(CTRL('q'),EAllStdModifiers,KPauseKeyModifier));
			aRuntime.ClearStateFlags(KStateEscapeOff);
			}
		else
			{
			aRuntime.ConEnv()->RootWin().RemovePriorityKey(EKeyEscape,EAllStdModifiers,KKillKeyModifier);
			aRuntime.ConEnv()->RootWin().RemovePriorityKey(CTRL('s'),EAllStdModifiers,KPauseKeyModifier);
			aRuntime.ConEnv()->RootWin().RemovePriorityKey(CTRL('q'),EAllStdModifiers,KPauseKeyModifier);
			aRuntime.SetStateFlags(KStateEscapeOff);
			}
		}
	}

void OpCode::Off(CStack& /*aStack*/, COplRuntime& /*aRuntime*/, CFrame* /*aFrame*/)
	{
	// OFF doesn't work on Crystal so until we find a fix, Leave
	User::Leave(KErrNotSupported);
//	UserHal::SwitchOff();
	}

void OpCode::OffFor(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFrame*/)
	{
	// OFF doesn't work on Crystal so until we find a fix, Leave (popping the stack
	// first to ensure compliant behaviour with the old keyword)
	aStack.PopInt16();
	User::Leave(KErrNotSupported);
/*	const TInt time=aStack.PopInt16();
	if (time<2)
		User::Leave(KErrArgument);
	else if (time==2)
		return;
	RTimer timer;
	User::LeaveIfError(timer.CreateLocal());
	TTime switchOnTime;
	switchOnTime.HomeTime();
	switchOnTime+=TTimeIntervalSeconds(time);
	TRequestStatus stat;
	timer.At(stat,switchOnTime);
	UserHal::SwitchOff();
	timer.Cancel();
	User::WaitForRequest(stat); // returns straightaway as the timer has been canceled
	timer.Close();
*/	}

class TOplAudioObserver : public MMdaAudioToneObserver
	{
public:
	inline TOplAudioObserver() :iError(KErrNone) {}
	inline TInt Error() const {return iError;}
	void MatoPrepareComplete(TInt aError);
	void MatoPlayComplete(TInt aError);
private:
	TInt iError;
	};

void TOplAudioObserver::MatoPrepareComplete(TInt aError)
	{
	iError=aError;
	CActiveScheduler::Stop();
	}

void TOplAudioObserver::MatoPlayComplete(TInt aError)
	{
	iError=aError;
	CActiveScheduler::Stop();
	}

/*
 * Beep using the Media Server
 */
void OpCode::Beep(CStack& aStack, COplRuntime& /*aRuntime*/, CFrame* /*aFrame*/)
	{
	// BEEP Time%,Pitch%
	// Time% is in 1/32 second units and Pitch% is 512/(Pitch%+1) KHz
	TInt pitch=(TUint16)(aStack.PopInt16());
	TInt time((aStack.PopInt16()));

	TOplAudioObserver toneObserver;
	CMdaAudioToneUtility *tonePlayer=CMdaAudioToneUtility::NewL(toneObserver);
	CleanupStack::PushL(tonePlayer);

	const TInt timeFactor=1000000/32;
	TTimeIntervalMicroSeconds duration(time*timeFactor);
	const TInt mediaPitch=512000/(pitch+1);
	tonePlayer->PrepareToPlayTone(mediaPitch,duration);
	CActiveScheduler::Start();

	User::LeaveIfError(toneObserver.Error());
	tonePlayer->SetVolume(tonePlayer->MaxVolume());
	tonePlayer->Play();
	CActiveScheduler::Start();

	User::LeaveIfError(toneObserver.Error());
	CleanupStack::PopAndDestroy(1); // tonePlayer
	}

void OpCode::ClearFlags(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	TInt flags=aStack.PopInt32();
	if (flags & ~KOplStateUserFlagsMask)
		User::Leave(KOplErrInvalidArgs);
	aRuntime.ClearUserFlags(flags);
	}

void OpCode::SetFlags(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	TInt flags=aStack.PopInt32();
	if (flags & ~KOplStateUserFlagsMask)
		User::Leave(KOplErrInvalidArgs);
	aRuntime.SetUserFlags(flags);
	}
	
const TInt KOplPointerFilterEnterExit = 0x001; 
const TInt KOplPointerFilterMove =	0x002; 
const TInt KOplPointerFilterDrag =	0x004; 
const TInt KOplPointerFilterMask =	KOplPointerFilterEnterExit|
									KOplPointerFilterMove|
									KOplPointerFilterDrag;

void OpCode::PointerFilter(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
#if EPointerFilterEnterExit-KOplPointerFilterEnterExit
	#error Wserv defines changed - the following code must be changed
#endif
#if EPointerFilterMove-KOplPointerFilterMove
	#error Wserv defines changed - the following code must be changed
#endif
#if EPointerFilterDrag-KOplPointerFilterDrag
	#error Wserv defines changed - the following code must be changed
#endif

	TInt mask=aStack.PopInt16();
	TInt filter=aStack.PopInt16();

	if (mask & ~KOplPointerFilterMask)
		User::Leave(KOplErrInvalidArgs);
	if (filter & ~KOplPointerFilterMask)
		User::Leave(KOplErrInvalidArgs);

	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	if (currentDrawable->DrawableFlag() == EIsOplBitmap)
		User::Leave(KOplErrInvalidArgs);
	else
		((COplWindow*)currentDrawable)->Window().PointerFilter(mask,filter);
	}

#if defined(__SERIES60__)
void OpCode::SetHelp(CStack&, COplRuntime&, CFrame* /*aFrame*/)
	{
	}
#else
void OpCode::SetHelp(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{ // SETHELP control%,context$
	TPtrC context=aStack.PopString();
	TUint16 controlIndex=aStack.PopInt16();
	if (context.Length()>KOplHelpMaxContextNameLen)
		User::Leave(KOplErrInvalidArgs);
	if (controlIndex>KOplHelpMaxControl)
		User::Leave(KOplErrInvalidArgs);
	TFixedArray<TCoeContextName,KOplLenContextNamesArray>* contextNames=aRuntime.HelpContextNamesArray();
	contextNames->At(controlIndex).Copy(context);
	}
#endif

#if defined(__SERIES60__)
void OpCode::ShowHelp(CStack& /*aStack*/, COplRuntime&, CFrame* /*aFrame*/)
	{
	}
#elif defined(__UIQ__)
void OpCode::ShowHelp(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{
	 // SHOWHELP
	//
	// Launch the context-sensitive help UI app, passing through the context id for the view.
	//
	TFixedArray<TCoeContextName,KOplLenContextNamesArray>* helpContext=aRuntime.HelpContextNamesArray();
	if (helpContext)
		{
		TCoeContextName context=helpContext->At(EOplHelpView);
		TUid helpUid=aRuntime.HelpUid();
		if (context.Length() && helpUid!=KNullUid)
			{
			CQikHelpLauncher::LaunchLD( TCoeHelpContext(helpUid,context));
			}
		}
	}
#else
void OpCode::ShowHelp(CStack& /*aStack*/, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{ // SHOWHELP
	//
	// Launch the context-sensitive help UI app, passing through the context id for the view.
	//
	CArrayFix<TCoeHelpContext>* viewHelpContext=new(ELeave) CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(viewHelpContext);

	TFixedArray<TCoeContextName,KOplLenContextNamesArray>* helpContext=aRuntime.HelpContextNamesArray();
	if (helpContext)
		{
		TCoeContextName context=helpContext->At(EOplHelpView);
		TUid helpUid=aRuntime.HelpUid();
		if (context.Length() && helpUid!=KNullUid)
			{
			viewHelpContext->AppendL(TCoeHelpContext(helpUid,context));
			}
		}
	HlpLauncher::LaunchHelpApplicationL(aRuntime.ConEnv()->WsSession(), viewHelpContext);
	CleanupStack::Pop(); // viewHelpContext
	}
#endif

#if defined(__SERIES60__)
void OpCode::SetHelpUid(CStack&, COplRuntime&, CFrame* /*aFrame*/)
	{
	}
#else
void OpCode::SetHelpUid(CStack& aStack, COplRuntime& aRuntime, CFrame* /*aFrame*/)
	{ // SETHELPUID Uid&
	TUid newUid(KNullUid);
	newUid.iUid=aStack.PopInt32();
	aRuntime.SetHelpUid(newUid);
	}
#endif
