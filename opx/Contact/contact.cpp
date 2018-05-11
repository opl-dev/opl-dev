// Contact.cpp - Contacts OPX
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#include "Contact.h"
#include "opxutil.h"
						    
////////////////////////////////////////////////////////////////////////////////////////////
// CNTOpx class : derives from COpxBase
//
// The language extension procedures provided by this OPX
//

CPtrs::~CPtrs()
	{
	delete iCurrentItem;
	delete iSortedIds;
	delete iFoundIds;
//	CContactIdArray* iGroupIds;
	}

CCntOpx::~CCntOpx()
	{
	delete iPtrs;
	delete iDbHandle;
	}

void CCntOpx::CreateContactFileL(OplAPI& aOplAPI)
// create new contacts database
// COCreateContactFile:(aFileName$):
	{
	if (iDbHandle)
		User::Leave(KOplErrAlreadyOpen);
	iDbHandle = CContactDatabase::CreateL(aOplAPI.PopString());
	iPtrs = new (ELeave) CPtrs();
	aOplAPI.Push(0.0);
	}

void CCntOpx::OpenContactFileL(OplAPI& aOplAPI)
// open contacts database
// COOpenContactFile:(aFileName$)
	{
	if (iDbHandle)
		User::Leave(KOplErrAlreadyOpen);
	iDbHandle = CContactDatabase::OpenL(aOplAPI.PopString());
	iPtrs = new (ELeave) CPtrs();
	aOplAPI.Push(0.0);
	}


void CCntOpx::CloseContactFileL(OplAPI& aOplAPI)                
// closes contacts database
// COCloseContactFile:
	{
	delete iDbHandle;
	iDbHandle = NULL;
	delete iPtrs;
	iPtrs = NULL;
	aOplAPI.Push(0.0);
	}

void CCntOpx::SortItemsL(OplAPI& aOplAPI)
// sort contacts database
// COSortContactFile:(aKuidMapping&)
	{
	LeaveIfNoOpenDbL();
	CArrayFix<CContactDatabase::TSortPref>* sortOrder=new(ELeave) CArrayFixFlat<CContactDatabase::TSortPref>(3);

	CleanupStack::PushL(sortOrder);
	
	TInt32* aKUidList = aOplAPI.PopPtrInt32();
		
    TInt count=aOplAPI.GetLong((TAny*)aKUidList);
	
	for(TInt i=count;i>0;i--)
		{
		TInt tempUid=aOplAPI.GetLong((TAny*)++aKUidList);
		sortOrder->AppendL(CContactDatabase::TSortPref(TUid::Uid(tempUid)));
		}
	iDbHandle->SortL(sortOrder);

	delete iPtrs->iSortedIds;
	iPtrs->iSortedIds=NULL;
	iPtrs->iSortedIds=CContactIdArray::NewL(iDbHandle->SortedItemsL());
	CleanupStack::Pop(); // sortOrder

	aOplAPI.Push(0.0);
	}

void CCntOpx::SortedItemsAtL(OplAPI& aOplAPI)
// indexes into the sorted contacts 
// COSortedContactFileAt:(aIndex&):
	{
	LeaveIfNoOpenDbL();
	TInt index = OpxUtil::CppIndex(aOplAPI.PopInt32());
	if (index >= iPtrs->iSortedIds->Count())
		User::Leave(KOplErrSubs);
	if (index<0)
		User::Leave(KOplErrSubs);
	TContactItemId tempId = (*iPtrs->iSortedIds)[index];
	DoCloseItemL();
	DoOpenItemL(tempId);
	aOplAPI.Push(0.0);
	}


void CCntOpx::CompressContactFile(OplAPI& aOplAPI)
// compress contacts database        
// COCompressContactFile:
	{
	LeaveIfNoOpenDbL();
	iDbHandle->CompressL();
	aOplAPI.Push(0.0);
	}


void CCntOpx::CompressNeeded(OplAPI& aOplAPI)                        
// is a compress required
// COCompressNeeded%:
	{
	LeaveIfNoOpenDbL();
	aOplAPI.Push((TInt16)iDbHandle->CompressRequired());
	}


void CCntOpx::CountItemsL(OplAPI& aOplAPI)                                
// counts contacts in database
// COCountContacts&: 
	{
	LeaveIfNoOpenDbL();
	aOplAPI.Push((TInt32)iDbHandle->CountL());
	}


