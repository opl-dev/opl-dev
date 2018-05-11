// Contact.h - Contacts OPX Header File
//
// Copyright (c) 1997-2000 Symbian Ltd. All rights reserved.

#ifndef __CNTOPX_H__
#define __CNTOPX_H__

#include <e32base.h>
#include <e32std.h>
#include <oplapi.h>
#include <opx.h>
#include <oplerr.h>
#include <badesca.h>
#include "cntdb.h"
#include "cntfield.h"
#include "cntfldst.h"
#include "cntitem.h"
#include "e32svr.h"

const TInt32 KUidContactOpxValue=0x10003ED7; 
const TUid KUidContactOpx={KUidContactOpxValue};

// The OPX's version number as used in the DECLARE OPX statement for your OPX
const TInt KOpxContactVersion=0x600;

class CPtrs : public CBase
	{
public:
	~CPtrs();
	TBool iOpenedForRead;
	TBool iDeleted;
	CContactItem* iCurrentItem;
	CContactItemField* iField;
	CContactIdArray* iSortedIds;
	CContactIdArray* iFoundIds;
//	CContactIdArray* iGroupIds;
	};

class CCntOpx : public CBase 
    {
public:
	enum TContactItemType
		{
		EContact      =0x01,
		EOwnCard      =0x02,
		ETemplate     =0x04,
		ECardTemplate =0x08,
		EGroup        =0x10
		};
	~CCntOpx();	
	void CreateContactFileL(OplAPI& aOplAPI);			// create new contacts database
	void OpenContactFileL(OplAPI& aOplAPI);			// open contacts database
	void CloseContactFileL(OplAPI& aOplAPI);			// closes contacts database
	void SortItemsL(OplAPI& aOplAPI);			// sort contacts database
	void SortedItemsAtL(OplAPI& aOplAPI);			// indexes into the sortd database
	void CompressContactFile(OplAPI& aOplAPI);		// compress contacts database
	void CompressNeeded(OplAPI& aOplAPI);				// is a compress required
	void CountItemsL(OplAPI& aOplAPI);
	void AddNewItemL(OplAPI& aOplAPI);
	void DeleteItemL(OplAPI& aOplAPI);
	void ReadItemL(OplAPI& aOplAPI);
	void OpenItemL(OplAPI& aOplAPI);
	void CloseItemL(OplAPI& aOplAPI);
	void ItemIdL(OplAPI& aOplAPI);				// returns the item Id
	void SetItemHiddenL(OplAPI& aOplAPI);
	void SetItemSystemL(OplAPI& aOplAPI);
	void IsItemHiddenL(OplAPI& aOplAPI);	
	void IsItemSystemL(OplAPI& aOplAPI);
	void FindItemL(OplAPI& aOplAPI);
	void OpenContactTemplate(OplAPI& aOplAPI);		// creates a contact template
	void CloseContactTemplate(OplAPI& aOplAPI);		// commits a contact template
	void CreateItemL(OplAPI& aOplAPI);
	void ContactFieldArrayAt(OplAPI& aOplAPI);		// indexs into field array
	void ContactFieldFind(OplAPI& aOplAPI);			// finds a field type in array
	void ContactFieldCount(OplAPI& aOplAPI);			// counts fields in field array
	void ContactFieldReset(OplAPI& aOplAPI);			// resets field array
	void ContactFieldAdd(OplAPI& aOplAPI);			// adds field to field array
	void ContactFieldDelete(OplAPI& aOplAPI);			// removes field from field array
	void ContactFieldInsert(OplAPI& aOplAPI);			// inserts a field into field array
	void ContactFieldMove(OplAPI& aOplAPI);			// moves field in field array
	void ContactFieldCreate(OplAPI& aOplAPI);			// creates a field
	void ContactFieldContent(OplAPI& aOplAPI);		// returns the content type of a field
	void ContactFieldIsHidden(OplAPI& aOplAPI);		// returns whether field is hidden
	void ContactFieldIsReadOnly(OplAPI& aOplAPI);		// returns whether field is read only
	void ContactFieldSetHidden(OplAPI& aOplAPI);		// set whether field is hidden
	void ContactFieldSetReadOnly(OplAPI& aOplAPI);	// set whether field is read only
	void ContactFieldLabel(OplAPI& aOplAPI);			// returns the label of the field
	void ContactFieldSetLabel(OplAPI& aOplAPI);		// sets the label of the field
	void ContactFieldAddType(OplAPI& aOplAPI);		// adds a type to the field
	void ContactFieldSetMapping(OplAPI& aOplAPI);		// sets a mapping for the field
	void ContactFieldSetId(OplAPI& aOplAPI);			// sets the UID of the field
	void ContactFieldId(OplAPI& aOplAPI);				// returns the UID of the field
	void ContactFieldSetText(OplAPI& aOplAPI);		// sets the text of the field
	void ContactFieldSetLong(OplAPI& aOplAPI);		// sets the text of the field based on size > 256
	void ContactFieldText(OplAPI& aOplAPI);			// returns the text in the field
	void ContactFieldLength(OplAPI& aOplAPI);			// returns the length of text in the field
	void ContactFieldLong(OplAPI& aOplAPI);			// returns the text in the field to a buffer
	void ContactFieldDisable(OplAPI& aOplAPI);			// sets the templates field disabled
	void ContactFieldIsDisabled(OplAPI& aOplAPI);			// sees if the template is field disabled
	void GroupCountL(OplAPI& aOplAPI);
	void GroupItemIdAtL(OplAPI& aOplAPI);
	void GroupAddItemL(OplAPI& aOplAPI);
	void GroupRemoveItemL(OplAPI& aOplAPI);
	void TemplateGoldenIdL(OplAPI& aOplAPI);
	void OwnCardIdL(OplAPI& aOplAPI);
	void ItemTypeL(OplAPI& AOplAPI);
private:	
	void DoOpenItemL(TInt acontactItemId);
	void DoCloseItemL();
	void LeaveIfNoOpenDbL();
	void LeaveIfNoReadableItemL();
	void LeaveIfNoWriteableItemL();
private:
	CPtrs* iPtrs;
	CContactDatabase* iDbHandle;
	};

