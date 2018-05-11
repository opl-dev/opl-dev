// from smartprofile
//IMPORT PROFILEENGINE_20 ; DATA XREF: .text:0040201C.o
//	GetProfileActiveNameL__11CProfileAPIG6TPtr16Pi
//IMPORT PROFILEENGINE_29 ; DATA XREF: .text:0040200C.o
//	GetProfileNameListL__11CProfileAPIP12MDesC16ArrayQ210CProfileDb11TDbSortType
//IMPORT PROFILEENGINE_49 ; DATA XREF: .text:00401FFC.o
//	NewLC__11CProfileAPIi
//IMPORT PROFILEENGINE_65 ; DATA XREF: .text:0040202C.o
//	SetProfileNameL__11CProfileAPIG7TPtrC16UcUc

// ff

#include <bamdesca.h> //MDesC16Array
#include <cmtcli.h> // cmtmsg

class CProfileAPI;
class CProfileDb;

class CProfileDb :public CBase
{
public:
	enum TDbSortType
	{
	Enosort = 0,
	Eactiveprofileiszero = 2
	};

	enum TDbErrorCode
	{
	Eaone =	1,
	Eatwo = 2
	};

	static CProfileDb* NewL();
	static CProfileDb* NewLC();
	static CProfileDb* ConstructL();
	~CProfileDb(void) {};

	IMPORT_C TInt16 GetProfileCountL();

//private:
	//(private: enum CProfileDb::TDbErrorCode  __thiscall CProfileDb::GetActiveProfileL(int *))
	IMPORT_C TDbErrorCode GetActiveProfileL(int *);

};

class CProfileAPI :public CBase
{
public:

	enum TProErrorCode
	{
	Eaone =	1,
	Eatwo = 2
	};

	static CProfileAPI* NewL(int);
	static CProfileAPI* NewLC(int);

	virtual ~CProfileAPI(void) {};

	//(public: enum CProfileAPI::TProErrorCode  __thiscall CProfileAPI::SetProfileNameL(class TPtrC16,unsigned char,unsigned char))
    IMPORT_C TProErrorCode SetProfileNameL(TPtrC16,TUint8,TUint8);
	IMPORT_C TInt GetProfileCountL(void);
	//(public: enum CProfileAPI::TProErrorCode  __thiscall CProfileAPI::GetProfileActiveNameL(class TPtr16,int *))
	IMPORT_C TProErrorCode GetProfileActiveNameL(TPtr16, int*);
	// (public: enum CProfileAPI::TProErrorCode  __thiscall CProfileAPI::GetProfileNameListL(class MDesC16Array *,enum CProfileDb::TDbSortType))
	IMPORT_C TProErrorCode GetProfileNameListL(MDesC16Array *,enum CProfileDb::TDbSortType);
	// (public: enum CProfileAPI::TProErrorCode  __thiscall CProfileAPI::GetHDSDefProfileL(class TPtr16))
	IMPORT_C TProErrorCode GetHDSDefProfileL(TPtr16);
	// (public: enum CProfileAPI::TProErrorCode  __thiscall CProfileAPI::GetCarDefProfileL(class TPtr16))
	IMPORT_C TProErrorCode GetCarDefProfileL(TPtr16);

	//public: class CPnMsg * __thiscall CProfileAPI::PbPnAccModeGetReqL(void)
	IMPORT_C CPnMsg* PbPnAccModeGetReqL();
	//public: class CPnMsg * __thiscall CProfileAPI::PbPnModeSettingChangedIndL(unsigned char,unsigned char)
	IMPORT_C CPnMsg* PbPnModeSettingChangedIndL(unsigned char,unsigned char);

};

