// OPLSTACK.INL
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Extracted from OPLTDEF.INL by HP, January 1997

// CTranStackBase
inline CTranStackBase::CTranStackBase(TBufRep aRep,TUint aLength,TUint aGranularity)
	: CArrayFixBase(aRep,aLength,aGranularity)
	{}
inline void CTranStackBase::PushL(const TAny * aPtr) {InsertL(Count(),aPtr,1);}
inline void CTranStackBase::Pop(TAny *aPtr) {Pick(aPtr,0);}
inline const TAny *CTranStackBase::Top() const {return Peek(0);}
inline TUint CTranStackBase::Depth() const {return Count();}

// CTranStack
template <class T>
inline void CTranStack<T>::PushL(const T* aPtr) {CTranStackBase::PushL((const TAny *)aPtr);}
template <class T>
inline void CTranStack<T>::Pop(T* aPtr) {CTranStackBase::Pop((TAny *)aPtr);}
template <class T>
inline T& CTranStack<T>::Top() {return *(T*)CTranStackBase::Top();}
template <class T>
inline T& CTranStack<T>::Peek(TInt aDepth) { return *(T*)CTranStackBase::Peek(aDepth);}
template <class T>
inline void CTranStack<T>::Pick(T* aRef, TInt aDepth) { CTranStackBase::Pick((TAny *)aRef,aDepth);}
template <class T>
inline CTranStack<T>::CTranStack(TBufRep aRep,TUint aGranularity)
	: CTranStackBase(aRep,sizeof(T),aGranularity)
	{}

// Class CTranStackFlat
template <class T>
inline CTranStackFlat<T>::CTranStackFlat(TUint aGranularity)
	: CTranStack<T>((TBufRep)CBufFlat::NewL,aGranularity)
	{}

// Class CTranStackSeg
template <class T>
inline CTranStackSeg<T>::CTranStackSeg(TUint aGranularity)
	: CTranStack<T>((TBufRep)CBufSeg::NewL,aGranularity)
	{}

