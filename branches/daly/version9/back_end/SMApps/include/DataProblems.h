// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Declarations of CM classes for fast problems
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#ifndef _DATAPROBLEMS_H_
#define _DATAPROBLEMS_H_


#include "ComputationManager.h"

//---------------------------------------------------------------------------//
//------------------------- FastComputation ---------------------------------//
//---------------------------------------------------------------------------//


class DataComputation : public ComputationManager
{
public:

  DataComputation( );

  void readMessage(class istream &) { }       // overrides SMObject

  void viewStructure(ostream& ostr) const;    // overrides SMObject

  void start( ) { }                           // overrides ComputationManager

  void terminate( ) { }                       // overrides ComputationManager
};


#endif 