void CCntOpx::AddNewItemL(OplAPI& aOplAPI)                                
// adds a new contact to database
// COAddNewContact: 
	{
	LeaveIfNoWriteableItemL();
	iDbHandle->AddNewContactL(*iPtrs->iCurrentItem);
	delete iPtrs->iCurrentItem;
	iPtrs->iCurrentItem=NULL;
	iPtrs->iField=NULL;
	aOplAPI.Push(0.0);
	}


void CCntOpx::DeleteItemL(OplAPI& aOplAPI)                                
// deletes a contact
// CODeleteContact:
	{
	LeaveIfNoWriteableItemL();
	iPtrs->iDeleted = ETrue;
	aOplAPI.Push(0.0);
	}


void CCntOpx::ReadItemL(OplAPI& aOplAPI)                                
// reads contact
// COReadContact:(aContactItemId&)
	{
	LeaveIfNoOpenDbL();
	iPtrs->iCurrentItem = iDbHandle->ReadContactL(aOplAPI.PopInt32());
	iPtrs->iOpenedForRead=ETrue;
	aOplAPI.Push(0.0);
	}


void CCntOpx::OpenItemL(OplAPI& aOplAPI)                                
// opens contact
// COOpenContact:(aContactItemId&)
	{
	LeaveIfNoOpenDbL();
	if ((iPtrs->iCurrentItem) && (iPtrs->iOpenedForRead==EFalse))
		User::Leave(KOplErrAlreadyOpen);
	TInt contactItemId = aOplAPI.PopInt32();
	DoOpenItemL(contactItemId);
	aOplAPI.Push(0.0);
	}


void CCntOpx::CloseItemL(OplAPI& aOplAPI)                                
// closes contact
// COCloseContact:
	{
	DoCloseItemL();
	aOplAPI.Push(0.0);
	}

void CCntOpx::ItemIdL(OplAPI& aOplAPI)                                
// returns contact ID
// COContactId&:
	{
	LeaveIfNoReadableItemL();
	aOplAPI.Push((TInt32)iPtrs->iCurrentItem->Id());
	}

void CCntOpx::SetItemHiddenL(OplAPI& aOplAPI)                        
// sets contact to hidden
// COSetContactHidden:(aHidden%)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iOpenedForRead)
		User::Leave(KErrNotSupported);
	TBool hidden = OpxUtil::CppBool(aOplAPI.PopInt16());
	iPtrs->iCurrentItem->SetHidden(hidden);
	aOplAPI.Push(0.0);
	}

void CCntOpx::SetItemSystemL(OplAPI& aOplAPI)                        
// sets contact to system
// COSetContactSystem:(aSystem%)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iOpenedForRead)
		User::Leave(KErrNotSupported);
	TBool system = OpxUtil::CppBool(aOplAPI.PopInt16());
	iPtrs->iCurrentItem->SetSystem(system);
	aOplAPI.Push(0.0);
	}

void CCntOpx::IsItemHiddenL(OplAPI& aOplAPI)                        
// test if contact is hidden
// COIsContactHidden%:
	{
	LeaveIfNoReadableItemL();
	aOplAPI.Push((TInt16)iPtrs->iCurrentItem->IsHidden());
	}


void CCntOpx::IsItemSystemL(OplAPI& aOplAPI)                        
// test if contact is system
// COIsContactSystem%:
	{
	LeaveIfNoReadableItemL();
	aOplAPI.Push((TInt16)iPtrs->iCurrentItem->IsSystem());
	}

/* not sure if this is needed
void CCntOpx::CommitItemL()             
// commits contact
// COCommitContact:
	{
	iPtrs->LeaveIfNoOpenItemL();
	iDbHandle->CommitContactL(*iPtrs->iCurrentItem);
	}
*/

void CCntOpx::FindItemL(OplAPI& aOplAPI)                                
// finds a contact
// COFindContact&:(aString$,aUIdArrayHandle&)
	{
	LeaveIfNoOpenDbL();
	CContactItemFieldDef* fieldDef=new(ELeave) CContactItemFieldDef;
	CleanupStack::PushL(fieldDef);

	TInt32* aKUidList = aOplAPI.PopPtrInt32();
		
    TInt count=aOplAPI.GetLong((TAny*)aKUidList);
	
	for(TInt i=count;i>0;i--)
		{
		TInt tempUid=aOplAPI.GetLong((TAny*)++aKUidList);
		fieldDef->AppendL(TUid::Uid(tempUid));
		}
	
	TPtrC text = aOplAPI.PopString();
	delete iPtrs->iFoundIds;
	iPtrs->iFoundIds = NULL;
	iPtrs->iFoundIds = iDbHandle->FindLC(text,fieldDef);
	fieldDef->Reset();
	CleanupStack::PopAndDestroy(); //fieldDef;
	aOplAPI.Push((TInt32)iPtrs->iFoundIds->Count());
	}

