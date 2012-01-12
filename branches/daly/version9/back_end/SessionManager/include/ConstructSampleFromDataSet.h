
/*
 *   $Id: ConstructSampleFromDataSet.h,v 1.5 2001/10/24 13 :24:12  $
 */
 
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


#ifndef _CONSTRUCTFROMDATASET_H_
#define _CONSTRUCTFROMDATASET_H_


#include "fastProblems.h"
#include "SMBaseOfDataSet.h"

class ConstructSampleFromDataSet : public FastComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  ConstructSampleFromDataSet( SMBaseOfDataSet& Ds):
  theDataSet ( Ds ) { }

  void takeControl( );              // overrides ComputationManager

private:

  const SMBaseOfDataSet& theDataSet;
};

#endif
