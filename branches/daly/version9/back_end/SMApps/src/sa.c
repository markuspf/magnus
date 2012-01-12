#line 1  "src/fastProblems.C"
/*
 *   $Id: fastProblems.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#line 1  "src/DataProblems.C"
/*
 *   $Id: DataProblems.C,v 1.35 2001/12/02 00:04:04 alex Exp $
 */
 
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


#line 1  "src/menuDefns.C"
/*
 *   $Id: menuDefns.C,v 1.56 2001/04/09 19:32:55 bormotov Exp $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of ObjectSmith methods outputCheckinMenuDefns,
//           outputToolsMenuDefns
//
// Principal Author: Dmitry Bormotov
//
// Status: Always being added to.
//
// Revision History:
//
// * 1/96 Dmitry B. completely rewrote almost all tools menus.
//

#include "ObjectSmith.h"

/////////////////////////////////////////////////////////////////////////
// Include the header for everything that can be made by choosing a
// menu item here:
//

//add the DataSet problems

#include "OVDSDataSummaryProblem.h"
#include "OVDSFullDataAnalysisProblem.h"
#include "OVDSGraphicalAnalysisProblem.h"
#include "OVDSGoodnessOfFitProblem.h"
#include "OVDSAnalysisOfMeanProblem.h"
#include "OVDSAnalysisOfVarianceProblem.h"
#include "OVDSFurtherAnalysisProblem.h"
#include "OVDSReportProblem.h"
#include "OVDSComparisionWithMeanProblem.h"
#include "OVDSComputationOfAnIntervalProblem.h"
#include "OVDSComparisionWithVarianceProblem.h"


// --------------- ObjectSmith::outputCheckinMenuDefns --------------------- //


void ObjectSmith::outputCheckinMenuDefns(ostream& ostr)
{

  {
    Menu0 M( Menu::CHECKIN );

	 
// ----------------------------- STATISTICS ------------------------------- //


    M.startItemGroup( Subject("STATISTICS") );
    //    M.add( ReadSMDataSet(), Text("Data set"), DefineDataSet() );

    M.add( ReadSMOneVariableDataSet(), Text("One Variable Data Set"), DefineOneVariableDataSet() );
    M.add( ReadSMTwoVariableDataSet(), Text("Two Variable Data Set"), DefineTwoVariableDataSet() );
    M.add( ReadSMMultiVariableDataSet(), Text("Multi Variable Data Set"), DefineMultiVariableDataSet() );
    M.add( ReadSMCrossClassifiedDataSet(), Text("Cross-Classified Data Set"), DefineCrossClassifiedDataSet() );
    M.add( ReadSMTimeSeriesDataSet(), Text("Time Series Data Set"), DefineTimeSeriesDataSet() );
    M.add( ReadSMQualityControlDataSet(), Text("Quality Control DataSet"), DefineQualityControlDataSet() );
    M.add( ReadSMProbabilityDistribution(), Text("Probability Distribution"), DefineProbabilityDistribution() );
    M.separator();
    M.addSimple(Text("Description of Data Set"), DefineDescriptionOfDataSet() );
   

// ----------------------------- CALCULUS --------------------------------- //
    M.startItemGroup( Subject("CALCULUS") );
// ----------------------------- GROUP THEORY ------------------------------ //
    M.startItemGroup( Subject("GROUP_THEORY") );
    M.done();
  }

  //----------------------------- Substructures -------------------------------//
  // ----------------------------- STATISTICS ------------------------------- //

  {
    Menu1<SMBaseOfDataSet> M( Menu::CHECKIN );

    M.startItemGroup();
	 
    M.add(ReadSMConstructSampleFromDataSet(), Text("Construct Random Sample From Data Set"),
	  DefineConstructSampleFromDataSet());

  
    M.done();
  }
// ----------------------------- CALCULUS --------------------------------- //

}


//---------------------------------------------------------------------------//
//----------------- ObjectSmith::outputToolsMenuDefns -----------------------//
//---------------------------------------------------------------------------//

void ObjectSmith::outputToolsMenuDefns(ostream& ostr)
{

  {
  // ----------------------------- STATISTICS ------------------------------- //
    Menu1<SMBaseOfDataSet> MDataSet( Menu::TOOLS );


   // --------------define the One Variable Tools Menu------------------------ //
    MDataSet.startItemGroup(CheckinType( 1 ) == SMBaseOfDataSet::ONEVARIABLE);

    MDataSet.add(ReadSMDataSummary(), Text("Data Summary for ") + Name(1),
	  DefineDataSetDataSummary());

    MDataSet.add(ReadSMFullAnalysis(), Text("Full Analysis for ") + Name(1),
	  DefineDataSetFullAnalysis());
    
    MDataSet.add(ReadSMGraphicAnalysis(), Text("Graphical Analysis for ") + Name(1),
	  DefineDataSetGraphicAnalysis());

    MDataSet.add(ReadSMGoodnessOfFit(), Text("GoodNess Of Fit for ") + Name(1),
	  DefineDataSetGoodnessOfFit());

    MDataSet.add(ReadSMMeanAnalysis(), Text("Analysis of the Mean for ") + Name(1),
	  DefineDataSetMeanAnalysis());

    MDataSet.add( ReadSMVarianceAnalysis(),Text("Analysis of Variance for ") + Name(1),
       	  DefineDataSetVarianceAnalysis());

//    MDataSet.add( Ctor1<OVDSFurtherAnalysisProblem, SMBaseOfDataSet>(),
//                  Text("Suggestions for Further Analysis for ") + Name(1)
//       	   );

    MDataSet.add( Ctor1<OVDSReportProblem, SMBaseOfDataSet>(),
                  Text("Report for ") + Name(1)
       	   );

    MDataSet.separator();

    MDataSet.addSimple(Text("Description of Tools"), DefineDescriptionOfOneVariableDataSet() );

   // --------------define the two Variable Tools Menu------------------------ //
    MDataSet.startItemGroup( CheckinType( 1 ) == SMBaseOfDataSet::TWOVARIABLE );
    MDataSet.startCascade( Text("Data Summary Two Variable"));

    MDataSet.closeCascade();
    MDataSet.done();
  }
  {
      Menu1<SMDataSetFullAnalysis> MDataSetCom( Menu::TOOLS );

      MDataSetCom.startItemGroup();
/*      MDataSetCom.add( Ctor1<OVDSAnalysisOfMeanProblem, SMDataSetFullAnalysis>(),
                  Text("Analysis of the Mean for ") + Name(1)
       	   );

      MDataSetCom.add( Ctor1<OVDSAnalysisOfVarianceProblem, SMDataSetFullAnalysis>(),
                  Text("Analysis of Variance for ") + Name(1)
       	   );
     	   
*/
      MDataSetCom.addSimple(Text("Description of Full analysis Tools"), DefineDescriptionOfFullAnalysis() );

      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetDataSummary> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Data Summary Tools"), DefineDescriptionOfDataSummary() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetGraphicAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Graphic Analysis Tools"), DefineDescriptionOfGraphicAnalysis() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetMeanAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Mean Analysis Tools"), DefineDescriptionOfMeanAnalysis() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetVarianceAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Variance Analysis Tools"), DefineDescriptionOfVarianceAnalysis() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetGoodnessOfFit> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Goodness Of Fit Tools"), DefineDescriptionOfGoodnessOfFit() );
      MDataSetCom.done();
  }

