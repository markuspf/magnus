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


#ifndef _IS_OVDSFULLDATA_ANALYSIS_H_
#define _IS_OVDSFULLDATA_ANALYSIS_H_

#include "DataProblems.h"
#include "SMBaseOfDataSet.h"

class OVDSFullDataAnalysisProblem : public DataComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  OVDSFullDataAnalysisProblem( SMBaseOfDataSet& Ds):
  theDataSet ( Ds ) { }
  void viewStructure(ostream& ostr) const;    // overrides FastComputation
  void takeControl( );              // overrides ComputationManager

private:

  const SMBaseOfDataSet& theDataSet;
};

#endif
