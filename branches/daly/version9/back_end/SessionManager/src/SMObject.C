// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMObject, and helper IconID
//
// Principal Author: Roger Needham
//
// Status: useable
//
// Revision History:
//


#include <string.h>
#include "SMObject.h"
#include "TheObjects.h"


//---------------------------------------------------------------------------//
//---------------------------- SMObject -------------------------------------//
//---------------------------------------------------------------------------//


SMObject::SMObject(bool is_cm)
  : theOid( TheObjects::reserveOid() ), isCM( is_cm )
{
  TheObjects::enroll( this );
}



//---------------------------------------------------------------------------//
//------------------------------ IconID -------------------------------------//
//---------------------------------------------------------------------------//

bool IconID::operator == ( const IconID& iid ) const
{
  return strcmp( theName, iid.theName ) == 0;
}


bool IconID::operator != ( const IconID& iid ) const
{
  return strcmp( theName, iid.theName ) != 0;
}


//---------------------------------------------------------------------------//
//------------------------------ IconID -------------------------------------//
//----------------------- static initializations ----------------------------//

const IconID IconID::group = IconID("group");
const IconID IconID::subgroup = IconID("subgroup");
const IconID IconID::SetOfWords = IconID("SetOfWords");
const IconID IconID::VectorOfWords = IconID("VectorOfWords");
const IconID IconID::elt = IconID("elt");
const IconID IconID::equation = IconID("equation");
const IconID IconID::systemOfEquations = IconID("systemOfEquations");
const IconID IconID::map = IconID("map");
const IconID IconID::homomorphism = IconID("homomorphism");
const IconID IconID::permutation = IconID("permutation");
const IconID IconID::enumerator_problem = IconID("enumerator_problem");
const IconID IconID::enumerator_object = IconID("enumerator_object");
const IconID IconID::list_object = IconID("list_object");
const IconID IconID::problem = IconID("problem");
//const IconID IconID::DataSetComputation = IconID("DataSetComputation");
const IconID IconID::DataSetDataSummary = IconID("DataSetDataSummary");
const IconID IconID::DataSetFullAnalysis = IconID("DataSetFullAnalysis");
const IconID IconID::DataSetGraphicAnalysis = IconID("DataSetGraphicAnalysis");
const IconID IconID::DataSetRegressionGraphicAnalysis = IconID("DataSetRegressionGraphicAnalysis");
const IconID IconID::DataSetGoodnessOfFit = IconID("DataSetGoodnessOfFit");
const IconID IconID::DataSetMeanAnalysis = IconID("DataSetMeanAnalysis");
const IconID IconID::DataSetVarianceAnalysis = IconID("DataSetVarianceAnalysis");
const IconID IconID::DataSetCompareMeanAnalysis = IconID("DataSetCompareMeanAnalysis");
const IconID IconID::DataSetCompareVarianceAnalysis = IconID("DataSetCompareVarianceAnalysis");
const IconID IconID::OVDSReportAnalysis = IconID("OVDSReportAnalysis");
const IconID IconID::DataSetNonParametricAnalysis = IconID("DataSetNonParametricAnalysis");
const IconID IconID::DataSetRegressionFullAnalysis = IconID("DataSetRegressionFullAnalysis");
const IconID IconID::DataSetResidualAnalysis = IconID("DataSetResidualAnalysis");
const IconID IconID::DataSetRegressionCorrelationAnalysis = IconID("DataSetRegressionCorrelationAnalysis");
const IconID IconID::DataSetPredictionAnalysis = IconID("DataSetPredictionAnalysis");
const IconID IconID::DataSetAlternateModelAnalysis = IconID("DataSetAlternateModelAnalysis");
const IconID IconID::DataSetTVDSReportIndependentAnalysis = IconID("DataSetTVDSReportIndependentAnalysis");
const IconID IconID::DataSetTVDSRegressionReportAnalysis = IconID("DataSetTVDSRegressionReportAnalysis");
const IconID IconID::CrossDatasetConfidenceOne = IconID("CrossDatasetConfidenceOne");
const IconID IconID::CrossDatasetHypothesisOne = IconID("CrossDatasetHypothesisOne");
const IconID IconID::CrossDatasetConfidenceTwo = IconID("CrossDatasetConfidenceTwo");
const IconID IconID::CrossDatasetHypothesisTwo = IconID("CrossDatasetHypothesisTwo");
const IconID IconID::CrossDatasetTableAnalysis = IconID("CrossDatasetTableAnalysis");
const IconID IconID::CrossDatasetFollowUpAnalysis = IconID("CrossDatasetFollowUpAnalysis");
const IconID IconID::DataSet = IconID("DataSet");
const IconID IconID::Polynomial = IconID("Polynomial");
const IconID IconID::none = IconID("none");
const IconID IconID::do_not_display = IconID("do_not_display");
