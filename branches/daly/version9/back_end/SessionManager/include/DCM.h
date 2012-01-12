// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Declaration of class DCM
//
// Principal Author: Xiaowei XU
//
// Status: in progress
//
// Revision History:
//


#ifndef _DCM_H_
#define _DCM_H_

#include "DataSet.h"
#include "ComputationManager.h"
#include "Supervisor.h"
#include "DIC.h"

// Computation Managers supervised by a DCM:


//---------------------------------------------------------------------------//
//-------------------------------- DCM --------------------------------------//
//---------------------------------------------------------------------------//


class DCM : public Supervisor
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  DCM( class SMBaseOfDataSet& D );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMBaseOfDataSet& getSMDataSet( ) const { return theSMDataSet; }

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void viewStructure(ostream& ostr) const;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Control:                                                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void takeControl( );        // overrides ComputationManager

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // State Transitions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void start( );              // overrides ComputationManager

  void terminate( );          // overrides ComputationManager


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  class SMBaseOfDataSet& theSMDataSet;

  bool didFastChecks;
  // The takeControl method does some sneaky checks of obvious properties
  // (this should be formalized, since they are not always that fast).
  // This records the first call, so that they are not done over and over.

};

#endif
