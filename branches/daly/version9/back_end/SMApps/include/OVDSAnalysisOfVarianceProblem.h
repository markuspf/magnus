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


#ifndef _IS_OVDSANALYSISOFVARIANCE_H_
#define _IS_OVDSANALYSISOFVARIANCE_H_


#include "fastProblems.h"
#include "SMDataSetComputationProblems.h"

class OVDSAnalysisOfVarianceProblem : public FastComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  OVDSAnalysisOfVarianceProblem( SMDataSetFullAnalysis& DsFa) :  theDataSetFa ( DsFa ) 
  { }

  void takeControl( );              // overrides ComputationManager

private:

  const SMDataSetFullAnalysis& theDataSetFa;
};

#endif
