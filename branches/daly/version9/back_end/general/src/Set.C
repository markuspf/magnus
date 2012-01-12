// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of Set and SetIterator class.
//
// Principal Author: Roger Needham
//
// Status: Useable.
//
// Revision History:
//
// * 01/96 Dmitry B. implemented IPC tools.
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "Set.h"
#include "Chars.h"

template <class T>
int SetData<T>::hashElement(const T & t) const { return t.hash();}

template <class T>
void SetData<T>::write( ostream& ostr ) const
{
#ifdef DEBUG
    ostr < Chars("Set<") < ' ';
#endif    

  ostr < numberOfElements < numberOfBuckets;

  for ( int i = 0; i < numberOfBuckets; i++ ) {

    CellType* scan = hashTable[i];

    if( scan == NULL )
      ostr < 0;
    else
      ostr < 1;
       
    while ( scan != NULL ) {
      ostr < *scan;
      scan = scan->nextCell;
      if( scan == NULL )
	ostr < 0;
      else
	ostr < 1;
    }
  }

#ifdef DEBUG
    ostr < ' ' < Chars(">Set");
#endif    
}


template <class T>
void SetData<T>::read( istream& istr )
{
  if ( hashTable ) {
    removeAllElements();
    delete [] hashTable;
    hashTable = NULL;
  }
  
#ifdef DEBUG
  {
    Chars header;
    istr > header;
    if( header != Chars("Set<") ) {
      error("Cannot load Set<T>: bad header of the data section.");
    }
  }
#endif

  istr > numberOfElements > numberOfBuckets;
  hashTable = new CellType*[numberOfBuckets];
  for( int i = 0; i < numberOfBuckets; i++ ) {
    int n;
    istr > n;
    if( n == 0)
      hashTable[i] = NULL;
    else {
      bool bFirst = true;
      CellType *scan, *old;
      
      while( true ) {
	scan = new CellType();
        istr > *scan;

	if( bFirst ) {
	  hashTable[i] = scan;
	  bFirst = false;
	}
	else
	  old->nextCell = scan;
	
	istr > n;
	if( n == 0 ) {
	  scan->nextCell == NULL;
	  break;
	}
	old = scan;
      }
    }
  }

#ifdef DEBUG
  {
    Chars footer;
    istr > footer;
    if( footer != Chars(">Set") ) {
      error("Cannot load Set<T>: bad footer of the data section.");
    }
  }
#endif
}


template <class T>
void SetOf<T>::shrinkToIntersectionWith(const SetOf<T>& S)
{
//@@rn  SetIterator< SetOf<T> > I(*this);
  SetIterator<T> I(*this);
  while ( !I.done() ) {
	 if ( !S.contains( I.value() ) ) removeElement( I.value() );
	 I.next();
  }
}


template <class T>
void SetOf<T>::adjoinElements(const SetOf<T>& S)
{
//@@rn  SetIterator< SetOf<T> > I(S);
  SetIterator<T> I(S);
  while ( !I.done() ) {
	 adjoinElement( I.value() );
	 I.next();
  }
}


template <class T>
void SetOf<T>::removeElements(const SetOf<T>& S)
{
//@@rn  SetIterator< SetOf<T> > I(S);
  SetIterator<T> I(S);
  while ( !I.done() ) {
	 removeElement( I.value() );
	 I.next();
  }
}


template <class T>
Bool SetOf<T>::contains(const SetOf<T>& S) const
{
//@@rn  SetIterator< SetOf<T> > I(S);
  SetIterator<T> I(S);
  while ( !I.done() ) {
	 if ( !contains( I.value() ) ) return 0;
	 I.next();
  }
  return 1;
}

#include "Integer.h"
int SetData<Integer>::hashElement(const Integer & t) const 
  { return t.as_long(); }

int SetData<Chars>::hashElement(const Chars& t) const { return t.length();}

int SetData<double>::hashElement(const double & d) const 
  { return (int)d; }
#include "../../SessionManager/include/OID.h"

template class SetOf<OID>;
void SetData<OID>::write( ostream& ostr ) const { }
void SetData<OID>::read( istream& istr ) { }

#include "Word.h"
template class SetData<Word>;
template class SetOf<Word>;
