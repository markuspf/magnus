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
    M.add( ReadSMCrossClassifiedDataSet(), Text("Attribute Data Set"), DefineCrossClassifiedDataSet() );
    M.add( ReadSMTimeSeriesDataSet(), Text("Time Series Data Set"), DefineTimeSeriesDataSet() );
    M.add( ReadSMQualityControlDataSet(), Text("Quality Control Data Set"), DefineQualityControlDataSet() );
    M.add( ReadSMProbabilityDistribution(), Text("Probability Distribution"), DefineProbabilityDistribution() );
    M.separator();
    M.addSimple(Text("Description of Data Sets"), DefineDescriptionOfDataSet() );
   

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

    M.startItemGroup(CheckinType( 1 ) == SMBaseOfDataSet::ONEVARIABLE);
	 
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

    MDataSet.add(ReadSMGoodnessOfFit(), Text("Goodness Of Fit for ") + Name(1),
	  DefineDataSetGoodnessOfFit());

    MDataSet.add(ReadSMMeanAnalysis(), Text("Analysis of the Mean for ") + Name(1),
	  DefineDataSetMeanAnalysis());

    MDataSet.add( ReadSMVarianceAnalysis(),Text("Analysis of Variance for ") + Name(1),
       	  DefineDataSetVarianceAnalysis());


    MDataSet.add( ReadSMOVDSReportAnalysis(),Text("Report for ") + Name(1),
       	  DefineDataSetOVDSReportAnalysis());

    MDataSet.separator();

    MDataSet.addSimple(Text("Description of Tools"), DefineDescriptionOfOneVariableDataSet() );

   // --------------define the two Variable Tools Menu------------------------ //
    MDataSet.startItemGroup( CheckinType( 1 ) == SMBaseOfDataSet::TWOVARIABLE );
    MDataSet.startCascade( Text("Independent Populations"));
    
    MDataSet.add(ReadSMDataSummary(), Text("Data Summary for ") + Name(1),
	  DefineDataSetDataSummary());

    MDataSet.add(ReadSMFullAnalysis(), Text("Full Analysis for ") + Name(1),
	  DefineDataSetFullAnalysis());
    
    MDataSet.add(ReadSMGraphicAnalysis(), Text("Graphical Analysis for ") + Name(1),
	  DefineDataSetGraphicAnalysis());

    MDataSet.add(ReadSMGoodnessOfFit(), Text("Goodness Of Fit for ") + Name(1),
	  DefineDataSetGoodnessOfFit2());

    MDataSet.add(ReadSMCompareMeanAnalysis(), Text("Comparision of Means for ") + Name(1),
	  DefineDataSetCompareMeanAnalysis());

    MDataSet.add( ReadSMCompareVarianceAnalysis(),Text("Comparision of Variances for ") + Name(1),
       	  DefineDataSetCompareVarianceAnalysis());

    MDataSet.add( ReadSMNonParametricAnalysis(),Text("Non-parametric Tests for ") + Name(1),
       	  DefineDataSetNonParametricAnalysis());

    MDataSet.add( ReadSMTVDSReportIndependent(),Text("Report for ") + Name(1),
       	  DefineDataSetTVDSReportIndependent());

    MDataSet.closeCascade();
    
    MDataSet.startCascade( Text("Regression Analysis"));
    MDataSet.add( ReadSMRegressionFullAnalysis(),Text("Full Regression Analysis for ") + Name(1),
       	  DefineDataSetRegressionFullAnalysis());
    MDataSet.add( ReadSMRegressionGraphicAnalysis(),Text("Graphical Analysis for ") + Name(1),
       	  DefineDataSetRegressionGraphicAnalysis());
    MDataSet.add( ReadSMResidualAnalysis(),Text("Residual Analysis for ") + Name(1),
       	  DefineDataSetResidualAnalysis());
    MDataSet.add( ReadSMPredictionAnalysis(),Text("Prediction and parameter Tests for ") + Name(1),
       	  DefineDataSetPredictionAnalysis());
    MDataSet.add( ReadSMAlternateModelAnalysis(),Text("Alternate Models for ") + Name(1),
       	  DefineDataSetAlternateModelAnalysis());
    MDataSet.add( ReadSMRegressionCorrelationAnalysis(),Text("Correlation Analysis for ") + Name(1),
       	  DefineDataSetRegressionCorrelationAnalysis());
    MDataSet.add( ReadSMTVDSRegressionReportAnalysis(),Text("Report for ") + Name(1),
       	  DefineDataSetTVDSRegressionReportAnalysis());
    
    MDataSet.closeCascade();

    MDataSet.separator();

    MDataSet.addSimple(Text("Description of Tools for a Two Variable Data Set"), DefineDescriptionOfTwoVariableDataSet() );
 // ---------------------------- CrossClassfied Dataset --------------------------------- //

    MDataSet.startItemGroup( CheckinType( 1 ) == SMBaseOfDataSet::CROSSCLASSIFIED);
    MDataSet.startCascade( Text("Proportion Testing"));
    
    MDataSet.add(ReadSMCrossConfidenceOne(), Text("Confidence Interval for One Proportion"),
	  DefineCrossDataSetConfidenceOne());

    MDataSet.add(ReadSMCrossHypothesisOne(), Text("Hypothesis Test for One Proportion "),
	  DefineCrossDataSetHypothesisOne());
    
    MDataSet.add(ReadSMCrossConfidenceTwo(), Text("Confidence Interval for Two Proportion"),
	  DefineCrossDataSetConfidenceTwo());

    MDataSet.add(ReadSMCrossHypothesisTwo(), Text("Hypothesis Test for Two Proportion"),
	  DefineCrossDataSetHypothesisTwo());

    MDataSet.closeCascade();
    
    MDataSet.startCascade( Text("Contingency Table"));

    MDataSet.add(ReadSMCrossTableAnalysis(), Text("Table Analysis"),
	  DefineCrossDataSetTableAnalysis());

    MDataSet.add(ReadSMCrossFollowUpAnalysis(), Text("Follow-up Analysis"),
	  DefineCrossDataSetFollowUpAnalysis());
        
    MDataSet.closeCascade();

    MDataSet.separator();

    MDataSet.addSimple(Text("Description of Tools"), DefineDescriptionOfCrossDataSet() );

    MDataSet.done();
  }
  {
      Menu1<SMDataSetFullAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
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

////-----------------------add two variable dataset help---------------------///

  {
      Menu1<SMDataSetCompareMeanAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Comparing Mean Tools"), DefineDescriptionOfCompareMeanDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetCompareVarianceAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Comparing Variance Tools"), DefineDescriptionOfCompareVarianceDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetNonParametricAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Non-Parametric Testing Tools"), DefineDescriptionOfNonParametricTestDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetRegressionFullAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Full Regression Analysis Tools"), DefineDescriptionOfFullRegressionDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetRegressionGraphicAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Graphical Regression Analysis Tools"), DefineDescriptionOfGraphicalRegressionDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetResidualAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Residual Analysis Tools"), DefineDescriptionOfResidualAnalysisDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetPredictionAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Prediction and Paramter Testing Tools"), DefineDescriptionOfPredictionParameterTestDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetAlternateModelAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Alternate Model Analysis Tools"), DefineDescriptionOfAlternateModelDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetRegressionCorrelationAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Correlation Analysis Tools"), DefineDescriptionOfCorrelationAnalysisDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetTVDSReportIndependentAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Report of Independent Analysis Tools"), DefineDescriptionOfTVDSIndependentReportDataSet() );
      MDataSetCom.done();
  }


  {
      Menu1<SMDataSetTVDSRegressionReportAnalysis> MDataSetCom( Menu::TOOLS );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Report of Regression Analysis Tools"), DefineDescriptionOfTVDSRegressionReportDataSet() );
      MDataSetCom.done();
  }

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

