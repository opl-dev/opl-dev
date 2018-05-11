// OT_KYWRD.CPP
//
// Copyright (c) 1997-2001 Symbian Ltd. All rights reserved.

#include "ot_std.h"

// Some useful numbers
const TInt KDefaultQualifier=0;

const TUint KPcPrintSpaceDelta=1;	// e.g. TOplKeyword::ELprintspace=TOplKeyword::ELprint+1
const TUint KPcPrintCrLfDelta=2;	//      TOplKeyword::ELprintspace=TOplKeyword::ELprint+2

//////////////////////////////////////////////////////////////////////////////
//
// COplParserBase
//
/////////////////////////////////////////////////////////////////////////////
void COplParserBase::PCodeCommandL(TOplKeyword::TCode aKeyword)
//
//
//
	{
	PCodeL(TPcode::ECommand);
	iCode<<aKeyword;
	}


//////////////////////////////////////////////////////////////////////////////
//
// COplModuleParser
//
/////////////////////////////////////////////////////////////////////////////
void COplModuleParser::KeywordStatementL()
//
// Have just lexed the start of a keyword statement.
// This just does a massive switch on the Parse Code for the keyword
//
	
	{
	
	TOplKeyword keyword=Lexer().Keyword();
	
	// Take care of TRAP statements as a special case - saves a recursion
	TBool isTrapped=EFalse;
	if (keyword.Code()==TOplKeyword::ETrap)
		{
		MustBeL(TOplToken::EKeywordToken); // Must be followed by a keyword
		keyword=Lexer().Keyword();
		if (!keyword.IsTrappable())
			SyntaxErrorL();
		isTrapped=ETrue;
		}

	// In most cases the switch just deals with arguments etc and then
	// we add the qcode at the end. However, in cases like PRINT we put out
	// the qcode within the switch statement itself in which case addQcode is set to EFalse.
	TBool addQcode=ETrue;
	
	TBool addQualifier=EFalse; // Some codes have a qualifier - usually to indicate 
	TInt32 qualifier=KDefaultQualifier; // how many optional arguments are on the stack
	
	TBool typeModified=EFalse; // Some things are type qualified 
	TOplToken::TType typeModifier=TOplToken::EBadType; // This is set to the type

	switch (keyword.Code())
		{
		// No Arguments
		case TOplKeyword::EAppend:		// APPEND
		case TOplKeyword::EBack:		// BACK
		case TOplKeyword::ECachetidy:	// CACHETIDY
		case TOplKeyword::EClose:		// CLOSE
		case TOplKeyword::ECls:			// CLS
		case TOplKeyword::EErase:		// ERASE
		case TOplKeyword::EFirst:		// FIRST
		case TOplKeyword::EGcls:		// GCLS
		case TOplKeyword::EIosignal:	// IOSIGNAL
		case TOplKeyword::EIoyield:		// IOYIELD
		case TOplKeyword::ELast:		// LAST
		case TOplKeyword::ELclose:		// LCLOSE
		case TOplKeyword::EMinit:		// MINIT
		case TOplKeyword::ENext:		// NEXT
		case TOplKeyword::EStop:		// STOP
		case TOplKeyword::EUpdate:		// UPDATE
		case TOplKeyword::EModify:		// MODIFY
		case TOplKeyword::EInsert:		// INSERT
		case TOplKeyword::ECancel:		// CANCEL
		case TOplKeyword::EPut:			// PUT
		case TOplKeyword::EBeginTrans:	// BEGINTRANS
		case TOplKeyword::ECommitTrans:	// COMMITTRANS
		case TOplKeyword::ERollBack:	// ROLLBACK
		case TOplKeyword::EShowHelp:	// SHOWHELP
			break;
		
		// 1 Word
		case TOplKeyword::ECachehdr:	// CACHEHDR addr(rec%)
		case TOplKeyword::EClosesprite: // CLOSESPRITE id%
		case TOplKeyword::EDefaultwin:	// DEFAULTWIN id%
		case TOplKeyword::EDiampos:		// DIAMPOS pos%
		case TOplKeyword::EGclose:		// GClose id%
		case TOplKeyword::EGgmode:		// GMODE mode%
		case TOplKeyword::EGgrey:		// GGREY mode%
		case TOplKeyword::EGstyle:		// GSTYLE style%
		case TOplKeyword::EGtmode:		// GTMODE mode%
		case TOplKeyword::EGunloadfont: // GUNLOADFONT fontid%
		case TOplKeyword::EGuse:		// GUSE id%
		case TOplKeyword::ELinklib:		// LINKLIB cat%
		case TOplKeyword::EPause:		// PAUSE time%
		case TOplKeyword::EPosition:	// POSITION pos%
		case TOplKeyword::ERaise:		// RAISE err%
		case TOplKeyword::EStyle:		// STYLE style%
		case TOplKeyword::EUsesprite:	// USESPRITE id%
		case TOplKeyword::EGotoMark:    // GOTOBOOKMARK mark%
		case TOplKeyword::EKillMark:	// KILLBOOKMARK mark%
		case TOplKeyword::EgSetPenWidth:// gSetPenWidth width%

			WordExpressionL();
			break;

		// 1 Word and 1 optional word
		case TOplKeyword::EGcircle:		// gCIRCLE radius%[,fill%]
			addQualifier=ETrue;
			WordExpressionL();
			if (NextIsCommaL())
				{
				qualifier++;
				WordExpressionL();
				}
			break;

		// 2 Words
		case TOplKeyword::EAt:			// AT x%,y%
		case TOplKeyword::EBeep:		// BEEP time%, pitch%
		case TOplKeyword::ECacherec:	// CACHEREC addr(rec%), offset%
		case TOplKeyword::EDposition:	// DPOSITION x%, y%
		case TOplKeyword::EDrawsprite:	// DRAWSPRITE x%, y%
		case TOplKeyword::EGat:			// GAT x%, y%
		case TOplKeyword::EGbox:		// GBOX width%, height%
		case TOplKeyword::EGinvert:		// GINVERT width%, height%
		case TOplKeyword::EGlineby:		// GLINEBY dx%, dy%
		case TOplKeyword::EGlineto:		// GLINETO x%, y%
		case TOplKeyword::EGmove:		// GMOVE dx%, dy%
		case TOplKeyword::EGorder:		// GORDER id%, position%
		case TOplKeyword::EPossprite:	// POSSPRITE x%, y%
		case TOplKeyword::EPointerFilter: // POINTERFILTER filter%,mask%
			WordExpressionsL(2);
			break;

		// 2 Words and 1 optional Word
		case TOplKeyword::EGellipse:	// gELLIPSE radiusx%,radiusy%[,fill%]
			addQualifier=ETrue;
			WordExpressionsL(2);
			if (NextIsCommaL())
				{
				qualifier++;
				WordExpressionL();
				}
			break;
		
		// 3 Words
		case TOplKeyword::EGcolor:		// GCOLOR red%,green%,blue%
		case TOplKeyword::EGcolorbackground:		// GCOLORBACKGROUND red%,green%,blue%
		case TOplKeyword::EGfill:		// GFILL width%, height%, gMode%
			WordExpressionsL(3);
			break;
		
		// 4 Words
		case TOplKeyword::EGdrawobject: // GDRAWOBJECT type%, flags%, w%, h%
		case TOplKeyword::EGpatt:
			WordExpressionsL(4);
			break;
		
		// 6 Words
		case TOplKeyword::EGcopy:		// GCOPY id%, x%, y%, w%, h%, mode%
			WordExpressionsL(6);
			break;
		
		case TOplKeyword::EFreealloc:	// FREEALLOC pCell& (was FREEALLOC pCell% on Opl1993)
		case TOplKeyword::EGfont:		// GFONT font& (was GFONT font% on Opl1993)
			NativeExpressionL();
			break;

        // 1 Long
		case TOplKeyword::ESetflags:	// SETFLAGS flags&
		case TOplKeyword::EClearflags:	// CLEARFLAGS flags&
			LongExpressionL();
			break;
		
		// 1 Native (word or long), 1 word
		case TOplKeyword::EFont:		// FONT id&, style% (hp: was FONT id%,style% on Opl1993)
			NativeExpressionL();
			CommaL();
			WordExpressionL();
			break;
	
        case TOplKeyword::EGbutton:		// GBUTTON text$, type%, w%, h%, st%[, bitMapId& [,maskId&[,layout%]]]
			StringExpressionL();
			CommaL();
			WordExpressionsL(4);
			if (!TargetIsOpl1993())
				{
				addQualifier=ETrue;
				if (NextIsCommaL())
					{
					LongExpressionL();	// bitMapId& (CFbsBitmap*)
					++qualifier;
					if (NextIsCommaL())
						{
						LongExpressionL();	// maskId&
						++qualifier;
						}
						if (NextIsCommaL())
							{
							WordExpressionL();	// layout%
							++qualifier;
							}
					}
				}
			break;
		
		case TOplKeyword::EChangesprite:// CHANGESPRITE ix%, tenths%, var bitmap$(),[dx%, dy%]
			// ix%
			WordExpressionL();
			CommaL();
		case TOplKeyword::EAppendsprite:// APPENDSPRITE tenths%,var bitmap$()[,dx%,dy%]
			addQualifier=ETrue;
			WordExpressionL(); // tenths%
			CommaL();
			Lexer().Mark(); // var bitmap$	
			MustBeL(TOplToken::EArray);
			if (Lexer().Type()!=TOplToken::EString)
				TypeMismatchL();
			MustBeL(TOplToken::ECloseBracket);
			Lexer().UnGetToMark();
			LeftSideReferenceL(NextL(),ESupplyIndices);			
			if (NextIsCommaL()) // [,dx%,dy%]
				{
				qualifier++;
				WordExpressionsL(2);
				}
			break;

		case TOplKeyword::EBreak:
		case TOplKeyword::EContinue:
			addQcode=EFalse;
			if (!iBranches.InLoop())
				User::Leave(EErrStructure);
			DoBranchL(keyword.Code()==TOplKeyword::EBreak ? TBranches::EBreak : TBranches::EContinue,
				TPcode::EBranchAlways);
			break;
		
		case TOplKeyword::EBusy:		// BUSY OFF|str$[,c%[,delay%]]
			addQualifier=ETrue;
			if (!NextIsOffL())
				{
				StringExpressionL();
				++qualifier;
				while (NextIsCommaL())
					{
					if (++qualifier>3) // too many args
						SyntaxErrorL();
					WordExpressionL();
					}
				}
			break;
		
		case TOplKeyword::ECache:		// CACHE ON | OFF | init%,max%
			addQualifier=ETrue;
			if (!NextIsOffOrOnL(qualifier))
				{
				qualifier++;
				WordExpressionsL(2);
				}
			break;
		
/*		case TOplKeyword::EDCheckBox:		// DCHECKBOX var choice%,p$
			IdentifierRefL(TOplToken::EWord);
			CommaL();
			StringExpressionL();
			break;
*/
		case TOplKeyword::EDeditMulti:		// dEditMulti ptrData&,prompt$,length%,numberOfLines%,maxLength&[,ReadOnly%]
			addQualifier=ETrue;
			LongExpressionL();
			CommaL();
			StringExpressionL();
			CommaL();
			WordExpressionL();
			CommaL();
			WordExpressionL();
			CommaL();
			LongExpressionL();
			if (NextIsCommaL())
				{
				qualifier++;
				WordExpressionL();
				}
			break;

		case TOplKeyword::EDchoice:			// dCHOICE var choice%,p$,list$[,IncrementalMatching%]
			addQualifier=ETrue;
			IdentifierRefL(TOplToken::EWord);
			CommaL();
			StringExpressionL();
			CommaL();
			StringExpressionL();
			if (NextIsCommaL())
				{
				qualifier++;
				WordExpressionL();
				}
			break;

		// 2 strings - fall-through is intentional
		case TOplKeyword::ECompress:	// COMPRESS src$, dest$
		case TOplKeyword::ECopy:		// COPY src$, dest$
		case TOplKeyword::ERename:
			StringExpressionL();
			CommaL();
		// 1 String
		case TOplKeyword::ECompact:		// COMPACT name$
		case TOplKeyword::ELoadm:		// LOADM module$
		case TOplKeyword::ELopen:		// LOPEN device$
		case TOplKeyword::EMkdir:		// MKDIR name$
		case TOplKeyword::ERmdir:		// RMDIR name$
		case TOplKeyword::ESetname:		// SETNAME name$
		case TOplKeyword::ESetdoc:		// SETDOC name$
		case TOplKeyword::ESetpath:		// SETPATH name$
		case TOplKeyword::EUnloadm:		// UNLOADM module$
			StringExpressionL();
			break;
		case TOplKeyword::EDelete:		// DELETE filename$[,table$]
			StringExpressionL();
			if (!(iTarget&(KOplTargetOpl1993))) // Not on OPL1993 machines
				{
				if (NextIsCommaL()) // Looks like delete table
					{
					StringExpressionL();
					keyword.SetCode(TOplKeyword::EDeleteTable);
					}
				}
			break;
		
		case TOplKeyword::ECreate:		// CREATE file$,log,f1$,..
		case TOplKeyword::EOpen:		// OPEN file$,log,f1$,..
		case TOplKeyword::EOpenr:		// OPENR file$,log,f1$,..
			{
			addQcode=EFalse;
			StringExpressionL();
			CommaL();
			if (isTrapped)
				PCodeCommandL(TOplKeyword::ETrap);
			PCodeL(TPcode::EQualifiedCommand);
			iCode<<keyword.Code()<<TUint8(LogicalDeviceL());
			CommaL();
			TUint fieldCount=0;
			do
				{
				if (fieldCount==KOplMaxFieldCount)
					User::Leave(EErrBadFieldList);
				if (!NextIsL(TOplToken::ESimple))
					User::Leave(EErrBadFieldName);
				PCodeL(TPcode::EFieldName);
				iCode<<Lexer().Type()<<Lexer().Name();
				fieldCount++;
				} while (NextIsCommaL());
			PCodeL(TPcode::EEndFieldList);
			}		
			break;
		case TOplKeyword::ECursor:		// CURSOR OFF | ON | id%[,asc%,w%,h%[,type%]]
			addQualifier=ETrue;
			if (!NextIsOffOrOnL(qualifier))
				{
				qualifier=CountWordExpressionsL(); // 1 4 or 5 OK
				switch (qualifier)
					{
					default:
						User::Leave(EErrFnArgument);
					case 1:
						qualifier=2; // ON is 1
						break;
					case 4:
						qualifier=3;
						break;
					case 5:
						qualifier=4;
						break;
					}
				}
			break;
		
		case TOplKeyword::EDtext:		// DTEXT p$, body$[, t%]
			StringExpressionL();
			CommaL();
		case TOplKeyword::EGiprint:		// GIPRINT str$[, c%]
			addQualifier=ETrue;	
			StringExpressionL();
			if (NextIsCommaL())
				{
				qualifier++;
				WordExpressionL();
				}
			break;
		
		case TOplKeyword::EDdate:		// DDATE var dt&,p$,min&,max&
		case TOplKeyword::EDlong:		// DLONG var lg&,p$,min&,max& 
			NumericDialogItemL(TOplToken::ELong);
			break;
		
		case TOplKeyword::EDfloat:		// DFloat var fp,p$,min,max
			NumericDialogItemL(TOplToken::EReal);
			break;
		
		case TOplKeyword::EDtime:		// DTIME vat tim&,p$,t%,min&,max&
			IdentifierRefL(TOplToken::ELong);
			CommaL();
			StringExpressionL();
			CommaL();
			WordExpressionL();
			CommaL();
			LongExpressionL();
			CommaL();
		case TOplKeyword::ERandomize:	// RANDOMIZE seed&
			LongExpressionL();
			break;
		
		case TOplKeyword::EDedit:		// DEDIT var str$, p$[, len%]
			IdentifierRefL(TOplToken::EString);
			CommaL();
			StringExpressionL();
			if (NextIsCommaL())
				{
				keyword.SetCode(TOplKeyword::EDSedit); // for some reason this is a different item type 
				WordExpressionL();
				}
			break;
		
		case TOplKeyword::EDxinput:		// DXINPUT var str$, p$[,SeedTextContents%]
			addQualifier=ETrue;
			IdentifierRefL(TOplToken::EString);
			CommaL();
			StringExpressionL();
			if (NextIsCommaL())
				{
				qualifier++;
				WordExpressionL();
				}
			break;

		case TOplKeyword::EDfile:		// DFILE var str$, p$, f%[,uid1=0,uid2=0,uid3=0]
			IdentifierRefL(TOplToken::EString);
			CommaL();
			StringExpressionL();
			CommaL();
			WordExpressionL();
			if (!TargetIsOpl1993())
				{
				TInt uidCount=0;
				for (;NextIsCommaL();++uidCount)
					{
					if (uidCount==3)
						User::Leave(EErrSyntax);
					LongExpressionL();
					}
				if (uidCount && uidCount!=3)
					User::Leave(EErrMissingComma);
				while (++uidCount<=3)
					{
					PCodeL(TPcode::EConstant);
					iCode<<TOplConstant(TInt32(0));
					}
				}
			break;

		case TOplKeyword::EGxprint:		// GXPRINT string$, flags%
			StringExpressionL();
			CommaL();
			WordExpressionL();
			break;
		
		case TOplKeyword::ESetHelp:		// SETHELP location%, helpId$
			WordExpressionL();
			CommaL();
			StringExpressionL();
			break;

		case TOplKeyword::ESetHelpUid:	// SETHELPUID Uid&
			LongExpressionL();
			break;
		
		case TOplKeyword::EDbuttons:	// DBUTTONS p1$, k1%[, p2$, k2%[, p3$, k3%]]
			addQualifier=ETrue;
			do
				{
				StringExpressionL();
				CommaL();
				WordExpressionL();
				qualifier++;
				} while (qualifier<50 && NextIsCommaL());	// no reason for limit really
			break;
		
		case TOplKeyword::EDiaminit:	// DIAMINIT [pos%[,str1$[,str2$[,...]]]		
			addQualifier=ETrue;
			if (!TestEosL())
				{
				qualifier++;
				WordExpressionL();
				while (NextIsCommaL())
					{
					qualifier++;
					StringExpressionL();
					}
				}
			break;
		
		case TOplKeyword::EDinits:		// DINITS [title$] - Only on Opl1994
		case TOplKeyword::EDinit:		// DINIT [title$] or DINIT title$[,flags%]
			addQualifier=ETrue;
			if (!TestEosL())
				{
				qualifier++;
				StringExpressionL();
				if (!TargetIsOpl1993())
					{
					if (NextIsCommaL())
						{
						qualifier++;
						WordExpressionL();
						}
					}
				}		
			break;
		
		case TOplKeyword::EEdit:		// EDIT str$ | str$(index) | a.str$
		case TOplKeyword::EInput:		// INPUT variable | field
			{
			TOplToken next=NextL();
			typeModifier=Lexer().Type();
			if (((next!=TOplToken::ESimple) && (next!=TOplToken::EArray) && (next!=TOplToken::EField))
				 || ((keyword.Code()==TOplKeyword::EEdit) && (typeModifier!=TOplToken::EString)))
				SyntaxErrorL();
			LeftSideReferenceL(next,EForceIndices);
			if (keyword.Code()==TOplKeyword::EInput)
				typeModified=ETrue;
			}
			break;
		
		// OFF | ON
		case TOplKeyword::EGvisible:	// GVISIBLE OFF | ON
		case TOplKeyword::EEscape:		// ESCAPE OFF | ON
		case TOplKeyword::ELock:
			addQualifier=ETrue;
			if (!NextIsOffOrOnL(qualifier))
				SyntaxErrorL();
			break;

		case TOplKeyword::EGborder:		// gBORDER flags%[, widths%, height%]
			addQualifier=ETrue;
			qualifier=CountWordExpressionsL();
			if ((qualifier!=1) && (qualifier !=3))
				SyntaxErrorL();
			break;

		case TOplKeyword::EGclock:		// GCLOCK OFF | ON [,mode%[,offset%[,format$,[font&[,style%]]]]]
			addQualifier=ETrue;
			if (!NextIsOffOrOnL(qualifier))
				SyntaxErrorL();
			if (qualifier!=0) // ON
				{
				if (NextIsCommaL())
					{
					qualifier++;
					WordExpressionL(); // mode%
					if (NextIsCommaL())
						{
						qualifier++;
						NativeExpressionL(); // offset& (was offset% on Opl1993)
						if (NextIsCommaL())
							{
							qualifier++;
							StringExpressionL(); // format$
							if (NextIsCommaL())
								{
								qualifier++;
								NativeExpressionL(); // font& (was font% on Opl1993)
								if (NextIsCommaL())
									{
									qualifier++;
									WordExpressionL(); // style%
									}
								}
							}
						}
					}
				}
			break;
		
		// var a%()
		case TOplKeyword::EGetevent:	// GETEVENT var a%()
		case TOplKeyword::EGinfo:		// GINFO var i%()
		case TOplKeyword::EGpoly:		// GPOLY l%()
		case TOplKeyword::EOdbinfo:		// ODBINFO var info%()
		case TOplKeyword::EScreeninfo:
			WordArrayRefL();
			break;

		// var int%, var a&()
		case TOplKeyword::EGeteventa32:	// GETEVENTA32 var stat%, var a&()
			ArgumentByRefL(TOplToken::EWord);
			CommaL();
			// drop through to var a&()
		// var a&()
		case TOplKeyword::EGetevent32:	// GETEVENT32 var a&()
		case TOplKeyword::EGinfo32:		// GINFO32 var i&()
		case TOplKeyword::EGcolorinfo:	// GCOLORINFO var i&()
			LongArrayRefL();
			break;


		case TOplKeyword::EGoto:		// GOTO label[::]
			addQcode=EFalse;	// BackEnd().BranchL deals with it.
			PCodeL(TPcode::EBranchAlways);
			LabelReferenceL();
			break;

		case TOplKeyword::EGpeekline:   // GPEEKLINE ix%, x%, y%, %(), ln%[,mode%=-1 (for opler1)]
			WordExpressionsL(3);// ix%, x%, y%
			CommaL();               
			WordArrayRefL();
			CommaL();
			WordExpressionL();      // ln%
			if (!TargetIsOpl1993())
				{
				if (NextIsCommaL())
					WordExpressionL();
				else
					{
					PCodeL(TPcode::EConstant);
					iCode<<TOplConstant(TInt16(-1));
					}
				}
			break;


		// These codes are followed immediately by their PrintSpace & PrintCrLf
		// variants & we rely on this.
		// NOTE PcGrint has no PrintCrLf variant 
		case TOplKeyword::EGprint:
		case TOplKeyword::ELprint:
		case TOplKeyword::EPrint: // PRINT  |  { PRINT expr [{;|,} expr]* [;|,]}
			{ 
			addQcode=EFalse;			
			TOplToken punctuator=TOplToken::EEos;
			while (!TestEosL())
				{
				TOplToken::TType type=ExpressionL();
				PCodeTypedCommandL(keyword.Code(),type);
		
				// Check for punctuation
				punctuator=NextL();
				if ((punctuator!=TOplToken::EComma) && (punctuator!=TOplToken::ESemiColon))
					{
					if (!IsEos(punctuator))  // E.g. PRINT x% y% as opposed to PRINT x% Eos
						SyntaxErrorL();
					Lexer().UnLex(); // Put back Eos
					}
				else if (punctuator==TOplToken::EComma)
					PCodeCommandL(TOplKeyword::TCode(keyword.Code()+KPcPrintSpaceDelta)); // PrintSpace variant
				}
			
			// By here punctuator is ;|,|Eos. For Eos add CrLf where allowed
			if (keyword.Code()!=TOplKeyword::EGprint && IsEos(punctuator))
				PCodeCommandL(TOplKeyword::TCode(keyword.Code()+KPcPrintCrLfDelta));
			}
			break;

		case TOplKeyword::EGprintb:		// GPRINTB t$,w%[,a1%[,tp%[,bt%[,m%]]]]
			addQualifier=ETrue;
			StringExpressionL();
			CommaL();
			qualifier=CountWordExpressionsL();
			if (qualifier>5)
				SyntaxErrorL();
			break;

		case TOplKeyword::EGsavebit:	// GSAVEBIT name$[, width%, height%]
			addQualifier=ETrue;
			StringExpressionL();
			if (NextIsCommaL())
				{
				qualifier++;
				WordExpressionsL(2);
				}
			break;

		case TOplKeyword::EGscroll:		// GSCROLL dx%, dy%[, x%, y%, dx%, dy%]
			addQualifier=ETrue;
			WordExpressionsL(qualifier=2);
			if (NextIsCommaL())
				{
				qualifier=6;
				WordExpressionsL(4);
				}			
			break;

		case TOplKeyword::EGsetwin:		// GSETWIN x%, y%[, width%, height %]
		case TOplKeyword::EGxborder:	// GXBORDER type%, flags%[,w%, h%]
			addQualifier=ETrue;
			WordExpressionsL(qualifier=2);
			if (NextIsCommaL())
				{
				qualifier=4;
				WordExpressionsL(2);
				}
			break;

		case TOplKeyword::EGxborder32:	// GXBORDER32 flags&[, w%, h%]
			addQualifier=ETrue;
			LongExpressionL();
			if (NextIsCommaL())
				{
				qualifier=3;
				WordExpressionsL(2);
				}
			break;

		case TOplKeyword::EGupdate:		// GUPDATE [OFF | ON]
			addQualifier=ETrue;
			if (!NextIsOffOrOnL(qualifier))
				qualifier=(-1);
			break;

		case TOplKeyword::EIowaitstat:	// IOWAITSTAT var status%
			ArgumentByRefL(TOplToken::EWord);
			break;

		case TOplKeyword::EIowaitstat32:	// IOWAITSTAT32 var status&
			ArgumentByRefL(TOplToken::ELong);
			break;

		case TOplKeyword::EMcard:		// MCARD title$[,n$,k%]+
		case TOplKeyword::EMcasc:		// MCASC cascId$[,n$,k%]+
			addQualifier=ETrue;
			StringExpressionL();
			CommaL();
			FOREVER
				{
				StringExpressionL();
				CommaL();
				WordExpressionL();
				qualifier++;
				if (!NextIsCommaL())
					break;
				}
			break;

		case TOplKeyword::EMcardX:		// MCARDX bitmapId%,maskId%[,n$,k%]+
			addQualifier=ETrue;
			WordExpressionL();
			CommaL();
			WordExpressionL();
			CommaL();
			FOREVER
				{
				StringExpressionL();
				CommaL();
				WordExpressionL();
				qualifier++;
				if (!NextIsCommaL())
					break;
				}
			break;

		case TOplKeyword::EOff:			// OFF [time%]
			if (!TestEosL()) 
				{
				keyword.SetCode(TOplKeyword::EOffTime);
				WordExpressionL();
				}
			break;
		
		case TOplKeyword::EOnerr:
			{
			addQcode=EFalse;
			PCodeL(TPcode::EOnErr);
			if (NextIsOffL())
				iCode<<(COplSymbol *)NULL;
			else
				LabelReferenceL();
			}			
			break;

		// NOTE THIS RELIES ON THE VALUES OF THE TYPES - SET IN OPLTOKEN.H
		case TOplKeyword::EPokes:		// POKE$ addr&, val$  (was addr% on Opl1993)
			qualifier++;
		case TOplKeyword::EPokef:		// POKEF addr&, val
			qualifier++;
		case TOplKeyword::EPokel:		// POKEL addr&, val&
			qualifier++;
		case TOplKeyword::EPokew:		// POKEW addr&, val%
		case TOplKeyword::EPokeb:		// POKEB addr&, val%
			ExpressionL(UnsignedNativeType());
			CommaL();
			ExpressionL(TOplToken::TType(qualifier));
			break;
		
		case TOplKeyword::EReturnValue:
			{
			iLastWasReturn=ETrue; // If this is the last statment then don't need to add default return
			typeModifier=iProcHeader->Type();
			if (TestEosL())
				{
				typeModified=ETrue;
				keyword.SetCode(TOplKeyword::EReturnNull);
				}
			else
				ExpressionL(typeModifier);
			break;
			}
		case TOplKeyword::EScreen:		// SCREEN x%, y%[, width%, height%]
			WordExpressionsL(2);
			if (NextIsCommaL())
				{
				keyword.SetCode(TOplKeyword::EGscreen);
				WordExpressionsL(2);
				}
			break;

		case TOplKeyword::EDaysToDate:		// DAYSTODATE d&[, var v%]3
			LongExpressionL();
			for (qualifier=3;qualifier;qualifier--)
				{
				CommaL();
				ArgumentByRefL(TOplToken::EWord);
				}
			break;

		case TOplKeyword::ESecstodate:		// SECSTODATE s&[, var v%]7
			LongExpressionL();
			for (qualifier=7;qualifier;qualifier--)
				{
				CommaL();
				ArgumentByRefL(TOplToken::EWord);
				}
			break;

		case TOplKeyword::EStatuswin:	// STATUSWIN OFF | ON[,type%]
			addQualifier=ETrue;
			if (!NextIsOffOrOnL(qualifier))
				SyntaxErrorL();
			if (qualifier!=0 && NextIsCommaL()) // [, type%] 
				{
				qualifier++;
				WordExpressionL();
				}
			break;
		case TOplKeyword::EUse:
			addQualifier=ETrue;
			qualifier=LogicalDeviceL();
			break;

		default:
		// None of the following can be obtained from the input token stream
		// They are modified cases of the 'reguler' ParseCodes.
		case TOplKeyword::EDSedit:
		case TOplKeyword::EGprintspace:
		case TOplKeyword::ELprintspace:
		case TOplKeyword::ELprintcrlf:
		case TOplKeyword::EPrintspace: 
		case TOplKeyword::EPrintcrlf:
		case TOplKeyword::EReturnNull:
		case TOplKeyword::EGscreen:
		case TOplKeyword::ETrap:		// This should have been picked out before the switch
			break;
		}
	
	// For vanilla cases we put out the qcode here	
	if (addQcode)
		{
		if (isTrapped)
			PCodeCommandL(TOplKeyword::ETrap);
		
		__ASSERT_ALWAYS(!(addQualifier && typeModified),Panic(EOpltKeywordTypedAndQualified));

		if (addQualifier)
			{
			PCodeL(TPcode::EQualifiedCommand);
			iCode<<keyword.Code()<<TUint8(qualifier);
			}
		else if (typeModified)
			PCodeTypedCommandL(keyword.Code(),typeModifier);
		else
			PCodeCommandL(keyword.Code());
		}
	}


