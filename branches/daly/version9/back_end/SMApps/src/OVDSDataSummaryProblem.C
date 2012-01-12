// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of Full Data Analysis Problem
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:


#include "OVDSDataSummaryProblem.h"
#include "OutMessages.h"
#include "conversions.h"
#include "OneVariableDataSet.h"

//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSDataSummaryProblem::takeControl( )
{
  OneVariableDataSet ds(theDataSet.getSet());
  ds.sort();  
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " Mean="),"FurtherAnylysis","meanAnalysis.txt" ) 
            << ds.mean() ;
     msg.send();}
}

void OVDSDataSummaryProblem::viewStructure(ostream& ostr) const
{
  SummaryView sv( ostr, oid(), "Summary Data Analysis result",
		  Text("DataSummaryAnalysis"),
		  "none",
		  "none"
		  );
  sv.done();
}

void OVDSDataSummaryProblem::printDefinition(ostream& ostr) const{
  OneVariableDataSet ds(theDataSet.getSet());
  ds.sort();
  ds.printDefinition(ostr);
}
