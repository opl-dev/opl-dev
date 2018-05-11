// T_TRAN.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.
//
// Command line transaltor ala OPL1993 WATRAN
//

#include <f32file.h>
//#include <fbs.h>
#include <opltran.h>
#include <e32twin.h>
#include "textfile.h"

LOCAL_D RFs theFileServer;
LOCAL_D ROplTextSystem theTextSystem;
LOCAL_D CConsoleTextWin *theConsole=NULL;
LOCAL_D COplModuleTranslator *theTranslator=NULL;
LOCAL_D CTextTranslator::TDebugFlag theDebugFlag=CTextTranslator::ERelease;

LOCAL_D TBuf<0x400> aBuf(0x400);   // for console output
LOCAL_C void Printf(TRefByValue<const TDesC> aFmt,...)

//
// Print to the console
//
	{
	VA_LIST list;
	VA_START(list,aFmt);
	aBuf.Zero();
	aBuf.AppendFormatList(aFmt,list);
	theConsole->Write(aBuf);
	}

class CLineEdit : public CBase
    {
public:
	enum TCursorType {ECursorNone=0,ECursorNormal=20,ECursorInsert=100};
	enum TEditMode {EEditOverWrite,EEditInsert};
public:
	static CLineEdit* NewL(CConsoleBase* aConsole,TInt aMaxHistory);
	~CLineEdit();
	const TFileName& Edit(const TDesC& aPrompt);
	void SetDefaultMode(TEditMode aMode);
protected:
	CLineEdit();
	TPoint Where();
	TInt Lines();
	TInt WordLeft();
	TInt WordRight();
	void ClearLine();
	void ClearLast(TInt aCnt);
	void Recall();
	void Cursor();
	void Refresh();
private:
	CArrayFixFlat<HBufC*>* iHistory;
	CConsoleBase* iConsole; // Not owned
	TInt iMaxHistory;
	TInt iWidth;
	TInt iHeight;
	TInt iPos;
	TInt iOrg;
	TInt iLine;
	TInt iRecall;
	TEditMode iDefaultMode;
	TEditMode iCurrentMode;
	TFileName iBuf;
	TFileName iPrompt;
	};

CLineEdit::CLineEdit()
//
// Constructor
//
	{

//	iHistory=NULL;
//	iConsole=NULL;
//	iMaxHistory=0;
//	iWidth=0;
//	iHeight=0;
//	iPos=0;
//	iOrg=0;
//	iLine=0;
//	iRecall=0;
//	iDefaultMode=EEditOverWrite;
//	iCurrentMode=EEditOverWrite;
//	iBuf=_L("");
//	iPrompt=_L("");
	}


CLineEdit::~CLineEdit()
//
// Destroy the line editor
//
	{

	TInt count=iHistory->Count();
	while (count--)
		User::Free((*iHistory)[count]);
	delete iHistory;
	}

CLineEdit* CLineEdit::NewL(CConsoleBase* aConsole,TInt aMaxHistory)
//
// Create a new line editor
//
	{

	CLineEdit* pE=new(ELeave) CLineEdit;
	pE->iHistory=new(ELeave) CArrayFixFlat<HBufC*>(aMaxHistory+2);
	pE->iConsole=aConsole;
	pE->iMaxHistory=aMaxHistory;
	pE->iWidth=pE->iConsole->ScreenSize().iWidth;
	pE->iHeight=pE->iConsole->ScreenSize().iHeight;
	return(pE);
	}

TInt CLineEdit::Lines()
//
// The number of lines being edited.
//
    {

    TInt nL=1;
    if (((TInt)iBuf.Length())>=(iWidth-2-iOrg))
		nL+=(((TInt)iBuf.Length())+iOrg)/(iWidth-2);
    return(nL);
    }

TPoint CLineEdit::Where()
//
// Return the real cursor position.
//
    {

    if (iPos>=(iWidth-2-iOrg))
		return(TPoint((iPos+iOrg)%(iWidth-2),((iPos+iOrg)/(iWidth-2))+iLine));
	return(TPoint(iPos+iOrg,iLine));
    }

void CLineEdit::ClearLine()
//
// Clears the line being edited.
//
    {

    if (iBuf.Length())
		{
		TInt nL=Lines();
		while (nL--)
	    	{
	    	iConsole->SetPos(nL ? 0 : iOrg,iLine+nL);
	    	iConsole->ClearToEndOfLine();
	    	}
		iBuf.Zero();
		iPos=0;
		}
    }

