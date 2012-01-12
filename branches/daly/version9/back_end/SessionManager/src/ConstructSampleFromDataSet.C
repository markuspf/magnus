// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of Construct Sample From Data Set Problem
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:


#include "ConstructSampleFromDataSet.h"
#include "OutMessages.h"
#include "conversions.h"
#include "OneVariableDataSet.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void ConstructSampleFromDataSet::takeControl( )
{
  LogMessage msg( "Construct Sample From Data Set :");
  OneVariableDataSet ds(theDataSet.getSet());
  msg << "Unimplemented! ";
  msg.send();
}
