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


#ifndef _IS_OVDSANYLYSISOFMEAN_H_
#define _IS_OVDSANYLYSISOFMEAN_H_


#include "fastProblems.h"
#include "SMDataSetComputationProblems.h"

class OVDSAnalysisOfMeanProblem : public FastComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  OVDSAnalysisOfMeanProblem( SMDataSetFullAnalysis& DsFA) :  theDataSetFA ( DsFA ) 
  { }

  void takeControl( );              // overrides ComputationManager

private:

  const SMDataSetFullAnalysis& theDataSetFA;
};

#endif