void CLineEdit::ClearLast(TInt aCnt)
//
// Clears the last aCnt characters.
//
    {

    TInt aPos=iPos;
    iPos=((TInt)iBuf.Length())-aCnt;
    while (iPos<((TInt)iBuf.Length()))
		{
		TPoint p=Where();
		iConsole->SetCursorPosAbs(p);
		iConsole->ClearToEndOfLine();
		iPos+=(iWidth-p.iX);
		}
    iPos=aPos;
    }

void CLineEdit::Recall()
//
// Recall a line for editing.
//
    {

	if (iRecall!=(-1))
		{
		ClearLine();
		HBufC* pL=(*iHistory)[iRecall];
		iBuf=(*pL);
		iConsole->Write(iBuf);
		iPos=iBuf.Length();
		TInt nL=Lines();
		if ((iLine+nL)>(iHeight-2))
	    	iLine=iHeight-2-nL;
		iBuf=(*pL);
		}
    }

TInt CLineEdit::WordLeft()
//
// Position the cursor to the next word left.
//
    {

    TInt x=iPos-1;
    while (x && TChar(iBuf[x]).IsSpace())
		x--;
    while (x && TChar(iBuf[x]).IsGraph())
		x--;
    if (TChar(iBuf[x]).IsSpace())
		x++;
    return(x);
    }

TInt CLineEdit::WordRight()
//
// Position the cursor to the next word right.
//
    {

    TInt x=iPos;
    while (x<(TInt)iBuf.Length() && TChar(iBuf[x]).IsGraph())
		x++;
    while (x<(TInt)iBuf.Length() && TChar(iBuf[x]).IsSpace())
		x++;
    return(x);
    }

void CLineEdit::Cursor()
//
// Position the cursor.
//
    {

    iConsole->SetCursorPosAbs(Where());
    }