void CCntOpx::CreateItemL(OplAPI& aOplAPI)                
// creates a contact
//        COCreateContact: 
	{        
	LeaveIfNoOpenDbL();
	TInt templateId = aOplAPI.PopInt32();
	TInt itemType = aOplAPI.PopInt32();
	switch (itemType)
		{
	case EContact:
			{
			//To Do: check templateId must be a template
			CContactItem* copyFrom = iDbHandle->ReadContactLC(templateId);
			iPtrs->iCurrentItem = CContactCard::NewL(copyFrom);
			CleanupStack::PopAndDestroy(); //copyFrom
			}
		break;
	case ETemplate:
			{
			//To Do: check templateId can be either a template or card 
			CContactItem* copyFrom = iDbHandle->ReadContactLC(templateId);
			iPtrs->iCurrentItem = CContactTemplate::NewL(copyFrom);
			CleanupStack::PopAndDestroy(); //copyFrom
			}
		break;
	case EGroup:
		iPtrs->iCurrentItem = CContactGroup::NewL();
		break;
	default:
		User::Leave(KOplErrOutOfRange);
		}
	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldArrayAt(OplAPI& aOplAPI)                
// indexs into field array
// COContactFieldArrayAt:(aIndex&)
	{ 
	LeaveIfNoReadableItemL();
	TInt index = OpxUtil::CppIndex(aOplAPI.PopInt32());
	CContactItemFieldSet& fields = iPtrs->iCurrentItem->CardFields();
	if ((index<0) || (index>fields.Count()))
		User::Leave(KOplErrSubs);
	iPtrs->iField = &fields[index]; 
	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldFind(OplAPI& aOplAPI)                        
// finds a field type in array
// COContactFieldFind%:(aTypeArrayHandle&)
	{
	LeaveIfNoReadableItemL();
	CContactItemFieldSet& fields = iPtrs->iCurrentItem->CardFields();
	TInt tempCount= fields.Count();
	TInt32* pointer = aOplAPI.PopPtrInt32();
    TInt count=aOplAPI.GetLong((TAny*)pointer);
	TUid tempMappingUid =TUid::Uid(aOplAPI.GetLong((TAny*)++pointer));
	TUid vCard1=TUid::Uid(aOplAPI.GetLong((TAny*)++pointer));
	TUid vCard2=TUid::Uid(aOplAPI.GetLong((TAny*)++pointer));
	TUid vCard3=TUid::Uid(aOplAPI.GetLong((TAny*)++pointer));
	TBool found=EFalse;
	TInt pos=KErrNotFound;
	for(TInt jj=0;jj<count;jj++)   // unreachable code here
		{                                     
		if (count==2)
			pos=fields.Find(tempMappingUid); // Bug Fix: In the case of a 2 item array, the 1st Item holds VCard1, but has been read into tempMappingUid
		else
			pos=fields.Find(tempMappingUid,vCard1);
		if (pos!=KErrNotFound)
			{
			if (count<=3)
				break;
			else 
				{
				const CContentType& tempContent=fields[pos].ContentType();
				if ((count==4) && (tempContent.ContainsFieldType(vCard2)) &&
					(tempContent.FieldTypeCount()==2))
					break;
				else if ((count==5) && (tempContent.ContainsFieldType(vCard2)) &&
					(tempContent.ContainsFieldType(vCard3)) && (tempContent.FieldTypeCount()==3))
					break;
				else
					{
					TInt start=pos+1;
					while ((start<tempCount) && (found==EFalse) && (pos!=KErrNotFound))
						{
						pos=fields.FindNext(tempMappingUid,vCard1,start);
						if (pos!=KErrNotFound)
							{
							const CContentType& tmpContent=fields[pos].ContentType();
							TInt fieldCount=tmpContent.FieldTypeCount();
							if (vCard3.iUid==0)
								{
								if ((tmpContent.ContainsFieldType(vCard2)) && (fieldCount==(count-2)))
									{
									found=ETrue;
									break;
									}
								}
							else
								{
								if ((tmpContent.ContainsFieldType(vCard2))  && (tmpContent.ContainsFieldType(vCard3))&& (fieldCount==(count-2)))
									{
									found=ETrue;
									break;
									}
								}
							}
						start=pos+1;
						}
					}
				}
			}
		}
	iPtrs->iField=NULL;
	if (pos!=KErrNotFound)
		aOplAPI.Push((TInt16)++pos);
	else
		User::Leave(KErrNotFound);
	}

void CCntOpx::ContactFieldCount(OplAPI& aOplAPI)                        
// counts fields in field array
// COContactFieldCount&: 
	{
	LeaveIfNoReadableItemL();
	CContactItemFieldSet& fields = iPtrs->iCurrentItem->CardFields();
	aOplAPI.Push((TInt32)fields.Count());
	}

void CCntOpx::ContactFieldReset(OplAPI& aOplAPI)                        
// resets field array
//        COContactFieldReset: 
	{
	LeaveIfNoWriteableItemL();
	iPtrs->iCurrentItem->CardFields().Reset();
	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldAdd(OplAPI& aOplAPI)                        
// adds field to field array
// COContactFieldAdd:
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrGenFail);
	iPtrs->iCurrentItem->AddFieldL(*(iPtrs->iField));
	iPtrs->iField=NULL;
	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldDelete(OplAPI& aOplAPI)                        
// removes field from field array
// COContactFieldDelete:
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotSupported);
	if (iPtrs->iCurrentItem->Type()==KUidContactTemplate)
		User::Leave(KOplErrAccess); // trying to delete template field
	CContactItemFieldSet& fields = iPtrs->iCurrentItem->CardFields();
	const CContentType& tempContentType = iPtrs->iField->ContentType();
	TInt count=tempContentType.FieldTypeCount();
	TInt tempCount= fields.Count();
	TUid vCard1 = tempContentType.Mapping();
	TUid vCard2;
	TUid tempMappingUid=tempContentType.FieldType(0);
	if (count > 1)
		{
		vCard2=tempContentType.FieldType(count-1);
		}
	else 
		{
		vCard2=TUid::Uid(NULL);
		}
	////////////////////
	TBool found=EFalse;
	
	TInt pos=KErrNotFound;
	for(TInt jj=0;jj<count;jj++) // unreachable code here
		{                                     
		pos=fields.Find(tempMappingUid,vCard1);
		if (pos!=KErrNotFound)
			{
			if (count==1)
				break;
			else 
				{
				const CContentType& tempContent=fields[pos].ContentType();
				if (tempContent.ContainsFieldType(vCard2))
					break;
				else
					{
					TInt start=pos+1;
					while ((start<tempCount) && (found==EFalse) && (pos!=KErrNotFound))
						{
						pos=fields.FindNext(tempMappingUid,vCard1,start);
						if (pos!=KErrNotFound)
							{
							const CContentType& tempContent=fields[pos].ContentType();
							if (tempContent.ContainsFieldType(vCard2))
								{
								found=ETrue;
								break;
								}
							}
						start=pos+1;
						}
					}
				}
			}
		}
	if (pos!=KErrNotFound)
		fields.Remove(pos);
	else
		User::Leave(KErrNotFound);
	
	iPtrs->iField=NULL;
	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldInsert(OplAPI& aOplAPI)                        
// inserts a field into field array
// COContactFieldInsert:(aIndex&)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotSupported);
	TInt index = OpxUtil::CppIndex(aOplAPI.PopInt32());
	CContactItemFieldSet& fields = iPtrs->iCurrentItem->CardFields();
	if ((index<0) || (index>fields.Count())) 
		User::Leave(KOplErrSubs);
	fields.InsertL(index,*(iPtrs->iField));
	iPtrs->iField=NULL;
	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldMove(OplAPI& aOplAPI)                        
// moves field in field array
// COContactFieldMove:(aPosFrom&,aPosTo&)
	{
	LeaveIfNoWriteableItemL();

	TInt indexFrom = aOplAPI.PopInt32();
	if(--indexFrom<0)
		User::Leave(KOplErrSubs);

	TInt indexTo= aOplAPI.PopInt32();
	if(--indexTo<0)
		User::Leave(KOplErrSubs);

	CContactItemFieldSet& fields = iPtrs->iCurrentItem->CardFields();

	TInt numFields = fields.Count();

	if ((indexFrom > numFields) || (indexTo > numFields))
		User::Leave(KErrArgument);

	fields.Move(indexFrom,indexTo);

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldCreate(OplAPI& aOplAPI)                        
// creates a field
//        COContactFieldCreate:
	{
	LeaveIfNoWriteableItemL();
	if (!iPtrs->iField)
		iPtrs->iField=NULL;

	CleanupStack::PushL(iPtrs);

	iPtrs->iField=CContactItemField::NewL(KStorageTypeText);

	CleanupStack::Pop(); //iPtrs

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldContent(OplAPI& aOplAPI)                
// returns the content type of a field
// COContactFieldContent:(contentTypeArray&)
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	const CContentType& tempContent = iPtrs->iField->ContentType();
	TInt32 count = tempContent.FieldTypeCount();
	TUid mapping = tempContent.Mapping();
	
	TInt32* pointer = aOplAPI.PopPtrInt32();


	aOplAPI.PutLong(pointer,count+2);
	aOplAPI.PutLong(++pointer,mapping.iUid);

	for (TInt i=0;i<count;i++)
		{
		aOplAPI.PutLong(++pointer,tempContent.FieldType(i).iUid);
		}

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldIsHidden(OplAPI& aOplAPI)                
// returns whether field is hidden
// COContactFieldIsHidden%: 
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);

	aOplAPI.Push((TInt16)iPtrs->iField->IsHidden());
	}

void CCntOpx::ContactFieldIsReadOnly(OplAPI& aOplAPI)                
// returns whether field is read only
// COContactFieldIsReadOnly%:
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);

	aOplAPI.Push((TInt16)iPtrs->iField->IsReadOnly());
	}


