// TGRAPHIC.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#define __DEBUG

#include <e32std.h>
#include <graphics.h>
#include "tgraphic.h"
#include <eikenv.h>
#include <eikappc.h>


// dummy table for testing purposes only
OPCODEFUNC OpCode::OpCodeTable[] =
		{
		&FieldRightSideInt,
		&FieldRightSideLong
		};			  
										 
// another dummy!!
void COplRuntime::SetUserRaisedError(TOplRaised aRaised)
	{
	iRaised = aRaised;
	}

TKeyResponse COplRuntime::OfferUnhandledKeyL(TKeyEvent* aKeyEvent,TEventCode aType)
	{
    if (aType==EEventKey && aKeyEvent->code==CTRL('q'))
        {
        CmdQuitL();
    	return(EKeyWasConsumed);
		}

	TInt code = aKeyEvent->code;
	switch(code)
		{
	case '0' :
		iStack->Push((TInt16)0);	// 0 for visible = false
		return(EKeyWasConsumed);
	case '1' :
		iStack->Push((TInt16)1);	// 1 for visible = true
		return(EKeyWasConsumed);
	case '2' :
		iStack->Push((TInt16)2);	// specifies window 2 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case '3' :
		iStack->Push((TInt16)3);	// specifies window 3 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case '4' :
		iStack->Push((TInt16)4);	// specifies window 4 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case '5' :
		iStack->Push((TInt16)5);	// specifies window 5 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case '6' :
		iStack->Push((TInt16)6);	// specifies window 6 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case '7' :
		iStack->Push((TInt16)6);	// specifies window 7 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case '8' :
		iStack->Push((TInt16)6);	// specifies window 8 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case '9' :
		iStack->Push((TInt16)6);	// specifies window 9 as needed by gUse	and gClose
		return(EKeyWasConsumed);
	case 'w' :
		RunTests(1);	// gCreate & gBorder
		return(EKeyWasConsumed);
	case 'l' :
		RunTests(2);	// gLineBy
		return(EKeyWasConsumed);
	case 'u' :
		RunTests(3);	// gUse
		return(EKeyWasConsumed);
	case 'b' :
		RunTests(4);	// gBox
		return(EKeyWasConsumed);
	case 'c' :
		RunTests(5);	// gCls
		return(EKeyWasConsumed);
	case 'e' :
		RunTests(6);	// gEllipse
		return(EKeyWasConsumed);
	case 'v' :
		RunTests(7);	// gVisible
		return(EKeyWasConsumed);
	case 'n' :
		RunTests(8);	// gSetWin
		return(EKeyWasConsumed);
	case 'x' :
		RunTests(9);	// gClose
		return(EKeyWasConsumed);
	case 'i' :
		RunTests(10);	// gCreateBit
		return(EKeyWasConsumed);
	case 'a' :
		RunTests(11);	// gAt
		return(EKeyWasConsumed);
	case 'p' :
		RunTests(12);	// gPrintWord
		return(EKeyWasConsumed);
	case 'd' :
		RunTests(13);	// gOriginX,gOriginY,gWidth,gHeight & gRank
		return(EKeyWasConsumed);
	case 'o' :
		RunTests(14);	// gOrder
		return(EKeyWasConsumed);
	case 'm' :
		RunTests(15);	// gGMode
		return(EKeyWasConsumed);
	case 'f' :
		RunTests(16);	// gFill
		return(EKeyWasConsumed);
	case 'y' :
		RunTests(17);	// gPoly
		return(EKeyWasConsumed);
	case 's' :
		RunTests(18);	// gSaveBit
		return(EKeyWasConsumed);
	case 't' :
		RunTests(19);	// gTMode
		return(EKeyWasConsumed);
	case 'r' :
		RunTests(20);	// gPrintBoxText
		return(EKeyWasConsumed);
	case 'h' :
		RunTests(21);	// gScroll
		return(EKeyWasConsumed);
		}
	return(EKeyWasNotConsumed);
	}

void COplRuntime::Destruct()
	{
	Adt::Destroy(iConsole);
	Adt::Destroy(iStack);
	Adt::Destroy(iDrawablesCollection);
 	Adt::Destroy(iTest);		// destroy test
	CEikAppControl::Destruct();
	}

COplRuntime::COplRuntime()
	{
	}

void COplRuntime::EikonEnvironmentReadyL()
	{
	SetTheRuntime(this);
	iConsole=new(ELeave) CEikConsoleScreen;
	CleanupStack::PushL(iConsole);
	iConsole->ConstructL(_L("TESTCONS.CPP"),TSize(80,44));
	CleanupStack::Pop();	   
	iStack=CStack::NewL(2048);
	iConsole->DrawCursor();
	iDrawablesCollection = new(ELeave) CDrawablesCollection;
	CleanupStack::PushL(iDrawablesCollection);
	iDrawablesCollection->ConstructL();
	CleanupStack::Pop();
	iTest=new(ELeave) CTestGraphics;	// create it
	TBuf<80> buf;
	buf.Format(_L("Starting Tests\r\n"));
	iConsole->Write(buf);
	buf.Format(_L("Press w to run graphics tests\r\n"));
	iConsole->Write(buf);
	}

