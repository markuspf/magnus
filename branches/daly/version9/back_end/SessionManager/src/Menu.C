// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes Ctor<n>, Menu<n>
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "Chars.h"
#include "Menu.h"
#include "ObjectSmith.h"
#include "SessionManager.h"
#include "TheObjects.h"
#include "OutMessages.h"

// Includes required for special cases:

#include "TheObjects.h"
#include "Type.h"
#include "ObjectFactory.h"

//---------------------------------------------------------------------------//
//------------------------------ Ctor<n> ------------------------------------//
//---------------------------------------------------------------------------//


void Ctor::newDependent(SMObject* smo, OID oid) const
{
  //@db smo->addDependent( oid );
  // addDependent() is now called by CheckinMessage 
}


SMObject* Ctor::get(int oid) const
{
  return TheObjects::get( OID(oid) );
}


void CtorAux0::readFrom(istream& istr) const {
  Chars name;
  SMObject* smo = construct( name, istr );
  if ( smo ) {
	 if ( smo->displayInFE() ) {
		ListOf<OID> dependencies;
		CheckinMessage( *smo, name, dependencies ).send();
	 }
  }
}


void CtorAux1::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  Chars name;
  SMObject* smo = construct( arg1, name, istr );
  if ( smo ) {
	 newDependent( arg1, *smo );
	 if ( smo->displayInFE() ) {
		ListOf<OID> dependencies( *arg1 );
		CheckinMessage( *smo, name, dependencies ).send();
	 }
	 //@rn Factor out somewhere
  }
  smo->sendResult();
}


void CtorAux2::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  istr >> i;
  SMObject* arg2 = get( i );
  if ( arg2 == 0 ) return;
  Chars name;
  SMObject* smo;
  if ( transposeARGS )
	 smo = construct( arg2, arg1, name, istr );
  else
	 smo = construct( arg1, arg2, name, istr );
  if ( smo ) {
	 newDependent( arg1, *smo );
	 newDependent( arg2, *smo );
	 if ( smo->displayInFE() ) {
		ListOf<OID> dependencies( *arg1 );
		dependencies.append( *arg2 );
		CheckinMessage( *smo, name, dependencies ).send();
	 }
	 //@rn Factor out somewhere
  }
}


void CtorAux3::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  istr >> i;
  SMObject* arg2 = get( i );
  if ( arg2 == 0 ) return;
  istr >> i;
  SMObject* arg3 = get( i );
  if ( arg3 == 0 ) return;
 
  Chars name;
  SMObject* smo = construct( arg1, arg2, arg3, name, istr );
  if ( smo ) {
    newDependent( arg1, *smo );
    newDependent( arg2, *smo );
    newDependent( arg3, *smo );
    if ( smo->displayInFE() ) {
      ListOf<OID> dependencies( *arg1 );
      dependencies.append( *arg2 );
      dependencies.append( *arg3 );
      CheckinMessage( *smo, name, dependencies ).send();
    }
    //@rn Factor out somewhere
  }
}


void CtorAux4::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  istr >> i;
  SMObject* arg2 = get( i );
  if ( arg2 == 0 ) return;
  istr >> i;
  SMObject* arg3 = get( i );
  if ( arg3 == 0 ) return;
  istr >> i;
  SMObject* arg4 = get( i );
  if ( arg4 == 0 ) return;

  Chars name;
  SMObject* smo = construct( arg1, arg2, arg3, arg4, name, istr );
  if ( smo ) {
    newDependent( arg1, *smo );
    newDependent( arg2, *smo );
    newDependent( arg3, *smo );
    newDependent( arg4, *smo );
    if ( smo->displayInFE() ) {
      ListOf<OID> dependencies( *arg1 );
      dependencies.append( *arg2 );
      dependencies.append( *arg3 );
      dependencies.append( *arg4 );
      CheckinMessage( *smo, name, dependencies ).send();
    }
    //@rn Factor out somewhere
  }
}



//---------------------------------------------------------------------------//
//------------------------------- Ctor --------------------------------------//
//--------------------------- special cases ---------------------------------//

// ---------------------------- STATISTICS ------------------------------------
SMObject*
ReadSMSimpleMenu::construct( Chars& name, istream& istr) const
{
  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {

    return new SMBaseOfDataSet( S, Text("User defined data set") );
    
  } else {
    
    ParseErrorMessage( name, err ).send();
    
    return 0;
  }
}