class CTlsDataCCntOPX : public COpxBase 
	{
public:
	static CTlsDataCCntOPX* NewL(OplAPI& aOplAPI);
	CTlsDataCCntOPX(OplAPI& aOplAPI);
	~CTlsDataCCntOPX();
	virtual void RunL(TInt aProcNum);
	void ConstructL();
	virtual TInt CheckVersion(TInt aVersion);
	CCntOpx* iCntHandle;
private:
	// the language extension procedures
	enum TExtensions
		{	
		ECreateContactFile =1,
		EOpenContactFile,
		ECloseContactFile,
		ECountItems,
		ESortItems,
		EFindItem,
		ESortedItemsAt,
		EItemId,
		EReadItem,
		EOpenItem,
		ECloseItem,
		ECreateItem,
		EAddNewItem,
		EDeleteItem,
//		EOpenContactTemplate,
//		ECloseContactTemplate,
		EContactFieldDisable = 17,
		EContactFieldIsDisabled,
		ESetItemHidden,
		EIsItemHidden,
		ESetItemSystem,
		EIsItemSystem,
		ECompressContactFile,
		ECompressNeeded,

		EContactFieldCount,
		EContactFieldArrayAt,
		EContactFieldFind,
		EContactFieldReset,
		EContactFieldCreate,
		EContactFieldInsert,
		EContactFieldAdd,
		EContactFieldDelete,
		EContactFieldMove,
		EContactFieldContent,
		EContactFieldSetHidden,
		EContactFieldIsHidden,
		EContactFieldSetReadOnly,
		EContactFieldIsReadOnly,
		EContactFieldSetLabel,
		EContactFieldLabel,
		EContactFieldSetMapping,
		EContactFieldAddType,
		EContactFieldSetText,
		EContactFieldSetLong,
		EContactFieldText,
		EContactFieldLength,
		EContactFieldLong,

		EGroupSetName = 100,
		EGroupName,
		EGroupCount,
		EGroupItemAt,
		EGroupAddItem,
		EGroupRemoveItem,

		ETemplateGoldenId = 200,
		EOwnCardId,
		EItemType
		};
	};

inline CTlsDataCCntOPX* TheTls() { return((CTlsDataCCntOPX *)Dll::Tls()); }
inline void SetTheTls(CTlsDataCCntOPX *theTls) { Dll::SetTls(theTls); }
    
#endif __CNTOPX_H__