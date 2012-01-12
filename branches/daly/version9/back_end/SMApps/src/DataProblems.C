// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of CM classes for fast problems
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//

#include "viewStructure.h"
#include "DataProblems.h"
#include "OutMessages.h"
#include "conversions.h"

// Includes required by the computations:

#define ATS


//---------------------------------------------------------------------------//
//------------------------- FastComputation ---------------------------------//
//---------------------------------------------------------------------------//


void DataComputation::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Temporary",
		  Text("Temporary"),
		  "none",
		  "Temporary"
		  );

  pv.startItemGroup();
  pv.done();
}

#ifndef ATS

DataComputation::DataComputation( ) : ComputationManager( )
{
  resultIsFast();
}

#else

DataComputation::DataComputation( ) : ComputationManager( true )
{
  resultIsFast();
  showMe=true;
}

#endif


