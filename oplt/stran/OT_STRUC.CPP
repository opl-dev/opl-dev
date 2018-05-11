// OT_STRUC.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// IF, DO, WHILE & VECTOR structures
//
//
#include "ot_std.h"

////////////////////////////////////////////////////////////////////////////
//
// TBranches - Used to keep track of branches used by structural stuff
//
////////////////////////////////////////////////////////////////////////////
TUint TBranches::Branch(TBranch aBranch)
//
// 
//
	{
	
	return iBranches[aBranch];
	}

TUint TBranches::NewBranch(TBranch aBranch)
//
// Gets the next branch number and assigns it to the branch of
// the right type
//
	{
	
	return iBranches[aBranch]=iNextBranch++;
	}

void TBranches::SaveBranchesL(TUint anArray[EMaxBranches])
//
// Squirrels away the branches before mucking with them and increments the depth
//
	{
	Mem::Copy(anArray,iBranches,sizeof(TUint)*EMaxBranches);
	if (++iDepth>KOplMaxStructureDepth)
		User::Leave(EErrTooComplex);
	}

void TBranches::RestoreBranches(TUint anArray[EMaxBranches])
//
// Restores saved branches and reduces the depth
//
	{
	Mem::Copy(iBranches,anArray,sizeof(TUint)*EMaxBranches);
	--iDepth;
	}

//////////////////////////////////////////////////////////////////////////////////
//
// COplModParser - Module parser for structural stuff
//
//////////////////////////////////////////////////////////////////////////////////
COplSymbol *COplModuleParser::LabelSymbolL(const TDesC& aLabel)
//
//	Check to see if aLabel has already been added. If not we add
// it as we assume this is a forward reference.
//
	{
	
	COplSymbol *pLabel=iSymbols->Find(aLabel,TOplToken::ELabel);
	if (pLabel==NULL) // Not in the table yet
		{
		pLabel=COplDeclarationSymbol::NewLC(COplSymbol::ELabel,TOplToken::ELabel,TOplToken::EBadType,aLabel);
		AddSymbolL(*pLabel);
		CleanupStack::Pop();
		}
	return pLabel;
	}

void COplModuleParser::DefineLabelL(const TDesC& aLabel)
//
//	Defines a Label & notes the definition in the PCode
//
	{

	// May already have been added due to forward reference
	COplLabelSymbol *pLabel=(COplLabelSymbol *)LabelSymbolL(aLabel);	
	
	// Check symbol for duplicate definition & mark definition seen
	if (pLabel->IsDefined())
		User::Leave(EErrDuplicateName);
	pLabel->Define();

	// And mark the definition the qcode	
	PCodeL(TPcode::ELabel);
	iCode<<(COplSymbol*)pLabel;
	}


void COplModuleParser::LabelReferenceL(TOplToken aToken)
//
// Parses a label reference
//
// label-reference := label || simple-id(real)
//
	{

	// Check it's OK
	if ((aToken!=TOplToken::ELabel) && (aToken!=TOplToken::ESimple || Lexer().Type()!=TOplToken::EReal))
		User::Leave(EErrMissingLabel);
	
	// Add it to the PCode	
	iCode<<LabelSymbolL(Lexer().Name());
	}

void COplModuleParser::LabelReferenceL()
//
// gets the next token and checks it out as being a good label reference
// syntacticly - carries no guarantees about the existence of teh label.
// 
	{
	
	LabelReferenceL(NextL());
	}

void COplModuleParser::NeedBranch(TBranches::TBranch aBranch)
//
// Says that we need a new branch of the passed type
//
	{
	iBranches.NewBranch(aBranch);
	}

void COplModuleParser::BranchName(TBranches::TBranch aBranch,TDes& aLabel)
//
// Makes the unique text for an internal label of the form .nnn
// e.g. 0x010a would have text \.\00a\000\001
//
	{
	
	aLabel.Zero();
	aLabel.Append(TChar('.')); // Illegal in OPL names
	for (TUint branchNo=iBranches.Branch(aBranch);branchNo;branchNo>>=2)
		aLabel.Append(TChar(branchNo&0xf));
	}

void COplModuleParser::SetBranchL(TBranches::TBranch aBranch)
//
// Adds the appropriate label for this branch
//
	{
	TBuf<KOplMaxIdentifierLength> branchLabel;

	BranchName(aBranch,branchLabel);
	DefineLabelL(branchLabel);
	}


