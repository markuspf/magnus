//   Copyright (C) 1999 The New York Group Theory Cooperative
//   See magnus/doc/COPYRIGHT for the full notice.
 
// Contents: Implementation of VectorPtr class
//
// Principal Author: Dmitry Bormotov 
//
// Status: Useable
//
// Revision History:
//

#include "VectorPtr.h"

template <class T>
VectorItemRef<T> VectorPtrOf<T>::operator [] ( int i ) 
{ 
  return change()->operator [](i); 
}

#include "../../SessionManager/include/OID.h"
template class VectorPtrOf<OID>;