void CCntOpx::ContactFieldSetHidden(OplAPI& aOplAPI)                
// set whether field is hidden
// COContactFieldSetHidden:(aHidden%)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	TBool hidden = aOplAPI.PopInt16();
	if ((hidden!=1) && (hidden!=0))
		User::Leave(KOplErrSubs);

	iPtrs->iField->SetHidden(hidden);

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldSetReadOnly(OplAPI& aOplAPI)        
// set whether field is read only
// COContactFieldSetReadOnly:(aReadOnly%)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	TBool readOnly = aOplAPI.PopInt16();
	if ((readOnly!=1) && (readOnly!=0))
		User::Leave(KOplErrSubs);

	iPtrs->iField->SetReadOnly(readOnly);

	aOplAPI.Push(0.0);
	}


void CCntOpx::ContactFieldLabel(OplAPI& aOplAPI)                        
// returns the label of the field
// COContactFieldLabel$: 
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	TPtrC labelString = iPtrs->iField->Label();

	aOplAPI.PushL(labelString);
	}

void CCntOpx::ContactFieldSetLabel(OplAPI& aOplAPI)                
// sets the label of the field
// COContactFieldSetLabel:(label$)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	TPtrC tempLabel = aOplAPI.PopString();

	HBufC* label = tempLabel.AllocL();

	iPtrs->iField->SetLabel(label);

	aOplAPI.Push(0.0);
	}


