// OT_EXPR.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Qcode for the evaluator translator
//

#include "ot_std.h"


const TInt KOperatorSeenShift=4;
const TInt KOperatorActionMask=0xf;
// The Opl actions. The rather truncated names are to try
// and stop the main table from being utterly unreadable
enum
	{
	ELErr, // Syntax error 
	ELStk, // Stack operator 
	ELAcc, // Accept operator 
	ELRed, // Reduce
	ELROB, // Reduce to an open bracket
	ELPer, // Percent modify top of stack
	ELDrp, // Drop - used for when Unary minus meets unary minus
	ELMis, // Case when you get EOE meeting Open-bracket.
	EHErr=ELErr<<KOperatorSeenShift,
	EHStk=ELStk<<KOperatorSeenShift,
	EHAcc=ELAcc<<KOperatorSeenShift,
	EHRed=ELRed<<KOperatorSeenShift,
	EHROB=ELROB<<KOperatorSeenShift,
	EHPer=ELPer<<KOperatorSeenShift,
	EHDrp=ELDrp<<KOperatorSeenShift,
	EHMis=ELMis<<KOperatorSeenShift
	};


LOCAL_D const TUint8 colArray[20]=
	{
	0,           // $
    1,1,1,1,1,1, // <,<=,>,>=,=,<>
    2,2,         // +,-
    3,3,         // *,/
    4,           // **
    7,7,          // &,|
    6,			 // !
    5,           // U
    8,           // %
    6,           // (
    9,           // )
    10            // E
	};

LOCAL_D const TUint8 rowArray[18]=
	{
    0,           // $
    2,1,2,1,1,1, // <,<=,>,>=,=,<>.  < & > are different 'cos of 1<2%
    3,3,         // +,-
    4,4,         // *,/
    5,           // **
    8,8,         // &, |
    7,           // !
    6,           // U
    0,           // dummy for % ejhich can never be on stack 
    9            // (
	};

LOCAL_D const TUint8 precedenceTable[10][11]=
//     $     <,<=,>,>=,<>,=      +,-         *,/          **           U           !,(         &,|          %            )            E
{																               
// $															               
{ EHErr|ELErr, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELStk, EHErr|ELErr, EHErr|ELErr, EHErr|ELAcc },
// <= >= <> = 													               
{ EHErr|ELErr, EHErr|ELRed, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELRed, EHErr|ELErr, EHErr|ELROB, EHErr|ELRed },
// < >															               
{ EHErr|ELErr, EHErr|ELRed, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELRed, EHErr|ELPer, EHErr|ELROB, EHErr|ELRed },
// + -															               
{ EHErr|ELErr, EHErr|ELRed, EHErr|ELRed, EHErr|ELStk, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELRed, EHErr|ELPer, EHErr|ELROB, EHErr|ELRed },
// *, /															               
{ EHErr|ELErr, EHErr|ELRed, EHErr|ELRed, EHErr|ELRed, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELRed, EHErr|ELPer, EHErr|ELROB, EHErr|ELRed },
// **															               
{ EHErr|ELErr, EHErr|ELRed, EHErr|ELRed, EHErr|ELRed, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELRed, EHErr|ELErr, EHErr|ELROB, EHErr|ELRed },
// U															               
{ EHErr|ELErr, EHErr|ELRed, EHErr|ELRed, EHErr|ELRed, EHErr|ELStk, EHDrp|ELDrp, EHStk|ELErr, EHErr|ELRed, EHErr|ELRed, EHErr|ELROB, EHErr|ELRed },
// !
{ EHErr|ELErr, EHErr|ELRed, EHErr|ELRed, EHErr|ELRed, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELRed, EHErr|ELRed, EHErr|ELROB, EHErr|ELRed },
// & |															               
{ EHErr|ELErr, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELRed, EHErr|ELErr, EHErr|ELROB, EHErr|ELRed },
// (															               
{ EHErr|ELErr, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHErr|ELStk, EHStk|ELErr, EHStk|ELErr, EHErr|ELStk, EHErr|ELErr, EHErr|ELROB, EHErr|ELMis }
};


