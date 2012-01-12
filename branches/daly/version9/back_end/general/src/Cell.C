// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: implementation of utility
//           template <class T> class Cell
//
// Cell is the basic component of externally linked lists.
//
// Principal Author: 
//
// Status: Useable.
//
// Revision History:


#include "Cell.h"

template<class T> 
void Cell<T>::readContents( istream& istr )
{
  if (!contents) { contents = new T();}
  istr > *contents;
}

template<class T> 
void Cell<T>::writeContents( ostream& ostr ) const
{
  ostr < *contents;
}

#include "Word.h"

template class Cell<Word>;

