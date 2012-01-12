/*
 *   $Id: GenericObject.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of classes GenericRep, GenericObject
//
// Principal Author: Stephane Collart
//
// Status: Under development
//
// Revision History:
//



#include "GenericObject.h"



//--------------------------------------------------------------------------//
//----------------------------- GenericRep ---------------------------------//
//------------------ static member initialisations -------------------------//


const Type GenericRep::theGenericObjectType = Type( Type::unique() );