//add help menus for one variable dataset
  {
    Menu1<SMBaseOfDataSet> MDataSet( Menu::HELP);

   // --------------define the One Variable Tools Menu------------------------ //
    MDataSet.startItemGroup(CheckinType( 1 ) == SMBaseOfDataSet::ONEVARIABLE);
    MDataSet.addSimple(Text("Description of One Variable Data Set Tools"), DefineDescriptionOfOneVariableDataSet() );

   // --------------define the One Variable Tools Menu------------------------ //
    MDataSet.startItemGroup(CheckinType( 1 ) == SMBaseOfDataSet::TWOVARIABLE);
    MDataSet.addSimple(Text("Description of Tools for a Two Variable Data Set"), DefineDescriptionOfTwoVariableDataSet() );
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

////-----------------------add two variable dataset help---------------------///


  {
      Menu1<SMDataSetCompareMeanAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Comparing Mean Tools"), DefineDescriptionOfCompareMeanDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetCompareVarianceAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Comparing Variance Tools"), DefineDescriptionOfCompareVarianceDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetNonParametricAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Non-Parametric Testing Tools"), DefineDescriptionOfNonParametricTestDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetRegressionFullAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Full Regression Analysis Tools"), DefineDescriptionOfFullRegressionDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetRegressionGraphicAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Graphical Regression Analysis Tools"), DefineDescriptionOfGraphicalRegressionDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetResidualAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Residual Analysis Tools"), DefineDescriptionOfResidualAnalysisDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetPredictionAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Prediction and Paramter Testing Tools"), DefineDescriptionOfPredictionParameterTestDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetAlternateModelAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Alternate Model Analysis Tools"), DefineDescriptionOfAlternateModelDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetRegressionCorrelationAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Correlation Analysis Tools"), DefineDescriptionOfCorrelationAnalysisDataSet() );
      MDataSetCom.done();
  }

  {
      Menu1<SMDataSetTVDSReportIndependentAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Report of Independent Analysis Tools"), DefineDescriptionOfTVDSIndependentReportDataSet() );
      MDataSetCom.done();
  }


  {
      Menu1<SMDataSetTVDSRegressionReportAnalysis> MDataSetCom( Menu::HELP );
      MDataSetCom.startItemGroup();
      MDataSetCom.addSimple(Text("Description of Report of Regression Analysis Tools"), DefineDescriptionOfTVDSRegressionReportDataSet() );
      MDataSetCom.done();
  }
}