void COplModuleParser::NumericDialogItemL(TOplToken::TType aType)
//
// e.g DLong var lg&, p$, min&, max&
//
	{
	IdentifierRefL(aType);
	CommaL();
	StringExpressionL(); // prompt$
	CommaL();
	ExpressionL(aType); // min
	CommaL();
	ExpressionL(aType); // max
	}

TUint COplModuleParser::CountWordExpressionsL()
//
// Parses comma separated word expressions while it can
//
	{
	
	TUint count=0;
	do
		{
		WordExpressionL();
		count++;
		} while (NextIsCommaL());
	return count;
	}

#pragma warning (disable:4706) // assignment within conditional expression
void COplModuleParser::WordExpressionsL(TInt aCount)
//
// Parses out aCount comma separated word expressions
// (count>=2)
	{
	FOREVER
		{
		WordExpressionL();
		if (!--aCount)
			break;
		CommaL();
		}
	}
#pragma warning (default:4706) 

TOplField::TDevice COplModuleParser::LogicalDeviceL()
//
// A logical device is A-D, i.e. a simple real identifier
// of length 1
//
	{
	MustBeL(TOplToken::ESimple);
	if (Lexer().Type()!=TOplToken::EReal || Lexer().Name().Length()!=1)
		User::Leave(EErrBadLogicalDevice);
	TUint device=Lexer().Name()[0]-'A';
	Lexer().CheckDeviceL(device);
	return TOplField::TDevice(device);
	}