void COplRuntime::RunTests(TInt aTestNo)	   // This is where to add your tests
	{
	iTest->DoTest(aTestNo,*iStack, *TheRuntime(), iFrame);			// test it
	}



//
// Main
//

EXPORT_C CEikAppControl* EikMain()
    {
	CEikAppControl* appControl=new COplRuntime;
	appControl->SetAppControlFlags(EEikAppControlNoAppResourceFile);
	return(appControl);
    }

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(ETrue);
	}


void CTestGraphics::Destruct()
	{
	}

void CTestGraphics::DoTest(TInt aTestNo, CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	switch(aTestNo)
		{
	case 1 :
		Test1(aStack,aRuntime,aFramePtr);	// test gCreate	& gBorder
		return;
	case 2 :
		Test2(aStack,aRuntime,aFramePtr);   // test gLineBy
		return;
	case 3 :
		Test3(aStack,aRuntime,aFramePtr);   // test gUse
		return;
	case 4 :
		Test4(aStack,aRuntime,aFramePtr);	// test gBox
		return;
	case 5 :
		Test5(aStack,aRuntime,aFramePtr);	// test gCls
		return;
	case 6 :
		Test6(aStack,aRuntime,aFramePtr);	// test gEllipse
		return;
	case 7 :
		Test7(aStack,aRuntime,aFramePtr);	// test gVisible
		return;
	case 8 :
		Test8(aStack,aRuntime,aFramePtr);	// test gSetWin
		return;
	case 9 :
		Test9(aStack,aRuntime,aFramePtr);	// test gClose
		return;
	case 10 :
		Test10(aStack,aRuntime,aFramePtr);	// test gCreateBit
		return;
	case 11 :
		Test11(aStack,aRuntime,aFramePtr);	// test gAt
		return;
	case 12 :
		Test12(aStack,aRuntime,aFramePtr);	// test gPrintWord
		return;
	case 13 :
		Test13(aStack,aRuntime,aFramePtr);	// test gOriginX,gOriginY,gWidth,gHeight & gRank
		return;
	case 14 :
		Test14(aStack,aRuntime,aFramePtr);	// test gOrder
		return;
	case 15 :
		Test15(aStack,aRuntime,aFramePtr);	// test gGMode
		return;
	case 16 :
		Test16(aStack,aRuntime,aFramePtr);	// test gFill
		return;
	case 17 :
		Test17(aStack,aRuntime,aFramePtr);	// test gPoly
		return;
	case 18 :
		Test18(aStack,aRuntime,aFramePtr);	// test gSaveBit
		return;
	case 19 :
		Test19(aStack,aRuntime,aFramePtr);	// test gTMode
		return;
	case 20 :
		Test20(aStack,aRuntime,aFramePtr);	// test gPrintBoxText
		return;
	case 21 :
		Test21(aStack,aRuntime,aFramePtr);	// test gScroll
		return;
		}									
	}

void CTestGraphics::Test1(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TInt16)450); // x pos
	aStack.Push((TInt16)70); // y pos
	aStack.Push((TInt16)100); // width
	aStack.Push((TInt16)110); // height
	aStack.Push((TInt16)1); // visible
	FuncOpCode::gCreate(aStack,aRuntime,aFramePtr);
	TInt id = aStack.PopInt16();
	
	TInt num = 1;
	TInt* pNum;
	pNum = &num;
	aRuntime.SetIP((TUint8*)pNum);
	aStack.Push((TInt16)3);
	OpCode::gBorder(aStack,aRuntime,aFramePtr);		 

	aStack.Push((TInt16)400); // x pos
	aStack.Push((TInt16)20); // y pos
	aStack.Push((TInt16)200); // width
	aStack.Push((TInt16)200); // height
	aStack.Push((TInt16)1); // visible
	FuncOpCode::gCreate(aStack,aRuntime,aFramePtr);
	id = aStack.PopInt16();
	
	aRuntime.SetIP((TUint8*)pNum);
	aStack.Push((TInt16)1);
	OpCode::gBorder(aStack,aRuntime,aFramePtr);		
	}

void CTestGraphics::Test2(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TInt16)30);	// x offset 
	aStack.Push((TInt16)30);	// y offset
	OpCode::gLineBy(aStack,aRuntime,aFramePtr);

