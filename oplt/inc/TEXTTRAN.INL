// TEXTTRAN.INL
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


// TTranslateError
inline TInt TTranslateError::Position() const {return iPosition;}
inline void TTranslateError::SetPosition(TInt aPosition) {iPosition=aPosition;}
inline TInt TTranslateError::Error() const {return iError;}
inline void TTranslateError::SetError(TInt anError) {iError=anError;}

// TFileTranslateError
inline void TSourceTranslateError::SetSource(const TDesC& aSourceName) {iSource=aSourceName;}
inline const TDesC& TSourceTranslateError::Source() const { return iSource;}
