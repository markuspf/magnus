// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of Mean Analysis Problem
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:


#include "OVDSAnalysisOfMeanProblem.h"
#include "OutMessages.h"
#include "conversions.h"
#include "OneVariableDataSet.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSAnalysisOfMeanProblem::takeControl( )
{
  LogMessage msg( "Analysis Of Mean : Uncompleted");
  msg.send();
}