//add by xuxw
SMObject*
ReadSMOneVariableDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;
  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined One Variable Data Set", SMBaseOfDataSet::ONEVARIABLE, name );

	 smo->dic.putIsOneVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject*
ReadSMTwoVariableDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Two Variable Data Set", SMBaseOfDataSet::TWOVARIABLE );

	 smo->dic.putIsTwoVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}


SMObject*
ReadSMMultiVariableDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Multi Variable Data Set", SMBaseOfDataSet::MULTIVARIABLE );

//	 smo->gic.putIsMultiVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject*
ReadSMCrossClassifiedDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Cross Classified DataSet", SMBaseOfDataSet::CROSSCLASSIFIED );

	 smo->dic.putIsCrossClassifiedDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject*
ReadSMTimeSeriesDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Time Series Data Set", SMBaseOfDataSet::TIMESERIES );

//	 smo->gic.putIsTimeSeriesDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}


SMObject*
ReadSMQualityControlDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Quality Control Data Set", SMBaseOfDataSet::QUALITYCONTROL );

//	 smo->gic.putIsQualityControlDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}



SMObject*
ReadSMProbabilityDistribution::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Probability Distribution ", SMBaseOfDataSet::PROBABILITY );

//	 smo->gic.putIsProbabilityDistribution();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}


SMObject* ReadSMConstructSampleFromDataSet::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int size;
  DataSet S;
  istr >> name;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined One Sample Data Set", SMBaseOfDataSet::ONEVARIABLE );

	 smo->dic.putIsOneVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject* ReadSMDataSummary::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  return new SMDataSetDataSummary( parent, "summaryView", (Chars)("DATA SUMMARY FOR ")+parent.getName());
}

SMObject* ReadSMFullAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  return new SMDataSetFullAnalysis( parent, "resultView", "DataSet Full Analysis");
}

SMObject* ReadSMGraphicAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  return new SMDataSetGraphicAnalysis( parent, "graphicView", (Chars)("Graphical Analysis of ")+parent.getName());
}


SMObject* ReadSMRegressionGraphicAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  return new SMDataSetRegressionGraphicAnalysis( parent, "regressionGraphicView", "DataSet Graphical Analysis");
}


SMObject* ReadSMGoodnessOfFit::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  int setNum;
  istr >> Type;
  istr >> level;
  istr >> setNum;

  SMDataSetGoodnessOfFit* smo
		= new SMDataSetGoodnessOfFit( parent, "resultView", "Goodness of Fit Analysis");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  smo->m_setNum=setNum;	 
  return smo;

}

SMObject* ReadSMMeanAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  double MU0;
  char MU0Type;
  istr >> Type;
  istr >> level;
  if(Type=='H') {
     istr >> MU0;
     istr >> MU0Type;
  }
  SMDataSetMeanAnalysis* smo
		= new SMDataSetMeanAnalysis( parent, "resultView", "Analysis of the Mean for a One Valriable Data Set");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  smo->m_MU0=MU0;	 
  smo->m_MU0Type=MU0Type;	 
  return smo;

}

SMObject* ReadSMOVDSReportAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  OVDSReportParam P;
  Chars err = istr >> P;

  SMOVDSReportAnalysis* smo
		= new SMOVDSReportAnalysis( parent, "ODVSReportView", "One-Variable DataSet Report",P);
  return smo;
}

SMObject* ReadSMVarianceAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  double MU0;
  char MU0Type;
  istr >> Type;
  istr >> level;
  if(Type=='H') {
     istr >> MU0;
     istr >> MU0Type;
  }

  SMDataSetVarianceAnalysis* smo
		= new SMDataSetVarianceAnalysis( parent, "resultView", "DataSet Variance Analysis");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  smo->m_MU0=MU0;	 
  smo->m_MU0Type=MU0Type;	 
  return smo;

}

// --------------------------two variable regression analysis tools------///

SMObject* ReadSMNonParametricAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  double level;
  istr >> level;
  SMDataSetNonParametricAnalysis* smo
		= new SMDataSetNonParametricAnalysis( parent, "resultView", "DataSet Non-Parametric Analysis");
  smo->m_Level=level;
  return smo;
}

SMObject* ReadSMCompareMeanAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  double MU0;
  char MU0Type;
  char equalAssumed;
  istr >> Type;
  istr >> level;
  if(Type=='H') {
     istr >> MU0;
     istr >> MU0Type;
     istr >> equalAssumed;
  }

  SMDataSetCompareMeanAnalysis* smo
		= new SMDataSetCompareMeanAnalysis( parent, "resultView", "DataSet Compare Mean Analysis");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  smo->m_MU0=MU0;	 
  smo->m_MU0Type=MU0Type;
  smo->m_EqualAssumed=(equalAssumed=='T')? true : false ;	 
  return smo;
}


SMObject* ReadSMCompareVarianceAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  istr >> Type;
  istr >> level;

  SMDataSetCompareVarianceAnalysis* smo
		= new SMDataSetCompareVarianceAnalysis( parent, "resultView", "DataSet Compare Variance Analysis");
  smo->m_Type=Type;	 
  smo->m_Level=level;	 
  return smo;
}

SMObject* ReadSMRegressionFullAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;

  SMDataSetRegressionFullAnalysis* smo
		= new SMDataSetRegressionFullAnalysis( parent, "resultView", "DataSet Full Regression Analysis");
  return smo;
}

SMObject* ReadSMResidualAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;

  SMDataSetResidualAnalysis* smo
		= new SMDataSetResidualAnalysis( parent, "residualView", "DataSetResidual Analysis");
  return smo;
}

///format {type level specialParameters}
SMObject* ReadSMPredictionAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double Level,PredictedPoint;
  double MU0;
  char MU0Type;
  TVDSAllPredictionParam param;
  istr >> Type;
  if(Type=='S'||Type=='I') {
     istr >> Level;
     istr >> MU0;
     istr >> MU0Type;
  }
  if(Type=='P') {
     istr >> Level;
     istr >> PredictedPoint;
  }

  if(Type=='A') {
     istr >> param;
  }

  if(Type=='s'||Type=='i'||Type=='M') {
     istr >> Level;
  }

  SMDataSetPredictionAnalysis* smo
		= new SMDataSetPredictionAnalysis( parent, "resultView", "DataSet Prediction and Parameter Testing",param);
  smo->m_Level=Level;	 
  smo->m_Type=Type;	 
  smo->m_MU0=MU0;	 
  smo->m_MU0Type=MU0Type;
  smo->m_PredictedPoint=PredictedPoint;
  return smo;
}

SMObject* ReadSMAlternateModelAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  istr >> Type;
  SMDataSetAlternateModelAnalysis* smo
		= new SMDataSetAlternateModelAnalysis( parent, "resultView", "DataSet Alternate Model Analysis");
  smo->m_Type=Type;	 
  return smo;
}

SMObject* ReadSMRegressionCorrelationAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  double MU0;
  char MU0Type;
  istr >> Type;
  istr >> level;
  if(Type=='H') {
     istr >> MU0;
     istr >> MU0Type;
  }

  SMDataSetRegressionCorrelationAnalysis* smo
		= new SMDataSetRegressionCorrelationAnalysis( parent, "resultView", "DataSet Correlation Analysis");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  smo->m_MU0=MU0;	 
  smo->m_MU0Type=MU0Type;	 
  return smo;
}

SMObject* ReadSMTVDSReportIndependent::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  TVDSReportIndependentParam P;
  Chars err = istr >> P;
  SMDataSetTVDSReportIndependentAnalysis* smo
	= new SMDataSetTVDSReportIndependentAnalysis( parent, "TVDSReportIndependentView", "DataSet Independent Report Analysis",P);
  return smo;
}

SMObject* ReadSMTVDSRegressionReportAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  TVDSReportRegressionParam P;
  Chars err = istr >> P;

  SMDataSetTVDSRegressionReportAnalysis* smo
		= new SMDataSetTVDSRegressionReportAnalysis( parent, "TVDSReportRegressionView", "DataSet Regression Report Analysis",P);
  return smo;
}
// ---------------------------- Cross  Dataset--------------------------------- //
SMObject* ReadSMCrossConfidenceOne::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  double Level;
  istr >> Level;
  SMCrossDataSetConfidenceOne* smo
	= new SMCrossDataSetConfidenceOne( parent, "resultView", "Attribute DataSet Confidence Interval for One Proportion");
  smo->m_Level=Level;
  return smo;
}

SMObject* ReadSMCrossHypothesisOne::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  double Level,MU0;
  char Type;
  istr >> Level;
  istr >> MU0;
  istr >> Type;
  SMCrossDataSetHypothesisOne* smo
	= new SMCrossDataSetHypothesisOne( parent, "resultView", "Attribute DataSet Hypothesis Test for One Proportion");
  smo->m_Level=Level;
  smo->m_MU0=MU0;
  smo->m_Type=Type;
  return smo;
}