void CCntOpx::ContactFieldAddType(OplAPI& aOplAPI)                
// adds a type to the field
// COContactFieldAddType:(aType&)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	iPtrs->iField->AddFieldTypeL(TUid::Uid(aOplAPI.PopInt32()));
	
	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldSetMapping(OplAPI& aOplAPI)                
// sets a mapping for the field
// COContactFieldSetMapping:(aMapping&)
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	iPtrs->iField->SetMapping(TUid::Uid(aOplAPI.PopInt32()));

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldSetText(OplAPI& aOplAPI)                
// sets the text of the field
// COContactFieldSetText:(aString$)
	{        
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	TPtrC fieldString = aOplAPI.PopString();

	HBufC* textData = fieldString.AllocL();
	CleanupStack::PushL(textData);
	
	STATIC_CAST(CContactTextField*,iPtrs->iField->Storage())->SetTextL(fieldString); //textData);

	CleanupStack::PopAndDestroy(); //textData
	// delete textData;

	aOplAPI.Push(0.0);
	}
	
void CCntOpx::ContactFieldSetLong(OplAPI& aOplAPI)                
// sets the text of the field
// COContactFieldSetLong:(aStringAddr&,aLength&)
	{        
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	TInt length = aOplAPI.PopInt32();

	TInt32* aBuf= aOplAPI.PopPtrInt32();

	TPtrC fieldText((const TText*)aBuf,length);

	HBufC* textData = fieldText.AllocL();
	CleanupStack::PushL(textData);
	
	STATIC_CAST(CContactTextField*,iPtrs->iField->Storage())->SetTextL(fieldText); //textData);

	CleanupStack::PopAndDestroy(); //textData

	//delete textData;

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldText(OplAPI& aOplAPI)                        
// returns the test in the field
// COContactFieldText$: 
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);

	TPtrC fieldString = STATIC_CAST(CContactTextField*,iPtrs->iField->Storage())->Text();
	
	if (fieldString.Length()>255)
		User::Leave(KOplErrStrTooLong);

	aOplAPI.PushL(fieldString);
	}

