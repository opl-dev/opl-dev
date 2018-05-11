// OPLCONV.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

class TCoerce
	{
public:
	static TInt ToWord(TInt16 &aWord,TInt32 aLong);
	static TInt ToWord(TInt16 &aWord,const TReal &aDouble);
	static TInt ToUWord(TUint16 &aWord,TInt32 aLong);
	static TInt ToUWord(TUint16 &aWord,const TReal &aDouble);
	static TInt ToLong(TInt32 &aLong,TInt16 aWord);
	static TInt ToLong(TInt32 &aLong,const TReal &aDouble);
	static TInt ToDouble(TReal &aDouble,TInt16 aWord);
	static TInt ToDouble(TReal &aDouble,TInt32 aLng);
	static TInt ToDouble(TReal &aDouble,const TDesC &aStr);
	static TInt HexToLong(TInt32 &aLong,const TDesC &aStr); // should be HexStrToDbl()
	static TInt ToHexString(TDes &aStr,TInt32 aLong);  
	static TInt ToString(TDes &aStr,const TReal &aDouble);  
// 	static void ToTBuf8(TBuf8<0x100> &aStr,const TUint8 *aOplStr);
// 	static void ToOplStr(TUint8 *aOplStr,const TBuf8<0x100> &aStr);
	};
	