/*	aStack.Push((TInt16)30);
	aStack.Push((TInt16)-30);
	OpCode::gLineBy(aStack,aRuntime,aFramePtr);

	aStack.Push((TInt16)30);
	aStack.Push((TInt16)30);
	OpCode::gLineBy(aStack,aRuntime,aFramePtr);

	aStack.Push((TInt16)30);
	aStack.Push((TInt16)-30);
	OpCode::gLineBy(aStack,aRuntime,aFramePtr);	 */
	  

	}

void CTestGraphics::Test3(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt winId = aStack.PopInt16();
	aStack.Push((TInt16)winId);
	OpCode::gUse(aStack,aRuntime,aFramePtr);	
	}

void CTestGraphics::Test4(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TInt16)25);	// width of box
	aStack.Push((TInt16)35);	// height of box
	OpCode::gBox(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test5(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	OpCode::gCls(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test6(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TInt16)20);	// half x-axis length
	aStack.Push((TInt16)30);	// half y-axis length
	OpCode::gEllipse(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test7(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt num = aStack.PopInt16();
	TInt* pNum;
	pNum = &num;
	aRuntime.SetIP((TUint8*)pNum);
	OpCode::gVisible(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test8(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TInt16)100);	// new x pos
	aStack.Push((TInt16)100);	// new y pos
	aStack.Push((TInt16)150);	// new width
	aStack.Push((TInt16)100);	// new height	  
	TInt num = 4;
	TInt* pNum;
	pNum = &num;
	aRuntime.SetIP((TUint8*)pNum);
	OpCode::gSetWin(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test9(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(aStack.PopInt16());
	OpCode::gClose(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test10(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TInt16)50); // bitmap width
	aStack.Push((TInt16)40); // bitmap height
	FuncOpCode::gCreateBit(aStack,aRuntime,aFramePtr);
	TInt id = aStack.PopInt16();

	TPtrC filename = _L("c:\\temp\\test.pbm");
	aStack.Push(filename);
	FuncOpCode::gLoadBit(aStack,aRuntime,aFramePtr);
	id = aStack.PopInt16();					 
	}

void CTestGraphics::Test11(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push((TInt16)10);	// x pos
	aStack.Push((TInt16)10);	// y pos
	OpCode::gAt(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test12(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{

/*	COplDrawable* currentDrawable = aRuntime.DrawablesCollection().CurrentDrawable();
	RWindowGc& gc = (RWindowGc&)currentDrawable->DrawableGc();
	
	TPoint top = TPoint(40,50);
	gc.DrawLine(top,TPoint(top.iX+50,top.iY));
	
	CFbsFont* font = currentDrawable->Font();
	
	TPoint ascent = TPoint(top.iX,top.iY+font->AscentInPixels());
	gc.DrawLine(ascent,TPoint(ascent.iX+50,ascent.iY));
	
	TPoint height = TPoint(top.iX,top.iY+font->HeightInPixels());
	gc.DrawLine(height,TPoint(height.iX+50,height.iY));

	TRect rect = TRect(top,TPoint(top.iX+50,top.iY+font->HeightInPixels()));
	gc.DrawText(_L("gPrint"),rect,font->AscentInPixels());
															  */
	aStack.Push((TInt16)2);	
	OpCode::gPrintWord(aStack,aRuntime,aFramePtr);

	TPtrC text = _L("get World!");
	aStack.Push(text);
	aStack.Push((TInt16)70);
	FuncOpCode::gPrintClip(aStack,aRuntime,aFramePtr);	
	}

void CTestGraphics::Test13(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	if (aRuntime.DrawablesCollection().CurrentDrawable()->DrawableFlag() == COplDrawable::EIsOplWindow)
		{
		FuncOpCode::gOriginX(aStack,aRuntime,aFramePtr);
		TInt x = aStack.PopInt16();
		FuncOpCode::gOriginY(aStack,aRuntime,aFramePtr);
		TInt y = aStack.PopInt16();
		FuncOpCode::gWidth(aStack,aRuntime,aFramePtr);
		TInt width = aStack.PopInt16();
		FuncOpCode::gHeight(aStack,aRuntime,aFramePtr);
		TInt height = aStack.PopInt16();
		FuncOpCode::gRank(aStack,aRuntime,aFramePtr);
		TInt rank = aStack.PopInt16();
		TBuf<40> buf;
		buf.Format(_L(" x=%d, y=%d, w=%d, h=%d, r=%d \r\n"),x,y,width,height,rank);
		aRuntime.Console().Write(buf);
		}
	}

void CTestGraphics::Test14(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt16 ordPos = aStack.PopInt16();
	TInt16 id = aStack.PopInt16();
	aStack.Push(id); 
	aStack.Push(ordPos); 
	OpCode::gOrder(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test15(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(aStack.PopInt16());
	OpCode::gGMode(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test16(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt mode = aStack.PopInt16();
	aStack.Push((TInt16)160); // width
	aStack.Push((TInt16)150); // height
	aStack.Push((TInt16)mode); // draw mode
	OpCode::gFill(aStack,aRuntime,aFramePtr);	 
	
/*	aRuntime.DrawablesCollection().CurrentDrawable()->SetCursorPosition(TPoint(40,40));
	aStack.Push((TInt16)20);	// width of rect
	aStack.Push((TInt16)15);	// height of rect
	OpCode::gInvert(aStack,aRuntime,aFramePtr);		 */
	}

void CTestGraphics::Test17(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt16 array[11];
	array[0] = 20;	array[1] = 80;	// startting x and y pos
	array[2] = 4;	// five operations
	array[3] = 40*2;	array[4] = -40;
	array[5] = 40*2;	array[6] = 40;
	array[7] = -40*2;	array[8] = 40;
	array[9] = -40*2;	array[10] = -40;
	TInt16* arrayPtr;
	arrayPtr = &array[0];
	aStack.PushRef(*arrayPtr);
	OpCode::gPoly(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test18(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
/*	TInt num = 1;
	TInt* pNum;
	pNum = &num;
	aRuntime.SetIP((TUint8*)pNum);
	TPtrC filename = _L("c:\\temp\\test.pbm");
	aStack.Push(filename);
	OpCode::gSaveBit(aStack,aRuntime,aFramePtr);   */
		
	aStack.Push((TInt16)55);	// bold, italic
	OpCode::gStyle(aStack,aRuntime,aFramePtr);	  
	}

void CTestGraphics::Test19(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	aStack.Push(aStack.PopInt16());	// text mode
	OpCode::gTMode(aStack,aRuntime,aFramePtr);
	}

void CTestGraphics::Test20(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt num = 2;
	TInt* pNum;
	pNum = &num;
	aRuntime.SetIP((TUint8*)pNum);
	TPtrC text = _L("(gPrintb)");
	aRuntime.DrawablesCollection().CurrentDrawable()->SetCursorPosition(TPoint(25,30));
	aStack.Push(text);
	aStack.Push((TInt16)100);	// box width = 100
	OpCode::gPrintBoxText(aStack,aRuntime,aFramePtr);

	num = 3;
	aRuntime.DrawablesCollection().CurrentDrawable()->SetCursorPosition(TPoint(25,60));
	aRuntime.SetIP((TUint8*)pNum);
	aStack.Push(text);
	aStack.Push((TInt16)100);	// box width = 100
	aStack.Push((TInt16)3);		// alignment is center
	OpCode::gPrintBoxText(aStack,aRuntime,aFramePtr);

	num = 4;
	aRuntime.DrawablesCollection().CurrentDrawable()->SetCursorPosition(TPoint(25,90));
	aRuntime.SetIP((TUint8*)pNum);
	aStack.Push(text);
	aStack.Push((TInt16)100);	// box width = 100
	aStack.Push((TInt16)-1);		// alignment is right
	aStack.Push((TInt16)2);		// top clearance = 2
	OpCode::gPrintBoxText(aStack,aRuntime,aFramePtr);

	num = 5;
	aRuntime.DrawablesCollection().CurrentDrawable()->SetCursorPosition(TPoint(25,120));
	aRuntime.SetIP((TUint8*)pNum);
	aStack.Push(text);
	aStack.Push((TInt16)100);   // box width = 100
	aStack.Push((TInt16)2);	   // alignment is left
	aStack.Push((TInt16)2);	   // top clearance = 2
	aStack.Push((TInt16)3);	   // bottom clearance = 3
	OpCode::gPrintBoxText(aStack,aRuntime,aFramePtr);

	num = 6;
	aRuntime.DrawablesCollection().CurrentDrawable()->SetCursorPosition(TPoint(25,150));
	aRuntime.SetIP((TUint8*)pNum);
	aStack.Push(text);
	aStack.Push((TInt16)100);	// box width = 100
	aStack.Push((TInt16)3);		// alignment is center
	aStack.Push((TInt16)0);		// top clearance = 0
	aStack.Push((TInt16)0);		// bottom clearance = 0
	aStack.Push((TInt16)7);	// margin = 7
	OpCode::gPrintBoxText(aStack,aRuntime,aFramePtr);

	}

void CTestGraphics::Test21(CStack& aStack, COplRuntime& aRuntime, CFrame* aFramePtr)
	{
	TInt num = 2;
	TInt* pNum;
	pNum = &num;
	aRuntime.SetIP((TUint8*)pNum);

	aStack.Push((TInt16)10);	// x offset
	aStack.Push((TInt16)10);	// y offset
	OpCode::gScroll(aStack,aRuntime,aFramePtr); // doesn't work due to bug in wserv!
	}