/////////////////////////////////////////////////////////////////////
//
// TEvaluatorState - used to track the state of the expression evaluator
//
/////////////////////////////////////////////////////////////////////
class TEvaluatorState
	{
public:
	inline TEvaluatorState();
	inline void SetOperator(TOplToken anOperator);
	inline TOplToken Operator();
	inline void SetOperatorSeen();
	inline void ClearOperatorSeen();
	inline TInt OperatorSeen();
	inline TOplToken::TType Type();
	inline void SetType(TOplToken::TType aType);
	inline TInt NestingLevel();
	inline void IncNesting();
	inline void DecNesting();
private:
	TOplToken iOperator; // Operator we are currently considering
	TInt iOperatorSeen;
	TOplToken::TType iType;
	TInt iNestingLevel;
	};

// TEvaluatorState
inline TEvaluatorState::TEvaluatorState()
	: iOperator(TOplToken::EStartOfExpression),iOperatorSeen(ETrue),iType(TOplToken::EWord),iNestingLevel(0)
	{}
inline void TEvaluatorState::SetOperator(TOplToken anOperator) {iOperator=anOperator;}
inline TOplToken TEvaluatorState::Operator() {return iOperator;}
inline void TEvaluatorState::SetOperatorSeen() { iOperatorSeen=KOperatorSeenShift;}
inline void TEvaluatorState::ClearOperatorSeen() { iOperatorSeen=0;}
inline TInt TEvaluatorState::OperatorSeen() { return iOperatorSeen;}
inline void TEvaluatorState::SetType(TOplToken::TType aType) {iType=aType;}
inline TOplToken::TType TEvaluatorState::Type() {return iType;}
inline TInt TEvaluatorState::NestingLevel() {return iNestingLevel;}
inline void TEvaluatorState::IncNesting() {iNestingLevel++;}
inline void TEvaluatorState::DecNesting() {iNestingLevel--;}

////////////////////////////////////////////////////////////////
//
// COplParserBase
//
///////////////////////////////////////////////////////////////////

void COplParserBase::StackOperatorL(TEvaluatorState& aState,TOplToken anOperator, TOplToken::TType aLeftSideType)
//
// Stacks an operator 
//	
	{

	TStackedOperator op(anOperator,aLeftSideType,CodeSizeL(),Lexer().TokenOffset());
	iOperatorStack.PushL(&op);
	aState.SetOperatorSeen();
	}

TStreamPos COplParserBase::CodeSizeL()
//
// gets the size of the PCode in the buffer so far
//
	{
	return iCode.Sink()->SeekL(0,EStreamEnd);
	}

void COplParserBase::FindOperatorL(TEvaluatorState& aState)
//
// Trucks through the expression to the next operator
//
// Identifiers and function calls go straight out into the PCODE
// The operators and punctuation sometimes need a little bit of massaging
// which is why this looks as unweildy as it does.
//
	{

	TUint found=EFalse;
	do
		{
		TOplToken next=Lexer().LexL();
		TOplToken::TClass tokenClass=next.Class();
		
		switch (tokenClass) 
			{
			case TOplToken::EReserved: // All kinds of things are forbidden in an expression
			case TOplToken::EStructure:
			case TOplToken::EKeyword:
				SyntaxErrorL();

			case TOplToken::EIdentifier:	// Something with value - not an operator
			case TOplToken::ECall:
				{
				if (!aState.OperatorSeen())		// Haven't seen an operator lately... 
					SyntaxErrorL(); 		    // ... something like '1+2 fred'

				// Filter for functions which return the native type and set type accordingly
				if (tokenClass==TOplToken::ECall)
					aState.SetType(CallL(next));
				else
					{
					aState.SetType(Lexer().Type());
					RightSideReferenceL(next);
					}
				aState.ClearOperatorSeen();
				}
				break;
			
			case TOplToken::EPunctuation: // Punctuation we treat as End of Expression.
				Lexer().UnLex();
				aState.SetOperator(TOplToken(TOplToken::EEndOfExpression));
				found=ETrue;
				break;
			
			case TOplToken::EOperator:
				{
				found=ETrue; // By and large, but we have to check ... 				
				aState.SetOperator(next);
				switch (next)		// .. some operators needs some special attention
					{
					case TOplToken::ECloseBracket:
						if (aState.NestingLevel()!=0)
							aState.DecNesting();
						else
							{
							Lexer().UnLex();
							aState.SetOperator(TOplToken(TOplToken::EEndOfExpression));
							}
						break;
					case TOplToken::EOpenBracket:
						aState.IncNesting();
						break;
					case TOplToken::EPerc:
						if (aState.OperatorSeen()) // i.e. 1+%a - % marks  a caharcter constant
							{
							PCodeL(TPcode::EConstant);
							iCode<<TOplConstant(TInt16(Lexer().NextChar()));
							found=EFalse; // In this case it's just like an operand
							aState.SetType(TOplToken::EWord);
							aState.ClearOperatorSeen();
							}
						break;
					case TOplToken::EMinus:
						if (aState.OperatorSeen()) // i.e. -3, or 1+-3						
							aState.SetOperator(TOplToken(TOplToken::EUnaryMinus));
					}
				break;
				}
			}
		} while (!found);
	}

			   

