// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of CM classes for fast problems
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//

#include "viewStructure.h"
#include "fastProblems.h"
#include "OutMessages.h"
#include "conversions.h"

// Includes required by the computations:

//#define ATS


//---------------------------------------------------------------------------//
//------------------------- FastComputation ---------------------------------//
//---------------------------------------------------------------------------//


void FastComputation::viewStructure(ostream& ostr) const
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

FastComputation::FastComputation( ) : ComputationManager( )
{
  resultIsFast();
}

#else

FastComputation::FastComputation( ) : ComputationManager( true )
{
  //attachClient( *this );
  //resultIsFast();
}

#endif


