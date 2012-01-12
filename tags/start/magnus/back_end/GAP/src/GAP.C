/*
 *   $Id: GAP.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of class GAPManager
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "GAPManager.h"


// ----------------------------- GAPManager -------------------------------- //







// Static initializations:

const int GAPManager::maxNumOfGaps = 3;
int GAPManager::theNumOfGaps = 0;
GAP* GAPManager::gap = 0;