TOplToken::TType COplParserBase::PromoteL(TOplToken::TType from,TOplToken::TType to,TStreamPos qcodeOffset)
//
// Do a if it promotes to a larger size	
//
	{
	
	if (from>=to) // Only cast to 'larger' size
		return from;
	
	// Insert the Qcode at the relevant part of the stream
	iCode.Sink()->SeekL(MStreamBuf::EWrite,qcodeOffset);
	PCodeCastL(from,to);
	iCode.Sink()->SeekL(MStreamBuf::EWrite,EStreamEnd);
	return to;
	}

void COplParserBase::PCodeOperatorL(TOplToken::TValue anOperator,TPcodeOperator::TArity anAriness,TOplToken::TType anOperandType, TOplToken::TType aResultingType)
	{
	PCodeL(TPcode::EOperator);
	iCode<<anOperandType<<aResultingType<<anAriness<<anOperator;
	}

void  COplParserBase::OutputOperatorL(TEvaluatorState &aState)
//
// Outputs the operator at the top of the stack.
// Much of this is checking for illegal types e.g. "fred"*"sally"
// Also, working out what the resulting type is e.g. 1.2<.4 is an integer
//
// !! - Needs to restore the operator position so we can get an error
// in the right place ERROR HANDLING
//
	{

	
	TStackedOperator oper;
	TPcodeOperator::TArity ariness;
	iOperatorStack.Pop(&oper);
	iError->SetPosition(oper.Offset()); // so that casting errors position us to the right place in the source

	TOplToken::TType leftSideType=oper.Type();
	TOplToken::TType rightSideType=aState.Type();
	TOplToken::TType resultingType;

	if (oper==TOplToken::EUnaryMinus || oper==TOplToken::ENot) // Unary operator
		{
		if (rightSideType==TOplToken::EString) // ONLY has right side
			TypeMismatchL();
		resultingType=rightSideType;
		if (oper==TOplToken::ENot && rightSideType==TOplToken::EReal)
			resultingType=TOplToken::EWord;
		ariness=TPcodeOperator::EUnary;
		}
	else
		{
		if (oper>=TOplToken::EPercLess && oper<=TOplToken::EPercDivide)		// Make sure both sides are the same type
			{ // left and right side of percentage operators MUST be TOplToken::EReal
			leftSideType=PromoteL(leftSideType,TOplToken::EReal,oper.PcodePos());
			rightSideType=PromoteL(rightSideType,TOplToken::EReal,CodeSizeL());
			resultingType=TOplToken::EReal;
			}
		else
			{
			leftSideType=PromoteL(leftSideType,rightSideType,oper.PcodePos());
			resultingType=rightSideType=PromoteL(rightSideType,leftSideType,CodeSizeL());
			
			// Strings can only be compared and added 
			if (rightSideType==TOplToken::EString && oper>TOplToken::EPlus)
				TypeMismatchL();
			// Some logical operators always result in type EWord
			if (oper<=TOplToken::ENotEqual)
				resultingType=TOplToken::EWord;
			else if (rightSideType==TOplToken::EReal && (oper==TOplToken::EAnd || oper==TOplToken::EOr))
				resultingType=TOplToken::EWord;
			}
		ariness=TPcodeOperator::EBinary;
		}
	aState.SetType(resultingType);
	
	PCodeOperatorL(oper,ariness,rightSideType,resultingType);
	iError->SetPosition(Lexer().TokenOffset()); // Now we've got rid of that, any errors are back at the end.
	}

struct TOplOpMap
	{
	TOplToken::TValue iOldOp;
	TOplToken::TValue iNewOp;
	};