void CCntOpx::ContactFieldLength(OplAPI& aOplAPI)                        
// returns the test in the field
// COContactFieldLength&: 
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);

	TPtrC fieldString = STATIC_CAST(CContactTextField*,iPtrs->iField->Storage())->Text();
		
	aOplAPI.Push((TInt32)fieldString.Length());
	}

void CCntOpx::ContactFieldLong(OplAPI& aOplAPI)                        
// returns the test in the field
// COContactFieldLong:(abufHandle&) 
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);

	TPtr* buffer = REINTERPRET_CAST(TPtr*,aOplAPI.PopPtrInt32());  

	TPtrC fieldString = STATIC_CAST(CContactTextField*,iPtrs->iField->Storage())->Text();
	TInt length = fieldString.Length();

	if (fieldString!=TPtrC())  // NOT NULL
		Mem::Copy(buffer,fieldString.Ptr(),length);

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldDisable(OplAPI& aOplAPI)
// sets the templates field disabled
// COContactFieldDisable:(aBool%) 
	{
	LeaveIfNoWriteableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);
	
	TBool disabled = aOplAPI.PopInt16();
	if ((disabled!=1) && (disabled!=0))
		User::Leave(KOplErrSubs);

	iPtrs->iField->SetDisabled(disabled);
	iPtrs->iField=NULL;

	aOplAPI.Push(0.0);
	}

void CCntOpx::ContactFieldIsDisabled(OplAPI& aOplAPI)
// test the templates field to see if disabled
// COContactFieldIsDisabled%: 
	{
	LeaveIfNoReadableItemL();
	if (iPtrs->iField==NULL)
		User::Leave(KOplErrNotExists);

	aOplAPI.Push((TInt16)iPtrs->iField->IsDisabled());
	}

void CCntOpx::DoOpenItemL(TInt acontactItemId)
// opens a contact
	{        
	iPtrs->iCurrentItem = iDbHandle->OpenContactL(acontactItemId); // takes ownership
	}

void CCntOpx::DoCloseItemL()
// close contact
	{
	LeaveIfNoWriteableItemL();
	TInt contactItemId = iPtrs->iCurrentItem->Id();
	if (!iPtrs->iDeleted)
		iDbHandle->CommitContactL(*iPtrs->iCurrentItem);
	iDbHandle->CloseContactL(contactItemId);
	delete iPtrs->iCurrentItem;
	iPtrs->iCurrentItem=NULL;
	iPtrs->iField=NULL;
	}

// Additional code for Crystal onwards

void CCntOpx::GroupCountL(OplAPI& aOplAPI)
	{
	CContactGroup* group = STATIC_CAST(CContactGroup*,iPtrs->iCurrentItem);
	TInt32 count = group->ItemsContained()->Count(); 
	aOplAPI.Push(count);
	}

void CCntOpx::GroupItemIdAtL(OplAPI& aOplAPI)
	{
	TInt32 index = aOplAPI.PopInt32();
	CContactGroup* group = STATIC_CAST(CContactGroup*,iPtrs->iCurrentItem);
	const CContactIdArray* items = group->ItemsContained(); // pointer to array owned by cntmodel
	TContactItemId id = (*items)[OpxUtil::CppIndex(index)];
	aOplAPI.Push(id);
	}

void CCntOpx::GroupAddItemL(OplAPI& aOplAPI)
	{
	LeaveIfNoOpenDbL();
	TInt itemId = aOplAPI.PopInt32();
	TInt groupId = aOplAPI.PopInt32();
	iDbHandle->AddContactToGroupL(groupId,itemId);
	aOplAPI.Push(0.0);
	}

void CCntOpx::GroupRemoveItemL(OplAPI& aOplAPI)
	{
	LeaveIfNoOpenDbL();
	TInt itemId = aOplAPI.PopInt32();
	TInt groupId = aOplAPI.PopInt32();
	iDbHandle->RemoveContactFromGroupL(groupId,itemId);
	aOplAPI.Push(0.0);
	}

void CCntOpx::TemplateGoldenIdL(OplAPI& aOplAPI)
	{
	LeaveIfNoOpenDbL();
	aOplAPI.Push(iDbHandle->TemplateId());
	}

void CCntOpx::OwnCardIdL(OplAPI& aOplAPI)
	{
	LeaveIfNoOpenDbL();
	TInt32 ownCardId=iDbHandle->OwnCardId();
	aOplAPI.Push(ownCardId);
	}

