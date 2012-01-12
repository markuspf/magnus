/*
 *   $Id: Vector.C 4 2005-01-07 00:28:20Z daly $
 */
 
//   Copyright (C) 1994 The New York Group Theory Cooperative
//   See magnus/doc/COPYRIGHT for the full notice.
 
// Contents: Implementation of Vector<T> class
//
// Principal Author: Stephane Collart, Roger Needham
//
// Status: Useable.
//
// Revision History:
//

//@rn It's all in Vector.h, so g++ 2.5.8 won't choke on templates.

//@sr Temporary gcc 2.6.3 hackery:

#include "Word.h"
#include "Vector.h"

template class VectorRep<Word>;
template class VectorRep<Chars>;
template class VectorOf<Generator>;