LOCAL_D const TOplOpMap percOpsMap[6]=
	{
	{TOplToken::ELessThan,TOplToken::EPercLess},
	{TOplToken::EGreaterThan,TOplToken::EPercGreater},
	{TOplToken::EPlus,TOplToken::EPercPlus},
	{TOplToken::EMinus,TOplToken::EPercMinus},
	{TOplToken::EMultiply,TOplToken::EPercMultiply},
	{TOplToken::EDivide,TOplToken::EPercDivide}
	};


TBool COplParserBase::ActOnOperatorL(TEvaluatorState& aState) 
//
// Just found an operator...
//
	{
	
	TBool expressionAccepted=EFalse;
	TBool again;
	TUint col=colArray[aState.Operator().Number()];
	do
		{
		again=EFalse;		
		TUint row=rowArray[iOperatorStack.Top().Operator().Number()];
		switch ((precedenceTable[row][col]>>aState.OperatorSeen())&KOperatorActionMask)
			{
			case ELErr:
				SyntaxErrorL();
				break;
			case ELStk:
				StackOperatorL(aState,aState.Operator(),aState.Type());
				break;
			case ELAcc:
				iOperatorStack.Drop(); // Remove the start of expression token
				expressionAccepted=ETrue;
				break;
			case ELRed:
				OutputOperatorL(aState);
				again=ETrue;
				break;
			case ELPer: // Converts the Tos to a percent operator.
				{		// And outputs it as %operatators are right associative & have  really high precedence
				TStackedOperator& tos=iOperatorStack.Top();
				for (const TOplOpMap *pMap=&percOpsMap[0];;pMap++)
					{
					if (tos==pMap->iOldOp)
						{
						tos.SetOperator(TOplToken(pMap->iNewOp));
						OutputOperatorL(aState);
						break;
						}
					}
				}
				break;			
			case ELROB:
				while (iOperatorStack.Top()!=TOplToken::EOpenBracket)
					{
					if (iOperatorStack.Top()==TOplToken::EStartOfExpression)
						User::Leave(EErrMismatchedBracket);
					OutputOperatorL(aState);					
					}
				PCodeOperatorL(TOplToken::ECloseBracket,TPcodeOperator::EUnary,aState.Type(),aState.Type());
				aState.ClearOperatorSeen();
				// Drops that open bracket operator off the top of the stack
			case ELDrp: // Happens when unary minus follows unary minus.
				iOperatorStack.Drop();
				break;
			case ELMis: // Case when you get EOE meeting Open-bracket.
				User::Leave(EErrMismatchedBracket);			
			}
		} while (again);
	return expressionAccepted;
	}

TOplToken::TType COplParserBase::ExpressionL()
//
// Re-entrant expression evaluation.
// Operator precedence table used to convert in-fix expressions to post-fix QCODE.
//
//
// !! Need some stack depth checking to preserved against Sin(Sin(Sin(Sin..) death.
//
	{
	TEvaluatorState state;
	StackOperatorL(state,TOplToken::EStartOfExpression,TOplToken::EWord);
	do
		{
		FindOperatorL(state);
		} while (!ActOnOperatorL(state));
	return state.Type();
	}


void COplParserBase::ExpressionL(TOplToken::TType aType)
//
// Gets an expression of a particular type;
//
	{

	PCodeCastL(ExpressionL(),aType);
	}


void COplParserBase::PCodeCastL(TOplToken::TType aFromType, TOplToken::TType aToType)
//
// Puts out a cast into the PCODE.
//
	{

	if (aFromType==aToType)
		return;
	PCodeL(TPcode::ECast);
	iCode<<aFromType;
	iCode<<aToType;
	}

void COplParserBase::WordExpressionL()
//
// An expression that is of (or is cast to) type Word
//
	{
	ExpressionL(TOplToken::EWord);
	}

void COplParserBase::LongExpressionL()
//
// An expression that is of (or can be cast to) type Long
//
	{
	ExpressionL(TOplToken::ELong);
	}

void COplParserBase::StringExpressionL()
//
// As it says an expression that leaves a string on the stack
//
	{
	ExpressionL(TOplToken::EString);
	}

void COplParserBase::NativeExpressionL()
//
// Word on Opl1993, Long on opler1
//	
	{
	ExpressionL(TargetIsOpl1993() ? TOplToken::EWord : TOplToken::ELong);
	}
