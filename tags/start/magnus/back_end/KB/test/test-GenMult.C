/*
 *   $Id: test-GenMult.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: test-GenMult.
//
// Principal Author: Sarah Rees
//
// Status: in progress
//
// Revision History:
//

#include "GenMult.h"
#include "FPGroup.h"
#include "Set.h"
#include "Word.h"

main()
{
  GenMult GM;
  GM.readFrom();
  GM.printOn(cout);
}