SMObject* ReadSMCrossConfidenceTwo::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  double Level;
  istr >> Level;
  SMCrossDataSetConfidenceTwo* smo
	= new SMCrossDataSetConfidenceTwo( parent, "resultView", "Attribute DataSet Confidence Interval for Two Proportion");
  smo->m_Level=Level;
  return smo;
}

SMObject* ReadSMCrossHypothesisTwo::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  double Level,MU0;
  char Type;
  istr >> Level;
  istr >> MU0;
  istr >> Type;
  SMCrossDataSetHypothesisTwo* smo
	= new SMCrossDataSetHypothesisTwo( parent, "resultView", "Attribute DataSet Hypothesis Test for Two Proportion");
  smo->m_Level=Level;
  smo->m_MU0=MU0;
  smo->m_Type=Type;
  return smo;
}

SMObject* ReadSMCrossTableAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  double Level;
  Chars RowFactor,ColumnFactor;
  istr >> Level;
  istr >> RowFactor;
  istr >> ColumnFactor;
  SMCrossDataSetTableAnalysis* smo
	= new SMCrossDataSetTableAnalysis( parent, "resultView", "Attribute DataSet Table Analysis");
  smo->m_Level=Level;
  smo->m_RowFactor=RowFactor;
  smo->m_ColumnFactor=ColumnFactor;
  return smo;
}

SMObject* ReadSMCrossFollowUpAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  SMCrossDataSetFollowUpAnalysis* smo
	= new SMCrossDataSetFollowUpAnalysis( parent, "resultView", "Attribute DataSet Follow-up Analysis");
  return smo;
}

// ---------------------------- CALCULUS --------------------------------- //

//---------------------------------------------------------------------------//
//-------------------------- Menu::Action -----------------------------------//
//---------------------------------------------------------------------------//

// -------------------------- STATISTICS --------------------------------- //


void Menu::DefineDataSet::print(ostream& ostr) const
{
  ostr << "_defineDataSet_ {Define a data set}";
}


void Menu::DefineOneVariableDataSet::print(ostream& ostr) const
{
  ostr << "_defineOneVariableDataSet_ {Define a one variable data set}";
}

void Menu::DefineTwoVariableDataSet::print(ostream& ostr) const
{
  ostr << "_defineTwoVariableDataSet_ {Define a two variable data set}";
}

void Menu::DefineMultiVariableDataSet::print(ostream& ostr) const
{
  ostr << "_defineMultiVariableDataSet_ {Define a multi variable data set}";
}

void Menu::DefineCrossClassifiedDataSet::print(ostream& ostr) const
{
  ostr << "_defineCrossClassifiedDataSet_ {Define a cross classified data set}";
}

void Menu::DefineTimeSeriesDataSet::print(ostream& ostr) const
{
  ostr << "_defineTimeSeriesDataSet_ {Define a time series data set}";
}

void Menu::DefineQualityControlDataSet::print(ostream& ostr) const
{
  ostr << "_defineQualityControlDataSet_ {Define a quality control data set}";
}

void Menu::DefineProbabilityDistribution::print(ostream& ostr) const
{
  ostr << "_defineProbabilityDistribution_ {Define a probability distribution}";
}

void Menu::showDataSetHelp::print(ostream& ostr) const
{
   ostr << "showHelp {DescriptionDataSet::Description Of Data Set}  ";
};

void Menu::DefineDescriptionOfDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {DescriptionDataSet::Description Of Data Set} ";
}

void Menu::DefineConstructSampleFromDataSet::print(ostream& ostr) const
{
   ostr << "_constructSampleFromDataSet_ {Construct Random Sample from Data Set}";
}


void Menu::DefineDataSetDataSummary::print(ostream& ostr) const
{
  ostr << "_defineDataSetDataSummary_ {Data Summary a Data Set}";
}

void Menu::DefineDataSetFullAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetFullAnalysis_ {Full Analysis a Data Set}";
}

void Menu::DefineDataSetGraphicAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetGraphicAnalysis_ {Graphic Analysis a Data Set}";
}

void Menu::DefineDataSetRegressionGraphicAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetRegressionGraphicAnalysis_ {Regression Graphic Analysis a Data Set}";
}

