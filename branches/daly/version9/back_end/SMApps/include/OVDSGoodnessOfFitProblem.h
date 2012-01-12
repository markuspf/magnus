// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Declaration of classes FullDataAnalysisProblem
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#ifndef _IS_GOODNESSOFFITPROBLEM_H_
#define _IS_GOODNESSOFFITPROBLEM_H_


#include "fastProblems.h"
#include "SMBaseOfDataSet.h"

class OVDSGoodnessOfFitProblem : public FastComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  OVDSGoodnessOfFitProblem( SMBaseOfDataSet& Ds):
  theDataSet ( Ds ) { }

  void takeControl( );              // overrides ComputationManager

private:

  const SMBaseOfDataSet& theDataSet;
};

#endif