void COplModuleParser::IdentifierRefL(TOplToken::TType aType)
//
// Does a type-safe left side ref
//
	{
	TOplToken next=NextL();
	if (next.Class()!=TOplToken::EIdentifier)
		SyntaxErrorL();
	if (Lexer().Type()!=aType)
		TypeMismatchL();
	LeftSideReferenceL(next,ESupplyIndices);
	}

void COplModuleParser::ArrayRefL(TOplToken::TType aType)
//
// Parses a reference to an array
//
	{

	TOplToken next=NextL();
	if (next!=TOplToken::EHash && (next!=TOplToken::EArray || Lexer().Type()!=aType))
		SyntaxErrorL();
	Lexer().UnLex();
	ArgumentByRefL();
	}

void COplModuleParser::WordArrayRefL()
//
// Parses a reference to a word array
//
	{

	ArrayRefL(TOplToken::EWord);
/*
	TOplToken next=NextL();
	if (next!=TOplToken::EHash && (next!=TOplToken::EArray || Lexer().Type()!=TOplToken::EWord))
		SyntaxErrorL();
	Lexer().UnLex();
	ArgumentByRefL();
*/
	}

void COplModuleParser::LongArrayRefL()
//
// Parses a reference to a long integer array
//
	{
	ArrayRefL(TOplToken::ELong);
	}

void COplModuleParser::PCodeTypedCommandL(TOplKeyword::TCode aKeyword,TOplToken::TType aType)
//
// Puts out PCode for a typed keyword
//
	{
	
	PCodeL(TPcode::ETypedCommand);
	iCode<<aKeyword<<aType;
	}