void CCntOpx::ItemTypeL(OplAPI& aOplAPI)
	{
	LeaveIfNoReadableItemL();
	TUid itemUid = iPtrs->iCurrentItem->Type();
	TInt32 itemType;
	if (itemUid==KUidContactCard)
		{
		itemType = EContact;
		}
	else if (itemUid==KUidContactGroup)
		{
		itemType = EGroup;
		}
	else if (itemUid==KUidContactTemplate)
		{
		itemType = ETemplate;
		}
	else if (itemUid==KUidContactOwnCard)
		{
		itemType = EOwnCard;
		}
	else if (itemUid==KUidContactCardTemplate)
		{
		itemType = ECardTemplate;
		}
	else 
		{
		itemType=NULL;
		User::Leave(KOplErrTypeViol);
		}
	aOplAPI.Push(itemType);
	}

void CCntOpx::LeaveIfNoOpenDbL()
	{
	if (iPtrs==NULL)
		User::Leave(KErrBadHandle);
	if (iDbHandle==NULL)
		User::Leave(KOplErrNotOpen);
	}
		
void CCntOpx::LeaveIfNoReadableItemL()
	{
	LeaveIfNoOpenDbL();
	if (!iPtrs->iCurrentItem)
		User::Leave(KOplErrRead);
	}

void CCntOpx::LeaveIfNoWriteableItemL()
	{
	LeaveIfNoOpenDbL();
	LeaveIfNoReadableItemL();
	if (iPtrs->iOpenedForRead)
		User::Leave(KOplErrWrite);
	}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

CTlsDataCCntOPX* CTlsDataCCntOPX::NewL(OplAPI& aOplAPI)
	{
    CTlsDataCCntOPX* This=new(ELeave) CTlsDataCCntOPX(aOplAPI);
    CleanupStack::PushL(This);
	This->ConstructL();
    CleanupStack::Pop();
    return This;
    }

CTlsDataCCntOPX::CTlsDataCCntOPX(OplAPI& aOplAPI) : COpxBase(aOplAPI)
    {
	}

void CTlsDataCCntOPX::ConstructL()
    {
	iCntHandle = new(ELeave) CCntOpx;
    } 

CTlsDataCCntOPX::~CTlsDataCCntOPX()
    {
	delete iCntHandle;
	Dll::FreeTls();                // Required so that Tls is set to zero on unloading the OPX in UNLOADM#
//	__UHEAP_MARKENDC(1);  // created & cleaned up by OPL
	}