/*  {
      Menu1<SMDataSetReportProblem> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Report Tools"), DefineDescriptionOfReportProblem() );
      MDataSetCom.done();
  }
*/  

 // ---------------------------- CALCULUS --------------------------------- //
}

// ----------------- ObjectSmith::outputMakeMenuDefns ---------------------- //


void ObjectSmith::outputPackagesMenuDefns(ostream& ostr)
{

}


// ----------------- ObjectSmith::outputMakeMenuDefns ---------------------- //


void ObjectSmith::outputMakeMenuDefns(ostream& ostr)
{

}


// --------------- ObjectSmith::outputTestingMenuDefns -------------------- //


void ObjectSmith::outputTestingMenuDefns(ostream& ostr)
{
  {
    Menu0 M( Menu::TESTING );

    M.startItemGroup( );
    
    M.addDisabled( "Nothing" );

    M.done();
  }
}

// --------------- ObjectSmith::outputHelpMenuDefns -------------------- //


void ObjectSmith::outputHelpMenuDefns(ostream& ostr)
{
  {
    Menu0 M( Menu::HELP );

// ----------------------------- STATISTICS ------------------------------- //


    M.startItemGroup( Subject("STATISTICS") );
    
    M.addSimple( "Help on Checking in Data Set ", showDataSetHelp());


    M.done();
  }
//add help menus for one variable dataset
  {
    Menu1<SMBaseOfDataSet> MDataSet( Menu::HELP);

   // --------------define the One Variable Tools Menu------------------------ //
    MDataSet.startItemGroup(CheckinType( 1 ) == SMBaseOfDataSet::ONEVARIABLE);
    MDataSet.addSimple(Text("Description of Tools"), DefineDescriptionOfOneVariableDataSet() );
    MDataSet.done();
  }
  {
      Menu1<SMDataSetFullAnalysis> MDataSetCom( Menu::HELP );

      MDataSetCom.startItemGroup();

      MDataSetCom.addSimple(Text("Description of Full analysis Tools"), DefineDescriptionOfFullAnalysis() );

      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetDataSummary> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Data Summary Tools"), DefineDescriptionOfDataSummary() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetGraphicAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Graphic Analysis Tools"), DefineDescriptionOfGraphicAnalysis() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetMeanAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Mean Analysis Tools"), DefineDescriptionOfMeanAnalysis() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetVarianceAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Variance Analysis Tools"), DefineDescriptionOfVarianceAnalysis() );
      MDataSetCom.done();
  }
  {
      Menu1<SMDataSetGoodnessOfFit> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Goodness Of Fit Tools"), DefineDescriptionOfGoodnessOfFit() );
      MDataSetCom.done();
  }


}
#line 1  "src/OVDSFullDataAnalysisProblem.C"
/*
 *   $Id: OVDSFullDataAnalysisProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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
#line 1  "src/OVDSGraphicalAnalysisProblem.C"
/*
 *   $Id: OVDSGraphicalAnalysisProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSGraphicalAnalysisProblem.h"
#include "OutMessages.h"
#include "conversions.h"
#include "OneVariableDataSet.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSGraphicalAnalysisProblem::takeControl( )
{
//  LogMessage msg( "by xuxw:");

//  msg << " OVDSGraphicalAnalysisProblem Has not been implemented. ";
//  msg.send();
}

void OVDSGraphicalAnalysisProblem::viewStructure(ostream& ostr) const
{
   GraphicView gv( ostr, oid(), "Graphic Analysis",
		  Text("GraphicAnalysis"),
		  "none",
		  "none"
		  );

  gv.done();
 
}

void OVDSGraphicalAnalysisProblem::printDefinition(ostream& ostr) const{
  OneVariableDataSet ds(theDataSet.getSet());
  ds.sort();
  ds.printDefinition(ostr);
}

#line 1  "src/OVDSGoodnessOfFitProblem.C"
/*
 *   $Id: OVDSGoodnessOfFitProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSGoodnessOfFitProblem.h"
#include "OutMessages.h"
#include "conversions.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSGoodnessOfFitProblem::takeControl( )
{
  LogMessage msg( "by xuxw:");

  msg << " OVDSGoodnessOfFitProblem Has not been implemented. ";

  msg.send();
}
#line 1  "src/OVDSAnalysisOfMeanProblem.C"
/*
 *   $Id: OVDSAnalysisOfMeanProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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
#line 1  "src/OVDSAnalysisOfVarianceProblem.C"
/*
 *   $Id: OVDSAnalysisOfVarianceProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSAnalysisOfVarianceProblem.h"
#include "OutMessages.h"
#include "conversions.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSAnalysisOfVarianceProblem::takeControl( )
{
  LogMessage msg( "by xuxw:");

  msg << " OVDSAnalysisOfVarianceProblem Has not been implemented. ";

  msg.send();
}
#line 1  "src/OVDSFurtherAnalysisProblem.C"
/*
 *   $Id: OVDSFurtherAnalysisProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSFurtherAnalysisProblem.h"
#include "OutMessages.h"
#include "conversions.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSFurtherAnalysisProblem::takeControl( )
{
  LogMessage msg( "by xuxw:");

  msg << " OVDSFurtherAnalysisProblem Has not been implemented. ";

  msg.send();
}
#line 1  "src/OVDSReportProblem.C"
/*
 *   $Id: OVDSReportProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSReportProblem.h"
#include "OutMessages.h"
#include "conversions.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSReportProblem::takeControl( )
{
  LogMessage msg( "by xuxw:");

  msg << " OVDSReportProblem Has not been implemented. ";

  msg.send();
}
#line 1  "src/OVDSComparisionWithMeanProblem.C"
/*
 *   $Id: OVDSComparisionWithMeanProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSComparisionWithMeanProblem.h"
#include "OutMessages.h"
#include "conversions.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSComparisionWithMeanProblem::takeControl( )
{
  LogMessage msg( "by xuxw:");

  msg << " OVDSComparisionWithMeanProblem Has not been implemented. ";

  msg.send();
}
#line 1  "src/OVDSComputationOfAnIntervalProblem.C"
/*
 *   $Id: OVDSComputationOfAnIntervalProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSComputationOfAnIntervalProblem.h"
#include "OutMessages.h"
#include "conversions.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSComputationOfAnIntervalProblem::takeControl( )
{
  LogMessage msg( "by xuxw:");

  msg << " OVDSComputationOfAnIntervalProblem Has not been implemented. ";

  msg.send();
}
#line 1  "src/OVDSComparisionWithVarianceProblem.C"
/*
 *   $Id: OVDSComparisionWithVarianceProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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


#include "OVDSComparisionWithVarianceProblem.h"
#include "OutMessages.h"
#include "conversions.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void OVDSComparisionWithVarianceProblem::takeControl( )
{
  LogMessage msg( "by xuxw:");

  msg << " OVDSComparisionWithVarianceProblem Has not been implemented. ";

  msg.send();
}
#line 1  "src/OVDSDataSummaryProblem.C"
/*
 *   $Id: OVDSDataSummaryProblem.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
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