void COplModuleParser::ConditionalExpressionL()
//
// Does the conditional expression and the branch
//
	{
	TOplToken::TType type=ExpressionL();
	if (type==TOplToken::EString)
		TypeMismatchL();
	if (type!=TOplToken::EWord)
		{
		PCodeL(TPcode::EConstant);
		if (type==TOplToken::ELong)
			//iCode<<TOplConstant(TInt32(1));
			iCode<<TOplConstant(TInt32(0));
		else
			//iCode<<TOplConstant(TReal64(1.0));
			iCode<<TOplConstant(TReal64(0.0));
		PCodeOperatorL(TOplToken::ENotEqual,TPcodeOperator::EBinary,type,TOplToken::EWord);
		}
	DoBranchL(TBranches::EFalse,TPcode::EBranchFalse);
	}

void COplModuleParser::DoBranchL(TBranches::TBranch aBranch, TPcode::TCode aBranchCode)
//
// Adds a branch to an internal label
//
	{

	PCodeL(aBranchCode);
	TBuf<KOplMaxIdentifierLength> branchLabel;
	BranchName(aBranch,branchLabel);
	iCode<<LabelSymbolL(branchLabel);
	}

void COplModuleParser::StructureL(TOplToken aToken)
//
// Deals with IF, DO,  WHILE & VECTOR structures.
// Basically LOTS of labels
//
//
// VectorStructure:= 
//	
	{
	
	
	TUint oldBranches[TBranches::EMaxBranches];
	iBranches.SaveBranchesL(oldBranches);
	switch (aToken)
		{
		case TOplToken::EVector: // VECTOR wordExpression Eos [label-lines]+ ENDV
			{
			// wordExpression EOS
			WordExpressionL();
			EosL();
			PCodeL(TPcode::EVector);
			
			// [label-line]+
			TOplToken next=SkipBlankLinesL();
			do	// label-line := label-ref[,label-ref]* EOS
				{
				FOREVER
					{
					PCodeL(TPcode::EVectorLabel);
					LabelReferenceL(next);
					if (NextIsL(TOplToken::EEos))
						break;
					CommaL();
					next=NextL();
					}
				next=SkipBlankLinesL();
				} while (next!=TOplToken::EEndV);
			PCodeL(TPcode::EEndVector);
			break;
			}
		case TOplToken::EDo:
		case TOplToken::EWhile:
			{
			iBranches.EnterLoop();
			NeedBranch(TBranches::EContinue);
			NeedBranch(TBranches::EBreak);
			NeedBranch(TBranches::EFalse);
			if (aToken==TOplToken::EDo) // DO statement-list UNTIL conditional-expr
				{
				SetBranchL(TBranches::EFalse);
				if (StatementListL()!=TOplToken::EUntil)
					User::Leave(EErrStructure);
				SetBranchL(TBranches::EContinue);
				ConditionalExpressionL();
				SetBranchL(TBranches::EBreak);
				}
			else						// WHILE conditional-expr statement-list ENDWH
				{
				SetBranchL(TBranches::EContinue);
				ConditionalExpressionL();
				if (StatementListL()!=TOplToken::EEndWh)
					User::Leave(EErrStructure);
				DoBranchL(TBranches::EContinue,TPcode::EBranchAlways);
				SetBranchL(TBranches::EBreak);
				SetBranchL(TBranches::EFalse);
				}
			iBranches.LeaveLoop();
			break;
			}
		case TOplToken::EIf: // IF if-body [ELSEIF if-body]* [ELSE statement-list] ENDIF
			{									 
			TOplToken token;
			NeedBranch(TBranches::EExit);		 
			do					//	if-body := conditional-expr EOS statement-list
				{
				NeedBranch(TBranches::EFalse);
				
				ConditionalExpressionL(); 
				EosL();
				token=StatementListL();
				
				if (token!=TOplToken::EEndIf)
					DoBranchL(TBranches::EExit,TPcode::EBranchAlways);
				SetBranchL(TBranches::EFalse); 
				} while (token==TOplToken::EElseIf);
			
			if (token==TOplToken::EElse) // [ELSE statment-list]
				token=StatementListL();

			if (token!=TOplToken::EEndIf) // ENDIF 
				User::Leave(EErrStructure);
			SetBranchL(TBranches::EExit);
			break;
			}
		default:
			Panic(EOpltParserUnknownStructure);
		}
	iBranches.RestoreBranches(oldBranches);
	}