void CLineEdit::Refresh()
//
// Refresh the line.
//
    {

	iConsole->SetCursorHeight(ECursorNone);
    iConsole->SetPos(iOrg,iLine);
    iConsole->Write(iBuf);
	Cursor();
	iConsole->SetCursorHeight(iCurrentMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
    }

void CLineEdit::SetDefaultMode(TEditMode aMode)
//
// Set the default edit mode
//
	{

	iDefaultMode=aMode;
	}

const TFileName& CLineEdit::Edit(const TDesC& aPrompt)
//
// Start the editor or a single key fetch.
//
    {

    iPos=0;
    iPrompt=aPrompt;
	iBuf.Zero();
	iCurrentMode=iDefaultMode;
	iConsole->SetCursorHeight(iCurrentMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
    iConsole->Write(iPrompt);
	iOrg=iConsole->WhereX();
	iLine=iConsole->WhereY();
	iRecall=(-1);
    TInt hCount=iHistory->Count();
    if (hCount>iMaxHistory)
		hCount=iMaxHistory;
	FOREVER
		{
		TChar gChar=iConsole->Getch();
		switch (gChar)
	    	{
		case EKeyEscape:
	    	ClearLine();
			iRecall=(-1);
	    	break;
		case EKeyHome:
	    	iPos=0;
	    	Cursor();
	    	break;
		case EKeyLeftArrow:
	    	if (iPos)
                {
                if(iConsole->KeyModifiers()==EModifierCtrl)
                    {
                    iPos=WordLeft();
                    }
                else
    				{
    				iPos--;
    				}
                Cursor();
                }
	    	break;
		case EKeyRightArrow:
	    	if (iPos<((TInt)iBuf.Length()))
                {
                if(iConsole->KeyModifiers()==EModifierCtrl)
                    {
                    iPos=WordRight();
                    }
                else
     				{
    				iPos++;
    				}
                Cursor();
                }
	    	break;
		case EKeyEnd:
	    	iPos=((TInt)iBuf.Length());
	    	Cursor();
	    	break;
		case EKeyPageUp:
	    	if (hCount==0)
				break;
	    	iRecall=hCount-1;
	    	Recall();
	    	break;
		case EKeyUpArrow:
	    	if (iRecall==(-1))
				{
				if (hCount==0)
		    		break;
				iRecall=0;
				}
	    	else if (iRecall>=(hCount-1))
				{
				ClearLine();
				iRecall=(-1);
				break;
				}
	    	else
				iRecall++;
	    	Recall();
	    	break;
		case EKeyDownArrow:
	    	if (iRecall==(-1))
				{
				if (hCount==0)
		    		break;
				iRecall=hCount-1;
				}
	    	else if (iRecall==0)
				{
				ClearLine();
				iRecall=(-1);
				break;
				}
	    	else
				iRecall--;
	    	Recall();
	    	break;
		case EKeyPageDown:
	    	if (hCount==0)
				break;
	    	iRecall=0;
	    	Recall();
	    	break;
		case EKeyEnter:
	    	iConsole->SetCursorHeight(ECursorNone);
	    	iConsole->SetPos(0,iLine+Lines()-1); // CR on the last line
	    	iConsole->Write(_L("\n")); // Just a line feed
			iConsole->SetCursorHeight(iDefaultMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
	    	iRecall=(-1);
	    	if (((TInt)iBuf.Length())>2)
				{
				if (((TInt)iHistory->Count())==(iMaxHistory+1))
					{
					User::Free((*iHistory)[iMaxHistory]);
		    		iHistory->Delete(iMaxHistory);
					}
				HBufC* pB=iBuf.Alloc();
				if (pB!=NULL)
					{
					TRAPD(r,iHistory->InsertL(0,pB))
					};
				}
	    	return(iBuf);
		case EKeyBackspace:
	    	if (iPos)
				{
				TInt iN=1;
				if (iConsole->KeyModifiers()==EModifierCtrl)
		    		iN=iPos-WordLeft();
				ClearLast(iN);
				iPos-=iN;
				iBuf.Delete(iPos,iN);
				Refresh();
				}
	    	break;
		case EKeyDelete:
	    	if (iPos<((TInt)iBuf.Length()))
				{
				TInt iN=1;
				if (iConsole->KeyModifiers()==EModifierCtrl)
		    		iN=WordRight()-iPos;
				ClearLast(iN);
				iBuf.Delete(iPos,iN);
				Refresh();
				}
	    	break;
		case EKeyInsert:
	    	iCurrentMode=(iCurrentMode==EEditOverWrite ? EEditInsert : EEditOverWrite);
			iConsole->SetCursorHeight(iCurrentMode==EEditOverWrite ? ECursorNormal : ECursorInsert);
	    	break;
		default:
            if(iConsole->KeyModifiers()&EModifierAlt)
                {
                if(hCount && gChar>='0' && gChar<='9')
                    {
    	        	iRecall=(gChar-TChar('0'));

    	    	    if (iConsole->KeyModifiers()&EModifierShift)
           				iRecall+=10;

    	        	if (iRecall>=hCount)
    		    		iRecall=hCount-1;
    	    	    Recall();
                    }
                break;
                }
	    	if (!gChar.IsPrint())
				break;
	    	if (iCurrentMode==EEditOverWrite && iPos<((TInt)iBuf.Length()))
				iBuf[iPos++]=(TText)gChar;
	    	else if (((TInt)iBuf.Length())<0x100)
				{
				TInt oL=Lines();
				TBuf<0x02> b;
				b.Append(gChar);
				iBuf.Insert(iPos++,b);
				TInt nL=Lines();
				if (nL!=oL)
		    		{
		    		iConsole->SetCursorHeight(ECursorNone);
		    		iConsole->SetPos(0,iLine+oL-1);
		    		iConsole->Write(_L("\n"));
		    		iConsole->SetPos(0,iLine);
		    		if (iLine+nL>iHeight-2)
						iLine=iHeight-2-nL;
		    		}
				}
			else
				{
				iConsole->Write(_L("\7"));
				break;
				}
			Refresh();
			}
		}
	}


LOCAL_C void Error(const TDesC& mess,TInt err)
//
//
//
	{
	Printf(_L("%S failed with error number %d\r\n"),&mess,err);
	TRequestStatus status;
	theConsole->Read(status);
	User::WaitForRequest(status);
	}

LOCAL_C TBool ParseCommandLine(TParse &aParse)
//
// Gets the command line from source. Returns ETrue if there is possible on
// the command line and EFalse otherwise
//
	{
	
	TBool ret=EFalse;
	TBuf<0x100> cmd;
	RProcess().CommandLine(cmd);
	cmd.Trim();
	TLex iLex(cmd);
	if (!iLex.Eos())
		{
		if (theFileServer.Parse(iLex.NextToken(),_L("*.OPL"),aParse)==KErrNone && iLex.Eos())
			ret=ETrue;
		}
	return ret;
	}

LOCAL_C void TranslateFile(const TDesC& aSource)
	{
	
	Printf(_L("Translating %S\r\n"),&aSource);
	TParse output;
	TInt r=theFileServer.Parse(_L(".OPO"),aSource,output);
	if (r!=KErrNone)
		Error(_L("Parsing output file name"),r);
	else
		{
		TSourceTranslateError anErr;
		TRAP(r,theTranslator->StartTranslateL(theTextSystem,aSource,output.FullName(),theDebugFlag,anErr));
		if (r!=KErrNone)
			Error(_L("Starting translation"),r);
		else
			{
			do
				{
				r=theTranslator->TranslateChunk();
				Printf(_L("."));
				} while (r==KErrNone);
			if (r!=KErrGeneral || anErr.Error()!=EErrSuccess)
				{				
				Printf(_L("\r\nTranslating failed with Error %d, at Offset %d in %S\r\n"), anErr.Error(),anErr.Position(),&anErr.Source());
				TRequestStatus status;
				theConsole->Read(status);
				User::WaitForRequest(status);
				}
			else
				Printf(_L("\r\n%S translated OK\r\n"),&aSource);
			}		
		}
	}


LOCAL_D void Translate(TParse& aSource)
//
// Checks for wild-cards and does the necessary
//
	{
	if (!aSource.IsWild())
		TranslateFile(aSource.FullName());
	else
		{
		RDir dir;
		TInt r=dir.Open(theFileServer,aSource.FullName(),KEntryAttNormal);
        
        if (r!=KErrNone)
			Error(_L("Opening directory"),r);
		else
			{
			TEntry entry;
			while (dir.Read(entry)==KErrNone)
                {
                theFileServer.Parse(entry.iName,aSource.FullName(),aSource);
				TranslateFile(aSource.FullName());
                }
			}
		}
	}

LOCAL_C TBool ProcessCommand(const TDesC& aCommandLine)
//
// Command line starting with !
//
	{
	
	TPtrC command=aCommandLine.Mid(1); // Stuff after the !
	TBool ret=EFalse;
	if (command.CompareF(_L("QUIT"))==0)
		ret=ETrue;
	else if (command.CompareF(_L("DEB"))==0)
		theDebugFlag=CTextTranslator::EDebug;
	else if (command.CompareF(_L("REL"))==0)
		theDebugFlag=CTextTranslator::ERelease;
	else // if (command.CompareF(_L("HELP")))
		Printf(_L("!QUIT to end\r\n!DEB - Produce debug ouput\r\n!REL - Produce release output\r\n!Help Display this message\r\nFilename to translate a file\r\n"));
	return ret;
	}

GLDEF_C TInt E32Main()
//
//
//
	{
	CTrapCleanup *trapCleanup=CTrapCleanup::New();

	TParse src;
	
	theFileServer.Connect();
	theTextSystem.Connect();
//	FbsStartup();
//	RFbsSession::Connect();

// ER5u problems -- Unresolved external CConsoleTextWin::NewL(Class TDesC16 const &,class TSize

	TRAPD(r,theConsole=CConsoleTextWin::NewL(_L("T_TRAN"),TSize(KDefaultConsWidth,KDefaultConsHeight)));
	if (r!=KErrNone || theConsole->Console().Control(_L("+Maximize +NewLine -Lock -Wrap"))!=KErrNone)
		User::Beep(440,5);
	else
		{
		TBool getKey=ETrue;
		
		TRAPD(r,theTranslator=COplModuleTranslator::NewL())
		if (r!=KErrNone)
			Error(_L("Allocating the translator"),r);
		else
			{
			TRAP(r,theTranslator->SetTarget(EOplTargetER1));
			if (r!=KErrNone)
				Error(_L("Setting translator target"),r);
			else if (ParseCommandLine(src)) // Passed something plausible on the command line 
				Translate(src);
			else
				{
				Printf(_L("Enter the name of the file to translate\r\n"));
				Printf(_L("Type '!Help' for instructions\r\n"));
				CLineEdit*editor=NULL;
				TRAP(r,editor=CLineEdit::NewL(theConsole,8));
				if (r!=KErrNone)
					Error(_L("Creating the line editor"),r);
				else
					{
					FOREVER
						{
						const TFileName& file=editor->Edit(theDebugFlag==CTextTranslator::ERelease ? _L("Release>") : _L("Debug>"));
						if (file.Length()==0)
							continue;
						if (file.Left(1).Compare(_L("!"))==0)
							{
							if (ProcessCommand(file))
								break;
							}
						else if ((r=theFileServer.Parse(file,_L("*.OPL"),src))!=KErrNone)
							Error(_L("Parsing the file name"),r);
						else
							Translate(src);
						}
					getKey=EFalse;
					}
				}
			}
		if (getKey)
			{
			TRequestStatus aStatus;
			theConsole->Read(aStatus);
			User::WaitForRequest(aStatus);
			}
		delete theTranslator;
		}

	delete theConsole;
//	RFbsSession::Disconnect();
	theTextSystem.Close();
	theFileServer.Close();
	delete trapCleanup;
	return  KErrNone;
	}