/*
 *   $Id: Type.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
 
// Contents: Implementation of class Type
//
// Principal Author: Stephane Collart
//
// Status: in use
//
// Revision History:
//
// Implementation Notes :
//


#include "Type.h"

 
//------------------------------ Type -------------------------------//
//-------------------------- static members -------------------------//

int Type::uniqueTypeNumber = 0;

// points to last number used
// numbering of types starts at 1; 0 is reserved for default `no type'