void CTlsDataCCntOPX::RunL(TInt aProcNum)
// Run a language extension procedure
	{
	switch (aProcNum)
		{
	case ECreateContactFile:
		iCntHandle->CreateContactFileL(iOplAPI);
		break;
	case EOpenContactFile:
		iCntHandle->OpenContactFileL(iOplAPI);
		break;
	case ECloseContactFile:
		iCntHandle->CloseContactFileL(iOplAPI);
		break;
	case ESortItems:
		iCntHandle->SortItemsL(iOplAPI);
		break;
	case ESortedItemsAt:
		iCntHandle->SortedItemsAtL(iOplAPI);
		break;
	case ECompressContactFile:
		iCntHandle->CompressContactFile(iOplAPI);
		break;
	case ECompressNeeded:
		iCntHandle->CompressNeeded(iOplAPI);
		break;
	case ECountItems:
		iCntHandle->CountItemsL(iOplAPI);
		break;
	case EAddNewItem:
		iCntHandle->AddNewItemL(iOplAPI);
		break;
	case EDeleteItem:
		iCntHandle->DeleteItemL(iOplAPI);
		break;
	case EReadItem:
		iCntHandle->ReadItemL(iOplAPI);
		break;
	case EOpenItem:
		iCntHandle->OpenItemL(iOplAPI);
		break;
	case ECloseItem:
		iCntHandle->CloseItemL(iOplAPI);
		break;
	case EItemId:
		iCntHandle->ItemIdL(iOplAPI);
		break;
	case ESetItemHidden:
		iCntHandle->SetItemHiddenL(iOplAPI);
		break;
	case ESetItemSystem:
		iCntHandle->SetItemSystemL(iOplAPI);
		break;
	case EIsItemHidden:
		iCntHandle->IsItemHiddenL(iOplAPI);
		break;
	case EIsItemSystem:
		iCntHandle->IsItemSystemL(iOplAPI);
		break;
	case EFindItem:
		iCntHandle->FindItemL(iOplAPI);
		break;
	case ECreateItem:
		iCntHandle->CreateItemL(iOplAPI);
		break;
	case EContactFieldArrayAt:
		iCntHandle->ContactFieldArrayAt(iOplAPI);
		break;
	case EContactFieldFind:
		iCntHandle->ContactFieldFind(iOplAPI);
		break;
	case EContactFieldCount:
		iCntHandle->ContactFieldCount(iOplAPI);
		break;
	case EContactFieldReset:
		iCntHandle->ContactFieldReset(iOplAPI);
		break;
	case EContactFieldAdd:
		iCntHandle->ContactFieldAdd(iOplAPI);
		break;
	case EContactFieldDelete:
		iCntHandle->ContactFieldDelete(iOplAPI);
		break;
	case EContactFieldInsert:
		iCntHandle->ContactFieldInsert(iOplAPI);
		break;
	case EContactFieldMove:
		iCntHandle->ContactFieldMove(iOplAPI);
		break;
	case EContactFieldCreate:
		iCntHandle->ContactFieldCreate(iOplAPI);
		break;
	case EContactFieldContent:
		iCntHandle->ContactFieldContent(iOplAPI);
		break;
	case EContactFieldIsHidden:
		iCntHandle->ContactFieldIsHidden(iOplAPI);
		break;
	case EContactFieldIsReadOnly:
		iCntHandle->ContactFieldIsReadOnly(iOplAPI);
		break;
	case EContactFieldSetHidden:
		iCntHandle->ContactFieldSetHidden(iOplAPI);
		break;
	case EContactFieldSetReadOnly:
		iCntHandle->ContactFieldSetReadOnly(iOplAPI);
		break;
	case EContactFieldLabel:
		iCntHandle->ContactFieldLabel(iOplAPI);
		break;
	case EContactFieldSetLabel:
		iCntHandle->ContactFieldSetLabel(iOplAPI);
		break;
	case EContactFieldAddType:
		iCntHandle->ContactFieldAddType(iOplAPI);
		break;
	case EContactFieldSetMapping:
		iCntHandle->ContactFieldSetMapping(iOplAPI);
		break;
	case EContactFieldSetText:
		iCntHandle->ContactFieldSetText(iOplAPI);
		break;
	case EContactFieldSetLong:
		iCntHandle->ContactFieldSetLong(iOplAPI);
		break;
	case EContactFieldText:
		iCntHandle->ContactFieldText(iOplAPI);
		break;
	case EContactFieldLength:
		iCntHandle->ContactFieldLength(iOplAPI);
		break;
	case EContactFieldLong:
		iCntHandle->ContactFieldLong(iOplAPI);
		break;
	case EContactFieldDisable:
		iCntHandle->ContactFieldDisable(iOplAPI);
		break;
	case EContactFieldIsDisabled:
		iCntHandle->ContactFieldIsDisabled(iOplAPI);
		break;
	case ETemplateGoldenId:
		iCntHandle->TemplateGoldenIdL(iOplAPI);
		break;
	case EOwnCardId:
		iCntHandle->OwnCardIdL(iOplAPI);
		break;
	case EItemType:
		iCntHandle->ItemTypeL(iOplAPI);
		break;
	default:
		User::Leave(KOplErrOpxProcNotFound);
		}
	}

//-------------------------------------------------------------- Check version
TBool CTlsDataCCntOPX::CheckVersion( TInt aVersion )
	{
	if ((aVersion & 0xFF00) > (KOpxContactVersion & 0xFF00))
		return EFalse;
	else
		return ETrue;
	}

//---------------------------------------------------------------- Constructor

//-------------------------------------------------------------------- Version
EXPORT_C TUint Version()
	{
	return KOpxContactVersion;
	}

//------------------------------------------------------------------------ New


EXPORT_C COpxBase* NewOpxL(OplAPI& aOplAPI)
// Creates a COpxBase instance as required by the OPL runtime
// This object is to be stored in the OPX's TLS as shown below
	{
//	__UHEAP_MARK;
	CTlsDataCCntOPX* tls=((CTlsDataCCntOPX*)Dll::Tls());
	if (tls==NULL)                // tls is NULL on loading an OPX DLL (also after unloading it)
		{
		tls=CTlsDataCCntOPX::NewL(aOplAPI);
		CleanupStack::PushL(tls);
	    TInt ret=Dll::SetTls(tls);
		User::LeaveIfError(ret);
		CleanupStack::Pop();        // tls
	}
    return (COpxBase *)tls;
	}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point
//
	{
	return(KErrNone);
	}