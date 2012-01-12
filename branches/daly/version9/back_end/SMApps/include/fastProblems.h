// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Declarations of CM classes for fast problems
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#ifndef _FASTPROBLEMS_H_
#define _FASTPROBLEMS_H_


#include "ComputationManager.h"

//---------------------------------------------------------------------------//
//------------------------- FastComputation ---------------------------------//
//---------------------------------------------------------------------------//


class FastComputation : public ComputationManager
{
public:

  FastComputation( );

  void readMessage(class istream &) { }       // overrides SMObject

  void viewStructure(ostream& ostr) const;    // overrides SMObject

  void start( ) { }                           // overrides ComputationManager

  void terminate( ) { }                       // overrides ComputationManager
};


#endif 