void Menu::DefineDataSetGoodnessOfFit::print(ostream& ostr) const
{
  ostr << "_defineDataSetGoodnessOfFit_ {Fit Analysis a Data Set}";
}

void Menu::DefineDataSetGoodnessOfFit2::print(ostream& ostr) const
{
  ostr << "_defineDataSetGoodnessOfFit2_ {Fit Analysis a Data Set}";
}
  
void Menu::DefineDataSetMeanAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetMeanAnalysis_ {Analysis of the mean for a One Variable Data Set}";
}

void Menu::DefineDataSetVarianceAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetVarianceAnalysis_ {Variance Analysis a Data Set}";
}

void Menu::DefineDataSetOVDSReportAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetOVDSReportAnalysis_ {Compare Variance Analysis a Data Set}";
}

void Menu::DefineDescriptionOfOneVariableDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfOneVariableDataSetTools::Description of One Variable Data Set Tools}  ";
}

void Menu::DefineDescriptionOfFullAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {fullAnalysis::How Full Analysis Works}  ";
}

void Menu::DefineDescriptionOfDataSummary ::print(ostream& ostr) const
{
   ostr << "showHelp {dataSummary::How Data Summary Works}  ";
}
void Menu::DefineDescriptionOfGraphicAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {graphicAnalysis::How Graphical Analysis Works}  ";
}
void Menu::DefineDescriptionOfMeanAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {meanAnalysis::How Mean Analysis Works}  ";
}
void Menu::DefineDescriptionOfVarianceAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {varianceAnalysis::How Variance Analysis Works}  ";
}
void Menu::DefineDescriptionOfGoodnessOfFit::print(ostream& ostr) const
{
   ostr << "showHelp {goodnessOfFit::How Goodness Of Fit Works}  ";
}

void Menu::DefineDescriptionOfTwoVariableDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfTwoVariableDataSetTools::Description of Two Variable Data Set Tools}  ";
}

void Menu::DefineDescriptionOfCompareMeanDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfCompareMean::Description of Compare Mean Analysis}  ";
}

void Menu::DefineDescriptionOfCompareVarianceDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfCompareVariance::Description of Compare Variance Analysis}  ";
}

void Menu::DefineDescriptionOfNonParametricTestDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfNonParametricTest::Description of Non-Parametric Testing}  ";
}

void Menu::DefineDescriptionOfFullRegressionDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfFullRegression::Description of Full Regression Analysis}  ";
}

void Menu::DefineDescriptionOfGraphicalRegressionDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfGraphicalRegression::Description of Graphical Regression Analysis}  ";
}

void Menu::DefineDescriptionOfResidualAnalysisDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfResidualAnalysis::Description of Residual Analysis}  ";
}

void Menu::DefineDescriptionOfPredictionParameterTestDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfPredictionParameterTest::Description of Prediction and Parameter Testing}  ";
}

void Menu::DefineDescriptionOfAlternateModelDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfAlternateModel::Description of Alternate Model Testing}  ";
}

void Menu::DefineDescriptionOfCorrelationAnalysisDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfCorrelationAnalysis::Description of Correlation Analysis}  ";
}

void Menu::DefineDescriptionOfTVDSIndependentReportDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfTVDSIndependentReport::Description of Two Variable Independent Report}  ";
}

void Menu::DefineDescriptionOfTVDSRegressionReportDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfTVDSRegressionReport::Description of Two Variable Regression Report}  ";
}

//----------------two variable regression analysis tools--------////

void Menu::DefineDataSetCompareMeanAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetCompareMeanAnalysis_ {Compare Mean Analysis a Data Set}";
}

void Menu::DefineDataSetCompareVarianceAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetCompareVarianceAnalysis_ {Compare Variance Analysis a Data Set}";
}

void Menu::DefineDataSetNonParametricAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetNonParametricAnalysis_ {Non-parametric Analysis a Data Set}";
}

void Menu::DefineDataSetRegressionFullAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetRegressionFullAnalysis_ {Regression Full Analysis a Data Set}";
}

void Menu::DefineDataSetResidualAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetResidualAnalysis_ {Residual Analysis a Data Set}";
}

void Menu::DefineDataSetPredictionAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetPredictionAnalysis_ {Prediction and Parameter Testing a Data Set}";
}

void Menu::DefineDataSetAlternateModelAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetAlternateModelAnalysis_ {Alternate Model for a Data Set}";
}

void Menu::DefineDataSetRegressionCorrelationAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetRegressionCorrelationAnalysis_ {Correlation Analysis a Data Set}";
}

