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


#include "OVDSFullDataAnalysisProblem.h"
#include "OutMessages.h"
#include "conversions.h"
#include "OneVariableDataSet.h"

//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSFullDataAnalysisProblem::takeControl( )
{
  OneVariableDataSet ds(theDataSet.getSet());
  ds.sort();  
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " Mean="),"FurtherAnylysis","meanAnalysis.txt" ) 
            << ds.mean() ;
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " Variance="),"FurtherAnylysis",
  		  "varianceAnalysis.txt" ) << ds.deviation() ;
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " Standard Deviation="),"FurtherAnylysis",
  		  "standardDeviationAnalysis.txt" ) << ds.standardDeviation() ;
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " Range="),"FurtherAnylysis",
  		  "rangeAnalysis.txt" ) << ds.range() ; 
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " Median="),"FurtherAnylysis",
  		  "medianAnalysis.txt" ) << ds.median() ; 
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " first Quartile="),"FurtherAnylysis",
  		  "firstQuartileAnalysis.txt" ) << ds.firstQuartile() ; 
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " Third Quartile="),"FurtherAnylysis",
  		  "thirdQuartileAnalysis.txt" ) << ds.thirdQuartile() ; 
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(Chars( " coefficient of Skewness="),"FurtherAnylysis",
  		  "coefficientSkewnessAnalysis.txt" ) << ds.coefficientSkewness(); 
     msg.send();}
}

void OVDSFullDataAnalysisProblem::viewStructure(ostream& ostr) const
{
  ResultView rv( ostr, oid(), "Full Data Analysis result",
		  Text("FullDataAnalysis"),
		  "none",
		  "none"
		  );
  rv.done();

}
