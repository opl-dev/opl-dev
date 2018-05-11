// OT_SYMBL.CPP
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

// Symbols & symbol table for the OPL tanslator
//
#include "ot_std.h"

///////////////////////////////////////////////////////
//
// COplSymbol
//
///////////////////////////////////////////////////////
COplSymbol::COplSymbol(COplSymbol::TClass aClass)
	: iClass(aClass)
	{
	}

///////////////////////////////////////////////////////
//
// COplDeclarationSymbol
//
///////////////////////////////////////////////////////

COplDeclarationSymbol *COplDeclarationSymbol::NewLC(TClass aClass, TOplToken aToken, TOplToken::TType aType, const TDesC& aName)
	{

	COplDeclarationSymbol *pSym=NULL;
	switch (aClass)
		{
		case ELabel:
			pSym=new(ELeave) COplLabelSymbol(aClass,aToken, aType);
			break;
		case EConstant:
			pSym=new(ELeave) COplConstantSymbol(aClass,aToken, aType);
			break;
		case EArgument:
		case EExternalDecl:
			pSym=new(ELeave) COplDeclarationSymbol(aClass,aToken, aType);
			break;
		case ELocal:
		case EGlobal:
			pSym=new(ELeave) COplAutomaticSymbol(aClass,aToken, aType);
			break;
		case EOpxFunction:
		case EProcDecl:
			pSym=new(ELeave) COplCallSymbol(aClass, aToken, aType);
			break;
		case EExternalRef:
		case EProcRef:
			Panic(EOpltIllegalDeclClass);
		}
   CleanupStack::PushL(pSym);
   pSym->ConstructL(aName);
   return pSym;
   }


COplReferenceSymbol *COplDeclarationSymbol::NewReferenceLC()
	{
	__ASSERT_DEBUG((Class()==EProcDecl || Class()==EExternalDecl), Panic(EOpltIllegalReferenceClass));

	COplReferenceSymbol *pRef=new(ELeave) COplReferenceSymbol(Class()==EProcDecl ? EProcRef : EExternalRef, *this);
	CleanupStack::PushL(pRef);
	return pRef;
	}

const TDesC& COplDeclarationSymbol::Name() const
//
// This is not inline as it is virtual
//
	{

	return *iName;
	}

TOplToken COplDeclarationSymbol::Token() const
//
// This is not inline as it is virtual
//
	{
	
	return iToken;
	}

TOplToken::TType COplDeclarationSymbol::Type() const
//
// This is not inline as it is virtual
//
	{

	return iType;
	}

COplDeclarationSymbol::~COplDeclarationSymbol()
//
// Delete the name
//
   {

   delete iName;
   }

COplDeclarationSymbol::COplDeclarationSymbol(TClass aClass, TOplToken aToken, TOplToken::TType aType)
	: COplSymbol(aClass),
	  iToken(aToken),
	  iType(aType)
	{
	}

void COplDeclarationSymbol::ConstructL(const TDesC& aName)
//
// Allocate a copy of the name
//
   {

   iName=aName.AllocL();
   }

///////////////////////////////////////////////////////////
//
// COplReferenceSymbol
//
///////////////////////////////////////////////////////////
const TDesC& COplReferenceSymbol::Name() const
	{
	return iDeclaration.Name();	
	}

TOplToken COplReferenceSymbol::Token() const
	{
	return iDeclaration.Token();
	}

TOplToken::TType COplReferenceSymbol::Type() const
	{
	return iDeclaration.Type();
	}

COplReferenceSymbol::COplReferenceSymbol(COplSymbol::TClass aClass, COplDeclarationSymbol& aDeclaration)
	: COplSymbol (aClass), iDeclaration(aDeclaration)
	{
	}

////////////////////////////////////////////////////////////
//
// COplConstantSymbol
//
/////////////////////////////////////////////////////////////
COplConstantSymbol::~COplConstantSymbol()
   {

	if (iValue.Type()==TOplToken::EString)
		delete (HBufC *)&iValue.AsStringL();		
	}