void Menu::DefineDataSetTVDSReportIndependent::print(ostream& ostr) const
{
  ostr << "_defineDataSetTVDSReportIndependent_ {Independent Report a Data Set}";
}

void Menu::DefineDataSetTVDSRegressionReportAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetTVDSRegressionReportAnalysis_ { Regression Report a Data Set}";
}

//----------------cross dataset analysis tools--------////

void Menu::DefineCrossDataSetConfidenceOne::print(ostream& ostr) const
{
  ostr << "_defineCrossDataSetConfidenceOneAnalysis_ {DataSetConfidence Interval for One Proportion}";
}

void Menu::DefineCrossDataSetHypothesisOne::print(ostream& ostr) const
{
  ostr << "_defineCrossDataSetHypothesisOneAnalysis_ {DataSet Hypothesis Test for One Proportion}";
}

void Menu::DefineCrossDataSetConfidenceTwo::print(ostream& ostr) const
{
  ostr << "_defineCrossDataSetConfidenceTwoAnalysis_ {DataSetConfidence Interval for Two Proportion}";
}

void Menu::DefineCrossDataSetHypothesisTwo::print(ostream& ostr) const
{
  ostr << "_defineCrossDataSetHypothesisTwoAnalysis_ {DataSet Hypothesis Test for Two Proportion}";
}

void Menu::DefineCrossDataSetTableAnalysis::print(ostream& ostr) const
{
  ostr << "_defineCrossDataSetTableAnalysis_ {Dataset Table Analysi}";
}

void Menu::DefineCrossDataSetFollowUpAnalysis::print(ostream& ostr) const
{
  ostr << "_defineCrossDataSetFollowUpAnalysis_ {Dataset Follow-up Analysis}";
}

void Menu::DefineDescriptionOfCrossDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {descriptionOfCrossDataset::Description of Attribute Dataset}  ";
}


// ---------------------------- CALCULUS --------------------------------- //


// ----------------------------------------------------------------------- //


//---------------------------------------------------------------------------//
//------------------------------ Menu<n> ------------------------------------//
//---------------------------------------------------------------------------//


Menu::Menu(Menu::MenuKind mk) : state( INIT )
{
  if ( mk == CHECKIN )
    cout << "menu_defn_ menu checkin signature {";
  else if( mk == TOOLS )
    cout << "menu_defn_ menu tools signature {";
  else if( mk == MAKE ) 
    cout << "menu_defn_ menu make signature {";
  else if( mk == DATABASE )
    cout << "menu_defn_ menu database signature {";
  else if( mk == PACKAGES )
    cout << "menu_defn_ menu packages signature {";
  else if( mk == TESTING )
    cout << "menu_defn_ menu testing signature {";
  else if( mk == HELP )
    cout << "menu_defn_ menu help signature {";
}


void Menu::startItemGroup( )
{
  if ( state == DONE ) error("Continued writing to closed menu");
  if ( state == ITEM_GROUP ) cout << "}} ";  // Close previous
  cout << "{1 {";
  state = ITEM_GROUP;
}


void Menu::startItemGroup(const Expression& E)
{
  if ( state == DONE ) error("Continued writing to closed menu");
  if ( state == ITEM_GROUP ) cout << "}} ";  // Close previous
  cout << "{{" << E << "} {";
  state = ITEM_GROUP;
}


void Menu::separator( )
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");
  cout << " s ";
}


void Menu::done( )
{
  if ( state == DONE ) error("Continued writing to closed menu");
  cout << "}}}" << endl << flush;
  state = DONE;
}


void Menu::add(Ctor* ctor, const Text& text, const Action& action)
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  cout << "{{" << text << "} {";
  action.print( cout );
  cout << "} {"
  
  // Do return message:

		 << SessionManager::getObjectSmithOid() << ' '
    
		 << ObjectSmith::registerCallBack( ctor )
  
		 << "}} ";
}


void Menu::addDisabled(const Text& text)
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  // @db comment out the next line to hide disabled menu items
  cout << "{{" << text << "} {} {}} ";
}

// add menus without communication between tcl and SessionManager.
//such as help menus.

void Menu::addSimple(const Text& text,const Action& action)
{
  add(ReadSMSimpleMenu().copy(),text,action);
}


void Menu::startCascade(const Text& text)
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  cout << "{c {" << text << "} {";
}


void Menu::closeCascade( )
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  cout << "}} ";
}
