// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of class SMDataSetComputation
//
// Principal Author: xiaowei xu
//
// Status: in progress
//
// Revision History:
//

#ifndef _SMCROSSDATASETCOMPUTATION_H_
#define _SMCROSSDATASETCOMPUTATION_H_

#include "AlgebraicObject.h"
#include "SMBaseOfDataSet.h"
#include "SMDataSetComputation.h"

//---------------------------------------------------------------------------//
//---------------------------- SMCrossDataSetComputation --------------------//
//---------------------------------------------------------------------------//

//-----------------Confidence Interval for One Proportion--------------------//
class SMCrossDataSetConfidenceOne : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMCrossDataSetConfidenceOne(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMCrossDataSetConfidenceOne(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMCrossDataSetConfidenceOne"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::CrossDatasetConfidenceOne; }  // overrides SMObject
  virtual void sendResult( ) const;

public:
  double m_Level;
};

//-----------------Hypothesis Test for One Proportion--------------------//
class SMCrossDataSetHypothesisOne : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMCrossDataSetHypothesisOne(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMCrossDataSetHypothesisOne(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMCrossDataSetHypothesisOne"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::CrossDatasetHypothesisOne; }  // overrides SMObject
  virtual void sendResult( ) const;

public:
  double m_Level,m_MU0;
  char m_Type;
};

//-----------------Confidence Interval for Two Proportion--------------------//
class SMCrossDataSetConfidenceTwo : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMCrossDataSetConfidenceTwo(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMCrossDataSetConfidenceTwo(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMCrossDataSetConfidenceTwo"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::CrossDatasetConfidenceTwo; }  // overrides SMObject
  virtual void sendResult( ) const;

public:
  double m_Level;
};

//-----------------Hypothesis Test for Two Proportion--------------------//
class SMCrossDataSetHypothesisTwo : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMCrossDataSetHypothesisTwo(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMCrossDataSetHypothesisTwo(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMCrossDataSetHypothesisTwo"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::CrossDatasetHypothesisTwo; }  // overrides SMObject
  virtual void sendResult( ) const;

public:
  double m_Level,m_MU0;
  char m_Type;

};


//-----------------Hypothesis Test for Table Analysis--------------------//
class SMCrossDataSetTableAnalysis : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMCrossDataSetTableAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMCrossDataSetTableAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMCrossDataSetTableAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::CrossDatasetTableAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

public:
  double m_Level;
  Chars m_RowFactor,m_ColumnFactor;
};

//-----------------Hypothesis Test for Follow-up Analysis--------------------//
class SMCrossDataSetFollowUpAnalysis : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMCrossDataSetFollowUpAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMCrossDataSetFollowUpAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMCrossDataSetFollowUpAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::CrossDatasetFollowUpAnalysis; }  // overrides SMObject

};

#endif