////////////////////////////////////////////////////
//
// COplLabelSymbol
//
////////////////////////////////////////////////////
COplLabelSymbol::COplLabelSymbol(TClass aClass, TOplToken aToken,TOplToken::TType aType)
   : COplDeclarationSymbol(aClass, aToken,aType), iReferences(KOplLabelReferenceGran)
//
// Safe construction
//
   {

   }

///////////////////////////////////////////////////////////////////////
//
// COplSymbolTable
//
///////////////////////////////////////////////////////////////////////

COplSymbolTable *COplSymbolTable::NewL()
//
// Gets a new symbol table - needs no second stage construction
//
   {

   return new(ELeave) COplSymbolTable();
   }

COplSymbolTable::~COplSymbolTable()
//
// Deletes all the symbols and tidies up the table
//
   {

   Reset();
   }

void COplSymbolTable::Reset()
//
// Resets the table deleting all the symbols
//
   {

   while (!iScopeQue.IsEmpty())
      Delete(iScopeQue.First());
   iCurrentScope=0;

#if defined(_DEBUG)
	TDblQue<COplSymbol> *pQ=&iTable[0];
	TDblQue<COplSymbol> *pE=&iTable[KOplHashTableSize];
	for (;pQ<pE;pQ++)
		__ASSERT_ALWAYS(pQ->IsEmpty(),Panic(EOpltQuesInconsistent));
#endif
   }


void COplSymbolTable::EndScope()
//
// Deletes all the symbols added since the last call to StartScope;
//
   {

   __ASSERT_ALWAYS(iCurrentScope>0,Panic(EOpltScopeUnderflow));
   while (!iScopeQue.IsEmpty())
      {
      COplSymbol *pSym=iScopeQue.First();
      if (pSym->iScope!=iCurrentScope)
         break;
      Delete(pSym);
      }
   iCurrentScope--;
   }

COplSymbol* COplSymbolTable::Find(const TDesC& aName, TOplToken aToken)
//
//
//
   {
   TDblQueIter<COplSymbol> iter(Hash(aName));
   COplSymbol *pSym=NULL;
   FOREVER
		{
		pSym=iter;
		if (pSym==NULL || (aName.Compare(pSym->Name())==0 && pSym->Token()==aToken))
			break;
		iter++;
		}
	return pSym;
   }

void COplSymbolTable::AddL(COplSymbol& aSymbol)
//
// Adds the symbol checking for duplicates within the current scope
//
	{

	COplSymbol *pSym=Find(aSymbol.Name(),aSymbol.Token());
	if (pSym!=NULL && pSym->iScope==iCurrentScope) // Already have that symbol in the current scope
		{
		// This is OK if we're adding a reference & the existing symbol is the declaration
		if (!((pSym->Class()==COplSymbol::EExternalDecl ||  pSym->Class()==COplSymbol::EProcDecl) && pSym==&((COplReferenceSymbol&)aSymbol).Declaration()))
			User::Leave(EErrDuplicateName);
		}   
	aSymbol.iScope=iCurrentScope;
	iScopeQue.AddFirst(aSymbol);
	Hash(aSymbol.Name()).AddFirst(aSymbol);
	}

void COplSymbolTable::Delete(COplSymbol* aSymbol)
//
// Removes a symbol from the table and deletes it
//
   {

   iScopeQue.Remove(*aSymbol);
   aSymbol->iHashLink.Deque();
   delete aSymbol;
   }

COplSymbolTable::COplSymbolTable()
   : iScopeQue(_FOFF(COplSymbol,iScopeLink))
//
// Stage 1 - 'safe' construction
//
	{
	TDblQue<COplSymbol> *pQ=&iTable[0];
	TDblQue<COplSymbol> *pE=&iTable[KOplHashTableSize];
	for (;pQ<pE;pQ++)
		new(pQ) TDblQue<COplSymbol>(_FOFF(COplSymbol,iHashLink));
	}

TDblQue<COplSymbol>& COplSymbolTable::Hash(const TDesC& aName)
//
// FOR NOW
//
	{
	TUint hashValue=0;
		
	// for now !! - grab better hash algorithm to spread stuff out.

	for (TInt i=0;i<aName.Length();i++)
		hashValue+=aName[i];
	return iTable[hashValue%KOplHashTableSize];
	}